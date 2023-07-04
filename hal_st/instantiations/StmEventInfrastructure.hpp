#ifndef HAL_ST_STM_EVENT_INFRASTRUCTURE_HPP
#define HAL_ST_STM_EVENT_INFRASTRUCTURE_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/LowPowerStrategyStm.hpp"
#include "hal_st/stm32fxxx/SystemTickTimerService.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"

namespace main_
{
    struct StmEventInfrastructure
    {
        explicit StmEventInfrastructure(infra::Duration tickDuration = std::chrono::milliseconds(1));

        void Run();

        hal::InterruptTable::WithStorage<128> interruptTable;
        infra::EventDispatcherWithWeakPtr::WithSize<50> eventDispatcher;
        hal::GpioStm gpio;

        hal::SystemTickTimerService systemTick;
    };

    struct LowPowerStmEventInfrastructure
    {
        explicit LowPowerStmEventInfrastructure(infra::Duration tickDuration = std::chrono::milliseconds(1));

        void Run();

        hal::InterruptTable::WithStorage<128> interruptTable;
        hal::LowPowerStrategyStm lowPowerStrategy;
        infra::LowPowerEventDispatcher::WithSize<50> eventDispatcher;
        hal::GpioStm gpio;

        hal::SystemTickTimerService systemTick;
    };
}

#endif
