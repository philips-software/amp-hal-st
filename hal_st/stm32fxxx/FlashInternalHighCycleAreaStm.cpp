#include "hal_st/stm32fxxx/FlashInternalHighCycleAreaStm.hpp"
#include "hal_st/stm32fxxx/FlashInternalStmDetail.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/MemoryRange.hpp"
#include "stm32h563xx.h"
#include <cstdint>
#include DEVICE_HEADER

namespace
{
    constexpr uint32_t fullSize{ FLASH_EDATA_SIZE };
    constexpr uint32_t fullbankSize{ fullSize / 2 };
    constexpr uint32_t sectorSize{ fullbankSize / FLASH_EDATA_SECTOR_NB };

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
        , bankConfig(ReadBankConfig())
    {
        really_assert(reinterpret_cast<uintptr_t>(flashMemory.begin()) >= FLASH_EDATA_BASE_NS);
        really_assert(reinterpret_cast<uintptr_t>(flashMemory.end()) <= FLASH_EDATA_BASE_NS + FLASH_EDATA_SIZE);

        const uint32_t totalEnabledSectors = bankConfig.enabledSectorsActiveBank + bankConfig.enabledSectorsInactiveBank;
        const uint32_t totalEnabledHalfWords = totalEnabledSectors * sectorSize / sizeof(uint16_t);

        really_assert(static_cast<uint32_t>(flashMemory.end() - flashMemory.begin()) == totalEnabledHalfWords);
    }

    void FlashInternalHighCycleAreaStm::ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        really_assert(buffer.size() % sizeof(uint16_t) == 0);
        really_assert(address + buffer.size() <= NumberOfSectors() * sectorSize);

        // address is byte-based, addressAdjusted is half-word-based
        auto addressAdjusted = address / 2;
        auto destination = infra::ReinterpretCastMemoryRange<uint16_t>(buffer);
        // explicitely copy data uint16_t aligned and prevent gcc from (falsely) optimizing to memmove which will reinterpret as arrays of uint8_t
        Copy(flashMemory.begin() + addressAdjusted, flashMemory.begin() + addressAdjusted + destination.size(), destination.begin());

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void FlashInternalHighCycleAreaStm::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        really_assert(buffer.size() % sizeof(uint16_t) == 0);
        really_assert(address + buffer.size() <= NumberOfSectors() * sectorSize);

        HAL_FLASH_Unlock();

        detail::AlignedWriteBuffer<uint16_t, FLASH_TYPEPROGRAM_HALFWORD_EDATA, true>(buffer, address, reinterpret_cast<uint32_t>(flashMemory.begin()));

        HAL_FLASH_Lock();

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void FlashInternalHighCycleAreaStm::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        HAL_FLASH_Unlock();

        uint32_t sectorError = 0;

        const uint32_t activeSectorsEnd = std::min(endIndex, bankConfig.enabledSectorsActiveBank);
        if (beginIndex < activeSectorsEnd)
        {
            FLASH_EraseInitTypeDef eraseInitStruct{};
            eraseInitStruct.Banks = bankConfig.activeBank;
            eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
            eraseInitStruct.Sector = beginIndex + FLASH_SECTOR_NB - bankConfig.enabledSectorsActiveBank;
            eraseInitStruct.NbSectors = activeSectorsEnd - beginIndex;

            auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
            really_assert(result == HAL_OK);
            really_assert(sectorError == 0xFFFFFFFFU);
        }

        const uint32_t inactiveSectorsBegin = std::max(beginIndex, bankConfig.enabledSectorsActiveBank);
        if (inactiveSectorsBegin < endIndex)
        {
            FLASH_EraseInitTypeDef eraseInitStruct{};
            eraseInitStruct.Banks = bankConfig.inactiveBank;
            eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
            eraseInitStruct.Sector = (inactiveSectorsBegin - bankConfig.enabledSectorsActiveBank) + FLASH_SECTOR_NB - bankConfig.enabledSectorsInactiveBank;
            eraseInitStruct.NbSectors = endIndex - inactiveSectorsBegin;

            auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
            really_assert(result == HAL_OK);
            really_assert(sectorError == 0xFFFFFFFFU);
        }

        HAL_FLASH_Lock();

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    uint32_t FlashInternalHighCycleAreaStm::NumberOfSectors() const
    {
        return bankConfig.enabledSectorsActiveBank + bankConfig.enabledSectorsInactiveBank;
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

    FlashInternalHighCycleAreaStm::BankConfig FlashInternalHighCycleAreaStm::ReadBankConfig()
    {
        const bool swapBankEnabled = (FLASH->OPTSR_CUR & FLASH_OPTSR_SWAP_BANK) != 0;
        const uint32_t activeBank = swapBankEnabled ? FLASH_BANK_2 : FLASH_BANK_1;
        const uint32_t inactiveBank = swapBankEnabled ? FLASH_BANK_1 : FLASH_BANK_2;

        FLASH_OBProgramInitTypeDef obCurrent{};
        obCurrent.Banks = activeBank;
        HAL_FLASHEx_OBGetConfig(&obCurrent);
        const uint32_t enabledSectorsActiveBank = obCurrent.EDATASize;

        obCurrent.Banks = inactiveBank;
        HAL_FLASHEx_OBGetConfig(&obCurrent);
        const uint32_t enabledSectorsInactiveBank = obCurrent.EDATASize;

        return { activeBank, inactiveBank, enabledSectorsActiveBank, enabledSectorsInactiveBank };
    }

    FlashInternalHighCycleAreaStm::WithIrqHandler::WithIrqHandler(HalfWordRange flashMemory)
        : FlashInternalHighCycleAreaStm(flashMemory)
        , HighCycleAreaOrOtpIrqHandler()
    {}
}
