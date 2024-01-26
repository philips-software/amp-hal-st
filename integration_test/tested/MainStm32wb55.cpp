#include "hal_st/instantiations/NucleoTracerInfrastructure.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoWB55RG.hpp"
#include "integration_test/tested/EchoFromTester.hpp"
#include "integration_test/tested/Tested.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/util/DebugLed.hpp"

unsigned int hse_value = 32000000;

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleoWB55RG();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NucleoWb55rgTracerInfrastructure tracerInfrastructure;
    services::SetGlobalTracerInstance(tracerInfrastructure.tracer);
    static main_::Nucleo64WBUi ui;
    static services::DebugLed debugLed(ui.ledBlue);

    static hal::DmaStm dma;
    static main_::EchoFromTester echo(dma);
    static main_::Tested tested(echo.echo, dma);

    services::GlobalTracer().Trace() << "Starting tested!";

    eventInfrastructure.Run();
    __builtin_unreachable();
}
