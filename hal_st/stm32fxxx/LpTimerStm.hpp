#ifndef HAL_ST_LP_TIMER_STM_HPP
#define HAL_ST_LP_TIMER_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/Function.hpp"
#include <atomic>
#include DEVICE_HEADER

#if defined(HAS_PERIPHERAL_LPTIMER)

namespace hal
{
    class LowPowerTimerBaseStm
    {
    public:
        struct Timing
        {
            uint32_t period{};
            uint32_t repetitionCounter{ 0 };
            uint32_t prescaler{ LPTIM_PRESCALER_DIV1 };
            uint32_t updateMode{ repetitionCounter == 0 ? LPTIM_UPDATE_IMMEDIATE : LPTIM_UPDATE_ENDOFPERIOD };
        };

        LowPowerTimerBaseStm(uint8_t oneBasedIndex, Timing timing);
        virtual void Start(const infra::Function<void()>& onIrq, InterruptType type = InterruptType::immediate) = 0;
        virtual void Stop() = 0;
        uint16_t Counter() const;

    protected:
        ~LowPowerTimerBaseStm();

        infra::Function<void()> onIrq;
        InterruptType type;
        LPTIM_HandleTypeDef handle{};
        const uint8_t timerIndex;
#if defined(STM32WB)
        uint32_t currentPeriod{ 0 };
#endif
        void OnInterrupt();
        virtual bool HasTimerEvent() const = 0;
        void ScheduleInterrupt();

    private:
        ImmediateInterruptHandler interruptHandler;
        std::atomic_bool scheduled{};
    };

    class PeriodicLowPowerTimerStm
        : public LowPowerTimerBaseStm
    {
    public:
        using LowPowerTimerBaseStm::LowPowerTimerBaseStm;

        void Start(const infra::Function<void()>& onIrq, InterruptType type = InterruptType::immediate) override;
        void Stop() override;

    private:
        bool HasTimerEvent() const override;
    };

    class FreeRunningLowPowerTimerStm
        : public LowPowerTimerBaseStm
    {
    public:
        using LowPowerTimerBaseStm::LowPowerTimerBaseStm;

        virtual void Start(const infra::Function<void()>& onIrq, InterruptType type = InterruptType::immediate) override;
        virtual void Stop() override;

        void ArmRelativeCompare(uint16_t ticks);

    private:
        bool HasTimerEvent() const override;
    };

}

#endif

#endif
