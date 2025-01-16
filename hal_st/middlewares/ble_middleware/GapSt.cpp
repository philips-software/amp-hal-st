#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "ble_gap_aci.h"
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

    const services::GapConnectionParameters GapSt::connectionParameters{
        6,
        6,
        0,
        500,
    };

    GapSt::GapSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration)
        : HciEventSink(hciEventSource)
        , bondStorageSynchronizer(bondStorageSynchronizer)
    {
        connectionContext.connectionHandle = GapSt::invalidConnection;

        // HCI Reset to synchronise BLE Stack
        hci_reset();

        // Write Identity root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, configuration.rootKeys.identity.data());

        // Write Encryption root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, configuration.rootKeys.encryption.data());

        aci_hal_set_tx_power_level(1, configuration.txPowerLevel);
        aci_gatt_init();

        aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, configuration.address.data());

        SVCCTL_Init();
    }

    uint16_t GapSt::EffectiveMaxAttMtuSize() const
    {
        return maxAttMtu;
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

    void GapSt::Pair()
    {
        really_assert(connectionContext.connectionHandle != GapSt::invalidConnection);

        aci_gap_send_pairing_req(connectionContext.connectionHandle, NO_BONDING);
    }

    void GapSt::SetSecurityMode(services::GapPairing::SecurityMode mode, services::GapPairing::SecurityLevel level)
    {
        assert(mode == services::GapPairing::SecurityMode::mode1);

        enum class SecureConnection : uint8_t
        {
            notSupported = 0,
            optional = 1,
            mandatory
        };

        SecureConnection secureConnectionSupport = (level == services::GapPairing::SecurityLevel::level4) ? SecureConnection::mandatory : SecureConnection::optional;
        uint8_t mitmMode = (level == services::GapPairing::SecurityLevel::level3 || level == services::GapPairing::SecurityLevel::level4) ? 1 : 0;

        aci_gap_set_authentication_requirement(bondingMode, mitmMode, static_cast<uint8_t>(secureConnectionSupport), keypressNotificationSupport, 16, 16, 0, 111111, GAP_PUBLIC_ADDR);
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

    void GapSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        auto disconnectionCompleteEvent = *reinterpret_cast<hci_disconnection_complete_event_rp0*>(eventPacket.data);

        really_assert(disconnectionCompleteEvent.Connection_Handle == connectionContext.connectionHandle);

        if (disconnectionCompleteEvent.Status == BLE_STATUS_SUCCESS)
            connectionContext.connectionHandle = GapSt::invalidConnection;
    }

    void GapSt::HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto connectionCompleteEvent = *reinterpret_cast<hci_le_connection_complete_event_rp0*>(metaEvent->data);

        if (connectionCompleteEvent.Status == BLE_STATUS_SUCCESS)
            SetConnectionContext(connectionCompleteEvent.Connection_Handle, static_cast<services::GapDeviceAddressType>(connectionCompleteEvent.Peer_Address_Type), &connectionCompleteEvent.Peer_Address[0]);
    }

    void GapSt::HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto connectionCompleteEvt = *reinterpret_cast<hci_le_enhanced_connection_complete_event_rp0*>(metaEvent->data);

        if (connectionCompleteEvt.Status == BLE_STATUS_SUCCESS)
            SetConnectionContext(connectionCompleteEvt.Connection_Handle, static_cast<services::GapDeviceAddressType>(connectionCompleteEvt.Peer_Address_Type), &connectionCompleteEvt.Peer_Address[0]);
    }

    void GapSt::HandleBondLostEvent(evt_blecore_aci* vendorEvent)
    {
        aci_gap_allow_rebond(connectionContext.connectionHandle);
    }

    void GapSt::HandleMtuExchangeResponseEvent(evt_blecore_aci* vendorEvent)
    {
        auto attExchangeMtuResponse = *reinterpret_cast<aci_att_exchange_mtu_resp_event_rp0*>(vendorEvent->data);

        really_assert(attExchangeMtuResponse.Connection_Handle == connectionContext.connectionHandle);
        maxAttMtu = attExchangeMtuResponse.Server_RX_MTU;

        AttMtuExchange::NotifyObservers([](auto& observer)
            {
                observer.ExchangedMaxAttMtuSize();
            });
    }

    void GapSt::HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        auto pairingComplete = reinterpret_cast<aci_gap_pairing_complete_event_rp0*>(vendorEvent->data);

        really_assert(pairingComplete->Connection_Handle == connectionContext.connectionHandle);

        if (IsDeviceBonded(connectionContext.peerAddress, connectionContext.peerAddressType))
        {
            hal::MacAddress address = connectionContext.peerAddress;
            aci_gap_resolve_private_addr(connectionContext.peerAddress.data(), address.data());
            bondStorageSynchronizer.UpdateBondedDevice(address);
            UpdateNrBonds();
        }

        if (pairingComplete->Status == SMP_PAIRING_STATUS_SUCCESS)
            GapPairing::NotifyObservers([](auto& observer)
                {
                    observer.PairingSuccessfullyCompleted();
                });
        else
            GapPairing::NotifyObservers([&pairingComplete](auto& observer)
                {
                    observer.PairingFailed(ParserPairingFailure(pairingComplete->Status, pairingComplete->Reason));
                });
    }

    void GapSt::SetAddress(const hal::MacAddress& address, services::GapDeviceAddressType addressType)
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
                HandleHciDisconnectEvent(event);
                break;
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

    void GapSt::HandleHciLeMetaEvent(hci_event_pckt& eventPacket)
    {
        auto metaEvent = reinterpret_cast<evt_le_meta_event*>(eventPacket.data);

        switch (metaEvent->subevent)
        {
            case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
                HandleHciLeConnectionCompleteEvent(metaEvent);
                break;
            case HCI_LE_ADVERTISING_REPORT_SUBEVT_CODE:
                HandleHciLeAdvertisingReportEvent(metaEvent);
                break;
            case HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVT_CODE:
                HandleHciLeConnectionUpdateCompleteEvent(metaEvent);
                break;
            case HCI_LE_DATA_LENGTH_CHANGE_SUBEVT_CODE:
                HandleHciLeDataLengthChangeEvent(metaEvent);
                break;
            case HCI_LE_PHY_UPDATE_COMPLETE_SUBEVT_CODE:
                HandleHciLePhyUpdateCompleteEvent(metaEvent);
                break;
            case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE:
                HandleHciLeEnhancedConnectionCompleteEvent(metaEvent);
                break;
            default:
                break;
        }
    }

    void GapSt::HandleHciVendorSpecificDebugEvent(hci_event_pckt& eventPacket)
    {
        auto vendorEvent = reinterpret_cast<evt_blecore_aci*>(eventPacket.data);

        switch (vendorEvent->ecode)
        {
            case ACI_GAP_PAIRING_COMPLETE_VSEVT_CODE:
                HandlePairingCompleteEvent(vendorEvent);
                break;
            case ACI_GAP_BOND_LOST_VSEVT_CODE:
                HandleBondLostEvent(vendorEvent);
                break;
            case ACI_GAP_PROC_COMPLETE_VSEVT_CODE:
                HandleGapProcedureCompleteEvent(vendorEvent);
                break;
            case ACI_GATT_PROC_COMPLETE_VSEVT_CODE:
                HandleGattCompleteEvent(vendorEvent);
                break;
            case ACI_L2CAP_CONNECTION_UPDATE_REQ_VSEVT_CODE:
                HandleL2capConnectionUpdateRequestEvent(vendorEvent);
                break;
            case ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE:
                HandleMtuExchangeResponseEvent(vendorEvent);
                break;
            default:
                break;
        }
    }

    void GapSt::SetConnectionContext(uint16_t connectionHandle, services::GapDeviceAddressType peerAddressType, uint8_t* peerAddress)
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
