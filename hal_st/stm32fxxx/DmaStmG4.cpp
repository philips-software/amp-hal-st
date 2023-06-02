#include "hal_st/stm32fxxx/DmaStmG4.hpp"
#include DEVICE_HEADER
#include "stm32g4xx_ll_dma.h"

namespace hal
{
    namespace
    {
#if defined(STM32G474xx)
        const std::array dmaISR{ &DMA1->ISR, &DMA2->ISR };
        const std::array dmaIFCR{ &DMA1->IFCR, &DMA2->IFCR };
#endif

#if defined(STM32G474xx)
        const std::array dmaPeripheral{ DMA1, DMA2 };

        const std::array dmaIrq = {
            std::array{
                DMA1_Channel1_IRQn,
                DMA1_Channel2_IRQn,
                DMA1_Channel3_IRQn,
                DMA1_Channel4_IRQn,
                DMA1_Channel5_IRQn,
                DMA1_Channel6_IRQn,
                DMA1_Channel7_IRQn,
                DMA1_Channel8_IRQn },
            std::array{
                DMA2_Channel1_IRQn,
                DMA2_Channel2_IRQn,
                DMA2_Channel3_IRQn,
                DMA2_Channel4_IRQn,
                DMA2_Channel5_IRQn,
                DMA2_Channel6_IRQn,
                DMA2_Channel7_IRQn,
                DMA2_Channel8_IRQn }
        };

        const std::array streamToTCIF{
            DMA_FLAG_TC1,
            DMA_FLAG_TC2,
            DMA_FLAG_TC3,
            DMA_FLAG_TC4,
            DMA_FLAG_TC5,
            DMA_FLAG_TC6,
            DMA_FLAG_TC7,
            DMA_FLAG_TC8
        };

        const std::array streamToHTIF{
            DMA_FLAG_HT1,
            DMA_FLAG_HT2,
            DMA_FLAG_HT3,
            DMA_FLAG_HT4,
            DMA_FLAG_HT5,
            DMA_FLAG_HT6,
            DMA_FLAG_HT7,
            DMA_FLAG_HT8
        };
#endif

        uint32_t dummy;
    }

    DmaStmG4::DmaStmG4()
    {
        __DMA1_CLK_ENABLE();
        __DMA2_CLK_ENABLE();
        __HAL_RCC_DMAMUX1_CLK_ENABLE();
    }

    DmaStmG4::~DmaStmG4()
    {
        __DMA1_CLK_DISABLE();
        __DMA2_CLK_DISABLE();
        __HAL_RCC_DMAMUX1_CLK_DISABLE();
    }

    void DmaStmG4::ReserveStream(uint8_t dmaIndex, uint8_t channelIndex)
    {
        assert((streamAllocation[dmaIndex] & (1 << channelIndex)) == 0);
        streamAllocation[dmaIndex] |= 1 << channelIndex;
    }

    void DmaStmG4::ReleaseStream(uint8_t dmaIndex, uint8_t channelIndex)
    {
        streamAllocation[dmaIndex] &= ~(1 << channelIndex);
    }

    DmaStmG4::StreamBase::StreamBase(DmaStmG4& dma, DmaChannelIdG4 channelId, volatile void* peripheralAddress)
        : dma{ dma }
        , dmaIndex{ channelId.dma }
        , channelIndex{ channelId.channel }
    {
        dma.ReserveStream(dmaIndex, channelIndex);
    }

    DmaStmG4::StreamBase::~StreamBase()
    {
        if (channelIndex != 0xff)
            dma.ReleaseStream(dmaIndex, channelIndex);
    }

    DmaStmG4::StreamBase::StreamBase(StreamBase&& other)
        : dma{ other.dma }
        , channelIndex{ other.channelIndex }
    {
        other.channelIndex = 0xff;
    }

    DmaStmG4::StreamBase& DmaStmG4::StreamBase::operator=(StreamBase&& other)
    {
        dmaIndex = other.dmaIndex;
        channelIndex = other.channelIndex;

        other.channelIndex = 0xff;

        return *this;
    }

    uint8_t DmaStmG4::StreamBase::DataSize() const
    {
        const auto psize = LL_DMA_GetPeriphSize(Peripheral(), channelIndex);
        return 1 << (psize >> DMA_CCR_PSIZE_Pos);
    }

