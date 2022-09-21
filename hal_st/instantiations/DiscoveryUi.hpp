#ifndef HAL_ST_DISCOVERY_UI_HPP
#define HAL_ST_DISCOVERY_UI_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"

namespace main_
{
    struct Discovery32f746Ui
    {
        hal::GpioPinStm userButtonPin{ hal::Port::I, 11 };
        hal::GpioPinStm ledGreen{ hal::Port::I, 1 };
    };
}

#endif
