/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

/*---------------------------------------------------------------------------
 * Implements some common PIB code.
 * This code should only be shared within the MAC layer and not
 * used by the upper layers.
 *---------------------------------------------------------------------------
 */

#ifndef ZBMAC_PIB_H
#define ZBMAC_PIB_H

#include "ieee802154_api.h"

/* IEEE Join List - max concurrent IEEE addresses in join list */
/* EXEGIN 1 - join list size? use a link list instead? */
#define WPAN_MAC_JOIN_IEEELIST_MAX              16

/* IEEE 802.15.4 MAC PIB */
struct WpanMacPibT {
    uint32_t macAckWaitDuration;
    uint8_t macAssociatedPanCoord;
    uint8_t macAssociationPermit;
    uint8_t macAutoRequest;
    uint8_t macBattLifeExt;
    uint8_t macBattLifeExtPeriods;
    uint8_t macBeaconPayload[WPAN_CONST_MAX_BEACON_PAYLOAD_LENGTH];
    uint8_t macBeaconPayloadLength;
    uint8_t macBeaconOrder;
    uint32_t macBeaconTxTime;
    uint8_t macBsn;
    uint64_t macCoordExtendedAddress;
    uint16_t macCoordShortAddress;
    uint8_t macDsn;
    uint8_t macGtsPermit;
    uint8_t macMaxBE;
    uint8_t macMaxCsmaBackoffs;
    uint32_t macMaxFrameTotalWaitTime; /* symbols */
    uint8_t macMaxFrameRetries;
    uint8_t macMinBE;
    uint16_t macPanId;
    uint8_t macPromiscuousMode;
    uint8_t macResponseWaitTime;
    uint8_t macRxOnWhenIdle;
    uint8_t macSecurityEnabled;
    /* macShortAddress -- 0xffff means not associated, 0xfffe means
     * associated, but use extended addressing, and anything else
     * means the short address to use while associated. */
    uint16_t macShortAddress;
    uint8_t macSuperframeOrder;
    uint16_t macSyncSymbolOffset;
    uint8_t macTimestampSupported;
    uint16_t macTransactionPersistenceTime;
    /* Exegin Add-ons */
    uint64_t macExtendedAddress;
    uint8_t macPromiscuousAutoCrc;
    uint8_t macKeySwitchIndex;
    uint64_t macKeySwitchMask;

    /* 802.15.4e - Enhanced Beacon Request */
#if 0 /* Created in NHLE */
    uint8_t macEbrFilters[2]; /* list of booleans [lqi, percent] */
    uint8_t macEbrPermitJoining; /* Whether to set "Permit Joining On" bit in EB Filter IE */
    uint8_t macEbrLinkQuality; /* Minimum LQI to allow through filter */
    uint8_t macEbrPercentFilter; /* Random filter */
    macEbrAttributeList;
#endif
    uint8_t macBeaconAutoRespond;

    /* mcp_macBeaconAppendixLength */
    uint8_t mcp_macBeaconAppendixLength;

    /* 802.15.4e - Enhanced Beacon */
#if 0 /* Handled by NHLE */
    uint8_t macUseEnhancedBeacon; /* for beacon-enabled PANs (ignored) */
    uint8_t macEbIeListData[WPAN_IE_MAX_SIZE]; /* EB IE list (should not be set for ZigBee PRO) */
    uint8_t macEbIeListDataLength; /* EB IE list length (should not be set for ZigBee PRO) */
    uint8_t macEbFilteringEnabled;
#endif
    uint8_t macEbSeqNum;

    uint64_t macJoinIeeeList[WPAN_MAC_JOIN_IEEELIST_MAX];

    /* LBT */
    uint8_t macLBTMaxTxRetries;

    /* SUN PIB attributes */
    uint8_t phyFskPreambleLength; /* PHY_IOCTL_PREAMBLE_LENGTH - Also used by GB-868 */
    uint8_t phySunFskSfd; /* PHY_IOCTL_FSK_SFD - only used to remember what we set to the radio. */
    uint8_t phyFskFecEnabled; /* PHY_IOCTL_FSK_FEC - Also used by GB-868 */
    uint8_t phyFskScramblePsdu; /* PHY_IOCTL_FSK_DW - Also used by GB-868 */
};

/* A copy of the PIB filled with default values. */
extern const struct WpanMacPibT WpanMacPibDefault;

void WpanMacDefaultSetReq(struct WpanMacPibT *pibPtr, struct mcp_setreq *req, struct wpan_set_conf *conf);
void WpanMacDefaultGetReq(struct WpanMacPibT *pibPtr, struct wpan_get_req *req, struct mcp_getconf *conf);

#endif /* ZBMAC_PIB_H */
