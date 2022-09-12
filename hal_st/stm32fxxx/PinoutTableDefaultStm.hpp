#ifndef HAL_PINOUT_TABLE_DEFAULT_STM_HPP
#define HAL_PINOUT_TABLE_DEFAULT_STM_HPP

#include "hal_st/stm32fxxx/public/GpioStm.hpp"
#include "infra/util/public/MemoryRange.hpp"

namespace hal
{
    extern const infra::MemoryRange<const infra::MemoryRange<const GpioStm::PinoutTable>> pinoutTableDefaultStm;

    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableUart;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableLpuart;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableSpi;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableCan;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableDac;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableEthernet;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableUsb;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableQuadSpi;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableFmc;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableLcd;
    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTableTimer;
}

#endif
