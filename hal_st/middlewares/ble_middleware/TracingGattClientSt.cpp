#include "hal_st/middlewares/ble_middleware/TracingGattClientSt.hpp"

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

    void TracingGattClientSt::StartCharacteristicDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle)
    {
        tracer.Trace() << "TracingGattClientSt::StartCharacteristicDiscovery";
        GattClientSt::StartCharacteristicDiscovery(handle, endHandle);
    }

    void TracingGattClientSt::StartDescriptorDiscovery(services::AttAttribute::Handle handle, services::AttAttribute::Handle endHandle)
    {
        tracer.Trace() << "TracingGattClientSt::StartDescriptorDiscovery";
        GattClientSt::StartDescriptorDiscovery(handle, endHandle);
    }

    void TracingGattClientSt::Read(const services::GattClientObserver& characteristic, const infra::Function<void(const infra::ConstByteRange&)>& onResponse, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::Read, Value Handle: " << infra::hex << characteristic.CharacteristicValueHandle();
        GattClientSt::Read(characteristic, onResponse, onDone);
    }

    void TracingGattClientSt::Write(const services::GattClientObserver& characteristic, infra::ConstByteRange data, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::Write, Value Handle: " << infra::hex << characteristic.CharacteristicValueHandle() << ", data: " << infra::AsHex(data);
        GattClientSt::Write(characteristic, data, onDone);
    }

    void TracingGattClientSt::WriteWithoutResponse(const services::GattClientObserver& characteristic, infra::ConstByteRange data)
    {
        tracer.Trace() << "TracingGattClientSt::WriteWithoutResponse, Value Handle: " << infra::hex << characteristic.CharacteristicValueHandle() << ", data: " << infra::AsHex(data);
        GattClientSt::WriteWithoutResponse(characteristic, data);
    }

    void TracingGattClientSt::EnableNotification(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::EnableNotification, Handle: " << infra::hex << (characteristic.CharacteristicValueHandle() + 1);
        GattClientSt::EnableNotification(characteristic, onDone);
    }

    void TracingGattClientSt::DisableNotification(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::DisableNotification, Handle: " << infra::hex << (characteristic.CharacteristicValueHandle() + 1);
        GattClientSt::DisableNotification(characteristic, onDone);
    }

    void TracingGattClientSt::EnableIndication(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::EnableIndication, Handle: " << infra::hex << (characteristic.CharacteristicValueHandle() + 1);
        GattClientSt::EnableIndication(characteristic, onDone);
    }

    void TracingGattClientSt::DisableIndication(const services::GattClientObserver& characteristic, const infra::Function<void(uint8_t)>& onDone)
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

    void TracingGattClientSt::HandleGattCompleteResponse(evt_blecore_aci* vendorEvent)
    {
        auto gattProcedureEvent = *reinterpret_cast<aci_gatt_proc_complete_event_rp0*>(vendorEvent->data);

        tracer.Trace() << "TracingGattClientSt::GATT complete response, handle: 0x" << infra::hex << gattProcedureEvent.Connection_Handle << ", status: 0x" << gattProcedureEvent.Error_Code;

        GattClientSt::HandleGattCompleteResponse(vendorEvent);
    }

    void TracingGattClientSt::HandleServiceDiscovered(infra::DataInputStream& stream, bool isUuid16)
    {
        infra::ByteInputStream tracingStream(stream.PeekContiguousRange(0), infra::softFail);

        while (!tracingStream.Empty())
        {
            Atttributes attributes;

            tracingStream >> attributes.startHandle >> attributes.endHandle;

            HandleUuidFromDiscovery(tracingStream, isUuid16, attributes.type);

            really_assert(!tracingStream.Failed());

            tracer.Trace() << "TracingGattClientSt::Service discovered, type: " << attributes.type << ", startHandle: 0x" << infra::hex << attributes.startHandle << ", endHandle: 0x" << infra::hex << attributes.endHandle;
        }

        GattClientSt::HandleServiceDiscovered(stream, isUuid16);
    }
}
