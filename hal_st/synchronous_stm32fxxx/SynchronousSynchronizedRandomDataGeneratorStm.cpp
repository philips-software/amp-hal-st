#include "hal_st/synchronous_stm32fxxx/SynchronousSynchronizedRandomDataGeneratorStm.hpp"
#include "infra/util/ReallyAssert.hpp"
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
        Hsi48Enabler hsi48Enabler(synchronousHardwareSemaphoreMasterStm);

        hwRngCreator.Emplace();
        hwRngCreator->GenerateRandomData(result);
        hwRngCreator.Destroy();
    }

    SynchronousSynchronizedRandomDataGeneratorStm::Hsi48Enabler::Hsi48Enabler(hal::SynchronousHardwareSemaphoreMasterStm& synchronousHardwareSemaphoreMasterStm)
    {
        bool clockConfigurationLocked = synchronousHardwareSemaphoreMasterStm.IsLockedByCurrentCore(hal::Semaphore::recoveryAndIndependentClockConfiguration);

        if (clockConfigurationLocked)
        {
            // HSI48 should already be enabled when the clock configuration is locked. Don't manage its lifecycle.
            really_assert(LL_RCC_HSI48_IsReady());
        }
        else
        {
            LL_RCC_HSI48_Enable();
            while (!LL_RCC_HSI48_IsReady())
            {}

            disableHsi48OnFinalization = true;
        }
    }

    SynchronousSynchronizedRandomDataGeneratorStm::Hsi48Enabler::~Hsi48Enabler()
    {
        if (disableHsi48OnFinalization)
            LL_RCC_HSI48_Disable();
    }
}
