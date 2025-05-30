#ifndef HAL_ANALOG_TO_DIGITAL_PIN_STM_HPP
#define HAL_ANALOG_TO_DIGITAL_PIN_STM_HPP

#include "hal/interfaces/AnalogToDigitalPin.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include <cstddef>
#include <cstdint>

#include DEVICE_HEADER

namespace hal
{
    namespace detail
    {
        struct AdcStmChannelConfig
        {
#if defined(ADC_SAMPLETIME_2CYCLES_5)
            uint32_t samplingTime{ ADC_SAMPLETIME_2CYCLES_5 };
#elif defined(ADC_SAMPLETIME_3CYCLES_5)
            uint32_t samplingTime{ ADC_SAMPLETIME_3CYCLES_5 };
#else
            uint32_t samplingTime{ ADC_SAMPLETIME_3CYCLES };
#endif
            bool differential{ false };
        };

        struct AdcStmConfig
        {
#ifdef STM32WBA
            uint32_t clockPrescaler{ ADC_CLOCK_ASYNC_DIV4 };
#else
            uint32_t clockPrescaler{ ADC_CLOCKPRESCALER_PCLK_DIV4 };
#endif
        };
    }

    class AdcStm;

    class AnalogToDigitalPinImplStm
        : public AnalogToDigitalPinImplBase<uint16_t>
    {
    public:
        using Config = detail::AdcStmChannelConfig;

        AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc, const Config& config = Config());

        void Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone) override;

    private:
        AnalogPinStm analogPin;
        AdcStm& adc;
        Config config;
    };

    class AnalogToDigitalInternalTemperatureStm
        : public AnalogToDigitalPinImplBase<uint16_t>
    {
    public:
        using Config = detail::AdcStmChannelConfig;

        explicit AnalogToDigitalInternalTemperatureStm(AdcStm& adc, const Config& config = Config());

        void Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone) override;

    private:
        AdcStm& adc;
        Config config;
    };

    class AdcStm
    {
    public:
        using Config = detail::AdcStmConfig;

        explicit AdcStm(uint8_t adcIndex, const Config& config = Config());
        ~AdcStm();

        uint32_t Channel(const hal::AnalogPinStm& pin) const;
        ADC_HandleTypeDef& Handle();

        void EnableOverrunInterrupt();

    private:
        void Measure(const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone);
        void MeasurementDone();

    private:
        friend class AnalogToDigitalPinImplStm;
        friend class AnalogToDigitalInternalTemperatureStm;

        uint8_t index;
        ADC_HandleTypeDef handle{};
        DispatchedInterruptHandler interruptHandler;
        uint16_t sample;
        infra::AutoResetFunction<void(infra::MemoryRange<uint16_t>)> onDone;
    };
}

#endif
