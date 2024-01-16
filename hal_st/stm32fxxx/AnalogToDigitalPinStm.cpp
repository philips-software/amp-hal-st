#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/EnumCast.hpp"
#include "infra/util/MemoryRange.hpp"
#include <array>
#include <cassert>

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

    constexpr std::array<uint32_t, 3> triggerSource = {
        ADC_SOFTWARE_START,

#if defined(STM32F4) || defined(STM32F7)
        ADC_EXTERNALTRIGCONV_T2_TRGO,
#elif defined(STM32G0)
        ADC_EXTERNALTRIG_T3_TRGO,
#else
        ADC_EXTERNALTRIG_T2_TRGO,
#endif

#if defined(STM32F4)
        ADC_EXTERNALTRIGCONV_Ext_IT11,
#elif defined(STM32F7)
        ADC_EXTERNALTRIG7_EXT_IT11,
#else
        ADC_EXTERNALTRIG_EXT_IT11,
#endif
    };
}

namespace hal
{
    AdcStm::AdcStm(uint8_t oneBasedIndex, const Config& config)
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
        handle.Init.ExternalTrigConvEdge = infra::enum_cast(config.triggerEdge);
#endif
        handle.Init.ExternalTrigConv = triggerSource.at(infra::enum_cast(config.triggerSource));
        handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        handle.Init.NbrOfConversion = 1;
#if !defined(STM32F3)
        handle.Init.DMAContinuousRequests = DISABLE;
#endif
#if defined(STM32WB)
        handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
        handle.Init.OversamplingMode = DISABLE;
#elif !defined(STM32F3)
        handle.Init.EOCSelection = DISABLE;
#endif
        HAL_StatusTypeDef result = HAL_ADC_Init(&handle);
        assert(result == HAL_OK);
    }

    AdcStm::AdcStm(uint8_t adcIndex)
        : AdcStm(adcIndex, { AdcStm::TriggerSource::software, AdcStm::TriggerEdge::none })
    {}

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
        sample = HAL_ADC_GetValue(&handle);
        onDone(infra::MakeRangeFromSingleObject(sample));
    }

    AnalogToDigitalPinImplStm::AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc)
        : analogPin(pin)
        , adc(adc)
    {}

    void AnalogToDigitalPinImplStm::Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone)
    {
        ADC_ChannelConfTypeDef channelConfig;
        channelConfig.Channel = adc.Channel(analogPin);
#if !defined(STM32WB)
        channelConfig.Rank = 1;
#else
        channelConfig.Rank = ADC_REGULAR_RANK_1;
#endif
#if defined(STM32F0) || defined(STM32F3)
        channelConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
#elif defined(STM32WB)
        channelConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        channelConfig.Offset = 0;
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
        channelConfig.SingleDiff = ADC_SINGLE_ENDED;
#elif defined(STM32G0)
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
#elif defined(STM32G4)
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
        channelConfig.Offset = 0;
#else
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        channelConfig.Offset = 0;
#endif
        HAL_StatusTypeDef result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
        assert(result == HAL_OK);

        adc.Measure(onDone);
    }
}
