/**
 * @file zcl.prepay.h
 * @heading Prepayment
 * @brief ZCL Prepayment cluster header
 * ZCL 8 section 10.8
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_PREPAY_H
# define ZCL_PREPAY_H

#include "zcl/zcl.h"

/*lint -e621 "identifier clash within 31 chars [MISRA Rule 5.1 (REQUIRED)]" */

/** Prepayment Server Attribute IDs */
enum ZbZclPreaymentSvrAttrT {
    /* Prepayment Information Attribute Set */
    ZCL_PREPAY_SVR_ATTR_PAYMENT_CONTROL_CONFIG = 0x0000, /**< Payment Control Configuration */
    ZCL_PREPAY_SVR_ATTR_CREDIT_REMAINING = 0x0001, /**< Credit Remaining (Optional) */
    ZCL_PREPAY_SVR_ATTR_EMERG_CREDIT_REMAINING = 0x0002, /**< Emergency Credit Remaining (Optional) */
    ZCL_PREPAY_SVR_ATTR_CREDIT_STATUS = 0x0003, /**< Credit Status (Optional) */
    ZCL_PREPAY_SVR_ATTR_CREDIT_REMAIN_TIMESTAMP = 0x0004, /**< CreditRemaining TimeStamp (Optional) */
    ZCL_PREPAY_SVR_ATTR_ACCUMULATED_DEBT = 0x0005, /**< Accumulated Debt (Optional) */
    ZCL_PREPAY_SVR_ATTR_OVERALL_DEBT_CAP = 0x0006, /**< OverallDebtCap (Optional) */
    /* Reserved (0x0007 to 0x000F) */
    ZCL_PREPAY_SVR_ATTR_EMERG_CREDIT_ALLOWANCE = 0x0010, /**< EmergencyCredit Limit/Allowance (Optional) */
    ZCL_PREPAY_SVR_ATTR_EMERG_CREDIT_THRESHOLD = 0x0011, /**< EmergencyCredit Threshold (Optional) */
    /* Reserved (0x0012 to 0x001F) */
    ZCL_PREPAY_SVR_ATTR_TOTAL_CREDIT_ADDED = 0x0020, /**< TotalCreditAdded (Optional) */
    ZCL_PREPAY_SVR_ATTR_MAX_CRED_LIMIT = 0x0021, /**< MaxCreditLimit (Optional) */
    ZCL_PREPAY_SVR_ATTR_MAX_CRED_PER_TOPUP = 0x0022, /**< MaxCreditPerTopUp (Optional) */
    /* Reserved (0x0023 to 0x002F) */
    ZCL_PREPAY_SVR_ATTR_FRIENDLY_CREDIT_WARNING = 0x0030, /**< FriendlyCredit Warning (Optional) */
    ZCL_PREPAY_SVR_ATTR_LOW_CREDIT_WARNING = 0x0031, /**< LowCredit Warning (Optional) */
    ZCL_PREPAY_SVR_ATTR_IHD_LOW_CREDIT_WARNING = 0x0032, /**< IHDLow CreditWarning (Optional) */
    ZCL_PREPAY_SVR_ATTR_INTERRUPT_SUSPEND_TIME = 0x0033, /**< InterruptSuspend Time (Optional) */
    ZCL_PREPAY_SVR_ATTR_REMAINING_FRIENDLY_CREDIT_TIME = 0x0034, /**< RemainingFriendlyCreditTime (Optional) */
    ZCL_PREPAY_SVR_ATTR_NEXT_FRIENDLY_CREDIT_TIME = 0x0035, /**< NextFriendly CreditPeriod (Optional) */
    /* Reserved (0x0036 to 0x003F) */
    ZCL_PREPAY_SVR_ATTR_CUT_OFF_VALUE = 0x0040, /**< CutOffValue (Optional) */
    /* Reserved (0x0041 to 0x007F) */
    ZCL_PREPAY_SVR_ATTR_TOKEN_CARRIER_ID = 0x0080, /**< TokenCarrierID (Optional) */

    /* Top-up Attribute Set (0x01) */
    ZCL_PREPAY_SVR_ATTR_TOPUP_DATE_TIME_1 = 0x0100,
    /**< Top up Date/Time #1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_TOPUP_DATE_TIME_N(1)
     * For the remaining Top up Date/Time attributes, use the ZCL_PREPAY_SVR_ATTR_TOPUP_DATE_TIME_N macro. */
    ZCL_PREPAY_SVR_ATTR_TOPUP_AMOUNT_1 = 0x0101,
    /**< Top up Amount #1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_TOPUP_AMOUNT_N(1)
     * For the remaining Top up Amount attributes, use the ZCL_PREPAY_SVR_ATTR_TOPUP_AMOUNT_N macro. */
    ZCL_PREPAY_SVR_ATTR_TOPUP_ORIGINATOR = 0x0102,
    /**< Originating Device #1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_TOPUP_ORIGINATOR_N(1)
     * For the remaining Orginating Device attributes, use the ZCL_PREPAY_SVR_ATTR_TOPUP_ORIGINATOR_N macro. */
    ZCL_PREPAY_SVR_ATTR_TOPUP_CODE = 0x0103,
    /**< Top up Code #1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_TOPUP_CODE_N(1)
     * For the remaining Top up Code attributes, use the ZCL_PREPAY_SVR_ATTR_TOPUP_CODE_N macro. */

