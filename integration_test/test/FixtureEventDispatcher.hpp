#ifndef FIXTURES_EVENTDISPATCHER_HPP
#define FIXTURES_EVENTDISPATCHER_HPP

#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include <condition_variable>
#include <mutex>

namespace infra
{
    struct EventDispatcherThreadAware
        : public infra::EventDispatcherWithWeakPtr::WithSize<50>
    {
        void RequestExecution() override;
        void Idle() override;

        void ExecuteUntil(const infra::Function<bool()>& pred);

    private:
        std::condition_variable condition;
        std::mutex readyMutex;
        bool ready{ false };
    };
}

#endif
