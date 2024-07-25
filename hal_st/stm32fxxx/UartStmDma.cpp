#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BoundedVector.hpp"

namespace hal
{
    namespace
    {
        volatile void* PeripheralAddress(infra::MemoryRange<USART_TypeDef* const> table, uint8_t uartIndex)
        {
#if defined(USART_TDR_TDR)
            return &table[uartIndex]->TDR;
#else
            return &table[uartIndex]->DR;
#endif
        }
    }

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, uartTx, uartRx, hal::dummyPinStm, hal::dummyPinStm, config, false }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config)
        : UartStmDma{ transmitStream, oneBasedIndex, uartTx, uartRx, uartRts, uartCts, config, true }
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, const Config& config, bool hasFlowControl)
        : uartIndex(oneBasedIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::uartTx, oneBasedIndex)
        , uartRx(uartRx, PinConfigTypeStm::uartRx, oneBasedIndex)
        , uartRts(uartRts, PinConfigTypeStm::uartRts, oneBasedIndex)
        , uartCts(uartCts, PinConfigTypeStm::uartCts, oneBasedIndex)
        , uartArray(peripheralUart)
        , uartIrqArray(peripheralUartIrq)
        , transmitDmaChannel{ transmitStream, PeripheralAddress(uartArray, uartIndex), 1, [this]()
            {
                TransferComplete();
            } }
    {
        RegisterInterrupt(config);
        EnableClockUart(uartIndex);
        UartStmHalInit(config, hasFlowControl);
    }

#if defined(HAS_PERIPHERAL_LPUART)
    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, LpUart lpUart, const Config& config)
        : UartStmDma(transmitStream, oneBasedIndex, uartTx, uartRx, dummyPinStm, dummyPinStm, lpUart, config, false)
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, LpUart lpUart, const Config& config)
        : UartStmDma(transmitStream, oneBasedIndex, uartTx, uartRx, uartRts, uartCts, lpUart, config, false)
    {}

    UartStmDma::UartStmDma(DmaStm::TransmitStream& transmitStream, uint8_t oneBasedIndex, GpioPinStm& uartTx, GpioPinStm& uartRx, GpioPinStm& uartRts, GpioPinStm& uartCts, LpUart lpUart, const Config& config, bool hasFlowControl)
        : uartIndex(oneBasedIndex - 1)
        , uartTx(uartTx, PinConfigTypeStm::lpuartTx, oneBasedIndex)
        , uartRx(uartRx, PinConfigTypeStm::lpuartRx, oneBasedIndex)
        , uartRts(uartRts, PinConfigTypeStm::lpuartRts, oneBasedIndex)
        , uartCts(uartCts, PinConfigTypeStm::lpuartCts, oneBasedIndex)
        , uartArray(peripheralLpuart)
        , uartIrqArray(peripheralLpuartIrq)
        , transmitDmaChannel{ transmitStream, PeripheralAddress(uartArray, uartIndex), 1, [this]()
            {
                TransferComplete();
            } }
    {
        RegisterInterrupt(config);
        EnableClockLpuart(uartIndex);
        UartStmHalInit(config, hasFlowControl);
    }
#endif

    UartStmDma::~UartStmDma()
    {
        uartArray[uartIndex]->CR1 &= ~(USART_CR1_TE | USART_CR1_RE);
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
        uartArray[uartIndex]->CR1 |= USART_IT_RXNE & USART_IT_MASK;
#else
        uartArray[uartIndex]->CR1 |= 1 << (USART_IT_RXNE & USART_IT_MASK);
#endif
    }

    void UartStmDma::UartStmHalInit(const Config& config, bool hasFlowControl)
    {
        UART_HandleTypeDef uartHandle = {};
        uartHandle.Instance = uartArray[uartIndex];
        uartHandle.Init.BaudRate = config.baudrate;
        uartHandle.Init.WordLength = config.parity == USART_PARITY_NONE ? USART_WORDLENGTH_8B : USART_WORDLENGTH_9B;
        uartHandle.Init.StopBits = USART_STOPBITS_1;
        uartHandle.Init.Parity = config.parity;
        uartHandle.Init.Mode = USART_MODE_TX_RX;

        if (hasFlowControl)
            uartHandle.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
        else
            uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;

#if defined(UART_ONE_BIT_SAMPLE_ENABLE)
        uartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
#endif
        uartHandle.Init.OverSampling = UART_OVERSAMPLING_8;

#if defined(UART_ADVFEATURE_NO_INIT)
        uartHandle.AdvancedInit = {};

#if defined(UART_ADVFEATURE_SWAP_INIT)
        if (config.swapTxRx)
        {
            uartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
            uartHandle.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
        }
#endif
#endif

        HAL_UART_Init(&uartHandle);

        uartArray[uartIndex]->CR2 &= ~USART_CLOCK_ENABLED;
        uartArray[uartIndex]->CR3 |= USART_CR3_DMAT;
    }

    void UartStmDma::RegisterInterrupt(const Config& config)
    {
        Register(uartIrqArray[uartIndex], config.priority);
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

    void UartStmDma::Invoke()
    {
        infra::BoundedVector<uint8_t>::WithMaxSize<8> buffer;

#if defined(USART_ISR_RXNE_RXFNE)
        while (!buffer.full() && uartArray[uartIndex]->ISR & USART_ISR_RXNE_RXFNE)
#elif defined(USART_ISR_RXNE)
        while (!buffer.full() && uartArray[uartIndex]->ISR & USART_ISR_RXNE)
#else
        while (!buffer.full() && uartArray[uartIndex]->SR & USART_SR_RXNE)
#endif
        {
            uint8_t receivedByte =
#if defined(USART_RDR_RDR)
                uartArray[uartIndex]->RDR;
#else
                uartArray[uartIndex]->DR;
#endif
            buffer.push_back(receivedByte);
        }

        // If buffer is empty then interrupt was raised by Overrun Error (ORE) and we miss data.
#if defined(USART_ISR_ORE)
        really_assert(!(buffer.empty() && uartArray[uartIndex]->ISR & USART_ISR_ORE));
#else
        really_assert(!(buffer.empty() && uartArray[uartIndex]->SR & USART_SR_ORE));
#endif

        if (dataReceived != nullptr)
            dataReceived(buffer.range());
    }
}
