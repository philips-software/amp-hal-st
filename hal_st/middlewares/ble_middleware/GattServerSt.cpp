#include "hal_st/middlewares/ble_middleware/GattServerSt.hpp"
#include "services/tracer/GlobalTracer.hpp"

namespace
{
    uint8_t UuidToType(const services::AttAttribute::Uuid& uuid)
    {
        if (uuid.Is<services::AttAttribute::Uuid16>())
            return 0x01;
        else if (uuid.Is<services::AttAttribute::Uuid128>())
            return 0x02;

        std::abort(); // Unsupported uuid type
    }

    template<class UuidType>
    const auto* ConvertUuid(const services::AttAttribute::Uuid& uuid)
    {
        if (uuid.Is<services::AttAttribute::Uuid16>())
            return reinterpret_cast<const UuidType*>(&uuid.Get<services::AttAttribute::Uuid16>());
        else if (uuid.Is<services::AttAttribute::Uuid128>())
            return reinterpret_cast<const UuidType*>(&uuid.Get<services::AttAttribute::Uuid128>());

        std::abort(); // Unsupported uuid type
    }

    uint8_t ConvertProperties(services::GattCharacteristic::PropertyFlags properties)
    {
        return infra::enum_cast(properties);
    }

    uint8_t ConvertPermissions(services::GattServerCharacteristic::PermissionFlags permissions)
    {
        return infra::enum_cast(permissions);
    }
}

namespace hal
{
    GattServerSt::GattServerSt(hal::HciEventSource& hciEventSource)
        : hal::HciEventSink(hciEventSource)
    {}

    void GattServerSt::AddService(services::GattServerService& service)
    {
        constexpr uint8_t gattPrimaryService = 0x01;
        uint8_t attributeCount = service.GetAttributeCount();

        services::GlobalTracer().Trace() << "GattServerSt::AddService: attributeCount=" << attributeCount;

        auto result = aci_gatt_add_service(UuidToType(service.Type()), ConvertUuid<Service_UUID_t>(service.Type()),
            gattPrimaryService, attributeCount, &service.Handle());

        if (result != BLE_STATUS_SUCCESS)
            ReportError(result);

        services::GlobalTracer().Trace() << "GattServerSt::AddService: Adding characteristics...";
        for (auto& characteristic : service.Characteristics())
        {
            AddCharacteristic(characteristic);

            for (auto& descriptor : characteristic.Descriptors())
            {
                AddCharacteristicDescriptor(characteristic, descriptor);
            }
        }

        services.push_front(service);
    }

    services::GattServerCharacteristicOperations::UpdateStatus GattServerSt::Update(const services::GattServerCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data) const
    {
        constexpr uint8_t valueOffset = 0;
        auto result = aci_gatt_update_char_value(characteristic.ServiceHandle(),
            characteristic.CharacteristicHandle(),
            valueOffset,
            data.size(),
            data.begin());

        if (result != BLE_STATUS_SUCCESS)
            ReportError(result);

        if (result == BLE_STATUS_SUCCESS)
            return UpdateStatus::success;
        else if (result == BLE_STATUS_INSUFFICIENT_RESOURCES)
            return UpdateStatus::retry;
        else
            return UpdateStatus::error;
    }

    void GattServerSt::HciEvent(hci_event_pckt& event)
    {
        if (event.evt == HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE)
        {
            assert(event.data != nullptr);

            auto& coreEvent = *reinterpret_cast<evt_blecore_aci*>(event.data);
            if (coreEvent.ecode == ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE)
            {
                assert(coreEvent.data != nullptr);

                auto& attributeModifiedEvent = *reinterpret_cast<aci_gatt_attribute_modified_event_rp0*>(coreEvent.data);
                HandleGattAttributeModified(attributeModifiedEvent);
            }
        }
    }

