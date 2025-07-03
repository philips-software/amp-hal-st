#include "services/st_util/FusOnStBootloaderCommunicator.hpp"

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
                auto stateWithErrorCode = ParseReturnedPacket(rxData);
                this->onDone(infra::MakeOptional(stateWithErrorCode));
            });
    }

    void FusOnStBootloaderCommunicator::FirmwareUpgrade(hal::FusWirelessStackUpgrade::OnDone onDone)
    {
        this->onDone = onDone;
        communicator.ExtendedSpecial(firmwareUpgrade, emptyPacket, emptyPacket, status, [this]()
            {
                OnExtendedSpecialDone(status);
            });
    }

    void FusOnStBootloaderCommunicator::StartWirelessStack(hal::FusWirelessStackUpgrade::OnDone onDone)
    {
        this->onDone = onDone;
        communicator.ExtendedSpecial(startWirelessStack, emptyPacket, emptyPacket, status, [this]()
            {
                OnExtendedSpecialDone(status);
            });
    }

    void FusOnStBootloaderCommunicator::DeleteWirelessStack(hal::FusWirelessStackUpgrade::OnDone onDone)
    {
        this->onDone = onDone;
        communicator.ExtendedSpecial(deleteWirelessStack, emptyPacket, emptyPacket, status, [this]()
            {
                OnExtendedSpecialDone(status);
            });
    }

    hal::FusWirelessStackUpgrade::StateWithErrorCode FusOnStBootloaderCommunicator::ParseReturnedPacket(infra::ByteRange packet)
    {
        hal::FusWirelessStackUpgrade::StateWithErrorCode stateWithErrorCode;

        packet.pop_front();
        stateWithErrorCode.state = static_cast<hal::FusWirelessStackUpgrade::State>(packet.front());
        packet.pop_front();
        stateWithErrorCode.errorCode = static_cast<hal::FusWirelessStackUpgrade::ErrorCode>(packet.front());

        return stateWithErrorCode;
    }

    void FusOnStBootloaderCommunicator::OnExtendedSpecialDone(infra::ByteRange range)
    {
        infra::Optional<hal::FusWirelessStackUpgrade::StateWithErrorCode> OptionalStateWithErrorCode = infra::none;

        auto error = range.front();
        if (error)
        {
            auto stateWithErrorCode = ParseReturnedPacket(range);
            OptionalStateWithErrorCode.Emplace(stateWithErrorCode.state, stateWithErrorCode.errorCode);
        }

        this->onDone(OptionalStateWithErrorCode);
    }
}
