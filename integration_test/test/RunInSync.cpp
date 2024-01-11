#include "integration_test/test/RunInSync.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "integration_test/test/FixtureSystemChanges.hpp"
#include "gmock/gmock.h"
#include <barrier>
#include <future>

void RunInSync(const std::function<void(const std::function<void()>&)>& action, std::ptrdiff_t barrierCount, infra::Duration timeout)
{
    std::promise<void> promise;
    std::future<void> future{ promise.get_future() };

    std::barrier barrier{ barrierCount, [&]() noexcept
        {
            promise.set_value();
        } };

    std::function<void()> onDone{ [&]()
        {
            barrier.arrive_and_drop();
        } };

    infra::EventDispatcher::Instance().Schedule([&]
        {
            action(onDone);
        });

    ASSERT_THAT(future.wait_for(timeout), testing::Eq(std::future_status::ready));
}

void RunInSyncOnSystemChange(const std::function<void(const std::function<void()>&)>& action, cucumber_cpp::Context& context, std::ptrdiff_t barrierCount, infra::Duration timeout)
{
    auto timeoutMoment = infra::Now() + timeout;
    bool timedOut = false;

    struct
    {
        std::mutex mutex;
        std::condition_variable condition;
        bool running = false;

        std::function<void()> onDone;
    } sync;

    sync.onDone = [&]()
    {
        --barrierCount;
    };

    std::unique_lock<std::mutex> lock(sync.mutex);
    while (barrierCount != 0 && !timedOut)
    {
        sync.running = true;
        infra::EventDispatcher::Instance().Schedule([&]
            {
                std::lock_guard<std::mutex> lock(sync.mutex);
                action(sync.onDone);
                sync.running = false;
                sync.condition.notify_all();
            });

        while (sync.running && !timedOut)
            if (sync.condition.wait_until(lock, timeoutMoment) == std::cv_status::timeout)
                timedOut = true;
    }

    ASSERT_THAT(barrierCount, testing::Eq(0));

    while (sync.running)
        sync.condition.wait(lock);
}
