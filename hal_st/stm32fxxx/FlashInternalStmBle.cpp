#include "hal_st/stm32fxxx/FlashInternalStmBle.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "services/util/FlashAlign.hpp"
#include "shci.h"
#include "stm32wbxx_ll_hsem.h"

namespace hal
{
    FlashInternalStmBle::FlashInternalStmBle(uint32_t numberOfSectors, uint32_t sizeOfEachSector, infra::ConstByteRange flashMemory)
        : FlashHomogeneousInternalStm(numberOfSectors, sizeOfEachSector, flashMemory)
        , flashMemory(flashMemory)
    {}

    void FlashInternalStmBle::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        while (LL_HSEM_1StepLock(HSEM, 2))
            ;
        HAL_FLASH_Unlock();
        AlignedWriteBuffer<uint64_t, FLASH_TYPEPROGRAM_DOUBLEWORD>(buffer, address);
        HAL_FLASH_Lock();
        LL_HSEM_ReleaseLock(HSEM, 2, 0);

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    void FlashInternalStmBle::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        while (LL_HSEM_1StepLock(HSEM, 2))
            ;
        HAL_FLASH_Unlock();
        SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_ON);
        for (uint32_t i = beginIndex; i < endIndex; ++i)
            EraseSingleSector(i);
        SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_OFF);
        HAL_FLASH_Lock();
        LL_HSEM_ReleaseLock(HSEM, 2, 0);

        infra::EventDispatcher::Instance().Schedule(onDone);
    }

    template<typename alignment, uint32_t flashType>
    void FlashInternalStmBle::AlignedWriteBuffer(infra::ConstByteRange buffer, uint32_t address)
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
                auto primaskBit = EnterSingleFlashOperation();
                auto result = HAL_FLASH_Program(flashType, fullAddress, data);
                really_assert(result == HAL_OK || result == HAL_TIMEOUT);
                ExitSingleFlashOperation(primaskBit);
                fullAddress += sizeof(alignment);
            }
            chunk = flashAlign.Next();
        }
    }

    void FlashInternalStmBle::EraseSingleSector(uint32_t index)
    {
        uint32_t pageError = 0;
        FLASH_EraseInitTypeDef eraseInitStruct;
        eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInitStruct.Page = index;
        eraseInitStruct.NbPages = 1;

        auto primaskBit = EnterSingleFlashOperation();
        auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
        really_assert(result == HAL_OK || result == HAL_TIMEOUT);
        ExitSingleFlashOperation(primaskBit);
    }

    uint32_t FlashInternalStmBle::EnterSingleFlashOperation()
    {
        // by default PESD bit mechanism is used by cpu2 to protect its timing. Pool here until it is cleared.
        while (LL_FLASH_IsActiveFlag_OperationSuspended())
            ;
        // enter critial section
        uint32_t primaskBit = __get_PRIMASK();
        __disable_irq();
        return primaskBit;
    }

    void FlashInternalStmBle::ExitSingleFlashOperation(uint32_t primaskBit)
    {
        // exit critical section
        __set_PRIMASK(primaskBit);
        // wait until the operation is completed
        while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_CFGBSY))
            ;
    }
}
