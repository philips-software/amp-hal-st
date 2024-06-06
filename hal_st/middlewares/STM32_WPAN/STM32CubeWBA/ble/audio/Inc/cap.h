/**
  ******************************************************************************
  * @file    cap.h
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
#ifndef __CAP_H
#define __CAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cap_types.h"
/* Defines -------------------------------------------------------------------*/

/* Types ---------------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* #############################################################################
   #                                COMMON APIs                                #
   ############################################################################# */

/**
  * @brief Init CAP layer based on BAP, VCP, MICP, CCP, MCP and CSIP roles and configs
  * @param  pCAP_Config: pointer on CAP Configuration
  * @param  pBAP_Config: pointer on BAP Configuration
  * @param  pVCP_Config: pointer on VCP Configuration
  * @param  pMICP_Config: pointer on MICP Configuration
  * @param  pCCP_Config: pointer on CCP Configuration
  * @param  pMCP_Config: pointer on MCP Configuration
  * @param  pCSIP_Config: pointer on CSIP Configuration
  * @retval status of the operation
  */
tBleStatus CAP_Init(const CAP_Config_t *pCAP_Config,
                    const BAP_Config_t *pBAP_Config,
                    const VCP_Config_t *pVCP_Config,
                    const MICP_Config_t *pMICP_Config,
                    const CCP_Config_t *pCCP_Config,
                    const MCP_Config_t *pMCP_Config,
                    const CSIP_Config_t *pCSIP_Config);

/**
  * @brief  Build ADV Packet for CAP Acceptor and Commander
  * @param Announcement: Targeted or General Announcement
  * @param pMetadata: pointer on the LTV-formatted Metadata
  * @param MetadataLength: size of the LTV-formatted Metadata
  * @param pAdvPacket: buffer provided by upper layer to store ADV information
  * @param AdvPacketLength: size of the buffer pointed by pAdvPacket.
  * @retval length of the built ADV Packet
  */
uint8_t CAP_BuildAdvPacket(CAP_Announcement_t Announcement,
                           uint8_t const *pMetadata,
                           uint8_t MetadataLength,
                           uint8_t *pAdvPacket,
                           uint8_t AdvPacketLength);

/**
  * @brief Perform Linkup operation on CAP, CSIP, BAP, VCP, MICP,CCP or MCP according to requested profile
  *        of the Generic Audio Framework to link.
  * @note The CAP_LINKUP_COMPLETE_EVT event is notified once the procedure is complete.
  * @param  ConnHandle: Connection handle of the remote device
  * @param  LinkMask: Mask of profiles, composing the Generic Audio Framework, to link
  * @param  CompleteLinkMode: If 1, a Complete Link process (service discovery and configuration) is performed even if
  *        the profiles information was already present in NVM, otherwise profiles are restored from NVM if it present.
  *        Values:
  *        - 0x00: NO
  *        - 0x01: YES
  * @retval status of the operation
  */
tBleStatus CAP_Linkup(uint16_t ConnHandle,GAF_Profiles_Link_t LinkMask,uint8_t CompleteLinkMode);

/**
  * @brief Indicate the profiles in ATT Client role of the Generic Audio Framework present in the Persistent Memory
  * @param Peer_Address_Type: Peer Address type
  * @param Peer_Address: Peer Address
  * @retval mask of present profiles of the Generic Audio Framework
  */
GAF_Profiles_Link_t CAP_DB_GetPresentGAFProfiles(uint8_t Peer_Address_Type,const uint8_t Peer_Address[6]);

/**
  * @brief Indicate the linked profiles of the Generic Audio Framework issued from a CAP linkup procedure
  * @note This function could not be used to get linked profiles in ATT Server role
  * @param ConnHandle: Connection handle
  * @retval mask of linked profiles of the Generic Audio Framework
  */
GAF_Profiles_Link_t CAP_GetCurrentLinkedProfiles(uint16_t ConnHandle);

/**
  * @brief Unlink the linked profiles of the Generic Audio Framework issued from a previous CAP linkup procedure
  * @param ConnHandle: Connection handle
  * @param LinkMask: Mask of profiles, composing the Generic Audio Framework, to link
  * @param NVMSave: Specify if profiles information should be stored in NVM
  *                 0x00: NO
  *                 0x01: YES
  * @retval status of the operation
  */
tBleStatus CAP_Unlink(uint16_t ConnHandle,GAF_Profiles_Link_t LinkMask,uint8_t NVMSave);

/**
  * @brief  Set supported audio context for reception and transmission
  * @note   This function is applicable only for CAP Acceptor/Commander in Unicast Server role or Scan Delegator role
  * @param  SnkContexts: bitmap of Audio Data Contexts values available for reception.
  *                      (0x0000 : device not available to receive audio for any Context Type)
  * @param  SrcContexts: bitmap of Audio Data Contexts values available for transmission.
  *                      (0x0000 : device not available to transmit audio for any Context Type)
  * @retval status of the operation
  */
