/**
  ******************************************************************************
  * @file    codec_mngr.h
  * @author  MCD Application Team
  * @brief   Header for managing Audio codec
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
#ifndef __CODEC_MNGR_H__
#define __CODEC_MNGR_H__

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "cmsis_compiler.h"

#include "audio_types.h"

/* Exported defines -----------------------------------------------------------*/

/* Configuration*/
#define MAX_PATH_ID             2       /* interface number : HCI + Shared RAM */

#define MAX_PATH_NB             4       /* max number of data path, a path may support multi channel */
#define MAX_CHANNEL_PER_PATH    4       /* max channel per data path */
#define MAX_CHANNEL             4       /* number of channel supported, can be <= MAX_PATH_NB*MAX_CHANNEL_PER_PATH */
#define MAX_ISO_GROUP_NB        1       /* linked to LL capabilities */
#define MAX_ISO_STRM_PER_GRP    2       /* linked to LL capabilities */

#define SUPPORT_LC3             1

#if SUPPORT_LC3 == 1
  #define SUPPORT_LC3_ENCODER   1       /* LC3 encoder support */
  #define SUPPORT_LC3_DECODER   1       /* LC3 decoder support */
  #define MIN_LC3_NBYTES        20      /* LC3 minimum encoded frame */
  #define MAX_LC3_NBYTES        155     /* LC3 maximum encoded frame */

  /* Codec max Million Cycle Per Second for each frequency listed codec capabilities LTV structure, 0 if unsupported
   * Values for codec V1.4
   *                   kHz:      8,     N/A,    16,     N/A,    24,     32,     44.1,    48,    N/A----------
   */
  #define LC3_MCPS_ENC_75MS     {15,     0,     19,     0,      23,     27,     35,      34,     0, 0, 0, 0, 0}
  #define LC3_MCPS_ENC_10MS     {13,     0,     17,     0,      22,     26,     30,      30,     0, 0, 0, 0, 0}
  #define LC3_MCPS_DEC_75MS     {6,      0,     9,      0,      13,     18,     23,      23,     0, 0, 0, 0, 0}
  #define LC3_MCPS_DEC_10MS     {5,      0,     8,      0,      12,     17,     22,      22,     0, 0, 0, 0, 0}

  #include "LC3.h"
  #define CODEC_GET_TOTAL_SESSION_BUFFER_SIZE(num_session)      (num_session * LC3_SESSION_STRUCT_SIZE)

  #if SUPPORT_LC3_ENCODER != 0
  #include "LC3_encoder.h"
  #define CODEC_GET_TOTAL_ENCODER_CH_BUFFER_SIZE(num_enc_ch)    (num_enc_ch * LC3_ENCODER_STRUCT_SIZE)
  #endif

  #if SUPPORT_LC3_DECODER != 0
  #include "LC3_decoder.h"
  #define CODEC_GET_TOTAL_DECODER_CH_BUFFER_SIZE(num_dec_ch)    (num_dec_ch * LC3_DECODER_STRUCT_SIZE)
  #endif

#endif /*SUPPORT_LC3*/

#define SUPPORTED_PRERECEIVED_PACKET    1   /* FIFO margin in media packet size */

#define LL_SETUP_TIME_WINDOWS_US        50  /* Margin given to the setup time for defining a forbidden windows */

#define ISO_DATA_PACK_MAX_SIZE          300 /* MAX value of an SDU supported by the controller before fragmentation */

#define USE_SW_SYNC_METHOD              1   /* Synchronization method used */

#define CODEC_DEBUG_GPIO                1

/* Exported typedef -----------------------------------------------------------*/

/* Vendor specific parameters used in HCI Configure Data Path command */
typedef struct
{
  uint8_t SampleDepth;          /* depth in bits of the sample */
  uint8_t Decimation;           /* pointer increment between two samples of the same channel */
} CODEC_DataPathParam_t;

#define CONFIGURE_DATA_PATH_CONFIG_LEN sizeof(CODEC_DataPathParam_t)

/* Codec Mode */
typedef uint8_t CODEC_Mode_t;
#define CODEC_MODE_DEFAULT      (0x00)

