/**
  ******************************************************************************
  * @file    bap_types.h
  * @author  MCD Application Team
  * @brief   This file contains definitions used for Basic Audio Profile
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
#ifndef __BAP_TYPES_H
#define __BAP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "audio_types.h"



/* Types ------------------------------------------------------------------*/
/* Types of roles of the Basic Audio Profile */
typedef uint8_t BAP_Role_t;
#define BAP_ROLE_UNICAST_SERVER			(0x01)
#define BAP_ROLE_UNICAST_CLIENT			(0x02)
#define BAP_ROLE_BROADCAST_SOURCE		(0x04)
#define BAP_ROLE_BROADCAST_SINK			(0x08)
#define BAP_ROLE_BROADCAST_ASSISTANT		(0x10)
#define BAP_ROLE_SCAN_DELEGATOR			(0x20)

/* Types of Audio roles */
typedef uint8_t Audio_Role_t;
#define AUDIO_ROLE_SINK				(0x01)
#define AUDIO_ROLE_SOURCE			(0x02)

/* Types of Announcement used by Unicast Server in its advertising Packet */
typedef uint8_t BAP_Unicast_Announcement_t;
#define BAP_UNICAST_GENERAL_ANNOUNCEMENT        (0x00)  /* Unicast Server is connectable and
                                                         * is not requesting a connection.
                                                         */
#define BAP_UNICAST_TARGETED_ANNOUNCEMENT       (0x01)  /* Unicast Server is connectable and
                                                         * is requesting a connection.
                                                         */

/* Types of Announcement used by Unicast Server in its advertising Packet */
typedef uint8_t BAP_Audio_Path_Direction_t;
#define BAP_AUDIO_PATH_INPUT                    (0x00)  /* Audio Data packets are sent to the remote device*/
#define BAP_AUDIO_PATH_OUTPUT                   (0x01)  /* Audio Data packets are received from the remote device.*/

/* Type used to specify the kind of encryption for adding a Broadcast Source to the BASS */
typedef uint8_t BAP_Broadcast_Source_Encryption_t;
#define BAP_BROADCAST_SOURCE_NOT_ENCRYPTED          (0x00) /* If added source is not encrypted or if status is unknown */
#define BAP_BROADCAST_SOURCE_ENCRYPTED_CODE_KNOWN   (0x01) /* If added source is encrypted and the broadcast code is known */
#define BAP_BROADCAST_SOURCE_ENCRYPTED_CODE_UNKNOWN (0x02) /* If added source is encrypted and the broadcast code is unknown */

/* Configuration settings of Audio Capabilities in Server role structure.
 * These configuration settings are required by Unicast server and Broadcast Sink role.
 */
typedef struct
{
  uint8_t       MaxNumSnkPACRecords;            /* Maximum Number of Sink PAC Records supported by Unicast server and
                                                 * Broadcast Sink roles
                                                 */
  uint8_t       MaxNumSrcPACRecords;            /* Maximum Number of Source PAC Records supported by Unicast server and
                                                 * Broadcast Sink roles
                                                 */
  uint8_t       *pStartRamAddr;                 /* Start address of the RAM buffer allocated for memory resource of
                                                 * Audio Capabilities in Server role
                                                 * It must be a 32bit aligned RAM area.
                                                 */
  uint16_t      RamSize;                        /* Size of the RAM allocated at pStartRamAddr pointer.
                                                 * Computed thanks to BAP_PACS_SRV_TOTAL_BUFFER_SIZE macro
                                                 */
} BAP_AudioCapSrvConfig_t;

/* Configuration settings of Audio Capabilities in Client role structure.
 * These configuration settings are required by Unicast Client and Broadcast Assistant role.
 */
typedef struct
{
  uint8_t       MaxNumSnkPACRecordsPerLink;     /* Maximum Number of Sink PAC Records per Link supported by
                                                 * Unicast Client and Broadcast Assistant
                                                 */
  uint8_t       MaxNumSrcPACRecordsPerLink;     /* Maximum Number of Source PAC Records per Link supported by
                                                 * Unicast Client and Broadcast Assistant
                                                 */
  uint8_t       *pStartRamAddr;                 /* Start address of the RAM buffer allocated for memory resource of
                                                 * Audio Capabilities in Client role
                                                 * It must be a 32bit aligned RAM area.
                                                 */
  uint16_t      RamSize;                        /* Size of the RAM allocated at pStartRamAddr pointer
                                                 * Computed thanks to BAP_PACS_CLT_TOTAL_BUFFER_SIZE macro
                                                 */
} BAP_AudioCapCltConfig_t;

/* Configuration settings of Audio Stream Control in Server role structure.
 * These configuration settings are required by Unicast server .
 */
