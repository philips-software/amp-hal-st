#include "hal_st/middlewares/ble_middleware/TracingGattServerSt.hpp"
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
    TracingGattServerSt::TracingGattServerSt(hal::HciEventSource& hciEventSource, services::Tracer& tracer)
        : GattServerSt(hciEventSource)
        , tracer(tracer)
    {
        tracer.Trace() << "GattServerSt::GattServerSt()";
    }

    void TracingGattServerSt::AddService(services::GattServerService& service)
    {
        GattServerSt::AddService(service);
        tracer.Trace() << "GattServerSt::AddService [" << service.Handle() << "] " << service.Type();
    }

    services::GattServerCharacteristicOperations::UpdateStatus TracingGattServerSt::Update(const services::GattServerCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data) const
    {
        tracer.Trace() << "GattServerSt::Update [" << characteristic.CharacteristicHandle() << "] 0x" << infra::AsHex(data);
        return GattServerSt::Update(characteristic, data);
    }

    void TracingGattServerSt::AddCharacteristic(services::GattServerCharacteristic& characteristic)
    {
        GattServerSt::AddCharacteristic(characteristic);
        tracer.Trace() << "GattServerSt::AddCharacteristic [" << characteristic.Handle() << "] " << characteristic.Type();
    }

    void TracingGattServerSt::HandleGattAttributeModified(aci_gatt_attribute_modified_event_rp0& event)
    {
        infra::ByteRange data{ event.Attr_Data, event.Attr_Data + event.Attr_Data_Length };
        tracer.Trace() << "GattServerSt::HandleGattAttributeModified [" << event.Attr_Handle << "] L:" << event.Attr_Data_Length << " 0x" << infra::AsHex(data);

        GattServerSt::HandleGattAttributeModified(event);
    }

    void TracingGattServerSt::ReportError(tBleStatus status) const
    {
        tracer.Trace() << "GattServerSt::ReportError " << status;
    }
}
