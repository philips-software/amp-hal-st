#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/SpiDataSizeConfiguratorStm.hpp"

namespace hal
{
    SpiDataSizeConfiguratorStm::SpiDataSizeConfiguratorStm(SpiMasterStmDma& spi, uint32_t dataSize)
        : spi(spi)
        , dataSize(dataSize)
    {}

    void SpiDataSizeConfiguratorStm::ActivateConfiguration()
    {
        oldDataSize = spi.DataSize();
        spi.SetDataSize(16);
    }

    void SpiDataSizeConfiguratorStm::DeactivateConfiguration()
    {
        spi.SetDataSize(oldDataSize);
    }
}