    /* Debt Attribute Set (0x02) */
    ZCL_PREPAY_SVR_ATTR_DEBT_LABEL_1 = 0x0210,
    /**< DebtLabel#1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_DEBT_LABEL_N(1)
     * For the remaining DebtLabel attributes, use the ZCL_PREPAY_SVR_ATTR_DEBT_LABEL_N macro. */
    ZCL_PREPAY_SVR_ATTR_DEBT_AMOUNT_1 = 0x0211,
    /**< DebtAmount#1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_DEBT_AMOUNT_N(1)
     * For the remaining DebtAmount attributes, use the ZCL_PREPAY_SVR_ATTR_DEBT_AMOUNT_N macro. */
    ZCL_PREPAY_SVR_ATTR_DEBT_REC_METHOD_1 = 0x0212,
    /**< DebtRecovery Method#1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_DEBT_REC_METHOD_N(1)
     * For the remaining DebtRecovery Method attributes, use the ZCL_PREPAY_SVR_ATTR_DEBT_REC_METHOD_N macro. */
    ZCL_PREPAY_SVR_ATTR_DEBT_REC_START_TIME_1 = 0x0213,
    /**< DebtRecovery StartTime#1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_DEBT_REC_START_TIME_N(1)
     * For the remaining DebtRecovery StartTime attributes, use the ZCL_PREPAY_SVR_ATTR_DEBT_REC_START_TIME_N macro. */
    ZCL_PREPAY_SVR_ATTR_DEBT_REC_COLLECT_TIME_1 = 0x0214,
    /**< DebtRecovery CollectionTime#1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_DEBT_REC_COLLECT_TIME_N(1)
     * For the remaining DebtRecovery CollectionTime attributes, use the ZCL_PREPAY_SVR_ATTR_DEBT_REC_COLLECT_TIME_N macro. */
    ZCL_PREPAY_SVR_ATTR_DEBT_REC_FREQ_1 = 0x0216,
    /**< DebtRecovery Frequency#1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_DEBT_REC_FREQ_N(1)
     * For the remaining DebtRecovery Frequency attributes, use the ZCL_PREPAY_SVR_ATTR_DEBT_REC_FREQ_N macro. */
    ZCL_PREPAY_SVR_ATTR_DEBT_REC_AMOUNT_1 = 0x0217,
    /**< DebtRecovery Amount#1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_DEBT_REC_AMOUNT_N(1)
     * For the remaining DebtRecovery Amount attributes, use the ZCL_PREPAY_SVR_ATTR_DEBT_REC_AMOUNT_N macro. */
    ZCL_PREPAY_SVR_ATTR_DEBT_REC_TOPUP_PERCENT_1 = 0x0219,
    /**< DebtRecovery TopUpPercentage#1 (Optional)
     * ZCL_PREPAY_SVR_ATTR_DEBT_REC_TOPUP_PERCENT_N(1)
     * For the remaining DebtRecovery TopUpPercentage attributes, use the ZCL_PREPAY_SVR_ATTR_DEBT_REC_TOPUP_PERCENT_N macro. */

    /* Alarms Attribute Set */
    ZCL_PREPAY_SVR_ATTR_ALARM_STATUS = 0x0400, /**< PrepaymentAlarmStatus (Optional) */
    ZCL_PREPAY_SVR_ATTR_GENERIC_ALARM_MASK = 0x0401, /**< PrepayGenericAlarmMask (Optional) */
    ZCL_PREPAY_SVR_ATTR_SWITCH_ALARM_MASK = 0x0402, /**< PrepaySwitchAlarmMask (Optional) */
    ZCL_PREPAY_SVR_ATTR_EVENT_ALARM_MASK = 0x0403, /**< PrepayEventAlarmMask (Optional) */

    /* Historical Cost Consumption Information Set */
    ZCL_PREPAY_SVR_ATTR_HIST_CCTION_FORMAT = 0x0500, /**< HistoricalCostConsumption Formatting (Optional) */
    ZCL_PREPAY_SVR_ATTR_CONSUMPTION_UNIT_OF_MEASURE = 0x0501, /**< ConsumptionUnitofMeasurement (Optional) */
    ZCL_PREPAY_SVR_ATTR_CURRENCY_SCALING_FACTOR = 0x0502, /**< CurrencyScalingFactor (Optional) */
    ZCL_PREPAY_SVR_ATTR_CURRENCY = 0x0503, /**< Currency (Optional) */

    /* 0x051C to 0x052D */
    /* Historical Cost Consumption Information Set (Day) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_DAY_0 = 0x051C,
    /**< CurrentDay CostConsumptionDelivered (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_DAY_N(0) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_DAY_0 = 0x051D,
    /**< CurrentDay CostConsumptionReceived (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_DAY_N(0) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_DAY_1 = 0x051E,
    /**< PreviousDay CostConsumptionDelivered (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_DAY_N(1) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_DAY_1 = 0x051F,
    /**< PreviousDay CostConsumptionReceived (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_DAY_N(1) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_DAY_2 = 0x0520,
    /**< PreviousDay2 CostConsumptionDelivered (Optional)
     * ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_DAY_N(2)
     * For the remaining days, use the ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_DAY_N(prev_day) macro. */
    ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_DAY_2 = 0x0521,
    /**< PreviousDay2 CostConsumptionReceived (Optional)
     * ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_DAY_N(2)
     * For the remaining days, use the ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_DAY_N(prev_day) macro. */

    /* 0x0530 to 0x053B */
    /* Historical Cost Consumption Information Set (Week) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_WEEK_0 = 0x0530,
    /**< CurrentWeek CostConsumptionDelivered (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_WEEK_N(0) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_WEEK_0 = 0x0531,
    /**< CurrentWeek CostConsumptionReceived (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_WEEK_N(0) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_WEEK_1 = 0x0532,
    /**< PreviousWeek CostConsumptionDelivered (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_WEEK_N(1) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_WEEK_1 = 0x0533,
    /**< PreviousWeek CostConsumptionReceived (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_WEEK_N(1) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_WEEK_2 = 0x0534,
    /**< PreviousWeek2 CostConsumptionDelivered (Optional)
     * ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_WEEK_N(2)
     * For the remaining weeks, use the ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_WEEK_N(prev_week) macro. */
    ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_WEEK_2 = 0x0535,
    /**< PreviousWeek2 CostConsumptionReceived (Optional)
     * ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_WEEK_N(2)
     * For the remaining weeks, use the ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_WEEK_N(prev_week) macro. */

    /* 0x0540 to 0x055B */
    /* Historical Cost Consumption Information Set (Month) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_MONTH_0 = 0x0540,
    /**< CurrentMonth CostConsumptionDelivered (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_MONTH_N(0) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_MONTH_0 = 0x0541,
    /**< CurrentMonth CostConsumptionReceived (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_MONTH_N(0) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_MONTH_1 = 0x0542,
    /**< PreviousMonth CostConsumptionDelivered (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_MONTH_N(1) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_MONTH_1 = 0x0543,
    /**< PreviousMonth CostConsumptionReceived (Optional) - ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_MONTH_N(1) */
    ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_MONTH_2 = 0x0544,
    /**< PreviousMonth2 CostConsumptionDelivered (Optional)
     * ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_MONTH_N(2)
     * For the remaining months, use the ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_MONTH_N(prev_month) macro. */
    ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_MONTH_2 = 0x0545,
    /**< PreviousMonth2 CostConsumptionReceived (Optional)
     * ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_MONTH_N(2)
     * For the remaining months, use the ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_MONTH_N(prev_month) macro. */

    ZCL_PREPAY_SVR_ATTR_HIST_FREEZE_TIME = 0x055C /**< Historical Freeze Time (Optional) */
};

/* Top-up Attribute Set (1 to 5) */
#define ZCL_PREPAY_SVR_ATTR_TOPUP_DATE_TIME_N(_n_)             (0x0100U + (0x10U * ((_n_) - 1U)))
#define ZCL_PREPAY_SVR_ATTR_TOPUP_AMOUNT_N(_n_)                (0x0101U + (0x10U * ((_n_) - 1U)))
#define ZCL_PREPAY_SVR_ATTR_TOPUP_ORIGINATOR_N(_n_)            (0x0102U + (0x10U * ((_n_) - 1U)))
#define ZCL_PREPAY_SVR_ATTR_TOPUP_CODE_N(_n_)                  (0x0103U + (0x10U * ((_n_) - 1U)))

