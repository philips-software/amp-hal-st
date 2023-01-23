#ifndef HAL_DIGITAL_TO_ANALOG_PIN_STM_HPP
#define HAL_DIGITAL_TO_ANALOG_PIN_STM_HPP
#include "hal/interfaces/DigitalToAnalogPin.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"

namespace hal
{
    class DacStm;

    class DigitalToAnalogPinImplStm
        : public DigitalToAnalogPinImplBase
    {
    public:
        DigitalToAnalogPinImplStm(hal::GpioPinStm& pin, hal::DacStm& dac);

        void Set(uint16_t value) override;

    private:
        AnalogPinStm pin;
        DacStm& dac;
        uint32_t channel;
    };

    class DacStm
    {
    public:
        DacStm(uint8_t oneBasedIndex);
        ~DacStm();

        void Set(uint32_t channel, uint32_t alignment, uint32_t data);

    private:
        friend class DigitalToAnalogPinImplStm;

        uint8_t index;
        DAC_HandleTypeDef handle;
    };
}

#endif
