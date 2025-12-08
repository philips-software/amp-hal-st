#ifndef HAL_ST_GATT_CLIENT_ST_HPP
#define HAL_ST_GATT_CLIENT_ST_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/stream/ByteInputStream.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "infra/util/BoundedVector.hpp"
#include "infra/util/Function.hpp"
#include "services/ble/Gatt.hpp"
#include "services/ble/GattClient.hpp"

namespace hal
{
    class GattClientSt
        : public services::GattClient
        , public hal::HciEventSink
        , public services::AttMtuExchangeReceiver
    {
    public:
        explicit GattClientSt(hal::HciEventSource& hciEventSource);

        // Implementation of services::GattClient
        void StartServiceDiscovery() override;
        void StartCharacteristicDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle) override;
        void StartDescriptorDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle) override;
        void Read(services::AttAttribute::Handle handle, const infra::Function<void(const infra::ConstByteRange&)>& onResponse, const infra::Function<void(services::OperationStatus)>& onDone) override;
        void Write(services::AttAttribute::Handle handle, infra::ConstByteRange data, const infra::Function<void(services::OperationStatus)>& onDone) override;
        void WriteWithoutResponse(services::AttAttribute::Handle handle, infra::ConstByteRange data, const infra::Function<void(services::OperationStatus)>& onDone) override;
        void EnableNotification(services::AttAttribute::Handle handle, const infra::Function<void(services::OperationStatus)>& onDone) override;
        void DisableNotification(services::AttAttribute::Handle handle, const infra::Function<void(services::OperationStatus)>& onDone) override;
        void EnableIndication(services::AttAttribute::Handle handle, const infra::Function<void(services::OperationStatus)>& onDone) override;
        void DisableIndication(services::AttAttribute::Handle handle, const infra::Function<void(services::OperationStatus)>& onDone) override;
        void MtuExchange() override;

        // Implementation of hal::HciEventSink
        void HciEvent(hci_event_pckt& event) override;

        // Implementation of AttMtuExchangeReceiver
        uint16_t EffectiveMaxAttMtuSize() const override;

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
        virtual void HandleAttExchangeMtuResponse(const aci_att_exchange_mtu_resp_event_rp0& event);

        virtual void HandleServiceDiscovered(infra::DataInputStream& stream, bool isUuid16);
        void HandleUuidFromDiscovery(infra::DataInputStream& stream, bool isUuid16, services::AttAttribute::Uuid& type);

    private:
        void HandleCharacteristicDiscovered(infra::DataInputStream& stream, bool isUuid16);
        void HandleDescriptorDiscovered(infra::DataInputStream& stream, bool isUuid16);

        void WriteCharacteristicDescriptor(services::AttAttribute::Handle handle, services::GattCharacteristic::PropertyFlags property, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue characteristicValue);

        template<services::GattCharacteristic::PropertyFlags p, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue v>
        void WriteCharacteristicDescriptor(services::AttAttribute::Handle handle, const infra::Function<void(services::OperationStatus)>& onDone)
        {
            this->onCharacteristicOperationsDone = [this, onDone](services::OperationStatus result)
            {
                onDone(result);
            };

            WriteCharacteristicDescriptor(handle, p, v);
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
        uint16_t maxAttMtu{ 23 };

        static constexpr uint16_t invalidConnection = 0xffff;

        infra::AutoResetFunction<void(services::OperationStatus status)> onDiscoveryCompletion;
        infra::AutoResetFunction<void(const infra::ConstByteRange&)> onReadResponse;
        infra::AutoResetFunction<void(services::OperationStatus), sizeof(void*) + sizeof(infra::Function<void(services::OperationStatus)>)> onCharacteristicOperationsDone;
    };
}

#endif
