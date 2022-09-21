#include "hal_st/middlewares/ble_middleware/GattServerSt.hpp"

extern "C"
{
    #include "ble/core/ble_core.h"
}

namespace
{
    uint8_t UuidToType(const services::GattAttribute::Uuid& uuid)
    {
        if (uuid.Is<services::GattAttribute::Uuid16>())
            return 0x01;
        else if (uuid.Is<services::GattAttribute::Uuid128>())
            return 0x02;
        
        std::abort(); // Unsupported uuid type
    }

    template<class UuidType>
    const auto* ConvertUuid(const services::GattAttribute::Uuid& uuid)
    {
        if (uuid.Is<services::GattAttribute::Uuid16>())
            return reinterpret_cast<const UuidType*>(&uuid.Get<services::GattAttribute::Uuid16>());
        else if (uuid.Is<services::GattAttribute::Uuid128>())
            return reinterpret_cast<const UuidType*>(&uuid.Get<services::GattAttribute::Uuid128>());

        std::abort(); // Unsupported uuid type
    }

    uint8_t ConvertProperties(services::GattCharacteristic::PropertyFlags properties)
    {
        return infra::enum_cast(properties);
    }

    uint8_t ConvertPermissions(services::GattCharacteristic::PermissionFlags permissions)
    {
        return infra::enum_cast(permissions);
    }
}

namespace hal
{
    GattServerSt::GattServerSt(hal::HciEventSource& hciEventSource)
        : hal::HciEventSink(hciEventSource)
    {}

    void GattServerSt::AddService(services::GattService& service)
    {
        constexpr uint8_t gattPrimaryService = 0x01;

        auto result = aci_gatt_add_service(UuidToType(service.Type()), ConvertUuid<Service_UUID_t>(service.Type()),
            gattPrimaryService, service.GetAttributeCount(), &service.Handle());

        if (result != BLE_STATUS_SUCCESS)
            ReportError(result);

        for (auto& characteristic : service.Characteristics())
            AddCharacteristic(characteristic);

        services.push_front(service);
    }

    services::GattCharacteristicClientOperations::UpdateStatus GattServerSt::Update(const services::GattCharacteristicClientOperationsObserver& characteristic, infra::ConstByteRange data) const
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

    void GattServerSt::AddCharacteristic(services::GattCharacteristic& characteristic)
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
        infra::ByteRange data{event.Attr_Data, event.Attr_Data + event.Attr_Data_Length};

        for (auto& service : services)
            for (auto& characteristic : service.Characteristics())
                if (event.Attr_Handle == characteristic.Handle() + valueAttributeOffset)
                    characteristic.NotifyObservers([data](auto& observer) { observer.DataReceived(data); });
    }

    void GattServerSt::ReportError(tBleStatus status) const
    {}
}
