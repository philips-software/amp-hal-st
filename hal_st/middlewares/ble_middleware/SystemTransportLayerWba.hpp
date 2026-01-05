#ifndef HAL_ST_SYSTEM_TRANSPORT_LAYER_WBA_HPP
#define HAL_ST_SYSTEM_TRANSPORT_LAYER_WBA_HPP

#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/util/InterfaceConnector.hpp"

namespace hal
{
    class SystemTransportLayerWba
        : public infra::InterfaceConnector<SystemTransportLayerWba>
        , public HciEventSource
    {
    public:
        explicit SystemTransportLayerWba(uint16_t attMtuSize);

        // Implementation of HciEventSource
        void HciEventHandler(hci_event_pckt& event) override;
    };
}

#endif
