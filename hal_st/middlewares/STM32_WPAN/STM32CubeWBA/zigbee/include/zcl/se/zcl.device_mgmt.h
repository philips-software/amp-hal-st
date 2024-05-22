/**
 * @file zcl.device_mgmt.h
 * @heading Device Management
 * @brief ZCL Device Management cluster header
 * ZCL 8 Section 10.10
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_DEVICE_MGMT_H
# define ZCL_DEVICE_MGMT_H

#include "zcl/zcl.h"

/** Device Management Server Attribute IDs */
enum ZbZclDeviceMgmtSvrAttrT {
    /* Supplier Control Attribute Set */
    ZCL_DEVICE_MGMT_SVR_ATTR_ProviderID = 0x0100, /**< ProviderID (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ProviderName = 0x0101, /**< ProviderName (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ProviderContactDetails = 0x0102, /**< ProviderContactDetails (Optional) */

    ZCL_DEVICE_MGMT_SVR_ATTR_ProposedProviderID = 0x0110, /**< ProposedProviderID (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ProposedProviderName = 0x0111, /**< ProposedProviderName (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ProposedProviderChangeDate = 0x0112, /**< ProposedProviderChangeDate/Time (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ProposedProviderChangeControl = 0x0113, /**< ProposedProviderChangeControl (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ProposedProviderContactDetails = 0x0114, /**< ProposedProviderContactDetails (Optional) */

    ZCL_DEVICE_MGMT_SVR_ATTR_ReceivedProviderID = 0x0120, /**< ReceivedProviderID (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ReceivedProviderName = 0x0121, /**< ReceivedProviderName (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ReceivedProviderContactDetails = 0x0122, /**< ReceivedProviderContactDetails (Optional) */

    ZCL_DEVICE_MGMT_SVR_ATTR_ReceivedProposedProviderID = 0x0130, /**< ReceivedProposedProviderID (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ReceivedProposedProviderName = 0x0131, /**< ReceivedProposedProviderName (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ReceivedProposedProviderChangeDate = 0x0132, /**< ReceivedProposedProviderChangeDate/Time (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ReceivedProposedProviderChangeControl = 0x0133, /**< ReceivedProposedProviderChangeControl (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ReceivedProposedProviderContactDetails = 0x0134, /**< ReceivedProposedProviderContactDetails (Optional) */

    /* Tenancy Control Attribute Set */
    ZCL_DEVICE_MGMT_SVR_ATTR_ChangeofTenancyUpdateDate = 0x0200, /**< ChangeofTenancyUpdateDate/Time (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_ProposedTenancyChangeControl = 0x0201, /**< ProposedTenancyChangeControl (Optional) */

    /* Backhaul Control Attribute Set */
    ZCL_DEVICE_MGMT_SVR_ATTR_WANStatus = 0x0300, /**< WAN Status (Optional) */

    /* HAN Control Attribute Set */
    ZCL_DEVICE_MGMT_SVR_ATTR_LowMediumThreshold = 0x0400, /**< LowMediumThreshold (Optional) */
    ZCL_DEVICE_MGMT_SVR_ATTR_MediumHighThreshold = 0x0401, /**< MediumHighThreshold (Optional) */
};

/** Device Management Client Attribute IDs */
enum ZbZclDeviceMgmtCliAttrT {
    ZCL_DEVICE_MGMT_CLI_ATTR_ProviderID = 0x0000, /**< Provider ID */
    ZCL_DEVICE_MGMT_CLI_ATTR_ReceivedProviderID = 0x0010, /**< ReceivedProvider ID */

