#include "hal_st/middlewares/ble_middleware/SystemTransportLayerWb.hpp"
#include "hci_tl.h"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "interface/patterns/ble_thread/tl/tl.h"
#include "shci.h"
#include "shci_tl.h"
#include "stm32wbxx_ll_system.h"
#include <atomic>

extern "C"
{
    SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification(void* packet)
    {
        assert(packet != nullptr);

        auto& event = *reinterpret_cast<hci_event_pckt*>(static_cast<hci_uart_pckt*>(packet)->data);
        hal::SystemTransportLayerWb::Instance().HciEventHandler(event);

        return SVCCTL_UserEvtFlowEnable;
    }

    void hci_notify_asynch_evt(void* data)
    {
        static std::atomic_bool notificationScheduled{ false };

        if (!notificationScheduled.exchange(true))
            infra::EventDispatcher::Instance().Schedule([]()
                {
                    notificationScheduled = false;
                    hci_user_evt_proc();
                });
    }

    void shci_notify_asynch_evt(void* data)
    {
        static std::atomic_bool notificationScheduled{ false };

        if (!notificationScheduled.exchange(true))
            infra::EventDispatcher::Instance().Schedule([]()
                {
                    notificationScheduled = false;
                    shci_user_evt_proc();
                });
    }
}

namespace
{
    const uint32_t bleBondsStorageLength = 507;
    const uint8_t bleEventQueueLength = 0x05;
    const uint8_t tlBleMaxEventPayloadSize = 0xFF;
    const uint16_t bleEventFrameSize = TL_EVT_HDR_SIZE + tlBleMaxEventPayloadSize;
    const uint32_t poolSize = bleEventQueueLength * 4u * DIVC((sizeof(TL_PacketHeader_t) + bleEventFrameSize), 4u);

    // Buffers to be used by the mailbox to report the events
    [[gnu::section("MB_MEM2")]] alignas(4) uint8_t evtPool[poolSize];
    [[gnu::section("MB_MEM2")]] alignas(4) uint8_t systemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
    [[gnu::section("MB_MEM2")]] alignas(4) uint8_t bleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
    [[gnu::section("MB_MEM2")]] uint32_t bleBondsStorage[bleBondsStorageLength];

    // Buffer to be used by the mailbox driver to send a BLE command
    [[gnu::section("MB_MEM2")]] alignas(4) TL_CmdPacket_t systemCmdBuffer;
    [[gnu::section("MB_MEM1")]] alignas(4) TL_CmdPacket_t bleCmdBuffer;

    // This is the registered callback in shci_init() to acknowledge if a system command can be
    // sent. It must be used in a multi-thread application where system commands may be sent
    // from different threads
    void ShciCommandStatus(SHCI_TL_CmdStatus_t status)
    {}

    // To be used in a multi-thread application where the BLE commands may be sent from different threads
    void HciCommandStatus(HCI_TL_CmdStatus_t status)
    {}

    void ShciUserEventHandler(void* payload)
    {
        hal::SystemTransportLayerWb::Instance().UserEventHandler(payload);
    }

    void HciUserEventHandler(void* payload)
    {
        auto pParam = static_cast<tHCI_UserEvtRxParam*>(payload);
        auto svctlReturnStatus = SVCCTL_UserEvtRx(static_cast<void*>(&(pParam->pckt->evtserial)));

        if (svctlReturnStatus != SVCCTL_UserEvtFlowDisable)
            pParam->status = HCI_TL_UserEventFlow_Enable;
        else
            pParam->status = HCI_TL_UserEventFlow_Disable;
    }

    uint8_t ToLowSpeedClock(hal::SystemTransportLayerWb::RfWakeupClock rfWakeupClock)
    {
        if (rfWakeupClock == hal::SystemTransportLayerWb::RfWakeupClock::highSpeedExternal)
            return SHCI_C2_BLE_INIT_CFG_BLE_LS_CLK_HSE_1024;
        else
            return SHCI_C2_BLE_INIT_CFG_BLE_LS_CLK_LSE;
    }

