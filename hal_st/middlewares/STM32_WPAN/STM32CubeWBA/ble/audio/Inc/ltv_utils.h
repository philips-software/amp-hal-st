/**
  ******************************************************************************
  * @file    ltv_utils.h
  * @author  MCD Application Team
  * @brief   This file contains definitions used for LTV Structures
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
#ifndef __LTV_UTILS_H
#define __LTV_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "audio_types.h"


/* Includes ------------------------------------------------------------------*/


/* Defines -----------------------------------------------------------*/

/* Types -----------------------------------------------------------*/

/**
 * @brief Return the Supported Sampling Frequency of a given codec specific capabilities array
 * @param codec_specific_capabilities: the codec specific capabilities array to parse
 * @param len: the length of the array to parse
 * @retval the Supported Sampling Frequency bitfield parsed, 0 if no Supported Sampling Frequency LTV was found
 */
Supported_Sampling_Freq_t LTV_GetSupportedSamplingFrequencies(uint8_t *codec_specific_capabilities, uint8_t len);

/**
 * @brief Return the Supported Frame Duration of a given codec specific capabilities array
 * @param codec_specific_capabilities: the codec specific capabilities array to parse
 * @param len: the length of the array to parse
 * @retval the Supported Frame Duration bitfield parsed, 0 if no Supported Frame Duration LTV was found
 */
Supported_Frame_Duration_t LTV_GetSupportedFrameDuration(uint8_t *codec_specific_capabilities, uint8_t len);

/**
 * @brief Return the Supported Audio Channel Counts of a given codec specific capabilities array
 * @param codec_specific_capabilities: the codec specific capabilities array to parse
 * @param len: the length of the array to parse
 * @retval the Supported Audio Channel Counts bitfield parsed
 */
Supported_Channel_Counts_t LTV_GetSupportedAudioChannelCounts(uint8_t *codec_specific_capabilities, uint8_t len);

/**
 * @brief Return the Supported Octets Per Codec Frame of a given codec specific capabilities array
 * @param codec_specific_capabilities: the codec specific capabilities array to parse
 * @param len: the length of the array to parse
 * @retval the value of the Supported Octets Per Codec Frame LTV found:
 *           Octet 0–1: Minimum number of octets supported per codec frame
 *           Octet 2–3: Maximum number of octets supported per codec frame
 *           0 if no Supported Octets Per Codec Frame LTV was found
 */
uint32_t LTV_GetSupportedOctetsPerCodecFrame(uint8_t *codec_specific_capabilities, uint8_t len);

/**
 * @brief Return the Supported Max Codec Frames per SDU of a given codec specific capabilities array
 * @param codec_specific_capabilities: the codec specific capabilities array to parse
 * @param len: the length of the array to parse
 * @retval the Maximum number of codec frames per SDU parsed
 */
uint8_t LTV_GetSupportedMaxCodecFramesPerSDU(uint8_t *codec_specific_capabilities, uint8_t len);

/**
 * @brief Return the Configured Sampling Frequency of a given codec specific capabilities array
 * @param codec_specific_configuration: the codec specific configuration array to parse
 * @param len: the length of the array to parse
 * @retval the Configured Sampling Frequency parsed, 0x00 if no Configured Sampling Frequency LTV was found
 */
Sampling_Freq_t LTV_GetConfiguredSamplingFrequency(uint8_t *codec_specific_configuration, uint8_t len);

/**
 * @brief Return the Configured Frame duration of a given codec specific capabilities array
 * @param codec_specific_configuration: the codec specific configuration array to parse
 * @param len: the length of the array to parse
 * @retval the Configured Frame duration parsed, 0xFF if no Configured Frame duration LTV was found
 */
Frame_Duration_t LTV_GetConfiguredFrameDuration(uint8_t *codec_specific_configuration, uint8_t len);

