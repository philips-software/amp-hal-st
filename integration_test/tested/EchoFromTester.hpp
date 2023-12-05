#ifndef HAL_ST_INTEGRATION_TEST_ECHO_FROM_TESTER_HPP
#define HAL_ST_INTEGRATION_TEST_ECHO_FROM_TESTER_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "services/util/EchoInstantiation.hpp"

namespace main_
{
    struct EchoFromTester
    {
        hal::GpioPinStm echoUartTx{ hal::Port::C, 12 };
        hal::GpioPinStm echoUartRx{ hal::Port::D, 2 };
        hal::UartStm echoUart{ 5, echoUartTx, echoUartRx };
        services::MethodSerializerFactory ::ForServices<testing::Tester, testing::Tested, testing::GpioObserverProxy, testing::GpioTester, testing::GpioTested>::AndProxies<testing::TesterProxy, testing::TestedProxy, testing::GpioObserver, testing::GpioTesterProxy, testing::GpioTestedProxy> serializerFactory;
        main_::EchoOnSerialCommunication<256> echo{ echoUart, serializerFactory };
    };
}

#endif
