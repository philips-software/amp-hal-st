#ifndef HAL_ST_INTEGRATION_TEST_ECHO_FROM_CLOUD_HPP
#define HAL_ST_INTEGRATION_TEST_ECHO_FROM_CLOUD_HPP

#include "generated/echo/Testing.pb.hpp"
#include "generated/echo/TracingTesting.pb.hpp"
#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/tracer/TracingEchoInstantiation.hpp"

namespace main_
{
    struct EchoFromCloud
    {
        EchoFromCloud(hal::DmaStm& dma, services::Tracer& tracer)
            : transmitStream{ dma, hal::DmaChannelId{ 1, 4, 4 } }
            , receiveStream{ dma, hal::DmaChannelId{ 1, 2, 4 } }
            , redTracer{ services::TracerColoured::red, tracer }
        {}

        hal::GpioPinStm hostUartTxPin{ hal::Port::C, 10 };
        hal::GpioPinStm hostUartRxPin{ hal::Port::C, 11 };
        hal::DmaStm::TransmitStream transmitStream;
        hal::DmaStm::ReceiveStream receiveStream;
        hal::UartStmDuplexDma::WithRxBuffer<256> hostUart{ transmitStream, receiveStream, 4, hostUartTxPin, hostUartRxPin };
        services::MethodSerializerFactory::ForServices<testing::Tester, testing::Tested, testing::GpioObserverProxy, testing::GpioTester, testing::GpioTested>::AndProxies<testing::TesterProxy, testing::TestedProxy, testing::GpioObserver, testing::GpioTesterProxy, testing::GpioTestedProxy> serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedHostUart{ hostUart };
        services::TracerColoured redTracer;
        main_::TracingEchoOnSesame<256> echo{ bufferedHostUart, serializerFactory, redTracer };

        testing::TesterTracer testerTracer{ echo.echo };
        testing::TestedTracer testedTracer{ echo.echo };
        testing::TestedObserverTracer testedObserverTracer{ echo.echo };
        testing::GpioTesterTracer gpioTesterTracer{ echo.echo };
        testing::GpioTestedTracer gpioTestedTracer{ echo.echo };
        testing::GpioObserverTracer gpioObserverTracer{ echo.echo };
        testing::UartTesterTracer uartTesterTracer{ echo.echo };
        testing::UartTestedTracer uartTestedTracer{ echo.echo };
        testing::UartObserverTracer uartObserverTracer{ echo.echo };
    };
}

#endif
