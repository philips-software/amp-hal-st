#include "hal_st/instantiations/NucleoTracerInfrastructure.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "hal_st/stm32fxxx/DefaultClockNucleoF767ZI.hpp"
#include "integration_test/tester/EchoFromCloud.hpp"
#include "integration_test/tester/ForwardingEchoToTested.hpp"
#include "integration_test/tester/Tester.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/util/DebugLed.hpp"

// Tester    Tested Function
// ------    ------ --------
// CN8-9  -> CN8-9  E5V (NOTE: set JP3 on both the Tester and Tested to E5V)
// CN8-11 -- CN8-11 ground
// CN8-10 <- CN8-12 ECHO on SESAME
// CN8-12 -> CN8-10 ECHO on SESAME
// CN9-20 -> CN8-5  nRst of Tested
// CN9-24 <- CN9-24 GPIO set by Tester
// CN9-26 -> CN9-26 Tester GPIO control

unsigned int hse_value = 8000000;

int main()
{
    HAL_Init();
    ConfigureDefaultClockNucleo767ZI();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NucleoF767ziTracerInfrastructure tracerInfrastructure;
    services::TracerColoured tracer{ services::TracerColoured::resetColour, tracerInfrastructure.tracer };
    services::SetGlobalTracerInstance(tracer);
    static main_::Nucleo144Ui ui;
    static services::DebugLed debugLed(ui.ledBlue);

    static hal::DmaStm dma;
    static main_::EchoFromCloud echo(dma, tracerInfrastructure.tracer);
    static infra::Creator<services::EchoOnSesame, main_::ForwardingEchoToTested, void()> forwarderCreator{
        [](infra::Optional<main_::ForwardingEchoToTested>& value)
        {
            value.Emplace(echo.echo.echo, dma, tracerInfrastructure.tracer);
        }
    };
    static main_::Tester tester(echo.echo.echo, forwarderCreator, dma);

    services::GlobalTracer().Trace() << "Starting tester!";

    eventInfrastructure.Run();
    __builtin_unreachable();
}
