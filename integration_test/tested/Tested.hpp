#ifndef HAL_ST_INTEGRATION_TEST_TESTED_TESTED_HPP
#define HAL_ST_INTEGRATION_TEST_TESTED_TESTED_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "integration_test/logic/Gpio.hpp"
#include "integration_test/logic/Tested.hpp"
#include "integration_test/logic/Uart.hpp"

namespace main_
{
    struct GpioTested
    {
        GpioTested(services::Echo& echo);

        hal::GpioPinStm inPin{ hal::Port::F, 7 };
        hal::GpioPinStm outPin{ hal::Port::F, 8 };
        application::GpioTested gpioTested;
    };

    struct UartTested
    {
        UartTested(services::Echo& echo);

        hal::GpioPinStm tx{ hal::Port::D, 5 };
        hal::GpioPinStm rx{ hal::Port::D, 6 };
        hal::UartStm uart{ 2, tx, rx };
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<32> bufferedUart{ uart };
        application::UartTested uartTested;
    };

    struct UartDuplexDmaTested
    {
        UartDuplexDmaTested(services::Echo& echo, hal::DmaStm& dma);

        hal::GpioPinStm tx{ hal::Port::D, 5 };
        hal::GpioPinStm rx{ hal::Port::D, 6 };
        hal::DmaStm::TransmitStream transmitStream;
        hal::DmaStm::ReceiveStream receiveStream;
        hal::UartStmDuplexDma::WithRxBuffer<32> uart{ transmitStream, receiveStream, 2, tx, rx };
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<32> bufferedUart{ uart };
        application::UartTested uartTested;
    };

    struct Tested
    {
        Tested(services::Echo& echo, hal::DmaStm& dma);

        application::Tested tested;
        application::Peripheral<main_::GpioTested> gpioTested{ tested, testing::Peripheral::gpio };
        application::Peripheral<main_::UartTested> uartTested{ tested, testing::Peripheral::uart };
        application::Peripheral<main_::UartDuplexDmaTested, hal::DmaStm&> uartDuplexDmaTested;
    };
}

#endif
