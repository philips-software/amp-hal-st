#pragma once

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal::fault
{
    void SetInterruptContext(const uint32_t* faultStack, uint32_t lrValue);
    [[noreturn]] void DumpInterruptStackAndAbort(infra::BoundedConstString fault);

    using TracerProvider = infra::Function<services::Tracer&()>;

    class DefaultHandler : public infra::InterfaceConnector<DefaultHandler>
    {
    public:
        explicit DefaultHandler(TracerProvider tracerProvider = nullptr);

        TracerProvider getTracerProvider() const
        {
            return tracerProvider;
        }

    private:
        TracerProvider tracerProvider = nullptr;
        hal::ImmediateInterruptHandler hardfaultRegistration;
    };

}
