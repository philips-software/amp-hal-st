#ifndef HAL_ST_STM_TRACER_INFRASTRUCTURE_HPP
#define HAL_ST_STM_TRACER_INFRASTRUCTURE_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/synchronous_stm32fxxx/SynchronousUartStm.hpp"
#include "services/tracer/StreamWriterOnSynchronousSerialCommunication.hpp"
#include "services/tracer/TracerWithDateTime.hpp"

namespace main_
{
    class TimeKeeperStub
        : public hal::TimeKeeper
    {
    public:
        virtual bool Timeout() override { return false; }
        virtual void Reset() override {}
    };

    struct StmTracerInfrastructure
    {
        struct Configuration
        {
            Configuration(uint8_t index, hal::GpioPinStm& tx, hal::GpioPinStm& rx);

            uint8_t index;
            hal::GpioPinStm& tx;
            hal::GpioPinStm& rx;
        };

        StmTracerInfrastructure(const Configuration& configuration);

        TimeKeeperStub timeKeeper;
        hal::SynchronousUartStmSendOnly traceUart;

        services::StreamWriterOnSynchronousSerialCommunication streamWriter;
        infra::TextOutputStream::WithErrorPolicy stream;
        services::TracerWithDateTime tracer;
    };
}

#endif
