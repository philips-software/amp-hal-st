#ifndef HAL_ST_SYSTEM_TRANSPORT_LAYER_WB_HPP
#define HAL_ST_SYSTEM_TRANSPORT_LAYER_WB_HPP

#include "ble/ble.h"
#include "hal_st/middlewares/ble_middleware/HciEventObserver.hpp"
#include "infra/util/Function.hpp"
#include "infra/util/InterfaceConnector.hpp"
#include "infra/util/ProxyCreator.hpp"
#include "services/ble/BondBlobPersistence.hpp"
#include "services/ble/BondStorageSynchronizer.hpp"

namespace hal
{
    class SystemTransportLayerWb
        : public infra::InterfaceConnector<SystemTransportLayerWb>
        , public HciEventSource
    {
    public:
        enum class RfWakeupClock : uint8_t
        {
            highSpeedExternal,
            lowSpeedExternal,
        };

        struct Configuration
        {
            uint16_t attMtuSize;
            RfWakeupClock rfWakeupClock;
        };

        struct Version
        {
            uint8_t firmwareMajor;
            uint8_t firmwareMinor;
            uint8_t firmwareSub;
            uint8_t firmwareBranch;
            uint8_t firmwareReleaseType;

            uint8_t fusMajor;
            uint8_t fusMinor;
            uint8_t fusSub;
        };

    public:
        using BondStorageSynchronizerCreator = infra::CreatorBase<services::BondStorageSynchronizer, void()>;

        SystemTransportLayerWb(services::ConfigurationStoreAccess<infra::ByteRange> flashStorage, BondStorageSynchronizerCreator& bondStorageSynchronizerCreator, Configuration configuration, const infra::Function<void(services::BondStorageSynchronizer&)>& onInitialized);

        Version GetVersion() const;

        // Implementation of HciEventSource
        void HciEventHandler(hci_event_pckt& event) override;

        virtual void UserEventHandler(void* payload);

    protected:
        virtual void HandleReadyEvent(void* payload);
        virtual void HandleErrorNotifyEvent(void* event);
        virtual void HandleBleNvmRamUpdateEvent(void* sysEvent);
        virtual void HandleUnknownEvent(void* event);

        virtual void HandleWirelessFwEvent(void* payload);
        virtual void HandleFusFwEvent(void* payload);
        virtual void HandleUnknwownReadyEvent(void* payload);

    private:
        void ShciInit();
        void HciInit();
        void MemoryChannelInit();

    private:
        services::BondBlobPersistence bondBlobPersistence;
        infra::DelayedProxyCreator<services::BondStorageSynchronizer, void()> bondStorageSynchronizerCreator;
        Configuration configuration;
        infra::AutoResetFunction<void(services::BondStorageSynchronizer&)> onInitialized;
    };
}

#endif
