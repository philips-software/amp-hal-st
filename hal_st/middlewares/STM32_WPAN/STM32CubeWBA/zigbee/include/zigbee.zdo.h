/**
 * @file zigbee.zdo.h
 * @heading ZDO Layer
 * @brief Zigbee ZDO header file
 * @author Exegin Technologies Limited
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 *
 * This file provides the declaration of all the public API's and macros used by the ZDO layer.
 */

#ifndef ZIGBEE_ZDO_H
# define ZIGBEE_ZDO_H

struct ZbZdoFilterT;

/* Profile, and Device Identifiers for the ZDO. */
#define ZB_ZDO_PROFILE_ID                           0x0000U
#define ZB_ZDO_DEVICE_ID                            0x0000U
#define ZB_ZDO_DEVICE_VER                           0x00U

#define ZB_ZDO_ADDRRSP_DEVLIST_MAX                  32U

/** NWK_addr_req / IEEE_addr_req definitions */
enum ZbZdoAddrReqTypeT {
    ZB_ZDO_ADDR_REQ_TYPE_SINGLE = 0x00,
    /**< Single address request - Returns the information requested only */
    ZB_ZDO_ADDR_REQ_TYPE_EXTENDED = 0x01
        /**< Extended address request - Returns the information requested and appends
         * the devices in its network table */
};

/* NWK_Update_Req special duration values. */
#define ZB_ZDP_NWK_UPDATE_MAX_DURATION              0x05U
#define ZB_ZDP_NWK_UPDATE_CHANNEL_SWITCH            0xfeU
#define ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS        0xffU

/* Maximum Response Sizes */
/* NOTE was 64, but end up allocating too much on the stack */
#define ZB_ZDO_CLUSTER_LIST_MAX_SZ                  16U
#define ZB_ZDO_NETWORK_LIST_MAX_SZ                  8U
#define ZB_ZDO_NEIGHBOR_LIST_MAX_SZ                 4U
#define ZB_ZDO_ROUTE_LIST_MAX_SZ                    16U
/* Each entry in the ZDO message is either 14 or 21 bytes long.
 * It's only possible to return up to 4 entries at a time. */
#define ZB_ZDO_BIND_LIST_MAX_SZ                     4U
#define ZB_ZDO_ENDPOINT_LIST_MAXSZ                  32U
#define ZB_ZDO_CHANNEL_LIST_MAXSZ                   (8U * sizeof(uint32_t))
#define ZB_ZDO_CHILD_LIST_MAXSZ                     8U

/* Node Descriptor Definitions. */
#define ZB_ZDP_NODE_TYPE_COORDINATOR                (uint8_t)0x00U
#define ZB_ZDP_NODE_TYPE_ROUTER                     (uint8_t)0x01U
#define ZB_ZDP_NODE_TYPE_END_DEVICE                 (uint8_t)0x02U
#define ZB_ZDP_NODE_FREQ_868MHz                     (uint8_t)0x01U
#define ZB_ZDP_NODE_FREQ_900MHz                     (uint8_t)0x04U
#define ZB_ZDP_NODE_FREQ_2400MHz                    (uint8_t)0x08U
#define ZB_ZDP_NODE_CAPABILITY_EXT_EPLIST           (uint8_t)0x01U /* Deprecated in R23 */
#define ZB_ZDP_NODE_CAPABILITY_EXT_SIMPLE           (uint8_t)0x02U /* Deprecated in R23 */

/* Server Mask Bits. */
#define ZB_ZDP_SERVER_PRIMARY_TRUST                 (uint16_t)0x0001U
#define ZB_ZDP_SERVER_BACKUP_TRUST                  (uint16_t)0x0002U
#define ZB_ZDP_SERVER_RESERVED_1                    (uint16_t)0x0004U /* Deprecated in R23 - was ZB_ZDP_SERVER_PRIMARY_BIND_CACHE */
#define ZB_ZDP_SERVER_RESERVED_2                    (uint16_t)0x0008U /* Deprecated in R23 - was ZB_ZDP_SERVER_BACKUP_BIND_CACHE */
#define ZB_ZDP_SERVER_RESERVED_3                    (uint16_t)0x0010U /* Deprecated in R23 - was ZB_ZDP_SERVER_PRIMARY_DISC_CACHE */
#define ZB_ZDP_SERVER_RESERVED_4                    (uint16_t)0x0020U /* Deprecated in R23 - was ZB_ZDP_SERVER_BACKUP_DISC_CACHE */
#define ZB_ZDP_SERVER_NETWORK_MGR                   (uint16_t)0x0040U
#define ZB_ZDP_SERVER_RESERVED_MASK                 (uint16_t)0x0180U /* Reserved bits */
#define ZB_ZDP_SERVER_REVISION_SHIFT                9U
#define ZB_ZDP_SERVER_REVISION_MASK                 (uint16_t)0xFE00U /* (0x007f << ZB_ZDP_SERVER_REVISION_SHIFT) */

/* Power Descriptor Definitions. */
#define ZB_ZDP_POWER_MODE_ON                        (uint8_t)0x00U
#define ZB_ZDP_POWER_MODE_PERIODIC                  (uint8_t)0x01U
#define ZB_ZDP_POWER_MODE_REQUEST                   (uint8_t)0x02U
#define ZB_ZDP_POWER_SOURCE_CONSTANT                (uint8_t)0x01U
#define ZB_ZDP_POWER_SOURCE_RECHARGEABLE            (uint8_t)0x02U
#define ZB_ZDP_POWER_SOURCE_DISPOSEABLE             (uint8_t)0x04U
#define ZB_ZDP_POWER_LEVEL_CRITICAL                 (uint8_t)0x00U
#define ZB_ZDP_POWER_LEVEL_33                       (uint8_t)0x04U
#define ZB_ZDP_POWER_LEVEL_66                       (uint8_t)0x08U
#define ZB_ZDP_POWER_LEVEL_100                      (uint8_t)0x0cU

/* ZbNodeDescriptorT Mask Flag - Section 2.3.2.3  */
#define ZB_ZDP_NODE_OPT_MASK_TYPE                   (uint16_t)0x0007U
#define ZB_ZDP_NODE_OPT_MASK_RSV_1                  (uint16_t)0x0008U /* Deprecated in R23 - was ZB_ZDP_NODE_OPT_MASK_COMPLEX */
#define ZB_ZDP_NODE_OPT_MASK_RSV_2                  (uint16_t)0x0010U /* Deprecated in R23 - was ZB_ZDP_NODE_OPT_MASK_USER */
#define ZB_ZDP_NODE_OPT_MASK_FRAG                   (uint16_t)0x0020U
#define ZB_ZDP_NODE_OPT_MASK_APSFLAG                (uint16_t)0x0700U
#define ZB_ZDP_NODE_OPT_MASK_FREQBAND               (uint16_t)0xf800U
#define ZB_ZDP_NODE_OPT_MASK_RESERVED               (uint16_t)0x00e0U

#define ZB_ZDP_NODE_OPT_OFFSET_TYPE                 0U
#define ZB_ZDP_NODE_OPT_OFFSET_RSV_1                3U /* Deprecated in R23 - was ZB_ZDP_NODE_OPT_OFFSET_COMPLEX */
#define ZB_ZDP_NODE_OPT_OFFSET_RSV_2                4U /* Deprecated in R23 - was ZB_ZDP_NODE_OPT_OFFSET_USER */
#define ZB_ZDP_NODE_OPT_OFFSET_FRAG                 5U
#define ZB_ZDP_NODE_OPT_OFFSET_RSV_3                6U
#define ZB_ZDP_NODE_OPT_OFFSET_APSFLAG              8U
#define ZB_ZDP_NODE_OPT_OFFSET_FREQBAND             11U

/* Node Descriptor Definitions */
#define ZB_ZDP_NODE_OPT_TYPE_COORDINATOR            0x00U
#define ZB_ZDP_NODE_OPT_TYPE_ROUTER                 0x01U
#define ZB_ZDP_NODE_OPT_TYPE_END_DEVICE             0x02U
#define ZB_ZDP_NODE_OPT_TYPE(_nodetype_)            (_nodetype_)

