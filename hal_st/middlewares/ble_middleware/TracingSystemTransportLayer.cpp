#include "hal_st/middlewares/ble_middleware/TracingSystemTransportLayer.hpp"
#include "shci.h"
#include "shci_tl.h"

namespace hal
{
    TracingSystemTransportLayer::TracingSystemTransportLayer(services::ConfigurationStoreAccess<infra::ByteRange> flashStorage, BondStorageSynchronizerCreator& bondStorageSynchronizerCreator, Configuration configuration, const infra::Function<void(services::BondStorageSynchronizer&)>& onInitialized, services::Tracer& tracer)
        : SystemTransportLayer(flashStorage, bondStorageSynchronizerCreator, configuration, onInitialized)
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

    void TracingSystemTransportLayer::HandleErrorNotifyEvent(void* sysEvent)
    {
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: SHCI_SUB_EVT_ERROR_NOTIF";
        SystemTransportLayer::HandleErrorNotifyEvent(sysEvent);
    }

    void TracingSystemTransportLayer::HandleBleNvmRamUpdateEvent(void* sysEvent)
    {
        auto& bleNvmRamUpdateEvent = *reinterpret_cast<SHCI_C2_BleNvmRamUpdate_Evt_t*>(reinterpret_cast<TL_AsynchEvt_t*>(sysEvent)->payload);
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: SHCI_SUB_EVT_BLE_NVM_RAM_UPDATE : size: " << bleNvmRamUpdateEvent.Size << ", address: 0x" << infra::hex << bleNvmRamUpdateEvent.StartAddress;
        SystemTransportLayer::HandleBleNvmRamUpdateEvent(sysEvent);
    }

    void TracingSystemTransportLayer::HandleUnknownEvent(void* sysEvent)
    {
        auto subEventCode = reinterpret_cast<TL_AsynchEvt_t*>(sysEvent)->subevtcode;
        tracer.Trace() << "SystemTransportLayer::UserEventHandler: Unsupported event found (" << subEventCode << ")";
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
