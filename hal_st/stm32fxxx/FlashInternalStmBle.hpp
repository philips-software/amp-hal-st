
#ifndef HAL_FLASH_INTERNAL_STM_BLE_HPP
#define HAL_FLASH_INTERNAL_STM_BLE_HPP

#include "hal_st/cortex/InterruptCortex.hpp"
#include "hal_st/stm32fxxx/FlashInternalStm.hpp"
#include "hal_st/stm32fxxx/WatchDogStm.hpp"
#include "infra/util/AutoResetFunction.hpp"
#include "services/util/FlashAlign.hpp"
#include <cstdint>
#include <variant>

namespace hal
{
    class FlashInternalStmBle
        : public FlashHomogeneousInternalStm
    {
    public:
        static constexpr uint32_t hwBlockFlashReqByCpu2 = 7;

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
        void TryFlashWrite();
        void TryFlashErase();
        bool FlashSingleOperation(FlashOperation operation);
        void FlashSingleWrite();
        void FlashSingleErase();

        class CriticalSectionScoped
        {
        public:
            CriticalSectionScoped(WatchDogStm& watchdog);
            ~CriticalSectionScoped();

        private:
            uint32_t primaskBit;
            WatchDogStm& watchdog;
        };

        infra::ConstByteRange flashMemory;
        WatchDogStm& watchdog;
        ImmediateInterruptHandler hwSemInterruptHandler;
        infra::Function<void()> onHwSemaphoreAvailable;

        services::FlashAlign::WithAlignment<sizeof(uint64_t)> flashAlign;
        services::FlashAlign::Chunk* chunkToWrite;
        infra::Function<void()> onWriteDone;

        uint32_t currentEraseIndex;
        uint32_t endEraseIndex;
        infra::Function<void()> onEraseDone;
    };

    class FlashInternalWithBleAware
        : public hal::Flash
    {
    public:
        FlashInternalWithBleAware(uint32_t numberOfSectors, uint32_t sizeOfEachSector, infra::ConstByteRange flashMemory, hal::WatchDogStm& watchdog);

        void EnableBleAwareness();

        uint32_t NumberOfSectors() const override;
        uint32_t SizeOfSector(uint32_t sectorIndex) const override;
        uint32_t SectorOfAddress(uint32_t address) const override;
        uint32_t AddressOfSector(uint32_t sectorIndex) const override;
        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;

    private:
        void SwitchFlashToBleAware();

        uint32_t numberOfSectors;
        uint32_t sizeOfEachSector;
        infra::ConstByteRange flashMemory;
        hal::WatchDogStm& watchdog;
        std::variant<hal::FlashHomogeneousInternalStm, hal::FlashInternalStmBle> flash;
        bool pendingSwitch = false;
        infra::AutoResetFunction<void()> onFlashDone;
    };
}

#endif
