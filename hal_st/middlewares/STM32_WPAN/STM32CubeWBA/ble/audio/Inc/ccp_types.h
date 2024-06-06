/**
  ******************************************************************************
  * @file    ccp_types.h
  * @author  MCD Application Team
  * @brief   This file contains type definitions used for Call Control Profile
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CCP_TYPES_H
#define __CCP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_types.h"

/* Defines -------------------------------------------------------------------*/

/* #############################################################################
   #       Defines and MACRO used to allocate GATT Services, GATT Attributes   #
   #       and storage area for Attribute values for Call Control Profile      #
   #       in Server role                                                      #
   ############################################################################# */

/*
 * CCP_SRV_NUM_GATT_SERVICES: number of GATT services required for Call Control Profile
 * in Server role
 *
 * @param num_bearers: Maximum number of supported Telephony Bearers
 */
#define CCP_SRV_NUM_GATT_SERVICES(num_bearers)          (num_bearers)

/*
 * CCP_SRV_NUM_GATT_ATTRIBUTES: number of GATT attributes required to support Telephony Bearers registration.
 * Additional GATT attributes could be required according to the CCP Option Features specified in the CCP_SRV_Config_t
 * settings submitted in the CCP_Init() function.
 *
 * @param num_bearers: Maximum number of supported Telephony Bearers
 *
 * @param ccp_feature: Supported CCP Option Feature (CCP_OptionFeatures_t type).
 */
#define CCP_SRV_NUM_GATT_ATTRIBUTES(num_bearers,ccp_feature) \
                                    (num_bearers * \
                                    (33u + (((ccp_feature & CCP_FEATURE_BEARER_SIGNAL_STRENGTH) != (0u) ) ? (5) : (0)) \
                                    + (((ccp_feature & CCP_FEATURE_INC_CALL_TARGET_BEARER_URI) != (0u)) ? (3) : (0)) \
                                    + (((ccp_feature & CCP_FEATURE_CALL_FRIENDLY_NAME) != (0u)) ? (3) : (0))))

/*
 * CCP_SRV_ATT_VALUE_ARRAY_SIZE: this macro returns the size of the storage area for Attribute values
 * in CCP Server.
 *
 * @param num_bearers: Maximum number of supported Telephony Bearers
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *
 * @param ccp_feature: Supported CCP Option Feature (CCP_OptionFeatures_t type).
 *
 * @param max_bearer_provider_name_length: Maximum size of the Bearer Provider Name.
 *
 * @param max_bearer_uci_length: Maximum supported size of the Bearer UCI
 *
 * @param max_uri_schemes_list_length: Maximum supported size of the URI Schemes List
 *
 * @param max_num_calls: Maximum number of supported calls.
 *
 * @param max_uri_length: Maximum size of the Call URI composed of URI scheme followed by the Caller ID.
 *
 * @param max_call_friendly_name_length: Maximum size of the Call Friendly Name.
 */
#define CCP_SRV_ATT_VALUE_ARRAY_SIZE(num_bearers,num_ble_links,ccp_feature,max_bearer_provider_name_length, \
                                     max_bearer_uci_length,max_uri_schemes_list_length,max_num_calls,max_uri_length, \
                                     max_call_friendly_name_length) \
                          (num_bearers * \
                          (68u + (18u*num_ble_links) \
                          + max_bearer_provider_name_length \
                          + max_bearer_uci_length \
                          + max_uri_schemes_list_length \
                          + (max_num_calls * (7u + max_uri_length)) \
                          + MAX((1+max_uri_length),(1+max_num_calls)) \
                          + max_uri_length \
                          + (((ccp_feature & CCP_FEATURE_BEARER_SIGNAL_STRENGTH) != (0x00)) ? (12u+2*num_ble_links) : (0)) \
                          + (((ccp_feature & CCP_FEATURE_INC_CALL_TARGET_BEARER_URI) != (0x00)) ? (5+2*num_ble_links+(1+max_uri_length)) : (0)) \
                          + (((ccp_feature & CCP_FEATURE_CALL_FRIENDLY_NAME) != (0x00)) ? (5+2*num_ble_links+(max_call_friendly_name_length)) : (0))))


/* #############################################################################
   #       Defines and MACRO used to allocate memory resource of               #
   #       Call Control Profile in CCP_Config_t                                  #
   #      (BLE_CCP_CLT_TOTAL_BUFFER_SIZE , BLE_CCP_SRV_TOTAL_BUFFER_SIZE)      #
   ############################################################################# */

/*
 * CCP_CLT_MEM_PER_CONN_SIZE_BYTES: memory size used per link in CCP Client role
 */
#define CCP_CLT_MEM_PER_CONN_SIZE_BYTES                 (36u)

/*
 * CCP_CLT_MEM_PER_BEARER_SIZE_BYTES: memory size used per Telephony Bearer in CCP Client role
 */
#define CCP_CLT_MEM_PER_BEARER_SIZE_BYTES               (164u)

/*
 * BLE_CCP_CLT_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes,needed for
 * the storage of data structures in CCP Client whose size depends on the number of supported
 * connections and the number of supported Telephony Bearer Instances.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device
 * will support. Valid values are from 1 to 8.
 *
 * @param num_bearer_instances: Maximum number of supported Telephony Bearer Instances (without
 * take in account the mandatory Generic Telephony Bearer) per connection.
 *
 * @param max_uri_length: Maximum size of the Call URI composed of URI scheme followed by the Caller ID.
 */
