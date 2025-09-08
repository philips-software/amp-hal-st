#ifndef HAL_ST_TRACING_GATT_CLIENT_ST_HPP
#define HAL_ST_TRACING_GATT_CLIENT_ST_HPP

#include "hal_st/middlewares/ble_middleware/GattClientSt.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal
{
    class TracingGattClientSt
        : public GattClientSt
    {
    public:
        explicit TracingGattClientSt(hal::HciEventSource& hciEventSource, services::Tracer& tracer);

        // Implementation of services::GattClient
        void StartServiceDiscovery() override;
        void StartCharacteristicDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle) override;
        void StartDescriptorDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle) override;
        void Read(services::AttAttribute::Handle handle, const infra::Function<void(const infra::ConstByteRange&)>& onResponse, const infra::Function<void(uint8_t)>& onDone) override;
        void Write(services::AttAttribute::Handle handle, infra::ConstByteRange data, const infra::Function<void(uint8_t)>& onDone) override;
        void WriteWithoutResponse(services::AttAttribute::Handle handle, infra::ConstByteRange data) override;
        void EnableNotification(services::AttAttribute::Handle handle, const infra::Function<void(uint8_t)>& onDone) override;
        void DisableNotification(services::AttAttribute::Handle handle, const infra::Function<void(uint8_t)>& onDone) override;
        void EnableIndication(services::AttAttribute::Handle handle, const infra::Function<void(uint8_t)>& onDone) override;
        void DisableIndication(services::AttAttribute::Handle handle, const infra::Function<void(uint8_t)>& onDone) override;

    protected:
        void HandleGattIndicationEvent(const aci_gatt_indication_event_rp0& event) override;
        void HandleGattNotificationEvent(const aci_gatt_notification_event_rp0& event) override;
        void HandleGattCompleteResponse(const aci_gatt_proc_complete_event_rp0& event) override;
        void HandleServiceDiscovered(infra::DataInputStream& stream, bool isUuid16) override;

    private:
        services::Tracer& tracer;
    };
}

#endif
