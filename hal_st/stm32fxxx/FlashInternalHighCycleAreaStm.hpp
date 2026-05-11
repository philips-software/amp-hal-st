#ifndef HAL_FLASH_INTERNAL_HIGH_CYCLE_AREA_STM_HPP
#define HAL_FLASH_INTERNAL_HIGH_CYCLE_AREA_STM_HPP

#include "hal/interfaces/FlashHomogeneous.hpp"
#include "hal_st/stm32fxxx/HighCycleAreaOrOtpIrqHandler.hpp"
#include "infra/util/MemoryRange.hpp"
#include <cstdint>
#include DEVICE_HEADER

namespace hal
{
    // This class is used to implement both the synchronous and asynchronous version of the high cycle flash,
    // as the underlying implementation is the same except for scheduling the onDone callback.
    class FlashInternalHighCycleAreaWorker
    {
    public:
        using HalfWordRange = infra::MemoryRange<uint16_t>;

        class WithIrqHandler;

        explicit FlashInternalHighCycleAreaWorker(HalfWordRange flashMemory);

        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address);
        void ReadBuffer(infra::ByteRange buffer, uint32_t address);
        void EraseSectors(uint32_t beginIndex, uint32_t endIndexExcluding);

        uint32_t TotalSectors() const;
        static uint32_t SectorSize();

    private:
        struct BankConfig
        {
            uint32_t activeBank;
            uint32_t inactiveBank;
            uint32_t enabledSectorsActiveBank;
            uint32_t enabledSectorsInactiveBank;
        };

        static BankConfig ReadBankConfig();

    private:
        HalfWordRange flashMemory;
        const BankConfig bankConfig;
        uint32_t amountOfSectorsInActiveBankInMemoryRange;
        uint32_t amountOfSectorsInInactiveBankInMemoryRange;
    };

    class FlashInternalHighCycleAreaStm
        : private FlashInternalHighCycleAreaWorker
        , public FlashHomogeneous
    {
    public:
        using HalfWordRange = FlashInternalHighCycleAreaWorker::HalfWordRange;

        class WithIrqHandler;

        explicit FlashInternalHighCycleAreaStm(HalfWordRange flashMemory);

        // implementation of Flash interface
        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;
    };

    class FlashInternalHighCycleAreaStm::WithIrqHandler
        : public FlashInternalHighCycleAreaStm
        , private HighCycleAreaOrOtpIrqHandler
    {
    public:
        explicit WithIrqHandler(HalfWordRange flashMemory);
    };
}

#endif // HAL_FLASH_INTERNAL_HIGH_CYCLE_AREA_STM_HPP
