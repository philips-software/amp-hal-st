#include "hal_st/instantiations/StmTracerInfrastructure.hpp"

namespace main_
{
    StmTracerInfrastructure::Configuration::Configuration(uint8_t index, hal::GpioPinStm& tx, hal::GpioPinStm& rx)
        : index(index)
        , tx(tx)
        , rx(rx)
    {}

    StmTracerInfrastructure::StmTracerInfrastructure(const Configuration& configuration)
        : traceUart(configuration.index, configuration.tx)
        , streamWriter(traceUart)
        , stream(streamWriter, infra::noFail)
        , tracer(stream)
    {
        tracer.Trace() << "----------------------------------------------------------";
    }
}
