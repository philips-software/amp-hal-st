#include "cucumber-cpp/Steps.hpp"
#include "generated/echo/Testing.pb.hpp"
#include "infra/timer/Waiting.hpp"
#include "integration_test/logic/Tested.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace
{
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
        GpioObserver(services::Echo& echo)
            : testing::GpioObserver(echo)
        {}

        void TesterGpioChanged(bool state) override
        {
            testerGpio = state;
            MethodDone();
        }

        void TestedGpioChanged(bool state) override
        {
            testedGpio = state;
            MethodDone();
        }

        bool testerGpio = false;
        bool testedGpio = false;
    };
}

STEP("gpio peripherals are enabled")
{
    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Emplace<GpioObserver>(context.Get<services::Echo>());

            context.Get<testing::TesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::TesterProxy>().EnablePeripheral(testing::Peripheral::gpio);
                    done();
                });
        }));

    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Get<testing::TestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::TestedProxy>().EnablePeripheral(testing::Peripheral::gpio);
                    context.Get<testing::TestedProxy>().RequestSend([&]()
                        {
                            context.Get<testing::TestedProxy>().Ping();
                            done();
                        });
                });
        }));

    EXPECT_TRUE(infra::WaitFor([&]()
        {
            return context.Get<application::TestedObserver>().ReceivedPong();
        }));

    context.Emplace<testing::GpioTesterProxy>(context.Get<services::Echo>());
    context.Emplace<testing::GpioTestedProxy>(context.Get<services::Echo>());
}

STEP("the tester sets its output pin (high|low)", (std::string state))
{
    context.EmplaceAt<bool>("state", ConvertPinState(state));

    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Get<testing::GpioTesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::GpioTesterProxy>().SetGpio(context.Get<bool>("state"));
                    done();
                });
        }));
}

STEP("the tester sees a (high|low) value", (std::string state))
{
    context.EmplaceAt<bool>("state", ConvertPinState(state));

    EXPECT_TRUE(infra::WaitFor([&]()
        {
            return context.Get<GpioObserver>().testerGpio == context.Get<bool>("state");
        }));
}

STEP("the tested sets its output pin (high|low)", (std::string state))
{
    context.EmplaceAt<bool>("state", ConvertPinState(state));

    EXPECT_TRUE(infra::WaitUntilDone([&](const infra::Function<void()>& done)
        {
            context.Get<testing::GpioTestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::GpioTestedProxy>().SetGpio(context.Get<bool>("state"));
                    done();
                });
        }));
}

STEP("the tested sees a (high|low) value", (std::string state))
{
    context.EmplaceAt<bool>("state", ConvertPinState(state));

    EXPECT_TRUE(infra::WaitFor([&]()
        {
            return context.Get<GpioObserver>().testedGpio == context.Get<bool>("state");
        }));
}
