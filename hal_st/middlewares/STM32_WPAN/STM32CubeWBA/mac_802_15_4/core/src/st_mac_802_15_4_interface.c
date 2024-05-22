/**
  ******************************************************************************
  * @file    st_mac_802_15_4_interface.c
  * @author  MCD-WBL Application Team
  * @brief   Mac Interface Feature Implementation
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

#include "st_mac_buffer_management.h"


#include "st_mac_802_15_4_sap.h"
#include "st_mac_802_15_4_types.h"


    

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
      
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

static uint8_t ST_MAC_associateReq_validate( const  ST_MAC_associateReq_t * pAssociateReq) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_disassociateReq_validate( const  ST_MAC_disassociateReq_t * pDisassociateReq) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_getReq_validate(const ST_MAC_getReq_t * pGetReq) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_resetReq_validate(const ST_MAC_resetReq_t * pResetReq ) {
  return MAC_SUCCESS;
}


static uint8_t ST_MAC_rxEnableReq_validate(const ST_MAC_rxEnableReq_t * pRxEnableReq) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_scanReq_validate(const ST_MAC_scanReq_t * pScanReq) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_setReq_validate(const ST_MAC_setReq_t * pSetReq) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_startReq_validate(const  ST_MAC_startReq_t * pStartReq) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_pollReq_validate(const ST_MAC_pollReq_t * pPollReq) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_associateRes_validate(const ST_MAC_associateRes_t * pAssociateRes) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_orphanRes_validate(const ST_MAC_orphanRes_t * pOrphanRes) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_dataReq_validate (const ST_MAC_dataReq_t * pDataReq ) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_purgeReq_validate (const ST_MAC_purgeReq_t * pPurgeReq ) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_BeaconReq_validate (const ST_MAC_beaconReq_t * pBeaconReq ) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_getPwrInfoTableReq_validate (const ST_MAC_getPwrInfoTableReq_t * pGetPwrInfoTableReq ) {
  return MAC_SUCCESS;
}

static uint8_t ST_MAC_setPwrInfoTableReq_validate (const ST_MAC_setPwrInfoTableReq_t * pSetPwrInfoTableReq ) {
  return MAC_SUCCESS;
}

/* External functions --------------------------------------------------------*/



/** MCPS Section */

/* Request */