/**
 * @brief Return the Configured Audio Channel Allocation of a given codec specific capabilities array
 * @param codec_specific_configuration: the codec specific configuration array to parse
 * @param len: the length of the array to parse
 * @retval the Configured Audio Channel Allocation parsed, 0 if no Configured Frame duration LTV was found
 */
Audio_Chnl_Allocation_t LTV_GetConfiguredAudioChannelAllocation(uint8_t *codec_specific_configuration, uint8_t len);

/**
 * @brief Return the Configured Octets Per Codec Frame of a given codec specific capabilities array
 * @param codec_specific_configuration: the codec specific configuration array to parse
 * @param len: the length of the array to parse
 * @retval the Octets Per Codec Frame parsed, 0 if no Octets Per Codec Frame LTV was found
 */
uint16_t LTV_GetConfiguredOctetsPerCodecFrame(uint8_t *codec_specific_configuration, uint8_t len);

/**
 * @brief Return the Configured Frames Block Per SDU of a given codec specific capabilities array
 * @param codec_specific_configuration: the codec specific configuration array to parse
 * @param len: the length of the array to parse
 * @retval the Frames Block Per SDU parsed, 0 if no Configured Frames Block Per SDU LTV was found
 */
uint8_t LTV_GetConfiguredFramesBlockPerSdu(uint8_t *codec_specific_configuration, uint8_t len);

/**
 * @brief Return the Preferred Audio Contexts of a given metadata array
 * @param metadata: the metadata array to parse
 * @param len: the length of the array to parse
 * @param Audio_Context : returned Preferred Audio Context bitfield if LTV is present
 * @retval 0 if Preferred Audio Context LTV is not present in Metadata
 *         1 if Preferred Audio Context LTV is present in Metadata
 */
uint8_t LTV_GetPreferredAudioContexts(uint8_t *metadata, uint8_t len,Audio_Context_t *Audio_Context);

/**
 * @brief Return the Streaming Audio Context of a given metadata array
 * @param metadata: the metadata array to parse
 * @param len: the length of the array to parse
 * @param Audio_Context : returned Streaming Audio Context bitfield if LTV is present
 * @retval 0 if Streaming Audio Context LTV is not present in Metadata
 *         1 if Streaming Audio Context LTV is present in Metadata
 */
uint8_t LTV_GetStreamingAudioContexts(uint8_t *metadata, uint8_t len,Audio_Context_t *Audio_Context);

/**
 * @brief Return the Extended Metadata LTV of a given metadata array
 * @param metadata: the metadata array to parse
 * @param len: the length of the array to parse
 * @retval a pointer to the Extended Metadata LTV parsed, 0 if no Extended Metadata LTV was found
 */
uint8_t* LTV_GetExtendedMetadataLTV(uint8_t *metadata, uint8_t len);

/**
 * @brief Return the Vendor Specific LTV of a given metadata array
 * @param metadata: the metadataarray to parse
 * @param len: the length of the array to parse
 * @retval a pointer to the Vendor Specific LTV parsed, 0 if no Vendor Specific LTV was found
 */
uint8_t* LTV_GetVendorSpecificLTV(uint8_t *metadata, uint8_t len);

/**
 * @brief Return the number of bit set in the given channel allocation
 * @param allocation: the Audio Channel Allocation bitfield to parse
 * @retval The number of bit set in the Audio Channel Allocation
 */
uint8_t LTV_GetNumberOfChannelsSet(Audio_Chnl_Allocation_t allocation);

/**
 * @brief Return the CCID list contained in the metadata
 * @param metadata: the metadata array to parse
 * @param len: the length of the array to parse
 * @param pList : returned pointer on CCID List if LTV is present
 * @param List_Length : returned length of the CCID List
 * @retval 0 if CCID list LTV is not present in Metadata
 *         1 if CCID list LTV is present in Metadata
 */
uint8_t LTV_GetCCIDList(uint8_t *metadata, uint8_t len,uint8_t **pList, uint8_t *List_Length);

#ifdef __cplusplus
}
#endif

#endif /* __LTV_UTILS_H */

