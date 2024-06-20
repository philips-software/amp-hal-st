#include "hal_st/stm32fxxx/SpiMasterStmDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"

namespace hal
{
    SpiMasterStmDma::SpiMasterStmDma(hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, const Config& config, GpioPinStm& slaveSelect)
        : spiInstance(oneBasedSpiIndex - 1)
        , clock(clock, PinConfigTypeStm::spiClock, oneBasedSpiIndex)
        , miso(miso, PinConfigTypeStm::spiMiso, oneBasedSpiIndex)
        , mosi(mosi, PinConfigTypeStm::spiMosi, oneBasedSpiIndex)
        , slaveSelect(slaveSelect, PinConfigTypeStm::spiSlaveSelect, oneBasedSpiIndex)
#ifdef SPI_TXDR_TXDR
        , tx(transmitStream, &peripheralSpi[spiInstance]->TXDR, 1, [this]()
#else
        , tx(transmitStream, &peripheralSpi[spiInstance]->DR, 1, [this]()
#endif
              {
                  SendDone();
              })
#ifdef SPI_RXDR_RXDR
        , rx(receiveStream, &peripheralSpi[spiInstance]->RXDR, 1, [this]()
#else
        , rx(receiveStream, &peripheralSpi[spiInstance]->DR, 1, [this]()
#endif
              {
                  ReceiveDone();
              })
    {
        EnableClockSpi(spiInstance);

        SPI_HandleTypeDef hspi{};
        hspi.Instance = peripheralSpi[spiInstance];
        hspi.Init.Mode = SPI_MODE_MASTER;
        hspi.Init.Direction = SPI_DIRECTION_2LINES;
        hspi.Init.DataSize = SPI_DATASIZE_8BIT;
        hspi.Init.CLKPolarity = config.polarityLow ? SPI_POLARITY_LOW : SPI_POLARITY_HIGH;
        hspi.Init.CLKPhase = config.phase1st ? SPI_PHASE_1EDGE : SPI_PHASE_2EDGE;
        hspi.Init.NSS = SPI_NSS_SOFT;
        hspi.Init.BaudRatePrescaler = config.baudRatePrescaler;
        hspi.Init.FirstBit = config.msbFirst ? SPI_FIRSTBIT_MSB : SPI_FIRSTBIT_LSB;
        hspi.Init.TIMode = SPI_TIMODE_DISABLED;
        hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
        hspi.Init.CRCPolynomial = 1;

#ifdef SPI_CRC_LENGTH_DATASIZE
        hspi.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
#endif
#ifdef SPI_NSS_PULSE_DISABLE
        hspi.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif
#ifdef SPI_CFG2_RDIMM
        hspi.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
#endif
#ifdef SPI_CFG2_RDIOP
        hspi.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
#endif
#ifdef SPI_CFG1_FTHLV
        hspi.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
#endif
#ifdef SPI_CR1_MASRX
        hspi.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
#endif
        HAL_SPI_Init(&hspi);

        EnableDma();
    }

    SpiMasterStmDma::~SpiMasterStmDma()
    {
        peripheralSpi[spiInstance]->CR1 &= ~SPI_CR1_SPE;
        DisableClockSpi(spiInstance);
    }

    void SpiMasterStmDma::SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, SpiAction nextAction, const infra::Function<void()>& onDone)
    {
        this->onDone = onDone;

        if (chipSelectConfigurator && !continuedSession)
            chipSelectConfigurator->StartSession();
        continuedSession = nextAction == SpiAction::continueSession;

        SendAndReceive(sendData, receiveData);
    }

    void SpiMasterStmDma::SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData)
    {
        const auto dataSize = DataSize();
        const auto maxDmaSize = 65535 - (dataSize > 8 ? 1 : 0);

        sendBuffer = infra::DiscardHead(sendData, maxDmaSize);
        sendData = infra::Head(sendData, maxDmaSize);
        receiveBuffer = infra::DiscardHead(receiveData, maxDmaSize);
        receiveData = infra::Head(receiveData, maxDmaSize);

        receiveDone = false;
        sendDone = false;

        if (!sendData.empty() && !receiveData.empty())
        {
            assert(sendData.size() == receiveData.size());

            if (dataSize <= 8)
            {
                rx.StartReceive(receiveData);
                tx.StartTransmit(sendData);
            }
            else
            {
                rx.StartReceive(infra::ReinterpretCastMemoryRange<uint16_t>(receiveData));
                tx.StartTransmit(infra::ReinterpretCastMemoryRange<const uint16_t>(sendData));
            }
        }
        else if (!sendData.empty())
        {
            rx.StartReceiveDummy(sendData.size(), dataSize <= 8 ? 1 : 2);

            if (dataSize <= 8)
                tx.StartTransmit(sendData);
            else
                tx.StartTransmit(infra::ReinterpretCastMemoryRange<const uint16_t>(sendData));
        }
        else if (!receiveData.empty())
        {
            if (dataSize <= 8)
                rx.StartReceive(receiveData);
            else
                rx.StartReceive(infra::ReinterpretCastMemoryRange<uint16_t>(receiveData));

            tx.StartTransmitDummy(receiveData.size(), dataSize <= 8 ? 1 : 2);
        }
        else
            std::abort();
        
#ifdef SPI_CR1_CSTART
        peripheralSpi[spiInstance]->CR1 |= SPI_CR1_CSTART;
#endif
        peripheralSpi[spiInstance]->CR1 |= SPI_CR1_SPE;
    }

