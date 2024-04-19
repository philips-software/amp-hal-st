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
    static hal::DmaStm dmaStm;

#if defined(STM32G0) || defined(STM32G4)
    static hal::GpioPinStm stLinkUartTxPin{ hal::Port::A, 2 };
    static hal::GpioPinStm stLinkUartRxPin{ hal::Port::A, 3 };
    static hal::DmaStm::TransmitStream transmitStream{ dmaStm, hal::DmaChannelId{ 1, 1, DMA_REQUEST_USART2_TX } };
    static hal::UartStmDma stLinkUartDma{ transmitStream, 2, stLinkUartTxPin, stLinkUartRxPin };
#endif

#if defined(STM32WB)
    static hal::GpioPinStm stLinkUartTxPin{ hal::Port::B, 6 };
    static hal::GpioPinStm stLinkUartRxPin{ hal::Port::B, 7 };
    static hal::DmaStm::TransmitStream transmitStream{ dmaStm, hal::DmaChannelId{ 1, 1, DMA_REQUEST_USART1_TX } };
    static hal::UartStmDma stLinkUartDma{ transmitStream, 1, stLinkUartTxPin, stLinkUartRxPin };
#endif

    static services::StreamWriterOnSerialCommunication::WithStorage<64> streamWriterOnSerialCommunication{ stLinkUartDma };
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
