#include "cucumber-cpp/Steps.hpp"
#include "generated/echo/Testing.pb.hpp"
#include "integration_test/test/Waiting.hpp"
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

class TestedObserver
    : public testing::TestedObserver
{
public:
    using testing::TestedObserver::TestedObserver;

    void Pong() override
    {
        pongReceived = true;
        MethodDone();
    }

    bool ReceivedPong()
    {
        return std::exchange(pongReceived, false);
    }

private:
    bool pongReceived = false;
};

STEP("gpio peripherals are enabled")
{
    infra::WaitUntilDone(context, [&](const std::function<void()>& done)
        {
            context.Emplace<GpioObserver>(context.Get<services::Echo>());
            context.Emplace<TestedObserver>(context.Get<services::Echo>());

            context.Get<testing::TesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::TesterProxy>().EnablePeripheral(testing::Peripheral::gpio);
                    done();
                });
        });

    infra::WaitUntilDone(context, [&](const std::function<void()>& done)
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
        });

    infra::WaitFor(context, [&]()
        {
            return context.Get<TestedObserver>().ReceivedPong();
        });

    context.Emplace<testing::GpioTesterProxy>(context.Get<services::Echo>());
    context.Emplace<testing::GpioTestedProxy>(context.Get<services::Echo>());
}

STEP("the tester sets its output pin (high|low)", (std::string state))
{
    context.EmplaceAt<bool>("state", ConvertPinState(state));

    infra::WaitUntilDone(context, [&](const std::function<void()>& done)
        {
            context.Get<testing::GpioTesterProxy>().RequestSend([&]()
                {
                    context.Get<testing::GpioTesterProxy>().SetGpio(context.Get<bool>("state"));
                    done();
                });
        });
}

STEP("the tester sees a (high|low) value", (std::string state))
{
    context.EmplaceAt<bool>("state", ConvertPinState(state));

    infra::WaitFor(context, [&]()
        {
            return context.Get<GpioObserver>().testerGpio == context.Get<bool>("state");
        });
}

STEP("the tested sets its output pin (high|low)", (std::string state))
{
    context.EmplaceAt<bool>("state", ConvertPinState(state));

    infra::WaitUntilDone(context, [&](const std::function<void()>& done)
        {
            context.Get<testing::GpioTestedProxy>().RequestSend([&]()
                {
                    context.Get<testing::GpioTestedProxy>().SetGpio(context.Get<bool>("state"));
                    done();
                });
        });
}

STEP("the tested sees a (high|low) value", (std::string state))
{
    context.EmplaceAt<bool>("state", ConvertPinState(state));

    infra::WaitFor(context, [&]()
        {
            return context.Get<GpioObserver>().testedGpio == context.Get<bool>("state");
        });
}
