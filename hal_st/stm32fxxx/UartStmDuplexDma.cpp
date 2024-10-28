#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"

namespace hal
{
    namespace
    {
        uint32_t defaultRxTimeout = 16;
    }

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
        : UartStmDma(transmitStream, oneBasedIndex, uartTx, uartRx, config)
        , rxBuffer{ rxBuffer }
        , receiveDmaChannel{ receiveStream, receiveRegister, 1, [this]
            {
                HalfReceiveComplete();
            },
            [this]
            {
                FullReceiveComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    }

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : UartStmDma(transmitStream, oneBasedIndex, uartTx, uartRx, uartRts, uartCts, config)
        , rxBuffer{ rxBuffer }
        , receiveDmaChannel{ receiveStream, receiveRegister, 1, [this]
            {
                HalfReceiveComplete();
            },
            [this]
            {
                FullReceiveComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    }

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config, bool hasFlowControl)
        : UartStmDma(transmitStream, oneBasedIndex, uartTx, uartRx, uartRts, uartCts, config)
        , rxBuffer{ rxBuffer }
        , receiveDmaChannel{ receiveStream, receiveRegister, 1, [this]
            {
                HalfReceiveComplete();
            },
            [this]
            {
                FullReceiveComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    }

    UartStmDuplexDma::~UartStmDuplexDma()
    {
        receiveDmaChannel.StopTransfer();
    }

    void UartStmDuplexDma::ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived)
    {
        this->dataReceived = dataReceived;

        if (dataReceived == nullptr)
            receiveDmaChannel.StopTransfer();
        else
        {
            receiveDmaChannel.StartReceive(rxBuffer);

            uartArray[uartIndex]->CR2 |= USART_CR2_RTOEN;
            uartArray[uartIndex]->CR1 |= USART_CR1_RE | USART_CR1_RTOIE;
            uartArray[uartIndex]->RTOR = defaultRxTimeout;
        }
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

        if (dataReceived != nullptr)
            dataReceived(receivedData);
    }

    void UartStmDuplexDma::Invoke()
    {
        if (uartArray[uartIndex]->ISR & USART_ISR_RTOF)
        {
            uartArray[uartIndex]->ICR = USART_ICR_RTOCF;

            const auto receivedSize = receiveDmaChannel.ReceivedSize();

            if (receiveDmaChannel.IsInterruptPending())
                return;

            ReceiveComplete(receivedSize);
        }
    }
}
