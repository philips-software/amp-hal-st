#ifndef HAL_ST_NUCLEO_UI_HPP
#define HAL_ST_NUCLEO_UI_HPP

#include "hal_st/stm32fxxx/DmaStm.hpp"
#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "hal_st/stm32fxxx/UartStmDma.hpp"

namespace main_
{
    // UM1956: MB1180 reference board with F031K6, F042K6, F301K8, F303K8, L011K4, L031K6, L412KB, L432KC
    // UM2397: MB1430 reference board with G431KB
    // UM2591: MB1455 reference board with G031K8
    struct Nucleo32Ui
    {
#if defined(STM32G4)
        hal::GpioPinStm ledGreen{ hal::Port::B, 8 };
#elif defined(STM32G0)
        hal::GpioPinStm ledGreen{ hal::Port::C, 6 };
#else
        hal::GpioPinStm ledGreen{ hal::Port::B, 3 };
#endif

#if defined(STM32G0) || defined(STM32G4)
        hal::GpioPinStm stLinkUartTxPin{ hal::Port::A, 2 };
        hal::GpioPinStm stLinkUartRxPin{ hal::Port::A, 3 };

        hal::DmaStm dmaStm;
        hal::DmaStm::TransmitStream transmitStream{ dmaStm, hal::DmaChannelId{ 1, 1, DMA_REQUEST_USART2_TX } };
        hal::UartStmDma stLinkUartDma{ transmitStream, 2, stLinkUartTxPin, stLinkUartRxPin };
#endif
    };

    // UM1724: MB1136 reference board with F030R8, F070RB, F072RB, F091RC, F103RB, F302R8, F303RE, F334R8, F401RE, F410RB, F411RE, F446RE, L010RB, L053R8, L073RZ, L152RE, L452RE, L476RG
    // UM2505: MB1367 reference board with G431RB, G474RE, G491RE
    // UM2324: MB1360 reference board with G070RB, G071RB, G0B1RE
    // UM2206: MB1319 reference board with L412RB-P, L433RC-P, L452RE-P
    // UM2953: MB1717 reference board with C031C6
    struct Nucleo64Ui
    {
        hal::GpioPinStm buttonOne{ hal::Port::C, 13, hal::Drive::Default, hal::Speed::Default, hal::WeakPull::Up };
#if defined(STM32L412RB_P) || defined(STM32L433RC_P) || defined(STM32L452RE_P) // Which other targets? UM1724 does not specify which targets use PA5 and which PB13
        hal::GpioPinStm ledGreen{ hal::Port::B, 13 };
#else
        hal::GpioPinStm ledGreen{ hal::Port::A, 5 }; // STM32G0, STM32G4
#endif

#if defined(STM32G0) || defined(STM32G4)
        hal::GpioPinStm stLinkUartTxPin{ hal::Port::A, 2 };
        hal::GpioPinStm stLinkUartRxPin{ hal::Port::A, 3 };

        hal::DmaStm dmaStm;
        hal::DmaStm::TransmitStream transmitStream{ dmaStm, hal::DmaChannelId{ 1, 1, DMA_REQUEST_USART2_TX } };
        hal::UartStmDma stLinkUartDma{ transmitStream, 2, stLinkUartTxPin, stLinkUartRxPin };
#endif
    };

    // UM2435: MB1355 reference board with STM32WB55RG
    struct Nucleo64WBUi
    {
        hal::GpioPinStm buttonOne{ hal::Port::C, 4, hal::Drive::Default, hal::Speed::Default, hal::WeakPull::Up };
        hal::GpioPinStm buttonTwo{ hal::Port::D, 0, hal::Drive::Default, hal::Speed::Default, hal::WeakPull::Up };
        hal::GpioPinStm buttonThree{ hal::Port::D, 1, hal::Drive::Default, hal::Speed::Default, hal::WeakPull::Up };
        hal::GpioPinStm ledRed{ hal::Port::B, 1 };
        hal::GpioPinStm ledGreen{ hal::Port::B, 0 };
        hal::GpioPinStm ledBlue{ hal::Port::B, 5 };

#if defined(STM32WB)
        hal::GpioPinStm stLinkUartTxPin{ hal::Port::B, 6 };
        hal::GpioPinStm stLinkUartRxPin{ hal::Port::B, 7 };

        hal::DmaStm dmaStm;
        hal::DmaStm::TransmitStream transmitStream{ dmaStm, hal::DmaChannelId{ 1, 1, DMA_REQUEST_USART1_TX } };
        hal::UartStmDma stLinkUartDma{ transmitStream, 1, stLinkUartTxPin, stLinkUartRxPin };
#endif
    };

    // UM1974: MB1137 reference board with F207ZG, F303ZE, F412ZG, F413ZH, F429ZI, F439ZI, F446ZE, F722ZE, F746ZG, F756ZG, F767ZI, H743ZI
    // UM2179: MB1312 reference board with L496ZG, L496ZG-P, L4A6ZG, L4P5ZG, LR5ZI, L4R5ZI-P
    struct Nucleo144Ui
    {
        hal::GpioPinStm buttonOne{ hal::Port::C, 13, hal::Drive::Default, hal::Speed::Default, hal::WeakPull::Up };
        hal::GpioPinStm ledRed{ hal::Port::B, 14 };
#if defined(STM32L496ZG) || defined(STM32L496ZG) || defined(STM32L496ZG_P) || defined(STM32L4A6ZG) || defined(STM32L4P5ZG) || defined(STM32LR5ZI) || defined(STM32L4R5ZI_P)
        hal::GpioPinStm ledGreen{ hal::Port::C, 7 };
#else
        hal::GpioPinStm ledGreen{ hal::Port::B, 0 };
#endif
        hal::GpioPinStm ledBlue{ hal::Port::B, 7 };
    };
}

#endif
