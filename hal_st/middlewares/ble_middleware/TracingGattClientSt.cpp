#include "hal_st/middlewares/ble_middleware/TracingGattClientSt.hpp"
#include "infra/stream/StringOutputStream.hpp"

namespace infra
{
    TextOutputStream& operator<<(TextOutputStream& stream, const services::AttAttribute::Uuid& uuid)
    {
        if (uuid.Is<services::AttAttribute::Uuid16>())
            stream << "0x" << hex << uuid.Get<services::AttAttribute::Uuid16>();
        else
            stream << "[" << AsHex(MakeByteRange(uuid.Get<services::AttAttribute::Uuid128>())) << "]";

        return stream;
    }
}

namespace hal
{
    TracingGattClientSt::TracingGattClientSt(hal::HciEventSource& hciEventSource, services::Tracer& tracer)
        : GattClientSt(hciEventSource)
        , tracer(tracer)
    {
        tracer.Trace() << "TracingGattClientSt::TracingGattClientSt()";
    }

    void TracingGattClientSt::StartServiceDiscovery()
    {
        tracer.Trace() << "TracingGattClientSt::StartServiceDiscovery";
        GattClientSt::StartServiceDiscovery();
    }

    void TracingGattClientSt::StartCharacteristicDiscovery(const services::GattService& service)
    {
        tracer.Trace() << "TracingGattClientSt::StartCharacteristicDiscovery";
        GattClientSt::StartCharacteristicDiscovery(service);
    }

    void TracingGattClientSt::StartDescriptorDiscovery(const services::GattService& service)
    {
        tracer.Trace() << "TracingGattClientSt::StartDescriptorDiscovery";
        GattClientSt::StartDescriptorDiscovery(service);
    }

    void TracingGattClientSt::Read(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void(const infra::ConstByteRange&)>& onResponse) const
    {
        tracer.Trace() << "TracingGattClientSt::Read, Value Handle: " << infra::hex << characteristic.CharacteristicValueHandle();
        GattClientSt::Read(characteristic, onResponse);
    }

    void TracingGattClientSt::Write(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data, const infra::Function<void()>& onDone) const
    {
        tracer.Trace() << "TracingGattClientSt::Write, Value Handle: " << infra::hex << characteristic.CharacteristicValueHandle() << ", data: " << infra::AsHex(data);
        GattClientSt::Write(characteristic, data, onDone);
    }

    void TracingGattClientSt::WriteWithoutResponse(const services::GattClientCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data) const
    {
        tracer.Trace() << "TracingGattClientSt::WriteWithoutResponse, Value Handle: " << infra::hex << characteristic.CharacteristicValueHandle() << ", data: " << infra::AsHex(data);
        GattClientSt::WriteWithoutResponse(characteristic, data);
    }

    void TracingGattClientSt::EnableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const
    {
        tracer.Trace() << "TracingGattClientSt::EnableNotification, Handle: " << infra::hex << (characteristic.CharacteristicValueHandle() + 1);
        GattClientSt::EnableNotification(characteristic, onDone);
    }

    void TracingGattClientSt::DisableNotification(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const
    {
        tracer.Trace() << "TracingGattClientSt::DisableNotification, Handle: " << infra::hex << (characteristic.CharacteristicValueHandle() + 1);
        GattClientSt::DisableNotification(characteristic, onDone);
    }

    void TracingGattClientSt::EnableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const
    {
        tracer.Trace() << "TracingGattClientSt::EnableIndication, Handle: " << infra::hex << (characteristic.CharacteristicValueHandle() + 1);
        GattClientSt::EnableIndication(characteristic, onDone);
    }

    void TracingGattClientSt::DisableIndication(const services::GattClientCharacteristicOperationsObserver& characteristic, const infra::Function<void()>& onDone) const
    {
        tracer.Trace() << "TracingGattClientSt::DisableIndication, Handle: " << infra::hex << (characteristic.CharacteristicValueHandle() + 1);
        GattClientSt::DisableIndication(characteristic, onDone);
    }

    void TracingGattClientSt::HandleGattIndicationEvent(evt_blecore_aci* vendorEvent)
    {
        auto& gattIndicationEvent = *reinterpret_cast<aci_gatt_indication_event_rp0*>(vendorEvent->data);

        infra::ByteRange data(&gattIndicationEvent.Attribute_Value[0], &gattIndicationEvent.Attribute_Value[0] + gattIndicationEvent.Attribute_Value_Length);

        tracer.Trace() << "TracingGattClientSt::Indication received, handle: " << infra::hex << gattIndicationEvent.Attribute_Handle << ", data: " << infra::AsHex(data);

        GattClientSt::HandleGattIndicationEvent(vendorEvent);
    }

    void TracingGattClientSt::HandleGattNotificationEvent(evt_blecore_aci* vendorEvent)
    {
        auto& gattNotificationEvent = *reinterpret_cast<aci_gatt_notification_event_rp0*>(vendorEvent->data);

        infra::ByteRange data(&gattNotificationEvent.Attribute_Value[0], &gattNotificationEvent.Attribute_Value[0] + gattNotificationEvent.Attribute_Value_Length);

        tracer.Trace() << "TracingGattClientSt::Notification received, handle: " << infra::hex << gattNotificationEvent.Attribute_Handle << ", data: " << infra::AsHex(data);

        GattClientSt::HandleGattNotificationEvent(vendorEvent);
    }
}
