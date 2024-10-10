#ifndef HAL_UART_STM_DMA_HPP
#define HAL_UART_STM_DMA_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include <cstdint>

namespace hal
{
    class UartStmDma
        : public UartStm
    {
    public:
        using Config = detail::UartStmConfig;

        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config = Config());
#if defined(HAS_PERIPHERAL_LPUART)
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, LpUart lpUart, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, LpUart lpUart, const Config& config = Config());
#endif
        ~UartStmDma();

        void SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion = infra::emptyFunction) override;
        void ReceiveData(infra::Function<void(infra::ConstByteRange data)> dataReceived) override;

    private:
        void TransferComplete();

        TransmitDmaChannel transmitDmaChannel;
        infra::Function<void()> transferDataComplete;
    };
}

#endif
