#include "hal_st/middlewares/ble_middleware/GapCentralSt.hpp"
#include "ble_defs.h"
#include "hal/interfaces/MacAddress.hpp"
#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "infra/util/Function.hpp"
#include "services/ble/Gap.hpp"
#include <algorithm>
#include <cmath>

namespace hal
{
    const services::GapConnectionParameters GapCentralSt::connectionUpdateParameters{
        6, // 7.5 ms
        6, // 7.5 ms
        0,
        50, // 500 ms
    };

    // Connection Interval parameters
    const uint16_t minConnectionEventLength = 0;
    const uint16_t maxConnectionEventLength = 0x280; // 400 ms

    // Discovery parameters
    const uint8_t filterDuplicatesEnabled = 1;
    const uint8_t acceptParameters = 1;
    const uint8_t rejectParameters = 0;

    namespace
    {
        services::GapAdvertisingEventType ToAdvertisingEventType(uint8_t eventType)
        {
            return static_cast<services::GapAdvertisingEventType>(eventType);
        }

        services::GapDeviceAddressType ToAdvertisingAddressType(uint8_t addressType)
        {
            return static_cast<services::GapDeviceAddressType>(addressType);
        }

        bool IsTxDataLengthConfigured(const hci_le_data_length_change_event_rp0& dataLengthChangeEvent)
        {
            return dataLengthChangeEvent.MaxTxOctets == services::GapConnectionParameters::connectionInitialMaxTxOctets &&
                   dataLengthChangeEvent.MaxTxTime == services::GapConnectionParameters::connectionInitialMaxTxTime;
        }
    }

