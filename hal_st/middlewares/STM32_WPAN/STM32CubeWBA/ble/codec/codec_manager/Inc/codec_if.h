/**
  ******************************************************************************
  * @file    codec_if.h
  * @author  MCD Application Team
  * @brief   header filer for codec manager integration: clocks and process
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CODEC_TIMER_IF__
#define __CODEC_TIMER_IF__

/* Includes ------------------------------------------------------------------*/
#include "codec_mngr.h"

#if USE_SW_SYNC_METHOD == 1
#define MAX_TIMER_NB            (MAX_PATH_NB)
#else
#define MAX_TIMER_NB            (MAX_PATH_NB + 1)
#endif /* USE_SW_SYNC_METHOD */

/* Exported typedef ----------------------------------------------------------*/
typedef enum
{
  CALIB_PROCESS = 0x01u,
  LL_HIGH_ISR   = 0x02u,
  CODEC_MNGR    = 0x04u,
} CODEC_MaskSrc_t;

typedef enum
{
  CODEC_IRQ_ENABLE  =  0x00u,
  CODEC_IRQ_DISABLE =  0x01u,
} CODEC_IRQReq_t;

typedef enum
{
  APPLI_SEND_DATA,
  APPLI_RECEIVE_DATA_REQ,
  CODEC_TO_LL_DATA_EVT,
  LL_TO_CODEC_DATA_EVT,
  START_ENCODING_EVT,
  STOP_ENCODING_EVT,
  START_DECODING_EVT,
  STOP_DECODING_EVT,
  PLC_TRIGGERED_EVT,
  CALIB_CLBK_EVT,
  CORRECTOR_PROCESS_EVT,
  TRIGGER_EVT,
  FIFO_UNDERRUN_EVT,
  FIFO_OVERRUN_EVT,
  SYNC_EVT,
  IRREGULARITY_EVT
} CODEC_TraceEvnt_t;

/* Exported functions prototypes ---------------------------------------------*/

/******************************************************************************/
/***************************** processing functions ***************************/
/******************************************************************************/

/**
  * @brief Weak function called by the codec manager for initializing process
  * @param none
  * @retval none
  */
void CODEC_ProcessInit( void );

/**
  * @brief Weak function called by the codec manager for requesting CODEC_ManagerProcess to be called
  * @param none
  * @retval none
  */
void CODEC_ProcessReq( void );

/**
  * @brief  Function to request masking and unmasking codec process
  * @note  any masking request should be considered prior unmasking
  * @param src : requestor of the demand
  * @param req : request type; enable or disable the interrupt
  * @retval none
  */
void CODEC_ReqIRQState( CODEC_MaskSrc_t msk_req, CODEC_IRQReq_t req );

/******************************************************************************/
/***************************** clock functions ********************************/
/******************************************************************************/
/**
  * @brief Function to be called for initializing the dedicated audio timer
  * @param none
  * @retval status
  */
int32_t CODEC_CLK_Init( void );

/**
  * @brief Function called by the codec manager for getting a timestamp
  * @param none
  * @retval timestamp
  */
uint32_t CODEC_CLK_GetHostTimestamp( void );

/**
  * @brief Function called by the codec manager for requesting a event with a specified 32 bits timestamps
  * @param ID : path identifier
  * @param trigger_ts : 32 bits timestamp
  * @retval status
  */
int32_t CODEC_CLK_RequestTimerEvent( uint8_t ID, uint32_t trigger_ts );

/**
  * @brief Function called by the codec manager for getting the timer prescaler used from PLL output to timer clock
  * @param none
  * @retval timer prescaler value
  */
uint16_t CODEC_CLK_GetTimerPrescaler( void );

/**
  * @brief Function called by the codec manager for getting the core clock for a given audio frequency
  * @note  Used for either CPU load estimation as well as timer timebase correction
  * @param freq_index : index of the frequency related to the list defined in the assigned numbers
  * @retval Core clock in MHz
  */
float CODEC_CLK_GetCoreClock( uint8_t freq_index );

/**
  * @brief Function called by the codec manager for getting the PLL N Frac value
  * @param none
  * @retval nfrac register value
  */
uint16_t CODEC_CLK_GetPLLNfrac( void );

/**
  * @brief Function called by the codec manager clock corrector for setting the PLL N Frac value for
  * @param n_frac : value of n frac register
  * @retval none
  */
void CODEC_CLK_SetPLLNfrac( uint16_t n_frac );

#if USE_SW_SYNC_METHOD == 0
/**
  * @brief Function called to provide the captured timestamp
  * @param iso_evnt_hw_ts : value of the captured timestamp
  * @retval none
  */
void CODEC_CLK_Provide_ISO_Captured_Timestamp( uint32_t iso_evnt_hw_ts );
#endif /* USE_SW_SYNC_METHOD == 0 */

/******************************************************************************/
/***************************** tracings events ********************************/
/******************************************************************************/
/**
  * @brief  Function called at specific event used for debugging purpose
  * @note This function is called only if CODEC_DEBUG_GPIO is set to 1
  * @param evnt : event of type CODEC_TraceEvnt_t
  * @retval none
  */
void CODEC_TraceEvnt(CODEC_TraceEvnt_t evnt);

/**
 * @brief Function called by the codec to print some logs
 * @param format: a list of printable arguments
 *
 */
void CODEC_DBG_Log(char *format,...);

#endif /* __CODEC_TIMER_IF__ */
