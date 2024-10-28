// Copyright (c) 2024 Koninklijke Philips N.V.
#ifndef HAL_FLASH_INTERNAL_HIGH_CYCLE_AREA_STM_HPP
#define HAL_FLASH_INTERNAL_HIGH_CYCLE_AREA_STM_HPP

#include DEVICE_HEADER
#include "hal/interfaces/Flash.hpp"
#include "hal_st/cortex/InterruptCortex.hpp"
#include "infra/util/MemoryRange.hpp"
#include <cstdint>

namespace hal
{
    class FlashInternalHighCycleAreaStm
        : public Flash
    {
    public:
        using HalfWordRange = infra::MemoryRange<uint16_t>;
        using ConstHalfWordRange = infra::MemoryRange<const uint16_t>;

        class WithIrqHandler;

        FlashInternalHighCycleAreaStm(uint32_t bank = FLASH_BANK_2);

        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;

        uint32_t NumberOfSectors() const override;
        uint32_t SizeOfSector(uint32_t sectorIndex) const override;
        uint32_t SectorOfAddress(uint32_t address) const override;
        uint32_t AddressOfSector(uint32_t sectorIndex) const override;

    private:
        ConstHalfWordRange flashMemory;
        uint32_t bank;
    };

    class FlashInternalHighCycleAreaStm::WithIrqHandler
        : public FlashInternalHighCycleAreaStm
    {
    public:
        WithIrqHandler(uint32_t bank = FLASH_BANK_2);

    private:
        hal::ImmediateInterruptHandler nmi;
    };
}

#endif // HAL_FLASH_INTERNAL_HIGH_CYCLE_AREA_STM_HPP