/* Structure for allocating RAM used by the LC3 codec */
typedef struct
{
  uint8_t NumSession;           /* number of supported LC3 session */
  uint8_t NumDecChannel;         /* number of supported LC3 decoding channel */
  uint8_t NumEncChannel;        /* number of supported LC3 encoding channel */

  void *pSessionStart;          /* pointer to the allocated RAM for sessions */
  void *pDecChannelStart;       /* pointer to the allocated RAM for decoding channels */
  void *pEncChannelStart;       /* pointer to the allocated RAM for encoding channels */
} CODEC_LC3Config_t;

/* Structure containing clock tree information needed for the frequency corrector */
typedef struct
{
  uint32_t PLLTargetFreq;       /* theoretical frequency of the PLL output to fit the audio peripheral requirement */
  float    VCOInputFreq;        /* frequency at the VCO input */
  uint32_t PLLOutputDiv;        /* divider applied to the VCO output */
} AUDIO_PLLConfig_t;

/* Structure for HCI Setup ISO Data Path command */
typedef __PACKED_STRUCT
{
  uint16_t con_hdle;
  uint8_t direction;
  uint8_t path_ID;
  uint8_t codec_ID[5];
  uint8_t controller_delay[3];
  uint8_t codec_conf_len;
  uint8_t codec_conf[19];
} CODEC_SetupIsoDataPathCmd_t;

/* Structure for HCI Configure Data Path command */
typedef __PACKED_STRUCT
{
  uint8_t direction;
  uint8_t path_ID;
  uint8_t config_len;
  uint8_t sample_depth;
  uint8_t decimation;
} CODEC_ConfigureDataPathCmd_t;

/* Data path types */
typedef enum
{
  DATA_PATH_HCI_LEGACY   =      0,
  DATA_PATH_CIRCULAR_BUF =      1
} CODEC_DataPath_t;

/* Status of the codec operation */
typedef enum
{
  CODEC_STATUS_SUCCESS  =       0,
  CODEC_STATUS_ERROR    =       1
} codec_status_t;

/* Status of the codec when received packet from link layer
  Busy means the packet has been handled but memory are currently full */
typedef enum
{
  CODEC_RCV_STATUS_OK =    0,
  CODEC_RCV_STATUS_FAIL =  1,
  CODEC_RCV_STATUS_BUSY =  2
} codec_rcv_status_t;


/*********************************************************************************************/
/******************************** interface with local application ***************************/
/*********************************************************************************************/

/* Init and process */
/**
  * @brief Initialize codec manager with given memories for codec and data buffering
  * @param media_packet_pool_size : size in bytes of the provided pool
  * @param media_packet_pool : pointer to the provided pool
  * @param codecRAMConfig : pointer to the RAM dedicated to the LC3 codec
  * @param margin_processing_us : margin is microseconds added to the controller delay
  *                               for including higher priority interrupt latency
  * @param rf_max_setup_time_us : maximum timing in microseconds measured from the radio interrupt to the beginning of
  *                               the corresponding ISO event (radio preparation). Added as an extra latency at source
  * @param codec_mode : configuration of the codec manager behavior, see CODEC_Mode_t
  * @retval status
  */
codec_status_t CODEC_ManagerInit(uint32_t media_packet_pool_size,
                                 uint8_t *media_packet_pool,
                                 CODEC_LC3Config_t *codecRAMConfig,
                                 uint16_t margin_processing_us,
                                 uint16_t rf_max_setup_time_us,
                                 CODEC_Mode_t codec_mode);

/**
  * @brief  Reset Codec manager states and memories
  * @param
  * @retval status
  */
codec_status_t CODEC_ManagerReset( void );

/**
  * @brief  Periodic Codec Processes that must be called after CODEC_ProcessReq has been raised
  * @param
  * @retval
  */
void CODEC_ManagerProcess( void );

/*---------------- local interfaces for audio triggering and data providing -----------------*/
/**
  * @brief  Register a function that is called once for triggering audio interface in order to respect a controller delay
  * @note At source, the trigger happen one controller delay plus one media packet before an anchor point
  * @note At sink, the trigger happen one controller delay after the SDU reference timing
  * @param path_id : path id
  * @param dir : direction (input or output)
  * @param clbk_function : callback function that will be called after de CIS/BIS is established
  * @retval status
  */
