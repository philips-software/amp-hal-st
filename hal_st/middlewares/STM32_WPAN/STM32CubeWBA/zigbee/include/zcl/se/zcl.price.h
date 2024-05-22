/**
 * @file zcl.price.h
 * @heading Price
 * @brief ZCL Price cluster header
 * ZCL 7 section 10.2
 * ZCL 8 section 10.2
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 */

/* EXEGIN - removed '@'PICS escape sequence, since these are not the
 * official PICS codes. These were added with the assumption that
 * SE clusters are being brought into the Zigbee 3.0 fold and will
 * use the same PICS codes and PICS format style. However, this has
 * yet to occur. */

/* ZCL.Price
 * SEPR.S | Server | True
 * SEPR.C | Client | True
 *
 * Server Attributes
 * SEPR.S.A0000 | Tier(1-48)PriceLabel | False | Optional
 * SEPR.S.A0100 | Block(1-48)Threshold | False | Optional
 * SEPR.S.A0200 | StartofBlockPeriod | False | Optional
 * SEPR.S.A0201 | BlockPeriodDuration | False | Optional
 * SEPR.S.A0300 | CommodityType | False | Optional
 * SEPR.S.A0301 | StandingCharge | False | Optional
 * SEPR.S.A0400 | NoTierBlock(1-6)Price | False | Optional
 * SEPR.S.A0410 | Tier(1-15)Block(1-16)Price | False | Optional
 * SEPR.S.A0700 | CurrentBillingPeriodStart | False | Optional
 * SEPR.S.A0701 | CurrentBillingPeriodDuration | False | Optional
 * SEPR.S.A0702 | LastBillingPeriodStart | False | Optional
 * SEPR.S.A0703 | LastBillingPeriodDuration | False | Optional
 * SEPR.S.A0704 | LastBillingPeriodConsolidatedBill | False | Optional
 * SEPR.S.Afffd | ClusterRevision | True
 * SEPR.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SEPR.S.C00.Rsp | Get Current Price | False
 * SEPR.S.C01.Rsp | Get Scheduled Prices | False
 * SEPR.S.C02.Rsp | Price Acknowledgement | False
 * SEPR.S.C03.Rsp | Get Block Period(s) | False
 * SEPR.S.C04.Rsp | GetConversionFactor | False
 * SEPR.S.C05.Rsp | GetCalorificValue | False
 * SEPR.S.C06.Rsp | GetTariffInformation | False
 * SEPR.S.C07.Rsp | GetPriceMatrix | False
 * SEPR.S.C08.Rsp | GetBlockThresholds | False
 * SEPR.S.C09.Rsp | GetCO2Value | False
 * SEPR.S.C0a.Rsp | GetTierLabels | False
 * SEPR.S.C0b.Rsp | GetBillingPeriod | False
 * SEPR.S.C0c.Rsp | GetConsolidatedBill | False
 * SEPR.S.C0d.Rsp | CPPEventResponse | False
 * SEPR.S.C0e.Rsp | GetCreditPayment | False
 * SEPR.S.C0f.Rsp | GetCurrencyConversion | False
 * SEPR.S.C10.Rsp | GetTariffCancellation | False
 *
 * Commands Generated
 * SEPR.S.C00.Tx | Publish Price | True
 * SEPR.S.C01.Tx | Publish Block Period | False
 * SEPR.S.C02.Tx | Publish Conversion Factor | False
 * SEPR.S.C03.Tx | Publish Calorific Value | False
 * SEPR.S.C04.Tx | PublishTariffInformation | True
 * SEPR.S.C05.Tx | PublishPriceMatrix | True
 * SEPR.S.C06.Tx | PublishBlockThresholds | True
 * SEPR.S.C07.Tx | PublishCO2Value | False
 * SEPR.S.C08.Tx | PublishTierLabels | False
 * SEPR.S.C09.Tx | PublishBillingPeriod | False
 * SEPR.S.C0a.Tx | PublishConsolidatedBill | False
 * SEPR.S.C0b.Tx | PublishCPPEvent | False
 * SEPR.S.C0c.Tx | PublishCreditPayment | False
 * SEPR.S.C0d.Tx | PublishCurrencyConversion | False
 * SEPR.S.C0e.Tx | CancelTariff | False
 *
 * Client Attributes
 * SEPR.C.A0000 | PriceIncreaseRandomizeMinutes | False | Optional
 * SEPR.C.A0001 | PriceDecreaseRandomizeMinutes | False | Optional
 * SEPR.C.A0002 | CommodityType | False | Optional
 * SEPR.C.Afffd | ClusterRevision | True
 * SEPR.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SEPR.C.C00.Rsp | Publish Price | False
 * SEPR.C.C01.Rsp | Publish Block Period | False
 * SEPR.C.C02.Rsp | Publish Conversion Factor | False
 * SEPR.C.C03.Rsp | Publish Calorific Value | False
 * SEPR.C.C04.Rsp | PublishTariffInformation | False
 * SEPR.C.C05.Rsp | PublishPriceMatrix | False
 * SEPR.C.C06.Rsp | PublishBlockThresholds | False
 * SEPR.C.C07.Rsp | PublishCO2Value | False
 * SEPR.C.C08.Rsp | PublishTierLabels | False
 * SEPR.C.C09.Rsp | PublishBillingPeriod | False
 * SEPR.C.C0a.Rsp | PublishConsolidatedBill | False
 * SEPR.C.C0b.Rsp | PublishCPPEvent | False
 * SEPR.C.C0c.Rsp | PublishCreditPayment | False
 * SEPR.C.C0d.Rsp | PublishCurrencyConversion | False
 * SEPR.C.C0e.Rsp | CancelTariff | False
 *
 * Commands Generated
 * SEPR.C.C00.Tx | Get Current Price | True
 * SEPR.C.C01.Tx | Get Scheduled Prices | True
 * SEPR.C.C02.Tx | Price Acknowledgement | True
 * SEPR.C.C03.Tx | Get Block Period(s) | False
 * SEPR.C.C04.Tx | GetConversionFactor | False
 * SEPR.C.C05.Tx | GetCalorificValue | False
 * SEPR.C.C06.Tx | GetTariffInformation | True
 * SEPR.C.C07.Tx | GetPriceMatrix | True
 * SEPR.C.C08.Tx | GetBlockThresholds | True
 * SEPR.C.C09.Tx | GetCO2Value | False
 * SEPR.C.C0a.Tx | GetTierLabels | False
 * SEPR.C.C0b.Tx | GetBillingPeriod | False
 * SEPR.C.C0c.Tx | GetConsolidatedBill | False
 * SEPR.C.C0d.Tx | CPPEventResponse | False
 * SEPR.C.C0e.Tx | GetCreditPayment | False
 * SEPR.C.C0f.Tx | GetCurrencyConversion | False
 * SEPR.C.C10.Tx | GetTariffCancellation | False
 */

#ifndef ZCL_PRICE_H
# define ZCL_PRICE_H

#include "zcl/zcl.h"
#include "zcl/se/zcl.meter.h" /* ZbZclMeterUnitsT */

