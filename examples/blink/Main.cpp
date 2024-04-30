#include "hal/interfaces/Gpio.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
// #include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "services/util/DebugLed.hpp"
#include "services/util/GpioPinInverted.hpp"

#if defined(STM32WBA)
unsigned int hse_value = 32000000;
#else
unsigned int hse_value = 8000000;
#endif

int main()
{
    HAL_Init();

    // Configure your clock here
    // ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;

    static main_::NUCLEO ui;
    static services::DebugLed debugLed(ui.ledGreen);

#if defined(STM32WBA) | defined(STM32WB)
    services::GpioPinInverted ledRed(ui.ledRed);
    services::GpioPinInverted ledBlue(ui.ledBlue);
    ledRed.Set(false);
    ledBlue.Set(false);

    ui.buttonOne.Config(hal::PinConfigType::input);
    ui.buttonTwo.EnableInterrupt([&ledRed]()
        {
            ledRed.GetOutputLatch() == true ? ledRed.Set(false) : ledRed.Set(true);
        },
        hal::InterruptTrigger::fallingEdge);

    ui.buttonTwo.Config(hal::PinConfigType::input);
    ui.buttonOne.EnableInterrupt([&ledBlue]()
        {
            ledBlue.GetOutputLatch() == true ? ledBlue.Set(false) : ledBlue.Set(true);
        },
        hal::InterruptTrigger::risingEdge);
#endif

    eventInfrastructure.Run();
    __builtin_unreachable();
}
