/**
  ******************************************************************************
  * @file    HAL/Src/stmac_app.c
  * @author  MCD-WBL Application Team
  * @brief   MAC MCPS Service Access Point
  *          This file provides definition of SAP-MCPS Methods
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "st_mac_802_15_4_svc.h"
#include "st_mac_802_15_4_core.h"
#include "st_mac_802_15_4_core_config.h"
#include "st_mac_802_15_4_mcps_svc.h"
#include "st_mac_buffer_management.h" // Buffer Management 

#include "mac_temporary.h"
// Public MAC interfaces    
#include "st_mac_802_15_4_types.h"

// SNPS MAC Interface
#include "mac_host_intf.h"
#include "common_mac.h"

// MAC log
#include "log_module.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* External variables --------------------------------------------------------*/
extern mac_ctx_st g_mac_ctx[MAC_NUMBER_OF_INSTANCE];
#if MAC_WRAPPER_LOG
uint32_t indice_req_number = 0;
uint32_t indice_cnf_number = 0;
#endif
/* External functions --------------------------------------------------------*/

/**********************************/
/* Incoming MCPS REQ (NWK -> MAC) */
/**********************************/
/**
 * @brief  This function is used to handle a MAC MCPS Data REQ as described
 *         in IEEE Std 802.15.4-2020 standard
 *
 * @param  MAC_dataReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MCPSDataReq( MAC_handle * st_mac_hndl ,const ST_MAC_dataReq_t * pDataReq )
{

  MAC_Status_t status = MAC_SUCCESS;
  mcps_data_req_params_st_t st_mcps_data_req_params;
  mcps_data_cfm_params_st_t st_mcps_data_cfm_params; // For invalid arguments

  st_mcps_data_req_params.enum_src_addrs_mode = ( mac_addrs_mode_enum_t ) pDataReq->src_addr_mode;
  st_mcps_data_req_params.enum_dstn_addrs_mode = ( mac_addrs_mode_enum_t ) pDataReq->dst_addr_mode;
  st_mcps_data_req_params.dstn_pan_id = READ_2_BYTES ( pDataReq->a_dst_PAN_id, 0 );

  if ( st_mcps_data_req_params.enum_dstn_addrs_mode == SHORT_ADDRS_16 )
  {
    MEMCPY ( st_mcps_data_req_params.dstn_addrs, pDataReq->dst_address.a_short_addr, SHORT_ADDRESS_LENGTH );
    }

  else if ( st_mcps_data_req_params.enum_dstn_addrs_mode == EXT_ADDRS_64 )
  {
    MEMCPY ( st_mcps_data_req_params.dstn_addrs, pDataReq->dst_address.a_extend_addr, EXT_ADDRESS_LENGTH );
  }
  
  else if ( st_mcps_data_req_params.enum_dstn_addrs_mode > EXT_ADDRS_64){
      st_mcps_data_cfm_params.timestamp = 0x00;
      st_mcps_data_cfm_params.msdu_hndl=pDataReq->msdu_handle;
      st_mcps_data_cfm_params.enum_data_tx_status = INVALID_PARAMETER;
      ST_MAC_handle_MCPSDataCnf(st_mac_hndl, &st_mcps_data_cfm_params);
      return MAC_INVALID_PARAMETER;
  }

  st_mcps_data_req_params.msdu_len = pDataReq->msdu_length;
  st_mcps_data_req_params.ptr_msdu = (uint8_t*)&(pDataReq->msduPtr);
  st_mcps_data_req_params.msdu_hndl = pDataReq->msdu_handle;
  st_mcps_data_req_params.tx_options = (( pDataReq->indirect_Tx & 0x1 ) << 2 )  | ( pDataReq->ack_Tx & 0x1 );
  st_mcps_data_req_params.enum_mcps_security_level = (sec_level_enum_t) pDataReq->security_level;
  if ( st_mcps_data_req_params.enum_mcps_security_level != 0 )
  {
#if SUPPORT_SEC
    st_mcps_data_req_params.key_id_mode = pDataReq->key_id_mode;
    switch ( st_mcps_data_req_params.key_id_mode )
    {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( st_mcps_data_req_params.key_src, pDataReq->a_key_source, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( st_mcps_data_req_params.key_src, pDataReq->a_key_source, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
      }

    st_mcps_data_req_params.key_index = pDataReq->key_index;
#else    
   st_mcps_data_req_params.enum_mcps_security_level = (sec_level_enum_t) 0;
#endif
    }

#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MCPSDataReq function\r\n");
  indice_req_number++;
#endif  
  /* CALL Data Send Req - Queue Req in SNPS Task */
  status = mcps_data_send_req ( &st_mcps_data_req_params, ( uint32_t ) &g_mac_ctx[0] );

  return status;

} /* End of MAC_MCPSDataReq */

/**
 * @brief  This function is used to handle a MAC MCPD Purge REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_purgeReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MCPSPurgeReq( MAC_handle * st_mac_hndl ,const ST_MAC_purgeReq_t * pPurgeReq )
{ 
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MCPSPurgeReq function\r\n");
#endif  
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_DATA_PURGE)
  mcps_purge_req((uint32_t) &g_mac_ctx[0], pPurgeReq->msdu_handle);
  
#else
  return MAC_NOT_SUPPORTED_STATUS;
#endif
  return MAC_SUCCESS;
} /* End of MAC_MCPSPurgeReq*/