/** Price Server Attribute IDs */
enum ZbZclPriceSvrAttrT {
    /* Tier Label (Delivered) Set (0x00) */
    ZCL_PRICE_SVR_ATTR_TIER1_LABEL = 0x0000,
    /**< Tier1PriceLabel (Optional)
     * ZCL_PRICE_SVR_ATTR_TIERN_LABEL(1)
     * For all the tiers, use the ZCL_PRICE_SVR_ATTR_TIERN_LABEL(tier) macro. */

    /* Block Threshold (Delivered) Set (0x01) */
    ZCL_PRICE_SVR_ATTR_BLOCK1_THRESHOLD = 0x0100,
    /**< Block1Threshold (Optional)
     * ZCL_PRICE_SVR_ATTR_BLOCKN_THRESHOLD(1)
     * For all the blocks, use the ZCL_PRICE_SVR_ATTR_BLOCKN_THRESHOLD(block) macro.
     * For all tiers and blocks, use the ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_THRESHOLD(tier, block) macro */

    ZCL_PRICE_SVR_ATTR_BLOCK_THRESHOLD_COUNT = 0x010F,
    /**< BlockThresholdCount (Optional)
     * ZCL_PRICE_SVR_ATTR_TIERN_BLOCK_THRESHOLD_COUNT(0)
     * For all the tiers, use the ZCL_PRICE_SVR_ATTR_TIERN_BLOCK_THRESHOLD_COUNT(tier) macro. */

    /* Block Period (Delivered) Set (0x02) */
    ZCL_PRICE_SVR_ATTR_START_OF_BLOCK_PERIOD = 0x0200, /**< StartofBlockPeriod (Optional) */
    ZCL_PRICE_SVR_ATTR_BLOCK_PERIOD_DURATION = 0x0201, /**< BlockPeriodDuration (Optional) */
    ZCL_PRICE_SVR_ATTR_THRESHOLD_MULTIPLIER = 0x0202, /**< ThresholdMultiplier (Optional) */
    ZCL_PRICE_SVR_ATTR_THRESHOLD_DIVISOR = 0x0203, /**< ThresholdDivisor (Optional) */
    ZCL_PRICE_SVR_ATTR_BLOCK_PERIOD_DURATION_TYPE = 0x0204, /**< BlockPeriodDurationType (Optional) */

    /* Commodity Set (0x03) */
    ZCL_PRICE_SVR_ATTR_COMMODITY_TYPE = 0x0300, /**< CommodityType (Optional) */
    ZCL_PRICE_SVR_ATTR_STANDING_CHARGE = 0x0301, /**< StandingCharge (Optional) */
    ZCL_PRICE_SVR_ATTR_CONVERSION_FACTOR = 0x0302, /**< ConversionFactor (Optional) */
    ZCL_PRICE_SVR_ATTR_CONVERSION_FACTOR_TRAILING_DIGIT = 0X0303, /**< ConversionFactorTrailingDigit (Optional) */
    ZCL_PRICE_SVR_ATTR_CALORIFIC_VALUE = 0x0304, /**< CalorificValue (Optional) */
    ZCL_PRICE_SVR_ATTR_CALORIFIC_VALUE_UNIT = 0x0305, /**< CalorificValueUnit (Optional) */
    ZCL_PRICE_SVR_ATTR_CALORIFIC_VALUE_TRAILING_DIGIT = 0x0306, /**< CalorificValueTrailingDigit (Optional) */

    /* Block Price Information (Delivered) Set (0x04) */
    ZCL_PRICE_SVR_ATTR_NO_TIER_BLOCK1_PRICE = 0x0400,
    /**< NoTierBlock1Price (Optional)
     * ZCL_PRICE_SVR_ATTR_NO_TIER_BLOCKN_PRICE(1)
     * For all the blocks, use the ZCL_PRICE_SVR_ATTR_NO_TIER_BLOCKN_PRICE(block) macro. */

    ZCL_PRICE_SVR_ATTR_TIER1_BLOCK1_PRICE = 0x0401,
    /**< Tier1Block1Price (Optional)
     * ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(1,1)
     * For all the tiers and blocks, use the ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(tier, block) macro. */

    /* Extended Price Information (Delivered) Set (0x05) */
    ZCL_PRICE_SVR_ATTR_PRICE_TIER16 = 0x050F,
    /**< PriceTier16 (Optional)
     * ZCL_PRICE_SVR_ATTR_PRICE_TIERN(16)
     * For all the tiers, use the ZCL_PRICE_SVR_ATTR_PRICE_TIERN(tier) macro. */
    ZCL_PRICE_SVR_ATTR_CPP1_PRICE = 0X05FE, /**< CPP1 Price (Optional) */
    ZCL_PRICE_SVR_ATTR_CPP2_PRICE = 0X05FF, /**< CPP2 Price (Optional) */

    /* Tariff Information (Delivered) Attribute Set (0x06) */
    ZCL_PRICE_SVR_ATTR_TARIFF_LABEL = 0x0610, /**< TariffLabel (Optional) */
    ZCL_PRICE_SVR_ATTR_NO_PRICE_TIERS_IN_USE = 0x0611, /**< NumberOfPriceTiersInUse (Optional) */
    ZCL_PRICE_SVR_ATTR_NO_BLOCK_THRESH_IN_USE = 0x0612, /**< NumberOfBlockThresholdsInUse (Optional) */
    ZCL_PRICE_SVR_ATTR_TIER_BLOCK_MODE = 0x0613, /**< TierBlockMode (Optional) */
    ZCL_PRICE_SVR_ATTR_UNIT_OF_MEASURE = 0x0615, /**< UnitOfMeasure (Optional) */
    ZCL_PRICE_SVR_ATTR_CURRENCY = 0x0616, /**< Currency (Optional) */
    ZCL_PRICE_SVR_ATTR_PRICE_TRAILING_DIGIT = 0x0617, /**< PriceTrailingDigit (Optional) */
    ZCL_PRICE_SVR_ATTR_TARIFF_RESLTN_PERIOD = 0x0619, /**< TariffResolutionPeriod (Optional) */
    ZCL_PRICE_SVR_ATTR_CO2 = 0X0620, /**< CO2Value (Optional) */
    ZCL_PRICE_SVR_ATTR_CO2_UNIT = 0X0621, /**< CO2ValueUnit (Optional) */
    ZCL_PRICE_SVR_ATTR_CO2_TRAILING_DIGIT = 0X0622, /**< CO2TrailingDigit (Optional) */

    /* Billing Information (Delivered) Attribute Set (0x07) */
    ZCL_PRICE_SVR_ATTR_CBP_START = 0x0700, /**< CurrentBillingPeriodStart (Optional) */
    ZCL_PRICE_SVR_ATTR_CBP_DURATION = 0x0701, /**< CurrentBillingPeriodDuration (Optional) */
    ZCL_PRICE_SVR_ATTR_LBP_START = 0x0702, /**< LastBillingPeriodStart (Optional) */
    ZCL_PRICE_SVR_ATTR_LBP_DURATION = 0x0703, /**< LastBillingPeriodDuration (Optional) */
    ZCL_PRICE_SVR_ATTR_LBP_CONSOLIDATED_BILL = 0x0704, /**< LastBillingPeriodConsolidatedBill (Optional) */

