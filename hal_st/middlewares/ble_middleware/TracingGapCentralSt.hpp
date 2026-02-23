#ifndef HAL_ST_TRACING_GAP_CENTRAL_ST_HPP
#define HAL_ST_TRACING_GAP_CENTRAL_ST_HPP

#include "hal_st/middlewares/ble_middleware/GapCentralSt.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal
{
    class TracingGapCentralSt
        : public GapCentralSt
    {
    public:
        TracingGapCentralSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration, services::Tracer& tracer);

        // Implementation of services::GapCentral
        void Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType, infra::Duration initiatingTimeout) override;
        void Disconnect() override;
        void SetAddress(services::GapAddress address) override;
        void StartDeviceDiscovery() override;
        void StopDeviceDiscovery() override;
        std::optional<hal::MacAddress> ResolvePrivateAddress(hal::MacAddress address) const override;

        // Implementation of GapBonding
        void RemoveAllBonds() override;
        void RemoveOldestBond() override;
        std::size_t GetMaxNumberOfBonds() const override;
        std::size_t GetNumberOfBonds() const override;
        bool IsDeviceBonded(hal::MacAddress address, services::GapDeviceAddressType addressType) const override;

        // Implementation of GapPairing
        void SetSecurityMode(services::GapPairing::SecurityMode mode, services::GapPairing::SecurityLevel level) override;
        void PairAndBond() override;
        void SetIoCapabilities(services::GapPairing::IoCapabilities caps) override;
        void AuthenticateWithPasskey(uint32_t passkey) override;
        void NumericComparisonConfirm(bool accept) override;

    protected:
        // Implementation of GapCentralSt
        void HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event) override;
        void HandleHciLeConnectionCompleteEvent(const hci_le_connection_complete_event_rp0& event) override;
        void HandleHciLeConnectionUpdateCompleteEvent(const hci_le_connection_update_complete_event_rp0& event) override;
        void HandleHciLeDataLengthChangeEvent(const hci_le_data_length_change_event_rp0& event) override;
        void HandleHciLePhyUpdateCompleteEvent(const hci_le_phy_update_complete_event_rp0& event) override;
        void HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event) override;
        void HandleGapProcedureCompleteEvent(const aci_gap_proc_complete_event_rp0& event) override;
        void HandleL2capConnectionUpdateRequestEvent(const aci_l2cap_connection_update_req_event_rp0& event) override;
        void HandleMtuExchangeResponseEvent(const aci_att_exchange_mtu_resp_event_rp0& event) override;
        void HandlePairingCompleteEvent(const aci_gap_pairing_complete_event_rp0& event) override;

    private:
        services::Tracer& tracer;
    };
}

#endif
