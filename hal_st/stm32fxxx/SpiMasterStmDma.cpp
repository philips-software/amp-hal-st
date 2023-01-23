#include "hal_st/stm32fxxx/SpiMasterStmDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"

namespace hal
{
    namespace
    {
        const std::array<std::pair<DmaChannelId, DmaChannelId>, 6> defaultDmaChannelId = { { std::make_pair(DmaChannelId{ 2, 3, 3 }, DmaChannelId{ 2, 0, 3 }),
            std::make_pair(DmaChannelId{ 1, 4, 0 }, DmaChannelId{ 1, 3, 0 }),
            std::make_pair(DmaChannelId{ 1, 5, 0 }, DmaChannelId{ 1, 0, 0 }),
            std::make_pair(DmaChannelId{ 2, 1, 4 }, DmaChannelId{ 2, 0, 4 }),
            std::make_pair(DmaChannelId{ 2, 4, 2 }, DmaChannelId{ 2, 3, 2 }),
            std::make_pair(DmaChannelId{ 2, 5, 1 }, DmaChannelId{ 2, 6, 1 }) } };
    }

    SpiMasterStmDma::SpiMasterStmDma(hal::DmaStm& dmaStm, uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, const Config& config, GpioPinStm& slaveSelect)
        : spiInstance(oneBasedSpiIndex - 1)
        , clock(clock, PinConfigTypeStm::spiClock, oneBasedSpiIndex)
        , miso(miso, PinConfigTypeStm::spiMiso, oneBasedSpiIndex)
        , mosi(mosi, PinConfigTypeStm::spiMosi, oneBasedSpiIndex)
        , slaveSelect(slaveSelect, PinConfigTypeStm::spiSlaveSelect, oneBasedSpiIndex)
        , rx(dmaStm, config.dmaChannelRx.ValueOr(defaultDmaChannelId[spiInstance].second), &peripheralSpi[spiInstance]->DR, [this]()
              { ReceiveDone(); })
        , tx(dmaStm, config.dmaChannelTx.ValueOr(defaultDmaChannelId[spiInstance].first), &peripheralSpi[spiInstance]->DR, [this]()
              { SendDone(); })
    {
        EnableClockSpi(spiInstance);

        SPI_HandleTypeDef hspi;
        hspi.Instance = peripheralSpi[spiInstance];
        hspi.Init.Mode = SPI_MODE_MASTER;
        hspi.Init.Direction = SPI_DIRECTION_2LINES;
        hspi.Init.DataSize = SPI_DATASIZE_8BIT;
        hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
        hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi.Init.NSS = SPI_NSS_SOFT;
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7)
        hspi.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif
        hspi.Init.BaudRatePrescaler = config.baudRatePrescaler;
        hspi.Init.FirstBit = config.msbFirst ? SPI_FIRSTBIT_MSB : SPI_FIRSTBIT_LSB;
        hspi.Init.TIMode = SPI_TIMODE_DISABLED;
        hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
        hspi.Init.CRCPolynomial = 1;
        HAL_SPI_Init(&hspi);

        __HAL_SPI_ENABLE(&hspi);
        EnableDma();
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
        uint32_t maxDmaSize = 65535 - (DataSize() > 8 ? 1 : 0);
        sendBuffer = infra::DiscardHead(sendData, maxDmaSize);
        sendData = infra::Head(sendData, maxDmaSize);
        receiveBuffer = infra::DiscardHead(receiveData, maxDmaSize);
        receiveData = infra::Head(receiveData, maxDmaSize);

        receiveDone = false;
        sendDone = false;

        if (!sendData.empty() && !receiveData.empty())
        {
            assert(sendData.size() == receiveData.size());
            rx.StartReceive(receiveData);
            tx.StartTransmit(sendData);
        }
        else if (!sendData.empty())
        {
            rx.StartReceiveDummy(sendData.size());
            tx.StartTransmit(sendData);
        }
        else if (!receiveData.empty())
        {
            rx.StartReceive(receiveData);
            tx.StartTransmitDummy(receiveData.size());
        }
        else
            std::abort();
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
        DisableDma();
        peripheralSpi[spiInstance]->CR1 &= ~SPI_CR1_SPE;

#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7)
        assert(dataSizeInBits >= 4 && dataSizeInBits <= 16);
        peripheralSpi[spiInstance]->CR2 = (peripheralSpi[spiInstance]->CR2 & ~SPI_CR2_DS) | ((dataSizeInBits - 1) << POSITION_VAL(SPI_CR2_DS)) | (dataSizeInBits <= 8 ? SPI_CR2_FRXTH : 0);
#else
        assert(dataSizeInBits == 8 || dataSizeInBits == 16);
        peripheralSpi[spiInstance]->CR1 = peripheralSpi[spiInstance]->CR1 & ~SPI_CR1_DFF | (dataSizeInBits == 16 ? SPI_CR1_DFF : 0);
#endif
        tx.SetDataSize(dataSizeInBits <= 8 ? 1 : 2);
        rx.SetDataSize(dataSizeInBits <= 8 ? 1 : 2);

        peripheralSpi[spiInstance]->CR1 |= SPI_CR1_SPE;
        EnableDma();
    }

    uint8_t SpiMasterStmDma::DataSize() const
    {
#if defined(STM32F0) || defined(STM32F3) || defined(STM32F7)
        return ((peripheralSpi[spiInstance]->CR2 & SPI_CR2_DS) >> POSITION_VAL(SPI_CR2_DS)) + 1;
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
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_RXDMAEN;
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_TXDMAEN;
    }

    void SpiMasterStmDma::DisableDma()
    {
        peripheralSpi[spiInstance]->CR2 &= ~SPI_CR2_TXDMAEN;
        peripheralSpi[spiInstance]->CR2 &= ~SPI_CR2_RXDMAEN;
    }
}
