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
        void SetAddress(hal::MacAddress macAddress, services::GapDeviceAddressType addressType) override;
        void StartDeviceDiscovery() override;
        void StopDeviceDiscovery() override;
        infra::Optional<hal::MacAddress> ResolvePrivateAddress(hal::MacAddress address) const override;

        // Implementation of GapBonding
        void RemoveAllBonds() override;
        void RemoveOldestBond() override;
        std::size_t GetMaxNumberOfBonds() const override;
        std::size_t GetNumberOfBonds() const override;
        bool IsDeviceBonded(hal::MacAddress address, services::GapDeviceAddressType addressType) const override;

        // Implementation of GapPairing
        void PairAndBond() override;
        void SetSecurityMode(SecurityMode mode, SecurityLevel level) override;
        void SetIoCapabilities(services::GapPairing::IoCapabilities caps) override;
        void AuthenticateWithPasskey(uint32_t passkey) override;
        void NumericComparisonConfirm(bool accept) override;

    protected:
        // Implementation of GapCentralSt
        void HandleHciDisconnectEvent(hci_event_pckt& eventPacket) override;
        void HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleGapProcedureCompleteEvent(evt_blecore_aci* vendorEvent) override;
        void HandleL2capConnectionUpdateRequestEvent(evt_blecore_aci* vendorEvent) override;
        void HandleMtuExchangeResponseEvent(evt_blecore_aci* vendorEvent) override;
        void HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent) override;

    private:
        services::Tracer& tracer;
    };
}

#endif
