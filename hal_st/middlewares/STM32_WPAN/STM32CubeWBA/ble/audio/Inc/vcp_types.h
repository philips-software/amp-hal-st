/**
  ******************************************************************************
  * @file    vcp_types.h
  * @author  MCD Application Team
  * @brief   This file contains types used for Volume Control Profile
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
#ifndef __VCP_TYPES_H
#define __VCP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_types.h"
#include "audio_types.h"

/* Defines -------------------------------------------------------------------*/

/* #############################################################################
   #       Defines and MACRO used to allocate GATT Services, GATT Attributes   #
   #       and storage area for Attribute values for Volume Control Profile    #
   #       in Renderer role                                                    #
   ############################################################################# */

/*
 * VCP_RENDERER_NUM_GATT_SERVICES: number of GATT services required for Volume Control Profile in Renderer role .
 */
#define VCP_RENDERER_NUM_GATT_SERVICES(num_aic_instances,num_voc_instances) \
                                        (1u + num_aic_instances + num_voc_instances)

/*
 * VCP_RENDERER_NUM_GATT_ATTRIBUTES: minimum number of GATT attributes required for Volume Control Profile
 * in Renderer role registration.
 *
 * @param num_aic_instances: Maximum number of supported Audio Input Control Instances.
 *
 * @param num_voc_instances: Maximum number of supported Volume Offset Control Instances.
 */
#define VCP_RENDERER_NUM_GATT_ATTRIBUTES(num_aic_instances,num_voc_instances) \
                                        (8u + (16u * num_aic_instances) + (12u * num_voc_instances))

/*
 * VCP_RENDERER_ATT_VALUE_ARRAY_SIZE: this macro returns the size of the storage area for Attribute values
 * in VCP Renderer.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *
 * @param num_aic_instances: Maximum number of supported Audio Input Control Instances.
 *
 * @param num_voc_instances: Maximum number of supported Volume Offset Control Instances.
 *
 * @param max_aic_description_length: Maximum length of the Audio Input Description.
 *
 * @param max_voc_description_length: Maximum length of the Audio Output Description.
 */
#define VCP_RENDERER_ATT_VALUE_ARRAY_SIZE(num_ble_links,num_aic_instances,num_voc_instances, \
                                          max_aic_description_length,max_voc_description_length) \
                                          (22u + (4u*num_ble_links) \
                                           + (num_aic_instances * (48u + max_aic_description_length + (6u*num_ble_links))) \
                                           + (num_voc_instances * (37u + max_voc_description_length + (6u*num_ble_links))))

/* #############################################################################
   #       Defines and MACRO used to allocate memory resource of               #
   #       Volume Control Profile in VCP_Config_t                              #
   #   (BLE_VCP_CTLR_TOTAL_BUFFER_SIZE, BLE_VCP_RENDERER_TOTAL_BUFFER_SIZE)    #
   ############################################################################# */

/*
 * VCP_CTLR_MEM_PER_CONN_SIZE_BYTES: memory size used per link in VCP Controller role
 */
#define VCP_CTLR_MEM_PER_CONN_SIZE_BYTES                        (76u)

/*
 * VCP_CTLR_MEM_PER_CONN_SIZE_BYTES: memory size used per Audio Input Control Instance in VCP Controller role
 */
#define VCP_CTLR_MEM_PER_AIC_INST_SIZE_BYTES                    (54u)

/*
 * VCP_CTLR_MEM_PER_VOC_INST_SIZE_BYTES: memory size used per Volume Offset Control Instance in VCP Controller role
 */
#define VCP_CTLR_MEM_PER_VOC_INST_SIZE_BYTES                    (36u)

/*
 * BLE_VCP_CTLR_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes,needed for
 * the storage of data structures in VCP Controller whose size depends on the number of supported
 * connections and the number of supported Audio Output Control Instances and Volume Offset Control Instances.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *                      Valid values are from 1 to 8.
 *
 * @param num_aic_instances: Maximum number of supported Audio Input Control Instances per connection.
 *
 * @param num_voc_instances: Maximum number of supported Volume Offset Control Instances per connection.
 */
