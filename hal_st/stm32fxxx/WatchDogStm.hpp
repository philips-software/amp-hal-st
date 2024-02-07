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
        enum Prescalers
        {
            Prescaler1,
            Prescaler2,
            Prescaler4,
            Prescaler8,
            Prescaler16,
            Prescaler32,
            Prescaler64,
            Prescaler128,
        };

        WatchDogStm(const infra::Function<void()>& onExpired, Prescalers prescaler = Prescalers::Prescaler8);

        void Interrupt();

    private:
        void Feed();
        uint32_t GetPrescaler(Prescalers prescaler) const;

        ImmediateInterruptHandler interruptRegistration;
        infra::TimerRepeating feedingTimer;
        WWDG_HandleTypeDef handle;
        std::atomic<uint32_t> delay{ 0 };
        infra::Function<void()> onExpired;
    };
}

#endif
