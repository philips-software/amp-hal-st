/**
  ******************************************************************************
  * @file    audio_types.h
  * @author  MCD Application Team
  * @brief   This file contains definitions used for Common Audio Profile
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
#ifndef __CAP_TYPES_H
#define __CAP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "audio_types.h"
#include "ble_types.h"
#include "bap_types.h"
#include "ccp_types.h"
#include "mcp_types.h"
#include "csip_types.h"
#include "vcp_types.h"
#include "micp_types.h"

/* Defines -------------------------------------------------------------------*/

/* #############################################################################
   #       Defines and MACRO used to allocate GATT Services, GATT Attributes   #
   #       and storage area for Attribute values for Common Audio Profile      #
   #       in Acceptor role and/or Commander role                              #
   ############################################################################# */

/*
 * CAP_ACC_COM_NUM_GATT_SERVICES: number of GATT services required for Common Audio Profile in Acceptor role and/or
 * Commander role registration.
 */
#define CAP_ACC_COM_NUM_GATT_SERVICES                           (1u)

/* #############################################################################
   #       Defines and MACRO used to allocate memory resource of               #
   #       Common Audio Profile in CAP_Config_t                                #
   # (CAP_INITIATOR_MEM_TOTAL_BUFFER_SIZE , CAP_ACCEPTOR_MEM_TOTAL_BUFFER_SIZE)#
   ############################################################################# */
/*
 * CAP_MEM_PER_ASE_SIZE_BYTES: memory size used per Audio Stream Endpoint
 */
#define CAP_MEM_PER_ASE_SIZE_BYTES                              (3u)

/*
 * CAP_MEM_PER_CIG_SIZE_BYTES: memory size used per CIG in CAP Initiator
 */
#define CAP_MEM_PER_CIG_SIZE_BYTES                              (8u)

/*
 * CAP_MEM_PER_CIS_SIZE_BYTES: memory size used per CIS in CAP Initiator
 */
#define CAP_MEM_PER_CIS_SIZE_BYTES                              (8u)

/*
 * CAP_INITIATOR_MEM_UNICAST_CTXT_PER_CONN_SIZE_BYTES: memory size used by CAP Initiator for Unicast Context Allocation
 *                                                      per connection with remote CAP Acceptor
 *
 */
#define CAP_INITIATOR_MEM_UNICAST_CTXT_PER_CONN_SIZE_BYTES      (12u)

/*
 * CAP_INITIATOR_MEM_CONTEXT_SIZE_BYTES: memory size used by CAP Initiator Context Allocation
 */
#define CAP_INITIATOR_MEM_CONTEXT_SIZE_BYTES                    (60u)

/*
 * CAP_VCP_MEM_PER_AIC_INSTANCES: memory size used per an AIC Instance for VCP in CAP Commander
 */
#define CAP_VCP_MEM_PER_AIC_INSTANCES                           (8u)

/*
 * CAP_VCP_MEM_PER_VOC_INSTANCES: memory size used per an VOC Instance for VCP in CAP Commander
 */
#define CAP_VCP_MEM_PER_VOC_INSTANCES                           (8u)

/*
 * CAP_MICP_MEM_PER_VOC_INSTANCES: memory size used per an AIC Instance for MICP in CAP Commander
 */
#define CAP_MICP_MEM_PER_AIC_INSTANCES                          (8u)

/*
 * CAP_MEM_PER_CONNECTION_SIZE_BYTES: memory size used per connection by CAP
 */
#define CAP_MEM_PER_CONNECTION_SIZE_BYTES                       (104u)


/* CAP_MEM_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes, needed for the storage of the
 * CAP Context whose size depends on the CAP role and related features.
 *
 * @param cap_role: CAP Role (see CAP_Role_t)
 *
 * @param num_ble_links: Maximum number of ble Links with remote CAP devices.
 *
 * @param ucl_max_num_cig: Maximum number of CIGes supported by the local Unicast Client
 *
 * @param ucl_max_num_cis_per_cig: Maximum number of CISes per CIG supported by the local Unicast Client
 *
 * @param ucl_max_num_snk_ase_per_link: Maximum number of Sink Audio Stream Endpoints per connection
 *                                      with remote CAP Acceptor
 *
 * @param ucl_max_num_src_ase_per_link: Maximum number of Source Audio Stream Endpoints per connection
 *                                      with remote CAP Acceptor
 *
 * @param ccp_clt_feature_support : 1 if Call Control Profile feature is supported by local CAP Acceptor/Commander
 *
 * @param max_num_bearer_instances: Maximum number of Telephony Bearer Instances (without take in account
 *                                  the mandatory Generic Telephony Bearer) per connection supported
 *                                  by CAP Acceptor/Commander in CCP Client role
 *
 * @param mcp_clt_feature_support : 1 if Media Control Profile feature is supported by local CAP Acceptor/Commander
 *
 * @param max_num_media_player_instances: Maximum number of Media Player Instances (without take in account
 *                                        the mandatory Generic Media Player) per connection supported
 *                                        by CAP Acceptor in MCP Client role
 *
 * @param usr_max_num_snk_ase_per_link: Maximum number of Sink Audio Stream Endpoints per connection
 *                                      with remote CAP Initiator
 *
 * @param usr_max_num_src_ase_per_link: Maximum number of Source Audio Stream Endpoints per connection
 *                                      with remote CAP Initiator
 *
 * @param vcp_ctlr_feature_support : 1 if Volume Control Profile feature is supported by local CAP Commander
 *
 * @param max_num_vcp_aic_instances: Maximum number of Audio Input Control Instances in VCP per connection supported
 *                                   by CAP Commander in MCP Controller role
 *
 * @param max_num_vcp_voc_instances: Maximum number of Volume Offset Control Instances in VCP per connection supported
 *                                   by CAP Commander in VCP Controller role
 *
 * @param micp_ctlr_feature_support : 1 if Microphone Control Profile feature is supported by local CAP Commander
 *
 * @param max_num_vcp_aic_instances: Maximum number of Audio Input Control Instances in MICP per connection supported
 *                                   by CAP Commander in MICP Controller role
 *
 */
