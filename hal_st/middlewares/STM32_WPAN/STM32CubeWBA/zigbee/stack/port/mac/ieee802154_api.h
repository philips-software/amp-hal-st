/* Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved. */

/* API between MAC and NHLE */

#ifndef IEEE802154_API_H
#define IEEE802154_API_H

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "ieee802154.h"
#include "ieee802154_enums.h"
#include "ieee802154_crc.h"
#include "mcp.h"

#if defined(__GNUC__)
# define WPAN_WARN_UNUSED __attribute__((warn_unused_result))
#else
# define WPAN_WARN_UNUSED
#endif

#ifndef MIN
# define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif

struct WpanPublicT;

/*----------------------------------------------------------------------------
 * phy.h
 *----------------------------------------------------------------------------
 */
#ifndef PHY_RSSI_INVALID
# define PHY_RSSI_INVALID                   (-128)
#endif

/* PHY_TX_POWER_MAX */
#ifndef PHY_TX_POWER_MAX
# define PHY_TX_POWER_MAX                   14 /* dBm */
# define PHY_TX_POWER_MIN                   (-20) /* dBm */
#endif

/*----------------------------------------------------------------------------
 * Packet Durations (symbols)
 *----------------------------------------------------------------------------
 */
#define WPAN_CONST_BASE_SLOT_DURATION           60U
#define WPAN_CONST_NUM_SUPERFRAME_SLOTS         16U
#define WPAN_CONST_UNIT_BACKOFF_PERIOD          20U
#define WPAN_CONST_TURNAROUND_TIME              12U
#define WPAN_CONST_BASE_SUPERFRAME_DURATION     (WPAN_CONST_BASE_SLOT_DURATION * WPAN_CONST_NUM_SUPERFRAME_SLOTS)

/* macResponseWaitTime - multiples of aBaseSuperframeDuration (960) symbols. */
#define WPAN_CONST_RESPONSE_WAIT_DEFAULT        32U /* 30,720 symbols (@ 62,500 sym/sec = 492 mS) */

/*----------------------------------------------------------------------------
 * Superframe Specification field bits
 *----------------------------------------------------------------------------
 */
#define WPAN_SUPERFRAME_MASK_BEACON             0x000fU
#define WPAN_SUPERFRAME_MASK_SUPERFRAME         0x00f0U
#define WPAN_SUPERFRAME_MASK_FINAL_CAP          0x0f00U
#define WPAN_SUPERFRAME_MASK_BATTERY_EXT        0x1000U
#define WPAN_SUPERFRAME_MASK_RESERVED           0x2000U
#define WPAN_SUPERFRAME_MASK_PAN_COORD          0x4000U
#define WPAN_SUPERFRAME_MASK_PERMIT_JOIN        0x8000U

#define WPAN_SUPERFRAME_OFFSET_BEACON           0U
#define WPAN_SUPERFRAME_OFFSET_SUPERFRAME       4U
#define WPAN_SUPERFRAME_OFFSET_FINAL_CAP        8U
#define WPAN_SUPERFRAME_OFFSET_BATTERY_EXT      12U
#define WPAN_SUPERFRAME_OFFSET_PAN_COORD        14U
#define WPAN_SUPERFRAME_OFFSET_PERMIT_JOIN      15U

/*----------------------------------------------------------------------------
 * GTS Specification field bits
 *----------------------------------------------------------------------------
 */
#define WPAN_GTS_MASK_COUNT                     0x07U
#define WPAN_GTS_MASK_PERMIT                    0x80U
#define WPAN_GTS_OFFSET_COUNT                   0U
#define WPAN_GTS_OFFSET_PERMIT                  7U

/*----------------------------------------------------------------------------
 * Pending Address field bits
 *----------------------------------------------------------------------------
 */
#define WPAN_PENDADDR_MASK_SHORT                0x07U
#define WPAN_PENDADDR_MASK_LONG                 0x70U
#define WPAN_PENDADDR_MASK_RESERVED             0x88U

#define WPAN_PENDADDR_OFFSET_SHORT              0U
#define WPAN_PENDADDR_OFFSET_LONG               4U

/*----------------------------------------------------------------------------
 * Transmit Power
 *----------------------------------------------------------------------------
 */
/* Transmit power format. */
#define WPAN_TXPOWER_POWER_MASK                 0x3fU
#define WPAN_TXPOWER_POWER_MAX                  31
#define WPAN_TXPOWER_POWER_MIN                  -32
#define WPAN_TXPOWER_TOLERANCE_MASK             0xc0U
#define WPAN_TXPOWER_TOLERANCE_1DB              0x00U
#define WPAN_TXPOWER_TOLERANCE_3DB              0x40U
#define WPAN_TXPOWER_TOLERANCE_6DB              0x80U

/* Sign-extend the 6bit transmit power value to a signed integer. */
#define WPAN_TXPOWER_AS_INT(_v_)                ((int)(((_v_) & 0x3f) ^ ~0x1f) - ~0x1f)

#if 0 /* security not supported */
/*----------------------------------------------------------------------------
 * Security (CONFIG_MAC_SECURITY)
 *----------------------------------------------------------------------------
 */
#define WPAN_KEY_SIZE                       16
#define WPAN_KEY_SOURCE_MAX_SIZE            8
#define WPAN_MIC_SIZE(_level_)              ((2 << ((_level_) & 0x3)) & ~0x3)
#define WPAN_ENCRYPTION_LEVEL               0x04

/* AES-CCM* constants used in IEEE 802.15.4.  */
#define WPAN_AES_CCM_L                      2
#define WPAN_NONCE_SIZE                     (15 - WPAN_AES_CCM_L)

/* Security level comparison macros. */
#define WPAN_SECURITY_LEVEL_EQ(_a_, _b_)    ((_a_) == (_b_))
#define WPAN_SECURITY_LEVEL_GT(_a_, _b_) \
    ((((_a_) & WPAN_ENCRYPTION_LEVEL) >= ((_b_) & WPAN_ENCRYPTION_LEVEL)) && (((_a_) & 0x3) > ((_b_) & 0x3)))