tBleStatus CAP_SetSupportedAudioContexts(Audio_Context_t SnkContexts,Audio_Context_t SrcContexts);

/**
  * @brief  Set available audio context for reception and transmission.
  * @note   This function is applicable only for CAP Acceptor/Commander in Unicast Server role or Scan Delegator role
  * @note   This function is applicable only for Unicast Server role and Broadcast Sink role
  * @param  SnkContexts: bitmap of Audio Data Contexts values available for reception.
  *                      (0x0000 : device not available to receive audio for any Context Type)
  * @param  SrcContexts: bitmap of Audio Data Contexts values available for transmission.
  *                      (0x0000 : device not available to transmit audio for any Context Type)
  * @retval status of the operation
  */
tBleStatus CAP_SetAvailableAudioContexts(Audio_Context_t SnkContexts,Audio_Context_t SrcContexts);

/**
  * @brief  Set the supported Sink Audio Locations.
  * @note   This function is applicable only for CAP Acceptor/Commander in Unicast Server role or Scan Delegator role
  * @param  SnkAudioLocations: bitmap of supported Sink Audio Location values
  * @retval status of the operation
  */
tBleStatus CAP_SetSnkAudioLocations(Audio_Location_t SnkAudioLocations);

/**
  * @brief  Set the supported Source Audio Locations.
  * @note   This function is applicable only for CAP Acceptor/Commander in Unicast Server role or Scan Delegator role
  * @param  SrcAudioLocations: bitmap of supported Src Audio Location values
  * @retval status of the operation
  */
tBleStatus CAP_SetSrcAudioLocations(Audio_Location_t SrcAudioLocations);

/**
  * @brief  Set the Source Audio Locations of the remote CAP Acceptor/Commander in Unicast Server or Scan Delegator role
  * @param  ConnHandle: connection handle
  * @param  AudioLocations: bitmap of Audio Location values
  * @retval status of the operation
  */
tBleStatus CAP_SetRemoteSrcAudioLocations(uint16_t ConnHandle,Audio_Location_t AudioLocations);

/**
  * @brief  Set the Sink Audio Locations of the remote CAP Acceptor/Commander in Unicast Server or Scan Delegator role
  * @param  ConnHandle: connection handle
  * @param  AudioLocations: bitmap of Audio Location values
  * @retval status of the operation
  */
tBleStatus CAP_SetRemoteSnkAudioLocations(uint16_t ConnHandle,Audio_Location_t AudioLocations);

/**
  * @brief  Register Published Audio Capabilities Record for Audio Sink role
  * @param  pRecord: pointer on Audio PAC Record
  * @param[out]  pHandle: handle of the registered Audio PAC Record (valid if status is BLE_STATUS_SUCCESS)
  * @retval status of the operation
  */
tBleStatus CAP_RegisterSnkPACRecord(BAP_AudioPACRecord_t *pRecord,uint16_t *pHandle);

/**
  * @brief  Register Published Audio Capabilities Record for Audio Source role
  * @param  pRecord: pointer on Audio PAC Record
  * @param[out]  pHandle: handle of the registered Audio PAC Record (valid if status is BLE_STATUS_SUCCESS)
  * @retval status of the operation
  */
tBleStatus CAP_RegisterSrcPACRecord(BAP_AudioPACRecord_t *pRecord,uint16_t *pHandle);

/**
  * @brief  Update a registered Audio PAC Record for Audio Sink/Source role
  * @param  Handle: handle of the registered Audio PAC Record
  * @param  pRecord: pointer on Audio PAC Record
  * @retval status of the operation
  */
tBleStatus CAP_UpdatePACRecord(uint16_t Handle,BAP_AudioPACRecord_t *pRecord);

/**
  * @brief  Read the PAC Sink Records of the remote CAP Acceptor/Commander in Unicast Server or Scan Delegator role
  * @note   Once operation is complete, the CAP_READ_REM_SNK_PAC_RECORDS_COMPLETE_EVT event is notified to upper layer.
  *         When a record is read, the value is notified to upper layer over the CAP_REM_ACC_SNK_PAC_RECORD_INFO_EVT event.
  * @param  ConnHandle: connection handle
  * @retval status of the operation
  */
tBleStatus CAP_ReadRemoteSnkPACRecords(uint16_t ConnHandle);

/**
  * @brief  Read the PAC Source Records of the remote CAP Acceptor/Commander in Unicast Server or Scan Delegator role
  * @note   Once operation is complete, the CAP_READ_REM_SRC_PAC_RECORDS_COMPLETE_EVT event is notified to upper layer.
  *         When a record is read, the value is notified to upper layer over the CAP_REM_ACC_SRC_PAC_RECORD_INFO_EVT event.
  * @param  ConnHandle: connection handle
  * @retval status of the operation
  */
tBleStatus CAP_ReadRemoteSrcPACRecords(uint16_t ConnHandle);

