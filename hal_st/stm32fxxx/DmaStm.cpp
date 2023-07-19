#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "infra/util/ByteRange.hpp"

#if !defined(STM32F0) && !defined(STM32F1) && !defined(STM32F3)

namespace hal
{
    namespace
    {
#if defined(DMA_STREAM_BASED)
        const std::array dmaISR{
            std::array{ &DMA1->LISR, &DMA1->LISR, &DMA1->LISR, &DMA1->LISR, &DMA1->HISR, &DMA1->HISR, &DMA1->HISR, &DMA1->HISR },
            std::array{ &DMA2->LISR, &DMA2->LISR, &DMA2->LISR, &DMA2->LISR, &DMA2->HISR, &DMA2->HISR, &DMA2->HISR, &DMA2->HISR },
        };

        const std::array dmaIFCR{
            std::array{ &DMA1->LIFCR, &DMA1->LIFCR, &DMA1->LIFCR, &DMA1->LIFCR, &DMA1->HIFCR, &DMA1->HIFCR, &DMA1->HIFCR, &DMA1->HIFCR },
            std::array{ &DMA2->LIFCR, &DMA2->LIFCR, &DMA2->LIFCR, &DMA2->LIFCR, &DMA2->HIFCR, &DMA2->HIFCR, &DMA2->HIFCR, &DMA2->HIFCR },
        };

        const std::arrayDmaChannel{
            std::array{ DMA1_Stream0, DMA1_Stream1, DMA1_Stream2, DMA1_Stream3, DMA1_Stream4, DMA1_Stream5, DMA1_Stream6, DMA1_Stream7 },
            std::array{ DMA2_Stream0, DMA2_Stream1, DMA2_Stream2, DMA2_Stream3, DMA2_Stream4, DMA2_Stream5, DMA2_Stream6, DMA2_Stream7 },
        };

        const std::array dmaIrq{
            std::array{ DMA1_Stream0_IRQn, DMA1_Stream1_IRQn, DMA1_Stream2_IRQn, DMA1_Stream3_IRQn, DMA1_Stream4_IRQn, DMA1_Stream5_IRQn, DMA1_Stream6_IRQn, DMA1_Stream7_IRQn },
            std::array{ DMA2_Stream0_IRQn, DMA2_Stream1_IRQn, DMA2_Stream2_IRQn, DMA2_Stream3_IRQn, DMA2_Stream4_IRQn, DMA2_Stream5_IRQn, DMA2_Stream6_IRQn, DMA2_Stream7_IRQn },
        };

        const std::array dmaChannel
        {
            DMA_CHANNEL_0,
                DMA_CHANNEL_1,
                DMA_CHANNEL_2,
                DMA_CHANNEL_3,
                DMA_CHANNEL_4,
                DMA_CHANNEL_5,
                DMA_CHANNEL_6,
                DMA_CHANNEL_7,
#if defined(STM32F765xx) || defined(STM32F767xx) || defined(STM32F769xx) || defined(STM32F777xx) || defined(STM32F779xx)
                DMA_CHANNEL_8,
                DMA_CHANNEL_9,
                DMA_CHANNEL_10,
                DMA_CHANNEL_11,
                DMA_CHANNEL_12,
                DMA_CHANNEL_13,
                DMA_CHANNEL_14,
                DMA_CHANNEL_15,
#endif
        };

