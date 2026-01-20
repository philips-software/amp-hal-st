#include "hal_st/middlewares/ble_middleware/BleDtmSt.hpp"
#include <optional>

extern "C"
{
#include "ble_hal_aci.h"
#include "ble_hci_le.h"
}

namespace hal
{
    bool BleDtmSt::StartTone(uint8_t rfChannel, uint8_t offset)
    {
        if (rfChannel >= rfChannelLimits.min && rfChannel <= rfChannelLimits.max && offset >= rfOffsetLimits.min && offset <= rfOffsetLimits.max)
            return aci_hal_tone_start(rfChannel, offset) == BLE_STATUS_SUCCESS;

        return false;
    }

    bool BleDtmSt::StopTone()
    {
        return aci_hal_tone_stop() == BLE_STATUS_SUCCESS;
    }

    bool BleDtmSt::SetTxPowerLevel(uint8_t txPower)
    {
        if (txPower >= txPowerLimits.min && txPower <= txPowerLimits.max)
            return aci_hal_set_tx_power_level(0, txPower) == BLE_STATUS_SUCCESS;

        return false;
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

    std::optional<uint16_t> BleDtmSt::StopTest()
    {
        uint16_t numberOfPackets = 0;

        if (hci_le_test_end(&numberOfPackets) == BLE_STATUS_SUCCESS)
            return std::make_optional(numberOfPackets);

        return std::nullopt;
    }
}
