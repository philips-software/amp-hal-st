/**
 * @file zigbee.tlv.h
 * @heading TLV Utilities
 * @brief ZigBee TLV implementation.
 * @author Exegin Technologies Limited
 * @copyright Copyright [2018 - 2022] Exegin Technologies Limited. All rights reserved.
 *
 * This file defines all the ZigBee TLV APIs and macros.
 */

#ifndef ZIGBEE_TLV_H
#define ZIGBEE_TLV_H

#include <stdint.h>
#include <stdbool.h>

/*lint -e621 "Identifier clash [MISRA Rule 5.1 (REQUIRED), MISRA Rule 5.2 (REQUIRED),
 * MISRA Rule 5.4 (REQUIRED), MISRA Rule 5.5 (REQUIRED)]" */

/** Clear All Bindings Req EUI64 TLV minimum length
 * TLV format: EUI64 Count (1 octet) + EUI64 (8 octets) + .....
 */
#define ZB_TLV_CLEAR_ALL_BIND_REQ_EUI64_MIN_LEN             9U

/** Key Negotiation Req/Rsp Curve25519 Public Point TLV length
 * TLV format: Device EUI64 (8 octets) + Public Point (32 octets).
 */
#define ZB_TLV_KEY_NEGO_CURVE25519_PUB_MIN_LEN              40U

/** Authentication Token ID TLV length
 * TLV format: Authentication Token ID (TLV Type ID) (1 octet)
 */
#define ZB_TLV_AUTH_TOKEN_ID_MIN_LEN                        1U

/** ZDO Security_Get_authentication_level_req Target IEEE Address TLV length
 * TLV format: IEEEAddrOfInterest (8 octets)
 */
#define ZB_TLV_TARGET_IEEE_ADDR_MIN_LEN                     8U

/** ZDO Security_Get_authentication_level_rsp Device Authentication Level TLV length
 * TLV format: IEEEAddrRemoteNode (8 octets) + InitialJoinMethod (1 octet)
 * + ActiveLinkKeyType (1 octet)
 */
#define ZB_TLV_DEVICE_AUTH_LEVEL_MIN_LEN                    10U

/** Key Update Req/Rsp Selected Key Negotiation Method TLV length
 * TLV format: Selected Key Enumeration (1 octet) + Selected Pre-shared secret (1 octet)
 * + Source Device EUI64 (8 octets)
 */
#define ZB_TLV_KEY_UPDATE_SEL_KEY_NEGO_METHOD_MIN_LEN       10U

/** ZDO Security_Decommission_Req Device EUI64 List TLV minimum length
 * TLV format: EUI64 Count (1 octet) + EUI64 (8 octets) + .....
 */
#define ZB_TLV_DEVICE_EUI64_LIST_MIN_LEN                    9U

/** ZDO Security_Challenge_Req APS frame counter challenge TLV length
 * TLV format: Sender EUI64 (8 octets) + Challenge Value (8 octets)
 */
#define ZB_TLV_APS_FC_CHALLENGE_MIN_LEN                     16U

/** Security Challenge Rsp APS frame counter response TLV length
 * TLV format: Sender EUI64 (8 octets) + Received Challenge Value (8 octets)
 * + APS Frame Counter (4 octets) + Challenge Security Frame counter (4 octets)
 * + MIC (8 octets)
 */
#define ZB_TLV_APS_FC_RESPONSE_MIN_LEN                      32U

/** Node_Desc_Rsp Selected Key Negotiation Method TLV length
 * TLV format: Source Device EUI64 (8 octets) + Selected Key Enumeration (1 octet)
 * + Selected Pre-shared secret (1 octet)
 */
#define ZB_TLV_NODE_DESC_RSP_SEL_KEY_NEGO_METHOD_MIN_LEN    10U

/** Mgmt_Beacon_Survey_Req Configuration TLV minimum length
 * TLV format: Configuration bitmask (1 octet) + ScanChannelListStructure, channel count (1 octet)
 * + channelmask (4 octet)
 */
#define ZB_TLV_BCN_SURVEY_CONFIG_MIN_LEN                    6U

/** Mgmt_Beacon_Survey_Rsp Results TLV minimum length
 * TLV format: Number of potential parents from the current Zigbee network (1 octet) + Number of detected non-
 * Zigbee networks (1 octet) + Number of detected Zigbee networks (1 octet) + Total Beacons Surveyed (1 octet)
 */