#define BLE_VCP_CTLR_TOTAL_BUFFER_SIZE(num_ble_links,num_aic_instances,num_voc_instances) \
          (8u + (VCP_CTLR_MEM_PER_AIC_INST_SIZE_BYTES * num_ble_links * num_aic_instances) + \
           (VCP_CTLR_MEM_PER_VOC_INST_SIZE_BYTES * num_ble_links * num_voc_instances) + \
           (VCP_CTLR_MEM_PER_CONN_SIZE_BYTES * num_ble_links))


/*
 * VCP_RENDERER_MEM_CONTEXT_SIZE_BYTES: memory size used for Context  in VCP Render role
 */
#define VCP_RENDERER_MEM_CONTEXT_SIZE_BYTES                     (36u)

/*
 * VCP_RENDERER_MEM_PER_AIC_INST_SIZE_BYTES: memory size used per Audio Input Control Instance in VCP Renderer role
 */
#define VCP_RENDERER_MEM_PER_AIC_INST_SIZE_BYTES                (36u)

/*
 * VCP_RENDERER_MEM_PER_VOC_INST_SIZE_BYTES: memory size used per Audio Output Control Instance in VCP Renderer role
 */
#define VCP_RENDERER_MEM_PER_VOC_INST_SIZE_BYTES                (36u)

/*
 * BLE_VCP_RENDERER_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes,needed for
 * the storage of data structures in VCP Renderer whose size depends on the number of supported
 * connections and the number of supported Audio Output Control Instances and Volume Offset Control Instances.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *                      Valid values are from 1 to 8.
 *
 * @param num_aic_instances: Maximum number of supported Audio Input Control Instances.
 *
 * @param num_voc_instances: Maximum number of supported Volume Offset Control Instances.
 */
#define BLE_VCP_RENDERER_TOTAL_BUFFER_SIZE(num_ble_links,num_aic_instances,num_voc_instances) \
          (VCP_RENDERER_MEM_CONTEXT_SIZE_BYTES \
            + (VCP_RENDERER_MEM_PER_AIC_INST_SIZE_BYTES * num_aic_instances) \
            + (VCP_RENDERER_MEM_PER_VOC_INST_SIZE_BYTES * num_voc_instances))

/* Types ---------------------------------------------------------------------*/

/* Types of mask for roles of the Volume Control Profile */
typedef uint8_t VCP_Role_t;
#define VCP_ROLE_RENDERER                               (0x01)
#define VCP_ROLE_CONTROLLER                             (0x02)

/* Types of Gain Mode for Audio Input Control representing the VCP Renderer’s gain adjustment functionality.*/
typedef uint8_t VCP_AICModeGain_t;
#define VCP_AIC_GAIN_MODE_MANUAL_ONLY                   (0x00u)
#define VCP_AIC_GAIN_MODE_AUTO_ONLY                     (0x01u)
#define VCP_AIC_GAIN_MODE_MANUAL                        (0x02u)
#define VCP_AIC_GAIN_MODE_AUTO                          (0x03u)

/* Types of Mute State for Audio Input Control */
typedef uint8_t VCP_AICMute_t;
#define VCP_AIC_UNMUTE                                  (0x00u)
#define VCP_AIC_MUTE                                    (0x01u)
#define VCP_AIC_MUTE_DISABLED                           (0x02u)

/* Status of the audio input*/
typedef uint8_t VCP_AudioInputStatus_t;
#define VCP_AUDIO_INPUT_INACTIVE                        (0x00u)
#define VCP_AUDIO_INPUT_ACTIVE                          (0x01u)

