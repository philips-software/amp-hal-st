#include "hal_st/middlewares/ble_middleware/GattClientSt.hpp"
#include "infra/event/EventDispatcherWithWeakPtr.hpp"
#include "infra/stream/InputStream.hpp"
#include "infra/util/Endian.hpp"

extern "C"
{
#include "auto/ble_gatt_aci.h"
#include "ble/ble.h"
}

namespace hal
{
    GattClientSt::GattClientSt(hal::HciEventSource& hciEventSource)
        : hal::HciEventSink(hciEventSource)
        , connectionHandle(invalidConnection)
    {}

    void GattClientSt::StartServiceDiscovery()
    {
        onDiscoveryCompletion = [this]()
        {
            infra::Subject<services::GattClientObserver>::NotifyObservers([](auto& observer)
                {
                    observer.ServiceDiscoveryComplete();
                });
        };

        aci_gatt_disc_all_primary_services(connectionHandle);
    }

    void GattClientSt::StartCharacteristicDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle)
    {
        onDiscoveryCompletion = [this]()
        {
            infra::Subject<services::GattClientObserver>::NotifyObservers([](auto& observer)
                {
                    observer.CharacteristicDiscoveryComplete();
                });
        };

        aci_gatt_disc_all_char_of_service(connectionHandle, handle, endHandle);
    }

    void GattClientSt::StartDescriptorDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle)
    {
        onDiscoveryCompletion = [this]()
        {
            infra::Subject<services::GattClientObserver>::NotifyObservers([](auto& observer)
                {
                    observer.DescriptorDiscoveryComplete();
                });
        };

        aci_gatt_disc_all_char_desc(connectionHandle, handle, endHandle);
    }

    void GattClientSt::Read(const services::GattClientObserver& characteristic, const infra::Function<void(const infra::ConstByteRange&)>& onResponse, const infra::Function<void(uint8_t)>& onDone)
    {
        this->onReadResponse = onResponse;
        this->onCharacteristicOperationsDone = [this, onDone](uint8_t result)
        {
            onDone(result);
        };

        aci_gatt_read_char_value(connectionHandle, characteristic.CharacteristicValueHandle());
    }

    void GattClientSt::Write(const services::GattClientObserver& characteristic, infra::ConstByteRange data, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        this->onCharacteristicOperationsDone = [onDone](uint8_t result)
        {
            onDone(result == BLE_STATUS_SUCCESS ? services::OperationStatus::success : services::OperationStatus::error);
        };

        if (auto status = aci_gatt_write_char_value(connectionHandle, characteristic.CharacteristicValueHandle(), data.size(), data.cbegin()); status != BLE_STATUS_SUCCESS)
            onDone(status == BLE_STATUS_INSUFFICIENT_RESOURCES ? services::OperationStatus::retry : services::OperationStatus::error);
    }

    void GattClientSt::WriteWithoutResponse(const services::GattClientObserver& characteristic, infra::ConstByteRange data, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        auto var = aci_gatt_write_without_resp(connectionHandle, characteristic.CharacteristicValueHandle(), data.size(), data.cbegin());
        if (var == BLE_STATUS_SUCCESS)
            onDone(services::OperationStatus::success);
        else if (var == BLE_STATUS_INSUFFICIENT_RESOURCES)
            onDone(services::OperationStatus::retry);
        else
            onDone(services::OperationStatus::error);
    }

    void GattClientSt::EnableNotification(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::notify, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::enableNotification>(characteristic, onDone);
    }

    void GattClientSt::DisableNotification(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::notify, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::disable>(characteristic, onDone);
    }

    void GattClientSt::EnableIndication(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::indicate, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::enableIndication>(characteristic, onDone);
    }

    void GattClientSt::DisableIndication(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::indicate, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::disable>(characteristic, onDone);
    }

    void GattClientSt::HciEvent(hci_event_pckt& event)
    {
        switch (event.evt)
        {
            case HCI_DISCONNECTION_COMPLETE_EVT_CODE:
                HandleHciDisconnectEvent(*reinterpret_cast<const hci_disconnection_complete_event_rp0*>(event.data));
                break;
            case HCI_LE_META_EVT_CODE:
                HandleHciLeMetaEvent(*reinterpret_cast<const evt_le_meta_event*>(event.data));
                break;
            case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
                HandleHciVendorSpecificDebugEvent(*reinterpret_cast<const evt_blecore_aci*>(event.data));
                break;
            default:
                break;
        }
    }

    void GattClientSt::HandleHciLeMetaEvent(const evt_le_meta_event& metaEvent)
    {
        switch (metaEvent.subevent)
        {
            case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE:
                HandleHciLeEnhancedConnectionCompleteEvent(*reinterpret_cast<const hci_le_enhanced_connection_complete_event_rp0*>(metaEvent.data));
                break;
            case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
                HandleHciLeConnectionCompleteEvent(*reinterpret_cast<const hci_le_connection_complete_event_rp0*>(metaEvent.data));
                break;
            default:
                break;
        }
    }

    void GattClientSt::HandleHciVendorSpecificDebugEvent(const evt_blecore_aci& event)
    {
        switch (event.ecode)
        {
            case ACI_ATT_READ_BY_GROUP_TYPE_RESP_VSEVT_CODE:
                HandleAttReadByGroupTypeResponse(*reinterpret_cast<const aci_att_read_by_group_type_resp_event_rp0*>(event.data));
                break;
            case ACI_ATT_READ_BY_TYPE_RESP_VSEVT_CODE:
                HandleAttReadByTypeResponse(*reinterpret_cast<const aci_att_read_by_type_resp_event_rp0*>(event.data));
                break;
            case ACI_ATT_FIND_INFO_RESP_VSEVT_CODE:
                HandleAttFindInfoResponse(*reinterpret_cast<const aci_att_find_info_resp_event_rp0*>(event.data));
                break;
            case ACI_GATT_PROC_COMPLETE_VSEVT_CODE:
                HandleGattCompleteResponse(*reinterpret_cast<const aci_gatt_proc_complete_event_rp0*>(event.data));
                break;
            case ACI_GATT_INDICATION_VSEVT_CODE:
                HandleGattIndicationEvent(*reinterpret_cast<const aci_gatt_indication_event_rp0*>(event.data));
                break;
            case ACI_GATT_NOTIFICATION_VSEVT_CODE:
                HandleGattNotificationEvent(*reinterpret_cast<const aci_gatt_notification_event_rp0*>(event.data));
                break;
            case ACI_ATT_READ_RESP_VSEVT_CODE:
                HandleAttReadResponse(*reinterpret_cast<const aci_att_read_resp_event_rp0*>(event.data));
                break;
            default:
                break;
        }
    }

    void GattClientSt::HandleAttReadByGroupTypeResponse(const aci_att_read_by_group_type_resp_event_rp0& event)
    {
        const uint8_t uuid16 = 6;

        infra::ConstByteRange data(&event.Attribute_Data_List[0], &event.Attribute_Data_List[0] + event.Data_Length);
        infra::ByteInputStream stream(data, infra::softFail);

        really_assert(event.Connection_Handle == connectionHandle);

        HandleServiceDiscovered(stream, event.Attribute_Data_Length == uuid16);
    }

    void GattClientSt::HandleAttReadByTypeResponse(const aci_att_read_by_type_resp_event_rp0& event)
    {
        const uint8_t uuid16 = 7;

        infra::ConstByteRange data(&event.Handle_Value_Pair_Data[0], &event.Handle_Value_Pair_Data[0] + event.Data_Length);
        infra::ByteInputStream stream(data, infra::softFail);

        really_assert(event.Connection_Handle == connectionHandle);

        HandleCharacteristicDiscovered(stream, event.Handle_Value_Pair_Length == uuid16);
    }

    void GattClientSt::HandleAttFindInfoResponse(const aci_att_find_info_resp_event_rp0& event)
    {
        infra::ConstByteRange data(&event.Handle_UUID_Pair[0], &event.Handle_UUID_Pair[0] + event.Event_Data_Length);
        infra::ByteInputStream stream(data, infra::softFail);

        really_assert(event.Connection_Handle == connectionHandle);

        HandleDescriptorDiscovered(stream, event.Format == UUID_TYPE_16);
    }

    void GattClientSt::HandleGattCompleteResponse(const aci_gatt_proc_complete_event_rp0& event)
    {
        if (onDiscoveryCompletion)
            onDiscoveryCompletion(); // Does this conflict with other operations? If not, why do we even get a GattComplete for discovery?
        else if (onCharacteristicOperationsDone)
            onCharacteristicOperationsDone(event.Error_Code);
    }

    void GattClientSt::HandleHciLeConnectionCompleteEvent(const hci_le_connection_complete_event_rp0& event)
    {
        if (event.Status == BLE_STATUS_SUCCESS)
            connectionHandle = event.Connection_Handle;
    }

    void GattClientSt::HandleHciLeEnhancedConnectionCompleteEvent(const hci_le_enhanced_connection_complete_event_rp0& event)
    {
        if (event.Status == BLE_STATUS_SUCCESS)
            connectionHandle = event.Connection_Handle;
    }

    void GattClientSt::HandleHciDisconnectEvent(const hci_disconnection_complete_event_rp0& event)
    {
        really_assert(event.Connection_Handle == connectionHandle);

        if (event.Status == BLE_STATUS_SUCCESS)
            connectionHandle = GattClientSt::invalidConnection;
    }

    void GattClientSt::HandleAttReadResponse(const aci_att_read_resp_event_rp0& event)
    {
        infra::ConstByteRange data(&event.Attribute_Value[0], &event.Attribute_Value[0] + event.Event_Data_Length);

        really_assert(event.Connection_Handle == connectionHandle);

        if (onReadResponse)
            onReadResponse(data);
    }

    void GattClientSt::HandleGattIndicationEvent(const aci_gatt_indication_event_rp0& event)
    {
        infra::ConstByteRange data(&event.Attribute_Value[0], &event.Attribute_Value[0] + event.Attribute_Value_Length);

        really_assert(event.Connection_Handle == connectionHandle);

        GattClient::NotifyObservers([this, &event, &data](auto& observer)
            {
                observer.IndicationReceived(event.Attribute_Handle, data, [this]()
                    {
                        this->HandleGattConfirmIndication();
                    });
            });
    }

    void GattClientSt::HandleGattNotificationEvent(const aci_gatt_notification_event_rp0& event)
    {
        infra::ConstByteRange data(&event.Attribute_Value[0], &event.Attribute_Value[0] + event.Attribute_Value_Length);

        really_assert(event.Connection_Handle == connectionHandle);

        GattClient::NotifyObservers([&event, &data](auto& observer)
            {
                observer.NotificationReceived(event.Attribute_Handle, data);
            });
    }

    void GattClientSt::HandleGattConfirmIndication()
    {
        aci_gatt_confirm_indication(connectionHandle);
    }

    void GattClientSt::HandleServiceDiscovered(infra::DataInputStream& stream, bool isUuid16)
    {
        while (!stream.Empty())
        {
            Atttributes attributes;

            stream >> attributes.startHandle >> attributes.endHandle;

            HandleUuidFromDiscovery(stream, isUuid16, attributes.type);

            really_assert(!stream.Failed());

            infra::Subject<services::GattClientObserver>::NotifyObservers([&attributes](auto& observer)
                {
                    observer.ServiceDiscovered(attributes.type, attributes.startHandle, attributes.endHandle);
                });
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

            infra::Subject<services::GattClientObserver>::NotifyObservers([&attributes](auto& observer)
                {
                    observer.CharacteristicDiscovered(attributes.type, attributes.startHandle, attributes.endHandle, attributes.properties);
                });
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

            infra::Subject<services::GattClientObserver>::NotifyObservers([&attributes](auto& observer)
                {
                    observer.DescriptorDiscovered(attributes.type, attributes.startHandle);
                });
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

    void GattClientSt::WriteCharacteristicDescriptor(const services::GattClientObserver& characteristic, services::GattCharacteristic::PropertyFlags property, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue characteristicValue) const
    {
        const uint16_t offsetCccd = 1;

        aci_gatt_write_char_desc(connectionHandle, characteristic.CharacteristicValueHandle() + offsetCccd, sizeof(characteristicValue), reinterpret_cast<uint8_t*>(&characteristicValue));
    }
}
