#ifndef HAL_BACKUPRAM_STM_HPP
#define HAL_BACKUPRAM_STM_HPP

#include DEVICE_HEADER
#include "hal/interfaces/BackupRam.hpp"

namespace hal
{
    class BackupRamStm
        : hal::BackupRam<volatile uint32_t>
    {
    public:
        virtual infra::MemoryRange<volatile uint32_t> Get() const override;
    };
}

#endif
