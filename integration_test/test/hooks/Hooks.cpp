#include "cucumber-cpp/Hooks.hpp"
#include "cucumber-cpp-example/fixtures/Fixture.hpp"
#include "cucumber-cpp/Context.hpp"

HOOK_BEFORE_ALL()
{
    // spawn eventdispatcher
    context.set(std::make_shared<NordicBleFixture>());
}

HOOK_BEFORE()
{
}

HOOK_BEFORE_COND("@dingus")
{
    std::cout << "running only for dingus tests\n";
}

HOOK_BEFORE_STEP()
{
}

HOOK_AFTER_STEP()
{
}

HOOK_AFTER()
{
}

HOOK_AFTER_ALL()
{
}
