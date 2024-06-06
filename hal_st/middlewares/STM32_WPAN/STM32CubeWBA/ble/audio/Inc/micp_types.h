/**
  ******************************************************************************
  * @file    micp_types.h
  * @author  MCD Application Team
  * @brief   This file contains types used for Microphone Control Profile
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
#ifndef __MICP_TYPES_H
#define __MICP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_types.h"
#include "audio_types.h"

/* Defines -------------------------------------------------------------------*/

/* #############################################################################
   #       Defines and MACRO used to allocate GATT Services, GATT Attributes   #
   #       and storage area for Attribute values for Microphone Control Profile    #
   #       in Device role                                                    #
   ############################################################################# */

/*
 * MICP_DEVICE_NUM_GATT_SERVICES: number of GATT services required for Microphone Control Profile in Device role .
 */
#define MICP_DEVICE_NUM_GATT_SERVICES(num_aic_instances) (1u + num_aic_instances)

/*
 * MICP_DEVICE_NUM_GATT_ATTRIBUTES: minimum number of GATT attributes required for Microphone Control Profile
 * in Device role registration.
 *
 * @param num_aic_instances: Maximum number of supported Audio Input Control Instances.
 */
#define MICP_DEVICE_NUM_GATT_ATTRIBUTES(num_aic_instances) (3u + (16u * num_aic_instances))

/*
 * MICP_DEVICE_ATT_VALUE_ARRAY_SIZE: this macro returns the size of the storage area for Attribute values
 * in MICP Device.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *
 * @param num_aic_instances: Maximum number of supported Audio Input Control Instances.
 *
 * @param max_aic_description_length: Maximum length of the Audio Input Description.
 */
#define MICP_DEVICE_ATT_VALUE_ARRAY_SIZE(num_ble_links, num_aic_instances, max_aic_description_length) \
                                          (6u + (2u*num_ble_links) \
                                            + (num_aic_instances * (48u + max_aic_description_length + (6u*num_ble_links))))

/* #############################################################################
   #       Defines and MACRO used to allocate memory resource of               #
   #       Microphone Control Profile in MICP_Config_t                         #
   #   (BLE_MICP_CTLR_TOTAL_BUFFER_SIZE, BLE_MICP_DEVICE_TOTAL_BUFFER_SIZE)    #
   ############################################################################# */

/*
 * MICP_CTLR_MEM_PER_CONN_SIZE_BYTES: memory size used per link in MICP Controller role
 */
#define MICP_CTLR_MEM_PER_CONN_SIZE_BYTES                        (48u)

/*
 * MICP_CTLR_MEM_PER_CONN_SIZE_BYTES: memory size used per Audio Input Control Instance in MICP Controller role
 */
#define MICP_CTLR_MEM_PER_AIC_INST_SIZE_BYTES                    (54u)

/*
 * BLE_MICP_CTLR_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes,needed for
 * the storage of data structures in MICP Controller whose size depends on the number of supported
 * connections and the number of supported Audio Output Control Instances.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *                      Valid values are from 1 to 8.
 *
 * @param num_aic_instances: Maximum number of supported Audio Input Control Instances per connection.
 */
#define BLE_MICP_CTLR_TOTAL_BUFFER_SIZE(num_ble_links,num_aic_instances) \
          (8u + (MICP_CTLR_MEM_PER_AIC_INST_SIZE_BYTES * num_ble_links * num_aic_instances) + \
           (MICP_CTLR_MEM_PER_CONN_SIZE_BYTES * num_ble_links))


/*
 * MICP_DEVICE_MEM_CONTEXT_SIZE_BYTES: memory size used for Context  in MICP Render role
 */
#define MICP_DEVICE_MEM_CONTEXT_SIZE_BYTES                     (36u)

/*
 * MICP_DEVICE_MEM_PER_AIC_INST_SIZE_BYTES: memory size used per Audio Input Control Instance in MICP Device role
 */
#define MICP_DEVICE_MEM_PER_AIC_INST_SIZE_BYTES                (36u)

/*
 * BLE_MICP_DEVICE_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes,needed for
 * the storage of data structures in MICP Device whose size depends on the number of supported
 * connections and the number of supported Audio Output Control Instances
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *                      Valid values are from 1 to 8.
 *
 * @param num_aic_instances: Maximum number of supported Audio Input Control Instances.
 */
