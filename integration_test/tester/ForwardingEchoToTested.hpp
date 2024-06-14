#ifndef HAL_ST_INTEGRATION_TEST_FORWARDING_ECHO_TO_TESTED_HPP
#define HAL_ST_INTEGRATION_TEST_FORWARDING_ECHO_TO_TESTED_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "protobuf/echo/ServiceForwarder.hpp"
#include "services/util/EchoInstantiation.hpp"

namespace main_
{
    struct EchoToTested
    {
        hal::GpioPinStm echoUartTx{ hal::Port::C, 12 };
        hal::GpioPinStm echoUartRx{ hal::Port::D, 2 };
        hal::UartStm echoUart{ 5, echoUartTx, echoUartRx };
        services::MethodSerializerFactory ::ForServices<testing::Tester, testing::Tested, testing::GpioObserverProxy, testing::GpioTester, testing::GpioTested>::AndProxies<testing::TesterProxy, testing::TestedProxy, testing::GpioObserver, testing::GpioTesterProxy, testing::GpioTestedProxy> serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedEchoUart{ echoUart };
        main_::EchoOnSesame<256> echo{ bufferedEchoUart, serializerFactory };
    };

    struct ForwardingEchoToTested
    {
        ForwardingEchoToTested(services::Echo& echo);

        main_::EchoToTested echoToTested;
        services::ServiceForwarder forwardTested;
        services::ServiceForwarder forwardGpioTested;
        services::ServiceForwarder forwardGpioObserver;
    };
}

#endif
