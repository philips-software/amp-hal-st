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
            const auto head = infra::Head(buffer, 255);
            writeTail = infra::DiscardHead(buffer, 255);
            tailAddress = address + head.size();
            communicator.WriteMemory(address, head, [this]()
                {
                    WriteBuffer(writeTail, tailAddress, std::exchange(this->onDone, nullptr));
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
            auto head = infra::Head(buffer, 255);
            readTail = infra::DiscardHead(buffer, 255);
            tailAddress = address + head.size();
            communicator.ReadMemory(address, head, [this]()
                {
                    ReadBuffer(readTail, tailAddress, std::exchange(this->onDone, nullptr));
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
            this->page[1] = static_cast<uint8_t>(beginIndex);
            communicator.ExtendedErase(infra::MakeConstByteRange(page), [this]()
                {
                    EraseSectors(this->beginIndex + 1, this->endIndex, std::exchange(this->onDone, nullptr));
                });
        }
    }
}
