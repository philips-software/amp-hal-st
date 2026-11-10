#ifndef HAL_UART_STM_HPP
#define HAL_UART_STM_HPP

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
        struct GpioPins
        {
            GpioPinStm& tx;
            GpioPinStm& rx;
        };

        struct GpioPinsFlowCtrl
        {
            GpioPinStm& tx;
            GpioPinStm& rx;
            GpioPinStm& rts;
            GpioPinStm& cts;
        };

        struct GpioOwners
        {
            GpioPinStmOwner txOwner;
            GpioPinStmOwner rxOwner;
        };

        struct GpioOwnersFlowCtrl
        {
            GpioPinStmOwner txOwner;
            GpioPinStmOwner rxOwner;
            GpioPinStmOwner rtsOwner;
            GpioPinStmOwner ctsOwner;
        };

        using Config = detail::UartStmConfig;

        [[deprecated("Use UartStm with GpioPins")]] UartStm(uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, const Config& config = Config());
        [[deprecated("Use UartStm with GpioPinsFlowCtrl")]] UartStm(uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, GpioPinStm& rts, GpioPinStm& cts, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, const GpioPins& pins, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, const GpioPinsFlowCtrl& pins, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, GpioOwners&& gpioOwners, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, const Config& config = Config());

#if defined(HAS_PERIPHERAL_LPUART)
        [[deprecated("Use UartStm with GpioPins and LpUart")]] UartStm(uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, LpUart lpUart, const Config& config = Config());
        [[deprecated("Use UartStm with GpioPinsFlowCtrl and LpUart")]] UartStm(uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, GpioPinStm& rts, GpioPinStm& cts, LpUart lpUart, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, const GpioPins& pins, LpUart lpUart, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, const GpioPinsFlowCtrl& pins, LpUart lpUart, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, GpioOwners&& gpioOwners, LpUart lpUart, const Config& config = Config());
        UartStm(uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, LpUart lpUart, const Config& config = Config());
#endif

    private:
        UartStm(uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, const Config& config, bool hasFlowControl);
#if defined(HAS_PERIPHERAL_LPUART)
        UartStm(uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, LpUart lpUart, const Config& config, bool hasFlowControl);
#endif

    public:
        ~UartStm();

        void SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion = infra::emptyFunction) override;
        void ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived) override;

        hal::PeripheralPinStm& TxPin();
        hal::PeripheralPinStm& RxPin();
        hal::PeripheralPinStm& RtsPin();
        hal::PeripheralPinStm& CtsPin();

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
#if defined(HAS_PERIPHERAL_LPUART)
        bool isLpUart = false;
#endif
        infra::MemoryRange<IRQn_Type const> uartIrqArray;
    };
}

#endif
