#include "integration_test/logic/Tester.hpp"

namespace application
{
    Tester::Tester(services::Echo& echo, hal::GpioPin& resetTesterPin)
        : testing::Tester(echo)
        , resetTester(resetTesterPin, true)
    {}

    void Tester::Reset()
    {
        resetTester.Set(false);

        resetTimer.Start(std::chrono::milliseconds(3000), [this]()
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

    services::Echo& Tester::GetEcho() const
    {
        return testing::Tester::Subject();
    }
}
