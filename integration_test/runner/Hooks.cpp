#include "cucumber-cpp/Hooks.hpp"
#include "args.hxx"
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
    args::ArgumentParser parser("Integration test runner for amp-hal-st");
    args::Group arguments(parser, "Arguments:");
    args::Positional<std::string> target(arguments, "target", "COM port or hostname (ws://<host>/path or tcp://<host>) of the amp-hal-st integration test board", args::Options::Required);
    args::Group flags(parser, "Optional flags:");
    args::HelpFlag help(flags, "help", "Display this help menu.", { 'h', "help" });

    try
    {
        const auto& args = context.Get<std::vector<std::string_view>>("args");
        std::vector<std::string> stringArgs(args.begin(), args.end());
        parser.ParseArgs(stringArgs);

        auto networkAdapter = std::make_shared<main_::NetworkAdapter>();
        context.SetShared(std::shared_ptr<infra::EventDispatcherWithWeakPtrWorker>(networkAdapter, &networkAdapter->EventDispatcher()));
        context.SetShared(std::shared_ptr<services::ConnectionFactoryWithNameResolver>(networkAdapter, &networkAdapter->ConnectionFactoryWithNameResolver()));
        context.Emplace<hal::TimerServiceGeneric>();

        if (args::get(target).substr(0, 3) == "COM" || args::get(target).substr(0, 4) == "/dev")
        {
            auto echoFixture = std::make_shared<main_::FixtureEchoSerial>(args::get(target));
            context.SetShared(std::shared_ptr<services::Echo>(echoFixture, &echoFixture->echo));
        }
        else if (services::SchemeFromUrl(infra::BoundedConstString(args::get(target))) == "ws")
        {
            if (!infra::WaitUntilDone(
                    context, [&](const std::function<void()>& done)
                    {
                        static hal::SynchronousRandomDataGeneratorGeneric randomDataGenerator;

                        auto echoFixture = std::make_shared<main_::EchoClientWebSocket>(
                            context.Get<services::ConnectionFactoryWithNameResolver>(),
                            args::get(target), randomDataGenerator);
                        echoFixture->OnDone([&, echoFixture](services::Echo& echo)
                            {
                                context.SetShared(std::shared_ptr<services::Echo>(echoFixture, &echo));
                                done();
                            });
                    },
                    std::chrono::seconds(10)))
                throw std::runtime_error("Couldn't open websocket connection");
        }
        else if (services::SchemeFromUrl(infra::BoundedConstString(args::get(target))) == "tcp")
        {
            if (!infra::WaitUntilDone(
                    context, [&](const std::function<void()>& done)
                    {
                        static hal::SynchronousRandomDataGeneratorGeneric randomDataGenerator;

                        auto echoFixture = std::make_shared<main_::EchoClientTcp>(
                            context.Get<services::ConnectionFactoryWithNameResolver>(),
                            services::HostFromUrl(infra::BoundedConstString(args::get(target))), services::PortFromUrl(args::get(target)).ValueOr(1234));
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
            throw std::runtime_error("Don't know how to open " + args::get(target));

        context.Emplace<testing::TesterProxy>(context.Get<services::Echo>());
        context.Emplace<testing::TestedProxy>(context.Get<services::Echo>());
        context.Emplace<application::TestedObserver>(context.Get<services::Echo>());
    }
    catch (const args::Error& error)
    {
        std::ostringstream errorPlusHelp;
        errorPlusHelp << error.what() << parser;
        throw std::runtime_error(errorPlusHelp.str());
    }
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