#define BLE_MICP_DEVICE_TOTAL_BUFFER_SIZE(num_ble_links,num_aic_instances) \
          (MICP_DEVICE_MEM_CONTEXT_SIZE_BYTES \
            + (MICP_DEVICE_MEM_PER_AIC_INST_SIZE_BYTES * num_aic_instances))

/* Types ---------------------------------------------------------------------*/

/* Types of mask for roles of the Microphone Control Profile */
typedef uint8_t MICP_Role_t;
#define MICP_ROLE_DEVICE                                 (0x01)
#define MICP_ROLE_CONTROLLER                             (0x02)

/* Microphone Mute Characteristic Values*/
typedef uint8_t Microphone_Mute_t;
#define MICROPHONE_MUTE_NOT_MUTED                        (0x00)  /* Microphone is unmuted */
#define MICROPHONE_MUTE_MUTED                            (0x01)  /* Microphone is muted */
#define MICROPHONE_MUTE_DISABLED                         (0x02)  /* Mute command are disabled and microphone is muted*/


/* Types of Gain Mode for Audio Input Control representing the MICP Renderer’s gain adjustment functionality.*/
typedef uint8_t MICP_AICModeGain_t;
#define MICP_AIC_GAIN_MODE_MANUAL_ONLY                   (0x00u)
#define MICP_AIC_GAIN_MODE_AUTO_ONLY                     (0x01u)
#define MICP_AIC_GAIN_MODE_MANUAL                        (0x02u)
#define MICP_AIC_GAIN_MODE_AUTO                          (0x03u)

/* Types of Mute State for Audio Input Control */
typedef uint8_t MICP_AICMute_t;
#define MICP_AIC_UNMUTE                                  (0x00u)
#define MICP_AIC_MUTE                                    (0x01u)
#define MICP_AIC_MUTE_DISABLED                           (0x02u)

/* Status of the audio input*/
typedef uint8_t MICP_AudioInputStatus_t;
#define MICP_AUDIO_INPUT_INACTIVE                        (0x00u)
#define MICP_AUDIO_INPUT_ACTIVE                          (0x01u)

/*Microphone Control Opcode Type*/
typedef uint8_t MICP_CtrlOpcode_t;
#define MICP_CTRL_UNMUTE                                 (0x01u) /* Unmute*/
#define MICP_CTRL_MUTE                                   (0x02u) /* Mute*/
#define MICP_AICS_CTRL_SET_GAIN_SETTING                  (0x03u) /* Audio Input Control : Set the value of the Gain Setting */
#define MICP_AICS_CTRL_UNMUTE                            (0x04u) /* Audio Input Control : Unmute */
#define MICP_AICS_CTRL_MUTE                              (0x05u) /* Audio Input Control : Mute */
#define MICP_AICS_CTRL_SET_GAIN_MODE_MANUAL              (0x06u) /* Audio Input Control : Set Gain Mode in Manual */
#define MICP_AICS_CTRL_SET_GAIN_MODE_AUTOMATIC           (0x07u) /* Audio Input Control : Set Gain Mode in Automatic */

/*Microphone Operation Type*/
typedef uint8_t MICP_Opcode_t;
#define MICP_CONFIG_MUTE_NOTIF                           (0x01u) /* Operation issued from MICP_CONTROLLER_ConfigureMuteNotification() */
#define MICP_READ_MUTE                                   (0x02u) /* Operation issued from MICP_CONTROLLER_ReadMute() */
#define MICP_CONFIG_AUDIO_INPUT_STATE_NOTIF              (0x03u) /* Operation issued from MICP_CONTROLLER_ConfigureAudioInputStateNotification() */
#define MICP_CONFIG_AUDIO_INPUT_STATUS_NOTIF             (0x04u) /* Operation issued from MICP_CONTROLLER_ConfigureAudioInputStatusNotification() */
#define MICP_CONFIG_AUDIO_INPUT_DESCRIPTION_NOTIF        (0x05u) /* Operation issued from MICP_CONTROLLER_ConfigureAudioInputDescNotification() */
#define MICP_READ_AUDIO_INPUT_STATE                      (0x06u) /* Operation issued from MICP_CONTROLLER_ReadAudioInputState() */
#define MICP_READ_GAIN_SETTING_PROPERTIES                (0x07u) /* Operation issued from MICP_CONTROLLER_ReadGainSettingProperties() */
#define MICP_READ_AUDIO_INPUT_STATUS                     (0x08u) /* Operation issued from MICP_CONTROLLER_ReadAudioInputStatus() */
#define MICP_READ_AUDIO_INPUT_DESCRIPTION                (0x09u) /* Operation issued from MICP_CONTROLLER_ReadAudioInputDescription() */

