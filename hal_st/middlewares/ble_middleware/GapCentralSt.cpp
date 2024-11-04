#include "hal_st/middlewares/ble_middleware/GapCentralSt.hpp"
#include "ble_defs.h"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include <chrono>
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

        services::GapAdvertisingEventAddressType ToAdvertisingAddressType(uint8_t addressType)
        {
            return static_cast<services::GapAdvertisingEventAddressType>(addressType);
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
        Initialize(configuration.gapService);

        infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer)
            {
                observer.StateChanged(services::GapState::standby);
            });
    }

    void GapCentralSt::Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType, infra::Duration initiatingTimeout)
    {
        auto peerAddress = addressType == services::GapDeviceAddressType::publicAddress ? GAP_PUBLIC_ADDR : GAP_STATIC_RANDOM_ADDR;

        aci_gap_create_connection(
            leScanInterval, leScanWindow, peerAddress, macAddress.data(), GAP_RESOLVABLE_PRIVATE_ADDR,
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
            aci_gap_start_general_discovery_proc(leScanInterval, leScanWindow, GAP_RESOLVABLE_PRIVATE_ADDR, filterDuplicatesEnabled);
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

    void GapCentralSt::AllowPairing(bool)
    {}

    void GapCentralSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        GapSt::HandleHciDisconnectEvent(eventPacket);

        infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer)
            {
                observer.StateChanged(services::GapState::standby);
            });
    }

    void GapCentralSt::HandleHciLeAdvertisingReportEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLeAdvertisingReportEvent(metaEvent);

        auto advertisingEvent = *reinterpret_cast<hci_le_advertising_report_event_rp0*>(metaEvent->data);

        for (uint8_t i = 0; i < advertisingEvent.Num_Reports; i++)
            HandleAdvertisingReport(advertisingEvent.Advertising_Report[i]);
    }

    void GapCentralSt::HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLeConnectionCompleteEvent(metaEvent);
        initiatingStateTimer.Cancel();
    }

    void GapCentralSt::HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLeEnhancedConnectionCompleteEvent(metaEvent);
        initiatingStateTimer.Cancel();
    }

    void GapCentralSt::HandleGapProcedureCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        GapSt::HandleGapProcedureCompleteEvent(vendorEvent);

        auto gapProcedureEvent = *reinterpret_cast<aci_gap_proc_complete_event_rp0*>(vendorEvent->data);

        really_assert(gapProcedureEvent.Status == BLE_STATUS_SUCCESS);

        if (gapProcedureEvent.Procedure_Code == GAP_LIMITED_DISCOVERY_PROC || gapProcedureEvent.Procedure_Code == GAP_GENERAL_DISCOVERY_PROC)
            HandleGapDiscoveryProcedureEvent();
        else if (gapProcedureEvent.Procedure_Code == GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC)
            HandleGapDirectConnectionProcedureCompleteEvent();
    }

    void GapCentralSt::HandleGattCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        GapSt::HandleGattCompleteEvent(vendorEvent);

        auto gattCompleteEvent = *reinterpret_cast<aci_gatt_proc_complete_event_rp0*>(vendorEvent->data);

        if (onMtuExchangeDone && gattCompleteEvent.Error_Code == BLE_STATUS_SUCCESS)
        {
            really_assert(gattCompleteEvent.Connection_Handle == connectionContext.connectionHandle);
            onMtuExchangeDone();
        }
    }

    void GapCentralSt::HandleL2capConnectionUpdateRequestEvent(evt_blecore_aci* vendorEvent)
    {
        GapSt::HandleL2capConnectionUpdateRequestEvent(vendorEvent);

        auto identifier = reinterpret_cast<aci_l2cap_connection_update_req_event_rp0*>(vendorEvent->data)->Identifier;

        infra::EventDispatcherWithWeakPtr::Instance().Schedule([this, identifier]()
            {
                auto status = aci_l2cap_connection_parameter_update_resp(
                    connectionContext.connectionHandle, connectionUpdateParameters.minConnIntMultiplier, connectionUpdateParameters.maxConnIntMultiplier,
                    connectionUpdateParameters.slaveLatency, connectionUpdateParameters.supervisorTimeoutMs,
                    minConnectionEventLength, maxConnectionEventLength, identifier, rejectParameters);
                assert(status == BLE_STATUS_SUCCESS);
            });

        infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
            {
                MtuExchange();
            });
    }

    void GapCentralSt::HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLeDataLengthChangeEvent(metaEvent);

        auto dataLengthChangeEvent = *reinterpret_cast<hci_le_data_length_change_event_rp0*>(metaEvent->data);

        really_assert(dataLengthChangeEvent.Connection_Handle == connectionContext.connectionHandle);

        if (!IsTxDataLengthConfigured(dataLengthChangeEvent))
            onMtuExchangeDone = [this]()
            {
                infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
                    {
                        SetDataLength();
                    });
            };
        else
            onMtuExchangeDone = [this]()
            {
                infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
                    {
                        SetPhy();
                    });
            };

        if (onDataLengthChanged)
            onDataLengthChanged();
    }

    void GapCentralSt::HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLePhyUpdateCompleteEvent(metaEvent);

        auto phyUpdateCompleteEvent = *reinterpret_cast<hci_le_phy_update_complete_event_rp0*>(metaEvent->data);

        really_assert(phyUpdateCompleteEvent.Connection_Handle == connectionContext.connectionHandle);

        onMtuExchangeDone = nullptr;

        infra::Subject<services::GapCentralObserver>::NotifyObservers([](services::GapCentralObserver& observer)
            {
                observer.StateChanged(services::GapState::connected);
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

    void GapCentralSt::MtuExchange() const
    {
        auto status = aci_gatt_exchange_config(this->connectionContext.connectionHandle);
        assert(status == BLE_STATUS_SUCCESS);
    }

    void GapCentralSt::SetDataLength()
    {
        onDataLengthChanged = [this]()
        {
            infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
                {
                    SetPhy();
                });
        };

        auto status = hci_le_set_data_length(this->connectionContext.connectionHandle, services::GapConnectionParameters::connectionInitialMaxTxOctets, services::GapConnectionParameters::connectionInitialMaxTxTime);
        assert(status == BLE_STATUS_SUCCESS);
    }

    void GapCentralSt::SetPhy() const
    {
        auto status = hci_le_set_phy(this->connectionContext.connectionHandle, GapSt::allPhys, GapSt::speed2Mbps, GapSt::speed2Mbps, 0);
        assert(status == BLE_STATUS_SUCCESS);
    }

    void GapCentralSt::HandleAdvertisingReport(const Advertising_Report_t& advertisingReport)
    {
        services::GapAdvertisingReport discoveredDevice;

        auto advertisementData = const_cast<uint8_t*>(&advertisingReport.Length_Data) + 1;
        std::copy_n(std::begin(advertisingReport.Address), discoveredDevice.address.size(), std::begin(discoveredDevice.address));
        discoveredDevice.eventType = ToAdvertisingEventType(advertisingReport.Event_Type);
        discoveredDevice.addressType = ToAdvertisingAddressType(advertisingReport.Address_Type);
        discoveredDevice.data = infra::MemoryRange(advertisementData, advertisementData + advertisingReport.Length_Data);
        discoveredDevice.rssi = static_cast<int8_t>(*const_cast<uint8_t*>(advertisementData + advertisingReport.Length_Data));

        infra::Subject<services::GapCentralObserver>::NotifyObservers([&discoveredDevice](auto& observer)
            {
                observer.DeviceDiscovered(discoveredDevice);
            });
    }

    void GapCentralSt::Initialize(const GapService& gapService)
    {
        uint16_t gapServiceHandle, gapDevNameCharHandle, gapAppearanceCharHandle;

        aci_gap_init(GAP_CENTRAL_ROLE, PRIVACY_ENABLED, gapService.deviceName.size(), &gapServiceHandle, &gapDevNameCharHandle, &gapAppearanceCharHandle);
        aci_gatt_update_char_value(gapServiceHandle, gapDevNameCharHandle, 0, gapService.deviceName.size(), reinterpret_cast<const uint8_t*>(gapService.deviceName.data()));
        aci_gatt_update_char_value(gapServiceHandle, gapAppearanceCharHandle, 0, sizeof(gapService.appearance), reinterpret_cast<const uint8_t*>(&gapService.appearance));

        SetIoCapabilities(services::GapPairing::IoCapabilities::none);
        SetSecurityMode(services::GapPairing::SecurityMode::mode1, services::GapPairing::SecurityLevel::level1);
    }
}
