/**
  ******************************************************************************
  * @file    st_mac_802_15_4_core.c
  * @author  MCD-WBL Application Team
  * @brief   Mac Core Implementation .
  *          This file provides mac manager exposed to upper layer through
  *          standardized Interface
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
#include "st_mac_802_15_4_core.h"
#include "st_mac_802_15_4_core_config.h"
#include "st_mac_802_15_4_svc.h"
#include "st_mac_802_15_4_sap.h"
#include "stm32wbaxx_ll_system.h"
#include "st_mac_802_15_4_types.h"
#include "st_mac_queue.h"
#include "mac_temporary.h" // Usefull before integration activities

#include "common_types.h"   // SNPS Common types.
#include "mac_host_intf.h" // SNPS Mac Host Interface

#include "st_mac_buffer_management.h" // Buffer Management 

#include "common_mac.h"
#include "ral.h"

/* Private typedef -----------------------------------------------------------*/
// Struct to store current MAC config
// Set during Init
// Struct should be clean after a device reset.
typedef struct ST_MAC_Core_Config_tag {
  ST_MAC_callbacks_t * mac_callbacks;
  st_mac_queue_t incomingMacBufferQueue; // Queue To handle message from Radio Transceiver to Upper layer
  st_mac_queue_t outgoingMacBufferQueue;
  } ST_MAC_Core_Config_t;

/* Private define ------------------------------------------------------------*/    
#define MAX_RADIO_INTERFACE 0x01
#define UNKNOWN_RADIO_INTERFACE MAX_RADIO_INTERFACE
/* Private macro -------------------------------------------------------------*/
#define STUFF_VALUE 0xFF
/* Private variables ---------------------------------------------------------*/
ST_MAC_Core_Config_t st_mac_config;

const uint8_t g_command_size[] = {
                                  sizeof(ST_MAC_associateReq_t),
                                  sizeof(ST_MAC_disassociateReq_t),
                                  sizeof(ST_MAC_getReq_t),
                                  sizeof(ST_MAC_resetReq_t),
                                  sizeof(ST_MAC_rxEnableReq_t),
                                  sizeof(ST_MAC_scanReq_t),
                                  sizeof(ST_MAC_setReq_t),
                                  sizeof(ST_MAC_startReq_t),
                                  sizeof(ST_MAC_pollReq_t),
                                  sizeof(ST_MAC_associateRes_t),
                                  sizeof(ST_MAC_orphanRes_t),
                                  sizeof(ST_MAC_dataReq_t),
                                  sizeof(ST_MAC_purgeReq_t),
                                  sizeof(ST_MAC_beaconReq_t),
                                  sizeof(ST_MAC_getPwrInfoTableReq_t),
                                  sizeof(ST_MAC_setPwrInfoTableReq_t)
};

mac_ctx_st * mac_radio_context[MAX_RADIO_INTERFACE];

st_mlme_pib_t st_mlme_pib;

extern bool system_startup_done;

// required during SNPS MAC Init
// TODO : Check LL implementation strategy

/* Private function prototypes -----------------------------------------------*/
static void ST_MAC_setDefaultPibAttr(void);


/* Private functions ---------------------------------------------------------*/

static void ST_MAC_setDefaultPibAttr(void)
{
  uint32_t mac_otInstance;
  
  otPlatRadioGetTransmitPower((otInstance*)&mac_otInstance, &st_mlme_pib.txPwr);
}

/* Wrapper Util Methode Implementations */
MAC_handle ST_MAC_get_mac_interface(void * mac_cntx_ptr) {
  MAC_handle id;
  for ( id = 0; id<MAX_RADIO_INTERFACE;id++ )
    if (mac_radio_context[id] == (mac_ctx_st *)mac_cntx_ptr )
      break;
  return (id==MAX_RADIO_INTERFACE)?id:UNKNOWN_RADIO_INTERFACE;
}

