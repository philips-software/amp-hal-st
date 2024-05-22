/**
 * @file zcl.calendar.h
 * @heading Calendar
 * @brief ZCL Calendar cluster header
 * ZCL 8 Section 10.9
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the SE Calendar (TOU) cluster.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_CALENDAR_H
# define ZCL_CALENDAR_H

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */

/* String length, not including 1-octet length byte.  */
#define ZCL_CAL_SVR_ATTR_AUXSWTCHLBL_LEN_MAX            22U

/* Length doesn't include length byte. */
#define ZCL_CAL_NAME_LEN_MAX                            12U

/* Calendar Server Generated Commands */
enum ZbZclCalServerCommandT {
    ZCL_CAL_SVR_PUBLISH_CALENDAR = 0x00,
    ZCL_CAL_SVR_PUBLISH_DAY_PROFILE = 0x01,
    ZCL_CAL_SVR_PUBLISH_WEEK_PROFILE = 0x02,
    ZCL_CAL_SVR_PUBLISH_SEASONS = 0x03,
    ZCL_CAL_SVR_PUBLISH_SPECIAL_DAYS = 0x04,
    ZCL_CAL_SVR_CANCEL_CALENDAR = 0x05
};

/* Calendar Client Generated Commands */
enum ZbZclCalClientCommandT {
    ZCL_CAL_CLI_GET_CALENDAR = 0x00,
    ZCL_CAL_CLI_GET_DAY_PROFILES = 0x01,
    ZCL_CAL_CLI_GET_WEEK_PROFILES = 0x02,
    ZCL_CAL_CLI_GET_SEASONS = 0x03,
    ZCL_CAL_CLI_GET_SPECIAL_DAYS = 0x04,
    ZCL_CAL_CLI_GET_CALENDAR_CANCELLATION = 0x05
};

/** Calendar Server Attribute IDs */
enum ZbZclCalSvrAttrT {
    ZCL_CAL_SVR_ATTR_AuxSwitch1Label = 0x0000, /**< AuxSwitch1Label (Optional) */
    ZCL_CAL_SVR_ATTR_AuxSwitch2Label = 0x0001, /**< AuxSwitch2Label (Optional) */
    ZCL_CAL_SVR_ATTR_AuxSwitch3Label = 0x0002, /**< AuxSwitch3Label (Optional) */
    ZCL_CAL_SVR_ATTR_AuxSwitch4Label = 0x0003, /**< AuxSwitch4Label (Optional) */
    ZCL_CAL_SVR_ATTR_AuxSwitch5Label = 0x0004, /**< AuxSwitch5Label (Optional) */
    ZCL_CAL_SVR_ATTR_AuxSwitch6Label = 0x0005, /**< AuxSwitch6Label (Optional) */
    ZCL_CAL_SVR_ATTR_AuxSwitch7Label = 0x0006, /**< AuxSwitch7Label (Optional) */
    ZCL_CAL_SVR_ATTR_AuxSwitch8Label = 0x0007 /**< AuxSwitch8Label (Optional) */
};

/* SE 1.4: Table D-159 Calendar Type Enumeration */
enum ZbZclCalTypeT {
    ZCL_CAL_TYPE_DELIVERED = 0x00, /**< Delivered Calendar */
    ZCL_CAL_TYPE_RECEIVED = 0x01, /**< Received Calendar */
    ZCL_CAL_TYPE_DELIV_AND_RECV = 0x02, /**< Delivered and Received Calendar */
    ZCL_CAL_TYPE_FRIENDLY_CREDIT = 0x03, /**< Friendly Credit Calendar */
    ZCL_CAL_TYPE_AUX_LOAD_SWITCH = 0x04, /**< Auxillary Load Switch Calendar */
    ZCL_CAL_TYPE_ANY = 0xff /**<  0xff means any */
};

