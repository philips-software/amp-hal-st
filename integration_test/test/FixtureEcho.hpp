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
        FixtureEcho(const std::string& portName)
            : serial(portName)
        {}

#ifdef EMIL_HAL_WINDOWS
        hal::UartWindows serial;
#else
        hal::UartUnix serial;
#endif
        services::MethodSerializerFactory::OnHeap serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedSerial{ serial };
        main_::EchoOnSesame<256> echo{ bufferedSerial, serializerFactory };

        // testing::TesterTracer testerTracer{ echo.echo };
        // testing::TestedTracer testedTracer{ echo.echo };
        // testing::TestedObserverTracer testedObserverTracer{ echo.echo };
        // testing::GpioTesterTracer gpioTesterTracer{ echo.echo };
        // testing::GpioTestedTracer gpioTestedTracer{ echo.echo };
        // testing::GpioObserverTracer gpioObserverTracer{ echo.echo };
    };
}

#endif
