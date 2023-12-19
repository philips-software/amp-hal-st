#include "hal/interfaces/Gpio.hpp"
#include "hal_st/instantiations/NucleoUi.hpp"
#include "hal_st/instantiations/StmEventInfrastructure.hpp"
#include "infra/timer/Timer.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/tracer/StreamWriterOnSerialCommunication.hpp"
#include "services/tracer/Tracer.hpp"
#include "services/tracer/TracerWithDateTime.hpp"
#include "services/util/DebugLed.hpp"
#include <array>
#include <chrono>

unsigned int hse_value = 24'000'000;

int main()
{
    HAL_Init();

    static main_::StmEventInfrastructure eventInfrastructure;
    static main_::NUCLEO ui;
    static services::DebugLed debugLed(ui.ledGreen);

    static services::StreamWriterOnSerialCommunication::WithStorage<64> streamWriterOnSerialCommunication{ ui.stLinkUartDma };
    static infra::TextOutputStream::WithErrorPolicy textOutputStream{ streamWriterOnSerialCommunication };
    static services::TracerWithDateTime tracerWithDateTime{ textOutputStream };

    services::SetGlobalTracerInstance(tracerWithDateTime);

    static infra::TimerRepeating timerRepeating{ std::chrono::seconds{ 1 }, []
        {
            services::GlobalTracer().Trace() << "Hello World !";
        } };

    eventInfrastructure.Run();
    __builtin_unreachable();
}