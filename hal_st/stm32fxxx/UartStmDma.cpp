#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include <cstdint>

#include DEVICE_HEADER

namespace hal
{
    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
        : UartStm(oneBasedIndex, uartTx, uartRx, config)
        , transmitDmaChannel{ transmitStream, transmitRegister, 1, [this]
            {
                TransferComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : UartStm(oneBasedIndex, uartTx, uartRx, uartRts, uartCts, config)
        , transmitDmaChannel{ transmitStream, transmitRegister, 1, [this]
            {
                TransferComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

#if defined(HAS_PERIPHERAL_LPUART)
    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, LpUart lpUart, const Config& config)
        : UartStm(oneBasedIndex, uartTx, uartRx, lpUart, config)
        , transmitDmaChannel{ transmitStream, transmitRegister, 1, [this]
            {
                TransferComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, LpUart lpUart, const Config& config)
        : UartStm(oneBasedIndex, uartTx, uartRx, uartRts, uartCts, lpUart, config)
        , transmitDmaChannel{ transmitStream, transmitRegister, 1, [this]
            {
                TransferComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }
#endif

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
