/**
  ******************************************************************************
  * @file    audio_types.h
  * @author  MCD Application Team
  * @brief   This file contains definitions used for Audio
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
#ifndef __AUDIO_TYPES_H
#define __AUDIO_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Audio Data Path Direction Type*/
typedef	uint8_t	Audio_Data_Direction_t;
#define DATA_PATH_INPUT                         (0x00)
#define DATA_PATH_OUTPUT                        (0x01)

/* Audio Data Interface.
 * DATA_PATH_HCI is defined in Bluetooth Specification. Other values are Vendor Specific
*/
#define DATA_PATH_HCI                           (0x00)

/* Coding Format Type
 * (see Bluetooth Assigned Numbers for Host Controller Interface)
 */
typedef	uint8_t	Audio_Coding_Format_t;

#define AUDIO_CODING_FORMAT_ULAW                (0x00)
#define AUDIO_CODING_FORMAT_ALAW                (0x01)
#define AUDIO_CODING_FORMAT_CVSD                (0x02)
#define AUDIO_CODING_FORMAT_TRANSPARENT         (0x03)
#define AUDIO_CODING_FORMAT_LINEAR_PCM	        (0x04)
#define AUDIO_CODING_FORMAT_MSBC                (0x05)
#define AUDIO_CODING_FORMAT_LC3                 (0x06)
#define AUDIO_CODING_FORMAT_G729A               (0x07)
#define AUDIO_CODING_FORMAT_VS                  (0xFF)

/* Audio Location Type
 * Bitmap of audio location
 */
typedef	uint32_t Audio_Location_t;
#define FRONT_LEFT                              (0x00000001)
#define FRONT_RIGHT                             (0x00000002)
#define FRONT_CENTER			        (0x00000004)
#define LOW_FREQ_EFFECTS_1		        (0x00000008)
#define BACK_LEFT                               (0x00000010)
#define BACK_RIGHT                              (0x00000020)
#define FRONT_LEFT_CENTER		        (0x00000040)
#define FRONT_RIGHT_CENTER		        (0x00000080)
#define BACK_CENTER                             (0x00000100)
#define LOW_FREQ_EFFECTS_2		        (0x00000200)
#define SIDE_LEFT                               (0x00000400)
#define SIDE_RIGHT                              (0x00000800)
#define TOP_FRONT_LEFT			        (0x00001000)
#define TOP_FRONT_RIGHT			        (0x00002000)
#define TOP_FRONT_CENTER		        (0x00004000)
#define TOP_CENTER                              (0x00008000)
#define TOP_BACK_LEFT			        (0x00010000)
#define TOP_BACK_RIGHT			        (0x00020000)
#define TOP_SIDE_LEFT			        (0x00040000)
#define TOP_SIDE_RIGHT			        (0x00080000)
#define TOP_BACK_CENTER			        (0x00100000)
#define BOTTOM_FRONT_CENTER		        (0x00200000)
#define BOTTOM_FRONT_LEFT		        (0x00400000)
#define BOTTOM_FRONT_RIGHT		        (0x00800000)
#define FRONT_LEFT_WIDE			        (0x01000000)
#define FRONT_RIGHT_WIDE		        (0x02000000)
#define LEFT_SURROUND			        (0x04000000)
#define RIGHT_SURROUND			        (0x08000000)

/* Types of audio input*/
typedef uint8_t Audio_Input_t;
#define AUDIO_INPUT_UNSPECIFIED                 (0x00u) /* Unspecified Input */
#define AUDIO_INPUT_BLUETOOTH                   (0x01u) /* Bluetooth Audio Stream */
#define AUDIO_INPUT_MICROPHONE                  (0x02u) /* Microphone */
#define AUDIO_INPUT_ANALOG                      (0x03u) /* Analog Interface */
#define AUDIO_INPUT_DIGITAL                     (0x04u) /* Digital Interface */
#define AUDIO_INPUT_RADIO                       (0x05u) /* AM/FM/XM/etc */
#define AUDIO_INPUT_STREAMING                   (0x06u) /* Streaming Audio Source */


/* Audio Contexts Type
 */
