/**
 * @file zcl.energy_mgmt.h
 * @heading Energy Management
 * @brief ZCL Energy Management cluster header
 * SE 1.4 Section D.12
 * @copyright Copyright [2022 - 2023] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_ENERGY_MGMT_H
# define ZCL_ENERGY_MGMT_H

#include "zcl/zcl.h"
#include "zcl/se/zcl.drlc.h"

/** Energy Management Server Attribute IDs */
enum ZbZclEnergyMgmtSvrAttrT {
    ZCL_ENERGY_MGMT_SVR_ATTR_LOAD_CONTROL_STATE = 0x0000, /**< LoadControlState */
    ZCL_ENERGY_MGMT_SVR_ATTR_CURR_EVENT_ID = 0x0001, /**< CurrentEventID */
    ZCL_ENERGY_MGMT_SVR_ATTR_CURR_EVENT_STATUS = 0x0002, /**< CurrentEventStatus */
    ZCL_ENERGY_MGMT_SVR_ATTR_CONFORMANCE_LEVEL = 0x0003, /**< ConformanceLevel */
    ZCL_ENERGY_MGMT_SVR_ATTR_MIN_OFF_TIME = 0x0004, /**< MinimumOffTime */
    ZCL_ENERGY_MGMT_SVR_ATTR_MIN_ON_TIME = 0x0005, /**< MinimumOnTime */
    ZCL_ENERGY_MGMT_SVR_ATTR_MIN_CYCLE_PERIOD = 0x0006 /**< MinimumCyclePeriod */
};

/* LoadControlState Bits */
#define ZCL_ENERGY_MGMT_LC_STATE_RELAY_OPEN             0x01U /* Relay Open or Consumption Interupted */
#define ZCL_ENERGY_MGMT_LC_STATE_EVENT_IN_PROG          0x02U /* Event In Progress */
#define ZCL_ENERGY_MGMT_LC_STATE_POWER_STABALIZING      0x04U /* Power Stabilizing */
#define ZCL_ENERGY_MGMT_LC_STATE_OTHER_LOAD_REDUC       0x08U /* Other Load Reduction */
#define ZCL_ENERGY_MGMT_LC_STATE_CURRENT_FLOW           0x10U /* Current Flow or Consuming Commodity */
#define ZCL_ENERGY_MGMT_LC_STATE_LOAD_CALL              0x20U /* Load Call */
#define ZCL_ENERGY_MGMT_LC_STATE_RESERVED               0xC0U /* Reserved */

/* CurrentEventStatus Bits */
#define ZCL_ENERGY_MGMT_EVT_STATUS_RAND_START           0x01U /* Randomized Start Time */
#define ZCL_ENERGY_MGMT_EVT_STATUS_RAND_DURATION        0x02U /* Randomized Duration */
#define ZCL_ENERGY_MGMT_EVT_STATUS_EXT_BITS             0x04U /* Extended Bits Present */
#define ZCL_ENERGY_MGMT_EVT_STATUS_EVT_ACTIVE           0x08U /* Event Active */
#define ZCL_ENERGY_MGMT_EVT_STATUS_DEV_PARTICIP         0x10U /* Device participating in Event (not opted out) */
#define ZCL_ENERGY_MGMT_EVT_STATUS_REDUCING_LOAD        0x20U /* Reducing Load */
#define ZCL_ENERGY_MGMT_EVT_STATUS_ON_AT_END            0x40U /* On at end of Event */
#define ZCL_ENERGY_MGMT_EVT_STATUS_RESERVED             0x80U /* Reserved */

/* ZCL_ENERGY_MGMT_CLI_CMD_MANAGE_EVENT "Action(s) Required" Bits */
#define ZCL_ENERGY_MGMT_ACT_REQ_OPT_OUT                 0x01U /* Opt Out of Event */
#define ZCL_ENERGY_MGMT_ACT_REQ_OPT_IN                  0x02U /* Opt Into Event */
#define ZCL_ENERGY_MGMT_ACT_REQ_DISABLE_DC              0x04U /* Disable Duty Cycling */
#define ZCL_ENERGY_MGMT_ACT_REQ_ENABLE_DC               0x08U /* Enable Duty Cycling */
#define ZCL_ENERGY_MGMT_ACT_REQ_RESERVED                0xf0U /* Reserved */

/* Energy Management Client Cluster Generated Commands */
enum ZbZclEnergyMgmtClientCommandT {
    ZCL_ENERGY_MGMT_CLI_CMD_MANAGE_EVENT = 0x00U
};

/* Energy Management Server Cluster Generated Commands */
enum ZbZclEnergyMgmtServerCommandT {
    ZCL_ENERGY_MGMT_SVR_CMD_REPORT_EVT_STATUS = 0x00U
};

/** Manage Event (ZCL_ENERGY_MGMT_CLI_CMD_MANAGE_EVENT) command structure */
struct ZbZclEnergyMgmtManageEventT {
    uint32_t issuer_id; /**< Issuer Event ID */
    uint16_t device_class; /**< Device Class */
    uint8_t utility_enrol_group; /**< Utility Enrollment Group */
    uint8_t actions_required; /**< Action(s) Required */
};

/*---------------------------------------------------------------
 * Server API
 *---------------------------------------------------------------
 */

/** Energy Management Server callbacks configuration */
struct ZbZclEnergyMgmtServerCallbacksT {
    enum ZclStatusCodeT (*manage_event)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclEnergyMgmtManageEventT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Manage Event command */
};

/**
 * Create a new instance of the Energy Management Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclEnergyMgmtServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclEnergyMgmtServerCallbacksT *callbacks, void *arg);

/**
 * Create a new instance of the Energy Management Server cluster
 * @param cluster
 * @param dst
 * @param info
 * @param callback
 * @param arg
 * @return
 */
enum ZclStatusCodeT ZbZclEnergyMgmtServerReportEvtStatus(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDrlcStatusT *info, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client API
 *---------------------------------------------------------------
 */

/**
 * Create a new instance of the Energy Management Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclEnergyMgmtClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send a Manage Event command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Manage Event command request structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclEnergyMgmtClientManageEventReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclEnergyMgmtManageEventT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif
