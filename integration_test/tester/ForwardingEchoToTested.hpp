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
        main_::EchoOnSerialCommunication<1024> echo{ echoUart };
    };

    struct ForwardingEchoToTested
    {
        ForwardingEchoToTested(services::Echo& echo);

        main_::EchoToTested echoToTested;
        std::array<uint8_t, 1024> buffer;
        services::ServiceForwarder forwardTested;
        services::ServiceForwarder forwardGpioTested;
        services::ServiceForwarder forwardGpioObserver;
    };
}

#endif
