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
        : public AnalogToDigitalPinImplBase<uint16_t>
    {
    public:
        explicit AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc);
        virtual ~AnalogToDigitalPinImplStm() = default;

        void Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone) override;

    private:
        AnalogPinStm analogPin;
        AdcStm& adc;
    };

    class AnalogToDigitalInternalTemperatureImplStm
        : public AnalogToDigitalPinImplBase<uint16_t>
    {
    public:
        explicit AnalogToDigitalInternalTemperatureImplStm(AdcStm& adc);
        virtual ~AnalogToDigitalInternalTemperatureImplStm() = default;

        void Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone) override;

    private:
        AdcStm& adc;
    };

    class ConvertToCelsiusDegreesHelperStm
    {
    public:
        ConvertToCelsiusDegreesHelperStm(uint16_t sample, uint16_t voltageReferenceMiliVolts);
        uint16_t Value() const;

    private:
        uint16_t convertedValue;
    };

    class AdcTriggeredByTimerWithDma;

    class AdcStm
    {
    public:
        explicit AdcStm(uint8_t adcIndex);
        ~AdcStm();

    protected:
        uint32_t Channel(const hal::AnalogPinStm& pin) const;
        ADC_HandleTypeDef& Handle();

    private:
        void Measure(const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone);
        void MeasurementDone();

    private:
        friend class AnalogToDigitalPinImplStm;
        friend class AnalogToDigitalInternalTemperatureImplStm;
        friend class AdcTriggeredByTimerWithDma;

        uint8_t index;
        ADC_HandleTypeDef handle{};
        DispatchedInterruptHandler interruptHandler;
        uint16_t sample;
        infra::AutoResetFunction<void(infra::MemoryRange<uint16_t>)> onDone;
    };
}

#endif
