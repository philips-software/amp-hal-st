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
        TracingSystemTransportLayer(const infra::Function<void()>& protoStackInit, services::Tracer& tracer);

    protected:
        virtual void UserEventHandler(void* pPayload) override;
        virtual void HandleReadyEvent(void* pPayload) override;
        virtual void HandleErrorNotifyEvent(TL_AsynchEvt_t* sysEvent) override;
        virtual void HandleUnknownEvent(TL_AsynchEvt_t* sysEvent) override;

        virtual void HandleWirelessFwEvent(void* pPayload) override;
        virtual void HandleFusFwEvent(void* pPayload) override;
        virtual void HandleUnknwownReadyEvent(void* pPayload) override;

    private:
        services::Tracer& tracer;
    };
}

#endif