#define BLE_CCP_CLT_TOTAL_BUFFER_SIZE(num_ble_links, num_bearer_instances,max_uri_length) \
          ((CCP_CLT_MEM_PER_BEARER_SIZE_BYTES * num_ble_links * (1 + num_bearer_instances)) + \
           (CCP_CLT_MEM_PER_CONN_SIZE_BYTES * num_ble_links) + \
           (DIVC((max_uri_length * num_ble_links * (1 + num_bearer_instances)),4u) * 4u))

/*
 * CCP_SRV_MEM_PER_CALL_SIZE_BYTES: memory size used per call in CCP Server role
 */
#define CCP_SRV_MEM_PER_CALL_SIZE_BYTES                 (28u)

/*
 * CCP_SRV_MEM_PER_BEARER_SIZE_BYTES: memory size used per Telephony Bearer in CCP Server role
 */
#define CCP_SRV_MEM_PER_BEARER_SIZE_BYTES               (112u)

/*
 * CCP_SRV_MEM_PER_READLONG_PROC_SIZE_BYTES: memory size used per Read Long Procedure in CCP Server role
 */
#define CCP_SRV_MEM_PER_READLONG_PROC_SIZE_BYTES        (12u)


/*
 * BLE_CCP_SRV_MEM_BLOCKS_URI_SCHEMES_LIST_SIZE_BYTES: this macro returns the amount of memory,in bytes, needed for
 * the storage of the URI Schemes List data.
 *
 * @param num_bearer_instances: Maximum number of supported Telephony Bearer Instances (without
 * take in account the mandatory Generic Telephony Bearer).
 *
 * @param max_uri_schemes_list_length: Maximum supported size of the URI Schemes List.
 */
#define BLE_CCP_SRV_MEM_BLOCKS_URI_SCHEMES_LIST_SIZE_BYTES(num_bearer_instances,max_uri_schemes_list_length) \
          (DIVC((max_uri_schemes_list_length * (1+num_bearer_instances)),4u) * 4u)

/*
 * BLE_CCP_SRV_MEM_BLOCKS_URI_SIZE_BYTES: this macro returns the amount of memory,in bytes, needed for
 * the storage of the URI data.
 *
 * @param num_bearer_instances: Maximum number of supported Telephony Bearer Instances (without
 * take in account the mandatory Generic Telephony Bearer).
 *
 * @param max_num_calls: Maximum number of supported calls.
 *
 * @param ccp_feature: Supported CCP Option Feature (CCP_OptionFeatures_t type).
 *
 * @param max_uri_length: Maximum size of the Call URI composed of URI scheme followed by the Caller ID.
 */
#define BLE_CCP_SRV_MEM_BLOCKS_URI_SIZE_BYTES(num_bearer_instances,max_num_calls,ccp_feature,max_uri_length) \
          ((DIVC((max_uri_length * max_num_calls * (1+num_bearer_instances)),4u) * 4u) + \
          (((ccp_feature & CCP_FEATURE_INC_CALL_TARGET_BEARER_URI) != (0x00)) ? ((DIVC((max_uri_length * max_num_calls * (1+num_bearer_instances)),4u) * 4u)) : (0)))

/*
 * BLE_CCP_SRV_MEM_BLOCKS_CALL_FRIENDLY_NAME_SIZE_BYTES: this macro returns the amount of memory,in bytes, needed for
 * the storage of the Call Friendly Name data.
 *
 * @param num_bearer_instances: Maximum number of supported Telephony Bearer Instances (without
 * take in account the mandatory Generic Telephony Bearer).
 *
 * @param max_num_calls: Maximum number of supported calls.
 *
 * @param max_call_friendly_name_length: Maximum size of the Call Friendly Name.
 */
#define BLE_CCP_SRV_MEM_BLOCKS_CALL_FRIENDLY_NAME_SIZE_BYTES(num_bearer_instances,max_num_calls,max_call_friendly_name_length) \
          (DIVC((max_call_friendly_name_length * max_num_calls * (1+num_bearer_instances)),4u) * 4u)

/*
 * BLE_CCP_SRV_TOTAL_BUFFER_SIZE:  this macro returns the amount of memory, in bytes,needed for
 * the storage of data structures in CCP Server.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device
 * will support. Valid values are from 1 to 8.
 *
 * @param num_bearer_instances: Maximum number of supported Telephony Bearer Instances (without
 * take in account the mandatory Generic Telephony Bearer).
 *
 * @param max_num_calls: Maximum number of supported calls.
 *
 * @param read_long_en: Indicate if the remote Read Long procedure is supported.
 *
 * @param mem_blocks_uri: Memory Size allocated for Call URI storage.
 *
 * @param mem_blocks_uri_schemes_list: Memory Size allocated for URI Schemes List storage.
 *
 * @param mem_blocks_call_friendly_name: Memory Size allocated for Call Friendly Name storage.
 */
#define BLE_CCP_SRV_TOTAL_BUFFER_SIZE(num_ble_links,num_bearer_instances,max_num_calls,read_long_en,\
                                      mem_blocks_uri,mem_blocks_uri_schemes_list,mem_blocks_call_friendly_name) \
          ((CCP_SRV_MEM_PER_BEARER_SIZE_BYTES * (1 + num_bearer_instances)) + \
           (CCP_SRV_MEM_PER_CALL_SIZE_BYTES * max_num_calls * (1 + num_bearer_instances)) + \
           (((read_long_en) == (0x01)) ? (CCP_SRV_MEM_PER_READLONG_PROC_SIZE_BYTES * num_ble_links * (1 + num_bearer_instances)) : (0)) + \
           mem_blocks_uri + mem_blocks_uri_schemes_list + mem_blocks_call_friendly_name)

/* Types ---------------------------------------------------------------------*/

