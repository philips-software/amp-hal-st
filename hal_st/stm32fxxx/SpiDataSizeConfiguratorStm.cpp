#include "hal_st/stm32fxxx/SpiDataSizeConfiguratorStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"

namespace hal
{
    SpiDataSizeConfiguratorStm::SpiDataSizeConfiguratorStm(SpiMasterStmDma& spi, uint32_t dataSize)
        : spi(spi)
        , dataSize(dataSize)
    {}

    void SpiDataSizeConfiguratorStm::ActivateConfiguration()
    {
        oldDataSize = spi.DataSize();
        spi.SetDataSize(dataSize);
    }

    void SpiDataSizeConfiguratorStm::DeactivateConfiguration()
    {
        spi.SetDataSize(oldDataSize);
    }
}
