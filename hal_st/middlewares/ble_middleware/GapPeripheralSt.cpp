#include "hal_st/middlewares/ble_middleware/GapPeripheralSt.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"

namespace
{
    uint8_t ConvertAdvertisementType(services::GapPeripheral::AdvertisementType type)
    {
        return type == services::GapPeripheral::AdvertisementType::advInd ? ADV_IND : ADV_NONCONN_IND;
    }
}

namespace hal
{
    GapPeripheralSt::GapPeripheralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, uint16_t maxAttMtuSize, infra::CreatorBase<services::BondStorageManager, void()>& bondStorageManagerCreator)
        : HciEventSink(hciEventSource)
    {
        really_assert(maxAttMtuSize >= BLE_DEFAULT_ATT_MTU && maxAttMtuSize <= 251);
        // BLE middleware supported maxAttMtuSize = 512. Current usage of library limits maxAttMtuSize to 251 (max HCI buffer size)

        const uint8_t maxNumberOfBleLinks = 0x02;
        const uint8_t prepareWriteListSize = BLE_PREP_WRITE_X_ATT(maxAttMtuSize);
        const uint8_t numberOfBleMemoryBlocks = BLE_MBLOCKS_CALC(prepareWriteListSize, maxAttMtuSize, maxNumberOfBleLinks);
        const uint8_t bleStackOptions = ( SHCI_C2_BLE_INIT_OPTIONS_LL_HOST | SHCI_C2_BLE_INIT_OPTIONS_WITH_SVC_CHANGE_DESC | SHCI_C2_BLE_INIT_OPTIONS_DEVICE_NAME_RW | SHCI_C2_BLE_INIT_OPTIONS_NO_EXT_ADV | SHCI_C2_BLE_INIT_OPTIONS_NO_CS_ALGO2 | SHCI_C2_BLE_INIT_OPTIONS_POWER_CLASS_2_3 );

        SHCI_C2_Ble_Init_Cmd_Packet_t bleInitCmdPacket =
        {
            {{0,0,0}},      // Header (unused)
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
                0x00,       // HW version (unused)
                0x00,       // Maximum number of connection-oriented channels in initiator mode
                0x00,       // Minimum transmit power in dBm supported by the Controller
                0x00,       // Maximum transmit power in dBm supported by the Controller
                SHCI_C2_BLE_INIT_RX_MODEL_AGC_RSSI_LEGACY // BLE Rx model configuration flags
            }
        };

        if (SHCI_C2_BLE_Init(&bleInitCmdPacket) != SHCI_Success)
            std::abort();

        HciGapGattInit();
        SVCCTL_Init();
        hci_le_write_suggested_default_data_length(connectionInitialMaxTxOctets, connectionInitialMaxTxTime);

        SetPublicAddress(address);

        bondStorageManager.Emplace(bondStorageManagerCreator);
    }

    void GapPeripheralSt::RemoveAllBonds()
    {
        (*bondStorageManager)->RemoveAllBonds();
    }

    void GapPeripheralSt::SetPublicAddress(const hal::MacAddress& address)
    {
        aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, address.data());
    }

    hal::MacAddress GapPeripheralSt::GetPublicAddress() const
    {
        hal::MacAddress address;
        uint8_t length;
        aci_hal_read_config_data(CONFIG_DATA_PUBADDR_OFFSET, &length, address.data());
        assert(length == CONFIG_DATA_PUBADDR_LEN);
        return address;
    }

    void GapPeripheralSt::SetAdvertisementData(infra::ConstByteRange data)
    {
        advertisementData.assign(data);
    }

    void GapPeripheralSt::SetScanResponseData(infra::ConstByteRange data)
    {
        scanResponseData.assign(data);
    }

    void GapPeripheralSt::UpdateAdvertisementData()
    {
        //First clear the data set by the aci_gap_set_discoverable call by default
        aci_gap_delete_ad_type(AD_TYPE_TX_POWER_LEVEL);
        aci_gap_delete_ad_type(AD_TYPE_FLAGS);

        aci_gap_update_adv_data(advertisementData.size(), advertisementData.begin());

        hci_le_set_scan_response_data(scanResponseData.size(), scanResponseData.begin());
    }

    void GapPeripheralSt::UpdateState(services::GapPeripheralState newState)
    {
        state = newState;
        infra::EventDispatcher::Instance().Schedule([this]() { GapPeripheral::NotifyObservers([this](auto& obs) { obs.StateUpdated(state); }); });
    }

    void GapPeripheralSt::Advertise(AdvertisementType type, AdvertisementIntervalMultiplier multiplier)
    {
        assert(multiplier >= advertisementIntervalMultiplierMin && multiplier <= advertisementIntervalMultiplierMax);

        auto advTypeSt = ConvertAdvertisementType(type);

        tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
        if (allowPairing)
            ret = aci_gap_set_discoverable(advTypeSt, multiplier, multiplier, PUBLIC_ADDR, NO_WHITE_LIST_USE, 0, NULL, 0, NULL, 0, 0);
        else
            ret = aci_gap_set_undirected_connectable(multiplier, multiplier, PUBLIC_ADDR, WHITE_LIST_FOR_ALL);

        UpdateAdvertisementData();

        if (ret == BLE_STATUS_SUCCESS)
            UpdateState(services::GapPeripheralState::Advertising);
    }

    void GapPeripheralSt::Standby()
    {
        aci_gap_set_non_discoverable();
        UpdateState(services::GapPeripheralState::Standby);
    }

    void GapPeripheralSt::AllowPairing(bool allow)
    {
        allowPairing = allow;
        if (!allow)
            aci_gap_configure_whitelist();
    }

    void GapPeripheralSt::HciEvent(hci_event_pckt& event)
    {
        switch (event.evt)
        {
            case HCI_DISCONNECTION_COMPLETE_EVT_CODE:
                HandleHciDisconnectEvent(event);
                break;
            case HCI_LE_META_EVT_CODE:
                HandleHciLeMetaEvent(event);
                break;
            case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
                HandleHciVendorSpecificDebugEvent(event);
                break;
            default:
                HandleHciUnknownEvent(event);
                break;
        }
    }

    uint16_t GapPeripheralSt::EffectiveMaxAttMtuSize() const
    {
        return maxAttMtu;
    }

    void GapPeripheralSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        auto disconnectEvt = reinterpret_cast<hci_disconnection_complete_event_rp0*>(eventPacket.data);

        if (disconnectEvt->Connection_Handle == connectionContext.connectionHandle)
        {
            connectionContext.connectionHandle = 0;
            UpdateState(services::GapPeripheralState::Standby);
        }
    }

    void GapPeripheralSt::HandleHciLeConnectionUpdateEvent(evt_le_meta_event* metaEvent)
    {}

    void GapPeripheralSt::HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent)
    {}

    void GapPeripheralSt::HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto connectionCompleteEvt = reinterpret_cast<hci_le_connection_complete_event_rp0*>(metaEvent->data);

        hal::MacAddress address;
        std::copy(std::begin(connectionCompleteEvt->Peer_Address), std::end(connectionCompleteEvt->Peer_Address), std::begin(address));
 
        connectionContext.connectionHandle = connectionCompleteEvt->Connection_Handle;
        connectionContext.peerAddressType = connectionCompleteEvt->Peer_Address_Type;
        std::copy(std::begin(connectionCompleteEvt->Peer_Address), std::end(connectionCompleteEvt->Peer_Address), std::begin(connectionContext.peerAddress));

        maxAttMtu = defaultMaxAttMtuSize;
        UpdateState(services::GapPeripheralState::Connected);
    }

    void GapPeripheralSt::HandleHciLeDataLengthUpdateEvent(evt_le_meta_event* metaEvent)
    {}

    void GapPeripheralSt::HandleHciLeUnknownEvent(evt_le_meta_event* metaEvent)
    {}
    
    void GapPeripheralSt::HandleHciLeMetaEvent(hci_event_pckt& eventPacket)
    {
        auto metaEvent = reinterpret_cast<evt_le_meta_event*>(eventPacket.data);

        switch (metaEvent->subevent)
        {
            case HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVT_CODE:
                HandleHciLeConnectionUpdateEvent(metaEvent);
                break;
            case HCI_LE_PHY_UPDATE_COMPLETE_SUBEVT_CODE:
                HandleHciLePhyUpdateCompleteEvent(metaEvent);
                break;
            case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
                HandleHciLeConnectionCompleteEvent(metaEvent);
                break;
            case HCI_LE_DATA_LENGTH_CHANGE_SUBEVT_CODE:
                HandleHciLeDataLengthUpdateEvent(metaEvent);
                break;
            default:
                HandleHciLeUnknownEvent(metaEvent);
                break;
        }
    }

    void GapPeripheralSt::HandleBondLostEvent(evt_blecore_aci* vendorEvent)
    {
        aci_gap_allow_rebond(connectionContext.connectionHandle);
    }

    void GapPeripheralSt::HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        auto pairingComplete = reinterpret_cast<aci_gap_pairing_complete_event_rp0*>(vendorEvent->data);
 
        if (pairingComplete->Connection_Handle == connectionContext.connectionHandle
            && aci_gap_is_device_bonded(connectionContext.peerAddressType, connectionContext.peerAddress) == BLE_STATUS_SUCCESS)
        {
            hal::MacAddress address;

            auto result = aci_gap_resolve_private_addr(connectionContext.peerAddress, address.data());
            if (result != BLE_STATUS_SUCCESS)
                std::copy(std::begin(connectionContext.peerAddress), std::end(connectionContext.peerAddress), std::begin(address));

            (*bondStorageManager)->UpdateBondedDevice(address);
        }
    }

    void GapPeripheralSt::HandleMtuExchangeEvent(evt_blecore_aci* vendorEvent)
    {
        maxAttMtu = reinterpret_cast<aci_att_exchange_mtu_resp_event_rp0*>(vendorEvent->data)->Server_RX_MTU;
        AttMtuExchange::NotifyObservers([](auto& observer) { observer.ExchangedMaxAttMtuSize(); });
    }

    void GapPeripheralSt::HandleHciVendorSpecificDebugEvent(hci_event_pckt& eventPacket)
    {
        auto vendorEvent = reinterpret_cast<evt_blecore_aci*>(eventPacket.data);

        switch (vendorEvent->ecode)
        {
            case ACI_GAP_BOND_LOST_VSEVT_CODE:
                HandleBondLostEvent(vendorEvent);
                break; 
            case ACI_GAP_PAIRING_COMPLETE_VSEVT_CODE:
                HandlePairingCompleteEvent(vendorEvent);
                break;
            case ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE:
                HandleMtuExchangeEvent(vendorEvent);
                break;
            default:
                break;
        }
    }

    void GapPeripheralSt::HandleHciUnknownEvent(hci_event_pckt& eventPacket)
    {}

    void GapPeripheralSt::HciGapGattInit()
    {
        uint16_t gapServiceHandle, gapDevNameCharHandle, gapAppearanceCharHandle;
        uint16_t appearance[1] = { 0x0 };

        //HCI Reset to synchronise BLE Stack
        hci_reset();

        // Write Identity root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, (uint8_t*)identityRootKey);

        // Write Encryption root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, (uint8_t*)encryptionRootKey);

        aci_hal_set_tx_power_level(1, txPowerLevel);
        aci_gatt_init();
        aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 8, &gapServiceHandle, &gapDevNameCharHandle, &gapAppearanceCharHandle);
        aci_gatt_update_char_value(gapServiceHandle, gapDevNameCharHandle, 0, strlen(devName), (uint8_t*)devName);
        aci_gatt_update_char_value(gapServiceHandle, gapAppearanceCharHandle, 0, 2, (uint8_t*)&appearance);
        aci_gap_set_io_capability(ioCapability);
        aci_gap_set_authentication_requirement(bondingMode, mitmMode, secureConnectionSupport, keypressNotificationSupport, 16, 16, 0, 111111, PUBLIC_ADDR);
    }
}
