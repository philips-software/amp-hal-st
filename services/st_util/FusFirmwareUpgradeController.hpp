#ifndef SERVICES_ST_UTIL_FUS_FIRMWARE_UPGRADE_CONTROLLER_HPP
#define SERVICES_ST_UTIL_FUS_FIRMWARE_UPGRADE_CONTROLLER_HPP

#include "hal/interfaces/Flash.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Observer.hpp"
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
        virtual void Initialized() = 0;
    };

    class StBootloaderCommunicatorInitializer
        : public infra::Subject<StBootloaderCommunicatorInitializerObserver>
    {
    public:
        virtual void Initialize() = 0;
    };

    class FusFirmwareUpgradeController
        : public StBootloaderCommunicatorInitializerObserver
    {
    public:
        FusFirmwareUpgradeController(hal::FusWirelessStackUpgrade& fus, StBootloaderCommunicatorInitializer& stBootloaderCommunicatorInitializer);

        void DeleteWirelessStack(infra::Function<void()> onDone);

        // Implementation of StBootloaderCommunicatorInitializerObserver
        void Initialized() override;

    private:
        void ActivateFus(hal::FusWirelessStackUpgrade::OnDone fusActivated);

    private:
        hal::FusWirelessStackUpgrade& fus;
        infra::Function<void()> onDone;
        hal::FusWirelessStackUpgrade::OnDone fusActivated;
    };
}

#endif
