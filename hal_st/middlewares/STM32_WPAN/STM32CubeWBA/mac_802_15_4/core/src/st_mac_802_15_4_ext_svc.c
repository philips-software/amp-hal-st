/**
  ******************************************************************************
  * @file    st_mac_802_15_4_ext_svc.c
  * @author  MCD-WBL Application Team
  * @brief   Mac Extended Feature Implementation
  *          This file provides latest MAC Implementation.
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
#include "st_mac_802_15_4_ext_svc.h"
#include "st_mac_802_15_4_sys_svc.h"
#include "st_mac_802_15_4_sap.h"
#include "st_mac_802_15_4_types.h"
#include "mac_temporary.h"

#include "ral.h"
#include "llhwc_cmn.h"
#include "ll_tx_pwr_if.h"


#define TX_ACK_MASK    0x20  // ACK mask mandatory in Frame_Ctrl if ACK requested

/**
 * @brief Extended Context
 * 
 */
typedef struct {
  ST_MAC_ExtConfig Config;
  ST_MAC_ExtCallback_Dispatcher Clbk_Disp;
  ST_MAC_ExtState State;
  uint8_t Retry_count;
  ST_MAC_ExtAckMode_t CurrentTxAckState;
  ST_MAC_ExtRxCont Rx_Cont;
}ST_MAC_ExtContext;


/**
 * @brief Extended Tx FIFO flag enum
 * 
 */
typedef enum {
  EXT_TX_START,
  EXT_TX_RETRY
}ST_MAC_ExtTxFlag_t;

/**
 * @brief Ext Tx Frame struct
 */
typedef struct {
  uint8_t Frame[EXT_MAX_TX_LEN];
  uint8_t Len;
}ST_MAC_ExtTxFrame_s;

/**
 * @brief Extended Tx FIFO struct
 * 
 */
typedef struct {
  ST_MAC_ExtTxFrame_s TabTxFrame[TXFIFO_SIZE];
  uint8_t maskpopulated;
  uint8_t current_out_idx;
  uint8_t current_in_idx;
}ST_MAC_ExtTxFifo_s;


//typedef struct {
//  ST_MAC_ExtFrame_s TabRxFrame[RXFIFO_SIZE];
//  uint8_t maskpopulated;
//  uint8_t current_out_idx;
//  uint8_t current_in_idx;
//}ST_MAC_ExtRxFifo_s;

/**
 * @brief Extended Scan Mask
 * 
 */
typedef struct {
  uint64_t AskScanMask;
  uint64_t CurrScanMask;
}ST_MAC_ExtScanChanMask;

/**
 * @brief Extended Scan Context  
 * 
 */
typedef struct {
  ST_MAC_ExtScanChanMask ScanMask;
  uint32_t ScanDurChan;
  uint8_t better_channel;
  uint8_t better_ed;  
  uint8_t EdchanTab[EXT_MAX_CHANNEL];
}ST_MAC_ExtScanCtxt;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define EXT_ASSERT_IS_INIT(ExtContext) if (ExtContext.State == EXT_NOT_INIT){return MAC_ERROR;}

/* Private variables ---------------------------------------------------------*/
static ral_cbk_dispatch_tbl_st  ext_cbk_dispatcher;
static ral_instance_t           ext_ralInst;
static ral_auto_ack_confg_st    auto_ack_confg;
static ral_pkt_st  *pExtRalPkt = NULL;

static ST_MAC_ExtAckCfg   Ext_Ackcfg;
static ST_MAC_ExtTxCfg    Ext_TxCfg;
static ST_MAC_ExtContext  Ext_Context = {.Config.AckCfg = &Ext_Ackcfg, .Config.TxCfg = &Ext_TxCfg};
static ST_MAC_ExtAckFrame_s  Ext_AckFrame = {0};

static ST_MAC_ExtTxFifo_s pExtFifo = {0};
static ST_MAC_ExtTxFrame_s*  pCurrentTxFrame = NULL;

//static ST_MAC_ExtRxFifo_s ExtTabRxFifo = {0};

static ST_MAC_ExtScanCtxt sScanCtx;

static ST_MAC_ExtRxFrame_s RxFrame;


/* Private function prototypes -----------------------------------------------*/
static void ext_ral_energy_scan_done( ral_instance_t ral_instance, uint32_t scan_channel, int8_t max_rssi);
static void ext_ral_tx_done(ral_instance_t ralInstance, ral_pkt_st * ptrTxPkt, ral_pkt_st * ptrAckPkt, ral_error_enum_t txError);
static void ext_ral_rx_done(ral_instance_t ral_instance, ral_pkt_st * ptr_rx_pkt, ral_error_enum_t rx_error);
static void ext_custom_ack(ral_instance_t ral_instance, uint8_t * ptr_ack_pkt, uint16_t *ack_len, uint8_t * ptr_rx_pkt);