    /* Credit Payment Attribute Set (0x08) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_DUE_DATE = 0x0800, /**< CreditPaymentDueDate (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_STATUS = 0x0801, /**< CreditPaymentStatus (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_OVERDUE_AMT = 0x0802, /**< CreditPaymentOverdueAmount (Optional) */
    ZCL_PRICE_SVR_ATTR_PAYMENT_DISCOUNT = 0x080A, /**< PaymentDiscount (Optional) */
    ZCL_PRICE_SVR_ATTR_PAYMENT_DISCOUNT_PERIOD = 0x080B, /**< PaymentDiscountPeriod (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_1 = 0x0810, /**< CreditPayment#1 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_DATE_1 = 0x0811, /**< CreditPaymentDate#1 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_REF_1 = 0x0812, /**< CreditPaymentRef#1 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_2 = 0x0820, /**< CreditPayment#2 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_DATE_2 = 0x0821, /**< CreditPaymentDate#2 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_REF_2 = 0x0822, /**< CreditPaymentRef#2 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_3 = 0x0830, /**< CreditPayment#3 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_DATE_3 = 0x0831, /**< CreditPaymentDate#3 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_REF_3 = 0x0832, /**< CreditPaymentRef#3 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_4 = 0x0840, /**< CreditPayment#4 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_DATE_4 = 0x0841, /**< CreditPaymentDate#4 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_REF_4 = 0x0842, /**< CreditPaymentRef#4 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_5 = 0x0850, /**< CreditPayment#5 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_DATE_5 = 0x0851, /**< CreditPaymentDate#5 (Optional) */
    ZCL_PRICE_SVR_ATTR_CRDT_PAYMENT_REF_5 = 0x0852, /**< CreditPaymentRef#5 (Optional) */

    /* Received Tier Label Set (0x80) */
    ZCL_PRICE_SVR_ATTR_RECV_TIER1_LABEL = 0x8000,
    /**< ReceivedTier1PriceLabel
     * ZCL_PRICE_SVR_ATTR_RECV_TIERN_LABEL(1)
     * For all the tiers, use the ZCL_PRICE_SVR_ATTR_RECV_TIERN_LABEL(tier) macro. */

    /* Received Block Threshold Set (0x81) */
    ZCL_PRICE_SVR_ATTR_RECV_BLOCK1_THRESHOLD = 0x8100,
    /**< ReceivedBlock1Threshold
     * ZCL_PRICE_SVR_ATTR_RECV_BLOCKN_THRESHOLD(1)
     * For all the blocks, use the ZCL_PRICE_SVR_ATTR_RECV_BLOCKN_THRESHOLD(block) macro. */

    /* Received Block Period Set (0x82) */
    ZCL_PRICE_SVR_ATTR_RECV_START_OF_BLOCK_PERIOD = 0x8200, /**< ReceivedStartofBlockPeriod (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_BLOCK_PERIOD_DURATION = 0x8201, /**< ReceivedBlockPeriodDuration (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_THRESHOLD_MULTIPLIER = 0x8202, /**< ReceivedThresholdMultiplier (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_THRESHOLD_DIVISOR = 0x8203, /**< ReceivedThresholdDivisor (Optional) */

    /* Received Block Price Information Set (0x84) */
    ZCL_PRICE_SVR_ATTR_RECV_NO_TIER_BLOCK1_PRICE = 0x8400,
    /**< RxNoTierBlock1Price
     * ZCL_PRICE_SVR_ATTR_RECV_NO_TIER_BLOCKN_PRICE(1)
     * For all the blocks, use the ZCL_PRICE_SVR_ATTR_RECV_NO_TIER_BLOCKN_PRICE(block) macro. */

    ZCL_PRICE_SVR_ATTR_RECV_TIER1_BLOCK1_PRICE = 0x8410,
    /**< RxTier1Block1Price
     * ZCL_PRICE_SVR_ATTR_RECV_TIERN_BLOCKN_PRICE(1,1)
     * For all the tiers and blocks, use the ZCL_PRICE_SVR_ATTR_RECV_TIERN_BLOCKN_PRICE(tier, block) macro. */

    /* Received Extended Price Information Set (0x85) */
    ZCL_PRICE_SVR_ATTR_RECV_PRICE_TIER16 = 0x850F,
    /**< RxPriceTier16
     * ZCL_PRICE_SVR_ATTR_RECV_PRICE_TIERN(16)
     * For all the tiers, use the ZCL_PRICE_SVR_ATTR_RECV_PRICE_TIERN(tier) macro. */

    /* Received Tariff Information Set (0x86) */
    ZCL_PRICE_SVR_ATTR_RECV_TARIFF_LABEL = 0x8610, /**< ReceivedTariffLabel (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_NO_PRICE_TIERS_IN_USE = 0x8611, /**< ReceivedNumberOfPriceTiersInUse (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_NO_BLOCK_THRESH_IN_USE = 0x8612, /**< ReceivedNumberOfBlockThresholdsInUse (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_TIER_BLOCK_MODE = 0x8613, /**< ReceivedTierBlockMode (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_TARIFF_RESLTN_PERIOD = 0x8615, /**< ReceivedTariffResolutionPeriod (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_CO2 = 0X8625, /**< ReceivedCO2Value (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_CO2_UNIT = 0X8626, /**< ReceivedCO2ValueUnit (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_CO2_TRAILING_DIGIT = 0X8627, /**< ReceivedCO2TrailingDigit (Optional) */

    /* Received Billing Information Set (0x87) */
    ZCL_PRICE_SVR_ATTR_RECV_CBP_START = 0x8700, /**< ReceivedCurrentBillingPeriodStart (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_CBP_DURATION = 0x8701, /**< ReceivedCurrentBillingPeriodDuration (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_LBP_START = 0x8702, /**< ReceivedLastBillingPeriodStart (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_LBP_DURATION = 0x8703, /**< ReceivedLastBillingPeriodDuration (Optional) */
    ZCL_PRICE_SVR_ATTR_RECV_LBP_CONSOLIDATED_BILL = 0x8704, /**< ReceivedLastBillingPeriodConsolidatedBill (Optional) */
};

/* Price Cluster Attribute identifiers. */
/* Tiers start at 1 and go to a max of 48. */
#define ZCL_PRICE_SVR_ATTR_TIERN_LABEL(tier)                           (0x0000U + ((tier) - 1U))
#define ZCL_PRICE_SVR_ATTR_BLOCKN_THRESHOLD(block)                     (0x0100U + ((block) - 1U))
#define ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_THRESHOLD(tier, block)         (0x0100U + ((block) - 1U) + (0x10U * (tier)))
#define ZCL_PRICE_SVR_ATTR_TIERN_BLOCK_THRESHOLD_COUNT(tier)           (0x010FU + (0x10U * (tier)))

#define ZCL_PRICE_SVR_ATTR_PRICE_TIERN(tier)                           (0x0500U + ((tier) - 1U))

