#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BoundedVector.hpp"

namespace hal
{
    namespace
    {
        uint16_t defaultRxTimeout = 16;

#if defined(STM32WB)
        const std::array<DmaChannelId, 1> defaultTxDmaChannelId = { { DmaChannelId{ 1, 0, DMA_REQUEST_USART1_TX } } };

        const std::array<DmaChannelId, 1> defaultRxDmaChannelId = { { DmaChannelId{ 1, 0, DMA_REQUEST_USART1_RX } } };
#else
        const std::array<DmaChannelId, 8> defaultTxDmaChannelId = { { DmaChannelId{ 2, 7, 4 },
            DmaChannelId{ 1, 6, 4 },
            DmaChannelId{ 1, 3, 4 },
            DmaChannelId{ 1, 4, 4 },
            DmaChannelId{ 1, 7, 4 },
            DmaChannelId{ 2, 6, 5 },
            DmaChannelId{ 1, 1, 5 },
            DmaChannelId{ 1, 0, 5 } } };

        const std::array<DmaChannelId, 8> defaultRxDmaChannelId = { { DmaChannelId{ 2, 2, 4 },
            DmaChannelId{ 1, 5, 4 },
            DmaChannelId{ 1, 1, 4 },
            DmaChannelId{ 1, 2, 4 },
            DmaChannelId{ 1, 0, 4 },
            DmaChannelId{ 2, 7, 5 },
            DmaChannelId{ 1, 3, 5 },
            DmaChannelId{ 1, 6, 5 } } };
#endif

    }

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm& dma, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
        : rxBuffer(rxBuffer)
        , uartIndex(aUartIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, aUartIndex)
        , uartHandle()
        , dma(dma)
#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
        , transmitDmaChannel(dma, config.dmaChannelTx.ValueOr(defaultTxDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->TDR, [this]()
              { TransferComplete(); })
        , receiveDmaChannel(
              dma, config.dmaChannelRx.ValueOr(defaultRxDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->RDR, [this]()
              { ReceiveComplete(this->rxBuffer.size() / 2); },
              [this]()
              { ReceiveComplete(this->rxBuffer.size()); })
#else
        , transmitDmaChannel(dma, config.dmaChannelTx.ValueOr(defaultTxDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->DR, [this]()
              { TransferComplete(); })
        , receiveDmaChannel(
              dma, config.dmaChannelRx.ValueOr(defaultRxDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->DR, [this]()
              { ReceiveComplete(this->rxBuffer.size() / 2); },
              [this]()
              { ReceiveComplete(this->rxBuffer.size()); })
#endif
    {
        Configure(config);
    }

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm& dma, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : rxBuffer(rxBuffer)
        , uartIndex(aUartIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, aUartIndex)
        , uartRts(infra::inPlace, uartRts, PinConfigTypeStm::uartRts, aUartIndex)
        , uartCts(infra::inPlace, uartCts, PinConfigTypeStm::uartCts, aUartIndex)
        , uartHandle()
        , dma(dma)
#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
        , transmitDmaChannel(dma, config.dmaChannelTx.ValueOr(defaultTxDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->TDR, [this]()
              { TransferComplete(); })
        , receiveDmaChannel(
              dma, config.dmaChannelRx.ValueOr(defaultRxDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->RDR, [this]()
              { ReceiveComplete(this->rxBuffer.size() / 2); },
              [this]()
              { ReceiveComplete(this->rxBuffer.size()); })
#else
        , transmitDmaChannel(dma, config.dmaChannelTx.ValueOr(defaultTxDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->DR, [this]()
              { TransferComplete(); })
        , receiveDmaChannel(
              dma, config.dmaChannelRx.ValueOr(defaultRxDmaChannelId[uartIndex]), &peripheralUart[uartIndex]->DR, [this]()
              { ReceiveComplete(this->rxBuffer.size() / 2); },
              [this]()
              { ReceiveComplete(this->rxBuffer.size()); })
#endif
    {
        Configure(config);
    }

    UartStmDuplexDma::~UartStmDuplexDma()
    {
        receiveDmaChannel.StopTransfer();
        peripheralUart[uartIndex]->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);
        DisableClockUart(uartIndex);
    }

    void UartStmDuplexDma::SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion)
    {
        if (!data.empty())
        {
            transferDataComplete = actionOnCompletion;
            transmitDmaChannel.StartTransmit(data);
        }
        else
            infra::EventDispatcher::Instance().Schedule(actionOnCompletion);
    }

    void UartStmDuplexDma::ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived)
    {
        this->dataReceived = dataReceived;

        receiveDmaChannel.StartReceive(rxBuffer);

        peripheralUart[uartIndex]->CR2 |= USART_CR2_RTOEN;
        peripheralUart[uartIndex]->CR1 |= USART_CR1_RE | USART_CR1_RTOIE;
        peripheralUart[uartIndex]->RTOR = defaultRxTimeout;
    }

    void UartStmDuplexDma::Configure(const Config& config)
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
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;

        HAL_UART_Init(&uartHandle);

#if defined(STM32WB)
        peripheralUart[uartIndex]->CR1 &= ~USART_CR1_UE;
        peripheralUart[uartIndex]->CR1 |= USART_CR1_FIFOEN | USART_CR1_UE;
#endif

        peripheralUart[uartIndex]->CR1 &= ~USART_CR1_RE;
        peripheralUart[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;
        peripheralUart[uartIndex]->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    }

    void UartStmDuplexDma::ReceiveComplete(size_t currentPosition)
    {
        if (currentPosition == lastReceivedPosition)
            return;

        if (dataReceived)
        {
            infra::ConstByteRange receivedData(rxBuffer.begin() + lastReceivedPosition, rxBuffer.begin() + currentPosition);
            lastReceivedPosition = currentPosition == rxBuffer.size() ? 0 : currentPosition;
            dataReceived(receivedData);
        }
    }

    void UartStmDuplexDma::RegisterInterrupt(const Config& config)
    {
        if (config.priority)
            Register(peripheralUartIrq[uartIndex], *config.priority);
        else
            Register(peripheralUartIrq[uartIndex]);
    }

    void UartStmDuplexDma::TransferComplete()
    {
        if (transferDataComplete)
        {
            infra::EventDispatcher::Instance().Schedule(transferDataComplete);
            transferDataComplete = nullptr;
        }
    }

    void UartStmDuplexDma::Invoke()
    {
        if (peripheralUart[uartIndex]->ISR & USART_ISR_RTOF)
        {
            peripheralUart[uartIndex]->ICR = USART_ICR_RTOCF;
            ReceiveComplete(receiveDmaChannel.ReceivedSize());
        }
    }
}
