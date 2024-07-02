#include "hal_st/middlewares/ble_middleware/TracingGapPeripheralSt.hpp"

namespace hal
{
    TracingGapPeripheralSt::TracingGapPeripheralSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration, services::Tracer& tracer)
        : GapPeripheralSt(hciEventSource, bondStorageSynchronizer, configuration)
        , tracer(tracer)
    {}

    void TracingGapPeripheralSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        const auto disconnectEvt = reinterpret_cast<hci_disconnection_complete_event_rp0*>(eventPacket.data);
        tracer.Trace() << "GapPeripheralSt::HandleHciDisconnectEvent Handle = " << disconnectEvt->Connection_Handle;
        tracer.Continue() << " : Reason = " << disconnectEvt->Reason;
        GapPeripheralSt::HandleHciDisconnectEvent(eventPacket);
    }

    void TracingGapPeripheralSt::HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent)
    {
        const auto evtConnectionUpdate = reinterpret_cast<hci_le_connection_update_complete_event_rp0*>(metaEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandleHciLeConnectionUpdateCompleteEvent";
        tracer.Trace() << "\tConnection Interval : " << evtConnectionUpdate->Conn_Interval;
        tracer.Trace() << "\tConnection Latency  : " << evtConnectionUpdate->Conn_Latency;
        tracer.Trace() << "\tSupervision Timeout : " << evtConnectionUpdate->Supervision_Timeout;
        GapPeripheralSt::HandleHciLeConnectionUpdateCompleteEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent)
    {
        const auto dataLengthUpdate = reinterpret_cast<hci_le_data_length_change_event_rp0*>(metaEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandleHciLeDataLengthChangeEvent";
        tracer.Trace() << "\tMaxTxOctets : " << dataLengthUpdate->MaxTxOctets;
        tracer.Trace() << "\tMaxTxTime   : " << dataLengthUpdate->MaxTxTime;
        tracer.Trace() << "\tMaxRxOctets : " << dataLengthUpdate->MaxRxOctets;
        tracer.Trace() << "\tMaxRxTime   : " << dataLengthUpdate->MaxRxTime;
        GapPeripheralSt::HandleHciLeDataLengthChangeEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent)
    {
        const auto evtLePhyUpdate = reinterpret_cast<hci_le_phy_update_complete_event_rp0*>(metaEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandleHciLePhyUpdateCompleteEvent " << evtLePhyUpdate->Status;
        GapPeripheralSt::HandleHciLePhyUpdateCompleteEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        const auto enhancedConnectionCompleteEvt = reinterpret_cast<hci_le_enhanced_connection_complete_event_rp0*>(metaEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandleHciLeEnhancedConnectionCompleteEvent Handle - " << enhancedConnectionCompleteEvt->Connection_Handle;

        hal::MacAddress mac;
        std::copy(std::begin(enhancedConnectionCompleteEvt->Peer_Address),
            std::end(enhancedConnectionCompleteEvt->Peer_Address), std::begin(mac));
        tracer.Continue() << " Peer address - " << infra::AsMacAddress(mac) << " type - " << enhancedConnectionCompleteEvt->Peer_Address_Type;
        GapPeripheralSt::HandleHciLeEnhancedConnectionCompleteEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        const auto pairingComplete = reinterpret_cast<aci_gap_pairing_complete_event_rp0*>(vendorEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandlePairingCompleteEvent " << pairingComplete->Status;
        GapPeripheralSt::HandlePairingCompleteEvent(vendorEvent);
    }

    void TracingGapPeripheralSt::HandleBondLostEvent(evt_blecore_aci* vendorEvent)
    {
        tracer.Trace() << "GapPeripheralSt::HandleBondLostEvent";
        GapPeripheralSt::HandleBondLostEvent(vendorEvent);
    }

    void TracingGapPeripheralSt::HandleMtuExchangeResponseEvent(evt_blecore_aci* vendorEvent)
    {
        const auto mtuExchangeEvent = reinterpret_cast<aci_att_exchange_mtu_resp_event_rp0*>(vendorEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandleMtuExchangeResponseEvent Server_RX_MTU = " << mtuExchangeEvent->Server_RX_MTU;
        GapPeripheralSt::HandleMtuExchangeResponseEvent(vendorEvent);
    }
}
