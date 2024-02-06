#ifndef HAL_ST_SYSTEM_TRANSPORT_LAYER_HPP
#define HAL_ST_SYSTEM_TRANSPORT_LAYER_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "interface/patterns/ble_thread/tl/tl.h"
#include "services/ble/BondBlobPersistence.hpp"
#include "services/tracer/Tracer.hpp"

namespace hal
{
    class SystemTransportLayer
        : public infra::InterfaceConnector<SystemTransportLayer>
        , public HciEventSource
    {
    public:
        struct Version
        {
            uint8_t firmwareMajor;
            uint8_t firmwareMinor;
            uint8_t firmwareSub;
            uint8_t firmwareReleaseType;

            uint8_t fusMajor;
            uint8_t fusMinor;
            uint8_t fusSub;
        };

    public:
        SystemTransportLayer(services::ConfigurationStoreAccess<infra::ByteRange> flashStorage, const infra::Function<void(uint32_t*)>& protocolStackInitialized, services::Tracer& tracer);

        Version GetVersion() const;

        virtual void UserEventHandler(void* payload);
        virtual void HciEventHandler(hci_event_pckt& event);

    protected:
        virtual void HandleReadyEvent(void* payload);
        virtual void HandleErrorNotifyEvent(TL_AsynchEvt_t* event);
        virtual void HandleBleNvmRamUpdateEvent(TL_AsynchEvt_t* sysEvent);
        virtual void HandleUnknownEvent(TL_AsynchEvt_t* event);

        virtual void HandleWirelessFwEvent(void* payload);
        virtual void HandleFusFwEvent(void* payload);
        virtual void HandleUnknwownReadyEvent(void* payload);

    private:
        void ShciInit();
        void HciInit();
        void MemoryChannelInit();

    private:
        services::BondBlobPersistence bondBlobPersistence;
        infra::Function<void(uint32_t*)> protocolStackInitialized;

        services::Tracer& tracer;
    };
}

#endif
