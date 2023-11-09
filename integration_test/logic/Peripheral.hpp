#ifndef HAL_ST_INTEGRATION_TEST_PERIPHERAL_HPP
#define HAL_ST_INTEGRATION_TEST_PERIPHERAL_HPP

#include "generated/echo/Testing.pb.hpp"
#include "infra/util/Observer.hpp"
#include "infra/util/SharedOptional.hpp"

namespace application
{
    class Peripherals;

    class PeripheralBase
        : public infra::Observer<PeripheralBase, Peripherals>
    {
    public:
        PeripheralBase(Peripherals& subject, testing::Peripheral type);

        testing::Peripheral Type() const;
        virtual infra::SharedPtr<void> Construct() = 0;

    private:
        testing::Peripheral type;
    };

    class Peripherals
        : public infra::Subject<PeripheralBase>
    {
    public:
        virtual services::Echo& GetEcho() const = 0;

        void EnablePeripheral(testing::Peripheral type);

    private:
        infra::SharedPtr<void> currentPeripheral;
    };

    template<class Constructed>
    class Perpipheral
        : public PeripheralBase
    {
    public:
        Perpipheral(Peripherals& subject, testing::Peripheral type);

        infra::SharedPtr<void> Construct() override;

    private:
        services::Echo& echo;
        infra::SharedOptional<Constructed> constructed;
    };

    //// Implementation ////

    template<class Constructed>
    Perpipheral<Constructed>::Perpipheral(Peripherals& subject, testing::Peripheral type)
        : PeripheralBase(subject, type)
        , echo(subject.GetEcho())
    {}

    template<class Constructed>
    infra::SharedPtr<void> Perpipheral<Constructed>::Construct()
    {
        return constructed.Emplace(echo);
    }
}

#endif
