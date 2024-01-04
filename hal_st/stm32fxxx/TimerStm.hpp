#ifndef HAL_TIMER_STM_HPP
#define HAL_TIMER_STM_HPP

#include DEVICE_HEADER
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "infra/util/Optional.hpp"

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
            infra::Optional<Trigger> trigger;
        };

        TimerBaseStm(uint8_t aTimerIndex, Timing timing, const Config& config);
        ~TimerBaseStm();

        void Start();
        void Stop();

    private:
        void ConfigureTrigger();

    private:
        uint8_t timerIndex;
        Config config;
        TIM_HandleTypeDef handle = {};
    };
}

#endif

#endif
