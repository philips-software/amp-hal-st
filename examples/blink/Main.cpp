#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "services/util/DebugLed.hpp"

extern "C" void Default_Handler()
{
    hal::InterruptTable::Instance().Invoke(hal::ActiveInterrupt());
}

int main()
{
    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NucleoF767ziUi ui;
    static services::DebugLed debugLed(ui.ledBlue);

    eventInfrastructure.Run();
    __builtin_unreachable();
}
