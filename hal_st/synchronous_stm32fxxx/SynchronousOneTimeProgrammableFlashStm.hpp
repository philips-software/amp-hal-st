#ifndef HALST_SYNCHRONOUS_ONE_TIME_PROGRAMMABLE_FLASH_STM_HPP
#define HALST_SYNCHRONOUS_ONE_TIME_PROGRAMMABLE_FLASH_STM_HPP

#include "hal/synchronous_interfaces/SynchronousFlashHomogeneous.hpp"
#include "hal_st/stm32fxxx/OneTimeProgrammableFlashStm.hpp"

namespace hal
{
    class SynchronousOneTimeProgrammableFlashStm
        : public SynchronousFlashHomogeneous
        , private OneTimeProgrammableFlashWorker
    {
    public:
        class WithIrqHandler;

        SynchronousOneTimeProgrammableFlashStm();

        // implementation of SynchronousFlash interface
        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex) override;

        using OneTimeProgrammableFlashWorker::Range;
    };

    class SynchronousOneTimeProgrammableFlashStm::WithIrqHandler
        : public SynchronousOneTimeProgrammableFlashStm
        , private HighCycleAreaOrOtpIrqHandler
    {};
}

#endif
