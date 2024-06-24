#ifndef HAL_ST_SYSTEM_TRANSPORT_LAYER_HPP
#define HAL_ST_SYSTEM_TRANSPORT_LAYER_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "services/ble/BondBlobPersistence.hpp"

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
        SystemTransportLayer(services::ConfigurationStoreAccess<infra::ByteRange> flashStorage, const infra::Function<void(uint32_t*)>& protocolStackInitialized);

        Version GetVersion() const;

        // Implementation of HciEventSource
        void HciEventHandler(hci_event_pckt& event) override;

        virtual void UserEventHandler(void* payload);

    protected:
        virtual void HandleReadyEvent(void* payload);
        virtual void HandleErrorNotifyEvent(void* event);
        virtual void HandleBleNvmRamUpdateEvent(void* sysEvent);
        virtual void HandleUnknownEvent(void* event);

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
    };
}

#endif
