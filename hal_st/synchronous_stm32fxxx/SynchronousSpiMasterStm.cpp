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

        SPI_HandleTypeDef spiHandle{};
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

#ifdef SPI_CRC_LENGTH_DATASIZE
        spiHandle.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
#endif
#ifdef SPI_NSS_PULSE_DISABLE
        spiHandle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif
#ifdef SPI_CFG2_RDIMM
        spiHandle.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
#endif
#ifdef SPI_CFG2_RDIOP
        spiHandle.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
#endif
#ifdef SPI_CFG1_FTHLV
        spiHandle.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
#endif
#ifdef SPI_CR1_MASRX
        spiHandle.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
#endif
        HAL_SPI_Init(&spiHandle);

#ifdef SPI_CR2_FRXTH
        peripheralSpi[spiInstance]->CR2 |= SPI_RXFIFO_THRESHOLD_QF;
#endif

        __HAL_SPI_ENABLE(&spiHandle);
    }

    SynchronousSpiMasterStm::~SynchronousSpiMasterStm()
    {
        peripheralSpi[spiInstance]->CR1 &= ~SPI_CR1_SPE;
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

#ifdef SPI_IER_RXPIE
        peripheralSpi[spiInstance]->IER |= SPI_IT_TXP;
        peripheralSpi[spiInstance]->IER |= SPI_IT_RXP;
#else
        peripheralSpi[spiInstance]->CR2 |= SPI_IT_TXE;
        peripheralSpi[spiInstance]->CR2 |= SPI_IT_RXNE;
#endif

        while (sending || receiving || dummyToSend != 0 || dummyToReceive != 0)
            HandleInterrupt();
    }

    void SynchronousSpiMasterStm::HandleInterrupt()
    {
        uint32_t status = peripheralSpi[spiInstance]->SR;
#ifdef SPI_FLAG_RXNE
        if ((status & SPI_FLAG_RXNE) != 0)
#else
        if ((status & SPI_IT_RXP) != 0)
#endif
        {
            if (dummyToReceive != 0)
            {
#ifdef SPI_RXDR_RXDR
                (void)peripheralSpi[spiInstance]->RXDR;
#else
                (void)peripheralSpi[spiInstance]->DR;
#endif
                --dummyToReceive;
            }
            else if (receiving)
            {
#ifdef SPI_RXDR_RXDR
                receiveData.front() = peripheralSpi[spiInstance]->RXDR;
#else
                receiveData.front() = peripheralSpi[spiInstance]->DR;
#endif
                receiveData.pop_front();
            }

            receiving &= !receiveData.empty();

            if (dummyToReceive == 0 && !receiving)
#ifdef SPI_IER_RXPIE
                peripheralSpi[spiInstance]->IER &= ~SPI_IT_RXP;
#else
                peripheralSpi[spiInstance]->CR2 &= ~SPI_IT_RXNE;
#endif
        }

#ifdef SPI_SR_TXP
        if ((status & SPI_FLAG_TXP) != 0)
#else
        if ((status & SPI_FLAG_TXE) != 0)
#endif
        {
            if (dummyToSend != 0)
            {
#ifdef SPI_TXDR_TXDR
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->TXDR) = 0;
#else
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->DR) = 0;
#endif
                --dummyToSend;
            }
            else if (sending)
            {
#ifdef SPI_TXDR_TXDR
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->TXDR) = sendData.front();
                peripheralSpi[spiInstance]->CR1 |= SPI_CR1_CSTART;
#else
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->DR) = sendData.front();
#endif
                sendData.pop_front();
            }

            sending &= !sendData.empty();

            // After the first transmit, disable interrupt on transmit buffer empty,
            // so that a receive is done before each transmit
#ifdef SPI_IER_TXPIE
            peripheralSpi[spiInstance]->IER &= ~SPI_IT_TXP;
#else
            peripheralSpi[spiInstance]->CR2 &= ~SPI_IT_TXE;
#endif
        }

        really_assert(!(peripheralSpi[spiInstance]->SR & SPI_FLAG_OVR));
    }
}
