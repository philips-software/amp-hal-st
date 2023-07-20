#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "ble_gap_aci.h"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "services/ble/Gap.hpp"
#include "shci.h"
#include "stm32wbxx_ll_system.h"

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

    GapSt::GapSt(hal::HciEventSource& hciEventSource, hal::MacAddress& address, const RootKeys& rootKeys, uint16_t& maxAttMtuSize, uint8_t& txPowerLevel, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t& bleBondsStorage)
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

        // HCI Reset to synchronise BLE Stack
        hci_reset();

        // Write Identity root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, rootKeys.identity.data());

        // Write Encryption root key used to derive LTK and CSRK
        aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, rootKeys.encryption.data());

        bondStorageSynchronizer.Emplace(bondStorageSynchronizerCreator);

        aci_hal_set_tx_power_level(1, txPowerLevel);
        aci_gatt_init();

        aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, address.data());

        SVCCTL_Init();

        hci_le_write_suggested_default_data_length(services::GapConnectionParameters::connectionInitialMaxTxOctets, services::GapConnectionParameters::connectionInitialMaxTxTime);
        hci_le_set_default_phy(allPhys, speed2Mbps, speed2Mbps);
    }

    uint16_t GapSt::EffectiveMaxAttMtuSize() const
    {
        return maxAttMtu;
    }

    void GapSt::RemoveAllBonds()
    {
        (*bondStorageSynchronizer)->RemoveAllBonds();
    }

    void GapSt::RemoveOldestBond()
    {
        std::abort();
    }

    std::size_t GapSt::GetMaxNumberOfBonds() const
    {
        if (bondStorageSynchronizer)
            return (*bondStorageSynchronizer)->GetMaxNumberOfBonds();

        return 0;
    }

    std::size_t GapSt::GetNumberOfBonds() const
    {
        uint8_t numberOfBondedAddress = 0;
        std::array<Bonded_Device_Entry_t, maxNumberOfBonds> bondedDevices;
        aci_gap_get_bonded_devices(&numberOfBondedAddress, bondedDevices.data());

        return numberOfBondedAddress;
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
        really_assert(disconnectionCompleteEvent.Status == BLE_STATUS_SUCCESS);

        connectionContext.connectionHandle = GapSt::invalidConnection;
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

        if (aci_gap_is_device_bonded(connectionContext.peerAddressType, connectionContext.peerAddress.data()) == BLE_STATUS_SUCCESS)
        {
            hal::MacAddress address = connectionContext.peerAddress;
            aci_gap_resolve_private_addr(connectionContext.peerAddress.data(), address.data());
            (*bondStorageSynchronizer)->UpdateBondedDevice(address);
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

    void GapSt::SetConnectionContext(uint16_t connectionHandle, uint8_t peerAddressType, uint8_t* peerAddress)
    {
        static constexpr auto deducePeerAddressType = [](auto peerAddressType)
            {
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
        connectionContext.peerAddressType = deducePeerAddressType(peerAddressType);
        std::copy_n(peerAddress, connectionContext.peerAddress.size(), std::begin(connectionContext.peerAddress));
    }
}
