#ifndef HAL_ST_GAP_CENTRAL_ST_HPP
#define HAL_ST_GAP_CENTRAL_ST_HPP

#include "ble/ble.h"
#include "ble/svc/Inc/svc_ctl.h"
#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "hci_tl.h"
#include "infra/util/BoundedString.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/ProxyCreator.hpp"
#include "services/ble/BondStorageSynchronizer.hpp"
#include "services/ble/Gap.hpp"
#include "services/ble/Gatt.hpp"
#include "shci.h"

namespace hal
{
    class GapCentralSt
        : public services::GapCentral
        , public GapSt
    {
    public:
        GapCentralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService gapService, uint32_t* bleBondsStorage);

        // Implementation of services::GapCentral
        virtual void Connect(hal::MacAddress macAddress, services::GapDeviceAddressType addressType) override;
        virtual void Disconnect() override;
        virtual void SetAddress(hal::MacAddress macAddress, services::GapDeviceAddressType addressType) override;
        virtual void StartDeviceDiscovery() override;
        virtual void StopDeviceDiscovery() override;

    protected:
        virtual void HandleHciDisconnectEvent(hci_event_pckt& eventPacket) override;
        virtual void HandleHciLeAdvertisingReportEvent(evt_le_meta_event* metaEvent) override;
        virtual void HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;
        virtual void HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent) override;
        virtual void HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;
        virtual void HandleGapProcedureCompleteEvent(evt_blecore_aci* vendorEvent) override;
        virtual void HandleL2capConnectionUpdateRequestEvent(evt_blecore_aci* vendorEvent) override;

    private:
        void HandleGapDiscoveryProcedureEvent();
        void HandleGapDirectConnectionEstablishmentEvent();

        void HandleAdvertisingReport(const Advertising_Report_t& advertisingReport);
        void SetConnectionInterval(uint16_t connectionInterval, uint16_t slaveLatency, uint16_t timeoutMultiplier);
        void SetPhy();
        void DataLengthUpdate();
        void ConnectionUpdate();
        void Initialize(const GapService& gapService);

    private:
        static const services::GapConnectionParameters connectionUpdateParameters;

        const uint16_t transmissionOctets = 251;
        const uint16_t transmissionTime = 2120;

        // Create connection parameters
        const uint16_t leScanInterval = 0x320;
        const uint16_t leScanWindow = 0x320;
        const uint16_t connectionMaxCeLength = 0x3e8;

        // Connection Interval parameters
        const uint16_t minConnectionEventLength = 0;
        const uint16_t maxConnectionEventLength = 0x280;

        // Terminate connection
        const uint8_t remoteUserTerminatedConnection = 0x13;

        // Discovery parameters
        const uint8_t filterDuplicatesEnabled = 1;
        const uint8_t acceptAllParameters = 1;

        bool discovering = false;
        services::GapConnectionParameters connectionParameters;
    };
}

#endif
