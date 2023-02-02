#include "hal_st/middlewares/ble_middleware/TracingGapPeripheralSt.hpp"

namespace hal
{
    TracingGapPeripheralSt::TracingGapPeripheralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService& gapService, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t* bleBondsStorage, services::Tracer& tracer)
        : GapPeripheralSt(hciEventSource, address, rootKeys, maxAttMtuSize, txPowerLevel, gapService, bondStorageSynchronizerCreator, bleBondsStorage)
        , tracer(tracer)
    {}

    void TracingGapPeripheralSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        const auto disconnectEvt = reinterpret_cast<hci_disconnection_complete_event_rp0*>(eventPacket.data);
        tracer.Trace() << "GapPeripheralSt::HandleHciDisconnectEvent handle = " << disconnectEvt->Connection_Handle;
        GapPeripheralSt::HandleHciDisconnectEvent(eventPacket);
    }

    void TracingGapPeripheralSt::HandleHciUnknownEvent(hci_event_pckt& eventPacket)
    {
        tracer.Trace() << "GapPeripheralSt::HandleHciUnknownEvent " << eventPacket.evt;
        GapPeripheralSt::HandleHciUnknownEvent(eventPacket);
    }

    void TracingGapPeripheralSt::HandleHciLeConnectionUpdateEvent(evt_le_meta_event* metaEvent)
    {
        const auto evtConnectionUpdate = reinterpret_cast<hci_le_connection_update_complete_event_rp0*>(metaEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandleHciLeConnectionUpdateEvent";
        tracer.Trace() << "\tConnection Interval : " << evtConnectionUpdate->Conn_Interval;
        tracer.Trace() << "\tConnection Latency  : " << evtConnectionUpdate->Conn_Latency;
        tracer.Trace() << "\tSupervision Timeout : " << evtConnectionUpdate->Supervision_Timeout;
        GapPeripheralSt::HandleHciLeConnectionUpdateEvent(metaEvent);
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

    void TracingGapPeripheralSt::HandleHciLeDataLengthUpdateEvent(evt_le_meta_event* metaEvent)
    {
        const auto dataLengthUpdate = reinterpret_cast<hci_le_data_length_change_event_rp0*>(metaEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandleHciLeDataLengthUpdateEvent";
        tracer.Trace() << "\tMaxTxOctets : " << dataLengthUpdate->MaxTxOctets;
        tracer.Trace() << "\tMaxTxTime   : " << dataLengthUpdate->MaxTxTime;
        tracer.Trace() << "\tMaxRxOctets : " << dataLengthUpdate->MaxRxOctets;
        tracer.Trace() << "\tMaxRxTime   : " << dataLengthUpdate->MaxRxTime;
        GapPeripheralSt::HandleHciLeDataLengthUpdateEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleHciLeUnknownEvent(evt_le_meta_event* metaEvent)
    {
        tracer.Trace() << "GapPeripheralSt::HandleHciLeUnknownEvent " << metaEvent->subevent;
        GapPeripheralSt::HandleHciLeUnknownEvent(metaEvent);
    }

    void TracingGapPeripheralSt::HandleBondLostEvent(evt_blecore_aci* vendorEvent)
    {
        tracer.Trace() << "GapPeripheralSt::HandleBondLostEvent";
        GapPeripheralSt::HandleBondLostEvent(vendorEvent);
    }

    void TracingGapPeripheralSt::HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent)
    {
        const auto pairingComplete = reinterpret_cast<aci_gap_pairing_complete_event_rp0*>(vendorEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandlePairingCompleteEvent " << pairingComplete->Status;
        GapPeripheralSt::HandlePairingCompleteEvent(vendorEvent);
    }

    void TracingGapPeripheralSt::HandleMtuExchangeEvent(evt_blecore_aci* vendorEvent)
    {
        const auto mtuExchangeEvent = reinterpret_cast<aci_att_exchange_mtu_resp_event_rp0*>(vendorEvent->data);
        tracer.Trace() << "GapPeripheralSt::HandleMtuExchangeEvent Server_RX_MTU = " << mtuExchangeEvent->Server_RX_MTU;
        GapPeripheralSt::HandleMtuExchangeEvent(vendorEvent);
    }
}
