/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dbg_trace.h
  * @author  MCD Application Team
  * @brief   Header for dbg_trace.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#ifndef DBG_TRACE_H
#define DBG_TRACE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#if ( ( CFG_DEBUG_TRACE_FULL != 0 ) || ( CFG_DEBUG_TRACE_LIGHT != 0 ) )
#define PRINT_LOG_BUFF_DBG(...) DbgTraceBuffer(__VA_ARGS__)
#if ( CFG_DEBUG_TRACE_FULL != 0 )
#define PRINT_MESG_DBG(...)     do{printf("\r\n [%s][%s][%d] ", DbgTraceGetFileName(__FILE__),__FUNCTION__,__LINE__);printf(__VA_ARGS__);}while(0);
#else
#define PRINT_MESG_DBG          printf
#endif
#else
#define PRINT_LOG_BUFF_DBG(...)
#define PRINT_MESG_DBG(...)
#endif

#define PRINT_NO_MESG(...)

#ifdef __cplusplus
}
#endif

#endif /* DBG_TRACE_H */

