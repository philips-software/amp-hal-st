#include "hal_st/middlewares/ble_middleware/TracingGapPeripheralSt.hpp"

namespace hal
{
    TracingGapPeripheralSt::TracingGapPeripheralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, uint16_t maxAttMtuSize, infra::CreatorBase<services::BondStorageManager, void()>& bondStorageManagerCreator, services::Tracer& tracer)
        : GapPeripheralSt(hciEventSource, address, maxAttMtuSize, bondStorageManagerCreator)
        , tracer(tracer)
    {}

    void TracingGapPeripheralSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        auto disconnectEvt = reinterpret_cast<hci_disconnection_complete_event_rp0*>(eventPacket.data);

        tracer.Trace() << "Disconnect for connection handle: " << disconnectEvt->Connection_Handle;
        GapPeripheralSt::HandleHciDisconnectEvent(eventPacket);
    }

    void TracingGapPeripheralSt::HandleHciUnknownEvent(hci_event_pckt& eventPacket)
    {
        tracer.Trace() << "HandleHciUnknownEvent: Unsupported event (" << eventPacket.evt << ")";
        GapPeripheralSt::HandleHciUnknownEvent(eventPacket);
    }

    void TracingGapPeripheralSt::HandleHciLeConnectionUpdateEvent(evt_le_meta_event* metaEvent)
    {
        auto evtConnectionUpdate = reinterpret_cast<hci_le_connection_update_complete_event_rp0*>(metaEvent->data);
        tracer.Trace() << "Connection update event";
        tracer.Trace() << "Connection Interval  : " << evtConnectionUpdate->Conn_Interval;
        tracer.Trace() << "Connection Latency   : " << evtConnectionUpdate->Conn_Latency;
        tracer.Trace() << "Supervision Timeout   : " << evtConnectionUpdate->Supervision_Timeout;
        GapPeripheralSt::HandleHciLeConnectionUpdateEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto evtLePhyUpdate = reinterpret_cast<hci_le_phy_update_complete_event_rp0*>(metaEvent->data);
        tracer.Trace() << "Phy update complete event, status " << evtLePhyUpdate->Status;
        GapPeripheralSt::HandleHciLePhyUpdateCompleteEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto connectionCompleteEvt = reinterpret_cast<hci_le_connection_complete_event_rp0*>(metaEvent->data);
        tracer.Trace() << "Connection complete event : Handle - " << connectionCompleteEvt->Connection_Handle;

        hal::MacAddress mac;
        std::reverse_copy(std::begin(connectionCompleteEvt->Peer_Address),
                std::end(connectionCompleteEvt->Peer_Address), std::begin(mac));
        tracer.Continue() << " Peer address - " << infra::AsMacAddress(mac);
        GapPeripheralSt::HandleHciLeConnectionCompleteEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleHciLeDataLengthUpdateEvent(evt_le_meta_event* metaEvent)
    {
        auto dataLengthUpdate = reinterpret_cast<hci_le_data_length_change_event_rp0*>(metaEvent->data);
        tracer.Trace() << "Data length change event";
        tracer.Trace() << "MaxTxOctets  : " << dataLengthUpdate->MaxTxOctets;
        tracer.Trace() << "MaxTxTime    : " << dataLengthUpdate->MaxTxTime;
        tracer.Trace() << "MaxRxOctets  : " << dataLengthUpdate->MaxRxOctets;
        tracer.Trace() << "MaxRxTime    : " << dataLengthUpdate->MaxRxTime;
        GapPeripheralSt::HandleHciLeDataLengthUpdateEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleHciLeUnknownEvent(evt_le_meta_event* metaEvent)
    {
        tracer.Trace() << "HandleHciLeUnknownEvent : Unsupported event (" << metaEvent->subevent << ")";
        GapPeripheralSt::HandleHciLeUnknownEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleBondLostEvent(evt_blecore_aci* vendorEvent)
    {
        tracer.Trace() << "Bond lost event: Send allow rebond";
        GapPeripheralSt::HandleBondLostEvent(vendorEvent);
    }

    void TracingGapPeripheralSt::HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        auto pairingComplete = reinterpret_cast<aci_gap_pairing_complete_event_rp0*>(vendorEvent->data);
        tracer.Trace() << "GAP pairing complete : Status = " << pairingComplete->Status;
        GapPeripheralSt::HandlePairingCompleteEvent(vendorEvent);
    }

    void TracingGapPeripheralSt::HandleMtuExchangeEvent(evt_blecore_aci* vendorEvent)
    {
        auto mtuExchangeEvent = reinterpret_cast<aci_att_exchange_mtu_resp_event_rp0*>(vendorEvent->data);
        tracer.Trace() << "MTU Exhange event : Server_RX_MTU = " << mtuExchangeEvent->Server_RX_MTU;
        GapPeripheralSt::HandleMtuExchangeEvent(vendorEvent);
    }
}