#define WPAN_SECURITY_LEVEL_GTEQ(_a_, _b_) \
    ((((_a_) & WPAN_ENCRYPTION_LEVEL) >= ((_b_) & WPAN_ENCRYPTION_LEVEL)) && (((_a_) & 0x3) >= ((_b_) & 0x3)))
#define WPAN_SECURITY_LEVEL_LT(_a_, _b_)    WPAN_SECURITY_LEVEL_GT(_b_, _a_)
#define WPAN_SECURITY_LEVEL_LTEQ(_a_, _b_)  WPAN_SECURITY_LEVEL_GTEQ(_b_, _a_)

/* Device descriptor */
struct WpanKeyDeviceT {
    uint16_t panId;
    uint16_t shortAddress;
    uint64_t extAddress;
    uint32_t frameCounter;
    uint8_t exempt;
};

/*
 * The IEEE 802.15.4 spec is a gong show when it comes to the layout of the PIB
 * for the MAC security attributes, so we've had to do a little redesign when it
 * comes to storing this stuff in an embedded-type device.
 *
 * idMode/source:
 *   Each key is identified by a string of 5-9 bytes. It would seem that this
 *   identifier would typically contain an address or something in it, but that
 *   isn't specified by the IEEE. The spec states that each key may have a list
 *   of these identifiers to match it against, but instead we use a 1:1 mapping
 *   between ids and keys. You can always just create copies of the key for
 *   each id if you really need 1:*.
 *
 * index:
 *   The key identifier actually consists of a 32/64 bit string concatenated
 *   with an 8-bit key index. We choose to break it apart and keep the source
 *   and index separate.
 *
 * devicelist:
 *   The IEEE spec calls for list of device descriptors for each device that
 *   may communicate with this key, which seems to duplicate the macDeviceTable
 *   attribute in the PIB. So, instead of wasting gobs of memory by putting a
 *   table in each key descriptor, we use a bitmask in the key descriptor which
 *   specifies the indexes of the entries in the macDeviceTable that may use
 *   this key. Note that this will limit the number of entries in the
 *   macDeviceTable to the bit-width of this mask (32 in this case).
 *   The device descriptor list is only used for making policy decisions on
 *   incoming frames, and is ignored entirely for outgoing frames.
 *
 * blacklist:
 *   In each of the device descriptors, a blacklist bit is kept which states
 *   whether we are allowed to receive packets from this device. These bits
 *   are kept in a separate bitmask. The mask of allowed devices can be found
 *   by (device & (~blacklist)).
 *
 * usage:
 *   The IEEE calls for another list in the key descriptor here, one entry for
 *   each type of MAC frame that this key can be used for. Instead we use a
 *   bitmask to store this information. Use the WPAN_KEY_USAGE macro to get
 *   the appropriate bit for a given frame type or command. The first bits
 *   are for one of the frame types (except commands). The bits for commands
 *   start after the frame type bits.
 *   Note that this will limit the maximum possible commands types to 16-3 = 13
 *   before the bitmask is exhausted.
 *   The usage list is only used for making policy decisions on incoming frames
 *   and is ignored entirely for outgoing frames.
 *
 * key:
 *   Finally! Something simple: 16-bytes contains the actual key being used.
 */
/* Key structure */
struct WpanKeyT {
    uint8_t idMode; /* IEEE802154_SCTRL_KEYID_ */
    uint8_t source[WPAN_KEY_SOURCE_MAX_SIZE];
    uint8_t index;
    uint32_t devicelist; /* For incoming policy only. */
    uint32_t blacklist; /* For incoming policy only. */
    uint16_t usage; /* For incoming policy only. */
    uint8_t key[WPAN_KEY_SIZE];
};
#endif

#if 0 /* Beacon enabled networks only (currently not supported) */
/*---------------------------------------------------------
 * MLME-SYNC
 *---------------------------------------------------------
 */
/*
 * The MLME-SYNC service is used to synchronize the device with a parnet's
 * superframe configuration. This is only used in beacon-enabled networks, and
 * is not currently being used or implemented anywhere.
 */
/* MLME-SYNC.request */
struct WpanSyncReqT {
    uint16_t logicalChannel;
    uint8_t channelPage;
    uint8_t trackBeacon;
};

/* MLME-SYNC-LOSS.indication */
struct WpanSyncLossIndT {
    uint8_t lossReason;
    uint16_t panId;
    uint16_t logicalChannel;
    uint8_t channelPage;
    struct mcp_security security;
};
#endif

/*--------------------------------------------------------------------------
 * License Challenge/Response Authentication protocol.
 *--------------------------------------------------------------------------
 */
/*
 * The challenge/response authentication protocol is used
 * to check that the hardware was correctly licensed to use
 * the stack.
 *
 * The host software writes a challenge into the device via
 * an MLME-SET.request to the macLicenseChallenge attribute.
 * The hardware will provide an ECDSA signature of the
 * challenge and a random salt via the macLicenseResponse
 * attribute.
 *
 * The underlying cryptography uses ECDSA and the NIST-P256
 * prime curve. The challenge should be a 256-bit random
 * number. Refer to ANSI X9.62 for a guide on how to generate
 * good random numbers.
 *
 * The CA signature is computed with a AES-MMO hash of the certificate with
 * the r and s elements truncated:
 *      AES-MMO(eui64 | flags | rflags | pen | product | license | reserved | pubx)
 *
 * The integer fields (EUI64, flags, rflags, pen and license) are in little endian.
 * The product string should be padded with zeros in the trailing array positions.
 * The cryptographic fields (pubx, r and s) are in big endian.
 */
#define WPAN_ECDSA_POINT_SIZE   32 /* Size of NIST-P256. */
#define WPAN_ECDSA_VERSION      0x01 /* Current version of the response. */
#define WPAN_ECDSA_FLAG_VERSION 0x00ff /* The version contains the low 8-bits of the flags .*/
#define WPAN_ECDSA_FLAG_PZ      0x0100 /* The compressed Y-coordinate of the public key. */
#define WPAN_ECDSA_FLAG_NOKEY   0x0200 /* The certificate is not backed by a private key - license only. */

