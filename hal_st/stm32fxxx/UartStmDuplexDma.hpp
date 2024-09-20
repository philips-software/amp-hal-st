#ifndef HAL_UART_STM_DUPLEX_DMA_HPP
#define HAL_UART_STM_DUPLEX_DMA_HPP

#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include <atomic>
#include <cstdint>

namespace hal
{
    class UartStmDuplexDma
        : public UartStm
    {
    public:
        using Config = detail::UartStmConfig;

        template<std::size_t RxBufferSize>
        using WithRxBuffer = infra::WithStorage<UartStmDuplexDma, std::array<uint8_t, RxBufferSize>>;

        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config = Config());
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config = Config());
#if defined(HAS_PERIPHERAL_LPUART)
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, LpUart lpUart, const Config& config = Config());
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, LpUart lpUart, const Config& config = Config());
#endif

    private:
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config, bool hasFlowControl);

    public:
        ~UartStmDuplexDma();

        // Implementation of SerialCommunication
        void SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion = infra::emptyFunction) override;
        void ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived) override;

    private:
        void HalfReceiveComplete();
        void FullReceiveComplete();
        void ReceiveComplete(size_t currentPosition);
        void TransferComplete();

        // Implementation InterruptHandler
        void Invoke() override;

    private:
        infra::MemoryRange<uint8_t> rxBuffer;
        hal::TransmitDmaChannel transmitDmaChannel;
        hal::CircularReceiveDmaChannel receiveDmaChannel;
        infra::Function<void()> transferDataComplete;
        infra::Function<void(infra::ConstByteRange data)> dataReceived;
        std::atomic<size_t> lastReceivedPosition{};
        uint8_t uartIndex;
    };
}

#endif