    void DmaStmG4::StreamBase::SetDataSize(uint8_t dataSizeInBytes)
    {
        LL_DMA_SetMemorySize(Peripheral(),
            channelIndex,
            dataSizeInBytes == 1   ? LL_DMA_MDATAALIGN_BYTE
            : dataSizeInBytes == 2 ? LL_DMA_MDATAALIGN_HALFWORD
                                   : LL_DMA_MDATAALIGN_WORD);

        LL_DMA_SetPeriphSize(Peripheral(),
            channelIndex,
            dataSizeInBytes == 1   ? LL_DMA_PDATAALIGN_BYTE
            : dataSizeInBytes == 2 ? LL_DMA_PDATAALIGN_HALFWORD
                                   : LL_DMA_PDATAALIGN_WORD);
    }

    bool DmaStmG4::StreamBase::StopTransfer()
    {
        bool finished = Finished();
        DisableHalfTransferCompleteInterrupt();
        DisableTransferCompleteInterrupt();
        Disable();
        *dmaIFCR[dmaIndex] |= streamToTCIF[channelIndex] | streamToHTIF[channelIndex];
        return !finished;
    }

    DMA_TypeDef* DmaStmG4::StreamBase::Peripheral() const
    {
        return dmaPeripheral[dmaIndex];
    }

    void DmaStmG4::StreamBase::Disable()
    {
        LL_DMA_DisableChannel(Peripheral(), channelIndex);
    }

    void DmaStmG4::StreamBase::DisableHalfTransferCompleteInterrupt()
    {
        LL_DMA_DisableIT_HT(Peripheral(), channelIndex);
    }

    void DmaStmG4::StreamBase::DisableMemoryIncrement()
    {
        LL_DMA_SetMemoryIncMode(Peripheral(), channelIndex, LL_DMA_MEMORY_NOINCREMENT);
    }

    void DmaStmG4::StreamBase::DisableTransferCompleteInterrupt()
    {
        LL_DMA_DisableIT_TC(Peripheral(), channelIndex);
    }

    void DmaStmG4::StreamBase::Enable()
    {
        LL_DMA_EnableChannel(Peripheral(), channelIndex);
    }

    void DmaStmG4::StreamBase::EnableHalfTransferCompleteInterrupt()
    {
        LL_DMA_EnableIT_HT(Peripheral(), channelIndex);
    }

    void DmaStmG4::StreamBase::EnableMemoryIncrement()
    {
        LL_DMA_SetMemoryIncMode(Peripheral(), channelIndex, LL_DMA_MEMORY_INCREMENT);
    }

    void DmaStmG4::StreamBase::EnableTransferCompleteInterrupt()
    {
        LL_DMA_EnableIT_TC(Peripheral(), channelIndex);
    }

    bool DmaStmG4::StreamBase::Finished() const
    {
        return LL_DMA_IsEnabledChannel(Peripheral(), channelIndex) == 0;
    }

    void DmaStmG4::StreamBase::SetMemoryAddress(const void* memoryAddress)
    {
        LL_DMA_SetMemoryAddress(Peripheral(), channelIndex, reinterpret_cast<uint32_t>(memoryAddress));
    }

