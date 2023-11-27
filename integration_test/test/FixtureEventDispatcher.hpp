#ifndef FIXTURES_EVENTDISPATCHER_HPP
#define FIXTURES_EVENTDISPATCHER_HPP

#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include <condition_variable>
#include <mutex>
#include <thread>

namespace infra
{
    struct EventDispatcherThreadAware
        : public infra::EventDispatcherWithWeakPtr::WithSize<50>
    {
        void RequestExecution() override;
        void Idle() override;

    private:
        std::condition_variable condition;
        std::mutex readyMutex;
        bool ready{ false };
    };
}

namespace main_
{
    struct FixtureEventDispatcher
    {
        ~FixtureEventDispatcher();

    private:
        struct ExitException
        {};

        infra::EventDispatcherThreadAware eventDispatcher;
        std::jthread worker{
            [this]
            {
                try
                {
                    eventDispatcher.Run();
                }
                catch (const ExitException& e)
                {}
            }
        };
    };
}

#endif
