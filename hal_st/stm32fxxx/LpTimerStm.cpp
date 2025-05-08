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
    {
        EnableClockLpTimer(timerIndex);

        handle.Instance = peripheralLpTimer[timerIndex];
        handle.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
        handle.Init.Clock.Prescaler = timing.prescaler;
        handle.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
        handle.Init.Period = timing.period;
        handle.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
        handle.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
        handle.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
        handle.Init.RepetitionCounter = timing.repetitionCounter;
        handle.Init.UpdateMode = timing.updateMode;

        auto result = HAL_LPTIM_Init(&handle);
        assert(result == HAL_OK);
    }

    LowPowerTimerBaseStm::~LowPowerTimerBaseStm()
    {
        peripheralLpTimer[timerIndex]->CR &= ~LPTIM_CR_ENABLE;
        DisableClockLpTimer(timerIndex);
    }

    FreeRunningLowPowerTimerStm::FreeRunningLowPowerTimerStm(uint8_t oneBasedIndex, Timing timing)
        : LowPowerTimerBaseStm{ oneBasedIndex, timing }
    {
    }

    void FreeRunningLowPowerTimerStm::Start()
    {
        auto result = HAL_LPTIM_Counter_Start(&handle);
        assert(result == HAL_OK);
    }

    void FreeRunningLowPowerTimerStm::Stop()
    {
        auto result = HAL_LPTIM_Counter_Stop(&handle);
        assert(result == HAL_OK);
    }

    LowPowerTimerWithInterruptStm::LowPowerTimerWithInterruptStm(uint8_t oneBasedIndex, Timing timing)
        : LowPowerTimerBaseStm(oneBasedIndex, timing)
        , interruptHandler{ peripheralLpTimerIrq[timerIndex], [this]
            {
                OnInterrupt();
            } }
    {
    }

    void LowPowerTimerWithInterruptStm::Start(const infra::Function<void()>& onIrq, InterruptType type)
    {
        this->onIrq = onIrq;
        this->type = type;
        auto result = HAL_LPTIM_Counter_Start_IT(&handle);
        assert(result == HAL_OK);
    }

    void LowPowerTimerWithInterruptStm::Stop()
    {
        auto result = HAL_LPTIM_Counter_Stop_IT(&handle);
        assert(result == HAL_OK);
    }

    void LowPowerTimerWithInterruptStm::OnInterrupt()
    {
        bool updateEvent = __HAL_LPTIM_GET_FLAG(&handle, LPTIM_FLAG_UPDATE);

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

    void LowPowerTimerWithInterruptStm::ScheduleInterrupt()
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
