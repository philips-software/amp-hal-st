#include "hal_st/middlewares/ble_middleware/TracingGattServerSt.hpp"

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

    void TracingGattServerSt::HandleAttExchangeMtuResponseEvent(evt_blecore_aci* vendorEvent)
    {
        const auto mtuExchangeEvent = reinterpret_cast<aci_att_exchange_mtu_resp_event_rp0*>(vendorEvent->data);
        tracer.Trace() << "GattServerSt::HandleAttExchangeMtuResponseEvent Server_RX_MTU = " << mtuExchangeEvent->Server_RX_MTU;
        GattServerSt::HandleAttExchangeMtuResponseEvent(vendorEvent);
    }

    void TracingGattServerSt::HandleGattAttributeModifiedResponseEvent(evt_blecore_aci* vendorEvent)
    {
        auto attributeModifiedEvent = *reinterpret_cast<aci_gatt_attribute_modified_event_rp0*>(vendorEvent->data);

        infra::ByteRange data{ attributeModifiedEvent.Attr_Data, attributeModifiedEvent.Attr_Data + attributeModifiedEvent.Attr_Data_Length };
        tracer.Trace() << "GattServerSt::HandleGattAttributeModified [" << attributeModifiedEvent.Attr_Handle << "] L:" << attributeModifiedEvent.Attr_Data_Length << " 0x" << infra::AsHex(data);

        GattServerSt::HandleGattAttributeModifiedResponseEvent(vendorEvent);
    }

    void TracingGattServerSt::ReportError(tBleStatus status) const
    {
        tracer.Trace() << "GattServerSt::ReportError " << status;
    }
}