#define CAP_MEM_TOTAL_BUFFER_SIZE(cap_role,num_ble_links, \
                                  ucl_max_num_cig,ucl_max_num_cis_per_cig, \
                                  ucl_max_num_snk_ase_per_link, \
                                  ucl_max_num_src_ase_per_link, \
                                  ccp_clt_feature_support,max_num_bearer_instances,\
                                  mcp_clt_feature_support,max_num_media_player_instances, \
                                  usr_max_num_snk_ase_per_link,usr_max_num_src_ase_per_link, \
                                  vcp_ctlr_feature_support,max_num_vcp_aic_instances,max_num_vcp_voc_instances, \
                                  micp_ctlr_feature_support,max_num_micp_aic_instances) \
        ((CAP_MEM_PER_CONNECTION_SIZE_BYTES * num_ble_links) \
        + (((cap_role & CAP_ROLE_INITIATOR) == CAP_ROLE_INITIATOR) ?  \
          (CAP_INITIATOR_MEM_CONTEXT_SIZE_BYTES \
          + ((CAP_MEM_PER_CIG_SIZE_BYTES + (CAP_MEM_PER_CIS_SIZE_BYTES * ucl_max_num_cis_per_cig))*ucl_max_num_cig) \
          + num_ble_links * (CAP_INITIATOR_MEM_UNICAST_CTXT_PER_CONN_SIZE_BYTES + DIVC(((ucl_max_num_snk_ase_per_link + ucl_max_num_src_ase_per_link) * CAP_MEM_PER_ASE_SIZE_BYTES),4u) * 4u)) : (0)) \
        + (((cap_role & CAP_ROLE_ACCEPTOR) == CAP_ROLE_ACCEPTOR) ?  \
          (num_ble_links * (DIVC(((usr_max_num_snk_ase_per_link+usr_max_num_src_ase_per_link) * CAP_MEM_PER_ASE_SIZE_BYTES),4u) * 4u)) : (0)) \
        + (((cap_role & (CAP_ROLE_ACCEPTOR)) != 0x00) ?  \
          (num_ble_links * \
          (((ccp_clt_feature_support == 1u ) ?  (DIVC((2u *(max_num_bearer_instances + 1u)),4u) * 4u) : (0)) \
          + ((mcp_clt_feature_support == 1u ) ?  (DIVC((2u *(max_num_media_player_instances + 1u)),4u) * 4u) : (0)))) : (0)) \
        + (((cap_role & CAP_ROLE_COMMANDER) != 0x00) ?  \
          (num_ble_links * \
          (((vcp_ctlr_feature_support == 1u ) ?  (DIVC((CAP_VCP_MEM_PER_AIC_INSTANCES * max_num_vcp_aic_instances),4u) * 4u) : (0)) \
          + ((vcp_ctlr_feature_support == 1u ) ?  (DIVC((CAP_VCP_MEM_PER_VOC_INSTANCES * max_num_vcp_voc_instances),4u) * 4u) : (0)) \
          + ((micp_ctlr_feature_support == 1u ) ?  (DIVC((CAP_MICP_MEM_PER_AIC_INSTANCES * max_num_micp_aic_instances),4u) * 4u) : (0)))) : (0)) \
        )

/* Types ---------------------------------------------------------------------*/

/* Types of roles of the Common Audio Profile */
typedef uint8_t CAP_Role_t;
#define CAP_ROLE_ACCEPTOR               (0x01)
#define CAP_ROLE_INITIATOR              (0x02)
#define CAP_ROLE_COMMANDER              (0x04)

/* Mask of Link about Audio Profile in Generic Audio Framework*/
typedef uint8_t GAF_Profiles_Link_t;
#define CSIP_LINK                       (0x01) /* Coordinated Set Identification Profile Link */
#define BAP_UNICAST_LINK                (0x02) /* BAP Unicast Link */
#define BAP_BA_LINK                     (0x04) /* BAP Broadcast Assistant link */
#define VCP_LINK                        (0x08) /* Volume Control Profile Link */
#define MICP_LINK                       (0x10) /* Microphone Control Profile Link */
#define CCP_LINK                        (0x20) /* Call Control Profile Link */
#define MCP_LINK                        (0x40) /* Media Control Profile Link */

/*Type used to specify the type of Set of Acceptors*/
typedef uint8_t CAP_Set_Acceptors_t;
#define CAP_SET_TYPE_AD_HOC             (0x00u)  /* Ad-hoc Set of Acceptors */
#define CAP_SET_TYPE_CSIP               (0x01u)  /* CSIP Coordinated Set of Acceptors */

/* Type used to specify the type of operation to use in the Broadcast Audio Reception Start Procedure */
typedef uint8_t CAP_BA_Operation_t;
#define CAP_BA_ADD_SOURCE_OPERATION    (0x00u)
#define CAP_BA_MODIFY_SOURCE_OPERATION (0x01u)

