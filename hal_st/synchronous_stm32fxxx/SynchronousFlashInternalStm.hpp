#ifndef SYNCHRONOUS_HAL_SYNCHRONOUS_FLASH_INTERNAL_STM_HPP
#define SYNCHRONOUS_HAL_SYNCHRONOUS_FLASH_INTERNAL_STM_HPP

#include "hal/synchronous_interfaces/SynchronousFlash.hpp"
#include "infra/util/ByteRange.hpp"
#include "cmsis_device.h"

namespace hal
{
    class SynchronousFlashInternalStm
        : public hal::SynchronousFlash
    {
    public:
        SynchronousFlashInternalStm(infra::MemoryRange<uint32_t> sectorSizes, infra::ConstByteRange flashMemory);

        uint32_t NumberOfSectors() const override;
        uint32_t SizeOfSector(uint32_t sectorIndex) const override;
        uint32_t SectorOfAddress(uint32_t address) const override;
        uint32_t AddressOfSector(uint32_t sectorIndex) const override;

        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex) override;

    private:
        infra::MemoryRange<uint32_t> sectorSizes;
        infra::ConstByteRange flashMemory;
    };
}

#endif