    GapCentralSt::GapCentralSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration)
        : GapSt(hciEventSource, bondStorageSynchronizer, configuration)
    {
        Initialize(configuration);

        infra::EventDispatcher::Instance().Schedule([this]
            {
                infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer)
                    {
                        observer.StateChanged(services::GapState::standby);
                    });
            });
    }

    void GapCentralSt::Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType, infra::Duration initiatingTimeout)
    {
        auto peerAddress = addressType == services::GapDeviceAddressType::publicAddress ? GAP_PUBLIC_ADDR : GAP_STATIC_RANDOM_ADDR;

        aci_gap_create_connection(
            leScanInterval, leScanWindow, peerAddress, macAddress.data(), ownAddressType,
            connectionUpdateParameters.minConnIntMultiplier, connectionUpdateParameters.maxConnIntMultiplier,
            connectionUpdateParameters.slaveLatency, connectionUpdateParameters.supervisorTimeoutMs,
            minConnectionEventLength, maxConnectionEventLength);

        infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer)
            {
                observer.StateChanged(services::GapState::initiating);
            });

        initiatingStateTimer.Start(initiatingTimeout, [this]()
            {
                aci_gap_terminate_gap_proc(GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC);
            });
    }

    void GapCentralSt::CancelConnect()
    {
        if (initiatingStateTimer.Armed())
        {
            initiatingStateTimer.Cancel();
            aci_gap_terminate_gap_proc(GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC);
        }
    }

    void GapCentralSt::Disconnect()
    {
        aci_gap_terminate(connectionContext.connectionHandle, remoteUserTerminatedConnection);
    }

    void GapCentralSt::SetAddress(hal::MacAddress macAddress, services::GapDeviceAddressType addressType)
    {
        GapSt::SetAddress(macAddress, addressType);
    }

    void GapCentralSt::StartDeviceDiscovery()
    {
        if (!std::exchange(discovering, true))
        {
            aci_gap_start_general_discovery_proc(leScanInterval, leScanWindow, ownAddressType, filterDuplicatesEnabled);
            infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer)
                {
                    observer.StateChanged(services::GapState::scanning);
                });
        }
    }

    void GapCentralSt::StopDeviceDiscovery()
    {
        if (std::exchange(discovering, false))
            aci_gap_terminate_gap_proc(GAP_GENERAL_DISCOVERY_PROC);
    }

    std::optional<hal::MacAddress> GapCentralSt::ResolvePrivateAddress(hal::MacAddress address) const
    {
        hal::MacAddress identityAddress;
        if (aci_gap_resolve_private_addr(address.data(), identityAddress.data()) != BLE_STATUS_SUCCESS)
            return std::nullopt;
        return std::make_optional(identityAddress);
    }

    void GapCentralSt::AllowPairing(bool)
    {}

    void GapCentralSt::NumericComparisonConfirm(bool accept)
    {
        aci_gap_numeric_comparison_value_confirm_yesno(connectionContext.connectionHandle, accept ? 1 : 0);
    }

    void GapCentralSt::HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event)
    {
        GapSt::HandleHciDisconnectEvent(event);

        infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer)
            {
                observer.StateChanged(services::GapState::standby);
            });
    }

    void GapCentralSt::HandleHciLeAdvertisingReportEvent(const hci_le_advertising_report_event_rp0& event)
    {
        GapSt::HandleHciLeAdvertisingReportEvent(event);

        for (uint8_t i = 0; i != event.Num_Reports; ++i)
            HandleAdvertisingReport(event.Advertising_Report[i]);
    }

    void GapCentralSt::HandleHciLeConnectionCompleteEvent(const hci_le_connection_complete_event_rp0& event)
    {
        HandleConnectionCompleteCommon(event.Status);
        GapSt::HandleHciLeConnectionCompleteEvent(event);
    }

    void GapCentralSt::HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event)
    {
        HandleConnectionCompleteCommon(event.Status);
        GapSt::HandleHciLeEnhancedConnectionCompleteEvent(event);
    }

    void GapCentralSt::UpdateStateOnConnectionComplete(uint8_t status)
    {
        services::GapState state = status == BLE_STATUS_SUCCESS ? services::GapState::connected : services::GapState::standby;

        infra::Subject<services::GapCentralObserver>::NotifyObservers([state](services::GapCentralObserver& observer)
            {
                observer.StateChanged(state);
            });
    }

    void GapCentralSt::HandleGapProcedureCompleteEvent(const aci_gap_proc_complete_event_rp0& event)
    {
        GapSt::HandleGapProcedureCompleteEvent(event);

        really_assert(event.Status == BLE_STATUS_SUCCESS);

        if (event.Procedure_Code == GAP_LIMITED_DISCOVERY_PROC || event.Procedure_Code == GAP_GENERAL_DISCOVERY_PROC)
            HandleGapDiscoveryProcedureEvent();
        else if (event.Procedure_Code == GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC)
            HandleGapDirectConnectionProcedureCompleteEvent();
    }

    void GapCentralSt::HandleGattCompleteEvent(const aci_gatt_proc_complete_event_rp0& event)
    {
        GapSt::HandleGattCompleteEvent(event);

        if (event.Error_Code == BLE_STATUS_SUCCESS)
            really_assert(event.Connection_Handle == connectionContext.connectionHandle);
    }

    void GapCentralSt::HandleL2capConnectionUpdateRequestEvent(const aci_l2cap_connection_update_req_event_rp0& event)
    {
        GapSt::HandleL2capConnectionUpdateRequestEvent(event);

        auto identifier = event.Identifier;

        infra::EventDispatcherWithWeakPtr::Instance().Schedule([this, identifier]()
            {
                auto status = aci_l2cap_connection_parameter_update_resp(
                    connectionContext.connectionHandle, connectionUpdateParameters.minConnIntMultiplier, connectionUpdateParameters.maxConnIntMultiplier,
                    connectionUpdateParameters.slaveLatency, connectionUpdateParameters.supervisorTimeoutMs,
                    minConnectionEventLength, maxConnectionEventLength, identifier, rejectParameters);
                assert(status == BLE_STATUS_SUCCESS);
            });
    }

    void GapCentralSt::HandleHciLeDataLengthChangeEvent(const hci_le_data_length_change_event_rp0& event)
    {
        GapSt::HandleHciLeDataLengthChangeEvent(event);

        really_assert(event.Connection_Handle == connectionContext.connectionHandle);

        if (!IsTxDataLengthConfigured(event))
        {
            infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
                {
                    SetDataLength();
                });
        }
    }

    void GapCentralSt::HandleHciLePhyUpdateCompleteEvent(const hci_le_phy_update_complete_event_rp0& event)
    {
        GapSt::HandleHciLePhyUpdateCompleteEvent(event);

        really_assert(event.Connection_Handle == connectionContext.connectionHandle);
    }

    void GapCentralSt::HandleNumericComparisonValueEvent(const aci_gap_numeric_comparison_value_event_rp0& event)
    {
        GapSt::HandleNumericComparisonValueEvent(event);

        really_assert(event.Connection_Handle == connectionContext.connectionHandle);

        GapPairing::NotifyObservers([passkey = event.Numeric_Value](auto& observer)
            {
                observer.DisplayPasskey(static_cast<int32_t>(passkey), true);
            });
    }

    void GapCentralSt::HandleGapDiscoveryProcedureEvent()
    {
        discovering = false;

        infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer)
            {
                observer.StateChanged(services::GapState::standby);
            });
    }

    void GapCentralSt::HandleGapDirectConnectionProcedureCompleteEvent()
    {
        if (!initiatingStateTimer.Armed())
            infra::Subject<services::GapCentralObserver>::NotifyObservers([](services::GapCentralObserver& observer)
                {
                    observer.StateChanged(services::GapState::standby);
                });
    }

    void GapCentralSt::SetDataLength()
    {
        auto status = hci_le_set_data_length(this->connectionContext.connectionHandle, services::GapConnectionParameters::connectionInitialMaxTxOctets, services::GapConnectionParameters::connectionInitialMaxTxTime);
        assert(status == BLE_STATUS_SUCCESS);
    }

    void GapCentralSt::HandleAdvertisingReport(const Advertising_Report_t& advertisingReport)
    {
        services::GapAdvertisingReport discoveredDevice;

        auto advertisementData = const_cast<uint8_t*>(&advertisingReport.Length_Data) + 1;
        std::copy_n(std::begin(advertisingReport.Address), discoveredDevice.address.size(), std::begin(discoveredDevice.address));
        discoveredDevice.eventType = ToAdvertisingEventType(advertisingReport.Event_Type);
        discoveredDevice.addressType = ToAdvertisingAddressType(advertisingReport.Address_Type);
        discoveredDevice.data.assign(advertisementData, advertisementData + advertisingReport.Length_Data);
        discoveredDevice.rssi = static_cast<int8_t>(*const_cast<uint8_t*>(advertisementData + advertisingReport.Length_Data));

        infra::Subject<services::GapCentralObserver>::NotifyObservers([&discoveredDevice](auto& observer)
            {
                observer.DeviceDiscovered(discoveredDevice);
            });
    }

    GapSt::SecureConnection GapCentralSt::SecurityLevelToSecureConnection(services::GapPairing::SecurityLevel level) const
    {
        if (level == services::GapPairing::SecurityLevel::level1)
            return SecureConnection::notSupported;
        else if (level == services::GapPairing::SecurityLevel::level4)
            return SecureConnection::mandatory;

        return SecureConnection::optional;
    }

    void GapCentralSt::Initialize(const Configuration& configuration)
    {
        uint16_t gapServiceHandle, gapDevNameCharHandle, gapAppearanceCharHandle;

        aci_gap_init(GAP_CENTRAL_ROLE, configuration.privacy ? PRIVACY_ENABLED : PRIVACY_DISABLED, configuration.gapService.deviceName.size(), &gapServiceHandle, &gapDevNameCharHandle, &gapAppearanceCharHandle);
        aci_gatt_update_char_value(gapServiceHandle, gapDevNameCharHandle, 0, configuration.gapService.deviceName.size(), reinterpret_cast<const uint8_t*>(configuration.gapService.deviceName.data()));
        aci_gatt_update_char_value(gapServiceHandle, gapAppearanceCharHandle, 0, sizeof(configuration.gapService.appearance), reinterpret_cast<const uint8_t*>(&configuration.gapService.appearance));

        SetIoCapabilities(configuration.security.ioCapabilities);
        SetSecurityMode(configuration.security.securityMode, configuration.security.securityLevel);
        hci_le_set_default_phy(allPhys, speed2Mbps, speed2Mbps);
    }

    void GapCentralSt::HandleConnectionCompleteCommon(uint8_t status)
    {
        UpdateStateOnConnectionComplete(status);
        initiatingStateTimer.Cancel();

        if (status == BLE_STATUS_SUCCESS)
        {
            infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
                {
                    SetDataLength();
                });
        }
    }
}
