/**
 * @file zigbee.filter.h
 * @heading Zigbee Message Filters
 * @brief Zigbee header file.
 * @author Exegin Technologies
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZIGBEE_FILTER_H
# define ZIGBEE_FILTER_H

/*---------------------------------------------------------------
 * Asynchronous messaging filter API
 *---------------------------------------------------------------
 */
/* Asynchronous message filter mask. */
/* NWK Indications */
#define ZB_MSG_FILTER_JOIN_IND                  0x00000001U /* NLME-JOIN.indication (struct ZbNlmeJoinIndT) */
#define ZB_MSG_FILTER_LEAVE_IND                 0x00000002U /* NLME-LEAVE.indication (struct ZbNlmeLeaveIndT) */
#define ZB_MSG_FILTER_STATUS_IND                0x00000004U /* NLME-NETWORK-STATUS.indication (struct ZbNlmeNetworkStatusIndT) */
#define ZB_MSG_FILTER_STACK_EVENT               0x00000008U /* General stack event (struct ZbMsgStackEventT) */

/* Data and Command Indications */
#define ZB_MSG_FILTER_MCPS_DATA_IND             0x00000010U /* MCPS-DATA.indication (struct mcp_dataind) */
#define ZB_MSG_FILTER_NLDE_DATA_IND             0x00000020U /* NLDE-DATA.indication (struct ZbNldeDataIndT) */
#define ZB_MSG_FILTER_APSDE_DATA_IND            0x00000040U /* APSDE-DATA.indication (struct ZbApsdeDataIndT) */
#define ZB_MSG_FILTER_APS_COMMAND_IND           0x00000080U /* APS Commands (struct ZbMsgApsCommandT) */

/* R23 security ZDO Indications. */
#define ZB_MSG_FILTER_SEC_KEY_UPDATE_IND        0x00000100U /* struct ZbZdoSecKeyUpdateIndT */
#define ZB_MSG_FILTER_SEC_KEY_NEGO_IND          0x00000200U /* struct ZbApsmeKeyNegoIndT */
#define ZB_MSG_FILTER_SEC_CHALLENGE_IND         0x00000400U /* struct ZbApsmeSecChallengeIndT */
/* R23 Device interview event. */
#define ZB_MSG_FILTER_DEVICE_INTERVIEW_IND      0x00000800U /* struct ZbMsgDeviceInterviewIndT */
/* Note, max filter bit we can specify here is  0x00080000U */

/* Groups of messages that are filterable. */
#define ZB_MSG_FILTER_NLME \
    (ZB_MSG_FILTER_JOIN_IND | ZB_MSG_FILTER_LEAVE_IND | ZB_MSG_FILTER_STATUS_IND)

#define ZB_MSG_FILTER_APSME \
    (ZB_MSG_FILTER_APS_COMMAND_IND | ZB_MSG_FILTER_SEC_KEY_UPDATE_IND | ZB_MSG_FILTER_SEC_KEY_NEGO_IND | \
     ZB_MSG_FILTER_SEC_CHALLENGE_IND | ZB_MSG_FILTER_DEVICE_INTERVIEW_IND)

/* Message filter priorities (255 = highest, 0 = lowest) */
#define ZB_MSG_INTERNAL_PRIO                128U /* default stack priority */
#define ZB_MSG_DEFAULT_PRIO                 64U /* default application priority */
#define ZB_MSG_LOWEST_PRIO                  0U

struct ZbApsFilterT;

struct ZbMsgFilterT * ZbMsgFilterRegister(struct ZigBeeT *zb, uint32_t mask, uint8_t prio,
    enum zb_msg_filter_rc (*callback)(struct ZigBeeT *zb, uint32_t id, void *msg, void *cbarg), void *arg);

/**
 * Removes the given message filter from the stack.
 * @param zb Pointer to Zigbee stack instance
 * @param filter Pointer to filter struct returned by ZbMsgFilterRegister.
 */
void ZbMsgFilterRemove(struct ZigBeeT *zb, struct ZbMsgFilterT *filter);

/*---------------------------------------------------------
 * APS Command Indication Struct
 *---------------------------------------------------------
 */
struct ZbMsgApsCommandT {
    enum ZbApsCmdIdT id; /**< The APS Command Id. e.g. ZB_APS_CMD_TRANSPORT_KEY */
    union {
        struct ZbApsmeTransKeyIndT trans_key; /* ZB_APS_CMD_TRANSPORT_KEY */
        struct ZbApsmeUpdateDeviceIndT update_device; /* ZB_APS_CMD_UPDATE_DEVICE */
        struct ZbApsmeRemoveDeviceIndT remove_device; /* ZB_APS_CMD_REMOVE_DEVICE */
        struct ZbApsmeRequestKeyIndT request_key; /* ZB_APS_CMD_REQUEST_KEY */
        struct ZbApsmeSwitchKeyIndT switch_key; /* ZB_APS_CMD_SWITCH_KEY */
        struct ZbApsmeVerifyKeyIndT verify_key; /* ZB_APS_CMD_VERIFY_KEY */
        struct ZbApsmeConfirmKeyIndT confirm_key; /* ZB_APS_CMD_CONFIRM_KEY */
    } data;
};

