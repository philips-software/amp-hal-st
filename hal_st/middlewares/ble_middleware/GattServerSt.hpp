#ifndef HAL_ST_GATT_SERVER_ST_HPP
#define HAL_ST_GATT_SERVER_ST_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "services/ble/Gatt.hpp"

namespace hal
{
    class GattServerSt
        : public services::GattServer
        , public services::GattCharacteristicClientOperations
        , public hal::HciEventSink
    {
    public:
        explicit GattServerSt(hal::HciEventSource& hciEventSource);

        // Implementation of services::GattServer
        virtual void AddService(services::GattService& service);

        // Implementation of services::GattCharacteristicClientOperations
        virtual UpdateStatus Update(const services::GattCharacteristicClientOperationsObserver& characteristic, infra::ConstByteRange data) const;

        // Implementation of hal::HciEventSink
        virtual void HciEvent(hci_event_pckt& event);

    protected:
        virtual void AddCharacteristic(services::GattCharacteristic& characteristic);
        virtual void HandleGattAttributeModified(aci_gatt_attribute_modified_event_rp0& event);
        virtual void ReportError(tBleStatus status) const;

    private:
        infra::IntrusiveForwardList<services::GattService> services;
    };
}

#endif
