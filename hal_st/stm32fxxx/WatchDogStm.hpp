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

            // WWDG clock (Hz) = PCLK1 / (4096 * Prescaler)                     --> 54Mhz / 32768 = 1728 Hz
            // WWDG timeout (mS) = 1000 * Counter / WWDG clock                  -->  73 ms
            // WWDG Counter refresh is allowed between the following limits :
            // min time (mS) = 1000 * (Counter _ Window) / WWDG clock           --> 0
            // max time (mS) = 1000 * (Counter _ 0x40) / WWDG clock             --> 36 ms
            // expire time (mS) =  max time (mS) * maxMissedFeeds               --> 41 * 36ms = 1.5s
            uint32_t prescaler{ WWDG_PRESCALER_8 };
            infra::Duration feedTimerInterval{ std::chrono::milliseconds(25) };
            uint32_t maxMissedFeeds{ 41 };
        };

        WatchDogStm(const infra::Function<void()>& onExpired, const Config& config = Config());

        void WatchDogRefresh();

    private:
        void Interrupt();
        void Feed();

        ImmediateInterruptHandler interruptRegistration;
        infra::TimerRepeating feedingTimer;
        WWDG_HandleTypeDef handle;
        std::atomic<uint32_t> missedFeedCount{ 0 };
        const uint32_t maxMissedFeeds{ 0 };
        infra::Function<void()> onExpired;
    };
}

#endif
