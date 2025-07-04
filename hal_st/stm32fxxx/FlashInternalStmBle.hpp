
#ifndef HAL_FLASH_INTERNAL_STM_BLE_HPP
#define HAL_FLASH_INTERNAL_STM_BLE_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/FlashInternalStm.hpp"
#include "hal_st/stm32fxxx/WatchDogStm.hpp"
#include "services/util/FlashAlign.hpp"
#include <cstdint>

namespace hal
{
    class FlashInternalStmBle
        : public FlashHomogeneousInternalStm
    {
    public:
        FlashInternalStmBle(uint32_t numberOfSectors, uint32_t sizeOfEachSector, infra::ConstByteRange flashMemory, WatchDogStm& watchdog);

        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;

    private:
        enum class FlashOperation
        {
            write,
            erase
        };

        void WaitForCpu2AllowFlashOperation(infra::Function<void()> onAvailable);
        void HsemInterruptHandler();
        void EccErrorHandler();
        void TryWrite();
        void TryErase();
        bool SingleOperation(FlashOperation operation);
        void SingleWrite();
        void SingleErase();

        class CriticalSectionScoped
        {
        public:
            CriticalSectionScoped(WatchDogStm& watchdog);
            ~CriticalSectionScoped();

        private:
            uint32_t primaskBit;
            WatchDogStm& watchdog;
        };

        static constexpr uint32_t hwBlockFlashReqByCpu2 = 7;
        infra::ConstByteRange flashMemory;
        WatchDogStm& watchdog;
        ImmediateInterruptHandler hwSemInterruptHandler;
        ImmediateInterruptHandler nmiHandler;
        infra::Function<void()> onHwSemaphoreAvailable;

        services::FlashAlign::WithAlignment<sizeof(uint64_t)> flashAlign;
        services::FlashAlign::Chunk* chunkToWrite;
        infra::Function<void()> onWriteDone;

        uint32_t currentEraseIndex;
        uint32_t endEraseIndex;
        infra::Function<void()> onEraseDone;
    };
}

#endif
