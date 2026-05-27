#include "hal_st/stm32fxxx/FlashInternalHighCycleAreaStm.hpp"
#include "hal_st/stm32fxxx/FlashInternalStmDetail.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "infra/util/ByteRange.hpp"
#include "infra/util/MemoryRange.hpp"
#include "infra/util/ReallyAssert.hpp"
#include <cstdint>
#include DEVICE_HEADER

namespace
{
    constexpr uint32_t fullSize{ FLASH_EDATA_SIZE };
    constexpr uint32_t fullbankSize{ fullSize / 2 };
    constexpr uint32_t sectorSize{ fullbankSize / FLASH_EDATA_SECTOR_NB };
    const uint32_t activeBankEndAddress{ FLASH_EDATA_BASE_NS + fullbankSize };

    void Copy(const uint16_t* begin, const uint16_t* end, uint16_t* destination)
    {
        for (; begin != end; ++destination, ++begin)
            *destination = *begin;
    }
}

namespace hal
{
    FlashInternalHighCycleAreaWorker::FlashInternalHighCycleAreaWorker(HalfWordRange flashMemory)
        : flashMemory(flashMemory)
        , bankConfig(ReadBankConfig())
    {
        const uint32_t flashBeginAddress = reinterpret_cast<uintptr_t>(flashMemory.begin());
        const uint32_t flashEndAddress = reinterpret_cast<uintptr_t>(flashMemory.end());

        // The flashMemory range must be fully contained within the flash edata area (high cycle area)
        really_assert(flashBeginAddress >= FLASH_EDATA_BASE_NS);
        really_assert(flashEndAddress <= FLASH_EDATA_BASE_NS + FLASH_EDATA_SIZE);

        const uint32_t activeBankEdataStart = FLASH_EDATA_BASE_NS + fullbankSize - bankConfig.enabledSectorsActiveBank * sectorSize;
        const uint32_t inactiveBankEdataStart = activeBankEndAddress + fullbankSize - bankConfig.enabledSectorsInactiveBank * sectorSize;

        amountOfSectorsInActiveBankInMemoryRange = static_cast<uint32_t>(std::ceil(static_cast<float>(std::min(activeBankEndAddress, flashEndAddress) - std::min(activeBankEndAddress, flashBeginAddress)) / sectorSize));
        amountOfSectorsInInactiveBankInMemoryRange = static_cast<uint32_t>(std::ceil(static_cast<float>(std::max(activeBankEndAddress, flashEndAddress) - std::max(activeBankEndAddress, flashBeginAddress)) / sectorSize));

        firstSectorOffsetInActiveBank = (flashBeginAddress < activeBankEndAddress)
                                            ? (std::max(flashBeginAddress, activeBankEdataStart) - activeBankEdataStart) / sectorSize
                                            : 0;
        firstSectorOffsetInInactiveBank = (flashBeginAddress >= activeBankEndAddress)
                                              ? (std::max(flashBeginAddress, inactiveBankEdataStart) - inactiveBankEdataStart) / sectorSize
                                              : 0;

        really_assert(bankConfig.enabledSectorsActiveBank >= amountOfSectorsInActiveBankInMemoryRange + firstSectorOffsetInActiveBank);
        really_assert(bankConfig.enabledSectorsInactiveBank >= amountOfSectorsInInactiveBankInMemoryRange + firstSectorOffsetInInactiveBank);

        if (amountOfSectorsInInactiveBankInMemoryRange > 0 && amountOfSectorsInInactiveBankInMemoryRange < 8)
        {
            // If some but not all sectors of the inactive bank are part of the memory map then the active bank must not contain any mapped sectors
            // otherwise the memory mapped high cycle area would be not continuous
            really_assert(amountOfSectorsInActiveBankInMemoryRange == 0);
        }
    }

    void FlashInternalHighCycleAreaWorker::ReadBuffer(infra::ByteRange buffer, uint32_t address)
    {
        really_assert(buffer.size() % sizeof(uint16_t) == 0);
        really_assert(address + buffer.size() <= TotalSectors() * SectorSize());

        // address is byte-based, addressAdjusted is half-word-based
        auto addressAdjusted = address / 2;
        auto destination = infra::ReinterpretCastMemoryRange<uint16_t>(buffer);
        // explicitely copy data uint16_t aligned and prevent gcc from (falsely) optimizing to memmove which will reinterpret as arrays of uint8_t
        Copy(flashMemory.begin() + addressAdjusted, flashMemory.begin() + addressAdjusted + destination.size(), destination.begin());
    }

