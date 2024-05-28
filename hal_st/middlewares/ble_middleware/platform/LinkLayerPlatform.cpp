#include <algorithm>
#include "infra/util/ReallyAssert.hpp"
#include "stm32wbaxx_ll_rcc.h"
extern "C"
{
#include "linklayer_plat.h"
#include "ll_sys.h"
}

namespace
{
    constexpr uint8_t highIsrPriorityWhenRadioIsActive = 0;
    constexpr uint8_t highIsrPriorityWhenRadioIsInactive = 5;
    constexpr uint8_t lowIsrPriority = 15;

    /* Radio critical sections */
    uint32_t primask = 0;
    constexpr int32_t lowestThresholdCounter = 0;
    volatile int32_t priorityHighIsrCounter = 0;
    volatile int32_t priorityLowIsrCounter = 0;
    volatile int32_t prioritySystemIsrCounter = 0;
    volatile int32_t irqCounter = 0;
    volatile uint32_t localBasePriValue = 0;

    /* Radio SW low ISR global variable */
    volatile bool isRadioRunningInHighPriority = 0;

    /* 2.4GHz RADIO ISR callbacks */
    void (*radioCallback)() = nullptr;
    void (*lowIsrCallback)() = nullptr;
}

extern "C"
{
    void ll_sys_bg_process_init()
    {}

    void ll_sys_schedule_bg_process()
    {}

    void ll_sys_schedule_bg_process_isr()
    {}

    void ll_sys_config_params()
    {}

    void ll_sys_bg_temperature_measurement_init()
    {}

    void ll_sys_bg_temperature_measurement()
    {}

    void LINKLAYER_PLAT_ClockInit()
    {
        really_assert(LL_RCC_RADIO_GetSleepTimerClockSource() != LL_RCC_RADIOSLEEPSOURCE_NONE);

        __HAL_RCC_RADIO_CLK_ENABLE();
    }

    void LINKLAYER_PLAT_DelayUs(uint32_t delay)
    {
        __IO uint32_t delayUs = delay * (SystemCoreClock / 1000000U);
        do
        {
            __NOP();
        }
        while (delayUs --);
    }

    void LINKLAYER_PLAT_Assert(uint8_t condition)
    {
        really_assert(condition);
    }

    void LINKLAYER_PLAT_WaitHclkRdy()
    {
        while (HAL_RCCEx_GetRadioBusClockReadiness() != RCC_RADIO_BUS_CLOCK_READY);
    }

    void LINKLAYER_PLAT_AclkCtrl(uint8_t enable)
    {
        if (enable)
        {
            HAL_RCCEx_EnableRadioBBClock();

            while (LL_RCC_HSE_IsReady() == 0)
            {}
        }
        else
            HAL_RCCEx_DisableRadioBBClock();
    }

    void LINKLAYER_PLAT_GetRNG(uint8_t *ptr_rnd, uint32_t len)
    {}

    void LINKLAYER_PLAT_SetupRadioIT(void (*intr_cb)())
    {
        radioCallback = intr_cb;

        HAL_NVIC_SetPriority(RADIO_IRQn, highIsrPriorityWhenRadioIsActive, 0);
        HAL_NVIC_EnableIRQ(RADIO_IRQn);
    }

    void LINKLAYER_PLAT_SetupSwLowIT(void (*intr_cb)())
    {
        lowIsrCallback = intr_cb;

        HAL_NVIC_SetPriority(HASH_IRQn, lowIsrPriority, 0);
        HAL_NVIC_EnableIRQ(HASH_IRQn);
    }

    void LINKLAYER_PLAT_TriggerSwLowIT(uint8_t priority)
    {
        if (NVIC_GetActive(HASH_IRQn) == 0)
            HAL_NVIC_SetPriority(HASH_IRQn, priority == 0 ? lowIsrPriority : highIsrPriorityWhenRadioIsInactive, 0);
        else
        {
            if(priority != 0)
                isRadioRunningInHighPriority = true;
        }

        HAL_NVIC_SetPendingIRQ(HASH_IRQn);
    }

    void LINKLAYER_PLAT_EnableIRQ()
    {
        irqCounter = std::max(lowestThresholdCounter, irqCounter - 1);

        if(irqCounter == 0)
            __set_PRIMASK(primask);
    }

    void LINKLAYER_PLAT_DisableIRQ()
    {
        if (irqCounter == 0)
            primask= __get_PRIMASK();

        __disable_irq();
        irqCounter++;
    }

    void LINKLAYER_PLAT_EnableSpecificIRQ(uint8_t isr_type)
    {
        if (isr_type & LL_HIGH_ISR_ONLY)
        {
            priorityHighIsrCounter--;
            if(priorityHighIsrCounter == 0)
                HAL_NVIC_EnableIRQ(RADIO_IRQn);
        }

        if (isr_type & LL_LOW_ISR_ONLY)
        {
            priorityLowIsrCounter--;
            if(priorityLowIsrCounter == 0)
                HAL_NVIC_EnableIRQ(HASH_IRQn);
        }

        if (isr_type & SYS_LOW_ISR)
        {
            prioritySystemIsrCounter--;
            if(prioritySystemIsrCounter == 0)
                __set_BASEPRI(localBasePriValue);
        }
    }

    void LINKLAYER_PLAT_DisableSpecificIRQ(uint8_t isr_type)
    {
        if (isr_type & LL_HIGH_ISR_ONLY)
        {
            priorityHighIsrCounter++;
            if(priorityHighIsrCounter == 1)
                HAL_NVIC_DisableIRQ(RADIO_IRQn);
        }

        if (isr_type & LL_LOW_ISR_ONLY)
        {
            priorityLowIsrCounter++;
            if(priorityLowIsrCounter == 1)
                HAL_NVIC_DisableIRQ(HASH_IRQn);
        }

        if (isr_type & SYS_LOW_ISR)
        {
            prioritySystemIsrCounter++;
            if (prioritySystemIsrCounter == 1)
            {
                localBasePriValue = __get_BASEPRI();
                __set_BASEPRI_MAX(5<<4);
            }
        }
    }

    void LINKLAYER_PLAT_EnableRadioIT()
    {
        HAL_NVIC_EnableIRQ(RADIO_IRQn);
    }

    void LINKLAYER_PLAT_DisableRadioIT()
    {
        HAL_NVIC_DisableIRQ(RADIO_IRQn);
    }

    void LINKLAYER_PLAT_StartRadioEvt()
    {
        __HAL_RCC_RADIO_CLK_SLEEP_ENABLE();
        NVIC_SetPriority(RADIO_IRQn, highIsrPriorityWhenRadioIsActive);
    }

    void LINKLAYER_PLAT_StopRadioEvt()
    {
        __HAL_RCC_RADIO_CLK_SLEEP_DISABLE();
        NVIC_SetPriority(RADIO_IRQn, highIsrPriorityWhenRadioIsInactive);
    }

    void LINKLAYER_PLAT_RCOStartClbr()
    {}

    void LINKLAYER_PLAT_RCOStopClbr()
    {}

    void LINKLAYER_PLAT_RequestTemperature()
    {}

    void LINKLAYER_PLAT_EnableOSContextSwitch()
    {}

    void LINKLAYER_PLAT_DisableOSContextSwitch()
    {}

    void LINKLAYER_PLAT_SCHLDR_TIMING_UPDATE_NOT(Evnt_timing_t * p_evnt_timing)
    {}

    void RADIO_IRQHandler()
    {
        uint32_t mul = 0;
        uint32_t div = 0;

        ll_intf_get_aligned_us_now(&mul, &div);

        if(radioCallback)
            radioCallback();

        LL_RCC_RADIO_DisableSleepTimerClock();
        __ISB();
    }

    void HASH_IRQHandler()
    {
        NVIC_DisableIRQ(HASH_IRQn);

        if(lowIsrCallback)
            lowIsrCallback();

        if(isRadioRunningInHighPriority != 0)
        {
            HAL_NVIC_SetPriority(HASH_IRQn, highIsrPriorityWhenRadioIsInactive, 0);
            isRadioRunningInHighPriority = 0;
        }

        NVIC_EnableIRQ(HASH_IRQn);
    }

    void LINKLAYER_DEBUG_SIGNAL_SET(Debug_GPIO_t)
    {}

    void LINKLAYER_DEBUG_SIGNAL_RESET(Debug_GPIO_t)
    {}

    void LINKLAYER_DEBUG_SIGNAL_TOGGLE(Debug_GPIO_t)
    {}
}
