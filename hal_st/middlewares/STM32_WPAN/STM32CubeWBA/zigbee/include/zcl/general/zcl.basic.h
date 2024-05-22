/**
 * There is no public API to create a new Basic cluster server instance, i.e. ZbZclBasicServerAlloc does not exist. The stack automatically
 * allocates a new instance of the server on every endpoint
 * @file zcl.basic.h
 * @heading Basic
 * @brief ZCL Basic cluster header
 * ZCL 7 section 3.2
 * ZCL 8 section 3.2
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_BASIC_H
# define ZCL_BASIC_H

/* @PICS.ZCL.Basic
 * B.S | Server | True
 * B.C | Client | True
 *
 * Server Attributes
 * B.S.A0000 | ZCLVersion | True
 * B.S.A0001 | ApplicationVersion | True | Optional
 * B.S.A0002 | StackVersion | True | Optional
 * B.S.A0003 | HWVersion | True | Optional
 * B.S.A0004 | ManufacturerName | True | Optional
 * B.S.A0005 | ModelIdentifier | True | Optional
 * B.S.A0006 | DateCode | True | Optional
 * B.S.A0007 | PowerSource | True
 * B.S.A0008 | GenericDevice-Class | False | Optional
 * B.S.A0009 | GenericDevice-Type | False | Optional
 * B.S.A000a | ProductCode | False | Optional
 * B.S.A000b | ProductURL | False | Optional
 * B.S.A0010 | LocationDescription | True | Optional
 * B.S.A0011 | PhysicalEnvironment | True | Optional
 * B.S.A0012 | DeviceEnabled | False | Optional
 * B.S.A0013 | AlarmMask | True | Optional
 * B.S.A0014 | DisableLocalConfig | True | Optional
 * B.S.A4000 | SWBuildID | True | Optional
 * B.S.Afffd | ClusterRevision | True
 * B.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * B.S.C00.Rsp | Reset to Factory Defaults | True
 *
 * Client Attributes
 * B.C.Afffd | ClusterRevision | True
 * B.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Generated
 * B.C.C00.Tx | Reset to Factory Defaults | True
 */

#include "zcl/zcl.h"

/** Basic Server Attribute IDs */
enum ZbZclBasicSvrAttrT {
    ZCL_BASIC_ATTR_ZCL_VERSION = 0x0000, /**< ZCLVersion */
    ZCL_BASIC_ATTR_APP_VERSION = 0x0001, /**< ApplicationVersion (Optional) */
    ZCL_BASIC_ATTR_STACK_VERSION = 0x0002, /**< StackVersion (Optional) */
    ZCL_BASIC_ATTR_HARDWARE_VERSION = 0x0003, /**< HWVersion (Optional) */
    ZCL_BASIC_ATTR_MFR_NAME = 0x0004, /**< ManufacturerName (Optional) */
    ZCL_BASIC_ATTR_MODEL_NAME = 0x0005, /**< ModelIdentifier (Optional) */
    ZCL_BASIC_ATTR_DATE_CODE = 0x0006, /**< DateCode (Optional) */
    ZCL_BASIC_ATTR_POWER_SOURCE = 0x0007, /**< PowerSource */

    /* CCB 1584 */
    ZCL_BASIC_ATTR_MFR_VERSION_DETAILS = 0x000c, /**< ManufacturerVersionDetails (Optional) */
    ZCL_BASIC_ATTR_SERIAL_NUMBER = 0x000d, /**< SerialNumber (Optional) */
    ZCL_BASIC_ATTR_PRODUCT_LABEL = 0x000e, /**< ProductLabel (Optional) */

    ZCL_BASIC_ATTR_LOCATION = 0x0010, /**< LocationDescription (Optional) */
    ZCL_BASIC_ATTR_ENVIRONMENT = 0x0011, /**< PhysicalEnvironment (Optional) */
    ZCL_BASIC_ATTR_ENABLED = 0x0012, /**< DeviceEnabled (Optional) */
    ZCL_BASIC_ATTR_ALARM_MASK = 0x0013, /**< AlarmMask (Optional) */
    ZCL_BASIC_ATTR_DISABLE_LOCAL_CONFIG = 0x0014, /**< DisableLocalConfig (Optional) */

    ZCL_BASIC_ATTR_SW_BUILD_ID = 0x4000 /**< SWBuildID (Optional) */
};

/* Power Source Enumerations */
#define ZCL_BASIC_POWER_UNKNOWN                     0x00
#define ZCL_BASIC_POWER_SINGLE_PHASE                0x01
#define ZCL_BASIC_POWER_THREE_PHASE                 0x02
#define ZCL_BASIC_POWER_BATTERY                     0x03
#define ZCL_BASIC_POWER_DC                          0x04
#define ZCL_BASIC_POWER_EMERGENCY_CONSTANT          0x05
#define ZCL_BASIC_POWER_EMERGENCY_TRANSFER          0x06
#define ZCL_BASIC_POWER_BATTERY_BACKUP_BIT          0x80