/* Types of mask for roles of the Call Control Profile */
typedef uint8_t CCP_Role_t;
#define CCP_ROLE_SERVER			        (0x01)
#define CCP_ROLE_CLIENT		                (0x02)


/* CCP Client Procedure type.
 * This type is used during CCP Initialization to describe what are the CCP procedures supported by the CCP Client
 */
typedef uint32_t CCP_CLT_Procedure_t;
#define CCP_CLT_SUPPORT_PROC_READ_BEARER_PROVIDER_NAME                  (0x00000001)    /* Read Bearer Provider Name */
#define CCP_CLT_SUPPORT_PROC_READ_BEARER_UCI                            (0x00000002)    /* Read Bearer UCI */
#define CCP_CLT_SUPPORT_PROC_READ_BEARER_TECHNOLOGY                     (0x00000004)    /* Read Bearer Technology */
#define CCP_CLT_SUPPORT_PROC_READ_BEARER_URI_SCHEMES_LIST               (0x00000008)    /* Read Bearer URI Schemes
                                                                                         * Supported List
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_READ_BEARER_SIGNAL_STRENGTH                (0x00000010)    /* Read Bearer Signal Strength*/
#define CCP_CLT_SUPPORT_PROC_READ_BEARER_SSR_INTERVAL                   (0x00000020)    /* Read Bearer Signal Strength
                                                                                         * Reporting Interval
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_SET_BEARER_SSR_INTERVAL                    (0x00000040)    /* Set Bearer Signal Strength
                                                                                         * Reporting Interval
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_READ_BEARER_LIST_CURRENT_CALLS             (0x00000080)    /* Read Bearer List Current
                                                                                         * Calls
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_READ_INC_CALL_TARGET_BEARER_URI            (0x00000100)    /* Read Incoming Call Target
                                                                                         * Bearer URI
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_READ_STATUS_FLAGS                          (0x00000200)    /* Read Status Flags */
#define CCP_CLT_SUPPORT_PROC_CC_ANSWER_CALL                             (0x00000400)    /* Call Control Point Procedures
                                                                                         * "Answer Incoming Call"
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_CC_TERMINATE_CALL                          (0x00000800)    /* Call Control Point Procedures
                                                                                         * "Terminate Call"
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_CC_LOCAL_HOLD                              (0x00001000)    /* Call Control Point Procedures
                                                                                         * "Move Call To Local Hold"
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_CC_LOCAL_HELD_TO_ACTIVE                    (0x00002000)    /* Call Control Point Procedures
                                                                                         * "Move Locally Held Call To
                                                                                         * Active Call"
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_CC_LOCAL_AND_REMOTE_HELD_TO_REMOTE_HELD    (0x00004000)    /* Call Control Point Procedures
                                                                                         * "Move Locally And Remotely
                                                                                          * Held Call To Remotely
                                                                                          * Held Call"
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_CC_ORIGINATE_CALL                          (0x00008000)    /* Call Control Point Procedures
                                                                                         * "Originate Call"
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_CC_JOIN_CALLS                              (0x00010000)    /* Call Control Point Procedures
                                                                                         * "Join Calls"
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_READ_CC_OPTION_OPCODES                     (0x00020000)    /* Read Call Control Point
                                                                                         * Optional Opcodes
                                                                                         */
#define CCP_CLT_SUPPORT_PROC_READ_INC_CALL                              (0x00040000)    /* Read Incoming Call */
#define CCP_CLT_SUPPORT_PROC_READ_CALL_FRIENDLY_NAME                    (0x00080000)    /* Read Call Friendly Name */
#define CCP_CLT_SUPPORT_PROC_ALL                                        (0x000FFFFF)    /* All CCP procedures */

/* Telephony Bearer optional features*/
typedef uint8_t CCP_OptionFeatures_t;
#define CCP_FEATURE_BEARER_SIGNAL_STRENGTH      (0x01) /* Bearer Signal Strength  is supported.
                                                        * If this feature is supported, the Bearer
                                                        * Signal Strength Reporting (SSR) Interval
                                                        * feature is supported, else excluded
                                                        */
#define CCP_FEATURE_INC_CALL_TARGET_BEARER_URI  (0x02) /* Incoming Call Target Bearer URI feature is supported.*/
#define CCP_FEATURE_CALL_FRIENDLY_NAME          (0x04) /* Call friendly Name feature is supported.*/


/* Types of Bearer Technology. Defined in the Bluetooth assigned numbers */
typedef uint8_t CCP_BearerTechnology_t;
#define CCP_BEARER_TECHNOLOGY_3G                (0x01u)
#define CCP_BEARER_TECHNOLOGY_4G                (0x02u)
#define CCP_BEARER_TECHNOLOGY_LTE               (0x03u)
#define CCP_BEARER_TECHNOLOGY_WIFI              (0x04u)
#define CCP_BEARER_TECHNOLOGY_5G                (0x05u)
#define CCP_BEARER_TECHNOLOGY_GSM               (0x06u)
#define CCP_BEARER_TECHNOLOGY_CDMA              (0x07u)
#define CCP_BEARER_TECHNOLOGY_2G                (0x08u)
#define CCP_BEARER_TECHNOLOGY_WCDMA             (0x09u)

/* Types describing the inband ringtone feature status */
typedef uint8_t CCP_InbandRingToneStatus_t;
#define CCP_INBAND_RINGTONE_DISABLED            (0x00u)     /* inband ringtone disabled */
#define CCP_INBAND_RINGTONE_ENABLED             (0x01u)     /* inband ringtone enabled */

/* Types describing the silent mode feature status */
typedef uint8_t CCP_SilentModeStatus_t;
#define CCP_SILENT_MODE_OFF                     (0x00u) /* Server is not in silent mode */
#define CCP_SILENT_MODE_ON                      (0x01u) /* Server is in silent mode*/

