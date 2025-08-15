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
        void Write(const services::GattClientObserver& characteristic, infra::ConstByteRange data, const infra::Function<void(services::OperationStatus)>& onDone) override;
        void WriteWithoutResponse(const services::GattClientObserver& characteristic, infra::ConstByteRange data, const infra::Function<void(services::OperationStatus)>& onDone) override;
        void EnableNotification(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        void DisableNotification(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        void EnableIndication(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        void DisableIndication(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;

        // Implementation of hal::HciEventSink
        void HciEvent(hci_event_pckt& event) override;

    protected:
        virtual void HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event);
        virtual void HandleHciLeMetaEvent(const evt_le_meta_event& metaEvent);
        virtual void HandleHciVendorSpecificDebugEvent(const evt_blecore_aci& event);

        virtual void HandleHciLeConnectionCompleteEvent(const hci_le_connection_complete_event_rp0& event);
        virtual void HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event);

        virtual void HandleGattIndicationEvent(const aci_gatt_indication_event_rp0& event);
        virtual void HandleGattNotificationEvent(const aci_gatt_notification_event_rp0& event);
        virtual void HandleGattConfirmIndication();
        virtual void HandleGattCompleteResponse(const aci_gatt_proc_complete_event_rp0& event);

        virtual void HandleAttReadResponse(const aci_att_read_resp_event_rp0& event);
        virtual void HandleAttReadByGroupTypeResponse(const aci_att_read_by_group_type_resp_event_rp0& event);
        virtual void HandleAttReadByTypeResponse(const aci_att_read_by_type_resp_event_rp0& event);
        virtual void HandleAttFindInfoResponse(const aci_att_find_info_resp_event_rp0& event);

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
