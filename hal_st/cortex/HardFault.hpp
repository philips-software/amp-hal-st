#pragma once

#include "infra/util/Function.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal::hard_fault
{
    using TracerProvider = infra::Function<services::Tracer&()>;

    void RegisterDefaultHandler();
    void RegisterTracerProvider(TracerProvider provider);

}
