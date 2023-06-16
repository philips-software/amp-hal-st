#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "integration_test/tester/EchoFromCloud.hpp"
#include "integration_test/tester/ForwardingEchoToTested.hpp"
#include "integration_test/tester/Tester.hpp"
#include "services/util/DebugLed.hpp"

// Tester    Tested Function
// ------    ------ --------
// CN8-9  -> CN8-9  E5V (NOTE: set JP3 on the Stim to E5V)
// CN8-11 -- CN8-11 ground
// CN8-10 <- CN8-12 ECHO on SESAME
// CN8-12 -> CN8-10 ECHO on SESAME
// CN9-20 -> CN8-5  nRst of Stim
// CN9-24 <- CN9-24 GPIO set by Tester
// CN9-26 -> CN9-26 Tester GPIO control

// Connect debug session to GDB, bring up with:
// JLinkGDBServer -select USB -device STM32F767ZI -endian little -if JTAG -speed auto -noir -LocalhostOnly -nologtofile

unsigned int hse_value = 8000000;

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::Nucleo144Ui ui;
    static services::DebugLed debugLed(ui.ledBlue);

    static main_::EchoFromCloud echo;
    static main_::Tester tester(echo.echo);
    static main_::ForwardingEchoToTested forwarder(echo.echo);

    eventInfrastructure.Run();
    __builtin_unreachable();
}
