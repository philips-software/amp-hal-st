#ifndef HAL_UART_STM_HPP
#define HAL_UART_STM_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"

namespace hal
{
    class UartStm
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
            infra::Optional<InterruptPriority> priority;
        };

        UartStm(uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config = Config());
        ~UartStm();

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

        UART_HandleTypeDef uartHandle = {};

        infra::Function<void()> transferDataComplete;
        infra::Function<void(infra::ConstByteRange data)> dataReceived;

        infra::MemoryRange<const uint8_t> sendData;
        bool sending = false;
    };
}

#endif
