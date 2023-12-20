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
        enum class CounterMode : uint8_t
        {
            up = TIM_COUNTERMODE_UP,
            down = TIM_COUNTERMODE_DOWN
        };

        struct Trigger
        {
            enum class TriggerOutput
            {
                reset = TIM_TRGO_RESET,
                enable = TIM_TRGO_ENABLE,
                update = TIM_TRGO_UPDATE
            };

            TriggerOutput triggerOutput;
            bool isSlaveMode;
        };

        struct Config
        {
            uint32_t prescaler;
            uint32_t period;
            CounterMode counterMode;
            infra::Optional<Trigger> trigger;
        };

        TimerBaseStm(uint8_t aTimerIndex, const Config& config);
        virtual ~TimerBaseStm();

    private:
        void ConfigureTrigger();

    private:
        uint8_t timerIndex;
        const Config& config;
        TIM_HandleTypeDef handle = {};
    };
}

#endif

#endif
