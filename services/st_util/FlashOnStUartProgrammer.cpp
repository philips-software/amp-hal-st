#include "services/st_util/FlashOnStUartProgrammer.hpp"

namespace services
{
    FlashOnStUartProgrammer::FlashOnStUartProgrammer(infra::MemoryRange<uint32_t> sectorSizes, StUartProgrammer& programmer)
        : sectorSizes(sectorSizes)
        , programmer(programmer)
    {}

    uint32_t FlashOnStUartProgrammer::NumberOfSectors() const
    {
        return sectorSizes.size();
    }

    uint32_t FlashOnStUartProgrammer::FlashOnStUartProgrammer::SizeOfSector(uint32_t sectorIndex) const
    {
        return sectorSizes[sectorIndex];
    }

    uint32_t FlashOnStUartProgrammer::SectorOfAddress(uint32_t address) const
    {
        uint32_t totalSize = 0;
        for (uint32_t sector = 0; sector != sectorSizes.size(); ++sector)
        {
            totalSize += sectorSizes[sector];
            if (address < totalSize)
                return sector;
        }

        assert(address == totalSize);
        return sectorSizes.size();
    }

    uint32_t FlashOnStUartProgrammer::AddressOfSector(uint32_t sectorIndex) const
    {
        uint32_t address = 0;
        for (uint32_t sector = 0; sector != sectorIndex; ++sector)
            address += sectorSizes[sector];
        return address;
    }

    void FlashOnStUartProgrammer::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        if (buffer.empty())
            onDone();
        else
        {
            this->onDone = onDone;
            auto head = infra::Head(buffer, 256);
            writeTail = infra::DiscardHead(buffer, 256);
            tailAddress = address + head.size();
            programmer.WriteMemory(address + 0x8000000, head, [this]()
                {
                    WriteBuffer(writeTail, tailAddress, std::exchange(this->onDone, nullptr));
                });
        }
    }

    void FlashOnStUartProgrammer::ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        if (buffer.empty())
            onDone();
        else
        {
            this->onDone = onDone;
            auto head = infra::Head(buffer, 256);
            readTail = infra::DiscardHead(buffer, 256);
            tailAddress = address + head.size();
            programmer.ReadMemory(address + 0x8000000, head, [this]()
                {
                    ReadBuffer(readTail, tailAddress, std::exchange(this->onDone, nullptr));
                });
        }
    }

    void FlashOnStUartProgrammer::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        if (beginIndex == endIndex)
            onDone();
        if (beginIndex == 0 && endIndex == NumberOfSectors())
            programmer.EraseAll(onDone);
        else
        {
            this->onDone = onDone;
            this->beginIndex = beginIndex;
            this->endIndex = endIndex;
            programmer.ErasePage(beginIndex, [this]()
                {
                    EraseSectors(this->beginIndex + 1, this->endIndex, std::exchange(this->onDone, nullptr));
                });
        }
    }
}
