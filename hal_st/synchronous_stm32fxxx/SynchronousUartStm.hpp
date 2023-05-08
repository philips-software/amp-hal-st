#ifndef SYNCHRONOUS_HAL_SYNCHRONOUS_UART_STM_HPP
#define SYNCHRONOUS_HAL_SYNCHRONOUS_UART_STM_HPP

#include "hal/synchronous_interfaces/SynchronousSerialCommunication.hpp"
#include "hal/synchronous_interfaces/TimeKeeper.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/WithStorage.hpp"
#include <atomic>

namespace hal
{
#if defined(STM32WB)
    struct SyncLpUart
    {};

    extern const SyncLpUart syncLpUart;
#endif
    class SynchronousUartStm
        : public SynchronousSerialCommunication
        , private InterruptHandler
    {
    public:
        enum HwFlowControl : uint32_t
        {
            hwControlDisable = UART_HWCONTROL_NONE,
            hwControlRtsEnable = UART_HWCONTROL_RTS,
            hwControlCtsEnable = UART_HWCONTROL_CTS,
            hwControlRtsCtsEnable = UART_HWCONTROL_RTS_CTS
        };

        template<std::size_t Size>
        using WithStorage = infra::WithStorage<SynchronousUartStm, std::array<uint8_t, Size>>;

        SynchronousUartStm(infra::ByteRange readBuffer, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, TimeKeeper& timeKeeper, uint32_t baudrate = 115200);
        SynchronousUartStm(infra::ByteRange readBuffer, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, TimeKeeper& timeKeeper,
            HwFlowControl flowControl = HwFlowControl::hwControlRtsCtsEnable, uint32_t baudrate = 115200);

        ~SynchronousUartStm();

        void SendData(infra::ConstByteRange data) override;
        bool ReceiveData(infra::ByteRange data) override;

    private:
        virtual void Invoke() override;

        bool Full() const;
        bool Empty() const;

    private:
        uint8_t uartIndex;
        PeripheralPinStm uartTx;
        PeripheralPinStm uartRx;
        infra::Optional<PeripheralPinStm> uartRts;
        infra::Optional<PeripheralPinStm> uartCts;
        TimeKeeper& timeKeeper;
        infra::ByteRange readBuffer;
        std::atomic<uint8_t*> contentsBegin;
        std::atomic<uint8_t*> contentsEnd;
    };

    class SynchronousUartStmSendOnly
        : public SynchronousSerialCommunication
    {
    public:
        enum HwFlowControl : uint32_t
        {
            hwControlDisable = UART_HWCONTROL_NONE,
            hwControlRtsEnable = UART_HWCONTROL_RTS,
            hwControlCtsEnable = UART_HWCONTROL_CTS,
            hwControlRtsCtsEnable = UART_HWCONTROL_RTS_CTS
        };

        SynchronousUartStmSendOnly(uint8_t aUartIndex, GpioPinStm& uartTx, uint32_t baudrate = 115200);
        SynchronousUartStmSendOnly(uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRts,
            HwFlowControl flowControl = HwFlowControl::hwControlRtsCtsEnable, uint32_t baudrate = 115200);
#if defined(STM32WB)
        SynchronousUartStmSendOnly(uint8_t aUartIndex, GpioPinStm& uartTx, SyncLpUart lpUart, uint32_t baudrate = 115200);
        SynchronousUartStmSendOnly(uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRts, SyncLpUart lpUart, HwFlowControl flowControl = HwFlowControl::hwControlRtsCtsEnable, uint32_t baudrate = 115200);
#endif
        ~SynchronousUartStmSendOnly();

        void SendData(infra::ConstByteRange data) override;
        bool ReceiveData(infra::ByteRange data) override;

    private:
        void UartStmHalInit(HwFlowControl flowControl, uint32_t baudrate);

        USART_TypeDef* const uartBase;
        PeripheralPinStm uartTx;
        infra::Optional<PeripheralPinStm> uartRts;
    };
}

#endif
