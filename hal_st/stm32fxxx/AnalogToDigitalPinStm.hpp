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
        explicit AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc);
        virtual ~AnalogToDigitalPinImplStm() = default;

        void Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone) override;

    private:
        AnalogPinStm analogPin;
        AdcStm& adc;
    };

    class AdcStm
    {
    public:
        explicit AdcStm(uint8_t adcIndex);
        ~AdcStm();

    protected:
        enum class TriggerSource : uint32_t
        {
            software,
            timer,
            external,
        };

        enum class TriggerEdge : uint32_t
        {
            none = ADC_EXTERNALTRIGCONVEDGE_NONE,
            rising = ADC_EXTERNALTRIGCONVEDGE_RISING,
            falling = ADC_EXTERNALTRIGCONVEDGE_FALLING,
            both = ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING,
        };

        struct Config
        {
            TriggerSource triggerSource;
            TriggerEdge triggerEdge;
        };

        AdcStm(uint8_t adcIndex, const Config& config);
        uint32_t Channel(const hal::AnalogPinStm& pin) const;
        ADC_HandleTypeDef& Handle();

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