        const std::array streamToTCIF{ DMA_FLAG_TCIF0_4, DMA_FLAG_TCIF1_5, DMA_FLAG_TCIF2_6, DMA_FLAG_TCIF3_7, DMA_FLAG_TCIF0_4, DMA_FLAG_TCIF1_5, DMA_FLAG_TCIF2_6, DMA_FLAG_TCIF3_7 };
        const std::array streamToHTIF{ DMA_FLAG_HTIF0_4, DMA_FLAG_HTIF1_5, DMA_FLAG_HTIF2_6, DMA_FLAG_HTIF3_7, DMA_FLAG_HTIF0_4, DMA_FLAG_HTIF1_5, DMA_FLAG_HTIF2_6, DMA_FLAG_HTIF3_7 };
        const std::array streamToTEIF{ DMA_FLAG_TEIF0_4, DMA_FLAG_TEIF1_5, DMA_FLAG_TEIF2_6, DMA_FLAG_TEIF3_7, DMA_FLAG_TEIF0_4, DMA_FLAG_TEIF1_5, DMA_FLAG_TEIF2_6, DMA_FLAG_TEIF3_7 };
#endif

#if defined(DMA_CHANNEL_BASED)
        const std::array dmaISR
        {
            std::array
            {
                &DMA1->ISR,
                    &DMA1->ISR,
                    &DMA1->ISR,
                    &DMA1->ISR,
                    &DMA1->ISR,
                    &DMA1->ISR,
#if defined(STM32G0) || defined(STM32G4) || defined(STM32WB)
                    &DMA1->ISR,
#endif
#if defined(STM32G4)
                    &DMA1->ISR,
#endif
            }
            ,
#if defined(STM32G4) || defined(STM32WB)
                std::array
            {
                &DMA2->ISR,
                    &DMA2->ISR,
                    &DMA2->ISR,
                    &DMA2->ISR,
                    &DMA2->ISR,
                    &DMA2->ISR,
#if defined(STM32G4) || defined(STM32WB)
                    &DMA2->ISR,
#endif
#if defined(STM32G4)
                    &DMA2->ISR,
#endif
            }
            ,
#endif
        };

        const std::array dmaIFCR
        {
            std::array
            {
                &DMA1->IFCR,
                    &DMA1->IFCR,
                    &DMA1->IFCR,
                    &DMA1->IFCR,
                    &DMA1->IFCR,
                    &DMA1->IFCR,
#if defined(STM32G0) || defined(STM32G4) || defined(STM32WB)
                    &DMA1->IFCR,
#endif
#if defined(STM32G4)
                    &DMA1->IFCR,
#endif
            }
            ,
#if defined(STM32G4) || defined(STM32WB)
                std::array
            {
                &DMA2->IFCR,
                    &DMA2->IFCR,
                    &DMA2->IFCR,
                    &DMA2->IFCR,
                    &DMA2->IFCR,
                    &DMA2->IFCR,
#if defined(STM32G4) || defined(STM32WB)
                    &DMA2->IFCR,
#endif
#if defined(STM32G4)
                    &DMA2->IFCR,
#endif
            }
            ,
#endif
        };

        const std::array DmaChannel
        {
            std::array
            {
                DMA1_Channel1,
                    DMA1_Channel2,
                    DMA1_Channel3,
                    DMA1_Channel4,
                    DMA1_Channel5,
                    DMA1_Channel6,
#if defined(STM32G0) || defined(STM32G4) || defined(STM32WB)
                    DMA1_Channel7,
#endif
#if defined(STM32G4)
                    DMA1_Channel8,
#endif
            }
            ,
#if defined(STM32G4) || defined(STM32WB)
                std::array
            {
                DMA2_Channel1,
                    DMA2_Channel2,
                    DMA2_Channel3,
                    DMA2_Channel4,
                    DMA2_Channel5,
                    DMA2_Channel6,
#if defined(STM32G4) || defined(STM32WB)
                    DMA2_Channel7,
#endif
#if defined(STM32G4)
                    DMA2_Channel8,
#endif
            }
            ,
#endif
        };
        const std::array dmaIrq
        {
            std::array
            {
                DMA1_Channel1_IRQn,
                    DMA1_Channel2_IRQn,
                    DMA1_Channel3_IRQn,
                    DMA1_Channel4_IRQn,
                    DMA1_Channel5_IRQn,
                    DMA1_Channel6_IRQn,
#if defined(STM32G0) || defined(STM32G4) || defined(STM32WB)
                    DMA1_Channel7_IRQn,
#endif
#if defined(STM32G4)
                    DMA1_Channel8_IRQn,
#endif
            }
            ,
#if defined(STM32G4) || defined(STM32WB)
                std::array
            {
                DMA2_Channel1_IRQn,
                    DMA2_Channel2_IRQn,
                    DMA2_Channel3_IRQn,
                    DMA2_Channel4_IRQn,
                    DMA2_Channel5_IRQn,
                    DMA2_Channel6_IRQn,
#if defined(STM32G4) || defined(STM32WB)
                    DMA2_Channel7_IRQn,
#endif
#if defined(STM32G4)
                    DMA2_Channel8_IRQn,
#endif
            }
            ,
#endif
        };