/* Types describing the local Hold feature support */
typedef uint8_t CCP_LocalHoldOption_t;
#define CCP_LOCAL_HOLD_FEATURE_NOT_SUPPORTED    (0x00u) /* Local Hold and Local Retrieve Call operation are not supported */
#define CCP_LOCAL_HOLD_FEATURE_SUPPORTED        (0x01u) /* Local Hold and Local Retrieve Call operation are supported */

/* Types describing the Join feature support */
typedef uint8_t CCP_JoinOption_t;
#define CCP_JOIN_FEATURE_NOT_SUPPORTED          (0x00u) /* Join Call operation is not supported */
#define CCP_JOIN_FEATURE_SUPPORTED              (0x01u) /* Join Call operation is supported*/


typedef uint8_t CCP_CallState_t;
#define CCP_CS_INCOMING                         (0x00)  /* Incoming A remote party is calling (incoming call).*/
#define CCP_CS_DIALING                          (0x01)  /* Dialing The process to call the remote party has started on
                                                         * the server, but the remote party is not being alerted
                                                         * (outgoing call).
                                                         */
#define CCP_CS_ALERTING                         (0x02)  /* Alerting A remote party is being alerted (outgoing call). */
#define CCP_CS_ACTIVE                           (0x03)  /* Active The call is in an active conversation. */
#define CCP_CS_LOCALLY_HELD                     (0x04)  /* Locally Held
                                                         * The call is connected but held locally.“Locally Held” implies
                                                         * that either the server or the client can affect the state.
                                                         */
#define CCP_CS_REMOTELY_HELD                    (0x05)  /* Remotely Held
                                                         * The call is connected but held remotely. “Remotely Held”
                                                         * means that the state is controlled by the remote party of a
                                                         * call.
                                                         */
#define CCP_CS_LOCALLY_REMOTELY_HELD            (0x06)  /* Locally and Remotely Held */
#define CCP_CS_IDLE                             (0xFF)  /* Call is terminated */

/*Call Flags Type*/
typedef uint8_t CCP_CallFlags_t;
#define CCP_CALL_FLAG_DIRECTION_CALL_POS            (0U)
#define CCP_CALL_FLAG_DIRECTION_CALL_MSK            (0x1UL << CCP_CALL_FLAG_DIRECTION_CALL_POS)         /* 0x00000001 */
#define CCP_CALL_FLAG_INCOMING_CALL                 (0U)                                                /* Call is an incoming call*/
#define CCP_CALL_FLAG_OUTGOING_CALL                 CCP_CALL_FLAG_DIRECTION_CALL_MSK                    /* Call is an outgoing call*/
#define CCP_CALL_FLAG_INFO_WITHHELD_SERVER_POS      (01U)                                               /* Position of the Bit used to indicate if URI and Friendly Name
                                                                                                         * information are withhold by the server
                                                                                                         */
#define CCP_CALL_FLAG_INFO_WITHHELD_SERVER_MSK      (0x1UL << CCP_CALL_FLAG_INFO_WITHHELD_SERVER_POS)   /* 0x00000002 */
#define CCP_CALL_FLAG_INFO_NOT_WITHHELD_SERVER      (0U)                                                /* Information not withhold*/
#define CCP_CALL_FLAG_INFO_WITHHELD_SERVER          CCP_CALL_FLAG_INFO_WITHHELD_SERVER_MSK              /* Information withheld by server*/
#define CCP_CALL_FLAG_INFO_WITHHELD_NETWORK_POS     (02U)                                               /* Position of the Bit used to indicate if URI and Friendly Name
                                                                                                         * information are withhold or provided by the network
                                                                                                         */
#define CCP_CALL_FLAG_INFO_WITHHELD_NETWORK_MSK     (0x1UL << CCP_CALL_FLAG_INFO_WITHHELD_NETWORK_POS)  /* 0x00000004 */
#define CCP_CALL_FLAG_INFO_PROVIDED_NETWORK_SERVER  (0u)                                                /* Information provided by network */
#define CCP_CALL_FLAG_INFO_WITHHELD_NETWORK_SERVER  CCP_CALL_FLAG_INFO_WITHHELD_NETWORK_MSK             /* Information withheld by network */


/*Call Control Operation Code Type*/
typedef uint8_t CCP_CallCtrlOpcode_t;
#define CCP_CALL_CTRL_ACCEPT                    (0x00)  /* Answer the incoming call identified by Call_Index.*/
#define CCP_CALL_CTRL_TERMINATE                 (0x01)  /* End the active, alerting (outgoing), dialing (outgoing),
                                                         * incoming, or held (locally or remotely) call.
                                                         */
#define CCP_CALL_CTRL_LOCAL_HOLD                (0x02)  /* Place the active or incoming call identified
                                                         * by on local hold.
                                                         */
#define CCP_CALL_CTRL_LOCAL_RETRIEVE            (0x03)  /* Move a locally held call to an active call.
                                                         * Move a locally and remotely held call to a remotely held call.
                                                         */
#define CCP_CALL_CTRL_ORIGINATE                 (0x04)  /* Initiate a call to the remote party identified by the URI.*/
#define CCP_CALL_CTRL_JOIN                      (0x05)  /* Put calls in the list that are not in the Remotely Held state
                                                         * to the active state and join the calls.
                                                         * Any calls in the Remotely Held or Locally and Remotely Held
                                                         * states move to the Remotely Held state and are joined with
                                                         * the other calls.
                                                         */