#define ZB_ZDP_NODE_OPT_FRAGMENTATION_SUPPORTED     0x01U
#define ZB_ZDP_NODE_OPT_FRAG(_frag_)                (_frag_ << ZB_ZDP_NODE_OPT_OFFSET_FRAG)

#define ZB_ZDP_NODE_OPT_FREQBAND_868MHZ_BPSK        0x01U
#define ZB_ZDP_NODE_OPT_FREQBAND_915MHZ_BPSK        0x04U
#define ZB_ZDP_NODE_OPT_FREQBAND_2400MHZ            0x08U
#define ZB_ZDP_NODE_OPT_FREQBAND_GB_SE_FSK_SUBGHZ   0x10U /* GB SE FSK 868 MHz and 915 MHz bands. */
#define ZB_ZDP_NODE_OPT_BAND(_freqband_)            (((uint16_t)_freqband_) << ZB_ZDP_NODE_OPT_OFFSET_FREQBAND)

/* struct ZbPowerDescriptorT Mask Flag - Section 2.3.2.4 */
#define ZB_ZDP_POWER_MASK_MODE                      0x000fU
#define ZB_ZDP_POWER_MASK_AVAILABLE                 0x00f0U
#define ZB_ZDP_POWER_MASK_SOURCE                    0x0f00U
#define ZB_ZDP_POWER_MASK_LEVEL                     0xf000U

#define ZB_ZDP_POWER_OFFSET_MODE                    0U
#define ZB_ZDP_POWER_OFFSET_AVAILABLE               4U
#define ZB_ZDP_POWER_OFFSET_SOURCE                  8U
#define ZB_ZDP_POWER_OFFSET_LEVEL                   12U

/* Complex descriptor tag values. */
#define ZB_ZDP_COMPLEX_TAG_LANGUAGE                 0x01U
#define ZB_ZDP_COMPLEX_TAG_MANUFACTURFER            0x02U
#define ZB_ZDP_COMPLEX_TAG_MODEL                    0x03U
#define ZB_ZDP_COMPLEX_TAG_SERIAL                   0x04U
#define ZB_ZDP_COMPLEX_TAG_DEVICE_URL               0x05U
#define ZB_ZDP_COMPLEX_TAG_ICON                     0x06U
#define ZB_ZDP_COMPLEX_TAG_ICON_URL                 0x07U

/* Network Descriptor Flags. */
#define ZB_ZDP_NETWORK_MASK_PROFILE                 0x0fU
#define ZB_ZDP_NETWORK_MASK_VERSION                 0xf0U
#define ZB_ZDP_NETWORK_MASK_BEACON                  0x0fU
#define ZB_ZDP_NETWORK_MASK_SUPERFRAME              0xf0U
#define ZB_ZDP_NETWORK_MASK_PERMIT_JOIN             0x01U

#define ZB_ZDP_NETWORK_OFFSET_PROFILE               0U
#define ZB_ZDP_NETWORK_OFFSET_VERSION               4U
#define ZB_ZDP_NETWORK_OFFSET_BEACON                0U
#define ZB_ZDP_NETWORK_OFFSET_SUPERFRAME            4U
#define ZB_ZDP_NETWORK_OFFSET_PERMIT_JOIN           0U

/* Neighbor Descriptor Flags */
#define ZB_ZDP_NEIGHBOR_MASK_TYPE                   0x03U
#define ZB_ZDP_NEIGHBOR_MASK_RX_ON_IDLE             0x0cU
#define ZB_ZDP_NEIGHBOR_MASK_RELATIONSHIP           0x70U
#define ZB_ZDP_NEIGHBOR_MASK_PERMIT_JOIN            0x03U

#define ZB_ZDP_NEIGHBOR_OFFSET_TYPE                 0U
#define ZB_ZDP_NEIGHBOR_OFFSET_RX_ON_IDLE           2U
#define ZB_ZDP_NEIGHBOR_OFFSET_RELATIONSHIP         4U
#define ZB_ZDP_NEIGHBOR_OFFSET_PERMIT_JOIN          0U

/* Neighbor Descriptor Definitions */
#define ZB_ZDP_NEIGHBOR_TYPE_COORDINATOR            0x00U
#define ZB_ZDP_NEIGHBOR_TYPE_ROUTER                 0x01U
#define ZB_ZDP_NEIGHBOR_TYPE_END_DEVICE             0x02U
#define ZB_ZDP_NEIGHBOR_RELATIONSHIP_PARENT         0x00U /* ZB_NWK_NEIGHBOR_REL_PARENT */
#define ZB_ZDP_NEIGHBOR_RELATIONSHIP_CHILD          0x01U /* ZB_NWK_NEIGHBOR_REL_CHILD */
#define ZB_ZDP_NEIGHBOR_RELATIONSHIP_SIBLING        0x02U /* ZB_NWK_NEIGHBOR_REL_SIBLING */
#define ZB_ZDP_NEIGHBOR_RELATIONSHIP_NONE           0x03U /* ZB_NWK_NEIGHBOR_REL_NONE */
#define ZD_ZDP_NEIGHBOR_RELATIONSHIP_PREVIOUS       0x04U /* ZB_NWK_NEIGHBOR_REL_PREV_CHILD */
#define ZB_ZDP_NEIGHBOR_PERMIT_JOIN_DISABLED        0x00U
#define ZB_ZDP_NEIGHBOR_PERMIT_JOIN_ENABLED         0x01U
#define ZB_ZDP_NEIGHBOR_PERMIT_JOIN_UNKNOWN         0x02U

/* Routing Descriptor Flags */
#define ZB_ZDP_ROUTE_MASK_STATUS                    0x07U
#define ZB_ZDP_ROUTE_MASK_CONSTRAINED               0x08U
#define ZB_ZDP_ROUTE_MASK_MANY_TO_ONE               0x10U
#define ZB_ZDP_ROUTE_MASK_RECORD_REQUIRED           0x20U

#define ZB_ZDP_ROUTE_OFFSET_STATUS                  0U
#define ZB_ZDP_ROUTE_OFFSET_CONSTRAINED             3U
#define ZB_ZDP_ROUTE_OFFSET_MANY_TO_ONE             4U
#define ZB_ZDP_ROUTE_OFFSET_RECORD_REQUIRED         5U

/*
 * Maximum size of the TLV data that can be sent as part of ZDO Mgmt_Permit_Join_Req command
 * in an (unfragmented) APS payload, assuming an NWK secured packet with many-to-one source
 * routing (source routing level = 6). i.e ZB_APS_CONST_SAFE_NWKSEC_PAYLOAD_SIZE.
 * Adjust size to account for (seqnum + permit duration + tc_significance + frame_control).
 */
#define ZB_ZDO_PERMIT_JOIN_TLV_DATA_MAX_SZ          (ZB_APS_CONST_SAFE_NWKSEC_PAYLOAD_SIZE - 4U)

/**
 * Maximum size of the eui64 array/list sent as part of ZDO Clear_All_Bindings_req.
 *
 * EUI64 array can be as big as the number of entries that can be accommodated in the Maximum
 * (unfragmented) APS payload, assuming an NWK secured packet with many-to-one source routing
 * (source routing level = 6). Adjust size to account for seqnum & eui64_count.
 */
#define ZB_ZDO_CLEAR_ALL_BIND_EUI64_LIST_MAX_SZ \
    ((ZB_APS_CONST_SAFE_NWKSEC_PAYLOAD_SIZE - 2U) / sizeof(uint64_t)) /* 8 */

/**
 * Security_Get_Configuration_Req TLV ID list can be as big as the Maximum (unfragmented) APS payload,
 * assuming an APS secured packet with many-to-one source routing (source routing level = 6).
 * Adjust size to account for seqnum & tlvCount.
 */
#define ZB_ZDO_GET_CONFIG_REQ_TLV_LIST_SIZE         (ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE - 2U)

/**
 * Security_Get_Configuration_Rsp TLV list can be as big as the Maximum (unfragmented) APS payload,
 * assuming an APS secured packet with many-to-one source routing (source routing level = 6).
 * Adjust size to account for seqnum & status.
 */
#define ZB_ZDO_GET_CONFIG_RSP_TLV_LIST_SIZE         (ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE - 2U)

