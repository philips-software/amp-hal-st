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
        // HACK: Add extra attributes for HID service descriptors
        uint8_t attributeCount = service.GetAttributeCount();
        if (service.Type().Is<services::AttAttribute::Uuid16>() &&
            service.Type().Get<services::AttAttribute::Uuid16>() == 0x1812) // HID Service UUID
        {
            attributeCount += 1; // Add 1 for the Report Reference descriptor (only for Report characteristic)
            services::GlobalTracer().Trace() << "GattServerSt::AddService: Adding HID Service with " << static_cast<uint32_t>(attributeCount) << " attributes";
        }

        auto result = aci_gatt_add_service(UuidToType(service.Type()), ConvertUuid<Service_UUID_t>(service.Type()),
            gattPrimaryService, attributeCount, &service.Handle());

        if (result != BLE_STATUS_SUCCESS)
            ReportError(result);

        services::GlobalTracer().Trace() << "GattServerSt::AddService: Adding characteristics...";
        for (auto& characteristic : service.Characteristics())
        {
            auto charUuid = characteristic.Type().Get<services::AttAttribute::Uuid16>();

            services::GlobalTracer().Trace() << "GattServerSt::AddService: Adding characteristic UUID=0x" << infra::hex << charUuid;
            AddCharacteristic(characteristic);

            if (charUuid == 0x2A4D) // HID Report characteristic
            {
                // HACK: Add Report Reference descriptors immediately after adding 0x2A4D descriptor. <--- shows 0x2908 immediately after 0x2A4D in nRF connect, but now keys do not work in android.  Keypress does not work in windows
                services::GlobalTracer().Trace() << "GattServerSt::AddService: Found HID Report characteristic (0x2A4D), adding Report Reference descriptor";
                constexpr services::AttAttribute::Uuid16 reportRefDescUuid{ 0x2908 };
                constexpr uint8_t reportRefData[] = { 0x01, 0x01 }; // Report ID 1, Input Report
                AddDescriptor(characteristic, reportRefDescUuid, infra::MakeConstByteRange(reportRefData));
            }
        }

        // HACK: Add Report Reference descriptors AFTER all characteristics and CCCDs are created   <---- works on android, but shows 0x2908 as last entry under 0x2a4a (Hid information) in nRF connect. Windows connects, but keypress does not work
        // This ensures we don't interfere with the automatic CCCD creation for notify characteristics
        // if (service.Type().Is<services::AttAttribute::Uuid16>() &&
        //     service.Type().Get<services::AttAttribute::Uuid16>() == 0x1812) // HID Service UUID
        // {
        //     services::GlobalTracer().Trace() << "GattServerSt::AddService: Looking for HID Report characteristic to add descriptor...";
        //     // Find the specific Report characteristic (0x2A4D) and add descriptor only to it
        //     for (auto& characteristic : service.Characteristics())
        //     {
        //         if (characteristic.Type().Is<services::AttAttribute::Uuid16>())
        //         {
        //             auto charUuid = characteristic.Type().Get<services::AttAttribute::Uuid16>();
        //             services::GlobalTracer().Trace() << "GattServerSt::AddService: Checking characteristic UUID=0x" << infra::hex << charUuid;

        //             // Add Report Reference descriptor to Report characteristic (0x2A4D) ONLY
        //             // Boot Protocol characteristics should NOT have Report Reference descriptors per HID spec
        //             if (charUuid == 0x2A4D) // HID Report characteristic
        //             {
        //                 services::GlobalTracer().Trace() << "GattServerSt::AddService: Found HID Report characteristic (0x2A4D), adding Report Reference descriptor";
        //                 constexpr services::AttAttribute::Uuid16 reportRefDescUuid{ 0x2908 };
        //                 constexpr uint8_t reportRefData[] = { 0x01, 0x01 }; // Report ID 1, Input Report (fix: correct report ID)
        //                 AddDescriptor(characteristic, reportRefDescUuid, infra::MakeConstByteRange(reportRefData));
        //                 break; // Only add to the first (and should be only) Report characteristic
        //             }
        //         }
        //     }
        // }

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

    void GattServerSt::AddDescriptor(const services::GattServerCharacteristicOperationsObserver& characteristic, const services::AttAttribute::Uuid& uuid, infra::ConstByteRange data)
    {
        services::GlobalTracer().Trace() << "GattServerSt::AddDescriptor: Adding descriptor UUID=0x"
                                         << infra::hex << (uuid.Is<services::AttAttribute::Uuid16>() ? uuid.Get<services::AttAttribute::Uuid16>() : 0)
                                         << ", size=" << data.size()
                                         << ", data=0x" << infra::AsHex(data);

        constexpr uint8_t encryptionKeySize = 0x10;
        constexpr uint8_t attributePermissions = ATTR_PERMISSION_NONE;
        constexpr uint8_t attributeAccess = ATTR_ACCESS_READ_ONLY;
        constexpr uint8_t notifyEvent = GATT_DONT_NOTIFY_EVENTS;
        constexpr uint8_t variableLength = 0;

        uint16_t descriptorHandle;

        auto result = aci_gatt_add_char_desc(characteristic.ServiceHandle(),
            characteristic.CharacteristicHandle(),
            UuidToType(uuid),
            ConvertUuid<Char_Desc_Uuid_t>(uuid),
            data.size(),
            data.size(),
            data.begin(),
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
