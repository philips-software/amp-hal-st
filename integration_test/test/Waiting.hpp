#ifndef INTEGRATION_TEST_WAITING_HPP
#define INTEGRATION_TEST_WAITING_HPP

#include "cucumber-cpp/Context.hpp"
#include "infra/timer/Timer.hpp"
#include "integration_test/test/FixtureEventDispatcher.hpp"

namespace infra
{
    constexpr inline infra::Duration defaultTimeout{ std::chrono::seconds{ 1 } };

    bool WaitUntilDone(cucumber_cpp::Context& context, const std::function<void(const std::function<void()>&)>& action, infra::Duration timeout = defaultTimeout);
    bool WaitFor(cucumber_cpp::Context& context, const std::function<bool()>& pred, infra::Duration timeout = defaultTimeout);
    void WaitFor(cucumber_cpp::Context& context, infra::Duration timeout);

    template<class Obj, class T, class... Arg, class Proj = std::identity>
    bool WaitForValue(cucumber_cpp::Context& context, const Obj& obj, const T& value, Proj proj = {}, Arg&&... args, infra::Duration timeout = defaultTimeout)
    {
        return WaitFor(
            context, [&proj, &obj, &args..., &value]()
            {
                return std::invoke(proj, obj, std::forward<Arg>(args)...) == value;
            },
            timeout);
    }

    struct Async
    {
        explicit Async(cucumber_cpp::Context& context);
        Async(cucumber_cpp::Context& context, std::function<bool()> func);

        operator infra::Function<void()>();

        void Notify();

        [[nodiscard]] bool Wait(infra::Duration timeout) const;

    private:
        cucumber_cpp::Context& context;
        std::function<bool()> func;
        bool notified{ false };
    };
}

#endif
