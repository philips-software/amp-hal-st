#include "hal_st/middlewares/ble_middleware/TracingGapPeripheralSt.hpp"
#include "infra/util/BoundedString.hpp"
#include <cstdint>

namespace hal
{
    TracingGapPeripheralSt::TracingGapPeripheralSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration, services::Tracer& tracer)
        : GapPeripheralSt(hciEventSource, bondStorageSynchronizer, configuration)
        , tracer(tracer)
    {}

    void TracingGapPeripheralSt::HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event)
    {
        tracer.Trace() << "GapPeripheralSt::HandleHciDisconnectEvent Handle = " << event.Connection_Handle;
        tracer.Continue() << " : Reason = " << event.Reason;
        GapPeripheralSt::HandleHciDisconnectEvent(event);
    }

    void TracingGapPeripheralSt::HandleHciLeConnectionUpdateCompleteEvent(const hci_le_connection_update_complete_event_rp0& event)
    {
        tracer.Trace() << "GapPeripheralSt::HandleHciLeConnectionUpdateCompleteEvent";
        tracer.Trace() << "\tConnection Interval : " << event.Conn_Interval;
        tracer.Trace() << "\tConnection Latency  : " << event.Conn_Latency;
        tracer.Trace() << "\tSupervision Timeout : " << event.Supervision_Timeout;
        GapPeripheralSt::HandleHciLeConnectionUpdateCompleteEvent(event);
    }

    void TracingGapPeripheralSt::HandleHciLeDataLengthChangeEvent(const hci_le_data_length_change_event_rp0& event)
    {
        tracer.Trace() << "GapPeripheralSt::HandleHciLeDataLengthChangeEvent";
        tracer.Trace() << "\tMaxTxOctets : " << event.MaxTxOctets;
        tracer.Trace() << "\tMaxTxTime   : " << event.MaxTxTime;
        tracer.Trace() << "\tMaxRxOctets : " << event.MaxRxOctets;
        tracer.Trace() << "\tMaxRxTime   : " << event.MaxRxTime;
        GapPeripheralSt::HandleHciLeDataLengthChangeEvent(event);
    }

    void TracingGapPeripheralSt::HandleHciLePhyUpdateCompleteEvent(const hci_le_phy_update_complete_event_rp0& event)
    {
        tracer.Trace() << "GapPeripheralSt::HandleHciLePhyUpdateCompleteEvent " << event.Status;
        GapPeripheralSt::HandleHciLePhyUpdateCompleteEvent(event);
    }

    void TracingGapPeripheralSt::HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event)
    {
        tracer.Trace() << "GapPeripheralSt::HandleHciLeEnhancedConnectionCompleteEvent Handle - " << event.Connection_Handle;

        hal::MacAddress mac;
        std::copy(std::begin(event.Peer_Address), std::end(event.Peer_Address), std::begin(mac));
        tracer.Continue() << " Peer address - " << infra::AsMacAddress(mac) << " type - " << event.Peer_Address_Type;
        GapPeripheralSt::HandleHciLeEnhancedConnectionCompleteEvent(event);
    }

    void TracingGapPeripheralSt::HandlePairingCompleteEvent(const aci_gap_pairing_complete_event_rp0& event)
    {
        tracer.Trace() << "GapPeripheralSt::HandlePairingCompleteEvent " << event.Status;
        GapPeripheralSt::HandlePairingCompleteEvent(event);
    }

    void TracingGapPeripheralSt::HandleBondLostEvent()
    {
        tracer.Trace() << "GapPeripheralSt::HandleBondLostEvent";
        GapPeripheralSt::HandleBondLostEvent();
    }

    void TracingGapPeripheralSt::HandleMtuExchangeResponseEvent(const aci_att_exchange_mtu_resp_event_rp0& event)
    {
        tracer.Trace() << "GapPeripheralSt::HandleMtuExchangeResponseEvent Server_RX_MTU = " << event.Server_RX_MTU;
        GapPeripheralSt::HandleMtuExchangeResponseEvent(event);
    }

    void TracingGapPeripheralSt::StartedAdvertising(infra::BoundedConstString functionName)
    {
        tracer.Trace() << "ST BLE Peripheral advertising with " << functionName;
    }

    void TracingGapPeripheralSt::ReceivedNumberOfBondedAddresses(uint8_t numberOfBondedAddresses)
    {
        tracer.Trace() << "ST BLE Peripheral numberOfBondedAddresses: " << numberOfBondedAddresses;
    }
}
