/**
 * @file zigbee.dlk.h
 * @heading DLK Utilities
 * @brief ZigBee DLK implementation.
 * @author Exegin Technologies Limited
 * @copyright Copyright [2019 - 2022] Exegin Technologies Limited. All rights reserved.
 *
 * This file defines all the Public API's used as part of ZigBee dynamic link key (DLK) implementation.
 */

#ifndef ZIGBEE_DLK_H
#define ZIGBEE_DLK_H

#include <stdint.h>
#include <stdbool.h>

/** Lists of all the available dynamic link key negotiation methods */
/** Key Request (ZigBee 3.0) */
#define ZB_DLK_STATIC_KEY_REQUEST                   0x01U
/** ECDHE using Curve25519 and Hash AES-MMO-128 */
#define ZB_DLK_ECDHE_C25519_AES_MMO_128             0x02U
/** ECDHE using Curve25519 and Hash SHA-256 */
#define ZB_DLK_ECDHE_C25519_SHA_256                 0x04U

/** Lists of all the supported PSK (pre-shared) secrets bitmask. */
/** 128-bit pre-configured link-key derived from install code. */
#define ZB_DLK_SUPP_PSK_SYMMETRIC_AUTH_TOKEN        0x01U
/** 128-bit pre-configured link-key derived from install code. */
#define ZB_DLK_SUPP_PSK_IC_KEY                      0x02U
/** Variable-length pass code for PAKE protocols. */
#define ZB_DLK_SUPP_PSK_PASSCODE_PAKE               0x04U
/** Basic Access Key. */
#define ZB_DLK_SUPP_PSK_BASIC_ACCESS_KEY            0x08U
/** Administrative Access Key. */
#define ZB_DLK_SUPP_PSK_ADMIN_ACCESS_KEY            0x10U

/** CURVE25519 public point length */
#define ZB_DLK_CURVE25519_PUB_POINT_LEN             32U

/** Enumeration for selected key negotiation methods. */
enum ZbSelKeyNegoMethodT {
    ZB_SEL_DLK_METHOD_RESERVED = 0x00U,
    /**< Reserved */
    ZB_SEL_DLK_METHOD_C25519_AES_MMO_128 = 0x01U,
    /**< ECDHE using Curve25519 with Hash AES-MMO-128 */
    ZB_SEL_DLK_METHOD_C25519_SHA_256 = 0x02U
        /**< ECDHE using Curve25519 with Hash SHA-256 */
        /* 3 - 255 reserved */
};

/** Enumeration for selected PSK (Pre-shared) secret. */
enum ZbSelPreSharedSecretT {
    ZB_SEL_PSK_SYMMETRIC_AUTH_TOKEN = 0x00U,
    /**< Symmetric authentication token negotiated using DLK */
    ZB_SEL_PSK_IC_KEY = 0x01U,
    /**< Pre-configured link-key derived from installation code */
    ZB_SEL_PSK_PASSCODE_PAKE = 0x02U,
    /**< Variable-length pass code (for PAKE protocols) */
    ZB_SEL_PSK_BASIC_ACCESS_KEY = 0x03U,
    /**< Basic Access Key */
    ZB_SEL_PSK_ADMIN_ACCESS_KEY = 0x04U,
    /**< Administrative Access Key */
    /* Discontinuity 05 - 254 reserved */
    ZB_SEL_PSK_ANON_WELL_KNOWN_SECRET = 0xffU
        /**< Well-known secret - 'ZigBeeAlliance18' */
};

/**
 * Perform dynamic link key negotiation (DLK) with a partner R23 device after the initiator
 * has joined the network. The initiator and the responder devices can be TC or non-TC.
 * In effect this API can be used to negotiate both trust center and application link key.
 * @param zb Zigbee instance
 * @param partnerAddr EUI64 of the responder for DLK.
 * @param callback Callback function pointer.
 * @param cbarg Callback argument.
 * @return ZB Status Code. If not ZB_STATUS_SUCCESS, then the callback will not be called.
 */
enum ZbStatusCodeT ZbDlkKeyNegoWithDevice(struct ZigBeeT *zb, uint64_t partnerAddr,
    void (*callback)(enum ZbStatusCodeT status, void *arg), void *arg);

#endif