typedef struct
{
  Audio_Role_t  AudioRole;              /* Audio Role (Sink and/or Source) of Unicast server*/
  uint8_t       MaxNumSnkASEs;          /* Maximum Number of Sink Audio Stream Endpoints supported by
                                         * Unicast server per connection */
  uint8_t       MaxNumSrcASEs;          /* Maximum Number of Source Audio Stream Endpoints supported by
                                         * Unicast server per connection */
  uint8_t       MaxCodecConfSize;       /* Maximum Size of the buffer allocated for each ASE to store
                                         * its Codec Specific Configuration
                                         */
  uint8_t       MaxMetadataLength;      /* Maximum Length of the buffer allocated for each ASE to store its
                                         * associated Metadata
                                         */
  uint8_t       *pStartRamAddr;         /* Start address of the RAM buffer allocated for memory resource of
                                         * Audio Stream Control
                                         * It must be a 32bit aligned RAM area.
                                         */
  uint16_t      RamSize;                /* Size of the RAM allocated at pStartRamAddr pointer
                                         * Computed thanks to BAP_ASCS_SRV_TOTAL_BUFFER_SIZE macro
                                         */
} BAP_AudioStreamCtrlSrvConfig_t;

/* Configuration settings of Audio Stream Control in Client role structure.
 * These configuration settings are required by Unicast Client .
 */
typedef struct
{
  Audio_Role_t  AudioRole;              /* Audio Role (Sink and/or Source) of Unicast Client*/
  uint8_t       MaxNumSnkASEs;          /* Maximum Number of Sink Audio Stream Endpoints supported by
                                         * Unicast Client per connection
                                         */
  uint8_t       MaxNumSrcASEs;          /* Maximum Number of Source Audio Stream Endpoints supported by
                                         * Unicast Client per connection
                                         */
  uint8_t       MaxCodecConfSize;       /* Maximum Size of the buffer allocated for each ASE to store
                                         * its Codec Specific Configuration
                                         */
  uint8_t       MaxMetadataLength;      /* Maximum Length of the buffer allocated for each ASE to
                                         * store its associated Metadata
                                         */
  uint8_t       *pStartRamAddr;         /* Start address of the RAM buffer allocated for memory resource of
                                         * Audio Stream Control
                                         * It must be a 32bit aligned RAM area.
                                         */
  uint16_t      RamSize;                /* Size of the RAM allocated at pStartRamAddr pointer
                                         * Computed thanks to BAP_ASCS_CLT_TOTAL_BUFFER_SIZE macro
                                         */
} BAP_AudioStreamCtrlCltConfig_t;

/* Configuration settings of Isochronous Channels Management.
 * These configuration settings are required by Unicast Client and Unicast Server.
 */
typedef struct
{
   uint8_t      MaxNumCIG;              /* Maximum Number of supported CIGs
                                         * (shall not exceed the LE Controller capacity)
                                         */
  uint8_t       MaxNumCISPerCIG;        /* Maximum Number of supported CISs per CIG
                                         * (shall not exceed the LE Controller capacity)
                                         */
   uint8_t      MaxNumBIG;              /* Maximum Number of supported BIGs
                                         * (shall not exceed the LE Controller capacity)
                                         */
  uint8_t       MaxNumBISPerBIG;        /* Maximum Number of supported BISs per BIG
                                         * (shall not exceed the LE Controller capacity)
                                         */
  uint8_t       *pStartRamAddr;         /* Start address of the RAM buffer allocated for memory resource of
                                         * Isochronous Channel management.
                                         * It must be a 32bit aligned RAM area.
                                         */
  uint16_t      RamSize;                /* Size of the RAM allocated at pStartRamAddr pointer
                                         * Computed thanks to BAP_MEM_BLOCKS_CIG_SIZE_BYTES macro
                                         */
} BAP_ISOChannelConfig_t;

/* Configuration settings of Non-Volatile Memory Management for BAP Services restoration.
 * These configuration settings are required by Unicast Client, Unicast Server, Scan Delegator and Broadcast Assistant.
 */
typedef struct
{
  uint8_t       *pStartRamAddr;                 /* Start address of the RAM buffer allocated for memory resource of
                                                 * Non-Volatile Memory Management process.
                                                 * It must be a 32bit aligned RAM area.
                                                 */
  uint16_t      RamSize;                        /* Size of the RAM allocated at pStartRamAddr pointer.
                                                 * Computed thanks to BAP_NVM_MGMT_PER_CONN_SIZE_BYTES macro
                                                 */
} BAP_NvmMgmtConfig_t;

/* Configuration settings of Broadcast Audio Scan in Client role structure.
 * These configuration settings are required by Broadcast Assistant .
 */
typedef struct
{
  uint8_t       MaxNumBSRCInfo;         /* Maximum Number of Broadcast Source Information contexts supported by
                                         * Broadcast Assistant per connection
                                         */
  uint8_t       *pStartRamAddr;         /* Start address of the RAM buffer allocated for memory resource of
                                         * Broadcast Audio Scan in Client role
                                         * It must be a 32bit aligned RAM area.
                                         */
  uint16_t      RamSize;                /* Size of the RAM allocated at pStartRamAddr pointer
                                         * Computed thanks to BAP_BASS_CLT_TOTAL_BUFFER_SIZE macro
                                         */
} BAP_BroadcastAudioScanCltConfig_t;

/* Configuration settings of Broadcast Audio Scan in Server role structure.
 * These configuration settings are required by Scan Delegator .
 */
