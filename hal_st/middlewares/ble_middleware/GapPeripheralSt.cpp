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
    GapPeripheralSt::GapPeripheralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService gapService, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t* bleBondsStorage)
        : HciEventSink(hciEventSource)
        , txPowerLevel(txPowerLevel)
        , gapService(gapService)
    {
        really_assert(maxAttMtuSize >= BLE_DEFAULT_ATT_MTU && maxAttMtuSize <= 251);
        // BLE middleware supported maxAttMtuSize = 512. Current usage of library limits maxAttMtuSize to 251 (max HCI buffer size)

        const uint8_t maxNumberOfBleLinks = 0x01;
        const uint8_t prepareWriteListSize = BLE_PREP_WRITE_X_ATT(maxAttMtuSize);
        const uint8_t numberOfBleMemoryBlocks = BLE_MBLOCKS_CALC(prepareWriteListSize, maxAttMtuSize, maxNumberOfBleLinks);
        const uint8_t bleStackOptions = ( SHCI_C2_BLE_INIT_OPTIONS_LL_HOST | SHCI_C2_BLE_INIT_OPTIONS_WITH_SVC_CHANGE_DESC | SHCI_C2_BLE_INIT_OPTIONS_DEVICE_NAME_RW | SHCI_C2_BLE_INIT_OPTIONS_NO_EXT_ADV | SHCI_C2_BLE_INIT_OPTIONS_NO_CS_ALGO2 | SHCI_C2_BLE_INIT_OPTIONS_POWER_CLASS_2_3 );

        SHCI_C2_CONFIG_Cmd_Param_t configParam = {
            SHCI_C2_CONFIG_PAYLOAD_CMD_SIZE,
            SHCI_C2_CONFIG_CONFIG1_BIT0_BLE_NVM_DATA_TO_SRAM,
            SHCI_C2_CONFIG_EVTMASK1_BIT1_BLE_NVM_RAM_UPDATE_ENABLE,
            0,
            reinterpret_cast<uint32_t>(bleBondsStorage),
            0,
            0,
        };

        if (SHCI_C2_Config(&configParam) != SHCI_Success)
            std::abort();

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

        HciGapGattInit(rootKeys.identity, rootKeys.encryption);
        SVCCTL_Init();
        hci_le_write_suggested_default_data_length(connectionInitialMaxTxOctets, connectionInitialMaxTxTime);

        SetPublicAddress(address);
        bondStorageSynchronizer.Emplace(bondStorageSynchronizerCreator);
    }

    void GapPeripheralSt::RemoveAllBonds()
    {
        (*bondStorageSynchronizer)->RemoveAllBonds();
    }

    void GapPeripheralSt::SetPublicAddress(const hal::MacAddress& address)
    {
        aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, address.data());
    }

    hal::MacAddress GapPeripheralSt::GetResolvableAddress() const
    {
        hal::MacAddress address;
        /* Use last peer addres to get current RPA */
        [[maybe_unused]] auto status = hci_le_read_local_resolvable_address(connectionContext.peerAddressType, connectionContext.peerAddress.data(), address.data());

        assert(status == BLE_STATUS_SUCCESS);

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

    void GapPeripheralSt::RequestConnectionParameterUpdate()
    {
        aci_l2cap_connection_parameter_update_req(connectionContext.connectionHandle, minConnectionInterval, maxConnectionInterval, slaveLatency, supervisionTimeout);
    }

    void GapPeripheralSt::Advertise(AdvertisementType type, AdvertisementIntervalMultiplier multiplier)
    {
        assert(multiplier >= advertisementIntervalMultiplierMin && multiplier <= advertisementIntervalMultiplierMax);

        auto advTypeSt = ConvertAdvertisementType(type);

        UpdateResolvingList();

        tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
        if (allowPairing)
            ret = aci_gap_set_discoverable(advTypeSt, multiplier, multiplier, GAP_RESOLVABLE_PRIVATE_ADDR, NO_WHITE_LIST_USE, 0, NULL, 0, NULL, 0, 0);
        else
            ret = aci_gap_set_undirected_connectable(multiplier, multiplier, GAP_RESOLVABLE_PRIVATE_ADDR, WHITE_LIST_FOR_ALL);

        UpdateAdvertisementData();

        if (ret == BLE_STATUS_SUCCESS)
            UpdateState(services::GapPeripheralState::Advertising);
    }

    void GapPeripheralSt::Standby()
    {
        if (connectionContext.connectionHandle != 0)
            aci_gap_terminate(connectionContext.connectionHandle, 0x13);
        else
        {
            aci_gap_set_non_discoverable();
            UpdateState(services::GapPeripheralState::Standby);
        }
    }

    void GapPeripheralSt::AllowPairing(bool allow)
    {
        allowPairing = allow;
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

    void GapPeripheralSt::HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        static constexpr auto deducePeerAddressType = [](auto peerAddressType){ 
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
                    return infra::enum_cast(PeerAddressType::RANDOM);
            }
        };

        auto connectionCompleteEvt = reinterpret_cast<hci_le_enhanced_connection_complete_event_rp0*>(metaEvent->data);

        connectionContext.connectionHandle = connectionCompleteEvt->Connection_Handle;

        RequestConnectionParameterUpdate();

        connectionContext.peerAddressType = deducePeerAddressType(connectionCompleteEvt->Peer_Address_Type);
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
        case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE:
            HandleHciLeEnhancedConnectionCompleteEvent(metaEvent);
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
            && aci_gap_is_device_bonded(connectionContext.peerAddressType, connectionContext.peerAddress.data()) == BLE_STATUS_SUCCESS)
            {
                hal::MacAddress address = connectionContext.peerAddress;
                aci_gap_resolve_private_addr(connectionContext.peerAddress.data(), address.data());
                (*bondStorageSynchronizer)->UpdateBondedDevice(address);
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

    void GapPeripheralSt::UpdateResolvingList()
    {
        aci_gap_configure_whitelist();

        uint8_t numberOfBondedAddress;
        std::array<Bonded_Device_Entry_t, maxNumberOfBonds> bondedDevices;
        aci_gap_get_bonded_devices(&numberOfBondedAddress, bondedDevices.data());

        if (numberOfBondedAddress == 0)
        {
            aci_gap_add_devices_to_resolving_list(1, &dummyPeer, 1);

            std::copy(std::begin(dummyPeer.Peer_Identity_Address), std::end(dummyPeer.Peer_Identity_Address), connectionContext.peerAddress.begin());
            connectionContext.peerAddressType = dummyPeer.Peer_Identity_Address_Type;
        }
        else
        {
            aci_gap_add_devices_to_resolving_list(numberOfBondedAddress, reinterpret_cast<const Whitelist_Identity_Entry_t*>(bondedDevices.begin()), 1);

            std::copy(std::begin(bondedDevices[numberOfBondedAddress-1].Address), std::end(bondedDevices[numberOfBondedAddress-1].Address), connectionContext.peerAddress.begin());
            connectionContext.peerAddressType = bondedDevices[numberOfBondedAddress-1].Address_Type;
        }
    }

    void GapPeripheralSt::ClearResolvingList()
    {
        aci_gap_configure_whitelist();

        aci_gap_add_devices_to_resolving_list(0, nullptr, 1);
    }

    void GapPeripheralSt::HandleHciUnknownEvent(hci_event_pckt& eventPacket)
    {}

    void GapPeripheralSt::HciGapGattInit(const std::array<uint8_t, 16>& identityRootKey, const std::array<uint8_t, 16>& encryptionRootKey)
    {
        uint16_t gapServiceHandle, gapDevNameCharHandle, gapAppearanceCharHandle;

        //HCI Reset to synchronise BLE Stack
        hci_reset();

        // Write Identity root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, identityRootKey.data());

        // Write Encryption root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, encryptionRootKey.data());

        aci_hal_set_tx_power_level(1, txPowerLevel);
        aci_gatt_init();
        aci_gap_init(GAP_PERIPHERAL_ROLE, PRIVACY_ENABLED, 8, &gapServiceHandle, &gapDevNameCharHandle, &gapAppearanceCharHandle);
        aci_gatt_update_char_value(gapServiceHandle, gapDevNameCharHandle, 0, gapService.deviceName.size(), (uint8_t*)gapService.deviceName.data());
        aci_gatt_update_char_value(gapServiceHandle, gapAppearanceCharHandle, 0, 2, (uint8_t*)&gapService.appearance);
        aci_gap_set_io_capability(ioCapability);
        aci_gap_set_authentication_requirement(bondingMode, mitmMode, secureConnectionSupport, keypressNotificationSupport, 16, 16, 0, 111111, GAP_PUBLIC_ADDR);
    }
}