/* SE 1.4: Table D-97 Price Tier Sub-field Enumerations */
enum ZbZclCalPriceTierT {
    ZCL_CAL_PRICE_TIER_NONE = 0x0, /**< No Tier Related */
    ZCL_CAL_PRICE_TIER_1 = 0x1, /**< Reference Tier1PriceLabel */
    ZCL_CAL_PRICE_TIER_2 = 0x2, /**< Reference Tier2PriceLabel */
    ZCL_CAL_PRICE_TIER_3 = 0x3, /**< Reference Tier3PriceLabel */
    ZCL_CAL_PRICE_TIER_4 = 0x4, /**< Reference Tier4PriceLabel */
    ZCL_CAL_PRICE_TIER_5 = 0x5, /**< Reference Tier5PriceLabel */
    ZCL_CAL_PRICE_TIER_6 = 0x6, /**< Reference Tier6PriceLabel */
    ZCL_CAL_PRICE_TIER_7 = 0x7, /**< Reference Tier7PriceLabel */
    ZCL_CAL_PRICE_TIER_8 = 0x8, /**< Reference Tier8PriceLabel */
    ZCL_CAL_PRICE_TIER_9 = 0x9, /**< Reference Tier9PriceLabel */
    ZCL_CAL_PRICE_TIER_10 = 0xA, /**< Reference Tier10PriceLabel */
    ZCL_CAL_PRICE_TIER_11 = 0xB, /**< Reference Tier11PriceLabel */
    ZCL_CAL_PRICE_TIER_12 = 0xC, /**< Reference Tier12PriceLabel */
    ZCL_CAL_PRICE_TIER_13 = 0xD, /**< Reference Tier13PriceLabel */
    ZCL_CAL_PRICE_TIER_14 = 0xE, /**< Reference Tier14PriceLabel */
    ZCL_CAL_PRICE_TIER_EXT = 0xF /**< Dependant on the value of the Extended Price Tier field */
};

/*---------------------------------------------------------------
 * API Struct Types
 *---------------------------------------------------------------
 */

#define ZCL_CAL_PUBLISH_CALENDAR_MIN_SIZE               22U

/** Publish Calendar command structure */
struct ZbZclCalServerPublishCalendarT {
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
    uint32_t start_time; /**< Start Time - UTCTime */
    enum ZbZclCalTypeT calendar_type; /**< Calendary Type */
    uint8_t calendar_time_ref; /**< Calendar Time Reference */
    uint8_t calendar_name[ZCL_CAL_NAME_LEN_MAX + 1U]; /**< Calendar Name (ZCL string char, first byte is length) */
    uint8_t num_seasons; /**< Number of Seasons */
    uint8_t num_week_profiles; /**< Number of Week Profiles */
    uint8_t num_day_profiles; /**< Number of Day Profiles */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD (in One-Way Mirror). */
};

#define ZCL_CAL_PUBLISH_DAY_PROFILE_MIN_SIZE            17U
#define ZCL_CAL_PUBLISH_DAY_PROFILE_NUM_ENTRIES_MAX     ((ZCL_PAYLOAD_UNFRAG_SAFE_SIZE - ZCL_CAL_PUBLISH_DAY_PROFILE_MIN_SIZE) / 3U)

/** Day Schedule Entries structure */
struct ZbZclCalendarDayScheduleT {
    uint16_t start_time; /**< Start Time */
    uint8_t value_u8; /**< Price Tier - if calendar type is 0x00 – 0x02.
                           Friendly Credit Enable - if calendar type is 0x03.
                           Auxiliary Load Switch State - if calendar type is 0x04. */
};

/** Publish Day Profile command structure */
struct ZbZclCalServerPublishDayProfileT {
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
    uint8_t day_id; /**< Day ID */
    uint8_t total_entries; /**< Total Number of Schedule Entries */
    uint8_t command_index; /**< Command Index */
    uint8_t num_commands; /**< Total Number of Commands */
    enum ZbZclCalTypeT calendar_type; /**< Calendar Type */
    struct ZbZclCalendarDayScheduleT entry_list[ZCL_CAL_PUBLISH_DAY_PROFILE_NUM_ENTRIES_MAX]; /**< Day Schedule Entries */
    uint8_t num_entries; /**< Number of Day Schedule Entries - maximum is 12, since ZCL_PAYLOAD_UNFRAG_SAFE_SIZE = 54 */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD (in One-Way Mirror). */
};

#define ZCL_CAL_PUBLISH_WEEK_PROFILE_MIN_SIZE           20U

/** Publish Week Profile command structure */
struct ZbZclCalServerPublishWeekProfileT {
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
    uint8_t week_id; /**< Week ID */
    uint8_t day_id_ref_mon; /**< Day ID Ref Monday */
    uint8_t day_id_ref_tue; /**< Day ID Ref Tuesday */
    uint8_t day_id_ref_wed; /**< Day ID Ref Wednesday */
    uint8_t day_id_ref_thu; /**< Day ID Ref Thursday */
    uint8_t day_id_ref_fri; /**< Day ID Ref Friday */
    uint8_t day_id_ref_sat; /**< Day ID Ref Saturday */
    uint8_t day_id_ref_sun; /**< Day ID Ref Sunday */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD (in One-Way Mirror). */
};

