/**
  ******************************************************************************
  * @file    ccp.h
  * @author  MCD Application Team
  * @brief   This file contains definitions used for Call Control Profile
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
#ifndef __CCP_H
#define __CCP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_types.h"
#include "ccp_types.h"

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Set the Telephone Bearer Provider Name
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  pName: pointer on the Bearer Provider name
  * @param  NameLen: Bearer Provider name length
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetProviderName(uint8_t ContentControlID, uint8_t *pName, uint16_t NameLen);

/**
  * @brief  Set the Telephone Bearer Technology
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  TechnologyID: Bearer Technology type
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetBearerTechnology(uint8_t ContentControlID, CCP_BearerTechnology_t TechnologyID);

/**
  * @brief  Set the list of the supported URI schemes
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  pURISchemes: Pointer on the comma-delimited list of supported schemes
  *                      expressed as a UTF-8 string
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetURISchemesSupportedList(uint8_t ContentControlID, uint8_t *pURISchemes);

/**
  * @brief  Set the current Signal Strength
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  SignalStrength: level of the signal of the telephone bearer (can hold a value from 0 to 100, or 255)
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetBearerSignalStrength(uint8_t ContentControlID, uint8_t SignalStrength);

/**
  * @brief  Set the current status of features that the server device supports.
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  InbandRingtone: Inband Ringtone feature status
  * @param  SilentMode: Silent Mode feature status
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetFeaturesStatus(uint8_t ContentControlID,
                                        CCP_InbandRingToneStatus_t InbandRingtone,
                                        CCP_SilentModeStatus_t SilentMode);

/**
  * @brief  Set the Call Control Options supported or not by the Server
  * @note   This function shall not be called during a connection
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  LocalHoldOption: Local Hold Feature option
  * @param  JoinOption: Join Feature option
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetCallControlOptions(uint8_t ContentControlID,
                                            CCP_LocalHoldOption_t LocalHoldOption,
                                            CCP_JoinOption_t JoinOption);

/**
  * @brief  Set the Incoming Call Target Bearer URI
  * @note   This function shall be called when an incoming call arrives at the telephone bearer.
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  CallIdx: Call Index
  * @param  pIncomingCallTargetURI: pointer on URI of the target of an incoming call.Format as a UTF-8 string
  *                                (the string is populated with the URI scheme followed by the identity of the target
  *                                of the call).
  * @param  Len: length of the URI
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetIncomingCallTargetURI(uint8_t ContentControlID,
                                               uint8_t CallIdx,
                                               uint8_t *pIncomingCallTargetURI,
                                               uint16_t Len);

/**
  * @brief  Set the Call Friendly Name
  * @note   This function shall be called when there is an new incoming or outgoing call.
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  CallIdx: Call Index
  * @param  pFriendlyName: pointer on Friendly Name.Format as a UTF-8 string
  * @param  FriendlyNameLength: length of the Friendly Name
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetCallFriendlyName(uint8_t ContentControlID,
                                          uint8_t CallIdx,
                                          uint8_t *pFriendlyName,
                                          uint16_t FriendlyNameLength);

/**
  * @brief  Setup an Incoming Call
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer on which incoming call will be populated
  * @param  pIncomingCallerURI: pointer on URI of the incoming caller.Format as a UTF-8 string
  * @param  Len: length of the URI
  * @param  WithHeldByServer: 0 if URI and Friendly Name information are NOT withhold by the server
  *                           1 if URI and Friendly Name information are withhold by the server
  * @param  WithHeldByNetwork: 0 if URI and Friendly Name information are provided by the network
  *                            1 if URI and Friendly Name information are withhold by the network
  * @param[out]  CallIdx: Call Index on which incoming call is setup.
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetupIncomingCall(uint8_t ContentControlID,
                                        uint8_t *pIncomingCallerURI,
                                        uint16_t Len,
                                        uint8_t WithHeldByServer,
                                        uint8_t WithHeldByNetwork,
                                        uint8_t *CallIdx);

/**
  * @brief  Setup an outgoing Call
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer on which outgoing call will be populated
  * @param  CallState: Dialing or Alerting Call State
  * @param  pURI: pointer on URI (URI UTF-8 string : URI scheme followed by Caller ID)
  * @param  URILength: length of the URI
  * @param  WithHeldByServer: 0 if URI and Friendly Name information are NOT withhold by the server
  *                           1 if URI and Friendly Name information are withhold by the server
  * @param  WithHeldByNetwork: 0 if URI and Friendly Name information are provided by the network
  *                            1 if URI and Friendly Name information are withhold by the network
  * @param[out] CallIdx: Call Index on which outgoing call is setup.
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetupOutgoingCall(uint8_t ContentControlID,
                                        CCP_CallState_t CallState,
                                        uint8_t *pURI,
                                        uint16_t URILength,
                                        uint8_t WithHeldByServer,
                                        uint8_t WithHeldByNetwork,
                                        uint8_t *CallIdx);

/**
  * @brief  Set an outgoing Call in alerting state
  * @note   Once the the CCP Server is informed that the alerting of the remote party has begun,
  *         the CCP Server shall call this function to notify remote CCP Client that the call is
  *         in alerting state.
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer on which  call state change
  *                           will be populated.
  * @param  CallIdx: Index of the call to set in alerting state
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetAlertingCall(uint8_t ContentControlID,uint8_t CallIdx);

/**
  * @brief  Terminate a call
  * @note   This operation has meaning when the call state is in any state.
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer on which  call state change
  *                           will be populated.
  * @param  CallIdx: Index of the call to terminate
  * @param  Reason: Call Termination reason
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_TerminateCall(uint8_t ContentControlID,uint8_t CallIdx,CCP_CallTerminationReason_t Reason);

/**
  * @brief  Set a call in active state
  * @note   This operation has meaning when the call is in Dialing, Alerting or Incoming state.
  * @note   Any other calls that are in the Active state would be moved to the Locally Held state.
  * @note   If any calls are in the Remotely Held state, they would be moved to the Locally and
  *         Remotely Held state.
  * @note   To have multiple calls active at the same time, the CCP_SERVER_JoinCalls() should be used.
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer on which  call state change
  *                           will be populated.
  * @param  CallIdx: Index of the call to set in active state
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetActiveCall(uint8_t ContentControlID,uint8_t CallIdx);

/**
  * @brief  Place a call in Locally Held state
  * @note   This operation has meaning when the call is in Incoming, Active, or Remotely Held state.
  * @note   If the call is in Incoming or Active state, the call state transitions to Locally Held state.
  * @note   If the call is in Remotely Held state, the call state transitions to Locally and Remotely Held.
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer on which  call state change
  *                           will be populated.
  * @param  CallIdx: Index of the call to set in in Locally Held state
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetLocalHoldCall(uint8_t ContentControlID,uint8_t CallIdx);

/**
  * @brief  Place a call in Remotely Held state
  * @note   This operation has meaning when the call is in Incoming, Active, or Locally Held state.
  * @note   If the call is in Incoming or Active state, the call state transitions to Remotely Held state.
  * @note   If the call is in Locally Held state, the call state transitions to Locally and Remotely Held
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer on which  call state change
  *                           will be populated.
  * @param  CallIdx: Index of the call to set in in Remotely Held state
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_SetRemoteHoldCall(uint8_t ContentControlID,uint8_t CallIdx);

/**
  * @brief  Retrieve a call in Locally Held or Locally and Remotely Held state.
  * @note   This operation has meaning when the call is in Locally Held or Locally and Remotely Held state.
  * @note   If the call is in Locally Held state, the call state transitions to Active state. Any other calls
  *         with a call state of Active transition to the Locally Held state.
  * @note   If the call is in Locally and Remotely Held state, the call state transitions to Remotely Held state.
  *         Any other calls with a call state of Active transition to the Locally Held state.
  * @note   Any other calls with a call state of Remotely Held transition to the Locally and Remotely Held state.
  * @note   The Retrieve operation should not be used to join calls. The CCP_SERVER_JoinCalls() is used to join calls.
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer on which  call state change
  *                           will be populated.
  * @param  CallIdx: Index of the call to retrieve
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_RetrieveCall(uint8_t ContentControlID,uint8_t CallIdx);

/**
  * @brief  Join multiple calls in Active state.
  * @note   This operation has meaning when the call is in any state except of Incoming State.
  * @note   If the state of the call is Active state, the call remains in Active state.
  * @note   If the state of the call is Locally Held state, the state of the call transitions to Active state.
  * @note   If the state of the call is Remotely Held state, the call remains in Remotely Held state.
  * @note   If the state of the call is Locally and Remotely Held state, the state of the call transitions
  *         to Remotely Held state.
  * @note   If the state of the call is Alerting or Dialing state, the Call State remains in that state.
  *         It is left up to the server implementation on how the call is joined after transitioning to the Active state
  *         with a remote answer.
  * @note   If the state of any call NOT in the list of calls to be joined is Active, the state for each of these calls
  *         transitions to Locally Held state.
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer on which  call state change
  *                           will be populated.
  * @param  NumCalls: number of the calls to join (minimum value is 2)
  * @param  pCallIdx: table of the call Index to join
  * @retval status of the operation
  */