#define ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(tier, block)             (0x0400U + ((block) - 1U) + (0x10U * (tier)))
#define ZCL_PRICE_SVR_ATTR_NO_TIER_BLOCKN_PRICE(block)                 ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(0U, block)

#define ZCL_PRICE_SVR_ATTR_RECV_TIERN_LABEL(tier)                      (0x8000U + ((tier) - 1U))
#define ZCL_PRICE_SVR_ATTR_RECV_BLOCKN_THRESHOLD(block)                (0x8100U + ((block) - 1U))
#define ZCL_PRICE_SVR_ATTR_RECV_NO_TIER_BLOCKN_PRICE(block)            (0x8400U + ((block) - 1U))
#define ZCL_PRICE_SVR_ATTR_RECV_TIERN_BLOCKN_PRICE(tier, block)        (0x8400U + ((block) - 1U) + (0x10U * (tier)))
#define ZCL_PRICE_SVR_ATTR_RECV_PRICE_TIERN(tier)                      (0x8500U + ((tier) - 1U))

/* Price Server Generated Commands */
enum ZbZclPriceServerCommandT {
    ZCL_PRICE_SVR_CMD_PUB_PRICE = 0x00, /* mandatory for GBCS ESI */
    ZCL_PRICE_SVR_CMD_PUB_BLK_PERIOD = 0x01,
    ZCL_PRICE_SVR_CMD_PUB_CONVERSION_FACTOR = 0x02,
    ZCL_PRICE_SVR_CMD_PUB_CALORIFIC_VALUE = 0x03,
    ZCL_PRICE_SVR_CMD_PUB_TARIFF_INFORMATION = 0x04, /* mandatory for GBCS ESI */
    ZCL_PRICE_SVR_CMD_PUB_PRICE_MATRIX = 0x05, /* mandatory for GBCS ESI */
    ZCL_PRICE_SVR_CMD_PUB_BLK_THRESHOLDS = 0x06, /* mandatory for GBCS ESI */
    ZCL_PRICE_SVR_CMD_PUB_CO2_VALUE = 0x07,
    ZCL_PRICE_SVR_CMD_PUB_TIER_LABELS = 0x08,
    ZCL_PRICE_SVR_CMD_PUB_BILLING_PERIOD = 0x09,
    ZCL_PRICE_SVR_CMD_PUB_CONSOLIDATED_BILL = 0x0a,
    ZCL_PRICE_SVR_CMD_PUB_CPP_EVENT = 0x0b,
    ZCL_PRICE_SVR_CMD_PUB_CREDIT_PAYMENT = 0x0c,
    ZCL_PRICE_SVR_CMD_PUB_CURRENCY_CONVERSION = 0x0d,
    ZCL_PRICE_SVR_CMD_PUB_CANCEL_TARIFF = 0x0e
};

/* Price Client Generated Commands */
enum ZbZclPriceClientCommandT {
    ZCL_PRICE_CLI_CMD_GET_CURRENT_PRICE = 0x00, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_SCHEDULED_PRICES = 0x01,
    ZCL_PRICE_CLI_CMD_PRICE_ACKNOWLEDGEMENT = 0x02, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_BLOCK_PERIODS = 0x03,
    ZCL_PRICE_CLI_CMD_GET_CONVERSION_FACTOR = 0x04,
    ZCL_PRICE_CLI_CMD_GET_CALORIFIC_VALUE = 0x05,
    ZCL_PRICE_CLI_CMD_GET_TARIFF_INFORMATION = 0x06, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_PRICE_MATRIX = 0x07, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_BLOCK_THRESHOLDS = 0x08, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_CO2_VALUE = 0x09,
    ZCL_PRICE_CLI_CMD_GET_TIER_LABELS = 0x0a,
    ZCL_PRICE_CLI_CMD_GET_BILLING_PERIOD = 0x0b,
    ZCL_PRICE_CLI_CMD_GET_CONSOLIDATED_BILL = 0x0c,
    ZCL_PRICE_CLI_CMD_CPP_EVENT_RESPONSE = 0x0d,
    ZCL_PRICE_CLI_CMD_GET_CREDIT_PAYMENT = 0x0e,
    ZCL_PRICE_CLI_CMD_GET_CURRENCY_CONVERSION = 0x0f,
    ZCL_PRICE_CLI_CMD_GET_TARIFF_CANCELLATION = 0x10
};

/* Price Cluster get current flags. */
#define ZCL_PRICE_GET_FLAGS_RX_ON_IDLE           0x01U

/* Price tier label string lengths. */
#define ZCL_PRICE_LABEL_MAX_LENGTH               12U

#define ZCL_PRICE_TARIFF_LABEL_MAX_LENGTH        24U

/* Infinite price duration (until changed) */
#define ZCL_PRICE_DURATION_INFINITE              0xFFFFU

/* Infinite price duration (until changed) */
#define ZCL_PRICE_PERIOD_DURATION_INFINITE       0xFFFFFFU

/* Alternate Cost units. */
#define ZCL_PRICE_ALTERNATE_KG_CO2               0x01U

/* Currency, from ISO 4217 */
enum ZbZclPriceCurrencyCodeT {
    ZCL_PRICE_CURRENCY_CODE_AUD = 36, /* Australian dollar */
    ZCL_PRICE_CURRENCY_CODE_CAD = 124, /* Canadian dollar */
    ZCL_PRICE_CURRENCY_CODE_CNY = 156, /* Chinese yuan */
    ZCL_PRICE_CURRENCY_CODE_EUR = 978, /* Euro */
    ZCL_PRICE_CURRENCY_CODE_GBP = 826, /* Pound sterling */
    ZCL_PRICE_CURRENCY_CODE_HKD = 344, /* Hong Kong dollar */
    ZCL_PRICE_CURRENCY_CODE_INR = 356, /* Indian rupee */
    ZCL_PRICE_CURRENCY_CODE_RUB = 643, /* Russian ruble */
    ZCL_PRICE_CURRENCY_CODE_USD = 840, /* United States dollar */
    ZCL_PRICE_CURRENCY_CODE_XTS = 963, /* Code reserved for testing */
    ZCL_PRICE_CURRENCY_CODE_XXX = 999 /* No currency */
};

/* Calorific Value Unit Enumeration */
enum ZbZclPriceCalorificUnitT {
    ZCL_PRICE_CALORIFIC_UNIT_MJ_M3 = 0x01, /**< MJ/M3, MegaJoules per cubic meter */
    ZCL_PRICE_CALORIFIC_UNIT_MJ_KG = 0x02 /**< MJ/Kg, MegaJoules per kilogram */
};

/* CO2 Unit Enumeration */
enum ZbZclPriceCO2UnitT {
    /* Reserved (0x0) */
    ZCL_PRICE_CO2_UNIT_KG_KWH = 0x01, /**< Kg/kWh, Kilograms per kiloWatt-hour */
    ZCL_PRICE_CO2_UNIT_KG_GAS = 0x02, /**< Kg per gallon of gasoline */
    ZCL_PRICE_CO2_UNIT_KG_NG = 0x03, /**< Kg per therm of natural gas */
    /* Reserved (0x4 to 0xFF) */
};

