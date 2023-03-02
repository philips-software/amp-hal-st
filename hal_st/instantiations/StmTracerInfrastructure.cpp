#include "hal_st/instantiations/StmTracerInfrastructure.hpp"

namespace main_
{
    StmTracerInfrastructure::Configuration::Configuration(uint8_t index, hal::GpioPinStm& tx, hal::GpioPinStm& rx)
        : index(index)
        , tx(tx)
        , rx(rx)
    {}

    StmTracerInfrastructure::StmTracerInfrastructure(const Configuration& configuration, bool loggingEnabled)
        : traceUart(configuration.index, configuration.tx)
        , traceWriter(traceUart)
        , alwaysEnabledTracerOutputStream(traceWriter)
        , tracerOutputStream(GetStreamWriter(loggingEnabled), infra::noFail)
        , alwaysEnabledTracer(alwaysEnabledTracerOutputStream)
        , tracer(tracerOutputStream)
    {
        tracer.Trace() << "----------------------------------------------------------";
    }

#if defined(STM32WB)
    StmTracerInfrastructure::StmTracerInfrastructure(const Configuration& configuration, hal::SyncLpUart syncLpUart, bool loggingEnabled)
        : traceUart(configuration.index, configuration.tx, syncLpUart)
        , traceWriter(traceUart)
        , alwaysEnabledTracerOutputStream(traceWriter)
        , tracerOutputStream(GetStreamWriter(loggingEnabled), infra::noFail)
        , alwaysEnabledTracer(alwaysEnabledTracerOutputStream)
        , tracer(tracerOutputStream)
    {
        tracer.Trace() << "----------------------------------------------------------";
    }
#endif

    infra::StreamWriter& StmTracerInfrastructure::GetStreamWriter(bool loggingEnabled)
    {
        if (loggingEnabled)
            return traceWriter;
        else
            return dummyWriter;
    }
}