/**
 * Maximum size of the eui64 array/list sent as part of ZDO Security_Decommission_req.
 *
 * EUI64 array can be as big as the number of entries that can be accommodated in the Maximum
 * (unfragmented) APS payload, assuming an APS secured packet with many-to-one source routing
 * (source routing level = 6). Adjust size to account for seqnum & eui64_count.
 */
#define ZB_ZDO_DECOMMISSION_EUI64_LIST_MAX_SZ \
    ((ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE - 2U) / sizeof(uint64_t)) /* 6 */

/**
 * Length of APS frame counter challenge sent as part of
 * ZDO Security_Challenge_req.
 */
#define ZB_ZDO_SEC_CHALLENGE_LEN                    (8U)

/**
 * Length of MIC used as part of APS frame counter challenge.
 */
#define ZB_ZDO_SEC_CHALLENGE_MIC_LEN                (8U)

/*
 * ZigBee Device Object cluster definitions.
 */
/* This bit in the cluster ID denotes a response or request. */
#define ZB_ZDO_CLUSTER_RESPONSE_BIT                 0x8000U

/* ZDP Request IDs (Responses are the same, but with ZB_ZDO_CLUSTER_RESPONSE_BIT set) */
#define ZB_ZDO_NWK_ADDR_REQ                         0x0000U
#define ZB_ZDO_IEEE_ADDR_REQ                        0x0001U
#define ZB_ZDO_NODE_DESC_REQ                        0x0002U
#define ZB_ZDO_POWER_DESC_REQ                       0x0003U
#define ZB_ZDO_SIMPLE_DESC_REQ                      0x0004U
#define ZB_ZDO_ACTIVE_EP_REQ                        0x0005U
#define ZB_ZDO_MATCH_DESC_REQ                       0x0006U
#define ZB_ZDO_COMPLEX_DESC_REQ                     0x0010U
#define ZB_ZDO_USER_DESC_REQ                        0x0011U
#define ZB_ZDO_DISCOVERY_CACHE_REQ                  0x0012U
#define ZB_ZDO_DEVICE_ANNCE                         0x0013U
#define ZB_ZDO_USER_DESC_SET                        0x0014U
#define ZB_ZDO_SYSTEM_SERVER_DISC_REQ               0x0015U
#define ZB_ZDO_DISCOVERY_STORE_REQ                  0x0016U
#define ZB_ZDO_NODE_DESC_STORE_REQ                  0x0017U
#define ZB_ZDO_POWER_DESC_STORE_REQ                 0x0018U
#define ZB_ZDO_ACTIVE_EP_STORE_REQ                  0x0019U
#define ZB_ZDO_SIMPLE_DESC_STORE_REQ                0x001aU
#define ZB_ZDO_REMOVE_NODE_CACHE_REQ                0x001bU
#define ZB_ZDO_FIND_NODE_CACHE_REQ                  0x001cU
#define ZB_ZDO_EXT_SIMPLE_DESC_REQ                  0x001dU
#define ZB_ZDO_EXT_ACTIVE_EP_REQ                    0x001eU
#define ZB_ZDO_PARENT_ANNCE                         0x001fU
#define ZB_ZDO_END_DEVICE_BIND_REQ                  0x0020U
#define ZB_ZDO_BIND_REQ                             0x0021U
#define ZB_ZDO_UNBIND_REQ                           0x0022U
#define ZB_ZDO_CLEAR_ALL_BINDINGS_REQ               0x002bU
/* ZB_ZDO_MGMT_NWK_DISC_REQ depreciated             0x0030U */
#define ZB_ZDO_MGMT_LQI_REQ                         0x0031U
#define ZB_ZDO_MGMT_RTG_REQ                         0x0032U
#define ZB_ZDO_MGMT_BIND_REQ                        0x0033U
#define ZB_ZDO_MGMT_LEAVE_REQ                       0x0034U
#define ZB_ZDO_MGMT_DIRECT_JOIN_REQ                 0x0035U
#define ZB_ZDO_MGMT_PERMIT_JOINING_REQ              0x0036U
#define ZB_ZDO_MGMT_CACHE_REQ                       0x0037U
#define ZB_ZDO_MGMT_NWK_UPDATE_REQ                  0x0038U
#define ZB_ZDO_MGMT_NWK_ENHANCED_UPDATE_REQ         0x0039U
#define ZB_ZDO_MGMT_IEEE_JOINING_LIST_REQ           0x003aU
#define ZB_ZDO_MGMT_UNSOLICITED_UPDATE_NOTIFY       0x003bU
#define ZB_ZDO_MGMT_BCN_SURVEY_REQ                  0x003cU
#define ZB_ZDO_SEC_KEY_NEGO_REQ                     0x0040U
#define ZB_ZDO_SEC_RETRIEVE_AUTH_TOKEN_REQ          0x0041U
#define ZB_ZDO_SEC_GET_AUTH_LEVEL_REQ               0x0042U
#define ZB_ZDO_SEC_SET_CONFIG_REQ                   0x0043U
#define ZB_ZDO_SEC_GET_CONFIG_REQ                   0x0044U
#define ZB_ZDO_SEC_KEY_UPDATE_REQ                   0x0045U
#define ZB_ZDO_SEC_DECOMMISSION_REQ                 0x0046U
#define ZB_ZDO_SEC_CHALLENGE_REQ                    0x0047U

/* Note that legacy devices may be setting bits that are getting ignored,
 * this is due to these values being depreciated in R23 */

/** ZigBee Node Descriptor */
struct ZbNodeDescriptorT {
    uint8_t logicalType; /**< Logical type */
    uint8_t fragSupported; /**< Fragmentation Supported */
    uint8_t apsFlags; /**< APS flags */
    uint8_t freqBands; /**< Frequency band */
    uint8_t macCapability; /**< MAC capability flags */
    uint16_t mfrCode; /**< Manufacturer code */
    uint8_t maxBuffer; /**< Maximum buffer size */
    uint16_t maxIncomingTransfer; /**< Maximum incoming transfer size */
    uint16_t serverMask; /**< Server mask */
    uint16_t maxOutgoingTransfer; /**< Maximum outgoing transfer size */
    uint8_t descCapability; /**< Descriptor capability field */
};

/** ZigBee Power Descriptor */
struct ZbPowerDescriptorT {
    uint8_t currentMode; /**< Current power mode */
    uint8_t availableSources; /**< Available power sources */
    uint8_t currentSource; /**< Current power source */
    uint8_t currentLevel; /**< Current power source level */
};

/** ZigBee Simple (Application) Descriptor. */
struct ZbSimpleDescriptorT {
    uint8_t endpoint; /**< Endpoint */
    uint16_t profileId; /**< Application profile identifier */
    uint16_t deviceId; /**< Application device identifier */
    uint8_t version; /**< Application device version */
    uint8_t inputClusterCount; /**< Application input cluster count */
    uint16_t inputClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; /**< Application input cluster list */
    uint8_t outputClusterCount; /**< Application output cluster count */
    uint16_t outputClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; /**< Application output cluster list */
};

/** Network Descriptor */
struct ZbZdoNetworkDescT {
    uint64_t epid; /**< ExtendedPANId */
    uint8_t logicalChannel; /**< LogicalChannel */
    uint8_t version; /**< ZigBeeVersion */
    uint8_t stackProfile; /**< StackProfile */
    uint8_t superframeOrder; /**< SuperframeOrder */
    uint8_t beaconOrder; /**< BeaconOrder */
    uint8_t permitJoin; /**< PermitJoining */
};

/** Neighbor Descriptor */
struct ZbZdoNeighborDescT {
    uint64_t epid; /**< Extended PAN ID */
    uint64_t extAddr; /**< Extended address */
    uint16_t nwkAddr; /**< Network address */
    uint8_t deviceType; /**< Device type */
    uint8_t rxOnWhenIdle; /**< RxOnWhenIdle */
    uint8_t relationship; /**< Relationship */
    uint8_t permitJoin; /**< Permit joining */
    uint8_t depth; /**< Depth */
    uint8_t lqi; /**< LQI */
};

/** Routing Descriptor */
struct ZbZdoRoutingDescT {
    uint16_t destAddr; /**< Destination address */
    uint8_t status; /**< Status */ /* EXEGIN - convert to an enum type? */
    uint8_t constrained; /**< Memory Constrained */
    uint8_t manyToOne; /**< Many-to-one */
    uint8_t recordRequired; /**< Route record required */
    uint16_t nextHopAddr; /**< Next-hop address */
};