uint8_t mac_get_pib_attribute_len(void* mac_cntx_ptr,uint8_t pib_attr_id)
{
  uint8_t attr_len = 0;
  switch(pib_attr_id) {
      case PAN_ID_ID:
      case SHORT_ADDR_ID:
      case PERSISTEN_TIME_ID:
      case COORD_SHRT_ADDR_ID:
      case SYNC_SYMB_OFFS_ID:
      case EBR_PRCNT_FLTR_ID:
      case MIB_EXPIRY_INTRVL_ID:
      case PAN_COORD_SHRT_ADDR_ID:
        attr_len = 2;
      break;
      case MAX_FRAME_WAIT_TIME_ID:
      case FRM_CNTR_ID:
        attr_len = 4;
      break;
      case COORD_EXT_ADDR_ID:
      case CUSTOM_IEEE_EXTD_ADDRS:
        attr_len = 8;
      break;
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_SEND_BEACON)
      case BEACON_PAY_ID:
        attr_len = ((mac_ctx_st*)mac_cntx_ptr)->mlme_pib.mac_bcon_payld_len;
        break;

      case MIB_JOINING_IEEE_LIST_ID:
        // To be clarify - from where we can retrieve the value
        attr_len = ((mac_ctx_st*)mac_cntx_ptr)->mlme_pib.mib_join_list.list_count * g_EXTENDED_ADDRESS_LENGTH_c;
        break;
#endif
      case BEACON_TX_TIME_ID:
        attr_len = 3;
        break;
      case	ACK_WAIT_DUR_ID :
      case	ASSOC_PAN_COORD_ID:
      case	ASSOC_PERMIT_ID:
      case	AUTO_REQ_ID:
      case	BECON_PAY_LEN_ID:
      case	BEACON_ORDER_ID:
      case	BSN_ID:
      case	DSN_ID:
      case	MAX_BE_ID:
      case	MAX_CSMA_ID:
      case	MAX_FRAME_RETRY_ID:
      case	MIN_BE_ID:
      case	PROMIS_MODE_ID:
      case	RSP_WAIT_TIME_ID:
      case	RX_WHEN_IDLE_ID:
      case	SEC_ENABLED_ID:
      case	SUPER_FRAME_ORD_ID:
      case	TIME_STAMP_SUPP_ID:
      case	PHY_CHANNEL_ID:
      case	PHY_CHANNEL_SUPPORTED:
      case	PHY_CCA_MODE:
      case	PHY_CURRENT_PAGE:
      case	PHY_MAX_FRAME_DUR:
      case	PHY_SHR_DUR:
      case	PHY_SYMBOLS_PER_OCTET:
      case	KEY_TBL_ID:
      case	DEVICE_TBL_ID:
      case	SEC_LVL_TBL_ID:
      case	AUTO_REQ_SEC_LVL_ID:
      case	AUTO_REQ_KEY_MODE_ID:
      case	AUTO_REQ_KEY_SRC_ID:
      case	AUTO_REQ_KEY_INDX_ID:
      case	DFLT_KEY_SRC_ID:
      case	EB_HDR_IE_LIST_ID:
      case	EB_PYLD_IE_LIST_ID:
      case	EB_FLTR_Enbld_ID:
      case	EBSN_ID:
      case	EB_AUTO_SA_ID:
      case	EBR_PRMT_JOIN_ID:
      case	EBR_FLTRS_ID:
      case	EBR_LQ_ID:
      case	EBR_AUTO_RSPND_ID:
      case	MIB_JOINING_POLICY_ID:
      case	MIB_EXPIRY_INTRVL_COUNTDOWN_ID:
      case      g_PHY_TRANSMIT_POWER_c:
        attr_len = 1;
    default:
    break;
    }
  return attr_len;
}

