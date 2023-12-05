#include "integration_test/tested/Tested.hpp"

namespace main_
{
    GpioTested::GpioTested(services::Echo& echo)
        : gpioTested(echo, inPin, outPin)
    {}

    Tested::Tested(services::Echo& echo)
        : tested(echo)
    {}
}
