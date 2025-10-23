#include "integration_test/tester/Tester.hpp"

namespace main_
{
    GpioTester::GpioTester(services::Echo& echo)
        : gpioTester{ echo, inPin, outPin }
    {}

    UartTester::UartTester(services::Echo& echo)
        : uartTester{ echo, bufferedUart }
    {}

    UartDuplexDmaTester::UartDuplexDmaTester(services::Echo& echo, hal::DmaStm& dma)
        : uartTester{ echo, bufferedUart }
        , transmitStream{ dma, hal::DmaChannelId{ 1, 6, 4 } }
        , receiveStream{ dma, hal::DmaChannelId{ 1, 5, 4 } }
    {}

    FlashTested::FlashTested(services::Echo& echo, hal::DmaStm& dma, const infra::Function<void(infra::BoundedConstString result)>& onDone)
        : onDone(onDone)
        , transmitStream{ dma, hal::DmaChannelId{ 2, 7, 5 } }
        , receiveStream{ dma, hal::DmaChannelId{ 2, 1, 5 } }
        , flashOverEcho(echo, flashProgrammer)
    {}

    Tester::Tester(services::Echo& echo, application::Tester::EchoToTestedCreator& echoToTestedCreator, hal::DmaStm& dma)
        : dma(dma)
        , flashTestedCreator([this](std::optional<FlashTested>& value, services::Echo& echo, const infra::Function<void(infra::BoundedConstString result)>& onDone)
              {
                  value.emplace(echo, this->dma, onDone);
              })
        , tester(echo, nResetTested, boot0Tested, echoToTestedCreator, flashTestedCreator)
        , uartDuplexDmaTester{ tester, testing::Peripheral::uartDuplexDma, dma }
    {}
}
