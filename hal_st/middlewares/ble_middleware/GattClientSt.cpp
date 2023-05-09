#include "hal_st/middlewares/ble_middleware/GattClientSt.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "infra/stream/InputStream.hpp"
#include "infra/util/Endian.hpp"

extern "C"
{
#include "auto/ble_gatt_aci.h"
#include "ble/ble.h"
#include "ble/core/ble_core.h"
}

namespace hal
{
    GattClientSt::GattClientSt(hal::HciEventSource& hciEventSource)
        : hal::HciEventSink(hciEventSource)
        , connectionHandle(invalidConnection)
    {}

    void GattClientSt::StartServiceDiscovery()
    {
        onDiscoveryCompletion = [](services::GattClientDiscoveryObserver& observer) { observer.ServiceDiscoveryComplete(); };

        aci_gatt_disc_all_primary_services(connectionHandle);
    }

    void GattClientSt::StartCharacteristicDiscovery(const services::GattService& service)
    {
        onDiscoveryCompletion = [](services::GattClientDiscoveryObserver& observer) { observer.CharacteristicDiscoveryComplete(); };

        aci_gatt_disc_all_char_of_service(connectionHandle, service.Handle(), service.EndHandle());
    }

    void GattClientSt::StartDescriptorDiscovery(const services::GattService& service)
    {
        onDiscoveryCompletion = [](services::GattClientDiscoveryObserver& observer) { observer.DescriptorDiscoveryComplete(); };

        aci_gatt_disc_all_char_desc(connectionHandle, service.Handle(), service.EndHandle());
    }

    void GattClientSt::HciEvent(hci_event_pckt& event)
    {
        switch (event.evt)
        {
        case HCI_DISCONNECTION_COMPLETE_EVT_CODE:
            HandleHciDisconnectEvent(event);
            break;
        case HCI_LE_META_EVT_CODE:
            HandleHciLeMetaEvent(event);
            break;
        case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
            HandleHciVendorSpecificDebugEvent(event);
            break;
        default:
            break;
        }
    }

    void GattClientSt::HandleHciLeMetaEvent(hci_event_pckt& eventPacket)
    {
        auto metaEvent = reinterpret_cast<evt_le_meta_event*>(eventPacket.data);

        switch (metaEvent->subevent)
        {
        case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE:
            HandleHciLeEnhancedConnectionCompleteEvent(metaEvent);
            break;
        case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
            HandleHciLeConnectionCompleteEvent(metaEvent);
            break;
        default:
            break;
        }
    }

    void GattClientSt::HandleHciVendorSpecificDebugEvent(hci_event_pckt& eventPacket)
    {
        auto vendorEvent = reinterpret_cast<evt_blecore_aci*>(eventPacket.data);

        switch (vendorEvent->ecode)
        {
        case ACI_ATT_READ_BY_GROUP_TYPE_RESP_VSEVT_CODE:
            HandleAttReadByGroupTypeResponse(vendorEvent);
            break;
        case ACI_ATT_READ_BY_TYPE_RESP_VSEVT_CODE:
            HandleAttReadByTypeResponse(vendorEvent);
            break;
        case ACI_ATT_FIND_INFO_RESP_VSEVT_CODE:
            HandleAttFindInfoResponse(vendorEvent);
            break;
        case ACI_GATT_PROC_COMPLETE_VSEVT_CODE:
            HandleGattCompleteResponse(vendorEvent);
            break;
        default:
            break;
        }
    }

    void GattClientSt::HandleAttReadByGroupTypeResponse(evt_blecore_aci* vendorEvent)
    {
        const uint8_t uuid16 = 6;
        auto attReadByGroupResponse = *reinterpret_cast<aci_att_read_by_group_type_resp_event_rp0*>(vendorEvent->data);

        infra::ByteRange data(&attReadByGroupResponse.Attribute_Data_List[0], &attReadByGroupResponse.Attribute_Data_List[0] + attReadByGroupResponse.Data_Length);
        infra::ByteInputStream stream(data, infra::softFail);

        really_assert(attReadByGroupResponse.Connection_Handle == connectionHandle);

        HandleServiceDiscovered(stream, attReadByGroupResponse.Attribute_Data_Length == uuid16);
    }

    void GattClientSt::HandleAttReadByTypeResponse(evt_blecore_aci* vendorEvent)
    {
        const uint8_t uuid16 = 7;
        auto attReadByTypeResponse = *reinterpret_cast<aci_att_read_by_type_resp_event_rp0*>(vendorEvent->data);

        infra::ByteRange data(&attReadByTypeResponse.Handle_Value_Pair_Data[0], &attReadByTypeResponse.Handle_Value_Pair_Data[0] + attReadByTypeResponse.Data_Length);
        infra::ByteInputStream stream(data, infra::softFail);

        really_assert(attReadByTypeResponse.Connection_Handle == connectionHandle);

        HandleCharacteristicDiscovered(stream, attReadByTypeResponse.Handle_Value_Pair_Length == uuid16);
    }

