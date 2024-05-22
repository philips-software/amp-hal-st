/**
  ******************************************************************************
  * @file    st_mac_802_15_4_sap.h
  * @author  MCD Application Team
  * @brief   Header for mac interface API
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
#ifndef _802_15_4_MAC_SAP_H_
#define _802_15_4_MAC_SAP_H_

/* Includes ------------------------------------------------------------------*/
#include "st_mac_802_15_4_types.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* general */
void ST_MAC_preInit(void);
MAC_Status_t ST_MAC_init(ST_MAC_callbacks_t * macCallback);

/* Request */
MAC_Status_t ST_MAC_MLMEAssociateReq( MAC_handle st_mac_hndl , const  ST_MAC_associateReq_t * pAssociateReq );
MAC_Status_t ST_MAC_MLMEDisassociateReq( MAC_handle st_mac_hndl , const  ST_MAC_disassociateReq_t * pDisassiociateReq );
MAC_Status_t ST_MAC_MLMEGetReq( MAC_handle  st_mac_hndl ,const  ST_MAC_getReq_t * pGetReq );
MAC_Status_t ST_MAC_MLMEGtsReq( MAC_handle  st_mac_hndl ,const  ST_MAC_gtsReq_t * pGtsReq );
MAC_Status_t ST_MAC_MLMEResetReq( MAC_handle  st_mac_hndl ,const  ST_MAC_resetReq_t * pResetReq );
MAC_Status_t ST_MAC_MLMERxEnableReq( MAC_handle  st_mac_hndl ,const  ST_MAC_rxEnableReq_t * pRxEnableReq );
MAC_Status_t ST_MAC_MLMEScanReq( MAC_handle  st_mac_hndl ,const  ST_MAC_scanReq_t * pScanReq );
MAC_Status_t ST_MAC_MLMESetReq( MAC_handle  st_mac_hndl ,const  ST_MAC_setReq_t * pSetReq);
MAC_Status_t ST_MAC_MLMEStartReq( MAC_handle  st_mac_hndl ,const  ST_MAC_startReq_t * pStartReq);
MAC_Status_t ST_MAC_MLMEPollReq( MAC_handle  st_mac_hndl ,const ST_MAC_pollReq_t * pPollReq );
MAC_Status_t ST_MAC_MLMEDpsReq( MAC_handle  st_mac_hndl ,const ST_MAC_dpsReq_t * pDpsReq);
MAC_Status_t ST_MAC_MLMESoundingReq( MAC_handle  st_mac_hndl ,const ST_MAC_soundingReq_t * pSoundingReq);
MAC_Status_t ST_MAC_MLMECalibrateReq( MAC_handle  st_mac_hndl ,const ST_MAC_calibrateReq_t * pCalibrateReq);
MAC_Status_t ST_MAC_MLMEBeaconReq( MAC_handle  st_mac_hndl ,const ST_MAC_beaconReq_t * pBeaconReq );
MAC_Status_t ST_MAC_MLMEGetPwrInfoTableReq( MAC_handle  st_mac_hndl, const ST_MAC_getPwrInfoTableReq_t * pGetPwrInfoTableReq );
MAC_Status_t ST_MAC_MLMESetPwrInfoTableReq( MAC_handle  st_mac_hndl, const ST_MAC_setPwrInfoTableReq_t * pSetPwrInfoTableReq );

MAC_Status_t ST_MAC_MCPSDataReq( MAC_handle  st_mac_hndl ,const ST_MAC_dataReq_t * pDataReq );
MAC_Status_t ST_MAC_MCPSPurgeReq( MAC_handle  st_mac_hndl ,const ST_MAC_purgeReq_t * pPurgeReq );

/* Response */
MAC_Status_t ST_MAC_MLMEAssociateRes(MAC_handle st_mac_hndl ,const ST_MAC_associateRes_t * pAssociateRes);
MAC_Status_t ST_MAC_MLMEOrphanRes(MAC_handle st_mac_hndl ,const ST_MAC_orphanRes_t * pOrphanRes);


#endif /* _802_15_4_MAC_SAP_H_ */