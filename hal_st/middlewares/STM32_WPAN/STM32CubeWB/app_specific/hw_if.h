/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hw_if.h
  * @author  MCD Application Team
  * @brief   Hardware Interface
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HW_IF_H
#define HW_IF_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
#include "stm32wbxx.h"
#include "stm32wbxx_ll_exti.h"
#include "stm32wbxx_ll_system.h"
#include "stm32wbxx_ll_rcc.h"
#include "stm32wbxx_ll_ipcc.h"
#include "stm32wbxx_ll_bus.h"
#include "stm32wbxx_ll_pwr.h"
#include "stm32wbxx_ll_cortex.h"
#include "stm32wbxx_ll_utils.h"
#include "stm32wbxx_ll_hsem.h"
#include "stm32wbxx_ll_gpio.h"
#include "stm32wbxx_ll_rtc.h"

#ifdef  USE_STM32WBXX_USB_DONGLE
#include "stm32wbxx_usb_dongle.h"
#endif
#ifdef  USE_STM32WBXX_NUCLEO
#include "stm32wbxx_nucleo.h"
#endif
#ifdef  USE_X_NUCLEO_EPD
#include "x_nucleo_epd.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /*HW_IF_H */