#define ZCL_CAL_PUBLISH_SEASONS_MIN_SIZE                14U
#define ZCL_CAL_PUBLISH_SEASONS_NUM_ENTRIES_MAX         ((ZCL_PAYLOAD_UNFRAG_SAFE_SIZE - ZCL_CAL_PUBLISH_SEASONS_MIN_SIZE) / 5U)

/** Season Entry structure */
struct ZbZclCalendarSeasonT {
    uint32_t start_date; /* ZCL date format */
    uint8_t week_id;
};

/** Publish Seasons command structure */
struct ZbZclCalPublishSeasonsT {
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
    uint8_t command_index; /**< Command Index */
    uint8_t num_commands; /**< Total Number of Commands */
    struct ZbZclCalendarSeasonT entry_list[ZCL_CAL_PUBLISH_SEASONS_NUM_ENTRIES_MAX]; /**< Season Entry */
    uint8_t num_entries; /**< Number of Season Entries - maximum is 8, since ZCL_PAYLOAD_UNFRAG_SAFE_SIZE = 54 */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD (in One-Way Mirror). */
};

#define ZCL_CAL_PUBLISH_SPECIAL_DAYS_MIN_SIZE           20U
#define ZCL_CAL_PUBLISH_SPECIAL_DAYS_NUM_ENTRIES_MAX    ((ZCL_PAYLOAD_UNFRAG_SAFE_SIZE - ZCL_CAL_PUBLISH_SPECIAL_DAYS_MIN_SIZE) / 5U)

/** Season Entry structure */
struct ZbZclCalendarSpecialDayT {
    uint32_t specialDayDate; /* ZCL date format */
    uint8_t day_id;
};

/** Publish Special Days command structure */
struct ZbZclCalPublishSpecialDaysT {
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
    uint32_t start_time; /**< Start Time - UTCTime */
    enum ZbZclCalTypeT calendar_type; /**< Calendar Type */
    uint8_t total_days; /**< Total Number of Special Days */
    uint8_t command_index; /**< Command Index */
    uint8_t num_commands; /**< Total Number of Commands */
    struct ZbZclCalendarSpecialDayT entry_list[ZCL_CAL_PUBLISH_SPECIAL_DAYS_NUM_ENTRIES_MAX]; /**< Special Day Entry */
    uint8_t num_entries; /**< Number of Special Day Entries - maximum is 6, since ZCL_PAYLOAD_UNFRAG_SAFE_SIZE = 54 */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD (in One-Way Mirror). */
};

#define ZCL_CAL_CANCEL_CALENDAR_MIN_SIZE                9U

/** Cancel Calendar command structure */
struct ZbZclCalCancelCalendarT {
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
    enum ZbZclCalTypeT calendar_type; /**< Calendar Type */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD (in One-Way Mirror). */
};

/** Get Calendar command structure */
struct ZbZclCalGetCalendarT {
    uint32_t earliest_start_time; /**< Earliest Start Time */
    uint32_t min_issuer_event_id; /**< Min. Issuer Even ID */
    uint8_t num_calendars; /**< Number of Calendars */
    enum ZbZclCalTypeT calendar_type; /**< Calendar Type */
    uint32_t provider_id; /**< Provider Id */
};

/** Get Day Profiles command structure */
struct ZbZclCalGetDayProfilesT {
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
    uint8_t start_day_id; /**< Start Day Id */
    uint8_t num_days; /**< Number of Days */
};

/** Get Week Profiles command structure */
struct ZbZclCalGetWeekProfilesT {
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
    uint8_t start_week_id; /**< Start Week Id */
    uint8_t num_weeks; /**< Number of Weeks */
};

/** Get Seasons command structure */
struct ZbZclCalGetSeasonsT {
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
};

/** Get Special Days command structure */
struct ZbZclCalGetSpecialDaysT {
    uint32_t start_time; /**< Start Time */
    uint8_t num_events; /**< Number of Events */
    enum ZbZclCalTypeT calendar_type; /**< Calendar Type */
    uint32_t provider_id; /**< Provider Id */
    uint32_t issuer_calendar_id; /**< Issuer Calendar ID */
};

/*---------------------------------------------------------------
 * API Callback Types
 *---------------------------------------------------------------
 */

/* The callback handler in the application should return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE
 * if a response has been generated, and therefore no Default Response command should be sent.
 * Or, the handler should return a ZCL error code that gets sent in a Default Response command. */

