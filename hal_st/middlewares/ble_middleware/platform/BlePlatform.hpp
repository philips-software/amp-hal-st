#ifndef HAL_ST_BLE_PLATFORM_HPP
#define HAL_ST_BLE_PLATFORM_HPP

#include "hal/synchronous_interfaces/SynchronousRandomDataGenerator.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "infra/timer/Timer.hpp"

namespace hal
{
    class BlePlatform
        : public infra::InterfaceConnector<BlePlatform>
    {
    public:

    private:
        infra::TimerSingleShot timer;
    };
}

#endif
