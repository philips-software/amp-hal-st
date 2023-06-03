#include "hal_st/middlewares/ble_middleware/GapPeripheralSt.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"

namespace
{
    uint8_t ConvertAdvertisementType(services::GapAdvertisementType type)
    {
        return type == services::GapAdvertisementType::advInd ? ADV_IND : ADV_NONCONN_IND;
    }
}

namespace hal
{
    GapPeripheralSt::GapPeripheralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const hal::GapSt::RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService gapService, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t* bleBondsStorage)
        : GapSt(hciEventSource, address, rootKeys, maxAttMtuSize, txPowerLevel, *bleBondsStorage)
    {
        Initialize(gapService);
        bondStorageSynchronizer.Emplace(bondStorageSynchronizerCreator);
    }

    void GapPeripheralSt::RemoveAllBonds()
    {
        (*bondStorageSynchronizer)->RemoveAllBonds();
    }

    void GapPeripheralSt::RemoveOldestBond()
    {
        std::abort();
    }

    std::size_t GapPeripheralSt::GetMaxNumberOfBonds() const
    {
        if (bondStorageSynchronizer)
            return (*bondStorageSynchronizer)->GetMaxNumberOfBonds();

        return 0;
    }

    std::size_t GapPeripheralSt::GetNumberOfBonds() const
    {
        uint8_t numberOfBondedAddress = 0;
        std::array<Bonded_Device_Entry_t, maxNumberOfBonds> bondedDevices;
        aci_gap_get_bonded_devices(&numberOfBondedAddress, bondedDevices.data());

        return numberOfBondedAddress;
    }

    services::GapAddress GapPeripheralSt::GetAddress() const
    {
        services::GapAddress address;
        /* Use last peer addres to get current RPA */
        [[maybe_unused]] auto status = hci_le_read_local_resolvable_address(connectionContext.peerAddressType, connectionContext.peerAddress.data(), address.address.data());

        address.type = services::GapDeviceAddressType::publicAddress;

        assert(status == BLE_STATUS_SUCCESS);

        return address;
    }

    services::GapAddress GapPeripheralSt::GetIdentityAddress() const
    {
        services::GapAddress address;
        uint8_t length = 0;

        aci_hal_read_config_data(CONFIG_DATA_PUBADDR_OFFSET, &length, address.address.data());
        address.type = services::GapDeviceAddressType::publicAddress;

        return address;
    }

    void GapPeripheralSt::SetAdvertisementData(infra::ConstByteRange data)
    {
        advertisementData.assign(data);
    }

    infra::ConstByteRange GapPeripheralSt::GetAdvertisementData() const
    {
        return infra::MakeRange(advertisementData);
    }

    void GapPeripheralSt::SetScanResponseData(infra::ConstByteRange data)
    {
        scanResponseData.assign(data);
    }

    infra::ConstByteRange GapPeripheralSt::GetScanResponseData() const
    {
        return infra::MakeRange(scanResponseData);
    }

    void GapPeripheralSt::UpdateAdvertisementData()
    {
        //First clear the data set by the aci_gap_set_discoverable call by default
        aci_gap_delete_ad_type(AD_TYPE_TX_POWER_LEVEL);
        aci_gap_delete_ad_type(AD_TYPE_FLAGS);

        aci_gap_update_adv_data(advertisementData.size(), advertisementData.begin());

        hci_le_set_scan_response_data(scanResponseData.size(), scanResponseData.begin());
    }

    void GapPeripheralSt::UpdateState(services::GapState newState)
    {
        state = newState;
        infra::EventDispatcher::Instance().Schedule([this]() { GapPeripheral::NotifyObservers([this](auto& obs) { obs.StateChanged(state); }); });
    }

    void GapPeripheralSt::Advertise(services::GapAdvertisementType type, AdvertisementIntervalMultiplier multiplier)
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
            UpdateState(services::GapState::advertising);
    }

    void GapPeripheralSt::Standby()
    {
        if (connectionContext.connectionHandle != 0)
            aci_gap_terminate(connectionContext.connectionHandle, 0x13);
        else
        {
            aci_gap_set_non_discoverable();
            UpdateState(services::GapState::standby);
        }
    }

    void GapPeripheralSt::AllowPairing(bool allow)
    {
        allowPairing = allow;
    }

    void GapPeripheralSt::SetSecurityMode(services::GapSecurityMode mode, services::GapSecurityLevel level)
    {
        assert(mode == services::GapSecurityMode::mode1);

        enum class SecureConnection : uint8_t
        {
            notSupported = 0,
            optional = 1,
            mandatory
        };

        SecureConnection secureConnectionSupport = (level == services::GapSecurityLevel::level4) ? SecureConnection::mandatory : SecureConnection::optional;
        uint8_t mitmMode = (level == services::GapSecurityLevel::level3 || level == services::GapSecurityLevel::level4) ? 1 : 0;

        aci_gap_set_authentication_requirement(bondingMode, mitmMode, static_cast<uint8_t>(secureConnectionSupport), keypressNotificationSupport, 16, 16, 0, 111111, GAP_PUBLIC_ADDR);
    }

    void GapPeripheralSt::SetIoCapabilities(services::GapIoCapabilities caps)
    {
        tBleStatus status = BLE_STATUS_FAILED;

        switch (caps)
        {
            case services::GapIoCapabilities::display:
                status = aci_gap_set_io_capability(0);
                break;
            case services::GapIoCapabilities::displayYesNo:
                status = aci_gap_set_io_capability(1);
                break;
            case services::GapIoCapabilities::keyboard:
                status = aci_gap_set_io_capability(2);
                break;
            case services::GapIoCapabilities::none:
                status = aci_gap_set_io_capability(3);
                break;
            case services::GapIoCapabilities::keyboardDisplay:
                status = aci_gap_set_io_capability(4);
                break;
        }

        assert(status == BLE_STATUS_SUCCESS);
    }

    void GapPeripheralSt::AuthenticateWithPasskey(uint32_t passkey)
    {
        std::abort();
    }

    void GapPeripheralSt::NumericComparisonConfirm(bool accept)
    {
        std::abort();
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

    void GapPeripheralSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        GapSt::HandleHciDisconnectEvent(eventPacket);

        auto disconnectEvt = reinterpret_cast<hci_disconnection_complete_event_rp0*>(eventPacket.data);

        if (disconnectEvt->Connection_Handle == connectionContext.connectionHandle)
        {
            connectionContext.connectionHandle = 0;

            UpdateState(services::GapState::standby);
        }
    }

    void GapPeripheralSt::HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLeEnhancedConnectionCompleteEvent(metaEvent);

        RequestConnectionParameterUpdate();

        UpdateState(services::GapState::connected);
    }

    void GapPeripheralSt::HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        GapSt::HandlePairingCompleteEvent(vendorEvent);

        auto pairingComplete = reinterpret_cast<aci_gap_pairing_complete_event_rp0*>(vendorEvent->data);

        if (pairingComplete->Connection_Handle == connectionContext.connectionHandle && aci_gap_is_device_bonded(connectionContext.peerAddressType, connectionContext.peerAddress.data()) == BLE_STATUS_SUCCESS)
        {
            hal::MacAddress address = connectionContext.peerAddress;
            aci_gap_resolve_private_addr(connectionContext.peerAddress.data(), address.data());
            (*bondStorageSynchronizer)->UpdateBondedDevice(address);
        }
    }

    void GapPeripheralSt::RequestConnectionParameterUpdate()
    {
        aci_l2cap_connection_parameter_update_req(connectionContext.connectionHandle,
            connectionParameters.minConnIntMultiplier, connectionParameters.maxConnIntMultiplier,
            connectionParameters.slaveLatency, connectionParameters.supervisorTimeoutMs);
    }

    void GapPeripheralSt::Initialize(const GapService& gapService)
    {
        uint16_t gapServiceHandle, gapDevNameCharHandle, gapAppearanceCharHandle;

        aci_gap_init(GAP_PERIPHERAL_ROLE, PRIVACY_ENABLED, gapService.deviceName.size(), &gapServiceHandle, &gapDevNameCharHandle, &gapAppearanceCharHandle);
        aci_gatt_update_char_value(gapServiceHandle, gapDevNameCharHandle, 0, gapService.deviceName.size(), reinterpret_cast<const uint8_t*>(gapService.deviceName.data()));
        aci_gatt_update_char_value(gapServiceHandle, gapAppearanceCharHandle, 0, sizeof(gapService.appearance), reinterpret_cast<const uint8_t*>(&gapService.appearance));

        SetIoCapabilities(services::GapIoCapabilities::none);
        SetSecurityMode(services::GapSecurityMode::mode1, services::GapSecurityLevel::level1);
    }
}
