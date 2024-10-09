#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"

namespace hal
{
    namespace
    {
        volatile void* TransmitRegister(uint8_t uartIndex)
        {
#if defined(USART_TDR_TDR)
            return &peripheralUart[uartIndex]->TDR;
#else
            return &peripheralUart[uartIndex]->DR;
#endif
        }
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
        : UartStm(oneBasedIndex, uartTx, uartRx, config)
        , transmitDmaChannel{ transmitStream, TransmitRegister(uartIndex), 1, [this]
            {
                TransferComplete();
            } }
    {
        peripheralUart[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : UartStm(oneBasedIndex, uartTx, uartRx, uartRts, uartCts, config)
        , transmitDmaChannel{ transmitStream, TransmitRegister(uartIndex), 1, [this]
            {
                TransferComplete();
            } }
    {
        peripheralUart[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

#if defined(HAS_PERIPHERAL_LPUART)
    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, LpUart lpUart, const Config& config)
        : UartStm(oneBasedIndex, uartTx, uartRx, lpUart, config)
        , transmitDmaChannel{ transmitStream, TransmitRegister(uartIndex), 1, [this]
            {
                TransferComplete();
            } }
    {
        peripheralUart[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, LpUart lpUart, const Config& config)
        : UartStm(oneBasedIndex, uartTx, uartRx, uartRts, uartCts, lpUart, config)
        , transmitDmaChannel{ transmitStream, TransmitRegister(uartIndex), 1, [this]
            {
                TransferComplete();
            } }
    {
        peripheralUart[uartIndex]->CR3 |= USART_CR3_DMAT;
    }
#endif

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

#if defined(STM32F4) || defined(STM32G4)
        peripheralUart[uartIndex]->CR1 |= USART_IT_RXNE & USART_IT_MASK;
#else
        peripheralUart[uartIndex]->CR1 |= 1 << (USART_IT_RXNE & USART_IT_MASK);
#endif
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
}
