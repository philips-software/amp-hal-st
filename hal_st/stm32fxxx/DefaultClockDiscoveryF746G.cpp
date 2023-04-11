#include DEVICE_HEADER
#include "hal_st/stm32fxxx/DefaultClockDiscoveryF746G.hpp"

/* The system Clock is configured as follows:
 *    System Clock source            = PLL (HSE)
 *    SYSCLK(Hz)                     = 216000000
 *    HCLK(Hz)                       = 216000000
 *    AHB Prescaler                  = 1
 *    APB1 Prescaler                 = 4
 *    APB2 Prescaler                 = 2
 *    HSE Frequency(Hz)              = 25MHz
 *    VDD(V)                         = 3.3
 *    Main regulator output voltage  = Scale1 mode
 */
void ConfigureDefaultClockDiscoveryF746G()
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
    RCC_OscInitTypeDef RCC_OscInitStruct = {};

    // Enable Power Control clock
    __PWR_CLK_ENABLE();

    // The voltage scaling allows optimizing the power consumption when the
    // device is clocked below the maximum system frequency, to update the
    // voltage scaling value regarding system frequency refer to product
    // datasheet.
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Enable HSE Oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;              // Divides the 25MHz HSE to 1MHz
    RCC_OscInitStruct.PLL.PLLN = 432;             // Multiplies the 1MHz to 432MHz
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;   // Divides the 432MHz to 216MHz for SYSCLK
    RCC_OscInitStruct.PLL.PLLQ = 9;               // Divides the 432MHz to 48MHz for USB, SDMMC and RNG
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    HAL_PWREx_EnableOverDrive();

    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
    // clocks dividers
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
}
