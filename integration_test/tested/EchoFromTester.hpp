#ifndef HAL_ST_INTEGRATION_TEST_ECHO_FROM_TESTER_HPP
#define HAL_ST_INTEGRATION_TEST_ECHO_FROM_TESTER_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "services/util/EchoInstantiation.hpp"

namespace main_
{
    struct EchoFromTester
    {
        hal::GpioPinStm echoUartTx{ hal::Port::C, 12 };
        hal::GpioPinStm echoUartRx{ hal::Port::D, 2 };
        hal::UartStm echoUart{ 5, echoUartTx, echoUartRx };
        main_::EchoOnSerialCommunication<1024> echo{ echoUart };
    };
}

#endif
