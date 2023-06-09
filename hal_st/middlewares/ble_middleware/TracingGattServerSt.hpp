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
        void AddService(services::GattServerService& service) override;

        // From services::GattCharacteristicClientOperations
        UpdateStatus Update(const services::GattServerCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data) const override;

    protected:
        void AddCharacteristic(services::GattServerCharacteristic& characteristic) override;
        void HandleAttExchangeMtuResponseEvent(evt_blecore_aci* vendorEvent) override;
        void HandleGattAttributeModifiedResponseEvent(evt_blecore_aci* vendorEvent) override;
        void ReportError(tBleStatus status) const override;

    private:
        services::Tracer& tracer;
    };
}

#endif
