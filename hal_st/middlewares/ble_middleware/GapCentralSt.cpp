#include "hal_st/middlewares/ble_middleware/GapCentralSt.hpp"
#include "ble_defs.h"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "infra/stream/ByteInputStream.hpp"

namespace hal
{
    const services::GapConnectionParameters GapCentralSt::connectionUpdateParameters
    {
        6, // 7.5 ms
        6, // 7.5 ms
        0,
        500,
    };

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
    }

    GapCentralSt::GapCentralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService gapService, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t* bleBondsStorage)
        : GapSt(hciEventSource, address, rootKeys, maxAttMtuSize, txPowerLevel, bondStorageSynchronizerCreator, *bleBondsStorage)
    {
        Initialize(gapService);

        infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer) { observer.StateChanged(services::GapState::standby); });
    }

    void GapCentralSt::Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType)
    {
        auto peerAddress = addressType == services::GapDeviceAddressType::publicAddress ? PUBLIC_ADDR : RANDOM_ADDR;

        aci_gap_create_connection(
            leScanInterval, leScanWindow, peerAddress, macAddress.data(), RESOLVABLE_PRIVATE_ADDR,
            connectionUpdateParameters.minConnIntMultiplier, connectionUpdateParameters.maxConnIntMultiplier,
            connectionUpdateParameters.slaveLatency, connectionUpdateParameters.supervisorTimeoutMs,
            minConnectionEventLength, maxConnectionEventLength);

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
        if (!discovering)
        {
            aci_gap_start_general_discovery_proc(leScanInterval, leScanWindow, GAP_RESOLVABLE_PRIVATE_ADDR, filterDuplicatesEnabled);
            discovering = true;
            infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer) { observer.StateChanged(services::GapState::scanning); });
        }
    }

    void GapCentralSt::StopDeviceDiscovery()
    {
        if (discovering)
        {
            aci_gap_terminate_gap_proc(GAP_GENERAL_DISCOVERY_PROC);
            discovering = false;
        }
    }

    void GapCentralSt::AllowPairing(bool)
    {
    }

    void GapCentralSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        GapSt::HandleHciDisconnectEvent(eventPacket);

        infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer) { observer.StateChanged(services::GapState::standby); });
    }

    void GapCentralSt::HandleHciLeAdvertisingReportEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLeAdvertisingReportEvent(metaEvent);

        auto advertisingEvent = *reinterpret_cast<hci_le_advertising_report_event_rp0*>(metaEvent->data);

        for (uint8_t i = 0; i < advertisingEvent.Num_Reports; i++)
            HandleAdvertisingReport(advertisingEvent.Advertising_Report[i]);
    }

    void GapCentralSt::HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLeConnectionUpdateCompleteEvent(metaEvent);

        const auto evtConnectionUpdate = reinterpret_cast<hci_le_connection_update_complete_event_rp0*>(metaEvent->data);

        connectionParameters.slaveLatency = evtConnectionUpdate->Conn_Latency;
        connectionParameters.supervisorTimeoutMs = evtConnectionUpdate->Supervision_Timeout * 10;
    }

    void GapCentralSt::HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLeDataLengthChangeEvent(metaEvent);

        auto dataLengthChangeEvent = *reinterpret_cast<hci_le_data_length_change_event_rp0*>(metaEvent->data);

        really_assert(dataLengthChangeEvent.Connection_Handle == connectionContext.connectionHandle);
    }

    void GapCentralSt::HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent)
    {
        GapSt::HandleHciLePhyUpdateCompleteEvent(metaEvent);

        auto phyUpdateCompleteEvent = *reinterpret_cast<hci_le_phy_update_complete_event_rp0*>(metaEvent->data);

        really_assert(phyUpdateCompleteEvent.Status == BLE_STATUS_SUCCESS);
    }

    void GapCentralSt::HandleGapProcedureCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        GapSt::HandleGapProcedureCompleteEvent(vendorEvent);

        auto gapProcedureEvent = *reinterpret_cast<aci_gap_proc_complete_event_rp0*>(vendorEvent->data);

        really_assert(gapProcedureEvent.Status == BLE_STATUS_SUCCESS);

        switch (gapProcedureEvent.Procedure_Code)
        {
        case GAP_LIMITED_DISCOVERY_PROC:
        case GAP_GENERAL_DISCOVERY_PROC:
            HandleGapDiscoveryProcedureEvent();
            break;

        case GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC:
            HandleGapDirectConnectionEstablishmentEvent();
            break;
        }
    }

    void GapCentralSt::HandleGattCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        GapSt::HandleGattCompleteEvent(vendorEvent);

        auto gattCompleteEvent = *reinterpret_cast<aci_gatt_proc_complete_event_rp0*>(vendorEvent->data);

        auto localOnConnection = std::exchange(onConnection, nullptr);

        if (localOnConnection && gattCompleteEvent.Error_Code == BLE_STATUS_SUCCESS)
        {
            really_assert(gattCompleteEvent.Connection_Handle == connectionContext.connectionHandle);
            infra::Subject<services::GapCentralObserver>::NotifyObservers(localOnConnection);
        }
    }

    void GapCentralSt::HandleL2capConnectionUpdateRequestEvent(evt_blecore_aci* vendorEvent)
    {
        GapSt::HandleL2capConnectionUpdateRequestEvent(vendorEvent);

        auto l2capEvent = *reinterpret_cast<aci_l2cap_connection_update_req_event_rp0*>(vendorEvent->data);

        aci_l2cap_connection_parameter_update_resp(
            l2capEvent.Connection_Handle, l2capEvent.Interval_Min, l2capEvent.Interval_Max, l2capEvent.Slave_Latency,
            l2capEvent.Timeout_Multiplier, minConnectionEventLength, maxConnectionEventLength, l2capEvent.Identifier, acceptAllParameters);
    }

    void GapCentralSt::HandleGapDiscoveryProcedureEvent()
    {
        discovering = false;

        infra::Subject<services::GapCentralObserver>::NotifyObservers([](auto& observer) { observer.StateChanged(services::GapState::standby); });
    }

    void GapCentralSt::HandleGapDirectConnectionEstablishmentEvent()
    {
        onConnection = [](services::GapCentralObserver& observer)
        {
            observer.StateChanged(services::GapState::connected);
        };

        SetPhy();
        SetDataLength();
        MtuExchange();
    }

    void GapCentralSt::SetPhy()
    {
        infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
            {
                if (hci_le_set_phy(this->connectionContext.connectionHandle, GapSt::allPhys, GapSt::speed2Mbps, GapSt::speed2Mbps, 0) == commandDisallowed)
                {
                    infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
                        {
                            this->SetPhy();
                        });
                }
            });
    }

    void GapCentralSt::SetDataLength() const
    {
        infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
            {
                hci_le_set_data_length(this->connectionContext.connectionHandle, services::GapConnectionParameters::connectionInitialMaxTxOctets, services::GapConnectionParameters::connectionInitialMaxTxTime);
            });
    }

    void GapCentralSt::MtuExchange() const
    {
        infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
            {
                if (aci_gatt_exchange_config(this->connectionContext.connectionHandle) == commandDisallowed)
                {
                    infra::EventDispatcherWithWeakPtr::Instance().Schedule([this]()
                        {
                            this->MtuExchange();
                        });
                }
            });
    }

    void GapCentralSt::HandleAdvertisingReport(const Advertising_Report_t& advertisingReport)
    {
        services::GapAdvertisingReport discoveredDevice;

        discoveredDevice.eventType = ToAdvertisingEventType(advertisingReport.Event_Type);
        discoveredDevice.addressType = ToAdvertisingAddressType(advertisingReport.Address_Type);
        discoveredDevice.rssi = -static_cast<int8_t>(advertisingReport.RSSI);
        std::copy_n(std::begin(advertisingReport.Address), discoveredDevice.address.size(), std::begin(discoveredDevice.address));

        auto advertisementData = reinterpret_cast<const uint8_t*>(&advertisingReport.Length_Data) + 1;

        discoveredDevice.data = infra::MemoryRange(advertisementData, advertisementData + advertisingReport.Length_Data);

        infra::Subject<services::GapCentralObserver>::NotifyObservers([&discoveredDevice](auto& observer)
            {
                observer.DeviceDiscovered(discoveredDevice);
            });
    }

    void GapCentralSt::SetConnectionInterval() const
    {
        aci_l2cap_connection_parameter_update_req(connectionContext.connectionHandle,
            connectionParameters.minConnIntMultiplier, connectionParameters.maxConnIntMultiplier,
            connectionParameters.slaveLatency, connectionParameters.supervisorTimeoutMs);
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