MAC_Status_t ST_MAC_enqueue_radio_Incoming(uint8_t command_id, MAC_handle st_mac_hndl, uint8_t req_len, void * reqPtr) {

  uint8_t  macBufferId;				/* to hold the MAC Req buffer ID */
  uint8_t* p_macBuffer;				/* pointer to the MAC buffer */
   
  // Enqueue Current Request
  macBufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_SIZE_c);
  if (macBufferId == NO_AVAILABLE_BUFFER) {
    return MAC_ERROR;
  } else {
    /* Get the pointer to the buffer allocated for the MAC message */
    p_macBuffer = buffMgmt_getBufferPointer(macBufferId);
    /* Fill the message */
    p_macBuffer[0] = command_id;
    p_macBuffer[1] = st_mac_hndl;
    p_macBuffer[2] = req_len;
    p_macBuffer[3] = STUFF_VALUE;
    MAC_MEMCPY( p_macBuffer+4, (uint8_t *)reqPtr, req_len );
    
    /* Enqueue Buffer in */
    if (QUEUE_FULL == Enqueue( st_mac_config.outgoingMacBufferQueue, macBufferId )) {
      buffMgmt_freeBuffer(macBufferId, 00);
      MacSys_SemaphoreSet();
      return MAC_ERROR;
    }
    // Add sequence task to process each time we add element in the radio queue
    MacSys_SemaphoreSet();
  }

  
      
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_enqueue_radio_Incoming_with_payload(uint8_t command_id, MAC_handle st_mac_hndl, uint8_t req_len, void * reqPtr, uint8_t * payload, uint8_t payload_len) {

  uint8_t  macBufferId;				/* to hold the MAC Req buffer ID */
  uint8_t* p_macBuffer;				/* pointer to the MAC buffer */
   
  // Enqueue Current Request
  macBufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_SIZE_c);
  if (macBufferId == NO_AVAILABLE_BUFFER) {
    return MAC_ERROR;
  } else {
    /* Get the pointer to the buffer allocated for the MAC message */
    p_macBuffer = buffMgmt_getBufferPointer(macBufferId);
    /* Fill the message */
    p_macBuffer[0] = command_id;
    p_macBuffer[1] = st_mac_hndl;
    p_macBuffer[2] = req_len + payload_len - 1; // First payload byte belongs to the Cnf / Int structue.
    p_macBuffer[3] = STUFF_VALUE;
    /* Payload is assumed to be @ then end of the structure: payload field*/
    MAC_MEMCPY( p_macBuffer+4, (uint8_t *)reqPtr, req_len-1 );
    uint8_t * targetMem = p_macBuffer+4+req_len-1;
    MAC_MEMCPY( targetMem, (uint8_t *)payload, payload_len );//Keil here use ble_memcpy
#if 0
    ST_MAC_dataInd_t * mac_msg;
    if ( command_id == ST_MAC_DATA_IND_INT_MSG_ID ) {
      mac_msg = (ST_MAC_dataInd_t *)(targetMem-req_len+1);
      mac_msg->rssi = 0xF3;
    }
#endif
    /* Enqueue Buffer in */
    if (QUEUE_FULL == Enqueue( st_mac_config.outgoingMacBufferQueue, macBufferId )) {
      buffMgmt_freeBuffer(macBufferId, 00);
      MacSys_SemaphoreSet();
      return MAC_ERROR;
    }
    // Add sequence task to process each time we add element in the radio queue
    MacSys_SemaphoreSet();
  }
  

  return MAC_SUCCESS;
}

uint8_t ST_MAC_enqueue_req(uint8_t command_id, MAC_handle st_mac_hndl, uint8_t req_len, void * reqPtr) {

  uint8_t  macBufferId;				/* to hold the MAC Req buffer ID */
  uint8_t* p_macBuffer;				/* pointer to the MAC buffer */
  
  uint8_t cPayloadLength;
  ST_MAC_dataReq_t * pRequest;
  
  // Check if command ID is supported and if req_len relevant to Command ID
  if (mac_command_validate(command_id, req_len) != MAC_SUCCESS) {
    return MAC_ERROR;
  }
  
  // Enqueue Current Request
  macBufferId = buffMgmt_allocateBuffer(g_LARGE_BUFFER_SIZE_c);
  if (macBufferId == NO_AVAILABLE_BUFFER) {
    return MAC_ERROR;
  } else {
    /* Get the pointer to the buffer allocated for the MAC message */
    p_macBuffer = buffMgmt_getBufferPointer(macBufferId);
   
    /* Fill the message */
    if (command_id == ST_MAC_DATA_REQ_INT_CMD_ID) {     
      pRequest = ( ST_MAC_dataReq_t * )reqPtr;
      cPayloadLength = pRequest->msdu_length;
      
      /* Verify if not overflow */
      /* do not take into account the 4 bytes due to void* on serialized structure */
      if ( ( req_len + cPayloadLength ) > g_LARGE_BUFFER_SIZE_c) {
        cPayloadLength = g_LARGE_BUFFER_SIZE_c - req_len;
      }
      
      /* Fill the header */
      p_macBuffer[0] = command_id;
      p_macBuffer[1] = st_mac_hndl;
      p_macBuffer[2] = req_len;
      p_macBuffer[3] = STUFF_VALUE;
      
      /* Update Request */
      pRequest->msdu_length = cPayloadLength;
      
      /* Fill the Request, removing 4 bytes due to void* on serialized structure */
      MAC_MEMCPY( p_macBuffer+4, (uint8_t *)pRequest, req_len-4 );
      
      /* Fill the payload */
      MAC_MEMCPY( p_macBuffer+req_len, pRequest->msduPtr, cPayloadLength );
    } else {
      /* Fill the message */
      p_macBuffer[0] = command_id;
      p_macBuffer[1] = st_mac_hndl;
      p_macBuffer[2] = req_len;
      p_macBuffer[3] = STUFF_VALUE;
      MAC_MEMCPY( &p_macBuffer[4], (uint8_t *)reqPtr, req_len );
    }
    
    /* Enqueue Buffer in */
    if (QUEUE_FULL == Enqueue( st_mac_config.incomingMacBufferQueue, macBufferId )) {
      buffMgmt_freeBuffer(macBufferId, 00); 
      MacSys_SemaphoreSet();
      return MAC_ERROR;
    }
    // Add sequence task to process each time we add element in the radio queue
    MacSys_SemaphoreSet();
    
    return MAC_SUCCESS;
  }

    
}