typedef struct
{
  uint8_t       MaxNumBSRC;             /* Maximum Number of Broadcast Source Information
                                         * Server should expose a number of Broadcast Receive State characteristics at
                                         * least equal to the number of BIGs that the server can simultaneously maintain
                                         * synchronization to.
                                         */
  uint8_t       MaxCodecConfSize;       /* Maximum Size of the buffer allocated for Size for each Broadcast Source
                                         * Information supported by the Scan Delegator to store the
                                         * Codec Specific Configuration
                                         */
  uint8_t       MaxMetadataLength;      /* Maximum Length of the buffer allocated for each BASE subgroups
                                         * to store its Metadata
                                         */
  uint8_t       MaxNumBaseSubgroups;    /* Maximum number of number of subgroups present in the BASE structure,
                                         * used to describe a BIG
                                         */
  uint8_t       *pStartRamAddr;         /* Start address of the RAM buffer allocated for memory resource of
                                         * Broadcast Audio Scan in Server role
                                         * It must be a 32bit aligned RAM area.
                                         */
  uint16_t      RamSize;                /* Size of the RAM allocated at pStartRamAddr pointer
                                         * Computed thanks to BAP_BASS_SRV_TOTAL_BUFFER_SIZE macro
                                         */
}BAP_BroadcastAudioScanSrvConfig_t;


/* Basic Audio Profile Configuration used in BAP_Init() parameter */
typedef struct
{
  BAP_Role_t                            Role;           /* BAP roles supported by local device*/
  uint8_t                               MaxNumBleLinks; /* Maximum number of BLE Links */
  uint8_t                               MaxNumUCLLinks; /* Maximum number of BLE Links in Unicast Client Role */
  uint8_t                               MaxNumUSRLinks; /* Maximum number of BLE Links in Unicast Server Role */
  uint8_t                               MaxNumBALinks;  /* Maximum number of BLE Links in Broadcast Assistant Role */
  BAP_AudioCapSrvConfig_t               PACSSrvConfig;  /* Published Audio Capabilities Server Configuration for
                                                         * Unicast server and Broadcast Sink
                                                         */
  BAP_AudioCapCltConfig_t               PACSCltConfig;  /* Published Audio Capabilities Server Configuration for
                                                         * Unicast Client and Broadcast Assistant
                                                         */
  BAP_AudioStreamCtrlSrvConfig_t        ASCSSrvConfig;  /* Audio Stream Endpoint Management Unicast Server */
  BAP_AudioStreamCtrlCltConfig_t        ASCSCltConfig;  /* Audio Stream Endpoint Management Unicast Client */
  BAP_BroadcastAudioScanCltConfig_t     BASSCltConfig;  /* Broadcast Audio Scan Broadcast Assistant */
  BAP_BroadcastAudioScanSrvConfig_t     BASSSrvConfig;  /* Broadcast Audio Scan Scan Delegator */
  BAP_ISOChannelConfig_t                ISOChnlConfig;  /* Isochronous Channels Configuration*/
  BAP_NvmMgmtConfig_t                   NvmMgmtConfig;  /* Non-Volatile Memory Management for BAP Services restoration
                                                         * Unicast Client, Unicast Server, Scan Delegator and
                                                         * Broadcast Assistant
                                                         */
} BAP_Config_t;

/* Audio Codec ID type*/
typedef struct
{
  Audio_Coding_Format_t CodingFormat;   /* Coding Format */
  uint16_t              CompanyID;      /* Company Identifier (see Bluetooth Assigned Numbers) */
  uint16_t              VsCodecID;      /* Vendor-specific codec ID value of the PAC record.
                                         * Shall be 0x0000 if octet Coding Format is not CODING_FORMAT_VS
                                         */
} BAP_AudioCodecID_t;

/* Audio Codec Specific Capabilities type*/
typedef struct
{
  BAP_AudioCodecID_t    CodecID;                /*Codec ID information*/

  /*Codec Specific Capabilities*/
  uint8_t               SpecificCapLength;      /* Length, in octets, of the pCodecSpecificCap value */
  uint8_t               *pSpecificCap;          /* Pointer pointer on (LTV)-formatted structures containing
                                                 * Codec_Specific_Capabilities value (see Codec_Specific_Capabilities
                                                 * group)
                                                 */
  /*Metadata*/
  uint8_t               MetadataLength;         /* Length, in octets, of the metadata */
  uint8_t               *pMetadata;             /* Pointer on (LTV)-formatted structures containing metadata
                                                 * (see Metadata group)
                                                 */
} BAP_AudioCapabilities_t;

/* Published Audio Capabilities (PAC) Record type including Audio Codec Specific Capabilities*/
typedef struct
{
  uint32_t                      *Next;  /* Used internally by the Audio IP */
  uint32_t                      *Prev;  /* Used internally by the Audio IP */
  BAP_AudioCapabilities_t       Cap;    /* Audio Codec Capabilities */

} BAP_AudioPACRecord_t;

