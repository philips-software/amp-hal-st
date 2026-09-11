#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
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
                Drain();
            },
            [this]
            {
                Drain();
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
                Drain();
            },
            [this]
            {
                Drain();
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
                Drain();
            },
            [this]
            {
                Drain();
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

    std::size_t UartStmDuplexDma::OverrunCount() const
    {
        return overrunCount;
    }

    void UartStmDuplexDma::OverrunDetected(const infra::Function<void()>& onOverrun)
    {
        this->onOverrun = onOverrun;
    }

    void UartStmDuplexDma::Drain()
    {
        // Re-entered while already draining (e.g. RTOF firing mid-callback); the outer call will pick up the same progress.
        if (draining || !dataReceived)
            return;

        draining = true;

        // Both flags still pending here means the ISR is at least half a ring late.
        if (receiveDmaChannel.IsInterruptPending())
        {
            ++overrunCount;
            if (onOverrun)
                onOverrun();
        }

        for (;;)
        {
            auto currentPosition = receiveDmaChannel.ReceivedSize();
            if (currentPosition == rxBuffer.size())
                currentPosition = 0;

            if (currentPosition == lastReceivedPosition)
                break;

            if (currentPosition > lastReceivedPosition)
                dataReceived(infra::ConstByteRange(rxBuffer.begin() + lastReceivedPosition, rxBuffer.begin() + currentPosition));
            else
            {
                dataReceived(infra::ConstByteRange(rxBuffer.begin() + lastReceivedPosition, rxBuffer.end()));
                if (currentPosition != 0)
                    dataReceived(infra::ConstByteRange(rxBuffer.begin(), rxBuffer.begin() + currentPosition));
            }

            lastReceivedPosition = currentPosition;
        }

        draining = false;
    }

    void UartStmDuplexDma::Invoke()
    {
        if (uartArray[uartIndex]->ISR & USART_ISR_RTOF)
        {
            uartArray[uartIndex]->ICR = USART_ICR_RTOCF;
            Drain();
        }
    }
}
