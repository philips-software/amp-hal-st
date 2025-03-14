#ifndef HAL_FLASH_INTERNAL_STM_HPP
#define HAL_FLASH_INTERNAL_STM_HPP

#include DEVICE_HEADER
#include "hal/interfaces/Flash.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/Function.hpp"

namespace hal
{
    class FlashInternalStmBase
        : public hal::Flash
    {
    public:
        FlashInternalStmBase(infra::ConstByteRange flashMemory);

        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;

    private:
        infra::ConstByteRange flashMemory;
    };

    class FlashInternalStm
        : public FlashInternalStmBase
    {
    public:
        FlashInternalStm(infra::MemoryRange<const uint32_t> sectorSizes, infra::ConstByteRange flashMemory);

        uint32_t NumberOfSectors() const override;
        uint32_t SizeOfSector(uint32_t sectorIndex) const override;
        uint32_t SectorOfAddress(uint32_t address) const override;
        uint32_t AddressOfSector(uint32_t sectorIndex) const override;

    private:
        infra::MemoryRange<const uint32_t> sectorSizes;
    };

    class FlashHomogeneousInternalStm
        : public FlashInternalStmBase
    {
    public:
        FlashHomogeneousInternalStm(uint32_t numberOfSectors, uint32_t sizeOfEachSector, infra::ConstByteRange flashMemory);

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