/** Calendar Server callbacks configuration */
struct ZbZclCalServerCallbacksT {
    enum ZclStatusCodeT (*get_calendar)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalGetCalendarT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Calendar command.
     * The ZCL Status return value is included in the Default Response.
     * The application may call ZbZclCalServerPublishCalendarRsp to elicit a
     * PublishCalendar command notification.
     */

    enum ZclStatusCodeT (*get_day_profiles)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalGetDayProfilesT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Day Profiles command.
     * The ZCL Status return value is included in the Default Response.
     * The application may call ZbZclCalServerPublishDayProfile to elicit a
     * PublishDayProfile command notification.
     */

    enum ZclStatusCodeT (*get_week_profiles)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalGetWeekProfilesT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Week Profiles command.
     * The ZCL Status return value is included in the Default Response.
     * The application may call ZbZclCalServerPublishWeekProfile to elicit a
     * PublishWeekProfile command notification.
     */

    enum ZclStatusCodeT (*get_seasons)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalGetSeasonsT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Seasons command.
     * The ZCL Status return value is included in the Default Response.
     * The application may call ZbZclCalServerPublishSeasons to elicit a
     * PublishSeasons command notification.
     */

    enum ZclStatusCodeT (*get_special_days)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalGetSpecialDaysT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Special Days command.
     * The ZCL Status return value is included in the Default Response.
     * The application may call ZbZclCalServerPublishSpecialDays to elicit a
     * PublishSpecialDays command notification.
     */

    enum ZclStatusCodeT (*get_calendar_cancellation)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of a Get Calendar Cancellation
     * command. The ZCL Status return value is included in the Default Response.
     * The application may call ZbZclCalServerSendCancelCalendar to elicit a
     * CancelCalendar command notification. */
};

/*---------------------------------------------------------------
 * Server API
 *---------------------------------------------------------------
 */

/**
 * Create a new instance of the Calendar server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclCalServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclCalServerCallbacksT *callbacks, void *arg);

/* If publish command is sent in response to corresponding get command, use same sequence number. Otherwise use new sequence number from
 * ZbZclGetNextSeqnum() */

/**
 * Send a Publish Calendar as a response
 * @param cluster Cluster instance from which to send this command
 * @param dstInfo Destination address for response
 * @param info Publish Calendar command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishCalendarRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCalServerPublishCalendarT *info);

/**
 * Send a Publish Calendar as an unsolicited command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response
 * @param info Publish Calendar command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishCalendarUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclCalServerPublishCalendarT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Day Profile as a response
 * @param cluster Cluster instance from which to send this command
 * @param dstInfo Destination address for response
 * @param info Publish Day Profile command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishDayProfileRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCalServerPublishDayProfileT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send a Publish Day Profile as an unsolicited command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response
 * @param info Publish Day Profile command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishDayProfileUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclCalServerPublishDayProfileT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Week Profile as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response
 * @param info Publish Week Profile command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishWeekProfileRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCalServerPublishWeekProfileT *info);

/**
 * Send a Publish Week Profile as an unsolicited command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response
 * @param info Publish Week Profile command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishWeekProfileUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclCalServerPublishWeekProfileT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Seasons as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response
 * @param info Publish Seasons command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishSeasonsRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCalPublishSeasonsT *info);

/**
 * Send a Publish Seasons as an unsolicited command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response
 * @param info Publish Seasons command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishSeasonsUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclCalPublishSeasonsT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Special Days as a response
 * @param cluster Cluster instance from which to send this command
 * @param dstInfo Destination address for response
 * @param info Publish Special Days command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishSpecialDaysRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCalPublishSpecialDaysT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send a Publish Special Days as an unsolicited command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response
 * @param info Publish Special Days command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerPublishSpecialDaysUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclCalPublishSpecialDaysT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Cancel Calendar as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response
 * @param info Cancel Calendar command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerSendCancelCalendarRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCalCancelCalendarT *info);

/**
 * Send a Cancel Calendar as an unsolicited command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response
 * @param info Cancel Calendar command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalServerSendCancelCalendarUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclCalCancelCalendarT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client API
 *---------------------------------------------------------------
 */
/** Calendar Client callbacks configuration */
struct ZbZclCalClientCallbacksT {
    enum ZclStatusCodeT (*publish_calendar)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalServerPublishCalendarT *notify, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Calendar command */

    enum ZclStatusCodeT (*publish_day_profile)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalServerPublishDayProfileT *notify, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Day Profile command */