/* The Certificate format */
struct wpan_license {
    uint64_t eui64; /* The EUI-64 programmed during manufacturing. */
    uint16_t flags; /* Flags about the certificate.  */
    uint8_t euirange; /* The number of additional implied EUIs */
    uint8_t reserved1;
    uint32_t pen; /* Private enterprise number. */
    uint8_t product[8]; /* Product identifier string. */
    uint32_t license; /* Licensed product bits. */
    uint8_t reserved2[4]; /* Reserved for future use - set to zero. */
    uint8_t pubx[WPAN_ECDSA_POINT_SIZE];
    /* Certificate signature key (r and s) */
    uint8_t r[WPAN_ECDSA_POINT_SIZE];
    uint8_t s[WPAN_ECDSA_POINT_SIZE];
};

/* Size of the license, minus the trailing signature. */
#define WPAN_LICENSE_NON_SIG_SIZE   (sizeof(struct wpan_license) - (WPAN_ECDSA_POINT_SIZE * 2))

/*---------------------------------------------------------
 * Diagnostics Information structure
 *---------------------------------------------------------
 */
/* mcp_macDiagClusterInfo */
/* Make sure this struct is self-packing and aligned to 8-byte boundaries for transfering via MCP. */
struct mac_diag_info {
    uint32_t mac_rx_bcast; /* ZCL_DIAG_SVR_ATTR_MAC_RX_BCAST */
    uint32_t mac_tx_bcast; /* ZCL_DIAG_SVR_ATTR_MAC_TX_BCAST */
    uint32_t mac_rx_ucast; /* ZCL_DIAG_SVR_ATTR_MAC_RX_UCAST */
    uint32_t mac_tx_ucast; /* ZCL_DIAG_SVR_ATTR_MAC_TX_UCAST */
    uint16_t mac_tx_ucast_retry; /* ZCL_DIAG_SVR_ATTR_MAC_TX_UCAST_RETRY */
    uint16_t mac_tx_ucast_fail; /* ZCL_DIAG_SVR_ATTR_MAC_TX_UCAST_FAIL */
    uint16_t phy_to_mac_queue_lim_reached; /* ZCL_DIAG_SVR_ATTR_PHY_MAC_QUEUE_LIM */
    uint16_t packet_validate_drop_count; /* ZCL_DIAG_SVR_ATTR_PACKET_VAL_DROP_COUNT (Note, also in zb_diag_info) */
    uint16_t avg_mac_retry_per_aps_msg_sent; /* ZCL_DIAG_SVR_ATTR_AVG_MAC_RETRY_PER_APS_MSG */
    uint8_t last_message_lqi; /* ZCL_DIAG_SVR_ATTR_LAST_MSG_LQI */
    int8_t last_message_rssi; /* ZCL_DIAG_SVR_ATTR_LAST_MSG_RSSI */
    /* uint8_t rsv[4]; EXEGIN - should have this, but nothing will follow this struct */
};

/*---------------------------------------------------------------
 * MCP Error
 *---------------------------------------------------------------
 */
void wpan_mcp_error_init(struct mcp_error *error, uint8_t seqno);

/*---------------------------------------------------------------
 * Attribute Payload Types
 *---------------------------------------------------------------
 */
/* Scalar */
/* struct mcp_scalar */

