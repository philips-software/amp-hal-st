#include "hal_st/stm32fxxx/UartStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BoundedVector.hpp"

namespace hal
{
    UartStm::UartStm(uint8_t aUartIndex, GpioPinStm& uartTxPin, GpioPinStm& uartRxPin, const Config& config)
        : uartIndex(aUartIndex - 1)
        , uartTx(uartTxPin, PinConfigTypeStm::uartTx, aUartIndex)
        , uartRx(uartRxPin, PinConfigTypeStm::uartRx, aUartIndex)
        , uartHandle()
    {
        uartArray = peripheralUart;
        uartIrqArray = peripheralUartIrq;
        RegisterInterrupt(config);
        EnableClockUart(uartIndex);
        UartStmHalInit(config);
    }

    UartStm::UartStm(uint8_t aUartIndex, GpioPinStm& uartTxPin, GpioPinStm& uartRxPin, GpioPinStm& uartRtsPin, GpioPinStm& uartCtsPin, const Config& config)
        : UartStm(aUartIndex, uartTxPin, uartRxPin, config)
    {
        uartRts.Emplace(uartRtsPin, PinConfigTypeStm::uartRts, aUartIndex);
        uartCts.Emplace(uartCtsPin, PinConfigTypeStm::uartCts, aUartIndex);
    }

#if defined(STM32WB)
    UartStm::UartStm(uint8_t aUartIndex, GpioPinStm& uartTxPin, GpioPinStm& uartRxPin, LpUart lpUart, const Config& config)
    : uartIndex(aUartIndex - 1)
    , uartTx(uartTxPin, PinConfigTypeStm::lpuartTx, aUartIndex)
    , uartRx(uartRxPin, PinConfigTypeStm::lpuartRx, aUartIndex)
    , uartHandle()
    {
        uartArray = peripheralLpuart;
        uartIrqArray = peripheralLpuartIrq;
        RegisterInterrupt(config);
        EnableClockLpuart(uartIndex);
        UartStmHalInit(config);
    }

    UartStm::UartStm(uint8_t aUartIndex, GpioPinStm& uartTxPin, GpioPinStm& uartRxPin, GpioPinStm& uartRtsPin, GpioPinStm& uartCtsPin, LpUart lpUart, const Config& config)
    : UartStm(aUartIndex, uartTxPin, uartRxPin, lpUart, config)
    {
        uartRts.Emplace(uartRtsPin, PinConfigTypeStm::lpuartRts, aUartIndex);
        uartCts.Emplace(uartCtsPin, PinConfigTypeStm::lpuartCts, aUartIndex);
    }
#endif

    void UartStm::UartStmHalInit(const Config& config)
    {
        uartHandle.Instance = uartArray[uartIndex];
        uartHandle.Init.BaudRate = config.baudrate;
        uartHandle.Init.WordLength = config.parity == USART_PARITY_NONE ? USART_WORDLENGTH_8B : USART_WORDLENGTH_9B;
        uartHandle.Init.StopBits = USART_STOPBITS_1;
        uartHandle.Init.Parity = config.parity;
        uartHandle.Init.Mode = USART_MODE_TX_RX;
        uartHandle.Init.HwFlowCtl = config.hwFlowControl;
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
#else
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
#endif

        HAL_UART_Init(&uartHandle);

#if defined(STM32WB)
        // Enable FIFO. FIFO can only be enabled after 'HAL_UART_Init'; but UART must be disabled first.
        uartArray[uartIndex]->CR1 &= ~USART_CR1_UE;
        uartArray[uartIndex]->CR1 |= USART_CR1_FIFOEN | USART_CR1_UE;
#endif

        uartArray[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;
    }

    UartStm::~UartStm()
    {
        uartArray[uartIndex]->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);
        DisableClockUart(uartIndex);
    }

    void UartStm::SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion)
    {
        transferDataComplete = actionOnCompletion;
        sendData = data;
        sending = true;

        uartArray[uartIndex]->CR1 |= USART_CR1_TXEIE;
    }

    void UartStm::ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived)
    {
        this->dataReceived = dataReceived;

        if (dataReceived == nullptr)
            uartArray[uartIndex]->CR1 &= ~USART_CR1_RXNEIE;
        else
            uartArray[uartIndex]->CR1 |= USART_CR1_RXNEIE;
    }

    void UartStm::RegisterInterrupt(const Config& config)
    {
        if (config.priority)
            Register(uartIrqArray[uartIndex], *config.priority);
        else
            Register(uartIrqArray[uartIndex]);
    }

    void UartStm::TransferComplete()
    {
        infra::EventDispatcher::Instance().Schedule(transferDataComplete);
        transferDataComplete = nullptr;
    }

    void UartStm::Invoke()
    {
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
        if (uartArray[uartIndex]->ISR & USART_ISR_RXNE)
#else
        if (uartArray[uartIndex]->SR & USART_SR_RXNE)
#endif
        {
            infra::BoundedVector<uint8_t>::WithMaxSize<8> buffer;

#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
            while (!buffer.full() && (uartArray[uartIndex]->ISR & USART_ISR_RXNE))
#else
            while (!buffer.full() && (uartArray[uartIndex]->SR & USART_SR_RXNE))
#endif
            {
                uint8_t receivedByte =
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
                    uartArray[uartIndex]->RDR;
#else
                    uartArray[uartIndex]->DR;
#endif
                buffer.push_back(receivedByte);
            }

            // If buffer is empty then interrupt was raised by Overrun Error (ORE) and we miss data.
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
            really_assert(!(uartArray[uartIndex]->ISR & USART_ISR_ORE));
#else
            really_assert(!(uartArray[uartIndex]->SR & USART_SR_ORE));
#endif

            if (dataReceived != nullptr)
                dataReceived(buffer.range());
        }

        if (sending)
        {
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
            while (!sendData.empty() && (uartArray[uartIndex]->ISR & USART_ISR_TXE))
#else
            while (!sendData.empty() && (uartArray[uartIndex]->SR & USART_SR_TXE))
#endif
            {
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
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
