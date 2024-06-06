/**
  ******************************************************************************
  * @file    micp.h
  * @author  MCD Application Team
  * @brief   This file contains definitions used for Microphone Control Profile
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
#ifndef __MICP_H
#define __MICP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "micp_types.h"

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Set the mute value for the local MICP device
  * @param  Mute: Mute value
  * @retval status of the operation
  */
tBleStatus MICP_DEVICE_SetMute(Microphone_Mute_t Mute);

/**
  * @brief  Update the Audio Input State associated to a specified Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  pState: Pointer on Audio Input State settings
  * @retval status of the operation
  */
tBleStatus MICP_DEVICE_UpdateAudioInputState(uint8_t InstID,MICP_AudioInputState_t *pState);

/**
  * @brief  Update the Gain Setting Properties associated to a specified Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  pProp: Pointer on Gain Setting Properties settings
  * @retval status of the operation
  */
tBleStatus MICP_DEVICE_UpdateGainSettingProperties(uint8_t InstID,MICP_GainSettingProp_t *pProp);

/**
  * @brief  Update the type of the Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  AudioInputType: Audio Input Type value
  * @retval status of the operation
  */
tBleStatus MICP_DEVICE_UpdateAudioInputType(uint8_t InstID,Audio_Input_t AudioInputType);

/**
  * @brief  Update the statue of the Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  AudioInputStatus: Audio Input Type value
  * @retval status of the operation
  */
tBleStatus MICP_DEVICE_UpdateAudioInputStatus(uint8_t InstID, MICP_AudioInputStatus_t AudioInputStatus);

/**
  * @brief  Update the Description associated to a specified Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  pDescription: Pointer on Audio Input Description string
  * @param  DecriptionLength: Length of the Audio Input Description string
  * @retval status of the operation
  */
tBleStatus MICP_DEVICE_UpdateAudioInputDescription(uint8_t InstID,uint8_t *pDescription,uint8_t DecriptionLength);

