#ifndef HAL_ST_TRACING_GATT_SERVER_ST_HPP
#define HAL_ST_TRACING_GATT_SERVER_ST_HPP

#include "hal_st/middlewares/ble_middleware/GattServerSt.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal
{
    class TracingGattServerSt
        : public GattServerSt
    {
    public:
        TracingGattServerSt(hal::HciEventSource& hciEventSource, services::Tracer& tracer);

        // Implementation of services::GattServer
        void AddService(services::GattServerService& service) override;

        // Implementation of services::GattServerCharacteristicOperations
        UpdateStatus Update(const services::GattServerCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data) override;

    protected:
        void AddCharacteristic(services::GattServerCharacteristic& characteristic) override;
        void AddCharacteristicDescriptor(const services::GattServerCharacteristic& characteristic, services::GattServerDescriptor& descriptor) override;
        void HandleGattAttributeModified(aci_gatt_attribute_modified_event_rp0& event) override;
        void ReportError(tBleStatus status) const override;

    private:
        services::Tracer& tracer;
    };
}

#endif
