#include "hal_st/stm32fxxx/DmaStm.hpp"

#if !defined(STM32F0) && !defined(STM32F1) && !defined(STM32F3)

namespace hal
{
    namespace
    {
        volatile uint32_t* const dmaISR[2][8] = {
            { &DMA1->LISR, &DMA1->LISR, &DMA1->LISR, &DMA1->LISR, &DMA1->HISR, &DMA1->HISR, &DMA1->HISR, &DMA1->HISR },
            { &DMA2->LISR, &DMA2->LISR, &DMA2->LISR, &DMA2->LISR, &DMA2->HISR, &DMA2->HISR, &DMA2->HISR, &DMA2->HISR }
        };

        volatile uint32_t* const dmaIFCR[2][8] = {
            { &DMA1->LIFCR, &DMA1->LIFCR, &DMA1->LIFCR, &DMA1->LIFCR, &DMA1->HIFCR, &DMA1->HIFCR, &DMA1->HIFCR, &DMA1->HIFCR },
            { &DMA2->LIFCR, &DMA2->LIFCR, &DMA2->LIFCR, &DMA2->LIFCR, &DMA2->HIFCR, &DMA2->HIFCR, &DMA2->HIFCR, &DMA2->HIFCR }
        };

        DMA_Stream_TypeDef* const dmaStream[2][8] = {
            { DMA1_Stream0, DMA1_Stream1, DMA1_Stream2, DMA1_Stream3, DMA1_Stream4, DMA1_Stream5, DMA1_Stream6, DMA1_Stream7 },
            { DMA2_Stream0, DMA2_Stream1, DMA2_Stream2, DMA2_Stream3, DMA2_Stream4, DMA2_Stream5, DMA2_Stream6, DMA2_Stream7 }
        };

        const IRQn_Type dmaIrq[2][8] = {
            { DMA1_Stream0_IRQn, DMA1_Stream1_IRQn, DMA1_Stream2_IRQn, DMA1_Stream3_IRQn, DMA1_Stream4_IRQn, DMA1_Stream5_IRQn, DMA1_Stream6_IRQn, DMA1_Stream7_IRQn },
            { DMA2_Stream0_IRQn, DMA2_Stream1_IRQn, DMA2_Stream2_IRQn, DMA2_Stream3_IRQn, DMA2_Stream4_IRQn, DMA2_Stream5_IRQn, DMA2_Stream6_IRQn, DMA2_Stream7_IRQn }
        };

#if defined(STM32F765xx) || defined(STM32F767xx) || defined(STM32F769xx) || defined(STM32F777xx) || defined(STM32F779xx)
        const uint32_t dmaChannel[16] = {
            DMA_CHANNEL_0, DMA_CHANNEL_1, DMA_CHANNEL_2, DMA_CHANNEL_3, DMA_CHANNEL_4, DMA_CHANNEL_5, DMA_CHANNEL_6, DMA_CHANNEL_7,
            DMA_CHANNEL_8, DMA_CHANNEL_9, DMA_CHANNEL_10, DMA_CHANNEL_11, DMA_CHANNEL_12, DMA_CHANNEL_13, DMA_CHANNEL_14, DMA_CHANNEL_15
        };
#else
        const uint32_t dmaChannel[8] = {
            DMA_CHANNEL_0, DMA_CHANNEL_1, DMA_CHANNEL_2, DMA_CHANNEL_3, DMA_CHANNEL_4, DMA_CHANNEL_5, DMA_CHANNEL_6, DMA_CHANNEL_7
        };
#endif

        const uint32_t streamToTCIF[8] = { DMA_FLAG_TCIF0_4, DMA_FLAG_TCIF1_5, DMA_FLAG_TCIF2_6, DMA_FLAG_TCIF3_7, DMA_FLAG_TCIF0_4, DMA_FLAG_TCIF1_5, DMA_FLAG_TCIF2_6, DMA_FLAG_TCIF3_7 };
        const uint32_t streamToTEIF[8] = { DMA_FLAG_TEIF0_4, DMA_FLAG_TEIF1_5, DMA_FLAG_TEIF2_6, DMA_FLAG_TEIF3_7, DMA_FLAG_TEIF0_4, DMA_FLAG_TEIF1_5, DMA_FLAG_TEIF2_6, DMA_FLAG_TEIF3_7 };

