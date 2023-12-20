#include "hal_st/stm32fxxx/AdcDmaStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include <array>
#include <cassert>

namespace hal
{
    AdcTriggeredByTimerWithDma::AdcTriggeredByTimerWithDma(infra::BoundedVector<int16_t>& conversionBuffer, uint8_t adcIndex, hal::GpioPinStm& pin)
        : AdcStm(adcIndex, adcConfig)
        , conversionBuffer(conversionBuffer)
        , pin(pin)
        , timer(2, timerConfig)
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
        HAL_StatusTypeDef result = HAL_ADC_ConfigChannel(&Handle(), &channelConfig);
        assert(result == HAL_OK);
    }

    void AdcTriggeredByTimerWithDma::Measure(const infra::Function<void(infra::BoundedVector<int16_t>&)>& onDone)
    {
        this->onDone = onDone;
    }
}