typedef enum
{
/* #############################################################################
   #                               COMMON Events                               #
   ############################################################################# */

  CAP_LINKUP_COMPLETE_EVT,                      /* Event notified once CAP Linkup is complete */
  CAP_CODEC_CONFIGURED_EVT,                     /* This event is called by Unicast Server, Unicast Client, Broadcast
                                                 * Source or Broadcast Sink when the codec configuration for the current
                                                 * process has been set up
                                                 */
  CAP_REM_ACC_SNK_PAC_RECORD_INFO_EVT,          /* This event is notified when a remote CAP acceptor Sink capability
                                                 * has been discovered
                                                 * This event is notified during CAP linkup process
                                                 * The pInfo field indicates the Sink PAC Record information
                                                 * through the BAP_AudioCapabilities_t type.
                                                 */
  CAP_REM_ACC_SRC_PAC_RECORD_INFO_EVT,          /* This event is notified when a remote CAP acceptor Sink capability
                                                 * has been discovered
                                                 * This event is notified during CAP linkup process
                                                 * The pInfo field indicates the Source PAC Record information
                                                 * through the BAP_AudioCapabilities_t type.
                                                 */
  CAP_REM_ACC_SNK_AUDIO_LOCATIONS_UPDATED_EVT,  /* This event is notified when the Sink Audio Locations in the
                                                 * remote CAP Acceptor has been updated.
                                                 * The pInfo field indicates the new Sink Audio Locations value.
                                                 * For this, the pInfo should be cast in Audio_Location_t type.
                                                 */
  CAP_REM_ACC_SRC_AUDIO_LOCATIONS_UPDATED_EVT,  /* This event is notified when the Source Audio Locations in the
                                                 * remote CAP Acceptor has been updated.
                                                 * The pInfo field indicates the new Source Audio Locations value.
                                                 * For this, the pInfo should be cast in Audio_Location_t type.
                                                 */
  CAP_REM_ACC_AVAIL_AUDIO_CONTEXTS_UPDATED_EVT, /* This event is notified when the Available Audio Contexts in the
                                                 * remote CAP Acceptor has been updated.
                                                 * The pInfo field indicates the new Available Audio Contexts value.
                                                 * For this, the pInfo should be cast in BAP_Audio_Contexts_t type.
                                                 */
  CAP_REM_ACC_SUPP_AUDIO_CONTEXTS_UPDATED_EVT,  /* This event is notified when the Supported Audio Contexts in the
                                                 * remote CAP Acceptor has been updated.
                                                 * The pInfo field indicates the new Supported Audio Contexts value.
                                                 * For this, the pInfo should be cast in BAP_Audio_Contexts_t type.
                                                 */
  CAP_READ_REM_SNK_PAC_RECORDS_COMPLETE_EVT,    /* This event is notified when the process started by calling the
                                                 * CAP_ReadRemoteSnkPACRecords() API is complete.
                                                 */
  CAP_READ_REM_SRC_PAC_RECORDS_COMPLETE_EVT,    /* This event is notified when the process started by calling the
                                                 * CAP_ReadRemoteSrcPACRecords() API is complete.
                                                 */

/* #############################################################################
   #                              UNICAST Events                               #
   ############################################################################# */

  CAP_UNICAST_LINKUP_EVT,                       /* Event notified during CAP linkup procedure if CAP Initiator supports
                                                 * BAP Unicast Client role and discovers that the remote device supports
                                                 * BAP Unicast Server role.
                                                 * The pInfo field indicates information about unicast settings through
                                                 * the BAP_Unicast_Server_Info_t type.
                                                 */
  CAP_UNICAST_AUDIOSTARTED_EVT,                 /* This event is notified when Unicast Audio Start Procedure is complete
                                                 * for CAP Initiator Role
                                                 */
  CAP_UNICAST_AUDIOSTOPPED_EVT,                 /* This event is notified when Unicast Audio Start Procedure is complete
                                                 * for CAP Initiator Role
                                                 */
  CAP_UNICAST_PREFERRED_QOS_REQ_EVT,            /* This event is notified when Preferred QoS settings are requested
                                                 * for CAP Acceptor.
                                                 * The pInfo field indicates the Audio Stream Endpoint Identifier and
                                                 * type, the associated Codec Configuration, the Minimum and Maximum
                                                 * Controller Delay for the specified Codec Configuration and the
                                                 * preferred QoS fields to fill within the callback through
                                                 * the BAP_PrefQoSConfReq_t type.
                                                 */
  CAP_UNICAST_AUDIO_DATA_PATH_SETUP_REQ_EVT,    /* This event is notified when Audio Data Path shall be setup.
                                                 * The pInfo field indicates the Audio Stream Endpoint Audio Path
                                                 * Direction, the Codec Configuration and the Minimum and Maximum
                                                 * Controller Delays for the specified Codec Configuration through the
                                                 * BAP_SetupAudioDataPathReq_t type.
                                                 * The CAP_Unicast_SetupAudioDataPath() shall be called to responds
                                                 * to the Audio Data Path Setup Request.
                                                 */
  CAP_UNICAST_AUDIO_CONNECTION_UP_EVT,          /* This event is notified when Audio Path is up during Unicast Audio
                                                 * procedure.
                                                 * The pInfo field indicates audio information through the
                                                 * BAP_Unicast_Audio_Path_t type.
                                                 */
  CAP_UNICAST_AUDIO_CONNECTION_DOWN_EVT,        /* This event is notified when Audio Path is down during Unicast Audio
                                                 * procedure.
                                                 * The pInfo field indicates audio path direction through the
                                                 * BAP_Unicast_Audio_Path_t type.
                                                 */
  CAP_UNICAST_AUDIO_UPDATED_EVT,                /* Event notified when Unicast Audio Update procedure is complete
                                                 */
  CAP_UNICAST_CLIENT_ASE_STATE_EVT,             /* Event notified,in CAP Initiator, when Audio Stream Endpoint State
                                                 * changes.
                                                 * The pInfo field indicates the state of the ASE and the associated
                                                 * information through the BAP_ASE_State_Params_t type.
                                                 */
  CAP_UNICAST_SERVER_ASE_STATE_EVT,             /* Event notified,in CAP Acceptor, when Audio Stream Endpoint State
                                                 * changes.
                                                 * The pInfo field indicates the state of the ASE and the associated
                                                 * information through the BAP_ASE_State_Params_t type.
                                                 */
  CAP_UNICAST_ASE_METADATA_UPDATED_EVT,         /* Event notified when a metadata associated to an Audio Stream Endpoint
                                                 * is updated but Audio Stream Endpoint State doesn't change.
                                                 * The pInfo field indicates the state of the ASE and the associated
                                                 * information through the BAP_ASE_State_Params_t type.
                                                 */

/* #############################################################################
   #                              BROADCAST Events                             #
   ############################################################################# */

  CAP_BROADCAST_AUDIOSTARTED_EVT,               /* This event is notified when Broadcast Audio Start Procedure is
                                                 * complete for CAP Initiator Role
                                                 * The pInfo field indicates the number of BISes startes and their
                                                 * connection handles using the CAP_Broadcast_AudioStarted_Info_t type.
                                                 */
  CAP_BROADCAST_AUDIOSTOPPED_EVT,               /* This event is notified when Broadcast Audio Stop Procedure is
                                                 * complete for CAP Initiator Role
                                                 */
  CAP_BROADCAST_SOURCE_ADV_REPORT_EVT,          /* This event is notified by Acceptor when a Broadcast Source has been
                                                 * scanned
                                                 * The pInfo field indicates details about the Broadcast Source through
                                                 * the BAP_Broadcast_Source_Adv_Report_Data_t type
                                                 */
  CAP_BROADCAST_PA_SYNC_ESTABLISHED_EVT,        /* This event is notified by Acceptor when when synchronization to a
                                                 * periodic advertising train has been established
                                                 * The pInfo field indicates the details about the Periodic Advertising
                                                 * train through the BAP_PA_Sync_Established_Data_t type
                                                 */
  CAP_BROADCAST_BASE_REPORT_EVT,                /* This event is notified by Acceptor when a BASE report is received
                                                 * through a periodic advertising train
                                                 * The pInfo field indicates the content of the BASE structure through
                                                 * the BAP_BASE_Report_Data_t type
                                                 */
  CAP_BROADCAST_BIGINFO_REPORT_EVT,             /* This is event is notified by Acceptor when a BIGInfo report is
                                                 * received through a periodic advertising train
                                                 * The pInfo field indicates the content of the BIGInfo through
                                                 * the BAP_BIGInfo_Report_Data_t type
                                                 */
  CAP_BROADCAST_BIG_SYNC_ESTABLISHED_EVT,       /* This event is notified by Acceptor when synchronization to a BIG has
                                                 * been established
                                                 * The pInfo field indicates the details about the BIG through the
                                                 * BAP_BIG_Sync_Established_Data_t type
                                                 */
  CAP_BROADCAST_PA_SYNC_LOST_EVT,               /* This event is notified by Acceptor when synchronization to a periodic
                                                 * advertising train has been lost
                                                 * The pInfo field indicates the details about the Periodic Advertising
                                                 * train through the BAP_PA_Sync_Lost_Data_t type
                                                 */
  CAP_BROADCAST_BIG_SYNC_LOST_EVT,              /* This event is notified by Acceptor when synchronization to a BIG has
                                                 * been lost
                                                 * The pInfo field indicates the BIG Handle and the reason of the sync
                                                 * loss through the BAP_BIG_Sync_Lost_Data_t type
                                                 */
  CAP_BROADCAST_AUDIO_UP_EVT,                   /* This event is notified by acceptor and initiator when Audio Path has
                                                 * been setup
                                                 */
  CAP_BROADCAST_AUDIO_DOWN_EVT,                 /* This event is notified by acceptor and initiator when Audio Path has
                                                 * been removed
                                                 */
  CAP_BROADCAST_SDE_ADD_SOURCE_REQ_EVT,         /* This event is notified by Scan Delegator when an Add Source operation
                                                 * is requested by a remote Broadcast Assistant
                                                 * The pInfo field indicates the content of the Source through the
                                                 * BAP_BA_Broadcast_Source_Add_t type
                                                 */
  CAP_BROADCAST_SDE_MODIFY_SOURCE_REQ_EVT,      /* This event is notified by Scan Delegator when a Modify Source
                                                 * operation is requested by a remote Broadcast Assistant
                                                 * The pInfo field indicates the content of the Source through the
                                                 * BAP_BA_Broadcast_Source_Modify_t type
                                                 */
  CAP_BROADCAST_SDE_PA_SYNC_TERMINATED_EVT,     /* This event is notified by Scan Delegator when a command coming from
                                                 * a remote Broadcast Assistant has terminated a PA Sync
                                                 * The pInfo field indicates the Sync Handle of the PA through the
                                                 * uint16_t type
                                                 */
  CAP_BROADCAST_SDE_BIG_SYNC_TERMINATED_EVT,    /* This event is notified by Scan Delegator when a command coming from
                                                 * a remote Broadcast Assistant has terminated a BIG Sync
                                                 * The pInfo field indicates the BIG Handle of the BIG through the
                                                 * uint8_t type
                                                 */

/* #############################################################################
   #                          BROADCAST ASSISTANT Events                       #
   ############################################################################# */

  CAP_BA_LINKUP_EVT,                            /* Event notified during CAP linkup procedure if CAP Commander supports
                                                 * BAP Broadcast Assistant role and discovers that the remote device
                                                 * supports Scan Delegator role.
                                                 * The pInfo field indicates the number of Broadcast Receive State
                                                 * characteristics through the BAP_Broadcast_Assistant_Info_t type
                                                 */
  CAP_BA_REM_BROADCAST_RECEIVE_STATE_INFO_EVT,  /* This event is notified by Commander with Broadcast Assistant role
                                                 * to report Broadcast Receive State value
                                                 * This event is notified during CAP linkup process or when
                                                 * remote deviced updates it through GATT Notifications
                                                 * The pInfo field indicates the content of the broadcast receive state
                                                 * characteristic through the BAP_BA_Broadcast_Source_State_t type
                                                 */
  CAP_BA_AUDIO_RECEPTION_STARTED_EVT,           /* This event is notified by Commander with Broadcast Assistant role
                                                 * to report that the Audio Reception Start Procedure is complete
                                                 */
  CAP_BA_AUDIO_RECEPTION_STOPPED_EVT,           /* This event is notified by Commander with Broadcast Assistant role
                                                 * to report that the Audio Reception Stop Procedure is complete
                                                 */
  CAP_BA_REM_REMOVED_SOURCE_EVT,                /* This event is notified by Commander with Broadcast Assistant role
                                                 * to report that a remote Scan Delegator has removed a source
                                                 */
  CAP_BA_SOLICITING_SCAN_DELEGATOR_REPORT_EVT,  /* This event is notified by Commander with Broadcast Assistant role
                                                 * when a Scan Delegator soliciting request has been scanned
                                                 * The pInfo field indicates the address and address type of the Scan
                                                 * delagor through the BAP_Soliciting_Scan_Delegator_Data_t type
                                                 */
  CAP_BA_BROADCAST_SOURCE_ADV_REPORT_EVT,       /* This event is notified by Commander with Broadcast Assistant role
                                                 * when a Broadcast Sourcehas been scanned
                                                 * The pInfo field indicates details about the Broadcast Source through
                                                 * the BAP_Broadcast_Source_Adv_Report_Data_t type
                                                 */
  CAP_BA_PA_SYNC_ESTABLISHED_EVT,                /* This event is notified by Commander with Broadcast Assistant role
                                                 * when synchronizationto a periodic advertising train has been
                                                 * established
                                                 * The pInfo field indicates the details about the Periodic Advertising
                                                 * train through the BAP_PA_Sync_Established_Data_t type
                                                 */
  CAP_BA_PA_SYNC_LOST_EVT,                      /* This event is notified by Commander with Broadcast Assistant role
                                                 * when synchronization to a periodic advertising train has been lost
                                                 * The pInfo field indicates the details about the Periodic Advertising
                                                 * train through the BAP_PA_Sync_Lost_Data_t type
                                                 */
  CAP_BA_BASE_REPORT_EVT,                       /* This is event is notified by Commander with Broadcast Assistant role
                                                 * when a BASE report is received through a periodic advertising train
                                                 * The pInfo field indicates the content of the BASE structure through
                                                 * the BAP_BASE_Report_Data_t type
                                                 */
  CAP_BA_BIGINFO_REPORT_EVT,                    /* This is event is notified by Commander with Broadcast Assistant role
                                                 * when a BIGInfo report is received through a periodic advertising
                                                 * train
                                                 * The pInfo field indicates the content of the BIGInfo through
                                                 * the BAP_BIGInfo_Report_Data_t type
                                                 */

/* #############################################################################
   #                            Sub-Profiles Events                            #
   ############################################################################# */

  CAP_CCP_LINKUP_EVT,                           /* Event notified during CAP linkup procedure if CAP Commander or
                                                 * CAP Acceptor supports Call Control Client role and discovers
                                                 * that the remote device supports CCP Server role.
                                                 * The Generic Telephony Bearer and potential Telephony Bearer Instances
                                                 * discovery is complete.
                                                 */
  CAP_CCP_INST_ACTIVATED_EVT,                   /* This event is notified when a Telephone Bearer Instance of the
                                                 * Call Control Profile is activated and associated to an Audio Stream.
                                                 * This event is only notified for CAP Acceptor or CAP Commander role.
                                                 * The pInfo field indicates information about the Telephone Bearer
                                                 * Instance through the CAP_CCP_InstInfo_Evt_t type.
                                                 */
  CAP_CCP_INST_DEACTIVATED_EVT,                 /* This event is notified when a Telephone Bearer Instance of the
                                                 * Call Control Profile is deactivated.
                                                 * This event is only notified for CAP Acceptor or CAP Commander role.
                                                 * The pInfo field indicates the Content Control ID of the Telephone
                                                 * Bearer Instance.
                                                 */
  CAP_CCP_META_EVT,                             /* This event is notified for Call Control Profile.
                                                 * A subevent is associated of type CCP_NotCode_t.
                                                 * The pInfo field indicates information associated to the CCP subevent
                                                 * Instance through the CCP_Notification_Evt_t type.
                                                 */
  CAP_MCP_LINKUP_EVT,                           /* Event notified during CAP linkup procedure if CAP Commander or
                                                 * CAP Acceptor supports Media Control Client role and discovers
                                                 * that the remote device supports Media Server role.
                                                 * The Generic Media Player and potential Media Player Instances
                                                 * discovery is complete.
                                                 */
  CAP_MCP_INST_ACTIVATED_EVT,                   /* This event is notified when a Media Player Instance of the
                                                 * Media Control Profile is activated and associated to an Audio Stream.
                                                 * This event is only notified for CAP Acceptor or CAP Commander role.
                                                 * The pInfo field indicates information about the Media Player
                                                 * Instance through the CAP_MCP_InstInfo_Evt_t type.
                                                 */
  CAP_MCP_INST_DEACTIVATED_EVT,                 /* This event is notified when a Media Player Instance of the
                                                 * Media Control Profile is deactivated.
                                                 * This event is only notified for CAP Acceptor or CAP Commander role.
                                                 * The pInfo field indicates the Content Control ID of the Media Player
                                                 * Instance.
                                                 */
  CAP_MCP_META_EVT,                             /* This event is notified for Media Control Profile.
                                                 * A subevent is associated of type MCP_NotCode_t.
                                                 * The pInfo field indicates information associated to the MCP subevent
                                                 * Instance through the MCP_Notification_Evt_t type.
                                                 */
  CAP_CSI_LINKUP_EVT,                           /* Event notified during CAP linkup procedure if CAP Initiator/Commander
                                                 * supports CSIP Set Coordinator role and discovers that the remote
                                                 * device supports CSIP Set Member role.
                                                 * The pInfo field indicates information about the remote CSIP Set
                                                 * Member through the CSIP_SetMember_Info_t type
                                                 */
  CAP_VOLUME_CTRL_LINKUP_EVT,                   /* Event notified during CAP linkup procedure if CAP Commander supports
                                                 * VCP Volume Controller role and discovers that the remote device
                                                 * supports VCP Volume Renderer role.
                                                 * The pInfo field indicates information about the remote Volume
                                                 * Renderer through the VCP_Info_t type
                                                 */
  CAP_SET_VOLUME_PROCEDURE_COMPLETE_EVT,        /* Event notified once the CAP Set Volume procedure launched thanks to
                                                 * the CAP_VolumeController_StartSetVolumeProcedure() API is complete.
                                                 * This event is notified once the procedure to change the volume level
                                                 * on all the Acceptors of the Coordinated Set is done.
                                                 */
  CAP_SET_VOLUME_MUTE_STATE_PROCEDURE_COMPLETE_EVT,
                                                 /* Event notified once the CAP Change Mute State Volume procedure
                                                 * launched thanks to the CAP_VolumeController_StartSetVolumeMuteStateProcedure() API
                                                 * is complete.
                                                 * This event is notified once the procedure to change the Mute State
                                                 * on all the Acceptors of the Coordinated Set is done.
                                                 */
  CAP_SET_VOLUME_OFFSET_PROCEDURE_COMPLETE_EVT, /* Event notified once the CAP Set Volume Offset procedure launched thanks to
                                                 * the CAP_VolumeController_StartSetVolumeOffsetProcedure() API is complete.
                                                 * This event is notified once the procedure to change the volume offset
                                                 * on all the Acceptors of the Coordinated Set is done.
                                                 */
  CAP_VCP_META_EVT,                             /* This event is notified for Volume Control Profile.
                                                 * A subevent is associated of type VCP_NotCode_t.
                                                 * The pInfo field indicates information associated to the VCP subevent
                                                 * Instance through the VCP_Notification_Evt_t type.
                                                 */
  CAP_MICROPHONE_CTRL_LINKUP_EVT,               /* Event notified during CAP linkup procedure if CAP Commander supports
                                                 * MICP Microphone Controller role and discovers that the remote device
                                                 * supports MICP Microphone Device role.
                                                 * The pInfo field indicates information about the remote Microphone
                                                 * Device through the MICP_Info_t type
                                                 */
  CAP_SET_MICROPHONE_MUTE_PROCEDURE_COMPLETE_EVT,/* Event notified once the CAP Set Mute procedure launched thanks to
                                                 * the CAP_MicrophoneController_StartSetMuteProcedure() API is complete.
                                                 * This event is notified once the procedure to change the mute value
                                                 * on all the Acceptors of the Coordinated Set is done.
                                                 */
  CAP_SET_MICROPHONE_GAIN_SETTING_PROCEDURE_COMPLETE_EVT,
                                                /* Event notified once the CAP Set Gain Setting procedure launched thanks to
                                                 * the CAP_MicrophoneController_StartSetGainSettingProcedure() API is complete.
                                                 * This event is notified once the procedure to change the gain setting
                                                 * on all the Acceptors of the Coordinated Set is done.
                                                 */
  CAP_MICP_META_EVT,                            /* This event is notified for Volume Control Profile.
                                                 * A subevent is associated of type VCP_NotCode_t.
                                                 * The pInfo field indicates information associated to the VCP subevent
                                                 * Instance through the VCP_Notification_Evt_t type.
                                                 */
  CAP_CSIP_META_EVT,                             /* This event is notified for Coordinated Set Identification Profile.
                                                 * A subevent is associated of type CSIP_NotCode_t.
                                                 * The pInfo field indicates information associated to the VCP subevent
                                                 * Instance through the CSIP_Notification_Evt_t type.
                                                 */
} CAP_NotCode_t;

