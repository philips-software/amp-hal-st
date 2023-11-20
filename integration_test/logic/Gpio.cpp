#include "integration_test/logic/Gpio.hpp"

namespace application
{
    GpioBase::GpioBase(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin)
        : testing::GpioObserverProxy(echo)
        , in(inPin)
        , out(outPin, false)
    {
        in.EnableInterrupt([this]()
            {
                InChanged();
            },
            hal::InterruptTrigger::bothEdges);
    }

    GpioBase::~GpioBase()
    {
        in.DisableInterrupt();
    }

    void GpioBase::SetGpio(bool state, uint32_t pin)
    {
        out.Set(state);
    }

    void GpioBase::InChanged()
    {
        if (!sending)
            RequestSend([this]()
                {
                    sending = false;
                    TestedGpioChanged(in.Get(), 0);
                });

        sending = true;
    }

    GpioTester::GpioTester(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin)
        : GpioBase(echo, inPin, outPin)
        , testing::GpioTester(echo)
    {}

    void GpioTester::SetGpio(bool state, uint32_t pin)
    {
        GpioBase::SetGpio(state, pin);
        MethodDone();
    }

    GpioTested::GpioTested(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin)
        : GpioBase(echo, inPin, outPin)
        , testing::GpioTested(echo)
    {}

    void GpioTested::SetGpio(bool state, uint32_t pin)
    {
        GpioBase::SetGpio(state, pin);
        MethodDone();
    }
}
