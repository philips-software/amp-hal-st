#ifndef HAL_BACKUPRAM_STM_HPP
#define HAL_BACKUPRAM_STM_HPP

#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/interfaces/BackupRam.hpp"
#include "stm32wbxx_hal_rtc.h"

namespace hal
{
    class BackupRamStm
        : hal::BackupRam<volatile uint32_t>
    {
    public:
        BackupRamStm();

        virtual infra::MemoryRange<volatile uint32_t> Get() const override;

    private:
        RTC_HandleTypeDef peripheralHandle = {};
    };
}

#endif
