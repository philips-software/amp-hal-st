#ifndef HAL_ST_GAP_PERIPHERAL_ST_HPP
#define HAL_ST_GAP_PERIPHERAL_ST_HPP

#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/ProxyCreator.hpp"
#include "services/ble/BondStorageSynchronizer.hpp"
#include "services/ble/Gap.hpp"

namespace hal
{
    class GapPeripheralSt
        : public services::GapPeripheral
        , public services::GapPeripheralBonding
        , public services::GapPeripheralPairing
        , public GapSt
    {
    public:
        GapPeripheralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const hal::GapSt::RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService gapService, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t* bleBondsStorage);

        // Implementation of GapPeripheral
        services::GapAddress GetAddress() const override;
        services::GapAddress GetIdentityAddress() const override;
        void SetAdvertisementData(infra::ConstByteRange data) override;
        infra::ConstByteRange GetAdvertisementData() const override;
        void SetScanResponseData(infra::ConstByteRange data) override;
        infra::ConstByteRange GetScanResponseData() const override;
        void Advertise(services::GapAdvertisementType type, AdvertisementIntervalMultiplier multiplier) override;
        void Standby() override;

        // Implementation of GapPeripheralBonding
        void RemoveAllBonds() override;
        void RemoveOldestBond() override;
        std::size_t GetMaxNumberOfBonds() const override;
        std::size_t GetNumberOfBonds() const override;

        // Implementation of GapPeripheralPairing
        void AllowPairing(bool allow) override;
        void SetSecurityMode(services::GapSecurityMode mode, services::GapSecurityLevel level) override;
        void SetIoCapabilities(services::GapIoCapabilities caps) override;
        void AuthenticateWithPasskey(uint32_t passkey) override;
        void NumericComparisonConfirm(bool accept) override;

    protected:
        // Implementation of GapSt
        void HandleHciDisconnectEvent(hci_event_pckt& eventPacket) override;
        void HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent) override;
        void HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent) override;

    private:
        void RequestConnectionParameterUpdate();
        void UpdateAdvertisementData();
        void UpdateState(services::GapState newstate);
        void UpdateResolvingList();
        void ClearResolvingList();
        void Initialize(const GapService& gapService);

    private:
        infra::Optional<infra::ProxyCreator<services::BondStorageSynchronizer, void()>> bondStorageSynchronizer;
        services::GapState state = services::GapState::standby;
        bool allowPairing = true;

        infra::BoundedVector<uint8_t>::WithMaxSize<maxAdvertisementDataSize> advertisementData;
        infra::BoundedVector<uint8_t>::WithMaxSize<maxScanResponseDataSize> scanResponseData;

        const Whitelist_Identity_Entry_t dummyPeer {0x01, {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF}};
    };
}

#endif
