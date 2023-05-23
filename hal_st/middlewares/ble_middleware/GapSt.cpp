#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "stm32wbxx_ll_system.h"

namespace hal
{
    const services::GapConnectionParameters GapSt::connectionParameters
    {
        6,
        6,
        0,
        500,
    };

    GapSt::GapSt(hal::HciEventSource& hciEventSource, hal::MacAddress& address, const RootKeys& rootKeys, uint16_t& maxAttMtuSize, uint8_t& txPowerLevel, uint32_t& bleBondsStorage)
        : HciEventSink(hciEventSource)
        , txPowerLevel(txPowerLevel)
    {
        really_assert(maxAttMtuSize >= BLE_DEFAULT_ATT_MTU && maxAttMtuSize <= 251);
        // BLE middleware supported maxAttMtuSize = 512. Current usage of library limits maxAttMtuSize to 251 (max HCI buffer size)

        connectionContext.connectionHandle = GapSt::invalidConnection;

        const uint8_t maxNumberOfBleLinks = 0x01;
        const uint8_t prepareWriteListSize = BLE_PREP_WRITE_X_ATT(maxAttMtuSize);
        const uint8_t numberOfBleMemoryBlocks = BLE_MBLOCKS_CALC(prepareWriteListSize, maxAttMtuSize, maxNumberOfBleLinks);
        const uint8_t bleStackOptions = (SHCI_C2_BLE_INIT_OPTIONS_LL_HOST | SHCI_C2_BLE_INIT_OPTIONS_WITH_SVC_CHANGE_DESC | SHCI_C2_BLE_INIT_OPTIONS_DEVICE_NAME_RO | SHCI_C2_BLE_INIT_OPTIONS_NO_EXT_ADV | SHCI_C2_BLE_INIT_OPTIONS_NO_CS_ALGO2 |
            SHCI_C2_BLE_INIT_OPTIONS_FULL_GATTDB_NVM | SHCI_C2_BLE_INIT_OPTIONS_GATT_CACHING_NOTUSED | SHCI_C2_BLE_INIT_OPTIONS_POWER_CLASS_2_3 | SHCI_C2_BLE_INIT_OPTIONS_APPEARANCE_READONLY | SHCI_C2_BLE_INIT_OPTIONS_ENHANCED_ATT_NOTSUPPORTED);

        SHCI_C2_CONFIG_Cmd_Param_t configParam = {
            SHCI_C2_CONFIG_PAYLOAD_CMD_SIZE,
            SHCI_C2_CONFIG_CONFIG1_BIT0_BLE_NVM_DATA_TO_SRAM,
            SHCI_C2_CONFIG_EVTMASK1_BIT1_BLE_NVM_RAM_UPDATE_ENABLE,
            0, // Spare
            reinterpret_cast<uint32_t>(&bleBondsStorage),
            0, // ThreadNvmRamAddress
            static_cast<uint16_t>(LL_DBGMCU_GetRevisionID()),
            static_cast<uint16_t>(LL_DBGMCU_GetDeviceID()),
        };

        if (SHCI_C2_Config(&configParam) != SHCI_Success)
            std::abort();

        SHCI_C2_Ble_Init_Cmd_Packet_t bleInitCmdPacket = {
            { { 0, 0, 0 } }, // Header (unused)
            {
                0x00,       // BLE buffer address (unused)
                0x00,       // BLE buffer size (unused)
                0x44,       // Maximum number of GATT Attributes
                0x08,       // Maximum number of Services that can be stored in the GATT database
                0x540,      // Size of the storage area for Attribute values
                maxNumberOfBleLinks,
                0x01,       // Enable or disable the Extended Packet length feature
                prepareWriteListSize,
                numberOfBleMemoryBlocks,
                maxAttMtuSize,
                0x1FA,      // Sleep clock accuracy in Slave mode
                0x00,       // Sleep clock accuracy in Master mode
                0x00,       // Source for the low speed clock for RF wake-up
                0xFFFFFFFF, // Maximum duration of the connection event when the device is in Slave mode in units of 625/256 us (~2.44 us)
                0x148,      // Start up time of the high speed (16 or 32 MHz) crystal oscillator in units of 625/256 us (~2.44 us)
                0x01,       // Viterbi Mode
                bleStackOptions,
                0,          // HW version (unused)
                32,         // Maximum number of connection-oriented channels in initiator mode
                -40,        // Minimum transmit power in dBm supported by the Controller
                6,          // Maximum transmit power in dBm supported by the Controller
                SHCI_C2_BLE_INIT_RX_MODEL_AGC_RSSI_LEGACY,
                3,          // Maximum number of advertising sets.
                1650,       // Maximum advertising data length (in bytes)
                0,          // RF TX Path Compensation Value (16-bit signed integer). Units: 0.1 dB.
                0,          // RF RX Path Compensation Value (16-bit signed integer). Units: 0.1 dB.
                SHCI_C2_BLE_INIT_BLE_CORE_5_3
            }
        };

        if (SHCI_C2_BLE_Init(&bleInitCmdPacket) != SHCI_Success)
            std::abort();

        Initialization(rootKeys.identity, rootKeys.encryption);

        aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, address.data());

        SVCCTL_Init();

