#ifndef HAL_ST_TRACING_GAP_PERIPHERAL_ST_HPP
#define HAL_ST_TRACING_GAP_PERIPHERAL_ST_HPP

#include "hal_st/middlewares/ble_middleware/GapPeripheralSt.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal
{
    class TracingGapPeripheralSt
        : public GapPeripheralSt
    {
    public:
        TracingGapPeripheralSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration, services::Tracer& tracer);

    protected:
        // Implementation of GapPeripheralSt
        void HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event) override;
        void HandleHciLeConnectionUpdateCompleteEvent(const hci_le_connection_update_complete_event_rp0& event) override;
        void HandleHciLeDataLengthChangeEvent(const hci_le_data_length_change_event_rp0& event) override;
        void HandleHciLePhyUpdateCompleteEvent(const hci_le_phy_update_complete_event_rp0& event) override;
        void HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event) override;
        void HandlePairingCompleteEvent(const aci_gap_pairing_complete_event_rp0& event) override;
        void HandleBondLostEvent() override;
        void HandleMtuExchangeResponseEvent(const aci_att_exchange_mtu_resp_event_rp0& event) override;

    private:
        services::Tracer& tracer;
    };
}

#endif
