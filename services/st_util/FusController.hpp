#ifndef SERVICES_ST_UTIL_FUS_FIRMWARE_UPGRADE_CONTROLLER_HPP
#define SERVICES_ST_UTIL_FUS_FIRMWARE_UPGRADE_CONTROLLER_HPP

#include "infra/util/Observer.hpp"
#include "infra/util/PolymorphicVariant.hpp"
#include "services/st_util/FirmwareUpgradeService.hpp"
#include "services/st_util/StBootloaderCommunicator.hpp"

namespace services
{
    class StBootloaderCommunicatorInitializer;

    class StBootloaderCommunicatorInitializerObserver
        : public infra::Observer<StBootloaderCommunicatorInitializerObserver, StBootloaderCommunicatorInitializer>
    {
    public:
        using infra::Observer<StBootloaderCommunicatorInitializerObserver, StBootloaderCommunicatorInitializer>::Observer;
        virtual void Initialized(hal::FirmwareUpgradeService& fus) = 0;
    };

    class StBootloaderCommunicatorInitializer
        : public infra::Subject<StBootloaderCommunicatorInitializerObserver>
    {
    public:
        virtual void Initialize() = 0;
    };

    class FusController
    {
    public:
        virtual void Upgrade(const infra::Function<void()>& onDone) = 0;
        virtual void StartWirelessStack(const infra::Function<void()>& onDone) = 0;
        virtual void DeleteWirelessStack(const infra::Function<void()>& onDone) = 0;
    };

    class FusControllerImpl
        : public FusController
        , public StBootloaderCommunicatorInitializerObserver
    {
    public:
        FusControllerImpl(StBootloaderCommunicatorInitializer& stBootloaderCommunicatorInitializer);

        // Implementation of FusController
        void Upgrade(const infra::Function<void()>& onDone) override;
        void StartWirelessStack(const infra::Function<void()>& onDone) override;
        void DeleteWirelessStack(const infra::Function<void()>& onDone) override;

        // Implementation of StBootloaderCommunicatorInitializerObserver
        void Initialized(hal::FirmwareUpgradeService& fus) override;

    private:
        void ActivateFus(hal::FirmwareUpgradeService::OnDone fusActivated);

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
            explicit StateIdle(FusControllerImpl& controller);

            void Initialized() override;

        private:
            FusControllerImpl& controller;
        };

        class StateUpgradeWirelessStack
            : public State
        {
        public:
            explicit StateUpgradeWirelessStack(FusControllerImpl& controller);

            void Initialized() override;

        private:
            void WirelessStackUpgraded();

        private:
            FusControllerImpl& controller;
        };

        class StateStartWirelessStack
            : public State
        {
        public:
            explicit StateStartWirelessStack(FusControllerImpl& controller);

            void Initialized() override;

        private:
            void WirelessStackStarted();

        private:
            FusControllerImpl& controller;
        };

        class StateDeleteWirelessStack
            : public State
        {
        public:
            explicit StateDeleteWirelessStack(FusControllerImpl& controller);

            void Initialized() override;

        private:
            void WirelessStackDeleted();

        private:
            FusControllerImpl& controller;
        };

    private:
        infra::PolymorphicVariant<State, StateIdle, StateUpgradeWirelessStack, StateStartWirelessStack, StateDeleteWirelessStack> state;
        hal::FirmwareUpgradeService* fus;
        infra::Function<void()> onDone;
        hal::FirmwareUpgradeService::OnDone fusActivated;
    };
}

#endif
