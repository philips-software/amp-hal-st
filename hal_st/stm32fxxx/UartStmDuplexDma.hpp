#ifndef HAL_UART_STM_DUPLEX_DMA_HPP
#define HAL_UART_STM_DUPLEX_DMA_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include "infra/util/WithStorage.hpp"
#include <array>
#include <atomic>
#include <cstddef>
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

        // Number of times Drain() observed both the half- and full-transfer DMA flags pending at
        // once, meaning the ISR ran at least half a ring late and may have missed data.
        std::size_t OverrunCount() const;
        void OverrunDetected(const infra::Function<void()>& onOverrun);

    private:
        // Delivers every byte written by the DMA since the last call, deriving the range(s) from
        // the live DMA position instead of assuming which callback fired.
        void Drain();

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
        bool draining = false;
        std::size_t overrunCount = 0;
        infra::Function<void()> onOverrun;
    };
}

#endif
