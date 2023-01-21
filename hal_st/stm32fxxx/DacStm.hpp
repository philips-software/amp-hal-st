#ifndef HAL_DAC_STM_HPP
#define HAL_DAC_STM_HPP

#include "cmsis_device.h"
#include "hal/interfaces/Dac.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"

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
