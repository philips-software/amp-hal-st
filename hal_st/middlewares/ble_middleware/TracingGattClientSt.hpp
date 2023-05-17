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
        virtual void StartServiceDiscovery() override;
        virtual void StartCharacteristicDiscovery(const services::GattService& service) override;
        virtual void StartDescriptorDiscovery(const services::GattService& service) override;

        // Implementation of services::GattClientCharacteristicOperations
        virtual void Read(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(const infra::ConstByteRange&)>& onResponse) const override;
        virtual void Write(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data, const infra::Function<void()>& onDone) const override;
        virtual void WriteWithoutResponse(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data) const override;

        virtual void EnableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const override;
        virtual void DisableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const override;
        virtual void EnableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const override;
        virtual void DisableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const override;

    protected:
        virtual void HandleGattIndicationEvent(evt_blecore_aci* vendorEvent) override;
        virtual void HandleGattNotificationEvent(evt_blecore_aci* vendorEvent) override;

    private:
        services::Tracer& tracer;
    };
}

#endif