    /* Price Event Configuration Attribute Set */
    ZCL_DEVICE_MGMT_CLI_ATTR_PRICE_START = 0x0100, /**< Price Event Configuration Attribute Set (First Id) */
    ZCL_DEVICE_MGMT_CLI_ATTR_TOUTariffActivation = 0x0100, /**< TOUTariffActivation */
    ZCL_DEVICE_MGMT_CLI_ATTR_BlockTariffactivated = 0x0101, /**< BlockTariffactivated */
    ZCL_DEVICE_MGMT_CLI_ATTR_BlockTOUTariffActivated = 0x0102, /**< BlockTOUTariffActivated */
    ZCL_DEVICE_MGMT_CLI_ATTR_SingleTariffRateActivated = 0x0103, /**< SingleTariffRateActivated */
    ZCL_DEVICE_MGMT_CLI_ATTR_AsychronousBillingOccurred = 0x0104, /**< AsychronousBillingOccurred */
    ZCL_DEVICE_MGMT_CLI_ATTR_SynchronousBillingOccurred = 0x0105, /**< SynchronousBillingOccurred */
    ZCL_DEVICE_MGMT_CLI_ATTR_TariffNotSupported = 0x0106, /**< Tariff NotSupported */
    ZCL_DEVICE_MGMT_CLI_ATTR_PriceClusterNotFound = 0x0107, /**< PriceClusterNotFound */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishPriceReceived = 0x01b0, /**< PublishPriceReceived */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishPriceActioned = 0x01b1, /**< PublishPriceActioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishPriceCancelled = 0x01b2, /**< PublishPriceCancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishPriceRejected = 0x01b3, /**< PublishPriceRejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishTariffInformation = 0x01b4, /**< PublishTariffInformation Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishTariffInformationActioned = 0x01b5, /**< PublishTariffInformation Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishTariffInformationCancelled = 0x01b6, /**< PublishTariffInformation Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishTariffInformationRejected = 0x01b7, /**< PublishTariffInformation Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishPriceMatrixReceived = 0x01b8, /**< PublishPriceMatrixReceived */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishPriceMatrixActioned = 0x01b9, /**< PublishPriceMatrixActioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishPriceMatrixCancelled = 0x01ba, /**< PublishPriceMatrixCancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishPriceMatrixRejected = 0x01bb, /**< PublishPriceMatrixRejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishBlockThresholdsReceived = 0x01bc, /**< PublishBlockThresholdsReceived */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishBlockThresholdsActioned = 0x01bd, /**< PublishBlockThresholdsActioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishBlockThresholdsCancelled = 0x01be, /**< PublishBlockThresholdsCancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishBlockThresholdsRejected = 0x01bf, /**< PublishBlockThresholdsRejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PRICE_GROUP_ID = 0x01ff, /**< Price cluster Group ID (Last Id) */

    /* Metering Event Configuration Attribute Set */
    ZCL_DEVICE_MGMT_CLI_ATTR_METERING_START = 0x0200, /**< Metering Event Configuration Attribute Set (First Id) */
    ZCL_DEVICE_MGMT_CLI_ATTR_CheckMeter = 0x0200, /**< Check Meter */
    ZCL_DEVICE_MGMT_CLI_ATTR_LowBattery = 0x0201, /**< Low Battery */
    ZCL_DEVICE_MGMT_CLI_ATTR_TamperDetect = 0x0202, /**< Tamper Detect */
    ZCL_DEVICE_MGMT_CLI_ATTR_SupplyStatus = 0x0203, /**< Supply Status */
    ZCL_DEVICE_MGMT_CLI_ATTR_SupplyQuality = 0x0204, /**< Supply Quality */
    ZCL_DEVICE_MGMT_CLI_ATTR_LeakDetect = 0x0205, /**< Leak Detect */
    ZCL_DEVICE_MGMT_CLI_ATTR_BatteryFailure = 0x020c, /**< BatteryFailure */
    ZCL_DEVICE_MGMT_CLI_ATTR_LowVoltageL1 = 0x0210, /**< LowVoltageL1 */
    ZCL_DEVICE_MGMT_CLI_ATTR_HighVoltageL1 = 0x0211, /**< HighVoltageL1 */
    ZCL_DEVICE_MGMT_CLI_ATTR_UnderVoltage = 0x0223, /**< UnderVoltage */
    ZCL_DEVICE_MGMT_CLI_ATTR_OverVoltage = 0x0224, /**< OverVoltage */
    ZCL_DEVICE_MGMT_CLI_ATTR_PressureTooLow = 0x0231, /**< PressureTooLow */
    ZCL_DEVICE_MGMT_CLI_ATTR_PressureTooHigh = 0x0232, /**< PressureTooHigh */
    ZCL_DEVICE_MGMT_CLI_ATTR_GetProfileCommandReceived = 0x02c0, /**< Get Profile Command Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_GetProfileCommandActioned = 0x02c1, /**< Get Profile Command Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_GetProfileCommandCancelled = 0x02c2, /**< Get Profile Command Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_GetProfileCommandRejected = 0x02c3, /**< Get Profile Command Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_RequestMirrorResponseCommandReceived = 0x02c4, /**< RequestMirrorResponse Command Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_RequestMirrorResponseCommandActioned = 0x02c5, /**< RequestMirrorResponse Command Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_RequestMirrorResponseCommandCancelled = 0x02c6, /**< RequestMirrorResponse Command Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_RequestMirrorResponseCommandRejected = 0x02c7, /**< RequestMirrorResponse Command Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_MirrorRemoved0CommandReceived = 0x02c8, /**< MirrorRemoved Command Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_MirrorRemoved0CommandActioned = 0x02c9, /**< MirrorRemoved Command Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_MirrorRemoved0CommandCancelled = 0x02ca, /**< MirrorRemoved Command Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_MirrorRemoved0CommandRejected = 0x02cb, /**< MirrorRemoved Command Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_GetSampledDataCommandReceived = 0x02e0, /**< GetSampledData Command Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_GetSampledDataCommandActioned = 0x02e1, /**< GetSampledData Command Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_GetSampledDataCommandCancelled = 0x02e2, /**< GetSampledData Command Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_GetSampledDataCommandRejected = 0x02e3, /**< GetSampledData Command Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_SupplyON = 0x02e4, /**< Supply ON */
    ZCL_DEVICE_MGMT_CLI_ATTR_SupplyARMED = 0x02e5, /**< Supply ARMED */
    ZCL_DEVICE_MGMT_CLI_ATTR_SupplyOFF = 0x02e6, /**< Supply OFF */
    ZCL_DEVICE_MGMT_CLI_ATTR_METERING_GROUP_ID = 0x02ff, /**< Metering cluster Group ID (Last Id) */

