#include "hal_st/synchronous_stm32fxxx/SynchronousOneTimeProgrammableFlashStm.hpp"

namespace hal
{
    SynchronousOneTimeProgrammableFlashStm::SynchronousOneTimeProgrammableFlashStm()
        : SynchronousFlashHomogeneous{ OneTimeProgrammableFlashWorker::SectorNumber(), OneTimeProgrammableFlashWorker::SectorSize() }
        , OneTimeProgrammableFlashWorker{}
    {}

    void SynchronousOneTimeProgrammableFlashStm::ReadBuffer(infra::ByteRange buffer, uint32_t address)
    {
        OneTimeProgrammableFlashWorker::ReadBuffer(buffer, address);
    }

    void SynchronousOneTimeProgrammableFlashStm::WriteBuffer(infra::ConstByteRange buffer, uint32_t address)
    {
        OneTimeProgrammableFlashWorker::WriteBuffer(buffer, address);
    }

    void SynchronousOneTimeProgrammableFlashStm::EraseSectors(uint32_t beginIndex, uint32_t endIndex)
    {
        OneTimeProgrammableFlashWorker::EraseSectors(beginIndex, endIndex);
    }
}
