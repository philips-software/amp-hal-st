#ifndef HAL_ST_NUCLEO_TRACER_INFRASTRUCTURE_HPP
#define HAL_ST_NUCLEO_TRACER_INFRASTRUCTURE_HPP

#include "hal_st/instantiations/StmTracerInfrastructure.hpp"

namespace main_
{
    struct NucleoF767ziTracerInfrastructure
    {
        hal::GpioPinStm traceUartTx{ hal::Port::D, 8 };
        hal::GpioPinStm traceUartRx{ hal::Port::D, 9 };

        main_::StmTracerInfrastructure tracerInfrastructure{ { 3, traceUartTx, traceUartRx } };
        services::Tracer& tracer{ tracerInfrastructure.tracer };
    };

    struct NucleoWb55rgTracerInfrastructure
    {
        hal::GpioPinStm traceUartTx{ hal::Port::B, 6 };
        hal::GpioPinStm traceUartRx{ hal::Port::B, 7 };

        main_::StmTracerInfrastructure tracerInfrastructure{ { 1, traceUartTx, traceUartRx } };
        services::Tracer& tracer{ tracerInfrastructure.tracer };
    };
}

#endif
