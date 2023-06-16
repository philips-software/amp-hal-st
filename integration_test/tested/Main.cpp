#include "generated/echo/Testing.pb.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "integration_test/logic/Gpio.hpp"
#include "integration_test/logic/Tested.hpp"
#include "integration_test/instantiations_st/EchoOnSesame.hpp"
#include "services/util/DebugLed.hpp"

unsigned int hse_value = 8000000;

namespace main_
{
    struct GpioTested
    {
        GpioTested(services::Echo& echo);

        hal::GpioPinStm inPin{ hal::Port::F, 7 };
        hal::GpioPinStm outPin{ hal::Port::F, 8 };
        application::GpioTested gpioTested;
    };

    GpioTested::GpioTested(services::Echo& echo)
        : gpioTested(echo, inPin, outPin)
    {}

    struct Tested
    {
        Tested(services::Echo& echo);

        application::Tested tested;
        application::Perpipheral<main_::GpioTested> gpioTested{ tested, testing::Peripheral::gpio };
    };

    Tested::Tested(services::Echo& echo)
        : tested(echo)
    {}
}

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::Nucleo144Ui ui;
    static services::DebugLed debugLed(ui.ledBlue);

    static main_::EchoFromTester echo;
    static main_::Tested tested(echo.echo);

    eventInfrastructure.Run();
    __builtin_unreachable();
}
