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

    protected:
        LowPowerTimerBaseStm(uint8_t oneBasedIndex, Timing timing);
        ~LowPowerTimerBaseStm();

        LPTIM_HandleTypeDef handle{};
        const uint8_t timerIndex;

    public:
        auto& Handle()
        {
            return handle;
        }
    };

    class FreeRunningLowPowerTimerStm
        : public LowPowerTimerBaseStm
    {
    public:
        FreeRunningLowPowerTimerStm(uint8_t aTimerIndex, Timing timing);

        void Start();
        void Stop();
    };

    class LowPowerTimerWithInterruptStm
        : public LowPowerTimerBaseStm
    {
    public:
        LowPowerTimerWithInterruptStm(uint8_t aTimerIndex, Timing timing);

        void Start(const infra::Function<void()>& onIrq, InterruptType type = InterruptType::immediate);
        void Stop();

    private:
        ImmediateInterruptHandler interruptHandler;
        infra::Function<void()> onIrq;
        InterruptType type;
        std::atomic_bool scheduled{};

        void OnInterrupt();
        void ScheduleInterrupt();
    };
}

#endif

#endif
