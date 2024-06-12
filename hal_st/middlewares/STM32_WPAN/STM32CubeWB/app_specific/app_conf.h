/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_conf.h
  * @author  MCD Application Team
  * @brief   Application configuration file for STM32WPAN Middleware.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#ifndef APP_CONF_H
#define APP_CONF_H

#include "hw.h"
#include "hw_if.h"

/******************************************************************************
 * Debug
 ******************************************************************************/
/**
 * When CFG_DEBUG_TRACE_FULL is set to 1, the trace are output with the API name, the file name and the line number
 * When CFG_DEBUG_TRACE_LIGHT is set to 1, only the debug message is output
 *
 * When both are set to 0, no trace are output
 * When both are set to 1,  CFG_DEBUG_TRACE_FULL is selected
 */
#define CFG_DEBUG_TRACE_LIGHT     1
#define CFG_DEBUG_TRACE_FULL      0

#if (( CFG_DEBUG_TRACE != 0 ) && ( CFG_DEBUG_TRACE_LIGHT == 0 ) && (CFG_DEBUG_TRACE_FULL == 0))
#undef CFG_DEBUG_TRACE_FULL
#undef CFG_DEBUG_TRACE_LIGHT
#define CFG_DEBUG_TRACE_FULL      0
#define CFG_DEBUG_TRACE_LIGHT     1
#endif

#if ( CFG_DEBUG_TRACE == 0 )
#undef CFG_DEBUG_TRACE_FULL
#undef CFG_DEBUG_TRACE_LIGHT
#define CFG_DEBUG_TRACE_FULL      0
#define CFG_DEBUG_TRACE_LIGHT     0
#endif

/**
 * When not set, the traces is looping on sending the trace over UART
 */
#define DBG_TRACE_USE_CIRCULAR_QUEUE 1

/**
 * max buffer Size to queue data traces and max data trace allowed.
 * Only Used if DBG_TRACE_USE_CIRCULAR_QUEUE is defined
 */
#define DBG_TRACE_MSG_QUEUE_SIZE 4096
#define MAX_DBG_TRACE_MSG_SIZE 1024

/******************************************************************************
 * OTP manager
 ******************************************************************************/
#define CFG_OTP_BASE_ADDRESS    OTP_AREA_BASE

#define CFG_OTP_END_ADRESS      OTP_AREA_END_ADDR

#endif /*APP_CONF_H */
