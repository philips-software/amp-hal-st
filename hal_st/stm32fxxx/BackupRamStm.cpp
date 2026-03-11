#include "hal_st/stm32fxxx/BackupRamStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"

namespace hal
{
    infra::MemoryRange<volatile uint32_t> BackupRamStm::Get() const
    {
#if defined(STM32G0)
        return infra::MakeRange(&TAMP->BKP0R, &TAMP->BKP4R + 1);
#elif defined(STM32G4) || defined(STM32WBA)
        return infra::MakeRange(&TAMP->BKP0R, &TAMP->BKP15R + 1);
#elif defined(STM32H5)
        auto* base = reinterpret_cast<volatile uint32_t*>(BKPSRAM_BASE);
        return infra::MakeRange(base, base + BKPSRAM_SIZE / sizeof(uint32_t));
#else
        return infra::MakeRange(&peripheralRtc[0]->BKP0R, &peripheralRtc[0]->BKP19R + 1);
#endif
    }
}
