#include "hal_st/stm32fxxx/SpiMasterStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/BitLogic.hpp"

namespace hal
{
    SpiMasterStm::SpiMasterStm(uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, const Config& config, GpioPinStm& slaveSelect)
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

#if defined(SPI_CR2_FRXTH)
        peripheralSpi[spiInstance]->CR2 |= SPI_RXFIFO_THRESHOLD_QF;
#endif

        __HAL_SPI_ENABLE(&spiHandle);
    }

    SpiMasterStm::~SpiMasterStm()
    {
        peripheralSpi[spiInstance]->CR1 &= ~SPI_CR1_SPE;
        DisableClockSpi(spiInstance);
    }

    void SpiMasterStm::SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, SpiAction nextAction, const infra::Function<void()>& onDone)
    {
        this->onDone = onDone;
        if (chipSelectConfigurator && !continuedSession)
            chipSelectConfigurator->StartSession();
        continuedSession = nextAction == SpiAction::continueSession;

        assert(sendData.size() == receiveData.size() || sendData.empty() || receiveData.empty());
        this->sendData = sendData;
        this->receiveData = receiveData;
        sending = !sendData.empty();
        receiving = !receiveData.empty();

        if (!sending)
            dummyToSend = receiveData.size();
        if (!receiving)
            dummyToReceive = sendData.size();

        really_assert(!spiInterruptRegistration);
        spiInterruptRegistration.emplace(peripheralSpiIrq[spiInstance], [this]()
            {
                HandleInterrupt();
            });

#ifdef SPI_IER_RXPIE
        peripheralSpi[spiInstance]->IER |= SPI_IER_TXPIE;
        peripheralSpi[spiInstance]->IER |= SPI_IER_RXPIE;
#else
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_TXEIE;
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_RXNEIE;
#endif
    }

    void SpiMasterStm::SetChipSelectConfigurator(ChipSelectConfigurator& configurator)
    {
        chipSelectConfigurator = &configurator;
    }

    void SpiMasterStm::SetCommunicationConfigurator(CommunicationConfigurator& configurator)
    {
        communicationConfigurator = &configurator;
        communicationConfigurator->ActivateConfiguration();
    }

    void SpiMasterStm::ResetCommunicationConfigurator()
    {
        if (communicationConfigurator)
            communicationConfigurator->DeactivateConfiguration();

        communicationConfigurator = nullptr;
    }

    void SpiMasterStm::HandleInterrupt()
    {
        uint32_t status = peripheralSpi[spiInstance]->SR;
#ifdef SPI_SR_RXNE
        if ((status & SPI_SR_RXNE) != 0)
#else
        if ((status & SPI_SR_RXP) != 0)
#endif
        {
            if (dummyToReceive != 0)
            {
#ifdef SPI_DR_DR
                (void)peripheralSpi[spiInstance]->DR;
#else
                (void)peripheralSpi[spiInstance]->RXDR;
#endif
                --dummyToReceive;
            }
            else if (receiving)
            {
#ifdef SPI_DR_DR
                receiveData.front() = peripheralSpi[spiInstance]->DR;
#else
                receiveData.front() = peripheralSpi[spiInstance]->RXDR;
#endif
                receiveData.pop_front();
            }

            receiving &= !receiveData.empty();

            if (dummyToReceive == 0 && !receiving)
#ifdef SPI_CR2_RXNEIE
                peripheralSpi[spiInstance]->CR2 &= ~SPI_CR2_RXNEIE;
#else
                peripheralSpi[spiInstance]->IER &= ~SPI_IER_RXPIE;
#endif
        }
#ifdef SPI_SR_TXE
        if ((status & SPI_SR_TXE) != 0)
#else
        if ((status & SPI_SR_TXP) != 0)
#endif
        {
            if (dummyToSend != 0)
            {
#ifdef SPI_DR_DR
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->DR) = 0;
#else
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->TXDR) = 0;
#endif
                --dummyToSend;
            }
            else if (sending)
            {
#ifdef SPI_DR_DR
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->DR) = sendData.front();
#else
                reinterpret_cast<volatile uint8_t&>(peripheralSpi[spiInstance]->TXDR) = sendData.front();
                peripheralSpi[spiInstance]->CR1 |= SPI_CR1_CSTART;
#endif
                sendData.pop_front();
            }

            sending &= !sendData.empty();

            // After the first transmit, disable interrupt on transmit buffer empty,
            // so that a receive is done before each transmit
#ifdef SPI_CR2_TXEIE
            peripheralSpi[spiInstance]->CR2 &= ~SPI_CR2_TXEIE;
#else
            peripheralSpi[spiInstance]->IER &= ~SPI_IER_TXPIE;
#endif
        }

        really_assert(!(peripheralSpi[spiInstance]->SR & SPI_SR_OVR));

        spiInterruptRegistration->ClearPending();

        if (!sending && !receiving && dummyToSend == 0 && dummyToReceive == 0)
        {
            spiInterruptRegistration.reset();
            if (chipSelectConfigurator && !continuedSession)
                chipSelectConfigurator->EndSession();
            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    onDone();
                });
        }
    }
}
