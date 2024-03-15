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

    class TransceiverDmaChannel;
    class CircularTransceiverDmaChannel;

    class TransmitDmaChannel;
    class ReceiveDmaChannel;

    class CircularTransmitDmaChannel;
    class CircularReceiveDmaChannel;

    class DmaStm
    {
    public:
        class Stream
        {
        public:
            Stream(DmaStm& dma, DmaChannelId channelId);

        protected:
            Stream(const Stream&) = delete;
            Stream(Stream&& other) noexcept;
            virtual ~Stream();

        public:
            uint8_t DataSize() const;
            void SetDataSize(uint8_t dataSizeInBytes);
            void SetPeripheralDataSize(uint8_t dataSizeInBytes);
            void SetMemoryDataSize(uint8_t dataSizeInBytes);
            bool StopTransfer();

        protected:
            Stream& operator=(const Stream&) = delete;
            Stream& operator=(Stream&& other);

        protected:
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

            size_t BytesToTransfer() const;

        private:
            friend DmaStm;

            DmaStm& dma;
            uint8_t dmaIndex;
            uint8_t streamIndex = 0xff;
        };

        class TransceiveStream
            : private Stream
        {
        public:
            using Stream::Stream;

            void StartTransmit(infra::ConstByteRange data);
            void StartTransmitDummy(uint16_t size);
            void StartReceive(infra::ByteRange data);
            void StartReceiveDummy(uint16_t size);
            size_t ReceivedSize() const;

            friend DmaStm;
            friend TransceiverDmaChannel;
            friend CircularTransceiverDmaChannel;

        private:
            infra::ByteRange data;
        };

        class TransmitStream
            : private TransceiveStream
        {
        public:
            using TransceiveStream::TransceiveStream;

            using TransceiveStream::StartTransmit;
            using TransceiveStream::StartTransmitDummy;

            friend TransmitDmaChannel;
            friend CircularTransmitDmaChannel;
        };

        class ReceiveStream
            : private TransceiveStream
        {
        public:
            using TransceiveStream::TransceiveStream;

            using TransceiveStream::ReceivedSize;
            using TransceiveStream::StartReceive;
            using TransceiveStream::StartReceiveDummy;

            friend ReceiveDmaChannel;
            friend CircularReceiveDmaChannel;
        };

        class StreamInterruptHandler
        {
        public:
            StreamInterruptHandler(Stream& stream, const infra::Function<void()>& transferFullComplete);

        private:
            void OnInterrupt();

            Stream& stream;

            DispatchedInterruptHandler dispatchedInterruptHandler;

            infra::Function<void()> transferFullComplete;
        };

        class CircularStreamInterruptHandler
        {
        public:
            CircularStreamInterruptHandler(Stream& stream, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete);

            bool IsInterruptPending() const;

        private:
            void OnInterrupt();

            Stream& stream;

            ImmediateInterruptHandler immediateInterruptHandler;

            infra::Function<void()> transferHalfComplete;
            infra::Function<void()> transferFullComplete;
        };

        class PeripheralTransceiveStream
        {
        public:
            PeripheralTransceiveStream(TransceiveStream& stream, volatile void* peripheralAddress, uint8_t peripheralTransferSize);

            void SetPeripheralTransferSize(uint8_t peripheralTransferSize);
            bool StopTransfer();

            template<class U>
            void StartTransmit(infra::MemoryRange<const U> data);
            void StartTransmitDummy(uint16_t size, uint8_t dataSize = 1);

            template<class U>
            void StartReceive(infra::MemoryRange<U> data);
            void StartReceiveDummy(uint16_t size, uint8_t dataSize = 1);

            size_t ReceivedSize() const;

        private:
            TransceiveStream& stream;
        };

        class PeripheralTransmitStream
            : private PeripheralTransceiveStream
        {
        public:
            using PeripheralTransceiveStream::PeripheralTransceiveStream;

            using PeripheralTransceiveStream::SetPeripheralTransferSize;
            using PeripheralTransceiveStream::StopTransfer;

            using PeripheralTransceiveStream::StartTransmit;
            using PeripheralTransceiveStream::StartTransmitDummy;
        };

        class PeripheralReceiveStream
            : private PeripheralTransceiveStream
        {
        public:
            using PeripheralTransceiveStream::PeripheralTransceiveStream;

            using PeripheralTransceiveStream::SetPeripheralTransferSize;
            using PeripheralTransceiveStream::StopTransfer;

            using PeripheralTransceiveStream::StartReceive;
            using PeripheralTransceiveStream::StartReceiveDummy;

            using PeripheralTransceiveStream::ReceivedSize;
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

    class TransceiverDmaChannelBase
    {
    public:
        TransceiverDmaChannelBase(DmaStm::TransceiveStream& stream, volatile void* peripheralAddress, uint8_t peripheralTransferSize);

        bool StopTransfer();
        void SetPeripheralTransferSize(uint8_t peripheralTransferSize);

        template<class U>
        void StartTransmit(infra::MemoryRange<const U> data);
        void StartTransmitDummy(uint16_t size, uint8_t dataSize = 1);

        template<class U>
        void StartReceive(infra::MemoryRange<U> data);
        void StartReceiveDummy(uint16_t size, uint8_t dataSize = 1);

        size_t ReceivedSize() const;

    private:
        DmaStm::PeripheralTransceiveStream peripheralStream;
    };

    class TransceiverDmaChannel
        : public TransceiverDmaChannelBase
    {
    public:
        TransceiverDmaChannel(DmaStm::TransceiveStream& receiveStream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete);

    private:
        DmaStm::StreamInterruptHandler streamInterruptHandler;
    };

    class TransmitDmaChannel
        : private TransceiverDmaChannel
    {
    public:
        TransmitDmaChannel(DmaStm::TransmitStream& transmitStream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete);

        using TransceiverDmaChannel::SetPeripheralTransferSize;
        using TransceiverDmaChannel::StopTransfer;

        using TransceiverDmaChannel::StartTransmit;
        using TransceiverDmaChannel::StartTransmitDummy;

        using TransceiverDmaChannel::StartReceive;
        using TransceiverDmaChannel::StartReceiveDummy;

        using TransceiverDmaChannel::ReceivedSize;
    };

    class ReceiveDmaChannel
        : private TransceiverDmaChannel
    {
    public:
        ReceiveDmaChannel(DmaStm::ReceiveStream& receiveStream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete);

        using TransceiverDmaChannel::SetPeripheralTransferSize;
        using TransceiverDmaChannel::StopTransfer;

        using TransceiverDmaChannel::StartTransmit;
        using TransceiverDmaChannel::StartTransmitDummy;

        using TransceiverDmaChannel::StartReceive;
        using TransceiverDmaChannel::StartReceiveDummy;

        using TransceiverDmaChannel::ReceivedSize;
    };

    class CircularTransceiverDmaChannel
        : public TransceiverDmaChannelBase
    {
    public:
        CircularTransceiverDmaChannel(DmaStm::TransceiveStream& stream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete);

        bool IsInterruptPending() const;

    private:
        DmaStm::CircularStreamInterruptHandler circularStreamInterruptHandler;
    };

    class CircularTransmitDmaChannel
        : private CircularTransceiverDmaChannel
    {
    public:
        CircularTransmitDmaChannel(DmaStm::TransmitStream& stream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete);

        using CircularTransceiverDmaChannel::SetPeripheralTransferSize;
        using CircularTransceiverDmaChannel::StopTransfer;

        using CircularTransceiverDmaChannel::StartTransmit;
        using CircularTransceiverDmaChannel::StartTransmitDummy;

        using CircularTransceiverDmaChannel::StartReceive;
        using CircularTransceiverDmaChannel::StartReceiveDummy;

        using CircularTransceiverDmaChannel::ReceivedSize;
    };

    class CircularReceiveDmaChannel
        : private CircularTransceiverDmaChannel
    {
    public:
        CircularReceiveDmaChannel(DmaStm::ReceiveStream& stream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete);

        using CircularTransceiverDmaChannel::SetPeripheralTransferSize;
        using CircularTransceiverDmaChannel::StopTransfer;

        using CircularTransceiverDmaChannel::StartTransmit;
        using CircularTransceiverDmaChannel::StartTransmitDummy;

        using CircularTransceiverDmaChannel::StartReceive;
        using CircularTransceiverDmaChannel::StartReceiveDummy;

        using CircularTransceiverDmaChannel::ReceivedSize;

        using CircularTransceiverDmaChannel::IsInterruptPending;
    };

    ////    Implementation    ////

    template<class U>
    void DmaStm::PeripheralTransceiveStream::StartTransmit(infra::MemoryRange<const U> data)
    {
        stream.SetMemoryDataSize(sizeof(U));
        stream.StartTransmit(infra::ReinterpretCastByteRange(data));
    }

    template<class U>
    void DmaStm::PeripheralTransceiveStream::StartReceive(infra::MemoryRange<U> data)
    {
        stream.SetMemoryDataSize(sizeof(U));
        stream.StartReceive(infra::ReinterpretCastByteRange(data));
    }

    template<class U>
    void TransceiverDmaChannelBase::StartTransmit(infra::MemoryRange<const U> data)
    {
        peripheralStream.StartTransmit(data);
    }

    template<class U>
    void TransceiverDmaChannelBase::StartReceive(infra::MemoryRange<U> data)
    {
        peripheralStream.StartReceive(data);
    }

}

#endif
