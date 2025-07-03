#include "services/st_util/FusOnStBootloaderCommunicator.hpp"
#include "infra/util/Optional.hpp"

namespace
{
    constexpr uint8_t deleteWirelessStack = 0x52;
    constexpr uint8_t firmwareUpgrade = 0x53;
    constexpr uint8_t getFusState = 0x54;
    constexpr uint8_t startWirelessStack = 0x5A;

    auto emptyPacket = infra::ConstByteRange();
}

namespace services
{
    FusOnStBootloaderCommunicator::FusOnStBootloaderCommunicator(StBootloaderCommunicator& communicator)
        : communicator(communicator)
    {}

    void FusOnStBootloaderCommunicator::GetFusState(hal::FusWirelessStackUpgrade::OnDone onDone)
    {
        this->onDone = onDone;
        communicator.Special(getFusState, emptyPacket, rxData, status, [this]()
            {
                hal::FusWirelessStackUpgrade::StateWithErrorCode stateWithErrorCode;
                rxData.pop_front();
                stateWithErrorCode.state = static_cast<hal::FusWirelessStackUpgrade::State>(rxData.front());
                rxData.pop_front();
                stateWithErrorCode.errorCode = static_cast<hal::FusWirelessStackUpgrade::ErrorCode>(rxData.front());
                this->onDone(infra::MakeOptional(stateWithErrorCode));
            });
    }

    void FusOnStBootloaderCommunicator::FirmwareUpgrade(hal::FusWirelessStackUpgrade::OnDone onDone)
    {
        this->onDone = onDone;
        communicator.ExtendedSpecial(firmwareUpgrade, emptyPacket, emptyPacket, status, [this]()
            {
                ProcessReturnedStatus(status);
            });
    }

    void FusOnStBootloaderCommunicator::StartWirelessStack(hal::FusWirelessStackUpgrade::OnDone onDone)
    {
        this->onDone = onDone;
        communicator.ExtendedSpecial(startWirelessStack, emptyPacket, emptyPacket, status, [this]()
            {
                ProcessReturnedStatus(status);
            });
    }

    void FusOnStBootloaderCommunicator::DeleteWirelessStack(hal::FusWirelessStackUpgrade::OnDone onDone)
    {
        this->onDone = onDone;
        communicator.ExtendedSpecial(deleteWirelessStack, emptyPacket, emptyPacket, status, [this]()
            {
                ProcessReturnedStatus(status);
            });
    }

    void FusOnStBootloaderCommunicator::ProcessReturnedStatus(infra::ByteRange rxData)
    {
        infra::Optional<hal::FusWirelessStackUpgrade::StateWithErrorCode> stateWithErrorCode = infra::none;

        auto error = rxData.front();
        if (error)
        {
            rxData.pop_front();
            auto state = static_cast<hal::FusWirelessStackUpgrade::State>(rxData.front());
            rxData.pop_front();
            auto errorCode = static_cast<hal::FusWirelessStackUpgrade::ErrorCode>(rxData.front());
            stateWithErrorCode.Emplace(state, errorCode);
        }

        this->onDone(stateWithErrorCode);
    }
}
