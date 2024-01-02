#include "hal_st/stm32fxxx/AdcDmaStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/Function.hpp"
#include <array>
#include <cassert>

namespace hal
{
    AdcTriggeredByTimerWithDma::AdcTriggeredByTimerWithDma(infra::BoundedVector<int16_t>& conversionBuffer, hal::DmaStm& dma, DmaChannelId dmaChannelId, uint8_t adcIndex, hal::GpioPinStm& pin)
        : AdcStm(adcIndex, adcConfig)
        , conversionBuffer(conversionBuffer)
        , stream(dma, dmaChannelId, &Handle().Instance->DR, infra::emptyFunction, [this]()
            {
                TransferDone();
            })
        , pin(pin)
#if defined(STM32G0)
        , timer(3, timerConfig)
#else
        , timer(2, timerConfig)
#endif
    {
        ADC_ChannelConfTypeDef channelConfig;
        channelConfig.Channel = Channel(pin);
#if !defined(STM32WB)
        channelConfig.Rank = 1;
#else
        channelConfig.Rank = ADC_REGULAR_RANK_1;
#endif
#if defined(STM32F0) || defined(STM32F3)
        channelConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
#elif defined(STM32WB)
        channelConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
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
        auto result = HAL_ADC_ConfigChannel(&Handle(), &channelConfig);
        assert(result == HAL_OK);

        result = HAL_ADCEx_Calibration_Start(&Handle(), ADC_SINGLE_ENDED);
        assert(result == HAL_OK);
    }

    void AdcTriggeredByTimerWithDma::Measure(const infra::Function<void(infra::BoundedVector<int16_t>&)>& onDone)
    {
        this->onDone = onDone;

        timer.Start();

        auto result = LL_ADC_REG_IsConversionOngoing(Handle().Instance);
        assert(result == 0);

        result = ADC_Enable(&Handle());
        assert(result == HAL_OK);

        __HAL_ADC_CLEAR_FLAG(&Handle(), (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
        __HAL_ADC_ENABLE_IT(&Handle(), ADC_IT_OVR);
        SET_BIT(Handle().Instance->CFGR, ADC_CFGR_DMAEN);
        stream.TransferPeripheralToMemory(infra::MakeRange(conversionBuffer));
        LL_ADC_REG_StartConversion(Handle().Instance);
    }

    void AdcTriggeredByTimerWithDma::TransferDone()
    {
        auto result = ADC_Disable(&Handle());
        assert(result == HAL_OK);

        timer.Stop();

        if (this->onDone)
            this->onDone(conversionBuffer);
    }
}
