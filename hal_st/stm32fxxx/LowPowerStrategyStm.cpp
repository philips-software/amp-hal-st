#include "hal_st/stm32fxxx/LowPowerStrategyStm.hpp"
#include DEVICE_HEADER

namespace hal
{
    void LowPowerStrategyStm::RequestExecution()
    {
        __DSB();
        __SEV();
    }

    void LowPowerStrategyStm::Idle(const infra::EventDispatcherWorker& eventDispatcher)
    {
        __DSB();
        __WFE();
    }
}