/* Audio Codec Specific Configuration type*/
typedef struct
{
  BAP_AudioCodecID_t    CodecID;                /* Codec ID information*/

  /*Codec Specific Configuration*/
  uint8_t               SpecificConfLength;     /* Length, in octets, of the pCodecSpecificConf value */
  uint8_t               *pSpecificConf;         /* pointer pointer on (LTV)-formatted structures containing
                                                 * Codec_Specific_Configurations value
                                                 * (see Codec_Specific_Configurations group)
                                                 */
} BAP_AudioCodecConf_t;

/* Preferred configuration for QoS for an Audio Stream Endpoint */
typedef struct
{
  BAP_Framing_t         Framing;                        /* Server support for unframed ISOAL PDUs*/
  Preferred_PHY_t       PrefPHY;                        /* Server preferred value for the PHY parameter
                                                         * to be written by the client for the ASE in the
                                                         * Config QoS operation
                                                         */
  uint8_t               PrefRetransmissionNumber;       /* Server preferred value for the Retransmission_Number
                                                         * parameter to be written by the client for the ASE in
                                                         * the Config QoS operation
                                                         * Range: 0x00–0xFF
                                                         */
  uint16_t              MaxTransportLatency;            /* Largest Server supported value for the Max_Transport_Latency
                                                         * parameter to be written by the client for the ASE in
                                                         * the Config QoS operation defined.
                                                         * Range: 0x0005–0x0FA0
                                                         * Units: ms
                                                         */
  uint32_t              PresentationDelayMin;           /* Minimum server supported Presentation_Delay for the ASE
                                                         * Units: µs
                                                         */
  uint32_t              PresentationDelayMax;           /* Maximum server supported Presentation_Delay for the ASE
                                                         * Units: µs
                                                         */
  uint32_t              PrefPresentationDelayMin;       /* Server preferred minimum Presentation_Delay for the ASE.
                                                         * The server can use this parameter and
                                                         * Preferred_Presentation_Delay_Max narrower range of its
                                                         * supported presentation delay that the server prefers to
                                                         * operate in.
                                                         * If nonzero, shall be = Presentation_Delay_Min
                                                         * A value of 0x000000 indicates no preference.
                                                         * Units: µs
                                                         */
  uint32_t              PrefPresentationDelayMax;       /* Server preferred maximum Presentation_Delay for the ASE.
                                                         * The server can use this parameter and
                                                         * Preferred_Presentation_Delay_Min to express a narrower
                                                         * range of its supported presentation delay that the server
                                                         * prefers to operate in.
                                                         * If nonzero, shall be = Presentation_Delay_Max
                                                         * A value of 0x000000 indicates no preference.
                                                         * Units: µs
                                                         */

}BAP_PrefQoSConf_t;

/* Codec Configuration of an Audio Stream Endpoint */
typedef struct
{
  BAP_PrefQoSConf_t     PrefQoSConf;    /* Preferred Server Configuration for QoS Configuration */

  BAP_AudioCodecConf_t  CodecConf;      /* Codec specific configuration for the ASE */

}BAP_ASECodecConf_t;

/* QoS Configuration of an Audio Stream Endpoint */
typedef struct
{
  uint8_t               CIG_ID;                 /* CIG Identifier */
  uint8_t               CIS_ID;                 /* CIS Identifier */
  uint32_t              SDUInterval;            /* SDU Interval */
  BAP_Framing_t         Framing;                /* Server support for unframed ISOAL PDUs*/
  Preferred_PHY_t       PHY;                    /* 1M, 2M or LE Coded */
  uint16_t              MaxSDU;                 /* Maximum size of the SDU */
  uint8_t               RetransmissionNumber;   /* Retransmission Number */
  uint16_t              MaxTransportLatency;    /* Max Transport Latency */
  uint32_t              PresentationDelay;      /* Presentation Delay */

}BAP_ASEQoSConf_t;

/* Configuration of an Audio Stream Endpoint in Enabling, or Streaming or Disabling State */
typedef struct
{
  uint8_t       CIG_ID;                 /* CIG Identifier*/
  uint8_t       CIS_ID;                 /* CIS Identifier*/
  uint8_t       MetadataLength;         /* Length of data of the pMetadata*/
  uint8_t       *pMetadata;             /* Pointer on Metadata*/

}BAP_ASEEnStrDisConf_t;


/* Audio Stream Endpoint State Information including ASE ID, Type, State and associated parameters.
 */
typedef struct
{
  uint8_t                       ASE_ID;         /*Audio Stream Endpoint Identifier*/
  ASE_Type_t                    Type;           /*Source of Sink*/
  ASE_State_t                   State;          /*State of the ASE*/

  union{
    BAP_ASECodecConf_t          *pCodecConf;    /* Codec Configuration if ASE is in
                                                 * ASE_STATE_CODEC_CONFIGURED state
                                                 */
    BAP_ASEQoSConf_t            *pQoSConf;      /* QoS Configuration if ASE is in
                                                 * ASE_STATE_QOS_CONFIGURED state
                                                 */
    BAP_ASEEnStrDisConf_t       *pEnStrDisConf; /* ASE Configuration if ASE is in
                                                 * ASE_STATE_ENABLING, ASE_STATE_STREAMING
                                                 * or ASE_STATE_DISABLING state
                                                 */
  }p;

} BAP_ASE_State_Params_t;