typedef	uint16_t Audio_Context_t;
#define AUDIO_CONTEXT_UNSPECIFIED               (0x0001)
#define AUDIO_CONTEXT_CONVERSATIONAL	        (0x0002)
#define AUDIO_CONTEXT_MEDIA                     (0x0004)
#define AUDIO_CONTEXT_GAME                      (0x0008)
#define AUDIO_CONTEXT_INSTRUCTIONAL             (0x0010)
#define AUDIO_CONTEXT_VOICE_ASSISTANTS	        (0x0020)
#define AUDIO_CONTEXT_LIVE                      (0x0040)
#define AUDIO_CONTEXT_SOUND_EFFECTS             (0x0080)
#define AUDIO_CONTEXT_NOTIFICATIONS             (0x0100)
#define AUDIO_CONTEXT_RINGTONE                  (0x0200)
#define AUDIO_CONTEXT_ALERTS                    (0x0400)
#define AUDIO_CONTEXT_EMERGENCY_ALARM	        (0x0800)

/* Codec Specific Capabilities Type
 */
typedef	uint8_t  Codec_Specific_Cap_t;
#define CODEC_CAP_SAMPLING_FREQ                 (0x01)
#define CODEC_CAP_FRAME_DURATION                (0x02)
#define CODEC_CAP_AUDIO_CHNL_COUNTS             (0x03)
#define CODEC_CAP_OCTETS_PER_CODEC_FRAME        (0x04)
#define CODEC_CAP_MAX_CODEC_FRAMES_PER_SDU      (0x05)


/* Supported Sampling Frequencies Type
 * Bitfield of supported sampling frequencies
 */
typedef	uint16_t Supported_Sampling_Freq_t;
#define SUPPORTED_SAMPLE_FREQ_8000_HZ		(0x0001)
#define SUPPORTED_SAMPLE_FREQ_11025_HZ		(0x0002)
#define SUPPORTED_SAMPLE_FREQ_16000_HZ		(0x0004)
#define SUPPORTED_SAMPLE_FREQ_22050_HZ		(0x0008)
#define SUPPORTED_SAMPLE_FREQ_24000_HZ		(0x0010)
#define SUPPORTED_SAMPLE_FREQ_32000_HZ		(0x0020)
#define SUPPORTED_SAMPLE_FREQ_44100_HZ		(0x0040)
#define SUPPORTED_SAMPLE_FREQ_48000_HZ		(0x0080)
#define SUPPORTED_SAMPLE_FREQ_88200_HZ		(0x0100)
#define SUPPORTED_SAMPLE_FREQ_96000_HZ		(0x0200)
#define SUPPORTED_SAMPLE_FREQ_176400_HZ		(0x0400)
#define SUPPORTED_SAMPLE_FREQ_192000_HZ		(0x0800)
#define SUPPORTED_SAMPLE_FREQ_384000_HZ		(0x1000)

/* Supported Frame Duration Type
 * Bitfield of supported Frame Duration
 */
typedef	uint8_t Supported_Frame_Duration_t;
#define SUPPORTED_FRAME_DURATION_7_5_MS                 (0x01)
#define SUPPORTED_FRAME_DURATION_10_MS                  (0x02)
#define SUPPORTED_FRAME_DURATION_PREFERRED_7_5_MS	(0x10) /* 7.5 ms preferred. Valid only when 7.5 ms is supported and 10 ms is supported.
								Shall not be used if FRAME_DURATION_PREFERRED_10_MS is used*/
#define SUPPORTED_FRAME_DURATION_PREFERRED_10_MS	(0x20) /*10 ms preferred. Valid only when 7.5 ms is supported and 10 ms is supported.
								Shall not be used if FRAME_DURATION_PREFERRED_7_5_MS is used*/


/* Supported Audio Channel Counts Type
 * Bitfield of supported Audio Channel Counts
 */
typedef	uint8_t Supported_Channel_Counts_t;
#define SUPPORTED_AUDIO_CHNL_COUNT_1		(0x01)
#define SUPPORTED_AUDIO_CHNL_COUNT_2		(0x02)
#define SUPPORTED_AUDIO_CHNL_COUNT_3		(0x04)
#define SUPPORTED_AUDIO_CHNL_COUNT_4		(0x08)
#define SUPPORTED_AUDIO_CHNL_COUNT_5		(0x10)
#define SUPPORTED_AUDIO_CHNL_COUNT_6		(0x20)
#define SUPPORTED_AUDIO_CHNL_COUNT_7		(0x40)
#define SUPPORTED_AUDIO_CHNL_COUNT_8		(0x80)