/*Call Control Response Code Type*/
typedef uint8_t CCP_CallCtrlRespCode_t;
#define CCP_CALL_CTRL_RSP_SUCCESS               (0x00)  /* The opcode write was successful */
#define CCP_CALL_CTRL_RSP_NOT_SUPPORTED         (0x01)  /* An invalid opcode was used for the Call Control Point write*/
#define CCP_CALL_CTRL_RSP_NOT_POSSIBLE          (0x02)  /* The requested operation cannot be completed */
#define CCP_CALL_CTRL_RSP_INVALID_CALL_INDEX    (0x03)  /* The Call Index used for the Call Control Point write is
                                                         * invalid
                                                         */
#define CCP_CALL_CTRL_RSP_STATE_MISMATCH        (0x04)  /* The opcode written to the Call Control Point was received
                                                         * when the current Call State for the Call Index was not in
                                                         * the expected state
                                                         */
#define CCP_CALL_CTRL_RSP_LACK_OF_RESOURCES     (0x05)  /* Lack of internal resources to complete the requested action*/
#define CCP_CALL_CTRL_RSP_INVALID_OUTGOING_URI  (0x06)  /* The Outgoing URI is incorrect or when an Originate opcode
                                                         * is sent
                                                         */

typedef uint8_t CCP_CallTerminationReason_t;
#define CCP_CALL_TERMINATION_INVALID_URI        (0x00u) /* The URI value used to originate a call is formed improperly*/
#define CCP_CALL_TERMINATION_CALL_FAILED        (0x01u) /* The call failed */
#define CCP_CALL_TERMINATION_REMOTE_PARTY_END   (0x02u) /* The remote party ended the call */
#define CCP_CALL_TERMINATION_SERVER_END         (0x03u) /* The call ended from the server */
#define CCP_CALL_TERMINATION_BUSY_LINE          (0x04u) /* The line was busy */
#define CCP_CALL_TERMINATION_NETWORK_CONGESTION (0x05u) /* Network congestion */
#define CCP_CALL_TERMINATION_CLIENT_END         (0x06u) /* The client terminated the call */
#define CCP_CALL_TERMINATION_NO_SERVICE         (0x07u) /* No service */
#define CCP_CALL_TERMINATION_NO_ANSWER          (0x08u) /* No answer */
#define CCP_CALL_TERMINATION_UNSPECIFIED        (0x09u) /* Unspecified */

