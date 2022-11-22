
#include "hal_st/stm32fxxx/BackupRamStm.hpp"

namespace hal
{
    BackupRamStm::BackupRamStm()
    {
        peripheralHandle.Instance = peripheralRtc[0];
    }

    infra::MemoryRange<volatile uint32_t> BackupRamStm::Get() const
    {
        return infra::MakeRange(&peripheralHandle.Instance->BKP0R, (&peripheralHandle.Instance->BKP19R) + 1);
    }
}