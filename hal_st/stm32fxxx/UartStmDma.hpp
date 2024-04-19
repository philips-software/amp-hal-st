#ifndef HAL_UART_STM_DMA_HPP
#define HAL_UART_STM_DMA_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include <cstdint>

#include DEVICE_HEADER

namespace hal
{
    namespace detail
    {

        struct UartStmDmaConfig
        {
            uint32_t baudrate{ 115200 };
            uint32_t parity{ USART_PARITY_NONE };

            hal::InterruptPriority priority{ hal::InterruptPriority::Normal };

#if defined(UART_ADVFEATURE_SWAP_INIT)
            bool swapTxRx{ false };
#endif
        };
    }

    class UartStmDma
        : public SerialCommunication
        , private InterruptHandler
    {
    public:
        using Config = detail::UartStmDmaConfig;

        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config = Config());
        ~UartStmDma();

    private:
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config, bool hasFlowControl);

    public:
        void SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion = infra::emptyFunction) override;
        void ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived) override;

    private:
        void RegisterInterrupt(const Config& config);
        void TransferComplete();
        void Invoke() override;

    private:
        uint8_t uartIndex;
        PeripheralPinStm uartTx;
        PeripheralPinStm uartRx;
        PeripheralPinStm uartRts;
        PeripheralPinStm uartCts;

        TransmitDmaChannel transmitDmaChannel;

        infra::Function<void()> transferDataComplete;
        infra::Function<void(infra::ConstByteRange data)> dataReceived;
    };
}

#endif