/* Types the Microphone Control Events */
typedef enum
{
  MICP_DEVICE_UPDATED_MUTE_EVT,                 /* This event is notified by MICP Device when Microphone State has changed
                                                 * locally.
                                                 * The pInfo field indicates the current Microphone information
                                                 * through the MICP_MicrophoneState_Evt_t type.
                                                 */
  MICP_DEVICE_UPDATED_AUDIO_INPUT_STATE_EVT,    /* This event is notified by MICP Device when an Audio Input State
                                                 * of an Audio Input Instance has changed locally from a remote
                                                 * MICP Controller request or from local request.
                                                 * pInfo field indicates the Audio Input State information through the
                                                 * MICP_AudioInputState_Evt_t type.
                                                 */
  MICP_DEVICE_UPDATED_AUDIO_INPUT_DESCRIPTION_EVT,/* This event is notified by MICP Device when a Description
                                                   * of an Audio Input Instance has changed locally from a remote
                                                   * MICP Controller request only.
                                                   * pInfo field indicates the Audio Input Description information
                                                   * through the MICP_AudioDescription_Evt_t type.
                                                   */
  MICP_CONTROLLER_OPERATION_COMPLETE_EVT,        /* This event is notified when a MICP Controller Operation is complete.
                                                 * pInfo field indicates the Complete Operation to the remote
                                                 * Microphone Control Device through the MICP_CLT_OpComplete_Evt_t
                                                 * type.
                                                 * After receiving this event, a new MICP Controller operation could
                                                 * be started.
                                                 */
  MICP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT,/* This event is notified when a MICP Controller Control Operation is
                                                 * transmitted.
                                                 * pInfo field indicates the transmitted Microphone Control Operation to the
                                                 * remote Microphone Control Device through the MICP_CLT_CtrlOpTransmitted_Evt_t
                                                 * type.
                                                 * After receiving this event, a new MICP Controller operation could
                                                 * be started.
                                                 */
  MICP_CONTROLLER_MUTE_EVT,                     /* This event is notified by MICP Controller when a Microphone State is read
                                                 * thanks to the MICP_CONTROLLER_ReadMicrophoneState() function.
                                                 * pInfo field indicates the Microphone information through the
                                                 * uint8_t type.
                                                 */
  MICP_CONTROLLER_UPDATED_MUTE_EVT,             /* This event is notified by MICP Controller when a Microphone State change
                                                 * is notified from MICP Device side (MICP Controller
                                                 *  has subscribed to Microphone State Change notification).
                                                 * pInfo field indicates the Microphone information through the
                                                 * uint8_t type.
                                                 */
  MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT,        /* This event is notified by MICP Controller when an Audio Input State is
                                                 * read thanks to the MICP_CONTROLLER_ReadAudioInputState() function
                                                 * or if the Audio Input State has changed in MICP renderer side
                                                 * (MICP Controller has subscribed to Audio Input State Change
                                                 * notification).
                                                 * pInfo field indicates the Audio Input State information through the
                                                 * MICP_AudioInputState_Evt_t type.
                                                 */
  MICP_CONTROLLER_GAIN_SETTING_PROP_EVT,        /* This event is notified by MICP Controller when a Gain Setting
                                                 * Properties are read thanks to the function
                                                 * MICP_CONTROLLER_ReadGainSettingProperties()  or if the Gain Setting
                                                 * Properties have changed in MICP renderer side (MICP Controller has
                                                 * subscribed to Gain Setting Properties Change notification).
                                                 * pInfo field indicates the Gain Setting Properties through the
                                                 * MICP_GainSettingProp_Evt_t type.
                                                 */
  MICP_CONTROLLER_AUDIO_INPUT_STATUS_EVT,       /* This event is notified by MICP Controller when an Audio Input Status
                                                 * is read thanks to the MICP_CONTROLLER_ReadAudioInputStatus() function
                                                 * or if the Audio Input Status has changed in MICP renderer side
                                                 * (MICP Controller has subscribed to Audio Input Status Change
                                                 * notification).
                                                 * pInfo field indicates the Audio Input Status information through the
                                                 * MICP_AudioInputStatus_Evt_t type.
                                                 */
  MICP_CONTROLLER_AUDIO_INPUT_DESCRIPTION_EVT,  /* This event is notified by MICP Controller when an Audio Input
                                                 * Description is read thanks to the function
                                                 * MICP_CONTROLLER_ReadAudioInputDescription() function or if the
                                                 * Audio Input Description has changed in MICP renderer side
                                                 * (MICP Controller has subscribed to Audio Input Description Change
                                                 * notification).
                                                 * pInfo field indicates the Audio Input Description information
                                                 * through the MICP_AudioInputDesc_Evt_t type.
                                                 */
} MICP_NotCode_t;


