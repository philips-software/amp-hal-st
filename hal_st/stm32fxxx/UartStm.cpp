#include "hal_st/stm32fxxx/UartStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <cstdint>
#include <utility>

#include DEVICE_HEADER

namespace hal
{
    UartStm::UartStm(uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, const Config& config)
        : UartStm{ oneBasedIndex, GpioPins{ tx, rx }, config }
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, GpioPinStm& rts, GpioPinStm& cts, const Config& config)
        : UartStm{ oneBasedIndex, GpioPinsFlowCtrl{ tx, rx, rts, cts }, config }
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, const GpioPins& pins, const Config& config)
        : UartStm{ oneBasedIndex, GpioOwnersFlowCtrl{ GpioPinStmOwner{ pins.tx }, GpioPinStmOwner{ pins.rx }, GpioPinStmOwner{ dummyPinStm }, GpioPinStmOwner{ dummyPinStm } }, config, false }
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, const GpioPinsFlowCtrl& pins, const Config& config)
        : UartStm{ oneBasedIndex, GpioOwnersFlowCtrl{ GpioPinStmOwner{ pins.tx }, GpioPinStmOwner{ pins.rx }, GpioPinStmOwner{ pins.rts }, GpioPinStmOwner{ pins.cts } }, config, true }
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, GpioOwners&& gpioOwners, const Config& config)
        : UartStm{ oneBasedIndex, GpioOwnersFlowCtrl{ std::move(gpioOwners.txOwner), std::move(gpioOwners.rxOwner), GpioPinStmOwner{ dummyPinStm }, GpioPinStmOwner{ dummyPinStm } }, config, false }
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, const Config& config)
        : UartStm{ oneBasedIndex, std::move(gpioOwnersFlowCtrl), config, true }
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, const Config& config, bool hasFlowControl)
        : uartIndex{ static_cast<uint8_t>(oneBasedIndex - 1) }
        , uartArray{ peripheralUart }
        , uartTx{ std::move(gpioOwnersFlowCtrl.txOwner), PinConfigTypeStm::uartTx, oneBasedIndex }
        , uartRx{ std::move(gpioOwnersFlowCtrl.rxOwner), PinConfigTypeStm::uartRx, oneBasedIndex }
        , uartRts{ std::move(gpioOwnersFlowCtrl.rtsOwner), PinConfigTypeStm::uartRts, oneBasedIndex }
        , uartCts{ std::move(gpioOwnersFlowCtrl.ctsOwner), PinConfigTypeStm::uartCts, oneBasedIndex }
        , uartIrqArray{ peripheralUartIrq }
    {
        really_assert(oneBasedIndex != 0); // first element is 1 in oneBasedIndex
        RegisterInterrupt(config);
        EnableClockUart(uartIndex);
        UartStmHalInit(config, hasFlowControl);
    }

#if defined(HAS_PERIPHERAL_LPUART)
    UartStm::UartStm(uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, LpUart lpUart, const Config& config)
        : UartStm{ oneBasedIndex, GpioPins{ tx, rx }, lpUart, config }
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, GpioPinStm& rts, GpioPinStm& cts, LpUart lpUart, const Config& config)
        : UartStm{ oneBasedIndex, GpioPinsFlowCtrl{ tx, rx, rts, cts }, lpUart, config }
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, const GpioPins& pins, LpUart lpUart, const Config& config)
        : UartStm(oneBasedIndex, GpioOwnersFlowCtrl{ GpioPinStmOwner{ pins.tx }, GpioPinStmOwner{ pins.rx }, GpioPinStmOwner{ dummyPinStm }, GpioPinStmOwner{ dummyPinStm } }, lpUart, config, false)
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, const GpioPinsFlowCtrl& pins, LpUart lpUart, const Config& config)
        : UartStm(oneBasedIndex, GpioOwnersFlowCtrl{ GpioPinStmOwner{ pins.tx }, GpioPinStmOwner{ pins.rx }, GpioPinStmOwner{ pins.rts }, GpioPinStmOwner{ pins.cts } }, lpUart, config, true)
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, GpioOwners&& gpioOwners, LpUart lpUart, const Config& config)
        : UartStm(oneBasedIndex, GpioOwnersFlowCtrl{ std::move(gpioOwners.txOwner), std::move(gpioOwners.rxOwner), GpioPinStmOwner{ dummyPinStm }, GpioPinStmOwner{ dummyPinStm } }, lpUart, config, false)
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, LpUart lpUart, const Config& config)
        : UartStm(oneBasedIndex, std::move(gpioOwnersFlowCtrl), lpUart, config, true)
    {}

    UartStm::UartStm(uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, LpUart lpUart, const Config& config, bool hasFlowControl)
        : uartIndex{ static_cast<uint8_t>(oneBasedIndex - 1) }
        , uartArray{ peripheralLpuart }
        , uartTx{ std::move(gpioOwnersFlowCtrl.txOwner), PinConfigTypeStm::uartTx, oneBasedIndex }
        , uartRx{ std::move(gpioOwnersFlowCtrl.rxOwner), PinConfigTypeStm::uartRx, oneBasedIndex }
        , uartRts{ std::move(gpioOwnersFlowCtrl.rtsOwner), PinConfigTypeStm::uartRts, oneBasedIndex }
        , uartCts{ std::move(gpioOwnersFlowCtrl.ctsOwner), PinConfigTypeStm::uartCts, oneBasedIndex }
        , isLpUart{ true }
        , uartIrqArray{ peripheralLpuartIrq }
    {
        really_assert(oneBasedIndex != 0); // first element is 1 in oneBasedIndex
        RegisterInterrupt(config);
        EnableClockLpuart(uartIndex);
        UartStmHalInit(config, hasFlowControl);
    }
