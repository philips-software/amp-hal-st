#include "hal_st/instantiations/NucleoTracerInfrastructure.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "integration_test/tested/EchoFromTester.hpp"
#include "integration_test/tested/Tested.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/util/DebugLed.hpp"

unsigned int hse_value = 8000000;

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NucleoF767ziTracerInfrastructure tracerInfrastructure;
    services::SetGlobalTracerInstance(tracerInfrastructure.tracer);
    static main_::Nucleo144Ui ui;
    static services::DebugLed debugLed(ui.ledBlue);

    static hal::DmaStm dma;
    static main_::EchoFromTester echo(dma);
    static main_::Tested tested(echo.echo, dma);

    services::GlobalTracer().Trace() << "Starting tested!";

    eventInfrastructure.Run();
    __builtin_unreachable();
}
