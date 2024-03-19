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

        LL_RCC_HSI48_Enable();
        while (!LL_RCC_HSI48_IsReady())
        {}

        hwRngCreator.Emplace();
        hwRngCreator->GenerateRandomData(result);
        hwRngCreator.Destroy();

        LL_RCC_HSI48_Disable();
    }
}
