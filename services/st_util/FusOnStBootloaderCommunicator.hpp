#ifndef SERVICES_ST_UTIL_FUS_ON_ST_BOOTLOADER_COMMUNICATOR_HPP
#define SERVICES_ST_UTIL_FUS_ON_ST_BOOTLOADER_COMMUNICATOR_HPP

#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "services/st_util/FusWirelessStackUpgrade.hpp"
#include "services/st_util/StBootloaderCommunicator.hpp"
#include <array>
#include <cstdint>

namespace services
{
    class FusOnStBootloaderCommunicator
        : public hal::FusWirelessStackUpgrade
    {
    public:
        explicit FusOnStBootloaderCommunicator(StBootloaderCommunicator& communicator);

        // Implementation of FusWirelessStackUpgrade
        void GetFusState(hal::FusWirelessStackUpgrade::OnDone onDone) override;
        void FirmwareUpgrade(hal::FusWirelessStackUpgrade::OnDone onDone) override;
        void StartWirelessStack(hal::FusWirelessStackUpgrade::OnDone onDone) override;
        void DeleteWirelessStack(hal::FusWirelessStackUpgrade::OnDone onDone) override;

    private:
        void ProcessReturnedStatus(infra::ByteRange rxData);

    private:
        StBootloaderCommunicator& communicator;
        std::array<uint8_t, 3> rxDataContainer;
        infra::ByteRange rxData{ rxDataContainer };
        std::array<uint8_t, 3> statusContainer;
        infra::ByteRange status{ statusContainer };
        infra::Function<void(infra::Optional<StateWithErrorCode> stateWithErrorCode)> onDone;
    };
}

#endif
