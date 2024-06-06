/**
  ******************************************************************************
  * @file    vcp.h
  * @author  MCD Application Team
  * @brief   This file contains definitions used for Volume Control Profile
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
#ifndef __VCP_H
#define __VCP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vcp_types.h"

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Set Volume State to initialized values
  * @param  Volume: Volume Setting
  * @param  Mute: Mute State (0: Not Muted ; 1: Muted)
  * @retval status of the initialization
  */
tBleStatus VCP_RENDER_ResetVolumeState(uint8_t Volume, uint8_t Mute);

/**
  * @brief  Set Absolute Volume setting
  * @param  AbsVolume: Absolute Volume Setting
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_SetAbsVolume(uint8_t AbsVolume);

/**
  * @brief  Increase the Volume setting of one step size
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_IncreaseVolume(void);

/**
  * @brief  Decrease the Volume setting of one step size
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_DecreaseVolume(void);

/**
  * @brief  Set the Mute State
  * @param  Mute: Mute State (0: Not Muted ; 1: Muted)
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_SetMuteState(uint8_t Mute);

/**
  * @brief  Update the Audio Input State associated to a specified Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  pState: Pointer on Audio Input State settings
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_UpdateAudioInputState(uint8_t InstID,VCP_AudioInputState_t *pState);

/**
  * @brief  Update the Gain Setting Properties associated to a specified Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  pProp: Pointer on Gain Setting Properties settings
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_UpdateGainSettingProperties(uint8_t InstID,VCP_GainSettingProp_t *pProp);

/**
  * @brief  Update the type of the Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  AudioInputType: Audio Input Type value
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_UpdateAudioInputType(uint8_t InstID,Audio_Input_t AudioInputType);

/**
  * @brief  Update the statue of the Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  AudioInputStatus: Audio Input Type value
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_UpdateAudioInputStatus(uint8_t InstID,VCP_AudioInputStatus_t AudioInputStatus);

/**
  * @brief  Update the Description associated to a specified Audio Input Device Instance
  * @param  InstID: Identifier of the Audio Input Device
  * @param  pDescription: Pointer on Audio Input Description string
  * @param  DecriptionLength: Length of the Audio Input Description string
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_UpdateAudioInputDescription(uint8_t InstID,uint8_t *pDescription,uint8_t DecriptionLength);

/**
  * @brief  Update the Volume Offset State associated to a specified Audio Output Device Instance
  * @param  InstID: Identifier of the Audio Output Device
  * @param  VolumeOffset: Volume Offset State value [-255;255]
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_UpdateVolumeOffsetState(uint8_t InstID,int16_t VolumeOffset);

/**
  * @brief  Update the Audio Location associated to a specified Audio Output Device Instance
  * @param  InstID: Identifier of the Audio Output Device
  * @param  AudioLocation: bitmask that shall be set to a value that reflects the location
  *                       of the Audio Output Device Instance
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_UpdateAudioOutputLocation(uint8_t InstID,Audio_Location_t AudioLocation);

/**
  * @brief  Update the Description associated to a specified Audio Output Device Instance
  * @param  InstID: Identifier of the Audio Output Device
  * @param  pDescription: Pointer on Audio Output Description string
  * @param  DecriptionLength: Length of the Audio Output Description string
  * @retval status of the operation
  */
tBleStatus VCP_RENDER_UpdateAudioOutputDescription(uint8_t InstID,uint8_t *pDescription,uint8_t DecriptionLength);

