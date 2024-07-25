#include "hal_st/synchronous_stm32fxxx/SynchronousUartStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"

namespace hal
{
    SynchronousUartStm::SynchronousUartStm(infra::ByteRange readBuffer, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, TimeKeeper& timeKeeper, uint32_t baudrate)
        : SynchronousUartStm(readBuffer, aUartIndex, uartTx, uartRx, uartTx, uartRx, timeKeeper, HwFlowControl::hwControlDisable, baudrate)
    {}

    SynchronousUartStm::SynchronousUartStm(infra::ByteRange readBuffer, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, TimeKeeper& timeKeeper,
        HwFlowControl flowControl, uint32_t baudrate)
        : uartIndex(aUartIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, aUartIndex)
        , timeKeeper(timeKeeper)
        , readBuffer(readBuffer)
        , contentsBegin(readBuffer.begin())
        , contentsEnd(readBuffer.begin())
    {
        if (flowControl != HwFlowControl::hwControlDisable)
        {
            this->uartRts.Emplace(uartRts, PinConfigTypeStm::uartRts, aUartIndex);
            this->uartCts.Emplace(uartCts, PinConfigTypeStm::uartCts, aUartIndex);
        }
        Register(peripheralUartIrq[uartIndex]);
        EnableClockUart(uartIndex);

        UART_HandleTypeDef uartHandle = {};

        uartHandle.Instance = peripheralUart[uartIndex];
        uartHandle.Init.BaudRate = baudrate;
        uartHandle.Init.WordLength = USART_WORDLENGTH_8B;
        uartHandle.Init.StopBits = USART_STOPBITS_1;
        uartHandle.Init.Parity = USART_PARITY_NONE;
        uartHandle.Init.Mode = USART_MODE_TX_RX;
        uartHandle.Init.HwFlowCtl = flowControl;
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
        uartHandle.Init.OverSampling = USART_OVERSAMPLING_8;
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
#else
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
#endif
        HAL_UART_Init(&uartHandle);

        peripheralUart[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;

#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4) || defined(STM32G0)
        peripheralUart[uartIndex]->CR1 |= 1 << (USART_IT_RXNE & USART_IT_MASK);
#else
        peripheralUart[uartIndex]->CR1 |= USART_IT_RXNE & USART_IT_MASK;
#endif
    }

    SynchronousUartStm::~SynchronousUartStm()
    {
        peripheralUart[uartIndex]->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);
    }

    void SynchronousUartStm::SendData(infra::ConstByteRange data)
    {
        for (uint8_t byte : data)
        {
#if defined(USART_ISR_TXE)
            while ((peripheralUart[uartIndex]->ISR & USART_ISR_TXE) == 0)
            {}

            peripheralUart[uartIndex]->TDR = byte;
#else
            while ((peripheralUart[uartIndex]->SR & USART_SR_TXE) == 0)
            {}

            peripheralUart[uartIndex]->DR = byte;
#endif
        }

#if defined(USART_ISR_TXE)
        while ((peripheralUart[uartIndex]->ISR & USART_ISR_TXE) == 0)
        {}
#else
        while ((peripheralUart[uartIndex]->SR & USART_SR_TXE) == 0)
        {}
#endif
    }

    bool SynchronousUartStm::ReceiveData(infra::ByteRange data)
    {
        timeKeeper.Reset();

        for (uint8_t& byte : data)
        {
            while (Empty())
            {
                if (timeKeeper.Timeout())
                    return false;
            }

            byte = *contentsBegin;

            if (contentsBegin == readBuffer.end() - 1)
                contentsBegin = readBuffer.begin();
            else
                ++contentsBegin;
        }

        return true;
    }

    void SynchronousUartStm::Invoke()
    {
#if defined(USART_ISR_RXNE)
        while (peripheralUart[uartIndex]->ISR & USART_ISR_RXNE)
#else
        while (peripheralUart[uartIndex]->SR & USART_SR_RXNE)
#endif
        {
#if defined(USART_RDR_RDR)
            uint8_t received = peripheralUart[uartIndex]->RDR;
#else
            uint8_t received = peripheralUart[uartIndex]->DR;
#endif
            if (!Full())
            {
                *contentsEnd = received;

                if (contentsEnd == readBuffer.end() - 1)
                    contentsEnd = readBuffer.begin();
                else
                    ++contentsEnd;
            }
        }
    }

