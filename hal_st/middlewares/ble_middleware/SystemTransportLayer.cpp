#include "hal_st/middlewares/ble_middleware/SystemTransportLayer.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "shci.h"
#include "shci_tl.h"
#include "hci_tl.h"

extern "C"
{
    SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification(void* packet)
    {
        assert(packet != nullptr);

        auto& event = *reinterpret_cast<hci_event_pckt*>(static_cast<hci_uart_pckt*>(packet)->data);
        hal::SystemTransportLayer::Instance().HciEventHandler(event);

        return SVCCTL_UserEvtFlowEnable;
    }

    void hci_notify_asynch_evt(void* data)
    {
        infra::EventDispatcher::Instance().Schedule([]() { hci_user_evt_proc(); });
    }

    void shci_notify_asynch_evt(void* data)
    {
        infra::EventDispatcher::Instance().Schedule([]() { shci_user_evt_proc(); });
    }
}

namespace
{
    const uint8_t bleEventQueueLength = 0x05;
    const uint8_t tlBleMaxEventPayloadSize = 0xFF;
    const uint16_t bleEventFrameSize = TL_EVT_HDR_SIZE + tlBleMaxEventPayloadSize;
    const uint32_t poolSize = bleEventQueueLength * 4u * DIVC(( sizeof(TL_PacketHeader_t) + bleEventFrameSize ), 4u);

    // Buffers to be used by the mailbox to report the events
    [[gnu::section("MB_MEM2")]] alignas(4) static uint8_t evtPool[poolSize];
    [[gnu::section("MB_MEM2")]] alignas(4) static uint8_t systemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
    [[gnu::section("MB_MEM2")]] alignas(4) static uint8_t bleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];

    // Buffer to be used by the mailbox driver to send a BLE command
    [[gnu::section("MB_MEM2")]] alignas(4) static TL_CmdPacket_t systemCmdBuffer;
    [[gnu::section("MB_MEM1")]] alignas(4) static TL_CmdPacket_t bleCmdBuffer;

    // This is the registered callback in shci_init() to acknowledge if a system command can be 
    // sent. It must be used in a multi-thread application where system commands may be sent 
    // from different threads
    static void ShciCommandStatus(SHCI_TL_CmdStatus_t status)
    {}

    // To be used in a multi-thread application where the BLE commands may be sent from different threads
    static void HciCommandStatus(HCI_TL_CmdStatus_t status)
    {}

    static void ShciUserEventHandler(void* payload)
    {
        hal::SystemTransportLayer::Instance().UserEventHandler(payload);
    }

    static void HciUserEventHandler(void* payload)
    {
        auto pParam = static_cast<tHCI_UserEvtRxParam*>(payload);
        auto svctlReturnStatus = SVCCTL_UserEvtRx(static_cast<void*>(&(pParam->pckt->evtserial)));

        if (svctlReturnStatus != SVCCTL_UserEvtFlowDisable)
            pParam->status = HCI_TL_UserEventFlow_Enable;
        else
            pParam->status = HCI_TL_UserEventFlow_Disable;
    }
}

namespace hal
{
    SystemTransportLayer::SystemTransportLayer(const infra::Function<void()>& protocolStackInitialized)
        : protocolStackInitialized(protocolStackInitialized)
    {
        TL_Init();
        ShciInit();
        HciInit();
        MemoryChannelInit();
        TL_Enable();
    }

    void SystemTransportLayer::HandleErrorNotifyEvent(TL_AsynchEvt_t* SysEvent)
    {}

    void SystemTransportLayer::HandleUnknownEvent(TL_AsynchEvt_t* SysEvent)
    {}

    void SystemTransportLayer::UserEventHandler(void* payload)
    {
        auto event = reinterpret_cast<TL_AsynchEvt_t*>((static_cast<tSHCI_UserEvtRxParam*>(payload)->pckt)->evtserial.evt.payload);

        switch (event->subevtcode)
        {
        case SHCI_SUB_EVT_CODE_READY:
            HandleReadyEvent(payload);
            break;
        case SHCI_SUB_EVT_ERROR_NOTIF:
            HandleErrorNotifyEvent(event);
            break;
        default:
            HandleUnknownEvent(event);
            break;
        }
    }

    void SystemTransportLayer::HciEventHandler(hci_event_pckt& event)
    {
        infra::Subject<HciEventSink>::NotifyObservers([&event](auto& observer) { observer.HciEvent(event); });
    }

    void SystemTransportLayer::HandleWirelessFwEvent(void* payload)
    {
        protocolStackInitialized();
    }

    void SystemTransportLayer::HandleFusFwEvent(void* payload)
    {
        ((tSHCI_UserEvtRxParam*)payload)->status = SHCI_TL_UserEventFlow_Disable;
    }

    void SystemTransportLayer::HandleUnknwownReadyEvent(void* payload)
    {}

    void SystemTransportLayer::HandleReadyEvent(void* payload)
    {
        auto event = reinterpret_cast<TL_AsynchEvt_t*>((static_cast<tSHCI_UserEvtRxParam*>(payload)->pckt)->evtserial.evt.payload);
        auto readyEvent = reinterpret_cast<SHCI_C2_Ready_Evt_t*>(event->payload);

        switch (readyEvent->sysevt_ready_rsp)
        {
        case WIRELESS_FW_RUNNING:
            HandleWirelessFwEvent(payload);
            break;
        case FUS_FW_RUNNING:
            HandleFusFwEvent(payload);
            break;
        default:
            HandleUnknwownReadyEvent(payload);
            break;
        }
    }

    void SystemTransportLayer::ShciInit()
    {
        SHCI_TL_HciInitConf_t config;
        config.p_cmdbuffer = reinterpret_cast<uint8_t*>(&systemCmdBuffer);
        config.StatusNotCallBack = ShciCommandStatus;
        shci_init(ShciUserEventHandler, static_cast<void*>(&config));
    }

    void SystemTransportLayer::HciInit()
    {
        HCI_TL_HciInitConf_t config;
        config.p_cmdbuffer = reinterpret_cast<uint8_t*>(&bleCmdBuffer);
        config.StatusNotCallBack = HciCommandStatus;
        hci_init(HciUserEventHandler, static_cast<void*>(&config));
    }

    void SystemTransportLayer::MemoryChannelInit()
    {
        TL_MM_Config_t config;
        config.p_BleSpareEvtBuffer = bleSpareEvtBuffer;
        config.p_SystemSpareEvtBuffer = systemSpareEvtBuffer;
        config.p_AsynchEvtPool = evtPool;
        config.AsynchEvtPoolSize = poolSize;
        TL_MM_Init(&config);
    }
}
