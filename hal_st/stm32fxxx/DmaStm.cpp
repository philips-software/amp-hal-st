#include "hal_st/stm32fxxx/DmaStm.hpp"

#if !defined(STM32F0) && !defined(STM32F1) && !defined(STM32F3)

namespace hal
{
    namespace
    {

#if defined(STM32F7) || defined(STM32F4)

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
        const uint32_t streamToHTIF[8] = { DMA_FLAG_HTIF0_4, DMA_FLAG_HTIF1_5, DMA_FLAG_HTIF2_6, DMA_FLAG_HTIF3_7, DMA_FLAG_HTIF0_4, DMA_FLAG_HTIF1_5, DMA_FLAG_HTIF2_6, DMA_FLAG_HTIF3_7 };
        const uint32_t streamToTEIF[8] = { DMA_FLAG_TEIF0_4, DMA_FLAG_TEIF1_5, DMA_FLAG_TEIF2_6, DMA_FLAG_TEIF3_7, DMA_FLAG_TEIF0_4, DMA_FLAG_TEIF1_5, DMA_FLAG_TEIF2_6, DMA_FLAG_TEIF3_7 };

#elif defined(STM32WB) || defined(STM32G4)
        volatile uint32_t* const dmaISR[2][7] = {
            { &DMA1->ISR, &DMA1->ISR, &DMA1->ISR, &DMA1->ISR, &DMA1->ISR, &DMA1->ISR, &DMA1->ISR },
            { &DMA2->ISR, &DMA2->ISR, &DMA2->ISR, &DMA2->ISR, &DMA2->ISR, &DMA2->ISR, &DMA2->ISR }
        };

        volatile uint32_t* const dmaIFCR[2][7] = {
            { &DMA1->IFCR, &DMA1->IFCR, &DMA1->IFCR, &DMA1->IFCR, &DMA1->IFCR, &DMA1->IFCR, &DMA1->IFCR },
            { &DMA2->IFCR, &DMA2->IFCR, &DMA2->IFCR, &DMA2->IFCR, &DMA2->IFCR, &DMA2->IFCR, &DMA2->IFCR }
        };
#endif

#if defined(STM32WB)
        DMA_Channel_TypeDef* const dmaStream[2][7] = {
            { DMA1_Channel1, DMA1_Channel2, DMA1_Channel3, DMA1_Channel4, DMA1_Channel5, DMA1_Channel6, DMA1_Channel7 },
            { DMA2_Channel1, DMA2_Channel2, DMA2_Channel3, DMA2_Channel4, DMA2_Channel5, DMA2_Channel6, DMA2_Channel7 }
        };
        const IRQn_Type dmaIrq[2][7] = {
            { DMA1_Channel1_IRQn, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn, DMA1_Channel4_IRQn, DMA1_Channel5_IRQn, DMA1_Channel6_IRQn, DMA1_Channel7_IRQn },
            { DMA2_Channel1_IRQn, DMA2_Channel2_IRQn, DMA2_Channel3_IRQn, DMA2_Channel4_IRQn, DMA2_Channel5_IRQn, DMA2_Channel6_IRQn, DMA2_Channel7_IRQn }
        };

        const uint32_t streamToTCIF[7] = { DMA_FLAG_TC1, DMA_FLAG_TC2, DMA_FLAG_TC3, DMA_FLAG_TC4, DMA_FLAG_TC5, DMA_FLAG_TC6, DMA_FLAG_TC7 };
        const uint32_t streamToHTIF[7] = { DMA_FLAG_HT1, DMA_FLAG_HT2, DMA_FLAG_HT3, DMA_FLAG_HT4, DMA_FLAG_HT5, DMA_FLAG_HT6, DMA_FLAG_HT7 };
        const uint32_t streamToTEIF[7] = { DMA_FLAG_TE1, DMA_FLAG_TE2, DMA_FLAG_TE3, DMA_FLAG_TE4, DMA_FLAG_TE5, DMA_FLAG_TE6, DMA_FLAG_TE7 };
#elif defined(STM32G4)
        DMA_Channel_TypeDef* const dmaStream[2][6] = {
            { DMA1_Channel1, DMA1_Channel2, DMA1_Channel3, DMA1_Channel4, DMA1_Channel5, DMA1_Channel6 },
            { DMA2_Channel1, DMA2_Channel2, DMA2_Channel3, DMA2_Channel4, DMA2_Channel5, DMA2_Channel6 }
        };