    void FlashInternalHighCycleAreaWorker::WriteBuffer(infra::ConstByteRange buffer, uint32_t address)
    {
        really_assert(buffer.size() % sizeof(uint16_t) == 0);
        really_assert(address + buffer.size() <= TotalSectors() * SectorSize());

        HAL_FLASH_Unlock();

        detail::AlignedWriteBuffer<uint16_t, FLASH_TYPEPROGRAM_HALFWORD_EDATA, true>(buffer, address, reinterpret_cast<uint32_t>(flashMemory.begin()));

        HAL_FLASH_Lock();
    }

    void FlashInternalHighCycleAreaWorker::EraseSectors(uint32_t beginIndex, uint32_t endIndex)
    {
        HAL_FLASH_Unlock();

        really_assert(beginIndex < TotalSectors());
        really_assert(endIndex <= TotalSectors());

        const uint32_t activeSectorsEnd = std::min(endIndex, amountOfSectorsInActiveBankInMemoryRange);
        if (beginIndex < activeSectorsEnd)
        {
            FLASH_EraseInitTypeDef eraseInitStruct{};
            eraseInitStruct.Banks = bankConfig.activeBank;
            eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
            eraseInitStruct.Sector = FLASH_SECTOR_NB - bankConfig.enabledSectorsActiveBank + firstSectorOffsetInActiveBank + beginIndex;
            eraseInitStruct.NbSectors = activeSectorsEnd - beginIndex;

            uint32_t sectorError = 0;
            auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
            really_assert(result == HAL_OK);
            really_assert(sectorError == 0xFFFFFFFFU);
        }

        const uint32_t inactiveSectorsBegin = std::max(beginIndex, amountOfSectorsInActiveBankInMemoryRange);
        if (inactiveSectorsBegin < endIndex)
        {
            FLASH_EraseInitTypeDef eraseInitStruct{};
            eraseInitStruct.Banks = bankConfig.inactiveBank;
            eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
            eraseInitStruct.Sector = FLASH_SECTOR_NB - bankConfig.enabledSectorsInactiveBank + firstSectorOffsetInInactiveBank + (inactiveSectorsBegin - amountOfSectorsInActiveBankInMemoryRange);
            eraseInitStruct.NbSectors = endIndex - inactiveSectorsBegin;

            uint32_t sectorError = 0;
            auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
            really_assert(result == HAL_OK);
            really_assert(sectorError == 0xFFFFFFFFU);
        }

        HAL_FLASH_Lock();
    }

    uint32_t FlashInternalHighCycleAreaWorker::TotalSectors() const
    {
        return amountOfSectorsInActiveBankInMemoryRange + amountOfSectorsInInactiveBankInMemoryRange;
    }

    uint32_t FlashInternalHighCycleAreaWorker::SectorSize()
    {
        return sectorSize;
    }

    FlashInternalHighCycleAreaWorker::BankConfig FlashInternalHighCycleAreaWorker::ReadBankConfig()
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

    void FlashInternalHighCycleAreaStm::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        FlashInternalHighCycleAreaWorker::WriteBuffer(buffer, address);
        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void FlashInternalHighCycleAreaStm::ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        FlashInternalHighCycleAreaWorker::ReadBuffer(buffer, address);
        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void FlashInternalHighCycleAreaStm::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        FlashInternalHighCycleAreaWorker::EraseSectors(beginIndex, endIndex);
        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    FlashInternalHighCycleAreaStm::FlashInternalHighCycleAreaStm(HalfWordRange flashMemory)
        : FlashInternalHighCycleAreaWorker(flashMemory)
        , FlashHomogeneous{ FlashInternalHighCycleAreaWorker::TotalSectors(), sectorSize }
    {}

    FlashInternalHighCycleAreaStm::WithIrqHandler::WithIrqHandler(HalfWordRange flashMemory)
        : FlashInternalHighCycleAreaStm(flashMemory)
    {}
}
