#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BoundedVector.hpp"

namespace hal
{
    namespace
    {
        const std::array<DmaChannelId, 8> defaultDmaChannelId = {{
            DmaChannelId{ 2, 7, 4 },
            DmaChannelId{ 1, 6, 4 },
            DmaChannelId{ 1, 3, 4 },
            DmaChannelId{ 1, 4, 4 },
            DmaChannelId{ 1, 7, 4 },
            DmaChannelId{ 2, 6, 5 },
            DmaChannelId{ 1, 1, 5 },
            DmaChannelId{ 1, 0, 5 }
        }};
    }

    UartStmDma::UartStmDma(hal::DmaStm& dma, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
        : uartIndex(aUartIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, aUartIndex)
        , uartHandle()
        , dma(dma)
#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
        , transmitDmaChannel(dma, config.dmaChannelTx.ValueOr(defaultDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->TDR, [this]() { TransferComplete(); })
#else
        , transmitDmaChannel(dma, config.dmaChannelTx.ValueOr(defaultDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->DR, [this]() { TransferComplete(); })
#endif
    {
        RegisterInterrupt(config);
        EnableClockUart(uartIndex);

        uartHandle.Instance = peripheralUart[uartIndex];
        uartHandle.Init.BaudRate = config.baudrate;
        uartHandle.Init.WordLength = USART_WORDLENGTH_8B;
        uartHandle.Init.StopBits = USART_STOPBITS_1;
        uartHandle.Init.Parity = USART_PARITY_NONE;
        uartHandle.Init.Mode = USART_MODE_TX_RX;
        uartHandle.Init.HwFlowCtl = config.hwFlowControl;
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7)
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
#else
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
#endif
        HAL_UART_Init(&uartHandle);

        peripheralUart[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;
        peripheralUart[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::UartStmDma(hal::DmaStm& dma, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx
        , GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : uartIndex(aUartIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, aUartIndex)
        , uartRts(infra::inPlace, uartRts, PinConfigTypeStm::uartRts, aUartIndex)
        , uartCts(infra::inPlace, uartCts, PinConfigTypeStm::uartCts, aUartIndex)
        , uartHandle()
        , dma(dma)
#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
        , transmitDmaChannel(dma, config.dmaChannelTx.ValueOr(defaultDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->TDR, [this]() { TransferComplete(); })
#else
        , transmitDmaChannel(dma, config.dmaChannelTx.ValueOr(defaultDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->DR, [this]() { TransferComplete(); })
#endif
    {
        RegisterInterrupt(config);
        EnableClockUart(uartIndex);

        uartHandle.Instance = peripheralUart[uartIndex];
        uartHandle.Init.BaudRate = config.baudrate;
        uartHandle.Init.WordLength = USART_WORDLENGTH_8B;
        uartHandle.Init.StopBits = USART_STOPBITS_1;
        uartHandle.Init.Parity = USART_PARITY_NONE;
        uartHandle.Init.Mode = USART_MODE_TX_RX;
        uartHandle.Init.HwFlowCtl = config.hwFlowControl;
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7)
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
        transferDataComplete = actionOnCompletion;

        transmitDmaChannel.StartTransmit(data);
    }

    void UartStmDma::ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived)
    {
        this->dataReceived = dataReceived;

#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7)
        peripheralUart[uartIndex]->CR1 |= 1 << (USART_IT_RXNE & USART_IT_MASK);
#else
        peripheralUart[uartIndex]->CR1 |= USART_IT_RXNE & USART_IT_MASK;
#endif
    }

    void UartStmDma::RegisterInterrupt(const Config& config)
    {
        if (config.priority)
            Register(peripheralUartIrq[uartIndex], *config.priority);
        else
            Register(peripheralUartIrq[uartIndex]);
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

        const uint32_t ISR_RXNE = 0x20U;
        const uint32_t ISR_ORE = 0x8U;

        while (peripheralUart[uartIndex]->ISR & ISR_RXNE)
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
        really_assert(!(buffer.empty() && peripheralUart[uartIndex]->ISR & ISR_ORE));
        
        if (dataReceived != nullptr)
            dataReceived(buffer.range());
    }
}