tBleStatus CCP_SERVER_JoinCalls(uint8_t ContentControlID,uint8_t NumCalls,uint8_t *pCallIdx);


/**
  * @brief  Configure the Bearer Signal Strength update Notification
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, CCP_CLT_SIGNAL_STRENGTH_EVT event would be notified when
  *         Signal Strength value change in CCP Server and according to the Bearer Signal Strength Reporting Interval.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  Enabled: if 0, Signal Strength change in CCP Server is never notified.
  *                  if 1, Signal Strength change in CCP Server is notified.
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ConfigureSignalStrengthNotification(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Enabled);

/**
  * @brief  Configure the Bearer Current Calls List update Notification
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, CCP_CLT_CURRENT_CALL_EVT event would be notified when the list of the
  *         current calls changes in CCP Server .
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  Enabled: if 0, Current Calls List change in CCP Server is never notified.
  *                  if 1, Current Calls List change in CCP Server is notified.
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ConfigureCurrentCallsListNotification(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Enabled);

/**
  * @brief  Read the Bearer Provider Name
  * @note   The CCP_CLT_PROVIDER_NAME_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadProviderName(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Bearer Uniform Caller Identifier
  * @note   The CCP_CLT_UCI_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadUCI(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Bearer Technology
  * @note   The CCP_CLT_TECHNOLOGY_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadTechnology(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Bearer URI Schemes Supported List
  * @note   The CCP_CLT_URI_SCHEMES_LIST_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadSupportedURISchemesList(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Bearer Signal Strength
  * @note   The CCP_CLT_SIGNAL_STRENGTH_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadSignalStrength(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Bearer Signal Strength Reporting Interval
  * @note   The CCP_CLT_SSR_INTERVAL_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadSSRInterval(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Set the Bearer Signal Strength Reporting Interval
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  Interval: Signal Strength reporting interval in seconds.
  *                   The valid range of values for the Reporting Interval is 0 to 255.
  *                   If the interval is set to 0, the signal strength is reported as soon as the
  *                   signal strength changes.
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_SetSSRInterval(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t Interval);

/**
  * @brief  Read the Bearer List Current Calls
  * @note   The CCP_CLT_CURRENT_CALL_EVT events will be notified to report each current call.
  * @note   If no call is in progress in CCP Server side, the CCP_CLT_NO_CURRENT_CALL_EVT event
  *         is notified.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadListCurrentCalls(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Incoming Call Target Bearer URI
  * @note   The CCP_CLT_INC_CALL_TARGET_URI_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadIncomingCallTargetURI(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Feature (Inband RingTone and Silente Mode) status
  * @note   The CCP_CLT_FEATURES_STATUS_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadFeaturesStatus(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the different states that telephone calls are in.
  * @note   The CCP_CLT_CALL_STATE_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadCallState(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Call Control Point Optional Opcodes
  * @note   The CCP_CLT_CALL_CTRL_OPTIONS_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadCallCtrlOptions(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Incoming Call
  * @note   The CCP_CLT_INCOMING_CALL_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadIncomingCall(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Read the Call Friendly Name
  * @note   The CCP_CLT_CALL_FRIENDLY_NAME_EVT event will be notified to report the operation result.
  * @note   The CCP_CLT_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ReadCallFriendlyName(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Answer an incoming call and make the call active on a Call Control Server.
  * @note   The CCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the operation is
  *         transmitted to the Call Control Server.
  * @note   The CCP_CLT_RESPONSE_EVT event will be notified once the response of the Operation is
  *         received from the remote Call Control Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  CallIndex: Index of the call
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_AnswerIncomingCall(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t CallIndex);

/**
  * @brief  End an active, alerting (outgoing), dialing (outgoing), incoming, or held (locally or remotely)
  *         call  on the Call Control Server..
  * @note   The CCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the operation is
  *         transmitted to the Call Control Server.
  * @note   The CCP_CLT_RESPONSE_EVT event will be notified once the response of the Operation is
  *         received from the remote Call Control Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  CallIndex: Index of the call
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_TerminateCall(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t CallIndex);

/**
  * @brief  Place an active or incoming call on local hold on the Call Control Server.
  *         Place a remotely held call on locally and remotely hold on the Call Control Server.
  * @note   The CCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the operation is
  *         transmitted to the Call Control Server.
  * @note   The CCP_CLT_RESPONSE_EVT event will be notified once the response of the Operation is
  *         received from the remote Call Control Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  CallIndex: Index of the call
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_LocalHoldCall(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t CallIndex);

/**
  * @brief  Move a call that is being locally held to an active call on the Call Control Server.
  * @note   The CCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the operation is
  *         transmitted to the Call Control Server.
  * @note   The CCP_CLT_RESPONSE_EVT event will be notified once the response of the Operation is
  *         received from the remote Call Control Server.
  * @note   Any calls with a Call State of Active are expected to transition to the Locally Held
  *         state.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  CallIndex: Index of the call
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_ActivateLocalHeldCall(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t CallIndex);

/**
  * @brief  Move a call that is being locally and remotely held to a remotely held call on a Call
  *         Control Server.
  * @note   The CCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the operation is
  *         transmitted to the Call Control Server.
  * @note   The CCP_CLT_RESPONSE_EVT event will be notified once the response of the Operation is
  *         received from the remote Call Control Server.
  * @note   Any calls with a Call State of Active transition to the Locally Held state.
  *         Any calls with a Call State of Remotely Held transition to the Locally and Remotely Held state.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  CallIndex: Index of the call
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_RetrieveLocallyAndRemotelyHeldCall(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t CallIndex);

/**
  * @brief  Make an outgoing call on a Call Control Server.
  * @note   The CCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the operation is
  *         transmitted to the Call Control Server.
  * @note   The CCP_CLT_RESPONSE_EVT event will be notified once the response of the Operation is
  *         received from the remote Call Control Server.
  * @note   If there are any active calls when the Originate opcode is accepted, the calls would be
  *         moved to the Locally Held state.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  pURI: pointer on URI of the outguoing call.The URI is comprised of the URI scheme
  *         followed by the Caller ID (this could be a telephone number or username).
  *         For example, if the URI scheme is “skype” and the Caller ID (username) is “xyz”,
  *         the resultant URI is “skype:xyz” formatted as a UTF-8 string.
  * @param  URILength: Length of the URI string
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_OriginateCall(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t *pURI,uint8_t URILength);

/**
  * @brief  Join multiple calls on the Call Control Server.
  * @note   The CCP_CLT_OPERATION_TRANSMITTED_EVT event will be notified once the operation is
  *         transmitted to the Call Control Server.
  * @note   The CCP_CLT_RESPONSE_EVT event will be notified once the response of the Operation is
  *         received from the remote Call Control Server.
  * @note   The provided list of Call_Index fields shall not include any Call in Incoming State.
  * @note   If the Call State of the call is Active, the Call State shall remain Active.
  *         If the Call State of the call is Locally Held, the Call State transitions to Active.
  *         If the Call State of the call is Remotely Held, the Call State remains Remotely Held.
  *         If the Call State of the call is Locally and Remotely Held, the Call State transitions to Remotely Held.
  *         If the Call State of the call is Alerting or Dialing, the Call State remains in that state.
  *         If the Call State of any call NOT in the list of calls to be joined is Active, the Call State for each of
  *         these calls transitions to Locally Held.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param  NumCalls: Number of calls to join
  * @param  pCallIndexList: list of Call Indexes.
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_JoinCalls(uint16_t ConnHandle,uint8_t ContentControlID,uint8_t NumCalls,uint8_t *pCallIndexList);

/**
  * @brief  Check if the specified CCID corresponds to a Telephony Bearer discovered on the remote Call Control Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @retval if status is BLE_STATUS_SUCCESS, specified connection handle and Content Control ID corresponds to a
  *         discovered Telephony Bearer, else not.
  */
