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

    void GpioBase::SetGpio(bool state)
    {
        out.Set(state);
    }

    void GpioBase::InChanged()
    {
        if (!sending)
            RequestSend([this]()
                {
                    sending = false;
                    TestedGpioChanged(in.Get());
                });

        sending = true;
    }

    GpioTester::GpioTester(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin)
        : GpioBase(echo, inPin, outPin)
        , testing::GpioTester(echo)
    {}

    void GpioTester::SetGpio(bool state)
    {
        GpioBase::SetGpio(state);
        MethodDone();
    }

    GpioTested::GpioTested(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin)
        : GpioBase(echo, inPin, outPin)
        , testing::GpioTested(echo)
    {}

    void GpioTested::SetGpio(bool state)
    {
        GpioBase::SetGpio(state);
        MethodDone();
    }
}
