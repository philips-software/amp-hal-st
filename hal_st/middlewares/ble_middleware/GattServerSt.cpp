#include "hal_st/middlewares/ble_middleware/GattServerSt.hpp"

extern "C"
{
    #include "ble/core/ble_core.h"
}

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

    uint16_t GattServerSt::EffectiveMaxAttMtuSize() const
    {
        return maxAttMtu;
    }

    void GattServerSt::RequestMtuExchange()
    {
        std::abort();
    }

    void GattServerSt::AddService(services::GattServerService& service)
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
            auto vendorEvent = reinterpret_cast<evt_blecore_aci*>(event.data);

            switch (vendorEvent->ecode)
            {
                case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
                    HandleGattAttributeModifiedResponseEvent(vendorEvent);
                    break;
                case ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE:
                    HandleAttExchangeMtuResponseEvent(vendorEvent);
                    break;
                default:
                    break;
            }
        }
    }

    void GattServerSt::HandleAttExchangeMtuResponseEvent(evt_blecore_aci* vendorEvent)
    {
        maxAttMtu = reinterpret_cast<aci_att_exchange_mtu_resp_event_rp0*>(vendorEvent->data)->Server_RX_MTU;
        AttMtuExchange::NotifyObservers([](auto& observer) { observer.ExchangedMaxAttMtuSize(); });
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

    void GattServerSt::HandleGattAttributeModifiedResponseEvent(evt_blecore_aci* vendorEvent)
    {
        auto attributeModifiedEvent = *reinterpret_cast<aci_gatt_attribute_modified_event_rp0*>(vendorEvent->data);

        constexpr uint16_t valueAttributeOffset = 1;
        infra::ByteRange data{attributeModifiedEvent.Attr_Data, attributeModifiedEvent.Attr_Data + attributeModifiedEvent.Attr_Data_Length};

        for (auto& service : services)
            for (auto& characteristic : service.Characteristics())
                if (attributeModifiedEvent.Attr_Handle == characteristic.Handle() + valueAttributeOffset)
                    characteristic.NotifyObservers([data](auto& observer) { observer.DataReceived(data); });
    }

    void GattServerSt::ReportError(tBleStatus status) const
    {}
}
