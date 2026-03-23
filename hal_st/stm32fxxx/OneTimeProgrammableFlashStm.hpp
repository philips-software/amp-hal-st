#ifndef HALST_ONE_TIME_PROGRAMMABLE_FLASH_STM_HPP
#define HALST_ONE_TIME_PROGRAMMABLE_FLASH_STM_HPP

#include "hal/interfaces/FlashHomogeneous.hpp"
#include "hal_st/stm32fxxx/HighCycleAreaOrOtpIrqHandler.hpp"
#include <cstdint>

namespace hal
{
    // This class is used to implement both the synchronous and asynchronous version of the OTP flash,
    // as the underlying implementation is the same except for scheduling the onDone callback.
    class OneTimeProgrammableFlashWorker
    {
    public:
        using ConstHalfWordRange = infra::MemoryRange<const uint16_t>;

        class WithIrqHandler;

        OneTimeProgrammableFlashWorker();

        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address);
        void ReadBuffer(infra::ByteRange buffer, uint32_t address);
        [[noreturn]] void EraseSectors(uint32_t beginIndex, uint32_t endIndex);

        static uint32_t SectorNumber();
        static uint32_t SectorSize();
        static infra::MemoryRange<volatile uint16_t> Range();

    private:
        ConstHalfWordRange flashMemory;
        void AssertDestinationIsUntouched(infra::ConstByteRange buffer, uint32_t address);
    };

    class OneTimeProgrammableFlashStm
        : public FlashHomogeneous
        , private OneTimeProgrammableFlashWorker
    {
    public:
        class WithIrqHandler;

        OneTimeProgrammableFlashStm();

        // implementation of Flash interface
        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;

        using OneTimeProgrammableFlashWorker::Range;
    };

    class OneTimeProgrammableFlashStm::WithIrqHandler
        : public OneTimeProgrammableFlashStm
        , private HighCycleAreaOrOtpIrqHandler
    {};
}

#endif