        uint32_t dummy;
    }

    DmaStm::DmaStm()
    {
        __DMA1_CLK_ENABLE();
        __DMA2_CLK_ENABLE();
    }

    DmaStm::~DmaStm()
    {
        __DMA1_CLK_DISABLE();
        __DMA2_CLK_DISABLE();
    }

    void DmaStm::ReserveStream(uint8_t dmaIndex, uint8_t streamIndex)
    {
        assert((streamAllocation[dmaIndex] & (1 << streamIndex)) == 0);
        streamAllocation[dmaIndex] |= 1 << streamIndex;
    }

    void DmaStm::ReleaseStream(uint8_t dmaIndex, uint8_t streamIndex)
    {
        streamAllocation[dmaIndex] &= ~(1 << streamIndex);
    }

    DmaStm::Stream::Stream(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress, infra::Function<void()> actionOnTransferComplete)
        : dma(dma)
        , dmaIndex(channelId.dma)
        , streamIndex(channelId.stream)
        , actionOnTransferComplete(actionOnTransferComplete)
        , interruptHandler(hal::dmaIrq[dmaIndex][streamIndex], [this]()
              { OnInterrupt(); })
    {
        dma.ReserveStream(dmaIndex, streamIndex);

        DMA_HandleTypeDef dmaStreamHandle = {};

        dmaStreamHandle.Instance = dmaStream[dmaIndex][streamIndex];

        dmaStreamHandle.Init.Channel = dmaChannel[channelId.channel];
        dmaStreamHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
        dmaStreamHandle.Init.PeriphInc = DMA_PINC_DISABLE;
        dmaStreamHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        dmaStreamHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        dmaStreamHandle.Init.Mode = DMA_NORMAL;
        dmaStreamHandle.Init.Priority = DMA_PRIORITY_MEDIUM;
        dmaStreamHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        dmaStreamHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        dmaStreamHandle.Init.MemBurst = DMA_MBURST_SINGLE;
        dmaStreamHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;

        HAL_DMA_Init(&dmaStreamHandle);
        dmaStreamHandle.Instance->PAR = reinterpret_cast<uint32_t>(peripheralAddress);
    }

    DmaStm::Stream::Stream(Stream&& other)
        : dma(other.dma)
        , dmaIndex(other.dmaIndex)
        , streamIndex(other.streamIndex)
        , actionOnTransferComplete(other.actionOnTransferComplete)
        , interruptHandler(std::move(other.interruptHandler), [this]()
              { OnInterrupt(); })
    {
        other.streamIndex = 0xff;
        other.actionOnTransferComplete = nullptr;
    }

    DmaStm::Stream::~Stream()
    {
        if (streamIndex != 0xff)
            dma.ReleaseStream(dmaIndex, streamIndex);
    }

    DmaStm::Stream& DmaStm::Stream::operator=(Stream&& other)
    {
        dmaIndex = other.dmaIndex;
        streamIndex = other.streamIndex;
        actionOnTransferComplete = other.actionOnTransferComplete;
        interruptHandler.Assign(std::move(other.interruptHandler), [this]()
            { OnInterrupt(); });

        other.streamIndex = 0xff;
        other.actionOnTransferComplete = nullptr;

        return *this;
    }

    void DmaStm::Stream::StartTransmit(infra::ConstByteRange data)
    {
        assert(data.size() <= 65535);
        __DMB();
        DMA_Stream_TypeDef* streamRegister = dmaStream[dmaIndex][streamIndex];
        streamRegister->CR |= DMA_MINC_ENABLE;
        streamRegister->CR |= DMA_MEMORY_TO_PERIPH;

        streamRegister->NDTR = data.size() / DataSize();
        streamRegister->M0AR = reinterpret_cast<uint32_t>(data.begin());

        streamRegister->CR |= DMA_IT_TC;
        streamRegister->CR |= DMA_SxCR_EN;
    }

