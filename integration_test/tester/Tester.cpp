#include "integration_test/tester/Tester.hpp"

namespace main_
{
    GpioTester::GpioTester(services::Echo& echo)
        : gpioTester{ echo, inPin, outPin }
    {}

    UartTester::UartTester(services::Echo& echo)
        : uartTester{ echo, bufferedUart }
    {}

    Tester::Tester(services::Echo& echo, services::EchoOnSesame& echoToTested)
        : tester(echo, nResetTester, echoToTested)
    {}
}
