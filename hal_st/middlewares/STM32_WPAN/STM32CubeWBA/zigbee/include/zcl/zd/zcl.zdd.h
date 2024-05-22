/**
 * @file zcl.zdd.h
 * @heading Zigbee Direct Configuration Cluster
 * @brief ZCL Zigbee Direct Configuration Cluster header
 * Zigbee Direct Specification 20-27688-030
 * @copyright Copyright [2021 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_ZDD_H
#define ZCL_ZDD_H

#include "zcl/zcl.h"

/* EXEGIN - PICS? */

/** Zigbee Direct Configuration Server Attribute IDs */
enum {
    ZCL_ZDD_ATTR_IFC_STATE = 0x0000, /**< Interface State */
    ZCL_ZDD_ATTR_ANON_JOIN_TIMEOUT = 0x0001, /**< Anonymous Join Timeout */
};

/* The recommended default value for the AnonymousJoinTimeout (seconds). */
#define ZCL_ZDD_ANON_JOIN_TIMEOUT_DEFAULT   3600U /* 0x000E10U */

/* AnonymousJoinTimeout (seconds) min. */
#define ZCL_ZDD_ANON_JOIN_TIMEOUT_MIN       0x000000U

/* AnonymousJoinTimeout (seconds) max. */
#define ZCL_ZDD_ANON_JOIN_TIMEOUT_MAX       0x100000U /* 1048575 seconds. */

/* ZCL_ZDD_ATTR_IFC_STATE / ZCL_ZDD_CLI_CMD_CONFIG_ZDD_IFC */
#define ZCL_ZDD_IFC_STATE_DISABLED          0x00U
#define ZCL_ZDD_IFC_STATE_ENABLED           0x01U

/* Zigbee Direct Configuration Client Generated Commands */
enum {
    ZCL_ZDD_CLI_CMD_CONFIG_ZDD_IFC = 0x00U, /* Configure Zigbee Direct Interface */
    ZCL_ZDD_CLI_CMD_CONFIG_ZDD_ANON_JOIN_TIMEOUT = 0x01U /* Configure Zigbee Direct Anonymous Join Timeout. */
};

/* Zigbee Direct Configuration Server Generated Commands */
enum {
    ZCL_ZDD_SVR_CONFIG_ZDD_IFC = 0x00U /* Configure Zigbee Direct Interface response. */
};

/** Configure Zigbee Direct Interface request structure */
struct ZbZclZddClientConfigIfcReqT {
    uint8_t ifc_state; /**< E.g. ZCL_ZDD_IFC_STATE_ENABLED */
};

/** Configure Zigbee Direct Anonymous Join Timeout request structure */
struct ZbZclZddClientConfigAnonJoinTimeoutReqT {
    uint32_t anon_join_timeout;
};

/** Configure Zigbee Direct Server Interface response structure */
struct ZbZclZddServerConfigIfcRspT {
    enum ZclStatusCodeT status; /**< Status */
    uint8_t ifc_state; /**< E.g. ZCL_ZDD_IFC_STATE_ENABLED */
};

/**
 * Create a new instance of the Zigbee Direct Configuration Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclZddServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Create a new instance of the Zigbee Direct Configuration Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclZddClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Check if the Anonymous Join Timeout Timer is active or not.
 * @param cluster Cluster instance of Zigbee Direct configuration cluster.
 * @return True if the Anonymous Join Timeout Timer is running, false otherwise.
 */
bool ZbZclZddServerIsAnonJoinTimerActive(struct ZbZclClusterT *cluster);

/**
 * Send Zigbee Direct Client Configure Interface request command.
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param req Zigbee Direct configure anonymous join timeout request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclZddClientConfigIfcReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclZddClientConfigIfcReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send Zigbee Direct Client Configure Anonymous Join Timeout request command.
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param req Zigbee Direct configure anonymous join timeout request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclZddClientConfigAnonJoinTimeoutReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclZddClientConfigAnonJoinTimeoutReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif
