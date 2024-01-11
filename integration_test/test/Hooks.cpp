#include "cucumber-cpp/Hooks.hpp"
#include "generated/echo/Testing.pb.hpp"
#include "hal/generic/TimerServiceGeneric.hpp"
#include "integration_test/test/FixtureEcho.hpp"
#include "integration_test/test/FixtureEventDispatcher.hpp"
#include "integration_test/test/FixtureSystemChanges.hpp"
#include "integration_test/test/RunInSync.hpp"

HOOK_BEFORE_ALL()
{
    context.Emplace<main_::FixtureEventDispatcher>();
    context.Emplace<hal::TimerServiceGeneric>();
    context.Emplace<main_::SystemChanges>();

    auto echoFixture = std::make_shared<main_::FixtureEcho>();
    auto echo = std::shared_ptr<services::Echo>(echoFixture, &echoFixture->echo.echo);
    context.SetShared(echo);

    context.Emplace<testing::TesterProxy>(*echo);
    context.Emplace<testing::TestedProxy>(*echo);
}

HOOK_BEFORE()
{
    RunInSync([&](const std::function<void()>& done)
        {
            context.Get<testing::TesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::TesterProxy>().Reset();
                    done();
                });
        });
}
