#include "hal_st/stm32fxxx/FlashInternalHighCycleAreaStm.hpp"
#include "hal_st/stm32fxxx/FlashInternalStmDetail.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/MemoryRange.hpp"
#include <cstdint>
#include DEVICE_HEADER

namespace
{
    using ConstHalfWordRange = hal::FlashInternalHighCycleAreaStm::ConstHalfWordRange;

    constexpr uint32_t fullSize{ FLASH_EDATA_SIZE };
    constexpr uint32_t bankSize{ fullSize / 2 };
    constexpr uint32_t bankSectorNumber{ FLASH_EDATA_SECTOR_NB };
    constexpr uint32_t sectorSize{ bankSize / bankSectorNumber };
    constexpr uint32_t bankSectorOffset{ FLASH_SECTOR_NB - bankSectorNumber };

    template<uint32_t address>
    constexpr auto ptr()
    {
        return reinterpret_cast<const uint16_t*>(address);
    }

    const ConstHalfWordRange bank1Range{ ptr<FLASH_EDATA_BASE>(), ptr<FLASH_EDATA_BASE + bankSize>() };
    const ConstHalfWordRange bank2Range{ ptr<FLASH_EDATA_BASE + bankSize>(), ptr<FLASH_EDATA_BASE + fullSize>() };

    ConstHalfWordRange GetBankRange(uint32_t bank)
    {
        switch (bank)
        {
            case FLASH_BANK_1:
                return bank1Range;
            case FLASH_BANK_2:
                return bank2Range;
            default:
                std::abort();
        }
    }

    void Copy(const uint16_t* begin, const uint16_t* end, uint16_t* destination)
    {
        for (; begin != end; ++destination, ++begin)
            *destination = *begin;
    }
}

namespace hal
{
    FlashInternalHighCycleAreaStm::FlashInternalHighCycleAreaStm(HalfWordRange flashMemory)
        : flashMemory(flashMemory)
        , bank(GetBankRange(reinterpret_cast<uint32_t>(flashMemory.begin())) == bank1Range ? FLASH_BANK_1 : FLASH_BANK_2)
    {
    }

    void FlashInternalHighCycleAreaStm::ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        really_assert(buffer.size() % sizeof(uint16_t) == 0);
        // address is byte-based, addressAdjusted is half-word-based
        auto addressAdjusted = address / 2;
        auto destination = infra::ReinterpretCastMemoryRange<uint16_t>(buffer);
        // explicitely copy data uint16_t aligned and prevent gcc from (falsely) optimizing to memmove which will reinterpret as arrays of uint8_t
        Copy(flashMemory.begin() + addressAdjusted, flashMemory.begin() + addressAdjusted + destination.size(), destination.begin());

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void FlashInternalHighCycleAreaStm::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        HAL_FLASH_Unlock();

        detail::AlignedWriteBuffer<uint16_t, FLASH_TYPEPROGRAM_HALFWORD_EDATA, true>(buffer, address, reinterpret_cast<uint32_t>(flashMemory.begin()));

        HAL_FLASH_Lock();

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void FlashInternalHighCycleAreaStm::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        HAL_FLASH_Unlock();

        uint32_t sectorError = 0;

        FLASH_EraseInitTypeDef eraseInitStruct{};
        eraseInitStruct.Banks = bank;
        eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        eraseInitStruct.Sector = beginIndex + bankSectorOffset;
        eraseInitStruct.NbSectors = endIndex - beginIndex;

        auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
        really_assert(result == HAL_OK);

        HAL_FLASH_Lock();

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    uint32_t FlashInternalHighCycleAreaStm::NumberOfSectors() const
    {
        return bankSectorNumber;
    }

    uint32_t FlashInternalHighCycleAreaStm::SizeOfSector(uint32_t sectorIndex) const
    {
        return sectorSize;
    }

    uint32_t FlashInternalHighCycleAreaStm::SectorOfAddress(uint32_t address) const
    {
        return address / sectorSize;
    }

    uint32_t FlashInternalHighCycleAreaStm::AddressOfSector(uint32_t sectorIndex) const
    {
        return sectorIndex * sectorSize;
    }

    FlashInternalHighCycleAreaStm::WithIrqHandler::WithIrqHandler(HalfWordRange flashMemory)
        : FlashInternalHighCycleAreaStm(flashMemory)
        , HighCycleAreaOrOtpIrqHandler()
    {}
}
