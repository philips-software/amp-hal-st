/**
  ******************************************************************************
  * @file    ST_mac_wpan/Src/st_mac_802_15_4_mlme_svc.c
  * @author  MCD-WBL Application Team
  * @brief   Mac Implementation App.
  *          Handle the MAC MLME SAP - Part of Internal ST MAC Wrapper module
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

#include "st_mac_802_15_4_types.h"
#include "st_mac_802_15_4_svc.h"
#include "st_mac_802_15_4_core.h"
#include "st_mac_buffer_management.h"

#include "st_mac_802_15_4_core_config.h"
#include "mac_temporary.h"

// SNPS Link Layer
#include "mac_host_intf.h"
#include "common_mac.h"
#include "ral.h"

// MAC log
#include "log_module.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BEACON_ORDER_MAX_VALUE      15
#define SUPER_FRAME_ORD_MAX_VALUE   15
#define MIB_EXPIRY_INTRVL_MIN       1
#define MIB_EXPIRY_INTRVL_MAX       1440
#define MAX_BE_MAX_VALUE            8
#define MAX_BE_MIN_VALUE            3
#define MAX_CSMA_MAX_VALUE          5
#define MIN_BE_MAX_VALUE            8
#define MAC_ATTR_VAL_LEN [56]; // Beacon payload ? To be confirmed.

/* Private macro -------------------------------------------------------------*/
#define IS_BOOLEAN(X) ((((X) == FALSE) || ((X) == TRUE)) ? 1 : 0)

/* Public variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static attr_val_t  g_pib_attr_val;
static ral_instance_t ral_instance;
static uint8_t reset_macCoordExtendedAddress = 0x00;

/* Private function prototypes -----------------------------------------------*/
static MAC_Status_t ST_MAC_handle_getPibAttr(uint8_t pib_attr);
static MAC_Status_t ST_MAC_handle_setPibAttr(uint8_t pib_attr, attr_val_t *pib_attr_val);

/* Private functions ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern mac_ctx_st g_mac_ctx[MAC_NUMBER_OF_INSTANCE];
extern st_mlme_pib_t st_mlme_pib;
extern int8_t g_mac_max_tx_pwr;
/* External functions --------------------------------------------------------*/


static MAC_Status_t ST_MAC_handle_getPibAttr(uint8_t pib_attr) {
  
  MAC_Status_t status = MAC_SUCCESS;
  attr_val_t pib_attr_val;
  uint32_t mac_otInstance;
  
  MEMSET(&pib_attr_val, 0x00, sizeof(attr_val_t)); // Initialized value
  
  switch (pib_attr) {
    
    case g_PHY_TRANSMIT_POWER_c :
      // get current transmit power
      otPlatRadioGetTransmitPower((otInstance*)&mac_otInstance, &st_mlme_pib.txPwr);
      pib_attr_val.attr_int = (uint32_t)st_mlme_pib.txPwr;
      break;
      
    default:
      status = MAC_NOT_IMPLEMENTED_STATUS;
      break;
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEGetCnf function\r\n");
#endif
  
  // trigger get conf
  ST_MAC_handle_MLMEGetCnf(&g_mac_ctx[0], status, pib_attr, 0, &pib_attr_val);
  
  return status;
}

static MAC_Status_t ST_MAC_handle_setPibAttr(uint8_t pib_attr, attr_val_t *pib_attr_val) {
  
  MAC_Status_t status = MAC_SUCCESS;
  
  switch (pib_attr) {
    
    case g_PHY_TRANSMIT_POWER_c :       
      st_mlme_pib.txPwr = (int8_t) pib_attr_val->attr_int;
      // check limits on tx power
      if (st_mlme_pib.txPwr > MAC_MAX_TX_POWER) {
        st_mlme_pib.txPwr = MAC_MAX_TX_POWER;
      } else if (st_mlme_pib.txPwr < MAC_MIN_TX_POWER) {
        st_mlme_pib.txPwr = MAC_MIN_TX_POWER;
      }
      // set transmit power with new setting     
      g_mac_max_tx_pwr = st_mlme_pib.txPwr;      
      otPlatRadioSetTransmitPower(NULL, st_mlme_pib.txPwr);     
      break;
      
    default:
      status = MAC_NOT_IMPLEMENTED_STATUS;
      break;
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMESetCnf function\r\n");
#endif
  
  // trigger set conf
  ST_MAC_handle_MLMESetCnf(&g_mac_ctx[0], status, pib_attr, 0);
  
  return status;
}


/**
 * @brief  This function is used to handle a MAC MLME Disassociate REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_disassociateReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEDisassociateReq( MAC_handle * st_mac_hndl , const  ST_MAC_disassociateReq_t * pDisassiociateReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
  mlme_disassoc_req_param_st disassoc_req_params;
  
  /* Fill disassoc_req_params structure */
  disassoc_req_params.addr_mod = (mac_addrs_mode_enum_t)pDisassiociateReq->device_addr_mode;
  MEMCPY(disassoc_req_params.dev_addr, &(pDisassiociateReq->device_address.a_extend_addr)[0], EXT_ADDRESS_LENGTH);
  disassoc_req_params.dev_pan_id = READ_2_BYTES(pDisassiociateReq->a_device_PAN_id, 0);  
  disassoc_req_params.reason = pDisassiociateReq->disassociate_reason;
  
  /* Security parameters */
  disassoc_req_params.sec_params.sec_level = (sec_level_enum_t) pDisassiociateReq->security_level;
  if ( disassoc_req_params.sec_params.sec_level != 0 ) {
#if SUPPORT_SEC
    disassoc_req_params.sec_params.key_id_mod = pDisassiociateReq->key_id_mode;
    switch ( disassoc_req_params.sec_params.key_id_mod ) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( disassoc_req_params.sec_params.key_src, pDisassiociateReq->a_key_source, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( disassoc_req_params.sec_params.key_src, pDisassiociateReq->a_key_source, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
      }

    disassoc_req_params.sec_params.key_indx = pDisassiociateReq->key_index;
#else    
   disassoc_req_params.sec_params.sec_level = (sec_level_enum_t) 0;
#endif
  }

  disassoc_req_params.tx_indirect = pDisassiociateReq->tx_Indirect;

#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEDisassociateReq function\r\n");
#endif
  
  status = mlme_disassoc_req((uint32_t) &g_mac_ctx[0],
		&disassoc_req_params);
  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Get REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_getReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEGetReq( MAC_handle * st_mac_hndl ,const  ST_MAC_getReq_t * pGetReq )
{
  MAC_Status_t status = MAC_SUCCESS;
  uint8_t allocation_attr_data[MAX_SIZE_PIB];
  // SNPS need Cnf Struct on Req demande.
  // Workaround.. use a global static declaration to handle pib_attr_indx ans Pib_attr_value
  //TODO : Review pit Attribute Index Implementation
  static uint8_t     g_pib_attr_index;
  g_pib_attr_index = 0;

  switch (pGetReq->PIB_attribute) {
    
    case g_PHY_TRANSMIT_POWER_c :
      // call ST get PIB attributes service
      status = ST_MAC_handle_getPibAttr(pGetReq->PIB_attribute);
      break;
    
    case g_MAC_EXTENDED_ADDRESS_c :
       mlme_get_req((uint32_t) &g_mac_ctx[0], CUSTOM_IEEE_EXTD_ADDRS,
        g_pib_attr_index, &g_pib_attr_val);
      break;
      
    case EB_HDR_IE_LIST_ID :
    case EB_PYLD_IE_LIST_ID :
      g_pib_attr_val.attr_array.data = allocation_attr_data;
      mlme_get_req((uint32_t) &g_mac_ctx[0], pGetReq->PIB_attribute,
            g_pib_attr_index, &g_pib_attr_val);
      break;
      
    default :  
      mlme_get_req((uint32_t) &g_mac_ctx[0], pGetReq->PIB_attribute,
            g_pib_attr_index, &g_pib_attr_val);
  }

  return status;
}



/**
 * @brief  This function is used to handle a MAC MLME GTS REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_gtsReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEGtsReq( MAC_handle * st_mac_hndl ,const  ST_MAC_gtsReq_t * pGtsReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  return status;
}


/**
 * @brief  This function is used to handle a MAC MLME Reset REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_resetReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEResetReq( MAC_handle * st_mac_hndl ,const  ST_MAC_resetReq_t * pResetReq )
{
  MAC_Status_t status = MAC_SUCCESS;
  attr_val_t pib_attr_value;
  uint8_t tab_macCoordExtendedAddress[EXT_ADDRESS_LENGTH];
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEResetReq function\r\n");
#endif
  
  mlme_rst_req( (uint32_t) &g_mac_ctx[0], pResetReq->set_default_PIB);

  /* macCoordExtendedAddress to 0x0000000000000000 */
  reset_macCoordExtendedAddress = 0x00;
  if (pResetReq->set_default_PIB) {
    reset_macCoordExtendedAddress = 0x01;
    memset (&pib_attr_value, 0x00, sizeof(attr_val_t));
    memset (&tab_macCoordExtendedAddress, 0x00, EXT_ADDRESS_LENGTH);
    pib_attr_value.attr_array.data = (uint8_t *) &tab_macCoordExtendedAddress;
    pib_attr_value.attr_array.length = EXT_ADDRESS_LENGTH;
    mlme_set_req (( uint32_t ) &g_mac_ctx[0], COORD_EXT_ADDR_ID, 0x00, &pib_attr_value);
  }
  return status;
}


/**
 * @brief  This function is used to handle a MAC MLME Rx Enabled REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_rxEnableReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMERxEnableReq( MAC_handle * st_mac_hndl ,const  ST_MAC_rxEnableReq_t * pRxEnableReq )
{
  MAC_Status_t status;
  
  uint32_t rx_on_duration = READ_4_BYTES(pRxEnableReq->a_Rx_on_duration, 0);
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMERxEnableReq function\r\n");
#endif
  
  status = mlme_rx_enable_req((uint32_t) &g_mac_ctx[0], 
                              rx_on_duration,
                              pRxEnableReq->ranging_Rx_control);
  return status;
}

/**
* @brief  This function is used to handle a MAC MLME Scan REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_scanReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEScanReq( MAC_handle * st_mac_hndl , const  ST_MAC_scanReq_t * pScanReq )
{    
    MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
    mlme_scn_req_param_st scn_req_params;
    
    scn_req_params.scn_type = (scn_type_t) pScanReq->scan_type;
    scn_req_params.scn_chnls = READ_4_BYTES(pScanReq->a_scan_channels, 0);
    scn_req_params.scn_dur = pScanReq->scan_duration;
    scn_req_params.chnl_pge = pScanReq->channel_page;
    scn_req_params.sn_supr = pScanReq->scan_seq_sup;

    scn_req_params.ie_present = 0;
    scn_req_params.hdr_ie_list_count = 0;
    scn_req_params.pyld_ie_list_count = 0;
    if(scn_req_params.scn_type == ENHANCED_ACTIV_SCAN) {
      if(pScanReq->hdr_ie_list.hdr_ie_list_count != 0 || pScanReq->pyld_ie_list.pyld_ie_list_count != 0) {
        scn_req_params.ie_present = 1;
      }
      if(scn_req_params.ie_present != 0) {
        /* HDR IE format: Count + total Length + HIEs*/
        /*---------------------------Header IEs---------------------------------*/
        scn_req_params.hdr_ie_list_count = pScanReq->hdr_ie_list.hdr_ie_list_count;
        if(scn_req_params.hdr_ie_list_count > 0) {
          scn_req_params.ie_hdr_total_len = pScanReq->hdr_ie_list.hdr_ie_list_size;
          ASSERT(scn_req_params.ie_hdr_total_len <= MAX_HDR_IE_SIZE, SEVERITY_HIGH); /* To force stop of unsupported hdr IEs */
          scn_req_params.ptr_hdr_ie_list = (uint8_t*) pScanReq->hdr_ie_list.ptr_hdr_ie_list;// TEV new implementation
        }
        /* PYLD IE format: Count + total Length + PIEs*/
        /*---------------------------Payload IEs--------------------------------*/
        scn_req_params.pyld_ie_list_count = pScanReq->pyld_ie_list.pyld_ie_list_count;
        if(scn_req_params.pyld_ie_list_count > 0) {
          scn_req_params.ie_payld_total_len = pScanReq->pyld_ie_list.pyld_ie_list_size;
          ASSERT(scn_req_params.ie_payld_total_len <= MAX_ZIGBEE_EBR_IE_LEN, SEVERITY_HIGH);
          scn_req_params.ptr_pyld_ie_list = (uint8_t*) pScanReq->pyld_ie_list.ptr_pyld_ie_list;// TEV new implementation
        }
      }
    }
    
  /* Security parameters */
  scn_req_params.sec_params.sec_level = (sec_level_enum_t) pScanReq->security_level;
  if ( scn_req_params.sec_params.sec_level != 0 ) {
#if SUPPORT_SEC
    scn_req_params.sec_params.key_id_mod = pScanReq->key_id_mode;
    switch ( scn_req_params.sec_params.key_id_mod ) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( scn_req_params.sec_params.key_src, pScanReq->a_key_source, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( scn_req_params.sec_params.key_src, pScanReq->a_key_source, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
      }

    scn_req_params.sec_params.key_indx = pScanReq->key_index;
#else    
   scn_req_params.sec_params.sec_level = (sec_level_enum_t) 0;
#endif
    }

#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEScanReq function\r\n");
#endif
  
    status = mlme_scn_req((uint32_t) &g_mac_ctx[0], &scn_req_params);

   return status;
}