    void GattClientSt::HandleAttFindInfoResponse(evt_blecore_aci* vendorEvent)
    {
        auto attFindInfoResponse = *reinterpret_cast<aci_att_find_info_resp_event_rp0*>(vendorEvent->data);

        infra::ByteRange data(&attFindInfoResponse.Handle_UUID_Pair[0], &attFindInfoResponse.Handle_UUID_Pair[0] + attFindInfoResponse.Event_Data_Length);
        infra::ByteInputStream stream(data, infra::softFail);

        really_assert(attFindInfoResponse.Connection_Handle == connectionHandle);

        HandleDescriptorDiscovered(stream, attFindInfoResponse.Format == UUID_TYPE_16);
    }

    void GattClientSt::HandleGattCompleteResponse(evt_blecore_aci* vendorEvent)
    {
        auto gattProcedureEvent = *reinterpret_cast<aci_gatt_proc_complete_event_rp0*>(vendorEvent->data);

        really_assert(gattProcedureEvent.Connection_Handle == connectionHandle);
        really_assert(gattProcedureEvent.Error_Code == BLE_STATUS_SUCCESS);

        if (onDiscoveryCompletion)
            infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers(std::exchange(onDiscoveryCompletion, nullptr));
    }

    void GattClientSt::HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto connectionCompleteEvent = *reinterpret_cast<hci_le_connection_complete_event_rp0*>(metaEvent->data);

        really_assert(connectionCompleteEvent.Status == BLE_STATUS_SUCCESS);

        connectionHandle = connectionCompleteEvent.Connection_Handle;
    }

    void GattClientSt::HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto enhancedConnectionCompleteEvent = *reinterpret_cast<hci_le_enhanced_connection_complete_event_rp0*>(metaEvent->data);

        really_assert(enhancedConnectionCompleteEvent.Status == BLE_STATUS_SUCCESS);

        connectionHandle = enhancedConnectionCompleteEvent.Connection_Handle;
    }

    void GattClientSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        auto disconnectionCompleteEvent = *reinterpret_cast<hci_disconnection_complete_event_rp0*>(eventPacket.data);

        really_assert(disconnectionCompleteEvent.Connection_Handle == connectionHandle);
        really_assert(disconnectionCompleteEvent.Status == BLE_STATUS_SUCCESS);

        connectionHandle = GattClientSt::invalidConnection;
    }

    void GattClientSt::HandleServiceDiscovered(infra::DataInputStream& stream, bool isUuid16)
    {
        while (!stream.Empty())
        {
            Atttributes attributes;

            stream >> attributes.startHandle >> attributes.endHandle;

            HandleUuidFromDiscovery(stream, isUuid16, attributes.type);

            really_assert(!stream.Failed());

            infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers([&attributes](auto& observer) { observer.ServiceDiscovered(attributes.type, attributes.startHandle, attributes.endHandle); });
        }
    }

    void GattClientSt::HandleCharacteristicDiscovered(infra::DataInputStream& stream, bool isUuid16)
    {
        while (!stream.Empty())
        {
            Atttributes attributes;

            stream >> attributes.startHandle >> attributes.properties >> attributes.endHandle;

            HandleUuidFromDiscovery(stream, isUuid16, attributes.type);

            really_assert(!stream.Failed());

            infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers([&attributes](auto& observer) { observer.CharacteristicDiscovered(attributes.type, attributes.startHandle, attributes.endHandle, attributes.properties); });
        }
    }

    void GattClientSt::HandleDescriptorDiscovered(infra::DataInputStream& stream, bool isUuid16)
    {
        while (!stream.Empty())
        {
            Atttributes attributes;

            stream >> attributes.startHandle;

            HandleUuidFromDiscovery(stream, isUuid16, attributes.type);

            really_assert(!stream.Failed());

            infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers([&attributes](auto& observer) { observer.DescriptorDiscovered(attributes.type, attributes.startHandle); });
        }
    }

    void GattClientSt::HandleUuidFromDiscovery(infra::DataInputStream& stream, bool isUuid16, services::AttAttribute::Uuid& type)
    {
        if (isUuid16)
        {
            stream >> type.Emplace<services::AttAttribute::Uuid16>();
        }
        else
        {
            stream >> type.Emplace<services::AttAttribute::Uuid128>();
        }
    }
}