        const std::array streamToTCIF
        {
            DMA_FLAG_TC1,
                DMA_FLAG_TC2,
                DMA_FLAG_TC3,
                DMA_FLAG_TC4,
                DMA_FLAG_TC5,
                DMA_FLAG_TC6,
#if defined(STM32G0) || defined(STM32G4) || defined(STM32WB)
                DMA_FLAG_TC7,
#endif
#if defined(STM32G4)
                DMA_FLAG_TC8,
#endif
        };
        const std::array streamToHTIF
        {
            DMA_FLAG_HT1,
                DMA_FLAG_HT2,
                DMA_FLAG_HT3,
                DMA_FLAG_HT4,
                DMA_FLAG_HT5,
                DMA_FLAG_HT6,
#if defined(STM32G4) || defined(STM32WB)
                DMA_FLAG_HT7,
#endif
#if defined(STM32G4)
                DMA_FLAG_HT8,
#endif
        };
        const std::array streamToTEIF
        {
            DMA_FLAG_TE1,
                DMA_FLAG_TE2,
                DMA_FLAG_TE3,
                DMA_FLAG_TE4,
                DMA_FLAG_TE5,
                DMA_FLAG_TE6,
#if defined(STM32G4) || defined(STM32WB)
                DMA_FLAG_TE7,
#endif
#if defined(STM32G4)
                DMA_FLAG_TE8,
#endif
        };
#endif

        static_assert(dmaISR[0].size() == dmaIFCR[0].size());
        static_assert(dmaISR[0].size() == DmaChannel[0].size());
        static_assert(dmaISR[0].size() == dmaIrq[0].size());
        static_assert(dmaISR[0].size() == streamToTCIF.size());
        static_assert(dmaISR[0].size() == streamToHTIF.size());
        static_assert(dmaISR[0].size() == streamToTEIF.size());

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

#if defined(DMA_STREAM_BASED)
        , streamIndex(channelId.stream)
#else
        , streamIndex(channelId.channel)
#endif
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
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        return 1 << ((streamRegister->CR & DMA_SxCR_MSIZE) >> POSITION_VAL(DMA_SxCR_MSIZE));
#else
        return 1 << ((streamRegister->CCR & DMA_CCR_MSIZE) >> POSITION_VAL(DMA_CCR_MSIZE));
#endif
    }

    void DmaStm::StreamBase::SetDataSize(uint8_t dataSizeInBytes)
    {
        SetPeripheralDataSize(dataSizeInBytes);
        SetMemoryDataSize(dataSizeInBytes);
    }

    void DmaStm::StreamBase::SetPeripheralDataSize(uint8_t dataSizeInBytes)
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR = (streamRegister->CR & ~DMA_SxCR_PSIZE) | ((dataSizeInBytes & 1) == 0 ? DMA_SxCR_PSIZE_0 : 0) | (dataSizeInBytes > 2 ? DMA_SxCR_PSIZE_1 : 0);
#else
        streamRegister->CCR = (streamRegister->CCR & ~DMA_CCR_PSIZE) | ((dataSizeInBytes & 1) == 0 ? DMA_CCR_PSIZE_0 : 0) | (dataSizeInBytes > 2 ? DMA_CCR_PSIZE_1 : 0);
#endif
    }

    void DmaStm::StreamBase::SetMemoryDataSize(uint8_t dataSizeInBytes)
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR = (streamRegister->CR & ~DMA_SxCR_MSIZE) | ((dataSizeInBytes & 1) == 0 ? DMA_SxCR_MSIZE_0 : 0) | (dataSizeInBytes > 2 ? DMA_SxCR_MSIZE_1 : 0);
#else
        streamRegister->CCR = (streamRegister->CCR & ~DMA_CCR_MSIZE) | ((dataSizeInBytes & 1) == 0 ? DMA_CCR_MSIZE_0 : 0) | (dataSizeInBytes > 2 ? DMA_CCR_MSIZE_1 : 0);
#endif
    }

    bool DmaStm::StreamBase::StopTransfer()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
        bool finished = Finished();
        DisableHalfTransferCompleteInterrupt();
        DisableTransferCompleteInterrupt();
        Disable();
        *dmaIFCR[dmaIndex][streamIndex] |= streamToTCIF[streamIndex] | streamToHTIF[streamIndex];
        return !finished;
    }

    void DmaStm::StreamBase::Disable()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_SxCR_EN;
#else
        streamRegister->CCR &= ~DMA_CCR_EN;
#endif
    }

    void DmaStm::StreamBase::DisableHalfTransferCompleteInterrupt()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_IT_HT;
