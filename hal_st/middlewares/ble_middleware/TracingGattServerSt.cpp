#include "hal_st/middlewares/ble_middleware/TracingGattServerSt.hpp"
#include "infra/stream/StringOutputStream.hpp"

namespace
{
    infra::TextOutputStream& operator<<(infra::TextOutputStream& stream, const services::GattAttribute::Uuid& uuid)
    {
        if (uuid.Is<services::GattAttribute::Uuid16>())
            stream << "0x" << infra::hex << uuid.Get<services::GattAttribute::Uuid16>();
        else
            stream << "[" << infra::AsHex(infra::MakeByteRange(uuid.Get<services::GattAttribute::Uuid128>())) << "]";

        return stream;
    }
}

namespace hal
{
    TracingGattServerSt::TracingGattServerSt(hal::HciEventSource& hciEventSource, services::Tracer& tracer)
        : GattServerSt(hciEventSource)
        , tracer(tracer)
    {
        tracer.Trace() << "GattServerSt::GattServerSt()";
    }

    void TracingGattServerSt::AddService(services::GattService& service)
    {
        GattServerSt::AddService(service);
        tracer.Trace() << "GattServerSt::AddService [" << service.Handle() << "] " << service.Type();
    }

    services::GattCharacteristicClientOperations::UpdateStatus TracingGattServerSt::Update(const services::GattCharacteristicClientOperationsObserver& characteristic, infra::ConstByteRange data) const
    {
        tracer.Trace() << "GattServerSt::Update [" << characteristic.CharacteristicHandle() << "] " << infra::ByteRangeAsString(data);
        return GattServerSt::Update(characteristic, data);
    }

    void TracingGattServerSt::AddCharacteristic(services::GattCharacteristic& characteristic)
    {
        GattServerSt::AddCharacteristic(characteristic);
        tracer.Trace() << "GattServerSt::AddCharacteristic [" << characteristic.Handle() << "] " << characteristic.Type();
    }

    void TracingGattServerSt::HandleGattAttributeModified(aci_gatt_attribute_modified_event_rp0& event)
    {
        infra::ByteRange data{event.Attr_Data, event.Attr_Data + event.Attr_Data_Length};
        tracer.Trace() << "GattServerSt::HandleGattAttributeModified [" << event.Attr_Handle << "] L:" << infra::hex << event.Attr_Data_Length << " D:'" << infra::AsHex(data) << "'";

        GattServerSt::HandleGattAttributeModified(event);
    }

    void TracingGattServerSt::ReportError(tBleStatus status) const
    {
        tracer.Trace() << "GattServerSt::ReportError " << status;
    }
}