/** Binding Descriptor */
struct ZbZdoBindingDescT {
    uint64_t srcAddr; /**< SrcAddr */
    uint8_t srcEndpt; /**< SrcEndpoint */
    uint16_t clusterId; /**< ClusterId */
    /* dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst; /**< DstAddr */
};

/** NWK_addr_req */
struct ZbZdoNwkAddrReqT {
    uint16_t dstNwkAddr; /**<  Destination network address, not in spec */ /* typically set to ZB_NWK_ADDR_BCAST_RXON (0xfffd) */
    uint64_t extAddr; /**< IEEEAddr */
    enum ZbZdoAddrReqTypeT reqType; /**< RequestType */
    uint8_t startIndex; /**< StartIndex */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** NWK_addr_rsp */
struct ZbZdoNwkAddrRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint64_t extAddr; /**< IEEEAddrRemoteDev */
    uint16_t nwkAddr; /**< NWKAddrRemoteDev */
    uint8_t deviceCount; /**< NumAssocDev */
    uint8_t startIndex; /**< StartIndex */
    uint16_t deviceList[ZB_ZDO_ADDRRSP_DEVLIST_MAX]; /**< NWKAddrAssocDevList */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** IEEE_addr_req */
struct ZbZdoIeeeAddrReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
    enum ZbZdoAddrReqTypeT reqType; /**< RequestType */
    uint8_t startIndex; /**< StartIndex */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** IEEE_addr_rsp */
struct ZbZdoIeeeAddrRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint64_t extAddr; /**< IEEEAddrRemoteDev */
    uint16_t nwkAddr; /**< NWKAddrRemoteDev */
    uint8_t deviceCount; /**< NumAssocDev */
    uint8_t startIndex; /**< StartIndex */
    uint16_t deviceList[ZB_ZDO_ADDRRSP_DEVLIST_MAX]; /**< NWKAddrAssocDevList */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Node_Desc_req */
struct ZbZdoNodeDescReqT {
    uint16_t dstNwkAddr;
    /**< Destination network address. */
    uint16_t nwkAddrOfInterest;
    /**< Network address on interest. */
#if (CONFIG_ZB_REV >= 23)
    uint64_t joinerExtAddr;
    /**< Extended address of joiner. (R23+) */
    uint8_t suppKeyNegoMethod;
    /**< Supported key negotiation method. (R23+) */
    uint8_t suppPreSharedSecrets;
    /**< Supported Pre-Shared secrets. (R23+) */
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Node_Desc_rsp */
struct ZbZdoNodeDescRspT {
    enum ZbStatusCodeT status;
    /**< Status. */
    uint16_t nwkAddr;
    /**< short address. */
    struct ZbNodeDescriptorT nodeDesc;
    /**< Node descriptor structure. */
#if (CONFIG_ZB_REV >= 23)
    enum ZbSelKeyNegoMethodT selKeyNegoMethod;
    /**< Selected key negotiation method. (R23+) */
    enum ZbSelPreSharedSecretT selPreSharedSecret;
    /**< Selected Pre-Shared secret. (R23+) */
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Power_Desc_req */
struct ZbZdoPowerDescReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Power_Desc_rsp */
struct ZbZdoPowerDescRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t nwkAddr; /**< NWKAddrOfInterest */
    struct ZbPowerDescriptorT powerDesc; /**< PowerDescriptor */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Simple_Desc_req */
struct ZbZdoSimpleDescReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
    uint8_t endpt; /**< Endpoint */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Simple_Desc_rsp */
struct ZbZdoSimpleDescRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t nwkAddr; /**< NWKAddrOfInterest */
    struct ZbSimpleDescriptorT simpleDesc; /**< SimpleDescriptor */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Active_EP_req */
struct ZbZdoActiveEpReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Active_EP_rsp */
struct ZbZdoActiveEpRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t nwkAddr; /**< NWKAddrOfInterest */
    uint8_t activeEpCount; /**< ActiveEPCount */
    uint8_t activeEpList[ZB_ZDO_ENDPOINT_LIST_MAXSZ]; /**< ActiveEPList */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Match_Desc_req */
struct ZbZdoMatchDescReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
    uint16_t profileId; /**< ProfileID */
    uint8_t numInClusters; /**< NumInClusters */
    uint16_t inClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; /**< InClusterList */
    uint8_t numOutClusters; /**< NumOutClusters */
    uint16_t outClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; /**< OutClusterList */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Match_Desc_rsp */
struct ZbZdoMatchDescRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t nwkAddr; /**< NWKAddrOfInterest */
    uint8_t matchLength; /**< MatchLength */
    uint8_t matchList[ZB_ZDO_ENDPOINT_LIST_MAXSZ]; /**< MatchList */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. (R23+) */
#endif
};

/** Device_annce (ZB_ZDO_DEVICE_ANNCE) */
struct ZbZdoDeviceAnnceT {
    uint16_t nwkAddr; /**< NWKAddr */
    uint64_t extAddr; /**< IEEEAddr */
    uint8_t capability; /**< Capability */
};

/** System_Server_Discovery_req */
struct ZbZdoServerDiscReqT {
    uint16_t serverMask; /**< ServerMask */
};

/* System_Server_Discovery_rsp */
struct ZbZdoSsdRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t serverMask; /**< ServerMask */
    uint16_t nwkAddr; /**< Network address */ /* Added by Exegin */
};

/** Bind_req */
struct ZbZdoBindReqT {
    uint16_t target; /**< Destination for the ZDO Bind Request message */
    /* Binding Source */
    uint64_t srcExtAddr; /**< SrcAddress */
    uint8_t srcEndpt; /**< SrcEndp */
    /* Binding Cluster ID */
    uint16_t clusterId; /**< ClusterID */
    /* Binding Destination (not the destination of the message)
     * dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst; /**< DstAddrMode, DstAddress, DstEndp */
#if (CONFIG_ZB_REV >= 23)
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet.
     * NOTE: Only required for Golden Unit behaviour and if stack was built with
     * CONFIG_ZB_ENABLE_GU condition enabled, otherwise it's ignored. (R23+) */
#endif
};

/** Bind_rsp */
struct ZbZdoBindRspT {
    enum ZbStatusCodeT status; /**< Status */
};

/** Mgmt_Lqi_req */
struct ZbZdoLqiReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t startIndex; /**< StartIndex */
};

/** Mgmt_Lqi_rsp */
struct ZbZdoLqiRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t respAddr; /**< Response address */
    uint8_t neighborTblSz; /**< NeighborTableEntries */
    uint8_t startIndex; /**< StartIndex */
    uint8_t neighborListSz; /**< NeighborTableListCount */
    struct ZbZdoNeighborDescT neighborList[ZB_ZDO_NEIGHBOR_LIST_MAX_SZ]; /**< NeighborTableList */
};

/** Mgmt_Rtg_req */
struct ZbZdoRtgReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t startIndex; /**< StartIndex */
};

/** Mgmt_Rtg_rsp */
struct ZbZdoRtgRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t respAddr; /**< Response address */
    uint8_t routeTblSz; /**< RoutingTableEntries */
    uint8_t startIndex; /**< StartIndex */
    uint8_t routeListSz; /**< RoutingTableListCount */
    struct ZbZdoRoutingDescT routeList[ZB_ZDO_ROUTE_LIST_MAX_SZ]; /**< RoutingTableList */
};

/** Mgmt_Bind_req */
struct ZbZdoMgmtBindReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t startIndex; /**< StartIndex */
};

/** Mgmt_Bind_rsp */
struct ZbZdoMgmtBindRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint8_t bindTblSz; /**< BindingTableEntries */ /* Total number */
    uint8_t startIndex; /**< StartIndex */ /* Starting index being reported here */
    uint8_t bindListSz; /**< BindingTableListCount */ /* Number reported here */
    struct ZbZdoBindingDescT bindList[ZB_ZDO_BIND_LIST_MAX_SZ]; /**< BindingTableList */
};

