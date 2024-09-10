#include "integration_test/tester/Tester.hpp"

namespace main_
{
    GpioTester::GpioTester(services::Echo& echo)
        : gpioTester{ echo, inPin, outPin }
    {}

    UartTester::UartTester(services::Echo& echo)
        : uartTester{ echo, bufferedUart }
    {}

    UartDuplexDmaTester::UartDuplexDmaTester(services::Echo& echo, hal::DmaStm& dma)
        : uartTester{ echo, bufferedUart }
        , transmitStream{ dma, hal::DmaChannelId{ 1, 6, 4 } }
        , receiveStream{ dma, hal::DmaChannelId{ 1, 5, 4 } }
    {}

    Tester::Tester(services::Echo& echo, services::EchoOnSesame& echoToTested, hal::DmaStm& dma)
        : tester(echo, nResetTester, echoToTested)
        , uartDuplexDmaTester{ tester, testing::Peripheral::uartDuplexDma, dma }
    {}
}
