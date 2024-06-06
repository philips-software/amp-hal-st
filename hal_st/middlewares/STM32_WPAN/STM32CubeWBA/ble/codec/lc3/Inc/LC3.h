/**
  ******************************************************************************
  * @file    LC3.h
  * @author  MCD Application Team
  * @brief   Header file of LC3 session functions
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

#ifndef __LC3_H__
#define __LC3_H__

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include <string.h>

/* Exported types ------------------------------------------------------------*/
typedef float float32_t;

typedef enum {
    FREQ_8KHz =   8000u,
    FREQ_16KHz =  16000u,
    FREQ_24KHz =  24000u,
    FREQ_32KHz =  32000u,
    FREQ_44K1Hz = 44100u,
    FREQ_48KHz =  48000u,
} LC3_Frequency;

typedef enum {
    FRAME_10MS = 100u,
    FRAME_7MS5 = 75u,
} LC3_FrameDuration;

typedef enum {
    DEPTH_16 = 16u,
    DEPTH_24 = 24u,
    DEPTH_32 = 32u,
} LC3_SampleDepth;

typedef enum {
    LC3_OK                  =  0,
    LC3_FREQ_UNSUPORTED     = -1,
    LC3_FRAME_MS_UNSUPORTED = -2,
    LC3_BITRATE_UNSUPORTED  = -3,
    LC3_DEPTH_UNSUPORTED    = -4,
    LC3_UNKNOWN_ERROR       = -5,
    LC3_BAD_FRAME_ERROR     = +1
} LC3_Status;

/* Exported define -----------------------------------------------------------*/
#define LC3_SESSION_STRUCT_SIZE (604u)

/* Exported functions ------------------------------------------------------- */
/**
  * @brief  Initialize an LC3 session
  * @param  hSession : pointer to an object of size LC3_SESSION_STRUCT_SIZE bytes to handle the session
  * @param  Freq : frequency of the corresponding session
  * @param  Nms : frame duration of the corresponding session
  * @retval LC3 status
  */
LC3_Status lc3_session_init(void* hSession,
                            LC3_Frequency Freq,
                            LC3_FrameDuration Nms);

/**
  * @brief Get the Session Frequency
  * @param  hSession : pointer to the session, or either to the channel
  * @retval Session Frequency
  */
LC3_Frequency lc3_get_frequency(void* hSession);

/**
  * @brief Get the Session Frame Duration
  * @param  hSession : pointer to the session or either to the channel
  * @retval Session Frame Duration
  */
LC3_FrameDuration lc3_get_frame_duration(void* hSession);

/**
  * @brief Get the Session Frame size
  * @param  hSession : pointer to the session or either to the channel
  * @retval frame size in samples
  */
uint32_t lc3_get_frame_size(void* hSession);

/**
  * @brief Get the session pointer of a given channel handler
  * @param  hChannel : pointer to the channel handler
  * @retval pointer to the related session
  */
void* lc3_get_session_from_channel(void* hChannel);

/**
  * @brief Get the number of bytes in the compressed frame
  * @param hChannel : pointer to the channel handler
  * @retval number of bytes
  */
uint16_t lc3_get_bytes_from_channel(void* hChannel);

#endif /* __LC3_H__ */
