#ifndef SYNCHRONOUS_HAL_SYNCHRONOUS_QUAD_SPI_STM
#define SYNCHRONOUS_HAL_SYNCHRONOUS_QUAD_SPI_STM

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/synchronous_interfaces/SynchronousQuadSpi.hpp"
#include "hal_st/synchronous_stm32fxxx/SynchronousGpioStm.hpp"

#if defined(HAS_PERIPHERAL_QUADSPI)

namespace hal
{
    struct SingleSpeedSynchronousQuadSpiStmConfig
    {
        uint32_t prescaler = 2;
        uint32_t flashSizeLog2 = 24;
    };

    class SynchronousQuadSpiStm
        : public SynchronousQuadSpi
    {
    public:
        static const hal::SmallPeripheralPinStm::Definition<1> clock;
        static const hal::SmallPeripheralPinStm::Definition<1> slaveSelect;
        static const hal::SmallPeripheralPinStm::Definition<3> data0;
        static const hal::SmallPeripheralPinStm::Definition<3> data1;
        static const hal::SmallPeripheralPinStm::Definition<2> data2;
        static const hal::SmallPeripheralPinStm::Definition<3> data3;

    public:
        using Config = SingleSpeedSynchronousQuadSpiStmConfig;

        SynchronousQuadSpiStm(const Config& config = Config());
        ~SynchronousQuadSpiStm();

        void SendData(const Header& header, infra::ConstByteRange data) override;
        void SendDataQuad(const Header& header, infra::ConstByteRange data) override;
        void ReceiveData(const Header& header, infra::ByteRange data) override;
        void ReceiveDataQuad(const Header& header, infra::ByteRange data) override;

    private:
        QSPI_CommandTypeDef CreateConfig(const Header& header, uint32_t dataSize, bool quadSpeed);

    private:
        QSPI_HandleTypeDef handle;
    };
}

#endif

#endif
