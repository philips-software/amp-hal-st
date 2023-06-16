#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "integration_test/tested/EchoFromTester.hpp"
#include "integration_test/tested/Tested.hpp"
#include "services/util/DebugLed.hpp"

unsigned int hse_value = 8000000;

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
