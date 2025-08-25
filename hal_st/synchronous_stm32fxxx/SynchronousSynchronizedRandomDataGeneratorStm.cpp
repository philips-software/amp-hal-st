#include "hal_st/synchronous_stm32fxxx/SynchronousSynchronizedRandomDataGeneratorStm.hpp"
#include "stm32wbxx.h"

namespace hal
{
    SynchronousSynchronizedRandomDataGeneratorStm::SynchronousSynchronizedRandomDataGeneratorStm(infra::CreatorBase<hal::SynchronousRandomDataGenerator, void()>& hwRngCreator, hal::SynchronousHardwareSemaphoreMasterStm& synchronousHardwareSemaphoreMasterStm)
        : hwRngCreator(hwRngCreator)
        , synchronousHardwareSemaphoreMasterStm(synchronousHardwareSemaphoreMasterStm)
    {}

    void SynchronousSynchronizedRandomDataGeneratorStm::GenerateRandomData(infra::ByteRange result)
    {
        auto semaphore = hal::SynchronousHardwareSemaphoreStm(synchronousHardwareSemaphoreMasterStm, hal::Semaphore::randomNumberGenerator);
        ConditionalHsi48Enable conditionalHsi48Enable(synchronousHardwareSemaphoreMasterStm);

        hwRngCreator.Emplace();
        hwRngCreator->GenerateRandomData(result);
        hwRngCreator.Destroy();
    }

    SynchronousSynchronizedRandomDataGeneratorStm::ConditionalHsi48Enable::ConditionalHsi48Enable(hal::SynchronousHardwareSemaphoreMasterStm& synchronousHardwareSemaphoreMasterStm)
    {
        allowEnable = !synchronousHardwareSemaphoreMasterStm.IsLockedByCurrentCore(hal::Semaphore::recoveryAndIndependentClockConfiguration);
        if (allowEnable)
        {
            LL_RCC_HSI48_Enable();
        }

        while (!LL_RCC_HSI48_IsReady())
        {
        }
    }

    SynchronousSynchronizedRandomDataGeneratorStm::ConditionalHsi48Enable::~ConditionalHsi48Enable()
    {
        if (allowEnable)
            LL_RCC_HSI48_Disable();
    }
}
