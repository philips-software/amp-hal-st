#ifndef HAL_QUAD_SPI_STM_HPP
#define HAL_QUAD_SPI_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/interfaces/QuadSpi.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/AutoResetFunction.hpp"

#if defined(HAS_PERIPHERAL_QUADSPI)

namespace hal
{
    struct QuadSpiStmConfig
    {
        uint32_t prescaler = 2;
        uint32_t flashSizeLog2 = 24;
    };

    class QuadSpiStm
        : public QuadSpi
    {
    public:
        using Config = QuadSpiStmConfig;

        QuadSpiStm(GpioPinStm& clock, GpioPinStm& slaveSelect, GpioPinStm& data0, GpioPinStm& data1, GpioPinStm& data2, GpioPinStm& data3, const Config& config = Config());
        ~QuadSpiStm();

        virtual void SendData(const Header& header, infra::ConstByteRange data, Lines lines, const infra::Function<void()>& actionOnCompletion) override;
        virtual void ReceiveData(const Header& header, infra::ByteRange data, Lines lines, const infra::Function<void()>& actionOnCompletion) override;
        virtual void PollStatus(const Header& header, uint8_t nofBytes, uint32_t match, uint32_t mask, Lines lines, const infra::Function<void()>& actionOnCompletion) override;

    private:
        QSPI_CommandTypeDef CreateConfig(const Header& header, uint32_t dataSize, Lines lines);

    private:
        PeripheralPinStm clock;
        PeripheralPinStm slaveSelect;
        PeripheralPinStm data0;
        PeripheralPinStm data1;
        PeripheralPinStm data2;
        PeripheralPinStm data3;
        infra::AutoResetFunction<void()> onDone;
        QSPI_HandleTypeDef handle;
    };
}

#endif

#endif
