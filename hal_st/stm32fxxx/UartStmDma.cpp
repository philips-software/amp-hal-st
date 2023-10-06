#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BoundedVector.hpp"

namespace hal
{
    namespace
    {
        volatile void* PeripheralAddress(uint8_t uartIndex)
        {
#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G0) || defined(STM32G4)
            return &peripheralUart[uartIndex]->TDR;
#else
            return &peripheralUart[uartIndex]->DR;
#endif
        }
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, uartTx, uartRx, hal::dummyPinStm, hal::dummyPinStm, config, false }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, uartTx, uartRx, uartRts, uartCts, config, true }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config, bool hasFlowControl)
        : uartIndex(oneBasedIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, oneBasedIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, oneBasedIndex)
        , uartRts(uartRts, PinConfigTypeStm::uartRts, oneBasedIndex)
        , uartCts(uartCts, PinConfigTypeStm::uartCts, oneBasedIndex)
        , transmitDmaChannel{ transmitStream, PeripheralAddress(uartIndex), 1, [this]()
            {
                TransferComplete();
            } }
    {
        RegisterInterrupt(config);
        EnableClockUart(uartIndex);

        UART_HandleTypeDef uartHandle = {};
        uartHandle.Instance = peripheralUart[uartIndex];
        uartHandle.Init.BaudRate = config.baudrate;
        uartHandle.Init.WordLength = config.parity == USART_PARITY_NONE ? USART_WORDLENGTH_8B : USART_WORDLENGTH_9B;
        uartHandle.Init.StopBits = USART_STOPBITS_1;
        uartHandle.Init.Parity = config.parity;
        uartHandle.Init.Mode = USART_MODE_TX_RX;

        if (hasFlowControl)
            uartHandle.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
        else
            uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;

#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
#else
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
#endif
        HAL_UART_Init(&uartHandle);

        peripheralUart[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;
        peripheralUart[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::~UartStmDma()
    {
        peripheralUart[uartIndex]->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);
        DisableClockUart(uartIndex);
    }

    void UartStmDma::SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion)
    {
        if (!data.empty())
        {
            transferDataComplete = actionOnCompletion;
            transmitDmaChannel.StartTransmit(data);
        }
        else
            infra::EventDispatcher::Instance().Schedule(actionOnCompletion);
    }

    void UartStmDma::ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived)
    {
        this->dataReceived = dataReceived;

#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
        peripheralUart[uartIndex]->CR1 |= 1 << (USART_IT_RXNE & USART_IT_MASK);
#else
        peripheralUart[uartIndex]->CR1 |= USART_IT_RXNE & USART_IT_MASK;
#endif
    }

    void UartStmDma::RegisterInterrupt(const Config& config)
    {
        Register(peripheralUartIrq[uartIndex], config.priority);
    }

    void UartStmDma::TransferComplete()
    {
        if (transferDataComplete)
        {
            infra::Function<void()> callback = transferDataComplete;
            transferDataComplete = nullptr;
            infra::EventDispatcher::Instance().Schedule(callback);
        }
    }

    void UartStmDma::Invoke()
    {
        infra::BoundedVector<uint8_t>::WithMaxSize<8> buffer;

#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
        while (peripheralUart[uartIndex]->ISR & USART_ISR_NE)
#elif defined(STM32G0) || defined(STM32G4)
        while (peripheralUart[uartIndex]->ISR & USART_ISR_RXNE_RXFNE)
#else
        while (peripheralUart[uartIndex]->SR & USART_SR_RXNE)
#endif
        {
            uint8_t receivedByte =
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G0) || defined(STM32G4)
                peripheralUart[uartIndex]->RDR;
#else
                peripheralUart[uartIndex]->DR;
#endif
            buffer.push_back(receivedByte);
        }

        // If buffer is empty then interrupt was raised by Overrun Error (ORE) and we miss data.
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G0) || defined(STM32G4)
        really_assert(!(buffer.empty() && peripheralUart[uartIndex]->ISR & USART_ISR_ORE));
#else
        really_assert(!(buffer.empty() && peripheralUart[uartIndex]->SR & USART_SR_ORE));
#endif

        if (dataReceived != nullptr)
            dataReceived(buffer.range());
    }
}