/* Codec Specific Configuration Type
 */
typedef	uint8_t  Codec_Specific_Conf_t;
#define CODEC_SAMPLING_FREQ                     (0x01)
#define CODEC_FRAME_DURATION                    (0x02)
#define CODEC_AUDIO_CHNL_ALLOCATION             (0x03)
#define CODEC_OCTETS_PER_CODEC_FRAME            (0x04)
#define CODEC_FRAMES_BLOCKS_PER_SDU             (0x05)

/*  Sampling Frequencies Type
 */
typedef	uint8_t Sampling_Freq_t;
#define SAMPLE_FREQ_8000_HZ                     (0x01)
#define SAMPLE_FREQ_11025_HZ	                (0x02)
#define SAMPLE_FREQ_16000_HZ	                (0x03)
#define SAMPLE_FREQ_22050_HZ	                (0x04)
#define SAMPLE_FREQ_24000_HZ	                (0x05)
#define SAMPLE_FREQ_32000_HZ	                (0x06)
#define SAMPLE_FREQ_44100_HZ	                (0x07)
#define SAMPLE_FREQ_48000_HZ	                (0x08)
#define SAMPLE_FREQ_88200_HZ	                (0x09)
#define SAMPLE_FREQ_96000_HZ	                (0x0A)
#define SAMPLE_FREQ_176400_HZ	                (0x0B)
#define SAMPLE_FREQ_192000_HZ	                (0x0C)
#define SAMPLE_FREQ_384000_HZ	                (0x0D)

#define SAMPLE_FREQ_NUMBER                      (13U)

/* Frame Duration Type
 */
typedef	uint8_t Frame_Duration_t;
#define FRAME_DURATION_7_5_MS	                (0x00)
#define FRAME_DURATION_10_MS	                (0x01)

#define FRAME_DURATION_NUMBER                   (2U)

/* Audio Channel Allocation Type
 */
typedef	Audio_Location_t Audio_Chnl_Allocation_t;

/* Preferred Audio Contexts Type
 * Bitfield of Context Type values
 */
typedef	Audio_Context_t Preferred_Audio_Context_t;

/* Streaming Audio Contexts Type
 * Bitfield of Context Type values
 */
typedef	Audio_Context_t Streaming_Audio_Context_t;

/* Metadata LTV Type */
typedef	uint8_t  Metadata_t;
#define METADATA_PREFERRED_AUDIO_CONTEXTS               (0x01)
#define METADATA_STREAMING_AUDIO_CONTEXTS               (0x02)
#define METADATA_PROGRAM_INFO                           (0x03)
#define METADATA_LANGUAGE                               (0x04)
#define METADATA_CCID_LIST                              (0x05)
#define METADATA_PARENTAL_RATING                        (0x06)
#define METADATA_PROGRAM_INFO_URI                       (0x07)
#define METADATA_AUDIO_ACTIVE_STATE                     (0x08)
#define METADATA_BRD_AUDIO_IMMEDIATE_RENDERING_FLAG     (0x09)

#define METADATA_EXTENDED                               (0xFE)
#define METADATA_VENDOR_SPECIFIC                        (0xFF)

/* Metadata LTV Length */
#define METADATA_PREFERRED_AUDIO_CONTEXTS_LENGTH        (0x03)
#define METADATA_STREAMING_AUDIO_CONTEXTS_LENGTH        (0x03)

/* type for Audio Stream Endpoint */
typedef uint8_t ASE_Type_t;
#define ASE_SINK                                        (0x00)
#define ASE_SOURCE                                      (0x01)

/* State for Audio Stream Endpoint*/
typedef uint8_t ASE_State_t;
#define ASE_STATE_IDLE                  (0x00)  /* ASE is in idle state : No codec configuration or QoS configuration applied. */
#define ASE_STATE_CODEC_CONFIGURED      (0x01)  /* The ASE has a codec configuration applied. The server is exposing its preferred
                                                 * QoS parameters; however, the ASE has no QoS configuration applied yet.
                                                 */
