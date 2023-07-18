#ifndef HAL_LOW_POWER_STRATEGY_STM_HPP
#define HAL_LOW_POWER_STRATEGY_STM_HPP

#include "infra/event/LowPowerEventDispatcher.hpp"

namespace hal
{
    class LowPowerStrategyStm
        : public infra::LowPowerStrategy
    {
    public:
        LowPowerStrategyStm() = default;

        void RequestExecution() override;
        void Idle(const infra::EventDispatcherWorker& eventDispatcher) override;
    };
}

#endif