/*Volume Control Opcode Type*/
typedef uint8_t VCP_CtrlOpcode_t;
#define VCP_CTRL_REL_VOL_DOWN                           (0x01u) /* Reduce the value of the Volume Setting */
#define VCP_CTRL_REL_VOL_UP                             (0x02u) /* Increase the value of the Volume Setting. */
#define VCP_CTRL_UNMUTE_REL_VOL_DOWN                    (0x03u) /* Reduce the value of the Volume Setting and Unmute. */
#define VCP_CTRL_UNMUTE_REL_VOL_UP                      (0x04u) /* Increase the value of the Volume Setting and Unmute. */
#define VCP_CTRL_SET_ABS_VOL                            (0x05u) /* set the Volume_Setting field value to the Volume Setting
                                                                 * operand
                                                                 */
#define VCP_CTRL_UNMUTE                                 (0x06)  /* Unmute*/
#define VCP_CTRL_MUTE                                   (0x07u) /* Mute*/
#define VCP_AICS_CTRL_SET_GAIN_SETTING                  (0x08u) /* Audio Input Control : Set the value of the Gain Setting */
#define VCP_AICS_CTRL_UNMUTE                            (0x09u) /* Audio Input Control : Unmute */
#define VCP_AICS_CTRL_MUTE                              (0x0Au) /* Audio Input Control : Mute */
#define VCP_AICS_CTRL_SET_GAIN_MODE_MANUAL              (0x0Bu) /* Audio Input Control : Set Gain Mode in Manual */
#define VCP_AICS_CTRL_SET_GAIN_MODE_AUTOMATIC           (0x0Cu) /* Audio Input Control : Set Gain Mode in Automatic */
#define VCP_VOCS_CTRL_SET_VOLUME_OFFSET                 (0x0Du) /* Volume Offset Control : Set the value of the Volume Offset */
#define VCP_CTRL_INVALID                                (0xFFu) /* Invalid Operation type*/

/*Volume Operation Type*/
typedef uint8_t VCP_Opcode_t;
#define VCP_CONFIG_VOLUME_STATE_NOTIF                   (0x01u) /* Operation issued from VCP_CONTROLLER_ConfigureVolumeStateNotification() */
#define VCP_CONFIG_VOLUME_FLAGS_NOTIF                   (0x02u) /* Operation issued from VCP_CONTROLLER_ConfigureVolumeFlagsNotification() */
#define VCP_READ_VOLUME_STATE                           (0x03u) /* Operation issued from VCP_CONTROLLER_ReadVolumeState() */
#define VCP_READ_VOLUME_FLAGS                           (0x04u) /* Operation issued from VCP_CONTROLLER_ReadVolumeFlags() */
#define VCP_CONFIG_AUDIO_INPUT_STATE_NOTIF              (0x05u) /* Operation issued from VCP_CONTROLLER_ConfigureAudioInputStateNotification() */
#define VCP_CONFIG_AUDIO_INPUT_STATUS_NOTIF             (0x06u) /* Operation issued from VCP_CONTROLLER_ConfigureAudioInputStatusNotification() */
#define VCP_CONFIG_AUDIO_INPUT_DESCRIPTION_NOTIF        (0x07u) /* Operation issued from VCP_CONTROLLER_ConfigureAudioInputDescNotification() */
#define VCP_READ_AUDIO_INPUT_STATE                      (0x08u) /* Operation issued from VCP_CONTROLLER_ReadAudioInputState() */
#define VCP_READ_GAIN_SETTING_PROPERTIES                (0x09u) /* Operation issued from VCP_CONTROLLER_ReadGainSettingProperties() */
#define VCP_READ_AUDIO_INPUT_STATUS                     (0x0Au) /* Operation issued from VCP_CONTROLLER_ReadAudioInputStatus() */
#define VCP_READ_AUDIO_INPUT_DESCRIPTION                (0x0Bu) /* Operation issued from VCP_CONTROLLER_ReadAudioInputDescription() */
#define VCP_CONFIG_VOLUME_OFFSET_STATE_NOTIF            (0x0Cu) /* Operation issued from VCP_CONTROLLER_ConfigureVolumeOffsetStateNotification() */
#define VCP_CONFIG_AUDIO_LOCATION_NOTIF                 (0x0Du) /* Operation issued from VCP_CONTROLLER_ConfigureAudioLocationNotification() */
#define VCP_CONFIG_AUDIO_OUTPUT_DESCRIPTION_NOTIF       (0x0Eu) /* Operation issued from VCP_CONTROLLER_ConfigureAudioOutputDescNotification() */
#define VCP_READ_VOLUME_OFFSET_STATE                    (0x0Fu) /* Operation issued from VCP_CONTROLLER_ReadVolumeOffsetState() */
#define VCP_READ_AUDIO_LOCATION                         (0x10u) /* Operation issued from VCP_CONTROLLER_ReadAudioLocation() */
#define VCP_READ_AUDIO_OUTPUT_DESCRIPTION               (0x11u) /* Operation issued from VCP_CONTROLLER_ReadAudioOutputDescription() */

