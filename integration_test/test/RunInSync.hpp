#include "cucumber-cpp/Context.hpp"
#include "infra/timer/Timer.hpp"

void RunInSync(const std::function<void(const std::function<void()>&)>& action, std::ptrdiff_t barrierCount = 1, infra::Duration timeout = std::chrono::seconds(1));
void RunInSyncOnSystemChange(const std::function<void(const std::function<void()>&)>& action, cucumber_cpp::Context& context, std::ptrdiff_t barrierCount = 1, infra::Duration timeout = std::chrono::seconds(1));
