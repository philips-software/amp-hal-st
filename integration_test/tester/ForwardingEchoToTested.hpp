#ifndef HAL_ST_INTEGRATION_TEST_FORWARDING_ECHO_TO_TESTED_HPP
#define HAL_ST_INTEGRATION_TEST_FORWARDING_ECHO_TO_TESTED_HPP

#include "generated/echo/Testing.pb.hpp"
#include "generated/echo/TracingTesting.pb.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "protobuf/echo/ServiceForwarder.hpp"
#include "services/tracer/GlobalTracer.hpp"
#include "services/tracer/TracingEchoInstantiation.hpp"

namespace main_
{
    struct EchoToTested
    {
        EchoToTested(hal::DmaStm& dma, services::Tracer& tracer)
            : echoUart(dma, 5, echoUartTx, echoUartRx)
            , blueTracer(services::TracerColoured::blue, tracer)
        {}

        hal::GpioPinStm echoUartTx{ hal::Port::C, 12 };
        hal::GpioPinStm echoUartRx{ hal::Port::D, 2 };
        hal::UartStmDuplexDma::WithRxBuffer<256> echoUart;
        services::MethodSerializerFactory ::ForServices<testing::Tester, testing::Tested, testing::GpioObserverProxy, testing::GpioTester, testing::GpioTested>::AndProxies<testing::TesterProxy, testing::TestedProxy, testing::GpioObserver, testing::GpioTesterProxy, testing::GpioTestedProxy> serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedEchoUart{ echoUart };
        services::TracerColoured blueTracer;
        main_::TracingEchoOnSesame<256> echo{ bufferedEchoUart, serializerFactory, blueTracer };

        testing::TestedTracer testedTracer{ echo.echo };
        testing::TestedObserverTracer testedObserverTracer{ echo.echo };
        testing::GpioTestedTracer gpiotestedTracer{ echo.echo };
        testing::GpioObserverTracer gpioObserverTracer{ echo.echo };
    };

    struct ForwardingEchoToTested
    {
        ForwardingEchoToTested(services::Echo& echo, hal::DmaStm& dma, services::Tracer& tracer);

        main_::EchoToTested echoToTested;
        services::ServiceForwarder forwardTested;
        services::ServiceForwarder forwardTestedObserver;
        services::ServiceForwarder forwardGpioTested;
        services::ServiceForwarder forwardGpioObserver;
    };
}

#endif
