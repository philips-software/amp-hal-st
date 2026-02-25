#include "hal_st/middlewares/ble_middleware/TracingGapCentralSt.hpp"

namespace hal
{
    TracingGapCentralSt::TracingGapCentralSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration, services::Tracer& tracer)
        : GapCentralSt(hciEventSource, bondStorageSynchronizer, configuration)
        , tracer(tracer)
    {}

    void TracingGapCentralSt::Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType, infra::Duration initiatingTimeout)
    {
        tracer.Trace() << "TracingGapCentralSt::Connect, MAC address: "
                       << infra::AsMacAddress(macAddress)
                       << ", type: "
                       << addressType
                       << ", initiating timeout (ms): "
                       << std::chrono::duration_cast<std::chrono::milliseconds>(initiatingTimeout).count();
        GapCentralSt::Connect(macAddress, addressType, initiatingTimeout);
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

    std::optional<hal::MacAddress> TracingGapCentralSt::ResolvePrivateAddress(hal::MacAddress address) const
    {
        auto resolvedMac = GapCentralSt::ResolvePrivateAddress(address);
        tracer.Trace() << "TracingGapCentralSt::ResolvePrivateAddress, MAC address: " << infra::AsMacAddress(address);
        if (resolvedMac)
            tracer.Continue() << ", resolved MAC address " << infra::AsMacAddress(*resolvedMac);
        else
            tracer.Continue() << ", could not resolve MAC address";
        return resolvedMac;
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

    bool TracingGapCentralSt::IsDeviceBonded(hal::MacAddress address, services::GapDeviceAddressType addressType) const
    {
        auto ret = GapCentralSt::IsDeviceBonded(address, addressType);
        tracer.Trace() << "TracingGapCentralSt::IsDeviceBonded " << infra::AsMacAddress(address) << " -> " << (ret ? "true" : "false");
        return ret;
    }

    void TracingGapCentralSt::PairAndBond()
    {
        tracer.Trace() << "TracingGapCentralSt::PairAndBond";
        GapCentralSt::PairAndBond();
    }

    void TracingGapCentralSt::SetSecurityMode(SecurityMode mode, SecurityLevel level)
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

    void TracingGapCentralSt::HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event)
    {
        tracer.Trace() << "TracingGapCentralSt::HandleHciDisconnectEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tReason              : 0x" << infra::hex << event.Reason;

        GapCentralSt::HandleHciDisconnectEvent(event);
    }

    void TracingGapCentralSt::HandleHciLeConnectionCompleteEvent(const hci_le_connection_complete_event_rp0& event)
    {
        hal::MacAddress mac;
        infra::Copy(infra::MakeRange(event.Peer_Address), infra::MakeRange(mac));

        tracer.Trace() << "TracingGapCentralSt::HandleHciLeConnectionCompleteEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tPeer address        : " << infra::AsMacAddress(mac);
        tracer.Trace() << "\tPeer address type   : " << event.Peer_Address_Type;

        GapCentralSt::HandleHciLeConnectionCompleteEvent(event);
    }

    void TracingGapCentralSt::HandleHciLeConnectionUpdateCompleteEvent(const hci_le_connection_update_complete_event_rp0& event)
    {
        tracer.Trace() << "TracingGapCentralSt::HandleHciLeConnectionUpdateCompleteEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tConnection Interval : " << event.Conn_Interval;
        tracer.Trace() << "\tConnection Latency  : " << event.Conn_Latency;
        tracer.Trace() << "\tSupervision Timeout : " << event.Supervision_Timeout;

        GapCentralSt::HandleHciLeConnectionUpdateCompleteEvent(event);
    }

    void TracingGapCentralSt::HandleHciLeDataLengthChangeEvent(const hci_le_data_length_change_event_rp0& event)
    {
        tracer.Trace() << "TracingGapCentralSt::HandleHciLeDataLengthChangeEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tMax TX octets       : " << event.MaxTxOctets;
        tracer.Trace() << "\tMax TX time         : " << event.MaxTxTime;
        tracer.Trace() << "\tMax RX octets       : " << event.MaxRxOctets;
        tracer.Trace() << "\tMax RX time         : " << event.MaxRxTime;
        GapCentralSt::HandleHciLeDataLengthChangeEvent(event);
    }

    void TracingGapCentralSt::HandleHciLePhyUpdateCompleteEvent(const hci_le_phy_update_complete_event_rp0& event)
    {
        tracer.Trace() << "TracingGapCentralSt::HandleHciLePhyUpdateCompleteEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tRX phy              : " << event.RX_PHY << " Mbps";
        tracer.Trace() << "\tTX phy              : " << event.TX_PHY << " Mbps";

        GapCentralSt::HandleHciLePhyUpdateCompleteEvent(event);
    }

    void TracingGapCentralSt::HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event)
    {
        hal::MacAddress mac;
        infra::Copy(infra::MakeRange(event.Peer_Address), infra::MakeRange(mac));

        tracer.Trace() << "TracingGapCentralSt::HandleHciLeEnhancedConnectionCompleteEvent Handle";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tPeer address        : " << infra::AsMacAddress(mac);
        tracer.Trace() << "\tPeer address type   : " << event.Peer_Address_Type;

        GapCentralSt::HandleHciLeEnhancedConnectionCompleteEvent(event);
    }

    void TracingGapCentralSt::HandleGapProcedureCompleteEvent(const aci_gap_proc_complete_event_rp0& event)
    {
        if (event.Procedure_Code == GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC)
            tracer.Trace() << "TracingGapCentralSt::HandleGapProcedureCompleteEvent Direct Connection establishment status: 0x" << infra::hex << event.Status;

        GapCentralSt::HandleGapProcedureCompleteEvent(event);
    }

    void TracingGapCentralSt::HandleL2capConnectionUpdateRequestEvent(const aci_l2cap_connection_update_req_event_rp0& event)
    {
#if defined(STM32WB)
        auto latency = event.Slave_Latency;
#else
        auto latency = event.Latency;
#endif

        tracer.Trace() << "TracingGapCentralSt::HandleL2capConnectionUpdateRequestEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tIdentifier          : 0x" << infra::hex << event.Identifier;
        tracer.Trace() << "\tL2CAP length        : " << event.L2CAP_Length;
        tracer.Trace() << "\tInterval min        : " << event.Interval_Min;
        tracer.Trace() << "\tInterval max        : " << event.Interval_Max;
        tracer.Trace() << "\tSlave latency       : " << latency;

        GapCentralSt::HandleL2capConnectionUpdateRequestEvent(event);
    }

    void TracingGapCentralSt::HandleMtuExchangeResponseEvent(const aci_att_exchange_mtu_resp_event_rp0& event)
    {
        tracer.Trace() << "TracingGapCentralSt::HandleMtuExchangeResponseEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tServer TX MTU       : " << event.Server_RX_MTU;

        GapCentralSt::HandleMtuExchangeResponseEvent(event);
    }

    void TracingGapCentralSt::HandlePairingCompleteEvent(const aci_gap_pairing_complete_event_rp0& event)
    {
        tracer.Trace() << "TracingGapCentralSt::HandlePairingCompleteEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tStatus              : 0x" << infra::hex << event.Status;
        tracer.Trace() << "\tReason              : 0x" << infra::hex << event.Reason;
        GapCentralSt::HandlePairingCompleteEvent(event);
    }

    void TracingGapCentralSt::HandlePassKeyRequestEvent(const aci_gap_pass_key_req_event_rp0& event)
    {
        tracer.Trace() << "TracingGapCentralSt::HandlePassKeyRequestEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        GapCentralSt::HandlePassKeyRequestEvent(event);
    }

    void TracingGapCentralSt::HandleNumericComparisonValueEvent(const aci_gap_numeric_comparison_value_event_rp0& event)
    {
        tracer.Trace() << "TracingGapCentralSt::HandleNumericComparisonValueEvent";
        tracer.Trace() << "\tConnection handle   : 0x" << infra::hex << event.Connection_Handle;
        tracer.Trace() << "\tNumeric value       : " << event.Numeric_Value;
        GapCentralSt::HandleNumericComparisonValueEvent(event);
    }
}
