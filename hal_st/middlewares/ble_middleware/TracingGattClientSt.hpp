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

    private:
        services::Tracer& tracer;
    };
}

#endif
