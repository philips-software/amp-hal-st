#include "hal_st/stm32fxxx/LpTimerStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/event/EventDispatcher.hpp"
#include <chrono>
#include DEVICE_HEADER

#if defined(HAS_PERIPHERAL_LPTIMER)

namespace hal
{
    LowPowerTimerBaseStm::LowPowerTimerBaseStm(uint8_t oneBasedIndex, Timing timing)
        : timerIndex(oneBasedIndex - 1)
        , interruptHandler{ peripheralLpTimerIrq[timerIndex], [this]
            {
                OnInterrupt();
            } }
    {
        EnableClockLpTimer(timerIndex);

        handle.Instance = peripheralLpTimer[timerIndex];
        handle.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
        handle.Init.Clock.Prescaler = timing.prescaler;
        handle.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
#if defined(STM32WB)
        handle.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
        handle.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
        handle.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
        handle.Init.UpdateMode = timing.updateMode;
        handle.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;

        currentPeriod = timing.period;
#else
        handle.Init.Period = timing.period;
        handle.Init.RepetitionCounter = timing.repetitionCounter;
        handle.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
        handle.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
        handle.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
        handle.Init.UpdateMode = timing.updateMode;
#endif

        auto result = HAL_LPTIM_Init(&handle);
        assert(result == HAL_OK);
    }

    LowPowerTimerBaseStm::~LowPowerTimerBaseStm()
    {
        peripheralLpTimer[timerIndex]->CR &= ~LPTIM_CR_ENABLE;
        DisableClockLpTimer(timerIndex);
    }

    uint16_t LowPowerTimerBaseStm::Counter() const
    {
        return static_cast<uint16_t>(HAL_LPTIM_ReadCounter(&handle));
    }

    void LowPowerTimerBaseStm::OnInterrupt()
    {
        bool updateEvent = HasTimerEvent();

        // let HAL handle clearing all flags
        HAL_LPTIM_IRQHandler(&handle);

        if (updateEvent)
        {
            if (type == InterruptType::immediate)
            {
                onIrq();
            }
            else
                ScheduleInterrupt();
        }
    }

    void LowPowerTimerBaseStm::ScheduleInterrupt()
    {
        if (!scheduled.exchange(true))
            infra::EventDispatcher::Instance().Schedule([this]
                {
                    scheduled = false;
                    onIrq();
                });
    }

    void PeriodicLowPowerTimerStm::Start(const infra::Function<void()>& onIrq, InterruptType type)
    {
        this->onIrq = onIrq;
        this->type = type;

#if defined(STM32WB)
        __HAL_LPTIM_CLEAR_FLAG(&handle, LPTIM_FLAG_ARRM | LPTIM_FLAG_CMPM | LPTIM_FLAG_CMPOK | LPTIM_FLAG_ARROK);
        __HAL_LPTIM_ENABLE_IT(&handle, LPTIM_IT_ARRM);
        auto result = HAL_LPTIM_Counter_Start_IT(&handle, currentPeriod);
#else
        __HAL_LPTIM_CLEAR_FLAG(&handle, LPTIM_FLAG_UPDATE | LPTIM_FLAG_CMPM | LPTIM_FLAG_CMPOK | LPTIM_FLAG_ARROK);
        __HAL_LPTIM_ENABLE_IT(&handle, LPTIM_IT_UPDATE);
        auto result = HAL_LPTIM_Counter_Start_IT(&handle);
#endif
        assert(result == HAL_OK);
    }

    void PeriodicLowPowerTimerStm::Stop()
    {
#if defined(STM32WB)
        __HAL_LPTIM_DISABLE_IT(&handle, LPTIM_IT_ARRM);
#else
        __HAL_LPTIM_DISABLE_IT(&handle, LPTIM_IT_UPDATE);
#endif
        auto result = HAL_LPTIM_Counter_Stop_IT(&handle);
        assert(result == HAL_OK);
    }

    bool PeriodicLowPowerTimerStm::HasTimerEvent() const
    {
#if defined(STM32WB)
        return __HAL_LPTIM_GET_FLAG(&handle, LPTIM_FLAG_ARRM);
#else
        return __HAL_LPTIM_GET_FLAG(&handle, LPTIM_FLAG_UPDATE);
#endif
    }

    void FreeRunningLowPowerTimerStm::Start(const infra::Function<void()>& onIrq, InterruptType type)
    {
        this->onIrq = onIrq;
        this->type = type;

        __HAL_LPTIM_CLEAR_FLAG(&handle, LPTIM_FLAG_CMPM | LPTIM_FLAG_ARRM | LPTIM_FLAG_CMPOK | LPTIM_FLAG_ARROK);
        __HAL_LPTIM_ENABLE_IT(&handle, LPTIM_IT_CMPM);

#if defined(STM32WB)
        auto result = HAL_LPTIM_Counter_Start_IT(&handle, currentPeriod);
#else
        auto result = HAL_LPTIM_Counter_Start_IT(&handle);
#endif
        assert(result == HAL_OK);
    }

    void FreeRunningLowPowerTimerStm::Stop()
    {
        __HAL_LPTIM_DISABLE_IT(&handle, LPTIM_IT_CMPM);
        auto result = HAL_LPTIM_Counter_Stop_IT(&handle);
        assert(result == HAL_OK);
    }

    void FreeRunningLowPowerTimerStm::SetPeriod(uint16_t periodTicks)
    {
        assert(periodTicks != 0);

        uint16_t currentCounter = static_cast<uint16_t>(HAL_LPTIM_ReadCounter(&handle));
        uint16_t compareValue = currentCounter + periodTicks;

        __HAL_LPTIM_COMPARE_SET(&handle, compareValue);
    }

    bool FreeRunningLowPowerTimerStm::HasTimerEvent() const
    {
        return __HAL_LPTIM_GET_FLAG(&handle, LPTIM_FLAG_CMPM);
    }
}

#endif