/* Types of Announcement used by CAP Acceptor and CAP Commander in its advertising Packet */
typedef uint8_t CAP_Announcement_t;
#define CAP_GENERAL_ANNOUNCEMENT        (0x00)  /* Peripheral is connectable but is not requesting a connection.
                                                 */
#define CAP_TARGETED_ANNOUNCEMENT       (0x01)  /* Peripheral is connectable and is requesting a connection from the
                                                 * Central.
                                                 */

/* Structure passed in parameter of the BAP Notification callback function*/
typedef struct
{
  CAP_NotCode_t EvtOpcode;      /* Opcode of the notified event */
  tBleStatus    Status;         /* Status associated to the event */
  uint16_t      ConnHandle;     /* ACL Connection Handle associated to the event */
  uint8_t       *pInfo;         /* Pointer on information associated to the event */
}CAP_Notification_Evt_t;

/* Structure passed in parameter of the Unicast Audio Start function */
typedef struct
{
  Sampling_Freq_t          SamplingFrequency;  /* Sampling Frequency to configure */
  Frame_Duration_t         FrameDuration;      /* Frame Duration to configure */
  uint16_t                 OctetPerCodecFrame; /* Number of octet per codec frame to configure */
  uint8_t                  FrameBlockPerSdu;   /* Number of frame block per SDU to configure */
  uint8_t                  NumberChannels;     /* Number of channels to configure */
} CAP_Codec_Specific_Configuration_t;

