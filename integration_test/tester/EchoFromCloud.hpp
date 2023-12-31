#ifndef HAL_ST_INTEGRATION_TEST_ECHO_FROM_CLOUD_HPP
#define HAL_ST_INTEGRATION_TEST_ECHO_FROM_CLOUD_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"
#include "services/util/EchoInstantiation.hpp"

namespace main_
{
    struct EchoFromCloud
    {
        hal::GpioPinStm hostUartTxPin{ hal::Port::C, 10 };
        hal::GpioPinStm hostUartRxPin{ hal::Port::C, 11 };
        hal::DmaStm dma;
        hal::UartStmDma hostUart{ dma, 4, hostUartTxPin, hostUartRxPin };
        services::MethodSerializerFactory ::ForServices<testing::Tester, testing::Tested, testing::GpioObserverProxy, testing::GpioTester, testing::GpioTested>::AndProxies<testing::TesterProxy, testing::TestedProxy, testing::GpioObserver, testing::GpioTesterProxy, testing::GpioTestedProxy> serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedHostUart{ hostUart };
        main_::EchoOnSesame<256> echo{ bufferedHostUart, serializerFactory };
    };
}

#endif