static MAC_Status_t ST_MAC_ExtSetCustomAck(ST_MAC_ExtAckCfg* pCustomAckcfg);
static void ST_MAC_ExtInitTxFIFO(void);
static MAC_Status_t ST_MAC_ExtTx(ST_MAC_ExtTxFlag_t eTxFlag);
static ST_MAC_ExtTxFrame_s* ST_MAC_ExtPopTxFIFO(void);
//static MAC_Status_t ST_MAC_ExtPushRxFIFO(uint8_t* Frame, uint8_t Len);
static bool ext_is_channel_correct(uint8_t uChannel);
static uint8_t Get_Next_Channel_To_Scan(uint64_t ScanMask);

/* Private functions ---------------------------------------------------------*/

static void print_ext_ackframe(uint8_t* ackframe, uint8_t len)
{
  uint8_t i = 0;
  //LOG_INFO_APP("%02x | %02x | %02x | %02x | %02x | ", ackframe->frame_ctrl, ackframe->pt_id, ackframe->sequence_number, ackframe->uid_reserved, ackframe->payload_len);
  for (i = 0; i < len; i++)
  {
    //LOG_INFO_APP("%02x | ", ackframe[i]);
  }
  //LOG_INFO_APP("\r\n");
}


//static MAC_Status_t ST_MAC_ExtPushRxFIFO(uint8_t* Frame, uint8_t Len)
//{
//
//  if (Frame == NULL){return MAC_ERROR;}
//
//
//  //check if FIFO is populated at current out index
//  if (((0x01<<ExtTabRxFifo.current_in_idx)&ExtTabRxFifo.maskpopulated) != 0)
//  {
//    return MAC_ERROR;
//  }
//
//  ExtTabRxFifo.TabRxFrame[ExtTabRxFifo.current_in_idx].Len = Len;
//  MAC_MEMCPY(ExtTabRxFifo.TabRxFrame[ExtTabRxFifo.current_in_idx].Frame, Frame,  Len);
//
//  ExtTabRxFifo.maskpopulated |= 0x01<<ExtTabRxFifo.current_in_idx;
//  ExtTabRxFifo.current_in_idx++;
//
//  if (ExtTabRxFifo.current_in_idx >= RXFIFO_SIZE)
//  {
//    ExtTabRxFifo.current_in_idx = 0;
//  }
//
//  return MAC_SUCCESS;
//}




static bool ext_is_channel_correct(uint8_t uChannel)
{
  bool ret = true;
  if ((Ext_Context.Config.Bitrate == EXT_RATE_1M)||(Ext_Context.Config.Bitrate == EXT_RATE_2M))
  {
    if ( (uChannel < 11) || ((uChannel > 50) )){ ret = false;}
  }
  else
  {
    if ( (uChannel < 11) || ((uChannel > 26) )){ ret = false;}
  }
  return ret;
}


static MAC_Status_t ST_MAC_ExtSetCustomAck(ST_MAC_ExtAckCfg* pCustomAckcfg)
{
  MAC_Status_t eStatus = MAC_SUCCESS;

  if ((pCustomAckcfg == NULL) || 
      (pCustomAckcfg->Timeout > 512) ||
        (pCustomAckcfg->TurnaroundTime < 140)||(pCustomAckcfg->TurnaroundTime > 1000))
  {
    return MAC_ERROR;
  }

  Ext_Context.Config.AckMode = EXT_ACK_ENABLE;
  MAC_MEMCPY(Ext_Context.Config.AckCfg, pCustomAckcfg, sizeof(ST_MAC_ExtAckCfg));

  auto_ack_confg.ack_type = RAL_ACK_CUSTOM;
  auto_ack_confg.auto_tx_ack_state = RAL_ENABLE;
  auto_ack_confg.auto_rx_ack_state = RAL_ENABLE;

  auto_ack_confg.rspnd_fltr_confg.ack_fltr_state = RAL_DISABLE;
  auto_ack_confg.auto_tx_ack_turnaround = Ext_Context.Config.AckCfg->TurnaroundTime;
  auto_ack_confg.auto_rx_ack_turnaround = Ext_Context.Config.AckCfg->TurnaroundTime;
  auto_ack_confg.auto_rx_ack_timeout = Ext_Context.Config.AckCfg->Timeout;
  auto_ack_confg.auto_rx_enh_ack_timeout = 2176;
  auto_ack_confg.ack_req_confg.ack_req_bit_state = RAL_ENABLE;
  auto_ack_confg.ack_req_confg.byte_index = 0;
  auto_ack_confg.ack_req_confg.bit_index = 5;

  ral_confg_auto_ack(ext_ralInst, &auto_ack_confg);

  return eStatus;
}


