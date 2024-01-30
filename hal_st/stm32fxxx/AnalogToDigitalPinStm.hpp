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
        : protected AnalogToDigitalPinImplBase<uint16_t>
    {
    public:
        AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc);

        void Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone) override;

    private:
        AnalogPinStm analogPin;
        AdcStm& adc;
    };

    class AdcStm
    {
    public:
        AdcStm(uint8_t oneBasedIndex);
        ~AdcStm();

    private:
        void Measure(const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone);
        void MeasurementDone();

    private:
        friend class AnalogToDigitalPinImplStm;

        uint8_t index;
        ADC_HandleTypeDef handle{};
        DispatchedInterruptHandler interruptHandler;
        uint16_t sample;
        infra::AutoResetFunction<void(infra::MemoryRange<uint16_t>)> onDone;
    };
}

#endif
