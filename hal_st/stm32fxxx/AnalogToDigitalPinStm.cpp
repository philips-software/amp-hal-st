#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "stm32g4xx_hal_adc.h"
#include <cassert>

namespace hal
{
    namespace
    {
#if defined(STM32G4)
        constexpr std::array indexToChannel{
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

        constexpr std::array indexToIrq{
            IRQn_Type::ADC1_2_IRQn,
            IRQn_Type::ADC3_IRQn,
            IRQn_Type::ADC4_IRQn,
            IRQn_Type::ADC5_IRQn
        };

        std::array groupClockCount{ 0u, 0u };

        void EnableGroupClockAdc(uint32_t index)
        {
            auto& clockCount = (index <= 1) ? groupClockCount[0] : groupClockCount[1];
            ++clockCount;

            if (clockCount == 1)
                EnableClockAdc(index);
        }

        void DisableGroupClockAdc(uint32_t index)
        {
            auto& clockCount = (index <= 1) ? groupClockCount[0] : groupClockCount[1];
            --clockCount;

            if (clockCount == 0)
                DisableClockAdc(index);
        }
#endif
    }

    AnalogToDigitalPinImplStm::AnalogToDigitalPinImplStm(hal::GpioPinStm& pin, AdcStm& adc)
        : pin(pin)
        , adc(adc)
    {}

    void AnalogToDigitalPinImplStm::Measure(const infra::Function<void(int32_t value)>& onDone)
    {
        ADC_ChannelConfTypeDef channelConfig;

#if defined(STM32G4)
        channelConfig.Channel = indexToChannel[pin.AdcChannel(adc.index + 1)];
#else
        channelConfig.Channel = pin.AdcChannel(adc.index + 1);
#endif

#if defined(STM32G4)
        channelConfig.Rank = ADC_REGULAR_RANK_1;
#else
        channelConfig.Rank = 1;
#endif
#if defined(STM32F0) || defined(STM32F3)
        channelConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
#elif defined(STM32WB)
        channelConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        channelConfig.Offset = 0;
#elif defined(STM32G0)
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
#elif defined(STM32G4)
        channelConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        channelConfig.SingleDiff = ADC_SINGLE_ENDED;
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
        channelConfig.Offset = 0;
#else
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        channelConfig.Offset = 0;
#endif
        HAL_StatusTypeDef result = HAL_ADC_ConfigChannel(&adc.handle, &channelConfig);
        assert(result == HAL_OK);

        adc.Measure(pin, onDone);
    }

    AdcStm::AdcStm(uint8_t oneBasedIndex)
        : index(oneBasedIndex - 1)
        , handle()
#if defined(STM32WB) || defined(STM32G0)
        , interruptHandler(ADC1_IRQn, [this]()
#elif defined(STM32G4)
        , interruptHandler(indexToIrq[index], [this]()
#else
        , interruptHandler(ADC_IRQn, [this]()
#endif
              {
#if defined(STM32G4)
                const auto isr = peripheralAdc[index]->ISR;
                const auto ier = peripheralAdc[index]->IER;
                const auto interrupt_active = (isr & ADC_FLAG_EOC) && (ier & ADC_IT_EOC);

                if ((index == 0 && interrupt_active)
                 || (index == 1 && interrupt_active)
                 || (index > 1))
#endif
                MeasurementDone(); })
    {
#if defined(STM32G4)
        EnableGroupClockAdc(index);
#else
        EnableClockAdc(index);
#endif

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
#if defined(STM32G4)
        handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
#else
        handle.Init.EOCSelection = DISABLE;
#endif
#endif

#if defined(STM32G4)
        handle.Init.GainCompensation = 0;
        handle.Init.LowPowerAutoWait = DISABLE;
        handle.Init.Overrun = ADC_OVR_DATA_PRESERVED;
        handle.Init.OversamplingMode = DISABLE;
#endif

        HAL_StatusTypeDef result = HAL_ADC_Init(&handle);
        assert(result == HAL_OK);
    }

    AdcStm::~AdcStm()
    {
#if defined(STM32G4)
        DisableGroupClockAdc(index);
#else
        DisableClockAdc(index);
#endif
    }

    void AdcStm::Measure(AnalogPinStm& pin, const infra::Function<void(int32_t value)>& onDone)
    {
        this->onDone = onDone;

        HAL_StatusTypeDef result = HAL_ADC_Start_IT(&handle);
        assert(result == HAL_OK);
    }

    void AdcStm::MeasurementDone()
    {
        assert(onDone != nullptr);
#if defined(STM32WB) || defined(STM32G4) || defined(STM32G0)
        handle.Instance->ISR &= ~ADC_ISR_EOC;
#else
        handle.Instance->SR &= ~ADC_SR_EOC;
#endif
        interruptHandler.ClearPending();
        onDone(HAL_ADC_GetValue(&handle));
    }
}