/* Block Period Duration Types */
enum ZbZclPriceBlockPeriodDurationTypeT {
    ZCL_PRICE_BLOCK_PERIOD_DURATION_MINUTES = 0x00,
    ZCL_PRICE_BLOCK_PERIOD_DURATION_DAYS = 0x01,
    ZCL_PRICE_BLOCK_PERIOD_DURATION_WEEKS = 0x02,
    ZCL_PRICE_BLOCK_PERIOD_DURATION_MONTHS = 0x03,
    /* Reserved (0x4 to 0xF) */
};

enum ZbZclPriceBlockPeriodDurationControlT {
    ZCL_PRICE_BLOCK_PERIOD_START_OF_TIMEBASE = 0x00,
    ZCL_PRICE_BLOCK_PERIOD_END_OF_TIMEBASE = 0x01,
    ZCL_PRICE_BLOCK_PERIOD_UNSPECIFIED = 0x02,
    /* Reserved (0x3 to 0xF) */
};

/** Tariff Type Enumeration */
enum ZbZclPriceTariffTypeT {
    ZCL_PRICE_TARIFF_TYPE_DELIV = 0x00, /**< Delivered tariff */
    ZCL_PRICE_TARIFF_TYPE_RECV = 0x01, /**< Received tariff */
    ZCL_PRICE_TARIFF_TYPE_DELIV_RECV = 0x02, /**< Delivered and received tariff */
    /* Reserved (0x3 to 0xF) */
    ZCL_PRICE_TARIFF_TYPE_UNSPECIFIED = 0xff /**< Unspecified tariff type, meaning any tariff type. */
};

/** Tariff Charging Scheme Enumeration */
enum ZbZclPriceTariffChargingSchemeT {
    ZCL_PRICE_CHARGE_TOU_TARIFF, /**< TOU Tariff */
    ZCL_PRICE_CHARGE_BLOCK_TARIFF, /**< Block Tariff */
    ZCL_PRICE_CHARGE_BLOCK_TOU_COMM_THRESH, /**< Block/TOU Tariff with common thresholds */
    ZCL_PRICE_CHARGE_BLOCK_TOU_IND_THRESH /**< Block/TOU Tariff with individual thresholds per tier */
    /* Reserved (0x4 to 0xF) */
};

/** Tier Block Mode Enumeration */
enum ZbZclPriceTierBlockModeT {
    ZCL_PRICE_TIER_BLOCK_MODE_0 = 0x00,
    /**< This tariff employs a single set of thresholds. */
    ZCL_PRICE_TIER_BLOCK_MODE_1 = 0x01,
    /**< This tariff employs a single set of thresholds common across all TOU tiers. */
    ZCL_PRICE_TIER_BLOCK_MODE_2 = 0x02,
    /**< This combination tariff employs an individual set of Thresholds for each
     * TOU tier. */
    /* Reserved for future use (0x3 to 0xFE) */
    /* Reserved 0xFF */
};

/* Price Cluster Structures */

/* ZCL_PRICE_SVR_CMD_PUB_PRICE */
#define ZCL_PRICE_PUBLISH_MIN_SIZE               27U /* assume rate_label is only 1 byte */
#define ZCL_PRICE_PUBLISH_MAX_SIZE               59U

/* ZCL_PRICE_SVR_CMD_PUB_CALORIFIC_VALUE */
#define ZCL_PRICE_CALORIFIC_MIN_SIZE             14U

/* ZCL_PRICE_SVR_CMD_PUB_CONVERSION_FACTOR */
#define ZCL_PRICE_CONVERSION_PUB_MIN_SIZE        13U

/* ZCL_PRICE_SVR_CMD_GET_CONVERSION_FACTOR */
#define ZCL_PRICE_CONVERSION_GET_MIN_SIZE        9U

/* ZCL_PRICE_SVR_CMD_PUB_CO2_VALUE */
#define ZCL_PRICE_CO2_PUB_MIN_SIZE               19U

/* ZCL_PRICE_CLI_CMD_GET_CO2_VALUE */
#define ZCL_PRICE_CO2_GET_MIN_SIZE               9U
#define ZCL_PRICE_CO2_GET_MAX_SIZE               10U

/* ZCL_PRICE_SVR_CMD_PUB_BILLING_PERIOD */
#define ZCL_PRICE_BILLING_PUB_MIN_SIZE           17U

/* ZCL_PRICE_SVR_CMD_GET_BILLING_PERIOD */
#define ZCL_PRICE_BILLING_GET_MIN_SIZE           9U
#define ZCL_PRICE_BILLING_GET_MAX_SIZE           10U

/* ZCL_PRICE_SVR_CMD_PUB_CONSOLIDATED_BILL */
#define ZCL_PRICE_CONSOLIDATED_PUB_MIN_SIZE      24U

/* ZCL_PRICE_SVR_CMD_GET_CONSOLIDATED_BILL */
#define ZCL_PRICE_CONSOLIDATED_GET_MIN_SIZE      9U
#define ZCL_PRICE_CONSOLIDATED_GET_MAX_SIZE      10U

/* ZCL_PRICE_SVR_CMD_PUB_CURRENCY_CONVERSION */
#define ZCL_PRICE_CURRENCY_CONV_PUB_SIZE         25U

/* Table D-100 Price Control Field BitMap */
#define ZCL_PRICE_CTRL_MASK_PRICE_ACK               0x01 /* Price Acknowledgement required */
#define ZCL_PRICE_CTRL_MASK_TOTAL_TIERS_GT_15       0x02 /* Total Tiers exceeds the 15 specified in the command */

/** Publish Price command structure */
struct ZbZclPriceServerPublishPriceT {
    uint32_t provider_id; /**< Provider ID */
    char rate_label[ZCL_PRICE_LABEL_MAX_LENGTH + 1U]; /**< Rate Label array */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t current_Time; /**< Current Time - UTC Time */
    enum ZbZclMeterUnitsT unit_of_measure; /**< Unit of Measure */
    enum ZbZclPriceCurrencyCodeT currency; /**< Currency */
    uint8_t trailing_digit_and_price_tier; /**< Price Trailing Digit & Price Tier */
    uint8_t num_price_tiers; /**< Number of Price Tiers & Register Tier */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint16_t duration; /**< Duration in Minutes */
    uint32_t price; /**< Price */
    /* ZCL_PRICE_PUBLISH_MIN_SIZE */

    /* Optional parameters.
     * Value set to all 1's to indicate not present.
     * First parameter to be all 1's will cut-off the
     * response at that point. */
    uint8_t price_ratio; /**< Price Ratio (Optional) */
    uint32_t generation_price; /**< Generation Price (Optional) */
    uint8_t generation_price_ratio; /**< Generation Price Ratio (Optional) */

    uint32_t alternate_cost_delivered; /**< Alternate Cost Delivered (Optional) */
    uint8_t alternate_cost_units; /**< Alternate Cost Unit (Optional) */
    uint8_t alternate_cost_trail_digits; /**< Alternate Cost Trailing Digit (Optional) */
    uint8_t num_block_thresholds; /**< Number of Block Thresholds (Optional) */
    uint8_t price_control; /**< Price Control (Optional) */