/**
* @brief  This function is used to handle a MAC MLME Set REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_setReq_t ref
 * @retval MAC_Status_t
 */ 
MAC_Status_t ST_MAC_handle_MLMESetReq( MAC_handle * st_mac_hndl , const  ST_MAC_setReq_t * pSetReq) {

  MAC_Status_t    status = MAC_SUCCESS;
  attr_val_t      pib_attr_value;
  
  MEMSET(&pib_attr_value, 0x00, sizeof(attr_val_t));
  
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_SEND_BEACON) 
  uint8_t payload_len;
  uint32_t expiry_interval  = 0x00;
#endif
  uint8_t use_st_pibAttr_service = 0;
  uint8_t pib_attr = pSetReq->PIB_attribute;
  
  switch ( pib_attr ) {
    
    case g_PHY_TRANSMIT_POWER_c :
      use_st_pibAttr_service = 1;
      
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      // call ST set PIB attributes service
      status = ST_MAC_handle_setPibAttr(pib_attr, &pib_attr_value);
      break;
    
    case ACK_WAIT_DUR_ID:
    case BEACON_TX_TIME_ID:
    case PHY_CHANNEL_SUPPORTED:
    case SYNC_SYMB_OFFS_ID:
    case PHY_CURRENT_PAGE:
    case PHY_MAX_FRAME_DUR:
    case PHY_SHR_DUR:
    case PHY_SYMBOLS_PER_OCTET:
    case MIB_EXPIRY_INTRVL_COUNTDOWN_ID:
      status = READ_ONLY;
      break;
    case ASSOC_PAN_COORD_ID:
      pib_attr_value.attr_bool = *( pSetReq->PIB_attribute_valuePtr );
      break;
#if (FFD_DEVICE_CONFIG)
    case ASSOC_PERMIT_ID:
      if (IS_BOOLEAN(* ( pSetReq->PIB_attribute_valuePtr ))) {
        pib_attr_value.attr_bool = * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }        
      break;
#endif /* FFD_DEVICE_CONFIG */

    case AUTO_REQ_ID:
      if (IS_BOOLEAN(* ( pSetReq->PIB_attribute_valuePtr ))) {
        pib_attr_value.attr_bool = * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }        
      break;

#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_SEND_BEACON)
    case BEACON_PAY_ID:
      payload_len = g_mac_ctx[0].mlme_pib.mac_bcon_payld_len;
      payload_len = (payload_len>MAX_BEACON_PAY_LEN)?MAX_BEACON_PAY_LEN:payload_len;
      pib_attr_value.attr_array.data=pSetReq->PIB_attribute_valuePtr;
      pib_attr_value.attr_array.length = payload_len;
      break;

    case BECON_PAY_LEN_ID:
      if ( ( ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr ) ) <= MAX_BEACON_PAY_LEN ) {
        pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }
      break;

    case BSN_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case EBSN_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case BEACON_ORDER_ID:
      if (( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr ) <= BEACON_ORDER_MAX_VALUE) {
        pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }
      break;

    case SUPER_FRAME_ORD_ID:
      if (( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr ) <= SUPER_FRAME_ORD_MAX_VALUE) {
        pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }
      break;

    case MIB_JOINING_IEEE_LIST_ID:
      pib_attr_value.attr_array.data=pSetReq->PIB_attribute_valuePtr;
      pib_attr_value.attr_array.length = EXT_ADDRESS_LENGTH;
      break;

    case MIB_JOINING_POLICY_ID:
      if ( ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr ) <= IEEELIST_JOIN ) {
        pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }
      break;

    case MIB_EXPIRY_INTRVL_ID:
      MEMCPY ( &expiry_interval, (void const *) ( pSetReq->PIB_attribute_valuePtr ), 2 );
      if ( ( expiry_interval >= MIB_EXPIRY_INTRVL_MIN ) &&
         (  expiry_interval <= MIB_EXPIRY_INTRVL_MAX ) )
      {
        pib_attr_value.attr_int = ( uint32_t ) expiry_interval;
      }else {
        status = INVALID_PARAMETER;
      }
      break;

    case EB_HDR_IE_LIST_ID:
      pib_attr_value.attr_array.length = (uint16_t) ( pSetReq->PIB_attribute_valuePtr[0] );//the first byte is the length
      pib_attr_value.attr_array.data = &( pSetReq->PIB_attribute_valuePtr[1] );//the rest is the payload
      break;

    case EB_PYLD_IE_LIST_ID:
      pib_attr_value.attr_array.length = (uint16_t) ( pSetReq->PIB_attribute_valuePtr[0] );//the first byte is the length
      pib_attr_value.attr_array.data = &( pSetReq->PIB_attribute_valuePtr[1] );//the rest is the payload
      break;

    case EB_FLTR_Enbld_ID:
      pib_attr_value.attr_bool = * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case EB_AUTO_SA_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

#endif /* (FFD_DEVICE_CONFIG || RFD_SUPPORT_SEND_BEACON) */

    case COORD_EXT_ADDR_ID:
      pib_attr_value.attr_array.data =  ( pSetReq->PIB_attribute_valuePtr );
      pib_attr_value.attr_array.length =  EXT_ADDRESS_LENGTH;
      break;

    case COORD_SHRT_ADDR_ID:
      MEMCPY ( &pib_attr_value.attr_int, (void const *) ( pSetReq->PIB_attribute_valuePtr ), SHORT_ADDRESS_LENGTH );
      break;

    case DSN_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case MAX_BE_ID:
      if ((( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr ) >= MAX_BE_MIN_VALUE) &&
          (( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr ) <= MAX_BE_MAX_VALUE)) {
        pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }
      break;

    case MAX_CSMA_ID:
      if (( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr ) <= MAX_CSMA_MAX_VALUE) {
        pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }     
      break;

    case MAX_FRAME_WAIT_TIME_ID:
      MEMCPY ( &pib_attr_value.attr_int, (void const *) ( pSetReq->PIB_attribute_valuePtr ), 2 );
      break;

    case MAX_FRAME_RETRY_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case MIN_BE_ID:
      if (( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr ) <= MIN_BE_MAX_VALUE) {
        pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }
      break;

    case PAN_ID_ID:
      MEMCPY ( &pib_attr_value.attr_int, (void const *) ( pSetReq->PIB_attribute_valuePtr ), SHORT_ADDRESS_LENGTH );
      // pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

#if ( FFD_DEVICE_CONFIG || RFD_SUPPORT_PROMISCUOUS_MODE )
    case PROMIS_MODE_ID:
      if (IS_BOOLEAN(* ( pSetReq->PIB_attribute_valuePtr ))) {
        pib_attr_value.attr_bool = * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }
      break;