    void DmaStm::Stream::StartTransmitDummy(uint16_t size)
    {
        DMA_Stream_TypeDef* streamRegister = dmaStream[dmaIndex][streamIndex];
        streamRegister->CR &= ~DMA_MINC_ENABLE;
        streamRegister->CR |= DMA_MEMORY_TO_PERIPH;

        streamRegister->NDTR = size / DataSize();
        streamRegister->M0AR = reinterpret_cast<uint32_t>(&dummy);

        streamRegister->CR |= DMA_IT_TC;
        streamRegister->CR |= DMA_SxCR_EN;
    }

    void DmaStm::Stream::StartReceive(infra::ByteRange data)
    {
        assert(data.size() <= 65535);
        DMA_Stream_TypeDef* streamRegister = dmaStream[dmaIndex][streamIndex];
        streamRegister->CR &= ~DMA_MEMORY_TO_PERIPH;
        streamRegister->CR |= DMA_MINC_ENABLE;

        streamRegister->NDTR = data.size() / DataSize();
        streamRegister->M0AR = reinterpret_cast<uint32_t>(data.begin());

        streamRegister->CR |= DMA_IT_TC;
        streamRegister->CR |= DMA_SxCR_EN;
    }

    void DmaStm::Stream::StartReceiveDummy(uint16_t size)
    {
        DMA_Stream_TypeDef* streamRegister = dmaStream[dmaIndex][streamIndex];
        streamRegister->CR &= ~DMA_MEMORY_TO_PERIPH;
        streamRegister->CR &= ~DMA_MINC_ENABLE;

        streamRegister->NDTR = size / DataSize();
        streamRegister->M0AR = reinterpret_cast<uint32_t>(&dummy);

        streamRegister->CR |= DMA_IT_TC;
        streamRegister->CR |= DMA_SxCR_EN;
    }

    bool DmaStm::Stream::StopTransfer()
    {
        DMA_Stream_TypeDef* streamRegister = dmaStream[dmaIndex][streamIndex];
        bool finished = (streamRegister->CR & DMA_SxCR_EN) == 0;
        streamRegister->CR &= ~DMA_SxCR_EN;
        *dmaIFCR[dmaIndex][streamIndex] |= streamToTCIF[streamIndex];
        return !finished;
    }

    uint8_t DmaStm::Stream::DataSize() const
    {
        DMA_Stream_TypeDef* streamRegister = dmaStream[dmaIndex][streamIndex];
        return 1 << ((streamRegister->CR & DMA_SxCR_PSIZE) >> POSITION_VAL(DMA_SxCR_PSIZE));
    }

    void DmaStm::Stream::SetDataSize(uint8_t dataSizeInBytes)
    {
        DMA_Stream_TypeDef* streamRegister = dmaStream[dmaIndex][streamIndex];
        streamRegister->CR = (streamRegister->CR & ~DMA_SxCR_PSIZE) | ((dataSizeInBytes & 1) == 0 ? DMA_SxCR_PSIZE_0 : 0) | (dataSizeInBytes > 2 ? DMA_SxCR_PSIZE_1 : 0);
    }

    void DmaStm::Stream::OnInterrupt()
    {
        if (*dmaISR[dmaIndex][streamIndex] & streamToTCIF[streamIndex])
        {
            *dmaIFCR[dmaIndex][streamIndex] |= streamToTCIF[streamIndex];
            DMA_Stream_TypeDef* streamRegister = dmaStream[dmaIndex][streamIndex];
            streamRegister->CR &= ~DMA_SxCR_EN;

            __DMB();

            interruptHandler.ClearPending();
            actionOnTransferComplete();
        }
    }
}

#endif
