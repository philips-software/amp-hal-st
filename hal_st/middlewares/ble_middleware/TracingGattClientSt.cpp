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
}
