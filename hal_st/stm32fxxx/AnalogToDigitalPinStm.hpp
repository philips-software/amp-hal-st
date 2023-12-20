#ifndef HAL_ANALOG_TO_DIGITAL_PIN_STM_HPP
#define HAL_ANALOG_TO_DIGITAL_PIN_STM_HPP

#include DEVICE_HEADER
#include "hal/interfaces/AnalogToDigitalPin.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace hal
{
    class AdcStm
    {
    public:
        enum class TriggerSource : uint32_t
        {
            software = ADC_SOFTWARE_START,
            timer1 = ADC_EXTERNALTRIG_T1_TRGO,
            timer2 = ADC_EXTERNALTRIG_T2_TRGO,
            external = ADC_EXTERNALTRIG_EXT_IT11,
        };

        enum class TriggerEdge : uint32_t
        {
            none = ADC_EXTERNALTRIGCONVEDGE_NONE,
            rising = ADC_EXTERNALTRIGCONVEDGE_RISING,
            falling = ADC_EXTERNALTRIGCONVEDGE_FALLING,
            both = ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING,
        };

        enum class OverrunBehavior : uint32_t
        {
            preserve = ADC_OVR_DATA_PRESERVED,
            overwrite = ADC_OVR_DATA_OVERWRITTEN,
        };

        struct Config
        {
            TriggerSource triggerSource;
            TriggerEdge triggerEdge;
            OverrunBehavior overrun;
            bool isDmaUsed;
        };

        explicit AdcStm(uint8_t oneBasedIndex, const Config& config);
        ~AdcStm();

        void Measure(const infra::Function<void(int32_t value)>& onDone);

    protected:
        uint32_t Channel(const hal::AnalogPinStm& pin) const;
        ADC_HandleTypeDef& Handle();

    private:
        void MeasurementDone();

        uint8_t index;
        ADC_HandleTypeDef handle{};
        DispatchedInterruptHandler interruptHandler;
        infra::AutoResetFunction<void(int32_t value)> onDone;
    };

    class AnalogToDigitalPinImplStm
        : public AnalogToDigitalPinImplBase
        , private AdcStm
    {
    public:
        explicit AnalogToDigitalPinImplStm(uint8_t adcIndex, hal::GpioPinStm& pin);
        virtual ~AnalogToDigitalPinImplStm() = default;

        void Measure(const infra::Function<void(int32_t value)>& onDone) override;

    private:
        AnalogPinStm pin;
        AdcStm::Config config{ AdcStm::TriggerSource::software, AdcStm::TriggerEdge::none, AdcStm::OverrunBehavior::preserve, false };
    };
}

#endif