/**
  * @brief  Enable the specified Audio Codec in Controller for encoding decoding audio data.
  * @note   This configuration takes effect for next audio data path setup, not if a audio data
  *         path is already setup.
  * @note   in order to check if the Audio Codec is supported by the BLE Controller, the command
  *         hci_read_local_supported_codecs_v2 could be called before enabling the
  *         audio codec in LE Controller.
  * @param  pAudioCodecList : Pointer on Audio Codec List to enable in BLE Controller
  *         (this pointer shall be valid during application execution)
  */
tBleStatus CAP_EnableAudioCodecController(BAP_AudioCodecController_t *pAudioCodecList);

/**
  * @brief Register Sink/Source AUdio Channels for Unicast Server
  * @param  NumSinkASEs: number of Sink Audio Stream Endpoints to register
  * @param  NumSourceASEs: number of Sink Audio Stream Endpoints to register
  * @retval status of the operation
  */
tBleStatus CAP_RegisterAudioChannels(uint16_t NumSinkASEs,uint16_t NumSourceASEs);

/**
  * @brief  Remove the record of the services in Basic Audio Profile stored in the Non Volatile memory.
  * @param  PeerIdentityAddressType: Identity address type.
  *                                  Values:
  *                                     - 0x00: Public Identity Address
  *                                     - 0x01: Random (static) Identity Address
  * @param  PeerIdentityAddress : Public or Random (static) Identity address of the peer device
  * @param  BAP_Role : BAP role services records to remove
  * @retval status of the operation
  */
tBleStatus CAP_RemoveBAPServicesRecord(uint8_t PeerIdentityAddressType,
                                       const uint8_t* PeerIdentityAddress,
                                       BAP_Role_t BAP_Role);

/**
  * @brief Check if a remote audio capability matches with a list of codec specific configuration
  * @param  NumConf: Number of items in the codec specific configuration list
  * @param  pLocalSpecificConf: A list of codec Specific Configuration
  * @param  pRemoteAudioCap: A Remote Audio Capability
  * @param  pIsCompatible: a pointer to a variable containing the output result of the function
  * @retval BLE_STATUS_SUCCESS if the process was successful, BLE_STATUS_ERROR if an error was found in the remote audio
  *         capability
  */
tBleStatus CAP_CheckCodecSpecificCapabilitiesCompatibility(uint8_t NumConf,
                                                           CAP_Codec_Specific_Configuration_t *pLocalSpecificConf,
                                                           BAP_AudioCapabilities_t *pRemoteAudioCap,
                                                           uint8_t *pIsCompatible);

/* #############################################################################
   #                               UNICAST APIs                                #
   ############################################################################# */

/**
  * @brief Perform the Unicast Audio Start Procedure
  * @param SetType: Set Type (Ad-Hoc or Coordinated Set)
  * @param NumAcceptors: Number of CAP Acceptors
  * @param pStartStreamParams: Table of Start Streams Parameters for each CAP Acceptors
  * @retval status of the operation
  */
tBleStatus CAP_Unicast_AudioStart(CAP_Set_Acceptors_t SetType,
                                  uint8_t NumAcceptors,
                                  CAP_Unicast_AudioStart_Stream_Params_t *pStartStreamParams);

/**
  * @brief Perform the Unicast Audio Update procedure
  * @note This procedure is used to change the Context Type values and/or CCID values associated with one or more
  *       unicast Audio Streams.
  * @note The value of the Streaming Audio Contexts LTV must have at least one bit set.
  * @note This procedure shall only be used if the BAP Configuration (PAC Records, Audio Streams) remains the same on
  *       the CAP Acceptor involved in the new use case. Otherwise, the Unicast Audio Stop procedure and
  *       the Unicast Audio Start procedure shall be used.
  * @param NumAcceptors: Number of CAP Acceptors
  * @param pUpdateStreamParams: Table of Update Streams Parameters for each CAP Acceptors
  * @retval status of the operation
  */
tBleStatus CAP_Unicast_AudioUpdate(uint8_t NumAcceptors,
                                   CAP_Unicast_AudioUpdate_Stream_Params_t *pUpdateStreamParams);

/**
  * @brief Perform the Unicast Audio Stop procedure
  * @param NumAcceptors: Number of CAP Acceptors
  * @param pConnHandle: pointer on the table of the remote CAP Acceptors Connection Handle
  * @param Release: 0x01 to release the remote ASEs, 0x00 to only Disable them
  * @retval status of the operation
  */
tBleStatus CAP_Unicast_AudioStop(uint8_t NumAcceptors,
                                 uint16_t *pConnHandle,
                                 uint8_t Release);

/**
  * @brief Terminate Audio Stream
  * @note An Acceptor can terminate an Audio Stream at any time, including when it becomes unavailable for currently
  *       streaming Context Type values.
  * @param ConnHandle: Connection Handle of the remote device
  * @retval status of the operation
  */
tBleStatus CAP_Unicast_TerminateAudioStream(uint16_t ConnHandle);

