#include "hal_st/synchronous_stm32fxxx/SynchronousFlashInternalHighCycleAreaStm.hpp"
#include <cstdint>
#include DEVICE_HEADER

namespace hal
{
    SynchronousFlashInternalHighCycleAreaStm::SynchronousFlashInternalHighCycleAreaStm(HalfWordRange flashMemory)
        : FlashInternalHighCycleAreaWorker(flashMemory)
        , SynchronousFlashHomogeneous{ FlashInternalHighCycleAreaWorker::TotalSectors(), FlashInternalHighCycleAreaWorker::SectorSize() }
    {}

    void SynchronousFlashInternalHighCycleAreaStm::WriteBuffer(infra::ConstByteRange buffer, uint32_t address)
    {
        FlashInternalHighCycleAreaWorker::WriteBuffer(buffer, address);
    }

    void SynchronousFlashInternalHighCycleAreaStm::ReadBuffer(infra::ByteRange buffer, uint32_t address)
    {
        FlashInternalHighCycleAreaWorker::ReadBuffer(buffer, address);
    }

    void SynchronousFlashInternalHighCycleAreaStm::EraseSectors(uint32_t beginIndex, uint32_t endIndex)
    {
        FlashInternalHighCycleAreaWorker::EraseSectors(beginIndex, endIndex);
    }
}
