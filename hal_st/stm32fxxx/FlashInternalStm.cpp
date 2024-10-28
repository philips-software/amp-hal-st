#include "hal_st/stm32fxxx/FlashInternalStm.hpp"
#include "hal_st/stm32fxxx/FlashInternalStmDetail.hpp"
#include "infra/event/EventDispatcher.hpp"
#include <cstdint>

namespace
{
#if defined(STM32H5)
    uint32_t GetBank(const uint8_t* memoryBegin)
    {
        auto address = reinterpret_cast<uint32_t>(memoryBegin);
        if (READ_BIT(FLASH->OPTSR_CUR, FLASH_OPTSR_SWAP_BANK) == 0)
        {
            // No Bank swap
            return (address < (FLASH_BASE + FLASH_BANK_SIZE)) ? FLASH_BANK_1 : FLASH_BANK_2;
        }
        else
        {
            // Bank swap
            return (address < (FLASH_BASE + FLASH_BANK_SIZE)) ? FLASH_BANK_2 : FLASH_BANK_1;
        }
    }
#endif

    struct uint128_t
    {
#ifdef __ARM_BIG_ENDIAN
        uint64_t high;
        uint64_t low;
#else
        uint64_t low;
        uint64_t high;
#endif
    };
}

namespace hal
{
    FlashInternalStmBase::FlashInternalStmBase(infra::ConstByteRange flashMemory)
        : flashMemory(flashMemory)
    {
    }

    void FlashInternalStmBase::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        HAL_FLASH_Unlock();

        const auto flashBegin = reinterpret_cast<uint32_t>(flashMemory.begin());

#if defined(STM32WBA) || defined(STM32H5)
        detail::AlignedWriteBuffer<uint128_t, FLASH_TYPEPROGRAM_QUADWORD, true>(buffer, address, flashBegin);
#elif defined(STM32WB) || defined(STM32G4) || defined(STM32G0)
        detail::AlignedWriteBuffer<uint64_t, FLASH_TYPEPROGRAM_DOUBLEWORD, false>(buffer, address, flashBegin);
#else
        uint32_t word;
        while (buffer.size() >= sizeof(word) && ((address & (sizeof(word) - 1)) == 0))
        {
            std::copy(buffer.begin(), buffer.begin() + sizeof(word), reinterpret_cast<uint8_t*>(&word));
            auto result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashBegin + address, word);
            really_assert(result == HAL_OK);
            address += sizeof(word);
            buffer.pop_front(sizeof(word));
        }
#endif

#if defined(STM32F0) || defined(STM32F3)
        detail::AlignedWriteBuffer<uint16_t, FLASH_TYPEPROGRAM_HALFWORD, false>(buffer, address);
#elif !defined(STM32WB) && !defined(STM32G4) && !defined(STM32G0) && !defined(STM32WBA) && !defined(STM32H5)
        for (uint8_t byte : buffer)
        {
            auto result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, flashBegin + address, byte);
            really_assert(result == HAL_OK);
            ++address;
        }
#endif

        HAL_FLASH_Lock();

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void FlashInternalStmBase::ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        std::copy(flashMemory.begin() + address, flashMemory.begin() + address + buffer.size(), buffer.begin());
        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void FlashInternalStmBase::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        HAL_FLASH_Unlock();

#if defined(STM32WB) || defined(STM32G4) || defined(STM32G0) || defined(STM32WBA)
        uint32_t pageError = 0;

        FLASH_EraseInitTypeDef eraseInitStruct;
        eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInitStruct.Page = beginIndex;
        eraseInitStruct.NbPages = endIndex - beginIndex;

        auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
        really_assert(result == HAL_OK);

#elif defined(STM32H5)
        uint32_t sectorError = 0;

        FLASH_EraseInitTypeDef eraseInitStruct{};
        eraseInitStruct.Banks = GetBank(flashMemory.begin());
        eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        eraseInitStruct.Sector = beginIndex;
        eraseInitStruct.NbSectors = endIndex - beginIndex;

        auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
        really_assert(result == HAL_OK);
#else
        for (uint32_t index = beginIndex; index != endIndex; ++index)
        {
#if defined(STM32F0) || defined(STM32F3)
            abort(); // Not implemented
#else
            FLASH_Erase_Sector(index, VOLTAGE_RANGE_3);
#endif

            while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP))
            {
            }
        }
#endif

        HAL_FLASH_Lock();

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    FlashInternalStm::FlashInternalStm(infra::MemoryRange<uint32_t> sectorSizes, infra::ConstByteRange flashMemory)
        : FlashInternalStmBase(flashMemory)
        , sectorSizes(sectorSizes)
    {}

    uint32_t FlashInternalStm::NumberOfSectors() const
    {
        return sectorSizes.size();
    }

    uint32_t FlashInternalStm::SizeOfSector(uint32_t sectorIndex) const
    {
        return sectorSizes[sectorIndex];
    }

    uint32_t FlashInternalStm::SectorOfAddress(uint32_t address) const
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

    uint32_t FlashInternalStm::AddressOfSector(uint32_t sectorIndex) const
    {
        uint32_t address = 0;
        for (uint32_t sector = 0; sector != sectorIndex; ++sector)
            address += sectorSizes[sector];
        return address;
    }

    FlashHomogeneousInternalStm::FlashHomogeneousInternalStm(uint32_t numberOfSectors, uint32_t sizeOfEachSector, infra::ConstByteRange flashMemory)
        : FlashInternalStmBase(flashMemory)
        , numberOfSectors(numberOfSectors)
        , sizeOfEachSector(sizeOfEachSector)
    {}

    uint32_t FlashHomogeneousInternalStm::NumberOfSectors() const
    {
        return numberOfSectors;
    }

    uint32_t FlashHomogeneousInternalStm::SizeOfSector(uint32_t sectorIndex) const
    {
        return sizeOfEachSector;
    }

    uint32_t FlashHomogeneousInternalStm::SectorOfAddress(uint32_t address) const
    {
        return address / sizeOfEachSector;
    }

    uint32_t FlashHomogeneousInternalStm::AddressOfSector(uint32_t sectorIndex) const
    {
        return sectorIndex * sizeOfEachSector;
    }
}
