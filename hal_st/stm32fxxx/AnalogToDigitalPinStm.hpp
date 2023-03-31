#ifndef HAL_ANALOG_TO_DIGITAL_PIN_STM_HPP
#define HAL_ANALOG_TO_DIGITAL_PIN_STM_HPP

#include DEVICE_HEADER
#include "hal/interfaces/AnalogToDigitalPin.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace hal
{
    class AdcStm;

    class AnalogToDigitalPinImplStm
        : AnalogToDigitalPinImplBase
    {
    public:
        AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc);

        virtual void Measure(const infra::Function<void(int32_t value)>& onDone) override;

    private:
        AnalogPinStm pin;
        AdcStm& adc;
    };

    class AdcStm
    {
    public:
        AdcStm(uint8_t oneBasedIndex);
        ~AdcStm();

        void Measure(AnalogPinStm& pin, const infra::Function<void(int32_t value)>& onDone);

    private:
        void MeasurementDone();

    private:
        friend class AnalogToDigitalPinImplStm;

        DispatchedInterruptHandler interruptHandler;
        uint8_t index;
        ADC_HandleTypeDef handle;
        infra::AutoResetFunction<void(int32_t value)> onDone;
    };
}

#endif
