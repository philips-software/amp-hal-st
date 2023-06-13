#include "generated/echo/Testing.pb.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "integration_test/logic/Gpio.hpp"
#include "integration_test/instantiations_st/EchoOnSesame.hpp"
#include "services/util/DebugLed.hpp"
#include "services/util/GpioPinInverted.hpp"

unsigned int hse_value = 8000000;

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::Nucleo144Ui ui;
    static services::DebugLed debugLed(ui.ledBlue);

    static hal::GpioPinStm inPin(hal::Port::F, 7);
    static hal::GpioPinStm outPin(hal::Port::F, 8);

    static main_::EchoBetweenTesterAndTested echo;

    application::GpioTested gpioTested(echo.echo, inPin, outPin);

    eventInfrastructure.Run();
    __builtin_unreachable();
}
