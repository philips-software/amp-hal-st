#ifndef HAL_WATCHDOG_STM_HPP
#define HAL_WATCHDOG_STM_HPP

#include DEVICE_HEADER
#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/timer/Timer.hpp"
#include <atomic>

namespace hal
{
    class WatchDogStm
    {
    public:

        struct Config
        {
            constexpr Config()
            {}
            uint32_t prescaler = WWDG_PRESCALER_8;
            uint32_t feedTimerIntervalMs = 25;
        };

        WatchDogStm(const infra::Function<void()>& onExpired, const Config& config = Config());

        void Interrupt();

    private:
        void Feed();

        ImmediateInterruptHandler interruptRegistration;
        infra::TimerRepeating feedingTimer;
        WWDG_HandleTypeDef handle;
        std::atomic<uint32_t> delay{ 0 };
        infra::Function<void()> onExpired;
    };
}

#endif
