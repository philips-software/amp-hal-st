#ifndef HAL_TIMER_STM_HPP
#define HAL_TIMER_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/Function.hpp"
#include <atomic>
#include <optional>
#include DEVICE_HEADER

#if defined(HAS_PERIPHERAL_TIMER)

namespace hal
{
    class TimerBaseStm
    {
    public:
        struct Timing
        {
            uint32_t prescaler;
            uint32_t period;
        };

        enum class CounterMode : uint32_t
        {
            up = TIM_COUNTERMODE_UP,
            down = TIM_COUNTERMODE_DOWN
        };

        struct Trigger
        {
            enum class TriggerOutput : uint32_t
            {
                reset = TIM_TRGO_RESET,
                enable = TIM_TRGO_ENABLE,
                update = TIM_TRGO_UPDATE,
            };

            TriggerOutput triggerOutput;
            bool isSlaveMode;
        };

        struct Config
        {
            CounterMode counterMode;
            std::optional<Trigger> trigger;
        };

    protected:
        TimerBaseStm(uint8_t oneBasedIndex, Timing timing, const Config& config);
        ~TimerBaseStm();

    private:
        void ConfigureTrigger();

        const uint8_t timerIndex;
        Config config;

    protected:
        TIM_HandleTypeDef handle = {};

    public:
        auto& Handle()
        {
            return handle;
        }
    };

    class FreeRunningTimerStm
        : public TimerBaseStm
    {
    public:
        FreeRunningTimerStm(uint8_t oneBasedIndex, Timing timing, const Config& config);

        void Start();
        void Stop();
    };

    class TimerWithInterruptStm
        : public TimerBaseStm
    {
    public:
        TimerWithInterruptStm(uint8_t oneBasedIndex, Timing timing);

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
