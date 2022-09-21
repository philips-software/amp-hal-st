#ifndef HAL_ST_SYSTEM_TICK_TIMER_SERVICE_HPP
#define HAL_ST_SYSTEM_TICK_TIMER_SERVICE_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/timer/TickOnInterruptTimerService.hpp"
#include "infra/util/InterfaceConnector.hpp"

extern "C" uint32_t HAL_GetTick();

namespace hal
{
    class SystemTickTimerService
        : public infra::InterfaceConnector<SystemTickTimerService>
        , public infra::TickOnInterruptTimerService
        , private InterruptHandler
    {
    public:
        SystemTickTimerService(infra::Duration tickDuration = std::chrono::milliseconds(1), uint32_t id = infra::systemTimerServiceId);

        virtual infra::TimePoint Now() const override;

    private:
        friend uint32_t ::HAL_GetTick();

        void Invoke() override;
    };
}

#endif
