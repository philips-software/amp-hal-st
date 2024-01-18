#ifndef FIXTURES_ECHO_HPP
#define FIXTURES_ECHO_HPP

#include "generated/echo/TracingTesting.pb.hpp"
#ifdef EMIL_HAL_WINDOWS
#include "hal/windows/UartWindows.hpp"
#else
#include "hal/unix/UartUnix.hpp"
#endif
#include "services/tracer/GlobalTracer.hpp"
#include "services/tracer/TracingEchoInstantiation.hpp"

namespace main_
{
    struct FixtureEchoSerialBase
    {
        FixtureEchoSerialBase(const std::string& portName)
            : serial(portName)
        {}

#ifdef EMIL_HAL_WINDOWS
        hal::UartWindows serial;
#else
        hal::UartUnix serial;
#endif
        services::MethodSerializerFactory::OnHeap serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedSerial{ serial };
    };

    struct FixtureEchoSerial
        : FixtureEchoSerialBase
    {
        main_::EchoOnSesame<256> echoOnSesame{ bufferedSerial, serializerFactory };

        services::Echo& echo{ echoOnSesame.echo };
    };

    struct FixtureTracingEchoSerial
        : FixtureEchoSerialBase
    {
        main_::TracingEchoOnSesame<256> echoOnSesame{ bufferedSerial, serializerFactory, services::GlobalTracer() };

        services::Echo& echo{ echoOnSesame.echo };
        testing::TesterTracer testerTracer{ echoOnSesame.echo };
        testing::TestedTracer testedTracer{ echoOnSesame.echo };
        testing::TestedObserverTracer testedObserverTracer{ echoOnSesame.echo };
        testing::GpioTesterTracer gpioTesterTracer{ echoOnSesame.echo };
        testing::GpioTestedTracer gpioTestedTracer{ echoOnSesame.echo };
        testing::GpioObserverTracer gpioObserverTracer{ echoOnSesame.echo };
    };
}

#endif