#define ZB_TLV_BCN_SURVEY_RESULTS_MIN_LEN                   4U

/** Mgmt_Beacon_Survey_Rsp potential parents TLV minimum length
 * TLV format: Current parent (2 octets) + LQA (1 octet) + Count of potential parents (1 octet) + .....
 */
#define ZB_TLV_BCN_SURVEY_POTENTIAL_PARENTS_MIN_LEN         4U

/** Mgmt_Beacon_Survey_Rsp potential parents TLV maximum length
 * TLV format: Current Parent Short Address (2 octets) + LQA (1 octet) + Count of Potential Parents (1 octets)
 * + 5 * (Potential Parent Short Address (2 octets) + LQA (1 octet))
 */
#define ZB_TLV_BCN_SURVEY_POTENTIAL_PARENTS_MAX_LEN         19U

/** APS Relay message TLV contains the message to be relayed from TC to joiner or vice versa.
 * TLV format: Source/Destination EUI64 (8 octets) + message to be relayed (varies)
 */
#define ZB_TLV_RELAY_MSG_MIN_LEN                            8U

/** Link-Key Features & Capabilities TLV.
 * TLV format: Features (1 byte)
 */
#define ZB_TLV_LINK_KEY_FEATURES_CAPABILITIES_MIN_LEN       1U

/** Manufacturing Specific Global TLV minimum length
 * TLV format: ZigBee Manufacturer ID (2 octets) + additional data.....
 */
#define ZB_TLV_MFG_SPECIFIC_MIN_LEN                         2U

/** Supported Key Negotiation Methods TLV
 * TLV format: Key Negotiation Protocols Bitmask (1 octet) + Supported Pre-shared bitmask (1 octet)
 * + Source Device EUI64 (8 octets)
 */
#define ZB_TLV_SUPP_KEY_NEGO_METHOD_MIN_LEN                 10U

/** PANID conflict report Global TLV
 * TLV format: PANID conflict count (2 octets)
 */
#define ZB_TLV_PANID_CONFLICT_REPORT_MIN_LEN                2U

/** Next PAN ID Change Global TLV
 * TLV format: Next PANID (2 octets).
 */
#define ZB_TLV_NEXT_PANID_MIN_LEN                           2U

/** Next Channel Change Global TLV
 * TLV format: Next channel mask (4 octets).
 */
#define ZB_TLV_NEXT_CHANNEL_MIN_LEN                         4U

/** 128-bit Symmetric Passphrase TLV
 * TLV format: 128-bit Symmetric Passphrase (16 bytes).
 */
#define ZB_TLV_PASSPHRASE_128_MIN_LEN                       16U

/** Router Information TLV
 * TLV format: Bitmask of local data (2 bytes).
 */
#define ZB_TLV_ROUTER_INFO_MIN_LEN                          2U

/** Fragmentation Parameters Global TLV
 * TLV format: Node ID (2 octets) + Fragmentation Options (1 octets) + Maximum Incoming Transfer Unit (2 octets).
 */
#define ZB_TLV_FRAG_PARAM_MIN_LEN                           5U

/** Fragmentation options.
 * Bit 0: APS Fragmentation Supported.
 * Bit 1-7: Reserved for future use.
 * */
#define ZB_TLV_FRAG_PARAM_OPTS_APS_FRAG_SUPP                1U

/** Joiner Encapsulation Global TLV
 * TLV format: Additional TLVs.....
 */
#define ZB_TLV_JOINER_ENCAPSULATION_MIN_LEN                 0U

/** Beacon Appendix Encapsulation Global TLV
 * TLV format: Additional TLVs.....
 */
#define ZB_TLV_BEACON_APPENDIX_ENCAPSULATION_MIN_LEN        0U

/** Configuration Parameters TLV
 * TLV format: Bitmask of effected configuration attributes (2 octets).
 */
#define ZB_TLV_CONFIG_PARAM_MIN_LEN                         2U

#define ZB_TLV_DEVICE_CAP_EXT_GLOBAL_MIN_LEN                2U

/**
 * Bitmask for configuration parameters TLV.
 * ---------------------------------------------------------------------------
 *   Bit  Area                    Effected Configuration Attribute
 * ---------------------------------------------------------------------------
 *   0    AIB                     apsZdoRestrictedMode
 *   1    Device Security Policy  requireLinkKeyEncryptionForApsTransportKey
 *   2    NIB                     nwkLeaveRequestAllowed
 *   3-15 Reserved                Reserved
 * ---------------------------------------------------------------------------
 */
