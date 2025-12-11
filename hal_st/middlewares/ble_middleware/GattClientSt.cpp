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
        onDiscoveryCompletion = [this](services::OperationStatus status)
        {
            infra::Subject<services::GattClientObserver>::NotifyObservers([status](auto& observer)
                {
                    observer.ServiceDiscoveryComplete(status);
                });
        };

        auto ret = aci_gatt_disc_all_primary_services(connectionHandle);
        if (ret != BLE_STATUS_SUCCESS)
            onDiscoveryCompletion(services::OperationStatus::error);
    }

    void GattClientSt::StartCharacteristicDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle)
    {
        onDiscoveryCompletion = [this](services::OperationStatus status)
        {
            infra::Subject<services::GattClientObserver>::NotifyObservers([status](auto& observer)
                {
                    observer.CharacteristicDiscoveryComplete(status);
                });
        };

        auto ret = aci_gatt_disc_all_char_of_service(connectionHandle, handle, endHandle);
        if (ret != BLE_STATUS_SUCCESS)
            onDiscoveryCompletion(services::OperationStatus::error);
    }

    void GattClientSt::StartDescriptorDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle)
    {
        onDiscoveryCompletion = [this](services::OperationStatus status)
        {
            infra::Subject<services::GattClientObserver>::NotifyObservers([status](auto& observer)
                {
                    observer.DescriptorDiscoveryComplete(status);
                });
        };

        auto ret = aci_gatt_disc_all_char_desc(connectionHandle, handle, endHandle);
        if (ret != BLE_STATUS_SUCCESS)
            onDiscoveryCompletion(services::OperationStatus::error);
    }

    void GattClientSt::Read(services::AttAttribute::Handle handle, const infra::Function<void(const infra::ConstByteRange&)>& onResponse, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        this->onReadResponse = onResponse;
        this->onCharacteristicOperationsDone = [this, onDone](services::OperationStatus result)
        {
            onDone(result);
        };

        auto ret = aci_gatt_read_char_value(connectionHandle, handle);
        if (ret != BLE_STATUS_SUCCESS)
            this->onCharacteristicOperationsDone(services::OperationStatus::error);
    }

    void GattClientSt::Write(services::AttAttribute::Handle handle, infra::ConstByteRange data, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        this->onCharacteristicOperationsDone = [this, onDone](services::OperationStatus result)
        {
            onDone(result);
        };

        auto ret = aci_gatt_write_char_value(connectionHandle, handle, data.size(), data.cbegin());
        if (ret != BLE_STATUS_SUCCESS)
            this->onCharacteristicOperationsDone(services::OperationStatus::error);
    }

    void GattClientSt::WriteWithoutResponse(services::AttAttribute::Handle handle, infra::ConstByteRange data, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        auto var = aci_gatt_write_without_resp(connectionHandle, handle, data.size(), data.cbegin());
        if (var == BLE_STATUS_SUCCESS)
            onDone(services::OperationStatus::success);
        else if (var == BLE_STATUS_INSUFFICIENT_RESOURCES)
            onDone(services::OperationStatus::retry);
        else
            onDone(services::OperationStatus::error);
    }

    void GattClientSt::EnableNotification(services::AttAttribute::Handle handle, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::notify, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::enableNotification>(handle, onDone);
    }

    void GattClientSt::DisableNotification(services::AttAttribute::Handle handle, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::notify, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::disable>(handle, onDone);
    }

    void GattClientSt::EnableIndication(services::AttAttribute::Handle handle, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::indicate, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::enableIndication>(handle, onDone);
    }

    void GattClientSt::DisableIndication(services::AttAttribute::Handle handle, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::indicate, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::disable>(handle, onDone);
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
        if (this->onDiscoveryCompletion)
            this->onDiscoveryCompletion(event.Error_Code == BLE_STATUS_SUCCESS ? services::OperationStatus::success : services::OperationStatus::error); // Does this conflict with other operations? If not, why do we even get a GattComplete for discovery?
        else if (this->onCharacteristicOperationsDone)
            this->onCharacteristicOperationsDone(event.Error_Code == BLE_STATUS_SUCCESS ? services::OperationStatus::success : services::OperationStatus::error);
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

    void GattClientSt::WriteCharacteristicDescriptor(services::AttAttribute::Handle handle, services::GattCharacteristic::PropertyFlags property, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue characteristicValue)
    {
        const uint16_t offsetCccd = 1;

        auto ret = aci_gatt_write_char_desc(connectionHandle, handle + offsetCccd, sizeof(characteristicValue), reinterpret_cast<uint8_t*>(&characteristicValue));
        if (ret != BLE_STATUS_SUCCESS)
            onCharacteristicOperationsDone(services::OperationStatus::error);
    }

    void GattClientSt::MtuExchange()
    {
        auto status = aci_gatt_exchange_config(connectionHandle);
        assert(status == BLE_STATUS_SUCCESS);
    }

    uint16_t GattClientSt::EffectiveMaxAttMtuSize() const
    {
        return maxAttMtu;
    }

    void GattClientSt::HandleAttExchangeMtuResponse(const aci_att_exchange_mtu_resp_event_rp0& event)
    {
        really_assert(event.Connection_Handle == connectionHandle);
        maxAttMtu = event.Server_RX_MTU;

        AttMtuExchange::NotifyObservers([](auto& observer)
            {
                observer.ExchangedMaxAttMtuSize();
            });
    }
}
