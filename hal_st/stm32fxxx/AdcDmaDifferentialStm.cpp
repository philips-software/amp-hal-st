#include "hal_st/stm32fxxx/AdcDmaDifferentialStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/MemoryRange.hpp"
#include "stm32h5xx_hal_adc.h"
#include <cstdint>
#include DEVICE_HEADER

namespace hal
{
    AdcDifferentialTriggeredByTimer::AdcDifferentialTriggeredByTimer(AdcStm& adc, DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedTimerIndex, TimerBaseStm::Timing timing, hal::GpioPinStm& positivePin, hal::GpioPinStm& negativePin, const Config& config)
        : AdcTimerTriggeredBase(adc, oneBasedTimerIndex, timing)
        , adc(adc)
        , dmaStream(receiveStream, &adc.Handle().Instance->DR, sizeof(uint16_t), [] {}, [this]
              {
                  TransferDone();
              })
        , analogPinPositive(positivePin)
        , analogPinNegative(negativePin)
    {
        ADC_ChannelConfTypeDef channelConfig{};
        channelConfig.Channel = adc.Channel(analogPinPositive);
#ifdef ADC_REGULAR_RANK_1
        channelConfig.Rank = ADC_REGULAR_RANK_1;
#else
        channelConfig.Rank = 1;
#endif
        channelConfig.SamplingTime = config.samplingTime;
#ifdef ADC_OFFSET_NONE
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
#endif
#ifdef ADC_DIFFERENTIAL_ENDED
        channelConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;
#endif
#ifdef ADC_OFFSET_1
        channelConfig.Offset = 0;
#endif
        ReconfigureTrigger();

        auto result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
        assert(result == HAL_OK);

#ifdef ADC_DIFFERENTIAL_ENDED
        result = HAL_ADCEx_Calibration_Start(&adc.Handle(), ADC_DIFFERENTIAL_ENDED);
#elif defined(IS_ADC_CALFACT)
        result = HAL_ADCEx_Calibration_Start(&adc.Handle());
#endif
        assert(result == HAL_OK);
    }

    void AdcDifferentialTriggeredByTimer::Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone)
    {
        this->onDone = onDone;

        StartTimer();
        Configure();
        dmaStream.StartReceive(infra::MakeRangeFromSingleObject(buffer));
        LL_ADC_REG_StartConversion(adc.Handle().Instance);
    }

    void AdcDifferentialTriggeredByTimer::Stop()
    {
        if (LL_ADC_REG_IsConversionOngoing(adc.Handle().Instance))
        {
            LL_ADC_REG_StopConversion(adc.Handle().Instance);
        }

        dmaStream.StopTransfer();

        auto result = ADC_Disable(&adc.Handle());
        assert(result == HAL_OK);

        StopTimer();
    }

    void AdcDifferentialTriggeredByTimer::Configure()
    {
        auto result = LL_ADC_REG_IsConversionOngoing(adc.Handle().Instance);
        assert(result == 0);

        result = ADC_Enable(&adc.Handle());
        assert(result == HAL_OK);

        __HAL_ADC_CLEAR_FLAG(&adc.Handle(), (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
        LL_ADC_REG_SetDMATransfer(adc.Handle().Instance, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    }

    void AdcDifferentialTriggeredByTimer::TransferDone()
    {
        onDone(infra::MakeRangeFromSingleObject(buffer));
    }
}
