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

    void TracingGattClientSt::Read(services::AttAttribute::Handle handle, const infra::Function<void(const infra::ConstByteRange&)>& onResponse, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::Read, Value Handle: " << infra::hex << handle;
        GattClientSt::Read(handle, onResponse, onDone);
    }

    void TracingGattClientSt::Write(services::AttAttribute::Handle handle, infra::ConstByteRange data, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::Write, Value Handle: " << infra::hex << handle << ", data: " << infra::AsHex(data);
        GattClientSt::Write(handle, data, onDone);
    }

    void TracingGattClientSt::WriteWithoutResponse(services::AttAttribute::Handle handle, infra::ConstByteRange data, const infra::Function<void(services::OperationStatus)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::WriteWithoutResponse, Value Handle: " << infra::hex << handle << ", data: " << infra::AsHex(data);
        GattClientSt::WriteWithoutResponse(handle, data, onDone);
    }

    void TracingGattClientSt::EnableNotification(services::AttAttribute::Handle handle, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::EnableNotification, Handle: " << infra::hex << (handle + 1);
        GattClientSt::EnableNotification(handle, onDone);
    }

    void TracingGattClientSt::DisableNotification(services::AttAttribute::Handle handle, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::DisableNotification, Handle: " << infra::hex << (handle + 1);
        GattClientSt::DisableNotification(handle, onDone);
    }

    void TracingGattClientSt::EnableIndication(services::AttAttribute::Handle handle, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::EnableIndication, Handle: " << infra::hex << (handle + 1);
        GattClientSt::EnableIndication(handle, onDone);
    }

    void TracingGattClientSt::DisableIndication(services::AttAttribute::Handle handle, const infra::Function<void(uint8_t)>& onDone)
    {
        tracer.Trace() << "TracingGattClientSt::DisableIndication, Handle: " << infra::hex << (handle + 1);
        GattClientSt::DisableIndication(handle, onDone);
    }

    void TracingGattClientSt::HandleGattIndicationEvent(const aci_gatt_indication_event_rp0& event)
    {
        infra::ConstByteRange data(&event.Attribute_Value[0], &event.Attribute_Value[0] + event.Attribute_Value_Length);

        tracer.Trace() << "TracingGattClientSt::Indication received, handle: " << infra::hex << event.Attribute_Handle << ", data: " << infra::AsHex(data);

        GattClientSt::HandleGattIndicationEvent(event);
    }

    void TracingGattClientSt::HandleGattNotificationEvent(const aci_gatt_notification_event_rp0& event)
    {
        infra::ConstByteRange data(&event.Attribute_Value[0], &event.Attribute_Value[0] + event.Attribute_Value_Length);

        tracer.Trace() << "TracingGattClientSt::Notification received, handle: " << infra::hex << event.Attribute_Handle << ", data: " << infra::AsHex(data);

        GattClientSt::HandleGattNotificationEvent(event);
    }

    void TracingGattClientSt::HandleGattCompleteResponse(const aci_gatt_proc_complete_event_rp0& event)
    {
        tracer.Trace() << "TracingGattClientSt::GATT complete response, handle: 0x" << infra::hex << event.Connection_Handle << ", status: 0x" << event.Error_Code;

        GattClientSt::HandleGattCompleteResponse(event);
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
