#include "hal_st/stm32fxxx/LowPowerStrategyStm.hpp"
#include DEVICE_HEADER

namespace hal
{
    void LowPowerStrategyStm::RequestExecution()
    {
    }

    void LowPowerStrategyStm::Idle(const infra::EventDispatcherWorker& eventDispatcher)
    {
        __SEV();
        __WFE();
        __WFE();
    }
}
