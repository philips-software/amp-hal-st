#include DEVICE_HEADER
#include "hal_st/stm32fxxx/DefaultClockNucleoH563ZI.hpp"

/* The system Clock is configured as follows:
 *    System Clock source            = PLL (HSE)
 *    SYSCLK(Hz)                     = 250000000
 *    HCLK(Hz)                       = 250000000
 *    AHB Prescaler                  = 1
 *    APB1 Prescaler                 = 1
 *    APB2 Prescaler                 = 1
 *    APB3 Prescaler                 = 1
 *    HSE Frequency(Hz)              = 8MHz
 *    VDD(V)                         = 3.3
 *    Main regulator output voltage  = Scale0 mode
 */
void ConfigureDefaultClockNucleoH563ZI()
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
    RCC_OscInitTypeDef RCC_OscInitStruct = {};

    // Configure the main internal regulator output voltage
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
    {
    }

    // Enable HSE Oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;   // Divides the 8MHz HSI to 2MHz
    RCC_OscInitStruct.PLL.PLLN = 250; // Multiplies the 2MHz to 500MHz
    RCC_OscInitStruct.PLL.PLLP = 2;   // Divides the 500MHz to 250MHz for SYSCLK
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_1;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    // Select PLL as system clock source and configure the HCLK, PCLK1, PCLK2 and PCLK3
    // clocks dividers
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
