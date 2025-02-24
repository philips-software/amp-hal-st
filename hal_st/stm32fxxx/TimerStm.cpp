#include "hal_st/stm32fxxx/TimerStm.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/EnumCast.hpp"
#include "infra/util/Function.hpp"
#include DEVICE_HEADER

#if defined(HAS_PERIPHERAL_TIMER)

namespace
{
    constexpr auto TimerIrqUnsupported{ static_cast<IRQn_Type>(255) };

    constexpr std::array irqMap{
#if defined(TIM1)
#if defined(STM32WB)
        IRQn_Type::TIM1_UP_TIM16_IRQn,
#elif defined(STM32F7) || defined(STM32F4)
        IRQn_Type::TIM1_UP_TIM10_IRQn,
#else
        IRQn_Type::TIM1_UP_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM2)
        IRQn_Type::TIM2_IRQn,
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM3)
        IRQn_Type::TIM3_IRQn,
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM4)
        IRQn_Type::TIM4_IRQn,
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM5)
        IRQn_Type::TIM5_IRQn,
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM6)
#if defined(STM32F7) || defined(STM32F4)
        IRQn_Type::TIM6_DAC_IRQn,
#else
        IRQn_Type::TIM6_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM7)
        IRQn_Type::TIM7_IRQn,
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM8)
#if defined(STM32F7) || defined(STM32F4)
        IRQn_Type::TIM8_UP_TIM13_IRQn,
#else
        IRQn_Type::TIM8_UP_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM9)
#if defined(STM32F7) || defined(STM32F4)
        IRQn_Type::TIM1_BRK_TIM9_IRQn,
#else
        IRQn_Type::TIM9_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM10)
#if defined(STM32F7) || defined(STM32F4)
        IRQn_Type::TIM1_UP_TIM10_IRQn,
#else
        IRQn_Type::TIM10_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM11)
#if defined(STM32F7) || defined(STM32F4)
        IRQn_Type::TIM1_TRG_COM_TIM11_IRQn,
#else
        IRQn_Type::TIM11_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM12)
#if defined(STM32F7) || defined(STM32F4)
        IRQn_Type::TIM8_BRK_TIM12_IRQn,
#else
        IRQn_Type::TIM12_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM13)
#if defined(STM32F7) || defined(STM32F4)
        IRQn_Type::TIM8_UP_TIM13_IRQn,
#else
        IRQn_Type::TIM13_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM14)
#if defined(STM32F7) || defined(STM32F4)
        IRQn_Type::TIM8_TRG_COM_TIM14_IRQn,
#else
        IRQn_Type::TIM14_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM15)
        IRQn_Type::TIM15_IRQn,
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM16)
#if defined(STM32WB)
        IRQn_Type::TIM1_UP_TIM16_IRQn,
#else
        IRQn_Type::TIM16_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
#if defined(TIM17)
#if defined(STM32WB)
        IRQn_Type::TIM1_TRG_COM_TIM17_IRQn,
#else
        IRQn_Type::TIM17_IRQn,
#endif
#else
        TimerIrqUnsupported,
#endif
    };

    IRQn_Type LookupIrq(std::size_t index)
    {
        assert(index < irqMap.size());
        assert(irqMap[index] != TimerIrqUnsupported);
        return irqMap[index];
    }
}

namespace hal
{
    TimerBaseStm::TimerBaseStm(uint8_t oneBasedIndex, Timing timing, const Config& config)
        : timerIndex(oneBasedIndex - 1)
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

    void TimerBaseStm::ConfigureTrigger()
    {
        TIM_MasterConfigTypeDef masterConfig = { 0 };

        masterConfig.MasterOutputTrigger = infra::enum_cast(config.trigger->triggerOutput);
        masterConfig.MasterSlaveMode = config.trigger->isSlaveMode ? TIM_MASTERSLAVEMODE_ENABLE : TIM_MASTERSLAVEMODE_DISABLE;
        auto result = HAL_TIMEx_MasterConfigSynchronization(&handle, &masterConfig);
        assert(result == HAL_OK);
    }

    FreeRunningTimerStm::FreeRunningTimerStm(uint8_t oneBasedIndex, Timing timing, const Config& config)
        : TimerBaseStm(oneBasedIndex, timing, config)
    {
    }

    void FreeRunningTimerStm::Start()
    {
        auto result = HAL_TIM_Base_Start(&handle);
        assert(result == HAL_OK);
    }

    void FreeRunningTimerStm::Stop()
    {
        auto result = HAL_TIM_Base_Stop(&handle);
        assert(result == HAL_OK);
    }

    TimerWithInterruptStm::TimerWithInterruptStm(uint8_t oneBasedIndex, Timing timing)
        : TimerBaseStm(oneBasedIndex, timing, { CounterMode::up })
        , interruptHandler{ LookupIrq(oneBasedIndex - 1), [this]
            {
                OnInterrupt();
            } }
    {
    }

    void TimerWithInterruptStm::Start(const infra::Function<void()>& onIrq, InterruptType type)
    {
        this->onIrq = onIrq;
        this->type = type;
        auto result = HAL_TIM_Base_Start_IT(&handle);
        assert(result == HAL_OK);
    }

    void TimerWithInterruptStm::Stop()
    {
        auto result = HAL_TIM_Base_Stop_IT(&handle);
        assert(result == HAL_OK);
    }

    void TimerWithInterruptStm::OnInterrupt()
    {
        if (__HAL_TIM_GET_FLAG(&handle, TIM_FLAG_UPDATE))
        {
            __HAL_TIM_CLEAR_FLAG(&handle, TIM_FLAG_UPDATE);
            if (type == InterruptType::immediate)
            {
                onIrq();
            }
            else
                ScheduleInterrupt();
        }
    }

    void TimerWithInterruptStm::ScheduleInterrupt()
    {
        if (!scheduled.exchange(true))
            infra::EventDispatcher::Instance().Schedule([this]
                {
                    scheduled = false;
                    onIrq();
                });
    }
}

#endif
