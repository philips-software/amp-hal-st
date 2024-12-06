#include "hal_st/synchronous_stm32fxxx/SynchronousFlashInternalStm.hpp"
#include "hal_st/stm32fxxx/FlashInternalStmDetail.hpp"

namespace
{
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

#if defined(FLASH_DBANK_SUPPORT)
    uint32_t GetBank(uint32_t sectorIndex)
    {
        return sectorIndex < FLASH_PAGE_NB ? FLASH_BANK_1 : FLASH_BANK_2;
    }
#endif
}

namespace hal
{
    SynchronousFlashInternalStmBase::SynchronousFlashInternalStmBase(infra::ConstByteRange flashMemory)
        : flashMemory(flashMemory)
    {}

    void SynchronousFlashInternalStmBase::WriteBuffer(infra::ConstByteRange buffer, uint32_t address)
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
            auto result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, reinterpret_cast<uint32_t>(flashMemory.begin() + address), word);
            really_assert(result == HAL_OK);
            address += sizeof(word);
            buffer.pop_front(sizeof(word));
        }
#endif

#if defined(STM32F0) || defined(STM32F3)
        detail::AlignedWriteBuffer<uint16_t, FLASH_TYPEPROGRAM_HALFWORD, false>(buffer, address);
#elif !defined(STM32WB) && !defined(STM32G4) && !defined(STM32G0) && !defined(STM32WBA)
        for (uint8_t byte : buffer)
        {
            auto result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, reinterpret_cast<uint32_t>(flashMemory.begin() + address), byte);
            really_assert(result == HAL_OK);
            ++address;
        }
#endif

        HAL_FLASH_Lock();
    }

    void SynchronousFlashInternalStmBase::ReadBuffer(infra::ByteRange buffer, uint32_t address)
    {
        std::copy(flashMemory.begin() + address, flashMemory.begin() + address + buffer.size(), buffer.begin());
    }

    void SynchronousFlashInternalStmBase::EraseSectors(uint32_t beginIndex, uint32_t endIndex)
    {
        HAL_FLASH_Unlock();

#if defined(STM32WB) || defined(STM32G4) || defined(STM32G0) || defined(STM32WBA)

        auto erase = [](uint32_t beginIndex, uint32_t nbPages, uint32_t bank)
        {
            uint32_t sectorError = 0;

            FLASH_EraseInitTypeDef eraseInitStruct{};
#if defined(FLASH_DBANK_SUPPORT)
            eraseInitStruct.Banks = bank;
#endif
            eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
            eraseInitStruct.Page = beginIndex;
            eraseInitStruct.NbPages = nbPages;

            auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
            really_assert(result == HAL_OK);
        };

#if defined(FLASH_DBANK_SUPPORT)
        if (GetBank(beginIndex) == GetBank(endIndex - 1))
            erase(beginIndex % FLASH_PAGE_NB, endIndex - beginIndex, GetBank(beginIndex));
        else
        {
            erase(beginIndex, FLASH_PAGE_NB - beginIndex, FLASH_BANK_1);
            erase(0, endIndex - FLASH_PAGE_NB + 1, FLASH_BANK_2);
        }
#else
        erase(beginIndex, endIndex - beginIndex, 0);
#endif

#else
        for (uint32_t index = beginIndex; index != endIndex; ++index)
        {
#if defined(STM32F0) || defined(STM32F3)
            abort(); // Not implemented
#else
            FLASH_Erase_Sector(index, VOLTAGE_RANGE_3);
#endif

            while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))
            {}
        }
#endif

        HAL_FLASH_Lock();
    }

    SynchronousFlashInternalStm::SynchronousFlashInternalStm(infra::MemoryRange<uint32_t> sectorSizes, infra::ConstByteRange flashMemory)
        : SynchronousFlashInternalStmBase(flashMemory)
        , sectorSizes(sectorSizes)
    {}

    uint32_t SynchronousFlashInternalStm::NumberOfSectors() const
    {
        return sectorSizes.size();
    }

    uint32_t SynchronousFlashInternalStm::SizeOfSector(uint32_t sectorIndex) const
    {
        return sectorSizes[sectorIndex];
    }

    uint32_t SynchronousFlashInternalStm::SectorOfAddress(uint32_t address) const
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

    uint32_t SynchronousFlashInternalStm::AddressOfSector(uint32_t sectorIndex) const
    {
        uint32_t address = 0;
        for (uint32_t sector = 0; sector != sectorIndex; ++sector)
            address += sectorSizes[sector];
        return address;
    }

    SynchronousFlashHomogeneousInternalStm::SynchronousFlashHomogeneousInternalStm(uint32_t numberOfSectors, uint32_t sizeOfEachSector, infra::ConstByteRange flashMemory)
        : SynchronousFlashInternalStmBase(flashMemory)
        , numberOfSectors(numberOfSectors)
        , sizeOfEachSector(sizeOfEachSector)
    {}

    uint32_t SynchronousFlashHomogeneousInternalStm::NumberOfSectors() const
    {
        return numberOfSectors;
    }

    uint32_t SynchronousFlashHomogeneousInternalStm::SizeOfSector(uint32_t sectorIndex) const
    {
        return sizeOfEachSector;
    }

    uint32_t SynchronousFlashHomogeneousInternalStm::SectorOfAddress(uint32_t address) const
    {
        return address / sizeOfEachSector;
    }

    uint32_t SynchronousFlashHomogeneousInternalStm::AddressOfSector(uint32_t sectorIndex) const
    {
        return sectorIndex * sizeOfEachSector;
    }
}
