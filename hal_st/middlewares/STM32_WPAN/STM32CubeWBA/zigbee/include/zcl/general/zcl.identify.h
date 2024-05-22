/**
 * @file zcl.identify.h
 * @heading Identify
 * @brief ZCL Identify cluster header
 * ZCL 7 section 3.5
 * ZCL 8 section 3.5
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_CORE_IDENTIFY_H
# define ZCL_CORE_IDENTIFY_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Identify cluster.
 *--------------------------------------------------------------------------
 */

/* @PICS.ZCL.Identify Cluster (0x0003)
 *
 * I.S | Server | True
 * I.C | Client | True
 *
 * Server Attributes
 * I.S.A0000 | IdentifyTime | True
 * I.S.Afffd | ClusterRevision |True
 *
 * Commands Received
 * I.S.C00.Rsp | Identify | True
 * I.S.C01.Rsp | Identify Query | True
 * I.S.C40.Rsp | Trigger effect | False
 *
 * Commands Generated
 * I.S.C00.Tx | Identify Query Response | True
 *
 * Client Attributes
 * I.C.Afffd | ClusterRevision | True
 *
 * Commands Received
 * I.C.C00.Rsp | Identify Query Response | True
 *
 * Commands Generated
 * I.C.C00.Tx | Identify | True
 * I.C.C01.Tx | Identify Query | True
 * I.C.C40.Tx | Trigger effect | True
 */

#include "zcl/zcl.h"

/** Identify Server Attribute IDs */
enum ZbZclIdentifySvrAttrT {
    ZCL_IDENTIFY_ATTR_TIME = 0x0000 /**< IdentifyTime */
};

/* Identify Command IDs  */
enum ZbZclIdentifyClientCommandT {
    ZCL_IDENTIFY_CLI_CMD_IDENTIFY = 0x00, /**< Identify */
    ZCL_IDENTIFY_CLI_CMD_QUERY = 0x01, /**< Identify Query */
    ZCL_IDENTIFY_CLI_CMD_TRIGGER_EFFECT = 0x40 /**< Trigger effect */
};

enum ZbZclIdentifyServerCommandT {
    ZCL_IDENTIFY_SVR_CMD_QUERY_RESP = 0x00
};

struct ZbZclIdentifyClientTriggerEffectT {
    uint8_t effect_identifier; /**< Effect indentifier */
    uint8_t effect_variant; /**< Effect variant */
};

/*---------------------------------------------------------------
 * Identify Server Cluster
 *---------------------------------------------------------------
 */

enum ZbZclIdentifyServerStateT {
    ZCL_IDENTIFY_START = 0,
    ZCL_IDENTIFY_STOP
};

struct zcl_identify_server_callbacks_t {
    void (*identify)(struct ZbZclClusterT *cluster, enum ZbZclIdentifyServerStateT state, void *arg);
    /**< Callback to receive start and stop events */

    enum ZclStatusCodeT (*trigger_effect)(struct ZbZclClusterT *cluster,
        struct ZbZclIdentifyClientTriggerEffectT *info,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to receive Trigger Effect command */
};

/**
 * Create a new instance of the Identify Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIdentifyServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct zcl_identify_server_callbacks_t *callbacks, void *arg);

/**
 * Set the local Identify Server time
 *
 * If BDB_COMMISSION_MODE_FIND_BIND is enabled and seconds > 0, seconds
 * is adjusted to be >= ZB_BDBC_MinCommissioningTime
 * @param cluster Cluster instance with timer to set
 * @param seconds Seconds for updating the identify time counter
 * @return Void
 */
void ZbZclIdentifyServerSetTime(struct ZbZclClusterT *cluster, uint16_t seconds);

/**
 * Get the local Identify Server time
 * @param cluster Cluster instance with timer to get
 * @return uint16_t Time remaining in zigbee timer
 */
uint16_t ZbZclIdentifyServerGetTime(struct ZbZclClusterT *cluster);

/*---------------------------------------------------------------
 * Identify Client Cluster
 *---------------------------------------------------------------
 */

/**
 * Create a new instance of the Identify Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIdentifyClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send an Identify command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param identify_time Time which will be used to set the IdentifyTime attribute
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_identify_cli_identify_req(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    uint16_t identify_time, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send an Identify Query command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_identify_cli_query_req(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_identify_cli_trigger_effect_req(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclIdentifyClientTriggerEffectT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif
