#include "integration_test/logic/Gpio.hpp"

namespace application
{
    GpioTested::GpioTested(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin)
        : testing::GpioTested(echo)
        , testing::GpioObserverProxy(echo)
        , in(inPin)
        , out(outPin, false)
    {
        in.EnableInterrupt([this]()
            {
                InChanged();
            },
            hal::InterruptTrigger::bothEdges);
    }

    void GpioTested::SetGpio(bool state, uint32_t pin)
    {
        out.Set(state);
    }

    void GpioTested::InChanged()
    {
        if (!sending)
            RequestSend([this]()
                {
                    sending = false;
                    TestedGpioChanged(in.Get(), 0);
                });

        sending = true;
    }
}