        hci_le_write_suggested_default_data_length(services::GapPeripheral::connectionInitialMaxTxOctets, services::GapPeripheral::connectionInitialMaxTxTime);
        hci_le_set_default_phy(allPhys, speed2Mbps, speed2Mbps);
    }

    void GapSt::SetAddress(const hal::MacAddress& address, services::GapDeviceAddressType addressType)
    {
        auto offset = addressType == services::GapDeviceAddressType::publicAddress ? CONFIG_DATA_PUBADDR_OFFSET : CONFIG_DATA_RANDOM_ADDRESS_OFFSET;

        aci_hal_write_config_data(offset, CONFIG_DATA_PUBADDR_LEN, address.data());
    }

    void GapSt::HciEvent(hci_event_pckt& event)
    {
        switch (event.evt)
        {
        case HCI_LE_META_EVT_CODE:
            HandleHciLeMetaEvent(event);
            break;
        case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
            HandleHciVendorSpecificDebugEvent(event);
            break;
        default:
            break;
        }
    }

    uint16_t GapSt::EffectiveMaxAttMtuSize() const
    {
        return maxAttMtu;
    }

    void GapSt::HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto connectionCompleteEvent = *reinterpret_cast<hci_le_connection_complete_event_rp0*>(metaEvent->data);

        really_assert(connectionCompleteEvent.Status == BLE_STATUS_SUCCESS);

        SetConnectionContext(connectionCompleteEvent.Connection_Handle, connectionCompleteEvent.Peer_Address_Type, &connectionCompleteEvent.Peer_Address[0]);
    }

    void GapSt::HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto connectionCompleteEvt = reinterpret_cast<hci_le_enhanced_connection_complete_event_rp0*>(metaEvent->data);

        SetConnectionContext(connectionCompleteEvt->Connection_Handle, connectionCompleteEvt->Peer_Address_Type, &connectionCompleteEvt->Peer_Address[0]);

        maxAttMtu = defaultMaxAttMtuSize;
    }

    void GapSt::HandleHciLeMetaEvent(hci_event_pckt& eventPacket)
    {
        auto metaEvent = reinterpret_cast<evt_le_meta_event*>(eventPacket.data);

        switch (metaEvent->subevent)
        {
        case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE:
            HandleHciLeEnhancedConnectionCompleteEvent(metaEvent);
            break;
        case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
            HandleHciLeConnectionCompleteEvent(metaEvent);
            break;
        default:
            break;
        }
    }

    void GapSt::HandleBondLostEvent(evt_blecore_aci* vendorEvent)
    {
        aci_gap_allow_rebond(connectionContext.connectionHandle);
    }

    void GapSt::HandleMtuExchangeResponseEvent(evt_blecore_aci* vendorEvent)
    {
        maxAttMtu = reinterpret_cast<aci_att_exchange_mtu_resp_event_rp0*>(vendorEvent->data)->Server_RX_MTU;
        AttMtuExchange::NotifyObservers([](auto& observer) { observer.ExchangedMaxAttMtuSize(); });
    }

    void GapSt::HandleHciVendorSpecificDebugEvent(hci_event_pckt& eventPacket)
    {
        auto vendorEvent = reinterpret_cast<evt_blecore_aci*>(eventPacket.data);

        switch (vendorEvent->ecode)
        {
        case ACI_GAP_BOND_LOST_VSEVT_CODE:
            HandleBondLostEvent(vendorEvent);
            break;
        case ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE:
            HandleMtuExchangeResponseEvent(vendorEvent);
            break;
        default:
            break;
        }
    }

    void GapSt::SetConnectionContext(uint16_t connectionHandle, uint8_t perrAddressType, uint8_t* perAddress)
    {
        static constexpr auto deducePeerAddressType = [](auto peerAddressType) {
            enum class PeerAddressType : uint8_t
            {
                PUBLIC,
                RANDOM,
                RESOLVED_PUBLIC_IDENTITY,
                RESOLVED_RANDOM_STATIC_IDENTITY
            };

            switch (static_cast<PeerAddressType>(peerAddressType))
            {
            case PeerAddressType::PUBLIC:
            case PeerAddressType::RANDOM:
                return peerAddressType;

            case PeerAddressType::RESOLVED_PUBLIC_IDENTITY:
                return infra::enum_cast(PeerAddressType::PUBLIC);

            case PeerAddressType::RESOLVED_RANDOM_STATIC_IDENTITY:
            default:
                return infra::enum_cast(PeerAddressType::RANDOM);
            }
        };

        connectionContext.connectionHandle = connectionHandle;
        connectionContext.peerAddressType = deducePeerAddressType(perrAddressType);
        std::copy_n(perAddress, connectionContext.peerAddress.size(), std::begin(connectionContext.peerAddress));
    }

    void GapSt::Initialization(const std::array<uint8_t, 16>& identityRootKey, const std::array<uint8_t, 16>& encryptionRootKey)
    {
        // HCI Reset to synchronise BLE Stack
        hci_reset();

        // Write Identity root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, identityRootKey.data());

        // Write Encryption root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, encryptionRootKey.data());

        aci_hal_set_tx_power_level(1, txPowerLevel);
        aci_gatt_init();
    }
}