        const IRQn_Type dmaIrq[2][7] = {
            { DMA1_Channel1_IRQn, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn, DMA1_Channel4_IRQn, DMA1_Channel5_IRQn, DMA1_Channel6_IRQn },
            { DMA2_Channel1_IRQn, DMA2_Channel2_IRQn, DMA2_Channel3_IRQn, DMA2_Channel4_IRQn, DMA2_Channel5_IRQn, DMA2_Channel6_IRQn }
        };

        const uint32_t streamToTCIF[7] = { DMA_FLAG_TC1, DMA_FLAG_TC2, DMA_FLAG_TC3, DMA_FLAG_TC4, DMA_FLAG_TC5, DMA_FLAG_TC6 };
        const uint32_t streamToHTIF[7] = { DMA_FLAG_HT1, DMA_FLAG_HT2, DMA_FLAG_HT3, DMA_FLAG_HT4, DMA_FLAG_HT5, DMA_FLAG_HT6 };
        const uint32_t streamToTEIF[7] = { DMA_FLAG_TE1, DMA_FLAG_TE2, DMA_FLAG_TE3, DMA_FLAG_TE4, DMA_FLAG_TE5, DMA_FLAG_TE6 };
#endif

        uint32_t dummy;
    }

    DmaStm::DmaStm()
    {
        __DMA1_CLK_ENABLE();
        __DMA2_CLK_ENABLE();
#if defined(STM32WB) || defined(STM32G4)
        __HAL_RCC_DMAMUX1_CLK_ENABLE();
#endif
    }

    DmaStm::~DmaStm()
    {
        __DMA1_CLK_DISABLE();
        __DMA2_CLK_DISABLE();
#if defined(STM32WB) || defined(STM32G4)
        __HAL_RCC_DMAMUX1_CLK_DISABLE();
#endif
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

    DmaStm::StreamBase::StreamBase(DmaStm& dma, DmaChannelId channelId)
        : dma(dma)
        , dmaIndex(channelId.dma)
        , streamIndex(channelId.stream)
    {
        dma.ReserveStream(dmaIndex, streamIndex);
    }

    DmaStm::StreamBase::~StreamBase()
    {
        if (streamIndex != 0xff)
            dma.ReleaseStream(dmaIndex, streamIndex);
    }

    DmaStm::StreamBase::StreamBase(StreamBase&& other)
        : dma(other.dma)
        , dmaIndex(other.dmaIndex)
        , streamIndex(other.streamIndex)
    {
        other.streamIndex = 0xff;
    }

    DmaStm::StreamBase& DmaStm::StreamBase::operator=(StreamBase&& other)
    {
        dmaIndex = other.dmaIndex;
        streamIndex = other.streamIndex;

        other.streamIndex = 0xff;

        return *this;
    }

    uint8_t DmaStm::StreamBase::DataSize() const
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        return 1 << ((streamRegister->CR & DMA_SxCR_PSIZE) >> POSITION_VAL(DMA_SxCR_PSIZE));
#else
        return 1 << ((streamRegister->CCR & DMA_CCR_PSIZE) >> POSITION_VAL(DMA_CCR_PSIZE));
#endif
    }

    void DmaStm::StreamBase::SetDataSize(uint8_t dataSizeInBytes)
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR = (streamRegister->CR & ~DMA_SxCR_PSIZE) | ((dataSizeInBytes & 1) == 0 ? DMA_SxCR_PSIZE_0 : 0) | (dataSizeInBytes > 2 ? DMA_SxCR_PSIZE_1 : 0);
#else
        streamRegister->CCR = (streamRegister->CCR & ~DMA_CCR_PSIZE) | ((dataSizeInBytes & 1) == 0 ? DMA_CCR_PSIZE_0 : 0) | (dataSizeInBytes > 2 ? DMA_CCR_PSIZE_1 : 0);
#endif
    }

    bool DmaStm::StreamBase::StopTransfer()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
        bool finished = Finished();
        DisableHalfTransferCompleteInterrupt();
        DisableTransferCompleteInterrupt();
        Disable();
        *dmaIFCR[dmaIndex][streamIndex] |= streamToTCIF[streamIndex] | streamToHTIF[streamIndex];
        return !finished;
    }

    void DmaStm::StreamBase::Disable()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_SxCR_EN;
#else
        streamRegister->CCR &= ~DMA_CCR_EN;
#endif
    }

    void DmaStm::StreamBase::DisableHalfTransferCompleteInterrupt()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_IT_HT;
#else
        streamRegister->CCR &= ~DMA_IT_HT;
#endif
    }

    void DmaStm::StreamBase::DisableMemoryIncrement()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_MINC_ENABLE;