/** ZDO Mgmt_Permit_Join_req */
struct ZbZdoPermitJoinReqT {
    uint16_t destAddr; /**< Short dst addr */
    uint8_t duration; /**< Permit join duration */
    uint8_t tcSignificance; /**< Boolean flag which if TRUE, indicates request to change TC policy. */
#if (CONFIG_ZB_REV >= 23)
    uint8_t tlvData[ZB_ZDO_PERMIT_JOIN_TLV_DATA_MAX_SZ];
    /**< Optional TLV data in case of R23 devices. In case of TC,
     * it shall be beacon appendix encapsulation TLV. (R23+) */
    uint8_t tlvDataLen;
    /**< Length of TLV data, if present. (R23+) */
#endif
};

/** ZDO Mgmt_Permit_Join_rsp */
struct ZbZdoPermitJoinRspT {
    enum ZbStatusCodeT status; /**< Status */
};

/** Mgmt_Nwk_Update_req */
struct ZbZdoNwkUpdateReqT {
    uint16_t destAddr; /**< Destination address */
    uint32_t channelMask; /**< ScanChannels */
    uint8_t scanDuration; /**< ScanDuration */ /* or ZB_ZDP_NWK_UPDATE_CHANNEL_SWITCH, or ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS */
    uint8_t scanCount; /**< ScanCount */
    uint8_t updateId; /**< nwkUpdateId */ /* only for ZB_ZDP_NWK_UPDATE_CHANNEL_SWITCH or ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS */
    uint16_t managerAddr; /**< nwkManagerAddr */ /* only for ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS */
};

/** Mgmt_Nwk_Enhanced_Update_req */
struct ZbZdoNwkEnhUpdateReqT {
    uint16_t destAddr; /**< Destination address */
    struct ZbChannelListT channelList; /**< ScanChannelsListStructure */
    uint8_t scanDuration; /**< ScanDuration */
    uint8_t scanCount; /**< ScanCount */
    uint8_t updateId; /**< nwkUpdateId */
    uint16_t managerAddr; /**< nwkManagerAddr */
};

/** Mgmt_Nwk_Update_notify */
struct ZbZdoNwkUpdateNotifyT {
    enum ZbStatusCodeT status; /**< Status */
    uint32_t scannedChannels; /**< ScannedChannels */
    uint16_t txTotal; /**< TotalTransmissions */
    uint16_t txFails; /**< TransmissionFailures */
    uint8_t channelListSz; /**< ScannedChannelsListCount */
    uint8_t channelList[ZB_ZDO_CHANNEL_LIST_MAXSZ]; /**< Channel energy list */
};

#define ZB_ZDO_MGMT_NWK_LEAVE_FLAG_REJOIN                   0x80U
#define ZB_ZDO_MGMT_NWK_LEAVE_FLAG_REMOVE_CHILDREN          0x40U

/** Mgmt_Nwk_Leave_req */
struct ZbZdoLeaveReqT {
    uint16_t destAddr; /**< Destination address */
    uint64_t deviceAddr; /**< Device address */
    uint8_t flags; /**< Remove Children, Rejoin */
};

/** Mgmt_Nwk_Leave_rsp */
struct ZbZdoLeaveRspT {
    enum ZbStatusCodeT status; /**< Status */
};

#define ZDP_JOINING_LIST_MAX_LEN            9U

/** Mgmt_Nwk_Ieee_Joining_List_req */
struct ZbZdoNwkIeeeJoinListReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t startIndex; /**< StartIndex */
};

/** Mgmt_Nwk_Ieee_Joining_List_rsp */
struct ZbZdoNwkIeeeJoinListRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t respAddr; /**< Response address */ /* not part of ZDP payload */
    uint8_t updateId; /**< IeeeJoiningListUpdateID */
    enum WpanJoinPolicyT joiningPolicy; /**< JoiningPolicy */
    uint8_t ieeeJoiningListTotal; /**< IeeeJoiningListTotal */
    uint8_t startIndex; /**< StartIndex */
    uint8_t ieeeJoiningListCount; /**< IeeeJoiningCount */
    uint64_t ieeeJoiningList[ZDP_JOINING_LIST_MAX_LEN]; /**< IeeeJoiningList */
};

#if (CONFIG_ZB_REV >= 23)
/**
 * Mgmt_Beacon_Survey_Req.
 */
struct ZbZdoBcnSurveyReqT {
    uint16_t dest_addr;
    /**< destination address. */
    struct ZbChannelListT scan_channels;
    /**< ScanChannelListStructure. */
    uint8_t config_bitmask;
    /**< bit 0: 0 Active Scan
     *          1 Enhanced Active Scan.
     *   bit 1 - 7: reserved. */
};

/**
 * Mgmt_Beacon_Survey_Rsp.
 */
struct ZbZdoBcnSurveyRspT {
    enum ZbStatusCodeT status;
    /**< Status. */
    uint8_t total_beacons;
    /**< Total number of beacons received during a beacon survey. */
    uint8_t on_nwk_beacons;
    /**< The total number of Zigbee Network beacons where the Extended PAN ID matches the local
     * device’s nwkExtendedPanId. */
    uint8_t potential_parent_beacons;
    /**< The total number of Zigbee Network beacons where the Extended PAN ID matches and the
     * Zigbee Beacon payload indicates End Device Capacity = TRUE. */
    uint8_t other_nwk_beacons;
    /**< The total number of IEEE 802.15.4 beacons from other Zigbee networks or other IEEE
     * 802.15.4 networks. Other Zigbee network beacons are defined as when the Extended PAN
     * ID does not match the local Extended PAN ID. */
    struct {
        uint8_t count;
        /**< Count of potential parents detected. */

        struct {
            uint16_t nwkaddr;
            /**< Network/Short address of potential parent. */
            uint8_t lqa;
            /**< LQA for the parent link. */
        } entry[ZB_NWK_POTENTIAL_PARENT_LIST_SIZE];
        /**< Single potential parent entry. */
    } potential_parent_list;
    /**< Potential parent list. */
};

/**
 * Clear_All_Bindings_req
 */
struct ZbZdoClearAllBindingsReqT {
    uint16_t destAddr;
    /**< destination address. */
    uint8_t eui64_count;
    /**< EUI64count */
    uint64_t eui64_array[ZB_ZDO_CLEAR_ALL_BIND_EUI64_LIST_MAX_SZ];
    /**< EUI64_array. */
};

/*
 * Clear_All_Bindings_rsp (R23)
 */
struct ZbZdoClearAllBindingsRspT {
    enum ZbStatusCodeT status; /**< Status */
};

/*
 * Security_Key_Negotiation_Req (R23)
 */
struct ZbZdoSecKeyNegotiationReqT {
    uint64_t srcExtAddr;
    /**< Source extended address. */
    uint16_t destAddr;
    /**< Destination short address. */
    uint8_t publicPoint[ZB_DLK_CURVE25519_PUB_POINT_LEN];
    /**< public portion of Curve25519 public/private key pair */
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains required information to relay the APS packet. */
};

/*
 * Security_Key_Negotiation_Rsp (R23)
 */
struct ZbZdoSecKeyNegotiationRspT {
    enum ZbStatusCodeT status;
    /**< Status. */
    uint64_t srcExtAddr;
    /**< Source extended address. */
    uint16_t dstAddr;
    /**< Destination short address. */
    uint8_t publicPoint[ZB_DLK_CURVE25519_PUB_POINT_LEN];
    /**< public portion of Curve25519 public/private key pair */
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains required information to relay the APS packet. */
};

/*
 * Security_Retrieve_Auth_Token_Req (R23)
 */
struct ZbZdoSecRetrieveAuthTokenReqT {
    uint16_t destAddr;
    /**< Destination short address. */
    enum zb_tlv_type authTokenId;
    /**< Authentication token ID. */
};

/*
 * Security_Retrieve_Auth_Token_Rsp (R23)
 */
struct ZbZdoSecRetrieveAuthTokenRspT {
    enum ZbStatusCodeT status; /**< Status */
};

/*
 * Security_Get_Authentication_Level_Req (R23)
 */