    enum ZclStatusCodeT (*publish_week_profile)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalServerPublishWeekProfileT *notify, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Week Profile command */

    enum ZclStatusCodeT (*publish_seasons)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalPublishSeasonsT *notify, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Seasons command */

    enum ZclStatusCodeT (*publish_special_days)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalPublishSpecialDaysT *notify, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Special Days command */

    enum ZclStatusCodeT (*cancel_calendar)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclCalCancelCalendarT *notify, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Cancel Calendar command */
};

/**
 * Create a new instance of the Calendar client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclCalClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclCalClientCallbacksT *callbacks, void *arg);

/**
 * Send a Get Calendar command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Calendar command request structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalClientCommandGetCalReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclCalGetCalendarT *cmd_req, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Day Profiles command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Day Profiles command request structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalClientCommandGetDayProfilesReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclCalGetDayProfilesT *cmd_req, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Week Profile command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Week Profile command request structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalClientCommandGetWeekProfilesReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclCalGetWeekProfilesT *cmd_req, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Seasons command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Seasons command request structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalClientCommandGetSeasonsReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclCalGetSeasonsT *cmd_req, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Special Days command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Special Days command request structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalClientCommandGetSpecialDaysReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclCalGetSpecialDaysT *cmd_req, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Calendar Cancellation command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCalClientCommandGetCalCancelReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Server command parsers */

/**
 * Parse a Publish Calendar command
 * @param buf Buffer containing response information
 * @param len Length of response information in bytes
 * @param rsp Publish Calendar command structure
 * @return True on success, false otherwise
 */
bool ZbZclCalParsePublishCalendar(const uint8_t *buf, unsigned int len, struct ZbZclCalServerPublishCalendarT *rsp);

/**
 * Parse a Publish Day Profile command
 * @param buf Buffer containing response information
 * @param len Length of response information in bytes
 * @param rsp Publish Day Profile command structure
 * @return True on success, false otherwise
 */
bool ZbZclCalParsePublishDayProfile(const uint8_t *buf, unsigned int len, struct ZbZclCalServerPublishDayProfileT *rsp);

/**
 * Parse a Publish Week Profile command
 * @param buf Buffer containing response information
 * @param len Length of response information in bytes
 * @param rsp Publish Week Profile command structure
 * @return True on success, false otherwise
 */
bool ZbZclCalParsePublishWeekProfile(const uint8_t *buf, unsigned int len, struct ZbZclCalServerPublishWeekProfileT *rsp);

/**
 * Parse a Publish Seasons command
 * @param buf Buffer containing response information
 * @param len Length of response information in bytes
 * @param rsp Publish Seasons command structure
 * @return True on success, false otherwise
 */
bool ZbZclCalParsePublishSeasons(const uint8_t *buf, unsigned int len, struct ZbZclCalPublishSeasonsT *rsp);

/**
 * Parse a Publish Special Days command
 * @param buf Buffer containing response information
 * @param len Length of response information in bytes
 * @param rsp Publish Special Days command structure
 * @return True on success, false otherwise
 */
bool ZbZclCalParsePublishSpecialDays(const uint8_t *buf, unsigned int len, struct ZbZclCalPublishSpecialDaysT *rsp);

/**
 * Parse a Cancel Calendar command
 * @param buf Buffer containing response information
 * @param len Length of response information in bytes
 * @param rsp Cancel Calendar command structure
 * @return True on success, false otherwise
 */
bool ZbZclCalParseCancelCalendar(const uint8_t *buf, unsigned int len, struct ZbZclCalCancelCalendarT *rsp);

/*-----------------------------------------------------------------------------
 * Calendar Mirror Client
 *-----------------------------------------------------------------------------
 */

/**
 * Allocates a Calendar Client Mirror cluster.
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param calendar_server Pointer to Calendar Server cluster. This is used by the
 *  Metering Mirror to send mirrored commands to the BOMD.
 * @param meter_mirror Pointer to Metering Mirror cluster. This is used to queue
 *  any commands that need to be forwarded to the BOMD when it wakes up.
 * @param meter_client Pointer to Metering Client cluster. This is used to know the
 *  existing notification scheme setup between BOMD & ESI and to determine if the
 *  command needs to be queued or only the notification flag needs to be set.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclCalendarMirrorAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclClusterT *calendar_server, struct ZbZclClusterT *meter_mirror,
    struct ZbZclClusterT *meter_client);

#endif
