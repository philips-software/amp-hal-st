#ifndef HAL_ST_INTEGRATION_TEST_ECHO_FROM_TESTER_HPP
#define HAL_ST_INTEGRATION_TEST_ECHO_FROM_TESTER_HPP

#include "generated/echo/Testing.pb.hpp"
#include "generated/echo/TracingTesting.pb.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/tracer/TracingEchoInstantiation.hpp"

namespace main_
{
    struct EchoFromTester
    {
        EchoFromTester(hal::DmaStm& dma)
            : transmitStream{ dma, hal::DmaChannelId{ 1, 7, 4 } }
            , receiveStream{ dma, hal::DmaChannelId{ 1, 0, 4 } }
        {}

        hal::GpioPinStm echoUartTx{ hal::Port::C, 12 };
        hal::GpioPinStm echoUartRx{ hal::Port::D, 2 };
        hal::DmaStm::TransmitStream transmitStream;
        hal::DmaStm::ReceiveStream receiveStream;
        hal::UartStmDuplexDma::WithRxBuffer<256> echoUart{ transmitStream, receiveStream, 5, echoUartTx, echoUartRx };
        services::MethodSerializerFactory::ForServices<testing::Tester, testing::Tested, testing::GpioObserverProxy, testing::GpioTester, testing::GpioTested>::AndProxies<testing::TesterProxy, testing::TestedProxy, testing::GpioObserver, testing::GpioTesterProxy, testing::GpioTestedProxy> serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedEchoUart{ echoUart };
        main_::TracingEchoOnSesame<256> echo{ bufferedEchoUart, serializerFactory, services::GlobalTracer() };

        testing::TestedTracer testedTracer{ echo.echo };
        testing::TestedObserverTracer testedObserverTracer{ echo.echo };
        testing::GpioTestedTracer gpioTestedTracer{ echo.echo };
        testing::GpioObserverTracer gpioObserverTracer{ echo.echo };
        testing::UartTestedTracer uartTestedTracer{ echo.echo };
        testing::UartObserverTracer uartObserverTracer{ echo.echo };
    };
}

#endif
