/**
 ******************************************************************************
 * @file    logging_stm32wba.h
 * @author  MCD Application Team
 * @brief   HCI command for the system channel
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LOGGING_STM32WB_H
#define _LOGGING_STM32WB_H



#ifdef __cplusplus
extern "C" {
#endif

#include <platform/logging.h>

void setDebugLevel(uint8_t aDebugLevel);
uint8_t getDebugLevel(void);

///**
// * This function is used by the RF drivers (LLD) to send traces.
// *
// * @param[in]  aFormat     A pointer to the format string.
// * @param[in]  ...         Arguments for the format specification.
// *
// */
//void otPlatTraceDriver(const char* format, ...);


#ifdef __cplusplus
}
#endif

#endif /*_LOGGING_STM32WB_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
