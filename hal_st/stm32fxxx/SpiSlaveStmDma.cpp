#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/SpiSlaveStmDma.hpp"
#include "infra/util/BitLogic.hpp"

namespace hal
{
    namespace
    {
        const std::array<std::pair<DmaChannelId, DmaChannelId>, 6> defaultDmaChannelId = {{
            std::make_pair(DmaChannelId{ 2, 3, 3 }, DmaChannelId{ 2, 0, 3 }),
            std::make_pair(DmaChannelId{ 1, 4, 0 }, DmaChannelId{ 1, 3, 0 }),
            std::make_pair(DmaChannelId{ 1, 5, 0 }, DmaChannelId{ 1, 0, 0 }),
            std::make_pair(DmaChannelId{ 2, 1, 4 }, DmaChannelId{ 2, 0, 4 }),
            std::make_pair(DmaChannelId{ 2, 4, 2 }, DmaChannelId{ 2, 3, 2 }),
            std::make_pair(DmaChannelId{ 2, 5, 1 }, DmaChannelId{ 2, 6, 1 })
        }};
    }

    SpiSlaveStmDma::SpiSlaveStmDma(hal::DmaStm& dmaStm, uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, GpioPinStm& slaveSelect, const Config& config)
        : spiInstance(oneBasedSpiIndex - 1)
        , clock(clock, PinConfigTypeStm::spiClock, oneBasedSpiIndex)
        , miso(miso, PinConfigTypeStm::spiMiso, oneBasedSpiIndex)
        , mosi(mosi, PinConfigTypeStm::spiMosi, oneBasedSpiIndex)
        , slaveSelect(slaveSelect, PinConfigTypeStm::spiSlaveSelect, oneBasedSpiIndex)
        , rx(dmaStm, config.dmaChannelRx.ValueOr(defaultDmaChannelId[spiInstance].second), &peripheralSpi[spiInstance]->DR, [this]() { ReceiveDone(); })
        , tx(dmaStm, config.dmaChannelTx.ValueOr(defaultDmaChannelId[spiInstance].first), &peripheralSpi[spiInstance]->DR, [this]() { SendDone(); })
    {
        ResetPeripheralSpi(spiInstance);
        EnableClockSpi(spiInstance);
        Configure();
    }

    void SpiSlaveStmDma::SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData, const infra::Function<void()>& onDone)
    {
        this->onDone = onDone;

        SendAndReceive(sendData, receiveData);
    }

    bool SpiSlaveStmDma::CancelTransmission()
    {
        bool rxCancelled = rx.StopTransfer();
        bool txCancelled = tx.StopTransfer();

        onDone = nullptr;

        ResetPeripheralSpi(spiInstance);

        Configure();

        return rxCancelled || txCancelled;
    }

    void SpiSlaveStmDma::SendAndReceive(infra::ConstByteRange sendData, infra::ByteRange receiveData)
    {
        uint32_t maxDmaSize = 65535 /*- (DataSize() > 8 ? 1 : 0)*/;
        assert(sendData.size() <= maxDmaSize);
        assert(receiveData.size() <= maxDmaSize);

        receiveDone = false;
        sendDone = false;

        if (!sendData.empty() && !receiveData.empty())
        {
            assert(sendData.size() == receiveData.size());
            rx.StartReceive(receiveData);
            tx.StartTransmit(sendData);
        }
        else if(!sendData.empty())
        {
            rx.StartReceiveDummy(sendData.size());
            tx.StartTransmit(sendData);
        }
        else if(!receiveData.empty())
        {
            rx.StartReceive(receiveData);
            tx.StartTransmitDummy(receiveData.size());
        }
        else
            std::abort();
    }

    void SpiSlaveStmDma::ReceiveDone()
    {
        receiveDone = true;

        if (sendDone)
            TransferDone();
    }

    void SpiSlaveStmDma::SendDone()
    {
        sendDone = true;

        if (receiveDone)
            TransferDone();
    }

    void SpiSlaveStmDma::TransferDone()
    {
        onDone();
    }

    void SpiSlaveStmDma::Configure()
    {
        SPI_HandleTypeDef spiHandle;
        spiHandle.Instance = peripheralSpi[spiInstance];
        spiHandle.Init.Mode = SPI_MODE_SLAVE;
        spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
        spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
        spiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
        spiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
        spiHandle.Init.NSS = SPI_NSS_HARD_INPUT;
        spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
        spiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
        spiHandle.Init.TIMode = SPI_TIMODE_DISABLED;
        spiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
        spiHandle.Init.CRCPolynomial = 1;
#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7)
        spiHandle.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
        spiHandle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif
        HAL_SPI_Init(&spiHandle);

#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7)
        peripheralSpi[spiInstance]->CR2 |= SPI_RXFIFO_THRESHOLD_QF;
#endif

        __HAL_SPI_ENABLE(&spiHandle);
        EnableDma();
    }

    void SpiSlaveStmDma::EnableDma()
    {
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_RXDMAEN;
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_TXDMAEN;
    }

    void SpiSlaveStmDma::DisableDma()
    {
        peripheralSpi[spiInstance]->CR2 &= ~SPI_CR2_TXDMAEN;
        peripheralSpi[spiInstance]->CR2 &= ~SPI_CR2_RXDMAEN;
    }
}
