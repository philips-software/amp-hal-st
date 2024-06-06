#ifndef HAL_ST_INTEGRATION_TEST_FLASH_HPP
#define HAL_ST_INTEGRATION_TEST_FLASH_HPP

#include "generated/echo/Flash.pb.hpp"
#include "hal/interfaces/FlashHomogeneous.hpp"
#include "infra/util/AutoResetFunction.hpp"

namespace application
{
    class Flash
        : public flash::Flash
    {
    public:
        Flash(services::Echo& echo, hal::Flash& flash);

        // Implementation of flash::Flash
        void Read(uint32_t address, uint32_t size) override;
        void Write(uint32_t address, infra::ConstByteRange contents) override;
        void EraseSectors(uint32_t sector, uint32_t numberOfSectors) override;

    private:
        flash::FlashResultProxy flashResult;
        hal::Flash& flash;

        std::array<uint8_t, flash::WriteRequest::contentsSize> buffer;
    };

    class FlashProxy
        : public hal::Flash
        , private flash::FlashResult
    {
    public:
        FlashProxy(services::Echo& echo);

        // Implementation of hal::Flash
        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;

    private:
        // Implementation of flash::FlashResult
        void ReadDone(infra::ConstByteRange contents) override;
        void WriteDone() override;
        void EraseSectorsDone() override;

        void ReadPartialBuffer(uint32_t address, uint32_t start);

    private:
        flash::FlashProxy proxy;
        infra::AutoResetFunction<void()> onDone;
        infra::ConstByteRange writingBuffer;
        infra::ByteRange readingBuffer;
        uint32_t transferBuffers = 0;
        uint32_t start;
        uint32_t endIndex;
    };

    class FlashHomogeneousProxy
        : public FlashProxy
    {
    public:
        FlashHomogeneousProxy(services::Echo& echo, uint32_t numberOfSectors, uint32_t sizeOfEachSector);

        uint32_t NumberOfSectors() const override;
        uint32_t SizeOfSector(uint32_t sectorIndex) const override;
        uint32_t SectorOfAddress(uint32_t address) const override;
        uint32_t AddressOfSector(uint32_t sectorIndex) const override;

    private:
        uint32_t numberOfSectors;
        uint32_t sizeOfEachSector;
    };
}

#endif
