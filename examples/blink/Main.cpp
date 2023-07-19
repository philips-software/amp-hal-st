#include "hal/interfaces/Gpio.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
// #include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "services/util/DebugLed.hpp"
#include <array>

unsigned int hse_value = 8000000;

int main()
{
    HAL_Init();

    // Configure your clock here
    // ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NUCLEO ui;
    static services::DebugLed debugLed(ui.ledGreen);

    eventInfrastructure.Run();
    __builtin_unreachable();
}