#else
        streamRegister->CCR &= ~DMA_IT_HT;
#endif
    }

    void DmaStm::StreamBase::DisableMemoryIncrement()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_MINC_ENABLE;
#else
        streamRegister->CCR &= ~DMA_MINC_ENABLE;
#endif
    }

    void DmaStm::StreamBase::DisableTransferCompleteInterrupt()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_IT_TC;
#else
        streamRegister->CCR &= ~DMA_IT_TC;
#endif
    }

    void DmaStm::StreamBase::DisableCircularMode()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_SxCR_CIRC;
#else
        streamRegister->CCR &= ~DMA_CCR_CIRC;
#endif
    }

    void DmaStm::StreamBase::Enable()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_SxCR_EN;
#else
        streamRegister->CCR |= DMA_CCR_EN;
#endif
    }

    void DmaStm::StreamBase::EnableHalfTransferCompleteInterrupt()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_IT_HT;
#else
        streamRegister->CCR |= DMA_IT_HT;
#endif
    }

    void DmaStm::StreamBase::EnableMemoryIncrement()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_MINC_ENABLE;
#else
        streamRegister->CCR |= DMA_MINC_ENABLE;
#endif
    }

    void DmaStm::StreamBase::EnableTransferCompleteInterrupt()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_IT_TC;
#else
        streamRegister->CCR |= DMA_IT_TC;
#endif
    }

    void DmaStm::StreamBase::EnableCircularMode()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_SxCR_CIRC;
#else
        streamRegister->CCR |= DMA_CCR_CIRC;
#endif
    }

    bool DmaStm::StreamBase::Finished() const
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        return (streamRegister->CR & DMA_SxCR_EN) == 0;
#else
        return (streamRegister->CCR & DMA_CCR_EN) == 0;

#endif
    }

    void DmaStm::StreamBase::SetMemoryAddress(const void* memoryAddress)
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->M0AR = reinterpret_cast<uint32_t>(memoryAddress);
#else
        streamRegister->CMAR = reinterpret_cast<uint32_t>(memoryAddress);
#endif
    }

    void DmaStm::StreamBase::SetMemoryToPeripheralMode()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR |= DMA_MEMORY_TO_PERIPH;
#else
        streamRegister->CCR |= DMA_MEMORY_TO_PERIPH;
#endif
    }

    void DmaStm::StreamBase::SetPeripheralAddress(volatile void* peripheralAddress)
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->PAR = reinterpret_cast<uint32_t>(peripheralAddress);
#else
        streamRegister->CPAR = reinterpret_cast<uint32_t>(peripheralAddress);
#endif
    }

    void DmaStm::StreamBase::SetPeripheralToMemoryMode()
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->CR &= ~DMA_MEMORY_TO_PERIPH;
#else
        streamRegister->CCR &= ~DMA_MEMORY_TO_PERIPH;
#endif
    }

    void DmaStm::StreamBase::SetTransferSize(uint16_t size)
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
#if defined(STM32F7) || defined(STM32F4)
        streamRegister->NDTR = size / DataSize();
#else
        streamRegister->CNDTR = size / DataSize();
