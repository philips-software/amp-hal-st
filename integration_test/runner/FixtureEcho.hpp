#ifndef FIXTURES_ECHO_HPP
#define FIXTURES_ECHO_HPP

#include "generated/echo/TracingTesting.pb.hpp"
#include "services/tracer/TracingEchoInstantiation.hpp"

namespace main_
{
    using FixtureEchoSerial = EchoOnUart<256>;

    struct FixtureTracingEchoSerial
        : TracingEchoOnUart<256>
    {
        using TracingEchoOnUart<256>::TracingEchoOnUart;

        testing::TesterTracer testerTracer{ echoOnSesame.echo };
        testing::TestedTracer testedTracer{ echoOnSesame.echo };
        testing::TesterObserverTracer testerObserverTracer{ echoOnSesame.echo };
        testing::TestedObserverTracer testedObserverTracer{ echoOnSesame.echo };
        testing::GpioTesterTracer gpioTesterTracer{ echoOnSesame.echo };
        testing::GpioTestedTracer gpioTestedTracer{ echoOnSesame.echo };
        testing::GpioObserverTracer gpioObserverTracer{ echoOnSesame.echo };
    };
}

#endif
