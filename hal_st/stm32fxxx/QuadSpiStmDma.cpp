#include "hal_st/stm32fxxx/QuadSpiStmDma.hpp"

#if defined(HAS_PERIPHERAL_QUADSPI)

namespace hal
{
    namespace
    {
#if defined (STM32F765xx) || defined(STM32F767xx) || defined(STM32F769xx) || defined(STM32F777xx) || defined(STM32F779xx)
        const DmaChannelId quadSpiDmaChannel{ 2, 2, 11 };
#else
        const DmaChannelId quadSpiDmaChannel{ 2, 7, 3 };
#endif
    }

    QuadSpiStmDma::QuadSpiStmDma(hal::DmaStm& dma, GpioPinStm& clock, GpioPinStm& slaveSelect, GpioPinStm& data0, GpioPinStm& data1, GpioPinStm& data2, GpioPinStm& data3, const Config& config)
        : clock(clock, PinConfigTypeStm::quadSpiClock, 0)
        , slaveSelect(slaveSelect, PinConfigTypeStm::quadSpiSlaveSelect, 0)
        , data0(data0, PinConfigTypeStm::quadSpiData0, 0)
        , data1(data1, PinConfigTypeStm::quadSpiData1, 0)
        , data2(data2, PinConfigTypeStm::quadSpiData2, 0)
        , data3(data3, PinConfigTypeStm::quadSpiData3, 0)
        , dmaStream(dma, quadSpiDmaChannel, &QUADSPI->DR, [this]() { OnDmaTransferDone(); })
        , interruptHandler(QUADSPI_IRQn, [this]() { OnInterrupt(); })
    {
        EnableClockQuadSpi(0);

        QUADSPI->CR = (config.prescaler << 24) | QUADSPI_CR_APMS | QSPI_SAMPLE_SHIFTING_HALFCYCLE | (32 << 8) | QUADSPI_CR_EN;
        QUADSPI->DCR = ((config.flashSizeLog2 - 1) << 16) | QSPI_CS_HIGH_TIME_2_CYCLE;
    }

    QuadSpiStmDma::~QuadSpiStmDma()
    {
        QUADSPI->CR = 0;
        DisableClockQuadSpi(0);
    }

    void QuadSpiStmDma::SendData(const Header& header, infra::ConstByteRange data, Lines lines, const infra::Function<void()>& actionOnCompletion)
    {
        assert(!onDone);
        onDone = actionOnCompletion;

        if (!data.empty())
        {
            QUADSPI->DLR = data.size() - 1;
            //QUADSPI->CCR = (QUADSPI->CCR & ~QUADSPI_CCR_FMODE);
        }
        else
            QUADSPI->CR |= QUADSPI_CR_TCIE;

        SetConfig(header, data.size(), lines, FunctionalMode::write);

        if (!data.empty())
            dmaStream.StartTransmit(data);
    }

    void QuadSpiStmDma::ReceiveData(const Header& header, infra::ByteRange data, Lines lines, const infra::Function<void()>& actionOnCompletion)
    {
        assert(!onDone);
        onDone = actionOnCompletion;

        if (!data.empty())
        {
            QUADSPI->DLR = data.size() - 1;

            dmaStream.StartReceive(data);
        }
        else
            QUADSPI->CR |= QUADSPI_CR_TCIE;

        SetConfig(header, data.size(), lines, FunctionalMode::read);
    }

    void QuadSpiStmDma::PollStatus(const Header& header, uint8_t nofBytes, uint32_t match, uint32_t mask, Lines lines, const infra::Function<void()>& actionOnCompletion)
    {
        onDone = actionOnCompletion;

        QUADSPI->PSMKR = mask;
        QUADSPI->PSMAR = match;
        QUADSPI->PIR = 16;
        QUADSPI->DLR = nofBytes - 1;

        SetConfig(header, nofBytes, lines, FunctionalMode::poll);

        QUADSPI->CR |= QUADSPI_CR_SMIE;
    }

    void QuadSpiStmDma::SetConfig(const Header& header, uint32_t dataSize, Lines lines, FunctionalMode mode)
    {
        uint32_t ccr = QUADSPI->CCR & QUADSPI_CCR_DDRM | QUADSPI_CCR_DHHC;

        if (mode == FunctionalMode::read)
            ccr |= QUADSPI_CCR_FMODE_0;
        else if (mode == FunctionalMode::poll)
            ccr |= QUADSPI_CCR_FMODE_1;

        if (header.instruction)
        {
            static const std::array<uint32_t, 5> ccrLines = { {
                0,
                QSPI_INSTRUCTION_1_LINE,
                QSPI_INSTRUCTION_2_LINES,
                0,
                QSPI_INSTRUCTION_4_LINES
            } };

            ccr |= ccrLines[lines.instructionLines] | *header.instruction;
        }

        if (!header.address.empty())
        {
            static const std::array<uint32_t, 5> ccrLines = { {
                0,
                QSPI_ADDRESS_1_LINE,
                QSPI_ADDRESS_2_LINES,
                0,
                QSPI_ADDRESS_4_LINES
            } };

            ccr |= ccrLines[lines.addressLines] | (((header.address.size() - 1) & 0x03) << 12);
        }

        if (!header.alternate.empty())
        {
            static const std::array<uint32_t, 5> ccrLines = { {
                0,
                QSPI_ALTERNATE_BYTES_1_LINE,
                QSPI_ALTERNATE_BYTES_2_LINES,
                0,
                QSPI_ALTERNATE_BYTES_4_LINES
            } };

            ccr |= ccrLines[lines.alternateLines] | (((header.alternate.size() - 1) & 0x03) << 16);
            QUADSPI->ABR = *reinterpret_cast<const uint32_t*>(header.alternate.data());
        }

        if (header.nofDummyCycles != 0)
            ccr |= static_cast<uint32_t>(header.nofDummyCycles) << 18;

        if (dataSize != 0)
        {
            static const std::array<uint32_t, 5> ccrLines = { {
                0,
                QSPI_DATA_1_LINE,
                QSPI_DATA_2_LINES,
                0,
                QSPI_DATA_4_LINES
            } };

            ccr |= ccrLines[lines.dataLines];
        }

        QUADSPI->CCR = ccr;

        if (!header.address.empty())
            QUADSPI->AR = VectorToAddress(header.address);

        if (dataSize != 0 && mode != FunctionalMode::poll)
            QUADSPI->CR |= QUADSPI_CR_DMAEN;
    }

    void QuadSpiStmDma::OnDmaTransferDone()
    {
        QUADSPI->FCR |= QUADSPI_FCR_CTCF;
        QUADSPI->CR &= ~QUADSPI_CR_DMAEN;
        onDone();
    }

    void QuadSpiStmDma::OnInterrupt()
    {
        QUADSPI->FCR |= QUADSPI_FCR_CTCF | QUADSPI_FCR_CSMF;
        QUADSPI->CR &= ~(QUADSPI_CR_TCIE | QUADSPI_CR_SMIE);
        interruptHandler.ClearPending();
        onDone();
    }
}

#endif
