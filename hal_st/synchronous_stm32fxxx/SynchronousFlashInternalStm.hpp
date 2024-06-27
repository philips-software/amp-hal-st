#ifndef SYNCHRONOUS_HAL_SYNCHRONOUS_FLASH_INTERNAL_STM_HPP
#define SYNCHRONOUS_HAL_SYNCHRONOUS_FLASH_INTERNAL_STM_HPP

#include DEVICE_HEADER
#include "hal/synchronous_interfaces/SynchronousFlash.hpp"
#include "infra/util/ByteRange.hpp"

namespace hal
{
    class SynchronousFlashInternalStmBase
        : public hal::SynchronousFlash
    {
    public:
        SynchronousFlashInternalStmBase(infra::ConstByteRange flashMemory);

        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex) override;

    private:
        template<typename alignment, uint32_t flashType>
        void AlignedWriteBuffer(infra::ConstByteRange buffer, uint32_t address);
#ifdef STM32WBA
        void AlignedWriteBuffer(infra::ConstByteRange buffer, uint32_t address);
#endif

    private:
        infra::ConstByteRange flashMemory;
    };

    class SynchronousFlashInternalStm
        : public SynchronousFlashInternalStmBase
    {
    public:
        SynchronousFlashInternalStm(infra::MemoryRange<uint32_t> sectorSizes, infra::ConstByteRange flashMemory);

        uint32_t NumberOfSectors() const override;
        uint32_t SizeOfSector(uint32_t sectorIndex) const override;
        uint32_t SectorOfAddress(uint32_t address) const override;
        uint32_t AddressOfSector(uint32_t sectorIndex) const override;

    private:
        infra::MemoryRange<uint32_t> sectorSizes;
    };

    class SynchronousFlashHomogeneousInternalStm
        : public SynchronousFlashInternalStmBase
    {
    public:
        SynchronousFlashHomogeneousInternalStm(uint32_t numberOfSectors, uint32_t sizeOfEachSector, infra::ConstByteRange flashMemory);

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
