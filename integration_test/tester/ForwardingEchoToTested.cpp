#include "integration_test/tester/ForwardingEchoToTested.hpp"

namespace main_
{
    ForwardingEchoToTested::ForwardingEchoToTested(services::Echo& echo, hal::DmaStm& dma, services::Tracer& tracer)
        : echoToTested(dma, tracer)
        , forwardTested(echo, testing::Tested::serviceId, echoToTested.echo)
        , forwardGpioTested(echo, testing::GpioTested::serviceId, echoToTested.echo)
        , forwardGpioObserver(echoToTested.echo, testing::GpioObserver::serviceId, echo)
    {}
}
