#ifndef HAL_DMA_STM_G4_HPP
#define HAL_DMA_STM_G4_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"

namespace hal
{
    struct DmaChannelIdG4
    {
        // mux can be any from LL_DMAMUX_REQ_
        constexpr DmaChannelIdG4(uint8_t oneBasedDma, uint8_t channel, uint8_t mux)
            : dma{ static_cast<uint8_t>(oneBasedDma - 1) }
            , channel{ static_cast<uint8_t>(channel - 1) }
            , mux{ mux }
        {}

        uint8_t dma;
        uint8_t channel;
        uint8_t mux;
    };

    class DmaStmG4
    {
    public:
        class StreamBase
        {
        protected:
            StreamBase(DmaStmG4& dma, DmaChannelIdG4 channelId, volatile void* peripheralAddress);
            StreamBase(const StreamBase&) = delete;
            StreamBase(StreamBase&& other);
            virtual ~StreamBase();

        public:
            uint8_t DataSize() const;
            void SetDataSize(uint8_t dataSizeInBytes);
            bool StopTransfer();

        protected:
            StreamBase& operator=(const StreamBase&) = delete;
            StreamBase& operator=(StreamBase&& other);
            virtual void OnInterrupt() = 0;

            DMA_TypeDef* Peripheral() const;

        private:
            friend class DmaStmG4;

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
            DmaStmG4& dma;
            uint8_t dmaIndex;
            uint8_t channelIndex = 0xff;
        };

        class Stream : public StreamBase
        {
        public:
            Stream(DmaStmG4& dma,
                DmaChannelIdG4 channelId,
                volatile void* peripheralAddress,
                infra::Function<void()> actionOnTransferComplete);
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

        class CircularStream : public StreamBase
        {
        public:
            CircularStream(DmaStmG4& dma,
                DmaChannelIdG4 channelId,
                volatile void* peripheralAddress,
                infra::Function<void()> actionOnFirstHalfDone,
                infra::Function<void()> actionOnSecondHalfDone);

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
        DmaStmG4();
        ~DmaStmG4();

    private:
        void ReserveStream(uint8_t dmaIndex, uint8_t streamIndex);
        void ReleaseStream(uint8_t dmaIndex, uint8_t streamIndex);

    private:
        std::array<uint32_t, 2> streamAllocation{ 0, 0 };
    };
}

#endif