struct ZbZdoSecGetAuthLevelReqT {
    uint16_t destAddr;
    /**< Destination short address. */
    uint64_t ieeeAddrOfInterest;
    /**< EUI64 of device whose authentication level is required. */
    bool isApsUnsecured;
    /**< If set to true, it indicates that the ZDO request shall be sent without
     * APS encryption or with network security only. This is only needed for an R23
     * test case tc19.12.1/2 (TP/R23/ZDO/ATC-12). By default, the ZDO request shall
     * be APS secured. */
};

/*
 * Security_Get_Authentication_Level_Rsp (R23)
 */
struct ZbZdoSecGetAuthLevelRspT {
    enum ZbStatusCodeT status;
    /**< Processing status of request command. */
    uint64_t ieeeAddrRemoteNode;
    /**< EUI64 of the device whose authentication level is returned. */
    enum ZbApsInitialJoinAuthMethodT initialJoinMethod;
    /**< ZB_APS_INITIAL_JOIN_AUTH_NONE....  */
    enum ZbApsPostJoinKeyUpdateMethodT activeLinkKeyType;
    /**< ZB_APS_POST_JOIN_KEY_UPDATE_NONE....  */
};

/*
 * Security_Set_Configuration_Req (R23)
 */
struct ZbZdoSecSetConfigReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t tlvList[ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE - 1U];
    /**< Variable TLV list can be as big as the Maximum (unfragmented) APS payload,
     * assuming an APS secured packet with many-to-one source routing
     * (source routing level = 6).
     * decrement by 1U to account for seqnum. */
    uint8_t tlvListSize; /**< Exegin add on - size of TLV list. */
};

/*
 * Security_Set_Configuration_Rsp (R23)
 */
struct ZbZdoSecSetConfigRspT {
    enum ZbStatusCodeT status;
    /**< Status. */
    uint8_t tlvStatusCount;
    /**< Number of (TLV type ID, status) pairs. */
    uint8_t tlvIdStatusList[ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE - 3U];
    /**< TLV type ID and processing status list can be as big as the
     * Maximum (unfragmented) APS payload, assuming an APS secured
     * packet with many-to-one source routing (source routing level = 6).
     * decrement by 3U to account for seqnum, status & tlvStatusCount. */
};

/**
 * Security_Get_Configuration_Req.
 */
struct ZbZdoSecGetConfigReqT {
    uint16_t destAddr;
    /**< Short dst addr */
    uint8_t tlvCount;
    /**< Number of TLV ID's present in the TLV list. */
    uint8_t tlvIdList[ZB_ZDO_GET_CONFIG_REQ_TLV_LIST_SIZE];
    /**< Variable length TLV ID list. */
};

/**
 * Security_Get_Configuration_Rsp.
 */
struct ZbZdoSecGetConfigRspT {
    enum ZbStatusCodeT status;
    /**< Status. */
    uint8_t tlvList[ZB_ZDO_GET_CONFIG_RSP_TLV_LIST_SIZE];
    /**< Variable length TLV list. */
    uint8_t tlvListSize;
    /**< Size of TLV list. */
};

/**
 * Security_Key_Update_Req.
 */
struct ZbZdoSecKeyUpdateReqT {
    uint16_t dstNwkAddr;
    /**< Destination short address. */
    enum ZbSelKeyNegoMethodT selKeyNegoMethod;
    /**< Selected key negotiation method. */
    enum ZbSelPreSharedSecretT selPreSharedSecret;
    /**< Selected Pre-Shared secret. */
    uint16_t nodeId;
    /**< Node of the device that the subsequent fragmentation parameters apply to. */
    uint16_t maxInputBufferSize;
    /**< Maximum Reassembled Input Buffer Size. */
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains information required to relay the APS packet. */
};

/*
 * Security_Start_Key_Update_Rsp (R23)
 */
struct ZbZdoSecKeyUpdateRspT {
    enum ZbStatusCodeT status; /**< Status. */
};

/*
 * Security_Decommission_req (R23)
 */
struct ZbZdoSecDecommissionReqT {
    uint16_t destAddr;
    /**< destination address. */
    uint8_t eui64_count;
    /**< EUI64count */
    uint64_t eui64_array[ZB_ZDO_DECOMMISSION_EUI64_LIST_MAX_SZ];
    /**< EUI64_array. */
};

/*
 * Security_Decommission_rsp (R23)
 */
struct ZbZdoSecDecommissionRspT {
    enum ZbStatusCodeT status; /**< Status. */
};

/*
 * Security_Challenge_req (R23)
 */
struct ZbZdoSecChallengeReqT {
    uint16_t destAddr;
    /**< destination address. */
    uint64_t senderEUI64;
    /**< sender extended address. */
    uint8_t challengeValue[ZB_ZDO_SEC_CHALLENGE_LEN];
    /**< A randomly generated 64-bit value sent to a device to prove they
     * have the link key. */
};

/*
 * Security_Challenge_rsp (R23)
 */
struct ZbZdoSecChallengeRspT {
    enum ZbStatusCodeT status;
    uint64_t senderEUI64;
    /**< sender extended address. */
    uint8_t rxChallengeValue[ZB_ZDO_SEC_CHALLENGE_LEN];
    /**< Received APS link key transform. */
    uint32_t apsFrameCounter;
    /**< APS outgoing frame counter value. */
    uint32_t challengeSecFrameCounter;
    /**< The AES-CCM-128 outgoing frame counter used to generate the MIC over the
     * octet sequence { tag || length || responder EUI-64 || received challenge value ||
     * APS frame counter } using the special nonce and AES-128 key for frame counter
     * synchronization.. */
    uint8_t mic[ZB_ZDO_SEC_CHALLENGE_MIC_LEN];
    /**< The AES-128-CCM 64-bit MIC (security level 2) on all previous fields of this
     * TLV, excluding the challenge security frame counter, including Tag ID and
     * length fields. */
};

/*
 * Internal structure used to notify the NHLE on receiving
 * the Security_Key_Update_Req command (R23)
 */
struct ZbZdoSecKeyUpdateIndT {
    uint64_t srcExtAddr;
    /**< Source extended address */
    enum ZbSelKeyNegoMethodT selKeyNegoMethod;
    /**< Selected key negotiation method. */
    enum ZbSelPreSharedSecretT selPreSharedSecret;
    /**< Selected Pre-Shared secret. */
    uint16_t nodeId;
    /**< Node of the device that the subsequent fragmentation parameters apply to. */
    uint8_t frag_options;
    /**< e.g. ZB_TLV_FRAG_PARAM_OPTS_APS_FRAG_SUPP ... */
    uint16_t maxInputBufferSize;
    /**< Maximum Reassembled Input Buffer Size. */
    struct ZbApsRelayInfoT relayInfo;
    /**< Contains required information to relay the APS packet. */
};
#endif

/**
 * Return and increment the next ZDO sequence number.
 * @param zb Zigbee stack instance
 * @return Returns ZDO sequence number.
 */
uint8_t ZbZdoGetNextSeqNum(struct ZigBeeT *zb);