    bool SynchronousUartStm::Full() const
    {
        return (contentsEnd == readBuffer.end() - 1 || contentsBegin == contentsEnd + 1) && (contentsEnd != readBuffer.end() - 1 || contentsBegin == readBuffer.begin());
    }

    bool SynchronousUartStm::Empty() const
    {
        return contentsBegin.load() == contentsEnd.load();
    }

    SynchronousUartStmSendOnly::SynchronousUartStmSendOnly(uint8_t aUartIndex, GpioPinStm& uartTx, uint32_t baudrate)
        : SynchronousUartStmSendOnly(aUartIndex, uartTx, uartTx, HwFlowControl::hwControlDisable, baudrate)
    {}

    SynchronousUartStmSendOnly::SynchronousUartStmSendOnly(uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRts,
        HwFlowControl flowControl, uint32_t baudrate)
        : uartBase(peripheralUart[aUartIndex - 1])
        , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
    {
        EnableClockUart(aUartIndex - 1);

        if (flowControl != HwFlowControl::hwControlDisable)
            this->uartRts.Emplace(uartRts, PinConfigTypeStm::uartRts, aUartIndex);

        UartStmHalInit(flowControl, baudrate);
    }

#if defined(STM32WB)
    SynchronousUartStmSendOnly::SynchronousUartStmSendOnly(uint8_t aUartIndex, GpioPinStm& uartTx, SyncLpUart lpUart, uint32_t baudrate)
        : SynchronousUartStmSendOnly(aUartIndex, uartTx, uartTx, lpUart, HwFlowControl::hwControlDisable, baudrate)
    {}

    SynchronousUartStmSendOnly::SynchronousUartStmSendOnly(uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRts, SyncLpUart lpUart, HwFlowControl flowControl, uint32_t baudrate)
        : uartBase(peripheralLpuart[aUartIndex - 1])
        , uartTx(uartTx, PinConfigTypeStm::lpuartTx, aUartIndex)
    {
        EnableClockLpuart(aUartIndex - 1);

        if (flowControl != HwFlowControl::hwControlDisable)
            this->uartRts.Emplace(uartRts, PinConfigTypeStm::lpuartRts, aUartIndex);

        UartStmHalInit(flowControl, baudrate);
    }
#endif

    SynchronousUartStmSendOnly::~SynchronousUartStmSendOnly()
    {
        uartBase->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);
    }

    void SynchronousUartStmSendOnly::SendData(infra::ConstByteRange data)
    {
        for (uint8_t byte : data)
        {
#if defined(USART_ISR_TXE)
            while ((uartBase->ISR & USART_ISR_TXE) == 0)
            {}

            uartBase->TDR = byte;
#else
            while ((uartBase->SR & USART_SR_TXE) == 0)
            {}

            uartBase->DR = byte;
#endif
        }

#if defined(USART_ISR_TXE)
        while ((uartBase->ISR & USART_ISR_TXE) == 0)
        {}
#else
        while ((uartBase->SR & USART_SR_TXE) == 0)
        {}
#endif
    }

    bool SynchronousUartStmSendOnly::ReceiveData(infra::ByteRange data)
    {
        return false;
    }

    void SynchronousUartStmSendOnly::UartStmHalInit(HwFlowControl flowControl, uint32_t baudrate)
    {
        UART_HandleTypeDef uartHandle = {};

        uartHandle.Instance = uartBase;
        uartHandle.Init.BaudRate = baudrate;
        uartHandle.Init.WordLength = USART_WORDLENGTH_8B;
        uartHandle.Init.StopBits = USART_STOPBITS_1;
        uartHandle.Init.Parity = USART_PARITY_NONE;
        uartHandle.Init.Mode = USART_MODE_TX_RX;
        uartHandle.Init.HwFlowCtl = flowControl;
#if defined(USART_OVERSAMPLING_8)
        uartHandle.Init.OverSampling = USART_OVERSAMPLING_8;
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
#else
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
#endif
        HAL_UART_Init(&uartHandle);

        uartBase->CR2 &= ~USART_CLOCK_ENABLED;

#if defined(STM32F4) || defined(STM32G0)
        uartBase->CR1 |= USART_IT_RXNE & USART_IT_MASK;
#else
        uartBase->CR1 |= 1 << (USART_IT_RXNE & USART_IT_MASK);
#endif
    }
}
