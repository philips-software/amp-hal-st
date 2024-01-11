#ifndef HAL_ST_INTEGRATION_TEST_TESTER_TESTER_HPP
#define HAL_ST_INTEGRATION_TEST_TESTER_TESTER_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "integration_test/logic/Gpio.hpp"
#include "integration_test/logic/Tester.hpp"

namespace main_
{
    struct GpioTester
    {
        GpioTester(services::Echo& echo);

        hal::GpioPinStm inPin{ hal::Port::F, 8 };
        hal::GpioPinStm outPin{ hal::Port::F, 7 };
        application::GpioTester gpioTester;
    };

    struct Tester
    {
        Tester(services::Echo& echo, services::EchoOnSesame& echoToTested);

        hal::GpioPinStm nResetTester{ hal::Port::E, 6, hal::Drive::OpenDrain };
        application::Tester tester;
        application::Perpipheral<main_::GpioTester> gpioTester{ tester, testing::Peripheral::gpio };
    };
}

#endif
