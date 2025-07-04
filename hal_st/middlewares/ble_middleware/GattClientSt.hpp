#ifndef HAL_ST_GATT_CLIENT_ST_HPP
#define HAL_ST_GATT_CLIENT_ST_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/stream/ByteInputStream.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/Function.hpp"
#include "services/ble/GattClient.hpp"

namespace hal
{
    class GattClientSt
        : public services::GattClient
        , public hal::HciEventSink
    {
    public:
        explicit GattClientSt(hal::HciEventSource& hciEventSource);

        // Implementation of services::GattClientDiscovery
        void StartServiceDiscovery() override;
        void StartCharacteristicDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle) override;
        void StartDescriptorDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle) override;

        // Implementation of services::GattClientCharacteristicOperations
        void Read(const services::GattClientObserver& characteristic, const infra::Function<void(const infra::ConstByteRange&)>& onResponse, const infra::Function<void(uint8_t)>& onDone) override;
        void Write(const services::GattClientObserver& characteristic, infra::ConstByteRange data, const infra::Function<void(uint8_t)>& onDone) override;
        void WriteWithoutResponse(const services::GattClientObserver& characteristic, infra::ConstByteRange data) override;
        void EnableNotification(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        void DisableNotification(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        void EnableIndication(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        void DisableIndication(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;

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
        virtual void HandleGattConfirmIndication();
        virtual void HandleGattCompleteResponse(evt_blecore_aci* vendorEvent);

        virtual void HandleAttReadResponse(evt_blecore_aci* vendorEvent);
        virtual void HandleAttReadByGroupTypeResponse(evt_blecore_aci* vendorEvent);
        virtual void HandleAttReadByTypeResponse(evt_blecore_aci* vendorEvent);
        virtual void HandleAttFindInfoResponse(evt_blecore_aci* vendorEvent);

        virtual void HandleServiceDiscovered(infra::DataInputStream& stream, bool isUuid16);
        void HandleUuidFromDiscovery(infra::DataInputStream& stream, bool isUuid16, services::AttAttribute::Uuid& type);

    private:
        void HandleCharacteristicDiscovered(infra::DataInputStream& stream, bool isUuid16);
        void HandleDescriptorDiscovered(infra::DataInputStream& stream, bool isUuid16);

        void WriteCharacteristicDescriptor(const services::GattClientObserver& characteristic, services::GattCharacteristic::PropertyFlags property, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue characteristicValue) const;

        template<services::GattCharacteristic::PropertyFlags p, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue v>
        void WriteCharacteristicDescriptor(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
        {
            this->onCharacteristicOperationsDone = [this, onDone](uint8_t result)
            {
                onDone(result);
            };

            WriteCharacteristicDescriptor(characteristic, p, v);
        }

    protected:
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

        infra::AutoResetFunction<void()> onDiscoveryCompletion;
        infra::AutoResetFunction<void(const infra::ConstByteRange&)> onReadResponse;
        infra::AutoResetFunction<void(uint8_t), sizeof(void*) + sizeof(infra::Function<void()>)> onCharacteristicOperationsDone;
    };
}

#endif
