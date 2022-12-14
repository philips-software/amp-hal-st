#include "hal_st/middlewares/ble_middleware/TracingSystemTransportLayer.hpp"
#include "shci.h"
#include "shci_tl.h"
#include "hci_tl.h"
#include "ble/svc/Inc/svc_ctl.h"

extern "C"
{
    #include "app_conf.h"
    #include "ble.h"
}

namespace hal
{
    TracingSystemTransportLayer::TracingSystemTransportLayer(const infra::Function<void()>& protoStackInit, services::Tracer& tracer)
        : SystemTransportLayer(protoStackInit)
        , tracer(tracer)
    {}

    void TracingSystemTransportLayer::UserEventHandler(void* pPayload)
    {
        WirelessFwInfo_t wirelessInfo;

        SHCI_GetWirelessFwInfo(&wirelessInfo);
        tracer.Trace() << "Wireless Firmware version: " << wirelessInfo.VersionMajor << "." << wirelessInfo.VersionMinor << "." << wirelessInfo.VersionSub;
        tracer.Trace() << "Wireless Firmware build: " << wirelessInfo.VersionReleaseType;
        tracer.Trace() << "FUS version: " << wirelessInfo.FusVersionMajor << "." << wirelessInfo.FusVersionMinor << "." << wirelessInfo.FusVersionSub;
        SystemTransportLayer::UserEventHandler(pPayload);
    }

    void TracingSystemTransportLayer::HandleReadyEvent(void* pPayload)
    {
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: SHCI_SUB_EVT_CODE_READY";
        SystemTransportLayer::HandleReadyEvent(pPayload);
    }

    void TracingSystemTransportLayer::HandleErrorNotifyEvent(TL_AsynchEvt_t* sysEvent)
    {
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: SHCI_SUB_EVT_ERROR_NOTIF";
        SystemTransportLayer::HandleErrorNotifyEvent(sysEvent);
    }

    void TracingSystemTransportLayer::HandleUnknownEvent(TL_AsynchEvt_t* sysEvent)
    {
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: Unsupported event found (" << sysEvent->subevtcode << ")";
        SystemTransportLayer::HandleUnknownEvent(sysEvent);
    }

    void TracingSystemTransportLayer::HandleWirelessFwEvent(void* pPayload)
    {
        tracer.Trace() << "SystemTransportLayer::HandleReadyEvent: WIRELESS_FW_RUNNING";
        SystemTransportLayer::HandleWirelessFwEvent(pPayload);
    }

    void TracingSystemTransportLayer::HandleFusFwEvent(void* pPayload)
    {
        tracer.Trace() << "SystemTransportLayer::HandleReadyEvent: FUS_FW_RUNNING";
        SystemTransportLayer::HandleFusFwEvent(pPayload);
    }

    void TracingSystemTransportLayer::HandleUnknwownReadyEvent(void* pPayload)
    {
        auto pSysEvent = reinterpret_cast<TL_AsynchEvt_t*>((static_cast<tSHCI_UserEvtRxParam*>(pPayload)->pckt)->evtserial.evt.payload);
        auto pSysReadyEvent = reinterpret_cast<SHCI_C2_Ready_Evt_t*>(pSysEvent->payload);
        tracer.Trace() << "SystemTransportLayer::HandleReadyEvent: Unsupported event found (" << pSysReadyEvent->sysevt_ready_rsp << ")";
        SystemTransportLayer::HandleUnknwownReadyEvent(pPayload);
    }
}
