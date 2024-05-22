/**
  ******************************************************************************
  * @file    st_mac_802_15_4_svc.h
  * @author
  * @brief   Header for mac service (MLME / MCPS)
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

#ifndef _802_15_4_MAC_SVC_H_
#define _802_15_4_MAC_SVC_H_

/* Includes ------------------------------------------------------------------*/
#include "st_mac_802_15_4_types.h"
   
#include "mac_host_intf.h"

/* Exported defines ----------------------------------------------------------*/
#define MAX_SIZE_PIB 127

/* Exported types ------------------------------------------------------------*/

typedef struct mac_radio_msg_tag {
  uint8_t type;
  uint8_t payload_len;
  uint64_t timestamp;
  uint8_t payload[1];// Struct associated to the type
}mac_radio_msg_t;

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* general */


/* Request */
MAC_Status_t ST_MAC_handle_MLMEAssociateReq( MAC_handle * st_mac_hndl , const  ST_MAC_associateReq_t * pAssociateReq );
MAC_Status_t ST_MAC_handle_MLMEDisassociateReq( MAC_handle * st_mac_hndl , const  ST_MAC_disassociateReq_t * pDisassiociateReq );
MAC_Status_t ST_MAC_handle_MLMEGetReq( MAC_handle * st_mac_hndl ,const  ST_MAC_getReq_t * pGetReq );
MAC_Status_t ST_MAC_handle_MLMEGtsReq( MAC_handle * st_mac_hndl ,const  ST_MAC_gtsReq_t * pGtsReq );
MAC_Status_t ST_MAC_handle_MLMEResetReq( MAC_handle * st_mac_hndl ,const  ST_MAC_resetReq_t * pResetReq );
MAC_Status_t ST_MAC_handle_MLMERxEnableReq( MAC_handle * st_mac_hndl ,const  ST_MAC_rxEnableReq_t * pRxEnableReq );
MAC_Status_t ST_MAC_handle_MLMEScanReq( MAC_handle * st_mac_hndl ,const  ST_MAC_scanReq_t * pScanReq );
MAC_Status_t ST_MAC_handle_MLMESetReq( MAC_handle * st_mac_hndl ,const  ST_MAC_setReq_t * pSetReq);
MAC_Status_t ST_MAC_handle_MLMEStartReq( MAC_handle * st_mac_hndl ,const  ST_MAC_startReq_t * pStartReq);
MAC_Status_t ST_MAC_handle_MLMEPollReq( MAC_handle * st_mac_hndl ,const ST_MAC_pollReq_t * pPollReq );
MAC_Status_t ST_MAC_handle_MLMEDpsReq( MAC_handle * st_mac_hndl ,const ST_MAC_dpsReq_t * pDpsReq);
MAC_Status_t ST_MAC_handle_MLMESoundingReq( MAC_handle * st_mac_hndl ,const ST_MAC_soundingReq_t * pSoundingReq);
MAC_Status_t ST_MAC_handle_MLMECalibrateReq( MAC_handle * st_mac_hndl ,const ST_MAC_calibrateReq_t * pCalibrateReq);
MAC_Status_t ST_MAC_handle_MLMEBeaconReq(MAC_handle * st_mac_hndl ,const ST_MAC_beaconReq_t * pbeaconReq);
MAC_Status_t ST_MAC_handle_MLMEGetPwrInfoTableReq( MAC_handle * st_mac_hndl ,const ST_MAC_getPwrInfoTableReq_t * pGetPwrInfoTableReq );
MAC_Status_t ST_MAC_handle_MLMESetPwrInfoTableReq( MAC_handle * st_mac_hndl ,const ST_MAC_setPwrInfoTableReq_t * pSetPwrInfoTableReq );

MAC_Status_t ST_MAC_handle_MCPSDataReq( MAC_handle * st_mac_hndl ,const ST_MAC_dataReq_t * pDataReq );
MAC_Status_t ST_MAC_handle_MCPSPurgeReq( MAC_handle * st_mac_hndl ,const ST_MAC_purgeReq_t * pPurgeReq );

/* Response */
MAC_Status_t ST_MAC_handle_MLMEAssociateRes(MAC_handle * st_mac_hndl ,const ST_MAC_associateRes_t * pAssociateRes);
MAC_Status_t ST_MAC_handle_MLMEOrphanRes(MAC_handle * st_mac_hndl ,const ST_MAC_orphanRes_t * pOrphanRes);