#endif
    }

    bool DmaStm::StreamBase::IsHalfComplete() const
    {
        return *dmaISR[dmaIndex][streamIndex] & streamToHTIF[streamIndex];
    }

    bool DmaStm::StreamBase::IsFullComplete() const
    {
        return *dmaISR[dmaIndex][streamIndex] & streamToTCIF[streamIndex];
    }

    void DmaStm::StreamBase::ClearHalfComplete() const
    {
        *dmaIFCR[dmaIndex][streamIndex] |= streamToHTIF[streamIndex];
    }

    void DmaStm::StreamBase::ClearFullComplete() const
    {
        *dmaIFCR[dmaIndex][streamIndex] |= streamToTCIF[streamIndex];
    }

    DmaStm::Stream::Stream(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress, infra::Function<void()> actionOnTransferComplete)
        : StreamBase(dma, channelId)
        , actionOnTransferComplete(actionOnTransferComplete)
        , interruptHandler(hal::dmaIrq[dmaIndex][streamIndex], [this]()
              {
                  OnInterrupt();
              })
    {
        DMA_HandleTypeDef DmaChannelHandle = {};

        DmaChannelHandle.Instance = DmaChannel[dmaIndex][streamIndex];

        DmaChannelHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
        DmaChannelHandle.Init.PeriphInc = DMA_PINC_DISABLE;
        DmaChannelHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        DmaChannelHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        DmaChannelHandle.Init.Mode = DMA_NORMAL;
        DmaChannelHandle.Init.Priority = DMA_PRIORITY_MEDIUM;
#if defined(DMA_STREAM_BASED)
        DmaChannelHandle.Init.Channel = dmaChannel[channelId.channel];
        DmaChannelHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        DmaChannelHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        DmaChannelHandle.Init.MemBurst = DMA_MBURST_SINGLE;
        DmaChannelHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;
#else
        DmaChannelHandle.Init.Request = channelId.mux;
#endif

        HAL_DMA_Init(&DmaChannelHandle);
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
        DMA_HandleTypeDef DmaChannelHandle = {};

        DmaChannelHandle.Instance = DmaChannel[dmaIndex][streamIndex];

        DmaChannelHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
        DmaChannelHandle.Init.PeriphInc = DMA_PINC_DISABLE;
        DmaChannelHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        DmaChannelHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        DmaChannelHandle.Init.Mode = DMA_CIRCULAR;
        DmaChannelHandle.Init.Priority = DMA_PRIORITY_MEDIUM;
#if defined(DMA_STREAM_BASED)
        DmaChannelHandle.Init.Channel = dmaChannel[channelId.channel];
        DmaChannelHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        DmaChannelHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        DmaChannelHandle.Init.MemBurst = DMA_MBURST_SINGLE;
        DmaChannelHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;
#else
        DmaChannelHandle.Init.Request = channelId.mux;
#endif

        HAL_DMA_Init(&DmaChannelHandle);
        SetPeripheralAddress(peripheralAddress);
    }

    size_t DmaStm::CircularStream::ReceivedSize() const
    {
        auto streamRegister = DmaChannel[dmaIndex][streamIndex];
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

    /* ======================================================= */
    /* ======================================================= */
    /* ======================================================= */
    /* ======================================================= */
    /* ======================================================= */
    /* ======================================================= */
    /* ======================================================= */

    DmaStm::TransceiverStreamBase::TransceiverStreamBase(DmaStm& dma, DmaChannelId channelId)
        : StreamBase(dma, channelId)
    {
        DMA_HandleTypeDef DmaChannelHandle = {};

        DmaChannelHandle.Instance = DmaChannel[dmaIndex][streamIndex];

        DmaChannelHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
        DmaChannelHandle.Init.PeriphInc = DMA_PINC_DISABLE;
        DmaChannelHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        DmaChannelHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        DmaChannelHandle.Init.Mode = DMA_NORMAL;
        DmaChannelHandle.Init.Priority = DMA_PRIORITY_MEDIUM;
#if defined(DMA_STREAM_BASED)
        DmaChannelHandle.Init.Channel = dmaChannel[channelId.channel];
        DmaChannelHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        DmaChannelHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        DmaChannelHandle.Init.MemBurst = DMA_MBURST_SINGLE;
        DmaChannelHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;
#else
        DmaChannelHandle.Init.Request = channelId.mux;
#endif

        HAL_DMA_Init(&DmaChannelHandle);
    }

    DmaStm::TransmitStreamBase::TransmitStreamBase(DmaStm& dma, DmaChannelId channelId)
        : TransceiverStreamBase{ dma, channelId }
    {
        SetMemoryToPeripheralMode();
    }

    void DmaStm::TransmitStreamBase::StartTransmit(infra::ConstByteRange data)
    {
        EnableMemoryIncrement();
        SetTransferSize(data.size());
        SetMemoryAddress(data.begin());
        Enable();
    }

    void DmaStm::TransmitStreamBase::StartTransmitDummy(uint16_t size)
    {
        DisableMemoryIncrement();
        SetTransferSize(size);
        SetMemoryAddress(&dummy);
        Enable();
    }

    DmaStm::ReceiveStreamBase::ReceiveStreamBase(DmaStm& dma, DmaChannelId channelId)
        : TransceiverStreamBase{ dma, channelId }
    {
        SetPeripheralToMemoryMode();
    }

    void DmaStm::ReceiveStreamBase::StartReceive(infra::ByteRange data)
    {
        this->data = data;

        EnableMemoryIncrement();
        SetTransferSize(data.size());
        SetMemoryAddress(data.begin());
        Enable();
    }

    void DmaStm::ReceiveStreamBase::StartReceiveDummy(uint16_t size)
    {
        DisableMemoryIncrement();
        SetTransferSize(size);
        SetMemoryAddress(&dummy);
        Enable();
    }

    DmaStm::StreamInterruptHandler::StreamInterruptHandler(StreamBase& streamBase, const infra::Function<void()>& transferFullComplete)
        : streamBase{ streamBase }
        , dispatchedInterruptHandler{ IRQn_Type::DMA1_Channel1_IRQn, [this]
            {
                OnInterrupt();
            } }
        , transferFullComplete{ transferFullComplete }
    {
        streamBase.DisableCircularMode();
        streamBase.EnableTransferCompleteInterrupt();
    }

    void DmaStm::StreamInterruptHandler::OnInterrupt()
    {
        if (streamBase.IsFullComplete())
        {
            streamBase.ClearFullComplete();

            streamBase.Disable();

            __DMB();

            dispatchedInterruptHandler.ClearPending();
            transferFullComplete();
        }
    }

    DmaStm::CircularStreamInterruptHandler::CircularStreamInterruptHandler(StreamBase& streamBase, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete)
        : streamBase{ streamBase }
        , immediateInterruptHandler{ IRQn_Type::DMA1_Channel1_IRQn, [this]
            {
                OnInterrupt();
            } }
        , transferHalfComplete{ transferHalfComplete }
        , transferFullComplete{ transferFullComplete }
    {
        streamBase.EnableCircularMode();
        streamBase.EnableHalfTransferCompleteInterrupt();
        streamBase.EnableTransferCompleteInterrupt();
    }

    void DmaStm::CircularStreamInterruptHandler::OnInterrupt()
    {
        if (streamBase.IsHalfComplete())
        {
            streamBase.ClearHalfComplete();

            immediateInterruptHandler.ClearPending();

            transferHalfComplete();
        }

        if (streamBase.IsFullComplete())
        {
            streamBase.ClearFullComplete();

            immediateInterruptHandler.ClearPending();

            transferFullComplete();
        }
    }

    DmaStm::PeripheralStream::PeripheralStream(StreamBase& streamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize)
    {
        streamBase.SetPeripheralAddress(peripheralAddress);
        streamBase.SetPeripheralDataSize(peripheralTransferSize);
    }

    DmaStm::PeripheralTransmitStream::PeripheralTransmitStream(TransmitStreamBase& transmitStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize)
        : PeripheralStream{ transmitStreamBase, peripheralAddress, peripheralTransferSize }
        , transmitStreamBase{ transmitStreamBase }
    {}

    void DmaStm::PeripheralTransmitStream::StartTransmit(infra::MemoryRange<const uint8_t> data)
    {
        transmitStreamBase.SetMemoryDataSize(1);
        transmitStreamBase.StartTransmit(data);
    }

    void DmaStm::PeripheralTransmitStream::StartTransmit(infra::MemoryRange<const uint16_t> data)
    {
        transmitStreamBase.SetMemoryDataSize(2);
        transmitStreamBase.StartTransmit(infra::ReinterpretCastByteRange(data));
    }

    void DmaStm::PeripheralTransmitStream::StartTransmit(infra::MemoryRange<const uint32_t> data)
    {
        transmitStreamBase.SetMemoryDataSize(4);
        transmitStreamBase.StartTransmit(infra::ReinterpretCastByteRange(data));
    }

    void DmaStm::PeripheralTransmitStream::StartTransmitDummy(uint16_t size, uint8_t dataSize)
    {
        transmitStreamBase.SetMemoryDataSize(dataSize);
        transmitStreamBase.StartTransmitDummy(size);
    }

    DmaStm::PeripheralReceiveStream::PeripheralReceiveStream(ReceiveStreamBase& receiveStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize)
        : PeripheralStream{ receiveStreamBase, peripheralAddress, peripheralTransferSize }
        , receiveStreamBase{ receiveStreamBase }
    {}

    void DmaStm::PeripheralReceiveStream::StartReceive(infra::MemoryRange<uint8_t> data)
    {
        receiveStreamBase.SetMemoryDataSize(1);
        receiveStreamBase.StartReceive(data);
    }

    void DmaStm::PeripheralReceiveStream::StartReceive(infra::MemoryRange<uint16_t> data)
    {
        receiveStreamBase.SetMemoryDataSize(2);
        receiveStreamBase.StartReceive(infra::ReinterpretCastByteRange(data));
    }

    void DmaStm::PeripheralReceiveStream::StartReceive(infra::MemoryRange<uint32_t> data)
    {
        receiveStreamBase.SetMemoryDataSize(4);
        receiveStreamBase.StartReceive(infra::ReinterpretCastByteRange(data));
    }

    void DmaStm::PeripheralReceiveStream::StartReceiveDummy(uint16_t size, uint8_t dataSize)
    {
        receiveStreamBase.SetMemoryDataSize(dataSize);
        receiveStreamBase.StartReceiveDummy(size);
    }

    TransmitDmaChannel::TransmitDmaChannel(DmaStm::TransmitStreamBase& transmitStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete)
        : peripheralStream{ transmitStreamBase, peripheralAddress, peripheralTransferSize }
        , streamInterruptHandler{ transmitStreamBase, transferFullComplete }
    {
    }

    void TransmitDmaChannel::StartTransmit(infra::MemoryRange<const uint8_t> data)
    {
        peripheralStream.StartTransmit(data);
    }

    void TransmitDmaChannel::StartTransmit(infra::MemoryRange<const uint16_t> data)
    {
        peripheralStream.StartTransmit(data);
    }

    void TransmitDmaChannel::StartTransmit(infra::MemoryRange<const uint32_t> data)
    {
        peripheralStream.StartTransmit(data);
    }

    void TransmitDmaChannel::StartTransmitDummy(uint16_t size, uint8_t dataSize)
    {
        peripheralStream.StartTransmitDummy(size, dataSize);
    }

    CircularTransmitDmaChannel::CircularTransmitDmaChannel(DmaStm::TransmitStreamBase& transmitStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete)
        : peripheralStream{ transmitStreamBase, peripheralAddress, peripheralTransferSize }
        , circularStreamInterruptHandler{ transmitStreamBase, transferHalfComplete, transferFullComplete }
    {
    }

    void CircularTransmitDmaChannel::StartTransmit(infra::MemoryRange<const uint8_t> data)
    {
        peripheralStream.StartTransmit(data);
    }

    void CircularTransmitDmaChannel::StartTransmit(infra::MemoryRange<const uint16_t> data)
    {
        peripheralStream.StartTransmit(data);
    }

    void CircularTransmitDmaChannel::StartTransmit(infra::MemoryRange<const uint32_t> data)
    {
        peripheralStream.StartTransmit(data);
    }

    ReceiveDmaChannel::ReceiveDmaChannel(DmaStm::ReceiveStreamBase& receiveStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete)
        : peripheralStream{ receiveStreamBase, peripheralAddress, peripheralTransferSize }
        , streamInterruptHandler{ receiveStreamBase, transferFullComplete }
    {}

    void ReceiveDmaChannel::StartReceive(infra::MemoryRange<uint8_t> data)
    {
        peripheralStream.StartReceive(data);
    }

    void ReceiveDmaChannel::StartReceive(infra::MemoryRange<uint16_t> data)
    {
        peripheralStream.StartReceive(data);
    }

    void ReceiveDmaChannel::StartReceive(infra::MemoryRange<uint32_t> data)
    {
        peripheralStream.StartReceive(data);
    }

    void ReceiveDmaChannel::StartReceiveDummy(uint16_t size, uint8_t dataSize)
    {
        peripheralStream.StartReceiveDummy(size, dataSize);
    }

    CircularReceiveDmaChannel::CircularReceiveDmaChannel(DmaStm::ReceiveStreamBase& receiveStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete)
        : peripheralStream{ receiveStreamBase, peripheralAddress, peripheralTransferSize }
        , circularStreamInterruptHandler{ receiveStreamBase, transferHalfComplete, transferFullComplete }
    {}

    void CircularReceiveDmaChannel::StartReceive(infra::MemoryRange<uint8_t> data)
    {
        peripheralStream.StartReceive(data);
    }

    void CircularReceiveDmaChannel::StartReceive(infra::MemoryRange<uint16_t> data)
    {
        peripheralStream.StartReceive(data);
    }

    void CircularReceiveDmaChannel::StartReceive(infra::MemoryRange<uint32_t> data)
    {
        peripheralStream.StartReceive(data);
    }
}

#endif
