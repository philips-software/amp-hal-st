#include "integration_test/test/FixtureEventDispatcher.hpp"

namespace infra
{
    void EventDispatcherThreadAware::RequestExecution()
    {
        std::lock_guard lock{ readyMutex };

        ready = true;
        condition.notify_one();
    }

    void EventDispatcherThreadAware::Idle()
    {
        std::unique_lock lock{ readyMutex };

        condition.wait(lock, [this]
            {
                return ready;
            });
        ready = false;
    }

    void EventDispatcherThreadAware::ExecuteUntil(const infra::Function<bool()>& pred)
    {
        while (!pred())
        {
            ExecuteAllActions();
            Idle();
        }
    }
}
