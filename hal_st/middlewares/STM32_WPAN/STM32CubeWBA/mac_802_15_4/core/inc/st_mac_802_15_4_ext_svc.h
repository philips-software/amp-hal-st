/**
 ******************************************************************************
 * @file    st_mac_802_15_4_ext_svc.h
 * @author  MCD Application Team
 * @brief   Contains STM32WBA Extended Service declaration. Extended service is 
 * proprietary protocol enabler layer based on Phy layer.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

#ifndef _ST_MAC_802_15_4_EXT_SYS_H_
#define _ST_MAC_802_15_4_EXT_SYS_H_
#include <stdint.h>
#include "st_mac_802_15_4_sap.h"
#include "st_mac_802_15_4_types.h"
#include "mac_temporary.h"
#include "mem_intf.h"


/** First byte by provided by user must be one of folowwing :
  0x01(data) or 0x03(command) or 0x07(extended) 
  0x21(dataAckreq) or 0x23(commandAckreq) or 0x27(extendedAckreq)
-> MANDATORY for radio (other FCF is possible but not yet tested) 

3 weak funnctions can be overridden :
  ST_MAC_ParseRxFrame
  ST_MAC_ExtRxFilter
  ST_MAC_custom_ack_cb
Even if input and output of these functions are defined in this header file, do 
not hesitate to look at implementation example in Extended apps
**/

//Memcpy to be used in ack weak function if needed
#define ACK_MEMCPY ble_memcpy

#define EXT_MIN_TX_LEN 5   // Min Frame Len, do not modify
#define EXT_MAX_TX_LEN 127 // Max Frame Len, do not modify

#define EXT_MIN_ACK_LEN   5  // Min Ack Frame Len, do not modify
#define EXT_MAX_ACK_LEN  127 // Max Ack Frame Len, do not modify

#define EXT_CRC_LEN 2 //CRC Len

//DEfault Tx config
#define DEFAULT_TX_CONFIG  {.TxMode = EXT_TX_AUTO, .TxIfs = 192, .TxRetryTime = 512, .TxRetryCount = 3}

//Default config, can be used in init function parameter
#define DEFAULT_EXT_CONFIG {.AckMode = EXT_ACK_ENABLE, .Bitrate = EXT_RATE_2M, .Channel = 11, .Power = 10, .AckCfg = NULL, .TxCfg = NULL}

//Default Acknowledge Config
#define DEFAULT_ACK_CONFIG {.Timeout = 512, .TurnaroundTime = 192}

#define TXFIFO_SIZE 5
#define RXFIFO_SIZE 5

#define EXT_MAX_CHANNEL 40

#define EXT_SCAN_ALL_CHANNEL_KPBS  0xffff000000000000 //Scan all channel (11 to 26)
#define EXT_SCAN_ALL_CHANNEL_MPBS  0xffffffffff000000 //Scan all channel (11 to 50)
/* Private typedef -----------------------------------------------------------*/

/**
 * @brief Ext Ack Frame struct (returned in Tx success callback)
 */
typedef struct {
  uint8_t Frame[EXT_MAX_ACK_LEN];
  uint8_t Len;
}ST_MAC_ExtAckFrame_s;

/**
 * @brief this structure is used by default, but user can override this struct by is own
 * thanks ST_MAC_ParseRxFrame weak function
 *
 */
typedef struct {
  uint8_t* Frame;
  uint8_t Len;
  int8_t rssi;
  uint8_t lqi;
}ST_MAC_ExtRxFrame_s;

/**
 * @brief Extended Rate, Extended Service is abled to work at diffenrent radio rate
 */
typedef enum {
  EXT_RATE_125K,
  EXT_RATE_256K,
  EXT_RATE_1M,
  EXT_RATE_2M
}ST_MAC_ExtBitRate_t;

/**
 * @brief Extended Transission Mode
 */
typedef enum  {
  EXT_TX_AUTO,
  EXT_TX_MANUAL
}ST_MAC_ExtTxMode_t;

/**
 * @brief Extended Ack mode of the DRx device 
 * EXT_ACK_ENABLE or EXT_ACK_DISABLE
 */
typedef enum {
  EXT_ACK_ENABLE,
  EXT_ACK_DISABLE
}ST_MAC_ExtAckMode_t;

/**
 * @brief Extended Ack config of the device Exammple : 
 * DEFAULT_ACK_CONFIG
 */
typedef struct {
  uint16_t TurnaroundTime;
  uint16_t Timeout;
}ST_MAC_ExtAckCfg;

/**
 * @brief Extended Svc Tx config of the device Exammple : 
 * DEFAULT_TX_CONFIG
 */
