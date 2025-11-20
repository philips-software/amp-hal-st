#pragma once

#include "infra/util/Function.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal
{
    using HardFaultTracerProvider = infra::Function<services::Tracer&()>;

    void RegisterHardFaultTracerProvider(HardFaultTracerProvider provider);
    void HardFaultHandler(const uint32_t* faultStack, uint32_t lr_value);
}
