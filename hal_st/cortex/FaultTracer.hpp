#ifndef HAL_ST_CORTEX_FAULT_TRACER_HPP
#define HAL_ST_CORTEX_FAULT_TRACER_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "infra/util/MemoryRange.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal
{

    struct InterruptContext
    {
        const uint32_t* stack;
        uint32_t lrValue;
    };

    extern InterruptContext interruptContext;

    using TracerProvider = infra::Function<services::Tracer&()>;

    class DefaultFaultTracer
        : public infra::InterfaceConnector<DefaultFaultTracer>
    {
    public:
        explicit DefaultFaultTracer(const infra::MemoryRange<const uint32_t>& instructionRange, const uint32_t* endOfStack, TracerProvider tracerProvider = nullptr);

        [[noreturn]] void DumpInterruptStackAndAbort(infra::BoundedConstString fault) const;

    private:
        void PrintBacktrace(const uint32_t* stack, services::Tracer& tracer) const;
        void DumpCfsrContents(uint32_t cfsr, services::Tracer& tracer) const;
        [[noreturn]] void DumpCurrentInterruptStackAndAbort(services::Tracer& tracer) const;

    private:
        const infra::MemoryRange<const uint32_t> instructionRange;
        const uint32_t* endOfStack;
        TracerProvider tracerProvider = nullptr;
        hal::ImmediateInterruptHandler hardfaultRegistration;
    };

}

#endif // HAL_ST_CORTEX_FAULT_TRACER_HPP