typedef struct {
  ST_MAC_ExtTxMode_t TxMode;
  uint16_t TxIfs;
  uint16_t TxRetryTime;
  uint8_t  TxRetryCount;
}ST_MAC_ExtTxCfg;


/**
 * @brief Extended Svc config of the device Exammple : 
 * DEFAULT_EXT_CONFIG
 * 
 */
typedef struct {
  ST_MAC_ExtAckMode_t AckMode;
  ST_MAC_ExtBitRate_t Bitrate;
  uint8_t  Channel;  //rate dependent
  int8_t Power;
  ST_MAC_ExtAckCfg* AckCfg; //NULL if not needed
  ST_MAC_ExtTxCfg* TxCfg;
}ST_MAC_ExtConfig;

/**
 * @brief Current state of the device
 */
typedef enum {
  EXT_NOT_INIT,
  EXT_IDLE,
  EXT_TX,
  EXT_RX,
  EXT_ED
}ST_MAC_ExtState;

/**
 * @brief status of continuous reception
 */
typedef enum {
  EXT_RX_CONT_DISABLE,
  EXT_RX_CONT_ENABLE
}ST_MAC_ExtRxCont;

/**
 * @brief Struct Callback dispatcher, user will receive this callback in case of 
 * Transmission success or failure 
 * Reception Done
 * Scan done
 */
typedef struct  {
	void (*ext_tx_success)(ST_MAC_ExtAckFrame_s* AckFrame);
	void (*ext_tx_failure)(void);
	void (*ext_rx_done)(MAC_Status_t, void*);
        void (*ext_scan_done)(uint64_t, uint8_t, uint8_t, uint8_t*);
}ST_MAC_ExtCallback_Dispatcher;

/**
 * @brief (weak) ST_MAC_ParseRxFrame defined as weak can be overridden by user to parse frame at ext service layer
 * Hardware radio automatically adds CRC, in this function CRC is returned at end of InFrame
 *
 * @param InFrame* : RAW Frame (CRC included +2 bytes at end of frame)
 *        InLen    : Total Length of the RAW Frame (CRC included = +2 bytes)
 *        rssi     : receive streng signal indicator
 *        lqi      : link quality indicator
 * @retval void* Frame Parsed, return NULL if errors occurs during parsing 
 */
void* ST_MAC_ParseRxFrame(uint8_t* InFrame, uint8_t InLen, int8_t rssi, uint8_t lqi);

/**
 * @brief (weak) ST_MAC_ExtRxFilter defined as weak can be overidden by user to filter frame at ext service layer
 * if ST_MAC_ParseRxFrame has been defined by user InFrameStruct has same type else : ST_MAC_ExtRxFrame_s*
 *
 * @param InFrameStruct* : Frame Parsed to filter
 * @retval 0 = frame not filtered else frame will be filtered
 */
uint8_t ST_MAC_ExtRxFilter(void* InFrameStruct);

/**
 * @brief (weak) ST_MAC_custom_ack_cb defined as weak can be overridden by user but need to be optimized for fast execution 
 * By default weak function is defined as IEEE Ack 15.4
 * ACK_MEMCPY must be used to copy payload (avoid hardfault because of unaligned access)
 *
 * @param Ack : Ack Frame to fullfill (CRC added automatically by the HW radio)
 *        AckLen : Size of Ack Frame (CRC length will be added automatically)
 *        Rxframe : Frame has just received (sequence number can be retrieved from Rxframe for ex)
 */
void ST_MAC_custom_ack_cb(uint8_t* Ack, uint16_t* AckLen, uint8_t* Rxframe);

