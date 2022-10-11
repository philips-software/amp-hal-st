#ifndef HAL_ST_SYSTEM_TRANSPORT_LAYER_HPP
#define HAL_ST_SYSTEM_TRANSPORT_LAYER_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "interface/patterns/ble_thread/tl/tl.h"

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
        SystemTransportLayer(const infra::Function<void()>& protocolStackInitialized);

        Version GetVersion() const;

        virtual void UserEventHandler(void* payload);
        virtual void HciEventHandler(hci_event_pckt& event);

    protected:
        virtual void HandleReadyEvent(void* payload);
        virtual void HandleErrorNotifyEvent(TL_AsynchEvt_t* event);
        virtual void HandleUnknownEvent(TL_AsynchEvt_t* event);

        virtual void HandleWirelessFwEvent(void* payload);
        virtual void HandleFusFwEvent(void* payload);
        virtual void HandleUnknwownReadyEvent(void* payload);

    private:
        void ShciInit();
        void HciInit();
        void MemoryChannelInit();

    private:
        infra::Function<void()> protocolStackInitialized;
    };
}

#endif
