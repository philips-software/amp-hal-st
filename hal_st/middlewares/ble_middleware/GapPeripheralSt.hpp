#ifndef HAL_ST_GAP_PERIPHERAL_ST_HPP
#define HAL_ST_GAP_PERIPHERAL_ST_HPP

#include "ble/ble.h"
#include "ble/svc/Inc/svc_ctl.h"
#include "hal_st/middlewares/ble_middleware/GapSt.hpp"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
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
    class GapPeripheralSt
        : public services::GapPeripheral
        , public services::GapPeripheralBonding
        , public services::GapPeripheralPairing
        , public GapSt
    {
    public:
        GapPeripheralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, const hal::GapSt::RootKeys& rootKeys, uint16_t maxAttMtuSize, uint8_t txPowerLevel, const GapService gapService, infra::CreatorBase<services::BondStorageSynchronizer, void()>& bondStorageSynchronizerCreator, uint32_t* bleBondsStorage);

        // Implementation of GapPeripheral
        virtual services::GapAddress GetAddress() const override;
        virtual services::GapAddress GetIdentityAddress() const override;
        virtual void SetAdvertisementData(infra::ConstByteRange data) override;
        virtual infra::ConstByteRange GetAdvertisementData() override;
        virtual void SetScanResponseData(infra::ConstByteRange data) override;
        virtual infra::ConstByteRange GetScanResponseData() override;
        virtual void Advertise(services::GapAdvertisementType type, AdvertisementIntervalMultiplier multiplier) override;
        virtual void Standby() override;

        // Implementation of GapPeripheralBonding
        virtual void RemoveAllBonds() override;
        virtual void RemoveOldestBond() override;
        virtual std::size_t GetMaxNumberOfBonds() const override;
        virtual std::size_t GetNumberOfBonds() const override;

        // Implementation of GapPeripheralPairing
        virtual void AllowPairing(bool allow) override;
        virtual void SetSecurityMode(services::GapSecurityMode mode, services::GapSecurityLevel level) override;
        virtual void SetIoCapabilities(services::GapIoCapabilities caps) override;
        virtual void AuthenticateWithPasskey(uint32_t passkey) override;
        virtual void NumericComparisonConfirm(bool accept) override;

    protected:
        // Implementation of GapSt
        virtual void HciEvent(hci_event_pckt& event);

    protected:
        virtual void HandleHciDisconnectEvent(hci_event_pckt& metaEvent);
        virtual void HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent);
        virtual void HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent);
        virtual void HandleHciLeConnectionUpdateCompleteEvent(evt_le_meta_event* metaEvent) {};
        virtual void HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent) {};
        virtual void HandleHciLeDataLengthUpdateEvent(evt_le_meta_event* metaEvent) {};

    private:
        void HandleHciLeMetaEvent(hci_event_pckt& eventPacket);
        void HandleHciVendorSpecificDebugEvent(hci_event_pckt& eventPacket);
        void RequestConnectionParameterUpdate();
        void UpdateAdvertisementData();
        void UpdateState(services::GapState newstate);
        void Initialize(const GapService& gapService);

    private:
        infra::Optional<infra::ProxyCreator<services::BondStorageSynchronizer, void()>> bondStorageSynchronizer;
        services::GapState state = services::GapState::standby;
        bool allowPairing = true;

        infra::BoundedVector<uint8_t>::WithMaxSize<maxAdvertisementDataSize> advertisementData;
        infra::BoundedVector<uint8_t>::WithMaxSize<maxScanResponseDataSize> scanResponseData;
    };
}

#endif