/* Debt Attribute Set (1 to 3) */
#define ZCL_PREPAY_SVR_ATTR_DEBT_LABEL_N(_n_)                   (0x0200U + (0x10U * (_n_)))
#define ZCL_PREPAY_SVR_ATTR_DEBT_AMOUNT_N(_n_)                  (0x0201U + (0x10U * (_n_)))
#define ZCL_PREPAY_SVR_ATTR_DEBT_REC_METHOD_N(_n_)              (0x0202U + (0x10U * (_n_)))
#define ZCL_PREPAY_SVR_ATTR_DEBT_REC_START_TIME_N(_n_)          (0x0203U + (0x10U * (_n_)))
#define ZCL_PREPAY_SVR_ATTR_DEBT_REC_COLLECT_TIME_N(_n_)        (0x0204U + (0x10U * (_n_)))
/* Reserved (0x02N5) */
#define ZCL_PREPAY_SVR_ATTR_DEBT_REC_FREQ_N(_n_)                (0x0206U + (0x10U * (_n_)))
#define ZCL_PREPAY_SVR_ATTR_DEBT_REC_AMOUNT_N(_n_)              (0x0207U + (0x10U * (_n_)))
/* Reserved (0x02N8) */
#define ZCL_PREPAY_SVR_ATTR_DEBT_REC_TOPUP_PERCENT_N(_n_)      (0x0209U + (0x10U * (_n_)))

/* CurrentDayCostConsumptionDelivered(N = 0)..PreviousDay8CostConsumptionDelivered(N = 8) */
#define ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_DAY_N(prev_day)              (0x051CU + (2U * (prev_day)))
/* CurrentDayCostConsumptionReceived(N = 0)..PreviousDay8CostConsumptionReceived(N = 8) */
#define ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_DAY_N(prev_day)           (0x051DU + (2U * (prev_day)))

/* CurrentWeekCostConsumptionDelivered(N = 0)..PreviousWeek5CostConsumptionDelivered(N = 5) */
#define ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_WEEK_N(prev_week)            (0x0530U + (2U * (prev_week)))
/* CurrentWeekCostConsumptionReceived(N = 0)..PreviousWeek5CostConsumptionReceived(N = 5) */
#define ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_WEEK_N(prev_week)         (0x0531U + (2U * (prev_week)))

/* CurrentMonthCostConsumptionDelivered(N = 0)..PreviousMonth13CostConsumptionDelivered(N = 5) */
#define ZCL_PREPAY_SVR_ATTR_COSTCON_DELIV_MONTH_N(prev_month)          (0x0540U + (2U * (prev_month)))
/* CurrentMonthCostConsumptionReceived(N = 0)..PreviousMonth13CostConsumptionReceived(N = 5) */
#define ZCL_PREPAY_SVR_ATTR_COSTCON_RECEIVED_MONTH_N(prev_month)       (0x0541U + (2U * (prev_month)))

/* Payment Control Configuration Bits */
#define ZCL_PREPAY_CONTROL_DISCONNECT_ENABLE                    0x0001U
#define ZCL_PREPAY_CONTROL_PREPAYMENT_ENABLE                    0x0002U
#define ZCL_PREPAY_CONTROL_CREDIT_MANAGEMENT_ENABLE             0x0004U
#define ZCL_PREPAY_CONTROL_CREDIT_DISPLAY_ENABLE                0x0010U
#define ZCL_PREPAY_CONTROL_ACCOUNT_BASE                         0x0040U
#define ZCL_PREPAY_CONTROL_CONTRACTOR_FITTED                    0x0080U
#define ZCL_PREPAY_CONTROL_STANDING_CHARGE                      0x0100U
#define ZCL_PREPAY_CONTROL_EMERGENCY_STANDING_CHARGE            0x0200U
#define ZCL_PREPAY_CONTROL_DEBT_CONFIGURATION                   0x0400U
#define ZCL_PREPAY_CONTROL_EMERGENCY_DEBT_CONFIGURATION         0x0800U

/* Friendly Credit BitMap */
#define ZCL_PREPAY_FRIENDLY_CREDIT_ENABLED                      0x01U

/* Credit Status Bits */
#define ZCL_PREPAY_CREDIT_OK                                    0x01U
#define ZCL_PREPAY_CREDIT_LOW                                   0x02U
#define ZCL_PREPAY_CREDIT_EMERGENCY_ENABLED                     0x04U
#define ZCL_PREPAY_CREDIT_EMERGENCY_AVAILABLE                   0x08U
#define ZCL_PREPAY_CREDIT_EMERGENCY_SELECTED                    0x10U
#define ZCL_PREPAY_CREDIT_EMERGENCY_IN_USE                      0x20U
#define ZCL_PREPAY_CREDIT_EMERGENCY_EXHAUSTED                   0x40U

/* Debt recovery period enumerations. */
#define ZCL_PREPAY_RECOVERY_PER_HOUR                            0x0U
#define ZCL_PREPAY_RECOVERY_PER_DAY                             0x1U
#define ZCL_PREPAY_RECOVERY_PER_WEEK                            0x2U
#define ZCL_PREPAY_RECOVERY_PER_MONTH                           0x3U
#define ZCL_PREPAY_RECOVERY_PER_QUARTER                         0x4U

/* Delayed Supply Interrupt Value types. */
#define ZCL_PREPAY_INTERRUPT_TYPE_KWH                           0x0U
#define ZCL_PREPAY_INTERRUPT_TYPE_CUBIC_METER                   0x1U

/* Alarm Status bits */
#define ZCL_PREPAY_ALARM_LOW_CREDIT_WARN                        0x0001U
#define ZCL_PREPAY_ALARM_TOP_UP_CODE_ERROR                      0x0002U
#define ZCL_PREPAY_ALARM_TOP_UP_CODE_USED                       0x0004U
#define ZCL_PREPAY_ALARM_TOP_UP_CODE_INVALID                    0x0008U
#define ZCL_PREPAY_ALARM_FRIENDLY_CREDIT_IN_USE                 0x0010U
#define ZCL_PREPAY_ALARM_FRIENDLY_CREDIT_PERIOD_END_WARN        0x0020U
#define ZCL_PREPAY_ALARM_EC_AVAILABLE                           0x0040U
#define ZCL_PREPAY_ALARM_UNAUTH_ENERGY_USE                      0x0080U
#define ZCL_PREPAY_ALARM_DISCON_SUPPLY_DUE_TO_CREDIT            0x0100U
#define ZCL_PREPAY_ALARM_DISCON_SUPPLY_DUE_TO_TEMPER            0x0200U
#define ZCL_PREPAY_ALARM_DISCON_SUPPLY_DUE_TO_HES               0x0400U
#define ZCL_PREPAY_ALARM_PHYSICAL_ATTACK                        0x0800U
#define ZCL_PREPAY_ALARM_ELECTRONIC_ATTACK                      0x1000U
#define ZCL_PREPAY_ALARM_MANUF_ALARM_CODE_A                     0x2000U
#define ZCL_PREPAY_ALARM_MANUF_ALARM_CODE_B                     0x4000U

