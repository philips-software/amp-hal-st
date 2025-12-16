#include "integration_test/logic/Flash.hpp"
#include <utility>

namespace application
{
    Flash::Flash(services::Echo& echo, hal::Flash& flash)
        : flash::Flash(echo)
        , flashResult(echo)
        , flash(flash)
    {}


    void Flash::Read(uint32_t address, uint32_t size)
    {
        flash.ReadBuffer(infra::Head(infra::MakeRange(buffer), size), address, [this, size]()
            {
                flashResult.RequestSend([this, size]()
                    {
                        flashResult.ReadDone(infra::Head(infra::MakeRange(buffer), size));
                        MethodDone();
                    });
            });
    }

    void Flash::Write(uint32_t address, infra::ConstByteRange contents)
    {
        flash.WriteBuffer(contents, address, [this]()
            {
                flashResult.RequestSend([this]()
                    {
                        flashResult.WriteDone();
                        MethodDone();
                    });
            });
    }

    void Flash::EraseSectors(uint32_t sector, uint32_t numberOfSectors)
    {
        flash.EraseSectors(sector, sector + numberOfSectors, [this]()
            {
                flashResult.RequestSend([this]()
                    {
                        flashResult.EraseSectorsDone();
                        MethodDone();
                    });
            });
    }

    FlashProxy::FlashProxy(services::Echo& echo, infra::MemoryRange<const uint32_t> sectorSizes)
        : flash::FlashResult(echo)
        , sectorSizes(sectorSizes)
        , proxy(echo)
    {}

    uint32_t FlashProxy::NumberOfSectors() const
    {
        return sectorSizes.size();
    }

    uint32_t FlashProxy::SizeOfSector(uint32_t sectorIndex) const
    {
        return sectorSizes[sectorIndex];
    }

    uint32_t FlashProxy::SectorOfAddress(uint32_t address) const
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

    uint32_t FlashProxy::AddressOfSector(uint32_t sectorIndex) const
    {
        uint32_t address = 0;
        for (uint32_t sector = 0; sector != sectorIndex; ++sector)
            address += sectorSizes[sector];
        return address;
    }

    void FlashProxy::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        writingBuffer = buffer;
        this->onDone = onDone;

        if (!writingBuffer.empty())
        {
            proxy.RequestSend([this, address]()
                {
                    ++transferBuffers;
                    auto size = std::min<std::size_t>(writingBuffer.size(), flash::WriteRequest::contentsSize);
                    proxy.Write(address, infra::Head(writingBuffer, size));
                    WriteBuffer(infra::DiscardHead(writingBuffer, size), address + size, this->onDone.Clone());
                });
        }
    }

    void FlashProxy::ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        readingBuffer = buffer;
        this->onDone = onDone;

        ReadPartialBuffer(address, 0);
    }

    void FlashProxy::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        this->onDone = onDone;
        this->endIndex = endIndex;

        proxy.RequestSend([this, beginIndex]()
            {
                proxy.EraseSectors(beginIndex, this->endIndex - beginIndex);
            });
    }

    void FlashProxy::ReadDone(infra::ConstByteRange contents)
    {
        infra::Copy(contents, infra::Head(readingBuffer, contents.size()));
        readingBuffer = infra::DiscardHead(readingBuffer, contents.size());

        --transferBuffers;
        if (transferBuffers == 0 && readingBuffer.empty())
            onDone();

        MethodDone();
    }

    void FlashProxy::WriteDone()
    {
        --transferBuffers;

        if (transferBuffers == 0 && writingBuffer.empty())
            onDone();

        MethodDone();
    }

    void FlashProxy::EraseSectorsDone()
    {
        onDone();
        MethodDone();
    }

    void FlashProxy::ReadPartialBuffer(uint32_t address, uint32_t start)
    {
        if (readingBuffer.size() - start > 0)
        {
            this->start = start;
            proxy.RequestSend([this, address]()
                {
                    ++transferBuffers;
                    auto size = std::min<uint32_t>(readingBuffer.size() - this->start, flash::WriteRequest::contentsSize);
                    proxy.Read(address + this->start, size);
                    ReadPartialBuffer(address, this->start + size);
                });
        }
    }

    FlashHomogeneousProxy::FlashHomogeneousProxy(services::Echo& echo, uint32_t numberOfSectors, uint32_t sizeOfEachSector)
        : FlashProxy(echo, {})
        , numberOfSectors(numberOfSectors)
        , sizeOfEachSector(sizeOfEachSector)
    {}

    uint32_t FlashHomogeneousProxy::NumberOfSectors() const
    {
        return numberOfSectors;
    }

    uint32_t FlashHomogeneousProxy::SizeOfSector(uint32_t sectorIndex) const
    {
        return sizeOfEachSector;
    }

    uint32_t FlashHomogeneousProxy::SectorOfAddress(uint32_t address) const
    {
        return address / sizeOfEachSector;
    }

    uint32_t FlashHomogeneousProxy::AddressOfSector(uint32_t sectorIndex) const
    {
        return sectorIndex * sizeOfEachSector;
    }
}
