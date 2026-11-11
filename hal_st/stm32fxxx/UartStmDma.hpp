#ifndef HAL_UART_STM_DMA_HPP
#define HAL_UART_STM_DMA_HPP

#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/MemoryRange.hpp"
#include <cstdint>

namespace hal
{
    class UartStmDma
        : public UartStm
    {
    public:
        using Config = detail::UartStmConfig;

        [[deprecated("Use UartStmDma with GpioPins")]] UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, const Config& config = Config());
        [[deprecated("Use UartStmDma with GpioPinsFlowCtrl")]] UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, GpioPinStm& rts, GpioPinStm& cts, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, const GpioPins& pins, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, const GpioPinsFlowCtrl& pins, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioOwners&& gpioOwners, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, const Config& config = Config());
#if defined(HAS_PERIPHERAL_LPUART)
        [[deprecated("Use UartStmDma with GpioPins and LpUart")]] UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, LpUart lpUart, const Config& config = Config());
        [[deprecated("Use UartStmDma with GpioPinsFlowCtrl and LpUart")]] UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& tx, GpioPinStm& rx, GpioPinStm& rts, GpioPinStm& cts, LpUart lpUart, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, const GpioPins& pins, LpUart lpUart, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, const GpioPinsFlowCtrl& pins, LpUart lpUart, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioOwners&& gpioOwners, LpUart lpUart, const Config& config = Config());
        UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioOwnersFlowCtrl&& gpioOwnersFlowCtrl, LpUart lpUart, const Config& config = Config());
#endif
        ~UartStmDma();

        void SendData(infra::MemoryRange<const uint8_t> data, infra::Function<void()> actionOnCompletion = infra::emptyFunction) override;

    private:
        void TransferComplete();

        volatile void* transmitRegister =
#if defined(USART_TDR_TDR)
            &uartArray[uartIndex]->TDR;
#else
            &uartArray[uartIndex]->DR;
#endif
        TransmitDmaChannel transmitDmaChannel;
        infra::Function<void()> transferDataComplete;
    };
}

#endif