/* Prepay Snapshot cause bitmap. */
#define ZCL_PREPAY_SNAPSHOT_CAUSE_GENERAL                       0x00000001U
#define ZCL_PREPAY_SNAPSHOT_CAUSE_END_OF_BILLING_PERIOD         0x00000002U
/* bit 2 Reserved for metering cluster */
#define ZCL_PREPAY_SNAPSHOT_CAUSE_CHANGE_OF_TARIFF_INFO         0x00000008U
#define ZCL_PREPAY_SNAPSHOT_CAUSE_CHANGE_OF_PRICE_MATRIX        0x00000010U
/* bits 5 to 9 Reserved for metering cluster */
#define ZCL_PREPAY_SNAPSHOT_CAUSE_MANUALLY_TRIGGERED            0x00000400U
/* bit 11 Reserved for metering cluster */
#define ZCL_PREPAY_SNAPSHOT_CAUSE_CHANGE_OF_TENANCY             0x00001000U
#define ZCL_PREPAY_SNAPSHOT_CAUSE_CHANGE_OF_SUPPLIER            0x00002000U
#define ZCL_PREPAY_SNAPSHOT_CAUSE_CHANGE_OF_METER_MODE          0x00004000U
/* bits 15 to 17 Reserved for metering cluster */
#define ZCL_PREPAY_SNAPSHOT_CAUSE_TOPUP_ADDITION                0x00040000U
#define ZCL_PREPAY_SNAPSHOT_CAUSE_DEBIT_CREDIT_ADDITION         0x00080000U
/* bits 20 to 31 Reserved */

#define ZCL_PREPAY_TOPUP_CODE_STR_LEN_MAX                       25U /* 26 - 1 (length byte) */
#define ZCL_PREPAY_DEBT_LABEL_LEN_MAX                           12U /* 13 - 1 (length byte) */

/** Originating Device Field enumerations */
enum ZbZclPrepayOriginDeviceT {
    ZCL_PREPAY_ORIGIN_DEVICE_ESI = 0x00, /**< Energy Service Interface */
    ZCL_PREPAY_ORIGIN_DEVICE_METER = 0x01, /**< Meter */
    ZCL_PREPAY_ORIGIN_DEVICE_IHD = 0x02 /**< In-Home Display Device */
};

/** Debt Type Field enumerations */
enum ZbZclPrepayDebtTypeT {
    ZCL_PREPAY_DEBT_TYPE_1_ABSOLUTE = 0x00, /**< Debt Type 1 Absolute */
    ZCL_PREPAY_DEBT_TYPE_1_INCREMENTAL = 0x01, /**< Debt Type 1 Incremental */
    ZCL_PREPAY_DEBT_TYPE_2_ABSOLUTE = 0x02, /**< Debt Type 2 Absolute */
    ZCL_PREPAY_DEBT_TYPE_2_INCREMENTAL = 0x03, /**< Debt Type 2 Incremental */
    ZCL_PREPAY_DEBT_TYPE_3_ABSOLUTE = 0x04, /**< Debt Type 3 Absolute */
    ZCL_PREPAY_DEBT_TYPE_3_INCREMENTAL = 0x05, /**< Debt Type 3 Incremental */
    /**< 0x06 - 0xff Reserved. */
};

/** Debt Recovery Method enumerations */
enum ZbZclPrepayDebtRecMethodT {
    ZCL_PREPAY_REC_METHOD_TIME = 0x00, /**< Time Based */
    ZCL_PREPAY_REC_METHOD_PERCENTAGE = 0x01, /**< Percentage Based */
    ZCL_PREPAY_REC_METHOD_CATCH_UP = 0x02, /**< Catch-up Based */
    /**< 0x03 -0xff Reserved */
};

/** Credit Type Field enumerations */
enum ZbZclPrepayCreditTypeT {
    ZCL_PREPAY_CREDIT_TYPE_INCREMENTAL = 0x00, /**< Credit Incremental */
    ZCL_PREPAY_CREDIT_TYPE_ABSOLUTE = 0x01, /**< Credit Absolute */
    /* 0x02 – 0xFF (Reserved) */
};

/** Prepayment Snapshot Payload type. */
enum ZbZclPrepaySnapshotPayloadTypeT {
    ZCL_PREPAY_SNAPSHOT_PAYLOAD_TYPE_DEBT_STATUS /**< Debt/Credit Status */
    /* 0x01 - 0xfe are reserved */
    /* 0xff is not used */
};

/** Result Type Field enumerations */
enum ZbZclPrepayTopUpResultT {
    ZCL_PREPAY_TOPUP_RSLT_ACCEPTED = 0x00, /**< Accepted */
    ZCL_PREPAY_TOPUP_RSLT_RJCTD_INVALID = 0x01, /**< Rejected-Invalid Top Up */
    ZCL_PREPAY_TOPUP_RSLT_RJCTD_DUPLICATE = 0x02, /**< Rejected-Duplicate Top Up */
    ZCL_PREPAY_TOPUP_RSLT_RJCTD_ERROR = 0x03, /**< Rejected-Error */
    ZCL_PREPAY_TOPUP_RSLT_RJCTD_MAX_CREDIT = 0x04, /**< Rejected-Max Credit Reached */
    ZCL_PREPAY_TOPUP_RSLT_RJCTD_KEYPAD_LOCK = 0x05, /**< Rejected-Keypad Lock */
    ZCL_PREPAY_TOPUP_RSLT_RJCTD_TOO_LARGE = 0x06, /**< Rejected-Top Up Value Too Large */
    /* Reserved (0x07 - 0x0F) */
    ZCL_PREPAY_TOPUP_RSLT_ACCPD_SUPPLY_ENABLED = 0x10, /**< Accepted – Supply Enabled */
    ZCL_PREPAY_TOPUP_RSLT_ACCPD_SUPPLY_DISABLED = 0x11, /**< Accepted – Supply Disabled */
    ZCL_PREPAY_TOPUP_RSLT_ACCPD_SUPPLY_ARMED = 0x12 /**< Accepted – Supply Armed */
};

