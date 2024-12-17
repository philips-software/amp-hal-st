#ifndef SERVICES_ST_UTIL_FUS_ON_ST_BOOTLOADER_COMMUNICATOR_HPP
#define SERVICES_ST_UTIL_FUS_ON_ST_BOOTLOADER_COMMUNICATOR_HPP

#include "services/st_util/FusWirelessStackUpgrade.hpp"
#include "services/st_util/StBootloaderCommunicator.hpp"

namespace services
{
    class FusOnStBootloaderCommunicator
        : public hal::FusWirelessStackUpgrade
    {
    public:
        explicit FusOnStBootloaderCommunicator(StBootloaderCommunicator& communicator);

        // Implementation of FusWirelessStackUpgrade
        StateWithErrorCode GetFusState() const override;
        void FirmwareUpgrade() override;
        void StartWirelessStack() override;
        void DeleteWirelessStack() override;

    private:
        StBootloaderCommunicator& communicator;
    };
}

#endif
