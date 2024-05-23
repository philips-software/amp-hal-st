#ifndef HAL_ST_TRACING_SYSTEM_TRANSPORT_LAYER_HPP
#define HAL_ST_TRACING_SYSTEM_TRANSPORT_LAYER_HPP

#include "hal_st/middlewares/ble_middleware/SystemTransportLayer.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal
{
    class TracingSystemTransportLayer
        : public SystemTransportLayer
    {
    public:
        TracingSystemTransportLayer(services::ConfigurationStoreAccess<infra::ByteRange> flashStorage, const infra::Function<void(uint32_t*)>& protocolStackInitialized, services::Tracer& tracer);

    protected:
#if defined(STM32WB)
        void UserEventHandler(void* pPayload) override;
        void HandleReadyEvent(void* pPayload) override;
        void HandleErrorNotifyEvent(void* event) override;
        void HandleBleNvmRamUpdateEvent(void* event) override;
        void HandleUnknownEvent(void* event) override;

        void HandleWirelessFwEvent(void* pPayload) override;
        void HandleFusFwEvent(void* pPayload) override;
        void HandleUnknwownReadyEvent(void* pPayload) override;
#endif

    private:
        services::Tracer& tracer;
    };
}

#endif