/* Audio Stream Endpoint Information including ASE ID, Type, State and associated parameters */
typedef struct
{
  uint8_t                       ASE_ID;         /*Audio Stream Endpoint Identifier*/
  ASE_State_t                   State;          /*State of the ASE*/
  ASE_Type_t                    Type;           /*Source of Sink*/

  struct{
    BAP_ASECodecConf_t          Codec;          /* Codec Configuration : valid if ASE is in
                                                 * not in IDLE state
                                                 */
    BAP_ASEQoSConf_t            QoS;            /* QoS Configuration : valid if ASE is in
                                                 * ASE_STATE_QOS_CONFIGURED; ASE_STATE_ENABLING,
                                                 * ASE_STATE_STREAMING or ASE_STATE_DISABLING state
                                                 */
    BAP_ASEEnStrDisConf_t       ExtConf;        /* Additional Configuration Parameters : valid
                                                 * if ASE is in ASE_STATE_ENABLING, ASE_STATE_STREAMING
                                                 * or ASE_STATE_DISABLING state
                                                 */
  }params;

} BAP_ASE_Info_t;

/* Structure used in BAP_AudioCodecController_t for supported Standard Codec description */
typedef struct
{
  Audio_Coding_Format_t CodingFormat;           /* Coding Format */
  uint8_t               TransportMask;          /* Codec Transport Type :
                                                 * bit0: Unicast (CIS)
                                                 * bit1: Broadcast (BIS)
                                                 */
} BAP_SupportedStandardCodec_t;

/* Structure used in BAP_AudioCodecController_t for supported Standard Codec description */
typedef struct
{
  uint8_t               aCodecID[4];    /* Octets 0 and 1 : Company ID, Octets 2 and 3 Vendor-defined codec ID*/
  uint8_t               TransportMask;  /* Codec Transport Type :
                                         * bit0: Unicast (CIS)
                                         * bit1: Broadcast (BIS)
                                         */
} BAP_SupportedVSCodec_t;

/* Structure used to configure the Audio Codec in LE Controller layer */
typedef struct
{
  uint8_t                       NumStandardCodecs;              /* Total number of standard codecs enabled */
  BAP_SupportedStandardCodec_t  *pStandardCodec;                /* Pointer on a table of standard codecs enabled */
  uint8_t                       NumVendorSpecificCodecs;        /* Total number of vendor specific codecs enabled */
  BAP_SupportedVSCodec_t        *pVendorSpecificCodec;          /* Pointer on a table of vendor specific codecs enabled*/
} BAP_AudioCodecController_t;

/* Structure of Audio Contexts */
typedef struct
{
  Audio_Context_t       Snk;    /* sink audio context */
  Audio_Context_t       Src;    /* source audio context */
}BAP_Audio_Contexts_t;


/* Structure containing the content of the BIS in a BAP_BASE_Subgroup_t type */
typedef struct
{
  uint8_t       BIS_Index;
  uint8_t       CodecSpecificConfLength;
  uint8_t       *pCodecSpecificConf;
} BAP_BASE_BIS_t;

/* Structure containing the content of a subgroup in a BAP_BASE_Group_t */
typedef struct
{
  uint8_t               NumBISes;                       /* Number of BISes */
  BAP_BASE_BIS_t        *pBIS;                          /* Pointer on table of BIS content*/
  uint64_t              CodecID;
  uint8_t               CodecSpecificConfLength;
  uint8_t               *pCodecSpecificConf;
  uint8_t               MetadataLength;
  uint8_t               *pMetadata;
} BAP_BASE_Subgroup_t;

/* Structure passed in parameter of specific events notified when a BASE report is received
 * through a periodic advertising train.
 */
typedef struct
{
  uint32_t              PresentationDelay;
  uint8_t               NumSubgroups;
  BAP_BASE_Subgroup_t   *pSubgroups;
} BAP_BASE_Group_t;

typedef struct
{
  uint8_t  AdvSID;
  uint32_t AdvIntervalMin;
  uint32_t AdvIntervalMax;
  uint8_t  ChannelMap;
  uint8_t  OwnAddressType;
  uint8_t  PeerAddressType;
  uint8_t  PeerAddress[6u];
  uint8_t  TxPower;
  uint8_t  SecondaryAdvMaxSkip;
  uint8_t  SecondaryAdvPHY;
} BAP_Extended_Advertising_Params_t;

typedef struct
{
  uint32_t AdvIntervalMin;
  uint32_t AdvIntervalMax;
  uint8_t  AdvProperties;
} BAP_Periodic_Advertising_Params_t;

/* Structure passed in parameter of specific event notified when a BIG Info report is received
 * through a periodic advertising train.
 */
typedef struct
{
  uint16_t      SyncHandle;
  uint8_t       NumBIS;
  uint8_t       NSE;
  uint16_t      ISOInterval;
  uint8_t       BN;
  uint8_t       PTO;
  uint8_t       IRC;
  uint16_t      MaxPDU;
  uint32_t      SDUInterval;
  uint16_t      MaxSDU;
  uint8_t       PHY;
  uint8_t       Framing;
  uint8_t       Encryption;
}BAP_BIGInfo_Report_Data_t;