/* Types the Volume Control Events */
typedef enum
{
  VCP_RENDERER_UPDATED_VOLUME_STATE_EVT,        /* This event is notified by VCP Renderer when Volume State has changed
                                                 * locally.
                                                 * The pInfo field indicates the current Volume information
                                                 * through the VCP_VolumeState_Evt_t type.
                                                 */
  VCP_RENDERER_UPDATED_AUDIO_INPUT_STATE_EVT,   /* This event is notified by VCP Renderer when an Audio Input State
                                                 * of an Audio Input Instance has changed locally from a remote
                                                 * VCP Controller request or from local request.
                                                 * pInfo field indicates the Audio Input State information through the
                                                 * VCP_AudioInputState_Evt_t type.
                                                 */
  VCP_RENDERER_UPDATED_AUDIO_INPUT_DESCRIPTION_EVT,/* This event is notified by VCP Render when a Description
                                                 * of an Audio Input Instance has changed locally from a remote
                                                 * VCP Controller request only.
                                                 * pInfo field indicates the Audio Input Description information
                                                 * through the VCP_AudioDescription_Evt_t type.
                                                 */
  VCP_RENDERER_UPDATED_VOLUME_OFFSET_STATE_EVT, /* This event is notified by VCP Renderer when an Volume Offset State
                                                 * of an Audio Output Instance has changed locally from a remote
                                                 * VCP Controller request or from local request.
                                                 * pInfo field indicates the Volume Offset State information through the
                                                 * VCP_VolumeOffsetState_Evt_t type.
                                                 */
  VCP_RENDERER_UPDATED_AUDIO_OUTPUT_LOCATION_EVT,/* This event is notified by VCP Renderer when the Audio Location
                                                 * of an Audio Output Instance has changed locally from a remote
                                                 * VCP Controller request only.
                                                 * pInfo field indicates the Audio Location through the
                                                 * VCP_AudioLocation_Evt_t type.
                                                 */
  VCP_RENDERER_UPDATED_AUDIO_OUTPUT_DESCRIPTION_EVT,/* This event is notified by VCP Render when a Description
                                                 * of an Audio Output Instance has changed locally from a remote
                                                 * VCP Controller request only.
                                                 * pInfo field indicates the Audio Output Description information
                                                 * through the VCP_AudioDescription_Evt_t type.
                                                 */
  VCP_CONTROLLER_OPERATION_COMPLETE_EVT,        /* This event is notified when a VCP Controller Operation is complete.
                                                 * pInfo field indicates the Complete Operation to the remote
                                                 * Volume Control Renderer through the VCP_CLT_OpComplete_Evt_t
                                                 * type.
                                                 * After receiving this event, a new VCP Controller operation could
                                                 * be started.
                                                 */
  VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT,/* This event is notified when a VCP Controller Control Operation is
                                                 * transmitted.
                                                 * pInfo field indicates the transmitted Volume Control Operation to the
                                                 * remote Volume Control Renderer through the VCP_CLT_CtrlOpTransmitted_Evt_t
                                                 * type.
                                                 * After receiving this event, a new VCP Controller operation could
                                                 * be started.
                                                 */
  VCP_CONTROLLER_VOLUME_STATE_EVT,              /* This event is notified by VCP Controller when a Volume State is read
                                                 * thanks to the VCP_CONTROLLER_ReadVolumeState() function.
                                                 * pInfo field indicates the Volume information through the
                                                 * VCP_VolumeState_Evt_t type.
                                                 */
  VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT,      /* This event is notified by VCP Controller when a Volume State change
                                                 * is notified from VCP Renderer side (VCP Controller
                                                 *  has subscribed to Volume State Change notification).
                                                 * pInfo field indicates the Volume information through the
                                                 * VCP_VolumeState_Evt_t type.
                                                 */
  VCP_CONTROLLER_VOLUME_FLAGS_EVT,              /* This event is notified by VCP Controller when a Volume Flags is read
                                                 * thanks to the VCP_CTLR_ReadVolumeFlags() function.
                                                 * pInfo field indicates the Volume Flags through the
                                                 * VCP_VolumeFlags_Evt_t type.
                                                 */
  VCP_CONTROLLER_UPDATED_VOLUME_FLAGS_EVT,      /* This event is notified by VCP Controller when a Volume Flags change
                                                 * is notified from VCP Renderer side (VCP Controller has subscribed
                                                 * to Volume Flags Change notification).
                                                 * pInfo field indicates the Volume Flags through the
                                                 * VCP_VolumeFlags_Evt_t type.
                                                 */
  VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT,         /* This event is notified by VCP Controller when an Audio Input State is
                                                 * read thanks to the VCP_CONTROLLER_ReadAudioInputState() function
                                                 * or if the Audio Input State has changed in VCP renderer side
                                                 * (VCP Controller has subscribed to Audio Input State Change
                                                 * notification).
                                                 * pInfo field indicates the Audio Input State information through the
                                                 * VCP_AudioInputState_Evt_t type.
                                                 */
  VCP_CONTROLLER_GAIN_SETTING_PROP_EVT,         /* This event is notified by VCP Controller when a Gain Setting
                                                 * Properties are read thanks to the function
                                                 * VCP_CONTROLLER_ReadGainSettingProperties()  or if the Gain Setting
                                                 * Properties have changed in VCP renderer side (VCP Controller has
                                                 * subscribed to Gain Setting Properties Change notification).
                                                 * pInfo field indicates the Gain Setting Properties through the
                                                 * VCP_GainSettingProp_Evt_t type.
                                                 */
  VCP_CONTROLLER_AUDIO_INPUT_STATUS_EVT,        /* This event is notified by VCP Controller when an Audio Input Status
                                                 * is read thanks to the VCP_CONTROLLER_ReadAudioInputStatus() function
                                                 * or if the Audio Input Status has changed in VCP renderer side
                                                 * (VCP Controller has subscribed to Audio Input Status Change
                                                 * notification).
                                                 * pInfo field indicates the Audio Input Status information through the
                                                 * VCP_AudioInputStatus_Evt_t type.
                                                 */
  VCP_CONTROLLER_AUDIO_INPUT_DESCRIPTION_EVT,   /* This event is notified by VCP Controller when an Audio Input
                                                 * Description is read thanks to the function
                                                 * VCP_CONTROLLER_ReadAudioInputDescription() function or if the
                                                 * Audio Input Description has changed in VCP renderer side
                                                 * (VCP Controller has subscribed to Audio Input Description Change
                                                 * notification).
                                                 * pInfo field indicates the Audio Input Description information
                                                 * through the VCP_AudioInputDesc_Evt_t type.
                                                 */
  VCP_CONTROLLER_VOLUME_OFFSET_STATE_EVT,       /* This event is notified by VCP Controller when a Volume Offset State
                                                 * is read thanks to the VCP_CONTROLLER_ReadVolumeOffsetState() function
                                                 * or if the Volume Offset State has changed in VCP renderer side
                                                 * (VCP Controller has subscribed to Volume Offset State Change
                                                 * notification).
                                                 * pInfo field indicates the Volume Offset State information through the
                                                 * VCP_VolumeOffsetState_Evt_t type.
                                                 */
  VCP_CONTROLLER_AUDIO_OUTPUT_LOCATION_EVT,     /* This event is notified by VCP Controller when the Audio Location
                                                 * is read thanks to the VCP_CONTROLLER_ReadAudioLocation() function
                                                 * or if the Audio Location has changed in VCP renderer side
                                                 * (VCP Controller has subscribed to Audio Location Change
                                                 * notification).
                                                 * pInfo field indicates the Audio Location through the
                                                 * VCP_AudioLocation_Evt_t type.
                                                 */
  VCP_CONTROLLER_AUDIO_OUTPUT_DESCRIPTION_EVT,  /* This event is notified by VCP Controller when an Audio Output
                                                 * Description is read thanks to the function
                                                 * VCP_CONTROLLER_ReadAudioOutputDescription() function or if the
                                                 * Audio Output Description has changed in VCP renderer side
                                                 * (VCP Controller has subscribed to Audio Output Description Change
                                                 * notification).
                                                 * pInfo field indicates the Audio Output Description information
                                                 * through the VCP_AudioOutputDesc_Evt_t type.
                                                 */

} VCP_NotCode_t;

