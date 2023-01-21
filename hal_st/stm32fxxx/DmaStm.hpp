#ifndef HAL_DMA_STM_HPP
#define HAL_DMA_STM_HPP

#include "cmsis_device.h"
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
        class Stream
        {
        public:
            Stream(DmaStm& dma, DmaChannelId channelId, volatile void* peripheralAddress, infra::Function<void()> actionOnTransferComplete);
            Stream(const Stream&) = delete;
            Stream(Stream&& other);
            ~Stream();

            Stream& operator=(const Stream&) = delete;
            Stream& operator=(Stream&& other);

            void StartTransmit(infra::ConstByteRange data);
            void StartTransmitDummy(uint16_t size);
            void StartReceive(infra::ByteRange data);
            void StartReceiveDummy(uint16_t size);
            bool StopTransfer();

            uint8_t DataSize() const;
            void SetDataSize(uint8_t dataSizeInBytes);

        private:
            void OnInterrupt();

        private:
            friend class DmaStm;

            DmaStm& dma;
            uint8_t dmaIndex;
            uint8_t streamIndex = 0xff;
            infra::Function<void()> actionOnTransferComplete;
            DispatchedInterruptHandler interruptHandler;
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
}

#endif