#define ASE_STATE_QOS_CONFIGURED        (0x02)  /* The ASE has a codec configuration and a QoS configuration applied.
                                                 * If the ASE is in the QoS Configured state, a remote Unicast Client may
                                                 * attempt to establish a CIS for that ASE. In this case, when HCI_LE_CIS_Request
                                                 * event is notified to application layer, the BAP_CIS_Requested() function
                                                 * should be called to notify the BAP IP.In this case, the local Unicast
                                                 * Server rejects the establishment of the CIS (wait ASE is in the Enabling state).
                                                 * Local Unicast Client doesn't attempt to establish a CIS for ASE in QoS Configured
                                                 * state but wait for the ASE is in Enabling state.
                                                 */
#define ASE_STATE_ENABLING              (0x03)  /* The ASE has a codec configuration and a QoS configuration applied and any Metadata
                                                 * applied by the client or server is associated with the ASE.
                                                 * if the Unicast Client has not yet established a CIS for that ASE, it will attempts
                                                 * to establish a CIS.
                                                 * If local device is Unicast Server for that ASE, when HCI_LE_CIS_Request event
                                                 * is notified to application layer, the BAP_CIS_Requested() function should be
                                                 * called to notify the BAP IP. The local Unicast Server accepts the CIS
                                                 * establishment if CIS resource is available, else it rejects it.
                                                 * When a CIS is established, the HCI CIS Established Event  is notified
                                                 * to application layer and BAP_CIS_Established() function should be called
                                                 * to notify the BAP IP.
                                                 * If a Source ASE is in the Enabling state in Unicast Server side, the
                                                 * Unicast Server should not transmit audio data for that ASE until ASE
                                                 * is in Streaming state.
                                                 * If a Sink ASE is in the Enabling state in Unicast Client side, the
                                                 * Unicast Client should not transmit audio data for that ASE until
                                                 * the ASE is in Streaming state.
                                                 */
#define ASE_STATE_STREAMING             (0x04)  /* The ASE has a codec configuration and a QoS configuration applied,
                                                 * and any Metadata applied by the client or server is associated with
                                                 * the ASE. The ASE is coupled to a CIS.
                                                 * The ASE is ready to receive or transmit audio data.
                                                 */
#define ASE_STATE_DISABLING             (0x05)  /* Only for Source ASE.
                                                 * CIS might remain established or might be disconnected however the
                                                 * ASE is being decoupled from the CIS.
                                                 * ASE will be in QoS Configured state once Disabling Operation is complete.
                                                 * by Unicast Server and Unicast Client sides.
                                                 */
#define ASE_STATE_RELEASING             (0x06)  /* Any CIS established to transport audio data for the ASE is being disconnected
                                                 * or has been disconnected..
                                                 * Any previously applied codec configuration may be cached by the server,
                                                 * or the server may cache a codec configuration of the server’s choosing,
                                                 * or the codec configuration may be removed.
                                                 * Any previously applied QoS configuration is no longer valid.
                                                 * Any Metadata previously applied is no longer associated with the ASE.
                                                 */

/* Operation type on Audio Stream Endpoint*/
typedef uint8_t ASE_Operation_t;
#define ASE_CONFIG_CODEC_OPERATION                       (0x01)
#define ASE_CONFIG_QOS_OPERATION                         (0x02)
#define ASE_ENABLE_OPERATION                             (0x03)
#define ASE_RECEIVER_START_READY_OPERATION               (0x04)
#define ASE_DISABLE_OPERATION                            (0x05)
#define ASE_RECEIVER_STOP_READY_OPERATION                (0x06)
#define ASE_UPDATE_METADATA_OPERATION                    (0x07)
#define ASE_RELEASE_OPERATION                            (0x08)
#define ASE_RELEASED_OPERATION                           (0x09)

/* Operation Response type on Audio Stream Endpoint*/
typedef uint8_t ASE_Op_Resp_t;
#define ASE_OP_RESP_SUCCESS                              (0x00)
#define ASE_OP_RESP_UNSUPPORTED_OPCODE                   (0x01)
#define ASE_OP_RESP_INVALID_LENGTH                       (0x02)
#define ASE_OP_RESP_INVALID_ASE_ID                       (0x03)
#define ASE_OP_RESP_INVALID_ASE_STATE                    (0x04)
#define ASE_OP_RESP_INVALID_ASE_DIR                      (0x05)
#define ASE_OP_RESP_UNSUPPORTED_AUDIO_CAP                (0x06)
#define ASE_OP_RESP_UNSUPPORTED_CONF_PARAM               (0x07)
#define ASE_OP_RESP_REJECTED_CONF_PARAM                  (0x08)
#define ASE_OP_RESP_INVALID_CONF_PARAM                   (0x09)
#define ASE_OP_RESP_UNSUPPORTED_METADATA                 (0x0A)
#define ASE_OP_RESP_REJECTED_METADATA                    (0x0B)
#define ASE_OP_RESP_INVALID_METADATA                     (0x0C)
#define ASE_OP_RESP_INSUFFICIENT_RES                     (0x0D)
#define ASE_OP_RESP_UNSPECIFIED_ERROR                    (0x0E)

