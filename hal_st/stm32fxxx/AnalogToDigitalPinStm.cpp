#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/MemoryRange.hpp"
#include <array>
#include <cassert>
#if defined(STM32F7)
extern "C"
{
    #include "stm32f7xx_ll_adc.h"
}
#endif

namespace
{
    constexpr std::array<uint32_t, 19> adcChannel = {
        ADC_CHANNEL_0,
        ADC_CHANNEL_1,
        ADC_CHANNEL_2,
        ADC_CHANNEL_3,
        ADC_CHANNEL_4,
        ADC_CHANNEL_5,
        ADC_CHANNEL_6,
        ADC_CHANNEL_7,
        ADC_CHANNEL_8,
        ADC_CHANNEL_9,
        ADC_CHANNEL_10,
        ADC_CHANNEL_11,
        ADC_CHANNEL_12,
        ADC_CHANNEL_13,
        ADC_CHANNEL_14,
        ADC_CHANNEL_15,
        ADC_CHANNEL_16,
        ADC_CHANNEL_17,
        ADC_CHANNEL_18
    };
}

namespace hal
{
    AnalogToDigitalPinImplStm::AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc)
        : analogPin(pin)
        , adc(adc)
    {
        HAL_ADC_Stop(&adc.Handle());
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_SOFTWARE_START);
    }

    void AnalogToDigitalPinImplStm::Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone)
    {
        ADC_ChannelConfTypeDef channelConfig;
        channelConfig.Channel = adc.Channel(analogPin);
#if defined(STM32WB) || defined(STM32G0) || defined(STM32G4)
        channelConfig.Rank = ADC_REGULAR_RANK_1;
#else
        channelConfig.Rank = 1;
#endif
#if defined(STM32F0) || defined(STM32F3)
        channelConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
#elif defined(STM32WB) || defined(STM32G4)
        channelConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        channelConfig.Offset = 0;
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
        channelConfig.SingleDiff = ADC_SINGLE_ENDED;
#elif defined(STM32G0)
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
#else
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        channelConfig.Offset = 0;
#endif
        HAL_StatusTypeDef result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
        assert(result == HAL_OK);

        adc.Measure(onDone);
    }

    AnalogToDigitalInternalTemperatureImplStm::AnalogToDigitalInternalTemperatureImplStm(AdcStm& adc, uint16_t voltageReferenceMiliVolts)
        : adc(adc)
        , voltageReferenceMiliVolts(voltageReferenceMiliVolts)
    {
        HAL_ADC_Stop(&adc.Handle());
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_SOFTWARE_START);
    }

    void AnalogToDigitalInternalTemperatureImplStm::Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone)
    {
        ADC_ChannelConfTypeDef channelConfig;

        this->onDone = onDone;

#if !defined(STM32G4)
        channelConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
#else
        if (adc.index == 1)
            channelConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC1;
        else
            channelConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC5;
#endif
#if defined(STM32WB) || defined(STM32G0) || defined(STM32G4)
        channelConfig.Rank = ADC_REGULAR_RANK_1;
#else
        channelConfig.Rank = 1;
#endif
#if defined(STM32F0) || defined(STM32F3)
        channelConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
#elif defined(STM32WB) || defined(STM32G4)
        channelConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        channelConfig.Offset = 0;
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
        channelConfig.SingleDiff = ADC_SINGLE_ENDED;
#elif defined(STM32G0)
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
#else
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        channelConfig.Offset = 0;
#endif
        HAL_StatusTypeDef result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
        assert(result == HAL_OK);

        adc.Measure([this](infra::MemoryRange<uint16_t> samples)
            {
                for (auto& sample : samples)
                    sample = static_cast<uint16_t>(__LL_ADC_CALC_TEMPERATURE(voltageReferenceMiliVolts, sample, LL_ADC_RESOLUTION_12B));

                this->onDone(samples);
            });
    }

    AdcStm::AdcStm(uint8_t oneBasedIndex)
        : index(oneBasedIndex - 1)
#if defined(STM32WB) || defined(STM32G0)
        , interruptHandler(ADC1_IRQn, [this]()
#elif defined(STM32G4)
        , interruptHandler(ADC1_2_IRQn, [this]()
#else
        , interruptHandler(ADC_IRQn, [this]()
#endif
              { MeasurementDone(); })
    {
        EnableClockAdc(index);

        handle.Instance = peripheralAdc[index];
#if !defined(STM32F3)
        handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
        handle.Init.Resolution = ADC_RESOLUTION_12B;
#endif
        handle.Init.ScanConvMode = DISABLE;
        handle.Init.ContinuousConvMode = DISABLE;
        handle.Init.DiscontinuousConvMode = DISABLE;
#if !defined(STM32G0)
        handle.Init.NbrOfDiscConversion = 0;
#endif
#if !defined(STM32F3)
        handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
#endif
        handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        handle.Init.NbrOfConversion = 1;
#if !defined(STM32F3)
        handle.Init.DMAContinuousRequests = DISABLE;
#endif
#if defined(STM32WB) || defined(STM32G0) || defined(STM32G4)
        handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
        handle.Init.OversamplingMode = DISABLE;
#elif !defined(STM32F3)
        handle.Init.EOCSelection = DISABLE;
#endif
        HAL_StatusTypeDef result = HAL_ADC_Init(&handle);
        assert(result == HAL_OK);
    }

    AdcStm::~AdcStm()
    {
        DisableClockAdc(index);
    }

    void AdcStm::Measure(const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone)
    {
        this->onDone = onDone;

        HAL_StatusTypeDef result = HAL_ADC_Start_IT(&handle);
        assert(result == HAL_OK);
    }

    uint32_t AdcStm::Channel(const hal::AnalogPinStm& pin) const
    {
        return adcChannel[pin.AdcChannel(index + 1)];
    }

    ADC_HandleTypeDef& AdcStm::Handle()
    {
        return handle;
    }

    void AdcStm::MeasurementDone()
    {
        assert(onDone != nullptr);
#if defined(STM32WB) || defined(STM32G4) || defined(STM32G0)
        handle.Instance->ISR |= ADC_ISR_EOC | ADC_ISR_EOS;
#else
        handle.Instance->SR &= ~ADC_SR_EOC;
#endif
        interruptHandler.ClearPending();
        sample = static_cast<uint16_t>(HAL_ADC_GetValue(&handle));
        onDone(infra::MakeRangeFromSingleObject(sample));
    }
}
