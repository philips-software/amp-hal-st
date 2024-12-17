#include "services/st_util/FusOnStBootloaderCommunicator.hpp"

namespace services
{
    FusOnStBootloaderCommunicator::FusOnStBootloaderCommunicator(StBootloaderCommunicator& communicator)
        : communicator(communicator)
    {}

    hal::FusWirelessStackUpgrade::StateWithErrorCode FusOnStBootloaderCommunicator::GetFusState() const
    {
        return { hal::FusWirelessStackUpgrade::State::idle, hal::FusWirelessStackUpgrade::ErrorCode::noError };
    }

    void FusOnStBootloaderCommunicator::FirmwareUpgrade()
    {
    }

    void FusOnStBootloaderCommunicator::StartWirelessStack()
    {
    }

    void FusOnStBootloaderCommunicator::DeleteWirelessStack()
    {
    }
}