tBleStatus CCP_CLIENT_IsDiscoveredBearer(uint16_t ConnHandle,uint8_t ContentControlID);

/**
  * @brief  Get the CCP Option features supported by the specified Bearer on the remote Call Control Server.
  * @param  ConnHandle: ACL connection handle
  * @param  ContentControlID: Content Control Identifier of the Telephony Bearer
  * @param[out]  FeaturesMask : returned Mask of Bearer features supported on the remote Call Control Server
  * @retval if status is BLE_STATUS_SUCCESS, specified connection handle and Content Control ID corresponds to a
  *         discovered Telephony Bearer, else not.
  */
tBleStatus CCP_CLIENT_GetBearerOptionFeatures(uint16_t ConnHandle,
                                              uint8_t ContentControlID,
                                              CCP_OptionFeatures_t *FeaturesMask);

/**
  * @brief  Remove the record of the Call Control Profile Client stored in the Non Volatile memory.
  * @param  PeerIdentityAddressType: Identity address type.
  *                                  Values:
  *                                     - 0x00: Public Identity Address
  *                                     - 0x01: Random (static) Identity Address
  * @param  PeerIdentityAddress : Public or Random (static) Identity address of the peer device
  * @retval status of the operation
  */
tBleStatus CCP_CLIENT_RemoveRecord(uint8_t PeerIdentityAddressType,const uint8_t PeerIdentityAddress[6]);

#ifdef __cplusplus
}
#endif

#endif /* __CCP_H */
