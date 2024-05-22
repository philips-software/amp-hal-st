/**
  ******************************************************************************
  * @file    st_mac_802_15_4_core.h
  * @author  MCD Application Team
  * @brief   This file contains all the defines and structures used for the
  *          communication between the two core M0 and M4.
  *          This file is shared between the code running on M4 and the code
  *          running on M0.
  *
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



#ifndef _ST_MAC_802_15_4_CORE_H_
#define _ST_MAC_802_15_4_CORE_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

#include "stm32wbaxx_hal.h"
#include "st_mac_802_15_4_sys.h"
#include "st_mac_802_15_4_types.h"

/* SNPS */
#include "mac_host_intf.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Exported defines ----------------------------------------------------------*/

#define MAC_CMD_MSG_ID_OFFSET         0x00
#define MAC_CMD_MSG_HANDLE_ID_OFFSET  0x01
#define MAC_CMD_MSG_STRUCT_LEN_OFFSET 0x02
#define MAC_CMD_MSG_STUFF_OFFSET      0x03
#define MAC_CMD_MSG_HEADER_OFFSET     0x04



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */



// Struct to store dynamic MAC Fields
// State Machine, Modes – except PID attributes ) – store Svc Status ?
typedef struct ST_MAC_Core_Status_tag {
  uint8_t defTempo;
  } ST_MAC_Core_Status_t;

// Struct to store PIB Attributes as defined in MAC – IEEE 802.15.4 standard
// Attributes required to manage the MAC sublayer of a device.
// R/W Status should be Implemented
// Some are optional depending on node config ( FFD / RFD / … )

typedef struct ST_MAC_PIB_tag {
  uint8_t defTempo;
  } ST_MAC_PIB_t;

enum ST_MAC_FSM_STATE {
  MAC_RESET_STATE = 0x00,
  MAC_INIT_STATE = 0x01,
  MAC_RUNNING_STATE = 0x02,
  MAC_UNKNOWN_STATE
};



/* MAC command cmdcode range 0x280 .. 0x3DF = 352 */
#define MAC_802_15_4_CMD_OPCODE_OFFSET 0x280

#define MASK_CMD_CODE_OCF 0x3FF

/* Structure of the messages exchanged between M0 and M4 */
#define OT_CMD_BUFFER_SIZE 20U
typedef struct
{
  uint32_t  ID;
  uint32_t  Size;
  uint32_t  Data[OT_CMD_BUFFER_SIZE];
}MAC_802_15_4_CmdReq_OT_LIKE_t;

// typedef PACKED_STRUCT
// {
//   uint16_t  subEvtCode;
//   uint8_t   notPayload[1];
// }MAC_802_15_4_Notification_t;

/** @defgroup STM32WB55_MAC_CORE STM32WBxx Proxy MAC 802.15.4 Core
  * @brief           Proxy MAC module allows application to exchange in both ways MAC messages
  *                  between Application and RF Cores.
  *                  On application or network upper layer SAP-MAC primitive call, MAC request is
  *                  interpreted by the Proxy MAC layer and sent to the RF-Core.<br>
  *                  On RF-Core MAC notification, MAC messages are parsed and sent back to the upper
  *                  user implemented layer using user defined callback.
  *
  * @{
  */

/* External functions --------------------------------------------------------*/
MAC_Status_t ST_MAC_enqueue_radio_Incoming(uint8_t command_id, MAC_handle st_mac_hndl, uint8_t req_len, void * reqPtr);
MAC_Status_t ST_MAC_enqueue_radio_Incoming_with_payload(uint8_t command_id, MAC_handle st_mac_hndl, uint8_t req_len, void * reqPtr, uint8_t * payload, uint8_t payload_len);

uint8_t ST_MAC_enqueue_req(uint8_t command_id, MAC_handle st_mac_hndl, uint8_t req_len, void * reqPtr);
void mac_serialize_pan_desc_to_ST(  ST_MAC_PAN_Desc_t * pan_desc,  pan_descr_st * pan_desc_snps);
uint8_t is_MAC_ready(uint8_t mac_command_id);
uint8_t mac_command_validate(uint8_t command_id, uint8_t command_len);
uint8_t mac_get_pib_attribute_len(void* mac_cntx_ptr,uint8_t pib_attr_id);
MAC_handle ST_MAC_get_mac_interface(void * mac_cntx_ptr);

void mac_baremetal_run(void);

/**
  * @}
  */

/* List of modes available for UART configuration */
typedef enum
{
  SYS_LPUART1_CLI,
  SYS_USART1_CLI,
} Sys_ConfigUart_Enum_t;

/* Gravity error level */
typedef enum
{
  ERR_INTERFACE_FATAL= 1U,
  ERR_INTERFACE_WARNING = 2U
} Error_Interface_Level_Enum_t;


/** @addtogroup STM32WB55_MAC_CORE
 *
 */

/**
  * @brief  This function is used to manage all the callbacks used by the
  *         MAC 802.15.4 interface.
  *
  *         Important Note: This function must be called each time a message
  *         is sent from the M0 to the M4.
  *
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MAC_802_15_4_CallBack_Processing(void);

/**
  * @}
  */


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _ST_MAC_802_15_4_CORE_H_ */