    uint8_t number_of_gen_tiers; /**< Number of Generation Tiers (Optional) */
    uint8_t gen_tier; /**< Generation Tier (Optional) */
    uint8_t ext_number_price_tiers; /**< Extended Number of Price Tiers (Optional) */
    uint8_t ext_price_tier; /**< Extended Price Tier (Optional) */
    uint8_t ext_register_tier; /**< Extended Register Tier (Optional) */
};

#define ZCL_PRICE_PUBLISH_TARIFF_INFO_MIN_SIZE      37U /* assume tariff_label is only 1 byte */
#define ZCL_PRICE_PUBLISH_TARIFF_INFO_MAX_SIZE      61U

/** Publish Tariff Info command structure */
struct ZbZclPriceServerPublishTariffInfoT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint8_t tariff_type_and_charging_scheme; /**< Tariff Type/Charging Scheme */
    char tariff_label[ZCL_PRICE_TARIFF_LABEL_MAX_LENGTH + 1U]; /**< Tariff Label array */
    uint8_t number_of_price_tiers_in_use; /**< Number of Price Tiers in Use */
    uint8_t number_of_block_thresh_in_use; /**< Number of Block Thresholds in Use */
    uint8_t unit_of_measure; /**< Unit of Measure */
    uint16_t currency; /**< Currency */
    uint8_t price_trailing_digit; /**< Price Trailing Digit */
    uint32_t standing_charge; /**< Standing Charge */
    uint8_t tier_block_mode; /**< TierBlockMode */
    uint32_t block_thresh_multiplier; /**< Block Threshold Multiplier - 24-bit */
    uint32_t block_thresh_divisor; /**< Block Threshold Divisor - 24-bit */
};

/** Publish Conversion Factor command structure */
struct ZbZclPriceServerPublishConversionFactorT {
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint32_t conversion_factor; /**< Conversion Factor */
    uint8_t conversion_factor_trail_digits; /**< Conversion Factor Trailing Digit */
};

/** Publish Calorific Value command structure */
struct ZbZclPriceServerPublishCalorificValueT {
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint32_t calorific_value; /**< Calorific Value */
    uint8_t calorific_value_unit; /**< Calorific Value Unit */
    uint8_t calorific_value_trail_digits; /**< Calorific Value Trailing Digit */
};

/** Publish CO2 Value command structure */
struct ZbZclPriceServerPublishCO2ValueT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    enum ZbZclPriceTariffTypeT tariff_type; /**< Tariff Type */
    uint32_t co2_value; /**< CO2 Value */
    uint8_t co2_value_unit; /**< CO2 Value Unit */
    uint8_t co2_value_trail_digits; /**< CO2 Value Trailing Digit */
};

/** Publish Billing Period command structure */
struct ZbZclPriceServerPublishBillingPeriodT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Billing Period Start Time - UTC Time */
    uint32_t duration; /**< Billing Period Duration */
    uint8_t duration_type; /**< Billing Period Duration Type */
    enum ZbZclPriceTariffTypeT tariff_type; /**< Tariff Type */
};

/** Publish Consolidated Bill command structure */
struct ZbZclPriceServerPublishConsolidatedBillT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Billing Period Start Time - UTC Time */
    uint32_t duration; /**< Billing Period Duration */
    uint8_t duration_type; /**< Billing Period Duration Type */
    enum ZbZclPriceTariffTypeT tariff_type; /**< Tariff Type */
    uint32_t consolidated_bill; /**< Consolidated Bill */
    uint16_t currency; /**< Currency */
    uint8_t bill_trail_digits; /**< Bill Trailing Digit */
};

/** Publish Currency Conversion command structure */
struct ZbZclPriceServerPublishCurrencyConversionT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint16_t old_currency; /**< Old Currency */
    uint16_t new_currency; /**< New Currency */
    uint32_t conversion_factor; /**< Conversion Factor */
    uint8_t conversion_trail_digits; /**< Conversion Trailing Digit */
    uint32_t currency_change_control_flags; /**< Currency Change Control Flags */
};

/* ZCL_PRICE_SVR_CMD_PUB_PRICE_MATRIX */
#define ZCL_PRICE_SVR_PRICE_MATRIX_MIN_SIZE                 19U
#define ZCL_PRICE_SVR_PRICE_MATRIX_ENTRY_SIZE               5U
#define ZCL_PRICE_SVR_PRICE_MATRIX_NUM_SUB_PAYLOAD_MAX      ((ZCL_PAYLOAD_UNFRAG_SAFE_SIZE - ZCL_PRICE_SVR_PRICE_MATRIX_MIN_SIZE) / \
                                                             ZCL_PRICE_SVR_PRICE_MATRIX_ENTRY_SIZE)

/** Price Matrix Sub-Payload Control */
#define ZCL_PRICE_SVR_PRICE_MATRIX_CTRL_BLOCK_OR_MIXED      0x00U
#define ZCL_PRICE_SVR_PRICE_MATRIX_CTRL_TOU_ONLY            0x01U

/** Price Matrix Entry structure */
struct ZbZclPriceServerPriceMatrixEntryT {
    uint8_t tier_block_id; /**< Tier/Block ID */
    uint32_t price; /**< Price */
};

/** Publish Price Matrix command structure */
struct ZbZclPriceServerPublishPriceMatrixT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID */
    uint8_t command_index; /**< Command Index */
    uint8_t total_number_commands; /**< Total Number of Commands */
    uint8_t sub_payload_control; /**< Sub-payload Control */
    uint8_t num_sub_payload; /**< Number of items in Price Matrix Sub-payload */
    struct ZbZclPriceServerPriceMatrixEntryT sub_payload[ZCL_PRICE_SVR_PRICE_MATRIX_NUM_SUB_PAYLOAD_MAX];
    /**< Price Matrix Sub-payload */
};

/* ZCL_PRICE_SVR_CMD_PUB_BLK_THRESHOLDS */
#define ZCL_PRICE_SVR_BLOCK_THRESH_MIN_SIZE                 19U
#define ZCL_PRICE_SVR_BLOCK_THRESH_ENTRY_SIZE               7U
#define ZCL_PRICE_SVR_BLOCK_THRESH_NUM_SUB_PAYLOAD_MAX      ((ZCL_PAYLOAD_UNFRAG_SAFE_SIZE - ZCL_PRICE_SVR_BLOCK_THRESH_MIN_SIZE) / \
                                                             ZCL_PRICE_SVR_BLOCK_THRESH_ENTRY_SIZE)

/** Block Threshold Sub-Payload Control */
#define ZCL_PRICE_SVR_BLOCK_THRESH_CTRL_SPECIFIC_TOU_TIER               0x00U
#define ZCL_PRICE_SVR_BLOCK_THRESH_CTRL_ALL_TOU_TIERS_OR_BLOCK_ONLY     0x01U

