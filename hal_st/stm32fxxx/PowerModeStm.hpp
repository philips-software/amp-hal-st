#ifndef HAL_POWER_MODE_STM_HPP
#define HAL_POWER_MODE_STM_HPP

#include "hal/interfaces/PowerMode.hpp"
#include <cstdint>
#include DEVICE_HEADER

namespace hal
{
    class PowerModeStm
        : public PowerMode
    {
    public:
        struct Config
        {
            constexpr Config()
            {}

            bool enableUltraLowPowerMode{ true };
            bool disableDebugInLowPowerMode{ true };
        };

        explicit PowerModeStm(uint32_t wakeUpPin, const Config& config = Config());

        bool EnterStandby() override;

    private:
        uint32_t wakeUpPin;
        Config config;
    };
}

#endif
