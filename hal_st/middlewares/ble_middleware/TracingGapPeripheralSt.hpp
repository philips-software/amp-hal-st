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
        TracingGapPeripheralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService& gapService, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t* bleBondsStorage, services::Tracer& tracer);

    protected:
        // Implementation of GapPeripheralSt
        void HandleHciDisconnectEvent(hci_event_pckt& eventPacket) override;
        void HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent) override;
        void HandleBondLostEvent(evt_blecore_aci* vendorEvent) override;
        void HandleMtuExchangeResponseEvent(evt_blecore_aci* vendorEvent) override;

    private:
        services::Tracer& tracer;
    };
}

#endif