    /* Messaging Event Configuration Attribute Set */
    ZCL_DEVICE_MGMT_CLI_ATTR_MESSAGING_START = 0x0300, /**< Messaging Event Configuration Attribute Set (First Id) */
    ZCL_DEVICE_MGMT_CLI_ATTR_MessageConfirmationSent = 0x0300, /**< Message Confirmation Sent */
    ZCL_DEVICE_MGMT_CLI_ATTR_DisplayMessageReceived = 0x03c0, /**< DisplayMessageReceived */
    ZCL_DEVICE_MGMT_CLI_ATTR_DisplayMessageActioned = 0x03c1, /**< DisplayMessageActioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_DisplayMessageCancelled = 0x03c2, /**< DisplayMessageCancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_DisplayMessageRejected = 0x03c3, /**< DisplayMessageRejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_CancelMessageReceived = 0x03c4, /**< CancelMessageReceived */
    ZCL_DEVICE_MGMT_CLI_ATTR_CancelMessageActioned = 0x03c5, /**< CancelMessageActioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_CancelMessageCancelled = 0x03c6, /**< CancelMessageCancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_CancelMessageRejected = 0x03c7, /**< CancelMessageRejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_MESSAGING_GROUP_ID = 0x03ff, /**< Messaging cluster Group ID (Last Id) */

    /* Prepayment Event Configuration Attribute Set */
    ZCL_DEVICE_MGMT_CLI_ATTR_PREPAYMENT_START = 0x0400, /**< Prepayment Event Configuration Attribute Set (First Id) */
    ZCL_DEVICE_MGMT_CLI_ATTR_LowCredit = 0x0400, /**< Low Credit */
    ZCL_DEVICE_MGMT_CLI_ATTR_NoCredit = 0x0401, /**< No Credit (Zero Credit) */
    ZCL_DEVICE_MGMT_CLI_ATTR_CreditExhausted = 0x0402, /**< Credit Exhausted */
    ZCL_DEVICE_MGMT_CLI_ATTR_EmergencyCreditEnabled = 0x0404, /**< Emergency Credit Enabled */
    ZCL_DEVICE_MGMT_CLI_ATTR_EmergencyCreditExhausted = 0x0404, /**< Emergency Credit Exhausted */
    ZCL_DEVICE_MGMT_CLI_ATTR_CreditAdjustment = 0x0423, /**< Credit Adjustment */
    ZCL_DEVICE_MGMT_CLI_ATTR_CreditAdjustFail = 0x0424, /**< Credit Adjust Fail */
    ZCL_DEVICE_MGMT_CLI_ATTR_PrepayClusterNotFound = 0x0432, /**< Prepay Cluster Not Found */
    ZCL_DEVICE_MGMT_CLI_ATTR_SelectAvailableEmergencyCreditReceived = 0x04c0, /**< SelectAvailableEmergencyCredit Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_SelectAvailableEmergencyCreditActioned = 0x04c1, /**< SelectAvailableEmergencyCredit Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_SelectAvailableEmergencyCreditCancelled = 0x04c2, /**< SelectAvailableEmergencyCredit Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_SelectAvailableEmergencyCreditRejected = 0x04c3, /**< SelectAvailableEmergencyCredit Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_EmergencyCreditSetupReceived = 0x04c8, /**< Emergency Credit Setup Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_EmergencyCreditSetupActioned = 0x04c9, /**< Emergency Credit Setup Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_EmergencyCreditSetupCancelled = 0x04ca, /**< Emergency Credit Setup Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_EmergencyCreditSetupRejected = 0x04cb, /**< Emergency Credit Setup Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_CreditAdjustmentReceived = 0x04d0, /**< Credit Adjustment Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_CreditAdjustmentActioned = 0x04d1, /**< Credit Adjustment Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_CreditAdjustmentCancelled = 0x04d2, /**< Credit Adjustment Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_CreditAdjustmentRejected = 0x04d3, /**< Credit Adjustment Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetLowCreditWarningLevelReceived = 0x04e0, /**< Set Low Credit Warning Level Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetLowCreditWarningLevelActioned = 0x04e1, /**< Set Low Credit Warning Level Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetLowCreditWarningLevelCancelled = 0x04e2, /**< Set Low Credit Warning Level Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetLowCreditWarningLevelRejected = 0x04e3, /**< Set Low Credit Warning Level Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetMaximumCreditLimitReceived = 0x04e8, /**< SetMaximumCreditLimit Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetMaximumCreditLimitActioned = 0x04e9, /**< SetMaximumCreditLimit Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetMaximumCreditLimitCancelled = 0x04ea, /**< SetMaximumCreditLimit Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetMaximumCreditLimitRejected = 0x04eb, /**< SetMaximumCreditLimit Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PREPAYMENT_GROUP_ID = 0x04ff, /**< Prepayment cluster Group ID (Last Id) */