/* Structure passed in parameter of the Unicast Audio Started event notification */
typedef struct
{
  uint8_t               ConfID;         /* ID of the configuration */
  Audio_Coding_Format_t CodingFormat;   /* Coding Format */
  uint16_t              CompanyID;      /* Company Identifier (see Bluetooth Assigned Numbers) */
  uint16_t              VsCodecID;      /* Vendor-specific codec ID value of the PAC record.
                                         * Shall be 0x0000 if octet Coding Format is not CODING_FORMAT_VS
                                         */
  Audio_Context_t       PreferredAudioContext;  /* Preferred Audio Context found for this configuration */
} CAP_Compatible_Conf_t;

typedef struct
{
  uint16_t      StartAttHandle;                 /* ATT Start Handle of CSIS in the remote CSIP Set Member */
  uint16_t      EndAttHandle;                   /* ATT End Handle of CSIS in the remote CSIP Set Member */
  uint8_t       SetIdentityResolvingKeyType;
  uint8_t       *pSetIdentityResolvingKey;
  uint8_t       CoordinatedSetSize;           /*if value is 0, it means that related information of the field is not usable*/
  uint8_t       SetMemberRank;                /*if value is 0, it means that related information of the field is not usable*/
} CSIP_SetMember_Info_t;

typedef struct
{
  Target_Latency_t      TargetLatency;  /* Provides context for the server to
                                         * return meaningful values for QoS
                                         * preferences in Codec Configured state
                                         */
  Target_Phy_t          TargetPhy;      /* PHY parameter target to achieve the
                                         * Target_Latency value
                                         */
  BAP_AudioCodecID_t    CodecID;        /* Codec ID information*/

  /*Codec Specific Configuration*/
  CAP_Codec_Specific_Configuration_t CodecSpecificConfParams;

  Audio_Chnl_Allocation_t            AudioChannelAllocation;

  uint8_t                            ChannelPerCIS;
} CAP_Unicast_AudioStart_Codec_Conf_t;

