#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "generated/stm32fxxx/PinoutTableDefault.hpp"

namespace main_
{
    StmEventInfrastructure::StmEventInfrastructure(infra::Duration tickDuration)
        : gpio(hal::pinoutTableDefaultStm, hal::analogTableDefaultStm)
        , systemTick(tickDuration)
    {}

    void StmEventInfrastructure::Run()
    {
        eventDispatcher.Run();
    }

    LowPowerStmEventInfrastructure::LowPowerStmEventInfrastructure(infra::Duration tickDuration)
        : eventDispatcher(lowPowerStrategy)
        , gpio(hal::pinoutTableDefaultStm, hal::analogTableDefaultStm)
        , systemTick(tickDuration)
    {}

    void LowPowerStmEventInfrastructure::Run()
    {
        eventDispatcher.Run();
    }
}