    /* Calendar Event Configuration Attribute Set */
    ZCL_DEVICE_MGMT_CLI_ATTR_CALENDAR_START = 0x0500, /**< Calendar Event Configuration Attribute Set (First Id) */
    ZCL_DEVICE_MGMT_CLI_ATTR_CalendarClusterNotFound = 0x0500, /**< Calendar Cluster Not Found */
    ZCL_DEVICE_MGMT_CLI_ATTR_CalendarChangePassiveActivated = 0x0501, /**< Calendar Change Passive Activated */
    ZCL_DEVICE_MGMT_CLI_ATTR_CalendarChangePassiveUpdated = 0x0502, /**< Calendar Change Passive Updated */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCalendarReceived = 0x05c0, /**< PublishCalendar Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCalendarActioned = 0x05c1, /**< PublishCalendar Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCalendarCancelled = 0x05c2, /**< PublishCalendar Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCalendarRejected = 0x05c3, /**< PublishCalendar Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishDayProfileReceived = 0x05c4, /**< Publish Day Profile Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishDayProfileActioned = 0x05c5, /**< Publish Day Profile Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishDayProfileCancelled = 0x05c6, /**< Publish Day Profile Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishDayProfileRejected = 0x05c7, /**< Publish Day Profile Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishWeekProfileReceived = 0x05c8, /**< Publish Week Profile Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishWeekProfileActioned = 0x05c9, /**< Publish Week Profile Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishWeekProfileCancelled = 0x05ca, /**< Publish Week Profile Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishWeekProfileRejected = 0x05cb, /**< Publish Week Profile Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishSeasonsReceived = 0x05cc, /**< Publish Seasons Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishSeasonsActioned = 0x05cd, /**< Publish Seasons Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishSeasonsCancelled = 0x05ce, /**< Publish Seasons Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishSeasonsRejected = 0x05cf, /**< Publish Seasons Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishSpecialDaysReceived = 0x05d0, /**< Publish Special Days Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishSpecialDaysActioned = 0x05d1, /**< Publish Special Days Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishSpecialDaysCancelled = 0x05d2, /**< Publish Special Days Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishSpecialDaysRejected = 0x05d3, /**< Publish Special Days Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_CALENDAR_GROUP_ID = 0x05ff, /**< Calendar cluster Group ID (Last Id) */

    /* Device Management Event Configuration Attribute Set */
    ZCL_DEVICE_MGMT_CLI_ATTR_DEVICE_MGMT_START = 0x0600, /**< Device Management Event Configuration Attribute Set (First Id) */
    ZCL_DEVICE_MGMT_CLI_ATTR_Password1Change = 0x0600, /**< Password1Change */
    ZCL_DEVICE_MGMT_CLI_ATTR_EventLogCleared = 0x0604, /**< EventLogCleared */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCoTReceived = 0x06c0, /**< Publish CoT Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCoTActioned = 0x06c1, /**< Publish CoT Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCoTCancelled = 0x06c2, /**< Publish CoT Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCoTRejected = 0x06c3, /**< Publish CoT Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCoSReceived = 0x06c4, /**< Publish CoS Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCoSActioned = 0x06c5, /**< Publish CoS Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCoSCancelled = 0x06c6, /**< Publish CoS Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_PublishCoSRejected = 0x06c7, /**< Publish CoS Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_ChangePasswordReceived = 0x06c8, /**< Change Password Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_ChangepasswordActioned = 0x06c9, /**< Change password Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_ChangePasswordCancelled = 0x06ca, /**< Change Password Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_ChangePasswordRejected = 0x06cb, /**< Change Password Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetEventConfigurationReceived = 0x06cc, /**< SetEventConfiguration Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetEventConfigurationActioned = 0x06cd, /**< SetEventConfiguration Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetEventConfigurationCancelled = 0x06ce, /**< SetEventConfiguration Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_SetEventConfigurationRejected = 0x06cf, /**< SetEventConfiguration Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpdateSiteIDReceived = 0x06d0, /**< UpdateSiteID Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpdateSiteIDActioned = 0x06d1, /**< UpdateSiteID Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpdateSiteIDCancelled = 0x06d2, /**< UpdateSiteID Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpdateSiteIDRejected = 0x06d3, /**< UpdateSiteID Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpdateCINReceived = 0x06d4, /**< UpdateCIN Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpdateCINActioned = 0x06d5, /**< UpdateCIN Actioned */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpdateCINCancelled = 0x06d6, /**< UpdateCIN Cancelled */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpdateCINRejected = 0x06d7, /**< UpdateCIN Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_DEVICE_MGMT_GROUP_ID = 0x06ff, /**< Device Management cluster Group ID (Last Id) */