typedef struct
{
  uint8_t       NumBISes;               /* The number of BIS in the BIG */
  uint16_t      *pConnHandle;           /* The list of connection handles of all BISes in the BIG*/
} CAP_Broadcast_AudioStarted_Info_t;

typedef struct
{
  uint32_t              SDUInterval;            /* SDU Interval */
  BAP_Framing_t         Framing;                /* Server support for unframed ISOAL PDUs*/
  uint16_t              MaxSDU;                 /* Maximum size of the SDU */
  uint8_t               RetransmissionNumber;   /* Retransmission Number */
  uint16_t              MaxTransportLatency;    /* Max Transport Latency */
  uint32_t              PresentationDelay;      /* Presentation Delay */
} CAP_Unicast_AudioStart_QoS_Conf_t;

/*Audio Stream Configuration for CAp unicast Start Procedure*/
typedef struct
{
  CAP_Unicast_AudioStart_Codec_Conf_t  *pCodecConf;     /*Pointer to a Codec Configuration parameters*/
  CAP_Unicast_AudioStart_QoS_Conf_t    *pQoSConf;       /*Pointer to a QoS Configuration parameters*/
  uint8_t                               MetadataLen;    /*Length of the metadata*/
  uint8_t                               *pMetadata;     /*Pointer to the metadata array*/
} CAP_Unicast_Audio_Stream_Params_t;