/* Prepayment Server Cluster Commands Generated */
enum ZbZclPrepayServerCommandT {
    /* Reserved (0x00) */
    ZCL_PREPAY_SVR_CMD_PUBLISH_PREPAY_SNAPSHOT = 0x01,
    ZCL_PREPAY_SVR_CMD_CHANGE_PAYMENT_MODE_RESPONSE = 0x02,
    ZCL_PREPAY_SVR_CMD_CONSUMER_TOPUP_RESPONSE = 0x03,
    /* Reserved */
    ZCL_PREPAY_SVR_CMD_PUBLISH_TOPUP_LOG = 0x05,
    ZCL_PREPAY_SVR_CMD_PUBLISH_DEBT_LOG = 0x06
};

/* Prepayment Client Cluster Commands Generated */
enum ZbZclPrepayClientCommandT {
    ZCL_PREPAY_CLI_CMD_SELECT_AVAIL_EMERG_CREDIT = 0x00,
    /* Reserved (0x01) */
    ZCL_PREPAY_CLI_CMD_CHANGE_DEBT = 0x02,
    ZCL_PREPAY_CLI_CMD_EMERG_CREDIT_SETUP = 0x03,
    ZCL_PREPAY_CLI_CMD_CONSUMER_TOPUP = 0x04,
    ZCL_PREPAY_CLI_CMD_CREDIT_ADJUST = 0x05,
    ZCL_PREPAY_CLI_CMD_CHANGE_PAYMENT_MODE = 0x06,
    ZCL_PREPAY_CLI_CMD_GET_PREPAY_SNAPSHOT = 0x07,
    ZCL_PREPAY_CLI_CMD_GET_TOPUP_LOG = 0x08,
    ZCL_PREPAY_CLI_CMD_SET_LOW_CREDIT_WARN_LEVEL = 0x09,
    ZCL_PREPAY_CLI_CMD_GET_DEBT_REPAY_LOG = 0x0A,
    ZCL_PREPAY_CLI_CMD_SET_MAX_CREDIT_LIMIT = 0x0B,
    ZCL_PREPAY_CLI_CMD_SET_OVERALL_DEBT_CAP = 0x0C
};

