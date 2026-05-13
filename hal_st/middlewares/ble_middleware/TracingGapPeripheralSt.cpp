#include "hal_st/middlewares/ble_middleware/TracingGapPeripheralSt.hpp"
#include "ble_hal_aci.h"
#include "infra/util/BoundedString.hpp"
#include <array>
#include <cstdint>

namespace
{
    const char* RadioStateName(uint8_t state)
    {
        switch (state)
        {
            case 0x00: return "Idle";
            case 0x01: return "Advertising";
            case 0x02: return "Connection slave";
            case 0x03: return "Scanning";
            case 0x05: return "Connection master";
            case 0x06: return "TX test mode";
            case 0x07: return "RX test mode";
            default: return "Unknown";
        }
    }

    const char* LinkStatusName(uint8_t status)
    {
        switch (status)
        {
            case 0x00: return "Idle";
            case 0x01: return "Advertising";
            case 0x02: return "Connected (slave)";
            case 0x03: return "Scanning";
            case 0x05: return "Connected (master)";
            case 0x06: return "TX test mode";
            case 0x07: return "RX test mode";
            case 0x81: return "Advertising with additional beacon";
            default: return "Unknown";
        }
    }

    const char* HardwareErrorName(uint8_t code)
    {
        switch (code)
        {
            case 0x01: return "bluecore act2 error";
            case 0x02: return "time overrun error";
            case 0x03: return "internal FIFO full";
            default: return "unknown";
        }
    }
}

namespace hal
{
    TracingGapPeripheralSt::TracingGapPeripheralSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration, services::Tracer& tracer)
        : GapPeripheralSt(hciEventSource, bondStorageSynchronizer, configuration)
        , tracer(tracer)
    {}

    void TracingGapPeripheralSt::Advertise(services::GapAdvertisementType type, AdvertisementIntervalMultiplier multiplier)
    {
        tracer.Trace() << "GapPeripheralSt::Advertise Type = " << static_cast<uint8_t>(type) << " Multiplier = " << static_cast<uint16_t>(multiplier);
        GapPeripheralSt::Advertise(type, multiplier);
    }

    void TracingGapPeripheralSt::Standby()
    {
        tracer.Trace() << "GapPeripheralSt::Standby";
        GapPeripheralSt::Standby();
    }

    void TracingGapPeripheralSt::RemoveAllBonds()
    {
        tracer.Trace() << "GapPeripheralSt::RemoveAllBonds";
        GapPeripheralSt::RemoveAllBonds();
    }

    void TracingGapPeripheralSt::AllowPairing(bool allow)
    {
        tracer.Trace() << "GapPeripheralSt::AllowPairing Allow = " << allow;
        GapPeripheralSt::AllowPairing(allow);
    }

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

    void TracingGapPeripheralSt::HandleHciHardwareErrorEvent(const hci_hardware_error_event_rp0& event)
    {
        tracer.Trace() << "GapPeripheralSt::HandleHciHardwareErrorEvent Hardware_Code = " << event.Hardware_Code << " (" << HardwareErrorName(event.Hardware_Code) << ")";
    }

    void TracingGapPeripheralSt::HandleAciHalEndOfRadioActivityEvent(const aci_hal_end_of_radio_activity_event_rp0& event)
    {
        tracer.Trace() << "GapPeripheralSt::HandleAciHalEndOfRadioActivityEvent Last_State = " << RadioStateName(event.Last_State) << " -> Next_State = " << RadioStateName(event.Next_State);

        if (event.Last_State == 0x01 || event.Next_State == 0x01)
        {
            std::array<uint8_t, 8> linkStatus{};
            std::array<uint16_t, 8> linkConnectionHandle{};
            aci_hal_get_link_status(linkStatus.data(), linkConnectionHandle.data());
            std::size_t advertisingCount = 0;
            for (std::size_t i = 0; i < linkStatus.size(); ++i)
            {
                if (linkStatus[i] != 0x00)
                    tracer.Trace() << "GapPeripheralSt::HandleAciHalEndOfRadioActivityEvent link[" << i << "] = " << LinkStatusName(linkStatus[i]);
                if (linkStatus[i] == 0x01)
                    ++advertisingCount;
            }
            if (event.Next_State == 0x01 && advertisingCount != 1)
                tracer.Trace() << "GapPeripheralSt::HandleAciHalEndOfRadioActivityEvent unexpected: expected 1 advertising link, found " << advertisingCount;
            if (event.Last_State == 0x01 && event.Next_State == 0x00 && advertisingCount != 0)
                tracer.Trace() << "GapPeripheralSt::HandleAciHalEndOfRadioActivityEvent unexpected: expected 0 advertising links when going idle, found " << advertisingCount;
        }
    }
}