/*Structure used in parameter of the CAP_Unicast_AudioStart() API*/
typedef struct
{
  uint16_t                              ConnHandle;    /* Connection Handle of the remote CAP Acceptor */
  Audio_Role_t                          StreamRole;    /* Mask of Audio Role to configure on the remote Acceptor */
  CAP_Unicast_Audio_Stream_Params_t     StreamSnk;     /* Sink Stream Configuration (from Initiator to Acceptor)
                                                          Used only if StreamRole countains AUDIO_ROLE_SINK */
  CAP_Unicast_Audio_Stream_Params_t     StreamSrc;     /* Source Stream Configuration (from Acceptor to Initiator)
                                                          Used only if StreamRole countains AUDIO_ROLE_SOURCE */
} CAP_Unicast_AudioStart_Stream_Params_t;

/*Structure used in parameter of the CAP_Unicast_AudioUpdate() API*/
typedef struct
{
  uint16_t      ConnHandle;             /* Connection Handle of the remote CAP Acceptor */
  uint8_t       *pSnkMetadata;          /* Pointer on Metadata for Sink Stream */
  uint8_t       SnkMetadataLength;      /* Metadata length for Sink Stream */
  uint8_t       *pSrcMetadata;          /* Pointer on Metadata for Source Stream */
  uint8_t       SrcMetadataLength;      /* Metadata length for Source Stream */
} CAP_Unicast_AudioUpdate_Stream_Params_t;

/* Structure used in parameter of the CAP_Broadcast_AudioStart() API */
typedef struct
{
  BAP_BASE_Group_t                      *pBaseGroup; /* A pointer to a BASE group for the Broadcast Source */
  uint8_t                               BigHandle; /* The Handle of the BIG to create */
  uint8_t                               Rtn; /* The Number of retransmission of the BIG to create */
  Target_Phy_t                          Phy; /* The PHY of the BIG to create */
  BAP_Packing_t                         Packing; /* The Packing of the BIG */
  uint16_t                              MaxTransportLatency; /* The Maximum Transport Latency of the BIG to create */
  uint8_t                               Encryption; /* 0x00 to disable encryption, 0x01 to enable it */
  uint32_t                              *pBroadcastCode; /* The broadcast Code of the Broadcast Source to create */
  uint8_t                               AdvHandle; /* The Advertising Handle of the Extended Advertising and Periodic Advertising */
  uint32_t                              BroadcastId; /* The Broadcast ID of the Broadcast Source. Needs to be generated
                                                        randomly at each new BIG establishement */
  BAP_Extended_Advertising_Params_t     *pExtendedAdvParams; /* A pointer to the Extended Advertising Parameters structure */
  BAP_Periodic_Advertising_Params_t     *pPeriodicAdvParams; /* A pointer to the Periodic Advertising Parameters structure */
  uint8_t                               *pAdditionalAdvData; /* A pointer to a data array to add to advertising data */
  uint8_t                               AdditionalAdvDataLen; /* The length of the data array to add */
} CAP_Broadcast_AudioStart_Params_t;