codec_status_t CODEC_RegisterTriggerClbk(uint8_t path_id, uint8_t direction, void clbk_function(void));

/**
  * @brief  Notify the codec manager that new data is available on a given data path
  * @param iso_con_hdl : isochronous connection handle
  * @param path_id : path id
  * @param pdata : pointer to the data, format should be coherent with the configured data path
  * @retval none
  */
void CODEC_SendData(uint16_t iso_con_hdl, uint8_t path_id, void* pdata);

/**
  * @brief  Notify the codec manager that new data is needed on a given data path
  * @param iso_con_hdl : isochronous connection handle
  * @param path_id : path id
  * @param pdata : pointer to the buffer to be filled, format should be coherent with the configured data path
  * @retval none
  */
void CODEC_ReceiveData(uint16_t iso_con_hdl, uint8_t path_id, void* pdata);

/**
  * @brief Weak function called by the codec manager after finishing to process data on sink path
  * @note  Could be redefined for triggering other processing on that data at the application level
  * @param iso_con_hdl: isochronous connection handle
  * @param pdecoded_data : pointer to the decoded data
  * @retval none
  */
void CODEC_NotifyDataReady(uint16_t iso_con_hdl, void* pdecoded_data);

/*--------------------------------- integration with clock ----------------------------------*/
/**
  * @brief Function for notifying the codec manager that an event on a specific id has happen
  * @param id : identifier of the requestor
  * @retval None
  */
void CODEC_CLK_trigger_event_notify(uint8_t id);

/**
  * @brief Function for initializing the clock corrector by adjusting PLL N fractional value
  * @note A first drift correction is performed after initialMinSampling milliseconds where clocks may drift up to 550ppm
  * @note Then continuous correction is performed every minSampling milliseconds
  * @param PLLConfig : PLL configuration
  * @param initialMinSampling : first sampling period in ms for generating the first correction
  * @param minSampling : sampling period in ms used for continuous drift adjustment
  * @retval None
  */
void AUDIO_InitializeClockCorrector(AUDIO_PLLConfig_t *PLLConfig, uint32_t initialMinSampling, uint32_t minSampling);

/**
  * @brief Function for resetting corrector state
  * @retval None
  */
void AUDIO_DeinitializeClockCorrector( void );

/*********************************************************************************************/
/******************************** interface with controller **********************************/
/*********************************************************************************************/

/*--------------------------------------  HCI commands --------------------------------------*/
/**
  * @brief Function used for reading supported codecs, following the HCI standard
  * @param *num_stdr : pointer for returning the number of standards supported codec
  * @param **stdr_codec : pointer for returning an array containing standards codec information
  * @param *num_vdr : pointer for returning the number of vendor supported codec
  * @param **vdr_codec : pointer for returning an array containing vendor codec information
  * @retval HCI status
  */
uint8_t CODEC_ReadLocalSupportedCodecsV2(uint8_t *num_stdr, uint8_t **stdr_codec, uint8_t *num_vdr, uint8_t **vdr_codec);

/**
  * @brief Function used for reading codec capabilities, following the HCI standard
  * @param *codecID : array defining the Codec ID
  * @param transport : Bluetooth transport type
  * @param dir : direction of the codec
  * @param *num_codec_capabilities : pointer for returning number of capabilities structures
  * @param **codec_capability : pointer for returning an array of structure containing the capabilities
  * @retval HCI status
  */
uint8_t CODEC_ReadSupportedCodecCapabilies(uint8_t* codecID, uint8_t transport, uint8_t dir,
                                           uint8_t* num_codec_capabilities, uint8_t **codec_capability);

/**
  * @brief  Function used for reading controller delay range
  * @note This function don't handle case of parallel process, that may impact the minimum controller delay value
  * @note Minimum values are computed from the Codec MCPS and CPU target frequency
  * @note Maximum value is defined by the allocated RAM
  * @param *codecID : array defining the Codec ID
  * @param transport : Bluetooth transport type
  * @param dir : direction of the codec
  * @param codec_conf_len : codec configuration array length
  * @param *codec_conf : pointer to the codec configuration structure
  * @param *min_controller_delay : pointer to the minimum returned controller delay value
  * @param *max_controller_delay : pointer to the maximum returned controller delay value
  * @retval HCI status
  */
