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

        // From services::GattServer
        virtual void AddService(services::GattService& service);

        // From services::GattCharacteristicClientOperations
        virtual UpdateStatus Update(const services::GattCharacteristicClientOperationsObserver& characteristic, infra::ConstByteRange data) const;

    protected:
        virtual void AddCharacteristic(services::GattCharacteristic& characteristic);
        virtual void HandleGattAttributeModified(aci_gatt_attribute_modified_event_rp0& event);
        virtual void ReportError(tBleStatus status) const;

    private:
        services::Tracer& tracer;
    };
}

#endif
