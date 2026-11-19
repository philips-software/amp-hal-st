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

        [[deprecated("Use UartStmDuplexDma with GpioPins")]] UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, const Config& config = Config());
        [[deprecated("Use UartStmDuplexDma with GpioPinsFlowCtrl")]] UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, GpioPinStm& rts, GpioPinStm& cts, const Config& config = Config());
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, const GpioPins& pins, const Config& config = Config());
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, const GpioPinsFlowCtrl& pins, const Config& config = Config());
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioOwners&& gpioOwners, const Config& config = Config());
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, const Config& config = Config());

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
