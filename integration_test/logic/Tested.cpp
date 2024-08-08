#include "integration_test/logic/Tested.hpp"

namespace application
{
    Tested::Tested(services::Echo& echo)
        : testing::Tested(echo)
        , testedObserver(echo)
    {}

    void Tested::Ping()
    {
        testedObserver.RequestSend([this]()
            {
                testedObserver.Pong();
                MethodDone();
            });
    }

    void Tested::EnablePeripheral(testing::Peripheral type)
    {
        Peripherals::EnablePeripheral(type);

        MethodDone();
    }

    services::Echo& Tested::GetEcho() const
    {
        return testing::Tested::Subject();
    }
}