/**
  * @brief  Get Audio Stream Endpoint information
  * @note   This function is used in CAP Acceptor role (Unicast Server) and CAP Initiator (Unicast Client)
  * @param  ACL_ConnHandle: ACL connection handle
  * @param  ASE_ID: Audio Stream Endpoint Identifier
  * @retval Pointer on BAP_ASE_Info_t structure including ASE information
  */
BAP_ASE_Info_t *CAP_Unicast_GetASEInfo(uint16_t ACL_ConnHandle,uint8_t ASE_ID);

/**
  * @brief  Setup Audio Data Path associated to a specified Unicast ASE ID
  * @note   Once Audio Data Path setup is complete, the CAP_AUDIO_CONNECTION_UP_EVT event is notified.
  * @note   This function is used in CAP Acceptor role (Unicast Server) and CAP Initiator (Unicast Client)
  * @param  CIS_ConnHandle: CIS connection handle
  * @param  ASE_ID: Audio Stream Endpoint Identifier
  * @param  DataPathID: Audio Data Path ID for Path Directory associated to ASE ID
  *                     0x00 : HCI
  *                     0x01-0xFE : Logical_Channel_Number (vendor specific)
  * @param  ControllerDelay: Controller delay in microseconds
  *                         Range: 0x000000 to 0x3D0900
  *                         Time range: 0 s to 4 s
  * @param  VendorSpecificConfigLength: length of the Length of the vendor-specific configuration data
  * @param  pVendorSpecificConfig: Additional vendor-specific configuration information that a Host may
  *                                provide to the Controller in case of Audio Data Path is a non-HCI
  *                                transport data path.
  * @retval status of the operation
  */
tBleStatus CAP_Unicast_SetupAudioDataPath(uint16_t CIS_ConnHandle,
                                          uint8_t ASE_ID,
                                          uint8_t DataPathID,
                                          uint32_t ControllerDelay,
                                          uint8_t VendorSpecificConfigLength,
                                          const uint8_t* pVendorSpecificConfig);
/* #############################################################################
   #                               BROADCAST APIs                              #
   ############################################################################# */

/**
  * @brief Perform a Broadcast Audio Start procedure
  * @param pAudioStartParams: A pointer to the Broadcast Audio Start Parameters Structure
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_AudioStart(CAP_Broadcast_AudioStart_Params_t *pAudioStartParams);

/**
  * @brief Perform the Broadcast Audio Stop procedure
  * @param BigHandle: Handle of the BIG of the Broadcast Source to stop
  * @param Release: 0x00 to perform Disable operation, 0x01 to perform Release operation
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_AudioStop(uint8_t BigHandle, uint8_t Release);

/**
  * @brief Perform the Broadcast Audio Update procedure
  * @param AdvHandle: Handle of the periodic advertising of the broadcast source
  * @param pBASEGroup: Pointer to the BASE Group for the broadcast source
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_AudioUpdate(uint8_t AdvHandle, BAP_BASE_Group_t *pBASEGroup);

/**
  * @brief Start the parsing of Extended Advertising Reports to look for Broadcast UUIDs and generate
  *        CAP_BROADCAST_SOURCE_ADV_REPORT_EVT events
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_StartAdvReportParsing(void);

/**
  * @brief Stop the parsing of Extended Advertising Reports
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_StopAdvReportParsing(void);

/**
  * @brief Synchronize to the periodic advertising train of the given device to discover more details about the
  *   related audio stream
  * @param AdvSid: ahe Advertising Set ID of the periodic advertising train to synchronize to
  * @param pAdvAddress: a pointer to the address of the advertising device
  * @param pAddressType: Type of the peer address: 0x00 for Public, 0x01 for Random
  * @param Skip: Number of Periodic Advertising Events skippable
  * @param SyncTimeout: Synchronization Timeout
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_StartPASync(uint8_t AdvSid,
                                     const uint8_t *pAdvAddress,
                                     uint8_t pAddressType,
                                     uint16_t Skip,
                                     uint16_t SyncTimeout);

/**
  * @brief Synchronize to a BIG and start receiving data from it
  * @param BigHandle: the handle of the BIG
  * @param SyncHandle: The handle of the periodic advertising train related to the BIG
  * @param pBisIndex: an array of the BIS index of the BIG to synchronize to
  * @param NumBis: the number of BIS to synchronize
  * @param pGroup: a pointer to the BASE group related
  * @param SubgroupIndex: The index of the subgroup being synced
  * @param Encryption: 0x00 to disable encryption, 0x01 to enable it
  * @param BroadcastCode: Broadcast code for the encryption
  * @param MSE: Maximum of Sub Events Used
  * @param Timeout: BIG Synchronization Timeout
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_StartBIGSync(uint8_t BigHandle,
                                      uint16_t SyncHandle,
                                      uint8_t *pBisIndex,
                                      uint8_t NumBis,
                                      BAP_BASE_Group_t *pGroup,
                                      uint8_t SubgroupIndex,
                                      uint8_t Encryption,
                                      uint32_t *BroadcastCode,
                                      uint8_t MSE,
                                      uint16_t Timeout);

/**
  * @brief Stop the synchronization to a periodic advertising train
  * @param SyncHandle: The handle of the Periodic Advertising train
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_StopPASync(uint16_t SyncHandle);

/**
 * @brief Cancel the tentative for synchronization to a periodic advertising train
 */
