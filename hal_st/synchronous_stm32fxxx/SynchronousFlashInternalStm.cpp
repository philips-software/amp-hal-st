#include "hal_st/synchronous_stm32fxxx/SynchronousFlashInternalStm.hpp"
#include "infra/util/ReallyAssert.hpp"

namespace hal
{
    SynchronousFlashInternalStm::SynchronousFlashInternalStm(infra::MemoryRange<uint32_t> sectorSizes, infra::ConstByteRange flashMemory)
        : sectorSizes(sectorSizes)
        , flashMemory(flashMemory)
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

    void SynchronousFlashInternalStm::WriteBuffer(infra::ConstByteRange buffer, uint32_t address)
    {
        HAL_FLASH_Unlock();

        uint32_t word;
        while (buffer.size() >= sizeof(word) && ((address & (sizeof(word) - 1)) == 0))
        {
            std::copy(buffer.begin(), buffer.begin() + sizeof(word), reinterpret_cast<uint8_t*>(&word));
            HAL_StatusTypeDef result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, reinterpret_cast<uint32_t>(flashMemory.begin() + address), word);
            really_assert(result == HAL_OK);
            address += sizeof(word);
            buffer.pop_front(sizeof(word));
        }

#if defined(STM32F0) || defined(STM32F3)
        assert((address & 1) == 0);
        assert((buffer.size() & 1) == 0);
        for (uint16_t halfWord : infra::ReinterpretCastMemoryRange<const uint16_t>(buffer))
        {
            HAL_StatusTypeDef result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, reinterpret_cast<uint32_t>(flashMemory.begin() + address), halfWord);
            really_assert(result == HAL_OK);
            ++address;
        }
#else
        for (uint8_t byte : buffer)
        {
            HAL_StatusTypeDef result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, reinterpret_cast<uint32_t>(flashMemory.begin() + address), byte);
            really_assert(result == HAL_OK);
            ++address;
        }
#endif

        HAL_FLASH_Lock();
    }

    void SynchronousFlashInternalStm::ReadBuffer(infra::ByteRange buffer, uint32_t address)
    {
        std::copy(flashMemory.begin() + address, flashMemory.begin() + address + buffer.size(), buffer.begin());
    }

    void SynchronousFlashInternalStm::EraseSectors(uint32_t beginIndex, uint32_t endIndex)
    {
        HAL_FLASH_Unlock();

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

        HAL_FLASH_Lock();
    }
}
