#ifndef HAL_ST_GAP_CENTRAL_ST_HPP
#define HAL_ST_GAP_CENTRAL_ST_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/GapSt.hpp"

namespace hal
{
    class GapCentralSt
        : public services::GapCentral
        , public GapSt
    {
    public:
        GapCentralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService gapService, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t* bleBondsStorage);

        // Implementation of services::GapCentral
        void Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType) override;
        void Disconnect() override;
        void SetAddress(hal::MacAddress macAddress, services::GapDeviceAddressType addressType) override;
        void StartDeviceDiscovery() override;
        void StopDeviceDiscovery() override;

        // Implementation of GapPairing
        void AllowPairing(bool allow) override;

    protected:
        void HandleHciDisconnectEvent(hci_event_pckt& eventPacket) override;
        void HandleHciLeAdvertisingReportEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent) override;
        void HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandleGapProcedureCompleteEvent(evt_blecore_aci* vendorEvent) override;
        void HandleGattCompleteEvent(evt_blecore_aci* vendorEvent) override;
        void HandleL2capConnectionUpdateRequestEvent(evt_blecore_aci* vendorEvent) override;

    private:
        void HandleGapDiscoveryProcedureEvent();
        void HandleGapDirectConnectionEstablishmentEvent();

        void HandleAdvertisingReport(const Advertising_Report_t& advertisingReport);
        void SetConnectionInterval() const;
        void SetPhy();
        void SetDataLength() const;
        void MtuExchange() const;
        void Initialize(const GapService& gapService);

    private:
        static const services::GapConnectionParameters connectionUpdateParameters;

        // Create connection parameters
        const uint16_t leScanInterval = 0x320;
        const uint16_t leScanWindow = 0x320;

        // Connection Interval parameters
        const uint16_t minConnectionEventLength = 0;
        const uint16_t maxConnectionEventLength = 0x280; // 400 ms

        // Terminate connection
        const uint8_t remoteUserTerminatedConnection = 0x13;

        // Discovery parameters
        const uint8_t filterDuplicatesEnabled = 1;
        const uint8_t acceptAllParameters = 1;

        // HCI status
        const uint8_t commandDisallowed = 0x0c;

        bool discovering = false;
        services::GapConnectionParameters connectionParameters;
        infra::Function<void(services::GapCentralObserver&)> onConnection;
    };
}

#endif