/* Confirm */
void ST_MAC_handle_MLMEResetCnf(void* mac_cntx_ptr, uint8_t status);
void ST_MAC_handle_MLMESetCnf(void* mac_cntx_ptr, uint8_t status, uint8_t pib_attr_id, uint8_t pib_attr_indx);
void ST_MAC_handle_MLMEScanCnf(void* mac_cntx_ptr,mlme_scn_cmf_param_st* mlme_scan_cmf_params);
void ST_MAC_handle_MLMEAssociationCnf(void* mac_cntx_ptr, uint8_t status, uint16_t short_addr, prim_sec_param_st* ptr_sec_params);
void ST_MAC_handle_MLMEBeaconNotifyInd(void* mac_cntx_ptr, mlme_bcon_notfy_params_st* ptr_bcon_notify_params);
void ST_MAC_handle_MLMEStartCnf (void* mac_cntx_ptr, uint8_t status);
void ST_MAC_handle_MLMEAssociationCnf(void* mac_cntx_ptr, uint8_t status, uint16_t short_addr, prim_sec_param_st* ptr_sec_params);
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_ASSOCIATION_IND_RSP)
void ST_MAC_handle_MLMEAssociationInd(void* mac_cntx_ptr, mlme_assoc_ind_param_st *ptr_mlme_assoc_ind_params);
#endif
void ST_MAC_handle_MLMEGetCnf(void* mac_cntx_ptr, uint8_t status, uint8_t pib_attr_id, uint8_t pib_attr_indx ,attr_val_t* pib_attr_val);
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_ORPHAN_IND_RSP)
void ST_MAC_handle_MLMEOrphanInd(void* mac_cntx_ptr, mlme_orphan_ind_st_t *ptr_mlme_orphan_ind_params);
#endif
void ST_MAC_handle_MLMEComStatusInd(void* mac_cntx_ptr,mlme_comm_status_st_t *ptr_mlme_comm_status);
void ST_MAC_handle_MLMEDataPollCnf(void* ptr_mac_cntx, uint8_t status);
void ST_MAC_handle_MLMEDataPollInd(void* mac_cntx_ptr, uint8_t addr_mode, uint8_t* dev_addr);
void ST_MAC_handle_MLMESyncLossInd(void* mac_cntx_ptr, mlme_sync_loss_params_st_t* ptr_st_mlme_sync_loss);
void ST_MAC_handle_MLMEDisassociationCnf(void* mac_cntx_ptr, mlme_disassoc_cfm_params_st *ptr_mlme_disassoc_cfm_params);
void ST_MAC_handle_MLMEDisassociationInd(void* mac_cntx_ptr, mlme_disassoc_ind_st* ptr_mlme_disassoc_ind_params);
#if (FFD_DEVICE_CONFIG || RFD_SUPPORT_SEND_BEACON)
void ST_MAC_handle_MLMEBeaconReqInd (void* mac_cntx_ptr, mlme_bcon_req_ind_params_st *ptr_bcon_req_ind_params);
#endif
void ST_MAC_handle_MLMERxEnableCnf(void* mac_cntx_ptr, uint8_t status);
void ST_MAC_handle_MLMEBeaconCnf(void* mac_cntx_ptr, uint8_t status);
void ST_MAC_handle_MLMEGetPwrInfoTableCnf(void* mac_cntx_ptr, uint8_t status, uint16_t short_addrs, uint8_t  *ptr_ext_addrs,
			                            int8_t tx_pwr_level, int8_t  last_rssi_level, uint8_t nwk_negotiated);
void ST_MAC_handle_MLMESetPwrInfoTableCnf(void* mac_cntx_ptr, uint8_t status);

void ST_MAC_handle_MCPSDataInd(void* mac_cntx_ptr, mcps_indicate_params_st_t* ptr_mcps_data_ind);
void ST_MAC_handle_MCPSDataCnf(void* mac_cntx_ptr, mcps_data_cfm_params_st_t* ptr_mcps_data_cnf);
void ST_MAC_handle_MCPSDataPurgeCnf(void* mac_cntx_ptr, uint8_t msdu_hndl, uint8_t status);
     
#endif /* _802_15_4_MAC_SVC_H_ */
