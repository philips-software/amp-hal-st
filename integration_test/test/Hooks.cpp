#include "cucumber-cpp/Hooks.hpp"
#include "generated/echo/Testing.pb.hpp"
#include "hal/generic/TimerServiceGeneric.hpp"
#include "integration_test/test/FixtureEcho.hpp"
#include "integration_test/test/FixtureEventDispatcher.hpp"
#include "integration_test/test/Waiting.hpp"

HOOK_BEFORE_ALL()
{
    context.Emplace<infra::EventDispatcherThreadAware>();
    context.Emplace<hal::TimerServiceGeneric>();

    auto echoFixture = std::make_shared<main_::FixtureEcho>();
    auto echo = std::shared_ptr<services::Echo>(echoFixture, &echoFixture->echo.echo);
    context.SetShared(echo);

    context.Emplace<testing::TesterProxy>(*echo);
    context.Emplace<testing::TestedProxy>(*echo);
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
}