/** Block Threshold Entry structure */
struct ZbZclPriceServerBlockThreshEntryT {
    uint8_t tier_num_block_thresh; /**< Tier/NumberOfBlockThresholds */
    uint64_t *block_thresh; /**< Block Threshold */
};

/** Publish Block Thresholds command structure */
struct ZbZclPriceServerPublishBlockThresholdsT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID*/
    uint8_t command_index; /**< Command Index */
    uint8_t total_number_commands; /**< Total Number of Commands */
    uint8_t sub_payload_control; /**< Sub-payload Control */
    uint8_t num_sub_payload; /**< Number of items in Block Threshold Sub-payload */
    struct ZbZclPriceServerBlockThreshEntryT sub_payload[ZCL_PRICE_SVR_BLOCK_THRESH_NUM_SUB_PAYLOAD_MAX];
    /**< Block Threshold Sub-payload */
};

/** Publish Block Period */
struct ZbZclPriceServerPublishBlockPeriodT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t block_period_start_time; /**< Block Period Start Time - UTC Time */
    uint32_t block_period_duration; /**< Block Period Duration */
    uint8_t block_period_control; /**< Block Period Control */
    uint8_t block_period_duration_type; /**< Block Period Duration Type */
    enum ZbZclPriceTariffTypeT tariff_type; /**< Tariff Type */
    uint8_t tariff_resolution_period; /**< Tariff Resolution Period */
};

/** Get Current Price command structure */
struct ZbZclPriceClientGetCurrentPriceT {
    uint8_t command_options; /**< Command Options */
};

/** Price Acknowledge command structure */
struct ZbZclPriceClientPriceAckT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t price_ack_time; /**< Price Ack Time */
    uint8_t control; /**< Control. E.g. ZCL_PRICE_CTRL_MASK_PRICE_ACK */
};

/** Get Scheduled Prices command structure */
struct ZbZclPriceClientGetScheduledPricesT {
    uint32_t startTime; /**< StartTime */
    uint8_t maxPrices; /**< NumberofEvents */
};

/** Get Calorific Value command structure*/
struct ZbZclPriceClientGetCalorificValueT {
    uint32_t earliest_start_time; /**< Earliest Start Time */
    uint32_t min_issuer_event_id; /**< Min. Issuer Event ID */
    uint8_t num_commands; /**< Number of Commands */
};

/** Get Conversion Factor command structure*/
struct ZbZclPriceClientGetConversionFactorT {
    uint32_t earliest_start_time; /**< Earliest Start Time */
    uint32_t min_issuer_event_id; /**< Min. Issuer Event ID */
    uint8_t num_commands; /**< Number of Commands */
};

/** Get Tariff Information command structure */
struct ZbZclPriceClientGetTariffInfoT {
    uint32_t earliest_start_time; /**< Earliest Start Time */
    uint32_t min_issuer_event_id; /**< Min. Issuer Event ID */
    uint8_t num_commands; /**< Number of Commands */
    enum ZbZclPriceTariffTypeT tariff_type; /**< Tariff Type */
};

/** Get Price Matrix command structure */
struct ZbZclPriceClientGetPriceMatrixT {
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID */
};

/** Get Block Thresholds command structure */
struct ZbZclPriceClientGetBlockThresholdsT {
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID */
};

/** Get CO2 Value command structure */
struct ZbZclPriceClientGetCO2ValueT {
    uint32_t earliest_start_time; /**< Earliest Start Time */
    uint32_t min_issuer_event_id; /**< Min. Issuer Event ID */
    uint8_t num_commands; /**< Number of Commands */
    enum ZbZclPriceTariffTypeT tariff_type; /**< Optional - Tariff Type */
};

/** Get Block Period command structure */
struct ZbZclPriceClientGetBlockPeriodT {
    uint32_t start_time; /**< Start Time - UTC Time */
    uint8_t num_events; /**< Number of Events requested */
    enum ZbZclPriceTariffTypeT tariff_type; /**< Tariff Type */
};

/** Get Billing Period command structure */
struct ZbZclPriceClientGetBillingPeriodT {
    uint32_t earliest_start_time; /**< Earliest Start Time */
    uint32_t min_issuer_event_id; /**< Min. Issuer Event ID */
    uint8_t num_commands; /**< Number of Commands */
    enum ZbZclPriceTariffTypeT tariff_type; /**< Optional - Tariff Type */
};

/** Get Consolidated Bill command structure */
struct ZbZclPriceClientGetConsolidatedBillT {
    uint32_t earliest_start_time; /**< Earliest Start Time */
    uint32_t min_issuer_event_id; /**< Min. Issuer Event ID */
    uint8_t num_commands; /**< Number of Commands */
    enum ZbZclPriceTariffTypeT tariff_type; /**< Optional - Tariff Type */
};

/*---------------------------------------------------------
 * Price Server
 *---------------------------------------------------------
 */

/** Price Server callbacks configuration */
struct ZbZclPriceServerCallbacksT {
    enum ZclStatusCodeT (*get_current_price)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetCurrentPriceT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Current command.
     * Return ZCL_STATUS_SUCCESS if Publish Price command sent or ZCL_STATUS_NOT_FOUND
     * otherwise. */

    enum ZclStatusCodeT (*price_ack)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientPriceAckT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Price Acknowledgement command. */

    enum ZclStatusCodeT (*get_scheduled_prices)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetScheduledPricesT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Scheduled Prices command.
     * Return ZCL_STATUS_SUCCESS if relevant Events are sent in Publish Price commands or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_tariff_info)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetTariffInfoT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Tariff Information command.
     * Return ZCL_STATUS_SUCCESS if Publish Tariff Information command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_price_matrix)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetPriceMatrixT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Price Matrix command.
     * Return ZCL_STATUS_SUCCESS if Publish Price Matrix command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_block_thresholds)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetBlockThresholdsT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Block Thresholds command.
     * Return ZCL_STATUS_SUCCESS if Publish Block Thresholds command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_calorific_value)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetCalorificValueT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Calorific Value command.
     * Return ZCL_STATUS_SUCCESS if Publish Calorific Value command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_co2_value)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetCO2ValueT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get CO2 Value command.
     * Return ZCL_STATUS_SUCCESS if Publish CO2 Value command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_conversion_factor)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetConversionFactorT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Conversion Factor command.
     * Return ZCL_STATUS_SUCCESS if Publish Conversion Factor command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_block_period)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetBlockPeriodT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Block Period command.
     * Return ZCL_STATUS_SUCCESS if Publish Block Period command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_billing_period)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetBillingPeriodT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Billing Period command.
     * Return ZCL_STATUS_SUCCESS if Publish Billing Period command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_consolidated_bill)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetConsolidatedBillT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Consolidated Bill command.
     * Return ZCL_STATUS_SUCCESS if Publish Consolidated Bill command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_currency_conversion)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Currency Conversion command.
     * Return ZCL_STATUS_SUCCESS if Publish Currency Conversion command sent or
     * ZCL_STATUS_NOT_FOUND otherwise. */

    /* Optional commands not parsed into a struct or specific callback */
    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *cluster,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
    /**< Callback to application, invoked on receipt of optional Client commands. */
};

