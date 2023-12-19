#ifndef HAL_ST_INTEGRATION_TEST_TESTED_HPP
#define HAL_ST_INTEGRATION_TEST_TESTED_HPP

#include "generated/echo/Testing.pb.hpp"
#include "hal/interfaces/Gpio.hpp"
#include "infra/timer/Timer.hpp"
#include "integration_test/logic/Peripheral.hpp"

namespace application
{
    class Tested
        : public testing::Tested
        , public Peripherals
    {
    public:
        Tested(services::Echo& echo);

        // Implementation of Tested
        void EnablePeripheral(testing::Peripheral type) override;

        // Implementation of Peripherals
        services::Echo& GetEcho() const override;
    };
}

#endif