static MAC_Status_t ST_MAC_ExtTx(ST_MAC_ExtTxFlag_t eTxFlag)
{
  ble_time_t start_time = {0,0,0};
  uint32_t nxt_strt_time;
  MAC_Status_t eStatus = MAC_SUCCESS;
  ral_error_enum_t ral_error;

  pCurrentTxFrame = ST_MAC_ExtPopTxFIFO();

  if ((pCurrentTxFrame == NULL)&&(eTxFlag == EXT_TX_START))
  {
    return MAC_ERROR;
  }

  if (eTxFlag == EXT_TX_START)
  {
    Ext_Context.Retry_count = 0;

    MAC_MEMCPY(pExtRalPkt->ptr_pyld, pCurrentTxFrame->Frame, pCurrentTxFrame->Len);
    pExtRalPkt->pyld_len                         = pCurrentTxFrame->Len + EXT_CRC_LEN; //+2 CRC, CRC Len need to be added on payload even if it is added by ral.c
    pExtRalPkt->channel                          = Ext_Context.Config.Channel;
    pExtRalPkt->tx_rx_u.tx_info.tx_power         = Ext_Context.Config.Power;

    ral_error = ral_start_tx(ext_ralInst, RAL_SOURCE_PACKET, pExtRalPkt, NULL, 0, NULL);
    if (ral_error != RAL_ERROR_NONE)
    {
      eStatus = MAC_ERROR;

    }
    else
    {
      //Fill Current Tx Ack State, will be used in Ral_Rx_done callback to know if Ack must be returned to upper layer
      Ext_Context.CurrentTxAckState = ((pCurrentTxFrame->Frame[0]&TX_ACK_MASK) == TX_ACK_MASK)? EXT_ACK_ENABLE: EXT_ACK_DISABLE;
    }
  }
  else if (eTxFlag == EXT_TX_RETRY)
  {
    Ext_Context.Retry_count++;
    if (Ext_Context.Retry_count == Ext_Context.Config.TxCfg->TxRetryCount)
    {
      Ext_Context.Retry_count = 0;
      eStatus = MAC_ERROR;
    }
    else
    {
      llhwc_slptmr_get(&nxt_strt_time);
      nxt_strt_time += Ext_Context.Config.TxCfg->TxRetryTime;
      start_time.time_stamp_base = nxt_strt_time;

      ral_error = ral_start_tx(ext_ralInst, RAL_SOURCE_PACKET, pExtRalPkt, &start_time, 0, NULL);
      if (ral_error != RAL_ERROR_NONE)
      {
        eStatus = MAC_ERROR;

      }
    }
  }

  if ((ral_error == RAL_ERROR_NONE)&&(eStatus == MAC_SUCCESS))
  {
    Ext_Context.State = EXT_TX;
  }
  else
  {
    if (Ext_Context.Rx_Cont == EXT_RX_CONT_DISABLE)
    {
       Ext_Context.State = EXT_IDLE;
    }
    else
    {
      Ext_Context.State = EXT_RX;
    }
  }

  return eStatus;
}




static void ST_MAC_ExtInitTxFIFO(void)
{
  pExtFifo.maskpopulated = 0;
  pExtFifo.current_in_idx = 0;
  pExtFifo.current_out_idx = 0;
}



static ST_MAC_ExtTxFrame_s* ST_MAC_ExtPopTxFIFO(void)
{
  ST_MAC_ExtTxFrame_s* pExtTxFrameOut = NULL;

  //check if FIFO is populated at current out index
  if (((0x01<<pExtFifo.current_out_idx)&pExtFifo.maskpopulated) != 0)
  {
    pExtTxFrameOut = &pExtFifo.TabTxFrame[pExtFifo.current_out_idx];

    pExtFifo.maskpopulated &= ~(0x01<<pExtFifo.current_out_idx);     

    pExtFifo.current_out_idx++;
    if (pExtFifo.current_out_idx >= TXFIFO_SIZE)
    {
      pExtFifo.current_out_idx = 0;
    }
  }

  return pExtTxFrameOut;
}


static uint8_t Get_Next_Channel_To_Scan(uint64_t ScanMask)
{
  uint32_t MSB_Mask = (ScanMask&0xFFFFFFFF00000000)>>32;
  uint32_t LSB_Mask = (ScanMask&0x00000000FFFFFFFF);
  uint8_t channelret = 0;

  if (MSB_Mask != 0)
  {
    channelret = __CLZ(MSB_Mask) + 11;
  }
  else
  {
    channelret = __CLZ(LSB_Mask) + 11 + 32;
  }

  if ((channelret >= 11)&&(channelret <= 50))
  {
    return channelret;
  }
  else
  {
    return 0;
  }
}