#endif /* (FFD_DEVICE_CONFIG || RFD_SUPPORT_PROMISCUOUS_MODE) */

    case RSP_WAIT_TIME_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case RX_WHEN_IDLE_ID:
      if (IS_BOOLEAN(* ( pSetReq->PIB_attribute_valuePtr ))) {    
        if(* ( pSetReq->PIB_attribute_valuePtr ) == 1) {
          ral_set_auto_sleep_state(ral_instance, RAL_DISABLE);
        } else {
          ral_set_auto_sleep_state(ral_instance, RAL_ENABLE);
        }        
        pib_attr_value.attr_bool = * ( pSetReq->PIB_attribute_valuePtr );
      } else {
        status = INVALID_PARAMETER;
      }
      break;

    case SEC_ENABLED_ID:
      pib_attr_value.attr_bool = * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case SHORT_ADDR_ID:
      MEMCPY ( &pib_attr_value.attr_int, (void const *) ( pSetReq->PIB_attribute_valuePtr ), SHORT_ADDRESS_LENGTH );
      break;

    case TIME_STAMP_SUPP_ID:
      pib_attr_value.attr_bool = * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case PERSISTEN_TIME_ID:
      MEMCPY ( &pib_attr_value.attr_int, (void const *) ( pSetReq->PIB_attribute_valuePtr ), 2 );
      break;

    case PHY_CHANNEL_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case PHY_CCA_MODE:
      ASSERT ( 0 ,SEVERITY_HIGH); // NOT SUPPORTED
      // if (pib_attr_value.attr_int > 1) status = UNSUPPORTED_ATTRIBUTE;
      break;

    case EBR_PRMT_JOIN_ID:
      pib_attr_value.attr_bool = * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case EBR_FLTRS_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case EBR_LQ_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case EBR_PRCNT_FLTR_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case EBR_AUTO_RSPND_ID:
      pib_attr_value.attr_bool = * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case g_MAC_EXTENDED_ADDRESS_c:
      pib_attr = CUSTOM_IEEE_EXTD_ADDRS;
      pib_attr_value.attr_array.length = EXT_ADDRESS_LENGTH;
      pib_attr_value.attr_array.data = pSetReq->PIB_attribute_valuePtr;
      break;
      
    case MAX_FULL_CSMA_FRAME_RETRY_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

#if SUPPORT_SEC
    /* based on 2011 standard */ 
    case KEY_TBL_ID:
      pib_attr_value.attr_array.length = (uint16_t) ( pSetReq->PIB_attribute_valuePtr[0] );
      pib_attr_value.attr_array.data = &( pSetReq->PIB_attribute_valuePtr[1] );
      break;

    case DEVICE_TBL_ID:
      pib_attr_value.attr_array.length = (uint16_t) ( pSetReq->PIB_attribute_valuePtr[0] );
      pib_attr_value.attr_array.data = &( pSetReq->PIB_attribute_valuePtr[1] );
      break;

    case SEC_LVL_TBL_ID:
      pib_attr_value.attr_array.length = (uint16_t) ( pSetReq->PIB_attribute_valuePtr[0] );
      pib_attr_value.attr_array.data = &( pSetReq->PIB_attribute_valuePtr[1] );
      break;

    case FRM_CNTR_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case AUTO_REQ_SEC_LVL_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case AUTO_REQ_KEY_MODE_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case AUTO_REQ_KEY_SRC_ID:
      pib_attr_value.attr_array.length = 0x04;
      pib_attr_value.attr_array.data = pSetReq->PIB_attribute_valuePtr;
      break;

    case AUTO_REQ_KEY_INDX_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

    case DFLT_KEY_SRC_ID:
      pib_attr_value.attr_array.length = 0x08;
      pib_attr_value.attr_array.data = pSetReq->PIB_attribute_valuePtr;
      break;

    case PAN_COORD_EXT_ADDR_ID:
      pib_attr_value.attr_array.length = EXT_ADDRESS_LENGTH;
      pib_attr_value.attr_array.data = pSetReq->PIB_attribute_valuePtr;
      break;

    case PAN_COORD_SHRT_ADDR_ID:
      pib_attr_value.attr_int = ( uint32_t ) * ( pSetReq->PIB_attribute_valuePtr );
      break;

#endif /* SUPPORT_SEC */
    default:
        status = UNSUPPORTED_ATTRIBUTE;
        break;
    }
 
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMESetReq function, PIB_attribute=0x%02x\r\n", pSetReq->PIB_attribute);
#endif
  
  if (status == INVALID_PARAMETER) {
    // do not call SNPS mac service and trigger a setConf instead
    ST_MAC_handle_MLMESetCnf(&g_mac_ctx[0], INVALID_PARAMETER, pib_attr, 0);
  }
  else if (!use_st_pibAttr_service) {
    mlme_set_req ( ( uint32_t ) &g_mac_ctx[0], pib_attr, 0x00, &pib_attr_value );
  }
  
  return status;
}


/**
* @brief  This function is used to handle a MAC MLME Start REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_startReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEStartReq( MAC_handle * st_mac_hndl ,const  ST_MAC_startReq_t * pStartReq)
{
  MAC_Status_t status = MAC_NOT_SUPPORTED_STATUS;
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_START_PRIM)
  mlme_start_req_param_st mlme_start_req_params;
  MEMSET(&mlme_start_req_params,0x00,sizeof(mlme_start_req_param_st));

  MEMCPY(&mlme_start_req_params.pan_id,pStartReq->a_PAN_id,0x02);
  mlme_start_req_params.logic_chanl = pStartReq->channel_number;
  mlme_start_req_params.chnl_pge = pStartReq->channel_page;
  MEMCPY(&mlme_start_req_params.start_time,pStartReq->a_start_time,0x04);
  mlme_start_req_params.bcon_ord = pStartReq->beacon_order;
  mlme_start_req_params.super_frm_ord = pStartReq->superframe_order;
  mlme_start_req_params.pan_coord = pStartReq->PAN_coordinator;
  mlme_start_req_params.batt_life_ext = pStartReq->battery_life_extension;  // Beware not on the same Type (to be checked)
  mlme_start_req_params.coord_realign = pStartReq->coord_realignment;       // Beware not on the same Type (to be checked)
  
  /* Coordinator realign Security */
  mlme_start_req_params.coord_realign_sec_parms.sec_level = (sec_level_enum_t) pStartReq->coord_realign_security_level;
  if ( mlme_start_req_params.coord_realign_sec_parms.sec_level != 0 ) {
#if SUPPORT_SEC
    mlme_start_req_params.coord_realign_sec_parms.key_id_mod = pStartReq->coord_realign_key_id_mode;
    switch ( mlme_start_req_params.coord_realign_sec_parms.key_id_mod ) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( mlme_start_req_params.coord_realign_sec_parms.key_src, pStartReq->a_coord_realign_key_source, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( mlme_start_req_params.coord_realign_sec_parms.key_src, pStartReq->a_coord_realign_key_source, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
      }

    mlme_start_req_params.coord_realign_sec_parms.key_indx = pStartReq->coord_realign_key_index;
#else    
   mlme_start_req_params.coord_realign_sec_parms.sec_level = (sec_level_enum_t) 0;
#endif
  }
  
  /* Beacon Security */
  mlme_start_req_params.beacn_sec_params.sec_level = (sec_level_enum_t) pStartReq->beacon_security_level;
  if ( mlme_start_req_params.beacn_sec_params.sec_level != 0 ) {
#if SUPPORT_SEC
    mlme_start_req_params.beacn_sec_params.key_id_mod = pStartReq->beacon_key_id_mode;
    switch ( mlme_start_req_params.beacn_sec_params.key_id_mod ) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( mlme_start_req_params.beacn_sec_params.key_src, pStartReq->a_beacon_key_source, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( mlme_start_req_params.beacn_sec_params.key_src, pStartReq->a_beacon_key_source, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
      }

    mlme_start_req_params.beacn_sec_params.key_indx = pStartReq->beacon_key_index;
#else    
   mlme_start_req_params.beacn_sec_params.sec_level = (sec_level_enum_t) 0;
#endif
  }
  
  /*  Information element Wraping */
  mlme_start_req_params.hdr_ie_list_count = 0;
  mlme_start_req_params.pyld_ie_list_count = 0;
  mlme_start_req_params.ie_present = 0;
  if(pStartReq->hdr_ie_list.hdr_ie_list_count != 0 || pStartReq->pyld_ie_list.pyld_ie_list_count != 0) {
    mlme_start_req_params.ie_present = 1;
  }
  if(mlme_start_req_params.ie_present != 0) {
    /* HDR IE format: Count + total Length + HIEs*/
    /*---------------------------Header IEs---------------------------------*/
    mlme_start_req_params.hdr_ie_list_count = pStartReq->hdr_ie_list.hdr_ie_list_count;
    if(mlme_start_req_params.hdr_ie_list_count > 0) {
      mlme_start_req_params.ie_hdr_total_len = pStartReq->hdr_ie_list.hdr_ie_list_size;
      ASSERT(mlme_start_req_params.ie_hdr_total_len <= MAX_HDR_IE_SIZE, SEVERITY_HIGH); /* To force stop of unsupported hdr IEs */
      mlme_start_req_params.ptr_hdr_ie_list = (uint8_t*) pStartReq->hdr_ie_list.ptr_hdr_ie_list;// TEV new implementation
    }
    /* PYLD IE format: Count + total Length + PIEs*/
    /*---------------------------Payload IEs--------------------------------*/
    mlme_start_req_params.pyld_ie_list_count = pStartReq->pyld_ie_list.pyld_ie_list_count;
    if(mlme_start_req_params.pyld_ie_list_count > 0) {
      /* first 11 bits of payload IE */
      mlme_start_req_params.ie_payld_total_len = pStartReq->pyld_ie_list.pyld_ie_list_size;
      ASSERT(mlme_start_req_params.ie_payld_total_len <= MAX_ZIGBEE_EBR_IE_LEN, SEVERITY_HIGH);
      mlme_start_req_params.ptr_pyld_ie_list = (uint8_t*) pStartReq->pyld_ie_list.ptr_pyld_ie_list;// TEV new implementation
    }
  }

#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEStartReq function\r\n");
#endif
  
  /** non-standard but if a device is associate with this PANID address, 
   * then we can send frame with short address in source address.
   */
  if (mlme_start_req_params.pan_id != 0xFFFF) {
    status = mlme_strt_req((uint32_t) &g_mac_ctx[0], &mlme_start_req_params);
  }else {
    /* return MLME-start.confirm with status=INVALID_PARAMETER (0xE8) */
    ST_MAC_handle_MLMEStartCnf(&g_mac_ctx[0], INVALID_PARAMETER);
  }
#endif  
  
  return status;
}


/**
 * @brief  This function is used to handle a MAC MLME Associate REQ as described
 *         in IEEE Std 802.15.4-2011 standard.
 *
 *
 * @param  MAC_associateReq_t ref
 * @retval MAC_Status_t
 */

MAC_Status_t ST_MAC_handle_MLMEAssociateReq( MAC_handle * st_mac_hndl ,const  ST_MAC_associateReq_t * pAssociateReq )
{
    MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
    mlme_assoc_req_param_st mlme_assoc_req_params;
    MEMSET(&mlme_assoc_req_params,0x00,sizeof(mlme_assoc_req_param_st));

    mlme_assoc_req_params.chann_num = pAssociateReq->channel_number;
    mlme_assoc_req_params.chnl_pge  = pAssociateReq->channel_page;
    mlme_assoc_req_params.coord_pan_id = READ_2_BYTES(pAssociateReq->a_coord_PAN_id,0);
    mlme_assoc_req_params.coord_addr_mod = (mac_addrs_mode_enum_t) pAssociateReq->coord_addr_mode;
    if (mlme_assoc_req_params.coord_addr_mod == SHORT_ADDRS_16) {
          MEMCPY(mlme_assoc_req_params.coord_addr, pAssociateReq->coord_address.a_short_addr, SHORT_ADDRESS_LENGTH);
      } else if(mlme_assoc_req_params.coord_addr_mod == EXT_ADDRS_64) {
            MEMCPY(mlme_assoc_req_params.coord_addr, pAssociateReq->coord_address.a_extend_addr, EXT_ADDRESS_LENGTH);
      }

    mlme_assoc_req_params.capab_info = pAssociateReq->capability_information;
    
  /* Security parameters */
  mlme_assoc_req_params.sec_params.sec_level = (sec_level_enum_t) pAssociateReq->security_level;
  if (mlme_assoc_req_params.sec_params.sec_level != 0) {
#if SUPPORT_SEC
    // Parse Security parameters
    mlme_assoc_req_params.sec_params.key_id_mod = pAssociateReq->key_id_mode;
    switch(mlme_assoc_req_params.sec_params.key_id_mod) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY(mlme_assoc_req_params.sec_params.key_src, pAssociateReq->a_key_source, KEY_SRC_SIZE_MOD_2);
        break;
      case MODE_THREE:
        MEMCPY(mlme_assoc_req_params.sec_params.key_src, pAssociateReq->a_key_source, KEY_SRC_SIZE_MOD_3);
        break;
      default:
        ;
      /* An error will be generated in error checking in mac.c */
      }
      mlme_assoc_req_params.sec_params.key_indx = pAssociateReq->key_index;
#else    
   mlme_assoc_req_params.sec_params.sec_level = (sec_level_enum_t) 0;
#endif
    }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEAssociationReq function\r\n");
#endif
  
    // check status matching?
    status = mlme_assoc_req((uint32_t) &g_mac_ctx[0], &mlme_assoc_req_params);

  return status;
}



