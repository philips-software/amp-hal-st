#ifndef HAL_DMA_STM_HPP
#define HAL_DMA_STM_HPP

#include DEVICE_HEADER
#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include <cstdint>

namespace hal
{
#if defined(STM32G0) || defined(STM32G4) || defined(STM32WB)
#define DMA_CHANNEL_BASED
#else
#define DMA_STREAM_BASED
#endif

#if defined(DMA_STREAM_BASED)
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
#else
    struct DmaChannelId
    {
        constexpr DmaChannelId(uint8_t oneBasedDma, uint8_t oneBasedChannel, uint8_t mux)
            : dma{ static_cast<uint8_t>(oneBasedDma - 1) }
            , channel{ static_cast<uint8_t>(oneBasedChannel - 1) }
            , mux{ mux }
        {}

        uint8_t dma;
        uint8_t channel;
        uint8_t mux;
    };
#endif

    class DmaStm
    {
    public:
        class StreamBase
        {
        protected:
            StreamBase(DmaStm& dma, DmaChannelId channelId);
            StreamBase(const StreamBase&) = delete;
            StreamBase(StreamBase&& other);
            virtual ~StreamBase();

        public:
            uint8_t DataSize() const;
            void SetDataSize(uint8_t dataSizeInBytes);
            void SetPeripheralDataSize(uint8_t dataSizeInBytes);
            void SetMemoryDataSize(uint8_t dataSizeInBytes);
            bool StopTransfer();

        protected:
            StreamBase& operator=(const StreamBase&) = delete;
            StreamBase& operator=(StreamBase&& other);
            virtual void OnInterrupt() = 0;

        private:
            friend class DmaStm;

            void Disable();
            void DisableHalfTransferCompleteInterrupt();
            void DisableMemoryIncrement();
            void DisableTransferCompleteInterrupt();
            void DisableCircularMode();
            void Enable();
            void EnableHalfTransferCompleteInterrupt();
            void EnableMemoryIncrement();
            void EnableTransferCompleteInterrupt();
            void EnableCircularMode();
            bool Finished() const;
            void SetMemoryAddress(const void* memoryAddress);
            void SetMemoryToPeripheralMode();
            void SetPeripheralAddress(volatile void* peripheralAddress);
            void SetPeripheralToMemoryMode();
            void SetTransferSize(uint16_t size);

            bool IsHalfComplete() const;
            bool IsFullComplete() const;

            void ClearHalfComplete() const;
            void ClearFullComplete() const;

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
            Stream(Stream&& other);

            Stream& operator=(const Stream&) = delete;
            Stream& operator=(Stream&& other);

            void StartTransmit(infra::ConstByteRange data);
            void StartTransmitDummy(uint16_t size);
            void StartReceive(infra::ByteRange data);
            void StartReceiveDummy(uint16_t size);

        protected:
            virtual void OnInterrupt() override;

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
            virtual void OnInterrupt() override;

        private:
            ImmediateInterruptHandler interruptHandler;
            infra::Function<void()> actionOnFirstHalfDone;
            infra::Function<void()> actionOnSecondHalfDone;

            infra::ByteRange data;
        };

    public:
        DmaStm();
        ~DmaStm();

    private:
        void ReserveStream(uint8_t dmaIndex, uint8_t streamIndex);
        void ReleaseStream(uint8_t dmaIndex, uint8_t streamIndex);

    private:
        std::array<uint32_t, 2> streamAllocation = { { 0, 0 } };

    public:
        /* ======================================================= */
        /* ======================================================= */
        /* ======================================================= */
        /* ======================================================= */
        /* ======================================================= */
        /* ======================================================= */
        /* ======================================================= */

        class TransceiverStreamBase : public StreamBase
        {
        public:
            TransceiverStreamBase(DmaStm& dma, DmaChannelId channelId);

        private:
            [[deprecated]] void OnInterrupt() override
            {
                /* not required */
            }
        };

        class TransmitStreamBase : public TransceiverStreamBase
        {
        public:
            TransmitStreamBase(DmaStm& dma, DmaChannelId channelId);

            void StartTransmit(infra::ConstByteRange data);
            void StartTransmitDummy(uint16_t size);
        };

        class ReceiveStreamBase : public TransceiverStreamBase
        {
        public:
            ReceiveStreamBase(DmaStm& dma, DmaChannelId channelId);

