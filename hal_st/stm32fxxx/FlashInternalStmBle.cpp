#include "hal_st/stm32fxxx/FlashInternalStmBle.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "shci.h"
#include "st/STM32WBxx_HAL_Driver/Inc/stm32wbxx_hal_def.h"
#include "stm32wbxx_ll_hsem.h"
#include <cstdint>

namespace hal
{
    FlashInternalStmBle::FlashInternalStmBle(uint32_t numberOfSectors, uint32_t sizeOfEachSector, infra::ConstByteRange flashMemory, WatchDogStm& watchdog)
        : FlashHomogeneousInternalStm(numberOfSectors, sizeOfEachSector, flashMemory)
        , flashMemory(flashMemory)
        , watchdog(watchdog)
        , hwSemInterruptHandler(HSEM_IRQn, [this]()
              {
                  HsemInterruptHandler();
              })
        , nmiHandler(NonMaskableInt_IRQn, [this]()
              {
                  EccErrorHandler();
              })
    {
        SHCI_C2_SetFlashActivityControl(FLASH_ACTIVITY_CONTROL_SEM7);
    }

    void FlashInternalStmBle::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        onWriteDone = onDone;

        HAL_FLASH_Unlock();
        flashAlign.Align(address, buffer);
        chunkToWrite = flashAlign.First();
        TryWrite();
    }

    void FlashInternalStmBle::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        onEraseDone = onDone;

        HAL_FLASH_Unlock();
        SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_ON);
        currentEraseIndex = beginIndex;
        endEraseIndex = endIndex;
        TryErase();
    }

    void FlashInternalStmBle::WaitForCpu2AllowFlashOperation(infra::Function<void()> onAvailable)
    {
        if (LL_HSEM_GetStatus(HSEM, hwBlockFlashReqByCpu2) == 0)
            onAvailable();
        else
        {
            onHwSemaphoreAvailable = onAvailable;
            LL_HSEM_ClearFlag_C1ICR(HSEM, __HAL_HSEM_SEMID_TO_MASK(hwBlockFlashReqByCpu2));
            HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(hwBlockFlashReqByCpu2));
        }
    }

    void FlashInternalStmBle::HsemInterruptHandler()
    {
        uint32_t statusreg = HSEM_COMMON->MISR;
        /*Disable Interrupts*/
        HSEM_COMMON->IER &= ~((uint32_t)statusreg);
        /*Clear Flags*/
        HSEM_COMMON->ICR = ((uint32_t)statusreg);

        HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(hwBlockFlashReqByCpu2));
        if (statusreg == __HAL_HSEM_SEMID_TO_MASK(hwBlockFlashReqByCpu2))
            onHwSemaphoreAvailable();
    }

    void FlashInternalStmBle::EccErrorHandler()
    {
        if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_ECCD))
        {
            __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ECCD);
            uint32_t errorAddress = READ_BIT(FLASH->ECCR, FLASH_ECCR_ADDR_ECC);
            uint32_t errorSector = SectorOfAddress(errorAddress);
            uint32_t pageError = 0;
            FLASH_EraseInitTypeDef eraseInitStruct{ .TypeErase = FLASH_TYPEERASE_PAGES, .Page = errorSector, .NbPages = 1 };
            HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
        }
    }

    void FlashInternalStmBle::TryWrite()
    {
        constexpr uint32_t maxWriteSize = 2048; // 2KB write takes ~21ms, which is similar to one sector erase
        uint32_t writtenSize = 0;

        while (chunkToWrite != nullptr && writtenSize < maxWriteSize)
        {
            if (!SingleOperation(FlashOperation::write))
            {
                WaitForCpu2AllowFlashOperation([this]()
                    {
                        TryWrite();
                    });
                return;
            }
            writtenSize += sizeof(uint64_t);
        }

        if (chunkToWrite == nullptr)
        {
            HAL_FLASH_Lock();
            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    onWriteDone();
                });
        }
        else if (writtenSize >= maxWriteSize)
            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    TryWrite();
                });
    }

    void FlashInternalStmBle::TryErase()
    {
        if (currentEraseIndex == endEraseIndex)
        {
            SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_OFF);
            HAL_FLASH_Lock();
            infra::EventDispatcher::Instance().Schedule([this]()
                {
                    onEraseDone();
                });
        }
        else
        {
            if (!SingleOperation(FlashOperation::erase))
                WaitForCpu2AllowFlashOperation([this]()
                    {
                        TryErase();
                    });
            else
                infra::EventDispatcher::Instance().Schedule([this]()
                    {
                        TryErase();
                    });
        }
    }

    bool FlashInternalStmBle::SingleOperation(FlashOperation operation)
    {
        CriticalSectionScoped criticalSectionScoped(watchdog);

        if (LL_HSEM_1StepLock(HSEM, hwBlockFlashReqByCpu2) == 0)
        {
            if (operation == FlashOperation::write)
                SingleWrite();
            else if (operation == FlashOperation::erase)
                SingleErase();
            LL_HSEM_ReleaseLock(HSEM, hwBlockFlashReqByCpu2, 0);
            return true;
        }
        else
            return false;
    }

    void FlashInternalStmBle::SingleWrite()
    {
        uint32_t fullAddress = reinterpret_cast<uint32_t>(flashMemory.begin() + chunkToWrite->alignedAddress);
        const uint64_t data = *reinterpret_cast<const uint64_t*>(chunkToWrite->data.begin());
        auto result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, fullAddress, data);
        really_assert(result == HAL_OK);

        chunkToWrite->data.pop_front(sizeof(uint64_t));
        chunkToWrite->alignedAddress += sizeof(uint64_t);
        if (chunkToWrite->data.empty())
            chunkToWrite = flashAlign.Next();
    }

    void FlashInternalStmBle::SingleErase()
    {
        uint32_t pageError = 0;
        FLASH_EraseInitTypeDef eraseInitStruct;
        eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInitStruct.Page = currentEraseIndex;
        eraseInitStruct.NbPages = 1;
        auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
        really_assert(result == HAL_OK);
        currentEraseIndex++;
    }

    FlashInternalStmBle::CriticalSectionScoped::CriticalSectionScoped(WatchDogStm& watchdog)
        : watchdog(watchdog)
    {
        primaskBit = __get_PRIMASK();
        __disable_irq();
        watchdog.WatchDogRefresh();
    }

    FlashInternalStmBle::CriticalSectionScoped::~CriticalSectionScoped()
    {
        __set_PRIMASK(primaskBit);
        watchdog.WatchDogRefresh();
    }
}
