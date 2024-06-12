/**
  ******************************************************************************
  * @file    LC3_decoder.h
  * @author  MCD Application Team
  * @brief   Header file of LC3 decoder
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

#ifndef __LC3_DECODER_H__
#define __LC3_DECODER_H__

/* Includes ------------------------------------------------------------------*/
#include "LC3.h"

/* Exported define -----------------------------------------------------------*/
#define LC3_DECODER_STRUCT_SIZE (8340u)

/* Exported functions ------------------------------------------------------- */
/**
  * @brief  Initialize an decoder channel handle.
  * @param  hSession : Session handle that must have been initialized prior calling this function, the channel is attached
            to this session and gives information for initializing the channel
  * @param  handle : pointer to an object of size LC3_DECODER_STRUCT_SIZE bytes to handle the channel
  * @param  bitrate : initial bitrate of the channel from which the Nbytes are computed as Nbytes = bitrate*Nms/8*fscal;
  * @param  s_bits : samples format : 16 bits stored in 16 bits register or 24 or 32 bits both stored on 32 bits register
  * @retval LC3 status
  */
LC3_Status lc3_decoder_channel_init(void *hSession,
                                    void *handle,
                                    uint32_t bitrate,
                                    LC3_SampleDepth s_bits);

/**
  * @brief  Update an decoder channel bitrate
  * @note   This function has to be called before executing the decoding process but after the channel initialization
  * @param  handle : Channel handle pointer
  * @param  bitrate : bitrate of the channel from which the Nbytes are computed as Nbytes = bitrate*Nms/8*fscal
  * @retval LC3 status
  */
LC3_Status lc3_decoder_update_bitrate(void *handle,
                                      uint32_t bitrate);

/**
  * @brief  Process the LC3 decoding
  * @note   This function must be called after the channel has been initialized
  * @param  handle : Channel handle pointer
  * @param  bytes : pointer the input buffer that must be a size of Nbytes = bitrate*Nms/8*fscal
  * @param  output : pointer to the output samples array that must be formatted as defined at the channel initialization
            see s_bits format
  * @note   The buffer has to contain Nf samples where Nf = Frequency * Nms
  * @param  decimation : pointer increment : 1 for simple buffer, 2 if input is stereo I2S buffer
  * @param  BFI_in : Bad Frame Indicator that allows to activate the Packet Lost Concealment process (if defined)
  * @note   This flag is given as an input. An internal frame checking may also trig the PLC process.
  * @retval LC3 status
  */
LC3_Status lc3_decoder_process(void *handle,
                               uint8_t *bytes,
                               void *output,
                               uint32_t decimation,
                               uint32_t BFI_in);

#endif /* __LC3_DECODER_H__ */