            void StartReceive(infra::ByteRange data);
            void StartReceiveDummy(uint16_t size);

        private:
            infra::ByteRange data;
        };

        class StreamInterruptHandler
        {
        public:
            StreamInterruptHandler(StreamBase& streamBase, const infra::Function<void()>& transferFullComplete);

        private:
            void OnInterrupt();

            StreamBase& streamBase;

            DispatchedInterruptHandler dispatchedInterruptHandler;

            infra::Function<void()> transferFullComplete;
        };

        class CircularStreamInterruptHandler
        {
        public:
            CircularStreamInterruptHandler(StreamBase& streamBase, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete);

        private:
            void OnInterrupt();

            StreamBase& streamBase;

            ImmediateInterruptHandler immediateInterruptHandler;

            infra::Function<void()> transferHalfComplete;
            infra::Function<void()> transferFullComplete;
        };

        class PeripheralStream
        {
        public:
            PeripheralStream(StreamBase& streamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize);
        };

        class PeripheralTransmitStream : public PeripheralStream
        {
        public:
            PeripheralTransmitStream(TransmitStreamBase& transmitStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize);

            void StartTransmit(infra::MemoryRange<const uint8_t> data);
            void StartTransmit(infra::MemoryRange<const uint16_t> data);
            void StartTransmit(infra::MemoryRange<const uint32_t> data);

            void StartTransmitDummy(uint16_t size, uint8_t dataSize);

        private:
            TransmitStreamBase& transmitStreamBase;
        };

        class PeripheralReceiveStream : public PeripheralStream
        {
        public:
            PeripheralReceiveStream(ReceiveStreamBase& receiveStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize);

            void StartReceive(infra::MemoryRange<uint8_t> data);
            void StartReceive(infra::MemoryRange<uint16_t> data);
            void StartReceive(infra::MemoryRange<uint32_t> data);

            void StartReceiveDummy(uint16_t size, uint8_t dataSize);

        private:
            ReceiveStreamBase& receiveStreamBase;
        };
    };

    class TransmitDmaChannel
    {
    public:
        TransmitDmaChannel(DmaStm::TransmitStreamBase& transmitStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete);

        void StartTransmit(infra::MemoryRange<const uint8_t> data);
        void StartTransmit(infra::MemoryRange<const uint16_t> data);
        void StartTransmit(infra::MemoryRange<const uint32_t> data);

        void StartTransmitDummy(uint16_t size, uint8_t dataSize);

    private:
        DmaStm::PeripheralTransmitStream peripheralStream;
        DmaStm::StreamInterruptHandler streamInterruptHandler;
    };

    class CircularTransmitDmaChannel
    {
    public:
        CircularTransmitDmaChannel(DmaStm::TransmitStreamBase& transmitStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete);

        void StartTransmit(infra::MemoryRange<const uint8_t> data);
        void StartTransmit(infra::MemoryRange<const uint16_t> data);
        void StartTransmit(infra::MemoryRange<const uint32_t> data);

    private:
        DmaStm::PeripheralTransmitStream peripheralStream;
        DmaStm::CircularStreamInterruptHandler circularStreamInterruptHandler;
    };

    class ReceiveDmaChannel
    {

    public:
        ReceiveDmaChannel(DmaStm::ReceiveStreamBase& receiveStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete);

        void StartReceive(infra::MemoryRange<uint8_t> data);
        void StartReceive(infra::MemoryRange<uint16_t> data);
        void StartReceive(infra::MemoryRange<uint32_t> data);

        void StartReceiveDummy(uint16_t size, uint8_t dataSize);

    private:
        DmaStm::PeripheralReceiveStream peripheralStream;
        DmaStm::StreamInterruptHandler streamInterruptHandler;
    };

    class CircularReceiveDmaChannel
    {

    public:
        CircularReceiveDmaChannel(DmaStm::ReceiveStreamBase& receiveStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete);

        void StartReceive(infra::MemoryRange<uint8_t> data);
        void StartReceive(infra::MemoryRange<uint16_t> data);
        void StartReceive(infra::MemoryRange<uint32_t> data);

    private:
        DmaStm::PeripheralReceiveStream peripheralStream;
        DmaStm::CircularStreamInterruptHandler circularStreamInterruptHandler;
    };
}

#endif
