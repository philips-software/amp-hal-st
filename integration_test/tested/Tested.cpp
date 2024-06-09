#include "integration_test/tested/Tested.hpp"

namespace main_
{
    GpioTested::GpioTested(services::Echo& echo)
        : gpioTested(echo, inPin, outPin)
    {}

    UartTested::UartTested(services::Echo& echo)
        : uartTested{ echo, bufferedUart }
    {}

    UartDuplexDmaTested::UartDuplexDmaTested(services::Echo& echo, hal::DmaStm& dma)
        : uartTested{ echo, bufferedUart }
        , transmitStream{ dma, hal::DmaChannelId{ 1, 6, 4 } }
        , receiveStream{ dma, hal::DmaChannelId{ 1, 5, 4 } }
    {}

    Tested::Tested(services::Echo& echo, hal::DmaStm& dma)
        : tested(echo)
        , uartDuplexDmaTested{ tested, testing::Peripheral::uartDuplexDma, dma }
    {}
}
