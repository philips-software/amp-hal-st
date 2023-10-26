#include "hal_st/middlewares/ble_middleware/TracingGapCentralSt.hpp"

namespace hal
{
    TracingGapCentralSt::TracingGapCentralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService gapService, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t* bleBondsStorage, services::Tracer& tracer)
        : GapCentralSt(hciEventSource, address, rootKeys, maxAttMtuSize, txPowerLevel, gapService, bondStorageSynchronizerCreator, bleBondsStorage)
        , tracer(tracer)
    {}

    void TracingGapCentralSt::Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType)
    {
        tracer.Trace() << "TracingGapCentralSt::Connect, MAC address: "
                       << infra::AsMacAddress(macAddress)
                       << ", type: "
                       << addressType;
        GapCentralSt::Connect(macAddress, addressType);
    }

    void TracingGapCentralSt::Disconnect()
    {
        tracer.Trace() << "TracingGapCentralSt::Disconnect";
        GapCentralSt::Disconnect();
    }

    void TracingGapCentralSt::SetAddress(hal::MacAddress macAddress, services::GapDeviceAddressType addressType)
    {
        tracer.Trace() << "TracingGapCentralSt::SetAddress, MAC address: "
                       << infra::AsMacAddress(macAddress)
                       << ", type: "
                       << addressType;
        GapCentralSt::SetAddress(macAddress, addressType);
    }

    void TracingGapCentralSt::StartDeviceDiscovery()
    {
        tracer.Trace() << "TracingGapCentralSt::StartDeviceDiscovery";
        GapCentralSt::StartDeviceDiscovery();
    }

    void TracingGapCentralSt::StopDeviceDiscovery()
    {
        tracer.Trace() << "TracingGapCentralSt::StopDeviceDiscovery";
        GapCentralSt::StopDeviceDiscovery();
    }

    void TracingGapCentralSt::RemoveAllBonds()
    {
        tracer.Trace() << "TracingGapCentralSt::RemoveAllBonds";
        GapCentralSt::RemoveAllBonds();
    }

    void TracingGapCentralSt::RemoveOldestBond()
    {
        tracer.Trace() << "TracingGapCentralSt::RemoveOldestBond";
        GapCentralSt::RemoveOldestBond();
    }

    std::size_t TracingGapCentralSt::GetMaxNumberOfBonds() const
    {
        tracer.Trace() << "TracingGapCentralSt::GetMaxNumberOfBonds";
        return GapCentralSt::GetMaxNumberOfBonds();
    }

    std::size_t TracingGapCentralSt::GetNumberOfBonds() const
    {
        tracer.Trace() << "TracingGapCentralSt::GetNumberOfBonds";
        return GapCentralSt::GetNumberOfBonds();
    }

    void TracingGapCentralSt::Pair()
    {
        tracer.Trace() << "TracingGapCentralSt::Pair";
        GapCentralSt::Pair();
    }

    void TracingGapCentralSt::SetSecurityMode(services::GapPairing::SecurityMode mode, services::GapPairing::SecurityLevel level)
    {
        tracer.Trace() << "TracingGapCentralSt::SetSecurityMode";
        GapCentralSt::SetSecurityMode(mode, level);
    }

    void TracingGapCentralSt::SetIoCapabilities(services::GapPairing::IoCapabilities caps)
    {
        tracer.Trace() << "TracingGapCentralSt::SetIoCapabilities";
        GapCentralSt::SetIoCapabilities(caps);
    }

    void TracingGapCentralSt::AuthenticateWithPasskey(uint32_t passkey)
    {
        tracer.Trace() << "TracingGapCentralSt::AuthenticateWithPasskey";
        GapCentralSt::AuthenticateWithPasskey(passkey);
    }

    void TracingGapCentralSt::NumericComparisonConfirm(bool accept)
    {
        tracer.Trace() << "TracingGapCentralSt::NumericComparisonConfirm";
        GapCentralSt::NumericComparisonConfirm(accept);
    }

    void TracingGapCentralSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        const auto disconnectEvt = reinterpret_cast<hci_disconnection_complete_event_rp0*>(eventPacket.data);

        tracer.Trace() << "TracingGapCentralSt::HandleHciDisconnectEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << disconnectEvt->Connection_Handle;
        tracer.Trace() << "\tReason              : 0x" << infra::hex << disconnectEvt->Reason;

        GapCentralSt::HandleHciDisconnectEvent(eventPacket);
    }

    void TracingGapCentralSt::HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        const auto connectionCompleteEvt = reinterpret_cast<hci_le_connection_complete_event_rp0*>(metaEvent->data);

        hal::MacAddress mac;
        infra::Copy(infra::MakeRange(connectionCompleteEvt->Peer_Address), infra::MakeRange(mac));

        tracer.Trace() << "TracingGapCentralSt::HandleHciLeConnectionCompleteEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << connectionCompleteEvt->Connection_Handle;
        tracer.Trace() << "\tPeer address        : " << infra::AsMacAddress(mac);
        tracer.Trace() << "\tPeer address type   : " << connectionCompleteEvt->Peer_Address_Type;

        GapCentralSt::HandleHciLeConnectionCompleteEvent(metaEvent);
    }

    void TracingGapCentralSt::HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent)
    {
        const auto evtConnectionUpdate = reinterpret_cast<hci_le_connection_update_complete_event_rp0*>(metaEvent->data);

        tracer.Trace() << "TracingGapCentralSt::HandleHciLeConnectionUpdateCompleteEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << evtConnectionUpdate->Connection_Handle;
        tracer.Trace() << "\tConnection Interval : " << evtConnectionUpdate->Conn_Interval;
        tracer.Trace() << "\tConnection Latency  : " << evtConnectionUpdate->Conn_Latency;
        tracer.Trace() << "\tSupervision Timeout : " << evtConnectionUpdate->Supervision_Timeout;

        GapCentralSt::HandleHciLeConnectionUpdateCompleteEvent(metaEvent);
    }

    void TracingGapCentralSt::HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent)
    {
        const auto dataLengthChangeEvent = *reinterpret_cast<hci_le_data_length_change_event_rp0*>(metaEvent->data);

        tracer.Trace() << "TracingGapCentralSt::HandleHciLeDataLengthChangeEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << dataLengthChangeEvent.Connection_Handle;
        tracer.Trace() << "\tMax TX octets       : " << dataLengthChangeEvent.MaxTxOctets;
        tracer.Trace() << "\tMax TX time         : " << dataLengthChangeEvent.MaxTxTime;
        tracer.Trace() << "\tMax RX octets       : " << dataLengthChangeEvent.MaxRxOctets;
        tracer.Trace() << "\tMax RX time         : " << dataLengthChangeEvent.MaxRxTime;
        GapCentralSt::HandleHciLeDataLengthChangeEvent(metaEvent);
    }

    void TracingGapCentralSt::HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent)
    {
        const auto evtLePhyUpdate = reinterpret_cast<hci_le_phy_update_complete_event_rp0*>(metaEvent->data);

        tracer.Trace() << "TracingGapCentralSt::HandleHciLePhyUpdateCompleteEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << evtLePhyUpdate->Connection_Handle;
        tracer.Trace() << "\tRX phy              : " << evtLePhyUpdate->RX_PHY << " Mbps";
        tracer.Trace() << "\tTX phy              : " << evtLePhyUpdate->TX_PHY << " Mbps";

        GapCentralSt::HandleHciLePhyUpdateCompleteEvent(metaEvent);
    }

    void TracingGapCentralSt::HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        const auto enhancedConnectionCompleteEvt = reinterpret_cast<hci_le_enhanced_connection_complete_event_rp0*>(metaEvent->data);
        hal::MacAddress mac;
        infra::Copy(infra::MakeRange(enhancedConnectionCompleteEvt->Peer_Address), infra::MakeRange(mac));

        tracer.Trace() << "TracingGapCentralSt::HandleHciLeEnhancedConnectionCompleteEvent Handle";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << enhancedConnectionCompleteEvt->Connection_Handle;
        tracer.Trace() << "\tPeer address        : " << infra::AsMacAddress(mac);
        tracer.Trace() << "\tPeer address type   : " << enhancedConnectionCompleteEvt->Peer_Address_Type;

        GapCentralSt::HandleHciLeEnhancedConnectionCompleteEvent(metaEvent);
    }

    void TracingGapCentralSt::HandleGapProcedureCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        auto gapProcedureEvent = *reinterpret_cast<aci_gap_proc_complete_event_rp0*>(vendorEvent->data);

        if (gapProcedureEvent.Procedure_Code == GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC)
            tracer.Trace() << "TracingGapCentralSt::HandleGapProcedureCompleteEvent Direct Connection establishment status: 0x" << infra::hex <<gapProcedureEvent.Status;

        GapCentralSt::HandleGapProcedureCompleteEvent(vendorEvent);
    }

    void TracingGapCentralSt::HandleL2capConnectionUpdateRequestEvent(evt_blecore_aci* vendorEvent)
    {
        auto connectionUpdateEvent = *reinterpret_cast<aci_l2cap_connection_update_req_event_rp0*>(vendorEvent->data);

        tracer.Trace() << "TracingGapCentralSt::HandleL2capConnectionUpdateRequestEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex  << connectionUpdateEvent.Connection_Handle;
        tracer.Trace() << "\tIdentifier          : 0x" << infra::hex << connectionUpdateEvent.Identifier;
        tracer.Trace() << "\tL2CAP length        : " << connectionUpdateEvent.L2CAP_Length;
        tracer.Trace() << "\tInterval min        : " << connectionUpdateEvent.Interval_Min;
        tracer.Trace() << "\tInterval max        : " << connectionUpdateEvent.Interval_Max;
        tracer.Trace() << "\tSlave latency       : " << connectionUpdateEvent.Slave_Latency;

        GapCentralSt::HandleL2capConnectionUpdateRequestEvent(vendorEvent);
    }

    void TracingGapCentralSt::HandleMtuExchangeResponseEvent(evt_blecore_aci* vendorEvent)
    {
        const auto mtuExchangeEvent = reinterpret_cast<aci_att_exchange_mtu_resp_event_rp0*>(vendorEvent->data);

        tracer.Trace() << "TracingGapCentralSt::HandleMtuExchangeResponseEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << mtuExchangeEvent->Connection_Handle;
        tracer.Trace() << "\tServer TX MTU       : " << mtuExchangeEvent->Server_RX_MTU;

        GapCentralSt::HandleMtuExchangeResponseEvent(vendorEvent);
    }

    void TracingGapCentralSt::HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        const auto pairingComplete = reinterpret_cast<aci_gap_pairing_complete_event_rp0*>(vendorEvent->data);
        tracer.Trace() << "TracingGapCentralSt::HandlePairingCompleteEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << pairingComplete->Connection_Handle;
        tracer.Trace() << "\tStatus              : 0x" << infra::hex << pairingComplete->Status;
        tracer.Trace() << "\tReason              : 0x" << infra::hex << pairingComplete->Reason;
        GapCentralSt::HandlePairingCompleteEvent(vendorEvent);
    }
}