void ST_MAC_handle_MCPSDataInd(void* mac_cntx_ptr, mcps_indicate_params_st_t* ptr_mcps_data_ind){
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_dataInd_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_DATA_IND_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  MAC_MEMSET(&mac_msg,0x00,sizeof(ST_MAC_dataInd_t));
  
  /*************************/
  /* Source Fields Section */
  /*************************/
  
  MAC_MEMCPY ( mac_msg.a_src_PAN_id, ( uint8_t * ) &ptr_mcps_data_ind->src_pan_id, g_SHORT_PAN_ID_LENGTH_c );
  
  /* enum_src_addrs_mode is a none-init Enum  - Need to add a converter ?*/
  mac_msg.src_addr_mode = ptr_mcps_data_ind->enum_src_addrs_mode;
  if ( ptr_mcps_data_ind->enum_src_addrs_mode == EXT_ADDRS_64 ) {
    MAC_MEMCPY ( mac_msg.src_address.a_extend_addr , ( uint8_t * )  ptr_mcps_data_ind->src_addrs, g_EXTENDED_ADDRESS_LENGTH_c );
  }
  else if ( ptr_mcps_data_ind->enum_src_addrs_mode == SHORT_ADDRS_16 ) { 
    MAC_MEMCPY ( mac_msg.src_address.a_short_addr, ( uint8_t * ) ptr_mcps_data_ind->src_addrs, g_SHORT_ADDRESS_LENGTH_c );
  }
  
  /******************************/
  /* Destination Fields Section */
  /******************************/
  
  /* enum_dstn_addrs_mode is a none-init Enum  - Need to add a converter ?*/
  MAC_MEMCPY ( mac_msg.a_dst_PAN_id, ( uint8_t * )&ptr_mcps_data_ind->dstn_pan_id, g_SHORT_PAN_ID_LENGTH_c );
  mac_msg.dst_addr_mode =  ptr_mcps_data_ind->enum_dstn_addrs_mode;
  if ( ptr_mcps_data_ind->enum_dstn_addrs_mode == EXT_ADDRS_64 ) {
    MAC_MEMCPY ( mac_msg.dst_address.a_extend_addr, ( uint8_t * ) ptr_mcps_data_ind->dstn_addrs, g_EXTENDED_ADDRESS_LENGTH_c );
  }
  else {
    MAC_MEMCPY ( mac_msg.dst_address.a_short_addr, ( uint8_t * ) ptr_mcps_data_ind->dstn_addrs, g_SHORT_ADDRESS_LENGTH_c );
  }
  
  /***********************/
  /* Data Fields Section */
  /***********************/
  mac_msg.msdu_length = ptr_mcps_data_ind->msdu_len;
  mac_msg.mpdu_link_quality = ptr_mcps_data_ind->mpdu_link_qlty;
  mac_msg.DSN = ptr_mcps_data_ind->dsn;
  MAC_WRITE_4_BYTES(mac_msg.a_time_stamp, 0, ptr_mcps_data_ind->timestamp);
  /* Security parameters */
  mac_msg.security_level = ptr_mcps_data_ind->enum_mcps_security_level;
  if (mac_msg.security_level != 0) {
#if SUPPORT_SEC
    mac_msg.key_id_mode = ptr_mcps_data_ind->key_id_mode;
    switch ( ptr_mcps_data_ind->key_id_mode ) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( mac_msg.a_key_source, ptr_mcps_data_ind->key_src, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( mac_msg.a_key_source, ptr_mcps_data_ind->key_src, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
     }
    mac_msg.key_index = ptr_mcps_data_ind->key_index;
#else    
   mac_msg.security_level = (sec_level_enum_t) 0;
#endif
  }
  mac_msg.rssi = 0xFF; /* Currently provided through SNPS Data Ind API - May we need to retrieve it from other context */
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming_with_payload(command_id, st_mac_hndl, sizeof(ST_MAC_dataInd_t), &mac_msg, ptr_mcps_data_ind->ptr_msdu, mac_msg.msdu_length);
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MCPSDataInd function\r\n");
#endif  
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MCPSDataInd failed, buffer issue\r\n");
  }
  // Done - Nothing to return 
  
} /* End of lcl_mcps_data_cfm */




/**
 * @brief  This function Callback is used to handle a MAC MCPD Purge CNF as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_purgeReq_t ref
 * @retval MAC_Status_t
 */
void ST_MAC_handle_MCPSDataPurgeCnf(void* mac_cntx_ptr, uint8_t msdu_hndl, uint8_t status){
 
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_purgeCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_PURGE_CNF_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.msdu_handle = msdu_hndl;
  mac_msg.status = status;
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_purgeCnf_t), &mac_msg);
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MCPSPurgeCnf function\r\n");
#endif  
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MCPSDataPurge failed, buffer issue\r\n");
  }
  // Done - Nothing to return 
}


void ST_MAC_handle_MCPSDataCnf(void* mac_cntx_ptr, mcps_data_cfm_params_st_t* ptr_mcps_data_cnf) {
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_dataCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_DATA_CNF_INT_MSG_ID;

  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  MAC_MEMSET(&mac_msg, 0x00, sizeof(ST_MAC_dataCnf_t));
  mac_msg.msdu_handle = ptr_mcps_data_cnf->msdu_hndl;
  mac_msg.status = ptr_mcps_data_cnf->enum_data_tx_status;

  /* Enqueue Standardized Incoming Message */
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_dataCnf_t), &mac_msg);

#if MAC_WRAPPER_LOG
    LOG_INFO_APP("MCPSDataCnf function, status=0x%02x, datareq=%d, datacnf=%d \r\n", ptr_mcps_data_cnf->enum_data_tx_status, indice_req_number, indice_cnf_number);
    indice_cnf_number++;
#endif  
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MCPSDataCnf failed, buffer issue\r\n");
  }
} /* End of lcl_mcps_data_cfm */
