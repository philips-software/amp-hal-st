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

            size_t BytesToTransfer() const;

        private:
            DmaStm& dma;
            uint8_t dmaIndex;
            uint8_t streamIndex = 0xff;
        };

        template<class T>
        class TransmitStreamBaseV2
        {
        public:
            void StartTransmit(infra::ConstByteRange data);
            void StartTransmitDummy(uint16_t size);
        };

        template<class T>
        class ReceiveStreamBaseV2
        {
        public:
            void StartReceive(infra::ByteRange data);
            void StartReceiveDummy(uint16_t size);

            size_t ReceivedSize() const;

        private:
            infra::ByteRange data;
        };

        class TransmitStream : public Stream
            , public TransmitStreamBaseV2<TransmitStream>
        {
        public:
            using Stream::Stream;
        };

        class ReceiveStream : public Stream
            , public ReceiveStreamBaseV2<ReceiveStream>
        {
        public:
            using Stream::Stream;
        };

        class TransceiveStream : public Stream
            , public TransmitStreamBaseV2<TransceiveStream>
            , public ReceiveStreamBaseV2<TransceiveStream>
        {
        public:
            using Stream::Stream;
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

        class PeripheralStream
        {
        public:
            PeripheralStream(Stream& stream, volatile void* peripheralAddress, uint8_t peripheralTransferSize);
        };

        template<class T>
        class PeripheralStreamBase
        {
        public:
            void SetPeripheralTransferSize(uint8_t peripheralTransferSize);
        };

        template<class T>
        class PeripheralTransmitStreamBase
        {
        public:
            template<class U>
            void StartTransmit(infra::MemoryRange<const U> data);
            void StartTransmitDummy(uint16_t size, uint8_t dataSize = 1);

            bool StopTransfer();
        };

        template<class T>
        class PeripheralReceiveStreamBase
        {
        public:
            template<class U>
            void StartReceive(infra::MemoryRange<U> data);
            void StartReceiveDummy(uint16_t size, uint8_t dataSize = 1);

            bool StopTransfer();

            size_t ReceivedSize() const;
        };

        class PeripheralTransmitStream
            : public PeripheralStreamBase<PeripheralTransmitStream>
            , public PeripheralTransmitStreamBase<PeripheralTransmitStream>
        {
        public:
            PeripheralTransmitStream(TransmitStream& transmitStream, volatile void* peripheralAddress, uint8_t peripheralTransferSize);

        private:
            friend PeripheralStreamBase<PeripheralTransmitStream>;
            friend PeripheralTransmitStreamBase<PeripheralTransmitStream>;

            PeripheralStream peripheralStream;
            TransmitStream& stream;
        };

        class PeripheralReceiveStream
            : public PeripheralStreamBase<PeripheralReceiveStream>
            , public PeripheralReceiveStreamBase<PeripheralReceiveStream>
        {
        public:
            PeripheralReceiveStream(ReceiveStream& receiveStream, volatile void* peripheralAddress, uint8_t peripheralTransferSize);

        private:
            friend PeripheralStreamBase<PeripheralReceiveStream>;
            friend PeripheralReceiveStreamBase<PeripheralReceiveStream>;

            PeripheralStream peripheralStream;
            ReceiveStream& stream;
        };

        class PeripheralTransceiveStream
            : public PeripheralStreamBase<PeripheralTransceiveStream>
            , public PeripheralTransmitStreamBase<PeripheralTransceiveStream>
            , public PeripheralReceiveStreamBase<PeripheralTransceiveStream>
        {
        public:
            PeripheralTransceiveStream(TransceiveStream& transceiveStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize);

        private:
            friend PeripheralStreamBase<PeripheralTransceiveStream>;
            friend PeripheralTransmitStreamBase<PeripheralTransceiveStream>;
            friend PeripheralReceiveStreamBase<PeripheralTransceiveStream>;

            PeripheralStream peripheralStream;
            TransceiveStream& stream;
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

    namespace detail
    {
        template<class T>
        class TransmitRangeDmaChannelBase
        {
        public:
            template<class U>
            void StartTransmit(infra::MemoryRange<const U> data);
        };

        template<class T>
        class TransmitDummyDmaChannelBase
        {
        public:
            void StartTransmitDummy(uint16_t size, uint8_t dataSize = 1);
        };

        template<class T>
        class ReceiveRangeDmaChannelBase
        {
        public:
            template<class U>
            void StartReceive(infra::MemoryRange<U> data);

            size_t ReceivedSize() const;
        };

        template<class T>
        class ReceiveDummyDmaChannelBase
        {
        public:
            void StartReceiveDummy(uint16_t size, uint8_t dataSize = 1);
        };

        template<class T>
        class StopTransferDmaChannelBase
        {
        public:
            bool StopTransfer();
        };

        template<class T>
        class SetTransferSizeDmaChannelBase
        {
        public:
            void SetPeripheralTransferSize(uint8_t peripheralTransferSize);
        };
    }

    class TransmitDmaChannel
        : public detail::TransmitRangeDmaChannelBase<TransmitDmaChannel>
        , public detail::TransmitDummyDmaChannelBase<TransmitDmaChannel>
        , public detail::StopTransferDmaChannelBase<TransmitDmaChannel>
        , public detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>
    {
    public:
        TransmitDmaChannel(DmaStm::TransmitStream& transmitStream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete);

    private:
        friend detail::TransmitRangeDmaChannelBase<TransmitDmaChannel>;
        friend detail::TransmitDummyDmaChannelBase<TransmitDmaChannel>;
        friend detail::StopTransferDmaChannelBase<TransmitDmaChannel>;
        friend detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>;

        DmaStm::PeripheralTransmitStream peripheralStream;
        DmaStm::StreamInterruptHandler streamInterruptHandler;
    };

    class CircularTransmitDmaChannel
        : public detail::TransmitRangeDmaChannelBase<CircularTransmitDmaChannel>
        , public detail::StopTransferDmaChannelBase<CircularTransmitDmaChannel>
        , public detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>
    {
    public:
        CircularTransmitDmaChannel(DmaStm::TransmitStream& transmitStream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete);

    private:
        friend detail::TransmitRangeDmaChannelBase<CircularTransmitDmaChannel>;
        friend detail::StopTransferDmaChannelBase<CircularTransmitDmaChannel>;
        friend detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>;

        DmaStm::PeripheralTransmitStream peripheralStream;
        DmaStm::CircularStreamInterruptHandler circularStreamInterruptHandler;
    };

    class ReceiveDmaChannel
        : public detail::ReceiveRangeDmaChannelBase<ReceiveDmaChannel>
        , public detail::ReceiveDummyDmaChannelBase<ReceiveDmaChannel>
        , public detail::StopTransferDmaChannelBase<ReceiveDmaChannel>
        , public detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>
    {

    public:
        ReceiveDmaChannel(DmaStm::ReceiveStream& receiveStream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete);

    private:
        friend detail::ReceiveRangeDmaChannelBase<ReceiveDmaChannel>;
        friend detail::ReceiveDummyDmaChannelBase<ReceiveDmaChannel>;
        friend detail::StopTransferDmaChannelBase<ReceiveDmaChannel>;
        friend detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>;

        DmaStm::PeripheralReceiveStream peripheralStream;
        DmaStm::StreamInterruptHandler streamInterruptHandler;
    };

    class CircularReceiveDmaChannel
        : public detail::ReceiveRangeDmaChannelBase<CircularReceiveDmaChannel>
        , public detail::StopTransferDmaChannelBase<CircularReceiveDmaChannel>
        , public detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>
    {

    public:
        CircularReceiveDmaChannel(DmaStm::ReceiveStream& receiveStream, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferHalfComplete, const infra::Function<void()>& transferFullComplete);

        bool IsInterruptPending() const;

    private:
        friend detail::ReceiveRangeDmaChannelBase<CircularReceiveDmaChannel>;
        friend detail::StopTransferDmaChannelBase<CircularReceiveDmaChannel>;
        friend detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>;

        DmaStm::PeripheralReceiveStream peripheralStream;
        DmaStm::CircularStreamInterruptHandler circularStreamInterruptHandler;
    };

    class TransceiverDmaChannel
        : public detail::TransmitRangeDmaChannelBase<TransceiverDmaChannel>
        , public detail::TransmitDummyDmaChannelBase<TransceiverDmaChannel>
        , public detail::ReceiveRangeDmaChannelBase<TransceiverDmaChannel>
        , public detail::ReceiveDummyDmaChannelBase<TransceiverDmaChannel>
        , public detail::StopTransferDmaChannelBase<TransceiverDmaChannel>
        , public detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>
    {
    public:
        TransceiverDmaChannel(DmaStm::TransceiveStream& transceiveStreamBase, volatile void* peripheralAddress, uint8_t peripheralTransferSize, const infra::Function<void()>& transferFullComplete);

    private:
        friend detail::TransmitRangeDmaChannelBase<TransceiverDmaChannel>;
        friend detail::TransmitDummyDmaChannelBase<TransceiverDmaChannel>;
        friend detail::ReceiveRangeDmaChannelBase<TransceiverDmaChannel>;
        friend detail::ReceiveDummyDmaChannelBase<TransceiverDmaChannel>;
        friend detail::StopTransferDmaChannelBase<TransceiverDmaChannel>;
        friend detail::SetTransferSizeDmaChannelBase<TransmitDmaChannel>;

        DmaStm::PeripheralTransceiveStream peripheralStream;
        DmaStm::StreamInterruptHandler streamInterruptHandler;
    };

    ////    Implementation    ////

    template<class T>
    void DmaStm::TransmitStreamBaseV2<T>::StartTransmit(infra::ConstByteRange data)
    {
        auto& derived = *static_cast<T*>(this);

        derived.SetMemoryToPeripheralMode();
        derived.EnableMemoryIncrement();
        derived.SetTransferSize(data.size());
        derived.SetMemoryAddress(data.begin());
        derived.Enable();
    }

    template<class T>
    void DmaStm::TransmitStreamBaseV2<T>::StartTransmitDummy(uint16_t size)
    {
        static uint32_t dummy;

        auto& derived = *static_cast<T*>(this);

        derived.SetMemoryToPeripheralMode();
        derived.DisableMemoryIncrement();
        derived.SetTransferSize(size);
        derived.SetMemoryAddress(&dummy);
        derived.Enable();
    }

    template<class T>
    void DmaStm::ReceiveStreamBaseV2<T>::StartReceive(infra::ByteRange data)
    {
        auto& derived = *static_cast<T*>(this);

        this->data = data;

        derived.SetPeripheralToMemoryMode();
        derived.EnableMemoryIncrement();
        derived.SetTransferSize(data.size());
        derived.SetMemoryAddress(data.begin());
        derived.Enable();
    }

    template<class T>
    void DmaStm::ReceiveStreamBaseV2<T>::StartReceiveDummy(uint16_t size)
    {
        static uint32_t dummy;

        auto& derived = *static_cast<T*>(this);

        derived.SetPeripheralToMemoryMode();
        derived.DisableMemoryIncrement();
        derived.SetTransferSize(size);
        derived.SetMemoryAddress(&dummy);
        derived.Enable();
    }

    template<class T>
    size_t DmaStm::ReceiveStreamBaseV2<T>::ReceivedSize() const
    {
        const auto& derived = *static_cast<const T*>(this);
        return (data.size() - derived.BytesToTransfer()) / derived.DataSize();
    }

    template<class T>
    void DmaStm::PeripheralStreamBase<T>::SetPeripheralTransferSize(uint8_t peripheralTransferSize)
    {
        const auto& derived = *static_cast<T*>(this);
        derived.stream.SetPeripheralDataSize(peripheralTransferSize);
    }

    template<class T>
    template<class U>
    void DmaStm::PeripheralTransmitStreamBase<T>::StartTransmit(infra::MemoryRange<const U> data)
    {
        const auto& derived = *static_cast<T*>(this);

        derived.stream.SetMemoryDataSize(sizeof(U));
        derived.stream.StartTransmit(infra::ReinterpretCastByteRange(data));
    }

    template<class T>
    void DmaStm::PeripheralTransmitStreamBase<T>::StartTransmitDummy(uint16_t size, uint8_t dataSize)
    {
        const auto& derived = *static_cast<T*>(this);

        derived.stream.SetMemoryDataSize(dataSize);
        derived.stream.StartTransmitDummy(size);
    }

    template<class T>
    bool DmaStm::PeripheralTransmitStreamBase<T>::StopTransfer()
    {
        const auto& derived = *static_cast<T*>(this);

        return derived.stream.StopTransfer();
    }

    template<class T>
    template<class U>
    void DmaStm::PeripheralReceiveStreamBase<T>::StartReceive(infra::MemoryRange<U> data)
    {
        const auto& derived = *static_cast<T*>(this);

        derived.stream.SetMemoryDataSize(sizeof(U));
        derived.stream.StartReceive(infra::ReinterpretCastByteRange(data));
    }

    template<class T>
    void DmaStm::PeripheralReceiveStreamBase<T>::StartReceiveDummy(uint16_t size, uint8_t dataSize)
    {
        const auto& derived = *static_cast<T*>(this);

        derived.stream.SetMemoryDataSize(dataSize);
        derived.stream.StartReceiveDummy(size);
    }

    template<class T>
    bool DmaStm::PeripheralReceiveStreamBase<T>::StopTransfer()
    {
        const auto& derived = *static_cast<T*>(this);

        return derived.stream.StopTransfer();
    }

    template<class T>
    size_t DmaStm::PeripheralReceiveStreamBase<T>::ReceivedSize() const
    {
        const auto& derived = *static_cast<const T*>(this);

        return derived.stream.ReceivedSize();
    }

    namespace detail
    {
        template<class T>
        template<class U>
        void TransmitRangeDmaChannelBase<T>::StartTransmit(infra::MemoryRange<const U> data)
        {
            auto& derived = *static_cast<T*>(this);

            derived.peripheralStream.StartTransmit(data);
        }

        template<class T>
        void TransmitDummyDmaChannelBase<T>::StartTransmitDummy(uint16_t size, uint8_t dataSize)
        {
            auto& derived = *static_cast<T*>(this);

            derived.peripheralStream.StartTransmitDummy(size, dataSize);
        }

        template<class T>
        template<class U>
        void ReceiveRangeDmaChannelBase<T>::StartReceive(infra::MemoryRange<U> data)
        {
            auto& derived = *static_cast<T*>(this);

            derived.peripheralStream.StartReceive(data);
        }

        template<class T>
        size_t ReceiveRangeDmaChannelBase<T>::ReceivedSize() const
        {
            auto& derived = *static_cast<const T*>(this);

            return derived.peripheralStream.ReceivedSize();
        }

        template<class T>
        void ReceiveDummyDmaChannelBase<T>::StartReceiveDummy(uint16_t size, uint8_t dataSize)
        {
            auto& derived = *static_cast<T*>(this);

            derived.peripheralStream.StartReceiveDummy(size, dataSize);
        }

        template<class T>
        bool StopTransferDmaChannelBase<T>::StopTransfer()
        {
            auto& derived = *static_cast<T*>(this);

            return derived.peripheralStream.StopTransfer();
        }

        template<class T>
        void SetTransferSizeDmaChannelBase<T>::SetPeripheralTransferSize(uint8_t peripheralTransferSize)
        {
            auto& derived = *static_cast<T*>(this);

            derived.peripheralStream.SetPeripheralTransferSize(peripheralTransferSize);
        }
    }
}

#endif