/**
 * @fn ST_MAC_ExtInit
 *
 * @brief Init Extended Mac service with given configuration and link user callback
 * Configuration example : DEFAULT_EXT_CONFIG
 *
 * @param   *pStExtMacCfg : [in] configuration of extended svc 
 *          * pExtCallbackDispatcher : [in] user callback (ext_tx_success, ext_tx_failure, ext_rx_done, ext_scan_done)
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtInit(ST_MAC_ExtConfig* pStExtMacCfg, ST_MAC_ExtCallback_Dispatcher* pExtCallbackDispatcher);

/**
 * @fn ST_MAC_ExtSetPower
 *
 * @brief set Tx power in dbm
 *
 * @param   eExtPower : [in] Tx power in dbm (min -20dbm, max 10dbm)
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtSetPower(int8_t eExtPower);

/**
 * @fn ST_MAC_ExtSetRate
 *
 * @brief set radio Rate in Kbps
 *
 * @param   eExtRate : [in] radio in bps (  EXT_RATE_125K, EXT_RATE_256K, EXT_RATE_1M, EXT_RATE_2M)
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtSetRate(ST_MAC_ExtBitRate_t eExtRate);

/**
 * @fn ST_MAC_ExtSetTxIfs
 *
 * @brief set Tx interframe spacing 
 * (minimal time between 2 transmission at radio level, time between last byte of first frame and first byte of second frame)
 *
 * @param   TxIfs : [in] Tx  interframe spacing (us) min : 140 max 1000
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtSetTxIfs(uint16_t TxIfs);

/**
 * @fn ST_MAC_ExtSetChannel
 *
 * @brief Set radio frequency channel (dependent of radio rate)
 * EXT_RATE_125K, EXT_RATE_256K : min 11 max 26
 * EXT_RATE_1M, EXT_RATE_2M : min 11 max 50
 * Automatically resume RX if needed
 *
 * @param   uExtChannel : [in] channel
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtSetChannel(uint8_t uExtChannel);

/**
 * @fn ST_MAC_ExtSetAck
 *
 * @brief Set ack config : Custom Ack or Mac Ack. Only custom ack supported for the moment
 *
 * @param   eAckMode : [in] ST_MAC_ExtAckMode_t Enable or disable
 *         *pCustomAckcfg : [in] Ack config example DEFAULT_ACK_CONFIG
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtSetAck(ST_MAC_ExtAckMode_t eAckMode, ST_MAC_ExtAckCfg* pCustomAckcfg);

/**
 * @fn ST_MAC_ExtStartStopAck
 *
 * @brief Stop or Start acknowledgement at runtime
 *
 * @param   eAckMode : [in] ST_MAC_ExtAckMode_t Enable or disable
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtStartStopAck(ST_MAC_ExtAckMode_t eAckMode);

/**
 * @fn ST_MAC_ExtPushTxFIFO
 *
 * @brief Push Frame in Tx FIFO, 
 *      if EXT_TX_AUTO is set then the frame will be send as soon as possible
 *      else user needs to call ST_MAC_ExtStartTx()
 *
 * @param   *TxFrame : [in] TxFrame to send 
 *          Len      : [in] Size of the payloas
 *          eAck     : [in] ST_MAC_ExtAckMode_t Frame must be acknowledge by DRx or not
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtPushTxFIFO(uint8_t* TxFrame, uint8_t Len, ST_MAC_ExtAckMode_t eAck);

/**
 * @fn ST_MAC_ExtStartTx
 *
 * @brief Use to start transmission if EXT_TX_MANUAL is set
 *
 * @param  void
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtStartTx(void);

/**
 * @fn ST_MAC_ExtStartRx
 *
 * @brief Start reception in continuous mode or one time mode
 *
 * @param  ST_MAC_ExtRxCont Continuous_Rx : Start_Rx in continuous mode or not
 * if Continuous_Rx is set to EXT_RX_CONT_ENABLE, device will resume Rx after any event
 * Rx done callback will be called each time correct frame is received
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtStartRx(ST_MAC_ExtRxCont Continuous_Rx);

/**
 * @fn ST_MAC_ExtStopRx
 *
 * @brief Stop Reception
 *
 * @param  void
 *
 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtStopRx(void);

/**
 * @fn ST_MAC_GetState
 *
 * @brief Get state of Device
 *
 * @param  void
 *
 * @retval ST_MAC_ExtState (EXT_NOT_INIT, EXT_IDLE, EXT_TX, EXT_RX)
 */
ST_MAC_ExtState ST_MAC_GetState(void);

/**
 * @fn ST_MAC_ExtPopRxFIFO
 *
 * @brief Get out data of RxFIFO, you must give the uid of the DTx device
 *
 * @param   uid : [in] uid of the device you want to pop the data out the RxFIFO
 *
 * @retval ST_MAC_ExtFrame* Rx Frame 
 */
//ST_MAC_ExtFrame_s* ST_MAC_ExtPopRxFIFO(void);

/**
 * @fn ST_MAC_ExtStartScan
 *
 * @brief Use to start scan on different channel for DRx device, 
 * Scan callback function will be called when scan is finished
 *
 * @param  channel_mask : (uint64_t) mask must significant bit correspond to channel 11
 * example : 0x9010000000000000 -> channel 11, 14, 22: 
 *           EXT_SCAN_ALL_CHANNEL_KPBS = 0xffff000000000000 -> channel 11 to 26
 *           EXT_SCAN_ALL_CHANNEL_MPBS = 0xffffffffff000000 -> channel 11 to 50
 *          
 * @param ScanDurationChannel : Scanning time per channel (us) (increase Scanning time increase accuracy)

 * @retval MAC_Status_t
 */
MAC_Status_t ST_MAC_ExtStartScan(uint64_t channel_mask, uint32_t ScanDurationChannel);

#endif /* _ST_MAC_802_15_4_EXT_SYS_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/