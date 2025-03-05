#ifndef HAL_ST_STM_EVENT_INFRASTRUCTURE_HPP
#define HAL_ST_STM_EVENT_INFRASTRUCTURE_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/LowPowerStrategyStm.hpp"
#include "hal_st/stm32fxxx/SystemTickTimerService.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include <cstddef>

namespace main_
{
    constexpr std::size_t DefaultInterruptTableSize()
    {
#if defined(STM32H5)
        return 149;
#else
        return 128;
#endif
    }

    struct StmEventInfrastructure
    {
        explicit StmEventInfrastructure(infra::Duration tickDuration = std::chrono::milliseconds(1));

        void Run();

        hal::InterruptTable::WithStorage<DefaultInterruptTableSize()> interruptTable;
        infra::EventDispatcherWithWeakPtr::WithSize<50> eventDispatcher;
        hal::GpioStm gpio;

        hal::SystemTickTimerService systemTick;
    };

    struct LowPowerStmEventInfrastructure
    {
        explicit LowPowerStmEventInfrastructure(infra::Duration tickDuration = std::chrono::milliseconds(1));

        void Run();

        hal::InterruptTable::WithStorage<DefaultInterruptTableSize()> interruptTable;
        hal::LowPowerStrategyStm lowPowerStrategy;
        infra::LowPowerEventDispatcher::WithSize<50> eventDispatcher;
        hal::GpioStm gpio;

        hal::SystemTickTimerService systemTick;
    };
}

#endif
