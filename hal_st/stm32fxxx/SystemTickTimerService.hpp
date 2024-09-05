#ifndef HAL_ST_SYSTEM_TICK_TIMER_SERVICE_HPP
#define HAL_ST_SYSTEM_TICK_TIMER_SERVICE_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/timer/TickOnInterruptTimerService.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include <chrono>
#include <cstdint>

namespace hal
{
    class SystemTickTimerService
        : public infra::InterfaceConnector<SystemTickTimerService>
        , public infra::TickOnInterruptTimerService
        , private InterruptHandler
    {
    public:
        SystemTickTimerService(infra::Duration tickDuration = std::chrono::milliseconds(1), uint32_t id = infra::systemTimerServiceId);

        infra::TimePoint Now() const override;

    protected:
        void Invoke() override;
    };
}

#endif
