/**
  ******************************************************************************
  * @file    st_mac_802_15_4_pib.c
  * @author  MCD-WBL Application Team
  * @brief   Mac Implementation App.
  *          This file provide service to manage MAC Pib attributes
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
#include "st_mac_802_15_4_pib.h"


// SNPS Import
#include "common_mac.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* External functions --------------------------------------------------------*/


/**
  * @brief  This function returns the pib attributes len required due to sub
            layer implementation choice.
  * @param  None
  * @retval None
  */

uint8_t MAC_PIB_get_attribute_len(void* mac_cntx_ptr,uint8_t pib_attr_id)
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
      #endif
      case MIB_JOINING_IEEE_LIST_ID:
        // To be clarify - from where we can retrieve the value
        break;
      case  ACK_WAIT_DUR_ID :
      case  ASSOC_PAN_COORD_ID:
      case  ASSOC_PERMIT_ID:
      case  AUTO_REQ_ID:
      case  BECON_PAY_LEN_ID:
      case  BEACON_ORDER_ID:
      case  BEACON_TX_TIME_ID:
      case  BSN_ID:
      case  DSN_ID:
      case  MAX_BE_ID:
      case  MAX_CSMA_ID:
      case  MAX_FRAME_RETRY_ID:
      case  MIN_BE_ID:
      case  PROMIS_MODE_ID:
      case  RSP_WAIT_TIME_ID:
      case  RX_WHEN_IDLE_ID:
      case  SEC_ENABLED_ID:
      case  SUPER_FRAME_ORD_ID:
      case  TIME_STAMP_SUPP_ID:
      case  PHY_CHANNEL_ID:
      case  PHY_CHANNEL_SUPPORTED:
      case  PHY_CCA_MODE:
      case  PHY_CURRENT_PAGE:
      case  PHY_MAX_FRAME_DUR:
      case  PHY_SHR_DUR:
      case  PHY_SYMBOLS_PER_OCTET:
      case  KEY_TBL_ID:
      case  DEVICE_TBL_ID:
      case  SEC_LVL_TBL_ID:
      case  AUTO_REQ_SEC_LVL_ID:
      case  AUTO_REQ_KEY_MODE_ID:
      case  AUTO_REQ_KEY_SRC_ID:
      case  AUTO_REQ_KEY_INDX_ID:
      case  DFLT_KEY_SRC_ID:
      case  EB_HDR_IE_LIST_ID:
      case  EB_PYLD_IE_LIST_ID:
      case  EB_FLTR_Enbld_ID:
      case  EBSN_ID:
      case  EB_AUTO_SA_ID:
      case  EBR_PRMT_JOIN_ID:
      case  EBR_FLTRS_ID:
      case  EBR_LQ_ID:
      case  EBR_AUTO_RSPND_ID:
      case  MIB_JOINING_POLICY_ID:
      case  MIB_EXPIRY_INTRVL_COUNTDOWN_ID:
        attr_len = 1;
    default:
    break;
    }
  return attr_len;
}
