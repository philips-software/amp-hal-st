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
        , uart{ dma, 2, tx, rx }
    {}

    Tested::Tested(services::Echo& echo, hal::DmaStm& dma)
        : tested(echo)
        , uartDuplexDmaTested{ tested, testing::Peripheral::uartDuplexDma, dma }
    {}
}
