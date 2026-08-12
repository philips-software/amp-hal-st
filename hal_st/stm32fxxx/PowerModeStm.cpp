#include "hal_st/stm32fxxx/PowerModeStm.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "stm32wbaxx_ll_pwr.h"
#include "stm32wbaxx_ll_system.h"

namespace hal
{
    PowerModeStm::PowerModeStm(uint32_t wakeUpPin, const Config& config)
        : wakeUpPin(wakeUpPin)
        , config(config)
    {
        __HAL_RCC_PWR_CLK_ENABLE();

        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOPF);
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOP2F);
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SBF);
    }

    bool PowerModeStm::EnterStandby()
    {
        // Radio not yet in deep sleep: let the caller retry so the link-layer background task can settle it.
        if (LL_PWR_GetRadioMode() != LL_PWR_RADIO_DEEP_SLEEP_MODE)
            return false;

        if (config.disableDebugInLowPowerMode)
            LL_DBGMCU_DisableDBGStandbyMode();

        if (config.enableUltraLowPowerMode)
            LL_PWR_EnableUltraLowPowerMode();

        HAL_PWR_DisableWakeUpPin(wakeUpPin);
        HAL_PWR_GetClearWakeupSource();
        HAL_PWR_EnableWakeUpPin(wakeUpPin);

        __HAL_PWR_CLEAR_FLAG(PWR_WAKEUP_ALL_FLAG);

        // Peripherals are already destroyed and Standby's WFI only sleeps while nothing is pending, so mask
        // interrupts and clear the tick and every pending NVIC interrupt to guarantee Standby is reached.
        __disable_irq();
        HAL_SuspendTick();

        SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;
        for (auto& pending : NVIC->ICPR)
            pending = 0xFFFFFFFF;

        for (int i = -1; i < 82; ++i)
            NVIC_ClearPendingIRQ(static_cast<IRQn_Type>(i));

        __NOP();

        HAL_PWR_EnterSTANDBYMode();

        // Standby did not take (an autonomous peripheral bus clock request forced Stop 0). With every
        // peripheral destroyed there is nothing to return to, so reboot into a clean state.
        // NVIC_SystemReset();
        return false;
    }
}
