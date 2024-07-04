#include "cucumber-cpp/Hooks.hpp"
#include "generated/echo/Testing.pb.hpp"
#include "hal/generic/SynchronousRandomDataGeneratorGeneric.hpp"
#include "hal/generic/TimerServiceGeneric.hpp"
#include "infra/event/EventDispatcherThreadAware.hpp"
#include "integration_test/logic/Tested.hpp"
#include "integration_test/runner/FixtureEcho.hpp"
#include "integration_test/runner/Waiting.hpp"
#include "services/network_instantiations/NetworkAdapter.hpp"
#include "gtest/gtest.h"

HOOK_BEFORE_ALL()
{
    auto target = context.Get<std::string>("target");
    auto networkAdapter = std::make_shared<main_::NetworkAdapter>();
    context.SetShared(std::shared_ptr<infra::EventDispatcherWithWeakPtrWorker>(networkAdapter, &networkAdapter->EventDispatcher()));
    context.SetShared(std::shared_ptr<services::ConnectionFactoryWithNameResolver>(networkAdapter, &networkAdapter->ConnectionFactoryWithNameResolver()));
    context.Emplace<hal::TimerServiceGeneric>();

    if (target.substr(0, 3) == "COM" || target.substr(0, 4) == "/dev")
    {
        auto echoFixture = std::make_shared<main_::FixtureEchoSerial>(target);
        context.SetShared(std::shared_ptr<services::Echo>(echoFixture, &echoFixture->echo));
    }
    else if (services::SchemeFromUrl(target) == "ws")
    {
        if (!infra::WaitUntilDone(
                context, [&](const std::function<void()>& done)
                {
                    static hal::SynchronousRandomDataGeneratorGeneric randomDataGenerator;

                    auto echoFixture = std::make_shared<main_::EchoClientWebSocket>(
                        context.Get<services::ConnectionFactoryWithNameResolver>(),
                        target, randomDataGenerator);
                    echoFixture->OnDone([&, echoFixture](services::Echo& echo)
                        {
                            context.SetShared(std::shared_ptr<services::Echo>(echoFixture, &echo));
                            done();
                        });
                },
                std::chrono::seconds(10)))
            throw std::runtime_error("Couldn't open websocket connection");
    }
    else if (services::SchemeFromUrl(target) == "tcp")
    {
        if (!infra::WaitUntilDone(
                context, [&](const std::function<void()>& done)
                {
                    static hal::SynchronousRandomDataGeneratorGeneric randomDataGenerator;

                    auto echoFixture = std::make_shared<main_::EchoClientTcp>(
                        context.Get<services::ConnectionFactoryWithNameResolver>(),
                        services::HostFromUrl(target), services::PortFromUrl(target).ValueOr(1234));
                    echoFixture->OnDone([&, echoFixture](services::Echo& echo)
                        {
                            context.SetShared(std::shared_ptr<services::Echo>(echoFixture, &echo));
                            done();
                        });
                },
                std::chrono::seconds(10)))
            throw std::runtime_error("Couldn't open websocket connection");
    }
    else
        throw std::runtime_error("Don't know how to open " + target);

    context.Emplace<testing::TesterProxy>(context.Get<services::Echo>());
    context.Emplace<testing::TestedProxy>(context.Get<services::Echo>());
    context.Emplace<application::TestedObserver>(context.Get<services::Echo>());
}

HOOK_BEFORE_SCENARIO()
{
    infra::WaitUntilDone(context, [&](const std::function<void()>& done)
        {
            context.Get<testing::TesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::TesterProxy>().Reset();
                    done();
                });
        });

    infra::WaitUntilDone(context, [&](const std::function<void()>& done)
        {
            context.Get<testing::TestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::TestedProxy>().Ping();
                    done();
                });
        });

    infra::WaitFor(context, [&]()
        {
            return context.Get<application::TestedObserver>().ReceivedPong();
        });
}
