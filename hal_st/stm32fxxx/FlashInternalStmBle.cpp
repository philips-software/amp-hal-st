#include "hal_st/stm32fxxx/FlashInternalStmBle.hpp"
#include "infra/event/EventDispatcher.hpp"
#include "shci.h"
#include "st/STM32WBxx_HAL_Driver/Inc/stm32wbxx_hal_def.h"
#include "stm32wbxx_ll_hsem.h"

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
    {
        SHCI_C2_SetFlashActivityControl(FLASH_ACTIVITY_CONTROL_SEM7);
    }

    void FlashInternalStmBle::WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone)
    {
        while (LL_HSEM_1StepLock(HSEM, hwFlashSemaphoreId))
            ;
        HAL_FLASH_Unlock();

        onWriteDone = onDone;
        flashAlign.Align(address, buffer);
        chunkToWrite = flashAlign.First();
        TryFlashWrite();
    }

    void FlashInternalStmBle::EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone)
    {
        while (LL_HSEM_1StepLock(HSEM, hwFlashSemaphoreId))
            ;
        HAL_FLASH_Unlock();
        SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_ON);

        currentEraseIndex = beginIndex;
        endEraseIndex = endIndex;
        onEraseDone = onDone;
        TryFlashErase();
    }

    void FlashInternalStmBle::WaitForHwSemaphore(uint32_t hsemId, infra::Function<void()> onAvailable)
    {
        if (LL_HSEM_GetStatus(HSEM, hsemId) == 0)
            onAvailable();
        else
        {
            onHwSemaphoreAvailable = onAvailable;
            LL_HSEM_ClearFlag_C1ICR(HSEM, __HAL_HSEM_SEMID_TO_MASK(hsemId));
            HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(hsemId));
        }
    }

    void FlashInternalStmBle::HsemInterruptHandler()
    {
        uint32_t statusreg = HSEM_COMMON->MISR;
        /*Disable Interrupts*/
        HSEM_COMMON->IER &= ~((uint32_t)statusreg);
        /*Clear Flags*/
        HSEM_COMMON->ICR = ((uint32_t)statusreg);

        if (statusreg == __HAL_HSEM_SEMID_TO_MASK(hwFlashSemaphoreId) || statusreg == __HAL_HSEM_SEMID_TO_MASK(hwBlockFlashReqByCpu2))
            onHwSemaphoreAvailable();
    }

    void FlashInternalStmBle::TryFlashWrite()
    {
        if (chunkToWrite == nullptr)
        {
            HAL_FLASH_Lock();
            LL_HSEM_ReleaseLock(HSEM, hwFlashSemaphoreId, 0);
            infra::EventDispatcher::Instance().Schedule(onWriteDone);
        }
        else
            FlashSingleWrite();
    }

    void FlashInternalStmBle::FlashSingleWrite()
    {
        uint32_t primaskBit = __get_PRIMASK();
        __disable_irq();
        bool lockCpu2FlashReq = LL_HSEM_1StepLock(HSEM, hwBlockFlashReqByCpu2) == 0;

        if (lockCpu2FlashReq)
        {
            auto fullAddress = reinterpret_cast<uint32_t>(flashMemory.begin() + chunkToWrite->alignedAddress);
            const uint64_t data = *reinterpret_cast<const uint64_t*>(chunkToWrite->data.begin());
            auto result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, fullAddress, data);
            really_assert(result == HAL_OK);
            LL_HSEM_ReleaseLock(HSEM, hwBlockFlashReqByCpu2, 0);
            chunkToWrite = flashAlign.Next();
        }

        __set_PRIMASK(primaskBit);

        if (!lockCpu2FlashReq)
            WaitForHwSemaphore(hwBlockFlashReqByCpu2, [this]()
                {
                    HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(hwBlockFlashReqByCpu2));
                    FlashSingleWrite();
                });
        else
            TryFlashWrite();
    }

    void FlashInternalStmBle::TryFlashErase()
    {
        if (currentEraseIndex == endEraseIndex)
        {
            SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_OFF);
            HAL_FLASH_Lock();
            LL_HSEM_ReleaseLock(HSEM, hwFlashSemaphoreId, 0);
            infra::EventDispatcher::Instance().Schedule(onEraseDone);
        }
        else
            FlashSingleErase();
    }

    void FlashInternalStmBle::FlashSingleErase()
    {
        /* Add at least 5us (CPU1 up to 64MHz) to guarantee that CPU2 can take SEM7 to protect BLE timing */
        // for (volatile uint32_t i = 0; i < 35; i++)
        //     ;

        uint32_t primaskBit = __get_PRIMASK();
        __disable_irq();
        watchdog.Interrupt();

        bool lockCpu2FlashReq = LL_HSEM_1StepLock(HSEM, hwBlockFlashReqByCpu2) == 0;

        if (lockCpu2FlashReq)
        {
            uint32_t pageError = 0;
            FLASH_EraseInitTypeDef eraseInitStruct;
            eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
            eraseInitStruct.Page = currentEraseIndex;
            eraseInitStruct.NbPages = 1;
            auto result = HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
            really_assert(result == HAL_OK);
            currentEraseIndex++;
            LL_HSEM_ReleaseLock(HSEM, hwBlockFlashReqByCpu2, 0);
        }

        __set_PRIMASK(primaskBit);
        watchdog.Interrupt();

        // while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_CFGBSY))
        //     ;

        if (!lockCpu2FlashReq)
            WaitForHwSemaphore(hwBlockFlashReqByCpu2, [this]()
                {
                    HAL_HSEM_DeactivateNotification(__HAL_HSEM_SEMID_TO_MASK(hwBlockFlashReqByCpu2));
                    FlashSingleErase();
                });
        else
            TryFlashErase();
    }
}