tBleStatus CAP_Broadcast_CancelStartPASync();

/**
  * @brief Terminate the synchronization to a BIG
  * @param BigHandle: the handle of the BIG to terminate
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_StopBIGSync(uint8_t BigHandle);

/**
  * @brief Parse a periodic advertising data payload and fill the group structure
  * @param pData: a pointer to the periodic advertising data payload to parse
  * @param DataLen: the length of the periodic advertising data payload
  * @param pGroup: a pointer to the group structure to fill
  * @param pEndIndex: a pointer to the variable which will contain the index of the data following the group data parsed
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_ParseBASEGroup(uint8_t *pData,
                                        uint8_t DataLen,
                                        BAP_BASE_Group_t *pGroup,
                                        uint8_t *pEndIndex);

/**
  * @brief Parse a periodic advertising data payload and fill the subgroup structure
  * @param pData: a pointer to the periodic advertising data payload to parse
  * @param DataLen: the length of the periodic advertising data payload
  * @param pSubgroup: a pointer to the subgroup structure to fill
  * @param pEndIndex: a pointer to the variable which will contain the index of the data following the subgroup
  *                   data parsed
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_ParseBASESubgroup(uint8_t *pData,
                                           uint8_t DataLen,
                                           BAP_BASE_Subgroup_t *pSubgroup,
                                           uint8_t *pEndIndex);

/**
  * @brief Parse a periodic advertising data payload and fill the BIS structure
  * @param pData: a pointer to the periodic advertising data payload to parse
  * @param DataLen: the length of the periodic advertising data payload
  * @param pBis: a pointer to the BIS structure to fill
  * @param pEndIndex: a pointer to the variable which will contain the index of the data following the BIS data parsed
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_ParseBASEBIS(uint8_t *pData,
                                      uint8_t DataLen,
                                      BAP_BASE_BIS_t *pBis,
                                      uint8_t *pEndIndex);

/**
  * @brief Used by the BAP application for reading supported controller delay range function of the BIS number
  * @param NumBis: number of BIS
  * @param Direction: direction of the data path; input or output
  * @param pCodecId : pointer to the array for CODEC ID with first byte equal to the coding format
  * @param pCtrlDelayMin: pointer to the returned minimum value adjusted in function of the number of BIS
  * @param pCtrlDelayMax: pointer to the returned maximum value
  * @retval supported delay value
  */
uint8_t CAP_Broadcast_ReadSupportedControllerDelay(uint8_t NumBis,
                                                   uint8_t Direction,
                                                   uint8_t *pCodecId,
                                                   uint32_t *pCtrlDelayMin,
                                                   uint32_t *pCtrlDelayMax);

/**
  * @brief Used by the BAP application for setting the data path, mandatory step for enabling the audio
  * @param NumBis: number of BIS
  * @param pConnHandle: pointer to the connection handle array (size of NumBis)
  * @param Direction: direction of the data path; input or output
  * @param pCodecId : pointer to the array for CODEC ID with first byte equal to the coding format
  * @param CtrlDelay: controller delay, same value for each data path
  * @param pCodecId : pointer to the array for CODEC ID with first byte equal to the coding format
  * @param PathID: path ID, 0 for HCI
  * @param VendorSpecificConfigLength: length of the Length of the vendor-specific configuration data
  * @param pVendorSpecificConfig: Additional vendor-specific configuration information that a Host may
  *                               provide to the Controller in case of Audio Data Path is a non-HCI
  *                               transport data path.
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_SetupAudioDataPath(uint8_t NumBis,
                                             uint16_t *pConnHandle,
                                             uint8_t Direction,
                                             uint8_t *pCodecId,
                                             uint32_t CtrlDelay,
                                             uint8_t PathID,
                                             uint8_t VendorSpecificConfigLength,
                                             const uint8_t* pVendorSpecificConfig);

/**
  * @brief Build ADV Packet for Scan Delegator to request Broadcast Source Discovery to Broadcast Assistants
  * @note  The provided packet must be set in Advertising Packet to request remote Broadcast Assistants
  *        to start Broadcast Source discovery.
  * @param pAdvPacket: buffer provided by upper layer to store ADV information
  * @param AdvPacketLength: size of the buffer pointed by pAdvPacket.
  * @retval BLE_STATUS_SUCCESS if aAdvPacket buffer is correctly filled,
  *         HCI_COMMAND_DISALLOWED_ERR_CODE if Scan Delegator role is not supported or activated
  */
uint8_t CAP_Broadcast_BuildSolicitationRequestAdvPacket(uint8_t *pAdvPacket, uint8_t AdvPacketLength);

