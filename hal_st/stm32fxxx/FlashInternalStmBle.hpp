
#ifndef HAL_FLASH_INTERNAL_STM_BLE_HPP
#define HAL_FLASH_INTERNAL_STM_BLE_HPP

#include "hal_st/stm32fxxx/FlashInternalStm.hpp"

namespace hal
{
    class FlashInternalStmBle
        : public FlashHomogeneousInternalStm
    {
    public:
        FlashInternalStmBle(uint32_t numberOfSectors, uint32_t sizeOfEachSector, infra::ConstByteRange flashMemory);

        void WriteBuffer(infra::ConstByteRange buffer, uint32_t address, infra::Function<void()> onDone) override;
        void EraseSectors(uint32_t beginIndex, uint32_t endIndex, infra::Function<void()> onDone) override;

    private:
        template<typename alignment, uint32_t flashType>
        void AlignedWriteBuffer(infra::ConstByteRange buffer, uint32_t address);
        void EraseSingleSector(uint32_t index);
        uint32_t EnterSingleFlashOperation();
        void ExitSingleFlashOperation(uint32_t);

        infra::ConstByteRange flashMemory;
    };
}

#endif
