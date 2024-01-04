#ifndef HAL_DMA_STM_HPP
#define HAL_DMA_STM_HPP

#include "infra/util/MemoryRange.hpp"
#include DEVICE_HEADER
#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"

namespace hal
{
    struct DmaChannelId
    {
        constexpr DmaChannelId(uint8_t oneBasedDma, uint8_t stream, uint8_t channel)
            : dma(oneBasedDma - 1)
            , stream(stream)
            , channel(channel)
        {}

        uint8_t dma;
        uint8_t stream;
        uint8_t channel;
    };

    class DmaStm
    {
    public:
        class StreamBase
        {
        protected:
            StreamBase(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress);
            StreamBase(const StreamBase&) = delete;
            StreamBase(StreamBase&& other) noexcept;
            virtual ~StreamBase();

        public:
            uint8_t DataSize() const;
            void SetDataSize(uint8_t dataSizeInBytes);
            void SetPeripheralDataSize(uint8_t dataSizeInBytes);
            void SetMemoryDataSize(uint8_t dataSizeInBytes);
            bool StopTransfer();

        protected:
            StreamBase& operator=(const StreamBase&) = delete;
            StreamBase& operator=(StreamBase&& other) noexcept;
            virtual void OnInterrupt() = 0;

        private:
            friend class DmaStm;

            void Disable();
            void DisableHalfTransferCompleteInterrupt();
            void DisableMemoryIncrement();
            void DisableTransferCompleteInterrupt();
            void Enable();
            void EnableHalfTransferCompleteInterrupt();
            void EnableMemoryIncrement();
            void EnableTransferCompleteInterrupt();
            bool Finished() const;
            void SetMemoryAddress(const void* memoryAddress);
            void SetMemoryToPeripheralMode();
            void SetPeripheralAddress(volatile void* peripheralAddress);
            void SetPeripheralToMemoryMode();
            void SetTransferSize(uint16_t size);

        private:
            DmaStm& dma;
            uint8_t dmaIndex;
            uint8_t streamIndex = 0xff;
        };

        class Stream
            : public StreamBase
        {
        public:
            Stream(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress, infra::Function<void()> actionOnTransferComplete);
            Stream(const Stream&) = delete;
            Stream(Stream&& other) noexcept;
            virtual ~Stream() = default;

            Stream& operator=(const Stream&) = delete;
            Stream& operator=(Stream&& other) noexcept;

            virtual void StartTransmit(infra::ConstByteRange data);
            void StartTransmitDummy(uint16_t size);
            virtual void StartReceive(infra::ByteRange data);
            void StartReceiveDummy(uint16_t size);

        protected:
            void OnInterrupt() override;

        private:
            DispatchedInterruptHandler interruptHandler;
            infra::Function<void()> actionOnTransferComplete;
        };

        class CircularStream
            : public StreamBase
        {
        public:
            CircularStream(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress, infra::Function<void()> actionOnFirstHalfDone, infra::Function<void()> actionOnSecondHalfDone);

            std::size_t ReceivedSize() const;
            void StartReceive(infra::ByteRange data);

        protected:
            void OnInterrupt() override;

        private:
            ImmediateInterruptHandler interruptHandler;
            infra::Function<void()> actionOnFirstHalfDone;
            infra::Function<void()> actionOnSecondHalfDone;

            infra::ByteRange data;
        };

        template<typename T>
        class StreamGeneric
            : public Stream
        {
        public:
            StreamGeneric(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress, const infra::Function<void()>& actionOnHalfTransferComplete, const infra::Function<void()>& actionOnTransferComplete);
            virtual ~StreamGeneric() = default;

            void TransferMemoryToPeripheral(infra::MemoryRange<T> data);
            void TransferPeripheralToMemory(infra::MemoryRange<T> data);
        };

    public:
        DmaStm();
        ~DmaStm();

    private:
        void ReserveStream(uint8_t dmaIndex, uint8_t streamIndex);
        void ReleaseStream(uint8_t dmaIndex, uint8_t streamIndex);

    private:
        std::array<uint32_t, 2> streamAllocation = { { 0, 0 } };
    };

    //// Implementation ////

    template<typename T>
    DmaStm::StreamGeneric<T>::StreamGeneric(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress, const infra::Function<void()>& actionOnHalfTransferComplete, const infra::Function<void()>& actionOnTransferComplete)
        : Stream(dma, channelId, peripheralAddress, actionOnTransferComplete)
    {}

    template<typename T>
    void DmaStm::StreamGeneric<T>::TransferMemoryToPeripheral(infra::MemoryRange<T> data)
    {
        SetMemoryDataSize(sizeof(T));
        SetPeripheralDataSize(sizeof(T));
        EnableHalfTransferCompleteInterrupt();
        StartTransmit(infra::MakeRange(data));
    }

    template<typename T>
    void DmaStm::StreamGeneric<T>::TransferPeripheralToMemory(infra::MemoryRange<T> data)
    {
        SetMemoryDataSize(sizeof(T));
        SetPeripheralDataSize(sizeof(T));
        EnableHalfTransferCompleteInterrupt();
        StartReceive(infra::ReinterpretCastMemoryRange<uint8_t, T>(data));
    }
}

#endif
