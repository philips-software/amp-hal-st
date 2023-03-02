#ifndef HAL_UART_STM_HPP
#define HAL_UART_STM_HPP

#include "hal/interfaces/SerialCommunication.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/Optional.hpp"

namespace hal
{
#if defined(STM32WB)
    struct LpUart {};
    extern const LpUart lpUart;
#endif
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
        UartStm(uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config = Config());
#if defined(STM32WB)
        UartStm(uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, LpUart lpUart, const Config& config = Config());
        UartStm(uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, LpUart lpUart, const Config& config = Config());
#endif
        ~UartStm();

        virtual void SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion = infra::emptyFunction) override;
        virtual void ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived) override;

    private:
        void UartStmHalInit(const Config& config);
        void RegisterInterrupt(const Config& config);
        void TransferComplete();
        virtual void Invoke() override;

    private:
        uint8_t uartIndex;
        hal::PeripheralPinStm uartTx;
        hal::PeripheralPinStm uartRx;
        infra::Optional<hal::PeripheralPinStm> uartRts;
        infra::Optional<hal::PeripheralPinStm> uartCts;


        UART_HandleTypeDef uartHandle = {};

        infra::Function<void()> transferDataComplete;
        infra::Function<void(infra::ConstByteRange data)> dataReceived;

        infra::MemoryRange<const uint8_t> sendData;
        bool sending = false;

        infra::MemoryRange<USART_TypeDef* const> uartArray;
        infra::MemoryRange<IRQn_Type const> uartIrqArray;
    };
}

#endif