#else
        streamRegister->CCR &= ~DMA_MINC_ENABLE;
#endif
    }

    void DmaStm::StreamBase::DisableTransferCompleteInterrupt()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_IT_TC;
#else
        streamRegister->CCR &= ~DMA_IT_TC;
#endif
    }

    void DmaStm::StreamBase::Enable()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_SxCR_EN;
#else
        streamRegister->CCR |= DMA_CCR_EN;
#endif
    }

    void DmaStm::StreamBase::EnableHalfTransferCompleteInterrupt()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_IT_HT;
#else
        streamRegister->CCR |= DMA_IT_HT;
#endif
    }

    void DmaStm::StreamBase::EnableMemoryIncrement()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_MINC_ENABLE;
#else
        streamRegister->CCR |= DMA_MINC_ENABLE;
#endif
    }

    void DmaStm::StreamBase::EnableTransferCompleteInterrupt()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_IT_TC;
#else
        streamRegister->CCR |= DMA_IT_TC;
#endif
    }

    bool DmaStm::StreamBase::Finished() const
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        return (streamRegister->CR & DMA_SxCR_EN) == 0;
#else
        return (streamRegister->CCR & DMA_CCR_EN) == 0;

#endif
    }

    void DmaStm::StreamBase::SetMemoryAddress(const void* memoryAddress)
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->M0AR = reinterpret_cast<uint32_t>(memoryAddress);
#else
        streamRegister->CMAR = reinterpret_cast<uint32_t>(memoryAddress);
#endif
    }

    void DmaStm::StreamBase::SetMemoryToPeripheralMode()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_MEMORY_TO_PERIPH;
#else
        streamRegister->CCR |= DMA_MEMORY_TO_PERIPH;
#endif
    }

    void DmaStm::StreamBase::SetPeripheralAddress(volatile void* peripheralAddress)
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->PAR = reinterpret_cast<uint32_t>(peripheralAddress);
#else
        streamRegister->CPAR = reinterpret_cast<uint32_t>(peripheralAddress);
#endif
    }

    void DmaStm::StreamBase::SetPeripheralToMemoryMode()
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_MEMORY_TO_PERIPH;
#else
        streamRegister->CCR &= ~DMA_MEMORY_TO_PERIPH;
#endif
    }

    void DmaStm::StreamBase::SetTransferSize(uint16_t size)
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->NDTR = size / DataSize();
#else
        streamRegister->CNDTR = size / DataSize();
