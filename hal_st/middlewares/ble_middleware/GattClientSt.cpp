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
        claimerDiscovery.Claim([this]()
            {
                onDiscoveryCompletion = [this]()
                {
                    infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers([](auto& observer)
                        {
                            observer.ServiceDiscoveryComplete();
                        });

                    claimerDiscovery.Release();
                };

                aci_gatt_disc_all_primary_services(connectionHandle);
            });
    }

    void GattClientSt::StartCharacteristicDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle)
    {
        claimerDiscovery.Claim([this, handle, endHandle]()
            {
                onDiscoveryCompletion = [this]()
                {
                    infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers([](auto& observer)
                        {
                            observer.CharacteristicDiscoveryComplete();
                        });

                    claimerDiscovery.Release();
                };

                aci_gatt_disc_all_char_of_service(connectionHandle, handle, endHandle);
            });
    }

    void GattClientSt::StartDescriptorDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle)
    {
        claimerDiscovery.Claim([this, handle, endHandle]()
            {
                onDiscoveryCompletion = [this]()
                {
                    infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers([](auto& observer)
                        {
                            observer.DescriptorDiscoveryComplete();
                        });

                    claimerDiscovery.Release();
                };

                aci_gatt_disc_all_char_desc(connectionHandle, handle, endHandle);
            });
    }

    void GattClientSt::Read(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(const infra::ConstByteRange&)>& onResponse, const infra::Function<void(uint8_t)>& onDone)
    {
        claimerCharacteristicOperations.Claim([this, &characteristic, onResponse, onDone]()
            {
                this->onReadResponse = onResponse;
                this->onCharacteristicOperationsDone = [this, onDone](uint8_t result)
                {
                    auto onDoneCopy = onDone;
                    claimerCharacteristicOperations.Release();
                    onDoneCopy(result);
                };

                aci_gatt_read_char_value(connectionHandle, characteristic.CharacteristicValueHandle());
            });
    }

    void GattClientSt::Write(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data, const infra::Function<void(uint8_t)>& onDone)
    {
        claimerCharacteristicOperations.Claim([this, &characteristic, data, onDone]()
            {
                this->onCharacteristicOperationsDone = [this, onDone](uint8_t result)
                {
                    auto onDoneCopy = onDone;
                    claimerCharacteristicOperations.Release();
                    onDoneCopy(result);
                };

                aci_gatt_write_char_value(connectionHandle, characteristic.CharacteristicValueHandle(), data.size(), data.cbegin());
            });
    }

    services::GattCharacteristicOperationsResult GattClientSt::WriteWithoutResponse(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data)
    {
        auto result = aci_gatt_write_without_resp(connectionHandle, characteristic.CharacteristicValueHandle(), data.size(), data.cbegin());

        if (result == BLE_STATUS_SUCCESS)
            return services::GattCharacteristicOperationsResult::success;
        else if (result == BLE_STATUS_INSUFFICIENT_RESOURCES)
            return services::GattCharacteristicOperationsResult::retry;
        else
            return services::GattCharacteristicOperationsResult::error;
    }

    void GattClientSt::EnableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::notify, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::enableNotification>(characteristic, onDone);
    }

    void GattClientSt::DisableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::notify, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::disable>(characteristic, onDone);
    }

    void GattClientSt::EnableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::indicate, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::enableIndication>(characteristic, onDone);
    }

    void GattClientSt::DisableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        WriteCharacteristicDescriptor<services::GattCharacteristic::PropertyFlags::indicate, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue::disable>(characteristic, onDone);
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
            case ACI_GATT_INDICATION_VSEVT_CODE:
                HandleGattIndicationEvent(vendorEvent);
                break;
            case ACI_GATT_NOTIFICATION_VSEVT_CODE:
                HandleGattNotificationEvent(vendorEvent);
                break;
            case ACI_ATT_READ_RESP_VSEVT_CODE:
                HandleAttReadResponse(vendorEvent);
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

        if (onDiscoveryCompletion)
            onDiscoveryCompletion(); // Does this conflict with other operations? If not, why do we even get a GattComplete for discovery?
        else if (onCharacteristicOperationsDone)
            onCharacteristicOperationsDone(gattProcedureEvent.Error_Code);
    }

    void GattClientSt::HandleHciLeConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto connectionCompleteEvent = *reinterpret_cast<hci_le_connection_complete_event_rp0*>(metaEvent->data);

        if (connectionCompleteEvent.Status == BLE_STATUS_SUCCESS)
            connectionHandle = connectionCompleteEvent.Connection_Handle;
    }

    void GattClientSt::HandleHciLeEnhancedConnectionCompleteEvent(evt_le_meta_event* metaEvent)
    {
        auto enhancedConnectionCompleteEvent = *reinterpret_cast<hci_le_enhanced_connection_complete_event_rp0*>(metaEvent->data);

        if (enhancedConnectionCompleteEvent.Status == BLE_STATUS_SUCCESS)
            connectionHandle = enhancedConnectionCompleteEvent.Connection_Handle;
    }

    void GattClientSt::HandleHciDisconnectEvent(hci_event_pckt& eventPacket)
    {
        auto disconnectionCompleteEvent = *reinterpret_cast<hci_disconnection_complete_event_rp0*>(eventPacket.data);

        really_assert(disconnectionCompleteEvent.Connection_Handle == connectionHandle);

        if (disconnectionCompleteEvent.Status == BLE_STATUS_SUCCESS)
            connectionHandle = GattClientSt::invalidConnection;
    }

    void GattClientSt::HandleAttReadResponse(evt_blecore_aci* vendorEvent)
    {
        auto attReadResponse = *reinterpret_cast<aci_att_read_resp_event_rp0*>(vendorEvent->data);

        infra::ByteRange data(&attReadResponse.Attribute_Value[0], &attReadResponse.Attribute_Value[0] + attReadResponse.Event_Data_Length);

        really_assert(attReadResponse.Connection_Handle == connectionHandle);

        if (onReadResponse)
        {
            onReadResponse(data);
            claimerCharacteristicOperations.Release();
        }
    }

    void GattClientSt::HandleGattIndicationEvent(evt_blecore_aci* vendorEvent)
    {
        auto gattIndicationEvent = *reinterpret_cast<aci_gatt_indication_event_rp0*>(vendorEvent->data);

        infra::ByteRange data(&gattIndicationEvent.Attribute_Value[0], &gattIndicationEvent.Attribute_Value[0] + gattIndicationEvent.Attribute_Value_Length);

        really_assert(gattIndicationEvent.Connection_Handle == connectionHandle);

        infra::Subject<services::GattClientStackUpdateObserver>::NotifyObservers([this, &gattIndicationEvent, &data](auto& observer)
            {
                observer.IndicationReceived(gattIndicationEvent.Attribute_Handle, data, [this, &gattIndicationEvent]()
                    {
                        this->HandleGattConfirmIndication(gattIndicationEvent.Attribute_Handle);
                    });
            });
    }

    void GattClientSt::HandleGattNotificationEvent(evt_blecore_aci* vendorEvent)
    {
        auto gattNotificationEvent = *reinterpret_cast<aci_gatt_notification_event_rp0*>(vendorEvent->data);

        infra::ByteRange data(&gattNotificationEvent.Attribute_Value[0], &gattNotificationEvent.Attribute_Value[0] + gattNotificationEvent.Attribute_Value_Length);

        really_assert(gattNotificationEvent.Connection_Handle == connectionHandle);

        infra::Subject<services::GattClientStackUpdateObserver>::NotifyObservers([&gattNotificationEvent, &data](auto& observer)
            {
                observer.NotificationReceived(gattNotificationEvent.Attribute_Handle, data);
            });
    }

    void GattClientSt::HandleGattConfirmIndication(services::AttAttribute::Handle handle)
    {
        aci_gatt_confirm_indication(handle);
    }

    void GattClientSt::HandleServiceDiscovered(infra::DataInputStream& stream, bool isUuid16)
    {
        while (!stream.Empty())
        {
            Atttributes attributes;

            stream >> attributes.startHandle >> attributes.endHandle;

            HandleUuidFromDiscovery(stream, isUuid16, attributes.type);

            really_assert(!stream.Failed());

            infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers([&attributes](auto& observer)
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

            infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers([&attributes](auto& observer)
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

            infra::Subject<services::GattClientDiscoveryObserver>::NotifyObservers([&attributes](auto& observer)
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

    void GattClientSt::WriteCharacteristicDescriptor(const services::GattClientCharacteristicOperationsObserver& characteristic, services::GattCharacteristic::PropertyFlags property, services::GattDescriptor::ClientCharacteristicConfiguration::CharacteristicValue characteristicValue) const
    {
        const uint16_t offsetCccd = 1;

        aci_gatt_write_char_desc(connectionHandle, characteristic.CharacteristicValueHandle() + offsetCccd, sizeof(characteristicValue), reinterpret_cast<uint8_t*>(&characteristicValue));
    }
}