/*---------------------------------------------------------
 * ZB_MSG_FILTER_STACK_EVENT
 *---------------------------------------------------------
 */
enum ZbMsgStackEventTypeT {
    ZB_MSG_STACK_EVENT_STARTUP_DONE,
    /**< A message that is sent after the stack completes a Form, Join or Rejoin process. */

    ZB_MSG_STACK_EVENT_ATTEMPT_REJOIN,
    /**< A message that is sent if the stack handles an NLME-LEAVE.indication message
     * for the local device that has left the network and the rejoin flag was set.
     * The application should attempt to rejoin the network using the ZbStartupRejoin()
     * API call at its convenience. */

    ZB_MSG_STACK_EVENT_MAC_DUTY_CYCLE_IND,
    /**< The stack will forward the MLME-DUTYCYCLE-MODE.indication it receives from the
     * MAC layer up to the application layer. The application can then decide how to
     * regulate the packets it generates.
     * ZbMsgStackEventT.val is the new Duty Cycle Mode (e.g. MCP_DUTYCYCLE_STATUS_SUSPENDED) */

    ZB_MSG_STACK_EVENT_FACTORY_RESET,
    /**< A message that is sent after a NWK Leave or the ZCL Basic ZCL_BASIC_RESET_FACTORY,
     * to indicate to the application it should perform a factory reset. The extent of the
     * factory reset is up to the application and may be ignored. */

    ZB_MSG_STACK_EVENT_RESET_ZCL_REPORTS,
    /**< A message that may be sent in conjunction with a ZB_MSG_STACK_EVENT_FACTORY_RESET
     * message to indicate that ZCL Report Configurations should also be reset to defaults.
     * ZCL Clusters that are managed through the stack (i.e. attributes attached to a
     * cluster created by ZbZclClusterAlloc) will do this automatically. Note that the
     * application can configure the default reporting parameters for an attribute using
     * the ZbZclAttrReportConfigDefault API. */

    ZB_MSG_STACK_EVENT_DLK_SEC_KEY_NEGO_IND,
    /**< A message that is sent on receiving the APSME-KEY-NEGOTIATION.indication, to notify
     * the higher layer regarding different stages of DLK along with the status.
     * ZbMsgStackEventT.val is a status value. */

    ZB_MSG_STACK_EVENT_DLK_VERIFY_KEY_IND,
    /**< A message that is sent on receiving the APSME-VERIFY-KEY.indication as part
     * of the ongoing DLK negotiation session. */

    ZB_MSG_STACK_EVENT_ZDD_STATUS_UPDATE,
    /**< Inform ZDD application when certain status changes within the stack. These include:
     * Joined Status (i.e. Leave), NWK Permit Join (on/off), Short Address, Channel.
     * Will only get this notification if stack built with CONFIG_ZB_ZDD_SUPPORT enabled. */

    ZB_MSG_STACK_EVENT_ZVD_START_SEC_SESSION,
    /**< Inform ZVD application to start secure session during Tunnel join. Will only
     * get this notification if stack built with CONFIG_ZB_ZDD_SUPPORT enabled. */

    ZB_MSG_STACK_EVENT_ELEVATE_SEC_SESSION,
    /**< Inform ZDD/ZVD application to elevate the current secure session to a more secure
     * authorization state, so that ZVD can continue joining the legacy network. Will
     * only get this notification if stack built with CONFIG_ZB_ZDD_SUPPORT enabled. */

    ZB_MSG_STACK_EVENT_DISCONNECT_ZVD,
    /**< Inform ZDD application to disconnect from ZVD, provided it is connected to one
     * at the moment. This is done on network key rotation as ZVD's security credentials
     * are no more valid, forcing ZVD to setup limited authorization session and perform
     * trust center. Will only get this notification if stack built with CONFIG_ZB_ZDD_SUPPORT
     * enabled. */

    ZB_MSG_STACK_EVENT_STARTUP_JOIN_AUTH_TOKEN,
    /**< A message that is sent after the stack completes updating the symmetric Passphrase
     * after initially joining a network and negotiating the trust center link key using DLK. */

    ZB_MSG_STACK_EVENT_PARENT_LINK_FAIL
    /** A message that is sent if the device is an end-device and it has received an
     * NLME-NETWORK-STATUS.indication with the PARENT_LINK_FAILURE status set.
     * Outside of testing 18.4.* TP/R23/ZDO/APC-*, the application should attempt to
     * rejoin the network using the ZbStartupRejoin() API call at its convenience. */
};

struct ZbMsgStackEventT {
    enum ZbMsgStackEventTypeT type;
    /**< Enumeration of different stack event message type, sent to higher layer. */
    unsigned int val;
    /**< Depends on message type. E.g. enum ZbStatusCodeT status. */
};

struct ZbMsgDeviceInterviewIndT {
    uint64_t joiner_eui64;
    /**< EUI64 of the joiner device. */
};

#endif
