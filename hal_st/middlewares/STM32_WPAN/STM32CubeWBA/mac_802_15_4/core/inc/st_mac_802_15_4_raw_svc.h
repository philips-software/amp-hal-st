/**
  ******************************************************************************
  * @file    st_mac_802_15_4_raw_svc.h
  * @author
  * @brief   Header for mac raw service
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

#ifndef _802_15_4_MAC_RAW_SVC_H_
#define _802_15_4_MAC_RAW_SVC_H_

/* Includes ------------------------------------------------------------------*/
#include "st_mac_802_15_4_types.h"

#include "mac_host_intf.h"

/* Exported defines ----------------------------------------------------------*/

/** @brief Max size of payload reception (CRC not included) */
#define ST_MAC_RAW_RX_PAYLOAD_MAX_SIZE 125

#define ST_MAC_RAW_RX_FRAME_MAX_SIZE ST_MAC_RAW_RX_PAYLOAD_MAX_SIZE + 2


/******************************************************************************/
/** @brief Holder for events, to be accessed through macros. In general, each events
 should have a set, clear, is_set macros.
 */
extern uint32_t mac_radio_evts;

/* Exported types ------------------------------------------------------------*/
/** @brief Defines the current operation of the radio
 */
typedef enum {
  ST_MAC_UNITIALIZED = 0,
  ST_MAC_IDLE,
  ST_MAC_TX,
  ST_MAC_RX,
  ST_MAC_ED,
  ST_MAC_CCA,
  ST_MAC_NOTIF,
  ST_MAC_UNKNOWN, //Keep last
} MAC_RAW_State_t;

/** @brief Defines the type of transmission
 */
typedef enum {
  ST_MAC_NONE_TX = 0,
  /* Standard TX with paylaod */
  ST_MAC_STANDARD_TX,
  /* Continuous PRBS noise */
  ST_MAC_CONTINUOUS_TX,
  /* Unmodulated continuous TX */
  ST_MAC_CONTINUOUS_WAVE,
  ST_MAC_UNKNOWN_TX, //keep last
} MAC_RAW_TX_Type_t;

/** @brief TX status
 */
typedef enum {
  TX_SUCCESS = 0,
  TX_ACK_TIMEOUT,
  TX_FAILED,
} MAC_RAW_TX_Status_t;

/** @brief TX status
 */
typedef enum {
  RX_SUCCESS = 0,
  RX_NO_PAYLOAD,
  RX_REJECTED,
  RX_FILTERED,
} MAC_RAW_RX_Status_t;

/* Raw MAC requests */
/** @brief Defines the structure holding Radio capabilities
 */
typedef struct {
  /*! CCA threshold range (dbm) */
  int8_t min_cca_threshold;
  int8_t max_cca_threshold;
  /*! Transmission power range (dbm) */
  int8_t min_tx_power;
  int8_t max_tx_power;
} ST_MAC_raw_caps_t;

/** @brief Defines the structure for sending one or more frames regardless of the
 format. Only the PHY preamble at the start and the FCS field at the end are added.
 */
typedef struct {
  /*! The channel on which to send to the frame */
  uint8_t channel_number;
  /*! Frame power (dbm) */
  int8_t power;
  /*! Type of TX */
  MAC_RAW_TX_Type_t tx_type;
  /*! Payload to send (case ST_MAC_STANDARD_TX only) */
  uint8_t payload[ST_MAC_RAW_RX_PAYLOAD_MAX_SIZE];
  /*! Payload length (CRC excluded) */
  uint8_t payload_len;
  /*! Number of frames to send */
  uint16_t frames_number;  //0: Infinite
  /*! Delay between each frames (in ms) */
  uint16_t delay_ms;
  /*! Stop next TX if current one failed (0: continue, otherwise stop) */
  uint8_t stopTx_if_failure;
} ST_MAC_raw_TX_start_t;

/** @brief Defines the structure for starting the radio in reception.
 */
