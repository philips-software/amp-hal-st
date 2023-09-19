#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/Optional.hpp"
#include "infra/util/ReallyAssert.hpp"

namespace hal
{
    namespace
    {
        uint32_t defaultRxTimeout = 16;

        volatile void* TransmitRegister(uint8_t uartIndex)
        {
#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
            return &peripheralUart[uartIndex]->TDR;
#else
            return &peripheralUart[uartIndex]->DR;
#endif
        }

        volatile void* ReceiveRegister(uint8_t uartIndex)
        {
#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB) || defined(STM32G4)
            return &peripheralUart[uartIndex]->RDR;
#else
            return &peripheralUart[uartIndex]->DR;
#endif
        }
    }

    UartStmDuplexDmaOptionalFlowControl::UartStmDuplexDmaOptionalFlowControl(uint8_t aUartIndex, GpioPinStm* uartRts, GpioPinStm* uartCts)
    {
        if (uartRts != nullptr && uartCts != nullptr)
        {
            this->uartRts.Emplace(*uartRts, PinConfigTypeStm::uartRts, aUartIndex);
            this->uartCts.Emplace(*uartCts, PinConfigTypeStm::uartCts, aUartIndex);
        }
    }

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx)
        : UartStmDuplexDma{ rxBuffer, transmitStream, receiveStream, aUartIndex, uartTx, uartRx, {} }
    {}

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
        : UartStmDuplexDma{ rxBuffer, transmitStream, receiveStream, aUartIndex, uartTx, uartRx, nullptr, nullptr, config }
    {}

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts)
        : UartStmDuplexDma{ rxBuffer, transmitStream, receiveStream, aUartIndex, uartTx, uartRx, uartRts, uartCts, {} }
    {}

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : UartStmDuplexDma{ rxBuffer, transmitStream, receiveStream, aUartIndex, uartTx, uartRx, &uartRts, &uartCts, config }
    {}

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t aUartIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm* uartRts, GpioPinStm* uartCts, const Config& config)
        : UartStmDuplexDmaOptionalFlowControl{ aUartIndex, uartRts, uartCts }
        , rxBuffer(rxBuffer)
        , uartIndex(aUartIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, aUartIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, aUartIndex)
        , transmitDmaChannel(transmitStream, TransmitRegister(uartIndex), 1, [this]
              {
                  TransferComplete();
              })
        , receiveDmaChannel(
              receiveStream, ReceiveRegister(uartIndex), 1, [this]
              {
                  HalfReceiveComplete();
              },
              [this]
              {
                  FullReceiveComplete();
              })
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

        UART_HandleTypeDef uartHandle = {};
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

    void UartStmDuplexDma::HalfReceiveComplete()
    {
        ReceiveComplete(this->rxBuffer.size() / 2);
    }

    void UartStmDuplexDma::FullReceiveComplete()
    {
        ReceiveComplete(this->rxBuffer.size());
    }

    void UartStmDuplexDma::ReceiveComplete(size_t currentPosition)
    {
        if (currentPosition == lastReceivedPosition || !dataReceived)
            return;

        really_assert(lastReceivedPosition <= currentPosition);

        infra::ConstByteRange receivedData(rxBuffer.begin() + lastReceivedPosition, rxBuffer.begin() + currentPosition);
        lastReceivedPosition = currentPosition == rxBuffer.size() ? 0 : currentPosition;

        dataReceived(receivedData);
    }

    void UartStmDuplexDma::RegisterInterrupt(const Config& config)
    {
        Register(peripheralUartIrq[uartIndex], config.priority);
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

            const auto receivedSize = receiveDmaChannel.ReceivedSize();

            if (receiveDmaChannel.IsInterruptPending())
                return;

            ReceiveComplete(receivedSize);
        }
    }
}