/* Payload */
struct wpan_attr_payload {
    struct mcp_attr_hdr attr; /* len = MCP_TLV_LEN(sizeof(struct mcp_attr_hdr)) */
    struct mcp_payload_hdr payload;
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

/*---------------------------------------------------------------
 * MLME-GET
 *---------------------------------------------------------------
 */
/* MLME-GET.request */
struct wpan_get_req {
    struct mcp_getreq req;
    struct mcp_attr_hdr attr;
};

void wpan_get_req_init(struct wpan_get_req *req);
void wpan_get_conf_scalar_init(struct mcp_getconf *conf, uint16_t id, uint8_t seqno);
void wpan_get_conf_payload_init(struct mcp_getconf *conf, uint16_t id, uint8_t seqno);

bool WPAN_WARN_UNUSED wpan_get_uint64(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint64_t *value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_get_uint32(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint32_t *value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_get_uint16(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint16_t *value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_get_uint8(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint8_t *value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_get_license(struct WpanPublicT *publicPtr, struct wpan_license *license);

/*---------------------------------------------------------------
 * MLME-SET
 *---------------------------------------------------------------
 */

/* MLME-SET.request */
/* {
 *     struct mcp_setreq req;
 *     struct mcp_scalar scalar;
 * }
 *   or
 * {
 *      struct mcp_setreq req;
 *      struct wpan_attr_payload payload;
 * }
 */

/* MLME-SET.confirm */
struct wpan_set_conf {
    struct mcp_setconf conf;
    struct mcp_attr_hdr attr;
};

void wpan_set_req_scalar_init(struct mcp_setreq *req, uint16_t id);
void wpan_set_req_payload_init(struct mcp_setreq *req, uint16_t id);
void wpan_set_conf_init(struct wpan_set_conf *conf, uint16_t id, uint8_t seqno);

bool WPAN_WARN_UNUSED wpan_set_uint64(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint64_t value64, uint16_t index);
bool WPAN_WARN_UNUSED wpan_set_uint32(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint32_t value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_set_uint16(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint16_t value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_set_uint8(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint8_t value, uint16_t index);
bool WPAN_WARN_UNUSED wpan_set_bcnpayload(struct WpanPublicT *publicPtr, uint8_t *buf, uint16_t len);

/*---------------------------------------------------------------
 * MLME-GET/SET TX Power
 *---------------------------------------------------------------
 */
bool WpanGetTxPower(struct WpanPublicT *publicPtr, int *powerPtr, uint8_t *tolerancePtr);
bool WpanSetTxPower(struct WpanPublicT *publicPtr, int power, uint8_t tolerance);

/*---------------------------------------------------------
 * MCPS-PURGE
 *---------------------------------------------------------
 */
/*
 * The MCPS-PURGE service is used to cancel a pending MCPS-DATA.request that
 * has not yet been transmitted. The pending transation should be identified
 * by the MSDU handle provided in the MCPS-DATA.request. If the MSDU handle
 * not unique, then the behavior is unspecified.
 *
 * This function is optional, and does not need to be implemented on all
 * devices. Although, it is recommended for all devices that make use of the
 * indirect transmission service.
 */

/* MCPS-PURGE.request */
/* struct mcp_purgereq */

/* MCPS-PURGE.confirm */
/* struct mcp_purgeconf */

void wpan_purge_req_init(struct mcp_purgereq *req);
void wpan_purge_conf_init(struct mcp_purgeconf *conf, uint8_t seqno);
bool wpan_purge_req(struct WpanPublicT *publicPtr, struct mcp_purgereq *req, struct mcp_purgeconf *conf);

/*---------------------------------------------------------------
 * MLME-DATA
 *---------------------------------------------------------------
 */
/*
 * The MCPS-DATA.indication service is used to pass received packets to the
 * NHLE. And will be called for any received data packet that passes
 * address recognition.
 *
 * The MCPS-DATA.request service is used to begin the transmission of a data
 * packet. The payload buffer only needs to be valid for the duration of the
 * function call, and is not required to be allocated from the heap. The
 * MCPS-DATA.confirm service returns the results of the transmission attempt
 * and is returned asynchronously via a callback function.
 *
 * These functions are mandatory, and are the "meat and potatoes" of the
 * IEEE 802.15.4 device. For most applications, the acknowledgment and
 * indirect services must be supported (ZigBee). GTS transmissions are only
 * required for beacon-enabled PAN's.
 *
 * The MSDU handle is an 8-bit parameter specified by the upper layers to
 * associate a request with the asynchronous response. Earlier version. Due to
 * the 8-bit size, pointers may not be used here, but table indices are fine.
 *
 * For indirect and GTS transmissions, the packet will not be transmitted
 * immediately and may need to wait for a long duration. If necessary, the NHLE
 * can cancel the pending packet using the MCPS-PURGE service below. Likewise,
 * it is recommended that the MSDU handle should be unique within a single
 * device.
 */

/* Sniff Info */
#define MCP_TLV_SNIFF_INFO                  0xf0
struct mcp_sniff_info {
    uint8_t type; /* MCP_TLV_SNIFF_INFO */
    uint8_t len; /* MCP_TLV_LEN(sizeof(struct mcp_sniffer_info)) */
    uint8_t rsv[2]; /* EXEGIN - add modulation? */
    uint32_t freq_khz;
};

/* Maximum length of an MCPS-DATA.indication when not in promiscuous mode */
#define MCP_DATA_IND_MAX_LEN \
    (sizeof(struct mcp_dataind) + \
     sizeof(struct mcp_linkquality) + \
     sizeof(struct mcp_security) + \
     sizeof(struct mcp_payload_hdr) + \
     WPAN_CONST_MAX_PHY_PACKET_SIZE)

/* Maximum length of an MCPS-DATA.indication when in promiscuous mode (sniffer packets) */
#define MCP_DATA_IND_PROMISC_MAX_LEN \
    (sizeof(struct mcp_dataind) + \
     sizeof(struct mcp_linkquality) + \
     sizeof(struct mcp_sniff_info) + \
     sizeof(struct mcp_payload_hdr) + \
     WPAN_CONST_MAX_PHY_PACKET_SIZE)

/*---------------------------------------------------------------
 * MLME-RESET
 *---------------------------------------------------------------
 */
/* MLME-RESET.request */
/* struct mcp_resetreq */

/* MLME-RESET.confirm */
/* struct mcp_resetconf */

void wpan_reset_req_init(struct mcp_resetreq *req);
void wpan_reset_conf_init(struct mcp_resetconf *conf, uint8_t seqno);
bool wpan_reset_req(struct WpanPublicT *publicPtr, struct mcp_resetreq *req, struct mcp_resetconf *conf);

/*---------------------------------------------------------------
 * MLME-POLL
 *---------------------------------------------------------------
 */
/*
 * The MLME-POLL service is used to retrieve pending packets from a device's
 * parent.Issuing this request will transmit a data request command to the
 * specified device, and wait for an acknowledgement. If the frame-pending
 * bit in the acknowledgement is not set, then this will return a status of
 * NO_DATA.
 *
 * Strictly according to the specification, this should also wait for the
 * parent to dequeue and transmit the pending frames before returning SUCCESS
 * but in reality most devices just return after receiving the ACK.
 *
 * This function is mandatory for all deivce types, but is used most requently
 * deivces with macRxOnWhenidle set to FALSE.
 */
/* MLME-POLL.request */
struct wpan_poll_req {
    struct mcp_pollreq req;
    struct mcp_security security;
};

/* MLME-POLL.confirm */
/* struct mcp_pollconf */

/* MLME-POLL.indication */
/* struct mcp_pollind */

void wpan_poll_req_init(struct wpan_poll_req *req);
void wpan_poll_conf_init(struct mcp_pollconf *conf, uint8_t seqno);
void wpan_poll_ind_init(struct mcp_pollind *ind);
bool wpan_poll_req(struct WpanPublicT *publicPtr, struct wpan_poll_req *req);

/*---------------------------------------------------------------
 * MLME-START
 *---------------------------------------------------------------
 */
/*
 * The MLME-START service is used to begin operating as a full-function device, prior
 * to receiving an MLME-START.request, the device should not respond transmit beacon
 * frames, nor should it respond to beacon request commands.
 *
 * On full-function devices, this service may also be used to change the active channel
 * and beacon-timing parameters. (Note that for RFD's there is no service to change
 * channels, and the device should directly set the phyCurrentChannel PIB attribute (if
 * unsupported, orphan scanning is likely the only method left to rejoin a moved PAN).
 *
 * If the coordRealignment parameter is set to true, then device will also broadcast a
 * coordinator realignment command prior to changing its parameters.
 *
 * This service is mandatory for full-function devices, and is never used for end
 * devices.
 */
/* MLME-START.request */
/* struct mcp_startreq */

/* MLME-START.confirm */
/* struct mcp_startconf */

void wpan_start_req_init(struct mcp_startreq *req);
void wpan_start_conf_init(struct mcp_startconf *conf, uint8_t seqno);
bool wpan_start_req(struct WpanPublicT *publicPtr, struct mcp_startreq *req, struct mcp_startconf *conf);

/*---------------------------------------------------------------
 * MLME-RX-ENABLE
 *---------------------------------------------------------------
 */
/*
 * The MLME-RX-ENABLE service is used to enable (or disable) the receiver for a
 * period of time. This function should return immediately without waiting for
 * the receiver to be turned on.
 *
 * This function has no effect when macRxOnWhenIdle is true. Otherwise, the
 * receipt of this function should supersede all previous requests. Likewise,
 * setting rxOnDuration to 0 will disable the receiver if currently enabled.
 *
 * The deferPermit and rxOnTime parameters are ignored unless operating on a
 * beacon enabled network, in which case they specify which superframe this
 * command applies to, and when in the superframe to enable the receiver.
 *
 * On non-beacon enabled networks, the enable time takes effect immediately.
 */

/* MLME-RX-ENABLE.request */
/* struct mcp_rxenreq; */

/* MLME-RX-ENABLE.confirm */
/* struct mcp_rxenconf; */

void wpan_rx_enable_req_init(struct mcp_rxenreq *req);
void wpan_rx_enable_conf_init(struct mcp_rxenconf *conf, uint8_t seqno);
bool wpan_rx_enable_req(struct WpanPublicT *publicPtr, struct mcp_rxenreq *req, struct mcp_rxenconf *conf);

/*---------------------------------------------------------------
 * MLME-ASSOCIATE
 *---------------------------------------------------------------
 */
/*
 * The MLME-ASSOCIATE service is used to facilitate the MAC-layer
 * association procedure, and consists of four primitives:
 * MLME-ASSOCIATE.request - Requests the device to associate to a parent.
 * MLME-ASSOCIATE.confirm - Returns the result of an association attempt.
 * MLME-ASSOCIATE.indication - Notifies the NHLE that a device requested assoication.
 * MLME-ASSOCIATE.response - Requests that the device respond to an association request.
 *
 * MLME-ASSOCIATE.request and MLME-ASSOCIATE.confirm are implemented
 * with a blocking function, and are mandatory for all IEEE 802.15.4
 * devices.
 *
 * MLME-ASSOCIATE.indication is passed from the driver to the NHLE using
 * a callback function. The NHLE should not expect the
 * MLME-ASSOCIATE.indication callback to be executed from a thread-safe
 * context, the exact context is device dependent. After receiving this
 * indication, the NHLE must respond with an MLME-ASSOCIATE.response
 * within macResponseWaitTime.
 *
 * MLME-ASSOCIATE.response is implemented as a blocking function, returning
 * an MLME-COMM-STATUS.indication after the device successfully polls for the
 * association response command, or the indirect transmission expires.
 *
 * MLME-ASSOCIATE.indication and MLME-ASSOCIATE.response are mandatory for
 * full function devices (ZigBee coordinators and routers), and is optional
 * for reduced function devices (ZigBee end devices).
 *
 */

/* MLME-ASSOCIATE.request */
struct wpan_assoc_req {
    struct mcp_assocreq req;
    struct mcp_security security;
};

/* MLME-ASSOCIATE.confirm */
struct wpan_assoc_conf {
    struct mcp_assocconf conf;
    struct mcp_security security;
};

/* MLME-ASSOCIATE.indication */
struct wpan_assoc_ind {
    struct mcp_assocind ind;
    struct mcp_security security;
};

/* MLME-ASSOCIATE.response */
struct wpan_assoc_rsp {
    struct mcp_assocrsp rsp;
    struct mcp_security security;
};

void wpan_assoc_req_init(struct wpan_assoc_req *req);
void wpan_assoc_conf_init(struct wpan_assoc_conf *conf, uint8_t seqno);
void wpan_assoc_ind_init(struct wpan_assoc_ind *ind);
void wpan_assoc_rsp_init(struct wpan_assoc_rsp *rsp);
bool wpan_assoc_req(struct WpanPublicT *publicPtr, struct wpan_assoc_req *req);
bool wpan_assoc_rsp(struct WpanPublicT *publicPtr, struct wpan_assoc_rsp *rsp);

/*---------------------------------------------------------------
 * MLME-ORPHAN
 *---------------------------------------------------------------
 */
/* MLME-ORPHAN.indication */
struct wpan_orphan_ind {
    struct mcp_orphanind ind;
    struct mcp_security security;
};

/* MLME-ORPHAN.response */
struct wpan_orphan_rsp {
    struct mcp_orphanrsp rsp;
    struct mcp_security security;
};

void wpan_orphan_ind_init(struct wpan_orphan_ind *ind);
void wpan_orphan_rsp_init(struct wpan_orphan_rsp *rsp);
bool wpan_orphan_rsp(struct WpanPublicT *publicPtr, struct wpan_orphan_rsp *rsp);

/*---------------------------------------------------------
 * MLME-COMM-STATUS
 *---------------------------------------------------------
 */
/* MLME-COMM-STATUS.indication */
struct wpan_comm_status_ind {
    struct mcp_commind ind;
    struct mcp_security security;
};

void wpan_comm_status_ind_init(struct wpan_comm_status_ind *ind);

/*---------------------------------------------------------------
 * MLME-SCAN
 *---------------------------------------------------------------
 */
/*
 * The MLME-SCAN service is used to facilitate the scanning of channels for a
 * variety of purposes, and is implemented as a blocking function call (note
 * that this can potentially block for up to 250 seconds per channel!).
 *
 * Every scan type can specify a range of channels to scna using the scanChannels
 * bitmask, and a duration to scan the channel for. The actual time spent on each
 * channel (in symbols) is given by the forumla:
 *      aBaseSuperframeDuration * ((1ul << scanDuration) - 1)
 *
 * If for whatever reason, the device is unable to scan any channel, then it should
 * set the corresponding bits in the unscannedChannels bitmask found in the
 * MCPS-SCAN.confirm.
 *
 * Energy scans are performed by periodically sampling the channel energy level and
 * returning a list of the maximum energy level sampled on each channel.
 *
 * Active and passive scans are used to detect IEEE 802.15.4 networks operating on
 * the local chnanels by searching for beacon frames. The scan is performed by
 * listening on each channel for beacons, and returning them to the NHLE using the
 * PAN descriptor list in the MLME-SCAN.confirm (if macAutoRequest is true), or via
 * the MLME-BEACON-NOTIFY.indication service (if macAutoReqyest is FALSE, or if the
 * beacon has a non-zero payload length). The only difference between an active and
 * passive scan is that the active scan will first transmit a beacon request command
 * before listening for beacon frames. Active scans should be used to lcoate non-
 * beaconing networks, and passive scans for beacon-enabled networks.
 *
 * Orphan scans are used to implement the orphan rejoin procedure. An orphaned
 * device will attempt to locate its parent by iterating through the scan channel mask
 * and issuing orphan notification commands on each channel. If the devices's parent
 * detects this command is should respond with a coordinator realignment command and
 * the orphanded device should consider itself associated to the PAN again.
 */

/* Maximum valid scan duration */
#define WPAN_SCAN_DURATION_MAX                  14U

/* Filter fields of the Enhanced beacon request command. */
#define WPAN_SCAN_FILTER_PERMIT_JOIN            0x01U
#define WPAN_SCAN_FILTER_LINK_QUALITY           0x02U
#define WPAN_SCAN_FILTER_PERCENT                0x04U
#define WPAN_SCAN_FILTER_PAYLOAD                0x80U

/* MLME-SCAN.request (legacy 802.15.4-2003) */
struct wpan_scan_req {
    struct mcp_scanreq req;
    struct mcp_security security;
    struct mcp_payload_hdr pie;
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

/* MLME-SCAN.confirm */
struct wpan_scan_conf {
    struct mcp_scanconf conf;
    /* zigbee doesn't use pan descriptor */
    struct mcp_energylist_hdr energy;
    uint8_t buf[WPAN_CHANNELS_PER_MASK];
};

void wpan_scan_req_init(struct wpan_scan_req *req);
void wpan_scan_conf_init(struct wpan_scan_conf *conf, uint8_t seqno);
bool wpan_scan_req(struct WpanPublicT *publicPtr, struct wpan_scan_req *req);

/*---------------------------------------------------------------
 * MLME-BEACON
 *---------------------------------------------------------------
 */
struct wpan_pan_desc {
    struct mcp_pandesc pandesc;
    struct mcp_linkquality lqi;
    struct mcp_security security; /* ! make security optional */
};

/* MLME-BEACON-REQUST.indication */
struct wpan_beacon_request_ind {
    struct mcp_bcnreqind ind;
    /* use IE TLVs? For zigbee, this is easier for now. */
    struct mcp_payload_hdr ie_payload;
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

/* MLME-BEACON.request */
/* struct mcp_beaconreq */

/* MLME-BEACON.confirm */
/* struct mcp_beaconconf */

/*
 * The MLME-BEACON-NOTIFY.indication service is used to report information about
 * received beacons. This is implemented by a callback function, and may be called
 * during active scans, or on receipt of any beacon with a non-zero payload length.
 *
 * If macAutoRequest is set to FALSE, then the driver will NOT automatically
 * respond to beacons that advertise pending frames for the device. And the device
 * will issue the MLME-BEACON-NOTIFY.indication so the NHLE can manually query
 * its parent for pending packets.
 */

/* MLME-BEACON-NOTIFY.indication */
struct wpan_beacon_notify_ind {
    struct mcp_beaconind ind;
    struct wpan_pan_desc pandesc;
    /* pending addresses (struct mcp_pendaddr) are not used by zigbee */
    struct mcp_payload_hdr payload;
    uint8_t buf[WPAN_CONST_MAX_PHY_PACKET_SIZE];
};

void wpan_pan_desc_init(struct wpan_pan_desc *pandesc);
void wpan_pan_desc_set_sframe(struct mcp_pandesc *pandesc, uint16_t sframe);
uint16_t wpan_pan_desc_get_sframe(struct mcp_pandesc *pandesc);

void wpan_beacon_request_ind_init(struct wpan_beacon_request_ind *ind);
void wpan_beacon_req_init(struct mcp_beaconreq *req);
void wpan_beacon_conf_init(struct mcp_beaconconf *conf, uint8_t seqno);
void wpan_beacon_notify_ind_init(struct wpan_beacon_notify_ind *ind);
bool wpan_beacon_req(struct WpanPublicT *publicPtr, struct mcp_beaconreq *req, struct mcp_beaconconf *conf);

/*----------------------------------------------------------------------------
 * TX Power Table Configuration
 *----------------------------------------------------------------------------
 */
#define TX_POWER_CTRL_MAX                   PHY_TX_POWER_MAX
#define TX_POWER_CTRL_MIN                   PHY_TX_POWER_MIN
#define TX_POWER_CTRL_DELTA_MAX             6
#define TX_POWER_CTRL_DELTA_MIN             (-6)

void wpan_getpowertblreq_init(struct mcp_getpowertblreq *req);
void wpan_getpowertblconf_init(struct mcp_getpowertblconf *conf, uint8_t seqno);

/* Note: to clear the Power Information Table, do a MLME-SET-POWER-INFORMATION-TABLE.request
 * with dst_addr16 = IEEE802154_ADDR_SHORT_NONE, and dst_addr64 = 0. */
void wpan_setpowertblreq_init(struct mcp_setpowertblreq *req);
void wpan_setpowertblconf_init(struct mcp_setpowertblconf *conf, uint8_t seqno);

/*---------------------------------------------------------------
 * Duty Cycle Mode Indication
 *---------------------------------------------------------------
 */
void wpan_dutycycle_ind_init(struct mcp_dutycycleind *ind);

/*--------------------------------------------------------------------------
 * MAC Instance
 *--------------------------------------------------------------------------
 */
#define WPAN_DEVICE_NAME_MAXSZ                  16
#define WPAN_DEVICE_DESC_MAXSZ                  64

struct WpanPublicT {
    /* Link list linking (only for NHLE to use) */
    struct WpanPublicT *next;
    struct WpanPublicT *prev;

    /* Interface Description */
    char ifcName[WPAN_DEVICE_NAME_MAXSZ]; /* e.g. "wpan0", etc... */
    char ifcDesc[WPAN_DEVICE_DESC_MAXSZ]; /* e.g. "ChipCon CC2420 2.4GHz Transceiver" */
    bool isIpBridge; /* Is this an IP Bridge (i.e. BDL) device? */
    bool isZigbeeDirect; /* Is this a ZDD or ZVD device? */

    /*---------------------------------------------------------------
     * Serial-IO Device Management API
     *---------------------------------------------------------------
     */
    const struct WpanSioVersionT *sio_version_info;
    void (*sio_close)(struct WpanPublicT *publicPtr, bool doReset);
    /* Wait-Event and Kick MAC functions */
    int (*sio_get_fd)(struct WpanPublicT *publicPtr);
    /* process returns false if the communication link has been broken (e.g. dongle unplugged).
     * The application should call dev->close() on the device and detach it from any NHLE. */
    bool WPAN_WARN_UNUSED (*sio_process)(struct WpanPublicT *publicPtr);

    /*---------------------------------------------------------------
     * MAC Request API
     *---------------------------------------------------------------
     */
    void (*mcpsPurgeReq)(struct WpanPublicT *publicPtr, struct mcp_purgereq *req, struct mcp_purgeconf *conf);
    void (*mcpsDataReq)(struct WpanPublicT *publicPtr, struct mcp_datareq *req, uint16_t len);

    /*
     * The MLME-RESET service is used to return the device to its initial state
     * and is implemented as a blocking function call. When called, this function
     * should perform a hardware reset, clear all pending frames, and cease the
     * the transmission of beacons.
     *
     * When handling an MLME-RESET, the following attributes require special
     * handling:
     *  - macBsn and macDsn should be set to randomized values.
     *  - macExtendedAddress (if implemented) should not be modified, regardless of
     *          the value of setDefaultPib. The device should only change its
     *          extended address when explicitly instructed to do so by the NHLE.
     */
    void (*mlmeResetReq)(struct WpanPublicT *publicPtr, struct mcp_resetreq *req, struct mcp_resetconf *conf);

    /*
     * The MLME-SET service is provided to write status and configuration
     * data to an IEEE 802.15.4 device.
     *
     * All attributes are written in the host byte order. If the attribute
     * length does not match the size exactly then an error status will be
     * returned. In the event of an error, no change will be made to the
     * device configuration. The index parameter is ignored for scalar
     * attributes.
     *
     * A default implementation of this function is also provided, it does nothing
     * more than write the attribute into a cache of the PIB table located in the
     * device handle structure.
     *
     * MLME-SET is mandatory for all IEEE 802.15.4 devices.
     */
    void (*mlmeSetReq)(struct WpanPublicT *publicPtr, struct mcp_setreq *req, struct wpan_set_conf *conf);

    /*
     * The MLME-GET service is provided to read status and configuration
     * data from an IEEE 802.15.4 device, and has been modified slightly
     * to avoid requiring heap memory when returning an attribute to the
     * caller.
     *
     * The MLME-GET.request primitive specifies the attribute and an optional
     * index if the attribute is a table. The index is ignored for all scalar
     * attributes. The MLME-GET.request also specifies a buffer (and buffer
     * length) to write the attribute value into.
     *
     * All attributes are written in the host byte order starting at the
     * beginning of the buffer. If the attribute is longer than the buffer
     * then an error status will be returned. Refer to the definition of
     * struct WpanMacPibT for the size of each attribute.
     *
     * In addition, a helper function is provided to allow the NHLE to perform
     * a scalar MLME-GET without having to put the WpanGetReqT and WpanGetConfT
     * structures on the stack.
     *
     * A default implementation of this function is also provided, it does nothing
     * more than read the attribute frame a cache of the PIB table located in the
     * device handle structure.
     *
     * MLME-GET is mandatory for all IEEE 802.15.4 devices.
     */
    void (*mlmeGetReq)(struct WpanPublicT *publicPtr, struct wpan_get_req *req, struct mcp_getconf *conf);

    void (*mlmePollReq)(struct WpanPublicT *publicPtr, struct wpan_poll_req *req);
#if 0 /* for beacon-enabled networks only */
    void (*mlmeSyncReq)(struct WpanPublicT *, WpanSyncReqT *);
#endif
    void (*mlmeStartReq)(struct WpanPublicT *publicPtr, struct mcp_startreq *req, struct mcp_startconf *conf);
    /* MLME-RX-ENABLE.request
     *      req->deferPermit is ignored.
     *      req->rxOntime is ignored
     *      req->rxOnDuration is treated as a boolean value.
     *           0 = disable receiver, and go to sleep if supported
     *          >0 = wake up and/or enable receiver */
    void (*mlmeRxEnableReq)(struct WpanPublicT *publicPtr, struct mcp_rxenreq *req, struct mcp_rxenconf *conf);
    void (*mlmeAssociateReq)(struct WpanPublicT *publicPtr, struct wpan_assoc_req *req);
    void (*mlmeAssociateResp)(struct WpanPublicT *publicPtr, struct wpan_assoc_rsp *req);
#if 0 /* disassociate is not used and is now deprecated */
    void (*mlmeDisassociateReq)(struct WpanPublicT *publicPtr, WpanDisassociateReqT *req);
#endif
    void (*mlmeOrphanResp)(struct WpanPublicT *publicPtr, struct wpan_orphan_rsp *rsp);
    void (*mlmeScanReq)(struct WpanPublicT *publicPtr, struct wpan_scan_req *req);
    void (*mlmeBeaconReq)(struct WpanPublicT *publicPtr, struct mcp_beaconreq *req, struct mcp_beaconconf *conf);
    void (*mlmeGetPowerTblReq)(struct WpanPublicT *publicPtr, struct mcp_getpowertblreq *req, struct mcp_getpowertblconf *conf);
    void (*mlmeSetPowerTblReq)(struct WpanPublicT *publicPtr, struct mcp_setpowertblreq *req, struct mcp_setpowertblconf *conf);

    /*---------------------------------------------------------------
     * NHLE Callbacks: minimal API required
     *---------------------------------------------------------------
     */
    void (*mcpsDataConf)(void *nhleInstance, struct WpanPublicT *publicPtr, struct mcp_dataconf *conf);
    void (*mcpsDataInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct mcp_dataind *ind, uint16_t len);
    void (*mlmeAssociateConf)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_assoc_conf *conf);
    void (*mlmeAssociateInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_assoc_ind *ind);
#if 0 /* disassociate is not used and is now deprecated */
    void (*mlmeDisassociateConf)(void *nhleInstance, struct WpanPublicT *publicPtr, WpanDisassociateConfT *conf);
    void (*mlmeDisassociateInd)(void *nhleInstance, struct WpanPublicT *publicPtr, WpanDisassociateIndT *ind);
#endif
    void (*mlmeBeaconNotifyInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_beacon_notify_ind *ind);

    /* The MLME-ORPHAN service is used to respond to devices performing orphan
     * scans. The MLME-ORPAH.indication is implemented as a callback function
     * set by the NHLE and is called on recieipt of any orphan notification
     * commands.
     *
     * If the device is a parent of the orphaned child, then it should respond
     * to the MLME-ORPAN.indication by calling the MLME-ORPAN.response function
     * to reassociate the child to this PAN. If the orphaned device is not a
     * a child, then no action should be taken by the NHLE.
     *
     * This service is optional, but recommended for full function devices.
     */
    void (*mlmeOrphanInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_orphan_ind *ind);

    void (*mlmeCommStatusInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_comm_status_ind *ind);
    void (*mlmeScanConf)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_scan_conf *conf);
#if 0 /* for beacon-enabled networks only */
    void (*mlmeSyncLossInd)(void *nhleInstance, struct WpanPublicT *publicPtr, WpanSyncLossIndT *);
#endif
    void (*mlmePollConf)(void *nhleInstance, struct WpanPublicT *publicPtr, struct mcp_pollconf *conf);
    void (*mlmePollInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct mcp_pollind *ind);

    /*---------------------------------------------------------------
     * NHLE Callbacks: required for R22 GB-868 (Sub-GHz, Enhanced Beacons, Duty Cycle)
     *---------------------------------------------------------------
     */
    void (*mlmeBeaconRequestInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_beacon_request_ind *ind);
    void (*mlmeEnhBeaconNotifyInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct wpan_beacon_notify_ind *ind);
    /* LBT/DC: MLME-DUTYCYCLE-MODE.indication */
    void (*mlmeDutyCycleModeInd)(void *nhleInstance, struct WpanPublicT *publicPtr, struct mcp_dutycycleind *ind);

    /*---------------------------------------------------------------
     * Private Data
     *---------------------------------------------------------------
     */
    void *nhleInstance; /* NHLE's private data */
    void *priv; /* MAC's private data */
};

/*---------------------------------------------------------------
 * Serial-IO Device Management Wrappers
 *---------------------------------------------------------------
 */
bool WpanSerialProcess(struct WpanPublicT *publicPtr);
int WpanSerialFd(struct WpanPublicT *publicPtr);
void WpanSerialClose(struct WpanPublicT *publicPtr, bool doReset);

/*--------------------------------------------------------------------------
 * Enhanced Beacon Request and Enhanced Beacon IE Parser
 *--------------------------------------------------------------------------
 */
struct WpanIe {
    uint16_t len;
    const uint8_t *data;
};

struct WpanIeOpts {
    struct WpanIe ebf; /* MLME sub-IE: Enhanced beacon request filtering */
    struct WpanIe rejoin; /* ZigBee IE: Rejoin */
    struct WpanIe txPower; /* ZigBee IE: TX Power */
    struct WpanIe beaconPayload; /* ZigBee IE: beacon payload + superframe */
};

int WpanIeParseOpts(struct WpanIeOpts *opts, const void *ie, unsigned int len);

/*--------------------------------------------------------------------------
 * GB-868
 *--------------------------------------------------------------------------
 */
/* Helper to convert IE TX Power value to integer */
int8_t WpanEnhBeaconIeToTxPower(int8_t ieTxPower, int rssi, uint8_t currentPage);

/* Helper function to convert an RSSI measurement to an Energy (or LQI) value (linear and continuous) */
uint8_t WpanRssiToEnergy(int8_t rssi);

/* The ideal measured RSSI value we want to be operating at is
 * 20 dB above the -99 dBm floor. */
#ifndef WPAN_GB868_IDEAL_RX_RSSI
# define WPAN_GB868_IDEAL_RX_RSSI           (-79)
#endif

#ifndef WPAN_NA_IDEAL_RX_RSSI
# define WPAN_NA_IDEAL_RX_RSSI              (-71)
#endif

#ifndef WPAN_GB868_CCA_THRESHOLD
/* aLBTThresholdLevelLp */
/* ED must start being reported as non-zero at a value less than -89 dBm (-99 dBm + 10 dB).
 * We are assuming that we're supposed to indicate a busy channel 2 dB above this minimum ED
 * reporting level, so this is probably how they came up with -87 dBm for the CCA threshold.
 * HOWEVER, we should set this level to to -89 to make sure we are within the threshold. */
# define WPAN_GB868_CCA_THRESHOLD           (-89)
#endif

/*---------------------------------------------------------------
 * ieee802154_misc.
 *---------------------------------------------------------------
 */
bool WpanIsIpBridge(struct WpanPublicT *publicPtr);

uint8_t WpanTxPowerGetToleranceInt(uint8_t txpower);
const char * WpanStatusToStr(enum mcp_status status);
const char * WpanDutyCycleStatusToStr(uint8_t status);

#endif
