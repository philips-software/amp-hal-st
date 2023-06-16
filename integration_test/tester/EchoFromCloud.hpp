#ifndef HAL_ST_INTEGRATION_TEST_ECHO_FROM_CLOUD_HPP
#define HAL_ST_INTEGRATION_TEST_ECHO_FROM_CLOUD_HPP

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
        main_::EchoOnSerialCommunication<60> echo{ hostUart };
    };
}

#endif
