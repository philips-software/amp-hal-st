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
        std::optional<hal::MacAddress> ResolvePrivateAddress(hal::MacAddress address) const override;

        // Implementation of GapPairing
        void AllowPairing(bool allow) override;
        void NumericComparisonConfirm(bool accept) override;

    protected:
        [[nodiscard]] SecureConnection SecurityLevelToSecureConnection(services::GapPairing::SecurityLevel level) const override;

        void HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event) override;
        void HandleHciLeAdvertisingReportEvent(const hci_le_advertising_report_event_rp0& event) override;
        void HandleHciLeConnectionCompleteEvent(const hci_le_connection_complete_event_rp0& event) override;
        void HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event) override;
        void HandleHciLeDataLengthChangeEvent(const hci_le_data_length_change_event_rp0& event) override;
        void HandleHciLePhyUpdateCompleteEvent(const hci_le_phy_update_complete_event_rp0& event) override;
        void HandleNumericComparisonValueEvent(const aci_gap_numeric_comparison_value_event_rp0& event) override;
        void HandleGapProcedureCompleteEvent(const aci_gap_proc_complete_event_rp0& event) override;
        void HandleGattCompleteEvent(const aci_gatt_proc_complete_event_rp0& event) override;
        void HandleL2capConnectionUpdateRequestEvent(const aci_l2cap_connection_update_req_event_rp0& event) override;

    private:
        void HandleGapDiscoveryProcedureEvent();
        void HandleGapDirectConnectionProcedureCompleteEvent();

        void HandleAdvertisingReport(const Advertising_Report_t& advertisingReport);
        void SetDataLength();
        void Initialize(const GapService& gapService);
        void UpdateStateOnConnectionComplete(uint8_t status);
        void HandleConnectionCompleteCommon(uint8_t status);
        void HandleOobDataGeneration();
        void Initialize(const Configuration& configuration);

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
        infra::TimerSingleShot initiatingStateTimer;
    };
}

#endif