static void ext_ral_tx_done(ral_instance_t ralInstance, ral_pkt_st * ptrTxPkt, ral_pkt_st * ptrAckPkt, ral_error_enum_t txError)
{
  MAC_Status_t eStatus = MAC_SUCCESS;

  if (txError != RAL_ERROR_NONE)
  {
      eStatus = ST_MAC_ExtTx(EXT_TX_RETRY);
      if (eStatus != MAC_SUCCESS)
      {
        if (Ext_Context.Rx_Cont == EXT_RX_CONT_DISABLE)
        {
           Ext_Context.State = EXT_IDLE;
        }
        else
        {
           Ext_Context.State = EXT_RX;
        }
        if (Ext_Context.Clbk_Disp.ext_tx_failure != NULL)
        {
          Ext_Context.Clbk_Disp.ext_tx_failure();
        }
      }
  }
  else //No Tx Error
  {
    Ext_Context.Retry_count = 0;

    /* Tx with ACK, -> need to check ACK received Full RXfifo and handle ACK Header */
    if((ptrAckPkt != NULL) && (ptrAckPkt->pyld_len >= EXT_MIN_ACK_LEN) && (ptrAckPkt->pyld_len <= EXT_MAX_ACK_LEN))
    {  
      Ext_AckFrame.Len = ptrAckPkt->pyld_len;
      MAC_MEMCPY(Ext_AckFrame.Frame, ptrAckPkt->ptr_pyld, ptrAckPkt->pyld_len);
      print_ext_ackframe(Ext_AckFrame.Frame, Ext_AckFrame.Len);
    }

    /* If Tx automatic mode we try to send next FIFO Packet */
    if (Ext_Context.Config.TxCfg->TxMode == EXT_TX_AUTO)
    {
      eStatus = ST_MAC_ExtTx(EXT_TX_START);
      if (eStatus != MAC_SUCCESS) //no More packet in FIFO
      {
        if (Ext_Context.Rx_Cont == EXT_RX_CONT_DISABLE)
        {
           Ext_Context.State = EXT_IDLE;
        }
        else
        {
           Ext_Context.State = EXT_RX;
        }
      }
    }

    /* User Tx Success Callback */
    if (Ext_Context.Clbk_Disp.ext_tx_success != NULL)
    {
      if ((Ext_Context.CurrentTxAckState == EXT_ACK_ENABLE)&&(ptrAckPkt!=NULL))
      {
        Ext_Context.Clbk_Disp.ext_tx_success(&Ext_AckFrame);
      }
      else
      {
        Ext_Context.Clbk_Disp.ext_tx_success(NULL);
      }
    }
  }
}



static void ext_ral_rx_done(ral_instance_t ral_instance, ral_pkt_st * ptr_rx_pkt, ral_error_enum_t rx_error)
{
  void* FrameParsed = NULL;
  MAC_Status_t eMacStatus = MAC_SUCCESS;

  /* Ral Abort is called each time stop Rx is aborted (ral_stop_rx) */
  if (rx_error == RAL_ERROR_ABORT)
  {
    //Do not call user callback in this case
    //Ext_Context.State = EXT_IDLE;
    return;
  }

  if ((rx_error == RAL_ERROR_NONE) && (ptr_rx_pkt!=NULL) && (ptr_rx_pkt->pyld_len >= EXT_MIN_TX_LEN) && (ptr_rx_pkt->pyld_len <= EXT_MAX_TX_LEN))
  {
    FrameParsed = ST_MAC_ParseRxFrame(ptr_rx_pkt->ptr_pyld, ptr_rx_pkt->pyld_len, ptr_rx_pkt->tx_rx_u.rx_info.rssi, ptr_rx_pkt->tx_rx_u.rx_info.lqi);
    if (FrameParsed == NULL)
    {
      eMacStatus = MAC_ERROR;
      goto func_exit;
    }

    if(ST_MAC_ExtRxFilter(FrameParsed) != 0)
    {
      /* Maybe add calback for Rx filtered frame */
      eMacStatus = MAC_ERROR;
      goto func_exit;
    }
  }
  else
  {
    eMacStatus = MAC_ERROR;
  }

  func_exit:
  if (Ext_Context.Rx_Cont == EXT_RX_CONT_DISABLE)
  {
     Ext_Context.State = EXT_IDLE;
  }

  if (Ext_Context.Clbk_Disp.ext_rx_done != NULL)
  {
    Ext_Context.Clbk_Disp.ext_rx_done(eMacStatus, FrameParsed);
  }
}