/* Structure passed in parameter of specific events notified when a BASE report is received
 * through a periodic advertising train.
 */
typedef struct
{
  uint16_t      SyncHandle;
  uint8_t       *pBasePayload;
  uint8_t       BasePayloadLength;
}BAP_BASE_Report_Data_t;

/* Structure passed in parameter of specific event notified by Broadcast Source when its state machine has
 * changed to Streaming state (audio started).
*/
typedef struct
{
  uint8_t       BIGHandle;              /* The identifier of the BIG */
  uint8_t       NumBISes;               /* The number of BIS in the BIG */
  uint16_t      *pConnHandle;           /* The list of connection handles of all BISes in the BIG*/
  uint32_t      TransportLatency;       /* Transport Latency in ms*/
}BAP_Create_BIG_Complete_Data;

/* Structure passed in parameter of specific event when Audio Path is up during Broadcast Audio procedure.*/
typedef struct
{
  uint8_t       NumBISes;               /* The number of BIS in the BIG */
  uint16_t      *pConnHandle;           /* The list of connection handles of all BISes in the BIG*/
  uint8_t       Direction;              /* The direction of audio, 0 is source and 1 is sink */
  uint32_t      ControllerDelay;        /* Controller Delay in us*/
}BAP_Broadcast_Audio_Path_t;

typedef struct
{
  uint8_t       BIGHandle;              /* The identifier of the BIG */
  uint8_t       NumBISes;               /* The number of BIS in the BIG */
  uint16_t      *pConnHandle;           /* The list of connection handles of all BISes in the BIG */
  uint32_t      TransportLatency;       /* Transport Latency in ms*/
}BAP_BIG_Sync_Established_Data_t;


/* Structure passed in parameter of specific event notified by Broadcast Sink when synchronization
 * to a BIG has been lost.
 */
typedef struct
{
  uint8_t       BIGHandle;      /* The identifier of the BIG */
  uint8_t       Reason;         /* The reason of the sync lost */
}BAP_BIG_Sync_Lost_Data_t;


/* Type representing a subgroup in a Broadcast Source of a Broadcast Assistant */
typedef struct
{
  uint32_t BisSync;             /* BIS_Sync parameter for the subgroup in the BIG
                                 * 4-octet bitfield. Bit 0-30 = BIS_index[1-31]
                                 * 0x00000000: 0b0 = Do not synchronize to BIS_index[x]
                                 * 0xxxxxxxxx: 0b1 = Synchronize to BIS_index[x]
                                 * 0xFFFFFFFF: No preference
                                 */

  uint8_t MetadataLength;       /* Length of the Metadata parameter value for the subgroup in the BIG */

  uint8_t *pMetadata;           /* LTV-formatted Metadata for the subgroup in the BIG */
} BAP_BA_Broadcast_Source_Subgroup_t;

/* Type representing a subgroup state in a Broadcast Source of a Scan Delegator */
typedef struct
{
  uint32_t BisSyncState;        /* BIS_Sync_State for the subgroup
                                 * 4-octet bitfield. Bit 0-30 = BIS_index[1-31]
                                 * 0x00000000: 0b0 = Not synchronized to BIS_index[x]
                                 * 0xxxxxxxxx: 0b1 = Synchronized to BIS_index[x]
                                 * 0xFFFFFFFF: Failed to sync to BIG
                                 */

  uint8_t MetadataLength;       /* Length of the Metadata field for the subgroup */

  const uint8_t *pMetadata;     /* LTV-formatted Metadata for the subgroup */
} BAP_BA_Broadcast_Source_Subgroup_State_t;

/* Type used to add a Broadcast Source to a remote Scan Delegator */
typedef struct
{
  uint8_t AdvAddressType;                       /* Advertiser_Address_Type for the Broadcast Source
                                                 * 0x00 = Public Device Address or Public Identity Address
                                                 * 0x01 = Random Device Address or Random (static) Identity Address
                                                 */

  uint8_t aAdvAddress[6u];                      /* Advertiser_Address for the Broadcast Source */

  uint8_t AdvSid;                               /* Advertising_SID subfield of the ADI field of the AUX_ADV_IND PDU or
                                                 * the LL_PERIODIC_SYNC_IND containing the SyncInfo that points to the
                                                 * PA transmitted by the Broadcast Source.
                                                 * Range: 0x00-0x0F
                                                 */

  uint8_t aBroadcastId[3u];                     /* Broadcast_ID of the Broadcast Source */

  uint8_t PaSync;                               /* 0x00: Do not synchronize to PA
                                                 * 0x01: Synchronize to PA – PAST available
                                                 * 0x02: Synchronize to PA – PAST not available
                                                 */

  uint16_t PaInterval;                           /* SyncInfo field Interval parameter value
                                                 * 0xFFFF: PA_Interval unknown
                                                 */

  uint8_t NumSubgroups;                         /* Number of subgroups */

  BAP_BA_Broadcast_Source_Subgroup_t *pSubgroup; /* A pointer to an array of subgroups contained in the source */
} BAP_BA_Broadcast_Source_Add_t;