/**
* @brief  This function is used to handle a MAC MLME Poll REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 *
 * @param  MAC_pollReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEPollReq( MAC_handle * st_mac_hndl ,const ST_MAC_pollReq_t * pPollReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
  mlme_poll_req_param_st st_poll_req_param;

  st_poll_req_param.coord_addr_mod = (mac_addrs_mode_enum_t) pPollReq->coord_addr_mode;
  st_poll_req_param.coord_pan_id = READ_2_BYTES(pPollReq->a_coord_PAN_id, 0);

  if (st_poll_req_param.coord_addr_mod == EXT_ADDRS_64) {
        MEMCPY(st_poll_req_param.coord_addr, pPollReq->coord_address.a_extend_addr, EXT_ADDRESS_LENGTH);
      } else if (st_poll_req_param.coord_addr_mod == SHORT_ADDRS_16) {
        MEMCPY(st_poll_req_param.coord_addr, pPollReq->coord_address.a_short_addr,SHORT_ADDRESS_LENGTH);
      }
  
  /* Security parameters */
  st_poll_req_param.sec_params.sec_level = ( sec_level_enum_t ) pPollReq->security_level;
  if ( st_poll_req_param.sec_params.sec_level != 0 ) {
#if SUPPORT_SEC
    st_poll_req_param.sec_params.key_id_mod = pPollReq->key_id_mode;
    switch ( st_poll_req_param.sec_params.key_id_mod ) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( st_poll_req_param.sec_params.key_src, pPollReq->a_key_source, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( st_poll_req_param.sec_params.key_src, pPollReq->a_key_source, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
      }

    st_poll_req_param.sec_params.key_indx = pPollReq->key_index;
#else    
   st_poll_req_param.sec_params.sec_level = (sec_level_enum_t) 0;
#endif
    }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEPollReq function\r\n");
#endif
  
  status = mlme_poll_req((uint32_t)&g_mac_ctx[0], &st_poll_req_param);
  
  return status;
}


/**
* @brief  This function is used to handle a MAC MLME Get-Power-Information-Table REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 *
 * @param  MAC_getPwrInfoTableReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEGetPwrInfoTableReq( MAC_handle * st_mac_hndl ,const ST_MAC_getPwrInfoTableReq_t * pGetPwrInfoTableReq ) 
{
  MAC_Status_t status = MAC_SUCCESS;
 
  uint8_t  dev_address[EXT_ADDRESS_LENGTH] = {0};

#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEGetPwrInfoTableReq function\r\n");
#endif
  
  if (&(pGetPwrInfoTableReq->ieee_address)[0] == NULL) {
    MAC_MEMCPY(dev_address, &pGetPwrInfoTableReq->short_address, SHORT_ADDRESS_LENGTH);
    mlme_get_pwr_info_table_req( (uint32_t) &g_mac_ctx[0], SHORT_ADDRS_16, dev_address);
  } else {
    MAC_MEMCPY(dev_address, pGetPwrInfoTableReq->ieee_address, EXT_ADDRESS_LENGTH);
    mlme_get_pwr_info_table_req( (uint32_t) &g_mac_ctx[0], EXT_ADDRS_64, dev_address);
  }
  
  return status;
}


/**
* @brief  This function is used to handle a MAC MLME Set-Power-Information-Table REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 *
 * @param  MAC_setPwrInfoTableReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMESetPwrInfoTableReq( MAC_handle * st_mac_hndl ,const ST_MAC_setPwrInfoTableReq_t * pSetPwrInfoTableReq ) {
  
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  uint8_t *ptr_ext_addrs = (uint8_t*)(pSetPwrInfoTableReq->ieee_address); 
  uint8_t nwk_negociated = (pSetPwrInfoTableReq->nwk_negociated != 0) ? 1 : 0;

#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMESetPwrInfoTableReq function\r\n");
#endif
  
  status = mlme_set_pwr_info_table_req( (uint32_t) &g_mac_ctx[0], 
                                        pSetPwrInfoTableReq->short_address,
                                        ptr_ext_addrs,
                                        pSetPwrInfoTableReq->tx_pwr_level,
                                        pSetPwrInfoTableReq->last_rssi_level,
                                        nwk_negociated,
                                        1);  
    
  return status;
}


/**
 * @brief  This function is used to handle a MAC MLME Dps REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_dpsReq_t ref
 * @retval MAC_NOT_IMPLEMENTED_STATUS
 */
MAC_Status_t ST_MAC_handle_MLMEDpsReq( MAC_handle * st_mac_hndl ,const ST_MAC_dpsReq_t * pDpsReq)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
  return status;
}



/**
 * @brief  This function is used to handle a MAC MLME sounding REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_soundingReq_t ref
 * @retval MAC_NOT_IMPLEMENTED_STATUS
 */
MAC_Status_t ST_MAC_handle_MLMESoundingReq( MAC_handle * st_mac_hndl ,const ST_MAC_soundingReq_t * pSoundingReq)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
  return status;
}


/**
 * @brief  This function is used to handle a MAC MLME Calibrate REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_calibrateReq_t ref
 * @retval MAC_NOT_IMPLEMENTED_STATUS
 */
MAC_Status_t ST_MAC_handle_MLMECalibrateReq( MAC_handle * st_mac_hndl ,const ST_MAC_calibrateReq_t * pCalibrateReq)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Beacon REQ as described
 *         in IEEE Std 802.15.4-2020 standard
 *
 * @param  MAC_beaconReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEBeaconReq(MAC_handle * st_mac_hndl ,const ST_MAC_beaconReq_t * pbeaconReq){
 
  MAC_Status_t status = MAC_NOT_SUPPORTED_STATUS;
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_SEND_BEACON)
  mlme_bcon_send_req_params_st_t st_bcon_send_req_param;
    
  MEMSET(&st_bcon_send_req_param,0x00,sizeof(mlme_bcon_send_req_params_st_t));

  st_bcon_send_req_param.enum_bcon_typ = (bcon_typ_t) pbeaconReq->beacon_type;
  st_bcon_send_req_param.chnnl_num = pbeaconReq->channel_number;
  st_bcon_send_req_param.chnnl_page  = pbeaconReq->channel_page;
  st_bcon_send_req_param.super_frm_ord  = pbeaconReq->superframe_order;
    
  /* For header IE list */
  st_bcon_send_req_param.hdr_ie_list_count = pbeaconReq->hdr_ie_list.hdr_ie_list_count;
  if (st_bcon_send_req_param.hdr_ie_list_count != 0x00) {
    st_bcon_send_req_param.hdr_ie_total_len =pbeaconReq->hdr_ie_list.hdr_ie_list_size;
    ASSERT(st_bcon_send_req_param.hdr_ie_total_len <= MAX_HDR_IE_SIZE, SEVERITY_HIGH);
    st_bcon_send_req_param.ptr_hdr_ie_list = (uint8_t *) pbeaconReq->hdr_ie_list.ptr_hdr_ie_list;
  }
  /* For header IE list */
  st_bcon_send_req_param.pyld_ie_list_count = pbeaconReq->pyld_ie_list.pyld_ie_list_count;
  if (st_bcon_send_req_param.pyld_ie_list_count != 0x00) {
    st_bcon_send_req_param.pyld_ie_total_len =pbeaconReq->pyld_ie_list.pyld_ie_list_size;
    ASSERT(st_bcon_send_req_param.pyld_ie_total_len <= MAX_MAC_SAFE_PAYLOAD_SIZE, SEVERITY_HIGH);
    st_bcon_send_req_param.ptr_pyld_ie_list = (uint8_t *) pbeaconReq->pyld_ie_list.ptr_pyld_ie_list;
  }

  /* Security parameters */
  st_bcon_send_req_param.enum_mcps_security_level = (sec_level_enum_t) pbeaconReq->beacon_security_level;
  if (st_bcon_send_req_param.enum_mcps_security_level != 0) {
#if SUPPORT_SEC
    // Parse Security parameters
    st_bcon_send_req_param.key_id_mode = pbeaconReq->beacon_key_id_mode;
    switch(st_bcon_send_req_param.key_id_mode) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        // Attention must be paied here as never used and checked
        MEMCPY(st_bcon_send_req_param.key_src, pbeaconReq->beacon_key_source, KEY_SRC_SIZE_MOD_2);
        break;
      case MODE_THREE:
        // Attention must be paied here as never used and checked
        MEMCPY(st_bcon_send_req_param.key_src, pbeaconReq->beacon_key_source, KEY_SRC_SIZE_MOD_3);
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
    }
    st_bcon_send_req_param.key_index = pbeaconReq->beacon_key_index;