/* External variables --------------------------------------------------------*/

uint8_t ga_Buffer_Link_Array[g_TOTAL_NUMBER_OF_BUFFERS_c];
uint8_t ga_Heap[(g_TOTAL_NUMBER_OF_SMALL_BUFFERS_c * g_SMALL_BUFFER_SIZE_c) +
(g_TOTAL_NUMBER_OF_LARGE_BUFFERS_c * g_LARGE_BUFFER_SIZE_c)];


struct mac_dispatch_tbl mac_user_dispatch_tbl = {
  .mlme_bcon_notfy = ST_MAC_handle_MLMEBeaconNotifyInd,
  .mlme_get_cfm = ST_MAC_handle_MLMEGetCnf,
  .mlme_rst_cfm = ST_MAC_handle_MLMEResetCnf,
  .mlme_scn_cfm = ST_MAC_handle_MLMEScanCnf,
  .mlme_set_cfm = ST_MAC_handle_MLMESetCnf,
  .mcps_data_ind = ST_MAC_handle_MCPSDataInd,
  .mlme_sync_loss_ind = ST_MAC_handle_MLMESyncLossInd,
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_START_PRIM)
  .mlme_strt_cfm = ST_MAC_handle_MLMEStartCnf,
#endif /* (FFD_DEVICE_CONFIG || RFD_SUPPORT_START_PRIM) */
  .mlme_poll_cfm = ST_MAC_handle_MLMEDataPollCnf,
  .mcps_data_cfm = ST_MAC_handle_MCPSDataCnf,
#if (FFD_DEVICE_CONFIG)
  .mlme_poll_ind = ST_MAC_handle_MLMEDataPollInd,
#endif
  .mlme_assoc_cfm = ST_MAC_handle_MLMEAssociationCnf,
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_ASSOCIATION_IND_RSP)
  .mlme_assoc_ind = ST_MAC_handle_MLMEAssociationInd,
#endif /* (FFD_DEVICE_CONFIG || RFD_SUPPORT_ASSOCIATION_IND_RSP) */
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_DATA_PURGE)
  .mcps_purge_cfm = ST_MAC_handle_MCPSDataPurgeCnf,
#endif /* (FFD_DEVICE_CONFIG || RFD_SUPPORT_DATA_PURGE) */
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_ORPHAN_IND_RSP)
  .mlme_orphan_ind = ST_MAC_handle_MLMEOrphanInd,
#endif /* (FFD_DEVICE_CONFIG || RFD_SUPPORT_ORPHAN_IND_RSP) */
  .mlme_comm_status = ST_MAC_handle_MLMEComStatusInd,
  .mlme_set_pwr_info_table_cfm = ST_MAC_handle_MLMESetPwrInfoTableCnf,
  .mlme_get_pwr_info_table_cfm = ST_MAC_handle_MLMEGetPwrInfoTableCnf,
  .mlme_disassoc_cfm = ST_MAC_handle_MLMEDisassociationCnf,
  .mlme_disassoc_ind = ST_MAC_handle_MLMEDisassociationInd,
  .mlme_rx_enable_cfm = ST_MAC_handle_MLMERxEnableCnf,
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_SEND_BEACON)
  .mlme_bcon_req_ind = ST_MAC_handle_MLMEBeaconReqInd,
  .mlme_bcon_cfm = ST_MAC_handle_MLMEBeaconCnf,
#endif /* (FFD_DEVICE_CONFIG || RFD_SUPPORT_SEND_BEACON) */
};

ST_MAC_Core_Status_t st_mac_core_status;
ST_MAC_PIB_t    st_mac_pib;


/** @brief  Interface to access m_SMALL_BUFFER_SIZE_c from lib */
const uint8_t g_Small_Buffer_Size_c = g_SMALL_BUFFER_SIZE_c;

