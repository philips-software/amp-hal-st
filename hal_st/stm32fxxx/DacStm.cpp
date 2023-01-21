#include "hal_st/stm32fxxx/DacStm.hpp"
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include <type_traits>

#if defined(HAS_PERIPHERAL_DAC)

namespace hal
{
    DacStm::DacStm(GpioPinStm& pin)
        : pin(pin)
        , dacHandle()
    {
        EnableClockDac(0);

        dacHandle.Instance = peripheralDac[0];
        HAL_DAC_Init(&dacHandle);

        SetOutput(0);
        HAL_DAC_Start(&dacHandle, DAC_CHANNEL_1);
    }

    DacStm::~DacStm()
    {
        DisableClockDac(0);
    }

    void DacStm::SetOutput(uint16_t value)
    {
        HAL_DAC_SetValue(&dacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
    }
}

#endif