    /* Tunnel Event Configuration Attribute Set */
    ZCL_DEVICE_MGMT_CLI_ATTR_TUNNEL_START = 0x0700, /**< Tunnel Event Configuration Attribute Set (First Id) */
    ZCL_DEVICE_MGMT_CLI_ATTR_TunnelingClusterNotFound = 0x0700, /**< Tunneling Cluster Not Found */
    ZCL_DEVICE_MGMT_CLI_ATTR_RequestTunnelCommandReceived = 0x07c0, /**< RequestTunnel Command Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_RequestTunnelCommandRejected = 0x07c1, /**< RequestTunnel Command Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_RequestTunnelCommandGenerated = 0x07c2, /**< RequestTunnel Command Generated */
    ZCL_DEVICE_MGMT_CLI_ATTR_CloseTunnelCommandReceived = 0x07c3, /**< CloseTunnel Command Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_CloseTunnelCommandRejected = 0x07c4, /**< CloseTunnel Command Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_CloseTunnelCommandGenerated = 0x07c5, /**< CloseTunnel Command Generated */
    ZCL_DEVICE_MGMT_CLI_ATTR_TransferDataCommandReceived = 0x07c6, /**< TransferData Command Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_TransferDataCommandRejected = 0x07c7, /**< TransferData Command Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_TransferDataCommandGenerated = 0x07c8, /**< TransferData Command Generated */
    ZCL_DEVICE_MGMT_CLI_ATTR_TransferDataErrorCommandReceived = 0x07c9, /**< TransferDataError Command Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_TransferDataErrorCommandRejected = 0x07ca, /**< TransferDataError Command Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_TransferDataErrorCommandGenerated = 0x07cb, /**< TransferDataError Command Generated */
    ZCL_DEVICE_MGMT_CLI_ATTR_TUNNEL_GROUP_ID = 0x07ff, /**< Tunnel cluster Group ID (Last Id) */

    /* OTA Event Configuration Attribute Set */
    ZCL_DEVICE_MGMT_CLI_ATTR_OTA_START = 0x0800, /**< OTA Event Configuration Attribute Set (First Id) */
    ZCL_DEVICE_MGMT_CLI_ATTR_FirmwareReadyForActivation = 0x0800, /**< FirmwareReadyForActivation */
    ZCL_DEVICE_MGMT_CLI_ATTR_FirmwareActivated = 0x0801, /**< FirmwareActivated */
    ZCL_DEVICE_MGMT_CLI_ATTR_FirmwareActivationFailure = 0x0802, /**< Firmware Activation Failure */
    ZCL_DEVICE_MGMT_CLI_ATTR_PatchReadyForActivation = 0x0803, /**< Patch Ready For Activation */
    ZCL_DEVICE_MGMT_CLI_ATTR_PatchActivated = 0x0804, /**< Patch Activated */
    ZCL_DEVICE_MGMT_CLI_ATTR_PatchFailure = 0x0805, /**< Patch Failure */
    /* NOTE: discontinuity */
    ZCL_DEVICE_MGMT_CLI_ATTR_ImageNotifyCommandReceived = 0x08c0, /**< Image Notify Command Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_ImageNotifyCommandRejected = 0x08c1, /**< Image Notify Command Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_QueryNextImageRequestGenerated = 0x08c2, /**< Query Next Image Request Generated */
    ZCL_DEVICE_MGMT_CLI_ATTR_QueryNextImageResponseReceived = 0x08c3, /**< Query Next Image Response Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_QueryNextImageResponseRejected = 0x08c4, /**< Query Next Image Response Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_ImageBlockRequestGenerated = 0x08c5, /**< Image Block Request Generated */
    ZCL_DEVICE_MGMT_CLI_ATTR_ImagePageRequestGenerated = 0x08c6, /**< Image Page Request Generated */
    ZCL_DEVICE_MGMT_CLI_ATTR_ImageBlockResponseReceived = 0x08c7, /**< Image Block Response Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_ImageBlockResponseRejected = 0x08c8, /**< Image Block Response Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpgradeEndRequestGenerated = 0x08c9, /**< Upgrade End Request Generated */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpgradeEndResponseReceived = 0x08ca, /**< Upgrade End Response Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_UpgradeEndResponseRejected = 0x08cb, /**< Upgrade End Response Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_QuerySpecificFileRequestGenerated = 0x08cc, /**< Query Specific File Request Generated */
    ZCL_DEVICE_MGMT_CLI_ATTR_QuerySpecificFileResponseReceived = 0x08cd, /**< Query Specific File Response Received */
    ZCL_DEVICE_MGMT_CLI_ATTR_QuerySpecificFileResponseRejected = 0x08ce, /**< Query Specific File Response Rejected */
    ZCL_DEVICE_MGMT_CLI_ATTR_OTA_GROUP_ID = 0x08ff /**< OTA cluster Group ID (Last Id) */
};

