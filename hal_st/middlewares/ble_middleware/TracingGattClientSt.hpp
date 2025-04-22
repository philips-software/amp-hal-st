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

        // Implementation of services::GattClientDiscovery
        void StartServiceDiscovery() override;
        void StartCharacteristicDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle) override;
        void StartDescriptorDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle) override;

        // Implementation of services::GattClientCharacteristicOperations
        void Read(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(const infra::ConstByteRange&)>& onResponse, const infra::Function<void(uint8_t)>& onDone) override;
        void Write(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data, const infra::Function<void(uint8_t)>& onDone) override;
        services::GattCharacteristicOperationsResult WriteWithoutResponse(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data) override;
        void EnableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        void DisableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        void EnableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        void DisableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(uint8_t)>& onDone) override;
        
    protected:
        void HandleGattIndicationEvent(evt_blecore_aci* vendorEvent) override;
        void HandleGattNotificationEvent(evt_blecore_aci* vendorEvent) override;
        void HandleGattCompleteResponse(evt_blecore_aci* vendorEvent) override;
        void HandleServiceDiscovered(infra::DataInputStream& stream, bool isUuid16) override;

    private:
        services::Tracer& tracer;
    };
}

#endif