/** Select Available Emergency Credit command structure  */
struct ZbZclPrepaySelectAvailEmergCreditReqT {
    uint32_t issue_time; /**< Command Issue Date/Time */
    enum ZbZclPrepayOriginDeviceT origin_device; /**< Originating Device */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Change Debt command structure  */
struct ZbZclPrepayChangeDebtReqT {
    uint32_t issuer_event_id; /**< Issuer Event ID */
    char debt_label[ZCL_PREPAY_DEBT_LABEL_LEN_MAX + 1U]; /**< Debt Label, first byte is length. */
    int32_t debt_amount; /**< Debt Amount */
    enum ZbZclPrepayDebtRecMethodT rec_method; /**< Debt Recovery Method. */
    enum ZbZclPrepayDebtTypeT debt_type; /**< Debt Amount Type. */
    uint32_t rec_start_time; /**< Debt Recovery Start Time. */
    uint16_t rec_collect_time; /**< Debt Recovery Collection Time. */
    uint8_t rec_freq; /**< Debt Recovery Frequency. */
    int32_t rec_amount; /**< Debt Recovery Amount. */
    uint16_t rec_bal_percent; /**< Debt Recovery Balance Percentage. */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Emergency Credit Setup command structure  */
struct ZbZclPrepayEmergCreditSetupReqT {
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time */
    uint32_t emerg_credit_limit; /**< Emergency Credit Limit */
    uint32_t emerg_credit_thresh; /**< Emergency Credit Threshold */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Consumer Top Up command structure */
struct ZbZclPrepayConsumerTopUpReqT {
    enum ZbZclPrepayOriginDeviceT origin_device; /**< Originating Device */
    uint8_t topup_code[ZCL_PREPAY_TOPUP_CODE_STR_LEN_MAX + 1U]; /**< TopUp Code (ZCL octet string, first byte is length) */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Credit Adjustment command structure (ZCL_PREPAY_CLI_CMD_CREDIT_ADJUST) */
struct ZbZclPrepayCreditAdjustReqT {
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time */
    enum ZbZclPrepayCreditTypeT credit_adjust_type; /**< Credit Adjustment Type */
    int32_t credit_adjust_value; /**< Credit Adjustment Value */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Change Payment Mode command structure (ZCL_PREPAY_CLI_CMD_CHANGE_PAYMENT_MODE) */
struct ZbZclPrepayChangePaymentModeReqT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t implement_time; /**< Implementation Date/Time */
    uint16_t ctrl_config; /**< Proposed Payment Control Configuration */
    int32_t cutoff_value; /**< Cut Off Value */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Get Prepay Snapshot command structure (ZCL_PREPAY_CLI_CMD_GET_PREPAY_SNAPSHOT) */
struct ZbZclPrepayGetPrepaySnapshotReqT {
    uint32_t start_time; /**< Earliest Start Time - UTC Time */
    uint32_t latest_end_time; /**< Latest End Time - UTC Time */
    uint8_t snapshot_offset; /**< Snapshot offset. */
    uint32_t snapshot_cause; /**< Snapshot cause. */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Set Low Credit Warning Level */
struct ZbZclPrepaySetLowCreditWarnLevelReqT {
    uint32_t warn_level; /**< Low Credit Warning Level */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Get Debt Repayment Log command structure */
struct ZbZclPrepayGetDebtRepayLogReqT {
    uint32_t latest_end_time; /**< Latest EndTime - UTC Time */
    uint8_t number_of_debts; /**< Number of Debts */
    enum ZbZclPrepayDebtTypeT debt_type; /**< Debt Type */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Get TopUp Log command structure */
struct ZbZclPrepayGetTopUpLogReqT {
    uint32_t latest_end_time; /**< Latest EndTime - UTC Time */
    uint8_t number_of_records; /**< Number of Records */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Set Maximum Credit Limit */
struct ZbZclPrepaySetMaxCreditLimitReqT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t implement_time; /**< Implementation Data/Time */
    uint32_t max_credit_level; /**< Maximum Credit Level */
    uint32_t max_credit_per_topup; /**< Maximum Credit Per Top Up */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Set Overall Debt Cap */
struct ZbZclPrepaySetOverallDebtCapReqT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t implement_time; /**< Implementation Data/Time */
    int32_t overall_debt_cap; /**< Overall Debt Cap */
    bool to_bomd; /**< If true, the timeout for this command is extended since it's
     * being sent to a BOMD via a Mirror device. */
};

/** Debt Payload structure */
struct ZbZclPrepayDebtRecordT {
    uint32_t collect_time; /**< Collection Time - UTC Time */
    uint32_t amount_collect; /**< Amount Collected */
    enum ZbZclPrepayDebtTypeT debt_type; /**< Debt Type */
    uint32_t outstand_debt; /**< Outstanding Debt */
};

/** Consumer Change Supply Mode Response command structure */
struct ZbZclPrepayChangePaymentModeRspT {
    uint8_t friendly_credit; /**< Friendly Credit Bitmap */
    uint32_t friendly_calendar_id; /**< Friendly Credit Calendar Id */
    uint32_t emerg_credit_limit; /**< Emergency Credit Limit */
    uint32_t emerg_credit_thresh; /**< Emergency Credit Threshold */
};

/** Consumer Top Up Response command structure */
struct ZbZclPrepayConsumerTopUpRspT {
    enum ZbZclPrepayTopUpResultT result; /**< Result Type Field enumeration */
    int32_t value; /**< Top Up Value */
    enum ZbZclPrepayOriginDeviceT source; /**< Originating Device Field enumeration */
    int32_t credit_remain; /**< Credit Remaining */
};

/* Maximum number of Debt Records that will fit into a single
 * ZCL_PREPAY_SVR_CMD_PUBLISH_DEBT_LOG command.
 * sizeof(struct ZbZclPrepayDebtRecordT) = 13 if packed */
/* #define ZCL_PREPAY_DEBT_RECORDS_PAYLOAD_MAX     ((ZCL_PAYLOAD_UNFRAG_SAFE_SIZE - 3U) / 13U) */
/* Allowing for fragmentation. This number of records may not fit in a single packet. */
#define ZCL_PREPAY_DEBT_RECORDS_PAYLOAD_MAX         8U

/** Publish Debt Log command structure */
struct ZbZclPrepayPublishDebtLogT {
    uint8_t command_index; /**< Command Index */
    /* Total number of ZCL_PREPAY_SVR_CMD_PUBLISH_DEBT_LOG
     * commands to expect, in case records don't fit into a single command. */
    uint8_t total_commands; /**< Total Number of Commands */
    /* Debt Payload. Array of debt records to include in this particular command.
     * This is not the total list indexed by command_index. */
    struct ZbZclPrepayDebtRecordT debt_records[ZCL_PREPAY_DEBT_RECORDS_PAYLOAD_MAX]; /**< Debt Payload */
    /* Number of records in debt_records to include in the payload.
     * Max allowed is ZCL_PREPAY_DEBT_RECORDS_PAYLOAD_MAX. */
    uint8_t num_records; /**< Number of records in 'debt_records' */
};

/* Currently only Snapshot Payload Type = 0x00 (Debt/Credit Status) is defined in SE 1.4 Spec. */
struct ZbZclPrepaySnapshotPayloadT {
    int32_t accum_debut; /**< Accumulated Debt */
    uint32_t type_1_remain; /**< Type 1 Debt Remaining */
    uint32_t type_2_remain; /**< Type 2 Debt Remaining */
    uint32_t type_3_remain; /**< Type 3 Debt Remaining */
    int32_t emerg_credit_remain; /**< Emergency Credit Reamining */
    int32_t credit_remain; /**< Credit Remaining */
};

struct ZbZclPrepayPublishPrepaySnapshotT {
    uint32_t snapshot_id; /**< Snapshot ID */
    uint32_t snapshot_time; /**< Snapshot Time */
    uint8_t total_snapshots; /**< Total Snapshots Found */
    uint8_t command_index; /**< Command Index */
    uint8_t total_commands; /**< Total Number of Commands */
    uint32_t snapshot_cause; /**< Snapshot Cause */
    enum ZbZclPrepaySnapshotPayloadTypeT payload_type; /**< Snapshot Payload Type */
    struct ZbZclPrepaySnapshotPayloadT payload; /**< Snapshot Payload */
};

struct ZbZclPrepayTopUpPayloadT {
    uint8_t topup_code[ZCL_PREPAY_TOPUP_CODE_STR_LEN_MAX + 1U]; /**< TopUp Code (ZCL octet string, first byte is length) */
    uint32_t topup_amount; /**< TopUp Amount */
    uint32_t topup_time; /**< TopUp Time */
};

/* sizeof(struct ZbZclPrepayTopUpPayloadT) = 33 if packed and topup_code is max length.
 * However, let's allow for more entries than if topup_code was max length. */
/* Allowing for fragmentation. This number of records may not fit in a single packet. */
#define ZCL_PREPAY_PUBLISH_TOPUP_PAYLOADS_MAX       8U

struct ZbZclPrepayPublishTopUpLogT {
    uint8_t command_index; /**< Command Index */
    uint8_t total_commands; /**< Total Number of Commands */
    struct ZbZclPrepayTopUpPayloadT topup_records[ZCL_PREPAY_PUBLISH_TOPUP_PAYLOADS_MAX]; /**< Top Up Payload */
    uint8_t num_records; /**< Number of records in 'topup_records' */
};

/*-----------------------------------------------------------------------------
 * Server API
 *-----------------------------------------------------------------------------
 */

/** Prepayment Server callbacks configuration */
struct ZbZclPrepayServerCallbacksT {
    enum ZclStatusCodeT (*select_avail_emerg_credit)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepaySelectAvailEmergCreditReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Select Available Emergency Credit command
     * (ZCL_PREPAY_CLI_CMD_SELECT_AVAIL_EMERG_CREDIT).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response.
     * If the application callback returns ZCL status of ZCL_STATUS_SUCCESS, then the stack
     * updates ZCL_PREPAY_SVR_ATTR_CREDIT_STATUS attribute as per the command. */

    enum ZclStatusCodeT (*change_debt)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayChangeDebtReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Change Debt command
     * (ZCL_PREPAY_CLI_CMD_CHANGE_DEBT).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response.
     * If the application callback returns ZCL status of ZCL_STATUS_SUCCESS, then the stack
     * updates attributes ZCL_PREPAY_SVR_ATTR_DEBT_LABEL_N(n) to
     * ZCL_PREPAY_SVR_ATTR_DEBT_REC_TOPUP_PERCENT_N(n) as per the command. */

    enum ZclStatusCodeT (*emerg_credit_setup)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayEmergCreditSetupReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Emergency Credit Setup command
     * (ZCL_PREPAY_CLI_CMD_EMERG_CREDIT_SETUP).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response.
     * If the application callback returns ZCL status of ZCL_STATUS_SUCCESS, then the stack
     * updates the attributes ZCL_PREPAY_SVR_ATTR_EMERG_CREDIT_ALLOWANCE and
     * ZCL_PREPAY_SVR_ATTR_EMERG_CREDIT_THRESHOLD, provided start_time is != 0xffffffff.
     * If start_time is 0xffffffff then previous scheduled & unactioned emerg_credit_setup
     * requests with matching 'IssuerEvent ID' will be cancelled. */