static void ext_ral_energy_scan_done( ral_instance_t ral_instance, uint32_t duration, int8_t max_rssi)
{
  uint8_t nxt_channel = 0;
  uint8_t curr_channel = 0;
  uint32_t MSB_Mask = (sScanCtx.ScanMask.CurrScanMask&0xFFFFFFFF00000000)>>32;
  uint32_t LSB_Mask = (sScanCtx.ScanMask.CurrScanMask&0x00000000FFFFFFFF);
  uint8_t Max_Ed = (uint8_t)max_rssi; //Ral return already max ED not max rssi, but need to cast in unsigned
  ral_error_enum_t ral_error = RAL_ERROR_NONE;

  curr_channel = Get_Next_Channel_To_Scan(sScanCtx.ScanMask.CurrScanMask);

  //Clear current channel mask
  if (MSB_Mask != 0)
  {
    sScanCtx.ScanMask.CurrScanMask &= ~(0x8000000000000000>>(__CLZ(MSB_Mask)));
  }
  else
  {
    sScanCtx.ScanMask.CurrScanMask &= ~(0x8000000000000000>>(__CLZ(LSB_Mask) + 32));
  }

  nxt_channel = Get_Next_Channel_To_Scan(sScanCtx.ScanMask.CurrScanMask);

  //Fill energy Tab
  sScanCtx.EdchanTab[curr_channel-11] = Max_Ed;

  //If Max_ed < better energy -> better energy = Max_ed
  if (sScanCtx.better_ed >= Max_Ed) //if energy better take it
  {
    sScanCtx.better_ed = Max_Ed;
    sScanCtx.better_channel = curr_channel;
  }
  

  if (nxt_channel != 0) //if channel to scan
  {
    ral_error = ral_ed_scan(ext_ralInst, nxt_channel, sScanCtx.ScanDurChan, NULL);
    if (ral_error != RAL_ERROR_NONE) //Error detected, in this case return 0xff for this channel (no retry)
    {
      //call ext_ral_energy_scan_done with max RSSI to 0xff = Reset value or measurement not started
      ext_ral_energy_scan_done(ext_ralInst, sScanCtx.ScanDurChan, 0xff);
    }
  }
  else //all channel scanned, call user callback and resume Rx if needed
  {
    //note : Ral layer doesn't restart automatically Rx after ED scan (no ral_isr)
    if (Ext_Context.Rx_Cont == EXT_RX_CONT_DISABLE)
    {
       Ext_Context.State = EXT_IDLE;
    }
    else
    {
      ST_MAC_ExtStartRx(EXT_RX_CONT_ENABLE); 
    }
    Ext_Context.Clbk_Disp.ext_scan_done(sScanCtx.ScanMask.AskScanMask, sScanCtx.better_channel, sScanCtx.better_ed, sScanCtx.EdchanTab);
  }
}

/* Ral custom ack function */
static void ext_custom_ack(ral_instance_t ral_instance, uint8_t * ptr_ack_pkt, uint16_t *ack_len, uint8_t * ptr_rx_pkt)
{
  (void)ral_instance;
  ST_MAC_custom_ack_cb(ptr_ack_pkt, ack_len, ptr_rx_pkt);
  *ack_len += EXT_CRC_LEN; //Add CRC lenght
}

//Classic 15.4 Ack, this weak function can be ovveride but must be optimized (fast execution mandatory)
__WEAK void ST_MAC_custom_ack_cb(uint8_t* Ack, uint16_t* AckLen, uint8_t* Rxframe)
{
    uint8_t ack_frame_arr[3] = {0x02, 0x00 , 0x00};

    /* Set the sequence number of the acknowledge frame */
    ack_frame_arr[2] = Rxframe[2];
    /* Copy Ack to shared memory */
    ACK_MEMCPY(Ack, ack_frame_arr, 3);
    *AckLen = 3;
}

__WEAK void* ST_MAC_ParseRxFrame(uint8_t* InFrame, uint8_t InLen, int8_t rssi, uint8_t lqi)
{
  if (InFrame == NULL)
  {
    return NULL;
  }

  RxFrame.Frame = InFrame;
  RxFrame.Len   = InLen;
  RxFrame.rssi  =  rssi;
  RxFrame.lqi   =  lqi;

  return &RxFrame;
}

__WEAK uint8_t ST_MAC_ExtRxFilter(void* InFrameStruct)
{
  (void)InFrameStruct;
  return 0; //by default return 0 (frame no filter)
}
/* External variables --------------------------------------------------------*/

