#include "hal_st/middlewares/ble_middleware/TracingSystemTransportLayerWb.hpp"
#include "shci.h"
#include "shci_tl.h"

namespace hal
{
    TracingSystemTransportLayerWb::TracingSystemTransportLayerWb(services::ConfigurationStoreAccess<infra::ByteRange> flashStorage, BondStorageSynchronizerCreator& bondStorageSynchronizerCreator, Configuration configuration, const infra::Function<void(services::BondStorageSynchronizer&)>& onInitialized, services::Tracer& tracer)
        : SystemTransportLayerWb(flashStorage, bondStorageSynchronizerCreator, configuration, onInitialized)
        , tracer(tracer)
    {}

    void TracingSystemTransportLayerWb::UserEventHandler(void* pPayload)
    {
        SystemTransportLayerWb::UserEventHandler(pPayload);
    }

    void TracingSystemTransportLayerWb::HandleReadyEvent(void* pPayload)
    {
        const auto transportLayerVersion = GetVersion();

        tracer.Trace() << "Wireless Firmware version: " << transportLayerVersion.firmwareMajor << "." << transportLayerVersion.firmwareMinor << "." << transportLayerVersion.firmwareSub;
        tracer.Trace() << "Wireless Firmware build: " << transportLayerVersion.firmwareReleaseType;
        tracer.Trace() << "Wireless Firmware branch: " << transportLayerVersion.firmwareBranch;
        tracer.Trace() << "FUS version: " << transportLayerVersion.fusMajor << "." << transportLayerVersion.fusMinor << "." << transportLayerVersion.fusSub;
        tracer.Trace() << "SystemTransportLayerWb::UserEventHandler: SHCI_SUB_EVT_CODE_READY";

        SystemTransportLayerWb::HandleReadyEvent(pPayload);
    }

    void TracingSystemTransportLayerWb::HandleErrorNotifyEvent(void* sysEvent)
    {
        tracer.Trace() << "SystemTransportLayerWb::UserEventHandler: SHCI_SUB_EVT_ERROR_NOTIF";
        SystemTransportLayerWb::HandleErrorNotifyEvent(sysEvent);
    }

    void TracingSystemTransportLayerWb::HandleBleNvmRamUpdateEvent(void* sysEvent)
    {
        auto& bleNvmRamUpdateEvent = *reinterpret_cast<SHCI_C2_BleNvmRamUpdate_Evt_t*>(reinterpret_cast<TL_AsynchEvt_t*>(sysEvent)->payload);
        tracer.Trace() << "SystemTransportLayerWb::UserEventHandler: SHCI_SUB_EVT_BLE_NVM_RAM_UPDATE : size: " << bleNvmRamUpdateEvent.Size << ", address: 0x" << infra::hex << bleNvmRamUpdateEvent.StartAddress;
        SystemTransportLayerWb::HandleBleNvmRamUpdateEvent(sysEvent);
    }

    void TracingSystemTransportLayerWb::HandleUnknownEvent(void* sysEvent)
    {
        auto subEventCode = reinterpret_cast<TL_AsynchEvt_t*>(sysEvent)->subevtcode;
        tracer.Trace() << "SystemTransportLayerWb::UserEventHandler: Unsupported event found (" << subEventCode << ")";
        SystemTransportLayerWb::HandleUnknownEvent(sysEvent);
    }

    void TracingSystemTransportLayerWb::HandleWirelessFwEvent(void* pPayload)
    {
        tracer.Trace() << "SystemTransportLayerWb::HandleReadyEvent: WIRELESS_FW_RUNNING";
        SystemTransportLayerWb::HandleWirelessFwEvent(pPayload);
    }

    void TracingSystemTransportLayerWb::HandleFusFwEvent(void* pPayload)
    {
        tracer.Trace() << "SystemTransportLayerWb::HandleReadyEvent: FUS_FW_RUNNING";
        SystemTransportLayerWb::HandleFusFwEvent(pPayload);
    }

    void TracingSystemTransportLayerWb::HandleUnknwownReadyEvent(void* pPayload)
    {
        auto pSysEvent = reinterpret_cast<TL_AsynchEvt_t*>((static_cast<tSHCI_UserEvtRxParam*>(pPayload)->pckt)->evtserial.evt.payload);
        auto pSysReadyEvent = reinterpret_cast<SHCI_C2_Ready_Evt_t*>(pSysEvent->payload);
        tracer.Trace() << "SystemTransportLayerWb::HandleReadyEvent: Unsupported event found (" << pSysReadyEvent->sysevt_ready_rsp << ")";
        SystemTransportLayerWb::HandleUnknwownReadyEvent(pPayload);
    }
}
