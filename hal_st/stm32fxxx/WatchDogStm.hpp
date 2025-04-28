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
            uint32_t prescaler{ WWDG_PRESCALER_8 };
            infra::Duration feedTimerInterval{ std::chrono::milliseconds(25) };
        };

        WatchDogStm(const infra::Function<void()>& onExpired, const Config& config = Config());

        void WatchDogRefresh();
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