/* External functions --------------------------------------------------------*/
MAC_Status_t ST_MAC_ExtInit(ST_MAC_ExtConfig* pStExtMacCfg, ST_MAC_ExtCallback_Dispatcher* pExtCallbackDispatcher)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  ST_MAC_ExtTxCfg pExtTxDfltCfg = DEFAULT_TX_CONFIG;
  /* Sanity Check */
  if (pStExtMacCfg == NULL)
  {
    return MAC_ERROR;
  }

  /* Init Extended Mac context */
  Ext_Context.State = EXT_IDLE;
  Ext_Context.Rx_Cont = EXT_RX_CONT_DISABLE;
  Ext_Context.Config.Bitrate = pStExtMacCfg->Bitrate;
  Ext_Context.Config.Channel = pStExtMacCfg->Channel;
  Ext_Context.Config.AckMode = pStExtMacCfg->AckMode;

  if (pStExtMacCfg->AckCfg != NULL)
  {
    MAC_MEMCPY(Ext_Context.Config.AckCfg, pStExtMacCfg->AckCfg, sizeof(ST_MAC_ExtAckCfg));
  }

  if (pStExtMacCfg->TxCfg != NULL)
  {
    MAC_MEMCPY(Ext_Context.Config.TxCfg, pStExtMacCfg->TxCfg, sizeof(ST_MAC_ExtTxCfg));
  }
  else
  {
    MAC_MEMCPY(Ext_Context.Config.TxCfg, &pExtTxDfltCfg, sizeof(ST_MAC_ExtTxCfg));  
  }


  /* Map Callback */
  /* Ral intern Callback */
  ext_cbk_dispatcher.ral_ed_scan_done = ext_ral_energy_scan_done;
  ext_cbk_dispatcher.ral_tx_done = ext_ral_tx_done;
  ext_cbk_dispatcher.ral_rx_done = ext_ral_rx_done;
  ext_cbk_dispatcher.ral_configure_custom_ack = ext_custom_ack;
  /* User Callback */
  if (pExtCallbackDispatcher != NULL)
  {
    Ext_Context.Clbk_Disp.ext_tx_success = pExtCallbackDispatcher->ext_tx_success;
    Ext_Context.Clbk_Disp.ext_tx_failure = pExtCallbackDispatcher->ext_tx_failure;
    Ext_Context.Clbk_Disp.ext_rx_done = pExtCallbackDispatcher->ext_rx_done;
    Ext_Context.Clbk_Disp.ext_scan_done = pExtCallbackDispatcher->ext_scan_done;
  }
  else
  {
    Ext_Context.Clbk_Disp.ext_tx_success = NULL;
    Ext_Context.Clbk_Disp.ext_tx_failure = NULL;
    Ext_Context.Clbk_Disp.ext_rx_done = NULL;
    Ext_Context.Clbk_Disp.ext_scan_done = NULL;
  }

  /* Linklayer init */
  ext_ralInst = ral_init(&ext_cbk_dispatcher);

  /* Enable Auto Sleep state */
  ral_set_auto_sleep_state(ext_ralInst, RAL_ENABLE);

  pExtRalPkt = ral_get_tx_buf(ext_ralInst);

  /* Channel */
  eStatus |= ST_MAC_ExtSetPower(Ext_Context.Config.Power);

  /* Rate */
  eStatus |= ST_MAC_ExtSetRate(Ext_Context.Config.Bitrate);

  /* Ifs */
  ST_MAC_ExtInitTxFIFO();
  eStatus |= ST_MAC_ExtSetTxIfs(Ext_Context.Config.TxCfg->TxIfs);

  /* Channel */
  eStatus |= ST_MAC_ExtSetChannel(Ext_Context.Config.Channel);
  /* Ack config */
  eStatus |= ST_MAC_ExtSetAck(pStExtMacCfg->AckMode, pStExtMacCfg->AckCfg);

  ral_set_cca_ed_threshold(-75);

  Ext_Context.State = EXT_IDLE;

  return eStatus;
}



MAC_Status_t ST_MAC_ExtSetPower(int8_t eExtPower)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  int8_t llSelectedTxPwr 	= 0;
  uint8_t txPwrTableOffset = 0;

  /* This function checks the validity of the parameter and sets the context accordingly */

  /* Set the tx power, given power is withing the bounds (ll_tx_pwr_if_get_min_max_supprtd_tx_power_levels(&min_tx_pwr, &max_tx_pwr) called by the command interpreter.
   * Find the nearest available power in the table.
   */
  txPwrTableOffset =  ll_tx_pwr_if_get_tx_pwr_table_offset( eExtPower, FALSE, MAC_CNTRLR);
  llSelectedTxPwr  = g_power_info.ptr_tx_power_table[txPwrTableOffset].tx_pwr;
  Ext_Context.Config.Power   = llSelectedTxPwr;

  return eStatus;
}


MAC_Status_t ST_MAC_ExtSetRate(ST_MAC_ExtBitRate_t eExtRate)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  ral_error_enum_t eRalError = RAL_ERROR_NONE;

  eRalError = ral_set_rate(ext_ralInst, (ral_phy_rate_enum_t)eExtRate);
  if (eRalError != RAL_ERROR_NONE)
  {
    eStatus = MAC_ERROR;
  }
  else
  {
    Ext_Context.Config.Bitrate = eExtRate;
  }

  return eStatus;
}


MAC_Status_t ST_MAC_ExtSetTxIfs(uint16_t TxIfs)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  ral_error_enum_t eRalError = RAL_ERROR_NONE;

  eRalError = ral_set_ifs(ext_ralInst, TxIfs);
  if (eRalError != RAL_ERROR_NONE)
  {
    eStatus = MAC_ERROR;
  }
  else
  {
    Ext_Context.Config.TxCfg->TxIfs = TxIfs;
  }

  return eStatus;
}



