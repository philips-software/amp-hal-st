#ifndef HAL_ST_GAP_ST_HPP
#define HAL_ST_GAP_ST_HPP

#include "ble/ble.h"
#include "ble_defs.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/util/BoundedString.hpp"
#include "services/ble/BondStorageSynchronizer.hpp"
#include "services/ble/Gap.hpp"
#include "services/ble/Gatt.hpp"

namespace hal
{
    class GapSt
        : public services::AttMtuExchange
        , public services::GapBonding
        , public services::GapPairing
        , private HciEventSink
    {
    public:
        struct GapService
        {
            infra::BoundedString::WithStorage<32> deviceName;
            uint16_t appearance;
        };

        struct RootKeys
        {
            std::array<uint8_t, 16> identity;
            std::array<uint8_t, 16> encryption;
        };

        struct Security
        {
            services::GapPairing::IoCapabilities ioCapabilities;
            services::GapPairing::SecurityMode securityMode;
            services::GapPairing::SecurityLevel securityLevel;
        };

        static constexpr Security justWorks{ services::GapPairing::IoCapabilities::none, services::GapPairing::SecurityMode::mode1, services::GapPairing::SecurityLevel::level1 };
        static constexpr Security encrypted{ services::GapPairing::IoCapabilities::none, services::GapPairing::SecurityMode::mode1, services::GapPairing::SecurityLevel::level2 };
        static constexpr Security outOfBand{ services::GapPairing::IoCapabilities::none, services::GapPairing::SecurityMode::mode1, services::GapPairing::SecurityLevel::level4 };

        struct Configuration
        {
            const MacAddress& address;
            const GapService& gapService;
            const RootKeys& rootKeys;
            const Security& security;
            uint8_t txPowerLevel;
            bool privacy;
        };

        // Implementation of AttMtuExchange
        uint16_t EffectiveMaxAttMtuSize() const override;
        void MtuExchange() override;

        // Implementation of GapBonding
        void RemoveAllBonds() override;
        void RemoveOldestBond() override;
        std::size_t GetMaxNumberOfBonds() const override;
        std::size_t GetNumberOfBonds() const override;
        bool IsDeviceBonded(MacAddress address, services::GapDeviceAddressType addressType) const override;

        // Implementation of GapPairing
        void PairAndBond() override;
        void SetSecurityMode(services::GapPairing::SecurityMode mode, services::GapPairing::SecurityLevel level) override;
        void SetIoCapabilities(services::GapPairing::IoCapabilities caps) override;
        void AuthenticateWithPasskey(uint32_t passkey) override;
        void NumericComparisonConfirm(bool accept) override;
        void GenerateOutOfBandData() override;
        void SetOutOfBandData(const services::GapOutOfBandData& outOfBandData) override;

    protected:
        enum class SecureConnection : uint8_t
        {
            notSupported = 0,
            optional = 1,
            mandatory
        };

        GapSt(HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration);

        virtual void HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event);

        virtual void HandleHciLeConnectionCompleteEvent(const hci_le_connection_complete_event_rp0& event);
        virtual void HandleHciLeAdvertisingReportEvent(const hci_le_advertising_report_event_rp0& event) {};
        virtual void HandleHciLeConnectionUpdateCompleteEvent(const hci_le_connection_update_complete_event_rp0& event) {};
        virtual void HandleHciLeDataLengthChangeEvent(const hci_le_data_length_change_event_rp0& event) {};
        virtual void HandleHciLePhyUpdateCompleteEvent(const hci_le_phy_update_complete_event_rp0& event) {};
        virtual void HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event);
        virtual void HandleHciLeReadLocalP256PublicKeyCompleteEvent(const hci_le_read_local_p256_public_key_complete_event_rp0& event);

        virtual void HandlePairingCompleteEvent(const aci_gap_pairing_complete_event_rp0& event);
        virtual void HandleBondLostEvent();
        virtual void HandleNumericComparisonValueEvent(const aci_gap_numeric_comparison_value_event_rp0& event) {};
        virtual void HandleGapProcedureCompleteEvent(const aci_gap_proc_complete_event_rp0& event) {};
        virtual void HandleGattCompleteEvent(const aci_gatt_proc_complete_event_rp0& event) {};
        virtual void HandleL2capConnectionUpdateRequestEvent(const aci_l2cap_connection_update_req_event_rp0& event) {};
        virtual void HandleMtuExchangeResponseEvent(const aci_att_exchange_mtu_resp_event_rp0& event);

        [[nodiscard]] virtual SecureConnection SecurityLevelToSecureConnection(services::GapPairing::SecurityLevel level) const;
        [[nodiscard]] virtual uint8_t SecurityLevelToMITM(services::GapPairing::SecurityLevel level) const;

        void SetAddress(const MacAddress& address, services::GapDeviceAddressType addressType) const;

    private:
        // Implementation of HciEventSink
        void HciEvent(hci_event_pckt& event) override;

        void HandleHciLeMetaEvent(const evt_le_meta_event& metaEvent);
        void HandleHciVendorSpecificDebugEvent(const evt_blecore_aci& event);
        void HandleOobDataGeneration();

        void SetConnectionContext(uint16_t connectionHandle, services::GapDeviceAddressType peerAddressType, const uint8_t* peerAddress);
        void UpdateNrBonds();

    protected:
        struct ConnectionContext
        {
            uint16_t connectionHandle;
            services::GapDeviceAddressType peerAddressType;
            MacAddress peerAddress;
        };

        ConnectionContext connectionContext;
        uint8_t ownAddressType;
        services::GapPairing::SecurityLevel securityLevel;

        const uint16_t invalidConnection = 0xffff;

        // Set PHY
        const uint8_t allPhys = 0;
        const uint8_t speed1Mbps = 0x1;
        const uint8_t speed2Mbps = 0x2;

        const uint8_t ioCapability = IO_CAP_NO_INPUT_NO_OUTPUT;
        const uint8_t bondingMode = BONDING;
        const uint8_t secureConnectionSupport = 0x01; /* Secure Connections Pairing supported but optional */
        const uint8_t keypressNotificationSupport = KEYPRESS_SUPPORTED;
        static constexpr uint8_t maxNumberOfBonds = 10;

    private:
        services::BondStorageSynchronizer& bondStorageSynchronizer;
        uint16_t maxAttMtu = defaultMaxAttMtuSize;
    };
}

#endif
