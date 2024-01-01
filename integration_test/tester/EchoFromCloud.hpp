#ifndef HAL_ST_INTEGRATION_TEST_ECHO_FROM_CLOUD_HPP
#define HAL_ST_INTEGRATION_TEST_ECHO_FROM_CLOUD_HPP

#include "generated/echo/Testing.pb.hpp"
#include "services/util/EchoInstantiation.hpp"
#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"

namespace main_
{
    struct EchoFromCloud
    {
        EchoFromCloud(hal::DmaStm& dma)
            : hostUart(dma, 4, hostUartTxPin, hostUartRxPin)
        {}

        hal::GpioPinStm hostUartTxPin{ hal::Port::C, 10 };
        hal::GpioPinStm hostUartRxPin{ hal::Port::C, 11 };
        hal::DmaStm dma;
        hal::UartStmDuplexDma::WithRxBuffer<256> hostUart;
        services::MethodSerializerFactory ::ForServices<testing::Tester, testing::Tested, testing::GpioObserverProxy, testing::GpioTester, testing::GpioTested>::AndProxies<testing::TesterProxy, testing::TestedProxy, testing::GpioObserver, testing::GpioTesterProxy, testing::GpioTestedProxy> serializerFactory;
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<256> bufferedHostUart{ hostUart };
        main_::EchoOnSesame<256> echo{ bufferedHostUart, serializerFactory };
    };
}

#endif
