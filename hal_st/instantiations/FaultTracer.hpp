#pragma once

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "infra/util/MemoryRange.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal
{

    using TracerProvider = infra::Function<services::Tracer&()>;

    class DefaultFaultTracer
        : public infra::InterfaceConnector<DefaultFaultTracer>
    {
    public:
        explicit DefaultFaultTracer(const infra::MemoryRange<const uint32_t>& instructionRange, const uint32_t* endOfStack, TracerProvider tracerProvider = nullptr);

        void SetInterruptContext(const uint32_t* faultStack, uint32_t lrValue);
        [[noreturn]] void DumpInterruptStackAndAbort(infra::BoundedConstString fault) const;

    private:
        void PrintBacktrace(const uint32_t* stack, services::Tracer& tracer) const;
        [[noreturn]] void DumpCurrentInterruptStackAndAbort(services::Tracer& tracer) const;

    private:
        struct InterruptContext
        {
            const uint32_t* stack;
            uint32_t lrValue;
        };

        const infra::MemoryRange<const uint32_t> instructionRange;
        const uint32_t* endOfStack;
        TracerProvider tracerProvider = nullptr;
        hal::ImmediateInterruptHandler hardfaultRegistration;
        InterruptContext interruptContext;
    };

}
