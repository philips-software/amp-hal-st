#include "hal_st/synchronous_stm32fxxx/SynchronousHardwareSemaphoreStm.hpp"
#include "stm32wbxx.h"

namespace hal
{
    SynchronousHardwareSemaphoreMasterStm::SynchronousHardwareSemaphoreMasterStm()
    {
        __HAL_RCC_HSEM_CLK_ENABLE();
    }

    SynchronousHardwareSemaphoreMasterStm::~SynchronousHardwareSemaphoreMasterStm()
    {
        __HAL_RCC_HSEM_CLK_DISABLE();
    }

    void SynchronousHardwareSemaphoreMasterStm::WaitLock(hal::Semaphore semaphore) const
    {
        HSEM->C1IER |= 1 << static_cast<uint32_t>(semaphore);

        while (HSEM->RLR[static_cast<uint32_t>(semaphore)] != (HSEM_R_LOCK | HSEM_CR_COREID_CURRENT))
        {}
    }

    void SynchronousHardwareSemaphoreMasterStm::Release(hal::Semaphore semaphore) const
    {
        uint32_t mask = 1 << static_cast<uint32_t>(semaphore);
        HSEM->C1ICR = mask;
        HSEM->R[static_cast<uint32_t>(semaphore)] = HSEM_CR_COREID_CURRENT;
        HSEM->C1IER &= ~mask;
    }

    SynchronousHardwareSemaphoreStm::SynchronousHardwareSemaphoreStm(SynchronousHardwareSemaphoreMasterStm& synchronousHardwareSemaphoreMaster, Semaphore semaphore)
        : synchronousHardwareSemaphoreMaster(synchronousHardwareSemaphoreMaster)
        , semaphore(semaphore)
    {
        synchronousHardwareSemaphoreMaster.WaitLock(semaphore);
    }

    SynchronousHardwareSemaphoreStm::~SynchronousHardwareSemaphoreStm()
    {
        synchronousHardwareSemaphoreMaster.Release(semaphore);
    }
}
