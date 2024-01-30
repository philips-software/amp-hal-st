#ifndef HAL_ST_INTEGRATION_TEST_TESTER_TESTER_HPP
#define HAL_ST_INTEGRATION_TEST_TESTER_TESTER_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "integration_test/logic/Gpio.hpp"
#include "integration_test/logic/Tester.hpp"
#include "integration_test/logic/Uart.hpp"

namespace main_
{
    struct GpioTester
    {
        GpioTester(services::Echo& echo);

        hal::GpioPinStm inPin{ hal::Port::F, 8 };
        hal::GpioPinStm outPin{ hal::Port::F, 7 };
        application::GpioTester gpioTester;
    };

    struct UartTester
    {
        UartTester(services::Echo& echo);

        hal::GpioPinStm tx{ hal::Port::D, 5 };
        hal::GpioPinStm rx{ hal::Port::D, 6 };
        hal::UartStm uart{ 2, tx, rx };
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<32> bufferedUart{ uart };
        application::UartTester uartTester;
    };

    struct UartDuplexDmaTester
    {
        UartDuplexDmaTester(services::Echo& echo, hal::DmaStm& dma);

        hal::GpioPinStm tx{ hal::Port::D, 5 };
        hal::GpioPinStm rx{ hal::Port::D, 6 };
        hal::DmaStm::TransmitStream transmitStream;
        hal::DmaStm::ReceiveStream receiveStream;
        hal::UartStmDuplexDma::WithRxBuffer<32> uart{ transmitStream, receiveStream, 2, tx, rx };
        hal::BufferedSerialCommunicationOnUnbuffered::WithStorage<32> bufferedUart{ uart };
        application::UartTester uartTester;
    };

    struct Tester
    {
        Tester(services::Echo& echo, services::EchoOnSesame& echoToTested, hal::DmaStm& dma);

        hal::GpioPinStm nResetTester{ hal::Port::E, 6, hal::Drive::OpenDrain };
        application::Tester tester;
        application::Peripheral<main_::GpioTester> gpioTester{ tester, testing::Peripheral::gpio };
        application::Peripheral<main_::UartTester> uartTester{ tester, testing::Peripheral::uart };
        application::Peripheral<main_::UartDuplexDmaTester, hal::DmaStm&> uartDuplexDmaTester;
    };
}

#endif
