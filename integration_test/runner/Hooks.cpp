#include "cucumber_cpp/library/Hooks.hpp"
#include "generated/echo/Testing.pb.hpp"
#include "generated/echo/TracingTesting.pb.hpp"
#include "hal/generic/TimerServiceGeneric.hpp"
#include "infra/timer/Waiting.hpp"
#include "integration_test/logic/Tested.hpp"
#include "integration_test/logic/Tester.hpp"
#include "integration_test/runner/FixtureEcho.hpp"
#include "services/network_instantiations/EchoInstantiation.hpp"
#include "services/network_instantiations/NetworkAdapter.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "gtest/gtest.h"

HOOK_BEFORE_ALL()
{
    auto target = context.Get<std::string>("target");
    auto networkAdapter = std::make_shared<main_::NetworkAdapter>();
    context.SetShared(std::shared_ptr<infra::EventDispatcherWithWeakPtrWorker>(networkAdapter, &networkAdapter->EventDispatcher()));
    context.SetShared(std::shared_ptr<services::ConnectionFactoryWithNameResolver>(networkAdapter, &networkAdapter->ConnectionFactoryWithNameResolver()));
    context.Emplace<hal::TimerServiceGeneric>();

    try
    {
        services::GlobalTracer().Trace() << "Opening ECHO connection to: " << target;

        context.SetShared(application::OpenEcho(target, context.Get<services::ConnectionFactoryWithNameResolver>()));
        //auto tracingEcho = application::OpenTracingEcho(target, context.Get<services::ConnectionFactoryWithNameResolver>(), services::GlobalTracer());
        //context.SetShared(tracingEcho.first);
        //context.SetShared(tracingEcho.second);

        services::GlobalTracer().Trace() << "Established ECHO connection";
    }
    catch (std::exception& e)
    {
        services::GlobalTracer().Trace() << "Exception: " << e.what();
        throw;
    }

    context.Emplace<testing::TesterProxy>(context.Get<services::Echo>());
    context.Emplace<testing::TestedProxy>(context.Get<services::Echo>());
    context.Emplace<application::TesterObserver>(context.Get<services::Echo>());
    context.Emplace<application::TestedObserver>(context.Get<services::Echo>());

    //context.Emplace<testing::TesterTracer>(context.Get<services::TracingEchoOnStreams>());
    //context.Emplace<testing::TestedTracer>(context.Get<services::TracingEchoOnStreams>());
    //context.Emplace<testing::TesterObserverTracer>(context.Get<services::TracingEchoOnStreams>());
    //context.Emplace<testing::TestedObserverTracer>(context.Get<services::TracingEchoOnStreams>());
    //context.Emplace<testing::GpioTesterTracer>(context.Get<services::TracingEchoOnStreams>());
    //context.Emplace<testing::GpioTestedTracer>(context.Get<services::TracingEchoOnStreams>());
    //context.Emplace<testing::GpioObserverTracer>(context.Get<services::TracingEchoOnStreams>());
    //context.Emplace<testing::UartTesterTracer>(context.Get<services::TracingEchoOnStreams>());
    //context.Emplace<testing::UartTestedTracer>(context.Get<services::TracingEchoOnStreams>());
    //context.Emplace<testing::UartObserverTracer>(context.Get<services::TracingEchoOnStreams>());
}

HOOK_BEFORE_SCENARIO()
{
    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Get<testing::TesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::TesterProxy>().SetTestedMode(testing::TestedMode::reset);
                    context.Get<testing::TesterProxy>().RequestSend([&]()
                        {
                            context.Get<testing::TesterProxy>().SetTestedMode(testing::TestedMode::active);
                            done();
                        });
                });
        }));

    EXPECT_TRUE(infra::WaitFor([&]()
        {
            return context.Get<application::TesterObserver>().ReceivedTestedModeSet();
        }));

    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Get<testing::TestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::TestedProxy>().Ping();
                    done();
                });
        }));

    EXPECT_TRUE(infra::WaitFor([&]()
        {
            return context.Get<application::TestedObserver>().ReceivedPong();
        }));
}
