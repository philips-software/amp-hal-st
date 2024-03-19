#ifndef SYNCHRONOUS_HAL_SYNCHRONOUS_HARDWARE_SEMAPHORE_STM_HPP
#define SYNCHRONOUS_HAL_SYNCHRONOUS_HARDWARE_SEMAPHORE_STM_HPP

#include <stdint.h>

namespace hal
{
    enum class Semaphore : uint32_t
    {
        randomNumberGenerator = 0,
        publicKeyAccelerator,
        flashWriteErase,
        resetAndClockControl,
        clockControlForStopMode,
        recoveryAndIndependentClockConfiguration,
        preventCpu2FromWritingAndErasingToFlash,
        preventCpu1FromWritingAndErasingToFlash,
        preventCpu2FromUpdatingBlePersistentDataInSharedRam,
        preventCpu2FromUpdatingThreadPersistentDataInSharedRam,
    };

    class SynchronousHardwareSemaphoreMasterStm
    {
    public:
        SynchronousHardwareSemaphoreMasterStm();
        ~SynchronousHardwareSemaphoreMasterStm();

        void WaitLock(hal::Semaphore semaphore) const;
        void Release(hal::Semaphore semaphore) const;
    };

    class SynchronousHardwareSemaphoreStm
    {
    public:
        explicit SynchronousHardwareSemaphoreStm(SynchronousHardwareSemaphoreMasterStm& synchronousHardwareSemaphoreMaster, hal::Semaphore semaphore);
        ~SynchronousHardwareSemaphoreStm();

        SynchronousHardwareSemaphoreStm(const SynchronousHardwareSemaphoreStm& other) = delete;
        SynchronousHardwareSemaphoreStm& operator=(const SynchronousHardwareSemaphoreStm& other) = delete;

    private:
        SynchronousHardwareSemaphoreMasterStm& synchronousHardwareSemaphoreMaster;
        hal::Semaphore semaphore;
    };
}

#endif
