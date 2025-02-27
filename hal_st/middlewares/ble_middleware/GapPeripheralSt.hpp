#ifndef HAL_ST_GAP_PERIPHERAL_ST_HPP
#define HAL_ST_GAP_PERIPHERAL_ST_HPP

#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/Optional.hpp"

namespace hal
{
    class GapPeripheralSt
        : public services::GapPeripheral
        , public GapSt
    {
    public:
        GapPeripheralSt(hal::HciEventSource& hciEventSource, services::BondStorageSynchronizer& bondStorageSynchronizer, const Configuration& configuration);

        virtual void ConnectionUpdate(infra::Function<void(uint16_t intervalMultiplier)> onConnectionUpdate) override;
        infra::Function<void(uint16_t intervalMultiplier)> onConnectionUpdate;
        virtual void HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent) override;

        // Implementation of GapPeripheral
        services::GapAddress GetAddress() const override;
        services::GapAddress GetIdentityAddress() const override;
        void SetAdvertisementData(infra::ConstByteRange data) override;
        infra::ConstByteRange GetAdvertisementData() const override;
        void SetScanResponseData(infra::ConstByteRange data) override;
        infra::ConstByteRange GetScanResponseData() const override;
        void Advertise(services::GapAdvertisementType type, AdvertisementIntervalMultiplier multiplier) override;
        void Standby() override;

        // Implementation of GapPairing
        void AllowPairing(bool allow) override;

    protected:
        // Implementation of GapSt
        void HandleHciDisconnectEvent(hci_event_pckt& eventPacket) override;
        void HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent) override;

    private:
        void RequestConnectionParameterUpdate();
        void UpdateAdvertisementData();
        void UpdateState(services::GapState newstate);
        void UpdateResolvingList();
        void ClearResolvingList();
        void Initialize(const GapService& gapService);

    private:
        services::GapState state = services::GapState::standby;
        bool allowPairing = true;

        infra::BoundedVector<uint8_t>::WithMaxSize<maxAdvertisementDataSize> advertisementData;
        infra::BoundedVector<uint8_t>::WithMaxSize<maxScanResponseDataSize> scanResponseData;

        const Whitelist_Identity_Entry_t dummyPeer{ 0x01, { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF } };
    };
}

#endif