#endif
    }

    DmaStm::Stream::Stream(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress, infra::Function<void()> actionOnTransferComplete)
        : StreamBase(dma, channelId)
        , actionOnTransferComplete(actionOnTransferComplete)
        , interruptHandler(hal::dmaIrq[dmaIndex][streamIndex], [this]()
              {
                  OnInterrupt();
              })
    {
        DMA_HandleTypeDef dmaStreamHandle = {};

        dmaStreamHandle.Instance = dmaStream[dmaIndex][streamIndex];

        dmaStreamHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
        dmaStreamHandle.Init.PeriphInc = DMA_PINC_DISABLE;
        dmaStreamHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        dmaStreamHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        dmaStreamHandle.Init.Mode = DMA_NORMAL;
        dmaStreamHandle.Init.Priority = DMA_PRIORITY_MEDIUM;
#if defined(STM32F7) || defined(STM32F4)
        dmaStreamHandle.Init.Channel = dmaChannel[channelId.channel];
        dmaStreamHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        dmaStreamHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        dmaStreamHandle.Init.MemBurst = DMA_MBURST_SINGLE;
        dmaStreamHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;
#else
        dmaStreamHandle.Init.Request = channelId.channel;
#endif

        HAL_DMA_Init(&dmaStreamHandle);
        SetPeripheralAddress(peripheralAddress);
    }

    DmaStm::Stream::Stream(Stream&& other)
        : StreamBase(std::move(other))
        , interruptHandler(std::move(other.interruptHandler), [this]()
              {
                  OnInterrupt();
              })
        , actionOnTransferComplete(other.actionOnTransferComplete)
    {
        other.actionOnTransferComplete = nullptr;
    }

    DmaStm::Stream& DmaStm::Stream::operator=(Stream&& other)
    {
        StreamBase::operator=(std::move(other));

        actionOnTransferComplete = other.actionOnTransferComplete;
        interruptHandler.Assign(std::move(other.interruptHandler), [this]()
            {
                OnInterrupt();
            });

        other.actionOnTransferComplete = nullptr;

        return *this;
    }

    void DmaStm::Stream::StartTransmit(infra::ConstByteRange data)
    {
        assert(data.size() <= 65535);
        assert(!data.empty());
        __DMB();

        SetMemoryToPeripheralMode();
        EnableMemoryIncrement();
        SetTransferSize(data.size());
        SetMemoryAddress(data.begin());
        EnableTransferCompleteInterrupt();
        Enable();
    }

    void DmaStm::Stream::StartTransmitDummy(uint16_t size)
    {
        DisableMemoryIncrement();
        SetMemoryToPeripheralMode();
        SetTransferSize(size);
        SetMemoryAddress(&dummy);
        EnableTransferCompleteInterrupt();
        Enable();
    }

    void DmaStm::Stream::StartReceive(infra::ByteRange data)
    {
        assert(data.size() <= 65535);
        SetPeripheralToMemoryMode();
        EnableMemoryIncrement();
        SetTransferSize(data.size());
        SetMemoryAddress(data.begin());
        EnableTransferCompleteInterrupt();
        Enable();
    }

    void DmaStm::Stream::StartReceiveDummy(uint16_t size)
    {
        SetPeripheralToMemoryMode();
        DisableMemoryIncrement();
        SetTransferSize(size);
        SetMemoryAddress(&dummy);
        EnableTransferCompleteInterrupt();
        Enable();
    }

    void DmaStm::Stream::OnInterrupt()
    {
        if (*dmaISR[dmaIndex][streamIndex] & streamToTCIF[streamIndex])
        {
            *dmaIFCR[dmaIndex][streamIndex] |= streamToTCIF[streamIndex];
            Disable();

            __DMB();

            interruptHandler.ClearPending();
            actionOnTransferComplete();
        }
    }

    DmaStm::CircularStream::CircularStream(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress, infra::Function<void()> actionOnFirstHalfDone, infra::Function<void()> actionOnSecondHalfDone)
        : StreamBase(dma, channelId)
        , actionOnFirstHalfDone(actionOnFirstHalfDone)
        , actionOnSecondHalfDone(actionOnSecondHalfDone)
        , interruptHandler(hal::dmaIrq[dmaIndex][streamIndex], [this]()
              {
                  OnInterrupt();
              })
    {
        DMA_HandleTypeDef dmaStreamHandle = {};

        dmaStreamHandle.Instance = dmaStream[dmaIndex][streamIndex];

        dmaStreamHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
        dmaStreamHandle.Init.PeriphInc = DMA_PINC_DISABLE;
        dmaStreamHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        dmaStreamHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        dmaStreamHandle.Init.Mode = DMA_CIRCULAR;
        dmaStreamHandle.Init.Priority = DMA_PRIORITY_MEDIUM;
#if defined(STM32F7) || defined(STM32F4)
        dmaStreamHandle.Init.Channel = dmaChannel[channelId.channel];
        dmaStreamHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        dmaStreamHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        dmaStreamHandle.Init.MemBurst = DMA_MBURST_SINGLE;
        dmaStreamHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;
#else
        dmaStreamHandle.Init.Request = channelId.channel;
#endif

        HAL_DMA_Init(&dmaStreamHandle);
        SetPeripheralAddress(peripheralAddress);
    }

    size_t DmaStm::CircularStream::ReceivedSize() const
    {
        auto streamRegister = dmaStream[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        size_t bytesToTransfer = streamRegister->NDTR * DataSize();
#else
        size_t bytesToTransfer = streamRegister->CNDTR * DataSize();
#endif

        return data.size() - bytesToTransfer;
    }

    void DmaStm::CircularStream::StartReceive(infra::ByteRange data)
    {
        assert(data.size() <= 65535);

        this->data = data;

        SetPeripheralToMemoryMode();
        EnableMemoryIncrement();
        SetTransferSize(data.size());
        SetMemoryAddress(data.begin());
        EnableTransferCompleteInterrupt();
        EnableHalfTransferCompleteInterrupt();
        Enable();
    }

    void DmaStm::CircularStream::OnInterrupt()
    {
        if (*dmaISR[dmaIndex][streamIndex] & streamToTCIF[streamIndex])
        {
            *dmaIFCR[dmaIndex][streamIndex] |= streamToTCIF[streamIndex];

            interruptHandler.ClearPending();
            actionOnSecondHalfDone();
        }
        else if (*dmaISR[dmaIndex][streamIndex] & streamToHTIF[streamIndex])
        {
            *dmaIFCR[dmaIndex][streamIndex] |= streamToHTIF[streamIndex];

            interruptHandler.ClearPending();
            actionOnFirstHalfDone();
        }
    }
}

#endif
