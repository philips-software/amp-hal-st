#include "cucumber-cpp/Hooks.hpp"
#include "generated/echo/Testing.pb.hpp"
#include "integration_test/test/FixtureEcho.hpp"
#include "integration_test/test/FixtureEventDispatcher.hpp"
#include "integration_test/test/FixtureSystemChanges.hpp"

HOOK_BEFORE_ALL()
{
    context.emplace<main_::FixtureEventDispatcher>();
    context.emplace<main_::SystemChanges>();

    auto echoFixture = std::make_shared<main_::FixtureEcho>();
    auto echo = std::shared_ptr<services::Echo>(echoFixture, &echoFixture->echo.echo);
    context.set(echo);

    context.emplace<testing::TesterProxy>(*echo);
    context.emplace<testing::TestedProxy>(*echo);
}
