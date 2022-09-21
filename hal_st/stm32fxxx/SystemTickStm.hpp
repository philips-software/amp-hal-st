#ifndef HAL_ST_SYSTEM_TICK_STM_HPP
#define HAL_ST_SYSTEM_TICK_STM_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/timer/Timer.hpp"

namespace hal
{
    class SystemTickStm
        : private InterruptHandler
    {
    public:
        SystemTickStm(const infra::Function<void()>& callback, infra::Duration tickDuration = std::chrono::milliseconds(1));

    private:
        void Invoke() override;

    private:
        infra::Function<void()> callback;
    };
}

#endif