    enum ZclStatusCodeT (*consumer_topup)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayConsumerTopUpReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Consumer Top Up command.
     * Return ZCL_STATUS_SUCCESS if log(s) found and Consumer Top Up Response sent
     * (ZbZclPrepayServerConsumerTopUpRsp), or ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*credit_adjust)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayCreditAdjustReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Credit Adjustment command
     * (ZCL_PREPAY_CLI_CMD_CREDIT_ADJUST).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response.
     * If the application callback returns ZCL status of ZCL_STATUS_SUCCESS, then the stack
     * updates the attribute ZCL_PREPAY_SVR_ATTR_CREDIT_REMAINING, provided start_time
     * is != 0xffffffff. If start_time is 0xffffffff then previous scheduled & unactioned
     * credit_adjust requests with matching 'IssuerEvent ID' will be cancelled. */

    enum ZclStatusCodeT (*change_payment_mode)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayChangePaymentModeReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Change Payment Mode command
     * (ZCL_PREPAY_CLI_CMD_CHANGE_PAYMENT_MODE).
     * Returns ZCL status code from sending ZbZclPrepayServerChangePaymentModeRsp()
     * (e.g. ZCL_STATUS_SUCCESS) to include in Default Response.
     * If the application callback returns ZCL status of ZCL_STATUS_SUCCESS, then the stack
     * updates the attribute ZCL_PREPAY_SVR_ATTR_PAYMENT_CONTROL_CONFIG, provided start_time
     * is != 0xffffffff. If start_time is 0xffffffff then previous scheduled & unactioned
     * change_payment_mode requests with matching 'Provider ID' and 'IssuerEvent ID' will
     * be cancelled. */

    enum ZclStatusCodeT (*get_prepay_snapshot)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayGetPrepaySnapshotReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Prepay Snapshot command.
     * Return ZCL_STATUS_SUCCESS if snapshot(s) found and Publish Snapshot command sent
     * (ZbZclPrepayServerPublishPrepaySnapshotRsp), or ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_debt_repay_log)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayGetDebtRepayLogReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Debt Repayment Log command.
     * Return ZCL_STATUS_SUCCESS if log(s) found and Publish Debt Log command(s) sent
     * (ZbZclPrepayServerPublishDebtLogRsp), or ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_topup_log)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayGetTopUpLogReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get TopUp Log command.
     * Return ZCL_STATUS_SUCCESS if log(s) found and Publish TopUp Log command(s) sent
     * (ZbZclPrepayServerPublishTopUpLogRsp), or ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*set_low_credit_warn_level)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepaySetLowCreditWarnLevelReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Set Low Credit Warning Level command
     * (ZCL_PREPAY_CLI_CMD_SET_LOW_CREDIT_WARN_LEVEL).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response.
     * If the application callback returns ZCL status of ZCL_STATUS_SUCCESS, then the stack
     * updates the attributes ZCL_PREPAY_SVR_ATTR_LOW_CREDIT_WARNING and
     * ZCL_PREPAY_SVR_ATTR_CREDIT_REMAINING. */

    enum ZclStatusCodeT (*set_max_credit_limit)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepaySetMaxCreditLimitReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Set Max Credit Limit command
     * (ZCL_PREPAY_CLI_CMD_SET_MAX_CREDIT_LIMIT).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response.
     * If the application callback returns ZCL status of ZCL_STATUS_SUCCESS, then the stack
     * updates the attributes ZCL_PREPAY_SVR_ATTR_MAX_CRED_LIMIT and
     * ZCL_PREPAY_SVR_ATTR_MAX_CRED_PER_TOPUP, provided start_time is != 0xffffffff.
     * If start_time is 0xffffffff then previous scheduled & unactioned set_max_credit_limit
     * requests with matching 'Provider ID' and 'IssuerEvent ID' will be cancelled. */

