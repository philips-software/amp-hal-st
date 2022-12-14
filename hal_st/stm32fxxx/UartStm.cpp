#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BoundedVector.hpp"

namespace hal
{
    UartStm::UartStm(uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
    : uartIndex(aUartIndex - 1)
    , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
    , uartRx(uartRx, PinConfigTypeStm::uartRx, aUartIndex)
    , uartHandle()
    {
        RegisterInterrupt(config);
        EnableClockUart(uartIndex);

        uartHandle.Instance = peripheralUart[uartIndex];
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

        peripheralUart[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;
    }

    UartStm::~UartStm()
    {
        peripheralUart[uartIndex]->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);
        DisableClockUart(uartIndex);
    }

    void UartStm::SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion)
    {
        transferDataComplete = actionOnCompletion;
        sendData = data;
        sending = true;

#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
        peripheralUart[uartIndex]->CR1 |= 1 << (USART_IT_TXE & USART_IT_MASK);
#else
        peripheralUart[uartIndex]->CR1 |= USART_IT_TXE & USART_IT_MASK;
#endif
    }

    void UartStm::ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived)
    {
        this->dataReceived = dataReceived;

#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
    if (dataReceived == nullptr)
        peripheralUart[uartIndex]->CR1 &= ~(1 << (USART_IT_RXNE & USART_IT_MASK));
    else
        peripheralUart[uartIndex]->CR1 |= 1 << (USART_IT_RXNE & USART_IT_MASK);
#else
    if (dataReceived == nullptr)
        peripheralUart[uartIndex]->CR1 &= ~(USART_IT_RXNE & USART_IT_MASK);
    else
        peripheralUart[uartIndex]->CR1 |= USART_IT_RXNE & USART_IT_MASK;
#endif
    }

    void UartStm::RegisterInterrupt(const Config& config)
    {
        if (config.priority)
            Register(peripheralUartIrq[uartIndex], *config.priority);
        else
            Register(peripheralUartIrq[uartIndex]);
    }

    void UartStm::TransferComplete()
    {
        infra::EventDispatcher::Instance().Schedule(transferDataComplete);
        transferDataComplete = nullptr;
    }

    void UartStm::Invoke()
    {
        if (peripheralUart[uartIndex]->ISR & USART_ISR_RXNE)
        {
            infra::BoundedVector<uint8_t>::WithMaxSize<8> buffer;

            while (!buffer.full() && (peripheralUart[uartIndex]->ISR & USART_ISR_RXNE))
            {
                uint8_t receivedByte =
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
                peripheralUart[uartIndex]->RDR;
#else
                peripheralUart[uartIndex]->DR;
#endif
                buffer.push_back(receivedByte);
            }
            
            // If buffer is empty then interrupt was raised by Overrun Error (ORE) and we miss data.
            really_assert(!(buffer.empty() && peripheralUart[uartIndex]->ISR & USART_ISR_ORE));
            
            if (dataReceived != nullptr)
                dataReceived(buffer.range());
        }

        if (sending)
        {
            while (!sendData.empty() && (peripheralUart[uartIndex]->ISR & USART_ISR_TXE))
            {
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
                peripheralUart[uartIndex]->TDR = sendData.front();
#else
                peripheralUart[uartIndex]->DR = sendData.front();
#endif
                sendData.pop_front();
            }

            if (sendData.empty())
            {
                TransferComplete();
                peripheralUart[uartIndex]->CR1 &= ~(1 << (USART_IT_TXE & USART_IT_MASK));
                sending = false;
            }
        }
    }
}
