#ifndef HAL_ST_GAP_PERIPHERAL_ST_HPP
#define HAL_ST_GAP_PERIPHERAL_ST_HPP

#include "ble/ble.h"
#include "ble/svc/Inc/svc_ctl.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "hci_tl.h"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/ProxyCreator.hpp"
#include "services/ble/BondStorageManager.hpp"
#include "services/ble/Gatt.hpp"
#include "services/ble/Gap.hpp"
#include "shci.h"

namespace hal
{
    class GapPeripheralSt
        : public services::GapPeripheral
        , public services::AttMtuExchange
        , public services::GapPeripheralBonding
        , public services::GapPeripheralPairing
        , public hal::HciEventSink
    {
    public:
        GapPeripheralSt(hal::HciEventSource& hciEventSource, hal::MacAddress address, uint16_t maxAttMtuSize, infra::CreatorBase<services::BondStorageManager, void()>& bondStorageManagerCreator);

        // Implementation of GapPeripheral
        virtual hal::MacAddress GetPublicAddress() const override;
        virtual void SetAdvertisementData(infra::ConstByteRange data) override;
        virtual void SetScanResponseData(infra::ConstByteRange data) override;
        virtual void Advertise(AdvertisementType type, AdvertisementIntervalMultiplier multiplier) override;
        virtual void Standby() override;

        // Implementation of GapPeripheralBonding
        virtual void RemoveAllBonds();

        // Implementation of GapPeripheralPairing
        virtual void AllowPairing(bool allow) override;

        // Implementation of HciEventSink
        virtual void HciEvent(hci_event_pckt& event);

        // Implementation of AttMtuExchange
        virtual uint16_t EffectiveMaxAttMtuSize() const override;

    private:
        void UpdateAdvertisementData();

        void SetPublicAddress(const hal::MacAddress& address);
        void UpdateState(services::GapPeripheralState newstate);

        void HciGapGattInit();
        void HandleHciLeMetaEvent(hci_event_pckt& eventPacket);
        void HandleHciVendorSpecificDebugEvent(hci_event_pckt& eventPacket);

    protected:
        virtual void HandleHciDisconnectEvent(hci_event_pckt& eventPacket);
        virtual void HandleHciUnknownEvent(hci_event_pckt& eventPacket);

        virtual void HandleHciLeConnectionUpdateEvent(evt_le_meta_event* metaEvent);
        virtual void HandleHciLePhyUpdateCompleteEvent(evt_le_meta_event* metaEvent);
        virtual void HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent);
        virtual void HandleHciLeDataLengthUpdateEvent(evt_le_meta_event* metaEvent);
        virtual void HandleHciLeUnknownEvent(evt_le_meta_event* metaEvent);

        virtual void HandleBondLostEvent(evt_blecore_aci* vendorEvent);
        virtual void HandlePairingCompleteEvent(evt_blecore_aci* vendorEvent);
        virtual void HandleMtuExchangeEvent(evt_blecore_aci* vendorEvent);

    private:
        struct ConnectionContext
        {
            uint16_t connectionHandle;
            uint8_t peerAddressType;
            uint8_t peerAddress[6];
        };

    private:
        infra::Optional<infra::ProxyCreator<services::BondStorageManager, void()>> bondStorageManager;
        services::GapPeripheralState state = services::GapPeripheralState::Standby;
        bool allowPairing = true;

        ConnectionContext connectionContext;

        const char* devName = "ST-BLE";
        const uint8_t txPowerLevel = 0x18;
        const uint8_t bondingMode = 0x01;
        const uint8_t ioCapability = 0x03;
        const uint8_t mitmMode = 0x00;
        const uint8_t secureConnectionSupport = 0x01;
        const uint8_t keypressNotificationSupport = 0x00;

        const uint8_t identityRootKey[16] = { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0 };
        const uint8_t encryptionRootKey[16] = { 0xFE, 0xDC, 0xBA, 0x09, 0x87, 0x65, 0x43, 0x21, 0xFE, 0xDC, 0xBA, 0x09, 0x87, 0x65, 0x43, 0x21 };

        infra::BoundedVector<uint8_t>::WithMaxSize<maxAdvertisementDataSize> advertisementData;
        infra::BoundedVector<uint8_t>::WithMaxSize<maxScanResponseDataSize> scanResponseData;

        uint16_t maxAttMtu = defaultMaxAttMtuSize;
    };
}

#endif
