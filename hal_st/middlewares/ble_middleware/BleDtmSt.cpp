#include "hal_st/middlewares/ble_middleware/BleDtmSt.hpp"
#include <array>
#include <cstdint>
#include <limits>
#include <optional>

extern "C"
{
#include "ble_hal_aci.h"
#include "ble_hci_le.h"
}

namespace hal
{
    namespace
    {
        constexpr std::array<int16_t, 32> txPowerLevelsInHundredthDbm{
            -4000, -2085, -1975, -1885, -1760, -1650, -1525, -1410,
            -1315, -1205, -1090, -990, -885, -780, -690, -590,
            -495, -400, -315, -245, -180, -130, -85, -50,
            -15, 0, 100, 200, 300, 400, 500, 600
        };

        uint8_t ClosestTxPowerLevel(int8_t txPowerDbm)
        {
            const int16_t requestedPower = static_cast<int16_t>(txPowerDbm) * 100;
            uint8_t closestLevel = 0;
            int16_t smallestDifference = std::numeric_limits<int16_t>::max();

            for (uint8_t level = 0; level < txPowerLevelsInHundredthDbm.size(); ++level)
            {
                int16_t difference = requestedPower - txPowerLevelsInHundredthDbm[level];
                if (difference < 0)
                    difference = -difference;

                if (difference < smallestDifference)
                {
                    closestLevel = level;
                    smallestDifference = difference;
                }
            }

            return closestLevel;
        }
    }

    bool BleDtmSt::StartTone(uint8_t rfChannel)
    {
        if (rfChannel >= rfChannelLimits.min && rfChannel <= rfChannelLimits.max)
            return aci_hal_tone_start(rfChannel, 0) == BLE_STATUS_SUCCESS;

        return false;
    }

    bool BleDtmSt::StopTone()
    {
        return aci_hal_tone_stop() == BLE_STATUS_SUCCESS;
    }

    bool BleDtmSt::SetTxPowerLevel(int8_t txPowerDbm)
    {
        return aci_hal_set_tx_power_level(0, ClosestTxPowerLevel(txPowerDbm)) == BLE_STATUS_SUCCESS;
    }

    bool BleDtmSt::StartRxTest(uint8_t rxFrequency, uint8_t phy)
    {
        if (rxFrequency >= frequencyLimits.min && rxFrequency <= frequencyLimits.max && phy >= phyLimits.min && phy <= phyLimits.max)
            return hci_le_receiver_test_v2(rxFrequency, phy, 0) == BLE_STATUS_SUCCESS;

        return false;
    }

    bool BleDtmSt::StartTxTest(uint8_t txFrequency, uint8_t testDataLength, uint8_t packetPayload, uint8_t phy)
    {
        if (txFrequency >= frequencyLimits.min && txFrequency <= frequencyLimits.max &&
            packetPayload >= payLoadLimits.min && packetPayload <= payLoadLimits.max &&
            phy >= phyLimits.min && phy <= phyLimits.max)
            return hci_le_transmitter_test_v2(txFrequency, testDataLength, packetPayload, phy) == BLE_STATUS_SUCCESS;

        return false;
    }

    void BleDtmSt::StopTest(infra::AutoResetFunction<void(std::optional<uint16_t>)>&& onStopped)
    {
        uint16_t numberOfPackets = 0;

        if (hci_le_test_end(&numberOfPackets) == BLE_STATUS_SUCCESS)
            onStopped(std::make_optional(numberOfPackets));
        else
            onStopped(std::nullopt);
    }

    std::optional<int8_t> BleDtmSt::ReadRssi()
    {
        uint8_t rssi = 0;
        if (aci_hal_read_rssi(&rssi) != BLE_STATUS_SUCCESS || (rssi > 20 && rssi <= 128))
            return std::nullopt;

        return static_cast<int8_t>(rssi <= 127 ? rssi : static_cast<int16_t>(rssi) - 256);
    }

}
