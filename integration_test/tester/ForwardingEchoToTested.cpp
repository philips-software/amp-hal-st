#include "integration_test/tester/ForwardingEchoToTested.hpp"

namespace main_
{
    ForwardingEchoToTested::ForwardingEchoToTested(services::Echo& echo)
        : forwardTested(buffer, echo, testing::Tested::serviceId, echoToTested.echo)
        , forwardGpioTested(buffer, echo, testing::GpioTested::serviceId, echoToTested.echo)
        , forwardGpioObserver(buffer, echoToTested.echo, testing::GpioObserver::serviceId, echo)
    {}
}