#else    
   st_bcon_send_req_param.enum_mcps_security_level = (sec_level_enum_t) 0;
#endif
  }
    
    st_bcon_send_req_param.enum_src_addrs_mode = (mac_addrs_mode_enum_t) pbeaconReq->src_addr_mode;
    st_bcon_send_req_param.enum_dstn_addrs_mode = (mac_addrs_mode_enum_t) pbeaconReq->dst_addr_mode;
    if (st_bcon_send_req_param.enum_dstn_addrs_mode == SHORT_ADDRS_16) {
          MEMCPY(st_bcon_send_req_param.dstn_addrs, pbeaconReq->dst_address.a_short_addr, SHORT_ADDRESS_LENGTH);
      } else if(st_bcon_send_req_param.enum_dstn_addrs_mode == EXT_ADDRS_64) {
            MEMCPY(st_bcon_send_req_param.dstn_addrs, pbeaconReq->dst_address.a_extend_addr, EXT_ADDRESS_LENGTH);
      }
    st_bcon_send_req_param.bsn_supr = pbeaconReq->BSN_suppression;
    
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEBeaconReq function\r\n");
#endif
    
    status = mlme_bcon_send_req((uint32_t) &g_mac_ctx[0], &st_bcon_send_req_param);
#endif
    return status;
}


/**
 * @brief  This function is used to handle a MAC MLME Associate RES as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_associateRes_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEAssociateRes(MAC_handle * st_mac_hndl ,const ST_MAC_associateRes_t * pAssociateRes)
{
  MAC_Status_t status = MAC_NOT_SUPPORTED_STATUS;
  
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_ASSOCIATION_IND_RSP)
  mlme_assoc_res_param_st mlme_assoc_resp_params;

  MEMCPY(mlme_assoc_resp_params.dev_addr, pAssociateRes->a_device_address, EXT_ADDRESS_LENGTH);
  MEMCPY(&mlme_assoc_resp_params.dev_short_addr, pAssociateRes->a_assoc_short_address, SHORT_ADDRESS_LENGTH);
  mlme_assoc_resp_params.status = (mlme_assoc_status_enum_t) pAssociateRes->status;
  mlme_assoc_resp_params.sec_params.sec_level = (sec_level_enum_t) pAssociateRes->security_level; // None Zero Value not yet supported

#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEAssociateRes function\r\n");
#endif
  
  status = mlme_assoc_res((uint32_t) &g_mac_ctx[0], &mlme_assoc_resp_params);
#endif
  
  return status;
}


/**
 * @brief  This function is used to handle a MAC MLME Orphan RES as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_orphanRes_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_handle_MLMEOrphanRes(MAC_handle * st_mac_hndl ,const ST_MAC_orphanRes_t * pOrphanRes)
{
  MAC_Status_t status = MAC_NOT_SUPPORTED_STATUS;
  
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_ORPHAN_IND_RSP)
  mlme_orphan_rsp_st_t st_orphan_rsp_params;

  MEMCPY(st_orphan_rsp_params.orphan_addr, pOrphanRes->a_orphan_address,EXT_ADDRESS_LENGTH);
  st_orphan_rsp_params.shrt_addr = READ_2_BYTES(pOrphanRes->a_short_address, 0);
  st_orphan_rsp_params.assoc_member = pOrphanRes->associated_member;  
  
  /* Security parameters */
  st_orphan_rsp_params.sec_params.sec_level = (sec_level_enum_t) pOrphanRes->security_level;
  if (st_orphan_rsp_params.sec_params.sec_level != 0) {
#if SUPPORT_SEC
  // Parse Security parameters
  st_orphan_rsp_params.sec_params.key_id_mod = pOrphanRes->key_id_mode;
  switch(st_orphan_rsp_params.sec_params.key_id_mod) {
    case MODE_ZERO:
    case MODE_ONE:
      /* As for these cases key source shall be zero octet */
      break;
    case MODE_TWO:
      // Attention must be paied here as never used and checked
      MEMCPY(st_orphan_rsp_params.sec_params.key_src, pOrphanRes->a_key_source, KEY_SRC_SIZE_MOD_2);
      break;
    case MODE_THREE:
      // Attention must be paied here as never used and checked
      MEMCPY(st_orphan_rsp_params.sec_params.key_src, pOrphanRes->a_key_source, KEY_SRC_SIZE_MOD_3);
      break;
    default:
        ;
        /* An error will be generated in error checking in mac.c */
  }
  st_orphan_rsp_params.sec_params.key_indx = pOrphanRes->key_index;
#else    
   st_orphan_rsp_params.sec_params.sec_level = (sec_level_enum_t) 0;
#endif
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEOrphanRes function\r\n");
#endif
  
  status = mlme_orphan_rsp((uint32_t) &g_mac_ctx[0], &st_orphan_rsp_params);
#endif  
  
  return status;
}


void ST_MAC_handle_MLMEAssociationCnf(void* mac_cntx_ptr, uint8_t status, uint16_t short_addr, prim_sec_param_st* ptr_sec_params){
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_associateCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_ASSOCIATION_CNF_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = status;
  MAC_MEMCPY ( mac_msg.a_assoc_short_address, ( uint8_t * ) &short_addr, g_SHORT_ADDRESS_LENGTH_c );
  /* Security parameters */
  mac_msg.security_level = (sec_level_enum_t) ptr_sec_params->sec_level;
  if (mac_msg.security_level != 0) {
#if SUPPORT_SEC
  // Parse Security parameters
  mac_msg.key_id_mode = ptr_sec_params->key_id_mod;
  switch(mac_msg.key_id_mode) {
    case MODE_ZERO:
    case MODE_ONE:
      /* As for these cases key source shall be zero octet */
      break;
    case MODE_TWO:
      // Attention must be paied here as never used and checked
      MEMCPY(mac_msg.a_key_source, ptr_sec_params->key_src, KEY_SRC_SIZE_MOD_2);
      break;
    case MODE_THREE:
      // Attention must be paied here as never used and checked
      MEMCPY(mac_msg.a_key_source, ptr_sec_params->key_src, KEY_SRC_SIZE_MOD_3);
      break;
    default:
        ;
        /* An error will be generated in error checking in mac.c */
  }
  mac_msg.key_index = ptr_sec_params->key_indx;
#else    
   mac_msg.security_level  = (sec_level_enum_t) 0;
#endif
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEAssociationCnf function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_associateCnf_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEAssociationCnf failed, buffer issue\r\n");
  }
  
   
}

