#include "integration_test/tester/Tester.hpp"

namespace main_
{
    GpioTester::GpioTester(services::Echo& echo)
        : gpioTester{ echo, inPin, outPin }
    {}

    Tester::Tester(services::Echo& echo)
        : tester(echo, nResetTester)
    {}
}