/* Structure used as parameter for the CAP_Broadcast_AudioReceptionStart API */
typedef struct
{
  uint16_t ConnHandle;                           /* Connection Handle of the remote CAP Acceptor */

  CAP_BA_Operation_t Operation;                  /* 0x00 for Add Source operation
                                                    0x01 for Modify Source operation
                                                  */

  uint8_t SourceID;                              /* ID of the source to modify if Operation is 0x01 */

  uint8_t  AdvAddressType;                       /* Advertiser_Address_Type for the Broadcast Source
                                                  * 0x00 = Public Device Address or Public Identity Address
                                                  * 0x01 = Random Device Address or Random (static) Identity Address
                                                  */

  uint8_t  aAdvAddress[6u];                       /* Advertiser_Address for the Broadcast Source */

  uint8_t  AdvSid;                               /* Advertising_SID subfield of the ADI field of the AUX_ADV_IND PDU or
                                                  * the LL_PERIODIC_SYNC_IND containing the SyncInfo that points to the
                                                  * PA transmitted by the Broadcast Source.
                                                  * Range: 0x00-0x0F
                                                  */

  uint8_t  aBroadcastId[3u];                      /* Broadcast_ID of the Broadcast Source */

  uint8_t  PaSync;                               /* 0x00: Do not synchronize to PA
                                                  * 0x01: Synchronize to PA – PAST available
                                                  * 0x02: Synchronize to PA – PAST not available
                                                  */

  uint16_t  SyncHandle;                          /* Handle of the PA synchronization if the CAP Commander is
                                                  * synchronized. Needed to perform PAST
                                                  * 0xFFFF: CAP Commander not synchronized
                                                  */

  uint16_t PaInterval;                           /* PA Sync Interval parameter value
                                                  * 0xFFFF: PA_Interval unknown
                                                  */

  uint8_t Encryption;                            /* 0x00 if source is not encrypted
                                                  * 0x01 if source is encrypted
                                                  */

  uint32_t *pBroadcastCode;                      /* Pointer to the broadcast code to set if the source is encrypted */

  uint8_t  NumSubgroups;                         /* Number of subgroups */

  BAP_BA_Broadcast_Source_Subgroup_t *pSubgroup; /* A pointer to an array of subgroups contained in the source */
} CAP_Broadcast_AudioReceptionStart_Params_t;

/* Structure used as parameter for the CAP_Broadcast_AudioReceptionStop API */
typedef struct
{
  uint16_t ConnHandle;                           /* Connection Handle of the remote CAP Acceptor */

  uint8_t SourceID;                              /* ID of the source to modify if Operation is 0x01 */

  uint8_t  AdvAddressType;                       /* Advertiser_Address_Type for the Broadcast Source
                                                  * 0x00 = Public Device Address or Public Identity Address
                                                  * 0x01 = Random Device Address or Random (static) Identity Address
                                                  */

  uint8_t  aAdvAddress[6u];                      /* Advertiser_Address for the Broadcast Source */

  uint8_t  aBroadcastId[3u];                     /* Broadcast_ID of the Broadcast Source */

  uint16_t PaInterval;                           /* PA Sync Interval parameter value
                                                  * 0xFFFF: PA_Interval unknown
                                                  */

  uint8_t  NumSubgroups;                         /* Number of subgroups */

  BAP_BA_Broadcast_Source_Subgroup_t *pSubgroup; /* A pointer to an array of subgroups contained in the source
                                                  * bis_sync field should be set to zero
                                                  */

  uint8_t  Release;                              /* 0x00: Do not remove source from remote acceptor(s)
                                                    0x01: Remove source from remote acceptor(s)
                                                  */
} CAP_Broadcast_AudioReceptionStop_Params_t;

/* Structure used in parameter when CAP_CCP_INST_ACTIVATED_EVT event is notified*/
typedef struct
{
  uint8_t               ContentControlID;
  CCP_OptionFeatures_t  FeaturesMask;           /* Mask of Telephony Bearer Instance features
                                                 * supported on the remote device
                                                 */
} CAP_CCP_InstInfo_Evt_t;

/* Structure used in parameter when CAP_MCP_INST_ACTIVATED_EVT event is notified*/
typedef struct
{
  uint8_t                               ContentControlID;
  MCP_OptionFeatures_t                  OptionFeaturesMask;     /* Mask of Optional Media Control features
                                                                 * supported on the remote device
                                                                 */
  MCP_NotificationUpdateFeatures_t      NotifUpdateMask;        /* Mask of Notification update features
                                                                 * supported on the remote Media Control device.
                                                                 * If not activated by the local Media Controldevice,
                                                                 * the update notification is disabled by default.
                                                                 */
} CAP_MCP_InstInfo_Evt_t;

/*Structure used in parameters of the CAP_VolumeController_StartSetVolumeOffsetProcedure() API*/
typedef struct
{
  uint16_t      ConnHandle;     /* Connection Handle */
  uint8_t       VOCInst;        /* Volume Offset Control Instance */
  int16_t       VolOffset;      /* Volume Offset in range [-255;255]*/
} CAP_VCP_VolumeOffsetParms_t;

/*Structure used in parameters of the CAP_MicrophoneController_StartSetGainSettingProcedure() API*/
typedef struct
{
  uint16_t      ConnHandle;     /* Connection Handle */
  uint8_t       AICInst;        /* AIC Instance of the Audio Input */
  int8_t        GainSetting;    /* Gain Setting value */
} CAP_MICP_GainSettingParms_t;


/* Common Audio Profile Configuration used in CAP_Init() parameter */
typedef struct
{
  CAP_Role_t            Role;                   /* CAP roles supported by local device*/
  uint8_t               MaxNumLinks;            /* Maximum number of BLE Links */
  uint8_t               *pStartRamAddr;         /* Start address of the RAM buffer allocated for memory resource of
                                                 * Common Audio Profile
                                                 * It must be a 32bit aligned RAM area.
                                                 */
  uint16_t              RamSize;                /* Size of the RAM allocated at pStartRamAddr pointer.
                                                 * Computed thanks to BAP_CAP_TOTAL_BUFFER_SIZE macro
                                                 */

} CAP_Config_t;

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_TYPES_H */