/**
 * Retrive the 16-bit short address for a device if given an extended address.
 * @param zb Zigbee stack instance
 * @param req NWK_Addr_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkAddrReq(struct ZigBeeT *zb, struct ZbZdoNwkAddrReqT *req,
    void (*callback)(struct ZbZdoNwkAddrRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrive the 64-bit extended address for a device if given a short network address.
 * @param zb Zigbee stack instance
 * @param req IEEE_Addr_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoIeeeAddrReq(struct ZigBeeT *zb, struct ZbZdoIeeeAddrReqT *req,
    void (*callback)(struct ZbZdoIeeeAddrRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the node descriptor for a device.
 * @param zb Zigbee stack instance
 * @param req Node_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNodeDescReq(struct ZigBeeT *zb, struct ZbZdoNodeDescReqT *req,
    void (*callback)(struct ZbZdoNodeDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the power descriptor for a device.
 * @param zb Zigbee stack instance
 * @param req Power_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoPowerDescReq(struct ZigBeeT *zb, struct ZbZdoPowerDescReqT *req,
    void (*callback)(struct ZbZdoPowerDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the simple descriptor for a device.
 * @param zb Zigbee stack instance
 * @param req Simple_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoSimpleDescReq(struct ZigBeeT *zb, struct ZbZdoSimpleDescReqT *req,
    void (*callback)(struct ZbZdoSimpleDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the active endpoint list for a device.
 * @param zb Zigbee stack instance
 * @param req Active_ep_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoActiveEpReq(struct ZigBeeT *zb, struct ZbZdoActiveEpReqT *req,
    void (*callback)(struct ZbZdoActiveEpRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the mach descriptor for a device.
 * @param zb Zigbee stack instance
 * @param req Match_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoMatchDescReq(struct ZigBeeT *zb, struct ZbZdoMatchDescReqT *req,
    void (*callback)(struct ZbZdoMatchDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Send a ZDO Match-Desc request and receive multiple responses.
 * The responses, if any, are received by the callback.
 * Each callback contains the result of a callback from a single device.
 * The callback will be called each time a response is received.
 * After a timeout period, the callback is called with a status of ZB_ZDP_STATUS_TIMEOUT to indicate the internal response filter
 * is removed and any additional responses for this request are not processed.
 * @param zb Zigbee stack instance
 * @param req Match_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZB_ZDP_STATUS_SUCCESS for received responses, ZB_ZDP_STATUS_TABLE_FULL if there's a problem starting the request,
 * ZB_ZDP_STATUS_TIMEOUT when the stack decides to stop receiving responses, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoMatchDescMulti(struct ZigBeeT *zb, struct ZbZdoMatchDescReqT *req,
    void (*callback)(struct ZbZdoMatchDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the system server mask from a device.
 * @param zb Zigbee stack instance
 * @param req Server_desc_rec
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoSystemServerDiscReq(struct ZigBeeT *zb, struct ZbZdoServerDiscReqT *req,
    void (*callback)(struct ZbZdoSsdRspT *rsp, void *cb_arg), void *arg);

/**
 * Send a ZDO Match-Desc-Multi request and receive multiple responses.
 * @param zb Zigbee stack instance
 * @param req Server_desc_rec
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZbZdoSystemServerDiscMulti(struct ZigBeeT *zb, struct ZbZdoServerDiscReqT *req,
    void (*callback)(struct ZbZdoSsdRspT *rsp, void *cb_arg), void *arg);

/**
 * Send a Device_annce message to the network. Automatically called by the stack after joining.
 * @param zb Zigbee stack instance
 * @param deviceAnncePtr Pointer to Device_annce structure
 * @return Returns void
 */
void ZbZdoDeviceAnnce(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *deviceAnncePtr);

/**
 * Send a Device_annce message to the network using aliasing.
 * @param zb Zigbee stack instance
 * @param deviceAnncePtr Pointer to Device_annce structure
 * @return Returns void
 */
void ZbZdoDeviceAnnceAlias(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *deviceAnncePtr);

/**
 * Parse the device announce received.
 * @param structPtr Pointer to structure
 * @param buf Packet buffer
 * @param len Packet length
 * @return size of the device announce size, which will be 11, or other value on error
 */
int ZbZdoParseDeviceAnnce(struct ZbZdoDeviceAnnceT *structPtr, const uint8_t *buf, unsigned int len);

/**
 * Register a filter in the ZDO for the application to receive Device_Annce messages.
 * Freed by calling ZbZdoFilterRemove.
 * @param zb Zigbee stack instance
 * @param filter Optional pointer to a pre-allocated ZbZdoFilterT structure. If NULL, the struct will be allocated via ZbHeapAlloc().
 * @param callback Function to call upon reception of a valid ZDO Device-Annce message.
 * Callback can return ZB_MSG_CONTINUE to process other filters,
 * or ZB_MSG_DISCARD if all processing of this message should stop.
 * @param arg Callback argument
 * @return Pointer to filter structure
 */
struct ZbZdoFilterT * ZbZdoDeviceAnnceFilterRegister(struct ZigBeeT *zb, struct ZbZdoFilterT *filter,
    enum zb_msg_filter_rc (*callback)(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *annce, uint8_t seqno, void *arg), void *arg);

/**
 * Perform a ZDO Bind operation.
 * @param zb Zigbee stack instance
 * @param req Bind_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoBindReq(struct ZigBeeT *zb, struct ZbZdoBindReqT *req,
    void (*callback)(struct ZbZdoBindRspT *rsp, void *cb_arg), void *arg);

/**
 * Perform a ZDP Unbind Bind operation.
 * @param zb Zigbee stack instance
 * @param req Unbind_req
 * @param Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoUnbindReq(struct ZigBeeT *zb, struct ZbZdoBindReqT *req,
    void (*callback)(struct ZbZdoBindRspT *rsp, void *cb_arg), void *arg);

#if (CONFIG_ZB_REV >= 23)
/**
 * Send a ZDO Security Clear all bindings request command.
 * @param zb Zigbee stack instance
 * @param req clear all bindings req struct
 * @param callback callback function pointer
 * @param arg argument to response handler function
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZbZdoClearAllBindingsReq(struct ZigBeeT *zb, struct ZbZdoClearAllBindingsReqT *req,
    void (*callback)(struct ZbZdoClearAllBindingsRspT *rsp, void *cb_arg), void *arg);

/**
 * Send a ZDO Security Retrieve authentication token request command.
 * @param zb Zigbee stack instance.
 * @param req get device token req struct
 * @param callback Function to call on completion
 * @param arg argument to response handler function
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZbZdoSecRetrieveAuthTokenReq(struct ZigBeeT *zb, struct ZbZdoSecRetrieveAuthTokenReqT *req,
    void (*callback)(struct ZbZdoSecRetrieveAuthTokenRspT *rsp, void *cb_arg), void *arg);

/**
 * Send a ZDO Security get authentication level request command.
 * @param zb Zigbee stack instance
 * @param req get authentication level req struct
 * @param callback Function to call on completion
 * @param arg argument to response handler function
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZbZdoSecGetAuthLevelReq(struct ZigBeeT *zb, struct ZbZdoSecGetAuthLevelReqT *req,
    void (*callback)(struct ZbZdoSecGetAuthLevelRspT *rsp, void *cb_arg), void *arg);

/**
 * Send a ZDO Security set configuration request command.
 * @param zb Zigbee stack instance
 * @param req set configuration req struct.
 * @param callback Function to call on completion
 * @param arg argument to response handler function
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZbZdoSecSetConfigReq(struct ZigBeeT *zb, struct ZbZdoSecSetConfigReqT *req,
    void (*callback)(struct ZbZdoSecSetConfigRspT *rsp, void *cb_arg), void *arg);

/**
 * Sends ZDO Security get configuration request command.
 *
 * @param zb ZigBee stack structure.
 * @param req get configuration req struct.
 * @param callback callback function pointer.
 * @param arg argument to response handler function.
 * @return enum ZbStatusCodeT
 */
enum ZbStatusCodeT ZbZdoSecGetConfigReq(struct ZigBeeT *zb, struct ZbZdoSecGetConfigReqT *req,
    void (*callback)(struct ZbZdoSecGetConfigRspT *rsp, void *cb_arg), void *arg);

/**
 * Sends ZDO Security_Decommission_req command.
 *
 * @param zb ZigBee stack structure.
 * @param req decommission req struct.
 * @param callback callback function pointer.
 * @param arg argument to response handler function.
 * @return enum ZbStatusCodeT
 */
enum ZbStatusCodeT ZbZdoSecDecommissionReq(struct ZigBeeT *zb, struct ZbZdoSecDecommissionReqT *req,
    void (*callback)(struct ZbZdoSecDecommissionRspT *rsp, void *cb_arg), void *arg);

/**
 * Sends ZDO Security_Challenge_req command.
 *
 * @param zb ZigBee stack structure.
 * @param req challenge req struct.
 * @param callback callback function pointer.
 * @param arg argument to response handler function.
 * @return enum ZbStatusCodeT
 */
enum ZbStatusCodeT ZbZdoSecChallengeReq(struct ZigBeeT *zb, struct ZbZdoSecChallengeReqT *req,
    void (*callback)(struct ZbZdoSecChallengeRspT *rsp, void *cb_arg), void *arg);

/**
 * Sends ZDO Mgmt_Beacon_Survey_req command.
 *
 * @param zb ZigBee stack structure.
 * @param req Beacon survey req struct.
 * @param callback callback function pointer.
 * @param arg argument to response handler function.
 * @return enum ZbStatusCodeT
 */
