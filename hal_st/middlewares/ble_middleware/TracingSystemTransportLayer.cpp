#include "hal_st/middlewares/ble_middleware/TracingSystemTransportLayer.hpp"
#if defined(STM32WB)
#include "shci.h"
#include "shci_tl.h"
#endif

namespace hal
{
    TracingSystemTransportLayer::TracingSystemTransportLayer(services::ConfigurationStoreAccess<infra::ByteRange> flashStorage, const infra::Function<void(uint32_t*)>& protocolStackInitialized, services::Tracer& tracer)
        : SystemTransportLayer(flashStorage, protocolStackInitialized)
        , tracer(tracer)
    {}

#if defined(STM32WB)
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

    void TracingSystemTransportLayer::HandleErrorNotifyEvent(void* event)
    {
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: SHCI_SUB_EVT_ERROR_NOTIF";
        SystemTransportLayer::HandleErrorNotifyEvent(event);
    }

    void TracingSystemTransportLayer::HandleBleNvmRamUpdateEvent(void* event)
    {
        auto& sysEvent = *reinterpret_cast<TL_AsynchEvt_t*>(event);
        const auto& bleNvmRamUpdateEvent = *reinterpret_cast<SHCI_C2_BleNvmRamUpdate_Evt_t*>(sysEvent.payload);
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: SHCI_SUB_EVT_BLE_NVM_RAM_UPDATE : size: " << bleNvmRamUpdateEvent.Size << ", address: 0x" << infra::hex << bleNvmRamUpdateEvent.StartAddress;
        SystemTransportLayer::HandleBleNvmRamUpdateEvent(event);
    }

    void TracingSystemTransportLayer::HandleUnknownEvent(void* event)
    {
        auto& sysEvent = *reinterpret_cast<TL_AsynchEvt_t*>(event);
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: Unsupported event found (" << sysEvent.subevtcode << ")";
        SystemTransportLayer::HandleUnknownEvent(event);
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
#endif
}
