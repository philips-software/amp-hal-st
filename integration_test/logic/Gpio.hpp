#ifndef HAL_ST_INTEGRATION_TEST_GPIO_HPP
#define HAL_ST_INTEGRATION_TEST_GPIO_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal/interfaces/Gpio.hpp"

namespace application
{
    class GpioBase
        : public testing::GpioObserverProxy
    {
    public:
        GpioBase(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin);
        ~GpioBase();

        void SetGpio(bool state);

    protected:
        virtual void GpioChanged(bool state) = 0;

    private:
        void InChanged();

    private:
        hal::InputPin in;
        hal::OutputPin out;
        bool sending = false;
    };

    class GpioTester
        : public GpioBase
        , public testing::GpioTester
    {
    public:
        GpioTester(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin);

        // Implementation of GpioTester
        void SetGpio(bool state) override;

    protected:
        virtual void GpioChanged(bool state) override;
    };

    class GpioTested
        : public GpioBase
        , public testing::GpioTested
    {
    public:
        GpioTested(services::Echo& echo, hal::GpioPin& inPin, hal::GpioPin& outPin);

        // Implementation of GpioTested
        void SetGpio(bool state) override;

    protected:
        virtual void GpioChanged(bool state) override;
    };
}

#endif
