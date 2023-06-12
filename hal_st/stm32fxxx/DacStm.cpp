#include "hal_st/stm32fxxx/DacStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include <type_traits>

#if defined(HAS_PERIPHERAL_DAC)

namespace hal
{
    DacSingleStm::DacSingleStm(GpioPinStm& pin, uint32_t oneBasedIndex)
        : zeroBasedIndex{ oneBasedIndex - 1 }
        , pin(pin)
        , dacHandle()
    {
        EnableClockDac(zeroBasedIndex);

        dacHandle.Instance = peripheralDac[zeroBasedIndex];
        HAL_DAC_Init(&dacHandle);

        SetOutput(zeroBasedIndex);
        HAL_DAC_Start(&dacHandle, DAC_CHANNEL_1);
    }

    DacSingleStm::~DacSingleStm()
    {
        DisableClockDac(zeroBasedIndex);
    }

    void DacSingleStm::SetOutput(uint16_t value)
    {
        HAL_DAC_SetValue(&dacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
    }
}

#endif
