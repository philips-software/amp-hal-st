/**
 ******************************************************************************
 * @file    ble_audio_plat.h
 * @author  MCD Application Team
 * @brief   Bluetooth Low Energy Audio Stack Interface for Audio Stack
 *          integration.
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
#ifndef BLE_AUDIO_PLAT_H
#define BLE_AUDIO_PLAT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "ble_types.h"


/* Defines ---------------------------------------------------------------------*/


/* Types ---------------------------------------------------------------------*/

/* Enumerated values used for the return of the functions:
 */
enum
{
  BLEAUDIO_PLAT_NVM_OK    =  0,
  BLEAUDIO_PLAT_NVM_FULL  = -1,
  BLEAUDIO_PLAT_NVM_BUSY  = -2,
  BLEAUDIO_PLAT_NVM_EOF   = -3,
  BLEAUDIO_PLAT_NVM_WARN  = -4,
  BLEAUDIO_PLAT_NVM_ERROR = -5
};

/* Enumerated values used for the 'type' of NVM functions:
 */
enum
{
  BLEAUDIO_PLAT_NVM_TYPE_BAP_PACS = 2,
  BLEAUDIO_PLAT_NVM_TYPE_BAP_ASCS = 3,
  BLEAUDIO_PLAT_NVM_TYPE_BAP_BASS = 4,
  BLEAUDIO_PLAT_NVM_TYPE_CCP = 5,
  BLEAUDIO_PLAT_NVM_TYPE_CSIP = 6,
  BLEAUDIO_PLAT_NVM_TYPE_MCP = 7,
  BLEAUDIO_PLAT_NVM_TYPE_VCP = 8,
  BLEAUDIO_PLAT_NVM_TYPE_MICP = 9,
};

/* Enumerated values used for the 'mode' of NVM functions:
 */
enum
{
  BLEAUDIO_PLAT_NVM_FIRST    =  0,
  BLEAUDIO_PLAT_NVM_NEXT     =  1,
  BLEAUDIO_PLAT_NVM_CURRENT  =  2,
  BLEAUDIO_PLAT_NVM_ALL      =  3
};

/* Non Volatile Memory (NVM) interface:
 *
 * This interface is only called from BLE Audio stack context
 */

extern int BLE_AUDIO_PLAT_NvmAdd( uint8_t type,
                                 const uint8_t* data,
                                 uint16_t size,
                                 const uint8_t* extra_data,
                                 uint16_t extra_size );

extern int BLE_AUDIO_PLAT_NvmGet( uint8_t mode,
                                   uint8_t type,
                                   uint16_t offset,
                                   uint8_t* data,
                                   uint16_t size );

extern int BLE_AUDIO_PLAT_NvmCompare( uint16_t offset,
                                     const uint8_t* data,
                                     uint16_t size );

extern void BLE_AUDIO_PLAT_NvmDiscard( uint8_t mode );

/* Advanced Encryption Standard (AES) interface:
 */
extern void BLE_AUDIO_PLAT_AesEcbEncrypt( const uint8_t* key,
                                        const uint8_t* input,
                                        uint8_t* output );

extern void BLE_AUDIO_PLAT_AesCmacSetKey( const uint8_t* key );

extern void BLE_AUDIO_PLAT_AesCmacCompute( const uint8_t* input,
                                           uint32_t input_length,
                                           uint8_t* output_tag );

/* Random Number Generation (RNG) interface:
 */
extern void BLE_AUDIO_PLAT_RngGet( uint8_t n, uint32_t* val );

/* Timer interface:
 */
/**
 * @brief BLE_AUDIO_PLAT_TimerStart
 * This function is used to initialize and start the oneshot timer used by the BLE Audio Stack.
 *
 * @param pCallbackFunc : pointer to a function to call when the timer expires
 * @param pParam : param to pass in func when the timer expire
 * @param Timeout : time in milliseconds to wait before the timer expiration
 *
 */
extern void BLE_AUDIO_PLAT_TimerStart(void *pCallbackFunc, void *pParam, uint32_t Timeout);

/**
 * @brief BLE_AUDIO_PLAT_TimerStop
 * This function is used by the BLE Audio Stack to stop timer. This function
 * requests destruction of the timer created by BLE_AUDIO_PLAT_TimerStart()
 *
 */
extern void BLE_AUDIO_PLAT_TimerStop(void);

/**
 * @brief BLE_AUDIO_PLAT_TimerGetRemainMs
 * This function is used by the Audio Stack to get the remaining time
 * of the timer started thanks to the BLE_AUDIO_PLAT_TimerStart() function.
 *
 * @return remaining time in millisecond.
 */
extern uint32_t BLE_AUDIO_PLAT_TimerGetRemainMs(void);

/* Debug Trace interface:
 */
/**
 * @brief BLE_AUDIO_PLAT_DbgLog
 * This function is used by the Audio Stack to print some Debug Traces
 *
 * @param format: a list of printable arguments
 *
 */
extern void BLE_AUDIO_PLAT_DbgLog(char *format,...);

#ifdef __cplusplus
}
#endif

#endif /*BLE_AUDIO_PLAT_H*/
