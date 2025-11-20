#pragma once

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/Function.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal::fault
{
    void SetInterruptStack(const uint32_t* faultStack, uint32_t lrValue);

    using TracerProvider = infra::Function<services::Tracer&()>;

    class DefaultHandler
    {
    public:
        DefaultHandler(TracerProvider tracerProvider = nullptr);

    private:
        hal::ImmediateInterruptHandler hardfaultRegistration;
    };

}
