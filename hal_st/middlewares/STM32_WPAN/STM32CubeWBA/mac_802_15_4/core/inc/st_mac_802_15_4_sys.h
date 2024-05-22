/**
  ******************************************************************************
  * @file    mac_sys.h
  * @author  MCD Application Team
  * @brief   Include file for all RTOS/Sequencer can be used on WBA
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef MAC_SYS_H
#define MAC_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* External functions for various OS scheduling ------------------------------*/
extern void MacSys_Init(void);
extern void MacSys_Resume(void);
extern void MacSys_SemaphoreSet(void);
extern void MacSys_SemaphoreWait(void);
extern void MacSys_EventSet(void);
extern void MacSys_EventWait(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // MAC_SYS_H
