#ifndef SERVICES_ST_UTIL_FUS_ON_ST_BOOTLOADER_COMMUNICATOR_HPP
#define SERVICES_ST_UTIL_FUS_ON_ST_BOOTLOADER_COMMUNICATOR_HPP

#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"
#include "services/st_util/FirmwareUpgradeService.hpp"
#include "services/st_util/StBootloaderCommunicator.hpp"
#include <array>
#include <cstdint>

namespace services
{
    class FusOnStBootloaderCommunicator
        : public hal::FirmwareUpgradeService
    {
    public:
        explicit FusOnStBootloaderCommunicator(StBootloaderCommunicator& communicator);

        // Implementation of FirmwareUpgradeService
        void GetState(hal::FirmwareUpgradeService::OnDone onDone) override;
        void Upgrade(hal::FirmwareUpgradeService::OnDone onDone) override;
        void StartWirelessStack(hal::FirmwareUpgradeService::OnDone onDone) override;
        void DeleteWirelessStack(hal::FirmwareUpgradeService::OnDone onDone) override;

    private:
        hal::FirmwareUpgradeService::StateWithErrorCode ParseReturnedPacket(infra::ByteRange packet);
        void OnExtendedSpecialDone(infra::ByteRange range);

    private:
        StBootloaderCommunicator& communicator;
        std::array<uint8_t, 3> rxDataContainer;
        infra::ByteRange rxData;
        std::array<uint8_t, 3> statusContainer;
        infra::ByteRange status;
        infra::Function<void(infra::Optional<StateWithErrorCode> stateWithErrorCode)> onDone;
    };
}

#endif
