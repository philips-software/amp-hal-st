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

        uint16_t Counter() const;

        auto& Handle()
        {
            return handle;
        }

    protected:
        LowPowerTimerBaseStm(uint8_t oneBasedIndex, Timing timing);
        ~LowPowerTimerBaseStm();

        LPTIM_HandleTypeDef handle{};
        const uint8_t timerIndex;
#if defined(STM32WB)
        uint32_t currentPeriod{ 0 };
#endif
    };

    class FreeRunningLowPowerTimerStm
        : public LowPowerTimerBaseStm
    {
    public:
        FreeRunningLowPowerTimerStm(uint8_t aTimerIndex, Timing timing);

        void Start();
        void Stop();
    };

    class LowPowerTimerWithInterruptBaseStm
        : public LowPowerTimerBaseStm
    {
    public:
        virtual void Start(const infra::Function<void()>& onIrq, InterruptType type = InterruptType::immediate) = 0;
        virtual void Stop() = 0;

    protected:
        LowPowerTimerWithInterruptBaseStm(uint8_t aTimerIndex, Timing timing, uint32_t interruptFlagMask);

        ImmediateInterruptHandler interruptHandler;
        infra::Function<void()> onIrq;
        InterruptType type = InterruptType::immediate;
        uint32_t interruptFlagMask;
        std::atomic_bool scheduled{};

        void OnInterrupt();
        void ScheduleInterrupt();
    };

    class LowPowerPeriodicTimerWithInterruptStm
        : public LowPowerTimerWithInterruptBaseStm
    {
    public:
        LowPowerPeriodicTimerWithInterruptStm(uint8_t aTimerIndex, Timing timing);

        void Start(const infra::Function<void()>& onIrq, InterruptType type = InterruptType::immediate) override;
        void Stop() override;
    };

#if defined(STM32WB)
    class FreeRunningLowPowerTimerWithInterruptStm
        : public LowPowerTimerWithInterruptBaseStm
    {
    public:
        FreeRunningLowPowerTimerWithInterruptStm(uint8_t aTimerIndex, Timing timing);

        void Start(const infra::Function<void()>& onIrq, InterruptType type = InterruptType::immediate) override;
        void Stop() override;

        void ArmRelativeCompare(uint16_t ticks);
    };
#endif
}

#endif

#endif