/* Device Management Server Cluster Commands Generated */
enum ZbZclDeviceMgmtServerCommandT {
    ZCL_DEVICE_MGMT_SVR_CMD_PUBLISH_CHANGE_TENANCY = 0x00, /**< Publish Change of Tenancy */
    ZCL_DEVICE_MGMT_SVR_CMD_PUBLISH_CHANGE_SUPPLIER = 0x01, /**< Publish Change of Supplier */
    ZCL_DEVICE_MGMT_SVR_CMD_REQ_NEW_PASS_RSP = 0x02, /**< Request New Password Response */
    ZCL_DEVICE_MGMT_SVR_CMD_UPDATE_SITE_ID = 0x03, /**< Update SiteID */
    ZCL_DEVICE_MGMT_SVR_CMD_SET_EVENT_CONFIG = 0x04, /**< SetEventConfiguration */
    ZCL_DEVICE_MGMT_SVR_CMD_GET_EVENT_CONFIG = 0x05, /**< GetEventConfiguration */
    ZCL_DEVICE_MGMT_SVR_CMD_UPDATE_CIN = 0x06 /**< Update CIN */
};

/* Device Management Client Cluster Commands Generated */
enum ZbZclDeviceMgmtClientCommandT {
    ZCL_DEVICE_MGMT_CLI_CMD_GET_CHANGE_TENANCY = 0x00, /**< Get Change of Tenancy */
    ZCL_DEVICE_MGMT_CLI_CMD_GET_CHANGE_SUPPLIER = 0x01, /**< Get Change of Supplier */
    ZCL_DEVICE_MGMT_CLI_CMD_REQ_NEW_PASS = 0x02, /**< Request New Password */
    ZCL_DEVICE_MGMT_CLI_CMD_GET_SITE_ID = 0x03, /**< GetSiteID */
    ZCL_DEVICE_MGMT_CLI_CMD_REPORT_EVENT_CONFIG = 0x04, /**< Report Event Configuration */
    ZCL_DEVICE_MGMT_CLI_CMD_GET_CIN = 0x05 /**< GetCIN */
};

/* SE 1.4: Table D-172 Password Type Enumeration */
enum ZbZclDeviceMgmtPasswordType {
    ZCL_DEVICE_MGMT_PASSWORD_TYPE_1_SERVICE_MENU = 0x01, /**< Password 1 - Used for access to the Service menu */
    ZCL_DEVICE_MGMT_PASSWORD_TYPE_2_CONSUMER_MENU = 0x02, /**< Password 2 - Used for access to the Consumer menu */
    ZCL_DEVICE_MGMT_PASSWORD_TYPE_3 = 0x03, /**< Password 3 - TBD */
    ZCL_DEVICE_MGMT_PASSWORD_TYPE_4 = 0x04 /**< Password 4 - TBD */
};

/* SE 1.4: Table D-173 Configuration Control Enumeration */
enum ZbZclDeviceMgmtEventConfigCtrl {
    ZCL_DEVICE_MGMT_EVENT_CONFIG_CTRL_APPLY_BY_LIST = 0x00, /**< Apply by List */
    ZCL_DEVICE_MGMT_EVENT_CONFIG_CTRL_APPLY_BY_EVENT_GROUP = 0x01, /**< Apply by Event Group */
    ZCL_DEVICE_MGMT_EVENT_CONFIG_CTRL_APPLY_BY_LOG_TYPE = 0x02, /**< Apply by Log Type */
    ZCL_DEVICE_MGMT_EVENT_CONFIG_CTRL_APPLY_BY_CONFIG_MATCH = 0x03 /**< Apply by Configuration Match */
};

struct ZbZclDeviceMgmtReqNewPassT {
    uint8_t password_type;
};

struct ZbZclDeviceMgmtEventConfigPayloadT {
    uint16_t event_id;
    uint8_t event_config;
};

#define ZCL_DEVICE_MGMT_REPORT_EVENT_CONFIG_MAX     ((ZCL_PAYLOAD_UNFRAG_SAFE_SIZE - 2U) / 3U)

struct ZbZclDeviceMgmtReportEvtConfigT {
    uint8_t command_index;
    uint8_t total_commands;
    struct ZbZclDeviceMgmtEventConfigPayloadT event_config_payload[ZCL_DEVICE_MGMT_REPORT_EVENT_CONFIG_MAX];
    uint8_t num_entries;
};

struct ZbZclDeviceMgmtPublishChangeTenancyT {
    uint32_t provider_id;
    uint32_t issuer_id;
    uint8_t tariff_type;
    uint32_t implement_time;
    uint32_t prop_tenancy_change_ctrl;
};

#define ZCL_DEVICE_MGMT_PROVIDER_NAME_LEN_MAX       15U
#define ZCL_DEVICE_MGMT_PROVIDER_CONTACT_LEN_MAX    19U

