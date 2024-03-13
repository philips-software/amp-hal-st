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
#if defined(STM32G0)
        , timer(3, timing, { TimerBaseStm::CounterMode::up, infra::MakeOptional<TimerBaseStm::Trigger>({ TimerBaseStm::Trigger::TriggerOutput::update, false }) })
#else
        , timer(2, timing, { TimerBaseStm::CounterMode::up, infra::MakeOptional<TimerBaseStm::Trigger>({ TimerBaseStm::Trigger::TriggerOutput::update, false }) })
#endif
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
        channelConfig.SingleDiff = ADC_SINGLE_ENDED;
        channelConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
        channelConfig.Offset = 0;
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
#elif defined(STM32G0)
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
#elif defined(STM32G4)
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES_5;
        channelConfig.Offset = 0;
#else
        channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
        channelConfig.Offset = 0;
#endif
        ReconfigureTrigger();

        auto result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
        assert(result == HAL_OK);

        result = HAL_ADCEx_Calibration_Start(&adc.Handle(), ADC_SINGLE_ENDED);
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

#if defined(STM32F4) || defined(STM32F7)
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_EXTERNALTRIGCONV_T2_TRGO);
#elif defined(STM32G0)
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_EXTERNALTRIG_T3_TRGO);
#else
        LL_ADC_REG_SetTriggerSource(adc.Handle().Instance, ADC_EXTERNALTRIG_T2_TRGO);
#endif
        LL_ADC_REG_SetTriggerEdge(adc.Handle().Instance, LL_ADC_REG_TRIG_EXT_RISING);
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
