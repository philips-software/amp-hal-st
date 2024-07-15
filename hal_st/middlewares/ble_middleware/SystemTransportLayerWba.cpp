#include "hal_st/middlewares/ble_middleware/SystemTransportLayerWba.hpp"
#include <array>
extern "C"
{
#include "ble_common.h"
#include "ble_bufsize.h"
#include "blestack.h"
}

extern "C"
{
    SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification(void* packet)
    {
        assert(packet != nullptr);

        auto& event = *reinterpret_cast<hci_event_pckt*>(static_cast<hci_uart_pckt*>(packet)->data);
        hal::SystemTransportLayerWba::Instance().HciEventHandler(event);

        return SVCCTL_UserEvtFlowEnable;
    }

    tBleStatus ProcessEventPacket(const uint8_t* data)
    {
        SVCCTL_UserEvtRx(const_cast<uint8_t *>(data));
        return BLE_STATUS_SUCCESS;
    }

    tBleStatus BLECB_Indication(const uint8_t* data, uint16_t, const uint8_t*, uint16_t)
    {
        if (data[0] == HCI_EVENT_PKT_TYPE)
            return ProcessEventPacket(data);
        else if (data[0] == HCI_ACLDATA_PKT_TYPE)
            return BLE_STATUS_SUCCESS;

        return BLE_STATUS_FAILED;
    }
}

namespace
{
    const uint8_t maxNumberOfBleLinks = 0x01;
    const uint8_t numAttrRecord = 0x44;
    const uint8_t numAttrServ = 0x08;
    const uint16_t attrValueArrSize = 0x540;
    const uint8_t maxNumberOfConnectionOrientedChannels = 32;
    const std::size_t mblockCount = (BLE_MBLOCKS_CALC(BLE_DEFAULT_PREP_WRITE_LIST_SIZE, 251, maxNumberOfBleLinks) + 0x15);
    const std::size_t bufferSize = BLE_TOTAL_BUFFER_SIZE(maxNumberOfBleLinks, mblockCount);
    const std::size_t gattBufferSize = BLE_TOTAL_BUFFER_SIZE_GATT(numAttrRecord, numAttrServ, attrValueArrSize);
    std::array<uint32_t, DIVC(bufferSize, 4)> bleBuffer;
    std::array<uint32_t, DIVC(gattBufferSize, 4)> bleGattBuffer;
    const uint8_t bleStackOptions = 0;

    constexpr uint8_t PrepareWriteListSize(uint16_t maxAttMtuSize)
    {
        return static_cast<uint8_t>(BLE_PREP_WRITE_X_ATT(maxAttMtuSize));
    }
}

namespace hal
{
    SystemTransportLayerWba::SystemTransportLayerWba(uint16_t maxAttMtuSize)
    {
        really_assert(maxAttMtuSize >= BLE_DEFAULT_ATT_MTU && maxAttMtuSize <= 251);
        // BLE middleware supported maxAttMtuSize = 512. Current usage of library limits maxAttMtuSize to 251 (max HCI buffer size)

        BleStack_init_t bleStackInitParameters = {
            reinterpret_cast<uint8_t*>(bleBuffer.begin()),
            bufferSize,
            reinterpret_cast<uint8_t*>(bleGattBuffer.begin()),
            gattBufferSize,
            numAttrRecord,
            numAttrServ,
            attrValueArrSize,
            maxNumberOfBleLinks,
            PrepareWriteListSize(maxAttMtuSize),
            mblockCount,
            maxAttMtuSize,
            248,
            64,
            maxNumberOfConnectionOrientedChannels,
            bleStackOptions,
            0U
        };

        really_assert(BleStack_Init(&bleStackInitParameters) == BLE_STATUS_SUCCESS);
    }

    void SystemTransportLayerWba::HciEventHandler(hci_event_pckt& event)
    {
        infra::Subject<HciEventSink>::NotifyObservers([&event](auto& observer)
            {
                observer.HciEvent(event);
            });
    }
}
