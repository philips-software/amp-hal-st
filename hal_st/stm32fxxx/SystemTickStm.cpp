#include "hal_st/stm32fxxx/SystemTickStm.hpp"
#include DEVICE_HEADER
#include "hal/interfaces/Gpio.hpp"

namespace hal
{
    SystemTickStm::SystemTickStm(const infra::Function<void()>& callback, infra::Duration tickDuration)
        : callback(callback)
    {
        Register(SysTick_IRQn);
        SysTick->LOAD = SystemCoreClock / (1000000000 / std::chrono::duration_cast<std::chrono::nanoseconds>(tickDuration).count()) - 1ul;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
        SysTick->VAL = 0;
    }

    void SystemTickStm::Invoke()
    {
        callback();
    }
}