    void ShciCore2Init(const hal::SystemTransportLayerWb::Configuration& configuration)
    {
        const uint8_t maxNumberOfBleLinks = 0x01;
        const uint8_t prepareWriteListSize = BLE_PREP_WRITE_X_ATT(configuration.maxAttMtuSize);
        const uint8_t numberOfBleMemoryBlocks = BLE_MBLOCKS_CALC(prepareWriteListSize, configuration.maxAttMtuSize, maxNumberOfBleLinks);
        const uint8_t bleStackOptions = (SHCI_C2_BLE_INIT_OPTIONS_LL_HOST | SHCI_C2_BLE_INIT_OPTIONS_WITH_SVC_CHANGE_DESC | SHCI_C2_BLE_INIT_OPTIONS_DEVICE_NAME_RO | SHCI_C2_BLE_INIT_OPTIONS_NO_EXT_ADV | SHCI_C2_BLE_INIT_OPTIONS_NO_CS_ALGO2 |
                                         SHCI_C2_BLE_INIT_OPTIONS_FULL_GATTDB_NVM | SHCI_C2_BLE_INIT_OPTIONS_GATT_CACHING_NOTUSED | SHCI_C2_BLE_INIT_OPTIONS_POWER_CLASS_2_3 | SHCI_C2_BLE_INIT_OPTIONS_APPEARANCE_READONLY | SHCI_C2_BLE_INIT_OPTIONS_ENHANCED_ATT_NOTSUPPORTED);

        SHCI_C2_CONFIG_Cmd_Param_t configParam = {
            SHCI_C2_CONFIG_PAYLOAD_CMD_SIZE,
            SHCI_C2_CONFIG_CONFIG1_BIT0_BLE_NVM_DATA_TO_SRAM,
            SHCI_C2_CONFIG_EVTMASK1_BIT1_BLE_NVM_RAM_UPDATE_ENABLE,
            0, // Spare
            reinterpret_cast<uint32_t>(bleBondsStorage),
            0, // ThreadNvmRamAddress
            static_cast<uint16_t>(LL_DBGMCU_GetRevisionID()),
            static_cast<uint16_t>(LL_DBGMCU_GetDeviceID()),
        };

        if (SHCI_C2_Config(&configParam) != SHCI_Success)
            std::abort();

        SHCI_C2_Ble_Init_Cmd_Packet_t bleInitCmdPacket = {
            { { 0, 0, 0 } }, // Header (unused)
            {
                0x00,  // BLE buffer address (unused)
                0x00,  // BLE buffer size (unused)
                0x44,  // Maximum number of GATT Attributes
                0x08,  // Maximum number of Services that can be stored in the GATT database
                0x540, // Size of the storage area for Attribute values
                maxNumberOfBleLinks,
                0x01, // Enable or disable the Extended Packet length feature
                prepareWriteListSize,
                numberOfBleMemoryBlocks,
                configuration.maxAttMtuSize,
                0x1FA,                                        // Sleep clock accuracy in Slave mode
                0x00,                                         // Sleep clock accuracy in Master mode
                ToLowSpeedClock(configuration.rfWakeupClock), // Source for the low speed clock for RF wake-up
                0xFFFFFFFF,                                   // Maximum duration of the connection event when the device is in Slave mode in units of 625/256 us (~2.44 us)
                0x148,                                        // Start up time of the high speed (16 or 32 MHz) crystal oscillator in units of 625/256 us (~2.44 us)
                0x01,                                         // Viterbi Mode
                bleStackOptions,
                0,   // HW version (unused)
                32,  // Maximum number of connection-oriented channels in initiator mode
                -40, // Minimum transmit power in dBm supported by the Controller
                6,   // Maximum transmit power in dBm supported by the Controller
                SHCI_C2_BLE_INIT_RX_MODEL_AGC_RSSI_LEGACY,
                3,    // Maximum number of advertising sets.
                1650, // Maximum advertising data length (in bytes)
                0,    // RF TX Path Compensation Value (16-bit signed integer). Units: 0.1 dB.
                0,    // RF RX Path Compensation Value (16-bit signed integer). Units: 0.1 dB.
                SHCI_C2_BLE_INIT_BLE_CORE_5_3 }
        };

        if (SHCI_C2_BLE_Init(&bleInitCmdPacket) != SHCI_Success)
            std::abort();
    }
}

