#ifndef HAL_ST_GAP_CENTRAL_ST_HPP
#define HAL_ST_GAP_CENTRAL_ST_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "infra/timer/Timer.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace hal
{
    class GapCentralSt
        : public services::GapCentral
        , public GapSt
    {
    public:
        GapCentralSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration);

        // Implementation of services::GapCentral
        void Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType, infra::Duration initiatingTimeout) override;
        void CancelConnect() override;
        void Disconnect() override;
        void SetAddress(hal::MacAddress macAddress, services::GapDeviceAddressType addressType) override;
        void StartDeviceDiscovery() override;
        void StopDeviceDiscovery() override;
        infra::Optional<hal::MacAddress> ResolveDeviceAddress(hal::MacAddress deviceAddress) const override;

        // Implementation of GapPairing
        void AllowPairing(bool allow) override;

    protected:
        void HandleHciDisconnectEvent(hci_event_pckt& eventPacket) override;
        void HandleHciLeAdvertisingReportEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleGapProcedureCompleteEvent(evt_blecore_aci* vendorEvent) override;
        void HandleGattCompleteEvent(evt_blecore_aci* vendorEvent) override;
        void HandleL2capConnectionUpdateRequestEvent(evt_blecore_aci* vendorEvent) override;

    private:
        void HandleGapDiscoveryProcedureEvent();
        void HandleGapDirectConnectionProcedureCompleteEvent();

        void HandleAdvertisingReport(const Advertising_Report_t& advertisingReport);
        void SetDataLength();
        void MtuExchange() const;
        void Initialize(const GapService& gapService);
        void UpdateStateOnConnectionComplete(evt_le_meta_event* metaEvent);

    private:
        static const services::GapConnectionParameters connectionUpdateParameters;

        // Create connection parameters
        const uint16_t leScanInterval = 0x320;
        const uint16_t leScanWindow = 0x320;

        // Terminate connection
        const uint8_t remoteUserTerminatedConnection = 0x13;

        // HCI status
        const uint8_t commandDisallowed = 0x0c;

        bool discovering = false;
        services::GapConnectionParameters connectionParameters;
        infra::AutoResetFunction<void()> onMtuExchangeDone;
        infra::TimerSingleShot initiatingStateTimer;
    };
}

#endif