/* Call Control Profile Event*/
typedef enum
{
  CCP_SRV_ORIGINATE_CALL_REQ_EVT,       /* This event is notified when a remote CCP Client requests to origintae a call.
                                         * The Application Lyer shall specify if the server shall withhold the URI and
                                         * friendly name information.
                                         * The pInfo field indicates the state of the call and the associated
                                         * information through the CCP_SRV_Originate_Call_Req_Params_Evt_t type.
                                         */
  CCP_SRV_CALL_STATE_EVT,               /* This event is notified when the state of a call changes.
                                         * The state changes could be issued from a CCP Client
                                         * operation request or from a CCP Server operation.
                                         * The pInfo field indicates the state of the call and the associated
                                         * information through the CCP_SRV_Call_State_Params_Evt_t type.
                                         */
  CCP_CLT_GTBS_INFO_EVT,                /* This event is notified by CCP Client during the link up process for the
                                         * Generic Telephony Bearer.
                                         * The pInfo field indicates information through the CCP_CLT_BearerInfo_Evt_t
                                         * type.
                                         */
  CCP_CLT_TBS_INFO_EVT,                 /* This event is notified by CCP Client during the link up process for a
                                         * Telephony Bearer Instance.
                                         * Note this event is notified for each Telephony Bearer Instance
                                         * The pInfo field indicates information through the CCP_CLT_BearerInfo_Evt_t
                                         * type.
                                         */
  CCP_CLT_CALL_STATE_EVT,               /* This event is notified when the state of a call changes.
                                         * The state changes could be issued from a CCP Client
                                         * operation request or from a CCP Server operation.
                                         * The pInfo field indicates the state of the call and the associated
                                         * information through the CCP_CLT_Call_State_Params_Evt_t type.
                                         */
  CCP_CLT_CURRENT_CALL_EVT,             /* This event is notified by CCP Client for each current call
                                         * pInfo field indicates the Call Item through the CCP_CLT_Call_Item_Evt_t type.
                                         */
  CCP_CLT_OPERATION_COMPLETE_EVT,       /* This event is notified when a CCP Client Operation is complete.
                                         * A new CCP Client operation could be started
                                         */
  CCP_CLT_OPERATION_TRANSMITTED_EVT,    /* This event is notified when a CCP Client Call Control Operation is
                                         * transmitted.
                                         * pInfo field indicates the transmitted Call Control Operation to the remote
                                         * Call Control Server through the CCP_CLT_CallCtrlOpTransmitted_Evt_t type.
                                         * After receiving this event, a new CCP Client operation could be started.
                                         * A CCP_CLT_RESPONSE_EVT event should be received after this event.
                                         */
  CCP_CLT_RESPONSE_EVT,                 /* This event is notified once the response of a Call Control Operation is
                                         * received from the remote Call Control Server.
                                         * pInfo field indicates the information response from the remote
                                         * Call Control Server through the CCP_CLT_Call_Ctrl_Rsp_Evt_t type.
                                         */
  CCP_CLT_PROVIDER_NAME_EVT,            /* This event is notified by CCP when a Bearer Provider Name is read
                                         * thanks to the CCP_CLIENT_ReadProviderName() function or if
                                         * the Bearer Provider Name has changed in CCP Server side.
                                         * pInfo field indicates the Bearer Provider Name through the
                                         *  CCP_CLT_Bearer_Provider_Name_Evt_t type.
                                         */
  CCP_CLT_UCI_EVT,                      /* This event is notified by CCP when a Bearer UCI is read  thanks to the
                                         * CCP_CLIENT_ReadUCI() function or if the Bearer UCI has changed in
                                         * CCP Server side.
                                         * pInfo field indicates the Bearer UCI through the
                                         * CCP_CLT_Bearer_UCI_Evt_t type.
                                         */
  CCP_CLT_TECHNOLOGY_EVT,               /* This event is notified by CCP when a Bearer Technology is read thanks
                                         * to the CCP_CLIENT_ReadTechnology() function or if the Bearer Technology
                                         * has changed in CCP Server side.
                                         * pInfo field indicates the Bearer Technology through the
                                         * CCP_CLT_Bearer_Technology_Evt_t type.
                                         */
  CCP_CLT_URI_SCHEMES_LIST_EVT,         /* This event is notified by CCP when a Bearer URI Schemes Supported List is
                                         * read thanks to the CCP_CLIENT_ReadSupportedURISchemesList() function or if
                                         * the Bearer URI Schemes Supported List has changed in CCP Server side.
                                         * pInfo field indicates the Bearer URI Schemes Supported List through
                                         * the CCP_CLT_Bearer_URI_Schemes_List_Evt_t type.
                                         */
  CCP_CLT_SIGNAL_STRENGTH_EVT,          /* This event is notified by CCP when a Signal Strength is read thanks to
                                         * the CCP_CLIENT_ReadSignalStrength() function or if the Signal Strength has
                                         * changed in CCP Server side.
                                         * pInfo field indicates the Signal Strength through the
                                         * CCP_CLT_Signal_Strength_Evt_t type.
                                         */
  CCP_CLT_SSR_INTERVAL_EVT,             /* This event is notified by CCP when a Signal Strength Reporting Interval is
                                         * read thanks to the CCP_CLIENT_ReadSSRInterval() function or if the
                                         * Signal Strength Reporting Interval has changed in CCP Server side.
                                         * pInfo field indicates the Signal Strength Reporting Reporting Interval
                                         * through the CCP_CLT_SSR_Interval_Evt_t type.
                                         */
  CCP_CLT_NO_CURRENT_CALL_EVT,          /* This event is notified by CCP Client if no call is in progress in CCP Server
                                         * side when CCP Client has called the CCP_CLIENT_ReadListCurrentCalls()
                                         */
  CCP_CLT_INC_CALL_TARGET_URI_EVT,      /* This event is notified by CCP Clientwhen a Incoming Call Target URI is read
                                         * thanks to the CCP_CLIENT_ReadIncomingCallTargetURI() function or if the
                                         * Incoming Call Target URI has changed in CCP Server side.
                                         * pInfo field indicates the Incoming Call Target URI
                                         * through the CCP_CLT_Call_URI_Evt_t type.
                                         */
  CCP_CLT_FEATURES_STATUS_EVT,          /* This event is notified by CCP Client when a Status Flags is read thanks
                                         * to the CCP_CLIENT_ReadFeaturesStatus() function or if the Status Flags
                                         * has changed in CCP Server side.
                                         * pInfo field indicates the Status Flags through the
                                         * CCP_CLT_Status_Flags_Evt_t type.
                                         */

  CCP_CLT_CALL_CTRL_OPTIONS_EVT,        /* This event is notified by CCP Client when a Call Control Options are read
                                         * thanks to the CCP_CLIENT_ReadCallCtrlOptions() function or if the
                                         * Call Control Options have changed in CCP Server side.
                                         * pInfo field indicates the Status Flags through the
                                         * CCP_CLT_Call_Ctrl_Options_Evt_t type.
                                         */
  CCP_CLT_INCOMING_CALL_EVT,             /* This event is notified by CCP Client when an Incoming Caller information
                                         * is read thanks to the CCP_CLIENT_ReadIncomingCall() function or if the
                                         * Incoming Caller information is updated in CCP Server side.
                                         * pInfo field indicates the Incoming Caller information through the
                                         * CCP_CLT_Call_URI_Evt_t type.
                                         */
  CCP_CLT_CALL_FRIENDLY_NAME_EVT,       /* This event is notified by CCP Client when a Call Friendly Name information
                                         * is read thanks to the CCP_CLIENT_ReadCallFriendlyName() function or if the
                                         * Call Friendly Name has changed in CCP Server side.
                                         * pInfo field indicates the Call Friendly Name through the
                                         * CCP_CLT_Call_Name_Evt_t type.
                                         */

  CCP_CLT_CALL_TERMINATION_EVT          /* This event is notified by CCP Client when a Call is terminated.
                                         * pInfo field indicates the Call Friendly Name through the
                                         * CCP_CLT_Call_Termination_Evt_t type.
                                         */
} CCP_NotCode_t;


/* Originate Call Request Information used when CCP_SRV_ORIGINATE_CALL_REQ_EVT event is notified */
typedef struct
{
  uint8_t *WithHeldByServer;    /* 0 if URI and Friendly Name information are NOT withhold by the server
                                 * 1 if URI and Friendly Name information are withhold by the server
                                 */
  uint8_t *WithHeldByNetwork;   /* 0 if URI and Friendly Name information are provided by the network
                                 * 1 if URI and Friendly Name information are withhold by the network
                                 */
} CCP_SRV_Originate_Call_Req_Params_Evt_t;