#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_ASSOCIATION_IND_RSP)
void ST_MAC_handle_MLMEAssociationInd(void* mac_cntx_ptr, mlme_assoc_ind_param_st *ptr_mlme_assoc_ind_params){
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_associateInd_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_ASSOCIATION_IND_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */

  MAC_MEMCPY ( mac_msg.a_device_address, ( uint8_t * ) ptr_mlme_assoc_ind_params->dev_addr, EXT_ADDRESS_LENGTH );
  mac_msg.capability_information = ptr_mlme_assoc_ind_params->cap_info;
    
  /* Security parameters */
  mac_msg.security_level = ptr_mlme_assoc_ind_params->sec_params.sec_level;
  if (mac_msg.security_level != 0) {
#if SUPPORT_SEC
    mac_msg.key_id_mode = ptr_mlme_assoc_ind_params->sec_params.key_id_mod;
    switch ( ptr_mlme_assoc_ind_params->sec_params.key_id_mod ) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( mac_msg.a_key_source, ptr_mlme_assoc_ind_params->sec_params.key_src, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( mac_msg.a_key_source, ptr_mlme_assoc_ind_params->sec_params.key_src, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
     }
    mac_msg.key_index = ptr_mlme_assoc_ind_params->sec_params.key_indx;
#else    
   mac_msg.security_level = (sec_level_enum_t) 0;
#endif
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEAssociationInd function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_associateInd_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEAssociationInd failed, buffer issue\r\n");
  }
  // Done - Nothing to return 
}
#endif

void ST_MAC_handle_MLMEScanCnf(void* mac_cntx_ptr,mlme_scn_cmf_param_st* mlme_scan_cmf_params){
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_scanCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_SCAN_CNF_INT_MSG_ID;

  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = mlme_scan_cmf_params->status;
  mac_msg.scan_type = mlme_scan_cmf_params->scn_type;
  mac_msg.channel_page = mlme_scan_cmf_params->chnl_pge;
  WRITE_4_BYTES(mac_msg.a_unscanned_channels, 0, mlme_scan_cmf_params->unscan_chnls);
  mac_msg.result_list_size = mlme_scan_cmf_params->rslt_size;
  
  MEMSET(mac_msg.a_energy_detect_list, 0x00, g_MAX_ED_SCAN_RESULTS_SUPPORTED_c);
  MEMSET(mac_msg.a_PAN_descriptor_list, 0x00, sizeof(ST_MAC_PAN_Desc_t) * g_MAX_PAN_DESC_SUPPORTED_c);
  
  switch (mlme_scan_cmf_params->scn_type)
  {
  case ED_SCAN:
    if (mlme_scan_cmf_params->enrgy_detec_lst != NULL) {
      /* Energy detection */
      for (uint8_t  i=0; i<mlme_scan_cmf_params->rslt_size; i+=1) {
        mac_msg.a_energy_detect_list[i] = mlme_scan_cmf_params->enrgy_detec_lst[i]; 
      }
    }
    break;
  case MAC_PASS_SCAN:
  case MAC_ACTIV_SCAN:
    // There is a strange triming in HCI Transport mode... as mlme_scan_cmf_params has been updated...
    /* Active scan || Passive Scan*/ 
    for (uint8_t pan_descr_lst_idx=0; pan_descr_lst_idx<mlme_scan_cmf_params->rslt_size; pan_descr_lst_idx+=1) {
      mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].coord_addr_mode = mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].coord_addr_mod;
      MAC_MEMCPY( mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].a_coord_PAN_id, (uint8_t *) &mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].coord_pan_id, g_SHORT_PAN_ID_LENGTH_c);
      if (mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].coord_addr_mod == SHORT_ADDRS_16) {
        MAC_MEMCPY(mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].coord_addr.a_short_addr, mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].coord_addr, g_SHORT_ADDRESS_LENGTH_c);
      }
      else if (mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].coord_addr_mod == EXT_ADDRS_64) {
        MAC_MEMCPY(mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].coord_addr.a_extend_addr, mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].coord_addr, g_EXTENDED_ADDRESS_LENGTH_c);
      }
      mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].logical_channel = mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].logic_chanl; 
      /* not depiced in nts_interface.h */
      mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].channel_page = mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].chnl_pge;
      MAC_MEMCPY( mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].a_superframe_spec, (uint8_t *) &mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].super_frm_spec, 2);
      mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].gts_permit = (uint8_t) mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].gts_perm;
      mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].link_quality = mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].link_qual;
      /* as we have added the channel_page, let's sqy thqt the time stamp is only 3 bytes */
      WRITE_3_BYTES(mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].a_time_stamp, 0 , mlme_scan_cmf_params->pan_descr_lst[pan_descr_lst_idx].time_stamp );
      mac_msg.a_PAN_descriptor_list[pan_descr_lst_idx].security_level = 0 ; //Security level is set to zero;
    }
    break;
  case ORPH_SCAN:
    // TBC : nothing to do ?
  default:
    break;
  } 
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEScanCnf function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_scanCnf_t), &mac_msg);
    
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEScanCnf failed, buffer issue\r\n");
  }
  // Done - Nothing to return 
  
}

#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_ORPHAN_IND_RSP)
void ST_MAC_handle_MLMEOrphanInd(void* mac_cntx_ptr, mlme_orphan_ind_st_t *ptr_mlme_orphan_ind_params){
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_orphanInd_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_ORPHAN_IND_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  MAC_MEMCPY(mac_msg.a_orphan_address, (uint8_t *)ptr_mlme_orphan_ind_params->orphan_addr,g_EXTENDED_ADDRESS_LENGTH_c);
  
  /* Security parameters */
  mac_msg.security_level = (sec_level_enum_t) ptr_mlme_orphan_ind_params->sec_params.sec_level;
  if (mac_msg.security_level != 0) {
#if SUPPORT_SEC
  // Parse Security parameters
  mac_msg.key_id_mode = ptr_mlme_orphan_ind_params->sec_params.key_id_mod;
  switch(mac_msg.key_id_mode) {
    case MODE_ZERO:
    case MODE_ONE:
      /* As for these cases key source shall be zero octet */
      break;
    case MODE_TWO:
      // Attention must be paied here as never used and checked
      MEMCPY(mac_msg.a_key_source, ptr_mlme_orphan_ind_params->sec_params.key_src, KEY_SRC_SIZE_MOD_2);
      break;
    case MODE_THREE:
      // Attention must be paied here as never used and checked
      MEMCPY(mac_msg.a_key_source, ptr_mlme_orphan_ind_params->sec_params.key_src, KEY_SRC_SIZE_MOD_3);
      break;
    default:
        ;
        /* An error will be generated in error checking in mac.c */
  }
  mac_msg.key_index = ptr_mlme_orphan_ind_params->sec_params.key_indx;
#else    
   mac_msg.security_level  = (sec_level_enum_t) 0;
#endif
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEOrphanInd function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_orphanInd_t), &mac_msg);
    
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEOrphanInd failed, buffer issue\r\n");
  }
  // Done - Nothing to return 

}
#endif

void ST_MAC_handle_MLMEComStatusInd(void* mac_cntx_ptr,mlme_comm_status_st_t *ptr_mlme_comm_status){
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_commStatusInd_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_COM_STATUS_IND_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);

  /*  Fill mac struct */
  MAC_MEMCPY( mac_msg.a_PAN_id, (uint8_t *)&ptr_mlme_comm_status->pan_id, g_SHORT_PAN_ID_LENGTH_c);
  mac_msg.src_addr_mode = ptr_mlme_comm_status->src_addr_mode;
  mac_msg.dst_addr_mode =  ptr_mlme_comm_status->dst_addr_mode;
  mac_msg.status = ptr_mlme_comm_status->status;

  if (mac_msg.src_addr_mode == g_EXTENDED_ADDR_MODE_c) {
    MAC_MEMCPY( mac_msg.src_address.a_extend_addr, (uint8_t *)ptr_mlme_comm_status->src_add,g_EXTENDED_ADDRESS_LENGTH_c);
  } else {
    MAC_MEMCPY( mac_msg.src_address.a_short_addr, (uint8_t *)ptr_mlme_comm_status->src_add, g_SHORT_ADDRESS_LENGTH_c);
  }
  if (mac_msg.dst_addr_mode == g_EXTENDED_ADDR_MODE_c) {
    MAC_MEMCPY( mac_msg.dst_address.a_extend_addr, (uint8_t *)ptr_mlme_comm_status->dst_addr,g_EXTENDED_ADDRESS_LENGTH_c );
  } else {
    MAC_MEMCPY( mac_msg.dst_address.a_short_addr, (uint8_t *)ptr_mlme_comm_status->dst_addr, g_SHORT_ADDRESS_LENGTH_c);
  }
  
  /* Security parameters */
  mac_msg.security_level = (sec_level_enum_t) ptr_mlme_comm_status->sec_params.sec_level;
  if (mac_msg.security_level != 0) {
#if SUPPORT_SEC
  // Parse Security parameters
  mac_msg.key_id_mode = ptr_mlme_comm_status->sec_params.key_id_mod;
  switch(mac_msg.key_id_mode) {
    case MODE_ZERO:
    case MODE_ONE:
      /* As for these cases key source shall be zero octet */
      break;
    case MODE_TWO:
      // Attention must be paied here as never used and checked
      MEMCPY(mac_msg.a_key_source, ptr_mlme_comm_status->sec_params.key_src, KEY_SRC_SIZE_MOD_2);
      break;
    case MODE_THREE:
      // Attention must be paied here as never used and checked
      MEMCPY(mac_msg.a_key_source, ptr_mlme_comm_status->sec_params.key_src, KEY_SRC_SIZE_MOD_3);
      break;
    default:
        ;
        /* An error will be generated in error checking in mac.c */
  }
  mac_msg.key_index = ptr_mlme_comm_status->sec_params.key_indx;
#else    
   mac_msg.security_level  = (sec_level_enum_t) 0;
#endif
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEComStatusInd function\r\n");
#endif
  
  /* Note: all secutity is ignored */
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_commStatusInd_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEComStatusInd failed, buffer issue\r\n");
  }
}

void ST_MAC_handle_MLMEDataPollCnf(void* mac_cntx_ptr, uint8_t status){
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_pollCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_POLL_CNF_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = status;
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEDataPollCnf function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_pollCnf_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEPollCnf failed, buffer issue\r\n");
  }
  // Done - Nothing to return 
}


void ST_MAC_handle_MLMEDataPollInd(void* mac_cntx_ptr, uint8_t addr_mode, uint8_t* dev_addr){
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_pollInd_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_POLL_IND_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.addr_mode = addr_mode;
  if (addr_mode == g_EXTENDED_ADDR_MODE_c) {
    MAC_MEMCPY( &(mac_msg.request_address), (uint8_t *)dev_addr,g_EXTENDED_ADDRESS_LENGTH_c);
  } else {
    MAC_MEMCPY( &(mac_msg.request_address), (uint8_t *)dev_addr, g_SHORT_ADDRESS_LENGTH_c);
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEDataPollInd function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_pollInd_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEDataPollInd failed, buffer issue\r\n");
  }
  // Done - Nothing to return 
}

void ST_MAC_handle_MLMESyncLossInd(void* mac_cntx_ptr, mlme_sync_loss_params_st_t* ptr_st_mlme_sync_loss){
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_syncLoss_t mac_msg;
  MAC_handle st_mac_hndl;
  command_id = ST_MAC_SYNC_LOSS_IND_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  MAC_MEMCPY ( mac_msg.a_PAN_id, ( uint8_t * )&ptr_st_mlme_sync_loss->pan_id , g_SHORT_PAN_ID_LENGTH_c );
  mac_msg.channel_number = ptr_st_mlme_sync_loss->chnl_num;
  mac_msg.channel_page = ptr_st_mlme_sync_loss->chnl_pge;
  mac_msg.loss_reason = ptr_st_mlme_sync_loss->loss_reason;
  
  /* Security parameters */
  mac_msg.security_level = ptr_st_mlme_sync_loss->sec_params.sec_level;
  if (mac_msg.security_level != 0) {
#if SUPPORT_SEC
    mac_msg.key_id_mode = ptr_st_mlme_sync_loss->sec_params.key_id_mod;
    switch ( ptr_st_mlme_sync_loss->sec_params.key_id_mod ) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( mac_msg.a_key_source, ptr_st_mlme_sync_loss->sec_params.key_src, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( mac_msg.a_key_source, ptr_st_mlme_sync_loss->sec_params.key_src, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
     }
    mac_msg.key_index = ptr_st_mlme_sync_loss->sec_params.key_indx;
#else    
   mac_msg.security_level = (sec_level_enum_t) 0;
#endif
  }
      
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMESyncLossInd function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_syncLoss_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMESynclossInd failed, buffer issue\r\n");
  }
}

