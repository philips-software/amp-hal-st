#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "infra/util/EnumCast.hpp"

#if defined(HAS_PERIPHERAL_TIMER)

namespace hal
{
    TimerBaseStm::TimerBaseStm(uint8_t aTimerIndex, Timing timing, const Config& config)
        : timerIndex(aTimerIndex - 1)
        , config(config)
    {
        TIM_ClockConfigTypeDef clockSourceConfig{ 0, 0, 0, 0 };

        EnableClockTimer(timerIndex);

        handle.Instance = peripheralTimer[timerIndex];
        handle.Init.Prescaler = timing.prescaler;
        handle.Init.Period = timing.period;
        handle.Init.CounterMode = infra::enum_cast(config.counterMode);
        handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        HAL_TIM_Base_Init(&handle);

        clockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        HAL_TIM_ConfigClockSource(&handle, &clockSourceConfig);

        if (config.trigger)
            ConfigureTrigger();
    }

    TimerBaseStm::~TimerBaseStm()
    {
        peripheralTimer[timerIndex]->CR1 &= ~TIM_CR1_CEN;
        DisableClockTimer(timerIndex);
    }

    void TimerBaseStm::Start()
    {
        HAL_TIM_Base_Start(&handle);
    }

    void TimerBaseStm::Stop()
    {
        HAL_TIM_Base_Stop(&handle);
    }

    void TimerBaseStm::ConfigureTrigger()
    {
        TIM_MasterConfigTypeDef masterConfig = { 0 };

        masterConfig.MasterOutputTrigger = infra::enum_cast(config.trigger->triggerOutput);
        masterConfig.MasterSlaveMode = config.trigger->isSlaveMode ? TIM_MASTERSLAVEMODE_ENABLE : TIM_MASTERSLAVEMODE_DISABLE;
        HAL_TIMEx_MasterConfigSynchronization(&handle, &masterConfig);
    }
}

#endif
