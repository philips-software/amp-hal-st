/**
 * @file zcl.events.h
 * @heading Events
 * @brief ZCL Events cluster header
 * ZCL 8 section 10.11
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_EVENTS_H
# define ZCL_EVENTS_H

#include "zcl/zcl.h"

/* Events Server Cluster Commands Generated */
enum ZbZclEventsServerCommandT {
    ZCL_EVENTS_SVR_CMD_PUBLISH_EVENT = 0x00,
    ZCL_EVENTS_SVR_CMD_PUBLISH_EVENT_LOG = 0x01,
    ZCL_EVENTS_SVR_CMD_CLEAR_EVENT_LOG_RESPONSE = 0x02,
};

/* Events Client Cluster Commands Generated */
enum ZbZclEventsClientCommandT {
    ZCL_EVENTS_CLI_CMD_GET_EVENT_LOG = 0x00,
    ZCL_EVENTS_CLI_CMD_CLEAR_EVENT_LOG_REQUEST = 0x01,
};

#define ZCL_EVENTS_SVR_EVENT_DATA_MAX_LEN           (254U)
#define ZCL_EVENTS_SVR_PUBLISH_EVENT_MAX_LEN        (8U + ZCL_EVENTS_SVR_EVENT_DATA_MAX_LEN + 1U)
#define ZCL_EVENTS_SVR_EVENT_LIST_MAX_LEN           (15U)

/* Table D-186– Log ID Enumeration */
#define ZCL_EVENTS_LOG_ID_MASK                      0x0fU
#define ZCL_EVENTS_LOG_ID_ALL_LOGS                  0x00U
#define ZCL_EVENTS_LOG_ID_TAMPER_LOG                0x01U
#define ZCL_EVENTS_LOG_ID_FAULT_LOG                 0x02U
#define ZCL_EVENTS_LOG_ID_GENERAL_EVENT             0x03U
#define ZCL_EVENTS_LOG_ID_SECURITY_EVENT            0x04U
#define ZCL_EVENTS_LOG_ID_NETWORK_EVENT             0x05U

#define ZCL_EVENTS_EVENT_ID_ALL                     0x0000U

#define ZCL_EVENTS_PUBLISH_ACTION_REPORT_HAN        0x01U
#define ZCL_EVENTS_PUBLISH_ACTION_REPORT_WAN        0x02U

/** Publish Event command structure */
struct ZbZclEventsServerPublishEventT {
    uint8_t log_id; /**< Log ID */
    uint16_t event_id; /**< Event ID */
    uint32_t event_time; /**< Event Time - UTC time */
    uint8_t event_control; /**< Event Control */
    uint8_t *event_data; /**< Event Data - max length = ZCL_EVENTS_SVR_EVENT_DATA_MAX_LEN */
    uint8_t event_data_len; /**< Event Data length */
};

/** Publish Event Log Payload structure */
struct ZbZclEventsServerEventLogPayloadT {
    uint8_t num_events; /**< Number of Events */
    uint8_t control; /**< Log Payload Control */
    struct ZbZclEventsServerPublishEventT event_list[ZCL_EVENTS_SVR_EVENT_LIST_MAX_LEN]; /**< Publish Event Response Payload */
};

/** Publish Event Log command structure */
struct ZbZclEventsServerPublishEventLogT {
    uint16_t total_matching_events; /**< Total Number of Matching Events */
    uint8_t command_index; /**< Command Index */
    uint8_t total_commands; /**< Total Commands */
    struct ZbZclEventsServerEventLogPayloadT log_payload; /**< Log Payload */
};

/* Table D-191– ClearedEventsLogs Bitmap */
#define ZCL_EVENTS_CLEARED_ALL_LOGS         0x01U
#define ZCL_EVENTS_CLEARED_TAMPER_LOG       0x02U
#define ZCL_EVENTS_CLEARED_FAULT_LOG        0x04U
#define ZCL_EVENTS_CLEARED_GENERAL_LOG      0x08U
#define ZCL_EVENTS_CLEARED_SECURITY_LOG     0x10U
#define ZCL_EVENTS_CLEARED_NETWORK_LOG      0x20U