/**
  * @brief  Set the Mute value for the remote MICP Device
  * @note   The MICP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  * @param  ConnHandle: ACL connection handle
  * @param  Mute: Mute value to set
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_SetMute(uint16_t ConnHandle, Microphone_Mute_t Mute);

/**
  * @brief  Read the Mute value
  * @note   The MICP_CONTROLLER_MUTE_EVT event will be notified to report the operation result.
  * @note   The MICP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_ReadMute(uint16_t ConnHandle);

/**
  * @brief  Configure the Mute update Notification
  * @note   The MICP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MICP_CONTROLLER_MUTE_EVT event would be notified when Mute value change
  *         in MICP Device.
  * @param  ConnHandle: ACL connection handle
  * @param  Enabled: if 0, Mute value change in MICP Device is never notified.
  *                  if 1, Mute value change in MICP Device is notified.
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_ConfigureMuteNotification(uint16_t ConnHandle,uint8_t Enabled);

/**
  * @brief  Configure the Audio Input State update Notification
  * @note   The MICP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event would be notified when
  *         Audio Input State change in MICP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  Enabled: if 0, Audio Input State change in MICP Renderer is never notified.
  *                  if 1, Audio Input State change in MICP Renderer is notified.
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_ConfigureAudioInputStateNotification(uint16_t ConnHandle,uint8_t Inst,uint8_t Enabled);

/**
  * @brief  Configure the Audio Input Status update Notification
  * @note   The MICP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MICP_CONTROLLER_AUDIO_INPUT_STATUS_EVT event would be notified when
  *         Audio Input Status change in MICP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  Enabled: if 0, Audio Input Status change in MICP Renderer is never notified.
  *                  if 1, Audio Input Status change in MICP Renderer is notified.
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_ConfigureAudioInputStatusNotification(uint16_t ConnHandle,uint8_t Inst,uint8_t Enabled);

/**
  * @brief  Configure the Audio Input Description update Notification
  * @note   The MICP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @note   If notification is enabled, MICP_CONTROLLER_AUDIO_INPUT_DESCRIPTION_EVT event would be notified when
  *         Audio Input Description change in MICP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  Enabled: if 0, Audio Input Description change in MICP Renderer is never notified.
  *                  if 1, Audio Input Description change in MICP Renderer is notified.
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_ConfigureAudioInputDescNotification(uint16_t ConnHandle,uint8_t Inst,uint8_t Enabled);

/**
  * @brief  Read the Audio Input State
  * @note   The MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be notified to report the operation result.
  * @note   The MICP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_ReadAudioInputState(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Read the Gain Setting Properties of the Audio Input Control Instance on the remote MICP Renderer
  * @note   The MICP_CONTROLLER_GAIN_SETTING_PROP_EVT event will be notified to report the operation result.
  * @note   The MICP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_ReadGainSettingProperties(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Read the current state of the Audio Input Control Instance on the remote MICP Renderer
  * @note   The MICP_CONTROLLER_AUDIO_INPUT_STATUS_EVT event will be notified to report the operation result.
  * @note   The MICP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_ReadAudioInputStatus(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Read the description of the audio input that the Audio Input Control Instance describes
  *         on the remote MICP Renderer
  * @note   The MICP_CONTROLLER_AUDIO_INPUT_DESCRIPTION_EVT event will be notified to report the operation result.
  * @note   The MICP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_ReadAudioInputDescription(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Set Gain setting of the Audio Input Control Instance on the remote MICP Renderer
  * @note   The MICP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter'
  *         in parameter doesn't match with the Change Counter of the remote MICP Renderer.
  *         If the status associated to the event is HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE_ERR_CODE, it means
  *         that the GainSetting value is out of range Gain Setting Min and Gain Setting Max.
  * @note   This command doesn't affect the Mute setting of the AUdio Input Control Instance in the MICP Renderer.
  * @note   If the operation causes a Gain Setting change, the MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  GainSetting: Gain Setting
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in MICP Renderer returned in
  *                        MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_SetAudioInputGainSetting(uint16_t ConnHandle,
                                                   uint8_t Inst,
                                                   int8_t GainSetting,
                                                   uint8_t ChangeCounter);

/**
  * @brief  Unmute the Audio Input Control Instance on the remote MICP Renderer
  * @note   The MICP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote MICP Renderer.
  *         If the status associated to the event is HCI_COMMAND_DISALLOWED_ERR_CODE, it means that the Mute in
  *         MICP Renderer side is Disabled for the specified Audio Input Control Instance.
  * @note   If the operation causes a Mute State change, the MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in MICP Renderer returned in
  *                        MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_UnmuteAudioInput(uint16_t ConnHandle,uint8_t Inst,uint8_t ChangeCounter);

/**
  * @brief  Mute the Audio Input Control Instance on the remote MICP Renderer
  * @note   The MICP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote MICP Renderer.
  *         If the status associated to the event is HCI_COMMAND_DISALLOWED_ERR_CODE, it means that the Mute in
  *         MICP Renderer side is Disabled for the specified Audio Input Control Instance.
  * @note   If the operation causes a Mute State change, the MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in MICP Renderer returned in
  *                        MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_MuteAudioInput(uint16_t ConnHandle,uint8_t Inst,uint8_t ChangeCounter);

/**
  * @brief  Set The Gain Mode in Manual for the Audio Input Control Instance on the remote MICP Renderer
  * @note   The MICP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote MICP Renderer.
  *         If the status associated to the event is HCI_COMMAND_DISALLOWED_ERR_CODE, it means that the Gain Mode in
  *         MICP Renderer side is Automatic Only or Manual Only for the specified Audio Input Control Instance.
  * @note   If the operation causes a Gain Mode change, the MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in MICP Renderer returned in
  *                        MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_SetManualGainMode(uint16_t ConnHandle,uint8_t Inst,uint8_t ChangeCounter);

/**
  * @brief  Set The Gain Mode in Automatic for the Audio Input Control Instance on the remote MICP Renderer
  * @note   The MICP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote MICP Renderer.
  *         If the status associated to the event is HCI_COMMAND_DISALLOWED_ERR_CODE, it means that the Gain Mode in
  *         MICP Renderer side is Automatic Only or Manual Only for the specified Audio Input Control Instance.
  * @note   If the operation causes a Gain Mode change, the MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in MICP Renderer returned in
  *                        MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_SetAutomaticGainMode(uint16_t ConnHandle,uint8_t Inst,uint8_t ChangeCounter);

/**
  * @brief  Write the Audio Input Description of the Audio Input Control Instance in the remote MICP Renderer
  * @note   If the operation causes an Audio Input Description change, the MICP_CONTROLLER_AUDIO_INPUT_DESCRIPTION_EVT
  *         event will be received if Audio Input Description Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  pDesc: Pointer on the UTF-8 string corresponding to the description of the Audio input
  * @param  Length: length of the description string parameter
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_WriteAudioInputDesc(uint16_t ConnHandle,uint8_t Inst,uint8_t *pDesc,uint8_t Length);

/**
  * @brief  Remove the record of the Microphone Control Profile Controller stored in the Non Volatile memory.
  * @param  PeerIdentityAddressType: Identity address type.
  *                                  Values:
  *                                     - 0x00: Public Identity Address
  *                                     - 0x01: Random (static) Identity Address
  * @param  PeerIdentityAddress : Public or Random (static) Identity address of the peer device
  * @retval status of the operation
  */
tBleStatus MICP_CONTROLLER_RemoveRecord(uint8_t PeerIdentityAddressType,const uint8_t PeerIdentityAddress[6]);

#ifdef __cplusplus
}
#endif

#endif /* __MICP_H */