namespace hal
{
    SystemTransportLayerWb::SystemTransportLayerWb(services::ConfigurationStoreAccess<infra::ByteRange> flashStorage, BondStorageSynchronizerCreator& bondStorageSynchronizerCreator, Configuration configuration, const infra::Function<void(services::BondStorageSynchronizer&)>& onInitialized)
        : bondBlobPersistence(flashStorage, infra::MakeByteRange(bleBondsStorage))
        , bondStorageSynchronizerCreator(bondStorageSynchronizerCreator)
        , configuration(configuration)
        , onInitialized(onInitialized)
    {
        really_assert(configuration.maxAttMtuSize >= BLE_DEFAULT_ATT_MTU && configuration.maxAttMtuSize <= 251);
        // BLE middleware supported maxAttMtuSize = 512. Current usage of library limits maxAttMtuSize to 251 (max HCI buffer size)

        TL_Init();
        ShciInit();
        HciInit();
        MemoryChannelInit();
        TL_Enable();
    }

    void SystemTransportLayerWb::HciEventHandler(hci_event_pckt& event)
    {
        infra::Subject<HciEventSink>::NotifyObservers([&event](auto& observer)
            {
                observer.HciEvent(event);
            });
    }

    SystemTransportLayerWb::Version SystemTransportLayerWb::GetVersion() const
    {
        WirelessFwInfo_t wirelessInfo;
        SHCI_GetWirelessFwInfo(&wirelessInfo);

        return {
            wirelessInfo.VersionMajor,
            wirelessInfo.VersionMinor,
            wirelessInfo.VersionSub,
            wirelessInfo.VersionBranch,
            wirelessInfo.VersionReleaseType,
            wirelessInfo.FusVersionMajor,
            wirelessInfo.FusVersionMinor,
            wirelessInfo.FusVersionSub,
        };
    }

    void SystemTransportLayerWb::HandleErrorNotifyEvent(void* SysEvent)
    {}

    void SystemTransportLayerWb::HandleBleNvmRamUpdateEvent(void* sysEvent)
    {
        bondBlobPersistence.Update();
    }

    void SystemTransportLayerWb::HandleUnknownEvent(void* SysEvent)
    {}

    void SystemTransportLayerWb::UserEventHandler(void* payload)
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
            case SHCI_SUB_EVT_BLE_NVM_RAM_UPDATE:
                HandleBleNvmRamUpdateEvent(event);
                break;
            default:
                HandleUnknownEvent(event);
                break;
        }
    }

    void SystemTransportLayerWb::HandleWirelessFwEvent(void*)
    {
        ShciCore2Init(configuration);
        bondStorageSynchronizerCreator.Emplace();

        if (onInitialized)
            onInitialized(*bondStorageSynchronizerCreator);
    }

    void SystemTransportLayerWb::HandleFusFwEvent(void* payload)
    {
        ((tSHCI_UserEvtRxParam*)payload)->status = SHCI_TL_UserEventFlow_Disable;
    }

    void SystemTransportLayerWb::HandleUnknwownReadyEvent(void* payload)
    {}

    void SystemTransportLayerWb::HandleReadyEvent(void* payload)
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

    void SystemTransportLayerWb::ShciInit()
    {
        SHCI_TL_HciInitConf_t config;
        config.p_cmdbuffer = reinterpret_cast<uint8_t*>(&systemCmdBuffer);
        config.StatusNotCallBack = ShciCommandStatus;
        shci_init(ShciUserEventHandler, static_cast<void*>(&config));
    }

    void SystemTransportLayerWb::HciInit()
    {
        HCI_TL_HciInitConf_t config;
        config.p_cmdbuffer = reinterpret_cast<uint8_t*>(&bleCmdBuffer);
        config.StatusNotCallBack = HciCommandStatus;
        hci_init(HciUserEventHandler, static_cast<void*>(&config));
    }

    void SystemTransportLayerWb::MemoryChannelInit()
    {
        TL_MM_Config_t config;
        config.p_BleSpareEvtBuffer = bleSpareEvtBuffer;
        config.p_SystemSpareEvtBuffer = systemSpareEvtBuffer;
        config.p_AsynchEvtPool = evtPool;
        config.AsynchEvtPoolSize = poolSize;
        TL_MM_Init(&config);
    }
}
