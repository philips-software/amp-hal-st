#include "hal_st/cortex/EventDispatcherCortex.hpp"
#include DEVICE_HEADER

namespace hal
{
    EventDispatcherCortexWorker::EventDispatcherCortexWorker(infra::MemoryRange<std::pair<infra::Function<void()>, std::atomic<bool>>> scheduledActionsStorage)
        : infra::EventDispatcherWorkerImpl(scheduledActionsStorage)
    {}

    void EventDispatcherCortexWorker::Idle()
    {
        __DSB();
        __WFE();
    }

    void EventDispatcherCortexWorker::RequestExecution()
    {
        __DSB();
        __SEV();
    }
}
