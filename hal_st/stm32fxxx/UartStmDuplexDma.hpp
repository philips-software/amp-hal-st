#ifndef HAL_UART_STM_DUPLEX_DMA_HPP
#define HAL_UART_STM_DUPLEX_DMA_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include <atomic>

namespace hal
{
    class UartStmDuplexDma
        : public SerialCommunication
        , private InterruptHandler
    {
    public:
        struct Config
        {
            constexpr Config()
            {}

            uint32_t baudrate = 115200;
            uint32_t hwFlowControl = UART_HWCONTROL_NONE;
            infra::Optional<DmaChannelId> dmaChannelTx;
            infra::Optional<DmaChannelId> dmaChannelRx;
            infra::Optional<InterruptPriority> priority;
        };

        template<std::size_t RxBufferSize>
        using WithRxBuffer = infra::WithStorage<UartStmDuplexDma, std::array<uint8_t, RxBufferSize>>;

        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t uartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config = Config());
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t uartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config = Config());
        ~UartStmDuplexDma();

        virtual void SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion = infra::emptyFunction) override;
        virtual void ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived) override;

    private:
        void Configure(const Config& config);
        void ReceiveComplete(size_t currentPosition);
        void RegisterInterrupt(const Config& config);
        void TransferComplete();
        virtual void Invoke() override;

    private:
        infra::MemoryRange<uint8_t> rxBuffer;
        uint8_t uartIndex;
        PeripheralPinStm uartTx;
        PeripheralPinStm uartRx;
        infra::Optional<PeripheralPinStm> uartRts;
        infra::Optional<PeripheralPinStm> uartCts;

        UART_HandleTypeDef uartHandle = {};

        hal::TransmitDmaChannel transmitDmaChannel;
        hal::CircularReceiveDmaChannel receiveDmaChannel;

        infra::Function<void()> transferDataComplete;
        infra::Function<void(infra::ConstByteRange data)> dataReceived;

        std::atomic<size_t> lastReceivedPosition{};
    };
}

#endif
