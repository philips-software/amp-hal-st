#ifndef SERVICES_ST_UTIL_FUS_FIRMWARE_UPGRADE_CONTROLLER_HPP
#define SERVICES_ST_UTIL_FUS_FIRMWARE_UPGRADE_CONTROLLER_HPP

#include "hal/interfaces/Flash.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Observer.hpp"
#include "infra/util/PolymorphicVariant.hpp"
#include "services/st_util/FusWirelessStackUpgrade.hpp"
#include "services/st_util/StBootloaderCommunicatorUart.hpp"
#include <cstdint>

namespace services
{
    class StBootloaderCommunicatorInitializer;

    class StBootloaderCommunicatorInitializerObserver
        : public infra::Observer<StBootloaderCommunicatorInitializerObserver, StBootloaderCommunicatorInitializer>
    {
    public:
        using infra::Observer<StBootloaderCommunicatorInitializerObserver, StBootloaderCommunicatorInitializer>::Observer;
        virtual void Initialized(hal::FusWirelessStackUpgrade& fus) = 0;
    };

    class StBootloaderCommunicatorInitializer
        : public infra::Subject<StBootloaderCommunicatorInitializerObserver>
    {
    public:
        virtual void Initialize() = 0;
    };

    class FusFirmwareUpgradeController
    {
    public:
        virtual void DeleteWirelessStack(infra::Function<void()> onDone) = 0;
        virtual void Upgrade(infra::Function<void()> onDone) = 0;
        virtual void StartWirelessStack(infra::Function<void()> onDone) = 0;
    };

    class FusFirmwareUpgradeControllerImpl
        : public FusFirmwareUpgradeController
        , public StBootloaderCommunicatorInitializerObserver
    {
    public:
        FusFirmwareUpgradeControllerImpl(StBootloaderCommunicatorInitializer& stBootloaderCommunicatorInitializer);

        // Implementation of FusFirmwareUpgradeController
        void DeleteWirelessStack(infra::Function<void()> onDone) override;

        void Upgrade(infra::Function<void()> onDone) override
        {
            // Not implemented in this example
        }

        void StartWirelessStack(infra::Function<void()> onDone) override
        {
            // Not implemented in this example
        }

        // Implementation of StBootloaderCommunicatorInitializerObserver
        void Initialized(hal::FusWirelessStackUpgrade& fus) override;

    private:
        void ActivateFus(hal::FusWirelessStackUpgrade::OnDone fusActivated);

    private:
        class State
        {
        public:
            virtual ~State() = default;

            virtual void Initialized() = 0;
        };

        class StateIdle
            : public State
        {
        public:
            explicit StateIdle(FusFirmwareUpgradeControllerImpl& controller);

            void Initialized() override;

        private:
            FusFirmwareUpgradeControllerImpl& controller;
        };

        class StateDeleteWirelessStack
            : public State
        {
        public:
            explicit StateDeleteWirelessStack(FusFirmwareUpgradeControllerImpl& controller);

            void Initialized() override;

        private:
            void WirelessStackDeleted();

        private:
            FusFirmwareUpgradeControllerImpl& controller;
        };

    private:
        infra::PolymorphicVariant<State, StateIdle, StateDeleteWirelessStack> state;
        hal::FusWirelessStackUpgrade* fus;
        infra::Function<void()> onDone;
        hal::FusWirelessStackUpgrade::OnDone fusActivated;
    };
}

#endif