void ST_MAC_handle_MLMEDisassociationCnf(void* mac_cntx_ptr, mlme_disassoc_cfm_params_st *ptr_mlme_disassoc_cfm_params) {
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_disassociateCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  attr_val_t pib_attr_value;
  uint8_t tab_macCoordExtendedAddress[EXT_ADDRESS_LENGTH];
  
  command_id = ST_MAC_DISASSOCIATION_CNF_INT_CMD_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = ptr_mlme_disassoc_cfm_params->status;
  mac_msg.device_addr_mode = ptr_mlme_disassoc_cfm_params->dev_addr_mode;  
  MAC_MEMCPY ( mac_msg.a_device_PAN_id, ( uint8_t * ) &(ptr_mlme_disassoc_cfm_params->dev_pan_id), g_SHORT_PAN_ID_LENGTH_c );
  MAC_MEMCPY ( mac_msg.device_address.a_extend_addr, ptr_mlme_disassoc_cfm_params->dev_addr, g_EXTENDED_ADDRESS_LENGTH_c );
  
  
  /* macCoordExtendedAddress to 0x0000000000000000 */
  if (mac_msg.status == MAC_SUCCESS) {//if success
    reset_macCoordExtendedAddress = 0x01;
    memset (&tab_macCoordExtendedAddress, 0x00, EXT_ADDRESS_LENGTH);
    pib_attr_value.attr_array.data = (uint8_t *) &tab_macCoordExtendedAddress;
    pib_attr_value.attr_array.length = EXT_ADDRESS_LENGTH;
    mlme_set_req (( uint32_t ) mac_cntx_ptr, COORD_EXT_ADDR_ID, 0x00, &pib_attr_value);
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEDisassociationCnf function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_disassociateCnf_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEDisassociationCnf failed, buffer issue\r\n");
  }
}

void ST_MAC_handle_MLMEDisassociationInd(void* mac_cntx_ptr, mlme_disassoc_ind_st* ptr_mlme_disassoc_ind_params) {
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_disassociateInd_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_DISASSOCIATION_IND_INT_CMD_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  MAC_MEMCPY ( mac_msg.a_device_address, ptr_mlme_disassoc_ind_params->dev_addr , g_EXTENDED_ADDRESS_LENGTH_c );
  mac_msg.disassociate_reason = ptr_mlme_disassoc_ind_params->reason;
  
  /* Security parameters */
  mac_msg.security_level = ptr_mlme_disassoc_ind_params->beacn_sec_params.sec_level;
  if (mac_msg.security_level != 0) {
#if SUPPORT_SEC
    mac_msg.key_id_mode = ptr_mlme_disassoc_ind_params->beacn_sec_params.key_id_mod;
    switch ( ptr_mlme_disassoc_ind_params->beacn_sec_params.key_id_mod ) {
      case MODE_ZERO:
      case MODE_ONE:
        /* As for these cases key source shall be zero octet */
        break;
      case MODE_TWO:
        MEMCPY ( mac_msg.a_key_source, ptr_mlme_disassoc_ind_params->beacn_sec_params.key_src, KEY_SRC_SIZE_MOD_2 );
        break;
      case MODE_THREE:
        MEMCPY ( mac_msg.a_key_source, ptr_mlme_disassoc_ind_params->beacn_sec_params.key_src, KEY_SRC_SIZE_MOD_3 );
        break;
      default:
        ;
        /* An error will be generated in error checking in mac.c */
     }
    mac_msg.key_index = ptr_mlme_disassoc_ind_params->beacn_sec_params.key_indx;
#else    
   mac_msg.security_level = (sec_level_enum_t) 0;
#endif
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEDisassciationInd function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_disassociateInd_t), &mac_msg);

  // In Case of buff_status != MAC_SUCCESS we need to log the issue
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEDisassciationInd failed, buffer issue\r\n");
  }
  // Done - Nothing to return
}

#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_SEND_BEACON) 
void ST_MAC_handle_MLMEBeaconReqInd (void* mac_cntx_ptr, mlme_bcon_req_ind_params_st *ptr_bcon_req_ind_params){
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_beaconReqInd_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_BEACONREQ_IND_INT_CMD_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
   /*  Fill mac struct */
  MAC_MEMCPY ( mac_msg.src_address, ptr_bcon_req_ind_params->src_addr , g_EXTENDED_ADDRESS_LENGTH_c );
  MAC_MEMCPY ( mac_msg.a_device_PAN_id, ( uint8_t * ) &(ptr_bcon_req_ind_params->dst_pan_id), g_SHORT_PAN_ID_LENGTH_c );
  mac_msg.beacon_type = ptr_bcon_req_ind_params->bcon_type;//NORMAL_BEACON=0x00, ENHANCED_BEACON=0x01
  mac_msg.src_addr_mode = ptr_bcon_req_ind_params->src_addr_mode;
  
  mac_msg.hdr_ie_list.hdr_ie_list_count = ptr_bcon_req_ind_params->hdr_ie_list_count;
  if (mac_msg.hdr_ie_list.hdr_ie_list_count != 0 ){
    mac_msg.hdr_ie_list.hdr_ie_list_size = ptr_bcon_req_ind_params->hdr_ie_total_len;
    ASSERT(mac_msg.hdr_ie_list.hdr_ie_list_size <= MAX_HDR_IE_SIZE, SEVERITY_HIGH);
    MAC_MEMCPY ( mac_msg.hdr_ie_list.ptr_hdr_ie_list, ptr_bcon_req_ind_params->ptr_hdr_ie_list , mac_msg.hdr_ie_list.hdr_ie_list_size );
  }
  
  mac_msg.pyld_ie_list.pyld_ie_list_count = ptr_bcon_req_ind_params->pyld_ie_list_count;
  if (mac_msg.pyld_ie_list.pyld_ie_list_count != 0){
    mac_msg.pyld_ie_list.pyld_ie_list_size = ptr_bcon_req_ind_params->pyld_ie_total_len;
    ASSERT(mac_msg.pyld_ie_list.pyld_ie_list_size <= MAX_ZIGBEE_EBR_IE_LEN, SEVERITY_HIGH);
    MAC_MEMCPY ( mac_msg.pyld_ie_list.ptr_pyld_ie_list, ptr_bcon_req_ind_params->ptr_pyld_ie_list , mac_msg.pyld_ie_list.pyld_ie_list_size );
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEBeaconReqInd function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_beaconReqInd_t), &mac_msg);
  
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEBeaconReqInd failed, buffer issue\r\n");
  }
}
#endif

void ST_MAC_handle_MLMERxEnableCnf(void* mac_cntx_ptr, uint8_t status) {
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_rxEnableCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_RX_ENABLE_CNF_INT_CMD_ID;

  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = status;
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMERxEnableCnf function\r\n");
#endif
  
  /* Enqueue Standardized Incoming Message */
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_rxEnableCnf_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMERxEnableCnf failed, buffer issue\r\n");
  }
  // Done - Nothing to return
}

/**
    Call back to handle Reset Cnf From SNPS layer.
**/

void ST_MAC_handle_MLMEResetCnf(void* mac_cntx_ptr, uint8_t status) {
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_resetCnf_t mac_msg;
  MAC_handle st_mac_hndl;  
  command_id = ST_MAC_RESET_CNF_INT_MSG_ID;

  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = status;

#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEResetCnf function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_resetCnf_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEResetCnf failed, buffer issue\r\n");
  }
}

void ST_MAC_handle_MLMESetCnf(void* mac_cntx_ptr, uint8_t status, uint8_t pib_attr_id, uint8_t pib_attr_indx)  {
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_setCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_SET_CNF_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status        = status;
  mac_msg.PIB_attribute = pib_attr_id;
  
  /* re-align PIB attr id between SNPS and Exegin if needed */
  switch (pib_attr_id) {
    case CUSTOM_IEEE_EXTD_ADDRS:
      mac_msg.PIB_attribute = g_MAC_EXTENDED_ADDRESS_c;
      break;            
      
    default:
      break;
  }
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMESetCnf function, status=0x%02x, pib_attr_id=0x%02x\r\n", status, pib_attr_id);
#endif
  
  if(!reset_macCoordExtendedAddress) {
    enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_setCnf_t), &mac_msg);
  }else{
    reset_macCoordExtendedAddress = !reset_macCoordExtendedAddress;
  }
  // In Case of buff_status != MAC_SUCCESS we need to log the issue
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMESetCnf failed, buffer issue\r\n");
  }
  // Done - Nothing to return
}

void ST_MAC_handle_MLMEStartCnf (void* mac_cntx_ptr, uint8_t status) {
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_startCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_START_CNF_INT_MSG_ID;

  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = status;
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMESartCnf function\r\n");
#endif  
  
  /* Enqueue Standardized Incoming Message */
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_startCnf_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMESartCnf failed, buffer issue\r\n");
  }
  // Done - Nothing to return
}