/**
  * @brief As an Acceptor with Scan Delegator Role, accept the request to add a Broadcast Source from a remote Broadcast
  *        Assistant
  * @note  This function could be called after a BAP_SDE_ADD_SOURCE_REQ_EVT event notification.
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_AcceptAddBroadcastSource(void);

/**
  * @brief As an Acceptor with Scan Delegator Role, Reject the request to add a Broadcast Source from a remote Broadcast
  *        Assistant
  * @note  This function could be called after a BAP_SDE_ADD_SOURCE_REQ_EVT event notification.
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_RejectAddBroadcastSource(void);

/**
  * @brief As an Acceptor with Scan Delegator Role, Accept the request to Modify a Broadcast Source from a remote
  *        Broadcast Assistant
  * @note  This function could be called after a BAP_SDE_MODIFY_SOURCE_REQ_EVT event notification.
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_AcceptModifyBroadcastSource(void);

/**
  * @brief As an Acceptor with Scan Delegator Role, Reject the request to modify a Broadcast Source from a remote
  *        Broadcast Assistant
  * @note  This function could be called after a BAP_SDE_MODIFY_SOURCE_REQ_EVT event notification.
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_RejectModifyBroadcastSource(void);

/**
  * @brief As an Acceptor with Scan Delegator Role, add a Broadcast source manually to the BASS
  * @param AdvertisingSID: SID of the advertiser
  * @param AdvertisingAddressType: Address type of the advertiser
  * @param pAdvertiserAddress: a pointer to the address of the advertiser
  * @param BroadcastID: The broadcast ID of the source
  * @param Encryption: The encryption type of the source
  * @param pBroadcastCode: The broadcast code of the source, if known
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_AddSourceToBASS(uint8_t AdvertisingSID,
                                         uint8_t AdvertisingAddressType,
                                         const uint8_t *pAdvertiserAddress,
                                         uint32_t BroadcastID,
                                         BAP_Broadcast_Source_Encryption_t Encryption,
                                         uint32_t *pBroadcastCode);

/* #############################################################################
   #                          BROADCAST ASSISTANT APIs                         #
   ############################################################################# */

/**
  * @brief Start parsing Extended Advertising report to search for Scan Delagors solliciting assistance and Broadcast
  *        Sources on behalf a connected Scan Delegator
  * @param ConnHandle: The handle of the connection to the remote Scan Delegator. If no Scan Delegator is currently
  *                    connected, ignore this parameter
  * @retval status of the operation
  */
tBleStatus CAP_BroadcastAssistant_StartAdvReportParsing(uint16_t ConnHandle);

/**
  * @brief Stop parsing Extended Advertising report to search for Scan Delagors solliciting assistance and Broadcast
  *        Sources on behalf a connected Scan Delegator
  * @param ConnHandle: The handle of the connection to the remote Scan Delegator. If no Scan Delegator is currently
  *                    connected, ignore this parameter
  * @retval status of the operation
  */
tBleStatus CAP_BroadcastAssistant_StopAdvReportParsing(uint16_t ConnHandle);

/**
  * @brief Perform the Audio Reception Start Procedure to request remote acceptor(s) to synchronize to a Broadcast Source
  * @param SetType: Set Type (Ad-Hoc or Coordinated Set)
  * @param NumAcceptors: Number of CAP Acceptors
  * @param pAudioReceptionStart: Table of CAP_Broadcast_AudioReceptionStart_Params_t structures containing
                                 details about the Broadcast Source for each CAP Acceptors
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_AudioReceptionStart(CAP_Set_Acceptors_t SetType,
                                             uint8_t NumAcceptors,
                                             CAP_Broadcast_AudioReceptionStart_Params_t *pAudioReceptionStart);

/**
  * @brief Perform the Audio Reception Stop Procedure to request remote acceptor(s) to desynchronize from a Broadcast Source
  * @param SetType: Set Type (Ad-Hoc or Coordinated Set)
  * @param NumAcceptors: Number of CAP Acceptors
  * @param pAudioReceptionStop: Table of CAP_Broadcast_AudioReceptionStop_Params_t structures containing
                                 details about the Broadcast Source for each CAP Acceptors
  * @retval status of the operation
  */
tBleStatus CAP_Broadcast_AudioReceptionStop(CAP_Set_Acceptors_t SetType,
                                             uint8_t NumAcceptors,
                                             CAP_Broadcast_AudioReceptionStop_Params_t *pAudioReceptionStop);

/**
  * @brief Synchronize with a Periodic Advertising train to discover BASE info about a broadcast source
  * @param AdvSid: Advertising SID of the periodic advertising train to synchronize
  * @param pAdvAddress: Pointer to the address of the device doing periodic advertising
  * @param pAddressType: Type of the peer address: 0x00 for Public, 0x01 for Random
  * @param Skip: Number of Periodic Advertising Events skippable
  * @param SyncTimeout: Synchronization Timeout
  * @retval status of the operation
  */
tBleStatus CAP_BroadcastAssistant_StartPASync(uint8_t AdvSid,
                                              const uint8_t *pAdvAddress,
                                              uint8_t pAddressType,
                                              uint16_t Skip,
                                              uint16_t SyncTimeout);