/* Structure of VOC instance*/
typedef struct
{
  uint8_t               ID;
  uint16_t              StartAttHandle; /* ATT Start Handle of the VOC Instance in the remote VCP Renderer */
  uint16_t              EndAttHandle;   /* ATT End Handle of the VOC Instance in the remote VCP Renderer */
} VCP_VOCInstInfo_t;

/* Structure of AIC Instance*/
typedef struct
{
  uint8_t               ID;
  Audio_Input_t         AudioInputType;
  uint16_t              StartAttHandle; /* ATT Start Handle of the AIC Instance in the remote VCP Renderer */
  uint16_t              EndAttHandle;   /* ATT End Handle of the AIC Instance in the remote VCP Renderer */
} VCP_AICInstInfo_t;

typedef struct
{
  uint8_t               NumAICInst;     /*Number of discovered AIC Instances*/
  VCP_AICInstInfo_t     *pAICInst;      /*Pointer on discovered AIC Instance table*/
  uint8_t               NumVOCInst;     /*Number of discovered VOC Instances*/
  VCP_VOCInstInfo_t     *pVOCInst;      /*Pointer on discovered VOC Instance table*/
  uint16_t              StartAttHandle; /* ATT Start Handle of Volume Control Service in the remote MVCP Renderer */
  uint16_t              EndAttHandle;   /* ATT End Handle of the Volume Control Service in the remote VCP Renderer */
} VCP_Info_t;

