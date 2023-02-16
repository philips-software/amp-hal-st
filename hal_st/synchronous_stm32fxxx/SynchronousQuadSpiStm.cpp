#include "hal_st/synchronous_stm32fxxx/SynchronousQuadSpiStm.hpp"
#include "cmsis_device.h"
#include <limits>

#if defined(HAS_PERIPHERAL_QUADSPI)

namespace hal
{
    const hal::SmallPeripheralPinStm::Definition<1> SynchronousQuadSpiStm::clock = {
        { { { 0, hal::Port::B, 2, GPIO_AF9_QUADSPI } } }, hal::Drive::Default, hal::Speed::High, hal::WeakPull::Default
    };

    const hal::SmallPeripheralPinStm::Definition<1> SynchronousQuadSpiStm::slaveSelect = {
        { { { 0, hal::Port::B, 6, GPIO_AF10_QUADSPI } } }, hal::Drive::Default, hal::Speed::High, hal::WeakPull::Default
    };

    const hal::SmallPeripheralPinStm::Definition<3> SynchronousQuadSpiStm::data0 = {
        { { { 0, hal::Port::C, 9, GPIO_AF9_QUADSPI }, { 0, hal::Port::D, 11, GPIO_AF9_QUADSPI }, { 0, hal::Port::F, 8, GPIO_AF10_QUADSPI } } }, hal::Drive::Default, hal::Speed::High, hal::WeakPull::Default
    };

    const hal::SmallPeripheralPinStm::Definition<3> SynchronousQuadSpiStm::data1 = {
        { { { 0, hal::Port::C, 10, GPIO_AF9_QUADSPI }, { 0, hal::Port::D, 12, GPIO_AF9_QUADSPI }, { 0, hal::Port::F, 9, GPIO_AF10_QUADSPI } } }, hal::Drive::Default, hal::Speed::High, hal::WeakPull::Default
    };

    const hal::SmallPeripheralPinStm::Definition<2> SynchronousQuadSpiStm::data2 = {
        { { { 0, hal::Port::E, 2, GPIO_AF9_QUADSPI }, { 0, hal::Port::F, 7, GPIO_AF9_QUADSPI } } }, hal::Drive::Default, hal::Speed::High, hal::WeakPull::Default
    };

    const hal::SmallPeripheralPinStm::Definition<3> SynchronousQuadSpiStm::data3 = {
        { { { 0, hal::Port::A, 1, GPIO_AF9_QUADSPI }, { 0, hal::Port::D, 13, GPIO_AF9_QUADSPI }, { 0, hal::Port::F, 6, GPIO_AF9_QUADSPI } } }, hal::Drive::Default, hal::Speed::High, hal::WeakPull::Default
    };

    SynchronousQuadSpiStm::SynchronousQuadSpiStm(const Config& config)
        : handle()
    {
        __QSPI_CLK_ENABLE();

        handle.Instance = QUADSPI;

        handle.Init.ClockPrescaler = config.prescaler;
        handle.Init.FifoThreshold = 32;
        handle.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
        handle.Init.FlashSize = config.flashSizeLog2 - 1;
        handle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE;
        handle.Init.ClockMode = QSPI_CLOCK_MODE_0;
        handle.Init.FlashID = QSPI_FLASH_ID_1;
        handle.Init.DualFlash = QSPI_DUALFLASH_DISABLE;

        HAL_QSPI_Init(&handle);
    }

    SynchronousQuadSpiStm::~SynchronousQuadSpiStm()
    {
        HAL_QSPI_DeInit(&handle);
        __QSPI_CLK_DISABLE();
    }

    void SynchronousQuadSpiStm::SendData(const Header& header, infra::ConstByteRange data)
    {
        QSPI_CommandTypeDef command = CreateConfig(header, data.size(), false);
        HAL_StatusTypeDef status = HAL_QSPI_Command(&handle, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
        assert(status == HAL_OK);

        if (!data.empty())
        {
            HAL_StatusTypeDef status = HAL_QSPI_Transmit(&handle, const_cast<uint8_t*>(data.begin()), HAL_MAX_DELAY);
            assert(status == HAL_OK);
        }
    }

    void SynchronousQuadSpiStm::SendDataQuad(const Header& header, infra::ConstByteRange data)
    {
        QSPI_CommandTypeDef command = CreateConfig(header, data.size(), true);
        HAL_StatusTypeDef status = HAL_QSPI_Command(&handle, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
        assert(status == HAL_OK);

        if (!data.empty())
        {
            HAL_StatusTypeDef status = HAL_QSPI_Transmit(&handle, const_cast<uint8_t*>(data.begin()), HAL_MAX_DELAY);
            assert(status == HAL_OK);
        }
    }

    void SynchronousQuadSpiStm::ReceiveData(const Header& header, infra::ByteRange data)
    {
        QSPI_CommandTypeDef command = CreateConfig(header, data.size(), false);
        HAL_StatusTypeDef status = HAL_QSPI_Command(&handle, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
        assert(status == HAL_OK);

        if (!data.empty())
        {
            HAL_StatusTypeDef status = HAL_QSPI_Receive(&handle, data.begin(), HAL_MAX_DELAY);
            assert(status == HAL_OK);
        }
    }

    void SynchronousQuadSpiStm::ReceiveDataQuad(const Header& header, infra::ByteRange data)
    {
        QSPI_CommandTypeDef command = CreateConfig(header, data.size(), true);
        HAL_StatusTypeDef status = HAL_QSPI_Command(&handle, &command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
        assert(status == HAL_OK);

        if (!data.empty())
        {
            HAL_StatusTypeDef status = HAL_QSPI_Receive(&handle, data.begin(), HAL_MAX_DELAY);
            assert(status == HAL_OK);
        }
    }

    QSPI_CommandTypeDef SynchronousQuadSpiStm::CreateConfig(const Header& header, uint32_t dataSize, bool quadSpeed)
    {
        QSPI_CommandTypeDef command;

        command.Instruction = header.instruction;
        command.Address = header.address;
        command.AddressSize = (3 - 1) << 12;
        command.AlternateBytes = 0;
        command.AlternateBytesSize = 0;
        command.DummyCycles = header.nofDummyCycles;
        command.InstructionMode = quadSpeed ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
        command.AddressMode = header.address != std::numeric_limits<uint32_t>::max() ? (quadSpeed ? QSPI_ADDRESS_4_LINES : QSPI_ADDRESS_1_LINE) : QSPI_ADDRESS_NONE;
        command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
        command.DataMode = dataSize != 0 ? (quadSpeed ? QSPI_DATA_4_LINES : QSPI_DATA_1_LINE) : QSPI_DATA_NONE;
        command.NbData = dataSize;
        command.DdrMode = QSPI_DDR_MODE_DISABLE;
        command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
        command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

        return command;
    }
}

#endif