/**
  * @brief  Read the Volume State
  * @note   The VCP_CONTROLLER_VOLUME_STATE_EVT event will be notified to report the operation result.
  * @note   The VCP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_ReadVolumeState(uint16_t ConnHandle);

/**
  * @brief  Read the Volume Flags
  * @note   The VCP_CONTROLLER_VOLUME_FLAGS_EVT event will be notified to report the operation result.
  * @note   The VCP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_ReadVolumeFlags(uint16_t ConnHandle);

/**
  * @brief  Decrease the Volume setting of the remote VCP Renderer.
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  * @note   This command doesn't affect the Mute setting in the VCP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_VOLUME_STATE_EVT or VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_DecreaseVolume(uint16_t ConnHandle, uint8_t ChangeCounter);

/**
  * @brief  Increase the Volume setting of the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  * @note   This command doesn't affect the Mute setting in the VCP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_VOLUME_STATE_EVT or VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_IncreaseVolume(uint16_t ConnHandle, uint8_t ChangeCounter);

/**
  * @brief  Unmute and decrease the Volume setting of the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_VOLUME_STATE_EVT or VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_UnmuteAndDecreaseVolume(uint16_t ConnHandle, uint8_t ChangeCounter);

/**
  * @brief  Unmute and increase the Volume setting of the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_VOLUME_STATE_EVT or VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_UnmuteAndIncreaseVolume(uint16_t ConnHandle, uint8_t ChangeCounter);

/**
  * @brief  Set Absolute Volume setting of the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  AbsVolume: Volume Setting
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_VOLUME_STATE_EVT or VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_SetAbsVolume(uint16_t ConnHandle,uint8_t AbsVolume, uint8_t ChangeCounter);

/**
  * @brief  Unmute the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_VOLUME_STATE_EVT or VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_Unmute(uint16_t ConnHandle, uint8_t ChangeCounter);

/**
  * @brief  Mute the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  * @param  ConnHandle: ACL connection handle
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_VOLUME_STATE_EVT or VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_Mute(uint16_t ConnHandle, uint8_t ChangeCounter);

/**
  * @brief  Read the Audio Input State of the Audio Input Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be notified to report the operation result.
  * @note   The VCP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_ReadAudioInputState(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Read the Gain Setting Properties of the Audio Input Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_GAIN_SETTING_PROP_EVT event will be notified to report the operation result.
  * @note   The VCP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_ReadGainSettingProperties(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Read the current state of the Audio Input Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_AUDIO_INPUT_STATUS_EVT event will be notified to report the operation result.
  * @note   The VCP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_ReadAudioInputStatus(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Read the description of the audio input that the Audio Input Control Instance describes
  *         on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_AUDIO_INPUT_DESCRIPTION_EVT event will be notified to report the operation result.
  * @note   The VCP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_ReadAudioInputDescription(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Set Gain setting of the Audio Input Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter'
  *         in parameter doesn't match with the Change Counter of the remote VCP Renderer.
  *         If the status associated to the event is HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE_ERR_CODE, it means
  *         that the GainSetting value is out of range Gain Setting Min and Gain Setting Max.
  * @note   This command doesn't affect the Mute setting of the AUdio Input Control Instance in the VCP Renderer.
  * @note   If the operation causes a Gain Setting change, the VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  GainSetting: Gain Setting
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_SetAudioInputGainSetting(uint16_t ConnHandle,
                                                   uint8_t Inst,
                                                   int8_t GainSetting,
                                                   uint8_t ChangeCounter);

/**
  * @brief  Unmute the Audio Input Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  *         If the status associated to the event is HCI_COMMAND_DISALLOWED_ERR_CODE, it means that the Mute in
  *         VCP Renderer side is Disabled for the specified Audio Input Control Instance.
  * @note   If the operation causes a Mute State change, the VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_UnmuteAudioInput(uint16_t ConnHandle,uint8_t Inst,uint8_t ChangeCounter);

/**
  * @brief  Mute the Audio Input Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  *         If the status associated to the event is HCI_COMMAND_DISALLOWED_ERR_CODE, it means that the Mute in
  *         VCP Renderer side is Disabled for the specified Audio Input Control Instance.
  * @note   If the operation causes a Mute State change, the VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_MuteAudioInput(uint16_t ConnHandle,uint8_t Inst,uint8_t ChangeCounter);

/**
  * @brief  Set The Gain Mode in Manual for the Audio Input Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  *         If the status associated to the event is HCI_COMMAND_DISALLOWED_ERR_CODE, it means that the Gain Mode in
  *         VCP Renderer side is Automatic Only or Manual Only for the specified Audio Input Control Instance.
  * @note   If the operation causes a Gain Mode change, the VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_SetManualGainMode(uint16_t ConnHandle,uint8_t Inst,uint8_t ChangeCounter);

/**
  * @brief  Set The Gain Mode in Automatic for the Audio Input Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter' in
  *         parameter doesn't match with the Change Counter of the remote VCP Renderer.
  *         If the status associated to the event is HCI_COMMAND_DISALLOWED_ERR_CODE, it means that the Gain Mode in
  *         VCP Renderer side is Automatic Only or Manual Only for the specified Audio Input Control Instance.
  * @note   If the operation causes a Gain Mode change, the VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event will be
  *         received if Audio Input State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_SetAutomaticGainMode(uint16_t ConnHandle,uint8_t Inst,uint8_t ChangeCounter);

/**
  * @brief  Write the Audio Input Description of the Audio Input Control Instance in the remote VCP Renderer
  * @note   If the operation causes an Audio Input Description change, the VCP_CONTROLLER_AUDIO_INPUT_DESCRIPTION_EVT
  *         event will be received if Audio Input Description Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Audio Input Control Instance
  * @param  pDesc: Pointer on the UTF-8 string corresponding to the description of the Audio input
  * @param  Length: length of the description string parameter
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_WriteAudioInputDesc(uint16_t ConnHandle,uint8_t Inst,uint8_t *pDesc,uint8_t Length);

/**
  * @brief  Read the Volume Offset State of the Volume Offset Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_VOLUME_OFFSET_STATE_EVT event will be notified to report the operation result.
  * @note   The VCP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Volume Offset Control Instance
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_ReadVolumeOffsetState(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Read the current Audio Locations of the Volume Offset Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_AUDIO_LOCATION_EVT event will be notified to report the operation result.
  * @note   The VCP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Volume Offset Control Instance
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_ReadAudioLocation(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Read the description of the audio output that the Volume Offset Control Instance describes
  *         on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_AUDIO_OUTPUT_DESCRIPTION_EVT event will be notified to report the operation result.
  * @note   The VCP_CONTROLLER_OPERATION_COMPLETE_EVT event will be notified once the operation is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Volume Offset Control Instance
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_ReadAudioOutputDescription(uint16_t ConnHandle,uint8_t Inst);

/**
  * @brief  Write the Audio Location of the Volume Offset Control Instance in the remote VCP Renderer
  * @note   If the operation causes an Audio Input Description change, the VCP_CONTROLLER_AUDIO_LOCATION_EVT
  *         event will be received if Audio Location Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Volume Offset Control Instance
  * @param  AudioLocation: bitmask that reflects the location of the audio output
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_WriteAudioLocation(uint16_t ConnHandle,uint8_t Inst,Audio_Location_t AudioLocation);

/**
  * @brief  Write the Audio Output Description of the Volume Offset Control Instance in the remote VCP Renderer
  * @note   If the operation causes an Audio Input Description change, the VCP_CONTROLLER_AUDIO_OUTPUT_DESCRIPTION_EVT
  *         event will be received if Audio Output Description Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Volume Offset Control Instance
  * @param  pDesc: Pointer on the UTF-8 string corresponding to the description of the Audio output
  * @param  Length: length of the description string parameter
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_WriteAudioOutputDesc(uint16_t ConnHandle,uint8_t Inst,uint8_t *pDesc,uint8_t Length);

/**
  * @brief  Set the Volume Offset of the Volume Offset Control Instance on the remote VCP Renderer
  * @note   The VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event will be notified once the operation is transmitted.
  *         If the status associated to the event is BLE_STATUS_INVALID_PARAMS, it means that the 'ChangeCounter'
  *         in parameter doesn't match with the Change Counter of the remote VCP Renderer.
  * @note   This command doesn't affect the Mute setting of the AUdio Input Control Instance in the VCP Renderer.
  * @note   If the operation causes a Volume Offset change, the VCP_CONTROLLER_VOLUME_OFFSET_STATE_EVT event will be
  *         received if Volume Offset State Update Notification is enabled.
  * @param  ConnHandle: ACL connection handle
  * @param  Inst : Volume Offset Control Instance
  * @param  VolOffset: Volume Offset in range [-255;255]
  * @param  ChangeCounter: Change Counter value which shall match with the Change Counter in VCP Renderer returned in
  *                        VCP_CONTROLLER_VOLUME_OFFSET_STATE_EVT event.
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_SetVolumeOffset(uint16_t ConnHandle,uint8_t Inst,int16_t VolOffset,uint8_t ChangeCounter);

/**
  * @brief  Remove the record of the Volume Control Profile Controller stored in the Non Volatile memory.
  * @param  PeerIdentityAddressType: Identity address type.
  *                                  Values:
  *                                     - 0x00: Public Identity Address
  *                                     - 0x01: Random (static) Identity Address
  * @param  PeerIdentityAddress : Public or Random (static) Identity address of the peer device
  * @retval status of the operation
  */
tBleStatus VCP_CONTROLLER_RemoveRecord(uint8_t PeerIdentityAddressType,const uint8_t PeerIdentityAddress[6]);

#ifdef __cplusplus
}
#endif

#endif /* __VCP_H */