/**
  * @brief Stop the synchronization to a periodic advertising train
  * @param SyncHandle: The handle of the Periodic Advertising train
  * @retval status of the operation
  */
tBleStatus CAP_BroadcastAssistant_StopPASync(uint16_t SyncHandle);

/* #############################################################################
   #                             SUB-PROFILES APIs                             #
   ############################################################################# */

/**
  * @brief  Register the Generic Telephone Bearer
  * @note   Supported only if CAP Initiator role is initialized.
  * @note   Registration of the Generic Telephone Bearer is mandatory if Call Control in Server role is initialized
  * @param  ContentControlID: unique Content Control Identifier across Content Control Services (CCP and MCP)
  * @param  pBearerInfo: pointer on Telephony Brearer Information settings for registration
  * @retval status of the registration
  */
tBleStatus CAP_RegisterGenericTelephonyBearer(uint8_t ContentControlID,CCP_BearerInit_t *pBearerInfo);

/**
  * @brief  Register the Telephone Bearer Instance
  * @note   Supported only if CAP Initiator role is initialized.
  * @note   Registration of the Telephone Bearer Instance is optional if Call Control in Server role is initialized
  * @param  ContentControlID: unique Content Control Identifier across Content Control Services (CCP and MCP)
  * @param  pBearerInfo: pointer on Telephony Brearer Information settings for registration
  * @retval status of the registration
  */
tBleStatus CAP_RegisterTelephonyBearerInstance(uint8_t ContentControlID, CCP_BearerInit_t *pBearerInfo);

/**
  * @brief  Register the Generic Media Player
  * @note   Supported only if CAP Initiator role is initialized.
  * @note   Registration of the Generic Media Player is mandatory if Media Control in Server role is initialized
  * @param  ContentControlID: unique Content Control Identifier across Content Control Services (MCP and CCP)
  * @retval status of the registration
  */
tBleStatus CAP_RegisterGenericMediaPlayer(uint8_t ContentControlID);

/**
  * @brief  Register the Media Player Instance
  * @note   Supported only if CAP Initiator role is initialized.
  * @note   Registration of the Media Player Instance is optional if Media Control in Server role is initialized
  * @param  ContentControlID: unique Content Control Identifier across Content Control Services (MCP and CCP)
  * @retval status of the registration
  */
tBleStatus CAP_RegisterMediaPlayerInstance(uint8_t ContentControlID);

/**
  * @brief  Register the Coordinated Set Member in the CAP
  * @note   Supported only if CAP Acceptor role is initialized.
  * @note   Registration of the Coordinated Set Member Instance is mandatory if the device is a member of a Coordinated Set
  * @param  Instance_ID: Coordinate Set Identification Instance of the Set Member
  * @retval status of the registration
  */
tBleStatus CAP_RegisterCoordinatedSetMember(uint8_t Instance_ID);

/**
  * @brief  Start procedure to change the volume level on the Acceptors of the Coordinated Set acting in the VCP Volume
  *         Renderer role.
  * @note   A Commander acting in the VCP Volume Controller role uses this procedure changes the volume
  *         level on the Acceptors of the Coordinated Set acting in the VCP Volume Renderer role.
  * @note   This CAP procedure is based on VCP_CONTROLLER_SetAbsVolume() calls procedure for all the Acceptors
  *         of the Coordinated Set
  * @note   The VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT event will be notified for each Acceptor of the Coordinated Set
  *         for which the volume has changed.
  * @note   The CAP_SET_VOLUME_PROCEDURE_COMPLETE_EVT event will be notified once the procedure is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  AbsVolume: Volume Setting
  * @retval status of the operation
  */
tBleStatus CAP_VolumeController_StartSetVolumeProcedure(uint16_t ConnHandle,uint8_t AbsVolume);

/**
  * @brief  Start the procedure to change the (Volume) mute state on the Acceptors of the Coordinated Set acting
  *         in the VCP Volume Renderer role.
  * @note   A Commander acting in the VCP Volume Controller role uses this procedure to change the (Volume) mute state
  *         on the Acceptors of the Coordinated Set acting in the VCP Volume Renderer role.
  * @note   This CAP procedure is based on VCP_CONTROLLER_Unmute()/VCP_CONTROLLER_Mute calls procedure for all the
  *         Acceptors of the Coordinated Set
  * @note   The VCP_CONTROLLER_UPDATED_VOLUME_STATE_EVT event will be notified for each Acceptor of the Coordinated Set
  *         for which the mute state has changed.
  * @note   The CAP_SET_VOLUME_MUTE_STATE_PROCEDURE_COMPLETE_EVT event will be notified once the procedure is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  MuteState: Mute State (0 : UnMute, 1 : Mute)
  * @retval status of the operation
  */
tBleStatus CAP_VolumeController_StartSetVolumeMuteStateProcedure(uint16_t ConnHandle,uint8_t MuteState);