/** @brief  Interface to access m_LARGE_BUFFER_SIZE_c from lib */
const uint8_t g_Large_Buffer_Size_c = g_LARGE_BUFFER_SIZE_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_SMALL_BUFFERS_c from lib */
const uint8_t g_Total_Number_Of_Small_Buffers_c = g_TOTAL_NUMBER_OF_SMALL_BUFFERS_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_LARGE_BUFFERS_c from lib */
const uint8_t g_Total_Number_Of_Large_Buffers_c = g_TOTAL_NUMBER_OF_LARGE_BUFFERS_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_BUFFERS_c from lib */
const uint8_t g_Total_Number_Of_Buffers_c = g_TOTAL_NUMBER_OF_BUFFERS_c;


/* External functions --------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*   SYSTEM Methods                                                           */
/*----------------------------------------------------------------------------*/
void ST_MAC_preInit(void)
{
  uint32_t mac_hndl; // Ref not stored - Mac Instance to be managed Here.
  struct mac_dispatch_tbl* dispatch_tbl = &mac_user_dispatch_tbl;

  mac_init(&mac_hndl, dispatch_tbl);
  
  ral_set_frm_pend_bit_for_acks(FALSE);
  
  otPlatRadioSetCcaEnergyDetectThreshold((otInstance*) &mac_hndl, CCA_THRESHOLD);
}

MAC_Status_t ST_MAC_init(ST_MAC_callbacks_t * mac_callbacks) {

  // Setup MAC Core utilities 

  buffMgmt_init();
  
  st_mac_config.mac_callbacks = mac_callbacks;
  
    
  // Check FSM Status - If not Reset State -> error

  // Init [IN,OUT] queue

  queueMgmt_Init( & (st_mac_config.incomingMacBufferQueue), QUEUE_MAX_LEN ); 
  queueMgmt_Init( & (st_mac_config.outgoingMacBufferQueue), QUEUE_MAX_LEN );
  
  // Set PIB attributes To default Value
  ST_MAC_setDefaultPibAttr();
  
  // Init State Machine
  /* Register tasks */
  MacSys_Init();

  /* Run first time */
  MacSys_SemaphoreSet();

  return MAC_SUCCESS;
}

MAC_Status_t MAC_config() {
  return MAC_NOT_IMPLEMENTED_STATUS;
}

MAC_Status_t MAC_reset() {
  
  MacSys_Resume();
    
  // Reset [IN,OUT] queue
  queueMgmt_Reset(&st_mac_config.incomingMacBufferQueue);
  queueMgmt_Reset(&st_mac_config.outgoingMacBufferQueue);
  
  // Reset Buffer management.
  buffMgmt_reset();
  
  return MAC_SUCCESS;
}

