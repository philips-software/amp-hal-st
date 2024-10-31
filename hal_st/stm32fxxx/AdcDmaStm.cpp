#include "hal_st/stm32fxxx/AdcDmaStm.hpp"
#include "infra/event/EventDispatcher.hpp"

namespace hal
{
    AdcTriggeredByTimerWithDma::AdcTriggeredByTimerWithDma(infra::MemoryRange<uint16_t> buffer, AdcStm& adc, DmaStm::ReceiveStream& receiveStream, TimerBaseStm::Timing timing, hal::GpioPinStm& pin)
        : adc(adc)
        , buffer(buffer)
        , dmaStream(receiveStream, &adc.Handle().Instance->DR, sizeof(uint16_t), [this]()
            {
                TransferDone();
            })
        , analogPin(pin)
#ifdef TIM2
        , timer(2, timing, { TimerBaseStm::CounterMode::up, infra::MakeOptional<TimerBaseStm::Trigger>({ TimerBaseStm::Trigger::TriggerOutput::update, false }) })
#else
        , timer(3, timing, { TimerBaseStm::CounterMode::up, infra::MakeOptional<TimerBaseStm::Trigger>({ TimerBaseStm::Trigger::TriggerOutput::update, false }) })
#endif
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
        channelConfig.SamplingTime = hal::detail::AdcStmChannelConfig().samplingTime;
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
        ReconfigureTrigger();

        auto result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
        assert(result == HAL_OK);

#ifdef ADC_SINGLE_ENDED
        result = HAL_ADCEx_Calibration_Start(&adc.Handle(), ADC_SINGLE_ENDED);
#elif defined(IS_ADC_CALFACT)
        result = HAL_ADCEx_Calibration_Start(&adc.Handle());
#endif
        assert(result == HAL_OK);
    }

    void AdcTriggeredByTimerWithDma::Measure(std::size_t numberOfSamples, const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone)
    {
        this->onDone = onDone;

        timer.Start();
        Configure();
        dmaStream.StartReceive(buffer);
        LL_ADC_REG_StartConversion(adc.Handle().Instance);
    }

    void AdcTriggeredByTimerWithDma::ReconfigureTrigger()
    {
        HAL_ADC_Stop(&adc.Handle());

#if defined(ADC_EXTERNALTRIGCONV_T2_TRGO)
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_EXTERNALTRIGCONV_T2_TRGO);
#elif defined(ADC_EXTERNALTRIG_T3_TRGO)
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_EXTERNALTRIG_T3_TRGO);
#else
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_EXTERNALTRIG_T2_TRGO);
#endif
        LL_ADC_REG_SetTriggerEdge(adc.Handle().Instance, ADC_EXTERNALTRIGCONVEDGE_RISING);
    }

    void AdcTriggeredByTimerWithDma::Configure()
    {
        auto result = LL_ADC_REG_IsConversionOngoing(adc.Handle().Instance);
        assert(result == 0);

        result = ADC_Enable(&adc.Handle());
        assert(result == HAL_OK);

        __HAL_ADC_CLEAR_FLAG(&adc.Handle(), (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
        LL_ADC_REG_SetDMATransfer(adc.Handle().Instance, LL_ADC_REG_DMA_TRANSFER_LIMITED);
    }

    void AdcTriggeredByTimerWithDma::TransferDone()
    {
        auto result = ADC_Disable(&adc.Handle());
        assert(result == HAL_OK);

        timer.Stop();

        if (this->onDone)
            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    onDone(buffer);
                });
    }
}
