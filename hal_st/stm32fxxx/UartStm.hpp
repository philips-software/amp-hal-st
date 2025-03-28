#ifndef HAL_UART_STM_HPP
#define HAL_UART_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/interfaces/SerialCommunication.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include <cstdint>

namespace hal
{
#if defined(HAS_PERIPHERAL_LPUART)
    struct LpUart
    {};

    extern const LpUart lpUart;
#endif

    namespace detail
    {
        struct UartStmConfig
        {
            uint32_t baudrate{ 115200 };
            uint32_t parity{ USART_PARITY_NONE };
            InterruptPriority priority{ InterruptPriority::Normal };

#if defined(UART_ADVFEATURE_SWAP_INIT)
            bool swapTxRx{ false };
#endif
        };
    }

    class UartStm
        : public SerialCommunication
        , private InterruptHandler
    {
    public:
        using Config = detail::UartStmConfig;

        UartStm(uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config = Config());
#if defined(HAS_PERIPHERAL_LPUART)
        UartStm(uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, LpUart lpUart, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, LpUart lpUart, const Config& config = Config());
#endif
    private:
        UartStm(uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config, bool hasFlowControl);
#if defined(HAS_PERIPHERAL_LPUART)
        UartStm(uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, LpUart lpUart, const Config& config, bool hasFlowControl);
#endif

    public:
        ~UartStm();

        void SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion = infra::emptyFunction) override;
        void ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived) override;

    protected:
        uint8_t uartIndex;
        infra::Function<void(infra::ConstByteRange data)> dataReceived;
        infra::MemoryRange<USART_TypeDef* const> uartArray;

    private:
        void UartStmHalInit(const Config& config, bool hasFlowControl);
        void RegisterInterrupt(const Config& config);
        void TransferComplete();
        void Invoke() override;

    private:
        hal::PeripheralPinStm uartTx;
        hal::PeripheralPinStm uartRx;
        hal::PeripheralPinStm uartRts;
        hal::PeripheralPinStm uartCts;

        UART_HandleTypeDef uartHandle = {};

        infra::Function<void()> transferDataComplete;

        infra::MemoryRange<const uint8_t> sendData;
        bool sending = false;
        infra::MemoryRange<IRQn_Type const> uartIrqArray;
    };
}

#endif