static MAC_Status_t st_mac_handle_outgoing_mac_msg() {
  Buffer_Id_t bufferId;
  uint8_t * mac_msg;
  MAC_Status_t outgoing_status = MAC_NOT_IMPLEMENTED_STATUS;
  
  if (Is_Msg_Available (st_mac_config.outgoingMacBufferQueue) ) {
    /* pop up the buffer id from the queue and get a pointer to it */
    bufferId = queueMgmt_dequeue(&st_mac_config.outgoingMacBufferQueue);
    mac_msg = buffMgmt_getBufferPointer(bufferId);
    switch(mac_msg[MAC_CMD_MSG_ID_OFFSET]){
    case ST_MAC_RESET_CNF_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mlmeResetCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeResetCnfCb( (const  ST_MAC_resetCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_SET_CNF_INT_MSG_ID:
       if (st_mac_config.mac_callbacks->mlmeSetCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeSetCnfCb( (const  ST_MAC_setCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_SCAN_CNF_INT_MSG_ID:
       if (st_mac_config.mac_callbacks->mlmeScanCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeScanCnfCb( (const  ST_MAC_scanCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_BEACON_IND_INT_MSG_ID:
       if (st_mac_config.mac_callbacks->mlmeBeaconNotifyIndCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeBeaconNotifyIndCb( (const  ST_MAC_beaconNotifyInd_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_START_CNF_INT_MSG_ID:
       if (st_mac_config.mac_callbacks->mlmeStartCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeStartCnfCb( (const  ST_MAC_startCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_ASSOCIATION_IND_INT_MSG_ID:
       if (st_mac_config.mac_callbacks->mlmeAssociateIndCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeAssociateIndCb( (const  ST_MAC_associateInd_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_GET_CNF_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mlmeGetCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeGetCnfCb( (const  ST_MAC_getCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_DATA_CNF_INT_MSG_ID:
       if (st_mac_config.mac_callbacks->mcpsDataCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mcpsDataCnfCb( (const  ST_MAC_dataCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_DATA_IND_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mcpsDataIndCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mcpsDataIndCb( (const  ST_MAC_dataInd_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_ASSOCIATION_CNF_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mlmeAssociateCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeAssociateCnfCb( (const  ST_MAC_associateCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_COM_STATUS_IND_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mlmeCommStatusIndCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeCommStatusIndCb( (const  ST_MAC_commStatusInd_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }      
      break;  
    case ST_MAC_POLL_CNF_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mlmePollCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmePollCnfCb( (const  ST_MAC_pollCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;  
    case ST_MAC_POLL_IND_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mlmePollIndCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmePollIndCb( (const  ST_MAC_pollInd_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }      
      break;
    case ST_MAC_PURGE_CNF_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mcpsPurgeCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mcpsPurgeCnfCb( (const  ST_MAC_purgeCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }    
      break;
    case ST_MAC_ORPHAN_IND_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mlmeOrphanIndCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeOrphanIndCb( (const  ST_MAC_orphanInd_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }  
      break; 
    case ST_MAC_RX_ENABLE_CNF_INT_CMD_ID:
      if (st_mac_config.mac_callbacks->mlmeRxEnableCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeRxEnableCnfCb( (const  ST_MAC_rxEnableCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      } 
      break;
    case ST_MAC_DISASSOCIATION_CNF_INT_CMD_ID:
      if (st_mac_config.mac_callbacks->mlmeDisassociateCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeDisassociateCnfCb( (const  ST_MAC_disassociateCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_DISASSOCIATION_IND_INT_CMD_ID:
      if (st_mac_config.mac_callbacks->mlmeDisassociateIndCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeDisassociateIndCb( (const  ST_MAC_disassociateInd_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_BEACONREQ_IND_INT_CMD_ID:
      if (st_mac_config.mac_callbacks->mlmeBeaconReqIndCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeBeaconReqIndCb( (const  ST_MAC_beaconReqInd_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_BEACON_CNF_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mlmeBeaconCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeBeaconCnfCb( (const  ST_MAC_beaconCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_GET_PWR_INFO_TABLE_CNF_INT_CMD_ID:
      if (st_mac_config.mac_callbacks->mlmeGetPwrInfoTableCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeGetPwrInfoTableCnfCb( (const  ST_MAC_getPwrInfoTableCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_SET_PWR_INFO_TABLE_CNF_INT_CMD_ID:
      if (st_mac_config.mac_callbacks->mlmeSetPwrInfoTableCnfCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeSetPwrInfoTableCnfCb( (const  ST_MAC_setPwrInfoTableCnf_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break;
    case ST_MAC_SYNC_LOSS_IND_INT_MSG_ID:
      if (st_mac_config.mac_callbacks->mlmeSyncLossIndCb != NULL) {
        outgoing_status = st_mac_config.mac_callbacks->mlmeSyncLossIndCb( (const  ST_MAC_syncLoss_t *)( mac_msg + MAC_CMD_MSG_HEADER_OFFSET ));
      }
      break; 
    default:
      // Not implemented / supported Message
      break;
    }
    // Release buffer
    buffMgmt_freeBuffer ( bufferId, ST_MAC_HANDLE_OUTGOING_MAC_MSG);
  }
  /* Parse command Id and call associated call back registered from client layer  */
  return outgoing_status;
}

static MAC_Status_t st_mac_handle_incoming_mac_cmd() {

  Buffer_Id_t bufferId;
  uint8_t * mac_cmd;
  MAC_Status_t status = MAC_ERROR;
  
  if (Is_Msg_Available (st_mac_config.incomingMacBufferQueue) ) {
     bufferId = queueMgmt_dequeue(&st_mac_config.incomingMacBufferQueue);
     mac_cmd = buffMgmt_getBufferPointer(bufferId);
     switch(mac_cmd[MAC_CMD_MSG_ID_OFFSET]){
      case ST_MAC_ASSOCIATION_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMEAssociateReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_associateReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_DISASSOCIATION_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMEDisassociateReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_disassociateReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_GET_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMEGetReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_getReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_RESET_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMEResetReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_resetReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_RX_ENABLE_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMERxEnableReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_rxEnableReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_SCAN_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMEScanReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_scanReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_SET_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMESetReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_setReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_START_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMEStartReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_startReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_POLL_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMEPollReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_pollReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_ASSOCIATE_RES_INT_CMD_ID :
          status = ST_MAC_handle_MLMEAssociateRes( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_associateRes_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_ORPHAN_RES_INT_CMD_ID:
          status = ST_MAC_handle_MLMEOrphanRes( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_orphanRes_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_DATA_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MCPSDataReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_dataReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_PURGE_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MCPSPurgeReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_purgeReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_BEACON_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMEBeaconReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_beaconReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_GET_PWR_INFO_TABLE_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMEGetPwrInfoTableReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_getPwrInfoTableReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      case ST_MAC_SET_PWR_INFO_TABLE_REQ_INT_CMD_ID :
          status = ST_MAC_handle_MLMESetPwrInfoTableReq( &mac_cmd[MAC_CMD_MSG_HANDLE_ID_OFFSET] , (const  ST_MAC_setPwrInfoTableReq_t *) (&mac_cmd[MAC_CMD_MSG_HEADER_OFFSET]));
        break;
      default:
        status = MAC_NOT_IMPLEMENTED_STATUS;
        return status; // TEV not sure while 1?
        // Unsupported command - need to add a log here.
        break;
      }

      // Release buffer
      buffMgmt_freeBuffer ( bufferId, ST_MAC_HANDLE_INCOMING_MAC_CMD);

  }
  
  return status;
}

// Return if some msg or command are still stored in queue.

static uint8_t is_remains_mac_msg_to_handle() {
  return ((Is_Msg_Available (st_mac_config.outgoingMacBufferQueue)  || Is_Msg_Available(st_mac_config.incomingMacBufferQueue) )?TRUE:FALSE);
}
/*----------------------------------------------------------------------------*/
/*   ACCESSOR Methods                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*   RUNING Methods                                                           */
/*----------------------------------------------------------------------------*/
void mac_baremetal_run(void) {
  
  /* Check If there is something to handle */
  
  /* Check if outgoing Queue get some elements handle. */
  if ( st_mac_handle_outgoing_mac_msg()!= MAC_SUCCESS)
  { // TODO :  Log Status 
  }
  /*  Check if incomingQueue get some elements to handle */
  if (st_mac_handle_incoming_mac_cmd() != MAC_SUCCESS)
  { // TODO : Log Status 
  }
  
  if( is_remains_mac_msg_to_handle() == TRUE ) {
    MacSys_SemaphoreSet();
  }
}

void mac_azure_rtos_run() {
  while(1);
}


/*----------------------------------------------------------------------------*/
/*   SNPS MAC API Wrapper Methods                                             */
/*----------------------------------------------------------------------------*/

#if 0
// Currently not used - Mustn't be part as it's a wrapper between NTS and SNPS.
// Required to get a bridge struct from MAC Wrapper to hide SNPS Struct

static void mac_pan_desc_convert_to_ST( NTS_MAC_PAN_Descriptor_t * pan_desc,  pan_descr_st * pan_desc_snps) {

  /*! PAN identifier of the coordinator */
  MAC_MEMCPY(&pan_desc->a_coord_pan_id, &pan_desc_snps->coord_pan_id, g_SHORT_PAN_ID_LENGTH_c);
  /*! Coordinator addressing mode */
  pan_desc->coord_addr_mode = pan_desc_snps->coord_addr_mod;
  /*! The current logical channel occupied by the network */
  pan_desc->logical_channel = pan_desc_snps->logic_chanl;
  /*! Coordinator address */
  if(pan_desc->coord_addr_mode  == SHORT_ADDRS_16) {
    MAC_MEMCPY(&pan_desc->coord_addr, pan_desc_snps->coord_addr,g_SHORT_ADDRESS_LENGTH_c);
    }
  else if(pan_desc->coord_addr_mode  == EXT_ADDRS_64) {
    MAC_MEMCPY(&pan_desc->coord_addr, pan_desc_snps->coord_addr,g_EXTENDED_ADDRESS_LENGTH_c);
  }
  else {
    MAC_MEMSET(&pan_desc->coord_addr, 0x00,g_EXTENDED_ADDRESS_LENGTH_c);
    }/* would not occurs*/
  /*! The current channel page occupied by the network */
  /* TBD :  pan_desc->channel_page not supported in NTS Msg */
  // pan_desc->channel_page = pan_desc_snps->chnl_pge;

  /*! PAN coordinator is accepting GTS requests or not */
  pan_desc->gts_permit = pan_desc_snps->gts_perm;
  /*! Superframe specification as specified in the received beacon frame */
  MAC_MEMCPY(pan_desc->a_superframe_spec, &pan_desc_snps->super_frm_spec,0x02); /* TBD : a_superframe_spec size not declared */
  /*! The time at which the beacon frame was received, in symbols */
  MAC_MEMCPY(pan_desc->a_time_stamp, &pan_desc_snps->time_stamp,0x04);
  /*! The LQI at which the network beacon was received */
  pan_desc->link_quality = pan_desc_snps-> link_qual;
  /*! Security level purportedly used by the received beacon frame */
  pan_desc->security = pan_desc_snps->sec_fail_status;
}

#endif

/* Check if command Id is relevant for current MAC state*/
uint8_t is_MAC_ready(uint8_t mac_command_id){
   return MAC_SUCCESS;
}

uint8_t mac_command_validate(uint8_t command_id, uint8_t command_len) {
  
  if ( !(command_id<ST_MAC_MAX_INT_IN_CMD_ID) ) {
    return MAC_ERROR;
  }
  if ( g_command_size[command_id] != command_len) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

/*! Use for MLMEBeaconNotifyInd */
void mac_serialize_pan_desc_to_ST( ST_MAC_PAN_Desc_t* pan_desc,  pan_descr_st * pan_desc_snps) {


  /*! Coordinator addressing mode */
  pan_desc->coord_addr_mode = pan_desc_snps->coord_addr_mod;

  /*! PAN identifier of the coordinator */
  MAC_MEMCPY(pan_desc->a_coord_PAN_id, &pan_desc_snps->coord_pan_id, g_SHORT_PAN_ID_LENGTH_c);
  /*! Coordinator address */
  if( pan_desc_snps->coord_addr_mod  == SHORT_ADDRS_16) {
    MAC_MEMCPY(pan_desc->coord_addr.a_short_addr, pan_desc_snps->coord_addr,g_SHORT_ADDRESS_LENGTH_c);
    }
  else if( pan_desc_snps->coord_addr_mod  == EXT_ADDRS_64) {
    MAC_MEMCPY(pan_desc->coord_addr.a_extend_addr, pan_desc_snps->coord_addr,g_EXTENDED_ADDRESS_LENGTH_c);
  }
  else {
    /* would not occurs*/
    }
  /* The current channel page occupied by the network */
  pan_desc->channel_page = pan_desc_snps->chnl_pge;

  /*! The current logical channel occupied by the network */
  pan_desc->logical_channel = pan_desc_snps->logic_chanl;

  /*! Superframe specification as specified in the received beacon frame */
  WRITE_2_BYTES( (uint8_t *) pan_desc->a_superframe_spec,0,pan_desc_snps->super_frm_spec );

  /*! PAN coordinator is accepting GTS requests or not */
  pan_desc->gts_permit = pan_desc_snps->gts_perm;

  /*! The LQI at which the network beacon was received */
  pan_desc->link_quality = pan_desc_snps-> link_qual;

  /*! The time at which the beacon frame was received, in symbols */
  MAC_MEMCPY(pan_desc->a_time_stamp, &pan_desc_snps->time_stamp,0x03);

  /*! Security level purportedly used by the received beacon frame */
  pan_desc->security_level = pan_desc_snps->sec_fail_status;
}

#if 0
// TODO- we may split Queue : MLME / MCPS / SYS / DBG / EXT / ....
// SAP Stand for Service Access Point

void MAC_EnqueueToSAP ( uint8_t bufferId )
{
    /* Enque the msg in corresponding NHLE queue */
    if (QUEUE_FULL == queueMgmt_enqueue(&st_mac_config.outgoingMacBufferQueue, bufferId )) {
        buffMgmt_freeBuffer(bufferId, 0x01);
    }
    // TODO - Schedule MAC Task - Here
    // MAC_EnqueueToNwk
}

#endif 

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64)
{
      uint32_t company_id = LL_FLASH_GetSTCompanyID();
      uint32_t device_uid = LL_FLASH_GetUDN();

      //check if data correctly saved in ROM during factory process
      if (company_id == 0xffffffff)
      {
        company_id = 0x0080E1; //ST company ID
      }

      //check if data correctly saved in ROM during factory process
      if (device_uid == 0xffffffff)
      {
        device_uid = 0x2A; //WBA default device ID
        /* Need to add random in this case ? */
      }

      aIeeeEui64[7] = (company_id>>16)&0xff;
      aIeeeEui64[6] = (company_id>>8)&0xff;
      aIeeeEui64[5] = (company_id>>0)&0xff;     
      aIeeeEui64[4] = 0xFF;
      aIeeeEui64[3] = 0xFE;
      aIeeeEui64[2] = (device_uid>>16)&0xff;
      aIeeeEui64[1] = (device_uid>>8)&0xff;
      aIeeeEui64[0] = (device_uid>>0)&0xff;
      
}

/******************************************/
/* Incoming MLME CNF (RADIO Layer -> MAC) */
/******************************************/

/******************************************/
/* Incoming MCPS CNF (RADIO Layer -> MAC) */
/******************************************/
