#include "AdcDmaMultiChannelStm.hpp"
#include "infra/event/EventDispatcher.hpp"

namespace
{
    constexpr std::array<uint32_t, hal::AdcDmaMultiChannelStmBase::MaxChannels> rank = {
        ADC_REGULAR_RANK_1, ADC_REGULAR_RANK_2, ADC_REGULAR_RANK_3, ADC_REGULAR_RANK_4,
        ADC_REGULAR_RANK_5, ADC_REGULAR_RANK_6, ADC_REGULAR_RANK_7, ADC_REGULAR_RANK_8,
#if defined(ADC_REGULAR_RANK_9)
        ADC_REGULAR_RANK_9, ADC_REGULAR_RANK_10, ADC_REGULAR_RANK_11, ADC_REGULAR_RANK_12,
        ADC_REGULAR_RANK_13, ADC_REGULAR_RANK_14, ADC_REGULAR_RANK_15, ADC_REGULAR_RANK_16
#endif
    };
}

namespace hal
{
    AdcDmaMultiChannelStmBase::AdcDmaMultiChannelStmBase(
        infra::MemoryRange<uint16_t> buffer, infra::MemoryRange<AnalogPinStm> analogPins, AdcStm& adc,
        DmaStm::ReceiveStream& receiveStream)
        : buffer{ buffer }
        , analogPins{ analogPins }
        , adc{ adc }
        , dmaStream{ receiveStream, &adc.Handle().Instance->DR, sizeof(uint16_t), [this]()
            {
                TransferDone();
            } }
    {

#ifdef ADC_SINGLE_ENDED
        auto result = HAL_ADCEx_Calibration_Start(&adc.Handle(), ADC_SINGLE_ENDED);
        assert(result == HAL_OK);
#elif defined(IS_ADC_CALFACT)
        auto result = HAL_ADCEx_Calibration_Start(&adc.Handle());        
        assert(result == HAL_OK);
#endif

        LL_ADC_REG_SetDMATransfer(adc.Handle().Instance, LL_ADC_REG_DMA_TRANSFER_LIMITED);
    }

    void AdcDmaMultiChannelStmBase::Measure(const infra::Function<void(infra::MemoryRange<uint16_t>)>& onDone)
    {
        this->onDone = onDone;

        auto result = LL_ADC_REG_IsConversionOngoing(adc.Handle().Instance);
        assert(result == 0);

        result = ADC_Enable(&adc.Handle());
        assert(result == HAL_OK);

        __HAL_ADC_CLEAR_FLAG(&adc.Handle(), (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
        dmaStream.StartReceive(buffer);
        LL_ADC_REG_StartConversion(adc.Handle().Instance);
    }

    void AdcDmaMultiChannelStmBase::ConfigureChannels()
    {
        ADC_ChannelConfTypeDef channelConfig = { 0 };
#ifdef ADC_SMPR_SMP1
        channelConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
#else
        channelConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
#endif
#ifdef ADC_SINGLE_ENDED
        channelConfig.SingleDiff = ADC_SINGLE_ENDED;
#endif
#ifdef ADC_OFFSET_NONE
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
        channelConfig.Offset = 0;
#endif

        for (std::size_t i = 0; i != analogPins.size(); ++i)
        {
            channelConfig.Channel = adc.Channel(analogPins[i]);
            channelConfig.Rank = rank[i];
            auto result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
            assert(result == HAL_OK);
        }
        LL_ADC_REG_SetSequencerLength(adc.Handle().Instance, analogPins.size() - 1);
    }

    void AdcDmaMultiChannelStmBase::TransferDone()
    {
        auto result = ADC_Disable(&adc.Handle());
        assert(result == HAL_OK);

        if (this->onDone)
            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    onDone(buffer);
                });
    }
}
