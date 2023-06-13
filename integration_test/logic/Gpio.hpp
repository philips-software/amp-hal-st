#ifndef HAL_ST_INTEGRATION_TEST_GPIO_HPP
#define HAL_ST_INTEGRATION_TEST_GPIO_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal/interfaces/Gpio.hpp"

namespace application
{
    class GpioTested
        : public testing::GpioTested
        , public testing::GpioObserverProxy
    {
    public:
        GpioTested(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin);

        // Implementation of GpioTested
        void SetGpio(bool state, uint32_t pin) override;

    private:
        void InChanged();

    private:
        hal::InputPin in;
        hal::OutputPin out;
        bool sending = false;
    };
}

#endif