    void DmaStmG4::StreamBase::SetMemoryToPeripheralMode()
    {
        LL_DMA_SetDataTransferDirection(Peripheral(), channelIndex, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    }

    void DmaStmG4::StreamBase::SetPeripheralAddress(volatile void* peripheralAddress)
    {
        LL_DMA_SetPeriphAddress(Peripheral(), channelIndex, reinterpret_cast<uint32_t>(peripheralAddress));
    }

    void DmaStmG4::StreamBase::SetPeripheralToMemoryMode()
    {
        LL_DMA_SetDataTransferDirection(Peripheral(), channelIndex, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    }

    void DmaStmG4::StreamBase::SetTransferSize(uint16_t size)
    {
        LL_DMA_SetDataLength(Peripheral(), channelIndex, size / DataSize());
    }

    DmaStmG4::Stream::Stream(DmaStmG4& dma,
        DmaChannelIdG4 channelId,
        volatile void* peripheralAddress,
        infra::Function<void()> actionOnTransferComplete)
        : StreamBase(dma, channelId, peripheralAddress)
        , actionOnTransferComplete(actionOnTransferComplete)
        , interruptHandler(hal::dmaIrq[dmaIndex][channelIndex], [this]()
              { OnInterrupt(); })
    {
        LL_DMA_InitTypeDef dmaInitStruct;
        LL_DMA_StructInit(&dmaInitStruct);

        dmaInitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
        dmaInitStruct.Mode = LL_DMA_MODE_NORMAL;
        dmaInitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
        dmaInitStruct.PeriphRequest = channelId.mux;
        dmaInitStruct.Priority = LL_DMA_PRIORITY_LOW;
        LL_DMA_Init(Peripheral(), channelIndex, &dmaInitStruct);

        SetPeripheralAddress(peripheralAddress);
    }

    DmaStmG4::Stream::Stream(Stream&& other)
        : StreamBase(std::move(other))
        , interruptHandler(std::move(other.interruptHandler), [this]()
              { OnInterrupt(); })
        , actionOnTransferComplete(other.actionOnTransferComplete)
    {
        other.actionOnTransferComplete = nullptr;
    }

    DmaStmG4::Stream& DmaStmG4::Stream::operator=(Stream&& other)
    {
        StreamBase::operator=(std::move(other));

        actionOnTransferComplete = other.actionOnTransferComplete;
        interruptHandler.Assign(std::move(other.interruptHandler), [this]()
            { OnInterrupt(); });

        other.actionOnTransferComplete = nullptr;

        return *this;
    }

    void DmaStmG4::Stream::StartTransmit(infra::ConstByteRange data)
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

    void DmaStmG4::Stream::StartTransmitDummy(uint16_t size)
    {
        DisableMemoryIncrement();
        SetMemoryToPeripheralMode();
        SetTransferSize(size);
        SetMemoryAddress(&dummy);
        EnableTransferCompleteInterrupt();
        Enable();
    }

    void DmaStmG4::Stream::StartReceive(infra::ByteRange data)
    {
        assert(data.size() <= 65535);
        SetPeripheralToMemoryMode();
        EnableMemoryIncrement();
        SetTransferSize(data.size());
        SetMemoryAddress(data.begin());
        EnableTransferCompleteInterrupt();
        Enable();
    }

    void DmaStmG4::Stream::StartReceiveDummy(uint16_t size)
    {
        SetPeripheralToMemoryMode();
        DisableMemoryIncrement();
        SetTransferSize(size);
        SetMemoryAddress(&dummy);
        EnableTransferCompleteInterrupt();
        Enable();
    }

    void DmaStmG4::Stream::OnInterrupt()
    {
        if (*dmaISR[dmaIndex] & streamToTCIF[channelIndex])
        {
            *dmaIFCR[dmaIndex] = streamToTCIF[channelIndex];
            Disable();

            __DMB();

            interruptHandler.ClearPending();
            actionOnTransferComplete();
        }
    }

    DmaStmG4::CircularStream::CircularStream(DmaStmG4& dma,
        DmaChannelIdG4 channelId,
        volatile void* peripheralAddress,
        infra::Function<void()> actionOnFirstHalfDone,
        infra::Function<void()> actionOnSecondHalfDone)
        : StreamBase(dma, channelId, peripheralAddress)
        , actionOnFirstHalfDone(actionOnFirstHalfDone)
        , actionOnSecondHalfDone(actionOnSecondHalfDone)
        , interruptHandler(hal::dmaIrq[dmaIndex][channelIndex], [this]()
              { OnInterrupt(); })
    {
        LL_DMA_InitTypeDef dmaInitStruct;
        LL_DMA_StructInit(&dmaInitStruct);

        dmaInitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
        dmaInitStruct.Mode = LL_DMA_MODE_CIRCULAR;
        dmaInitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
        dmaInitStruct.PeriphRequest = channelId.mux;

        dmaInitStruct.Priority = LL_DMA_PRIORITY_LOW;
        LL_DMA_Init(Peripheral(), channelIndex, &dmaInitStruct);

        SetPeripheralAddress(peripheralAddress);
    }

    size_t DmaStmG4::CircularStream::ReceivedSize() const
    {
        const auto dataLength = LL_DMA_GetDataLength(Peripheral(), channelIndex);
        const auto bytesToTransfer = dataLength * DataSize();
        return data.size() - bytesToTransfer;
    }

    void DmaStmG4::CircularStream::StartReceive(infra::ByteRange data)
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

    void DmaStmG4::CircularStream::OnInterrupt()
    {

        if (*dmaISR[dmaIndex] & streamToHTIF[channelIndex])
        {
            *dmaIFCR[dmaIndex] = streamToHTIF[channelIndex];

            interruptHandler.ClearPending();
            actionOnFirstHalfDone();
        }

        if (*dmaISR[dmaIndex] & streamToTCIF[channelIndex])
        {
            *dmaIFCR[dmaIndex] = streamToTCIF[channelIndex];

            interruptHandler.ClearPending();
            actionOnSecondHalfDone();
        }
    }
}