/**
 * Create a new instance of the Price Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPriceServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPriceServerCallbacksT *callbacks, void *arg);

/**
 * Initialize Publish Price information
 * @param rsp Publish Price command structure
 * @return Void
 */
void ZbZclPriceServerPublishPriceInit(struct ZbZclPriceServerPublishPriceT *rsp);

/**
 * Send an unsolicited Publish Price command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Price command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishPriceUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishPriceT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Price command as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param info Publish Price command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishPriceRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishPriceT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish Tariff Information command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Tariff Info command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishTariffInfoUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishTariffInfoT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* EXEGIN - need description */
enum ZclStatusCodeT ZbZclPriceServerSendPublishTariffInfoRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishTariffInfoT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish Price Matrix command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Price Matrix command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishMatrixUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishPriceMatrixT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* EXEGIN - need description */
enum ZclStatusCodeT ZbZclPriceServerSendPublishMatrixRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishPriceMatrixT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish Block Thresholds command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Block Thresholds command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishBlockThresholdsUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishBlockThresholdsT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Block Thresholds command as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Block Thresholds command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishBlockThresholdsRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishBlockThresholdsT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish Block Period command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param info Publish Block Period command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishBlockPeriodUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishBlockPeriodT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Block Period command as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param info Publish Block Period command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishBlockPeriodRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishBlockPeriodT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish Calorific Value command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Calorific Value command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishCalorificValueUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishCalorificValueT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Calorific Value command as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param info Publish Calorific Value command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishCalorificValueRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishCalorificValueT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish Conversion Factor command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Conversion Factor command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishConversionFactorUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishConversionFactorT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Conversion Factor command as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param info Publish Conversion Factor command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishConversionFactorRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishConversionFactorT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish CO2 Value command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish CO2 Value command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishCO2ValueUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishCO2ValueT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish CO2 Value command as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param info Publish CO2 Value command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishCO2ValueRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishCO2ValueT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish Billing Period command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Billing Period command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishBillingPeriodUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishBillingPeriodT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Billing Period command as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param info Publish Billing Period command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishBillingPeriodRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishBillingPeriodT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish Consolidated Bill command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Consolidated Bill command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected.
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZCL_STATUS value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishConsolidatedBillUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishConsolidatedBillT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Consolidated Bill command as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param info Publish Consolidated Bill command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZCL_STATUS value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishConsolidatedBillRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishConsolidatedBillT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an unsolicited Publish Currency Conversion command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command
 * @param info Publish Currency Conversion command structure
 * @param callback Callback function that will be invoked when response
 * is received, if one is expected. If broadcasting, then this should be set to NULL
 * since no response is expected.
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful or other ZCL_STATUS value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishCurrencyConversionUnsolic(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishCurrencyConversionT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Currency Conversion command as a response
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param info Publish Currency Conversion command structure
 * @param callback Callback function for an APSDE-DATA.confirm
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS or other ZCL_STATUS value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishCurrencyConversionRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPriceServerPublishCurrencyConversionT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------
 * Price Client
 *---------------------------------------------------------
 */

/** Price Client callbacks configuration */
struct ZbZclPriceClientCallbacksT {
    enum ZclStatusCodeT (*publish_price)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishPriceT *price, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Price command. */

    enum ZclStatusCodeT (*publish_conversion_factor)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishConversionFactorT *co2, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Conversion Factor command. */

    enum ZclStatusCodeT (*publish_calorific_value)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishCalorificValueT *calorific, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Calorific Value command. */

    enum ZclStatusCodeT (*publish_tariff_info)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishTariffInfoT *tariff_info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Tariff information command. */

    enum ZclStatusCodeT (*publish_price_matrix)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishPriceMatrixT *price_matrix, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Price Matrix command. */

    enum ZclStatusCodeT (*publish_block_thresholds)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishBlockThresholdsT *block_threshs, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Block Thresholds command. */

    enum ZclStatusCodeT (*publish_block_period)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishBlockPeriodT *block_period, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Block Period command. */

    enum ZclStatusCodeT (*publish_co2_value)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishCO2ValueT *co2, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish CO2 Value command. */

    enum ZclStatusCodeT (*publish_billing_period)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishBillingPeriodT *period, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Billing Period command. */

    enum ZclStatusCodeT (*publish_consolidated_bill)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishConsolidatedBillT *bill, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Consolidated Bill command. */

    enum ZclStatusCodeT (*publish_currency_conversion)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishCurrencyConversionT *conversion, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Publish Currency Conversion command. */

    /* EXEGIN - other command callbacks needed? */

    /* Optional commands not parsed into a struct or specific callback */
    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *cluster,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
    /**< Callback to application, invoked on receipt of optional Server commands. */
};

/**
 * Create a new instance of the Price Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPriceClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPriceClientCallbacksT *callbacks, void *arg);

/**
 * Send a Get Current Price command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetCurrentPriceReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Scheduled Prices command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Scheduled Prices command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetScheduledPricesReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetScheduledPricesT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Block Period(s) command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Block Period(s) command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetBlockPeriodReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetBlockPeriodT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Price Acknowledgement command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Price Acknowledge command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandPriceAckReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientPriceAckT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Tariff Information command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Tariff Information command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetTariffInfoReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetTariffInfoT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Price Matrix command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Price Matrix command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetPriceMatrixReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetPriceMatrixT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Block Thresholds command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Block Thresholds command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetBlockThresholdsReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetBlockThresholdsT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get CO2 Value command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get CO2 Value command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetCO2ValueReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetCO2ValueT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Conversion Factor command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Conversion Factor command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetConversionFactorReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetConversionFactorT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Calorific Value command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Calorific Value command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetCalorificValueReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetCalorificValueT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Billing Period command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Billing Period command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetBillingPeriodReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetBillingPeriodT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Consolidated Bill command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Consolidated Bill command structure
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be provided back to the callback function
 * when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetConsolidatedBillReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetConsolidatedBillT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Currency Conversion command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked when the response is received.
 * @param arg Pointer to application data that will be provided back to the callback function
 * when invoked.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetCurrencyConversionReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg),
    void *arg);

/*-----------------------------------------------------------------------------
 * Price Mirror Client
 *-----------------------------------------------------------------------------
 */

/**
 * Allocates a Price Client Mirror cluster.
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param price_server Pointer to Price Server cluster. This is used by the
 *  Metering Mirror to send mirrored commands to the BOMD.
 * @param meter_mirror Pointer to Metering Mirror cluster. This is used to queue
 *  any commands that need to be forwarded to the BOMD when it wakes up.
 * @param meter_client Pointer to Metering Client cluster. This is used to know the
 *  existing notification scheme setup between BOMD & ESI and to determine if the
 *  command needs to be queued or only the notification flag needs to be set.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPriceMirrorAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclClusterT *price_server, struct ZbZclClusterT *meter_mirror,
    struct ZbZclClusterT *meter_client);

#endif