/* Call State State Information used when CCP_SRV_CALL_STATE_EVT event is notified */
typedef struct
{
  uint8_t               CallIndex;      /* Call Index */
  CCP_CallState_t       CallState;      /* CallState */
  CCP_CallFlags_t       CallFlags;      /* Call Flags */
  uint8_t               CallURILength;  /* URI Length*/
  uint8_t               *pCallURI;      /* Pointer on the Call URI*/
} CCP_SRV_Call_State_Params_Evt_t;



/* Structure used in parameter when CCP_CLT_GTBS_INFO_EVT and CCP_CLT_TBS_INFO_EVT events are notified*/
typedef struct
{
  CCP_OptionFeatures_t  FeaturesMask;   /*Mask of Bearer features supported on the remote CCP Server*/
  uint16_t              StartAttHandle; /* ATT Start Handle of the Telephone Bearer Service in the remote CCP Server */
  uint16_t              EndAttHandle;   /* ATT End Handle of the Telephone Bearer Service in the remote CCP Server */
} CCP_CLT_BearerInfo_Evt_t;

/* Structure used in parameter when CCP_CLT_OPERATION_TRANSMITTED_EVT event is notified*/
typedef struct
{
  CCP_CallCtrlOpcode_t          CallCtrlOp;     /* Call Control operation*/
} CCP_CLT_CallCtrlOpTransmitted_Evt_t;


/* Structure used in parameter when CCP_CLT_RESPONSE_EVT event is notified*/
typedef struct
{
  CCP_CallCtrlOpcode_t          CallCtrlOp;     /* Call Control request*/
  uint8_t                       CallIndex;      /* Call Index requested*/
  CCP_CallCtrlRespCode_t        CallCtrlResp;   /* Result Code*/
} CCP_CLT_Call_Ctrl_Rsp_Evt_t;

/* Structure used in parameter when CCP_CLT_PROVIDER_NAME_EVT event is notified*/
typedef struct
{
  uint8_t       *pName;         /* Pointer on the Name string*/
  uint8_t       NameLength;     /* length of the Provider Name*/
} CCP_CLT_Bearer_Provider_Name_Evt_t;

/* Structure used in parameter when CCP_CLT_UCI_EVT event is notified*/
typedef struct
{
  uint8_t       *pUCI;          /* Pointer on the UCI string*/
  uint8_t       Length;         /* length of the UCI*/
} CCP_CLT_Bearer_UCI_Evt_t;

/* Structure used in parameter when CCP_CLT_TECHNOLOGY_EVT event is notified*/
typedef struct
{
  CCP_BearerTechnology_t    Techno;         /* Bearer Technology*/
} CCP_CLT_Bearer_Technology_Evt_t;

/* Structure used in parameter when CCP_CLT_URI_SCHEMES_LIST_EVT event is notified*/
typedef struct
{
  uint8_t       *pURISchemes;   /* Pointer on the comma-delimited list of supported schemes
                                 * expressed as a UTF-8 string
                                 */
  uint8_t       Offset;         /* Offset of the string*/
  uint8_t       Length;         /* length of the URI schemes list string*/
} CCP_CLT_Bearer_URI_Schemes_List_Evt_t;

/* Structure used in parameter when CCP_CLT_SIGNAL_STRENGTH_EVT event is notified*/
typedef struct
{
  uint8_t       SignalStrength; /* Signal Strength*/
} CCP_CLT_Signal_Strength_Evt_t;

/* Structure used in parameter when CCP_CLT_SSR_INTERVAL_EVT event is notified*/
typedef struct
{
  uint8_t       SSR_Interval;   /* Signal Strength Reporting Interval in seconds*/
} CCP_CLT_SSR_Interval_Evt_t;

/* Structure used in parameter when CCP_CLT_CURRENT_CALL_EVT events are notified*/
typedef struct
{
  uint8_t               CallIndex;      /* Call Index */
  CCP_CallState_t       CallState;      /* CallState */
  CCP_CallFlags_t       CallFlags;      /* Call Flags */
  uint8_t               CallURILength;  /* URI Length*/
  uint8_t               *pCallURI;      /* Pointer on the Call URI*/
} CCP_CLT_Call_Item_Evt_t;

/* Structure used in parameter when CCP_CLT_INC_CALL_TARGET_URI_EVT or CCP_CLT_INCOMING_CALL_EVT
 * events are notified*/
typedef struct
{
  uint8_t       CallIndex;      /* Call Index */
  uint8_t       URILength;      /* Call URI Length*/
  uint8_t       *pURI;          /* Pointer on the Call URI Length*/
} CCP_CLT_Call_URI_Evt_t;

/* Structure used in parameter when CCP_CLT_FEATURES_STATUS_EVT event is notified*/
typedef struct
{
  CCP_InbandRingToneStatus_t    InbandRingtone; /* Inband ringtone feature status */
  CCP_SilentModeStatus_t        SilentMode;     /* Silent mode feature status */
} CCP_CLT_Features_Status_Evt_t;

/* Structure used in parameter when CCP_CLT_CALL_CTRL_OPTIONS_EVT event is notified*/
typedef struct
{
  CCP_LocalHoldOption_t LocalHoldOption;        /* local Hold feature support */
  CCP_JoinOption_t      JoinOption;             /* Join feature support */
} CCP_CLT_Call_Ctrl_Options_Evt_t;

/* Structure used in parameter when CCP_CLT_CALL_FRIENDLY_NAME_EVT event is notified*/
typedef struct
{
  uint8_t       CallIndex;      /* Call Index */
  uint8_t       *pName;         /* Pointer on the Friendly Call Name string*/
  uint8_t       NameLength;     /* Length of the Friendly Call Name*/
} CCP_CLT_Call_Name_Evt_t;

