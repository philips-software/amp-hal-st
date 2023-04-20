#include "hal_st/stm32fxxx/WatchDogStm.hpp"

namespace hal
{
    WatchDogStm::WatchDogStm(const infra::Function<void()>& onExpired)
        : onExpired(onExpired)
        , interruptRegistration(WWDG_IRQn, [this]()
              { Interrupt(); })
    {
        __WWDG_CLK_ENABLE();

        // WWDG clock (Hz) = PCLK1 / (4096 * Prescaler)                     --> 54Mhz / 32768 = 1728 Hz
        // WWDG timeout (mS) = 1000 * Counter / WWDG clock                  -->  73 ms
        // WWDG Counter refresh is allowed between the following limits :
        // min time (mS) = 1000 * (Counter _ Window) / WWDG clock           --> 0
        // max time (mS) = 1000 * (Counter _ 0x40) / WWDG clock             --> 36 ms
        handle.Instance = WWDG;
        handle.Init.Prescaler = WWDG_PRESCALER_8;
        handle.Init.Window = WWDG_CR_T;
        handle.Init.Counter = WWDG_CR_T;
#ifdef STM32F7
        handle.Init.EWIMode = WWDG_EWI_ENABLE;
#endif
        HAL_WWDG_Init(&handle);

        SCB->AIRCR = (0x5FAUL << SCB_AIRCR_VECTKEY_Pos)
#ifndef STM32G0
            | (0 << SCB_AIRCR_PRIGROUP_Pos)
#endif
            ;
        NVIC_SetPriority(WWDG_IRQn, 0);
        WWDG->CFR |= WWDG_CFR_EWI;

        feedingTimer.Start(std::chrono::milliseconds(25), [this]()
            { Feed(); });
    }

    void WatchDogStm::Interrupt()
    {
        HAL_WWDG_Refresh(&handle);
        WWDG->SR = 0;
        if (++delay == 41) // 41 * 36ms = 1.5s
            onExpired();
    }

    void WatchDogStm::Feed()
    {
        delay = 0;
    }
}