MAC_Status_t ST_MAC_ExtSetChannel(uint8_t uExtChannel)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  ral_error_enum_t eRalError = RAL_ERROR_FAILED;
  ral_event_state_enum_t curr_state;
  ral_instance_t curr_ral_inst;
  uint8_t curr_channel;

  if (ext_is_channel_correct(uExtChannel) == false)
  {
    return MAC_ERROR;
  }

  //Change Extended context
  Ext_Context.Config.Channel = uExtChannel;

  //If radio is currently in Rx we need to abort and relaunch it with new channel
  if (Ext_Context.State == EXT_RX)
  {
    //check ral state to know if Rx abort is needed
    curr_state = ral_get_current_event_state(&curr_ral_inst, &curr_channel);
    if ((curr_state != RAL_IDLE)&&(curr_channel != Ext_Context.Config.Channel))
    {
      eRalError = ral_abort_rx(ext_ralInst);
      if (eRalError == RAL_ERROR_NONE)
      {
        Ext_Context.State = EXT_IDLE;
        //Start Rx on new channel
        eStatus = ST_MAC_ExtStartRx(Ext_Context.Rx_Cont);
      }
      else
      {
        Ext_Context.State = EXT_IDLE;
        Ext_Context.Config.Channel = curr_channel;
        eStatus = MAC_ERROR;          
      }
    }
  }


  return eStatus;
}


MAC_Status_t ST_MAC_ExtSetAck(ST_MAC_ExtAckMode_t eAckMode, ST_MAC_ExtAckCfg* pCustomAckcfg)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  ST_MAC_ExtAckCfg pDefaultCustomAckCfg = DEFAULT_ACK_CONFIG;

  if(eAckMode == EXT_ACK_ENABLE)
  {
      if (pCustomAckcfg != NULL)
      {
        eStatus = ST_MAC_ExtSetCustomAck(pCustomAckcfg);
      }
      else
      {
        eStatus = ST_MAC_ExtSetCustomAck(&pDefaultCustomAckCfg);
      }
      
      Ext_Context.Config.AckMode = EXT_ACK_ENABLE;
  }
  else
  {
    if (Ext_Context.State == EXT_NOT_INIT)
    {
      eStatus = ST_MAC_ExtSetCustomAck(&pDefaultCustomAckCfg);
    }
    auto_ack_confg.auto_tx_ack_state = RAL_DISABLE;
    auto_ack_confg.auto_rx_ack_state = RAL_DISABLE;
    ral_confg_auto_ack(ext_ralInst, &auto_ack_confg);

    Ext_Context.Config.AckMode = EXT_ACK_DISABLE;
  }
  return eStatus;
}

MAC_Status_t ST_MAC_ExtStartStopAck(ST_MAC_ExtAckMode_t eAckMode)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  ral_state_enum_t ack_status = RAL_DISABLE;

  EXT_ASSERT_IS_INIT(Ext_Context);

  if (eAckMode == EXT_ACK_ENABLE)
  {
    Ext_Context.Config.AckMode = EXT_ACK_ENABLE;
    ack_status = RAL_ENABLE;
  }
  else
  {
    Ext_Context.Config.AckMode = EXT_ACK_DISABLE;
  }

  ral_pause_auto_ack(ext_ralInst, ack_status, ack_status);

  return eStatus;
}

MAC_Status_t ST_MAC_ExtPushTxFIFO(uint8_t* Payload, uint8_t Len, ST_MAC_ExtAckMode_t eAck)
{
  if (Payload == NULL){return MAC_ERROR;}

  //check if FIFO is populated at current out index
  if (((0x01<<pExtFifo.current_in_idx)&pExtFifo.maskpopulated) != 0)
  {
    return MAC_ERROR;
  }

  MAC_MEMCPY(pExtFifo.TabTxFrame[pExtFifo.current_in_idx].Frame, Payload, Len);
  pExtFifo.TabTxFrame[pExtFifo.current_in_idx].Len = Len;

  //check ack bit
  if (eAck == EXT_ACK_ENABLE)
  {
    pExtFifo.TabTxFrame[pExtFifo.current_in_idx].Frame[0] |= TX_ACK_MASK;
  }
  else
  {
    pExtFifo.TabTxFrame[pExtFifo.current_in_idx].Frame[0] &= ~TX_ACK_MASK;
  }

  pExtFifo.maskpopulated |= 0x01<<pExtFifo.current_in_idx;
  pExtFifo.current_in_idx++;

  if (pExtFifo.current_in_idx >= TXFIFO_SIZE)
  {
    pExtFifo.current_in_idx = 0;
  }

  //if Tx Mode is set to automatic and we are not currently on TX 6> Start Tx
  if ((Ext_Context.Config.TxCfg->TxMode == EXT_TX_AUTO)&&(Ext_Context.State != EXT_TX))
  {
    ST_MAC_ExtTx(EXT_TX_START);
  }

  return MAC_SUCCESS;
}