struct ZbZclDeviceMgmtPublishChangeSupplierT {
    uint32_t curr_provider_id;
    uint32_t issuer_event_id;
    uint8_t tariff_type;
    uint32_t proposed_provider_id;
    uint32_t provider_change_impl_time;
    uint32_t provider_change_ctrl;
    uint8_t proposed_provider_name[ZCL_DEVICE_MGMT_PROVIDER_NAME_LEN_MAX + 1U];
    uint8_t proposed_provider_contact[ZCL_DEVICE_MGMT_PROVIDER_CONTACT_LEN_MAX + 1U];
};

#define ZCL_DEVICE_MGMT_PASSWORD_LEN_MAX            10U

struct ZbZclDeviceMgmtReqNewPassRspT {
    uint32_t issuer_event_id;
    uint32_t implement_time;
    uint16_t duration;
    uint8_t password_type;
    uint8_t password[ZCL_DEVICE_MGMT_PASSWORD_LEN_MAX + 1U];
};

#define ZCL_DEVICE_MGMT_SITE_ID_LEN_MAX             32U

struct ZbZclDeviceMgmtUpdateSiteIdT {
    uint32_t issuer_event_id;
    uint32_t site_id_time;
    uint32_t provider_id;
    uint8_t site_id[ZCL_DEVICE_MGMT_SITE_ID_LEN_MAX + 1U];
};

#define ZCL_DEVICE_MGMT_SET_EVENT_ID_LIST_MAX        (ZCL_PAYLOAD_UNFRAG_SAFE_SIZE - 11U) / 2U

struct ZbZclDeviceMgmtSetEventConfigT {
    uint32_t issuer_event_id;
    uint32_t start_time;
    uint8_t event_config;
    enum ZbZclDeviceMgmtEventConfigCtrl config_ctrl;
    union {
        struct {
            uint8_t num_events;
            uint16_t event_id_list[ZCL_DEVICE_MGMT_SET_EVENT_ID_LIST_MAX];
        } apply_by_list;
        struct {
            uint16_t group_id;
        } apply_by_group_id;
        struct {
            uint8_t log_id;
        } apply_by_log_id;
        struct {
            uint8_t config_match;
        } apply_by_config_match;
    } event_config_payload;
    bool to_bomd;
};

struct ZbZclDeviceMgmtGetEventConfigT {
    uint16_t event_id;
    bool to_bomd;
};

#define ZCL_DEVICE_MGMT_CUSTOMER_ID_LEN_MAX         24U

struct ZbZclDeviceMgmtUpdateCinT {
    uint32_t issuer_event_id;
    uint32_t cin_implement_time;
    uint32_t provider_id;
    uint8_t customer_id[ZCL_DEVICE_MGMT_CUSTOMER_ID_LEN_MAX + 1U];
};

/*---------------------------------------------------------------
 * Server API
 *---------------------------------------------------------------
 */

/** Device Management Server callbacks configuration */
struct ZbZclDeviceMgmtServerCallbacksT {
    enum ZclStatusCodeT (*get_change_tenancy)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'Get Change of Tenancy' command.
     * The ESI may send a PublishChangeofTenancy command (ZbZclDeviceMgmtServerPublishChangeTenancy).
     * Return ZCL_STATUS_SUCCESS if PublishChangeofTenancy was sent, or ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_change_supplier)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'Get Change of Supplier' command.
     * The ESI may send a PublishChangeofSupplier command (ZbZclDeviceMgmtServerPublishChangeSupplier).
     * Return ZCL_STATUS_SUCCESS if PublishChangeofSupplier was sent, or ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*request_new_pass)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclDeviceMgmtReqNewPassT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'Request New Password' command.
     * The ESI may send a RequestNewPasswordResponse command (ZbZclDeviceMgmtServerReqNewPassRsp).
     * Return ZCL_STATUS_SUCCESS if RequestNewPasswordResponse was sent, or ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_site_id)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'GetSiteID' command.
     * The ESI may send a UpdateSiteID command (ZbZclDeviceMgmtServerUpdateSiteId).
     * Return ZCL_STATUS_SUCCESS if UpdateSiteID was sent, or ZCL_STATUS_NOT_FOUND otherwise. */

    enum ZclStatusCodeT (*get_cin)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'GetCIN' command.
     * The ESI may send a UpdateCIN command (ZbZclDeviceMgmtServerUpdateCin).
     * Return ZCL_STATUS_SUCCESS if UpdateCIN was sent, or ZCL_STATUS_NOT_FOUND otherwise. */
};

/**
 * Create a new instance of the Device Management Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDeviceMgmtServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclDeviceMgmtServerCallbacksT *callbacks, void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerPublishChangeTenancyRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDeviceMgmtPublishChangeTenancyT *info, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerPublishChangeTenancyUnsolic(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDeviceMgmtPublishChangeTenancyT *info, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerPublishChangeSupplierRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDeviceMgmtPublishChangeSupplierT *info, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerPublishChangeSupplierUnsolic(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDeviceMgmtPublishChangeSupplierT *info, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Sent as response to a 'Request New Password' command */