    enum ZclStatusCodeT (*set_overall_debt_cap)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepaySetOverallDebtCapReqT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Set Overall Debt Cap command
     * (ZCL_PREPAY_CLI_CMD_SET_OVERALL_DEBT_CAP).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response.
     * If the application callback returns ZCL status of ZCL_STATUS_SUCCESS, then the stack
     * updates the attribute ZCL_PREPAY_SVR_ATTR_OVERALL_DEBT_CAP, provided start_time
     * is != 0xffffffff. If start_time is 0xffffffff then previous scheduled & unactioned
     * set_overall_debt_cap requests with matching 'Provider ID' and 'IssuerEvent ID' will
     * be cancelled. */
};

/**
 * Create a new instance of the Prepayment Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param time_server Pointer to time server cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPrepayServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclClusterT *time_server, struct ZbZclPrepayServerCallbacksT *callbacks, void *arg);

/**
 * Set/Clear the credit status flag in 'Credit Status' attribute.
 * @param cluster Cluster instance for Prepayment server.
 * @param flag Credit status flags e.g, ZCL_PREPAY_CREDIT_OK
 * @param set if true, sets the input flags. Otherwise, clears the flags.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerUpdateCreditStatusFlag(struct ZbZclClusterT *cluster, uint8_t flag, bool set);

/**
 * Set/Clear the alram status flag in 'Alarm Status' attribute.
 * @param cluster Cluster instance for Prepayment server.
 * @param flag Alarm status flags e.g, ZCL_PREPAY_ALARM_LOW_CREDIT_WARN
 * @param set if true, sets the input flags. Otherwise, clears the flags.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerUpdateAlarmStatusFlag(struct ZbZclClusterT *cluster, uint16_t flag, bool set);

/**
 * Prepayment server default write callback for optional attribute 'Credit Remaining'.
 * @param cluster Cluster instance for Prepayment server.
 * @param info Attribute information structure.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerAttrCreditRemainingWriteCb(struct ZbZclClusterT *cluster,
    struct ZbZclAttrCbInfoT *info);

/**
 * Send a Change Payment Mode response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param info Change Payment Mode Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerChangePaymentModeRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPrepayChangePaymentModeRspT *info);

/**
 * Send a Consumer Top Up response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param info Consumer Top Up Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerConsumerTopUpRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPrepayConsumerTopUpRspT *info);

/**
 * Send a Publish Prepay Snapshot as a response to the GetPrepaySnapshot command.
 * @param cluster Cluster instance from which to send this command
 * @param dstInfo Destination address for response, including sequence number and tx options
 * @param info Publish Prepay Snapshot command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerPublishPrepaySnapshotRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclPrepayPublishPrepaySnapshotT *info);

/**
 * Send a Publish Prepay Snapshot as an unsolicited command
 * @param cluster Cluster instance from which to send this command
 * @param dstInfo Destination address for response, including sequence number and tx options
 * @param info Publish Prepay Snapshot command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerPublishPrepaySnapshotUnsolic(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclPrepayPublishPrepaySnapshotT *info, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Top Up Log as a response
 * @param cluster Cluster instance from which to send this command
 * @param dstInfo Destination address for response, including sequence number and tx options
 * @param info Publish Top Up Log command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerPublishTopUpLogRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dstInfo,
    struct ZbZclPrepayPublishTopUpLogT *info);

/**
 * Send a Publish Top Up Log as an unsolicited command
 * @param cluster Cluster instance from which to send this command
 * @param dstInfo Destination address for response, including sequence number and tx options
 * @param info Publish Top Up Log command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerPublishTopUpLogUnsolic(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclPrepayPublishTopUpLogT *info, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Debt Log Response
 * @param cluster Cluster instance from which to send this command
 * @param dstInfo Destination address for response, including sequence number and tx options
 * @param info Publish Debt Log command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerPublishDebtLogRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclPrepayPublishDebtLogT *info);

/**
 * Send an unsolicited Publish Debt Log
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for this unsolicted command.
 * @param info Publish Debt Log command structure.
 * @param callback Callback function that will be invoked when the Default Response is received, or times-out.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayServerPublishDebtLogUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepayPublishDebtLogT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Helper function to check if the topup code received is duplicate or not.
 * @param cluster Cluster instance from which to send this command
 * @param topup_code Pointer to topup_code received as part of comsumer topup command.
 * @return true if the received topup code is duplicate, false otherwise.
 */
bool zcl_prepay_topup_code_duplicate_check(struct ZbZclClusterT *cluster,
    const uint8_t *topup_code);

/**
 * Helper function to update the prepayment server topup attributes on receiving the
 * consumer topup command.
 * @param cluster Cluster instance to check the topup attributes.
 * @param origin_device Origin device of the consumer topup command.
 * @param topup_code Pointer to topup code received as part of consumer topup command.
 * @param topup_value topup amount derived from the topup code.
 * @return ZCL_PREPAY_TOPUP_RSLT_ACCEPTED if the topup attributes are updated successfully,
 * ZCL_PREPAY_TOPUP_RSLT_RJCTD_MAX_CREDIT if the top amount results in credit remaining
 * value exceeding the maximum credit limit.
 */
enum ZbZclPrepayTopUpResultT zcl_prepay_update_topup_attrs(struct ZbZclClusterT *cluster,
    enum ZbZclPrepayOriginDeviceT origin_device, uint8_t *topup_code,
    int32_t topup_value);

/**
 * Helper function to retrieve the topup record from the topup attribute set pointed
 * by attribute index.
 * @param cluster Cluster instance to get the topup record.
 * @param payload Pointer to TopUp record payload.
 * @param attr_idx Attribute index to the TopUp attribute set
 * @return true if the top up record is retrieved successfully, false otherwise.
 */
bool zcl_prepay_get_topup_record(struct ZbZclClusterT *cluster,
    struct ZbZclPrepayTopUpPayloadT *payload, uint8_t attr_idx);

/*-----------------------------------------------------------------------------
 * Client API
 *-----------------------------------------------------------------------------
 */

struct ZbZclPrepayClientCallbacksT {
    enum ZclStatusCodeT (*publish_prepay_snapshot)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayPublishPrepaySnapshotT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Prepay Snapshot command
     * (ZCL_PREPAY_SVR_CMD_PUBLISH_PREPAY_SNAPSHOT).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response. */

    enum ZclStatusCodeT (*publish_topup_log)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayPublishTopUpLogT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Top Up Log command
     * (ZCL_PREPAY_SVR_CMD_PUBLISH_TOPUP_LOG).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response. */

    enum ZclStatusCodeT (*publish_debt_log)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPrepayPublishDebtLogT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Debt Log command
     * (ZCL_PREPAY_SVR_CMD_PUBLISH_DEBT_LOG).
     * Return ZCL status code (e.g. ZCL_STATUS_SUCCESS) to include in Default Response. */
};

/**
 * Create a new instance of the Prepayment Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPrepayClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPrepayClientCallbacksT *callbacks, void *arg);

/**
 * Send a Select Available Emergency Credit command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Select Available Emergency Credit command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientSelectAvailEmergCreditReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepaySelectAvailEmergCreditReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Change Debt command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Change Debt command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientChangeDebtReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepayChangeDebtReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Emergency Credit Setup command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Emergency Credit Setup command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientEmergCreditSetupReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepayEmergCreditSetupReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Consumer Top Up command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Consumer Top Up command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientConsumerTopUpReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepayConsumerTopUpReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Credit Adjustment command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Credit Adjustment command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientCreditAdjustReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepayCreditAdjustReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Change Payment Mode command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Change Payment Mode command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientChangePaymentModeReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepayChangePaymentModeReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Prepay Snapshot command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Get Prepay Snapshot command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientGetPrepaySnapshotReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepayGetPrepaySnapshotReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Debt Repayment Log command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Get Debt Repayment Log command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientGetDebtRepayLogReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepayGetDebtRepayLogReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get TopUp Log command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Get TopUp Log command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientGetTopUpLogReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepayGetTopUpLogReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Set Low Credit Warning Level command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Set Low Credit Warning Level command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientSetLowCreditWarnLevelReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepaySetLowCreditWarnLevelReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Set Max Credit Limit command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Set Max Credit Limit command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientSetMaxCreditLimitReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepaySetMaxCreditLimitReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Set Overall Debt Cap command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param info Set Overall Debt Cap command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPrepayClientSetOverallDebtCapReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPrepaySetOverallDebtCapReqT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*-----------------------------------------------------------------------------
 * Mirror Server
 *-----------------------------------------------------------------------------
 */

/**
 * Allocates a PrePayment Server Mirror cluster. The caller (application) must
 *  attach any attributes to the cluster you want to mirror. By default, stack
 *  attaches the mandatory prepayment server attributes.
 * @param cluster Prepayment client cluster instance
 * @param endpoint Endpoint on which to create the mirror cluster
 * @param meter_mirror Pointer to Metering Mirror cluster. This is used to queue
 *  any commands that need to be forwarded to the BOMD when it wakes up.
 * @param bomd_ext_addr Extended address of the BOMD device being mirrored.
 * @param bomd_ep Endpoint of the BOMD device being mirrored.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPrepayClientCreateMirror(struct ZbZclClusterT *cluster, uint8_t endpoint,
    struct ZbZclClusterT *meter_mirror, uint64_t bomd_ext_addr, uint8_t bomd_ep);

#endif
