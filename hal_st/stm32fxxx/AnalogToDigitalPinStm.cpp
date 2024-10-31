#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include DEVICE_HEADER

#if defined(STM32F7)
extern "C"
{
#include "stm32f7xx_ll_adc.h"
}
#endif

namespace
{
#if defined(STM32G4)
    constexpr std::array irqMap
    {
#if defined(ADC1)
        std::make_pair(1, IRQn_Type::ADC1_2_IRQn), // only ADC1 or ADC2 can be configured to use the single interrupt vector
#endif
#if defined(ADC2)
            std::make_pair(2, IRQn_Type::ADC1_2_IRQn), // only ADC1 or ADC2 can be configured to use the single interrupt vector
#endif

#if defined(ADC3)
            std::make_pair(3, IRQn_Type::ADC3_IRQn),
#endif

#if defined(ADC4)
            std::make_pair(4, IRQn_Type::ADC4_IRQn),
#endif

#if defined(ADC5)
            std::make_pair(5, IRQn_Type::ADC5_IRQn),
#endif
    };

    IRQn_Type LookupIrq(std::size_t index)
    {
        auto iter = std::find_if(irqMap.begin(), irqMap.end(), [index](const auto& pair)
            {
                return pair.first == index;
            });

        if (iter == irqMap.end())
            std::abort();

        return iter->second;
    }
#endif

    constexpr std::array adcChannel{
        // STM32F4x header defines ADC_CHANNEL_0 as 0x0u, all others are cast to uint32_t
        // all other device headers are consistent with all their channel types
        static_cast<decltype(ADC_CHANNEL_1)>(ADC_CHANNEL_0),
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
#ifdef ADC_CHANNEL_18
        ADC_CHANNEL_14,
        ADC_CHANNEL_15,
        ADC_CHANNEL_16,
        ADC_CHANNEL_17,
        ADC_CHANNEL_18,
#endif
#ifdef ADC_CHANNEL_19
        ADC_CHANNEL_19,
#endif
    };
}

namespace hal
{
    AnalogToDigitalPinImplStm::AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc, const Config& config)
        : analogPin(pin)
        , adc(adc)
        , config(config)
    {
        HAL_ADC_Stop(&adc.Handle());
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_SOFTWARE_START);
    }

    void AnalogToDigitalPinImplStm::Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone)
    {
        ADC_ChannelConfTypeDef channelConfig{};
        channelConfig.Channel = adc.Channel(analogPin);

#ifdef ADC_REGULAR_RANK_1
        channelConfig.Rank = ADC_REGULAR_RANK_1;
#else
        channelConfig.Rank = 1;
#endif
#ifdef ADC_SMPR_SMP1
        channelConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
#else
        channelConfig.SamplingTime = config.samplingTime;
#endif
#ifdef ADC_OFFSET_NONE
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
#endif
#ifdef ADC_SINGLE_ENDED
        channelConfig.SingleDiff = ADC_SINGLE_ENDED;
#endif
#ifdef ADC_OFFSET_1
        channelConfig.Offset = 0;
#endif
        HAL_StatusTypeDef result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
        assert(result == HAL_OK);

        adc.Measure(onDone);
    }

    AnalogToDigitalInternalTemperatureStm::AnalogToDigitalInternalTemperatureStm(AdcStm& adc, const Config& config)
        : adc(adc)
        , config(config)
    {
        HAL_ADC_Stop(&adc.Handle());
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_SOFTWARE_START);
    }

    void AnalogToDigitalInternalTemperatureStm::Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone)
    {
        ADC_ChannelConfTypeDef channelConfig{};
#ifdef ADC_CHANNEL_TEMPSENSOR
        channelConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
#else
        if (adc.index == 1)
            channelConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC1;
        else
            channelConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC5;
#endif
#ifdef ADC_REGULAR_RANK_1
        channelConfig.Rank = ADC_REGULAR_RANK_1;
#else
        channelConfig.Rank = 1;
#endif
#ifdef ADC_OFFSET_NONE
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
#endif
#ifdef ADC_SINGLE_ENDED
        channelConfig.SingleDiff = ADC_SINGLE_ENDED;
#endif
#ifdef ADC_OFFSET_1
        channelConfig.Offset = 0;
#endif
        channelConfig.SamplingTime = config.samplingTime;

        HAL_StatusTypeDef result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
        assert(result == HAL_OK);

        adc.Measure(onDone);
    }

    AdcStm::AdcStm(uint8_t oneBasedIndex, const Config& config)
        : index(oneBasedIndex - 1)
#if defined(STM32WB) || defined(STM32G0)
        , interruptHandler(ADC1_IRQn, [this]()
#elif defined(STM32G4)
        , interruptHandler(LookupIrq(oneBasedIndex), [this]()
#elif defined(STM32WBA)
        , interruptHandler(ADC4_IRQn, [this]()
#elif defined(STM32H5)
        , interruptHandler(ADC2_IRQn, [this]()
#else
        , interruptHandler(ADC_IRQn, [this]()
#endif
              {
                  MeasurementDone();
              })
    {
        EnableClockAdc(index);

        handle.Instance = peripheralAdc[index];
        handle.Init.ClockPrescaler = config.clockPrescaler;
        handle.Init.Resolution = ADC_RESOLUTION_12B;
        handle.Init.ScanConvMode = DISABLE;
        handle.Init.ContinuousConvMode = DISABLE;
        handle.Init.DiscontinuousConvMode = DISABLE;
#if defined(ADC_CFGR_DISCNUM) || defined(ADC_CR1_DISCNUM)
        handle.Init.NbrOfDiscConversion = 0;
#endif
        handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
        handle.Init.DMAContinuousRequests = DISABLE;
        handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        handle.Init.NbrOfConversion = 1;
        handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
#ifdef ADC_OVR_DATA_OVERWRITTEN
        handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
#endif
#ifdef IS_ADC_OVERSAMPLING_RATIO
        handle.Init.OversamplingMode = DISABLE;
#endif
#ifdef ADC_SMPR_SMP1
        handle.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_79CYCLES_5;
#endif
        HAL_StatusTypeDef result = HAL_ADC_Init(&handle);
        assert(result == HAL_OK);

#ifdef IS_ADC_SINGLE_DIFFERENTIAL
        result = HAL_ADCEx_Calibration_Start(&handle, ADC_SINGLE_ENDED);
#elif !defined(STM32F7) && !defined(STM32F4)
        result = HAL_ADCEx_Calibration_Start(&handle);
#endif
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
#ifdef ADC_ISR_EOC
        handle.Instance->ISR |= ADC_ISR_EOC | ADC_ISR_EOS;
#else
        handle.Instance->SR &= ~ADC_SR_EOC;
#endif
        interruptHandler.ClearPending();
        sample = static_cast<uint16_t>(HAL_ADC_GetValue(&handle));

        if (onDone)
            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    onDone(infra::MakeRangeFromSingleObject(sample));
                });
    }
}