/* Physical Environment Enumerations */
enum ZbZclBasicPhysicalEnvT {
    ZCL_BASIC_ENV_UNSPECIFIED = 0x00, /**< Unspecified environment */
    /* CCB 2197. Deprecated 0x01. For Profile Id 0x0109 (SE) use and backward
     * compatibility only. Use 0x71 moving forward. */
    ZCL_BASIC_ENV_BAR = 0x02,
    ZCL_BASIC_ENV_COURTYARD = 0x03,
    ZCL_BASIC_ENV_BATHROOM = 0x04,
    ZCL_BASIC_ENV_BEDROOM = 0x05,
    ZCL_BASIC_ENV_BILLIARD = 0x06,
    ZCL_BASIC_ENV_UTILITY = 0x07,
    ZCL_BASIC_ENV_CELLAR = 0x08,
    ZCL_BASIC_ENV_STORAGE = 0x09,
    ZCL_BASIC_ENV_THEATER = 0x0a,
    ZCL_BASIC_ENV_OFFICE = 0x0b,
    ZCL_BASIC_ENV_DECK = 0x0c,
    ZCL_BASIC_ENV_DEN = 0x0d,
    ZCL_BASIC_ENV_DINING = 0x0e,
    ZCL_BASIC_ENV_ELECTRICAL = 0x0f,
    ZCL_BASIC_ENV_ELEVATOR = 0x10,
    ZCL_BASIC_ENV_ENTRY = 0x11,
    ZCL_BASIC_ENV_FAMILY = 0x12,
    ZCL_BASIC_ENV_MAIN_FLOOR = 0x13,
    ZCL_BASIC_ENV_UPSTAIRS = 0x14,
    ZCL_BASIC_ENV_DOWNSTAIRS = 0x15,
    ZCL_BASIC_ENV_BASEMENT = 0x16,
    ZCL_BASIC_ENV_GALLERY = 0x17,
    ZCL_BASIC_ENV_GAME = 0x18,
    ZCL_BASIC_ENV_GARAGE = 0x19,
    ZCL_BASIC_ENV_GYM = 0x1a,
    ZCL_BASIC_ENV_HALLWAY = 0x1b,
    ZCL_BASIC_ENV_HOUSE = 0x1c,
    ZCL_BASIC_ENV_KITCHEN = 0x1d,
    ZCL_BASIC_ENV_LAUNDRY = 0x1e,
    ZCL_BASIC_ENV_LIBRARY = 0x1f,
    ZCL_BASIC_ENV_MASTER_BEDROOM = 0x20,
    ZCL_BASIC_ENV_MUDROOM = 0x21,
    ZCL_BASIC_ENV_NURSERY = 0x22,
    ZCL_BASIC_ENV_PANTRY = 0x23,
    ZCL_BASIC_ENV_OFFICE_x24 = 0x24, /**< NOTE: Duplicate. Already defined as 0x0b */
    ZCL_BASIC_ENV_OUTSIDE = 0x25,
    ZCL_BASIC_ENV_POOL = 0x26,
    ZCL_BASIC_ENV_PORCH = 0x27,
    ZCL_BASIC_ENV_SEWING = 0x28,
    ZCL_BASIC_ENV_SITTING = 0x29,
    ZCL_BASIC_ENV_STAIRWAY = 0x2a,
    ZCL_BASIC_ENV_YARD = 0x2b,
    ZCL_BASIC_ENV_ATTIC = 0x2c,
    ZCL_BASIC_ENV_HOT_TUB = 0x2d,
    ZCL_BASIC_ENV_LIVING_ROOM = 0x2e,
    ZCL_BASIC_ENV_SAUNA = 0x2f,
    ZCL_BASIC_ENV_WORKSHOP = 0x30,
    ZCL_BASIC_ENV_GUEST_BEDROOM = 0x31,
    ZCL_BASIC_ENV_GUEST_BATH = 0x32,
    ZCL_BASIC_ENV_POWDER_ROOM = 0x33,
    ZCL_BASIC_ENV_BACK_YARD = 0x34,
    ZCL_BASIC_ENV_FRONT_YARD = 0x35,
    ZCL_BASIC_ENV_PATIO = 0x36,
    ZCL_BASIC_ENV_DRIVEWAY = 0x37,
    ZCL_BASIC_ENV_SUN_ROOM = 0x38,
    ZCL_BASIC_ENV_LIVING_ROOM_x39 = 0x39, /**< NOTE: Duplicate. Already defined as 0x2e */
    ZCL_BASIC_ENV_SPA = 0x3a,
    ZCL_BASIC_ENV_WHIRLPOOL = 0x3b,
    ZCL_BASIC_ENV_SHED = 0x3c,
    ZCL_BASIC_ENV_EQUIP_STORAGE = 0x3d,
    ZCL_BASIC_ENV_HOBBY_ROOM = 0x3e,
    ZCL_BASIC_ENV_FOUNTAIN = 0x3f,
    ZCL_BASIC_ENV_POND = 0x40,
    ZCL_BASIC_ENV_RECEPTION_ROOM = 0x41,
    ZCL_BASIC_ENV_BREAKFAST_ROOM = 0x42,
    ZCL_BASIC_ENV_NOOK = 0x43,
    ZCL_BASIC_ENV_GARDEN = 0x44,
    ZCL_BASIC_ENV_BALCONY = 0x45,
    ZCL_BASIC_ENV_PANIC_ROOM = 0x46,
    ZCL_BASIC_ENV_TERRACE = 0x47,
    ZCL_BASIC_ENV_ROOF = 0x48,
    ZCL_BASIC_ENV_TOILET = 0x49,
    ZCL_BASIC_ENV_TOILET_MAIN = 0x4a,
    ZCL_BASIC_ENV_TOILET_OUTSIDE = 0x4b,
    ZCL_BASIC_ENV_SHOWER_ROOM = 0x4c,
    ZCL_BASIC_ENV_STUDY = 0x4d,
    ZCL_BASIC_ENV_FRONT_GARDEN = 0x4e,
    ZCL_BASIC_ENV_BACK_GARDEN = 0x4f,
    ZCL_BASIC_ENV_KETTLE = 0x50,
    ZCL_BASIC_ENV_TELEVISION = 0x51,
    ZCL_BASIC_ENV_STOVE = 0x52,
    ZCL_BASIC_ENV_MICROWAVE = 0x53,
    ZCL_BASIC_ENV_TOASTER = 0x54,
    ZCL_BASIC_ENV_VACUUM = 0x55,
    ZCL_BASIC_ENV_APPLIANCE = 0x56,
    ZCL_BASIC_ENV_FRONT_DOOR = 0x57,
    ZCL_BASIC_ENV_BACK_DOOR = 0x58,
    ZCL_BASIC_ENV_FRIDGE_DOOR = 0x59,
    /* discontinuity. Spec editor doesn't know hex? */
    ZCL_BASIC_ENV_MEDICINE_DOOR = 0x60,
    ZCL_BASIC_ENV_WARDROBE_DOOR = 0x61,
    ZCL_BASIC_ENV_CUPBOARD_DOOR = 0x62,
    ZCL_BASIC_ENV_OTHER_DOOR = 0x63,
    ZCL_BASIC_ENV_WAITING_ROOM = 0x64,
    ZCL_BASIC_ENV_TRIAGE_ROOM = 0x65,
    ZCL_BASIC_ENV_DOCTOR_OFFICE = 0x66,
    ZCL_BASIC_ENV_PATIENT_ROOM = 0x67,
    ZCL_BASIC_ENV_CONSULT_ROOM = 0x68,
    ZCL_BASIC_ENV_NURSE_STATION = 0x69,
    ZCL_BASIC_ENV_WARD = 0x6a,
    ZCL_BASIC_ENV_CORRIDOR = 0x6b,
    ZCL_BASIC_ENV_OPERATING_THEATRE = 0x6c,
    ZCL_BASIC_ENV_DENTAL_SURGERY = 0x6d,
    ZCL_BASIC_ENV_MEDICAL_IMAGING = 0x6e,
    ZCL_BASIC_ENV_DECONTAMINATION = 0x6f,
    ZCL_BASIC_ENV_ATRIUM = 0x70,
    ZCL_BASIC_ENV_MIRROR = 0x71, /**< Mirror. CCB 2197 */
    ZCL_BASIC_ENV_UNKNOWN = 0xff /**< Unknown environment */
};