#define ZB_TLV_CONFIG_PARAM_APS_ZDO_RESTRICTED_MODE     0x01U
#define ZB_TLV_CONFIG_PARAM_REQUIRE_LINK_KEY_ENC        0x02U
#define ZB_TLV_CONFIG_PARAM_NWK_LEAVE_REQ_ALLOWED       0x04U

/** Router has connectivity to a Hub device. */
#define ZB_TLV_ROUTER_INFO_HUB_CONNECTIVITY             0x01U

/** Indicates device is up for > 24hrs. */
#define ZB_TLV_ROUTER_INFO_NWK_UPTIME                   0x02U

/** Router can be preferred to be a parent. */
#define ZB_TLV_ROUTER_INFO_PREFERRED_PARENT             0x04U

/** Indicates device has battery backup. */
#define ZB_TLV_ROUTER_INFO_BATTERY_BACKUP               0x08U

/** Enhanced beacon request/response supported. */
#define ZB_TLV_ROUTER_INFO_EBR_SUPPORT                  0x10U

/** MAC data poll keep-alive mechanism supported. */
#define ZB_TLV_ROUTER_INFO_MAC_DATA_POLL                0x20U

/** End device timeout keep-alive mechanism supported. */
#define ZB_TLV_ROUTER_INFO_KEEP_ALIVE                   0x40U

/** Power negotiation supported. */
#define ZB_TLV_ROUTER_INFO_POWER_NEGO                   0x80U

/*lint -save -e849 "enum1 and enum2 have same value [LINT]" */

/**
 * ZigBee TLV type ID enumeration.
 *
 * TLV IDs have been split into local and global TLVs based on their usage.
 * Local TLVs are specific to a single message and their tag IDs may overlap
 * with the local TLV IDs of different messages. Range of local TLV tag IDs is 0-63.
 *
 * However, global TLVs have a single format across all messages and layers
 * and are used in 2 or more different messages. Global TLV IDs represent
 * a global state and is not completely ephemeral in nature. Range of global
 * TLV tag IDs is 64-255.
 */
enum zb_tlv_type {
    /*-----------------------------------------------------
     * Local TLVs (0-63)
     *-----------------------------------------------------
     */
    /* ZDO Clear_All_Bindings_req */
    ZB_TLV_CLEAR_ALL_BIND_REQ_EUI64 = 0U, /**< Clear_All_Bindings_req EUI64 TLV. */

    /* ZDO Security_Key_negotiation_req/rsp */
    ZB_TLV_KEY_NEGO_CURVE25519_PUB = 0U, /**< Security_Key_negotiation_req/rsp Curve25519 Public Point TLV. */

    /* ZDO Security_Retrieve_authentication_token_req */
    ZB_TLV_AUTH_TOKEN_ID = 0U, /**< Security_Retrieve_authentication_token_req Authentication Token ID TLV. */

    /* ZDO Security_Get_authentication_level_req */
    ZB_TLV_TARGET_IEEE_ADDR = 0U, /**< ZDO Security_Get_authentication_level_req Target IEEE Address TLV. */

    /* ZDO Security_Get_authentication_level_rsp */
    ZB_TLV_DEVICE_AUTH_LEVEL = 0U, /**< ZDO Security_Get_authentication_level_rsp Device Authentication Level TLV. */

    /* ZDO Security_Set_Configuration_rsp */
    ZB_TLV_PROCESSING_STATUS = 0U, /**< ZDO Security_Set_Configuration_rsp Processing Status TLV. */

    /* ZDO Security_Key_update_req/rsp */
    ZB_TLV_KEY_UPDATE_SEL_KEY_NEGO_METHOD = 0U, /**< Security_Key_update_req/rsp selected key negotiation method TLV. */

    /* ZDO Security_Decommission_req */
    ZB_TLV_DEVICE_EUI64_LIST = 0U, /**< ZDO Security_Decommission_req Device EUI64 List TLV. */

    /* ZDO Security_Challenge_req */
    ZB_TLV_APS_FC_CHALLENGE = 0U, /**< ZDO Security_Challenge_req APS frame counter challenge TLV. */

    /* ZDO Security_Challenge_rsp */
    ZB_TLV_APS_FC_RESPONSE = 0U, /**< ZDO Security_Challenge_rsp APS frame counter response TLV. */

