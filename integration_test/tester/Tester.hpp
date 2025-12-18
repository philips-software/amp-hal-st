#ifndef HAL_ST_INTEGRATION_TEST_TESTER_TESTER_HPP
#define HAL_ST_INTEGRATION_TEST_TESTER_TESTER_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStm.hpp"
#include "hal_st/stm32fxxx/UartStmDuplexDma.hpp"
#include "integration_test/logic/Flash.hpp"
#include "integration_test/logic/Gpio.hpp"
#include "integration_test/logic/Tester.hpp"
#include "integration_test/logic/Uart.hpp"
#include "services/st_util/FlashOnStBootloaderCommunicator.hpp"
#include "services/st_util/StBootloaderCommunicatorUart.hpp"

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

    struct FlashTested
    {
        FlashTested(services::Echo& echo, hal::DmaStm& dma, const infra::Function<void(infra::BoundedConstString result)>& onDone);

        infra::Function<void(infra::BoundedConstString result)> onDone;

        hal::GpioPinStm tx{ hal::Port::G, 14 };
        hal::GpioPinStm rx{ hal::Port::G, 9 };
        hal::DmaStm::TransmitStream transmitStream;
        hal::DmaStm::ReceiveStream receiveStream;
        hal::UartStmDuplexDma::WithRxBuffer<256> uart{ transmitStream, receiveStream, 6, tx, rx, []()
                    {
                        hal::UartStmDuplexDma::Config config;
                        config.baudrate = 57600;
                        config.parity = USART_PARITY_EVEN;
                        return config;
                    }() };

        std::array<uint32_t, 12> sectorSizes{ { 32 * 1024,
            32 * 1024,
            32 * 1024,
            32 * 1024,
            128 * 1024,
            256 * 1024,
            256 * 1024,
            256 * 1024,
            256 * 1024,
            256 * 1024,
            256 * 1024,
            256 * 1024 } };

        services::StBootloaderCommunicatorUart programmer{
            uart, [this]()
            {
                this->onDone({});
            },
            [this](infra::BoundedConstString reason)
            {
                this->onDone(reason);
            }
        };
        services::FlashHeterogeneousOnStBootloaderCommunicator flashProgrammer{ infra::MakeRange(sectorSizes), programmer };
        application::Flash flashOverEcho;
    };

    struct Tester
    {
        Tester(services::Echo& echo, application::Tester::EchoToTestedCreator& echoToTestedCreator, hal::DmaStm& dma);

        hal::DmaStm& dma;
        hal::GpioPinStm nResetTested{ hal::Port::E, 6 };
        hal::GpioPinStm boot0Tested{ hal::Port::A, 5 };
        infra::Creator<void, FlashTested, void(services::Echo&, const infra::Function<void(infra::BoundedConstString result)>& onDone)> flashTestedCreator;
        application::Tester tester;
        application::Peripheral<main_::GpioTester> gpioTester{ tester, testing::Peripheral::gpio };
        application::Peripheral<main_::UartTester> uartTester{ tester, testing::Peripheral::uart };
        application::Peripheral<main_::UartDuplexDmaTester, hal::DmaStm&> uartDuplexDmaTester;
    };
}

#endif
