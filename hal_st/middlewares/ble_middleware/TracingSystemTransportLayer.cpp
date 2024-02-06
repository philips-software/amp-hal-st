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
    TracingSystemTransportLayer::TracingSystemTransportLayer(services::ConfigurationStoreAccess<infra::ByteRange> flashStorage, const infra::Function<void(uint32_t*)>& protocolStackInitialized, services::Tracer& tracer)
        : SystemTransportLayer(flashStorage, protocolStackInitialized, tracer)
        , tracer(tracer)
    {}

    void TracingSystemTransportLayer::UserEventHandler(void* pPayload)
    {
        SystemTransportLayer::UserEventHandler(pPayload);
    }

    void TracingSystemTransportLayer::HandleReadyEvent(void* pPayload)
    {
        const auto transportLayerVersion = GetVersion();

        tracer.Trace() << "Wireless Firmware version: " << transportLayerVersion.firmwareMajor << "." << transportLayerVersion.firmwareMinor << "." << transportLayerVersion.firmwareSub;
        tracer.Trace() << "Wireless Firmware build: " << transportLayerVersion.firmwareReleaseType;
        tracer.Trace() << "FUS version: " << transportLayerVersion.fusMajor << "." << transportLayerVersion.fusMinor << "." << transportLayerVersion.fusSub;
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: SHCI_SUB_EVT_CODE_READY";

        SystemTransportLayer::HandleReadyEvent(pPayload);
    }

    void TracingSystemTransportLayer::HandleErrorNotifyEvent(TL_AsynchEvt_t* sysEvent)
    {
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: SHCI_SUB_EVT_ERROR_NOTIF";
        SystemTransportLayer::HandleErrorNotifyEvent(sysEvent);
    }

    void TracingSystemTransportLayer::HandleBleNvmRamUpdateEvent(TL_AsynchEvt_t* sysEvent)
    {
        auto& bleNvmRamUpdateEvent = *reinterpret_cast<SHCI_C2_BleNvmRamUpdate_Evt_t*>(sysEvent->payload);
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: SHCI_SUB_EVT_BLE_NVM_RAM_UPDATE : size: " << bleNvmRamUpdateEvent.Size  << ", address: 0x" << infra::hex << bleNvmRamUpdateEvent.StartAddress;
        SystemTransportLayer::HandleBleNvmRamUpdateEvent(sysEvent);
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