enum ZbStatusCodeT ZbZdoBcnSurveyReq(struct ZigBeeT *zb, struct ZbZdoBcnSurveyReqT *req,
    void (*callback)(struct ZbZdoBcnSurveyRspT *rsp, void *cb_arg), void *arg);
#endif

/**
 * Perform a Mgmt_Lqi_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Lqi_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoLqiReq(struct ZigBeeT *zb, struct ZbZdoLqiReqT *req,
    void (*callback)(struct ZbZdoLqiRspT *rsp, void *cb_arg), void *arg);

/**
 * Perform a Mgmt_Rtg_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Rtg_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoRtgReq(struct ZigBeeT *zb, struct ZbZdoRtgReqT *req,
    void (*callback)(struct ZbZdoRtgRspT *rsp, void *cb_arg), void *arg);

/**
 * Perform a Mgmt_Bind_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_bind_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoMgmtBindReq(struct ZigBeeT *zb, struct ZbZdoMgmtBindReqT *req,
    void (*callback)(struct ZbZdoMgmtBindRspT *rsp, void *cb_arg), void *arg);

/**
 * Sends a ZDO Mgmt_permit_join_req command.
 *
 * With R23 and above, there are new fields added to the ZDO Mgmt_permit_join_req command
 * e.g, 'Frame Control' & 'Global Beacon Appendix'
 *
 * In case of centralized security networks only TC can send this command with TLV_UPDATE
 * bit set to 1 in frame control field. And if a non-TC device invokes this API with
 * TLV_UPDATE bit set, it will be treated as an invalid request.
 *
 * @param zb Zigbee stack instance
 * @param req Mgmt_permit_joining_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoPermitJoinReq(struct ZigBeeT *zb, struct ZbZdoPermitJoinReqT *req,
    void (*callback)(struct ZbZdoPermitJoinRspT *rsp, void *cb_arg), void *arg);

/**
 * Perform a Mgmt_Nwk_update_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Nwk_update_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkUpdateReq(struct ZigBeeT *zb, struct ZbZdoNwkUpdateReqT *req,
    void (*callback)(struct ZbZdoNwkUpdateNotifyT *reqPtr, void *cb_arg), void *arg);

/**
 * Perform a Mgmt_Nwk_Enhanced_Update_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Nwk_Enhanced_Update_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkEnhUpdateReq(struct ZigBeeT *zb, struct ZbZdoNwkEnhUpdateReqT *req,
    void (*callback)(struct ZbZdoNwkUpdateNotifyT *reqPtr, void *cb_arg), void *arg);

/**
 * Send an unsolicited Mgmt_Nwk_Update_notify command to the network manager. Does not wait for any response.
 * @param zb Zigbee stack instance
 * @param reqPtr Mgmt_Nwk_Update_notify
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkUpdateNotify(struct ZigBeeT *zb, struct ZbZdoNwkUpdateNotifyT *reqPtr);

/**
 * Register a filter in the ZDO for the application to receive Mgmt_Nwk_Update_notify messages.
 * Freed by calling ZbZdoFilterRemove.
 * @param zb Zigbee stack instance
 * @param filter Optional pointer to a pre-allocated ZbZdoFilterT structure. If NULL, the struct will be allocated via ZbHeapAlloc().
 * @param callback Function to call upon reception of a valid ZDO NWK-Update-Notify message.
 * Callback can return ZB_MSG_CONTINUE to process other filters,
 * or ZB_MSG_DISCARD if all processing of this message should stop.
 * @param arg Callback argument
 * @return Pointer to filter structure
 */
struct ZbZdoFilterT * ZbZdoNwkUpdateNotifyFilterRegister(struct ZigBeeT *zb, struct ZbZdoFilterT *filter,
    enum zb_msg_filter_rc (*callback)(struct ZigBeeT *zb, struct ZbZdoNwkUpdateNotifyT *msg, uint8_t seqno, void *arg), void *arg);

/**
 * Register a filter in the ZDO for the application to receive Mgmt_Nwk_Enhanced_Update_notify messages.
 * Freed by calling ZbZdoFilterRemove.
 * @param zb Zigbee stack instance
 * @param filter Optional pointer to a pre-allocated ZbZdoFilterT structure. If NULL, the struct will be allocated via ZbHeapAlloc().
 * @param callback Function to call upon reception of a valid ZDO NWK-Update-Notify message.
 * Callback can return ZB_MSG_CONTINUE to process other filters,
 * or ZB_MSG_DISCARD if all processing of this message should stop.
 * @param arg Callback argument
 * @return Pointer to filter structure
 */
struct ZbZdoFilterT * ZbZdoNwkEnhUpdateNotifyFilterRegister(struct ZigBeeT *zb, struct ZbZdoFilterT *filter,
    enum zb_msg_filter_rc (*callback)(struct ZigBeeT *zb, struct ZbZdoNwkUpdateNotifyT *msg, uint8_t seqno, void *arg), void *arg);

/**
 * Perform a Mgmt_leave_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Leave_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoLeaveReq(struct ZigBeeT *zb, struct ZbZdoLeaveReqT *req,
    void (*callback)(struct ZbZdoLeaveRspT *rsp, void *cb_arg), void *arg);

/**
 * Send an IEEE-Joining-List request.
 * @param zb Zigbee stack instance
 * @param IEEE-Joining-List.request Structure
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkIeeeJoinListReq(struct ZigBeeT *zb, struct ZbZdoNwkIeeeJoinListReqT *req,
    void (*callback)(struct ZbZdoNwkIeeeJoinListRspT *rsp, void *cb_arg), void *arg);

/**
 * Send an IEEE-Joining-List.response.
 * Used primarily by GB868 applications.
 * @param zb Zigbee stack instance
 * @param dstNwkAddr Destination network address
 * @param startIndex Start index
 * @param seqnum Sequence number
 * @param fromRequest Use start index from request
 * @return Number of entries sent in response
 */
unsigned int ZbZdoNwkIeeeJoinListRsp(struct ZigBeeT *zb, uint16_t dstNwkAddr,
    uint8_t startIndex, uint8_t seqnum, bool fromRequest);
/* Helper to broadcast the entire list to the network */

/**
 * Send an IEEE-Joining-List.response broadcast message for all entries in the IEEE Join List.
 * Used primarily by GB868 applications.
 * @param zb Zigbee stack instance
 * @return Returns number of entries sent
 */
unsigned int ZbZdoNwkIeeeJoinListBcastAll(struct ZigBeeT *zb);

#if (CONFIG_ZB_REV >= 23)
/**
 * Send a ZDO Security Key Update Request.
 * @param zb Zigbee stack instance
 * @param req security key update req struct
 * @param callback Function to call on completion
 * @param arg argument to response handler function
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZbZdoSecKeyUpdateReq(struct ZigBeeT *zb, struct ZbZdoSecKeyUpdateReqT *req,
    void (*callback)(struct ZbZdoSecKeyUpdateRspT *rsp, void *cb_arg), void *arg);

/**
 * Sends ZDO Security Key Negotiation response.
 * @param zb ZigBee stack structure.
 * @param rsp ZDO SEC Key negotiation rsp struct.
 * @param seqnum Sequence number.
 * @return enum ZbStatusCodeT
 */
enum ZbStatusCodeT ZbZdoSecKeyNegotiationRsp(struct ZigBeeT *zb, struct ZbZdoSecKeyNegotiationRspT *rsp,
    uint8_t seqnum);

/**
 * Send a ZDO Security Key Negotiation Request.
 * @param zb Zigbee stack instance
 * @param req security key negotiation req struct
 * @param callback Function to call on completion
 * @param arg argument to ZDO key negotiation response handler
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZbZdoSecKeyNegotiationReq(struct ZigBeeT *zb, struct ZbZdoSecKeyNegotiationReqT *req,
    void (*callback)(struct ZbApsmeKeyNegoConfirmT *conf, void *cb_arg), void *arg);
#endif

/**
 * Remove a ZDO filter.
 * @param zb Zigbee stack instance
 * @param filter Filter instance to remove and free
 * @return Returns void
 */
void ZbZdoFilterRemove(struct ZigBeeT *zb, struct ZbZdoFilterT *filter);

#endif
