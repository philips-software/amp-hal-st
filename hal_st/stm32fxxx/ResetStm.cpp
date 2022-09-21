#include "hal_st/stm32fxxx/ResetStm.hpp"
#include "cmsis_device.h"

namespace hal
{
    void ResetStm::ResetModule(const char* resetReason)
    {
        NVIC_SystemReset();
    }
}
