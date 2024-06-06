#include "integration_test/logic/Tester.hpp"

namespace application
{
    Tester::Tester(services::Echo& echo, hal::GpioPin& resetTesterPin, services::EchoOnSesame& echoToTested)
        : testing::Tester(echo)
        , echoToTested(echoToTested)
        , resetTester(resetTesterPin, true)
        , proxy(echo)
    {}

    void Tester::Reset()
    {
        resetTester.Set(false);

        Peripherals::Reset();
        echoToTested.Reset();

        resetTimer.Start(std::chrono::milliseconds(10), [this]()
            {
                resetTester.Set(true);
                MethodDone();
            });
    }

    void Tester::EnablePeripheral(testing::Peripheral type)
    {
        Peripherals::EnablePeripheral(type);

        MethodDone();
    }

    void Tester::Ping()
    {
        proxy.RequestSend([this]()
            {
                proxy.Ping();
                MethodDone();
            });
    }

    services::Echo& Tester::GetEcho() const
    {
        return testing::Tester::Subject();
    }
}
