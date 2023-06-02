#include "hal_st/stm32fxxx/UartStmDmaG4.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BoundedVector.hpp"

namespace hal
{
    UartStmDmaG4::UartStmDmaG4(hal::DmaStmG4& dma, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
        : uartIndex(aUartIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, aUartIndex)
        , uartHandle()
        , dma(dma)
        , transmitDmaChannel(dma, config.dmaChannelTx, &peripheralUart[uartIndex]->TDR, [this]()
              { TransferComplete(); })
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
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
        HAL_UART_Init(&uartHandle);

        peripheralUart[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;
        peripheralUart[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDmaG4::UartStmDmaG4(hal::DmaStmG4& dma, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : uartIndex(aUartIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, aUartIndex)
        , uartRts(infra::inPlace, uartRts, PinConfigTypeStm::uartRts, aUartIndex)
        , uartCts(infra::inPlace, uartCts, PinConfigTypeStm::uartCts, aUartIndex)
        , uartHandle()
        , dma(dma)
        , transmitDmaChannel(dma, config.dmaChannelTx, &peripheralUart[uartIndex]->TDR, [this]()
              { TransferComplete(); })
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
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
        HAL_UART_Init(&uartHandle);

        peripheralUart[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;
        peripheralUart[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDmaG4::~UartStmDmaG4()
    {
        peripheralUart[uartIndex]->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);
        DisableClockUart(uartIndex);
    }

    void UartStmDmaG4::SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion)
    {
        if (!data.empty())
        {
            transferDataComplete = actionOnCompletion;
            transmitDmaChannel.StartTransmit(data);
        }
        else
            infra::EventDispatcher::Instance().Schedule(actionOnCompletion);
    }

    void UartStmDmaG4::ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived)
    {
        this->dataReceived = dataReceived;

        peripheralUart[uartIndex]->CR1 |= 1 << (USART_IT_RXNE & USART_IT_MASK);
    }

    void UartStmDmaG4::RegisterInterrupt(const Config& config)
    {
        if (config.priority)
            Register(peripheralUartIrq[uartIndex], *config.priority);
        else
            Register(peripheralUartIrq[uartIndex]);
    }

    void UartStmDmaG4::TransferComplete()
    {
        if (transferDataComplete)
        {
            infra::Function<void()> callback = transferDataComplete;
            transferDataComplete = nullptr;
            infra::EventDispatcher::Instance().Schedule(callback);
        }
    }

    void UartStmDmaG4::Invoke()
    {
        infra::BoundedVector<uint8_t>::WithMaxSize<8> buffer;

        while (peripheralUart[uartIndex]->ISR & USART_ISR_RXNE)
        {
            uint8_t receivedByte = peripheralUart[uartIndex]->RDR;
            buffer.push_back(receivedByte);
        }

        // If buffer is empty then interrupt was raised by Overrun Error (ORE) and we miss data.
        really_assert(!(buffer.empty() && peripheralUart[uartIndex]->ISR & USART_ISR_ORE));

        if (dataReceived != nullptr)
            dataReceived(buffer.range());
    }
}
