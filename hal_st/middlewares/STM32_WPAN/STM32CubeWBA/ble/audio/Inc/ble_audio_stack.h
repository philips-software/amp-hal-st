/**
 ******************************************************************************
 * @file    ble_audio_stack.h
 * @author  MCD Application Team
 * @brief   Bluetooth Low Energy Audio Stack Interface
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
#ifndef BLE_AUDIO_STACK_H
#define BLE_AUDIO_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_types.h"


/* Defines ---------------------------------------------------------------------*/

/*
 * BLE_AUDIO_PER_LINK_SIZE_BYTES: additional memory size used per link
 */
#define BLE_AUDIO_PER_LINK_SIZE_BYTES       16

/*
 * BLE_AUDIO_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes,
 * needed for the storage of data structures whose size depends on the number of
 * supported connections.
 *
 * @param n_link: Maximum number of simultaneous connections that the device
 * will support. Valid values are from 1 to 8.
 */
#define BLE_AUDIO_TOTAL_BUFFER_SIZE(n_link) \
                  ((BLE_AUDIO_PER_LINK_SIZE_BYTES * (n_link)))

/* Types ---------------------------------------------------------------------*/

/*
 * This structure contains memory and configuration data for the Audio device
*/
typedef struct
{

  /* Start address of the RAM buffer allocated for BLE Audio library.
   * It must be a 32bit aligned RAM area.
   */
  uint8_t* bleStartRamAddress;

  /* Size of the RAM buffer allocated for BLE Audio library.
   * (could be filled with BLE_AUDIO_TOTAL_BUFFER_SIZE return value)
   */
  uint32_t total_buffer_size;

  /* Maximum number of simultaneous connections that the device will support.
   * Valid values are from 1 to 8 (NUM_LINKS used in the calculation of BLE_AUDIO_TOTAL_BUFFER_SIZE).
   */
  uint8_t NumOfLinks;

} BleAudioInit_t;

/**
  * @brief BLE Audio Stack initialization.
  * @note  This function shall be called before any Audio Profile function
  */
extern tBleStatus BLE_AUDIO_STACK_Init(BleAudioInit_t *pBleAudioInit);

/**
  * @brief Return a pointer to the Firmware Version String
  * @retval A pointer to the Firmware Version String
  */
extern uint8_t* BLE_AUDIO_STACK_GetFwVersion(void);

/**
  * @brief The BLE Audio Stack requests to be executed. BLE_AUDIO_STACK_Task() shall be called.
  * @note  If the Host task is executed in a separate thread, the thread shall be unlocked here.
  */
extern void BLE_AUDIO_STACK_NotifyToRun(void);

/**
  * @brief This function is used to run the BLE Audio Stack (may following a call by the stack of BLE_AUDIO_STACK_Task()).
  * @note  It could be called by a thread or by a scheduler. If the BLE Audio stack itself need to run it call the
  *        BLE_AUDIO_STACK_NotifyToRun() interface to inform application that it need to call BLE_AUDIO_STACK_Task().
  * @note  This function shall not be called it the BLE Audio stack context. It could be called by a thread
  *        or by a scheduler.
  */
extern void BLE_AUDIO_STACK_Task(void);

#ifdef __cplusplus
}
#endif

#endif /*BLE_AUDIO_STACK_H*/
