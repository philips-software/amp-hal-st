#ifndef HAL_ST_HCI_EVENT_OBSERVER_HPP
#define HAL_ST_HCI_EVENT_OBSERVER_HPP

#include "ble/ble.h"
#include "infra/util/Observer.hpp"

namespace hal
{
    class HciEventSource;

    class HciEventSink
        : public infra::Observer<HciEventSink, HciEventSource>
    {
    public:
        using infra::Observer<HciEventSink, HciEventSource>::Observer;

        virtual void HciEvent(hci_event_pckt& event) = 0;
    };

    class HciEventSource
        : public infra::Subject<HciEventSink>
    {
        virtual void HciEventHandler(hci_event_pckt& event) = 0;
    };
}

#endif