uint8_t CODEC_ReadLocalSupportedControllerDelay(uint8_t* codecID, uint8_t transport, uint8_t dir,
                                                uint8_t codec_conf_len, uint8_t * codec_conf,
                                                uint32_t* min_controller_delay, uint32_t* max_controller_delay);

/**
  * @brief  Parse HCI buffer for the HCI configure data path command, this function also reset the interface state
  * @param *hciparam : pointer to an array containing HCI parameters
  * @retval HCI status
  */
uint8_t CODEC_ConfigureDataPath(uint8_t *hciparam);

/**
  * @brief HCI setup ISO data path command used for path and codec initialization
  * @note This function initialize the data path, and the codec if present and used
  * @param *hciparam : pointer to an array containing HCI parameters
  * @retval HCI status
  */
uint8_t CODEC_SetupIsoDataPath(uint8_t *hciparam);

/**
  * @brief  HCI remove data path command for removing a data path defined by CODEC_SetupIsoDataPath
  * @param *hciparam : pointer to an array containing HCI parameters
  * @retval HCI status
  */
uint8_t CODEC_RemoveIsoDataPath(uint8_t *hciparam);

/*--------------------------------- Callbacks and events ------------------------------------*/
/**
  * @brief Function for handling vendor specific HCI event containing anchor point and calibration callback timestamp
  * @param group_id : specifies if concerning either BIG or CIG
  * @param next_anchor_point : value of the next anchor point
  * @param timestamp : timestamp of the previous sync event
  * @param next_sdu_delivery_timeout : timestamp of the limit time for providing an sdu to the link layer
  * @retval None
  */
void AUDIO_SyncEventClbk(uint8_t group_id, uint32_t next_anchor_point, uint32_t timestamp,
                         uint32_t next_sdu_delivery_timeout);

/**
  * @brief Calibration callback generated by the link layer
  * @note Used for mapping LinkLayer timestamps to host timestamps. This callback is in an interrupt context
  * @param timestamp : timestamp of the event based on linklayer clock
  * @retval None
  */
void AUDIO_CalibrationClbk(uint32_t timestamp);

/**
  * @brief Function for notifying the codec manager that a ISO group has been created
  * @note if the group already exists, the connection handle is added to that group
  * @param type : 0 for CIS and 1 for BIS
  * @param id : id of the CIG or BIG
  * @param num_str : streams numbers
  * @param iso_con_hdl : pointer to an array of ISO connection handles of size num_str
  * @param interval: interval of the RF event given in link layer unit
  * @param is_peripheral : set to 1 if the CIG group is on the peripheral side, 0 otherwise
  * @param m2s_transport_latency : master to slave transport latency
  * @param s2m_transport_latency : slave to master transport latency
  * @retval None
  */
void AUDIO_RegisterGroup(uint8_t type, uint8_t id, uint8_t num_str, uint16_t* iso_con_hdl, uint16_t interval,
                         uint8_t is_peripheral, uint32_t m2s_transport_latency, uint32_t s2m_transport_latency);

/**
  * @brief Notify the codec manager that a ISO group has been killed
  * @param type : 0 for CIS and 1 for BIS
  * @param id : id of the CIG or BIG
  * @retval None
  */
void AUDIO_UnregisterGroup(uint8_t type, uint8_t id);

/*-------------------------------------- Audio Data -----------------------------------------*/
/**
  * @brief  Function for receiving a media packet from the Link Layer
  * @param list of HCI ISO data params
  * @retval status of type codec_rcv_status_t
  */
codec_rcv_status_t CODEC_ReceiveMediaPacket(uint16_t iso_con_hdl, uint8_t pb_flag, uint8_t ts_flag, uint32_t timestamp,
                                  uint16_t PSN, uint8_t pkt_status_flag, uint16_t len, uint8_t* pdata);

#endif /* __CODEC_MNGR_H__ */