    /* ZDO Node_Desc_Rsp */
    ZB_TLV_NODE_DESC_RSP_SEL_KEY_NEGO_METHOD = 0U, /**< Node_Desc_Rsp selected key negotiation method TLV. */

    /* ZDO Mgmt_Beacon_Survey_req/rsp */
    ZB_TLV_BCN_SURVEY_CONFIG = 0U, /**< Mgmt_Beacon_Survey_req Configuration TLV. */
    ZB_TLV_BCN_SURVEY_RESULTS = 1U, /**< Mgmt_Beacon_Survey_rsp Results TLV. */
    ZB_TLV_BCN_SURVEY_POTENTIAL_PARENTS = 2U, /**< Mgmt_Beacon_Survey_rsp Potential Parents TLV. */

    /* APS Relay Command */
    ZB_TLV_RELAY_MSG = 0U, /**< APS Relay message TLV. */

    /* APSME-TRANSPORT-KEY.req */
    ZB_TLV_LINK_KEY_FEATURES_CAPABILITIES = 0U, /**< Link-Key Features & Capabilities TLV. */

    /*-----------------------------------------------------
     * Global TLVs (64-255)
     *-----------------------------------------------------
     */
    ZB_TLV_MFG_SPECIFIC = 64U, /**< Manufacturer specific TLV. */
    ZB_TLV_SUPP_KEY_NEGO_METHOD = 65U, /**< Supported key negotiation method TLV. */
    ZB_TLV_PANID_CONFLICT_REPORT = 66U, /**< PANID conflict report global TLV. */
    ZB_TLV_NEXT_PANID = 67U, /**< Next PAN ID TLV. */
    ZB_TLV_NEXT_CHANNEL = 68U, /**< Next channel change TLV. */
    ZB_TLV_PASSPHRASE_128 = 69U, /**< Symmetric passphrase TLV. */
    ZB_TLV_ROUTER_INFO = 70U, /**< Router Information TLV. */
    ZB_TLV_FRAG_PARAM = 71U, /**< Fragmentation Parameters TLV. */
    ZB_TLV_JOINER_ENCAPSULATION = 72U, /**< Joiner Encapsulation TLV. */
    ZB_TLV_BEACON_APPENDIX_ENCAPSULATION = 73U, /**< Beacon Appendix Encapsulation TLV. */
    /* discontinuity. */
    ZB_TLV_CONFIG_PARAM = 75U, /**< Configuration params TLV. */
    ZB_TLV_DEVICE_CAP_EXT_GLOBAL = 76U, /**< Device Capability Extension Global TLV (ID 76) */
    ZB_TLV_ID_RESERVED = 77U /**< Global TLV ID's >= this ID are reserved. */
};
/*lint -restore */

/**
 * ZigBee TLV structure definition.
 */
struct zb_tlv {
    enum zb_tlv_type type;
    uint16_t length;
    /**< Valid range for length field is 1-256. If the caller of
     * TLV API zb_tlv_encode() provides length field of a TLV
     * as 0 or >256 it is treated as invalid and returned with error
     * ZB_TLV_STATUS_INVALID_LEN.
     */
    const uint8_t *value;
    uint8_t flags;
};

/** Returns number of elements in a tlv array. */
#define ZB_TLV_ARRAY_SZ(x)                  (sizeof(x) / sizeof(struct zb_tlv))

/** TLV overhead length: 1 (TLV ID) + 1 (payload length). */
#define ZB_TLV_OVERHEAD_LEN                 2U

/** Minimum length of a TLV: 1 (TLV ID) + 1 (Length) + 1 (min value size) */
#define ZB_TLV_MINIMUM_LEN                  (ZB_TLV_OVERHEAD_LEN + 1U)

/** Minimum length of a TLV value field. */
#define ZB_TLV_MINIMUM_LEN_VALUE            1U

/** Maximum length of a TLV value field. */
#define ZB_TLV_MAXIMUM_LEN_VALUE            256U

/** TLV attribute flag NONE. */
#define ZB_TLV_FLAGS_NONE                   0x00U
/** TLV attribute flag MANDATORY. */
#define ZB_TLV_FLAGS_MANDATORY              0x01U

