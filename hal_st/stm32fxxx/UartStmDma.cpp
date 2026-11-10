#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include <cstdint>
#include <utility>

#include DEVICE_HEADER

namespace hal
{
    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, GpioPins{ tx, rx }, config }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, GpioPinStm& rts, GpioPinStm& cts, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, GpioPinsFlowCtrl{ tx, rx, rts, cts }, config }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, const GpioPins& pins, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, GpioOwners{ GpioPinStmOwner{ pins.tx }, GpioPinStmOwner{ pins.rx } }, config }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, const GpioPinsFlowCtrl& pins, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, GpioOwnersFlowCtrl{ GpioPinStmOwner{ pins.tx }, GpioPinStmOwner{ pins.rx }, GpioPinStmOwner{ pins.rts }, GpioPinStmOwner{ pins.cts } }, config }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioOwners&& gpioOwners, const Config& config)
        : UartStm{ oneBasedIndex, std::move(gpioOwners), config }
        , transmitDmaChannel{ transmitStream, transmitRegister, 1, [this]
            {
                TransferComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, const Config& config)
        : UartStm{ oneBasedIndex, std::move(gpioOwnersFlowCtrl), config }
        , transmitDmaChannel{ transmitStream, transmitRegister, 1, [this]
            {
                TransferComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

#if defined(HAS_PERIPHERAL_LPUART)
    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, LpUart lpUart, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, GpioPins{ tx, rx }, lpUart, config }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, GpioPinStm& rts, GpioPinStm& cts, LpUart lpUart, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, GpioPinsFlowCtrl{ tx, rx, rts, cts }, lpUart, config }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, const GpioPins& pins, LpUart lpUart, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, GpioOwners{ GpioPinStmOwner{ pins.tx }, GpioPinStmOwner{ pins.rx } }, lpUart, config }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, const GpioPinsFlowCtrl& pins, LpUart lpUart, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, GpioOwnersFlowCtrl{ GpioPinStmOwner{ pins.tx }, GpioPinStmOwner{ pins.rx }, GpioPinStmOwner{ pins.rts }, GpioPinStmOwner{ pins.cts } }, lpUart, config }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioOwners&& gpioOwners, LpUart lpUart, const Config& config)
        : UartStm{ oneBasedIndex, std::move(gpioOwners), lpUart, config }
        , transmitDmaChannel{ transmitStream, transmitRegister, 1, [this]
            {
                TransferComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, LpUart lpUart, const Config& config)
        : UartStm{ oneBasedIndex, std::move(gpioOwnersFlowCtrl), lpUart, config }
        , transmitDmaChannel{ transmitStream, transmitRegister, 1, [this]
            {
                TransferComplete();
            } }
    {
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }
#endif

    UartStmDma::~UartStmDma()
    {
        uartArray[uartIndex]->CR3 &= ~USART_CR3_DMAT;
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