/* Audio Input State Structure  */
typedef struct
{
  int8_t                GainSetting;    /* Gain of the audio input signal (in dB) to which the AIC Instance
                                         * in VCP Renderer applies.
                                         * This is a value for which a single increment or decrement results in a
                                         * corresponding increase or decrease of the input amplitude by the value of
                                         * the Gain Setting Units value specified in the Gain Setting Properties.
                                         * A Gain Setting value of 0 should result in no change to the input’s original
                                         * amplitude.
                                         */
  VCP_AICMute_t         Mute;           /* Mute State */
  VCP_AICModeGain_t     GainMode;       /* Gain Mode of the AIC Instance in VCP Renderer */
} VCP_AudioInputState_t;

/* Gain Setting Properties Structure  */
typedef struct
{
  uint8_t       GainSettingUnits;       /* Size of a single increment or decrement of the Gain Setting value
                                         * in 0.1 decibel units.
                                         */
  int8_t        GainSettingMin;         /* Minimum allowable value of the Gain Setting value */
  int8_t        GainSettingMax;         /* Maximum allowable value of the Gain Setting value */
} VCP_GainSettingProp_t;


/* Structure used in parameter when VCP_RENDERER_UPDATED_VOLUME_STATE_EVT and VCP_CONTROLLER_VOLUME_STATE_EVT events
 * are notified
*/
typedef struct
{
  uint8_t       VolSetting;     /* Unitless value such that a step increase in its value should result in a step
                                 * increase of the audio output volume and a step decrease in its value should result
                                 * in a step decrease of the audio output volume.
                                 * A value of 0 is the minimum volume output and a value of 255 is the maximum
                                 * volume output.
                                 */
  uint8_t       Mute;           /* 0 : Not Muted
                                 * 1 : Muted
                                 */
  uint8_t       ChangeCounter;  /*This value is incremented by 1 when the Volume Setting or Mute Setting changes*/
} VCP_VolumeState_Evt_t;


