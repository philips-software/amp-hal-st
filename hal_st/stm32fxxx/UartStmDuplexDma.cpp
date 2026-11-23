#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <cstddef>
#include <cstdint>

#include DEVICE_HEADER

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
                ReceiveComplete();
            },
            [this]
            {
                ReceiveComplete();
            },
            config.priority }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    }

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : UartStmDma(transmitStream, oneBasedIndex, uartTx, uartRx, uartRts, uartCts, config)
        , rxBuffer{ rxBuffer }
        , receiveDmaChannel{ receiveStream, receiveRegister, 1, [this]
            {
                ReceiveComplete();
            },
            [this]
            {
                ReceiveComplete();
            },
            config.priority }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    }

    UartStmDuplexDma::UartStmDuplexDma(infra::MemoryRange<uint8_t> rxBuffer, hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config, bool hasFlowControl)
        : UartStmDma(transmitStream, oneBasedIndex, uartTx, uartRx, uartRts, uartCts, config)
        , rxBuffer{ rxBuffer }
        , receiveDmaChannel{ receiveStream, receiveRegister, 1, [this]
            {
                ReceiveComplete();
            },
            [this]
            {
                ReceiveComplete();
            },
            config.priority }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    }

    UartStmDuplexDma::~UartStmDuplexDma()
    {
        receiveDmaChannel.StopTransfer();
        uartArray[uartIndex]->CR3 &= ~USART_CR3_DMAT & ~USART_CR3_DMAR;
        uartArray[uartIndex]->CR1 &= ~USART_CR1_RTOIE;
    }

    void UartStmDuplexDma::ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived)
    {
        this->dataReceived = dataReceived;

        if (dataReceived == nullptr)
        {
            uartArray[uartIndex]->CR1 &= ~USART_CR1_RE;
            receiveDmaChannel.StopTransfer();
        }
        else
        {
            lastReceivedPosition = 0;
            receiveDmaChannel.StartReceive(rxBuffer);

            uartArray[uartIndex]->CR2 |= USART_CR2_RTOEN;
            uartArray[uartIndex]->RTOR = defaultRxTimeout;
            uartArray[uartIndex]->CR1 |= USART_CR1_RE | USART_CR1_RTOIE;
        }
    }

    void UartStmDuplexDma::ReceiveComplete()
    {
        const auto currentPosition = receiveDmaChannel.ReceivedSize() % rxBuffer.size();

        if (currentPosition < lastReceivedPosition)
            ReceivedUpTo(rxBuffer.size());

        ReceivedUpTo(currentPosition);
    }

    void UartStmDuplexDma::ReceivedUpTo(size_t position)
    {
        if (position == lastReceivedPosition || !dataReceived)
            return;

        really_assert(lastReceivedPosition < position && position <= rxBuffer.size());

        infra::ConstByteRange receivedData(rxBuffer.begin() + lastReceivedPosition, rxBuffer.begin() + position);
        lastReceivedPosition = position % rxBuffer.size();

        dataReceived(receivedData);
    }

    void UartStmDuplexDma::Invoke()
    {
        if (uartArray[uartIndex]->ISR & USART_ISR_RTOF)
        {
            uartArray[uartIndex]->ICR = USART_ICR_RTOCF;
            ReceiveComplete();
        }
    }
}