/** TLV encode/decode operation status. */
/** Duplicate TLVs other than ZB_TLV_MFG_SPECIFIC found. */
#define ZB_TLV_STATUS_DUPLICATE             -1
/** TLV length < minimum required length. */
#define ZB_TLV_STATUS_INVALID_LEN           -2
/** TLV length causes known data to be truncated. */
#define ZB_TLV_STATUS_BUFFER_TRUNCATED      -3
/** Encapsulation TLV found inside another encapsulation TLV. */
#define ZB_TLV_STATUS_ENCAPSULATION_ERROR   -4
/** Invalid input parameters found. */
#define ZB_TLV_STATUS_INVALID_PARAMS        -5

/* Device Capability Extension Global TLV (ZB_TLV_DEVICE_CAP_EXT_GLOBAL) */
#define ZB_TLV_DEVICE_CAP_MASK_IS_ZVD       0x0001U /* Zigbee Direct Virtual Device */

/**
 * Given a list of desired TLVs, examine all the TLVs as per the 'General
 * TLV processing rules'. Update the desired TLV list with all matching TLVs
 * found in the buffer. It is expected that the desired TLV list will have the
 * length field populated with the 'minimum known length' for all the entries.
 * Desired TLVs found in the buffer with length < than the 'minimum known length'
 * are considered 'malformed TLVs'. All the unknown TLVs found in the buffer are
 * locally stored to ensure no duplicate TLVs are present.
 *
 * @note: This API only verifies the length of all the desired TLVs to be > than
 * the 'minimum known length' and does not check for TLVs with 'known data being
 * truncated'. It is the responsibility of caller to perform this check.
 *
 * @note: This API only verifies that the encapsulation TLVs do not contain any
 * other encapsulation TLVs. However, it does not perform general TLV processing
 * checks on the payload of the encapsulation TLV. It is the responsibility of
 * the caller to decode the payload of encapsulation TLV to verify the validity
 * of the TLVs enclosed.
 *
 * @param buffer (IN) input buffer with TLVs.
 * @param buffer_length (IN) input buffer length.
 * @param tlv_array (IN/OUT) the TLVs that we are looking for.
 * @param tlv_count (IN) number of TLVs in the array we are looking for.
 * @return length of buffer decoded on success.
 *         ZB_TLV_STATUS_FAILURE, if any of the input parameters were invalid.
 *         ZB_TLV_STATUS_DUPLICATE_TLV, if a duplicate TLV is found.
 *         ZB_TLV_STATUS_BUFFER_TRUNCATED, if any TLV is found to be malformed.
 */
int zb_tlv_decode(const uint8_t *buffer, unsigned int buffer_length, struct zb_tlv *tlv_array,
    const unsigned int tlv_count);

/**
 * Check all mandatory TLVs have non-NULL value fields.
 *
 * @param tlv_array (IN) TLV array.
 * @param tlv_count (IN) depth of input TLV array.
 * @return true if all mandatory TLVs were found false otherwise
 */
bool zb_tlv_decode_check_mandatory(const struct zb_tlv *tlv_array, const unsigned int tlv_count);

/**
 * Check if the TLV type exists in the input buffer. This API will return
 * failure if the TLV does not exist or is found to be truncated.
 *
 * @param buffer (IN) input buffer for TLV processing.
 * @param buffer_length (IN) input buffer length.
 * @param type (IN/OUT) TLV type ID being searched.
 * @return true if TLV exists, false otherwise
 */
bool zb_tlv_type_exists(uint8_t *buffer, unsigned int buffer_length, enum zb_tlv_type type);

/**
 * Compute the size of buffer required, if the input TLVs were to be encoded.
 *
 * @param tlv_array (IN) input TLV array.
 * @param tlv_count (IN) depth of input TLV array.
 * @return size of buffer required to encode input TLVs.
 */
int zb_tlv_encode_length(struct zb_tlv *tlv_array, const unsigned int tlv_count);

/**
 * Given a list of TLVs, this API encodes TLV list into input buffer.
 *
 * @param buffer (IN) input buffer with TLVs.
 * @param buffer_length (IN) input buffer length.
 * @param tlv_array (IN) array of TLVs to be encoded.
 * @param tlv_count (IN) depth of TLV array.
 * @return length encoded, negative on error
 */
int zb_tlv_encode(uint8_t *buffer, unsigned int buffer_length,
    const struct zb_tlv *tlv_array, const unsigned int tlv_count);

#endif /* ZIGBEE_TLV_H */
