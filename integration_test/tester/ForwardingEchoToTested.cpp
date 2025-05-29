#include "integration_test/tester/ForwardingEchoToTested.hpp"

namespace main_
{
    ForwardingEchoToTested::ForwardingEchoToTested(services::Echo& echo, hal::DmaStm& dma, services::Tracer& tracer)
        : echoToTested(dma, tracer)
        , forwardTested(echo, testing::Tested::serviceId, echoToTested.echo.echo)
        , forwardTestedObserver(echoToTested.echo.echo, testing::TestedObserver::serviceId, echo)
        , forwardGpioTested(echo, testing::GpioTested::serviceId, echoToTested.echo.echo)
        , forwardGpioObserver(echoToTested.echo.echo, testing::GpioObserver::serviceId, echo)
        , forwardUartTested(echo, testing::UartTested::serviceId, echoToTested.echo.echo)
        , forwardUartObserver(echoToTested.echo.echo, testing::UartObserver::serviceId, echo)
    {}
}
