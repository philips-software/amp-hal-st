#include "cucumber-cpp/Steps.hpp"
#include "generated/echo/Testing.pb.hpp"
#include "integration_test/test/FixtureSystemChanges.hpp"
#include "integration_test/test/RunInSync.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

bool ConvertPinState(const std::string& state)
{
    if (state == "high")
        return true;
    else if (state == "low")
        return false;
    else
        std::abort();
}

class GpioObserver
    : public testing::GpioObserver
{
public:
    GpioObserver(services::Echo& echo, main_::SystemChanges& systemChanges)
        : testing::GpioObserver(echo)
        , systemChanges(systemChanges)
    {}

    void TesterGpioChanged(bool state) override
    {
        testerGpio = state;
        systemChanges.Trigger();
    }

    void TestedGpioChanged(bool state) override
    {
        testedGpio = state;
        systemChanges.Trigger();
    }

    bool testerGpio = false;
    bool testedGpio = false;

    main_::SystemChanges& systemChanges;
};

STEP("gpio peripherals are enabled")
{
    RunInSync([&](const std::function<void()>& done)
        {
            context.emplace<GpioObserver>(*context.get<services::Echo>(), *context.get<main_::SystemChanges>());

            context.get<testing::TesterProxy>()->RequestSend([&]()
                {
                    context.get<testing::TesterProxy>()->EnablePeripheral(testing::Peripheral::gpio);
                    done();
                });

            context.get<testing::TestedProxy>()->RequestSend([&]()
                {
                    context.get<testing::TestedProxy>()->EnablePeripheral(testing::Peripheral::gpio);
                    done();
                });
        },
        2);

    context.emplace<testing::GpioTesterProxy>(*context.get<services::Echo>());
    context.emplace<testing::GpioTestedProxy>(*context.get<services::Echo>());
}

STEP("the tester sets its output pin (high|low)")
{
    context.emplace<bool>("state", ConvertPinState(parameters[0].get<std::string>()));

    RunInSync([&](const std::function<void()>& done)
        {
            context.get<testing::GpioTesterProxy>()->RequestSend([&]()
                {
                    context.get<testing::GpioTesterProxy>()->SetGpio(*context.get<bool>("state"));
                    done();
                });
        });
}

STEP("the tester sees a (high|low) value")
{
    context.emplace<bool>("state", ConvertPinState(parameters[0].get<std::string>()));

    RunInSyncOnSystemChange([&](const std::function<void()>& done)
        {
            if (context.get<GpioObserver>()->testerGpio == *context.get<bool>("state"))
                done();
        },
        context);
}

STEP("the tested sets its output pin (high|low)")
{
    context.emplace<bool>("state", ConvertPinState(parameters[0].get<std::string>()));

    RunInSync([&](const std::function<void()>& done)
        {
            context.get<testing::GpioTestedProxy>()->RequestSend([&]()
                {
                    context.get<testing::GpioTestedProxy>()->SetGpio(*context.get<bool>("state"));
                    done();
                });
        });
}

STEP("the tested sees a (high|low) value")
{
    context.emplace<bool>("state", ConvertPinState(parameters[0].get<std::string>()));

    RunInSyncOnSystemChange([&](const std::function<void()>& done)
        {
            if (context.get<GpioObserver>()->testedGpio == *context.get<bool>("state"))
                done();
        },
        context);
}
