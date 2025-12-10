#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "ble_defs.h"
#include "ble_gap_aci.h"
#include "ble_types.h"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "services/ble/Gap.hpp"

namespace hal
{
    namespace
    {
        constexpr services::GapPairingObserver::PairingErrorType ParserPairingFailure(uint8_t status, uint8_t error)
        {
            if (status == SMP_PAIRING_STATUS_SMP_TIMEOUT)
                return services::GapPairingObserver::PairingErrorType::timeout;
            else if (status == SMP_PAIRING_STATUS_ENCRYPT_FAILED)
                return services::GapPairingObserver::PairingErrorType::encryptionFailed;
            else
                switch (error)
                {
                    case PAIRING_NOT_SUPPORTED:
                        return services::GapPairingObserver::PairingErrorType::pairingNotSupported;
                    case AUTH_REQ_CANNOT_BE_MET:
                        return services::GapPairingObserver::PairingErrorType::authenticationRequirementsNotMet;
                    case INSUFF_ENCRYPTION_KEY_SIZE:
                        return services::GapPairingObserver::PairingErrorType::insufficientEncryptionKeySize;
                    case CONFIRM_VALUE_FAILED:
                        return services::GapPairingObserver::PairingErrorType::passkeyEntryFailed;
                    case SMP_SC_NUMCOMPARISON_FAILED:
                        return services::GapPairingObserver::PairingErrorType::numericComparisonFailed;
                    default:
                        return services::GapPairingObserver::PairingErrorType::unknown;
                }
        }
    }

