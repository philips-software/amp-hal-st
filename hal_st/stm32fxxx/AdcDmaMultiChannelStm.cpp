#include "hal_st/stm32fxxx/AdcDmaMultiChannelStm.hpp"
#include "hal_st/stm32fxxx/AnalogToDigitalPinStm.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include <cstdint>
#include DEVICE_HEADER

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
    AdcDmaMultiChannelStmBase::AdcDmaMultiChannelStmBase(infra::MemoryRange<uint16_t> buffer, infra::MemoryRange<AnalogPinStm> analogPins, AdcStm& adc, DmaStm::ReceiveStream& receiveStream, OneShot)
        : buffer{ buffer }
        , analogPins{ analogPins }
        , adc{ adc }
        , dmaStream(infra::InPlaceType<ReceiveDmaChannel>{}, receiveStream, &adc.Handle().Instance->DR, sizeof(uint16_t), [this]()
              {
                  TransferDone();
              },
              DmaStm::StreamInterruptHandler::immediate)
    {
        Initialize();
        LL_ADC_REG_SetDMATransfer(adc.Handle().Instance, LL_ADC_REG_DMA_TRANSFER_LIMITED);
    }

    AdcDmaMultiChannelStmBase::AdcDmaMultiChannelStmBase(infra::MemoryRange<uint16_t> buffer, infra::MemoryRange<AnalogPinStm> analogPins, AdcStm& adc, DmaStm::ReceiveStream& receiveStream, Unlimited)
        : buffer{ buffer }
        , analogPins{ analogPins }
        , adc{ adc }
        , dmaStream(infra::InPlaceType<CircularReceiveDmaChannel>{}, receiveStream, &adc.Handle().Instance->DR, sizeof(uint16_t), infra::emptyFunction, [this]()
              {
                  TransferDone();
              })
    {
        Initialize();
        LL_ADC_REG_SetDMATransfer(adc.Handle().Instance, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    }

    void AdcDmaMultiChannelStmBase::Initialize()
    {
#ifdef ADC_DIFFERENTIAL_ENDED
        // single ended calibration is already done by AdcStm
        auto result = HAL_ADCEx_Calibration_Start(&adc.Handle(), ADC_DIFFERENTIAL_ENDED);
        assert(result == HAL_OK);
#elif defined(IS_ADC_CALFACT)
        auto result = HAL_ADCEx_Calibration_Start(&adc.Handle());
        assert(result == HAL_OK);
#endif
        adc.EnableOverrunInterrupt();
    }

    void AdcDmaMultiChannelStmBase::Measure(const infra::Function<void(Samples)>& onDone)
    {
        this->onDone = onDone;

        auto result = LL_ADC_REG_IsConversionOngoing(adc.Handle().Instance);
        assert(result == 0);

        result = ADC_Enable(&adc.Handle());
        assert(result == HAL_OK);

        __HAL_ADC_CLEAR_FLAG(&adc.Handle(), (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
        if (dmaStream.Is<ReceiveDmaChannel>())
            dmaStream.Get<ReceiveDmaChannel>().StartReceive(buffer);
        else
            dmaStream.Get<CircularReceiveDmaChannel>().StartReceive(buffer);
        LL_ADC_REG_StartConversion(adc.Handle().Instance);
    }

    void AdcDmaMultiChannelStmBase::Stop()
    {
        if (LL_ADC_REG_IsConversionOngoing(adc.Handle().Instance))
            LL_ADC_REG_StopConversion(adc.Handle().Instance);

        if (dmaStream.Is<ReceiveDmaChannel>())
            dmaStream.Get<ReceiveDmaChannel>().StopTransfer();
        else
            dmaStream.Get<CircularReceiveDmaChannel>().StopTransfer();

        auto result = ADC_Disable(&adc.Handle());
        assert(result == HAL_OK);
    }

    void AdcDmaMultiChannelStmBase::ConfigureChannels(infra::MemoryRange<const detail::AdcStmChannelConfig> configs)
    {
        ADC_ChannelConfTypeDef channelConfig = { 0 };
#ifdef ADC_OFFSET_NONE
        channelConfig.OffsetNumber = ADC_OFFSET_NONE;
        channelConfig.Offset = 0;
#endif

        for (std::size_t i = 0; i != analogPins.size(); ++i)
        {
            channelConfig.SamplingTime = configs[i].samplingTime;
#ifdef ADC_SINGLE_ENDED
            channelConfig.SingleDiff = configs[i].differential ? ADC_DIFFERENTIAL_ENDED : ADC_SINGLE_ENDED;
#endif
            channelConfig.Channel = adc.Channel(analogPins[i]);
            channelConfig.Rank = rank[i];
            auto result = HAL_ADC_ConfigChannel(&adc.Handle(), &channelConfig);
            assert(result == HAL_OK);
        }
        LL_ADC_REG_SetSequencerLength(adc.Handle().Instance, analogPins.size() - 1);
    }

    void AdcDmaMultiChannelStmBase::TransferDone()
    {
        if (dmaStream.Is<ReceiveDmaChannel>())
        {
            auto result = ADC_Disable(&adc.Handle());
            assert(result == HAL_OK);
        }
        if (this->onDone)
            onDone(buffer);
    }
}
