#ifndef HAL_WATCHDOG_STM_HPP
#define HAL_WATCHDOG_STM_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/timer/Timer.hpp"
#include "cmsis_device.h"
#include <atomic>

namespace hal
{
    class WatchDogStm
    {
    public:
        WatchDogStm(const infra::Function<void()>& onExpired);

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
