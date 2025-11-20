#pragma once

#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/Function.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal::hard_fault
{
    using TracerProvider = infra::Function<services::Tracer&()>;

    class DefaultHandler
    {
    public:
        DefaultHandler(TracerProvider tracerProvider = nullptr);

    private:
        hal::ImmediateInterruptHandler hardfaultRegistration;
    };

}
