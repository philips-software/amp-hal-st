#ifndef HAL_ST_NUCLEO_UI_HPP
#define HAL_ST_NUCLEO_UI_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"

namespace main_
{
    struct NucleoF767ziUi
    {
        hal::GpioPinStm userButtonPin{ hal::Port::C, 13 };
        hal::GpioPinStm ledRed{ hal::Port::B, 14 };
        hal::GpioPinStm ledGreen{ hal::Port::B, 0 };
        hal::GpioPinStm ledBlue{ hal::Port::B, 7 };
    };

    struct NucleoWb55rgNucleoUi
    {
        hal::GpioPinStm buttonOne{ hal::Port::C, 4, hal::Drive::Default, hal::Speed::Default, hal::WeakPull::Up };
        hal::GpioPinStm buttonTwo{ hal::Port::D, 0, hal::Drive::Default, hal::Speed::Default, hal::WeakPull::Up };
        hal::GpioPinStm buttonThree{ hal::Port::D, 1, hal::Drive::Default, hal::Speed::Default, hal::WeakPull::Up };
        hal::GpioPinStm ledRed{ hal::Port::B, 1 };
        hal::GpioPinStm ledGreen{ hal::Port::B, 0 };
        hal::GpioPinStm ledBlue{ hal::Port::B, 5 };
    };
}

#endif