/* Call State State Information used when CCP_CLT_CALL_STATE_EVT event is notified */
typedef struct
{
  uint8_t               CallIdx;        /* Call Index (from 1 to 255)*/
  CCP_CallState_t       State;          /* State of the Call*/
  uint8_t               CallFlags;      /* Call Flags */
} CCP_CLT_Call_State_Params_Evt_t;

/* Structure used in parameter when CCP_CLT_CALL_TERMINATION_EVT event is notified*/
typedef struct
{
  uint8_t                       CallIndex;      /* Call Index */
  CCP_CallTerminationReason_t   Reason;         /* Termination Reason*/
} CCP_CLT_Call_Termination_Evt_t;

/*Structure used for Call Control Profile Event */
typedef struct
{
  CCP_NotCode_t EvtOpcode;              /* Opcode of the notified event */
  tBleStatus    Status;                 /* Status associated to the event */
  uint16_t      ConnHandle;             /* ACL Connection Handle associated to the event */
  uint8_t       ContentControlID;       /* Content Control ID of the Telephony Bearer
                                         * Meaningless for CCP_CLT_LINKUP_COMPLETE_EVT
                                         */
  uint8_t       *pInfo;                 /* Pointer on information associated to the event */
}CCP_Notification_Evt_t;

/* Structure used in parameter of the CCP Server Bearer registration*/
typedef struct
{
  uint8_t                       *pProviderName;         /* Friendly name of the provider (for example, a cellular
                                                         * service provider).It shall be different to 0.
                                                         */
  uint8_t                       *pUCI;                  /* Identifier of the telephone bearer (refer to UCI table
                                                         * in assigned number of uniform-caller-identifiers
                                                         * specification).It shall be different to 0.
                                                         */
  CCP_BearerTechnology_t        Technology;             /* Information about the type of technology that is being used
                                                         * for this telephone bearer.It shall be different to 0.
                                                         */
  uint8_t                       *pURISchemesList;       /* URI Schemes Supported List (comma-delimited list of supported
                                                         * schemes expressed as a UTF-8 string :“tel,sip,skype”)
                                                         * refer to Internet Assigned Numbers Authority (IANA)
                                                         * URI Schemes:
                                                         * https://iana.org/assignments/uri-schemes/uri-schemes.xhtml
                                                         * It shall be different to 0.
                                                         */
  uint8_t                       EnSignalStrength;       /* Indicate if Signal Strength feature is enabled or not*/
  uint8_t                       SignalStrength;         /* Signal of the telephone bearer(value from 0 to 100, or 255)*/
  CCP_InbandRingToneStatus_t    InbandRingtone;         /* Inband ringtone feature status */
  CCP_SilentModeStatus_t        SilentMode;             /* Silent mode feature status */
  CCP_LocalHoldOption_t         LocalHoldOption;        /* local Hold feature support */
  CCP_JoinOption_t              JoinOption;             /* Join feature support */
} CCP_BearerInit_t;


/* Call Control Server Configuration */
typedef struct
{
  CCP_OptionFeatures_t  ExtFeatures;                    /* Extended CCP Option Features */
  uint8_t               MaxNumTBInstances;              /* Maximum Number of Telephony Bearer Instances */
  uint8_t               MaxNumCalls;                    /* Maximum number of simultaneous calls */
  uint8_t               MaxCallURILength;               /* Maximum size of the Call URI composed of URI scheme
                                                         * followed by the Caller ID
                                                         */
  uint8_t               MaxBearerProviderNameLength;    /* Maximum Length of the Bearer Provider Name */
  uint8_t               MaxBearerUCILength;             /* Maximum Length of the Bearer Uniform Caller Identifier */
  uint8_t               MaxBearerURISchemesListLength;  /* Maximum Length of the Bearer URI Schemes Supported List */
  uint8_t               MaxCallFriendlyNameLength;      /* Maximum Length of the Call Friendly Name */
  uint8_t               ReadLongEn;                     /* Read Long value procedure configuration :
                                                         * - supported (0x01)
                                                         * - unsupported (0x00)
                                                         */
  uint8_t               *pStartRamAddr;                 /* Start address of the RAM buffer allocated for CCP Server
                                                         * Context memory resource.
                                                         * It must be a 32bit aligned RAM area.
                                                         */
  uint16_t              RamSize;                        /* Size of the RAM allocated at pStartRamAddr
                                                         * pointer.
                                                         */
} CCP_SRV_Config_t;

/* Call Control Client Configuration */
typedef struct
{
  CCP_CLT_Procedure_t   ProceduresMask;                 /* Mask of supported CCP procedures */
  uint8_t               MaxNumTBInstPerConn;            /* Maximum Number of Telephony Bearer Instances
                                                         * per CCP connections
                                                         */
  uint8_t               MaxCallURILength;               /* Maximum size of the Call URI composed of URI scheme
                                                         * followed by the Caller ID
                                                         */
  uint8_t               *pStartRamAddr;                 /* Start address of the RAM buffer allocated for CCP Client
                                                         * Context memory resource.
                                                         * It must be a 32bit aligned RAM area.
                                                         */
  uint16_t              RamSize;                        /* Size of the RAM allocated at pStartRamAddr
                                                         * pointer.
                                                         */

} CCP_CLT_Config_t;

/* Structure used of Call Control configuration*/
typedef struct
{
  CCP_Role_t            Role;                           /* Call Control roles supported by local device*/
  uint8_t               MaxNumBleLinks;                 /* Maximum number of BLE Links */
  CCP_SRV_Config_t      Srv;                            /* Call Control Server Configuration */
  CCP_CLT_Config_t      Clt;                            /* Call Control Client Configuration */
} CCP_Config_t;

#ifdef __cplusplus
}
#endif

#endif /* __CCP_TYPES_H */