/* Type used to modify a Broadcast Source to a remote Scan Delegator */
typedef struct
{
  uint8_t       SourceId;                                /* Source_ID assigned by the server to a Broadcast Receive State
                                                         * characteristic
                                                         */

  uint8_t       PaSync;                                 /* 0x00: Do not synchronize to PA
                                                         * 0x01: Synchronize to PA – PAST available
                                                         * 0x02: Synchronize to PA – PAST not available
                                                         */

  uint16_t      PaInterval;                             /* SyncInfo field Interval parameter value
                                                         * 0xFFFF: PA_Interval unknown
                                                         */

  uint8_t       NumSubgroups;                           /* Number of subgroups */

  BAP_BA_Broadcast_Source_Subgroup_t *pSubgroup;        /* A pointer to an array of subgroups contained in the source */
} BAP_BA_Broadcast_Source_Modify_t;

/* Structure representing a Broadcast Source state of a Scan Delegator, passed in parameter of specific event notified
 * by BAP Broadcast Assistant
 */
typedef struct
{
  uint8_t SourceId;                                     /* Assigned by the server
                                                         * Unique for each instance of the Broadcast Receive State
                                                         * characteristic exposed by the server
                                                         */

  uint8_t SourceAddressType;                            /* 0x00 = Public Device Address or Public Identity Address
                                                         * 0x01 = Random Device Address or Random (static) Identity
                                                         * Address
                                                         */

  uint8_t aSourceAddress[6u];                           /* Public Device Address, Random Device Address, Public Identity
                                                         * Address or Random (static) Identity Address of the Broadcast
                                                         * Source.
                                                         */

  uint8_t SourceAdvSid;                                 /* Advertising_SID subfield of the ADI field of the AUX_ADV_IND
                                                         * PDU or the LL_PERIODIC_SYNC_IND containing the SyncInfo that
                                                         * points to the PA transmitted by the Broadcast Source.
                                                         */

  uint8_t aBroadcastId[3u];                             /* Broadcast_ID of the Broadcast Source */

  uint8_t PaSyncState;                                  /* 0x00: Not synchronized to PA
                                                         * 0x01: SyncInfo Request
                                                         * 0x02: Synchronized to PA
                                                         * 0x03: Failed to synchronize to PA
                                                         * 0x04: No PAST
                                                         */

  uint8_t BIGEncryption;                               /* 0x00: Not encrypted
                                                         * 0x01: Broadcast_Code required
                                                         * 0x02: Decrypting
                                                         * 0x03: Bad_Code (incorrect encryption key)
                                                         */

  uint8_t aBadCode[16u];                                /* If BIG_Encryption field value = 0x00, 0x01, or 0x02: empty
                                                         * (zero length)
                                                         * If BIG_Encryption field value = 0x03 (Bad_Code), Bad_Code shall
                                                         * be set to the value of the incorrect 16-octet Broadcast_Code
                                                         * that fails to decrypt the BIG
                                                         */

  uint8_t NumSubgroups;                                 /* Number of subgroups */

  BAP_BA_Broadcast_Source_Subgroup_State_t *pSubgroup;  /* A pointer to an array of subgroups contained in the source */
} BAP_BA_Broadcast_Source_State_t;


/* Structure passed in parameter of specific event notified by BAP Broadcast Assistant once link with remote
 * Scan Delegator is complete.
 */
typedef struct
{
  uint8_t       NumberBroadcastReceiveStateChar;
  uint16_t      BASS_StartAttHandle;            /* ATT Start Handle of the BASS Service in the remote Unicast Server */
  uint16_t      BASS_EndAttHandle;              /* ATT End Handle of the BASS Service in the remote Unicast Server */
  uint16_t      PACS_StartAttHandle;            /* Start ATT Handle of the PACS Service in the remote Unicast Server */
  uint16_t      PACS_EndAttHandle;              /* End ATT Handle of the PACS Service in the remote Unicast Server */
} BAP_Broadcast_Assistant_Info_t;

/* Structure containing information about remote Unicast Server reported by Unicast Client once link is complete */
typedef struct
{
  Audio_Role_t          AudioRole;              /* audio role of the remote Unicast Server*/
  Audio_Location_t      SnkAudioLocations;      /* Sink audio Locations */
  Audio_Location_t      SrcAudioLocations;      /* Source audio Locations */
  BAP_Audio_Contexts_t  AvailAudioContexts;     /* Available audio contexts */
  BAP_Audio_Contexts_t  SuppAudioContexts;      /* Supported audio contexts */
  uint16_t              ASCS_StartAttHandle;    /* ATT Start Handle of the ASCS Service in the remote Unicast Server */
  uint16_t              ASCS_EndAttHandle;      /* ATT End Handle of the ASCS Service in the remote Unicast Server */
  uint16_t              PACS_StartAttHandle;    /* Start ATT Handle of the PACS Service in the remote Unicast Server */
  uint16_t              PACS_EndAttHandle;      /* End ATT Handle of the PACS Service in the remote Unicast Server */
} BAP_Unicast_Server_Info_t;


