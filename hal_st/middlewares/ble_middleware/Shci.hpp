#ifndef HAL_ST_SHCI_HPP
#define HAL_ST_SHCI_HPP

#include "hal_st/middlewares/ble_middleware/GapSt.hpp"

namespace hal
{
    void ShciInitialization(GapSt::BleBondStorage bleBondStorage, const GapSt::Configuration& configuration);
}

#endif