    GapSt::GapSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration)
        : HciEventSink(hciEventSource)
        , ownAddressType(configuration.privacy ? GAP_RESOLVABLE_PRIVATE_ADDR : GAP_PUBLIC_ADDR)
        , securityLevel(configuration.security.securityLevel)
        , bondStorageSynchronizer(bondStorageSynchronizer)
    {
        connectionContext.connectionHandle = GapSt::invalidConnection;

        // HCI Reset to synchronize BLE Stack
        hci_reset();

        // Write Identity root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, configuration.rootKeys.identity.data());

        // Write Encryption root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, configuration.rootKeys.encryption.data());

        aci_hal_set_tx_power_level(1, configuration.txPowerLevel);
        aci_gatt_init();

        aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, configuration.address.data());

        const std::array<uint8_t, 8> events = { { 0x9F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
        hci_le_set_event_mask(events.data());

        SVCCTL_Init();
    }

    uint16_t GapSt::EffectiveMaxAttMtuSize() const
    {
        return maxAttMtu;
    }

    void GapSt::MtuExchange()
    {
        auto status = aci_gatt_exchange_config(this->connectionContext.connectionHandle);
        assert(status == BLE_STATUS_SUCCESS);
    }

    void GapSt::RemoveAllBonds()
    {
        bondStorageSynchronizer.RemoveAllBonds();
        UpdateNrBonds();
    }

    void GapSt::RemoveOldestBond()
    {
        std::abort();
    }

    std::size_t GapSt::GetMaxNumberOfBonds() const
    {
        return bondStorageSynchronizer.GetMaxNumberOfBonds();
    }

    std::size_t GapSt::GetNumberOfBonds() const
    {
        uint8_t numberOfBondedAddress = 0;
        std::array<Bonded_Device_Entry_t, maxNumberOfBonds> bondedDevices;

        aci_gap_get_bonded_devices(&numberOfBondedAddress, bondedDevices.data());

        return numberOfBondedAddress;
    }

    bool GapSt::IsDeviceBonded(MacAddress address, services::GapDeviceAddressType addressType) const
    {
        return aci_gap_is_device_bonded(static_cast<uint8_t>(addressType), address.data()) == BLE_STATUS_SUCCESS;
    }

    void GapSt::PairAndBond()
    {
        really_assert(connectionContext.connectionHandle != GapSt::invalidConnection);

        aci_gap_send_pairing_req(connectionContext.connectionHandle, NO_BONDING);
    }

    GapSt::SecureConnection GapSt::SecurityLevelToSecureConnection(services::GapPairing::SecurityLevel level) const
    {
        return (level == services::GapPairing::SecurityLevel::level4) ? SecureConnection::mandatory : SecureConnection::optional;
    }

    uint8_t GapSt::SecurityLevelToMITM(services::GapPairing::SecurityLevel level) const
    {
        // Level 3 and Level 4 require MITM protection
        return (level == services::GapPairing::SecurityLevel::level3 ||
                   level == services::GapPairing::SecurityLevel::level4)
                   ? 1
                   : 0;
    }

    void GapSt::SetSecurityMode(services::GapPairing::SecurityMode mode, services::GapPairing::SecurityLevel level)
    {
        assert(mode == services::GapPairing::SecurityMode::mode1);

        SecureConnection secureConnectionSupport = SecurityLevelToSecureConnection(level);
        uint8_t mitmMode = SecurityLevelToMITM(level);

        aci_gap_set_authentication_requirement(bondingMode, mitmMode, static_cast<uint8_t>(secureConnectionSupport), keypressNotificationSupport, 16, 16, 1, 111111, GAP_PUBLIC_ADDR);
    }

    void GapSt::SetIoCapabilities(services::GapPairing::IoCapabilities caps)
    {
        tBleStatus status = BLE_STATUS_FAILED;

        switch (caps)
        {
            case services::GapPairing::IoCapabilities::display:
                status = aci_gap_set_io_capability(0);
                break;
            case services::GapPairing::IoCapabilities::displayYesNo:
                status = aci_gap_set_io_capability(1);
                break;
            case services::GapPairing::IoCapabilities::keyboard:
                status = aci_gap_set_io_capability(2);
                break;
            case services::GapPairing::IoCapabilities::none:
                status = aci_gap_set_io_capability(3);
                break;
            case services::GapPairing::IoCapabilities::keyboardDisplay:
                status = aci_gap_set_io_capability(4);
                break;
            default:
                std::abort();
                break;
        }

        assert(status == BLE_STATUS_SUCCESS);
    }

    void GapSt::AuthenticateWithPasskey(uint32_t passkey)
    {
        std::abort();
    }

    void GapSt::NumericComparisonConfirm(bool accept)
    {
        std::abort();
    }

    void GapSt::GenerateOutOfBandData()
    {
        really_assert(securityLevel == services::GapPairing::SecurityLevel::level4);

        auto status = hci_le_read_local_p256_public_key();
        really_assert(status == BLE_STATUS_SUCCESS);
    }

    void GapSt::SetOutOfBandData(const services::GapOutOfBandData& outOfBandData)
    {
        really_assert(securityLevel == services::GapPairing::SecurityLevel::level4);

        enum OobDataType
        {
            random = 1,
            confirm
        };

        uint8_t peerAddress = outOfBandData.addressType == services::GapDeviceAddressType::publicAddress ? GAP_PUBLIC_ADDR : GAP_STATIC_RANDOM_ADDR;

        auto result = aci_gap_set_oob_data(OOB_DEVICE_TYPE_REMOTE, peerAddress, outOfBandData.macAddress.data(), static_cast<uint8_t>(OobDataType::random), static_cast<uint8_t>(outOfBandData.randomData.size()), outOfBandData.randomData.begin());
        really_assert(result == BLE_STATUS_SUCCESS);

        result = aci_gap_set_oob_data(OOB_DEVICE_TYPE_REMOTE, peerAddress, outOfBandData.macAddress.data(), static_cast<uint8_t>(OobDataType::confirm), static_cast<uint8_t>(outOfBandData.confirmData.size()), outOfBandData.confirmData.begin());
        really_assert(result == BLE_STATUS_SUCCESS);
    }

    void GapSt::HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event)
    {
        really_assert(event.Connection_Handle == connectionContext.connectionHandle);

        if (event.Status == BLE_STATUS_SUCCESS)
            connectionContext.connectionHandle = GapSt::invalidConnection;
    }

    void GapSt::HandleHciLeConnectionCompleteEvent(const hci_le_connection_complete_event_rp0& event)
    {
        if (event.Status == BLE_STATUS_SUCCESS)
            SetConnectionContext(event.Connection_Handle, static_cast<services::GapDeviceAddressType>(event.Peer_Address_Type), &event.Peer_Address[0]);
    }

    void GapSt::HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event)
    {
        if (event.Status == BLE_STATUS_SUCCESS)
            SetConnectionContext(event.Connection_Handle, static_cast<services::GapDeviceAddressType>(event.Peer_Address_Type), &event.Peer_Address[0]);
    }

    void GapSt::HandleBondLostEvent()
    {
        aci_gap_allow_rebond(connectionContext.connectionHandle);
    }

    void GapSt::HandleMtuExchangeResponseEvent(const aci_att_exchange_mtu_resp_event_rp0& event)
    {
        really_assert(event.Connection_Handle == connectionContext.connectionHandle);
        maxAttMtu = event.Server_RX_MTU;

        AttMtuExchange::NotifyObservers([](auto& observer)
            {
                observer.ExchangedMaxAttMtuSize();
            });
    }

    void GapSt::HandlePairingCompleteEvent(const aci_gap_pairing_complete_event_rp0& event)
    {
        really_assert(event.Connection_Handle == connectionContext.connectionHandle);

        if (IsDeviceBonded(connectionContext.peerAddress, connectionContext.peerAddressType))
        {
            hal::MacAddress address = connectionContext.peerAddress;
            aci_gap_resolve_private_addr(connectionContext.peerAddress.data(), address.data());
            bondStorageSynchronizer.UpdateBondedDevice(address);
            UpdateNrBonds();
        }

        if (event.Status == SMP_PAIRING_STATUS_SUCCESS)
            GapPairing::NotifyObservers([](auto& observer)
                {
                    observer.PairingSuccessfullyCompleted();
                });
        else
            GapPairing::NotifyObservers([&event](auto& observer)
                {
                    observer.PairingFailed(ParserPairingFailure(event.Status, event.Reason));
                });
    }

    void GapSt::HandleHciLeReadLocalP256PublicKeyCompleteEvent(const hci_le_read_local_p256_public_key_complete_event_rp0& event)
    {
        really_assert(event.Status == BLE_STATUS_SUCCESS);

        infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
            {
                HandleOobDataGeneration();
            });
    }

    void GapSt::HandleOobDataGeneration()
    {
        uint8_t addressType = 0;
        hal::MacAddress address{};
        uint8_t dataSize = 0;
        std::array<uint8_t, 16> randomData{};
        std::array<uint8_t, 16> confirmData{};

        /* OOB data generation */
        aci_gap_set_oob_data(0x00, 0x00, nullptr, 0x00, 0x00, nullptr);

        /* OOB data recovery */
        auto status = aci_gap_get_oob_data(0x01, &addressType, address.data(), &dataSize, randomData.data());
        really_assert(status == BLE_STATUS_SUCCESS && dataSize == randomData.size());
        status = aci_gap_get_oob_data(0x02, &addressType, address.data(), &dataSize, confirmData.data());
        really_assert(status == BLE_STATUS_SUCCESS && dataSize == confirmData.size());

        auto addressTypeConverted = addressType == GAP_PUBLIC_ADDR ? services::GapDeviceAddressType::publicAddress : services::GapDeviceAddressType::randomAddress;
        services::GapOutOfBandData outOfBandData = { address, addressTypeConverted, infra::MakeConstByteRange(randomData), infra::MakeConstByteRange(confirmData) };

        infra::Subject<services::GapPairingObserver>::NotifyObservers([outOfBandData](auto& observer)
            {
                observer.OutOfBandDataGenerated(outOfBandData);
            });
    }

    void GapSt::SetAddress(const hal::MacAddress& address, services::GapDeviceAddressType addressType) const
    {
        uint8_t offset = addressType == services::GapDeviceAddressType::publicAddress ? CONFIG_DATA_PUBADDR_OFFSET : CONFIG_DATA_RANDOM_ADDRESS_OFFSET;
        uint8_t length = addressType == services::GapDeviceAddressType::publicAddress ? CONFIG_DATA_PUBADDR_LEN : CONFIG_DATA_RANDOM_ADDRESS_LEN;

        aci_hal_write_config_data(offset, length, address.data());
    }

    void GapSt::HciEvent(hci_event_pckt& event)
    {
        switch (event.evt)
        {
            case HCI_DISCONNECTION_COMPLETE_EVT_CODE:
                HandleHciDisconnectEvent(*reinterpret_cast<const hci_disconnection_complete_event_rp0*>(event.data));
                break;
            case HCI_LE_META_EVT_CODE:
                HandleHciLeMetaEvent(*reinterpret_cast<const evt_le_meta_event*>(event.data));
                break;
            case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
                HandleHciVendorSpecificDebugEvent(*reinterpret_cast<const evt_blecore_aci*>(event.data));
                break;
            default:
                break;
        }
    }

    void GapSt::HandleHciLeMetaEvent(const evt_le_meta_event& metaEvent)
    {
        switch (metaEvent.subevent)
        {
            case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
                HandleHciLeConnectionCompleteEvent(*reinterpret_cast<const hci_le_connection_complete_event_rp0*>(metaEvent.data));
                break;
            case HCI_LE_ADVERTISING_REPORT_SUBEVT_CODE:
                HandleHciLeAdvertisingReportEvent(*reinterpret_cast<const hci_le_advertising_report_event_rp0*>(metaEvent.data));
                break;
            case HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVT_CODE:
                HandleHciLeConnectionUpdateCompleteEvent(*reinterpret_cast<const hci_le_connection_update_complete_event_rp0*>(metaEvent.data));
                break;
            case HCI_LE_DATA_LENGTH_CHANGE_SUBEVT_CODE:
                HandleHciLeDataLengthChangeEvent(*reinterpret_cast<const hci_le_data_length_change_event_rp0*>(metaEvent.data));
                break;
            case HCI_LE_PHY_UPDATE_COMPLETE_SUBEVT_CODE:
                HandleHciLePhyUpdateCompleteEvent(*reinterpret_cast<const hci_le_phy_update_complete_event_rp0*>(metaEvent.data));
                break;
            case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE:
                HandleHciLeEnhancedConnectionCompleteEvent(*reinterpret_cast<const hci_le_enhanced_connection_complete_event_rp0*>(metaEvent.data));
                break;
            case HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_SUBEVT_CODE:
                HandleHciLeReadLocalP256PublicKeyCompleteEvent(*reinterpret_cast<const hci_le_read_local_p256_public_key_complete_event_rp0*>(metaEvent.data));
                break;
            default:
                break;
        }
    }

    void GapSt::HandleHciVendorSpecificDebugEvent(const evt_blecore_aci& event)
    {
        switch (event.ecode)
        {
            case ACI_GAP_PAIRING_COMPLETE_VSEVT_CODE:
                HandlePairingCompleteEvent(*reinterpret_cast<const aci_gap_pairing_complete_event_rp0*>(event.data));
                break;
            case ACI_GAP_BOND_LOST_VSEVT_CODE:
                HandleBondLostEvent();
                break;
            case ACI_GAP_NUMERIC_COMPARISON_VALUE_VSEVT_CODE:
                HandleNumericComparisonValueEvent(*reinterpret_cast<const aci_gap_numeric_comparison_value_event_rp0*>(event.data));
                break;
            case ACI_GAP_PROC_COMPLETE_VSEVT_CODE:
                HandleGapProcedureCompleteEvent(*reinterpret_cast<const aci_gap_proc_complete_event_rp0*>(event.data));
                break;
            case ACI_GATT_PROC_COMPLETE_VSEVT_CODE:
                HandleGattCompleteEvent(*reinterpret_cast<const aci_gatt_proc_complete_event_rp0*>(event.data));
                break;
            case ACI_L2CAP_CONNECTION_UPDATE_REQ_VSEVT_CODE:
                HandleL2capConnectionUpdateRequestEvent(*reinterpret_cast<const aci_l2cap_connection_update_req_event_rp0*>(event.data));
                break;
            case ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE:
                HandleMtuExchangeResponseEvent(*reinterpret_cast<const aci_att_exchange_mtu_resp_event_rp0*>(event.data));
                break;
            default:
                break;
        }
    }

    void GapSt::SetConnectionContext(uint16_t connectionHandle, services::GapDeviceAddressType peerAddressType, const uint8_t* peerAddress)
    {
        maxAttMtu = defaultMaxAttMtuSize;
        connectionContext.connectionHandle = connectionHandle;
        connectionContext.peerAddressType = peerAddressType;
        std::copy_n(peerAddress, connectionContext.peerAddress.size(), std::begin(connectionContext.peerAddress));
    }

    void GapSt::UpdateNrBonds()
    {
        auto nrBonds = this->GetNumberOfBonds();
        services::GapBonding::NotifyObservers([nrBonds](auto& obs)
            {
                obs.NumberOfBondsChanged(nrBonds);
            });
    }
}