/* Alarm Codes */
#define ZCL_BASIC_ALARM_CODE_HARDWARE               0x00
#define ZCL_BASIC_ALARM_CODE_SOFTWARE               0x01

/* Alarm Mask */
#define ZCL_BASIC_ALARM_MASK_ALL                    0x03U
#define ZCL_BASIC_ALARM_MASK_HARDWARE               0x01U
#define ZCL_BASIC_ALARM_MASK_SOFTWARE               0x02U

/* DisableLocalConfig Bit Mask (ZCL_BASIC_ATTR_DISABLE_LOCAL_CONFIG) */
#define ZCL_BASIC_DISABLE_MASK_RESET                0x01U /* If set, Reset (to factory defaults) is disabled. */
#define ZCL_BASIC_DISABLE_MASK_DEVICE_CONFIG        0x02U /* If set, Device configuration is disabled. */

/* ZCL_BASIC_ATTR_ZCL_VERSION */
enum {
    ZCL_BASIC_ZCL_VERSION_6 = 0x02U,
    ZCL_BASIC_ZCL_VERSION_7 = 0x03U,
    ZCL_BASIC_ZCL_VERSION_8 = 0x08U /* Starting from ZCL8, this number matches the revision of ZCL. */
};

/* Commands */
enum {
    ZCL_BASIC_RESET_FACTORY = 0x00
};

/* The Basic Server is a special cluster that has global attributes and
 * behaviour. The API is found in zigbee.h. */

/**
 * Create a new instance of the Basic Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclBasicClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send a Reset to Factory Defaults command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclBasicClientResetReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst);

#endif /* __ZCL_BASIC_H */