/* Structure passed in parameter of specific event notified by Unicast Server or Client when Audio Path is up or down
 * during Unicast Audio procedure.
 */
typedef struct
{
  uint16_t                      CIS_ConnHandle;         /* CIS Connection Handle */
  BAP_Audio_Path_Direction_t    AudioPathDirection;     /* Input or Output */
  uint32_t                      ControllerDelay;        /* Controller Delay in us*/
  uint32_t                      TransportLatency;       /* Transport Latency in ms*/
  BAP_AudioCodecConf_t          CodecConf;              /* Audio Codec Configuration */
}BAP_Unicast_Audio_Path_t;

/* Structure passed in parameter of specific event notified by Unicast Server when Preferred QoS settings are requested
 * for Unicast Audio procedure.
 */
typedef struct
{
  uint8_t                       ASE_ID;                 /* Audio Stream Endpoint Identifier */
  ASE_Type_t                    Type;                   /* ASE Type */
  Target_Latency_t              TargetLatency;          /* Target Latency */
  Target_Phy_t                  TargetPhy;              /* Target Phy */
  uint32_t                      SrcControllerDelayMin;  /* Minimum Controller Delay for the specified Codec in Source role.
                                                         * 0 if Codec in Controller is not used
                                                         */
  uint32_t                      SrcControllerDelayMax;  /* Maximum Controller Delay for the specified Codec in Source role.
                                                         * 0 if Codec in Controller is not used
                                                         */
  uint32_t                      SnkControllerDelayMin;  /* Minimum Controller Delay for the specified Codec in Sink role.
                                                         * 0 if Codec in Controller is not used
                                                         */
  uint32_t                      SnkControllerDelayMax;  /* Maximum Controller Delay for the specified Codec in Sink role.
                                                         * 0 if Codec in Controller is not used
                                                         */
  BAP_AudioCodecConf_t          CodecConf;              /* Audio Codec Configuration */
  ASE_Op_Resp_t                 *pResp;                 /* Indicate the Server Response of the requested QoS Operation */
  ASE_Op_Resp_Reason_t          *pReason;               /* Indicate the Failure Response reason of the requested QoS
                                                         * Operation
                                                         */
  BAP_PrefQoSConf_t             *pPrefQoSConfRsp;       /* Preferred Server QoS Configuration.
                                                         * Shall be filled by the application within the callback.
                                                         */
}BAP_PrefQoSConfReq_t;


/* Structure passed in parameter of specific event notified by Unicast Server and Client when Audio Data Path
 * shall be setup for Unicast Audio procedure.
 */
typedef struct
{
  uint16_t              CIS_ConnHandle;         /* CIS Connection Handle */
  uint8_t               ASE_ID;                 /* Audio Stream Endpoint ID*/
  uint8_t               PathDirection;          /* Path Direction (0 : Input, 1 : Output) */
  BAP_AudioCodecConf_t  CodecConf;              /* Codec Configuration */
  uint32_t              PresentationDelay;      /* Presentation Delay */
  uint32_t              ControllerDelayMin;     /* Minimum Controller Delay for the specified Codec.
                                                 * 0 if Codec in Controller is not used
                                                 */
  uint32_t              ControllerDelayMax;     /* Maximum Controller Delay for the specified Codec.
                                                 * 0 if Codec in Controller is not used
                                                 */
}BAP_SetupAudioDataPathReq_t;


/* Structure containing the details of a broadcast source advertising passed in parameter of specific events notified
 * by Broadcast Sink or Broadcast Assistant when a Broadcast Source has been scanned.
 */
typedef struct
{
  uint8_t       AdvSID;
  const uint8_t *pAdvAddress;
  uint8_t       AdvAddressType;
  uint32_t      BroadcastID;
  const uint8_t *pAdvertisingData;
  uint8_t       AdvertisingDataLength;
}BAP_Broadcast_Source_Adv_Report_Data_t;

/* Structure passed in parameter of specific events notified by Broadcast Sink or Broadcast Assistant when
 * synchronization to a periodic advertising train has been established.
 */
typedef struct
{
  uint8_t       AdvSID;
  const uint8_t *pAdvAddress;
  uint8_t       AdvAddressType;
  uint16_t      SyncHandle;
} BAP_PA_Sync_Established_Data_t;


/* Structure passed in parameter of specific events notified by Broadcast Sink or Broadcast Assistant when
 * synchronization to a periodic advertising train has been lost.
 */
typedef struct
{
  uint16_t SyncHandle;
} BAP_PA_Sync_Lost_Data_t;

/* Structure passed in parameter of specific event notified by Broadcast Assistant when a Scan Delegator
 * soliciting request has been scanned.
 */
typedef struct
{
  const uint8_t *pAdvAddress;
  uint8_t       AdvAddressType;
} BAP_Soliciting_Scan_Delegator_Data_t;

#ifdef __cplusplus
}
#endif

#endif /* __BAP_TYPES_H */

