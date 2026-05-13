#ifndef HAL_SYNCHRONOUS_FLASH_INTERNAL_HIGH_CYCLE_AREA_STM_HPP
#define HAL_SYNCHRONOUS_FLASH_INTERNAL_HIGH_CYCLE_AREA_STM_HPP

#include "hal/synchronous_interfaces/SynchronousFlashHomogeneous.hpp"
#include "hal_st/stm32fxxx/FlashInternalHighCycleAreaStm.hpp"
#include "hal_st/stm32fxxx/HighCycleAreaOrOtpIrqHandler.hpp"
#include <cstdint>
#include DEVICE_HEADER

namespace hal
{
    class SynchronousFlashInternalHighCycleAreaStm
        : private FlashInternalHighCycleAreaWorker
        , public SynchronousFlashHomogeneous
    {
    public:
        class WithIrqHandler;

        using HalfWordRange = FlashInternalHighCycleAreaWorker::HalfWordRange;

        explicit SynchronousFlashInternalHighCycleAreaStm(HalfWordRange flashMemory);

        // implementation of Flash interface
        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address) override;
        void ReadBuffer(infra::ByteRange buffer, uint32_t address) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex) override;
    };

    class SynchronousFlashInternalHighCycleAreaStm::WithIrqHandler
        : public SynchronousFlashInternalHighCycleAreaStm
        , private HighCycleAreaOrOtpIrqHandler
    {};
}

#endif // HAL_SYNCHRONOUS_FLASH_INTERNAL_HIGH_CYCLE_AREA_STM_HPP
