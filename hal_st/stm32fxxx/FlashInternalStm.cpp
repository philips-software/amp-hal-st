#include "hal_st/stm32fxxx/FlashInternalStm.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "services/util/FlashAlign.hpp"

namespace hal
{
    FlashInternalStmBase::FlashInternalStmBase(infra::ConstByteRange flashMemory)
        : flashMemory(flashMemory)
    {}

    void FlashInternalStmBase::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        HAL_FLASH_Unlock();

#if defined(STM32WBA)
        AlignedWriteBuffer(buffer, address);
#elif defined(STM32WB) || defined(STM32G4) || defined(STM32G0)
        AlignedWriteBuffer<uint64_t, FLASH_TYPEPROGRAM_DOUBLEWORD>(buffer, address);
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
        AlignedWriteBuffer<uint16_t, FLASH_TYPEPROGRAM_HALFWORD>(buffer, address);
#elif !defined(STM32WB) && !defined(STM32G4) && !defined(STM32G0) && !defined(STM32WBA)
        for (uint8_t byte : buffer)
        {
            auto result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, reinterpret_cast<uint32_t>(flashMemory.begin() + address), byte);
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
#else
        for (uint32_t index = beginIndex; index != endIndex; ++index)
        {
#if defined(STM32F0) || defined(STM32F3)
            abort(); // Not implemented
#else
            FLASH_Erase_Sector(index, VOLTAGE_RANGE_3);
#endif

            while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP))
            {}
        }
#endif

        HAL_FLASH_Lock();

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    template<typename alignment, uint32_t flashType>
    void FlashInternalStmBase::AlignedWriteBuffer(infra::ConstByteRange buffer, uint32_t address)
    {
        services::FlashAlign::WithAlignment<sizeof(alignment)> flashAlign;
        flashAlign.Align(address, buffer);

        services::FlashAlign::Chunk* chunk = flashAlign.First();
        while (chunk != nullptr)
        {
            really_assert(chunk->data.size() % sizeof(alignment) == 0);
            auto fullAddress = reinterpret_cast<uint32_t>(flashMemory.begin() + chunk->alignedAddress);

            for (alignment data : infra::ReinterpretCastMemoryRange<const alignment>(chunk->data))
            {
                auto result = HAL_FLASH_Program(flashType, fullAddress, data);
                really_assert(result == HAL_OK);
                fullAddress += sizeof(alignment);
            }
            chunk = flashAlign.Next();
        }
    }

#ifdef STM32WBA
    const uint8_t alignment = sizeof(uint64_t) * 2;
    void FlashInternalStmBase::AlignedWriteBuffer(infra::ConstByteRange buffer, uint32_t address)
    {
        services::FlashAlign::WithAlignment<alignment> flashAlign;
        flashAlign.Align(address, buffer);

        services::FlashAlign::Chunk* chunk = flashAlign.First();
        auto dataSize = chunk->data.size();
        while (chunk != nullptr)
        {
            really_assert(chunk->data.size() % sizeof(alignment) == 0);
            auto fullAddress = reinterpret_cast<uint32_t>(flashMemory.begin() + chunk->alignedAddress);

            uint32_t addr = reinterpret_cast<uint32_t>(chunk->data.begin());
            auto result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, fullAddress, addr);
            really_assert(result == HAL_OK);
            fullAddress += alignment;
            chunk = flashAlign.Next();
        }
    }
#endif

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
