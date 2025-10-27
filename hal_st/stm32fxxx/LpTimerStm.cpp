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
        return static_cast<uint16_t>(handle.Instance->CNT);
    }

    FreeRunningLowPowerTimerStm::FreeRunningLowPowerTimerStm(uint8_t oneBasedIndex, Timing timing)
        : LowPowerTimerBaseStm{ oneBasedIndex, timing }
    {
    }

    void FreeRunningLowPowerTimerStm::Start()
    {
#if defined(STM32WB)
        auto result = HAL_LPTIM_Counter_Start(&handle, currentPeriod);
#else
        auto result = HAL_LPTIM_Counter_Start(&handle);
#endif
        assert(result == HAL_OK);
    }

    void FreeRunningLowPowerTimerStm::Stop()
    {
        auto result = HAL_LPTIM_Counter_Stop(&handle);
        assert(result == HAL_OK);
    }

    LowPowerTimerWithInterruptBaseStm::LowPowerTimerWithInterruptBaseStm(uint8_t aTimerIndex, Timing timing, uint32_t interruptFlagMask)
        : LowPowerTimerBaseStm(aTimerIndex, timing)
        , interruptFlagMask(interruptFlagMask)
        , interruptHandler{
            peripheralLpTimerIrq[timerIndex], [this]
            {
                OnInterrupt();
            }
        }
    {
    }

    void LowPowerTimerWithInterruptBaseStm::OnInterrupt()
    {
        bool flag = __HAL_LPTIM_GET_FLAG(&handle, interruptFlagMask);

        // let HAL handle clearing all flags
        HAL_LPTIM_IRQHandler(&handle);

        if (flag)
        {
            if (type == InterruptType::immediate)
            {
                onIrq();
            }
            else
                ScheduleInterrupt();
        }
    }

    void LowPowerTimerWithInterruptBaseStm::ScheduleInterrupt()
    {
        if (!scheduled.exchange(true))
            infra::EventDispatcher::Instance().Schedule([this]
                {
                    scheduled = false;
                    onIrq();
                });
    }

    LowPowerPeriodicTimerWithInterruptStm::LowPowerPeriodicTimerWithInterruptStm(uint8_t aTimerIndex, Timing timing)
#if defined(STM32WB)
        : LowPowerTimerWithInterruptBaseStm{ aTimerIndex, timing, LPTIM_FLAG_ARRM }
#else
        : LowPowerTimerWithInterruptBaseStm{ aTimerIndex, timing, LPTIM_FLAG_UPDATE }
#endif
    {
    }

    void LowPowerPeriodicTimerWithInterruptStm::Start(const infra::Function<void()>& onIrq, InterruptType type)
    {
        this->onIrq = onIrq;
        this->type = type;

#if defined(STM32WB)
        auto result = HAL_LPTIM_Counter_Start_IT(&handle, currentPeriod);
#else
        auto result = HAL_LPTIM_Counter_Start_IT(&handle);
#endif
        assert(result == HAL_OK);
    }

    void LowPowerPeriodicTimerWithInterruptStm::Stop()
    {
        auto result = HAL_LPTIM_Counter_Stop_IT(&handle);
        assert(result == HAL_OK);
    }

#if defined(STM32WB)
    FreeRunningLowPowerTimerWithInterruptStm::FreeRunningLowPowerTimerWithInterruptStm(uint8_t aTimerIndex, Timing timing)
        : LowPowerTimerWithInterruptBaseStm{ aTimerIndex, timing, LPTIM_FLAG_CMPM }
    {
    }

    void FreeRunningLowPowerTimerWithInterruptStm::Start(const infra::Function<void()>& onIrq, InterruptType type)
    {
        this->onIrq = onIrq;
        this->type = type;

#if defined(STM32WB)
        auto result = HAL_LPTIM_Counter_Start(&handle, currentPeriod);
#else
        auto result = HAL_LPTIM_Counter_Start(&handle);
#endif
        assert(result == HAL_OK);
    }

    void FreeRunningLowPowerTimerWithInterruptStm::Stop()
    {
        __HAL_LPTIM_DISABLE_IT(&handle, LPTIM_FLAG_CMPM);

        auto result = HAL_LPTIM_Counter_Stop(&handle);
        assert(result == HAL_OK);
    }

    void FreeRunningLowPowerTimerWithInterruptStm::ArmRelativeCompare(uint16_t ticks)
    {
        __HAL_LPTIM_DISABLE_IT(&handle, LPTIM_IT_CMPM);
        __HAL_LPTIM_CLEAR_FLAG(&handle, LPTIM_FLAG_CMPM | LPTIM_FLAG_CMPOK);

        uint16_t compareValue = Counter() + ticks;
        __HAL_LPTIM_COMPARE_SET(&handle, compareValue);

        while (!__HAL_LPTIM_GET_FLAG(&handle, LPTIM_FLAG_CMPOK))
            ;

        __HAL_LPTIM_CLEAR_FLAG(&handle, LPTIM_FLAG_CMPOK);
        __HAL_LPTIM_ENABLE_IT(&handle, LPTIM_IT_CMPM);
    }
#endif
}

#endif