#endif

    void UartStm::UartStmHalInit(const Config& config, bool hasFlowControl)
    {
        uartHandle.Instance = uartArray[uartIndex];
        uartHandle.Init.BaudRate = config.baudrate;
        uartHandle.Init.WordLength = config.parity == USART_PARITY_NONE ? USART_WORDLENGTH_8B : USART_WORDLENGTH_9B;
        uartHandle.Init.StopBits = USART_STOPBITS_1;
        uartHandle.Init.Parity = config.parity;
        uartHandle.Init.Mode = USART_MODE_TX;
        uartHandle.Init.HwFlowCtl = hasFlowControl ? UART_HWCONTROL_RTS_CTS : UART_HWCONTROL_NONE;
#if defined(UART_ONE_BIT_SAMPLE_ENABLE)
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
#endif
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;

#if defined(UART_ADVFEATURE_NO_INIT)
        uartHandle.AdvancedInit = {};

#if defined(UART_ADVFEATURE_SWAP_INIT)
        if (config.swapTxRx)
        {
            uartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
            uartHandle.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
        }
#endif
#endif

        HAL_UART_Init(&uartHandle);

#if defined(STM32WB) || defined(STM32WBA)
        // Enable FIFO. FIFO can only be enabled after 'HAL_UART_Init'; but UART must be disabled first.
        uartArray[uartIndex]->CR1 &= ~USART_CR1_UE;
        uartArray[uartIndex]->CR1 |= USART_CR1_FIFOEN | USART_CR1_UE;
#endif

        uartArray[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;
    }

    UartStm::~UartStm()
    {
        uartArray[uartIndex]->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);

#if defined(HAS_PERIPHERAL_LPUART)
        if (isLpUart)
            DisableClockLpuart(uartIndex);
        else
#endif
            DisableClockUart(uartIndex);
    }

    void UartStm::SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion)
    {
        transferDataComplete = actionOnCompletion;
        sendData = data;
        sending = true;

        __DMB();

        uartArray[uartIndex]->CR1 |= USART_CR1_TXEIE;
    }

    void UartStm::ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived)
    {
        this->dataReceived = dataReceived;

        if (dataReceived == nullptr)
            uartArray[uartIndex]->CR1 &= ~(USART_CR1_RE | USART_CR1_RXNEIE);
        else
            uartArray[uartIndex]->CR1 |= USART_CR1_RE | USART_CR1_RXNEIE;
    }

    hal::PeripheralPinStm& UartStm::TxPin()
    {
        return uartTx;
    }

    hal::PeripheralPinStm& UartStm::RxPin()
    {
        return uartRx;
    }

    hal::PeripheralPinStm& UartStm::RtsPin()
    {
        return uartRts;
    }

    hal::PeripheralPinStm& UartStm::CtsPin()
    {
        return uartCts;
    }

    void UartStm::RegisterInterrupt(const Config& config)
    {
        Register(uartIrqArray[uartIndex], config.priority);
    }

    void UartStm::TransferComplete()
    {
        infra::Function<void()> callback = transferDataComplete;
        transferDataComplete = nullptr;
        infra::EventDispatcher::Instance().Schedule(callback);
    }

    void UartStm::Invoke()
    {
#if defined(USART_ISR_RXNE)
        if (uartArray[uartIndex]->ISR & USART_ISR_RXNE)
#else
        if (uartArray[uartIndex]->SR & USART_SR_RXNE)
#endif
        {
            infra::BoundedVector<uint8_t>::WithMaxSize<8> buffer;

#if defined(USART_ISR_RXNE)
            while (!buffer.full() && (uartArray[uartIndex]->ISR & USART_ISR_RXNE))
#else
            while (!buffer.full() && (uartArray[uartIndex]->SR & USART_SR_RXNE))
#endif
            {
                uint8_t receivedByte =
#if defined(USART_RDR_RDR)
                    uartArray[uartIndex]->RDR;
#else
                    uartArray[uartIndex]->DR;
#endif
                buffer.push_back(receivedByte);
            }

            // If buffer is empty then interrupt was raised by Overrun Error (ORE) and we miss data.
#if defined(USART_ISR_ORE)
            really_assert(!(uartArray[uartIndex]->ISR & USART_ISR_ORE));
#else
            really_assert(!(uartArray[uartIndex]->SR & USART_SR_ORE));
#endif

            if (dataReceived != nullptr)
                dataReceived(buffer.range());
        }

        if (sending && ((uartArray[uartIndex]->CR1 & USART_CR1_TXEIE) != 0))
        {
#if defined(USART_ISR_TXE)
            while (!sendData.empty() && (uartArray[uartIndex]->ISR & USART_ISR_TXE))
#else
            while (!sendData.empty() && (uartArray[uartIndex]->SR & USART_SR_TXE))
#endif
            {
#if defined(USART_TDR_TDR)
                uartArray[uartIndex]->TDR = sendData.front();
#else
                uartArray[uartIndex]->DR = sendData.front();
#endif
                sendData.pop_front();
            }

            if (sendData.empty())
            {
                TransferComplete();
                uartArray[uartIndex]->CR1 &= ~USART_CR1_TXEIE;
                sending = false;
            }
        }
    }
}
