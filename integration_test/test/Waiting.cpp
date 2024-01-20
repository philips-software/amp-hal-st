#include "integration_test/test/Waiting.hpp"
#include "infra/event/EventDispatcherThreadAware.hpp"
#include "gmock/gmock.h"

namespace infra
{
    bool WaitUntilDone(cucumber_cpp::Context& context, const std::function<void(const std::function<void()>&)>& action, infra::Duration timeout)
    {
        bool done{ false };
        bool timedOut{ false };

        std::function<void()> onDone = [&done]()
        {
            done = true;
        };

        action(onDone);

        infra::TimerSingleShot timeoutTimer{ timeout, [&timedOut]()
            {
                timedOut = true;
            } };

        context.Get<infra::EventDispatcherWithWeakPtrWorker>().ExecuteUntil([&done, &timedOut]()
            {
                return done || timedOut;
            });

        return !timedOut;
    }

    bool WaitFor(cucumber_cpp::Context& context, const std::function<bool()>& pred, infra::Duration timeout)
    {
        bool timedOut{ false };

        infra::TimerSingleShot timeoutTimer{ timeout, [&timedOut]
            {
                timedOut = true;
            } };

        context.Get<infra::EventDispatcherWithWeakPtrWorker>().ExecuteUntil([&pred, &timedOut]()
            {
                return pred() || timedOut;
            });

        return !timedOut;
    }

    void WaitFor(cucumber_cpp::Context& context, infra::Duration timeout)
    {
        bool done{ false };

        infra::TimerSingleShot timeoutTimer{ timeout, [&done]()
            {
                done = true;
            } };

        context.Get<infra::EventDispatcherWithWeakPtrWorker>().ExecuteUntil([&done]()
            {
                return done;
            });
    }

    Async::Async(cucumber_cpp::Context& context)
        : Async{ context, [this]()
            {
                return notified;
            } }
    {}

    Async::Async(cucumber_cpp::Context& context, std::function<bool()> func)
        : context{ context }
        , func{ std::move(func) }
    {}

    Async::operator infra::Function<void()>()
    {
        return [this]()
        {
            Notify();
        };
    }

    void Async::Notify()
    {
        notified = true;
    }

    [[nodiscard]] bool Async::Wait(infra::Duration timeout) const
    {
        return WaitFor(
            context, [this]()
            {
                return func();
            },
            timeout);
    }
}