/* Structure used in parameter when VCP_CONTROLLER_VOLUME_FLAGS_EVT event is notified*/
typedef struct
{
  uint8_t       VolFlags;
} VCP_VolumeFlags_Evt_t;


/* Type associated to VCP_CONTROLLER_OPERATION_COMPLETE_EVT event*/
typedef struct
{
  VCP_Opcode_t          Op;     /* VCP Controller Operation opcode */
  uint8_t               Inst;   /* Instance ID if Operation is an AICS or VOCS opetaion */
}VCP_CLT_OpComplete_Evt_t;

/* Type associated to VCP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event*/
typedef struct
{
  VCP_CtrlOpcode_t      CtrlOp; /* Control Operation opcode */
  uint8_t               Inst;   /* Instance ID if Control Operation is an AICS or VOCS control opetaion */
}VCP_CLT_CtrlOpTransmitted_Evt_t;


/* Structure used in parameter when VCP_CONTROLLER_AUDIO_INPUT_STATE_EVT event is notified */
typedef struct
{
  uint8_t               AICInst;        /* Audio Input Control Instance Identifier*/
  VCP_AudioInputState_t State;          /* Audio Input State */
  uint8_t               ChangeCounter;  /* This value is incremented by 1 when one of the other field change */
} VCP_AudioInputState_Evt_t;

/* Structure used in parameter when VCP_CONTROLLER_GAIN_SETTING_PROP_EVT event is notified */
typedef struct
{
  uint8_t               AICInst;        /* Audio Input Control Instance Identifier*/
  VCP_GainSettingProp_t Prop;           /* Gain Setting Properties */
} VCP_GainSettingProp_Evt_t;

/* Structure used in parameter when VCP_CONTROLLER_AUDIO_INPUT_STATUS_EVT event is notified */
typedef struct
{
  uint8_t       AICInst;                /* Audio Input Control Instance Identifier*/
  uint8_t       Status;                 /* Current state of the audio Input :
                                         * 0 : Inactive
                                         * 1 : Active
                                         */
} VCP_AudioInputStatus_Evt_t;

/* Structure used in parameter when VCP_CONTROLLER_AUDIO_INPUT_DESCRIPTION_EVT and
 * VCP_CONTROLLER_AUDIO_OUTPUT_DESCRIPTION_EVT events are notified */
typedef struct
{
  uint8_t       Inst;           /* Audio Input Device or Audio Output Device Instance Identifier*/
  uint8_t       *pData;         /* Pointer on String of description of the audio input*/
  uint8_t       DataLength;     /* Length of the pData field*/
} VCP_AudioDescription_Evt_t;

/* Structure used in parameter when VCP_CONTROLLER_VOLUME_OFFSET_STATE_EVT event is notified */
typedef struct
{
  uint8_t               VOCInst;        /* Volume Offset Control Instance Identifier*/
  int16_t               VolumeOffset;   /* current offset that is applied to the volume of the audio output
                                         * to which the VOC Instance in VCP Renderer applies.
                                         */
  uint8_t               ChangeCounter;  /* This value is incremented by 1 when one of the other field change */
} VCP_VolumeOffsetState_Evt_t;

/* Structure used in parameter when VCP_CONTROLLER_AUDIO_OUTPUT_LOCATION_EVT event or
 * VCP_RENDERER_AUDIO_OUTPUT_LOCATION_EVT are notified */
typedef struct
{
  uint8_t               VOCInst;        /* Volume Offset Control Instance Identifier*/
  Audio_Location_t      AudioLocation;  /* bitmask that reflects the location of the audio output
                                         * to which the VOC Instance in VCP Renderer applies.
                                         */
} VCP_AudioLocation_Evt_t;