enum ZclStatusCodeT ZbZclDeviceMgmtServerReqNewPassRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDeviceMgmtReqNewPassRspT *info, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/* Sent unsolicited (i.e. not in response to a 'Request New Password' command) */
enum ZclStatusCodeT ZbZclDeviceMgmtServerReqNewPassRspUnsolic(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDeviceMgmtReqNewPassRspT *info, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerUpdateSiteIdRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDeviceMgmtUpdateSiteIdT *info, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerUpdateSiteIdUnsolic(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDeviceMgmtUpdateSiteIdT *info, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerSetEventConfigReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDeviceMgmtSetEventConfigT *info, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerGetEventConfigReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDeviceMgmtGetEventConfigT *info, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerUpdateCinRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDeviceMgmtUpdateCinT *info, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtServerUpdateCinUnsolic(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDeviceMgmtUpdateCinT *info, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Parse a ZCL_DEVICE_MGMT_CLI_CMD_REPORT_EVENT_CONFIG command */
enum ZclStatusCodeT ZbZclDeviceMgmtParseReportEvtConfig(struct ZbZclClusterT *cluster, struct ZbApsdeDataIndT *dataIndPtr,
    struct ZbZclDeviceMgmtReportEvtConfigT *info);

/*---------------------------------------------------------------
 * Client API
 *---------------------------------------------------------------
 */

/** Device Management Client callbacks configuration */
struct ZbZclDeviceMgmtClientCallbacksT {
    enum ZclStatusCodeT (*publish_change_tenancy)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclDeviceMgmtPublishChangeTenancyT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'Publish Change Tenancy' command.
     * The return code (e.g. ZCL_STATUS_SUCCESS) is included in the standard Default Response. */

    enum ZclStatusCodeT (*publish_change_supplier)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclDeviceMgmtPublishChangeSupplierT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'Publish Change Supplier' command.
     * The return code (e.g. ZCL_STATUS_SUCCESS) is included in the standard Default Response. */

    enum ZclStatusCodeT (*req_new_pass_rsp_unsolic)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclDeviceMgmtReqNewPassRspT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of an unsolicited
     * 'Request New Password Response' command.
     * The return code (e.g. ZCL_STATUS_SUCCESS) is included in the standard Default Response. */

    enum ZclStatusCodeT (*update_site_id)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclDeviceMgmtUpdateSiteIdT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'Update SiteID' command.
     * The return code (e.g. ZCL_STATUS_SUCCESS) is included in the standard Default Response. */

    enum ZclStatusCodeT (*set_event_config)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclDeviceMgmtSetEventConfigT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'Set Event Configuration' command.
     * The return code (e.g. ZCL_STATUS_SUCCESS) is included in the standard Default Response. */

    enum ZclStatusCodeT (*get_event_config)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclDeviceMgmtGetEventConfigT *info, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of 'Get Event Configuration' command.
     * The return code (e.g. ZCL_STATUS_SUCCESS) is included in the standard Default Response. */

    enum ZclStatusCodeT (*update_cin)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclDeviceMgmtUpdateCinT *info, struct ZbZclAddrInfoT *srcInfo);
};

bool ZbZclDeviceMgmtParseReqNewPassRsp(const uint8_t *buf, unsigned int len, struct ZbZclDeviceMgmtReqNewPassRspT *rsp);
bool ZbZclDeviceMgmtParseUpdateCin(const uint8_t *buf, unsigned int len, struct ZbZclDeviceMgmtUpdateCinT *rsp);

/**
 * Create a new instance of the Device Management Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param arg Pointer to application data that will included in the callback when invoked.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDeviceMgmtClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclDeviceMgmtClientCallbacksT *callbacks, void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtClientGetChangeTenancy(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtClientGetChangeSupplier(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtClientReqNewPassword(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDeviceMgmtReqNewPassT *info,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtClientGetSiteId(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtClientReportEventConfig(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDeviceMgmtReportEvtConfigT *info,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDeviceMgmtClientGetCin(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*-----------------------------------------------------------------------------
 * Mirror Client
 *-----------------------------------------------------------------------------
 */

/**
 * Allocates a Device Management Client Mirror cluster. The caller (application) must
 *  attach any attributes to the cluster you want to mirror.
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param device_mgmt_server Pointer to Device Mgmt Server cluster. This is used by the
 *  Metering Mirror to send mirrored commands to the BOMD.
 * @param meter_mirror Pointer to Metering Mirror cluster. This is used to queue
 *  any commands that need to be forwarded to the BOMD when it wakes up.
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDeviceMgmtMirrorAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclClusterT *device_mgmt_server, struct ZbZclClusterT *meter_mirror);

#endif
