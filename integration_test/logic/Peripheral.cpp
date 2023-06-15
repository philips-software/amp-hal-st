#include "integration_test/logic/Peripheral.hpp"

namespace application
{
    PeripheralBase::PeripheralBase(Peripherals& subject, testing::Peripheral type)
        : infra::Observer<PeripheralBase, Peripherals>(subject)
        , type(type)
    {}

    testing::Peripheral PeripheralBase::Type() const
    {
        return type;
    }

    void Peripherals::EnablePeripheral(testing::Peripheral type)
    {
        currentPeripheral = nullptr;

        NotifyObservers([this, type](auto& peripheral)
            {
                if (peripheral.Type() == type)
                {
                    currentPeripheral = peripheral.Construct();
                    return true;
                }

                return false;
            });
    }
}
