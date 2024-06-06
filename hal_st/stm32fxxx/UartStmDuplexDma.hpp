#ifndef HAL_UART_STM_DUPLEX_DMA_HPP
#define HAL_UART_STM_DUPLEX_DMA_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include <atomic>
#include <cstdint>

#include DEVICE_HEADER

namespace hal
{
    namespace detail
    {
        struct UartStmDuplexDmaConfig
        {
            uint32_t baudrate{ 115200 };
            hal::InterruptPriority priority{ hal::InterruptPriority::Normal };

#if defined(UART_ADVFEATURE_SWAP_INIT)
            bool swapTxRx{ false };
#endif
        };
    }

    class UartStmDuplexDma
        : public SerialCommunication
        , private InterruptHandler
    {
    public:
        using Config = detail::UartStmDuplexDmaConfig;

        template<std::size_t RxBufferSize>
        using WithRxBuffer = infra::WithStorage<UartStmDuplexDma, std::array<uint8_t, RxBufferSize>>;

        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config = Config());
        UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config = Config());

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
        void RegisterInterrupt(const Config& config);
        void TransferComplete();

        // Implementation of InterruptHandler
        void Invoke() override;

    private:
        infra::MemoryRange<uint8_t> rxBuffer;
        uint8_t uartIndex;
        PeripheralPinStm uartTx;
        PeripheralPinStm uartRx;
        PeripheralPinStm uartRts;
        PeripheralPinStm uartCts;

        hal::TransmitDmaChannel transmitDmaChannel;
        hal::CircularReceiveDmaChannel receiveDmaChannel;

        infra::Function<void()> transferDataComplete;
        infra::Function<void(infra::ConstByteRange data)> dataReceived;

        std::atomic<size_t> lastReceivedPosition{};
    };
}

#endif