/* Structure of AIC Instance*/
typedef struct
{
  uint8_t               ID;
  Audio_Input_t         AudioInputType;
  uint16_t              StartAttHandle;         /* ATT Start Handle of the AIC Instance in the remote MICP Device */
  uint16_t              EndAttHandle;           /* ATT End Handle of the AIC Instance in the remote MICP Device */
} MICP_AICInstInfo_t;

/* Structure including information of the MICP*/
typedef struct
{
  uint8_t               NumAICInst;     /*Number of discovered AIC Instances*/
  MICP_AICInstInfo_t    *pAICInst;      /*Pointer on discovered AIC Instance table*/
  uint16_t              StartAttHandle; /* ATT Start Handle of Microphone Input Control Service in the remote MICP Device */
  uint16_t              EndAttHandle;   /* ATT End Handle of the Microphone Input Control Service in the remote MICP Device */
} MICP_Info_t;


/* Audio Input State Structure  */
typedef struct
{
  int8_t                GainSetting;    /* Gain of the audio input signal (in dB) to which the AIC Instance
                                         * in MICP Renderer applies.
                                         * This is a value for which a single increment or decrement results in a
                                         * corresponding increase or decrease of the input amplitude by the value of
                                         * the Gain Setting Units value specified in the Gain Setting Properties.
                                         * A Gain Setting value of 0 should result in no change to the input’s original
                                         * amplitude.
                                         */
  MICP_AICMute_t         Mute;           /* Mute State */
  MICP_AICModeGain_t     GainMode;       /* Gain Mode of the AIC Instance in MICP Renderer */
} MICP_AudioInputState_t;

/* Gain Setting Properties Structure  */
typedef struct
{
  uint8_t       GainSettingUnits;       /* Size of a single increment or decrement of the Gain Setting value
                                         * in 0.1 decibel units.
                                         */
  int8_t        GainSettingMin;         /* Minimum allowable value of the Gain Setting value */
  int8_t        GainSettingMax;         /* Maximum allowable value of the Gain Setting value */
} MICP_GainSettingProp_t;


/* Type associated to MICP_CONTROLLER_OPERATION_COMPLETE_EVT event*/
typedef struct
{
  MICP_Opcode_t          Op;     /* MICP Controller Operation opcode */
  uint8_t                Inst;   /* Instance ID if Operation is an AICS operation */
}MICP_CLT_OpComplete_Evt_t;

/* Type associated to MICP_CONTROLLER_CTRL_OPERATION_TRANSMITTED_EVT event*/
typedef struct
{
  MICP_CtrlOpcode_t     CtrlOp; /* Control Operation opcode */
  uint8_t               Inst;    /* Instance ID if Control Operation is an AICS control operation */
}MICP_CLT_CtrlOpTransmitted_Evt_t;


/* Structure used in parameter when MICP_CONTROLLER_AUDIO_INPUT_STATE_EVT event is notified */
typedef struct
{
  uint8_t               AICInst;         /* Audio Input Control Instance Identifier*/
  MICP_AudioInputState_t State;          /* Audio Input State */
  uint8_t               ChangeCounter;   /* This value is incremented by 1 when one of the other field change */
} MICP_AudioInputState_Evt_t;

