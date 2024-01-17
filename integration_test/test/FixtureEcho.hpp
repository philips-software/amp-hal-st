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
    struct FixtureEcho
    {
        FixtureEcho(services::Echo& echo)
            : echo(echo)
        {}

        services::Echo& echo;
        // testing::TesterTracer testerTracer{ echo };
        // testing::TestedTracer testedTracer{ echo };
        // testing::TestedObserverTracer testedObserverTracer{ echo };
        // testing::GpioTesterTracer gpioTesterTracer{ echo };
        // testing::GpioTestedTracer gpioTestedTracer{ echo };
        // testing::GpioObserverTracer gpioObserverTracer{ echo };
    };

    struct FixtureEchoSerial
    {
        FixtureEchoSerial(const std::string& portName)
            : serial(portName)
        {}

#ifdef EMIL_HAL_WINDOWS
        hal::UartWindows serial;
#else
        hal::UartUnix serial;
#endif
        services::MethodSerializerFactory::OnHeap serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedSerial{ serial };
        main_::EchoOnSesame<256> echoOnSesame{ bufferedSerial, serializerFactory };

        services::Echo& echo{ echoOnSesame.echo };
        FixtureEcho fixture{ echo };
    };
}

#endif
