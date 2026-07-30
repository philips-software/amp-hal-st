#include "hal_st/stm32fxxx/PowerModeStm.hpp"
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
        if (LL_PWR_GetRadioMode() != LL_PWR_RADIO_DEEP_SLEEP_MODE)
            return false;

        if (config.disableDebugInLowPowerMode)
            LL_DBGMCU_DisableDBGStandbyMode();

        __HAL_PWR_CLEAR_FLAG(PWR_WAKEUP_ALL_FLAG);

        HAL_PWR_DisableWakeUpPin(wakeUpPin);
        HAL_PWR_GetClearWakeupSource();
        HAL_PWR_EnableWakeUpPin(wakeUpPin);

        __HAL_PWR_CLEAR_FLAG(PWR_WAKEUP_ALL_FLAG);

        HAL_SuspendTick();

        if (config.enableUltraLowPowerMode)
            LL_PWR_EnableUltraLowPowerMode();

        HAL_PWR_EnterSTANDBYMode();

        return true;
    }
}