/* Structure used in parameter when MICP_CONTROLLER_GAIN_SETTING_PROP_EVT event is notified */
typedef struct
{
  uint8_t               AICInst;         /* Audio Input Control Instance Identifier*/
  MICP_GainSettingProp_t Prop;           /* Gain Setting Properties */
} MICP_GainSettingProp_Evt_t;

/* Structure used in parameter when MICP_CONTROLLER_AUDIO_INPUT_STATUS_EVT event is notified */
typedef struct
{
  uint8_t       AICInst;                /* Audio Input Control Instance Identifier*/
  uint8_t       Status;                 /* Current state of the audio Input :
                                         * 0 : Inactive
                                         * 1 : Active
                                         */
} MICP_AudioInputStatus_Evt_t;

/* Structure used in parameter when MICP_CONTROLLER_AUDIO_INPUT_DESCRIPTION_EVT and
 * MICP_CONTROLLER_AUDIO_OUTPUT_DESCRIPTION_EVT events are notified */
typedef struct
{
  uint8_t       Inst;           /* Audio Input Device or Audio Output Device Instance Identifier*/
  uint8_t       *pData;         /* Pointer on String of description of the audio input*/
  uint8_t       DataLength;     /* Length of the pData field*/
} MICP_AudioDescription_Evt_t;

typedef struct
{
  MICP_NotCode_t EvtOpcode;      /* Opcode of the notified event */
  tBleStatus    Status;         /* Status associated to the event */
  uint16_t      ConnHandle;     /* ACL Connection Handle associated to the event */
  uint8_t       *pInfo;         /* Pointer on information associated to the event */
} MICP_Notification_Evt_t;

typedef struct
{
  uint8_t                       InstID;                 /* Instance Identifier */
  uint8_t                       Status;                 /* Current state of the audio Input instance :
                                                         * 0 : Inactive
                                                         * 1 : Active
                                                         */
  MICP_AudioInputState_t        State;                  /* Audio Input State of the audio input Instance */
  MICP_GainSettingProp_t        Prop;                   /* Gain Setting Properties of the audio input Instance  */
  Audio_Input_t                 AudioInputType;         /* Audio Input Type of the audio input Instance  */
  uint8_t                       MaxDescriptionLength;   /* Maximum Size of the Audio input Description string */
  uint8_t                       *pDescription;          /* Pointer on Description string of the audio input Instance  */
  uint8_t                       DescriptionLength;      /* Length of the Description string */
}MICP_AIC_InitInst_t;

/* Microphone Control Device Configuration */
typedef struct
{
  uint8_t               InitialMuteState;       /* Initial Mute State (0: Not Muted ; 1: Muted ; 2: Disabled)*/
  uint8_t               *pStartRamAddr;         /* Start address of the RAM buffer allocated for MICP Device
                                                 * Context memory resource.
                                                 * It must be a 32bit aligned RAM area.
                                                 */
  uint16_t              RamSize;                /* Size of the RAM allocated at pStartRamAddr
                                                 * pointer.
                                                 */
  uint8_t               NumAICInst;             /* Number of Audio Input Control Instances */
  MICP_AIC_InitInst_t   *pAICInst;              /* Audio Input Control Instance Init Config */
} MICP_SRV_Config_t;

/* Microphone Control Controller Configuration */
typedef struct
{

  uint8_t               MaxNumAICInstPerConn;   /* Maximum Number of Audio Input Control Instances */
  uint8_t               *pStartRamAddr;         /* Start address of the RAM buffer allocated for MICP Controller
                                                 * Context memory resource.
                                                 * It must be a 32bit aligned RAM area.
                                                 */
  uint16_t              RamSize;                /* Size of the RAM allocated at pStartRamAddr
                                                 * pointer.
                                                 */

} MICP_CLT_Config_t;

/* Microphone Control Profile Configuration */
typedef struct
{
  MICP_Role_t            Role;                   /* MICP Role Mask */
  uint8_t                MaxNumBleLinks;         /* Maximum number of BLE Links */
  MICP_SRV_Config_t      Device;                 /* Microphone Control Device Configuration */
  MICP_CLT_Config_t      Controller;             /* Microphone Control Controller Configuration */
} MICP_Config_t;

#ifdef __cplusplus
}
#endif

#endif /* __MICP_TYPES_H */
