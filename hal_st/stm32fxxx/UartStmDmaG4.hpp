#ifndef HAL_UART_STM_DMA_HPP
#define HAL_UART_STM_DMA_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/DmaStmG4.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"

namespace hal
{
    class UartStmDmaG4
        : public SerialCommunication
        , private InterruptHandler
    {
    public:
        struct Config
        {
            Config(uint32_t baudrate, uint32_t hwFlowControl, uint32_t parity, DmaChannelIdG4 dmaChannelTx, infra::Optional<InterruptPriority> priority = {})
                : baudrate{ baudrate }
                , hwFlowControl{ hwFlowControl }
                , parity{ parity }
                , dmaChannelTx{ dmaChannelTx }
                , priority{ priority }
            {}

            Config(DmaChannelIdG4 dmaChannelTx, infra::Optional<InterruptPriority> priority = {})
                : dmaChannelTx{ dmaChannelTx }
                , priority{ priority }
            {}

            uint32_t baudrate = 115200;
            uint32_t hwFlowControl = UART_HWCONTROL_NONE;
            uint32_t parity = USART_PARITY_NONE;
            DmaChannelIdG4 dmaChannelTx;
            infra::Optional<InterruptPriority> priority;
        };

        UartStmDmaG4(hal::DmaStmG4& dmaStm, uint8_t uartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config);
        UartStmDmaG4(hal::DmaStmG4& dmaStm, uint8_t uartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config);
        ~UartStmDmaG4();

        virtual void SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion = infra::emptyFunction) override;
        virtual void ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived) override;

    private:
        void RegisterInterrupt(const Config& config);
        void TransferComplete();
        virtual void Invoke() override;

    private:
        uint8_t uartIndex;
        PeripheralPinStm uartTx;
        PeripheralPinStm uartRx;
        infra::Optional<PeripheralPinStm> uartRts;
        infra::Optional<PeripheralPinStm> uartCts;

        UART_HandleTypeDef uartHandle = {};

        hal::DmaStmG4& dma;
        DmaStmG4::Stream transmitDmaChannel;

        infra::Function<void()> transferDataComplete;
        infra::Function<void(infra::ConstByteRange data)> dataReceived;
    };
}

#endif
