#ifndef HAL_UART_STM_DUPLEX_DMA_HPP
#define HAL_UART_STM_DUPLEX_DMA_HPP

#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include <atomic>
#include <cstdint>

namespace hal
{
    class UartStmDuplexDma
        : public UartStmDma
    {
    public:
        using Config = detail::UartStmConfig;

        template<std::size_t RxBufferSize>
        using WithRxBuffer = infra::WithStorage<UartStmDuplexDma, std::array<uint8_t, RxBufferSize>>;

        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config = Config());
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config = Config());

    private:
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config, bool hasFlowControl);

    public:
        ~UartStmDuplexDma();

        // Implementation of SerialCommunication
        void ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived) override;

    private:
        void HalfReceiveComplete();
        void FullReceiveComplete();
        void ReceiveComplete(size_t currentPosition);

        // Implementation InterruptHandler
        void Invoke() override;

    private:
        volatile void* receiveRegister =
#if defined(USART_RDR_RDR)
            &peripheralUart[uartIndex]->RDR;
#else
            &peripheralUart[uartIndex]->DR;
#endif
        infra::MemoryRange<uint8_t> rxBuffer;
        hal::CircularReceiveDmaChannel receiveDmaChannel;
        std::atomic<size_t> lastReceivedPosition{};
    };
}

#endif
