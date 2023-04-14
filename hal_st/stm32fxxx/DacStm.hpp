#ifndef HAL_DAC_STM_HPP
#define HAL_DAC_STM_HPP

#include DEVICE_HEADER
#include "generated/stm32fxxx/PeripheralTable.hpp"
#include "hal/interfaces/Dac.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"

#if defined(HAS_PERIPHERAL_DAC)

namespace hal
{
    class DacStm
        : DacImplBase
    {
    public:
        DacStm(GpioPinStm& pin);
        ~DacStm();

        virtual void SetOutput(uint16_t value) override;

    private:
        AnalogPinStm pin;
        DAC_HandleTypeDef dacHandle;
    };
}

#endif

#endif