/* Operation Failure Response reason type on Audio Stream Endpoint*/
typedef uint8_t ASE_Op_Resp_Reason_t;
#define ASE_OP_RESP_NO_REASON                            (0x00)
#define ASE_OP_RESP_REASON_CODEC_ID                      (0x01)
#define ASE_OP_RESP_REASON_CODEC_SPEC_CONF               (0x02)
#define ASE_OP_RESP_REASON_SDU_INTERVAL                  (0x03)
#define ASE_OP_RESP_REASON_FRAMING                       (0x04)
#define ASE_OP_RESP_REASON_PHY                           (0x05)
#define ASE_OP_RESP_REASON_MAX_SDU                       (0x06)
#define ASE_OP_RESP_REASON_RTX_NB                        (0x07)
#define ASE_OP_RESP_REASON_MAX_TRANS_LATENCY             (0x08)
#define ASE_OP_RESP_REASON_PRES_DELAY                    (0x09)
#define ASE_OP_RESP_REASON_INV_ASE_CIS_MAP               (0x0A)

/* Target Latency Type */
typedef uint8_t Target_Latency_t;
#define TARGET_LATENCY_LOW                              (0x01)
#define TARGET_LATENCY_BALANCED_RELIABILITY             (0x02)
#define TARGET_LATENCY_HIGH_RELIABILITY                 (0x03)

/* Target Phy Type */
typedef uint8_t Target_Phy_t;
#define TARGET_LE_1M_PHY                                (0x01)
#define TARGET_LE_2M_PHY                                (0x02)
#define TARGET_LE_CODED_PHY                             (0x03)

/* Framing Type */
typedef uint8_t BAP_Framing_t;
#define BAP_FRAMING_UNFRAMED                            (0x00)
#define BAP_FRAMING_FRAMED                              (0x01)

/* Framing Type */
typedef uint8_t BAP_Packing_t;
#define BAP_PACKING_SEQUENTIAL                          (0x00)
#define BAP_PACKING_INTERLEAVED                         (0x01)

/* Preferred PHY Type
* Bitfied Server preferred value for the PHY parameter to be
* written by the client for this ASE in the Config QoS operation
*/
typedef uint8_t Preferred_PHY_t;
#define NO_PHY_PREFERRED                                (0x00)
#define LE_1M_PHY_PREFERRED                             (0x01)
#define LE_2M_PHY_PREFERRED                             (0x02)
#define LE_CODED_PHY_PREFERRED                          (0x04)

/* Audio Appearances */
#define APPEARANCE_GENERIC_AUDIO_SINK                   (0x0840)
#define APPEARANCE_STANDALONE_SPEAKER                   (0x0841)
#define APPEARANCE_SOUNDBAR                             (0x0842)
#define APPEARANCE_BOOKSHELF_SPEAKER                    (0x0843)
#define APPEARANCE_STANDMOUNTED_SPEAKER                 (0x0844)
#define APPEARANCE_SPEAKERPHONE                         (0x0845)
#define APPEARANCE_GENERIC_AUDIO_SOURCE                 (0x0880)
#define APPEARANCE_MICROPHONE                           (0x0881)
#define APPEARANCE_ALARM                                (0x0882)
#define APPEARANCE_BELL                                 (0x0883)
#define APPEARANCE_HORN                                 (0x0884)
#define APPEARANCE_BROADCASTING_DEVICE                  (0x0885)
#define APPEARANCE_SERVICE_DESK                         (0x0886)
#define APPEARANCE_KIOSK                                (0x0887)
#define APPEARANCE_BROADCASTING_ROOM                    (0x0888)
#define APPEARANCE_AUDITORIUM                           (0x0889)

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_TYPES_H */

