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

        void Measure(const infra::Function<void()>& onDone) override;

    private:
        std::array<uint16_t, 1> buffer;
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
        void Measure(infra::MemoryRange<uint16_t> buffer, const infra::Function<void()>& onDone);
        void MeasurementDone();

    private:
        friend class AnalogToDigitalPinImplStm;

        uint8_t index;
        ADC_HandleTypeDef handle{};
        DispatchedInterruptHandler interruptHandler;
        infra::MemoryRange<uint16_t> buffer;
        infra::AutoResetFunction<void()> onDone;
    };
}

#endif
