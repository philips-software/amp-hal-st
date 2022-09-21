#ifndef HAL_SPI_DATA_SIZE_CONFIGURATOR_STM_HPP
#define HAL_SPI_DATA_SIZE_CONFIGURATOR_STM_HPP

#include "hal_st/stm32fxxx/SpiMasterStmDma.hpp"

namespace hal
{
    class SpiDataSizeConfiguratorStm
        : public CommunicationConfigurator
    {
    public:
        SpiDataSizeConfiguratorStm(SpiMasterStmDma& spi, uint32_t dataSize);    // dataSize = SPI_DATASIZE_4BIT .. SPI_DATASIZE_16BIT

        virtual void ActivateConfiguration() override;
        virtual void DeactivateConfiguration() override;

    private:
        SpiMasterStmDma& spi;
        uint32_t dataSize;
        uint32_t oldDataSize = 0;
    };
}

#endif
