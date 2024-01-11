#ifndef FIXTURES_ECHO_HPP
#define FIXTURES_ECHO_HPP

#include "generated/echo/TracingTesting.pb.hpp"
#include "hal/windows/UartWindows.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/tracer/TracingEchoInstantiation.hpp"

namespace main_
{
    struct FixtureEcho
    {
        hal::UartWindows serial{ "COM7" };
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