/**
  * @brief  Start procedure to set the Volume Offset relative to the volume level set by the Change Volume procedure
  *         on the Acceptors of the Coordinated Set acting in the VCP Renderer role.
  * @note   A Commander acting in the VCP Volume Controller role uses this procedure changes the Volume Offset
  *         on the Acceptors of the Coordinated Set acting in the VCP Renderer role.
  * @note   This CAP procedure is based on VCP_CONTROLLER_SetVolumeOffset() calls procedure for all the
  *         Acceptors of the Coordinated Set
  * @note   The VCP_CONTROLLER_VOLUME_OFFSET_STATE_EVT event will be notified for each Acceptor of the Coordinated Set
  *         for which the Volume Offset value changed.
  * @note   The CAP_SET_VOLUME_OFFSET_PROCEDURE_COMPLETE_EVT event will be notified once the procedure is complete.
  * @param  pVolumeOffsetParms: Table of Volume Offset and VOC Instance of the CAP Acceptors
  * @param  NumAcceptors:Number of CAP Acceptors composing the Coordinated Set and so the table of pVolumeOffsetParms
  */
tBleStatus CAP_VolumeController_StartSetVolumeOffsetProcedure(CAP_VCP_VolumeOffsetParms_t *pVolumeOffsetParms,
                                                              uint8_t NumAcceptors);

/**
  * @brief  Start the procedure to change the Microphone mute value on the Acceptors of the Coordinated Set acting
  *         in the MICP Microphone Device role.
  * @note   A Commander acting in the MICP Microphone Controller role uses this procedure to change the Mute state
  *         on the Acceptors of the Coordinated Set acting in the MICP Microphone Device role.
  * @note   This CAP procedure is based on MICP_CONTROLLER_SetMute() call procedure for all the
  *         Acceptors of the Coordinated Set
  * @note   The MICP_CONTROLLER_UPDATED_MUTE_EVT event will be notified for each Acceptor of the Coordinated Set
  *         for which the mute state has changed.
  * @note   The CAP_SET_MICROPHONE_MUTE_PROCEDURE_COMPLETE_EVT event will be notified once the procedure is complete.
  * @param  ConnHandle: ACL connection handle
  * @param  Mute: Mute value (0x00 = Unmuted, 0x01 = Muted)
  * @retval status of the operation
  */
tBleStatus CAP_MicrophoneController_StartSetMuteProcedure(uint16_t ConnHandle, Microphone_Mute_t Mute);

/**
  * @brief  Start procedure to set the microphone gain setting on the Acceptors of the Coordinated Set acting in the
  *         MICP Device role.
  * @note   A Commander acting in the MICP Microphone Controller role uses this procedure changes the Gain Setting
  *         on the Acceptors of the Coordinated Set acting in the MICP Device role.
  * @note   This CAP procedure is based on MICP_CONTROLLER_SetAudioInputGainSetting() calls procedure for all the
  *         Acceptors of the Coordinated Set
  * @note   The MICP_DEVICE_UPDATED_AUDIO_INPUT_STATE_EVT event will be notified for each Acceptor of the Coordinated Set
  *         for which the Gain Setting changed.
  * @note   The CAP_SET_MICROPHONE_GAIN_SETTING_PROCEDURE_COMPLETE_EVT event will be notified once the procedure is complete.
  * @param  pGainSettingParms: Table of Microphone Gain Settings and AIC Instance of the CAP Acceptors
  * @param  NumAcceptors:Number of CAP Acceptors composing the Coordinated Set and so the table of pGainSettingParms
  */
tBleStatus CAP_MicrophoneController_StartSetGainSettingProcedure(CAP_MICP_GainSettingParms_t *pGainSettingParms,
                                                                 uint8_t NumAcceptors);

/**
  * @brief  Start Procedure to discover Set Members of the Coordinated Set in with specified device is member
  * @note   A gap discovery procedure ( see aci_gap_start_general_discovery_proc()) shall be in progress
  *         when the function is called.
  * @note : The CSIP_COO_ADV_REPORT_NEW_SET_MEMBER_DISCOVERED_EVT event is reported when a new Set Member is discovered,
  *         the application is responsible to perform gap connection to connect with the new Set Member.
  * @param  ConnHandle: Connection handle of a member of the Coordinated Set
  * @retval status of the procedure
  */
tBleStatus CAP_StartCoordinatedSetMemberDiscoveryProcedure(uint16_t ConnHandle);

/**
  * @brief  Stop Procedure to discover Set Members of the Coordinated Set in with specified device is member
  * @param  ConnHandle: Connection handle of a member of the Coordinated Set
  * @retval status of the procedure
  */
tBleStatus CAP_StopCoordinatedSetMemberDiscoveryProcedure(uint16_t ConnHandle);

/**
  * @brief  Notify CAP Event
  * @param  pNotification: pointer on notification information
  */
void CAP_Notification(CAP_Notification_Evt_t *pNotification);

#ifdef __cplusplus
}
#endif

#endif /* __CAP_H */
