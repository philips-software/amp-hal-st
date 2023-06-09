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
        TracingGapCentralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService gapService, uint32_t* bleBondsStorage, services::Tracer& tracer);

        void Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType) override;
        void Disconnect() override;
        void SetAddress(hal::MacAddress macAddress, services::GapDeviceAddressType addressType) override;
        void StartDeviceDiscovery() override;
        void StopDeviceDiscovery() override;

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

    private:
        services::Tracer& tracer;
    };
}

#endif
