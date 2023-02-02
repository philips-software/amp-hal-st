#ifndef HAL_ST_NUCLEO_TRACER_INFRASTRUCTURE_HPP
#define HAL_ST_NUCLEO_TRACER_INFRASTRUCTURE_HPP

#include "hal_st/instantiations/StmTracerInfrastructure.hpp"

namespace main_
{
    struct NucleoF767ziTracerInfrastructure
    {
        NucleoF767ziTracerInfrastructure(bool loggingEnabled = true)
            : traceUartTx(hal::Port::D, 8)
            , traceUartRx(hal::Port::D, 9)
            , tracerInfrastructure({ 3, traceUartTx, traceUartRx }, loggingEnabled)
            , tracer(tracerInfrastructure.tracer)
            , alwaysEnabledTracer(tracerInfrastructure.alwaysEnabledTracer)
        {}

        hal::GpioPinStm traceUartTx;
        hal::GpioPinStm traceUartRx;

        StmTracerInfrastructure tracerInfrastructure;
        services::Tracer& tracer;
        services::Tracer& alwaysEnabledTracer;
    };

    struct NucleoWb55rgTracerInfrastructure
    {
        NucleoWb55rgTracerInfrastructure(bool loggingEnabled = true)
            : traceUartTx(hal::Port::B, 6)
            , traceUartRx(hal::Port::B, 7)
            , tracerInfrastructure({ 1, traceUartTx, traceUartRx }, loggingEnabled)
            , tracer(tracerInfrastructure.tracer)
            , alwaysEnabledTracer(tracerInfrastructure.alwaysEnabledTracer)
        {}

        hal::GpioPinStm traceUartTx;
        hal::GpioPinStm traceUartRx;

        StmTracerInfrastructure tracerInfrastructure;
        services::Tracer& tracer;
        services::Tracer& alwaysEnabledTracer;
    };
}

#endif