typedef struct
{
  VCP_NotCode_t EvtOpcode;      /* Opcode of the notified event */
  tBleStatus    Status;         /* Status associated to the event */
  uint16_t      ConnHandle;     /* ACL Connection Handle associated to the event */
  uint8_t       *pInfo;         /* Pointer on information associated to the event */
}VCP_Notification_Evt_t;

typedef struct
{
  uint8_t               InstID;                 /* Instance Identifier */
  uint8_t               Status;                 /* Current state of the audio Input instance :
                                                 * 0 : Inactive
                                                 * 1 : Active
                                                 */
  VCP_AudioInputState_t State;                  /* Audio Input State of the audio input Instance */
  VCP_GainSettingProp_t Prop;                   /* Gain Setting Properties of the audio input Instance  */
  Audio_Input_t         AudioInputType;         /* Audio Input Type of the audio input Instance  */
  uint8_t               MaxDescriptionLength;   /* Maximum Size of the Audio input Description string */
  uint8_t               *pDescription;          /* Pointer on Description string of the audio input Instance  */
  uint8_t               DescriptionLength;      /* Length of the Description string */
}VCP_AIC_InitInst_t;

typedef struct
{
  uint8_t               InstID;                 /* Instance Identifier */
  int16_t               VolumeOffset;           /* Volume Offset State of the audio output Instance*/
  Audio_Location_t      AudioLocation;          /* bitmask value that reflects the location of the audio output Instance*/
  uint8_t               MaxDescriptionLength;   /* Maximum Size of the Audio input Description string */
  uint8_t               *pDescription;          /* Pointer on Description string of the audio output Instance */
  uint8_t               DescriptionLength;      /* Length of the Description string */
}VCP_VOC_InitInst_t;

/* Volume Control Renderer Configuration */
typedef struct
{
  uint8_t               InitialVolumeSetting;   /* Initial Volume value (min 0 ; max 255) */
  uint8_t               InitialMuteState;       /* Initial Mute State (0: Not Muted ; 1: Muted)*/
  uint8_t               VolumeStepSize;         /* Step Size of the volume setting change */
  uint8_t               NumAICInst;             /* Number of Audio Input Control Instances */
  VCP_AIC_InitInst_t    *pAICInst;              /* Audio Input Control Instance Init Config */
  uint8_t               NumVOCInst;             /* Number of Volume Offset Control Instances */
  VCP_VOC_InitInst_t    *pVOCInst;              /* Volume Offset Control Instance Init Config */
  uint8_t               *pStartRamAddr;         /* Start address of the RAM buffer allocated for VCP Renderer
                                                 * Context memory resource.
                                                 * It must be a 32bit aligned RAM area.
                                                 */
  uint16_t              RamSize;                /* Size of the RAM allocated at pStartRamAddr
                                                 * pointer.
                                                 */
} VCP_SRV_Config_t;

/* Volume Control Controller Configuration */
typedef struct
{

  uint8_t               MaxNumAICInstPerConn;   /* Maximum Number of Audio Input Control Instances */
  uint8_t               MaxNumVOCInstPerConn;   /* Maximum Number of Volume Offset Control Instances */
  uint8_t               *pStartRamAddr;         /* Start address of the RAM buffer allocated for VCP Controller
                                                 * Context memory resource.
                                                 * It must be a 32bit aligned RAM area.
                                                 */
  uint16_t              RamSize;                /* Size of the RAM allocated at pStartRamAddr
                                                 * pointer.
                                                 */

} VCP_CLT_Config_t;

/* Volume Control Profile Configuration */
typedef struct
{
  VCP_Role_t            Role;                   /* VCP Role Mask */
  uint8_t               MaxNumBleLinks;         /* Maximum number of BLE Links */
  VCP_SRV_Config_t      Renderer;               /* Volume Control Renderer Configuration */
  VCP_CLT_Config_t      Controller;             /* Volume Control Controller Configuration */
} VCP_Config_t;

#ifdef __cplusplus
}
#endif

#endif /* __VCP_TYPES_H */