MAC_Status_t ST_MAC_MLMEAssociateReq( MAC_handle st_mac_hndl , const  ST_MAC_associateReq_t * pAssociateReq ) {

  uint8_t mac_command_id  = ST_MAC_ASSOCIATION_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_associateReq_validate(pAssociateReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_associateReq_t), (void *) pAssociateReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMEDisassociateReq( MAC_handle  st_mac_hndl , const  ST_MAC_disassociateReq_t * pDisassiociateReq ){

  uint8_t mac_command_id  = ST_MAC_DISASSOCIATION_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_disassociateReq_validate(pDisassiociateReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_disassociateReq_t), (void *) pDisassiociateReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMEGetReq( MAC_handle  st_mac_hndl ,const  ST_MAC_getReq_t * pGetReq ){

  uint8_t mac_command_id  = ST_MAC_GET_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_getReq_validate(pGetReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_getReq_t), (void *) pGetReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMEResetReq( MAC_handle  st_mac_hndl ,const  ST_MAC_resetReq_t * pResetReq ){
  
  uint8_t mac_command_id  = ST_MAC_RESET_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_resetReq_validate(pResetReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_resetReq_t), (void *) pResetReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMERxEnableReq( MAC_handle  st_mac_hndl ,const  ST_MAC_rxEnableReq_t * pRxEnableReq ){
  
  uint8_t mac_command_id  = ST_MAC_RX_ENABLE_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_rxEnableReq_validate(pRxEnableReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_rxEnableReq_t), (void *) pRxEnableReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMEScanReq( MAC_handle  st_mac_hndl ,const  ST_MAC_scanReq_t * pScanReq ){

  uint8_t mac_command_id  = ST_MAC_SCAN_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_scanReq_validate(pScanReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_scanReq_t), (void *) pScanReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMESetReq( MAC_handle  st_mac_hndl ,const  ST_MAC_setReq_t * pSetReq){

  uint8_t mac_command_id  = ST_MAC_SET_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_setReq_validate(pSetReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_setReq_t), (void *) pSetReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMEStartReq( MAC_handle  st_mac_hndl ,const  ST_MAC_startReq_t * pStartReq) {

  uint8_t mac_command_id  = ST_MAC_START_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_startReq_validate(pStartReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_startReq_t), (void *) pStartReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMEPollReq( MAC_handle  st_mac_hndl ,const ST_MAC_pollReq_t * pPollReq ){

  uint8_t mac_command_id  = ST_MAC_POLL_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_pollReq_validate(pPollReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_pollReq_t), (void *) pPollReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMEBeaconReq( MAC_handle  st_mac_hndl ,const ST_MAC_beaconReq_t * pBeaconReq ){

  uint8_t mac_command_id  = ST_MAC_BEACON_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_BeaconReq_validate(pBeaconReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_beaconReq_t), (void *) pBeaconReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMEGetPwrInfoTableReq( MAC_handle  st_mac_hndl, const ST_MAC_getPwrInfoTableReq_t * pGetPwrInfoTableReq ) {
  
  uint8_t mac_command_id = ST_MAC_GET_PWR_INFO_TABLE_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_getPwrInfoTableReq_validate(pGetPwrInfoTableReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_getPwrInfoTableReq_t), (void *) pGetPwrInfoTableReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMESetPwrInfoTableReq( MAC_handle  st_mac_hndl, const ST_MAC_setPwrInfoTableReq_t * pSetPwrInfoTableReq ) {
  
  uint8_t mac_command_id = ST_MAC_SET_PWR_INFO_TABLE_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_setPwrInfoTableReq_validate(pSetPwrInfoTableReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_setPwrInfoTableReq_t), (void *) pSetPwrInfoTableReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

/* Response */
MAC_Status_t ST_MAC_MLMEAssociateRes(MAC_handle st_mac_hndl ,const ST_MAC_associateRes_t * pAssociateRes){

  uint8_t mac_command_id  = ST_MAC_ASSOCIATE_RES_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_associateRes_validate(pAssociateRes) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_associateRes_t), (void *) pAssociateRes) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MLMEOrphanRes(MAC_handle st_mac_hndl ,const ST_MAC_orphanRes_t * pOrphanRes){

  uint8_t mac_command_id  = ST_MAC_ORPHAN_RES_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_orphanRes_validate(pOrphanRes) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_orphanRes_t), (void *) pOrphanRes) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}


/** MCPS Section */

MAC_Status_t ST_MAC_MCPSDataReq( MAC_handle st_mac_hndl ,const ST_MAC_dataReq_t * pDataReq ){

  uint8_t mac_command_id  = ST_MAC_DATA_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_dataReq_validate(pDataReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_dataReq_t), (void *) pDataReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_MCPSPurgeReq( MAC_handle st_mac_hndl ,const ST_MAC_purgeReq_t * pPurgeReq ){
  
  uint8_t mac_command_id  = ST_MAC_PURGE_REQ_INT_CMD_ID;
  // Check MAC State - Return Error is current Req can't be handled
  if (is_MAC_ready(mac_command_id) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Validate current req - Return Error is fields is not supported
  if ( ST_MAC_purgeReq_validate(pPurgeReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  // Enqueue Current Req - Return Error if queue reaches its capacity
  if ( ST_MAC_enqueue_req(mac_command_id, st_mac_hndl, sizeof(ST_MAC_purgeReq_t), (void *) pPurgeReq) != MAC_SUCCESS ) {
    return MAC_ERROR;
  }
  return MAC_SUCCESS;
}


/** Not Implemented Section */

MAC_Status_t ST_MAC_MLMEGtsReq( MAC_handle  st_mac_hndl ,const  ST_MAC_gtsReq_t * pGtsReq ){
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
  return status;
}

MAC_Status_t ST_MAC_MLMEDpsReq( MAC_handle  st_mac_hndl ,const ST_MAC_dpsReq_t * pDpsReq){
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
  return status;  
}

MAC_Status_t ST_MAC_MLMESoundingReq( MAC_handle  st_mac_hndl ,const ST_MAC_soundingReq_t * pSoundingReq){
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
  return status;  
}

MAC_Status_t ST_MAC_MLMECalibrateReq( MAC_handle st_mac_hndl ,const ST_MAC_calibrateReq_t * pCalibrateReq){
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;
  return status;  
}