typedef struct {
  /*! The channel on which to receive trames */
  uint8_t channel_number;
  /*! Reception duration in ms */
  uint32_t period; // 0: infinite
  /*! Max number of frames to receive; once reached stop the reception */
  uint16_t frames_number; // 0: infinite
} ST_MAC_raw_RX_start_t;

/** @brief Defines the structure to request a Clear Channel Assessement
 */
typedef struct {
  /*! The channel on which to perform the CCA */
  uint8_t channel_number;
  /*! CCA threshold in dbm */
  int8_t cca_threshold;
  /*! CCA result: 1: clear, 0: busy */
  uint8_t cca_result;
} ST_MAC_raw_CCA_t;

/** @brief Defines the structure to request an ED scan
 */
typedef struct {
  /*! Channel */
  uint8_t channel;
} ST_MAC_raw_EDscan_t;

/** @brief Defines the structure for an ED scan result
 */
typedef struct {
  /*! ED result */
  uint8_t ed;
} ST_MAC_rw_EDscan_result_t;

/** @brief Defines the structure for a single RX event
 */
typedef struct {
  /*! Status of Reception */
  MAC_RAW_RX_Status_t rx_status;
  /*! Pointer to payload of frame received */
  uint8_t *payload_ptr;
  /*! Size of frame received */
  uint8_t payload_len;
  /*! RSSI of frame received */
  int8_t rssi;
   /*! LQI of frame received */
  uint8_t lqi; 
} ST_MAC_raw_single_RX_event_t;

/** @brief Defines the structure for a signel TX event
 */
typedef struct {
  /*! Status of transmission */
  MAC_RAW_TX_Status_t tx_status;
  /*! Pointer to ACK if any (NULL otherwise */
  uint8_t *ack_ptr;
  /*! Size of ACK (0 if any) */
  uint16_t ack_length;
} ST_MAC_raw_single_TX_event_t;

/** @brief Defines callback init structure for optional events
in MAC Initialisation */
/* Callback when radio enter NOTIF state */
typedef void (*ST_MAC_raw_Notif_callback)(MAC_RAW_State_t state);
typedef void (*ST_MAC_raw_single_RX_cbk) ( const ST_MAC_raw_single_RX_event_t * p_RX_evt);
typedef void (*ST_MAC_raw_single_TX_cbk) ( const ST_MAC_raw_single_TX_event_t * p_TX_evt);

typedef struct
{
  ST_MAC_raw_Notif_callback  p_Notif;
  ST_MAC_raw_single_RX_cbk   p_RX_Done;
  ST_MAC_raw_single_TX_cbk   p_TX_Done;
} ST_MAC_Raw_event_callbacks_t;

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/* General */
MAC_Status_t ST_MAC_raw_init(ST_MAC_Raw_event_callbacks_t *p_callback);

/* Services */
MAC_Status_t ST_MAC_raw_get_caps(MAC_handle * st_mac_hndl , ST_MAC_raw_caps_t *pRawCaps );

MAC_Status_t ST_MAC_raw_start_TX( MAC_handle * st_mac_hndl,  const ST_MAC_raw_TX_start_t * pRawTXStartReq );
MAC_Status_t ST_MAC_raw_stop_TX(void);

MAC_Status_t ST_MAC_raw_start_RX(MAC_handle * st_mac_hndl, const ST_MAC_raw_RX_start_t * pRawRXStartReq );
MAC_Status_t ST_MAC_raw_stop_RX(void);

MAC_Status_t ST_MAC_raw_start_CCA(MAC_handle * st_mac_hndl, ST_MAC_raw_CCA_t * pRawCCAStartReq );

MAC_Status_t ST_MAC_raw_EDscan(MAC_handle * st_mac_hndl, const ST_MAC_raw_EDscan_t * pRawEDscanReq );
MAC_Status_t ST_MAC_raw_EDscan_get_result(MAC_handle * st_mac_hndl, const ST_MAC_rw_EDscan_result_t * pRawEDscanReq );

#endif /* _802_15_4_MAC_RAW_SVC_H_ */
