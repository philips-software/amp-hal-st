#ifndef HAL_ST_INTEGRATION_TEST_TESTER_HPP
#define HAL_ST_INTEGRATION_TEST_TESTER_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "infra/timer/Timer.hpp"
#include "integration_test/logic/Peripheral.hpp"
#include "services/util/EchoOnSesame.hpp"

namespace application
{
    class Tester
        : public testing::Tester
        , public Peripherals
    {
    public:
        Tester(services::Echo& echo, hal::GpioPin& resetTesterPin, services::EchoOnSesame& echoToTested);

        // Implementation of Tester
        void Reset() override;
        void EnablePeripheral(testing::Peripheral type) override;

        // Implementation of Peripherals
        services::Echo& GetEcho() const override;

    private:
        services::EchoOnSesame& echoToTested;
        hal::OutputPin resetTester;
        infra::TimerSingleShot resetTimer;
    };
}

#endif
