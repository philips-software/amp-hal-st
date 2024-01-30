#ifndef HAL_UART_STM_DMA_HPP
#define HAL_UART_STM_DMA_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"

namespace hal
{
    class UartStmDma
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
            uint32_t parity = USART_PARITY_NONE;
            infra::Optional<DmaChannelId> dmaChannelTx;
            infra::Optional<InterruptPriority> priority;
        };

        UartStmDma(hal::DmaStm& dmaStm, uint8_t uartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config = Config());
        UartStmDma(hal::DmaStm& dmaStm, uint8_t uartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config = Config());
        ~UartStmDma();

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
        infra::Optional<PeripheralPinStm> uartRts;
        infra::Optional<PeripheralPinStm> uartCts;

        UART_HandleTypeDef uartHandle = {};

        hal::DmaStm& dma;
        DmaStm::Stream transmitDmaChannel;

        infra::Function<void()> transferDataComplete;
        infra::Function<void(infra::ConstByteRange data)> dataReceived;
    };
}

#endif
