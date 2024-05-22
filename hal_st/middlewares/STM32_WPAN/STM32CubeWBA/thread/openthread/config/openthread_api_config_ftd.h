/**
  ******************************************************************************
  * @file    openthread_api_config_ftd.h
  * @author  MCD Application Team
  * @brief   This file contains configuration header for configuring OpenThread
  *          stack in FTD mode
  ******************************************************************************
  * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENTHREAD_API_CONFIG_FTD_H_
#define OPENTHREAD_API_CONFIG_FTD_H_

#ifndef OPENTHREAD_FTD
#define OPENTHREAD_FTD 1
#endif

/* Define to 1 if you want to redirect OT assert */
#define OPENTHREAD_CONFIG_PLATFORM_ASSERT_MANAGEMENT 1

/* Define to 1 if you want to enable default log output. */
#define OPENTHREAD_CONFIG_LOG_OUTPUT  OPENTHREAD_CONFIG_LOG_OUTPUT_APP

/* Define to 1 to enable COAP. */
#define OPENTHREAD_CONFIG_COAP_API_ENABLE 1

/* Define to 1 to enable the commissioner role. */
#define OPENTHREAD_CONFIG_COMMISSIONER_ENABLE 1

/* Define to 1 to enable the joiner role. */
#define OPENTHREAD_CONFIG_JOINER_ENABLE 1

/* Define to 1 to enable DHCPv6 Client. */
#define OPENTHREAD_CONFIG_DHCP6_CLIENT_ENABLE 1

/* Define to 1 to enable DHCPv6 SERVER. */
#define OPENTHREAD_CONFIG_DHCP6_SERVER_ENABLE 0

/* Define to 1 to enable MAC whitelist/blacklist feature. */
#define OPENTHREAD_CONFIG_MAC_FILTER_ENABLE 1

/* Define to 1 to enable support for IEEE802.15.4 radio link. */
#define OPENTHREAD_CONFIG_RADIO_LINK_IEEE_802_15_4_ENABLE 1

/* Define to 1 to enable Service feature. */
#define OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE 1

/* Define to 1 to enable TMF network diagnostics client. */
#define OPENTHREAD_CONFIG_TMF_NETDIAG_CLIENT_ENABLE 1

/* Define to 1 to enable the Multicast Listener Registration parent proxing. */
#define OPENTHREAD_CONFIG_TMF_PROXY_MLR_ENABLE 1

/* Define to 1 to enable tracking the uptime of OpenThread instance. */
#define OPENTHREAD_CONFIG_UPTIME_ENABLE 1

/* Define to 1 to enable logging on the platform */
#define OPENTHREAD_CONFIG_LOG_PLATFORM 1

/*  Define as 1 to enable support for locally initializing an Active Operational Dataset. */
#define OPENTHREAD_CONFIG_OPERATIONAL_DATASET_AUTO_INIT 1

/* Define Log Level */
#define OPENTHREAD_CONFIG_LOG_LEVEL OT_LOG_LEVEL_INFO

/* Define to 1 to enable CoAP Secure API */
#define OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE 1

/* Define to 1 to enable IPv6 global addresses configuration utilities */
#define OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE 1

/* Define as 1 to enable saving the settings in RAM instead of flash */
#define OPENTHREAD_SETTINGS_RAM 1

/* Define to 1 to support ping cli command */
#define OPENTHREAD_CONFIG_PING_SENDER_ENABLE 1

/* Define to 1 to enable Mle Link Metrics Initiator */
#define OPENTHREAD_CONFIG_MLE_LINK_METRICS_INITIATOR_ENABLE 1

/* Define to 1 to enable Mle Link Metrics Subject */
#define OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE 1

/* Define to 1 if you want to enable microsecond backoff timer implemented in platform. */
#define OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE 1

/* Define to 1 to support DUA feature */
#define OPENTHREAD_CONFIG_DUA_ENABLE 1

/* Define to 1 to enable Thread Test Harness reference device support. */
#define OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE 1

/* Define to 1 to enable Config MLR */
#define OPENTHREAD_CONFIG_MLR_ENABLE 1

/* Define to 1 to enable DNS Client support */
#define OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE 1

/* Define to 1 to enable SRP Client support */
#define OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE 1

/* Define to 1 for the SRP client implementation to provide APIs that get/set the domain name */
#define OPENTHREAD_CONFIG_SRP_CLIENT_DOMAIN_NAME_API_ENABLE 1

/* Define to 1 to enable ECDSA support */
#define OPENTHREAD_CONFIG_ECDSA_ENABLE 1

/* Define if OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE defined to enable SRP Client buffer support */
#define OPENTHREAD_CONFIG_SRP_CLIENT_BUFFERS_ENABLE OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE

/* Define to 1 to enable SRP Client AutoStart mode support */
#define OPENTHREAD_CONFIG_SRP_CLIENT_AUTO_START_API_ENABLE 1

/* Define to 1 to enable TCP support */
#define OPENTHREAD_CONFIG_TCP_ENABLE 1

/* Define to 1 to enable DNS CLIENT NAT64 mode support */
#define OPENTHREAD_CONFIG_DNS_CLIENT_NAT64_ENABLE 1

/* Define to 1 to enable DNS CLIENT NAT64 mode support */
#define OPENTHREAD_CONFIG_DNS_CLIENT_DEFAULT_NAT64_ALLOWED 1

/* Define to 1 to enable DNS CLIENT SD mode support */
#define OPENTHREAD_CONFIG_DNS_CLIENT_SERVICE_DISCOVERY_ENABLE 1

/* Define to 1 to enable Mesh Diagnostic module. */
#define OPENTHREAD_CONFIG_MESH_DIAG_ENABLE 1

/* Define to 1 to support CSL feature */
#define OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE 1

/* Define Csl receive Time Ahead , Shift RX CSL window ReceiveAt function  */
#define OPENTHREAD_CONFIG_CSL_RECEIVE_TIME_AHEAD 2320

/* Define Csl Time Ahead function, Shift Call to TransmitAt function window  */
#define OPENTHREAD_CONFIG_MAC_CSL_REQUEST_AHEAD_US 3000

#endif /* OPENTHREAD_API_CONFIG_FTD_H_ */
