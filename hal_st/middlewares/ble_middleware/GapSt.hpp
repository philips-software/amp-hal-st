#ifndef HAL_ST_GAP_ST_HPP
#define HAL_ST_GAP_ST_HPP

#include "ble/ble.h"
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

        struct Configuration
        {
            const MacAddress& address;
            const GapService& gapService;
            const RootKeys& rootKeys;
            uint8_t txPowerLevel;
        };

        // Implementation of AttMtuExchange
        uint16_t EffectiveMaxAttMtuSize() const override;

        // Implementation of GapBonding
        void RemoveAllBonds() override;
        void RemoveOldestBond() override;
        std::size_t GetMaxNumberOfBonds() const override;
        std::size_t GetNumberOfBonds() const override;
        bool IsDeviceBonded(MacAddress address, services::GapDeviceAddressType addressType) const override;

        // Implementation of GapPairing
        void Pair() override;
        void SetSecurityMode(services::GapPairing::SecurityMode mode, services::GapPairing::SecurityLevel level) override;
        void SetIoCapabilities(services::GapPairing::IoCapabilities caps) override;
        void AuthenticateWithPasskey(uint32_t passkey) override;
        void NumericComparisonConfirm(bool accept) override;

    protected:
        GapSt(HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration);

        virtual void HandleHciDisconnectEvent(hci_event_pckt& eventPacket);

        virtual void HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent);
        virtual void HandleHciLeAdvertisingReportEvent(evt_le_meta_event* metaEvent){};
        virtual void HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent){};
        virtual void HandleHciLeDataLengthChangeEvent(evt_le_meta_event* metaEvent){};
        virtual void HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent){};
        virtual void HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent);

        virtual void HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent);
        virtual void HandleBondLostEvent(evt_blecore_aci* vendorEvent);
        virtual void HandleGapProcedureCompleteEvent(evt_blecore_aci* vendorEvent){};
        virtual void HandleGattCompleteEvent(evt_blecore_aci* vendorEvent){};
        virtual void HandleL2capConnectionUpdateRequestEvent(evt_blecore_aci* vendorEvent){};
        virtual void HandleMtuExchangeResponseEvent(evt_blecore_aci* vendorEvent);

        void SetAddress(const MacAddress& address, services::GapDeviceAddressType addressType);

    private:
        // Implementation of HciEventSink
        void HciEvent(hci_event_pckt& event) override;

        void HandleHciLeMetaEvent(hci_event_pckt& eventPacket);
        void HandleHciVendorSpecificDebugEvent(hci_event_pckt& eventPacket);

        void SetConnectionContext(uint16_t connectionHandle, services::GapDeviceAddressType peerAddressType, uint8_t* peerAddress);
        void UpdateNrBonds();

    protected:
        struct ConnectionContext
        {
            uint16_t connectionHandle;
            services::GapDeviceAddressType peerAddressType;
            MacAddress peerAddress;
        };

        ConnectionContext connectionContext;

        const uint16_t invalidConnection = 0xffff;

        // Set PHY
        const uint8_t allPhys = 0;
        const uint8_t speed1Mbps = 0x1;
        const uint8_t speed2Mbps = 0x2;

        const uint8_t ioCapability = IO_CAP_NO_INPUT_NO_OUTPUT;
        const uint8_t bondingMode = BONDING;
        const uint8_t mitmMode = MITM_PROTECTION_NOT_REQUIRED;
        const uint8_t secureConnectionSupport = 0x01; /* Secure Connections Pairing supported but optional */
        const uint8_t keypressNotificationSupport = KEYPRESS_SUPPORTED;
        static constexpr uint8_t maxNumberOfBonds = 10;

    private:
        services::BondStorageSynchronizer& bondStorageSynchronizer;
        uint16_t maxAttMtu = defaultMaxAttMtuSize;
    };
}

#endif
