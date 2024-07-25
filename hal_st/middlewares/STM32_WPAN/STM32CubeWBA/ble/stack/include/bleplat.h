/*****************************************************************************
 * @file    bleplat.h
 * @author  MDG
 * @brief   This file contains the interface of the BLE platform layer
 *          (lower interface of the BLE stack library).
 *          It is included by the STM32WBX BLE stack library.
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */

#ifndef BLEPLAT_H__
#define BLEPLAT_H__


#include <stdint.h>

/* Enumerated values used for the return of the functions:
 */
enum
{
  BLEPLAT_OK    =  0,
  BLEPLAT_FULL  = -1,
  BLEPLAT_BUSY  = -2,
  BLEPLAT_EOF   = -3,
  BLEPLAT_WARN  = -4,
  BLEPLAT_ERROR = -5
};

/* Enumerated values used for the 'type' of NVM functions:
 */
enum
{
  BLEPLAT_NVM_TYPE_SEC  =  0,
  BLEPLAT_NVM_TYPE_GATT =  1,
};

/* Enumerated values used for the 'mode' of NVM functions:
 */
enum
{
  BLEPLAT_NVM_FIRST    =  0,
  BLEPLAT_NVM_NEXT     =  1,
  BLEPLAT_NVM_CURRENT  =  2,
  BLEPLAT_NVM_ALL      =  3
};

/* General functions:
 */

extern void BLEPLAT_Init( void );

/* Non Volatile Memory (NVM) interface:
 */

extern int BLEPLAT_NvmAdd( uint8_t type,
                           const uint8_t* data,
                           uint16_t size,
                           const uint8_t* extra_data,
                           uint16_t extra_size );

extern int BLEPLAT_NvmGet( uint8_t mode,
                           uint8_t type,
                           uint16_t offset,
                           uint8_t* data,
                           uint16_t size );

extern int BLEPLAT_NvmCompare( uint16_t offset,
                               const uint8_t* data,
                               uint16_t size );

extern void BLEPLAT_NvmDiscard( uint8_t mode );

/* Public Key Algorithms (PKA) interface:
 */

extern int BLEPLAT_PkaStartP256Key( const uint32_t* local_private_key );

extern void BLEPLAT_PkaReadP256Key( uint32_t* local_public_key );

extern int BLEPLAT_PkaStartDhKey( const uint32_t* local_private_key,
                                  const uint32_t* remote_public_key );

extern int BLEPLAT_PkaReadDhKey( uint32_t* dh_key );

extern void BLEPLATCB_PkaComplete( void );

/* Advanced Encryption Standard (AES) interface:
 */

extern void BLEPLAT_AesEcbEncrypt( const uint8_t* key,
                                   const uint8_t* input,
                                   uint8_t* output );

extern void BLEPLAT_AesCmacSetKey( const uint8_t* key );

extern void BLEPLAT_AesCmacCompute( const uint8_t* input,
                                    uint32_t input_length,
                                    uint8_t* output_tag );

/* Random Number Generation (RNG) interface:
 */

extern void BLEPLAT_RngGet( uint8_t n, uint32_t* val );

/* Timer interface:
 */

extern uint8_t BLEPLAT_TimerStart( uint8_t layer,
                                   uint32_t timeout );

extern void BLEPLAT_TimerStop( uint8_t layer );

extern void BLEPLATCB_TimerExpiry( uint8_t layer );


#endif /* BLEPLAT_H__ */