    void SpiMasterStmDma::SetChipSelectConfigurator(ChipSelectConfigurator& configurator)
    {
        chipSelectConfigurator = &configurator;
    }

    void SpiMasterStmDma::SetCommunicationConfigurator(CommunicationConfigurator& configurator)
    {
        assert(communicationConfigurator == nullptr);
        communicationConfigurator = &configurator;
        communicationConfigurator->ActivateConfiguration();
    }

    void SpiMasterStmDma::ResetCommunicationConfigurator()
    {
        if (communicationConfigurator)
            communicationConfigurator->DeactivateConfiguration();

        communicationConfigurator = nullptr;
    }

    void SpiMasterStmDma::SetDataSize(uint8_t dataSizeInBits)
    {
        peripheralSpi[spiInstance]->CR1 &= ~SPI_CR1_SPE;
        DisableDma();

        tx.SetPeripheralTransferSize(dataSizeInBits <= 8 ? 1 : 2);
        rx.SetPeripheralTransferSize(dataSizeInBits <= 8 ? 1 : 2);

#if defined(SPI_CR2_DS)
        assert(dataSizeInBits >= 4 && dataSizeInBits <= 16);
        peripheralSpi[spiInstance]->CR2 = (peripheralSpi[spiInstance]->CR2 & ~SPI_CR2_DS) | ((dataSizeInBits - 1) << POSITION_VAL(SPI_CR2_DS)) | (dataSizeInBits <= 8 ? SPI_CR2_FRXTH : 0);
#elif defined(SPI_CFG1_DSIZE)
        assert(dataSizeInBits >= 4 || dataSizeInBits <= 32);
        peripheralSpi[spiInstance]->CFG1 = (peripheralSpi[spiInstance]->CFG1 & ~SPI_CFG1_DSIZE) | ((dataSizeInBits - 1) << POSITION_VAL(SPI_CFG1_DSIZE));
#else
        assert(dataSizeInBits == 8 || dataSizeInBits == 16);
        peripheralSpi[spiInstance]->CR1 = peripheralSpi[spiInstance]->CR1 & ~SPI_CR1_DFF | (dataSizeInBits == 16 ? SPI_CR1_DFF : 0);
#endif

        EnableDma();
        peripheralSpi[spiInstance]->CR1 |= SPI_CR1_SPE;
    }

    uint8_t SpiMasterStmDma::DataSize() const
    {
#if defined(SPI_CR2_DS)
        return ((peripheralSpi[spiInstance]->CR2 & SPI_CR2_DS) >> POSITION_VAL(SPI_CR2_DS)) + 1;
#elif defined(SPI_CFG1_DSIZE)
        return ((peripheralSpi[spiInstance]->CFG1 & SPI_CFG1_DSIZE) >> POSITION_VAL(SPI_CFG1_DSIZE)) + 1;
#else
        if ((peripheralSpi[spiInstance]->CR1 & SPI_CR1_DFF) != 0)
            return 16;
        else
            return 8;
#endif
    }

    void SpiMasterStmDma::ReceiveDone()
    {
        receiveDone = true;

        if (sendDone)
            TransferDone();
    }

    void SpiMasterStmDma::SendDone()
    {
        sendDone = true;

        if (receiveDone)
            TransferDone();
    }

    void SpiMasterStmDma::TransferDone()
    {
        if (!sendBuffer.empty() || !receiveBuffer.empty())
            SendAndReceive(sendBuffer, receiveBuffer);
        else
        {
            if (chipSelectConfigurator && !continuedSession)
                chipSelectConfigurator->EndSession();

            onDone();
        }
    }

    void SpiMasterStmDma::EnableDma()
    {
#ifdef SPI_CR2_RXDMAEN
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_RXDMAEN;
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_TXDMAEN;
#else
        peripheralSpi[spiInstance]->CFG1 |= SPI_CFG1_RXDMAEN;
        peripheralSpi[spiInstance]->CFG1 |= SPI_CFG1_TXDMAEN;
#endif
    }

    void SpiMasterStmDma::DisableDma()
    {
#ifdef SPI_CR2_TXDMAEN
        peripheralSpi[spiInstance]->CR2 &= ~SPI_CR2_TXDMAEN;
        peripheralSpi[spiInstance]->CR2 &= ~SPI_CR2_RXDMAEN;
#else
        peripheralSpi[spiInstance]->CFG1 &= ~SPI_CFG1_TXDMAEN;
        peripheralSpi[spiInstance]->CFG1 &= ~SPI_CFG1_RXDMAEN;
#endif
    }
}
