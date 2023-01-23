#ifndef HAL_EVENT_DISPATCHER_CORTEX_HPP
#define HAL_EVENT_DISPATCHER_CORTEX_HPP

#include "infra/event/EventDispatcher.hpp"

namespace hal
{
    class EventDispatcherCortexWorker
        : public infra::EventDispatcherWorkerImpl
    {
    public:
        template<std::size_t StorageSize>
        using WithSize = infra::WithStorage<EventDispatcherCortexWorker, std::array<std::pair<infra::Function<void()>, std::atomic<bool>>, StorageSize>>;

        EventDispatcherCortexWorker(infra::MemoryRange<std::pair<infra::Function<void()>, std::atomic<bool>>> scheduledActionsStorage);

    protected:
        virtual void Idle() override;
        virtual void RequestExecution() override;
    };

    using EventDispatcherCortex = infra::EventDispatcherConnector<EventDispatcherCortexWorker>;
}

#endif
