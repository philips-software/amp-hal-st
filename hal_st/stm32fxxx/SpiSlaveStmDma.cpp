#include "hal_st/stm32fxxx/SpiSlaveStmDma.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/BitLogic.hpp"

namespace hal
{
    SpiSlaveStmDma::SpiSlaveStmDma(hal::DmaStm::TransmitStream& transmitStream, hal::DmaStm::ReceiveStream& receiveStream, uint8_t oneBasedSpiIndex, GpioPinStm& clock, GpioPinStm& miso, GpioPinStm& mosi, GpioPinStm& slaveSelect)
        : spiInstance(oneBasedSpiIndex - 1)
        , clock(clock, PinConfigTypeStm::spiClock, oneBasedSpiIndex)
        , miso(miso, PinConfigTypeStm::spiMiso, oneBasedSpiIndex)
        , mosi(mosi, PinConfigTypeStm::spiMosi, oneBasedSpiIndex)
        , slaveSelect(slaveSelect, PinConfigTypeStm::spiSlaveSelect, oneBasedSpiIndex)
#if !defined(STM32WBA) && !defined(STM32H5)
        , tx(transmitStream, &peripheralSpi[spiInstance]->DR, 1, [this]()
#else
        , tx(transmitStream, &peripheralSpi[spiInstance]->TXDR, 1, [this]()
#endif
              {
                  SendDone();
              })
#if !defined(STM32WBA) && !defined(STM32H5)
        , rx(receiveStream, &peripheralSpi[spiInstance]->DR, 1, [this]()
#else
        , rx(receiveStream, &peripheralSpi[spiInstance]->RXDR, 1, [this]()
#endif
              {
                  ReceiveDone();
              })
    {
        ResetPeripheralSpi(spiInstance);
        EnableClockSpi(spiInstance);
        Configure();
    }

    SpiSlaveStmDma::~SpiSlaveStmDma()
    {
        DisableSpi();
        DisableClockSpi(spiInstance);
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

#ifdef STM32H5
        DisableSpi();
#endif

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

#ifdef STM32H5
        EnableSpi();
#endif
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
#ifdef STM32H5
        DisableSpi();
#endif
        onDone();
    }

    void SpiSlaveStmDma::Configure()
    {
        SPI_HandleTypeDef spiHandle{};
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
#endif
#ifdef SPI_CFG1_FTHLV
        spiHandle.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
#endif
#ifdef SPI_NSS_PULSE_DISABLE
        spiHandle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif
#ifdef SPI_NSS_POLARITY_LOW
        spiHandle.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
#endif

        auto result = HAL_SPI_Init(&spiHandle);
        assert(result == HAL_OK);

#if defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32F7)
        peripheralSpi[spiInstance]->CR2 |= SPI_RXFIFO_THRESHOLD_QF;
#endif

        EnableDma();
    }

    void SpiSlaveStmDma::EnableSpi()
    {
        peripheralSpi[spiInstance]->CR1 |= SPI_CR1_SPE;
    }

    void SpiSlaveStmDma::DisableSpi()
    {
        peripheralSpi[spiInstance]->CR1 |= SPI_CR1_SPE;
    }

    void SpiSlaveStmDma::EnableDma()
    {
#ifdef SPI_CR2_RXDMAEN
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_RXDMAEN;
        peripheralSpi[spiInstance]->CR2 |= SPI_CR2_TXDMAEN;
#else
        peripheralSpi[spiInstance]->CFG1 |= SPI_CFG1_RXDMAEN;
        peripheralSpi[spiInstance]->CFG1 |= SPI_CFG1_TXDMAEN;
#endif
    }

    void SpiSlaveStmDma::DisableDma()
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
