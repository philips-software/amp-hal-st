#ifndef HAL_ST_GATT_CLIENT_ST_HPP
#define HAL_ST_GATT_CLIENT_ST_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/stream/ByteInputStream.hpp"
#include "services/ble/GattClient.hpp"

namespace hal
{
    class GattClientSt
        : public services::GattClientDiscovery
        , public services::GattClientCharacteristicOperations
        , public hal::HciEventSink
    {
    public:
        explicit GattClientSt(hal::HciEventSource& hciEventSource);

        // Implementation of services::GattClientDiscovery
        void StartServiceDiscovery() override;
        void StartCharacteristicDiscovery(const services::GattService& service) override;
        void StartDescriptorDiscovery(const services::GattService& service) override;

        // Implementation of services::GattClientCharacteristicOperations
        void Read(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(const infra::ConstByteRange&)>& onDone) const override;
        void Write(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data, const infra::Function<void()>& onDone) const override;
        void WriteWithoutResponse(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data) const override;

        void EnableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const override;
        void DisableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const override;
        void EnableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const override;
        void DisableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const override;

        // Implementation of hal::HciEventSink
        void HciEvent(hci_event_pckt& event) override;

    protected:
        virtual void HandleHciDisconnectEvent(hci_event_pckt& eventPacket);
        virtual void HandleHciLeMetaEvent(hci_event_pckt& eventPacket);
        virtual void HandleHciVendorSpecificDebugEvent(hci_event_pckt& eventPacket);

        virtual void HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent);
        virtual void HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent);

        virtual void HandleGattIndicationEvent(evt_blecore_aci* vendorEvent);
        virtual void HandleGattNotificationEvent(evt_blecore_aci* vendorEvent);
        virtual void HandleGattConfirmIndication(services::AttAttribute::Handle handle);
        virtual void HandleGattCompleteResponse(evt_blecore_aci* vendorEvent);

        virtual void HandleAttReadResponse(evt_blecore_aci* vendorEvent);
        virtual void HandleAttReadByGroupTypeResponse(evt_blecore_aci* vendorEvent);
        virtual void HandleAttReadByTypeResponse(evt_blecore_aci* vendorEvent);
        virtual void HandleAttFindInfoResponse(evt_blecore_aci* vendorEvent);

    private:
        void HandleServiceDiscovered(infra::DataInputStream& stream, bool isUuid16);
        void HandleCharacteristicDiscovered(infra::DataInputStream& stream, bool isUuid16);
        void HandleDescriptorDiscovered(infra::DataInputStream& stream, bool isUuid16);
        void HandleUuidFromDiscovery(infra::DataInputStream& stream, bool isUuid16, services::AttAttribute::Uuid& type);

        void WriteCharacteristicDescriptor(const services::GattClientCharacteristicOperationsObserver& characteristic, services::GattCharacteristic::PropertyFlags property, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue characteristicValue) const;

    private:
        struct Atttributes
        {
            services::AttAttribute::Uuid type;
            services::AttAttribute::Handle startHandle;
            services::AttAttribute::Handle endHandle;
            services::GattCharacteristic::PropertyFlags properties;
        };

    private:
        uint16_t connectionHandle;

        static constexpr uint16_t invalidConnection = 0xffff;

        infra::Function<void(services::GattClientDiscoveryObserver&)> onDiscoveryCompletion;
        mutable infra::AutoResetFunction<void(const infra::ConstByteRange&)> onResponse;
        mutable infra::AutoResetFunction<void()> onDone;
    };
}

#endif