    void GattServerSt::AddCharacteristic(services::GattServerCharacteristic& characteristic)
    {
        constexpr uint8_t encryptionKeySize = 0x10;
        constexpr uint8_t notifyAttributeWrite = 0x01;
        constexpr uint8_t variableLength = 0x01;

        auto result = aci_gatt_add_char(characteristic.ServiceHandle(),
            UuidToType(characteristic.Type()),
            ConvertUuid<Char_UUID_t>(characteristic.Type()),
            characteristic.ValueLength(),
            ConvertProperties(characteristic.Properties()),
            ConvertPermissions(characteristic.Permissions()),
            notifyAttributeWrite,
            encryptionKeySize,
            variableLength,
            &characteristic.Handle());

        if (result == BLE_STATUS_SUCCESS)
            characteristic.Attach(*this);
        else
            ReportError(result);
    }

    void GattServerSt::AddCharacteristicDescriptor(const services::GattServerCharacteristic& characteristic, const services::GattServerCharacteristicDescriptor& descriptor)
    {
        services::GlobalTracer().Trace() << "GattServerSt::AddCharacteristicDescriptor: Adding  descriptor UUID=0x"
                                         << infra::hex << (descriptor.Uuid().Is<services::AttAttribute::Uuid16>() ? descriptor.Uuid().Get<services::AttAttribute::Uuid16>() : 0)
                                         << ", size=" << descriptor.Data().size()
                                         << ", data=0x" << infra::AsHex(descriptor.Data());

        constexpr uint8_t encryptionKeySize = 7;
        constexpr uint8_t attributePermissions = ATTR_PERMISSION_NONE;
        constexpr uint8_t attributeAccess = ATTR_ACCESS_READ_ONLY;
        constexpr uint8_t notifyEvent = GATT_DONT_NOTIFY_EVENTS;
        constexpr uint8_t variableLength = 0;

        uint16_t descriptorHandle;

        auto result = aci_gatt_add_char_desc(characteristic.ServiceHandle(),
            characteristic.CharacteristicHandle(),
            UuidToType(descriptor.Uuid()),
            ConvertUuid<Char_Desc_Uuid_t>(descriptor.Uuid()),
            descriptor.Data().size(),
            descriptor.Data().size(),
            descriptor.Data().begin(),
            attributePermissions,
            attributeAccess,
            notifyEvent,
            encryptionKeySize,
            variableLength,
            &descriptorHandle);

        if (result != BLE_STATUS_SUCCESS)
        {
            services::GlobalTracer().Trace() << "GattServerSt::AddDescriptor: Failed with result=0x"
                                             << infra::hex << static_cast<uint32_t>(result);
            ReportError(result);
        }
        else
        {
            services::GlobalTracer().Trace() << "GattServerSt::AddDescriptor: Success, handle=0x"
                                             << infra::hex << descriptorHandle;
        }
    }

    void GattServerSt::HandleGattAttributeModified(aci_gatt_attribute_modified_event_rp0& event)
    {
        constexpr uint16_t valueAttributeOffset = 1;
        infra::ByteRange data{ event.Attr_Data, event.Attr_Data + event.Attr_Data_Length };

        for (auto& service : services)
            for (auto& characteristic : service.Characteristics())
                if (event.Attr_Handle == characteristic.Handle() + valueAttributeOffset)
                    characteristic.NotifyObservers([data](auto& observer)
                        {
                            observer.DataReceived(data);
                        });
    }

    void GattServerSt::ReportError(tBleStatus status) const
    {}

    GattConfirmIndication::GattConfirmIndication(hal::HciEventSource& hciEventSource)
        : hal::HciEventSink(hciEventSource)
    {}

    void GattConfirmIndication::HciEvent(hci_event_pckt& event)
    {
        if (event.evt == HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE)
        {
            const auto vendorEvent = reinterpret_cast<evt_blecore_aci*>(event.data);
            if (vendorEvent->ecode == ACI_GATT_INDICATION_VSEVT_CODE)
            {
                const auto gattIndicationEvent = *reinterpret_cast<aci_gatt_indication_event_rp0*>(vendorEvent->data);
                aci_gatt_confirm_indication(gattIndicationEvent.Connection_Handle);
            }
        }
    }
}
