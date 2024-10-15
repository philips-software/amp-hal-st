#include "services/st_util/FlashOnStBootloaderCommunicator.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/MemoryRange.hpp"
#include "services/st_util/StBootloaderCommunicator.hpp"
#include <cstdint>

namespace services
{
    FlashOnStBootloaderCommunicatorBase::FlashOnStBootloaderCommunicatorBase(hal::Flash& flash, StBootloaderCommunicator& communicator)
        : communicator(communicator)
        , flash(flash)
    {}

    void FlashOnStBootloaderCommunicatorBase::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        if (buffer.empty())
            onDone();
        else
        {
            this->onDone = onDone;
            const auto head = infra::Head(buffer, 256);
            writeTail = infra::DiscardHead(buffer, 256);
            tailAddress = address + head.size();
            communicator.WriteMemory(address, head, [this]()
                {
                    WriteBuffer(this->writeTail, this->tailAddress, this->onDone.Clone());
                });
        }
    }

    void FlashOnStBootloaderCommunicatorBase::ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        if (buffer.empty())
            onDone();
        else
        {
            this->onDone = onDone;
            auto head = infra::Head(buffer, 256);
            readTail = infra::DiscardHead(buffer, 256);
            tailAddress = address + head.size();
            communicator.ReadMemory(address, head, [this]()
                {
                    ReadBuffer(this->readTail, this->tailAddress, this->onDone.Clone());
                });
        }
    }

    void FlashOnStBootloaderCommunicatorBase::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        if (beginIndex == endIndex)
            onDone();
        else if (beginIndex == 0 && endIndex == flash.NumberOfSectors())
            communicator.ExtendedMassErase(MassEraseSubcommand::global, onDone);
        else
        {
            this->onDone = onDone;
            this->beginIndex = beginIndex;
            this->endIndex = endIndex;
            auto index = static_cast<uint16_t>(beginIndex);;
            this->page = index;
            communicator.ExtendedErase(infra::MakeRangeFromSingleObject(page), [this]()
                {
                    EraseSectors(this->beginIndex + 1, this->endIndex, this->onDone.Clone());
                });
        }
    }
}
