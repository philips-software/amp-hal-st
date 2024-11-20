#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/event/EventDispatcher.hpp"
#include <cstdlib>

namespace hal
{
    namespace
    {
        void EnableInterrupt(IRQn_Type irq, InterruptPriority priority)
        {
            if (irq >= 0)
            {
                NVIC_SetPriority(irq, static_cast<uint32_t>(priority));
                NVIC_EnableIRQ(irq);
            }
            else if (irq == -13 /*HardFault_IRQn*/)
                ;
            else if (irq == NonMaskableInt_IRQn)
                ;
            else if (irq == PendSV_IRQn)
                ;
            else if (irq == SysTick_IRQn)
            {
                NVIC_SetPriority(irq, static_cast<uint32_t>(InterruptPriority::Normal));
                SysTick->CTRL |= 1 << 1;
            }
            else
                std::abort();
        }

        void DisableInterrupt(IRQn_Type irq)
        {
            if (irq >= 0)
            {
                NVIC_DisableIRQ(irq);
            }
            else if (irq == -13 /*HardFault_IRQn*/)
                ;
            else if (irq == PendSV_IRQn)
                ;
            else if (irq == SysTick_IRQn)
            {
                SysTick->CTRL &= ~(1 << 1);
                SCB->ICSR |= 1 << 25;
            }
            else
                std::abort();
        }
    }

    IRQn_Type ActiveInterrupt()
    {
        return static_cast<IRQn_Type>(((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) >> SCB_ICSR_VECTACTIVE_Pos) - 16);
    }

    InterruptHandler::InterruptHandler()
    {}

    InterruptHandler::InterruptHandler(InterruptHandler&& other)
        : irq(other.irq)
    {
        if (irq)
            InterruptTable::Instance().TakeOverHandler(*irq, *this, other);

        other.irq = infra::none;
    }

    InterruptHandler& InterruptHandler::operator=(InterruptHandler&& other)
    {
        if (irq)
            InterruptTable::Instance().DeregisterHandler(*irq, *this);

        irq = other.irq;

        if (irq)
            InterruptTable::Instance().TakeOverHandler(*irq, *this, other);

        other.irq = infra::none;

        return *this;
    }

    InterruptHandler::~InterruptHandler()
    {
        if (irq)
            InterruptTable::Instance().DeregisterHandler(*irq, *this);
    }

    void InterruptHandler::Register(IRQn_Type irq, InterruptPriority priority)
    {
        this->irq = irq;
        InterruptTable::Instance().RegisterHandler(*this->irq, *this, priority);
    }

    void InterruptHandler::Unregister()
    {
        if (irq)
            InterruptTable::Instance().DeregisterHandler(*irq, *this);

        irq = infra::none;
    }

    IRQn_Type InterruptHandler::Irq() const
    {
        return *irq;
    }

    void InterruptHandler::ClearPending()
    {
        NVIC_ClearPendingIRQ(*irq);
    }

    InterruptTable::InterruptTable(infra::MemoryRange<InterruptHandler*> table)
        : table(table)
    {
        std::fill(table.begin(), table.end(), nullptr);
    }

    void InterruptTable::Invoke(IRQn_Type irq)
    {
        __DMB();
        if (!table[irq + 16])
            std::abort();

        table[irq + 16]->Invoke();
    }

    InterruptHandler* InterruptTable::Handler(IRQn_Type irq)
    {
        return table[irq + 16];
    }

    void InterruptTable::RegisterHandler(IRQn_Type irq, InterruptHandler& handler, InterruptPriority priority)
    {
        assert(irq + 16 < table.size());
        assert(table[irq + 16] == nullptr);
        table[irq + 16] = &handler;
        __DSB();
        EnableInterrupt(irq, priority);
    }

    void InterruptTable::DeregisterHandler(IRQn_Type irq, InterruptHandler& handler)
    {
        assert(table[irq + 16] == &handler);
        DisableInterrupt(irq);
        __DSB();
        table[irq + 16] = nullptr;
    }

    void InterruptTable::TakeOverHandler(IRQn_Type irq, InterruptHandler& handler, const InterruptHandler& previous)
    {
        assert(table[irq + 16] == &previous);
        table[irq + 16] = &handler;
        __DSB();
    }

    DispatchedInterruptHandler::DispatchedInterruptHandler(IRQn_Type irq, const infra::Function<void()>& onInvoke)
        : InterruptHandler()
        , onInvoke(onInvoke)
    {
        Register(irq);
    }

    DispatchedInterruptHandler::DispatchedInterruptHandler(DispatchedInterruptHandler&& other, const infra::Function<void()>& onInvoke)
        : InterruptHandler(std::move(other))
        , onInvoke(onInvoke)
    {}

    DispatchedInterruptHandler& DispatchedInterruptHandler::Assign(DispatchedInterruptHandler&& other, const infra::Function<void()>& onInvoke)
    {
        InterruptHandler::operator=(std::move(other));
        this->onInvoke = onInvoke;
        return *this;
    }

    void DispatchedInterruptHandler::Invoke()
    {
        DisableInterrupt(Irq());
        assert(!pending);
        pending = true;

        IRQn_Type irq = Irq();
        DispatchedInterruptHandler& handler = *this;
        infra::EventDispatcher::Instance().Schedule([irq, &handler]()
            {
                InvokeScheduled(irq, handler);
            });
    }

    void DispatchedInterruptHandler::SetInvoke(const infra::Function<void()>& onInvoke)
    {
        this->onInvoke = onInvoke;
    }

    void DispatchedInterruptHandler::InvokeScheduled(IRQn_Type irq, DispatchedInterruptHandler& handler)
    {
        if (InterruptTable::Instance().Handler(irq) == &handler)
        {
            infra::Function<void()> invoke = handler.onInvoke;
            invoke();

            if (InterruptTable::Instance().Handler(irq) == &handler)
            {
                handler.pending = false;
                if (handler.onInvoke)
                    EnableInterrupt(irq, InterruptPriority::Normal);
            }
        }
    }

    ImmediateInterruptHandler::ImmediateInterruptHandler(IRQn_Type irq, const infra::Function<void()>& onInvoke)
        : InterruptHandler()
        , onInvoke(onInvoke)
    {
        Register(irq);
    }

    ImmediateInterruptHandler::ImmediateInterruptHandler(ImmediateInterruptHandler&& other, const infra::Function<void()>& onInvoke)
        : InterruptHandler(std::move(other))
        , onInvoke(onInvoke)
    {}

    ImmediateInterruptHandler& ImmediateInterruptHandler::Assign(ImmediateInterruptHandler&& other, const infra::Function<void()>& onInvoke)
    {
        InterruptHandler::operator=(std::move(other));
        this->onInvoke = onInvoke;
        return *this;
    }

    void ImmediateInterruptHandler::Invoke()
    {
        onInvoke();
    }
}
