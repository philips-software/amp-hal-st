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
        MethodDone();
    }

    void TestedGpioChanged(bool state) override
    {
        testedGpio = state;
        systemChanges.Trigger();
        MethodDone();
    }

    bool testerGpio = false;
    bool testedGpio = false;

    main_::SystemChanges& systemChanges;
};

STEP("gpio peripherals are enabled")
{
    RunInSync([&](const std::function<void()>& done)
        {
            context.Emplace<GpioObserver>(context.Get<services::Echo>(), context.Get<main_::SystemChanges>());

            context.Get<testing::TesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::TesterProxy>().EnablePeripheral(testing::Peripheral::gpio);
                    done();
                });

            context.Get<testing::TestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::TestedProxy>().EnablePeripheral(testing::Peripheral::gpio);
                    done();
                });
        },
        2);

    context.Emplace<testing::GpioTesterProxy>(context.Get<services::Echo>());
    context.Emplace<testing::GpioTestedProxy>(context.Get<services::Echo>());
}

STEP("the tester sets its output pin (high|low)")
{
    context.EmplaceAt<bool>("state", ConvertPinState(parameters[0].get<std::string>()));

    RunInSync([&](const std::function<void()>& done)
        {
            context.Get<testing::GpioTesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::GpioTesterProxy>().SetGpio(context.Get<bool>("state"));
                    done();
                });
        });
}

STEP("the tester sees a (high|low) value")
{
    context.EmplaceAt<bool>("state", ConvertPinState(parameters[0].get<std::string>()));

    RunInSyncOnSystemChange([&](const std::function<void()>& done)
        {
            if (context.Get<GpioObserver>().testerGpio == context.Get<bool>("state"))
                done();
        },
        context);
}

STEP("the tested sets its output pin (high|low)")
{
    context.EmplaceAt<bool>("state", ConvertPinState(parameters[0].get<std::string>()));

    RunInSync([&](const std::function<void()>& done)
        {
            context.Get<testing::GpioTestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::GpioTestedProxy>().SetGpio(context.Get<bool>("state"));
                    done();
                });
        });
}

STEP("the tested sees a (high|low) value")
{
    context.EmplaceAt<bool>("state", ConvertPinState(parameters[0].get<std::string>()));

    RunInSyncOnSystemChange([&](const std::function<void()>& done)
        {
            if (context.Get<GpioObserver>().testedGpio == context.Get<bool>("state"))
                done();
        },
        context);
}
