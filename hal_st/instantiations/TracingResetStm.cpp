#include "hal_st/instantiations/TracingResetStm.hpp"

namespace main_
{
    TracingResetStm::TracingResetStm(services::Tracer& tracer)
        : reset(resetStm, tracer)
    {}
}
