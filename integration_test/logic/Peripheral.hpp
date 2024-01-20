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
        void Reset();

    private:
        infra::SharedPtr<void> currentPeripheral;
    };

    template<class Constructed, class... Args>
    class Peripheral
        : public PeripheralBase
    {
    public:
        Peripheral(Peripherals& subject, testing::Peripheral type, Args&&... args);

        infra::SharedPtr<void> Construct() override;

    private:
        std::tuple<services::Echo&, Args...> args;
        infra::SharedOptional<Constructed> constructed;
    };

    //// Implementation ////

    template<class Constructed, class... Args>
    Peripheral<Constructed, Args...>::Peripheral(Peripherals& subject, testing::Peripheral type, Args&&... args)
        : PeripheralBase(subject, type)
        , args(subject.GetEcho(), std::forward<Args>(args)...)
    {}

    template<class Constructed, class... Args>
    infra::SharedPtr<void> Peripheral<Constructed, Args...>::Construct()
    {
        return std::apply(&infra::SharedOptional<Constructed>::template Emplace<services::Echo&, Args...>, std::tuple_cat(std::tuple<infra::SharedOptional<Constructed>&>(constructed), args));
    }
}

#endif
