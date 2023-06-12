#ifndef HAL_ST_INTEGRATION_TEST_ECHO_ON_SESAME_HPP
#define HAL_ST_INTEGRATION_TEST_ECHO_ON_SESAME_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "services/util/EchoInstantiation.hpp"

namespace main_
{
    struct EchoBetweenTesterAndTested
    {
        hal::GpioPinStm echoUartTx{ hal::Port::D, 2 };
        hal::GpioPinStm echoUartRx{ hal::Port::C, 12 };
        hal::UartStm echoUart{ 5, echoUartTx, echoUartRx };
        main_::EchoOnSerialCommunication<1024> echo{ echoUart };
    };
}

#endif
