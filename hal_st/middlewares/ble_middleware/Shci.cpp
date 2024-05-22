#include "hal_st/middlewares/ble_middleware/Shci.hpp"
#include "shci.h"
#include "stm32wbxx_ll_system.h"

namespace
{
    constexpr uint8_t RfWakeupClockSelection(const hal::GapSt::RfWakeupClock& rfWakeupClock)
    {
        if (rfWakeupClock == hal::GapSt::RfWakeupClock::highSpeedExternal)
            return SHCI_C2_BLE_INIT_CFG_BLE_LS_CLK_HSE_1024;
        else
            return SHCI_C2_BLE_INIT_CFG_BLE_LS_CLK_LSE;
    }
}

namespace hal
{
    void ShciInitialization(GapSt::BleBondStorage bleBondStorage, const GapSt::Configuration& configuration)
    {
        const uint8_t maxNumberOfBleLinks = 0x01;
        const uint8_t prepareWriteListSize = BLE_PREP_WRITE_X_ATT(configuration.maxAttMtuSize);
        const uint8_t numberOfBleMemoryBlocks = BLE_MBLOCKS_CALC(prepareWriteListSize, configuration.maxAttMtuSize, maxNumberOfBleLinks);
        const uint8_t bleStackOptions = (SHCI_C2_BLE_INIT_OPTIONS_LL_HOST | SHCI_C2_BLE_INIT_OPTIONS_WITH_SVC_CHANGE_DESC | SHCI_C2_BLE_INIT_OPTIONS_DEVICE_NAME_RO | SHCI_C2_BLE_INIT_OPTIONS_NO_EXT_ADV | SHCI_C2_BLE_INIT_OPTIONS_NO_CS_ALGO2 |
                                         SHCI_C2_BLE_INIT_OPTIONS_FULL_GATTDB_NVM | SHCI_C2_BLE_INIT_OPTIONS_GATT_CACHING_NOTUSED | SHCI_C2_BLE_INIT_OPTIONS_POWER_CLASS_2_3 | SHCI_C2_BLE_INIT_OPTIONS_APPEARANCE_READONLY | SHCI_C2_BLE_INIT_OPTIONS_ENHANCED_ATT_NOTSUPPORTED);

        SHCI_C2_CONFIG_Cmd_Param_t configParam = {
            SHCI_C2_CONFIG_PAYLOAD_CMD_SIZE,
            SHCI_C2_CONFIG_CONFIG1_BIT0_BLE_NVM_DATA_TO_SRAM,
            SHCI_C2_CONFIG_EVTMASK1_BIT1_BLE_NVM_RAM_UPDATE_ENABLE,
            0, // Spare
            reinterpret_cast<uint32_t>(&bleBondStorage.bleBondsStorage),
            0, // ThreadNvmRamAddress
            static_cast<uint16_t>(LL_DBGMCU_GetRevisionID()),
            static_cast<uint16_t>(LL_DBGMCU_GetDeviceID()),
        };

        if (SHCI_C2_Config(&configParam) != SHCI_Success)
            std::abort();

        SHCI_C2_Ble_Init_Cmd_Packet_t bleInitCmdPacket = {
            { { 0, 0, 0 } }, // Header (unused)
            {
                0x00,  // BLE buffer address (unused)
                0x00,  // BLE buffer size (unused)
                0x44,  // Maximum number of GATT Attributes
                0x08,  // Maximum number of Services that can be stored in the GATT database
                0x540, // Size of the storage area for Attribute values
                maxNumberOfBleLinks,
                0x01, // Enable or disable the Extended Packet length feature
                prepareWriteListSize,
                numberOfBleMemoryBlocks,
                configuration.maxAttMtuSize,
                0x1FA,                                               // Sleep clock accuracy in Slave mode
                0x00,                                                // Sleep clock accuracy in Master mode
                RfWakeupClockSelection(configuration.rfWakeupClock), // Source for the low speed clock for RF wake-up
                0xFFFFFFFF,                                          // Maximum duration of the connection event when the device is in Slave mode in units of 625/256 us (~2.44 us)
                0x148,                                               // Start up time of the high speed (16 or 32 MHz) crystal oscillator in units of 625/256 us (~2.44 us)
                0x01,                                                // Viterbi Mode
                bleStackOptions,
                0,   // HW version (unused)
                32,  // Maximum number of connection-oriented channels in initiator mode
                -40, // Minimum transmit power in dBm supported by the Controller
                6,   // Maximum transmit power in dBm supported by the Controller
                SHCI_C2_BLE_INIT_RX_MODEL_AGC_RSSI_LEGACY,
                3,    // Maximum number of advertising sets.
                1650, // Maximum advertising data length (in bytes)
                0,    // RF TX Path Compensation Value (16-bit signed integer). Units: 0.1 dB.
                0,    // RF RX Path Compensation Value (16-bit signed integer). Units: 0.1 dB.
                SHCI_C2_BLE_INIT_BLE_CORE_5_3 }
        };

        if (SHCI_C2_BLE_Init(&bleInitCmdPacket) != SHCI_Success)
            std::abort();
    }
}
