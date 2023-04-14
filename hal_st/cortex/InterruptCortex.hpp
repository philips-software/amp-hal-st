#ifndef HAL_INTERRUPT_CORTEX_HPP
#define HAL_INTERRUPT_CORTEX_HPP

#include DEVICE_HEADER
#include "infra/util/Function.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "infra/util/MemoryRange.hpp"
#include "infra/util/WithStorage.hpp"
#include <array>

namespace hal
{
    enum class InterruptPriority : uint8_t
    {
        High = 1,
        Normal = 2
    };

    IRQn_Type ActiveInterrupt();

    class InterruptHandler
    {
    protected:
        InterruptHandler();
        InterruptHandler(const InterruptHandler& other) = delete;
        InterruptHandler(InterruptHandler&& other);
        InterruptHandler& operator=(const InterruptHandler& other) = delete;
        InterruptHandler& operator=(InterruptHandler&& other);
        ~InterruptHandler();

    public:
        void Register(IRQn_Type irq, InterruptPriority priority = InterruptPriority::Normal);
        void Unregister();
        virtual void Invoke() = 0;

        IRQn_Type Irq() const;

        void ClearPending();

    private:
        IRQn_Type irq;
    };

    class InterruptTable
        : public infra::InterfaceConnector<InterruptTable>
    {
    public:
        template<std::size_t Size>
        using WithStorage = infra::WithStorage<InterruptTable, std::array<InterruptHandler*, Size>>;

        InterruptTable(infra::MemoryRange<InterruptHandler*> table);

        void Invoke(IRQn_Type irq);
        InterruptHandler* Handler(IRQn_Type irq);

    private:
        friend class InterruptHandler;

        void RegisterHandler(IRQn_Type irq, InterruptHandler& handler, InterruptPriority priority);
        void DeregisterHandler(IRQn_Type irq, InterruptHandler& handler);
        void TakeOverHandler(IRQn_Type irq, InterruptHandler& handler, const InterruptHandler& previous);

    private:
        infra::MemoryRange<InterruptHandler*> table;
    };

    class DispatchedInterruptHandler
        : public InterruptHandler
    {
    public:
        DispatchedInterruptHandler(IRQn_Type irq, const infra::Function<void()>& onInvoke);
        DispatchedInterruptHandler(const DispatchedInterruptHandler& other) = delete;
        DispatchedInterruptHandler(DispatchedInterruptHandler&& other) = delete;
        DispatchedInterruptHandler(DispatchedInterruptHandler&& other, const infra::Function<void()>& onInvoke);
        DispatchedInterruptHandler& operator=(const DispatchedInterruptHandler& other) = delete;
        DispatchedInterruptHandler& operator=(DispatchedInterruptHandler&& other) = delete;
        DispatchedInterruptHandler& Assign(DispatchedInterruptHandler&& other, const infra::Function<void()>& onInvoke);

        virtual void Invoke() final;
        void SetInvoke(const infra::Function<void()>& onInvoke);

    private:
        static void InvokeScheduled(IRQn_Type irq, DispatchedInterruptHandler& handler);

    private:
        infra::Function<void()> onInvoke;
        bool pending = false;
    };

    class ImmediateInterruptHandler
        : public InterruptHandler
    {
    public:
        ImmediateInterruptHandler(IRQn_Type irq, const infra::Function<void()>& onInvoke);
        ImmediateInterruptHandler(const ImmediateInterruptHandler& other) = delete;
        ImmediateInterruptHandler(ImmediateInterruptHandler&& other) = delete;
        ImmediateInterruptHandler(ImmediateInterruptHandler&& other, const infra::Function<void()>& onInvoke);
        ImmediateInterruptHandler& operator=(const ImmediateInterruptHandler& other) = delete;
        ImmediateInterruptHandler& operator=(ImmediateInterruptHandler&& other) = delete;
        ImmediateInterruptHandler& Assign(ImmediateInterruptHandler&& other, const infra::Function<void()>& onInvoke);

        virtual void Invoke() final;

    private:
        infra::Function<void()> onInvoke;
    };
}

#endif
