#include <functional>
#include <list>
#include <memory>

namespace main_
{
    class SystemChanges
    {
    public:
        std::shared_ptr<void> Register(const std::function<void()>& onChange)
        {
            onChanges.push_back(onChange);
            auto change = std::prev(onChanges.end());

            return std::shared_ptr<void>(&onChanges.back(), [this, change](auto element)
                {
                    onChanges.erase(change);
                });
        }

        void Trigger() const
        {
            for (const auto& change : onChanges)
                change();
        }

    private:
        std::list<std::function<void()>> onChanges;
    };
}
