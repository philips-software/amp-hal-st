#ifndef HAL_ST_GATT_SERVER_ST_HPP
#define HAL_ST_GATT_SERVER_ST_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "services/ble/GattServer.hpp"

namespace hal
{
    class GattServerSt
        : public services::AttMtuExchange
        , public services::GattServer
        , public services::GattServerCharacteristicOperations
        , public hal::HciEventSink
    {
    public:
        explicit GattServerSt(hal::HciEventSource& hciEventSource);

        // Implementation of AttMtuExchange
        uint16_t EffectiveMaxAttMtuSize() const override;
        void RequestMtuExchange() override;

        // Implementation of services::GattServer
        void AddService(services::GattServerService& service) override;

        // Implementation of services::GattCharacteristicClientOperations
        UpdateStatus Update(const services::GattServerCharacteristicOperationsObserver& characteristic, infra::ConstByteRange data) const override;

        // Implementation of hal::HciEventSink
        void HciEvent(hci_event_pckt& event) override;

    protected:
        virtual void AddCharacteristic(services::GattServerCharacteristic& characteristic);
        virtual void HandleAttExchangeMtuResponseEvent(evt_blecore_aci* vendorEvent);
        virtual void HandleGattAttributeModifiedResponseEvent(evt_blecore_aci* vendorEvent);
        virtual void ReportError(tBleStatus status) const;

    private:
        infra::IntrusiveForwardList<services::GattServerService> services;

        uint16_t maxAttMtu = defaultMaxAttMtuSize;
    };
}

#endif
