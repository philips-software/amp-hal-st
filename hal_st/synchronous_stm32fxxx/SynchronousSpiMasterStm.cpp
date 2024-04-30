#include "hal_st/synchronous_stm32fxxx/SynchronousSpiMasterStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/BitLogic.hpp"

namespace hal
{
    SynchronousSpiMasterStm::SynchronousSpiMasterStm(uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, const Config& config, GpioPinStm& slaveSelect)
        : spiInstance(oneBasedSpiIndex - 1)
        , clock(clock, PinConfigTypeStm::spiClock, oneBasedSpiIndex)
        , miso(miso, PinConfigTypeStm::spiMiso, oneBasedSpiIndex)
        , mosi(mosi, PinConfigTypeStm::spiMosi, oneBasedSpiIndex)
        , slaveSelect(slaveSelect, PinConfigTypeStm::spiSlaveSelect, oneBasedSpiIndex)
    {
        EnableClockSpi(spiInstance);

        spiHandle.Instance = peripheralSpi[spiInstance];
        spiHandle.Init.Mode = SPI_MODE_MASTER;
        spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
        spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
        spiHandle.Init.CLKPolarity = config.polarityLow ? SPI_POLARITY_LOW : SPI_POLARITY_HIGH;
        spiHandle.Init.CLKPhase = config.phase1st ? SPI_PHASE_1EDGE : SPI_PHASE_2EDGE;
        spiHandle.Init.NSS = SPI_NSS_SOFT;
        spiHandle.Init.BaudRatePrescaler = config.baudRatePrescaler;
        spiHandle.Init.FirstBit = config.msbFirst ? SPI_FIRSTBIT_MSB : SPI_FIRSTBIT_LSB;
        spiHandle.Init.TIMode = SPI_TIMODE_DISABLED;
        spiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
        spiHandle.Init.CRCPolynomial = 1;
#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7) || defined(STM32WB)
        spiHandle.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
        spiHandle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif
        HAL_SPI_Init(&spiHandle);

#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7)
        peripheralSpi[spiInstance]->CR2 |= SPI_RXFIFO_THRESHOLD_QF;
#endif

        __HAL_SPI_ENABLE(&spiHandle);
    }

    SynchronousSpiMasterStm::~SynchronousSpiMasterStm()
    {
        __HAL_SPI_DISABLE(&spiHandle);
        HAL_SPI_DeInit(&spiHandle);
        DisableClockSpi(spiInstance);
    }

    void SynchronousSpiMasterStm::SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, Action nextAction)
    {
        assert(sendData.size() == receiveData.size() || sendData.empty() || receiveData.empty());
        this->sendData = sendData;
        this->receiveData = receiveData;
        sending = !sendData.empty();
        receiving = !receiveData.empty();

        if (!sending)
            dummyToSend = receiveData.size();
        if (!receiving)
            dummyToReceive = sendData.size();

#if !defined(STM32WBA)
        peripheralSpi[spiInstance]->CR2 |= SPI_IT_TXE;
        peripheralSpi[spiInstance]->CR2 |= SPI_IT_RXNE;
#else
        peripheralSpi[spiInstance]->CR2 |= SPI_IT_TXP;
        peripheralSpi[spiInstance]->CR2 |= SPI_IT_RXP;
#endif

        while (sending || receiving || dummyToSend != 0 || dummyToReceive != 0)
            HandleInterrupt();
    }

    void SynchronousSpiMasterStm::HandleInterrupt()
    {
        uint32_t status = peripheralSpi[spiInstance]->SR;
#if !defined(STM32WBA)
        if ((status & SPI_FLAG_RXNE) != 0)
#else
        if ((status & SPI_FLAG_RXWNE) != 0)
#endif
        {
            if (dummyToReceive != 0)
            {
#if !defined(STM32WBA)
                (void)peripheralSpi[spiInstance]->DR;
#else
                (void)peripheralSpi[spiInstance]->RXDR;
#endif
                --dummyToReceive;
            }
            else if (receiving)
            {
#if !defined(STM32WBA)
                receiveData.front() = peripheralSpi[spiInstance]->DR;
#else
                receiveData.front() = peripheralSpi[spiInstance]->RXDR;
#endif
                receiveData.pop_front();
            }

            receiving &= !receiveData.empty();

            if (dummyToReceive == 0 && !receiving)
#if !defined(STM32WBA)
                peripheralSpi[spiInstance]->CR2 &= ~SPI_IT_RXNE;
#else
                peripheralSpi[spiInstance]->CR2 &= ~SPI_IT_RXP;
#endif
        }

#if !defined(STM32WBA)
        if ((status & SPI_FLAG_TXE) != 0)
#else
        if ((status & SPI_FLAG_TXC) != 0)
#endif
        {
            if (dummyToSend != 0)
            {
#if !defined(STM32WBA)
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->DR) = 0;
#else
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->TXDR) = 0;
#endif
                --dummyToSend;
            }
            else if (sending)
            {
#if !defined(STM32WBA)
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->DR) = sendData.front();
#else
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->TXDR) = sendData.front();
#endif
                sendData.pop_front();
            }

            sending &= !sendData.empty();

            // After the first transmit, disable interrupt on transmit buffer empty,
            // so that a receive is done before each transmit
#if !defined(STM32WBA)
            peripheralSpi[spiInstance]->CR2 &= ~SPI_IT_TXE;
#else
            peripheralSpi[spiInstance]->CR2 &= ~SPI_IT_TXP;
#endif
        }

        really_assert(!(peripheralSpi[spiInstance]->SR & SPI_FLAG_OVR));
    }
}