MAC_Status_t ST_MAC_ExtStartTx(void)
{
  MAC_Status_t eStatus = MAC_SUCCESS;

  if ((Ext_Context.State != EXT_IDLE)||(Ext_Context.Config.TxCfg->TxMode == EXT_TX_AUTO))
  {
    return MAC_ERROR;
  }

  eStatus = ST_MAC_ExtTx(EXT_TX_START);

  return eStatus;
}


MAC_Status_t ST_MAC_ExtStartRx(ST_MAC_ExtRxCont Continuous_Rx)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  ral_error_enum_t eRalError = RAL_ERROR_FAILED;
  ral_mac_fltr_confg_st Ext_macFltrConfg;

  //If radio is currently in Rx we need to abort and relaunch it with new channel
  if (Ext_Context.State == EXT_RX)
  {
    eRalError = ral_abort_rx(ext_ralInst);
    if (eRalError != RAL_ERROR_NONE)
    {
      eStatus = MAC_ERROR;
    }
  }

  /* Force the continuous reception mode */
  Ext_macFltrConfg.mac_fltr_state = RAL_DISABLE; // disable all filters (like PANID, ...)
  ral_confg_mac_fltr(ext_ralInst, &Ext_macFltrConfg);
  if (Continuous_Rx == EXT_RX_CONT_ENABLE)
  {
    ral_set_cont_recp_state( ext_ralInst, RAL_ENABLE);
  }
  else
  {
    ral_set_cont_recp_state( ext_ralInst, RAL_DISABLE);
  }

  /* Start the RX */
  eRalError = ral_start_rx(ext_ralInst, Ext_Context.Config.Channel, NULL, 0 /* timeout */,  0 /* periodicInterval */, NULL);  
  if (eRalError != RAL_ERROR_NONE)
  {
    eStatus = MAC_ERROR;
  }
  else
  {
    Ext_Context.State = EXT_RX;
    Ext_Context.Rx_Cont = Continuous_Rx;
  }

  return eStatus;
}


MAC_Status_t ST_MAC_ExtStopRx(void)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  ral_error_enum_t eRalError = RAL_ERROR_FAILED;


  /* Force the continuous reception mode to disable */
  ral_set_cont_recp_state(ext_ralInst, RAL_DISABLE);

  if (Ext_Context.State == EXT_RX)
  {
    eRalError = ral_abort_rx(ext_ralInst);
    if (eRalError != RAL_ERROR_NONE)
    {
      eStatus = MAC_ERROR;
    }
  }

  if (eStatus == MAC_SUCCESS)
  {
    Ext_Context.State = EXT_IDLE;
  }

  return eStatus;
}


ST_MAC_ExtState ST_MAC_GetState(void)
{
  return Ext_Context.State;
}

//ST_MAC_ExtFrame_s* ST_MAC_ExtPopRxFIFO(void)
//{
//  ST_MAC_ExtFrame_s* pExtTxFrameOut = NULL;
//  
//  //check if FIFO is populated at current out index
//  if (((0x01<<ExtTabRxFifo.current_out_idx)&ExtTabRxFifo.maskpopulated) != 0)
//  {
//    pExtTxFrameOut = &ExtTabRxFifo.TabRxFrame[ExtTabRxFifo.current_out_idx];
//    
//    ExtTabRxFifo.maskpopulated &= ~(0x01<<ExtTabRxFifo.current_out_idx);     
//    
//    ExtTabRxFifo.current_out_idx++;
//    if (ExtTabRxFifo.current_out_idx >= TXFIFO_SIZE)
//    {
//      ExtTabRxFifo.current_out_idx = 0;
//    }
//  }
//  
//  return pExtTxFrameOut;
//}


MAC_Status_t ST_MAC_ExtStartScan(uint64_t channel_mask, uint32_t ScanDurationChannel)
{
  MAC_Status_t eStatus = MAC_SUCCESS;
  ral_error_enum_t ral_error = RAL_ERROR_NONE;
  uint8_t ScanChannel = 0;

  //Reset Scan Context paramters
  sScanCtx.ScanDurChan = ScanDurationChannel;
  sScanCtx.ScanMask.AskScanMask = channel_mask;
  sScanCtx.ScanMask.CurrScanMask = channel_mask;

  sScanCtx.better_channel = 0;
  sScanCtx.better_ed = 0xff; //init at max energy

  ScanChannel = Get_Next_Channel_To_Scan(sScanCtx.ScanMask.AskScanMask);

  ral_error =  ral_ed_scan(ext_ralInst, ScanChannel,
		ScanDurationChannel, NULL);
  if (ral_error != RAL_ERROR_NONE)
  {
    eStatus = MAC_ERROR;
  }
  else
  {
    Ext_Context.State = EXT_ED;
  }

  return eStatus; 
}