void ST_MAC_handle_MLMEGetCnf(void* mac_cntx_ptr, uint8_t status, uint8_t pib_attr_id, uint8_t pib_attr_indx ,attr_val_t* pib_attr_val){
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_getCnf_t mac_msg;
  MAC_handle st_mac_hndl;

  uint8_t attributeValueLen;
  
  uint8_t * payload;
  uint8_t payload_len;
  
  command_id = ST_MAC_GET_CNF_INT_MSG_ID;

  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);

  /*  Fill mac struct */
  mac_msg.status = status;
  mac_msg.PIB_attribute = pib_attr_id;
  attributeValueLen   = mac_get_pib_attribute_len(mac_cntx_ptr,pib_attr_id);
  mac_msg.PIB_attribute_value_len = attributeValueLen;
  mac_msg.PIB_attribute_value[0] = 0x00; // Pointer Initilization
 
  	switch (pib_attr_id)
	{
	case BEACON_PAY_ID:
	case COORD_EXT_ADDR_ID:
	case PAN_COORD_EXT_ADDR_ID:
	case DFLT_KEY_SRC_ID:
	case AUTO_REQ_KEY_SRC_ID:
	case SEC_LVL_TBL_ID:
	case DEVICE_TBL_ID:
	case KEY_TBL_ID:
		payload_len = pib_attr_val->attr_array.length;
		payload = pib_attr_val->attr_array.data;
		break;
        case CUSTOM_IEEE_EXTD_ADDRS:
		payload_len = pib_attr_val->attr_array.length;
		payload = pib_attr_val->attr_array.data;
                mac_msg.PIB_attribute = g_MAC_EXTENDED_ADDRESS_c;
		break;
        case EB_HDR_IE_LIST_ID:
	case EB_PYLD_IE_LIST_ID:
                payload_len = pib_attr_val->attr_array.length;
                mac_msg.PIB_attribute_value_len = pib_attr_val->attr_array.length;//return a list with the same size
		payload = pib_attr_val->attr_array.data;
                break;
        case MIB_JOINING_IEEE_LIST_ID:
          payload_len = mac_msg.PIB_attribute_value_len;
	  payload = pib_attr_val->attr_array.data;
          break;
	case PAN_ID_ID:
	case COORD_SHRT_ADDR_ID:
	case PERSISTEN_TIME_ID:
	case SYNC_SYMB_OFFS_ID:
	case MIB_EXPIRY_INTRVL_ID:
	case PHY_MAX_FRAME_DUR:
        case SHORT_ADDR_ID: 
	case PAN_COORD_SHRT_ADDR_ID:
                payload_len = 2 ; // pib_attr_val->attr_array.length;
                payload = (uint8_t *)&pib_attr_val->attr_int;
		break;
	case BEACON_TX_TIME_ID:
                payload_len = 3;
                payload = (uint8_t *)&pib_attr_val->attr_int;
		break;
	case MAX_FRAME_WAIT_TIME_ID:
	case MIB_EXPIRY_INTRVL_COUNTDOWN_ID:
	case PHY_CHANNEL_SUPPORTED:
	case FRM_CNTR_ID:
                payload_len = 4;
                payload = (uint8_t *)&pib_attr_val->attr_int;
		break;
	default:
		/*
		 * ACK_WAIT_DUR_ID : max = 120
		 * ASSOC_PAN_COORD_ID
		 * ASSOC_PERMIT_ID
		 * AUTO_REQ_ID
		 * BECON_PAY_LEN_ID
		 * BEACON_ORDER_ID
		 * BSN_ID
		 * EBSN_ID
		 * MAX_BE_ID
		 * MAX_CSMA_ID
		 * MAX_FRAME_RETRY_ID
		 * MIN_BE_ID
		 * PROMIS_MODE_ID
		 * RSP_WAIT_TIME_ID
		 * RX_WHEN_IDLE_ID
		 * SEC_ENABLED_ID
		 * SUPER_FRAME_ORD_ID
		 * TIME_STAMP_SUPP_ID
		 * MIB_JOINING_POLICY_ID
		 * EB_FLTR_Enbld_ID
		 * EB_AUTO_SA_ID
		 * DSN_ID
		 * PHY_CHANNEL_ID
		 * PHY_CCA_MODE
		 * PHY_CURRENT_PAGE
		 * PHY_SHR_DUR
		 * PHY_SYMBOLS_PER_OCTET
		 * EBR_PRMT_JOIN_ID
		 * EBR_FLTRS_ID
		 * EBR_LQ_ID
		 * EBR_AUTO_RSPND_ID
                 * g_PHY_TRANSMIT_POWER_c
		 * */
		payload_len = 1;
                payload = (uint8_t *)&pib_attr_val->attr_int;
		break;
	}
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEGetCnf function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming_with_payload(command_id, st_mac_hndl, sizeof(ST_MAC_getCnf_t),&mac_msg, payload, payload_len );

  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEGetCnf failed, buffer issue\r\n");
  }
  // Done - Nothing to return
} 


void ST_MAC_handle_MLMEGetPwrInfoTableCnf( void* mac_cntx_ptr, uint8_t status, uint16_t short_addrs, uint8_t  *ptr_ext_addrs,
			                            int8_t tx_pwr_level, int8_t last_rssi_level, uint8_t nwk_negotiated) {
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_getPwrInfoTableCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_GET_PWR_INFO_TABLE_CNF_INT_CMD_ID;

  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = status;
  mac_msg.short_address = short_addrs;
  if (ptr_ext_addrs != NULL) {
    MAC_MEMCPY(mac_msg.ieee_address, ptr_ext_addrs, EXT_ADDRESS_LENGTH);
  } else {
    MAC_MEMSET(mac_msg.ieee_address, 0x00, EXT_ADDRESS_LENGTH);
  }
  mac_msg.tx_pwr_level = tx_pwr_level;
  mac_msg.last_rssi_level = last_rssi_level;
  mac_msg.nwk_negociated = nwk_negotiated;
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEGetPwrInfoCnf function\r\n");
#endif
  
  /* Enqueue Standardized Incoming Message */
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_getPwrInfoTableCnf_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEGetPwrInfoCnf failed, buffer issue\r\n");
  }
  // Done - Nothing to return
}


void ST_MAC_handle_MLMESetPwrInfoTableCnf( void* mac_cntx_ptr, uint8_t status) {
  
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_setPwrInfoTableCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_SET_PWR_INFO_TABLE_CNF_INT_CMD_ID;

  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = status;
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMESetPwrInfoCnf function\r\n");
#endif
  
  /* Enqueue Standardized Incoming Message */
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_setPwrInfoTableCnf_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMESetPwrInfoCnf failed, buffer issue\r\n");
  }
  // Done - Nothing to return
}


void ST_MAC_handle_MLMEBeaconCnf(void* mac_cntx_ptr, uint8_t status){
  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_beaconCnf_t mac_msg;
  MAC_handle st_mac_hndl;
  
  command_id = ST_MAC_BEACON_CNF_INT_MSG_ID;
  
  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);
  
  /*  Fill mac struct */
  mac_msg.status = status;
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEBeaconCnf function\r\n");
#endif
  
  enqueue_status = ST_MAC_enqueue_radio_Incoming(command_id, st_mac_hndl, sizeof(ST_MAC_beaconCnf_t), &mac_msg);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEBeaconCnf failed, buffer issue\r\n");
  }
  
   
}

/******************************************/
/* Incoming MLME IND (RADIO Layer -> MAC) */
/******************************************/
void ST_MAC_handle_MLMEBeaconNotifyInd(void* mac_cntx_ptr, mlme_bcon_notfy_params_st* ptr_bcon_notify_params) {

  uint8_t enqueue_status = MAC_SUCCESS;
  uint8_t command_id;
  ST_MAC_beaconNotifyInd_t mac_msg;
  MAC_handle st_mac_hndl;
  command_id = ST_MAC_BEACON_IND_INT_MSG_ID;

  /* Retrieve interface ID from mac context */
  st_mac_hndl = ST_MAC_get_mac_interface(mac_cntx_ptr);

  /* Fill mac struct */
  mac_msg.BSN = ptr_bcon_notify_params->bsn;
  mac_serialize_pan_desc_to_ST( &(mac_msg.PAN_descriptor) ,ptr_bcon_notify_params->pan_desc_ptr);
  mac_msg.pend_addr_spec = ptr_bcon_notify_params->pend_addrs;
  mac_msg.sduPtr[0] = 0x00; // Pointer initialization    
  MEMSET(mac_msg.a_addr_list, 0x00, sizeof(MAC_addr_t) * g_MAX_PENDING_ADDRESS_c);
 
  mac_msg.sdu_length = ptr_bcon_notify_params->sdu_length;
  mac_msg.beacon_type = ptr_bcon_notify_params->bcon_type;
  mac_msg.hdr_ie_list.hdr_ie_list_count = 0;
  mac_msg.pyld_ie_list.pyld_ie_list_count = 0;
  if(ptr_bcon_notify_params->bcon_type == ENHANCED_BEACON) {
    mac_msg.hdr_ie_list.hdr_ie_list_count = ptr_bcon_notify_params->hdr_ie_list_count;
    if(mac_msg.hdr_ie_list.hdr_ie_list_count != 0) {
      /* take 7 first bits for the length (we can add filter if we want bit 15->0x0 and bit 7-14->0x00 Vendor Specfic) */
      mac_msg.hdr_ie_list.hdr_ie_list_size = ptr_bcon_notify_params->ptr_hdr_ie_list[0] & 0x7;
      ASSERT(mac_msg.hdr_ie_list.hdr_ie_list_size <= MAX_HDR_IE_SIZE, SEVERITY_HIGH);
      MAC_MEMCPY(mac_msg.hdr_ie_list.ptr_hdr_ie_list, ptr_bcon_notify_params->ptr_hdr_ie_list, mac_msg.hdr_ie_list.hdr_ie_list_size);
    }
      
    mac_msg.pyld_ie_list.pyld_ie_list_count = ptr_bcon_notify_params->pyld_ie_list_count;
    if(mac_msg.pyld_ie_list.pyld_ie_list_count != 0) {
      /* take 11 first bits for the length (we can add filter if we want bit 15->0x1 and bit 11-14->0x02 Vendor Specfic) */
      mac_msg.pyld_ie_list.pyld_ie_list_size = ((ptr_bcon_notify_params->ptr_pyld_ie_list[1] << 8) + ptr_bcon_notify_params->ptr_pyld_ie_list[0]) & 0x7FF;
      ASSERT(mac_msg.pyld_ie_list.pyld_ie_list_size <= MAX_ZIGBEE_EBR_IE_LEN, SEVERITY_HIGH);
      MAC_MEMCPY(mac_msg.pyld_ie_list.ptr_pyld_ie_list, ptr_bcon_notify_params->ptr_pyld_ie_list, mac_msg.pyld_ie_list.pyld_ie_list_size);//Keil here use ble_memcpy
    }
  }    
  
#if MAC_WRAPPER_LOG
  LOG_INFO_APP("MLMEBeaconNotifyInd function\r\n");
#endif
  
  /* Enqueue Standardized Incoming Message */
  enqueue_status = ST_MAC_enqueue_radio_Incoming_with_payload(command_id, st_mac_hndl, sizeof(ST_MAC_beaconNotifyInd_t), &mac_msg, ptr_bcon_notify_params->sdu, mac_msg.sdu_length);
  
  /* In Case of buff_status != MAC_SUCCESS we need to log the issue */
  if (enqueue_status != MAC_SUCCESS) {
    LOG_INFO_APP("Warning MLMEBeaconNotifyInd failed, buffer issue\r\n");
  }

  // Done - Nothing to return 
}
