#ifndef HAL_ST_DTM_ST_HPP
#define HAL_ST_DTM_ST_HPP

#include "hal/interfaces/BleDtm.hpp"
#include "infra/util/Optional.hpp"
#include <cstdint>

namespace hal
{
    class BleDtmSt
        : public BleDtm
    {
    public:
        // Implementation of BleDtm
        bool StartTone(uint8_t rfChannel, uint8_t offset) override;
        bool StopTone() override;
        bool SetTxPowerLevel(uint8_t txPower) override;
        bool StartRxTest(uint8_t frequency, uint8_t phy) override;
        bool StartTxTest(uint8_t frequency, uint8_t dataLength, uint8_t packetPayload, uint8_t phy) override;
        std::optional<uint16_t> StopTest() override;

    private:
        struct ParameterLimits
        {
            uint8_t min;
            uint8_t max;
        };

        static constexpr ParameterLimits rfChannelLimits{ 0x00, 0x27 };
        static constexpr ParameterLimits rfOffsetLimits{ 0x00, 0xff };
        static constexpr ParameterLimits txPowerLimits{ 0x00, 0x23 };
        static constexpr ParameterLimits payLoadLimits{ 0x00, 0x07 };
        static constexpr ParameterLimits frequencyLimits{ 0x00, 0x27 };
        static constexpr ParameterLimits phyLimits{ 0x01, 0x04 };
    };
}

#endif
