#ifndef HAL_ST_INTEGRATION_TEST_TESTER_HPP
#define HAL_ST_INTEGRATION_TEST_TESTER_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/ProxyCreator.hpp"
#include "integration_test/logic/Flash.hpp"
#include "integration_test/logic/Peripheral.hpp"
#include "services/util/EchoOnSesame.hpp"

namespace application
{
    class Tester
        : public testing::Tester
        , public Peripherals
    {
    public:
        using FlashTestedCreator = infra::CreatorBase<void, void(services::Echo& echo, const infra::Function<void(infra::BoundedConstString result)>& onDone)>;
        using EchoToTestedCreator = infra::CreatorBase<services::EchoOnSesame, void()>;

        Tester(services::Echo& echo, hal::GpioPin& resetTestedPin, EchoToTestedCreator& echoToTestedCreator, FlashTestedCreator& flashTestedCreator);

        // Implementation of Tester
        void SetTestedMode(testing::TestedMode mode) override;
        void EnablePeripheral(testing::Peripheral type) override;

        // Implementation of Peripherals
        services::Echo& GetEcho() const override;

    private:
        EchoToTestedCreator& echoToTestedCreator;
        infra::Optional<infra::ProxyCreator<EchoToTestedCreator&>> echoToTested;
        hal::OutputPin resetTested;
        testing::TesterObserverProxy testerObserverProxy;
        FlashTestedCreator& flashTestedCreator;
        infra::Optional<infra::ProxyCreator<FlashTestedCreator&>> flash;
        infra::BoundedConstString result;
    };

    class TesterObserver
        : public testing::TesterObserver
    {
    public:
        TesterObserver(services::Echo& echo);

        // Implementation of TesterObserver
        void TestedModeSet(infra::BoundedConstString message) override;

        bool ReceivedTestedModeSet();

    private:
        bool receivedTestedModeSet = false;
    };
}

#endif
