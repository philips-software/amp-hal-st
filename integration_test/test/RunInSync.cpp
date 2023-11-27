#include "infra/event/EventDispatcher.hpp"
#include "integration_test/test/FixtureSystemChanges.hpp"
#include "integration_test/test/RunInSync.hpp"
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

    infra::EventDispatcher::Instance().Schedule([&]
        {
            action([&]()
                {
                    barrier.arrive_and_drop();
                });
        });

    ASSERT_THAT(future.wait_for(timeout), testing::Eq(std::future_status::ready));
}

void RunInSyncOnSystemChange(const std::function<void(const std::function<void()>&)>& action, cucumber_cpp::Context& context, std::ptrdiff_t barrierCount, infra::Duration timeout)
{
    auto timeoutMoment = infra::Now() + timeout;
    bool reached = false;
    std::promise<void> promise;
    std::future<void> future{ promise.get_future() };

    std::barrier barrier{ barrierCount, [&]() noexcept
        {
            reached = true;
            promise.set_value();
        } };

    auto changer = context.get<main_::SystemChanges>()->Register([&]()
        {
            promise.set_value();
        });

    while (!reached)
    {
        infra::EventDispatcher::Instance().Schedule([&]
            {
                action([&]()
                    {
                        barrier.arrive_and_drop();
                    });
            });

        if (future.wait_until(timeoutMoment) == std::future_status::timeout)
            break;

        promise = std::promise<void>{};
    }

    ASSERT_THAT(reached, testing::IsTrue());
}