/** Clear Event Log Response command structure */
struct ZbZclEventsServerClearEventLogResponseT {
    uint8_t cleared_events_logs; /**< ClearedEventsLogs */
};

/* Table D-187– Event Control Bitmap */
#define ZCL_EVENTS_GET_CONTROL_RETRIEVE_FULL        0x10U

/** Get Event Log command structure */
struct ZbZclEventsClientGetEventLogT {
    uint8_t event_ctrl_log_id; /**< Event Control/Log ID. Event Control is high nibble. Log ID is low nibble. */
    uint16_t event_id; /**< Event ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint32_t end_time; /**< End Time - UTC Time */
    uint8_t number_of_events; /**< Number of Events */
    uint16_t event_offset; /**< Event Offset */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD. */
};

/** Clear Event Log command structure */
struct ZbZclEventsClientClearEventLogT {
    uint8_t log_id; /**< Log ID */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD. */
};

/* Events Server */
/** Events Server callbacks configuration */
struct ZbZclEventsServerCallbacksT {
    enum ZclStatusCodeT (*get_event_log)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclEventsClientGetEventLogT *get_log, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Event Log command.
     * Application should call ZbZclEventsServerPublishEventLogRsp for any matching
     * event log, or return ZCL_STATUS_NOT_FOUND otherwise to return a Default Response. */

    enum ZclStatusCodeT (*clear_event_log)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclEventsClientClearEventLogT *clear_log, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Clear Event Log Request command.
     * Application calls ZbZclEventsServerClearEventLogRsp to send the response.
     * Application should always return ZCL_STATUS_SUCCESS. */
};

/**
 * Create a new instance of the Events Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclEventsServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclEventsServerCallbacksT *callbacks, void *arg);

/**
 * Send a Publish Event command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param event Publish Event command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclEventsServerPublishEventUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclEventsServerPublishEventT *event,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclEventsServerPublishEventLogRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclEventsServerPublishEventLogT *info);

/**
 * Send a Clear Event Log Response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param rsp Clear Event Log Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclEventsServerClearEventLogRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclEventsServerClearEventLogResponseT *rsp);

/* Events Client */
/** Events Client callbacks configuration */
struct ZbZclEventsClientCallbacksT {
    enum ZclStatusCodeT (*publish_event)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclEventsServerPublishEventT *event, const struct ZbApsAddrT *src);
    /**< Callback to application, invoked on receipt of Publish Event command. */

    enum ZclStatusCodeT (*publish_event_log)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclEventsServerPublishEventLogT *event_log, const struct ZbApsAddrT *src);
    /**< Callback to application, invoked on receipt of Publish Event Log command. */
};

/**
 * Create a new instance of the Events Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclEventsClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclEventsClientCallbacksT *callbacks, void *arg);

/**
 * Send a Get Event Log command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param cmd_req Get Event Log command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclEventsClientCommandGetEventLogReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclEventsClientGetEventLogT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Clear Event Log command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param cmd_req Clear Event Log command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclEventsClientCommandClearEventLogReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclEventsClientClearEventLogT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*-----------------------------------------------------------------------------
 * Mirror Server
 *-----------------------------------------------------------------------------
 */

/**
 * Allocates a Events Server Mirror cluster. The caller (application) must
 *  attach any attributes to the cluster you want to mirror.
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param events_client Pointer to Events Client cluster. This is used by the
 *  Metering Mirror to send mirrored commands to the BOMD.
 * @param meter_mirror Pointer to Metering Mirror cluster. This is used to queue
 *  any commands that need to be forwarded to the BOMD when it wakes up.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclEventsMirrorAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclClusterT *events_client, struct ZbZclClusterT *meter_mirror);

#endif
