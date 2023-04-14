#include "hal_st/stm32fxxx/ResetStm.hpp"
#include DEVICE_HEADER

namespace hal
{
    void ResetStm::ResetModule(const char* resetReason)
    {
        NVIC_SystemReset();
    }
}
