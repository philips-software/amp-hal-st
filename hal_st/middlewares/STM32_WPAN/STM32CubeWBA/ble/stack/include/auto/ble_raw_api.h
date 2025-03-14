/*****************************************************************************
 * @file    ble_raw_api.h
 * @brief   STM32WBA BLE API: commands and events
 *          Auto-generated file: do not edit!
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */

#ifndef BLE_RAW_API_H__
#define BLE_RAW_API_H__


#include "ble_types.h"

/* COMMANDS */

tBleStatus HCI_DISCONNECT( uint16_t Connection_Handle,
                           uint8_t Reason );

tBleStatus HCI_READ_REMOTE_VERSION_INFORMATION( uint16_t Connection_Handle );

tBleStatus HCI_SET_EVENT_MASK( const uint8_t* Event_Mask );

tBleStatus HCI_RESET( void );

tBleStatus HCI_READ_CONNECTION_ACCEPT_TIMEOUT( uint16_t* Connection_Accept_Timeout );

tBleStatus HCI_WRITE_CONNECTION_ACCEPT_TIMEOUT( uint16_t Connection_Accept_Timeout );

tBleStatus HCI_READ_TRANSMIT_POWER_LEVEL( uint16_t Connection_Handle,
                                          uint8_t Type,
                                          uint8_t* Transmit_Power_Level );

tBleStatus HCI_SET_CONTROLLER_TO_HOST_FLOW_CONTROL( uint8_t Flow_Control_Enable );

tBleStatus HCI_HOST_BUFFER_SIZE( uint16_t Host_ACL_Data_Packet_Length,
                                 uint8_t Host_Synchronous_Data_Packet_Length,
                                 uint16_t Host_Total_Num_ACL_Data_Packets,
                                 uint16_t Host_Total_Num_Synchronous_Data_Packets );

tBleStatus HCI_HOST_NUMBER_OF_COMPLETED_PACKETS( uint8_t Number_Of_Handles,
                                                 const Host_Nb_Of_Completed_Pkt_Pair_t* Host_Nb_Of_Completed_Pkt_Pair );

tBleStatus HCI_READ_AFH_CHANNEL_ASSESSMENT_MODE( uint8_t* AFH_Channel_Assessment_Mode );

tBleStatus HCI_WRITE_AFH_CHANNEL_ASSESSMENT_MODE( uint8_t AFH_Channel_Assessment_Mode );

tBleStatus HCI_SET_EVENT_MASK_PAGE_2( const uint8_t* Event_Mask_Page_2 );

tBleStatus HCI_READ_AUTHENTICATED_PAYLOAD_TIMEOUT( uint16_t Connection_Handle,
                                                   uint16_t* Authenticated_Payload_Timeout );

tBleStatus HCI_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT( uint16_t Connection_Handle,
                                                    uint16_t Authenticated_Payload_Timeout );

tBleStatus HCI_SET_ECOSYSTEM_BASE_INTERVAL( uint16_t Interval );

tBleStatus HCI_CONFIGURE_DATA_PATH( uint8_t Data_Path_Direction,
                                    uint8_t Data_Path_ID,
                                    uint8_t Vendor_Specific_Config_Length,
                                    const uint8_t* Vendor_Specific_Config );

tBleStatus HCI_READ_LOCAL_VERSION_INFORMATION( uint8_t* HCI_Version,
                                               uint16_t* HCI_Subversion,
                                               uint8_t* LMP_Version,
                                               uint16_t* Company_Identifier,
                                               uint16_t* LMP_Subversion );

tBleStatus HCI_READ_LOCAL_SUPPORTED_COMMANDS( uint8_t* Supported_Commands );

tBleStatus HCI_READ_LOCAL_SUPPORTED_FEATURES( uint8_t* LMP_Features );

tBleStatus HCI_READ_BD_ADDR( uint8_t* BD_ADDR );

tBleStatus HCI_READ_LOCAL_SUPPORTED_CODECS_V2( uint8_t* Num_Supported_Standard_Codecs,
                                               uint8_t* Standard_Codec,
                                               uint8_t* Num_Supported_Vendor_Specific_Codecs,
                                               uint8_t* Vendor_Specific_Codec );

tBleStatus HCI_READ_LOCAL_SUPPORTED_CODEC_CAPABILITIES( const uint8_t* Codec_ID,
                                                        uint8_t Logical_Transport_Type,
                                                        uint8_t Direction,
                                                        uint8_t* Num_Codec_Capabilities,
                                                        uint8_t* Codec_Capabilities );

tBleStatus HCI_READ_LOCAL_SUPPORTED_CONTROLLER_DELAY( const uint8_t* Codec_ID,
                                                      uint8_t Logical_Transport_Type,
                                                      uint8_t Direction,
                                                      uint8_t Codec_Configuration_Length,
                                                      const uint8_t* Codec_Configuration,
                                                      uint8_t* Min_Controller_Delay,
                                                      uint8_t* Max_Controller_Delay );

tBleStatus HCI_READ_RSSI( uint16_t Connection_Handle,
                          uint8_t* RSSI );

tBleStatus HCI_LE_SET_EVENT_MASK( const uint8_t* LE_Event_Mask );

tBleStatus HCI_LE_READ_BUFFER_SIZE( uint16_t* HC_LE_ACL_Data_Packet_Length,
                                    uint8_t* HC_Total_Num_LE_ACL_Data_Packets );

tBleStatus HCI_LE_READ_LOCAL_SUPPORTED_FEATURES( uint8_t* LE_Features );

tBleStatus HCI_LE_SET_RANDOM_ADDRESS( const uint8_t* Random_Address );

tBleStatus HCI_LE_SET_ADVERTISING_PARAMETERS( uint16_t Advertising_Interval_Min,
                                              uint16_t Advertising_Interval_Max,
                                              uint8_t Advertising_Type,
                                              uint8_t Own_Address_Type,
                                              uint8_t Peer_Address_Type,
                                              const uint8_t* Peer_Address,
                                              uint8_t Advertising_Channel_Map,
                                              uint8_t Advertising_Filter_Policy );

tBleStatus HCI_LE_READ_ADVERTISING_PHYSICAL_CHANNEL_TX_POWER( uint8_t* Transmit_Power_Level );

tBleStatus HCI_LE_SET_ADVERTISING_DATA( uint8_t Advertising_Data_Length,
                                        const uint8_t* Advertising_Data );

tBleStatus HCI_LE_SET_SCAN_RESPONSE_DATA( uint8_t Scan_Response_Data_Length,
                                          const uint8_t* Scan_Response_Data );

tBleStatus HCI_LE_SET_ADVERTISING_ENABLE( uint8_t Advertising_Enable );

tBleStatus HCI_LE_SET_SCAN_PARAMETERS( uint8_t LE_Scan_Type,
                                       uint16_t LE_Scan_Interval,
                                       uint16_t LE_Scan_Window,
                                       uint8_t Own_Address_Type,
                                       uint8_t Scanning_Filter_Policy );

tBleStatus HCI_LE_SET_SCAN_ENABLE( uint8_t LE_Scan_Enable,
                                   uint8_t Filter_Duplicates );

tBleStatus HCI_LE_CREATE_CONNECTION( uint16_t LE_Scan_Interval,
                                     uint16_t LE_Scan_Window,
                                     uint8_t Initiator_Filter_Policy,
                                     uint8_t Peer_Address_Type,
                                     const uint8_t* Peer_Address,
                                     uint8_t Own_Address_Type,
                                     uint16_t Conn_Interval_Min,
                                     uint16_t Conn_Interval_Max,
                                     uint16_t Conn_Latency,
                                     uint16_t Supervision_Timeout,
                                     uint16_t Minimum_CE_Length,
                                     uint16_t Maximum_CE_Length );

tBleStatus HCI_LE_CREATE_CONNECTION_CANCEL( void );

tBleStatus HCI_LE_READ_FILTER_ACCEPT_LIST_SIZE( uint8_t* Filter_Accept_List_Size );

tBleStatus HCI_LE_CLEAR_FILTER_ACCEPT_LIST( void );

tBleStatus HCI_LE_ADD_DEVICE_TO_FILTER_ACCEPT_LIST( uint8_t Address_Type,
                                                    const uint8_t* Address );

tBleStatus HCI_LE_REMOVE_DEVICE_FROM_FILTER_ACCEPT_LIST( uint8_t Address_Type,
                                                         const uint8_t* Address );

tBleStatus HCI_LE_CONNECTION_UPDATE( uint16_t Connection_Handle,
                                     uint16_t Conn_Interval_Min,
                                     uint16_t Conn_Interval_Max,
                                     uint16_t Conn_Latency,
                                     uint16_t Supervision_Timeout,
                                     uint16_t Minimum_CE_Length,
                                     uint16_t Maximum_CE_Length );

tBleStatus HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION( const uint8_t* LE_Channel_Map );

tBleStatus HCI_LE_READ_CHANNEL_MAP( uint16_t Connection_Handle,
                                    uint8_t* LE_Channel_Map );

tBleStatus HCI_LE_READ_REMOTE_FEATURES( uint16_t Connection_Handle );

tBleStatus HCI_LE_ENCRYPT( const uint8_t* Key,
                           const uint8_t* Plaintext_Data,
                           uint8_t* Encrypted_Data );

tBleStatus HCI_LE_RAND( uint8_t* Random_Number );

tBleStatus HCI_LE_ENABLE_ENCRYPTION( uint16_t Connection_Handle,
                                     const uint8_t* Random_Number,
                                     uint16_t Encrypted_Diversifier,
                                     const uint8_t* Long_Term_Key );

tBleStatus HCI_LE_LONG_TERM_KEY_REQUEST_REPLY( uint16_t Connection_Handle,
                                               const uint8_t* Long_Term_Key );

tBleStatus HCI_LE_LONG_TERM_KEY_REQUEST_NEGATIVE_REPLY( uint16_t Connection_Handle );

tBleStatus HCI_LE_READ_SUPPORTED_STATES( uint8_t* LE_States );

tBleStatus HCI_LE_RECEIVER_TEST( uint8_t RX_Frequency );

tBleStatus HCI_LE_TRANSMITTER_TEST( uint8_t TX_Frequency,
                                    uint8_t Length_Of_Test_Data,
                                    uint8_t Packet_Payload );

tBleStatus HCI_LE_TEST_END( uint16_t* Number_Of_Packets );

tBleStatus HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_REPLY( uint16_t Connection_Handle,
                                                             uint16_t Interval_Min,
                                                             uint16_t Interval_Max,
                                                             uint16_t Max_Latency,
                                                             uint16_t Timeout,
                                                             uint16_t Min_CE_Length,
                                                             uint16_t Max_CE_Length );

tBleStatus HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_NEGATIVE_REPLY( uint16_t Connection_Handle,
                                                                      uint8_t Reason );

tBleStatus HCI_LE_SET_DATA_LENGTH( uint16_t Connection_Handle,
                                   uint16_t TxOctets,
                                   uint16_t TxTime );

tBleStatus HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LENGTH( uint16_t* SuggestedMaxTxOctets,
                                                      uint16_t* SuggestedMaxTxTime );

tBleStatus HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH( uint16_t SuggestedMaxTxOctets,
                                                       uint16_t SuggestedMaxTxTime );

tBleStatus HCI_LE_READ_LOCAL_P256_PUBLIC_KEY( void );

tBleStatus HCI_LE_GENERATE_DHKEY( const uint8_t* Remote_P256_Public_Key );

tBleStatus HCI_LE_ADD_DEVICE_TO_RESOLVING_LIST( uint8_t Peer_Identity_Address_Type,
                                                const uint8_t* Peer_Identity_Address,
                                                const uint8_t* Peer_IRK,
                                                const uint8_t* Local_IRK );

tBleStatus HCI_LE_REMOVE_DEVICE_FROM_RESOLVING_LIST( uint8_t Peer_Identity_Address_Type,
                                                     const uint8_t* Peer_Identity_Address );

tBleStatus HCI_LE_CLEAR_RESOLVING_LIST( void );

tBleStatus HCI_LE_READ_RESOLVING_LIST_SIZE( uint8_t* Resolving_List_Size );

tBleStatus HCI_LE_READ_PEER_RESOLVABLE_ADDRESS( uint8_t Peer_Identity_Address_Type,
                                                const uint8_t* Peer_Identity_Address,
                                                uint8_t* Peer_Resolvable_Address );

tBleStatus HCI_LE_READ_LOCAL_RESOLVABLE_ADDRESS( uint8_t Peer_Identity_Address_Type,
                                                 const uint8_t* Peer_Identity_Address,
                                                 uint8_t* Local_Resolvable_Address );

tBleStatus HCI_LE_SET_ADDRESS_RESOLUTION_ENABLE( uint8_t Address_Resolution_Enable );

tBleStatus HCI_LE_SET_RESOLVABLE_PRIVATE_ADDRESS_TIMEOUT( uint16_t RPA_Timeout );

tBleStatus HCI_LE_READ_MAXIMUM_DATA_LENGTH( uint16_t* supportedMaxTxOctets,
                                            uint16_t* supportedMaxTxTime,
                                            uint16_t* supportedMaxRxOctets,
                                            uint16_t* supportedMaxRxTime );

tBleStatus HCI_LE_READ_PHY( uint16_t Connection_Handle,
                            uint8_t* TX_PHY,
                            uint8_t* RX_PHY );

tBleStatus HCI_LE_SET_DEFAULT_PHY( uint8_t ALL_PHYS,
                                   uint8_t TX_PHYS,
                                   uint8_t RX_PHYS );

tBleStatus HCI_LE_SET_PHY( uint16_t Connection_Handle,
                           uint8_t ALL_PHYS,
                           uint8_t TX_PHYS,
                           uint8_t RX_PHYS,
                           uint16_t PHY_options );

tBleStatus HCI_LE_RECEIVER_TEST_V2( uint8_t RX_Frequency,
                                    uint8_t PHY,
                                    uint8_t Modulation_Index );

tBleStatus HCI_LE_TRANSMITTER_TEST_V2( uint8_t TX_Frequency,
                                       uint8_t Length_Of_Test_Data,
                                       uint8_t Packet_Payload,
                                       uint8_t PHY );

tBleStatus HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDRESS( uint8_t Advertising_Handle,
                                                      const uint8_t* Random_Address );

tBleStatus HCI_LE_SET_EXTENDED_ADVERTISING_PARAMETERS( uint8_t Advertising_Handle,
                                                       uint16_t Adv_Event_Properties,
                                                       const uint8_t* Primary_Adv_Interval_Min,
                                                       const uint8_t* Primary_Adv_Interval_Max,
                                                       uint8_t Primary_Adv_Channel_Map,
                                                       uint8_t Own_Address_Type,
                                                       uint8_t Peer_Address_Type,
                                                       const uint8_t* Peer_Address,
                                                       uint8_t Adv_Filter_Policy,
                                                       uint8_t Adv_TX_Power,
                                                       uint8_t Primary_Adv_PHY,
                                                       uint8_t Secondary_Adv_Max_Skip,
                                                       uint8_t Secondary_Adv_PHY,
                                                       uint8_t Adv_SID,
                                                       uint8_t Scan_Req_Notification_Enable,
                                                       uint8_t* Selected_TX_Power );

tBleStatus HCI_LE_SET_EXTENDED_ADVERTISING_DATA( uint8_t Advertising_Handle,
                                                 uint8_t Operation,
                                                 uint8_t Fragment_Preference,
                                                 uint8_t Advertising_Data_Length,
                                                 const uint8_t* Advertising_Data );

tBleStatus HCI_LE_SET_EXTENDED_SCAN_RESPONSE_DATA( uint8_t Advertising_Handle,
                                                   uint8_t Operation,
                                                   uint8_t Fragment_Preference,
                                                   uint8_t Scan_Response_Data_Length,
                                                   const uint8_t* Scan_Response_Data );

tBleStatus HCI_LE_SET_EXTENDED_ADVERTISING_ENABLE( uint8_t Enable,
                                                   uint8_t Num_Sets,
                                                   const Adv_Set_t* Adv_Set );

tBleStatus HCI_LE_READ_MAXIMUM_ADVERTISING_DATA_LENGTH( uint16_t* Max_Advertising_Data_Length );

tBleStatus HCI_LE_READ_NUMBER_OF_SUPPORTED_ADVERTISING_SETS( uint8_t* Num_Supported_Advertising_Sets );

tBleStatus HCI_LE_REMOVE_ADVERTISING_SET( uint8_t Advertising_Handle );

tBleStatus HCI_LE_CLEAR_ADVERTISING_SETS( void );

tBleStatus HCI_LE_SET_PERIODIC_ADVERTISING_PARAMETERS( uint8_t Advertising_Handle,
                                                       uint16_t Periodic_Adv_Interval_Min,
                                                       uint16_t Periodic_Adv_Interval_Max,
                                                       uint16_t Periodic_Adv_Properties );

tBleStatus HCI_LE_SET_PERIODIC_ADVERTISING_DATA( uint8_t Advertising_Handle,
                                                 uint8_t Operation,
                                                 uint8_t Advertising_Data_Length,
                                                 const uint8_t* Advertising_Data );

tBleStatus HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE( uint8_t Enable,
                                                   uint8_t Advertising_Handle );

tBleStatus HCI_LE_SET_EXTENDED_SCAN_PARAMETERS( uint8_t Own_Address_Type,
                                                uint8_t Scanning_Filter_Policy,
                                                uint8_t Scanning_PHYs,
                                                const Scan_Param_Phy_t* Scan_Param_Phy );

tBleStatus HCI_LE_SET_EXTENDED_SCAN_ENABLE( uint8_t Enable,
                                            uint8_t Filter_Duplicates,
                                            uint16_t Duration,
                                            uint16_t Period );

tBleStatus HCI_LE_EXTENDED_CREATE_CONNECTION( uint8_t Initiator_Filter_Policy,
                                              uint8_t Own_Address_Type,
                                              uint8_t Peer_Address_Type,
                                              const uint8_t* Peer_Address,
                                              uint8_t Initiating_PHYs,
                                              const Init_Param_Phy_t* Init_Param_Phy );

tBleStatus HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC( uint8_t Options,
                                                    uint8_t Advertising_SID,
                                                    uint8_t Advertiser_Address_Type,
                                                    const uint8_t* Advertiser_Address,
                                                    uint16_t Skip,
                                                    uint16_t Sync_Timeout,
                                                    uint8_t Sync_CTE_Type );

tBleStatus HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL( void );

tBleStatus HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC( uint16_t Sync_Handle );

tBleStatus HCI_LE_ADD_DEVICE_TO_PERIODIC_ADVERTISER_LIST( uint8_t Advertiser_Address_Type,
                                                          const uint8_t* Advertiser_Address,
                                                          uint8_t Advertising_SID );

tBleStatus HCI_LE_REMOVE_DEVICE_FROM_PERIODIC_ADVERTISER_LIST( uint8_t Advertiser_Address_Type,
                                                               const uint8_t* Advertiser_Address,
                                                               uint8_t Advertising_SID );

tBleStatus HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST( void );

tBleStatus HCI_LE_READ_PERIODIC_ADVERTISER_LIST_SIZE( uint8_t* Periodic_Advertiser_List_Size );

tBleStatus HCI_LE_READ_TRANSMIT_POWER( uint8_t* Min_TX_Power,
                                       uint8_t* Max_TX_Power );

tBleStatus HCI_LE_READ_RF_PATH_COMPENSATION( uint16_t* RF_TX_Path_Compensation,
                                             uint16_t* RF_RX_Path_Compensation );

tBleStatus HCI_LE_WRITE_RF_PATH_COMPENSATION( uint16_t RF_TX_Path_Compensation,
                                              uint16_t RF_RX_Path_Compensation );

tBleStatus HCI_LE_SET_PRIVACY_MODE( uint8_t Peer_Identity_Address_Type,
                                    const uint8_t* Peer_Identity_Address,
                                    uint8_t Privacy_Mode );

tBleStatus HCI_LE_RECEIVER_TEST_V3( uint8_t RX_Frequency,
                                    uint8_t PHY,
                                    uint8_t Modulation_Index,
                                    uint8_t Expected_CTE_Length,
                                    uint8_t Expected_CTE_Type,
                                    uint8_t Slot_Durations,
                                    uint8_t Switching_Pattern_Length,
                                    const uint8_t* Antenna_IDs );

tBleStatus HCI_LE_TRANSMITTER_TEST_V3( uint8_t TX_Frequency,
                                       uint8_t Length_Of_Test_Data,
                                       uint8_t Packet_Payload,
                                       uint8_t PHY,
                                       uint8_t CTE_Length,
                                       uint8_t CTE_Type,
                                       uint8_t Switching_Pattern_Length,
                                       const uint8_t* Antenna_IDs );

tBleStatus HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_PARAMETERS( uint8_t Advertising_Handle,
                                                              uint8_t CTE_Length,
                                                              uint8_t CTE_Type,
                                                              uint8_t CTE_Count,
                                                              uint8_t Switching_Pattern_Length,
                                                              const uint8_t* Antenna_IDs );

tBleStatus HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_ENABLE( uint8_t Advertising_Handle,
                                                          uint8_t CTE_Enable );

tBleStatus HCI_LE_SET_CONNECTIONLESS_IQ_SAMPLING_ENABLE( uint16_t Sync_Handle,
                                                         uint8_t Sampling_Enable,
                                                         uint8_t Slot_Durations,
                                                         uint8_t Max_Sampled_CTEs,
                                                         uint8_t Switching_Pattern_Length,
                                                         const uint8_t* Antenna_IDs );

tBleStatus HCI_LE_SET_CONNECTION_CTE_RECEIVE_PARAMETERS( uint16_t Connection_Handle,
                                                         uint8_t Sampling_Enable,
                                                         uint8_t Slot_Durations,
                                                         uint8_t Switching_Pattern_Length,
                                                         const uint8_t* Antenna_IDs );

tBleStatus HCI_LE_SET_CONNECTION_CTE_TRANSMIT_PARAMETERS( uint16_t Connection_Handle,
                                                          uint8_t CTE_Types,
                                                          uint8_t Switching_Pattern_Length,
                                                          const uint8_t* Antenna_IDs );

tBleStatus HCI_LE_CONNECTION_CTE_REQUEST_ENABLE( uint16_t Connection_Handle,
                                                 uint8_t Enable,
                                                 uint16_t CTE_Request_Interval,
                                                 uint8_t Requested_CTE_Length,
                                                 uint8_t Requested_CTE_Type );

tBleStatus HCI_LE_CONNECTION_CTE_RESPONSE_ENABLE( uint16_t Connection_Handle,
                                                  uint8_t Enable );

tBleStatus HCI_LE_READ_ANTENNA_INFORMATION( uint8_t* Supported_Switching_Sampling_Rates,
                                            uint8_t* Num_Antennae,
                                            uint8_t* Max_Switching_Pattern_Length,
                                            uint8_t* Max_CTE_Length );

tBleStatus HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE( uint16_t Sync_Handle,
                                                           uint8_t Enable );

tBleStatus HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER( uint16_t Connection_Handle,
                                                      uint16_t Service_Data,
                                                      uint16_t Sync_Handle );

tBleStatus HCI_LE_PERIODIC_ADVERTISING_SET_INFO_TRANSFER( uint16_t Connection_Handle,
                                                          uint16_t Service_Data,
                                                          uint8_t Advertising_Handle );

tBleStatus HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS( uint16_t Connection_Handle,
                                                                     uint8_t Mode,
                                                                     uint16_t Skip,
                                                                     uint16_t Sync_Timeout,
                                                                     uint8_t CTE_Type );

tBleStatus HCI_LE_SET_DEFAULT_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS( uint8_t Mode,
                                                                             uint16_t Skip,
                                                                             uint16_t Sync_Timeout,
                                                                             uint8_t CTE_Type );

tBleStatus HCI_LE_GENERATE_DHKEY_V2( const uint8_t* Remote_P256_Public_Key,
                                     uint8_t Key_Type );

tBleStatus HCI_LE_READ_BUFFER_SIZE_V2( uint16_t* LE_ACL_Data_Packet_Length,
                                       uint8_t* Total_Num_LE_ACL_Data_Packets,
                                       uint16_t* ISO_Data_Packet_Length,
                                       uint8_t* Total_Num_ISO_Data_Packets );

tBleStatus HCI_LE_READ_ISO_TX_SYNC( uint16_t Connection_Handle,
                                    uint16_t* Packet_Sequence_Number,
                                    uint32_t* TX_Time_Stamp,
                                    uint8_t* Time_Offset );

tBleStatus HCI_LE_SET_CIG_PARAMETERS( uint8_t CIG_ID,
                                      const uint8_t* SDU_Interval_C_To_P,
                                      const uint8_t* SDU_Interval_P_To_C,
                                      uint8_t Worst_Case_SCA,
                                      uint8_t Packing,
                                      uint8_t Framing,
                                      uint16_t Max_Transport_Latency_C_To_P,
                                      uint16_t Max_Transport_Latency_P_To_C,
                                      uint8_t CIS_Count,
                                      const CIS_cfg_t* CIS_cfg,
                                      uint8_t* CIS_Count_Ret,
                                      uint16_t* Connection_Handle );

tBleStatus HCI_LE_SET_CIG_PARAMETERS_TEST( uint8_t CIG_ID,
                                           const uint8_t* SDU_Interval_C_To_P,
                                           const uint8_t* SDU_Interval_P_To_C,
                                           uint8_t FT_C_To_P,
                                           uint8_t FT_P_To_C,
                                           uint16_t ISO_Interval,
                                           uint8_t Worst_Case_SCA,
                                           uint8_t Packing,
                                           uint8_t Framing,
                                           uint8_t CIS_Count,
                                           const CIS_tst_cfg_t* CIS_tst_cfg,
                                           uint8_t* CIS_Count_Ret,
                                           uint16_t* Connection_Handle );

tBleStatus HCI_LE_CREATE_CIS( uint8_t CIS_Count,
                              const CIS_create_t* CIS_create );

tBleStatus HCI_LE_REMOVE_CIG( uint8_t CIG_ID );

tBleStatus HCI_LE_ACCEPT_CIS_REQUEST( uint16_t Connection_Handle );

tBleStatus HCI_LE_REJECT_CIS_REQUEST( uint16_t Connection_Handle,
                                      uint8_t Reason );

tBleStatus HCI_LE_CREATE_BIG( uint8_t BIG_Handle,
                              uint8_t Advertising_Handle,
                              uint8_t Num_BIS,
                              const uint8_t* SDU_Interval,
                              uint16_t Max_SDU,
                              uint16_t Max_Transport_Latency,
                              uint8_t RTN,
                              uint8_t PHY,
                              uint8_t Packing,
                              uint8_t Framing,
                              uint8_t Encryption,
                              const uint8_t* Broadcast_Code );

tBleStatus HCI_LE_CREATE_BIG_TEST( uint8_t BIG_Handle,
                                   uint8_t Advertising_Handle,
                                   uint8_t Num_BIS,
                                   const uint8_t* SDU_Interval,
                                   uint16_t ISO_Interval,
                                   uint8_t NSE,
                                   uint16_t Max_SDU,
                                   uint16_t Max_PDU,
                                   uint8_t PHY,
                                   uint8_t Packing,
                                   uint8_t Framing,
                                   uint8_t BN,
                                   uint8_t IRC,
                                   uint8_t PTO,
                                   uint8_t Encryption,
                                   const uint8_t* Broadcast_Code );

tBleStatus HCI_LE_TERMINATE_BIG( uint8_t BIG_Handle,
                                 uint8_t Reason );

tBleStatus HCI_LE_BIG_CREATE_SYNC( uint8_t BIG_Handle,
                                   uint16_t Sync_Handle,
                                   uint8_t Encryption,
                                   const uint8_t* Broadcast_Code,
                                   uint8_t MSE,
                                   uint16_t BIG_Sync_Timeout,
                                   uint8_t Num_BIS,
                                   const uint8_t* BIS );

tBleStatus HCI_LE_BIG_TERMINATE_SYNC( uint8_t BIG_Handle );

tBleStatus HCI_LE_REQUEST_PEER_SCA( uint16_t Connection_Handle );

tBleStatus HCI_LE_SETUP_ISO_DATA_PATH( uint16_t Connection_Handle,
                                       uint8_t Data_Path_Direction,
                                       uint8_t Data_Path_ID,
                                       const uint8_t* Codec_ID,
                                       const uint8_t* Controller_Delay,
                                       uint8_t Codec_Configuration_Length,
                                       const uint8_t* Codec_Configuration );

tBleStatus HCI_LE_REMOVE_ISO_DATA_PATH( uint16_t Connection_Handle,
                                        uint8_t Data_Path_Direction );

tBleStatus HCI_LE_ISO_TRANSMIT_TEST( uint16_t Connection_Handle,
                                     uint8_t Payload_Type );

tBleStatus HCI_LE_ISO_RECEIVE_TEST( uint16_t Connection_Handle,
                                    uint8_t Payload_Type );

tBleStatus HCI_LE_ISO_READ_TEST_COUNTERS( uint16_t Connection_Handle,
                                          uint32_t* Received_SDU_Count,
                                          uint32_t* Missed_SDU_Count,
                                          uint32_t* Failed_SDU_Count );

tBleStatus HCI_LE_ISO_TEST_END( uint16_t Connection_Handle,
                                uint32_t* Received_SDU_Count,
                                uint32_t* Missed_SDU_Count,
                                uint32_t* Failed_SDU_Count );

tBleStatus HCI_LE_SET_HOST_FEATURE( uint8_t Bit_Number,
                                    uint8_t Bit_Value );

tBleStatus HCI_LE_READ_ISO_LINK_QUALITY( uint16_t Connection_Handle,
                                         uint32_t* TX_UnACKed_Packets,
                                         uint32_t* TX_Flushed_Packets,
                                         uint32_t* TX_Last_Subevent_Packets,
                                         uint32_t* Retransmitted_Packets,
                                         uint32_t* CRC_Error_Packets,
                                         uint32_t* RX_Unreceived_Packets,
                                         uint32_t* Duplicate_Packets );

tBleStatus HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL( uint16_t Connection_Handle,
                                                      uint8_t PHY,
                                                      uint8_t* Current_TX_Power_Level,
                                                      uint8_t* Max_TX_Power_Level );

tBleStatus HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL( uint16_t Connection_Handle,
                                                    uint8_t PHY );

tBleStatus HCI_LE_SET_PATH_LOSS_REPORTING_PARAMETERS( uint16_t Connection_Handle,
                                                      uint8_t High_Threshold,
                                                      uint8_t High_Hysteresis,
                                                      uint8_t Low_Threshold,
                                                      uint8_t Low_Hysteresis,
                                                      uint16_t Min_Time_Spent );

tBleStatus HCI_LE_SET_PATH_LOSS_REPORTING_ENABLE( uint16_t Connection_Handle,
                                                  uint8_t Enable );

tBleStatus HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE( uint16_t Connection_Handle,
                                                       uint8_t Local_Enable,
                                                       uint8_t Remote_Enable );

tBleStatus HCI_LE_TRANSMITTER_TEST_V4( uint8_t TX_Frequency,
                                       uint8_t Length_Of_Test_Data,
                                       uint8_t Packet_Payload,
                                       uint8_t PHY,
                                       uint8_t CTE_Length,
                                       uint8_t CTE_Type,
                                       uint8_t Switching_Pattern_Length,
                                       const uint8_t* Antenna_IDs,
                                       uint8_t TX_Power_Level );

tBleStatus HCI_LE_SET_DATA_RELATED_ADDRESS_CHANGES( uint8_t Advertising_Handle,
                                                    uint8_t Change_Reasons );

tBleStatus HCI_LE_SET_DEFAULT_SUBRATE( uint16_t Subrate_Min,
                                       uint16_t Subrate_Max,
                                       uint16_t Max_Latency,
                                       uint16_t Continuation_Number,
                                       uint16_t Supervision_Timeout );

tBleStatus HCI_LE_SUBRATE_REQUEST( uint16_t Connection_Handle,
                                   uint16_t Subrate_Min,
                                   uint16_t Subrate_Max,
                                   uint16_t Max_Latency,
                                   uint16_t Continuation_Number,
                                   uint16_t Supervision_Timeout );

tBleStatus HCI_LE_SET_EXTENDED_ADVERTISING_PARAMETERS_V2( uint8_t Advertising_Handle,
                                                          uint16_t Adv_Event_Properties,
                                                          const uint8_t* Primary_Adv_Interval_Min,
                                                          const uint8_t* Primary_Adv_Interval_Max,
                                                          uint8_t Primary_Adv_Channel_Map,
                                                          uint8_t Own_Address_Type,
                                                          uint8_t Peer_Address_Type,
                                                          const uint8_t* Peer_Address,
                                                          uint8_t Adv_Filter_Policy,
                                                          uint8_t Adv_TX_Power,
                                                          uint8_t Primary_Adv_PHY,
                                                          uint8_t Secondary_Adv_Max_Skip,
                                                          uint8_t Secondary_Adv_PHY,
                                                          uint8_t Adv_SID,
                                                          uint8_t Scan_Req_Notification_Enable,
                                                          uint8_t Primary_Adv_PHY_Options,
                                                          uint8_t Secondary_Adv_PHY_Options,
                                                          uint8_t* Selected_TX_Power );

tBleStatus ACI_HAL_GET_FW_BUILD_NUMBER( uint16_t* Build_Number );

tBleStatus ACI_HAL_WRITE_CONFIG_DATA( uint8_t Offset,
                                      uint8_t Length,
                                      const uint8_t* Value );

tBleStatus ACI_HAL_READ_CONFIG_DATA( uint8_t Offset,
                                     uint8_t* Data_Length,
                                     uint8_t* Data );

tBleStatus ACI_HAL_SET_TX_POWER_LEVEL( uint8_t En_High_Power,
                                       uint8_t PA_Level );

tBleStatus ACI_HAL_LE_TX_TEST_PACKET_NUMBER( uint32_t* Number_Of_Packets );

tBleStatus ACI_HAL_TONE_START( uint8_t RF_Channel,
                               uint8_t Freq_offset );

tBleStatus ACI_HAL_TONE_STOP( void );

tBleStatus ACI_HAL_SET_RADIO_ACTIVITY_MASK( uint16_t Radio_Activity_Mask );

tBleStatus ACI_HAL_SET_EVENT_MASK( uint32_t Event_Mask );

tBleStatus ACI_HAL_GET_LINK_STATUS_V2( uint8_t* Link_Status,
                                       uint16_t* Link_Connection_Handle );

tBleStatus ACI_HAL_GET_PM_DEBUG_INFO_V2( uint16_t* Allocated_For_TX,
                                         uint16_t* Allocated_For_RX,
                                         uint16_t* Allocated_MBlocks );

tBleStatus ACI_HAL_SET_PERIPHERAL_LATENCY( uint8_t Enable );

tBleStatus ACI_HAL_SET_SYNC_EVENT_CONFIG( uint8_t Group_Id,
                                          uint8_t Enable_Sync,
                                          uint8_t Enable_Cb_Trigger,
                                          uint8_t Trigger_Source );

tBleStatus ACI_HAL_READ_RSSI( uint8_t* RSSI );

tBleStatus ACI_HAL_CONTINUOUS_TX_START( uint8_t RF_Channel,
                                        uint8_t PHY,
                                        uint8_t Pattern );

tBleStatus ACI_HAL_EAD_ENCRYPT_DECRYPT( uint8_t Mode,
                                        const uint8_t* Key,
                                        const uint8_t* IV,
                                        uint16_t In_Data_Length,
                                        const uint8_t* In_Data,
                                        uint16_t* Out_Data_Length,
                                        uint8_t* Out_Data );

tBleStatus ACI_GAP_SET_NON_DISCOVERABLE( void );

tBleStatus ACI_GAP_SET_LIMITED_DISCOVERABLE( uint8_t Advertising_Type,
                                             uint16_t Advertising_Interval_Min,
                                             uint16_t Advertising_Interval_Max,
                                             uint8_t Own_Address_Type,
                                             uint8_t Advertising_Filter_Policy,
                                             uint8_t Local_Name_Length,
                                             const uint8_t* Local_Name,
                                             uint8_t Service_Uuid_length,
                                             const uint8_t* Service_Uuid_List,
                                             uint16_t Conn_Interval_Min,
                                             uint16_t Conn_Interval_Max );

tBleStatus ACI_GAP_SET_DISCOVERABLE( uint8_t Advertising_Type,
                                     uint16_t Advertising_Interval_Min,
                                     uint16_t Advertising_Interval_Max,
                                     uint8_t Own_Address_Type,
                                     uint8_t Advertising_Filter_Policy,
                                     uint8_t Local_Name_Length,
                                     const uint8_t* Local_Name,
                                     uint8_t Service_Uuid_length,
                                     const uint8_t* Service_Uuid_List,
                                     uint16_t Conn_Interval_Min,
                                     uint16_t Conn_Interval_Max );

tBleStatus ACI_GAP_SET_DIRECT_CONNECTABLE( uint8_t Own_Address_Type,
                                           uint8_t Directed_Advertising_Type,
                                           uint8_t Direct_Address_Type,
                                           const uint8_t* Direct_Address,
                                           uint16_t Advertising_Interval_Min,
                                           uint16_t Advertising_Interval_Max );

tBleStatus ACI_GAP_SET_IO_CAPABILITY( uint8_t IO_Capability );

tBleStatus ACI_GAP_SET_AUTHENTICATION_REQUIREMENT( uint8_t Bonding_Mode,
                                                   uint8_t MITM_Mode,
                                                   uint8_t SC_Support,
                                                   uint8_t KeyPress_Notification_Support,
                                                   uint8_t Min_Encryption_Key_Size,
                                                   uint8_t Max_Encryption_Key_Size,
                                                   uint8_t Use_Fixed_Pin,
                                                   uint32_t Fixed_Pin,
                                                   uint8_t Identity_Address_Type );

tBleStatus ACI_GAP_SET_AUTHORIZATION_REQUIREMENT( uint16_t Connection_Handle,
                                                  uint8_t Authorization_Enable );

tBleStatus ACI_GAP_PASS_KEY_RESP( uint16_t Connection_Handle,
                                  uint32_t Pass_Key );

tBleStatus ACI_GAP_AUTHORIZATION_RESP( uint16_t Connection_Handle,
                                       uint8_t Authorize );

tBleStatus ACI_GAP_INIT( uint8_t Role,
                         uint8_t privacy_enabled,
                         uint8_t device_name_char_len,
                         uint16_t* Service_Handle,
                         uint16_t* Dev_Name_Char_Handle,
                         uint16_t* Appearance_Char_Handle );

tBleStatus ACI_GAP_SET_NON_CONNECTABLE( uint8_t Advertising_Event_Type,
                                        uint8_t Own_Address_Type );

tBleStatus ACI_GAP_SET_UNDIRECTED_CONNECTABLE( uint16_t Advertising_Interval_Min,
                                               uint16_t Advertising_Interval_Max,
                                               uint8_t Own_Address_Type,
                                               uint8_t Adv_Filter_Policy );

tBleStatus ACI_GAP_PERIPHERAL_SECURITY_REQ( uint16_t Connection_Handle );

tBleStatus ACI_GAP_UPDATE_ADV_DATA( uint8_t AdvDataLen,
                                    const uint8_t* AdvData );

tBleStatus ACI_GAP_DELETE_AD_TYPE( uint8_t ADType );

tBleStatus ACI_GAP_GET_SECURITY_LEVEL( uint16_t Connection_Handle,
                                       uint8_t* Security_Mode,
                                       uint8_t* Security_Level );

tBleStatus ACI_GAP_SET_EVENT_MASK( uint16_t GAP_Evt_Mask );

tBleStatus ACI_GAP_CONFIGURE_FILTER_ACCEPT_LIST( void );

tBleStatus ACI_GAP_TERMINATE( uint16_t Connection_Handle,
                              uint8_t Reason );

tBleStatus ACI_GAP_CLEAR_SECURITY_DB( void );

tBleStatus ACI_GAP_ALLOW_REBOND( uint16_t Connection_Handle );

tBleStatus ACI_GAP_START_LIMITED_DISCOVERY_PROC( uint16_t LE_Scan_Interval,
                                                 uint16_t LE_Scan_Window,
                                                 uint8_t Own_Address_Type,
                                                 uint8_t Filter_Duplicates );

tBleStatus ACI_GAP_START_GENERAL_DISCOVERY_PROC( uint16_t LE_Scan_Interval,
                                                 uint16_t LE_Scan_Window,
                                                 uint8_t Own_Address_Type,
                                                 uint8_t Filter_Duplicates );

tBleStatus ACI_GAP_START_AUTO_CONNECTION_ESTABLISH_PROC( uint16_t LE_Scan_Interval,
                                                         uint16_t LE_Scan_Window,
                                                         uint8_t Own_Address_Type,
                                                         uint16_t Conn_Interval_Min,
                                                         uint16_t Conn_Interval_Max,
                                                         uint16_t Conn_Latency,
                                                         uint16_t Supervision_Timeout,
                                                         uint16_t Minimum_CE_Length,
                                                         uint16_t Maximum_CE_Length,
                                                         uint8_t Num_of_Peer_Entries,
                                                         const Peer_Entry_t* Peer_Entry );

tBleStatus ACI_GAP_START_GENERAL_CONNECTION_ESTABLISH_PROC( uint8_t LE_Scan_Type,
                                                            uint16_t LE_Scan_Interval,
                                                            uint16_t LE_Scan_Window,
                                                            uint8_t Own_Address_Type,
                                                            uint8_t Scanning_Filter_Policy,
                                                            uint8_t Filter_Duplicates );

tBleStatus ACI_GAP_START_SELECTIVE_CONNECTION_ESTABLISH_PROC( uint8_t LE_Scan_Type,
                                                              uint16_t LE_Scan_Interval,
                                                              uint16_t LE_Scan_Window,
                                                              uint8_t Own_Address_Type,
                                                              uint8_t Scanning_Filter_Policy,
                                                              uint8_t Filter_Duplicates,
                                                              uint8_t Num_of_Peer_Entries,
                                                              const Peer_Entry_t* Peer_Entry );

tBleStatus ACI_GAP_CREATE_CONNECTION( uint16_t LE_Scan_Interval,
                                      uint16_t LE_Scan_Window,
                                      uint8_t Peer_Address_Type,
                                      const uint8_t* Peer_Address,
                                      uint8_t Own_Address_Type,
                                      uint16_t Conn_Interval_Min,
                                      uint16_t Conn_Interval_Max,
                                      uint16_t Conn_Latency,
                                      uint16_t Supervision_Timeout,
                                      uint16_t Minimum_CE_Length,
                                      uint16_t Maximum_CE_Length );

tBleStatus ACI_GAP_TERMINATE_GAP_PROC( uint8_t Procedure_Code );

tBleStatus ACI_GAP_START_CONNECTION_UPDATE( uint16_t Connection_Handle,
                                            uint16_t Conn_Interval_Min,
                                            uint16_t Conn_Interval_Max,
                                            uint16_t Conn_Latency,
                                            uint16_t Supervision_Timeout,
                                            uint16_t Minimum_CE_Length,
                                            uint16_t Maximum_CE_Length );

tBleStatus ACI_GAP_SEND_PAIRING_REQ( uint16_t Connection_Handle,
                                     uint8_t Force_Rebond );

tBleStatus ACI_GAP_SET_BROADCAST_MODE( uint16_t Advertising_Interval_Min,
                                       uint16_t Advertising_Interval_Max,
                                       uint8_t Advertising_Type,
                                       uint8_t Own_Address_Type,
                                       uint8_t Adv_Data_Length,
                                       const uint8_t* Adv_Data,
                                       uint8_t Num_of_Peer_Entries,
                                       const Peer_Entry_t* Peer_Entry );

tBleStatus ACI_GAP_START_OBSERVATION_PROC( uint16_t LE_Scan_Interval,
                                           uint16_t LE_Scan_Window,
                                           uint8_t LE_Scan_Type,
                                           uint8_t Own_Address_Type,
                                           uint8_t Filter_Duplicates,
                                           uint8_t Scanning_Filter_Policy );

tBleStatus ACI_GAP_GET_BONDED_DEVICES( uint8_t* Num_of_Addresses,
                                       Bonded_Device_Entry_t* Bonded_Device_Entry );

tBleStatus ACI_GAP_CHECK_BONDED_DEVICE( uint8_t Peer_Address_Type,
                                        const uint8_t* Peer_Address,
                                        uint8_t* Id_Address_Type,
                                        uint8_t* Id_Address );

tBleStatus ACI_GAP_NUMERIC_COMPARISON_VALUE_CONFIRM_YESNO( uint16_t Connection_Handle,
                                                           uint8_t Confirm_Yes_No );

tBleStatus ACI_GAP_PASSKEY_INPUT( uint16_t Connection_Handle,
                                  uint8_t Input_Type );

tBleStatus ACI_GAP_GET_OOB_DATA( uint8_t OOB_Data_Type,
                                 uint8_t* Address_Type,
                                 uint8_t* Address,
                                 uint8_t* OOB_Data_Len,
                                 uint8_t* OOB_Data );

tBleStatus ACI_GAP_SET_OOB_DATA( uint8_t Device_Type,
                                 uint8_t Address_Type,
                                 const uint8_t* Address,
                                 uint8_t OOB_Data_Type,
                                 uint8_t OOB_Data_Len,
                                 const uint8_t* OOB_Data );

tBleStatus ACI_GAP_ADD_DEVICES_TO_RESOLVING_LIST( uint8_t Num_of_Resolving_list_Entries,
                                                  const Identity_Entry_t* Identity_Entry,
                                                  uint8_t Clear_Resolving_List );

tBleStatus ACI_GAP_REMOVE_BONDED_DEVICE( uint8_t Peer_Identity_Address_Type,
                                         const uint8_t* Peer_Identity_Address );

tBleStatus ACI_GAP_ADD_DEVICES_TO_LIST( uint8_t Num_of_List_Entries,
                                        const List_Entry_t* List_Entry,
                                        uint8_t Mode );

tBleStatus ACI_GAP_PAIRING_REQUEST_REPLY( uint16_t Connection_Handle,
                                          uint8_t Accept );

tBleStatus ACI_GAP_ADV_SET_CONFIGURATION( uint8_t Adv_Mode,
                                          uint8_t Advertising_Handle,
                                          uint16_t Adv_Event_Properties,
                                          uint32_t Primary_Adv_Interval_Min,
                                          uint32_t Primary_Adv_Interval_Max,
                                          uint8_t Primary_Adv_Channel_Map,
                                          uint8_t Own_Address_Type,
                                          uint8_t Peer_Address_Type,
                                          const uint8_t* Peer_Address,
                                          uint8_t Adv_Filter_Policy,
                                          uint8_t Adv_TX_Power,
                                          uint8_t Secondary_Adv_Max_Skip,
                                          uint8_t Secondary_Adv_PHY,
                                          uint8_t Adv_SID,
                                          uint8_t Scan_Req_Notification_Enable );

tBleStatus ACI_GAP_ADV_SET_ENABLE( uint8_t Enable,
                                   uint8_t Num_Sets,
                                   const Adv_Set_t* Adv_Set );

tBleStatus ACI_GAP_ADV_SET_ADV_DATA( uint8_t Advertising_Handle,
                                     uint8_t Operation,
                                     uint8_t Fragment_Preference,
                                     uint8_t Advertising_Data_Length,
                                     const uint8_t* Advertising_Data );

tBleStatus ACI_GAP_ADV_SET_SCAN_RESP_DATA( uint8_t Advertising_Handle,
                                           uint8_t Operation,
                                           uint8_t Fragment_Preference,
                                           uint8_t Scan_Response_Data_Length,
                                           const uint8_t* Scan_Response_Data );

tBleStatus ACI_GAP_ADV_REMOVE_SET( uint8_t Advertising_Handle );

tBleStatus ACI_GAP_ADV_CLEAR_SETS( void );

tBleStatus ACI_GAP_ADV_SET_RANDOM_ADDRESS( uint8_t Advertising_Handle,
                                           const uint8_t* Random_Address );

tBleStatus ACI_GAP_ADV_SET_PERIODIC_PARAMETERS( uint8_t Advertising_Handle,
                                                uint16_t Periodic_Adv_Interval_Min,
                                                uint16_t Periodic_Adv_Interval_Max,
                                                uint16_t Periodic_Adv_Properties,
                                                uint8_t Num_Subevents,
                                                uint8_t Subevent_Interval,
                                                uint8_t Response_Slot_Delay,
                                                uint8_t Response_Slot_Spacing,
                                                uint8_t Num_Response_Slots );

tBleStatus ACI_GAP_ADV_SET_PERIODIC_DATA( uint8_t Advertising_Handle,
                                          uint8_t Operation,
                                          uint8_t Advertising_Data_Length,
                                          const uint8_t* Advertising_Data );

tBleStatus ACI_GAP_ADV_SET_PERIODIC_ENABLE( uint8_t Enable,
                                            uint8_t Advertising_Handle );

tBleStatus ACI_GAP_ADV_SET_CONFIGURATION_V2( uint8_t Adv_Mode,
                                             uint8_t Advertising_Handle,
                                             uint16_t Adv_Event_Properties,
                                             uint32_t Primary_Adv_Interval_Min,
                                             uint32_t Primary_Adv_Interval_Max,
                                             uint8_t Primary_Adv_Channel_Map,
                                             uint8_t Own_Address_Type,
                                             uint8_t Peer_Address_Type,
                                             const uint8_t* Peer_Address,
                                             uint8_t Adv_Filter_Policy,
                                             uint8_t Adv_TX_Power,
                                             uint8_t Primary_Adv_PHY,
                                             uint8_t Secondary_Adv_Max_Skip,
                                             uint8_t Secondary_Adv_PHY,
                                             uint8_t Adv_SID,
                                             uint8_t Scan_Req_Notification_Enable,
                                             uint8_t Primary_Adv_PHY_Options,
                                             uint8_t Secondary_Adv_PHY_Options );

tBleStatus ACI_GAP_EXT_START_SCAN( uint8_t Scan_Mode,
                                   uint8_t Procedure,
                                   uint8_t Own_Address_Type,
                                   uint8_t Filter_Duplicates,
                                   uint16_t Duration,
                                   uint16_t Period,
                                   uint8_t Scanning_Filter_Policy,
                                   uint8_t Scanning_PHYs,
                                   const Scan_Param_Phy_t* Scan_Param_Phy );

tBleStatus ACI_GAP_EXT_CREATE_CONNECTION( uint8_t Initiating_Mode,
                                          uint8_t Procedure,
                                          uint8_t Own_Address_Type,
                                          uint8_t Peer_Address_Type,
                                          const uint8_t* Peer_Address,
                                          uint8_t Advertising_Handle,
                                          uint8_t Subevent,
                                          uint8_t Initiator_Filter_Policy,
                                          uint8_t Initiating_PHYs,
                                          const Init_Param_Phy_t* Init_Param_Phy );

tBleStatus ACI_GATT_INIT( void );

tBleStatus ACI_GATT_ADD_SERVICE( uint8_t Service_UUID_Type,
                                 const Service_UUID_t* Service_UUID,
                                 uint8_t Service_Type,
                                 uint8_t Max_Attribute_Records,
                                 uint16_t* Service_Handle );

tBleStatus ACI_GATT_INCLUDE_SERVICE( uint16_t Service_Handle,
                                     uint16_t Include_Start_Handle,
                                     uint16_t Include_End_Handle,
                                     uint8_t Include_UUID_Type,
                                     const Include_UUID_t* Include_UUID,
                                     uint16_t* Include_Handle );

tBleStatus ACI_GATT_ADD_CHAR( uint16_t Service_Handle,
                              uint8_t Char_UUID_Type,
                              const Char_UUID_t* Char_UUID,
                              uint16_t Char_Value_Length,
                              uint8_t Char_Properties,
                              uint8_t Security_Permissions,
                              uint8_t GATT_Evt_Mask,
                              uint8_t Enc_Key_Size,
                              uint8_t Is_Variable,
                              uint16_t* Char_Handle );

tBleStatus ACI_GATT_ADD_CHAR_DESC( uint16_t Service_Handle,
                                   uint16_t Char_Handle,
                                   uint8_t Char_Desc_Uuid_Type,
                                   const Char_Desc_Uuid_t* Char_Desc_Uuid,
                                   uint8_t Char_Desc_Value_Max_Len,
                                   uint8_t Char_Desc_Value_Length,
                                   const uint8_t* Char_Desc_Value,
                                   uint8_t Security_Permissions,
                                   uint8_t Access_Permissions,
                                   uint8_t GATT_Evt_Mask,
                                   uint8_t Enc_Key_Size,
                                   uint8_t Is_Variable,
                                   uint16_t* Char_Desc_Handle );

tBleStatus ACI_GATT_UPDATE_CHAR_VALUE( uint16_t Service_Handle,
                                       uint16_t Char_Handle,
                                       uint8_t Val_Offset,
                                       uint8_t Char_Value_Length,
                                       const uint8_t* Char_Value );

tBleStatus ACI_GATT_DEL_CHAR( uint16_t Serv_Handle,
                              uint16_t Char_Handle );

tBleStatus ACI_GATT_DEL_SERVICE( uint16_t Serv_Handle );

tBleStatus ACI_GATT_DEL_INCLUDE_SERVICE( uint16_t Serv_Handle,
                                         uint16_t Include_Handle );

tBleStatus ACI_GATT_SET_EVENT_MASK( uint32_t GATT_Evt_Mask );

tBleStatus ACI_GATT_EXCHANGE_CONFIG( uint16_t Connection_Handle );

tBleStatus ACI_ATT_FIND_INFO_REQ( uint16_t Connection_Handle,
                                  uint16_t Start_Handle,
                                  uint16_t End_Handle );

tBleStatus ACI_ATT_FIND_BY_TYPE_VALUE_REQ( uint16_t Connection_Handle,
                                           uint16_t Start_Handle,
                                           uint16_t End_Handle,
                                           uint16_t UUID,
                                           uint8_t Attribute_Val_Length,
                                           const uint8_t* Attribute_Val );

tBleStatus ACI_ATT_READ_BY_TYPE_REQ( uint16_t Connection_Handle,
                                     uint16_t Start_Handle,
                                     uint16_t End_Handle,
                                     uint8_t UUID_Type,
                                     const UUID_t* UUID );

tBleStatus ACI_ATT_READ_BY_GROUP_TYPE_REQ( uint16_t Connection_Handle,
                                           uint16_t Start_Handle,
                                           uint16_t End_Handle,
                                           uint8_t UUID_Type,
                                           const UUID_t* UUID );

tBleStatus ACI_ATT_PREPARE_WRITE_REQ( uint16_t Connection_Handle,
                                      uint16_t Attr_Handle,
                                      uint16_t Val_Offset,
                                      uint8_t Attribute_Val_Length,
                                      const uint8_t* Attribute_Val );

tBleStatus ACI_ATT_EXECUTE_WRITE_REQ( uint16_t Connection_Handle,
                                      uint8_t Execute );

tBleStatus ACI_GATT_DISC_ALL_PRIMARY_SERVICES( uint16_t Connection_Handle );

tBleStatus ACI_GATT_DISC_PRIMARY_SERVICE_BY_UUID( uint16_t Connection_Handle,
                                                  uint8_t UUID_Type,
                                                  const UUID_t* UUID );

tBleStatus ACI_GATT_FIND_INCLUDED_SERVICES( uint16_t Connection_Handle,
                                            uint16_t Start_Handle,
                                            uint16_t End_Handle );

tBleStatus ACI_GATT_DISC_ALL_CHAR_OF_SERVICE( uint16_t Connection_Handle,
                                              uint16_t Start_Handle,
                                              uint16_t End_Handle );

tBleStatus ACI_GATT_DISC_CHAR_BY_UUID( uint16_t Connection_Handle,
                                       uint16_t Start_Handle,
                                       uint16_t End_Handle,
                                       uint8_t UUID_Type,
                                       const UUID_t* UUID );

tBleStatus ACI_GATT_DISC_ALL_CHAR_DESC( uint16_t Connection_Handle,
                                        uint16_t Char_Handle,
                                        uint16_t End_Handle );

tBleStatus ACI_GATT_READ_CHAR_VALUE( uint16_t Connection_Handle,
                                     uint16_t Attr_Handle );

tBleStatus ACI_GATT_READ_USING_CHAR_UUID( uint16_t Connection_Handle,
                                          uint16_t Start_Handle,
                                          uint16_t End_Handle,
                                          uint8_t UUID_Type,
                                          const UUID_t* UUID );

tBleStatus ACI_GATT_READ_LONG_CHAR_VALUE( uint16_t Connection_Handle,
                                          uint16_t Attr_Handle,
                                          uint16_t Val_Offset );

tBleStatus ACI_GATT_READ_MULTIPLE_CHAR_VALUE( uint16_t Connection_Handle,
                                              uint8_t Number_of_Handles,
                                              const Handle_Entry_t* Handle_Entry );

tBleStatus ACI_GATT_WRITE_CHAR_VALUE( uint16_t Connection_Handle,
                                      uint16_t Attr_Handle,
                                      uint8_t Attribute_Val_Length,
                                      const uint8_t* Attribute_Val );

tBleStatus ACI_GATT_WRITE_LONG_CHAR_VALUE( uint16_t Connection_Handle,
                                           uint16_t Attr_Handle,
                                           uint16_t Val_Offset,
                                           uint8_t Attribute_Val_Length,
                                           const uint8_t* Attribute_Val );

tBleStatus ACI_GATT_WRITE_CHAR_RELIABLE( uint16_t Connection_Handle,
                                         uint16_t Attr_Handle,
                                         uint16_t Val_Offset,
                                         uint8_t Attribute_Val_Length,
                                         const uint8_t* Attribute_Val );

tBleStatus ACI_GATT_WRITE_LONG_CHAR_DESC( uint16_t Connection_Handle,
                                          uint16_t Attr_Handle,
                                          uint16_t Val_Offset,
                                          uint8_t Attribute_Val_Length,
                                          const uint8_t* Attribute_Val );

tBleStatus ACI_GATT_READ_LONG_CHAR_DESC( uint16_t Connection_Handle,
                                         uint16_t Attr_Handle,
                                         uint16_t Val_Offset );

tBleStatus ACI_GATT_WRITE_CHAR_DESC( uint16_t Connection_Handle,
                                     uint16_t Attr_Handle,
                                     uint8_t Attribute_Val_Length,
                                     const uint8_t* Attribute_Val );

tBleStatus ACI_GATT_READ_CHAR_DESC( uint16_t Connection_Handle,
                                    uint16_t Attr_Handle );

tBleStatus ACI_GATT_WRITE_WITHOUT_RESP( uint16_t Connection_Handle,
                                        uint16_t Attr_Handle,
                                        uint8_t Attribute_Val_Length,
                                        const uint8_t* Attribute_Val );

tBleStatus ACI_GATT_SIGNED_WRITE_WITHOUT_RESP( uint16_t Connection_Handle,
                                               uint16_t Attr_Handle,
                                               uint8_t Attribute_Val_Length,
                                               const uint8_t* Attribute_Val );

tBleStatus ACI_GATT_CONFIRM_INDICATION( uint16_t Connection_Handle );

tBleStatus ACI_GATT_WRITE_RESP( uint16_t Connection_Handle,
                                uint16_t Attr_Handle,
                                uint8_t Write_status,
                                uint8_t Error_Code,
                                uint8_t Attribute_Val_Length,
                                const uint8_t* Attribute_Val );

tBleStatus ACI_GATT_ALLOW_READ( uint16_t Connection_Handle );

tBleStatus ACI_GATT_SET_SECURITY_PERMISSION( uint16_t Serv_Handle,
                                             uint16_t Attr_Handle,
                                             uint8_t Security_Permissions );

tBleStatus ACI_GATT_SET_DESC_VALUE( uint16_t Serv_Handle,
                                    uint16_t Char_Handle,
                                    uint16_t Char_Desc_Handle,
                                    uint16_t Val_Offset,
                                    uint8_t Char_Desc_Value_Length,
                                    const uint8_t* Char_Desc_Value );

tBleStatus ACI_GATT_READ_HANDLE_VALUE( uint16_t Attr_Handle,
                                       uint16_t Offset,
                                       uint16_t Value_Length_Requested,
                                       uint16_t* Length,
                                       uint16_t* Value_Length,
                                       uint8_t* Value );

tBleStatus ACI_GATT_UPDATE_CHAR_VALUE_EXT( uint16_t Conn_Handle_To_Notify,
                                           uint16_t Service_Handle,
                                           uint16_t Char_Handle,
                                           uint8_t Update_Type,
                                           uint16_t Char_Length,
                                           uint16_t Value_Offset,
                                           uint8_t Value_Length,
                                           const uint8_t* Value );

tBleStatus ACI_GATT_DENY_READ( uint16_t Connection_Handle,
                               uint8_t Error_Code );

tBleStatus ACI_GATT_SET_ACCESS_PERMISSION( uint16_t Serv_Handle,
                                           uint16_t Attr_Handle,
                                           uint8_t Access_Permissions );

tBleStatus ACI_GATT_STORE_DB( void );

tBleStatus ACI_GATT_SEND_MULT_NOTIFICATION( uint16_t Connection_Handle,
                                            uint8_t Number_of_Handles,
                                            const Handle_Entry_t* Handle_Entry );

tBleStatus ACI_GATT_READ_MULTIPLE_VAR_CHAR_VALUE( uint16_t Connection_Handle,
                                                  uint8_t Number_of_Handles,
                                                  const Handle_Entry_t* Handle_Entry );

tBleStatus ACI_L2CAP_CONNECTION_PARAMETER_UPDATE_REQ( uint16_t Connection_Handle,
                                                      uint16_t Conn_Interval_Min,
                                                      uint16_t Conn_Interval_Max,
                                                      uint16_t Latency,
                                                      uint16_t Timeout_Multiplier );

tBleStatus ACI_L2CAP_CONNECTION_PARAMETER_UPDATE_RESP( uint16_t Connection_Handle,
                                                       uint16_t Conn_Interval_Min,
                                                       uint16_t Conn_Interval_Max,
                                                       uint16_t Latency,
                                                       uint16_t Timeout_Multiplier,
                                                       uint16_t Minimum_CE_Length,
                                                       uint16_t Maximum_CE_Length,
                                                       uint8_t Identifier,
                                                       uint8_t Accept );

tBleStatus ACI_L2CAP_COC_CONNECT( uint16_t Connection_Handle,
                                  uint16_t SPSM,
                                  uint16_t MTU,
                                  uint16_t MPS,
                                  uint16_t Initial_Credits,
                                  uint8_t Channel_Number );

tBleStatus ACI_L2CAP_COC_CONNECT_CONFIRM( uint16_t Connection_Handle,
                                          uint16_t MTU,
                                          uint16_t MPS,
                                          uint16_t Initial_Credits,
                                          uint16_t Result,
                                          uint8_t* Channel_Number,
                                          uint8_t* Channel_Index_List );

tBleStatus ACI_L2CAP_COC_RECONF( uint16_t Connection_Handle,
                                 uint16_t MTU,
                                 uint16_t MPS,
                                 uint8_t Channel_Number,
                                 const uint8_t* Channel_Index_List );

tBleStatus ACI_L2CAP_COC_RECONF_CONFIRM( uint16_t Connection_Handle,
                                         uint16_t Result );

tBleStatus ACI_L2CAP_COC_DISCONNECT( uint8_t Channel_Index );

tBleStatus ACI_L2CAP_COC_FLOW_CONTROL( uint8_t Channel_Index,
                                       uint16_t Credits );

tBleStatus ACI_L2CAP_COC_TX_DATA( uint8_t Channel_Index,
                                  uint16_t Length,
                                  const uint8_t* Data );

/* EVENTS */

tBleStatus HCI_DISCONNECTION_COMPLETE_EVENT( uint8_t Status,
                                             uint16_t Connection_Handle,
                                             uint8_t Reason );

tBleStatus HCI_ENCRYPTION_CHANGE_EVENT( uint8_t Status,
                                        uint16_t Connection_Handle,
                                        uint8_t Encryption_Enabled );

tBleStatus HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE_EVENT( uint8_t Status,
                                                               uint16_t Connection_Handle,
                                                               uint8_t Version,
                                                               uint16_t Manufacturer_Name,
                                                               uint16_t Subversion );

tBleStatus HCI_HARDWARE_ERROR_EVENT( uint8_t Hardware_Code );

tBleStatus HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT( uint8_t Number_of_Handles,
                                                  const Handle_Packets_Pair_Entry_t* Handle_Packets_Pair_Entry );

tBleStatus HCI_DATA_BUFFER_OVERFLOW_EVENT( uint8_t Link_Type );

tBleStatus HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT( uint8_t Status,
                                                      uint16_t Connection_Handle );

tBleStatus HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_EVENT( uint16_t Connection_Handle );

tBleStatus HCI_LE_CONNECTION_COMPLETE_EVENT( uint8_t Status,
                                             uint16_t Connection_Handle,
                                             uint8_t Role,
                                             uint8_t Peer_Address_Type,
                                             const uint8_t* Peer_Address,
                                             uint16_t Conn_Interval,
                                             uint16_t Conn_Latency,
                                             uint16_t Supervision_Timeout,
                                             uint8_t Central_Clock_Accuracy );

tBleStatus HCI_LE_ADVERTISING_REPORT_EVENT( uint8_t Num_Reports,
                                            const Advertising_Report_t* Advertising_Report );

tBleStatus HCI_LE_CONNECTION_UPDATE_COMPLETE_EVENT( uint8_t Status,
                                                    uint16_t Connection_Handle,
                                                    uint16_t Conn_Interval,
                                                    uint16_t Conn_Latency,
                                                    uint16_t Supervision_Timeout );

tBleStatus HCI_LE_READ_REMOTE_FEATURES_COMPLETE_EVENT( uint8_t Status,
                                                       uint16_t Connection_Handle,
                                                       const uint8_t* LE_Features );

tBleStatus HCI_LE_LONG_TERM_KEY_REQUEST_EVENT( uint16_t Connection_Handle,
                                               const uint8_t* Random_Number,
                                               uint16_t Encrypted_Diversifier );

tBleStatus HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_EVENT( uint16_t Connection_Handle,
                                                             uint16_t Interval_Min,
                                                             uint16_t Interval_Max,
                                                             uint16_t Max_Latency,
                                                             uint16_t Timeout );

tBleStatus HCI_LE_DATA_LENGTH_CHANGE_EVENT( uint16_t Connection_Handle,
                                            uint16_t MaxTxOctets,
                                            uint16_t MaxTxTime,
                                            uint16_t MaxRxOctets,
                                            uint16_t MaxRxTime );

tBleStatus HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT( uint8_t Status,
                                                             const uint8_t* Local_P256_Public_Key );

tBleStatus HCI_LE_GENERATE_DHKEY_COMPLETE_EVENT( uint8_t Status,
                                                 const uint8_t* DHKey );

tBleStatus HCI_LE_ENHANCED_CONNECTION_COMPLETE_EVENT( uint8_t Status,
                                                      uint16_t Connection_Handle,
                                                      uint8_t Role,
                                                      uint8_t Peer_Address_Type,
                                                      const uint8_t* Peer_Address,
                                                      const uint8_t* Local_Resolvable_Private_Address,
                                                      const uint8_t* Peer_Resolvable_Private_Address,
                                                      uint16_t Conn_Interval,
                                                      uint16_t Conn_Latency,
                                                      uint16_t Supervision_Timeout,
                                                      uint8_t Central_Clock_Accuracy );

tBleStatus HCI_LE_DIRECTED_ADVERTISING_REPORT_EVENT( uint8_t Num_Reports,
                                                     const Direct_Advertising_Report_t* Direct_Advertising_Report );

tBleStatus HCI_LE_PHY_UPDATE_COMPLETE_EVENT( uint8_t Status,
                                             uint16_t Connection_Handle,
                                             uint8_t TX_PHY,
                                             uint8_t RX_PHY );

tBleStatus HCI_LE_EXTENDED_ADVERTISING_REPORT_EVENT( uint8_t Num_Reports,
                                                     uint16_t Event_Type,
                                                     uint8_t Address_Type,
                                                     const uint8_t* Address,
                                                     uint8_t Primary_PHY,
                                                     uint8_t Secondary_PHY,
                                                     uint8_t Advertising_SID,
                                                     uint8_t TX_Power,
                                                     uint8_t RSSI,
                                                     uint16_t Periodic_Adv_Interval,
                                                     uint8_t Direct_Address_Type,
                                                     const uint8_t* Direct_Address,
                                                     uint8_t Data_Length,
                                                     const uint8_t* Data );

tBleStatus HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_EVENT( uint8_t Status,
                                                               uint16_t Sync_Handle,
                                                               uint8_t Advertising_SID,
                                                               uint8_t Advertiser_Address_Type,
                                                               const uint8_t* Advertiser_Address,
                                                               uint8_t Advertiser_PHY,
                                                               uint16_t Periodic_Advertising_Interval,
                                                               uint8_t Advertiser_Clock_Accuracy );

tBleStatus HCI_LE_PERIODIC_ADVERTISING_REPORT_EVENT( uint16_t Sync_Handle,
                                                     uint8_t TX_Power,
                                                     uint8_t RSSI,
                                                     uint8_t CTE_Type,
                                                     uint8_t Data_Status,
                                                     uint8_t Data_Length,
                                                     const uint8_t* Data );

tBleStatus HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST_EVENT( uint16_t Sync_Handle );

tBleStatus HCI_LE_SCAN_TIMEOUT_EVENT( void );

tBleStatus HCI_LE_ADVERTISING_SET_TERMINATED_EVENT( uint8_t Status,
                                                    uint8_t Advertising_Handle,
                                                    uint16_t Connection_Handle,
                                                    uint8_t Num_Completed_Ext_Adv_Events );

tBleStatus HCI_LE_SCAN_REQUEST_RECEIVED_EVENT( uint8_t Advertising_Handle,
                                               uint8_t Scanner_Address_Type,
                                               const uint8_t* Scanner_Address );

tBleStatus HCI_LE_CHANNEL_SELECTION_ALGORITHM_EVENT( uint16_t Connection_Handle,
                                                     uint8_t Channel_Selection_Algorithm );

tBleStatus HCI_LE_CONNECTIONLESS_IQ_REPORT_EVENT( uint16_t Sync_Handle,
                                                  uint8_t Channel_Index,
                                                  uint16_t RSSI,
                                                  uint8_t RSSI_Antenna_ID,
                                                  uint8_t CTE_Type,
                                                  uint8_t Slot_Durations,
                                                  uint8_t Packet_Status,
                                                  uint16_t Periodic_Event_Counter,
                                                  uint8_t Sample_Count,
                                                  const IQ_Sample_t* IQ_Sample );

tBleStatus HCI_LE_CONNECTION_IQ_REPORT_EVENT( uint16_t Connection_Handle,
                                              uint8_t RX_PHY,
                                              uint8_t Data_Channel_Index,
                                              uint16_t RSSI,
                                              uint8_t RSSI_Antenna_ID,
                                              uint8_t CTE_Type,
                                              uint8_t Slot_Durations,
                                              uint8_t Packet_Status,
                                              uint16_t Connection_Event_Counter,
                                              uint8_t Sample_Count,
                                              const IQ_Sample_t* IQ_Sample );

tBleStatus HCI_LE_CTE_REQUEST_FAILED_EVENT( uint8_t Status,
                                            uint16_t Connection_Handle );

tBleStatus HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_EVENT( uint8_t Status,
                                                                     uint16_t Connection_Handle,
                                                                     uint16_t Service_Data,
                                                                     uint16_t Sync_Handle,
                                                                     uint8_t Advertising_SID,
                                                                     uint8_t Advertiser_Address_Type,
                                                                     const uint8_t* Advertiser_Address,
                                                                     uint8_t Advertiser_PHY,
                                                                     uint16_t Periodic_Advertising_Interval,
                                                                     uint8_t Advertiser_Clock_Accuracy );

tBleStatus HCI_LE_CIS_ESTABLISHED_EVENT( uint8_t Status,
                                         uint16_t Connection_Handle,
                                         const uint8_t* CIG_Sync_Delay,
                                         const uint8_t* CIS_Sync_Delay,
                                         const uint8_t* Transport_Latency_C_To_P,
                                         const uint8_t* Transport_Latency_P_To_C,
                                         uint8_t PHY_C_To_P,
                                         uint8_t PHY_P_To_C,
                                         uint8_t NSE,
                                         uint8_t BN_C_To_P,
                                         uint8_t BN_P_To_C,
                                         uint8_t FT_C_To_P,
                                         uint8_t FT_P_To_C,
                                         uint16_t Max_PDU_C_To_P,
                                         uint16_t Max_PDU_P_To_C,
                                         uint16_t ISO_Interval );

tBleStatus HCI_LE_CIS_REQUEST_EVENT( uint16_t ACL_Connection_Handle,
                                     uint16_t CIS_Connection_Handle,
                                     uint8_t CIG_ID,
                                     uint8_t CIS_ID );

tBleStatus HCI_LE_CREATE_BIG_COMPLETE_EVENT( uint8_t Status,
                                             uint8_t BIG_Handle,
                                             const uint8_t* BIG_Sync_Delay,
                                             const uint8_t* Transport_Latency_BIG,
                                             uint8_t PHY,
                                             uint8_t NSE,
                                             uint8_t BN,
                                             uint8_t PTO,
                                             uint8_t IRC,
                                             uint16_t Max_PDU,
                                             uint16_t ISO_Interval,
                                             uint8_t Num_BIS,
                                             const Connection_Handle_t* Connection_Handle );

tBleStatus HCI_LE_TERMINATE_BIG_COMPLETE_EVENT( uint8_t BIG_Handle,
                                                uint8_t Reason );

tBleStatus HCI_LE_BIG_SYNC_ESTABLISHED_EVENT( uint8_t Status,
                                              uint8_t BIG_Handle,
                                              const uint8_t* Transport_Latency_BIG,
                                              uint8_t NSE,
                                              uint8_t BN,
                                              uint8_t PTO,
                                              uint8_t IRC,
                                              uint16_t Max_PDU,
                                              uint16_t ISO_Interval,
                                              uint8_t Num_BIS,
                                              const Connection_Handle_t* Connection_Handle );

tBleStatus HCI_LE_BIG_SYNC_LOST_EVENT( uint8_t BIG_Handle,
                                       uint8_t Reason );

tBleStatus HCI_LE_REQUEST_PEER_SCA_COMPLETE_EVENT( uint8_t Status,
                                                   uint16_t Connection_Handle,
                                                   uint8_t Peer_Clock_Accuracy );

tBleStatus HCI_LE_PATH_LOSS_THRESHOLD_EVENT( uint16_t Connection_Handle,
                                             uint8_t Current_Path_Loss,
                                             uint8_t Zone_Entered );

tBleStatus HCI_LE_TRANSMIT_POWER_REPORTING_EVENT( uint8_t Status,
                                                  uint16_t Connection_Handle,
                                                  uint8_t Reason,
                                                  uint8_t PHY,
                                                  uint8_t TX_Power_Level,
                                                  uint8_t TX_Power_Level_Flag,
                                                  uint8_t Delta );

tBleStatus HCI_LE_BIGINFO_ADVERTISING_REPORT_EVENT( uint16_t Sync_Handle,
                                                    uint8_t Num_BIS,
                                                    uint8_t NSE,
                                                    uint16_t ISO_Interval,
                                                    uint8_t BN,
                                                    uint8_t PTO,
                                                    uint8_t IRC,
                                                    uint16_t Max_PDU,
                                                    const uint8_t* SDU_Interval,
                                                    uint16_t Max_SDU,
                                                    uint8_t PHY,
                                                    uint8_t Framing,
                                                    uint8_t Encryption );

tBleStatus HCI_LE_SUBRATE_CHANGE_EVENT( uint8_t Status,
                                        uint16_t Connection_Handle,
                                        uint16_t Subrate_Factor,
                                        uint16_t Peripheral_Latency,
                                        uint16_t Continuation_Number,
                                        uint16_t Supervision_Timeout );

tBleStatus HCI_LE_CIS_ESTABLISHED_V2_EVENT( uint8_t Status,
                                            uint16_t Connection_Handle,
                                            const uint8_t* CIG_Sync_Delay,
                                            const uint8_t* CIS_Sync_Delay,
                                            const uint8_t* Transport_Latency_C_To_P,
                                            const uint8_t* Transport_Latency_P_To_C,
                                            uint8_t PHY_C_To_P,
                                            uint8_t PHY_P_To_C,
                                            uint8_t NSE,
                                            uint8_t BN_C_To_P,
                                            uint8_t BN_P_To_C,
                                            uint8_t FT_C_To_P,
                                            uint8_t FT_P_To_C,
                                            uint16_t Max_PDU_C_To_P,
                                            uint16_t Max_PDU_P_To_C,
                                            uint16_t ISO_Interval,
                                            const uint8_t* Sub_Interval,
                                            uint16_t Max_SDU_C_To_P,
                                            uint16_t Max_SDU_P_To_C,
                                            const uint8_t* SDU_Interval_C_To_P,
                                            const uint8_t* SDU_Interval_P_To_C,
                                            uint8_t Framing );

tBleStatus ACI_HAL_END_OF_RADIO_ACTIVITY_EVENT( uint8_t Last_State,
                                                uint8_t Next_State,
                                                uint32_t Next_State_SysTime,
                                                uint8_t Last_State_Slot,
                                                uint8_t Next_State_Slot );

tBleStatus ACI_HAL_WARNING_EVENT( uint8_t Warning_Type,
                                  uint8_t Data_Length,
                                  const uint8_t* Data );

tBleStatus ACI_HAL_SYNC_EVENT( uint8_t Group_Id,
                               uint32_t Next_Anchor_Point,
                               uint32_t Time_Stamp,
                               uint32_t Next_Sdu_Delivery_Timeout );

tBleStatus ACI_GAP_LIMITED_DISCOVERABLE_EVENT( void );

tBleStatus ACI_GAP_PAIRING_COMPLETE_EVENT( uint16_t Connection_Handle,
                                           uint8_t Status,
                                           uint8_t Reason );

tBleStatus ACI_GAP_PASS_KEY_REQ_EVENT( uint16_t Connection_Handle );

tBleStatus ACI_GAP_AUTHORIZATION_REQ_EVENT( uint16_t Connection_Handle );

tBleStatus ACI_GAP_BOND_LOST_EVENT( uint16_t Connection_Handle );

tBleStatus ACI_GAP_PROC_COMPLETE_EVENT( uint8_t Procedure_Code,
                                        uint8_t Status,
                                        uint8_t Data_Length,
                                        const uint8_t* Data );

tBleStatus ACI_GAP_ADDR_NOT_RESOLVED_EVENT( uint16_t Connection_Handle );

tBleStatus ACI_GAP_NUMERIC_COMPARISON_VALUE_EVENT( uint16_t Connection_Handle,
                                                   uint32_t Numeric_Value );

tBleStatus ACI_GAP_KEYPRESS_NOTIFICATION_EVENT( uint16_t Connection_Handle,
                                                uint8_t Notification_Type );

tBleStatus ACI_GAP_PAIRING_REQUEST_EVENT( uint16_t Connection_Handle,
                                          uint8_t Bonded,
                                          uint8_t Auth_Req );

tBleStatus ACI_L2CAP_CONNECTION_UPDATE_RESP_EVENT( uint16_t Connection_Handle,
                                                   uint16_t Result );

tBleStatus ACI_L2CAP_PROC_TIMEOUT_EVENT( uint16_t Connection_Handle,
                                         uint8_t Data_Length,
                                         const uint8_t* Data );

tBleStatus ACI_L2CAP_CONNECTION_UPDATE_REQ_EVENT( uint16_t Connection_Handle,
                                                  uint8_t Identifier,
                                                  uint16_t L2CAP_Length,
                                                  uint16_t Interval_Min,
                                                  uint16_t Interval_Max,
                                                  uint16_t Latency,
                                                  uint16_t Timeout_Multiplier );

tBleStatus ACI_L2CAP_COMMAND_REJECT_EVENT( uint16_t Connection_Handle,
                                           uint8_t Identifier,
                                           uint16_t Reason,
                                           uint8_t Data_Length,
                                           const uint8_t* Data );

tBleStatus ACI_L2CAP_COC_CONNECT_EVENT( uint16_t Connection_Handle,
                                        uint16_t SPSM,
                                        uint16_t MTU,
                                        uint16_t MPS,
                                        uint16_t Initial_Credits,
                                        uint8_t Channel_Number );

tBleStatus ACI_L2CAP_COC_CONNECT_CONFIRM_EVENT( uint16_t Connection_Handle,
                                                uint16_t MTU,
                                                uint16_t MPS,
                                                uint16_t Initial_Credits,
                                                uint16_t Result,
                                                uint8_t Channel_Number,
                                                const uint8_t* Channel_Index_List );

tBleStatus ACI_L2CAP_COC_RECONF_EVENT( uint16_t Connection_Handle,
                                       uint16_t MTU,
                                       uint16_t MPS,
                                       uint8_t Channel_Number,
                                       const uint8_t* Channel_Index_List );

tBleStatus ACI_L2CAP_COC_RECONF_CONFIRM_EVENT( uint16_t Connection_Handle,
                                               uint16_t Result );

tBleStatus ACI_L2CAP_COC_DISCONNECT_EVENT( uint8_t Channel_Index );

tBleStatus ACI_L2CAP_COC_FLOW_CONTROL_EVENT( uint8_t Channel_Index,
                                             uint16_t Credits );

tBleStatus ACI_L2CAP_COC_RX_DATA_EVENT( uint8_t Channel_Index,
                                        uint16_t Length,
                                        const uint8_t* Data );

tBleStatus ACI_L2CAP_COC_TX_POOL_AVAILABLE_EVENT( void );

tBleStatus ACI_GATT_ATTRIBUTE_MODIFIED_EVENT( uint16_t Connection_Handle,
                                              uint16_t Attr_Handle,
                                              uint16_t Offset,
                                              uint16_t Attr_Data_Length,
                                              const uint8_t* Attr_Data );

tBleStatus ACI_GATT_PROC_TIMEOUT_EVENT( uint16_t Connection_Handle );

tBleStatus ACI_ATT_EXCHANGE_MTU_RESP_EVENT( uint16_t Connection_Handle,
                                            uint16_t Server_RX_MTU );

tBleStatus ACI_ATT_FIND_INFO_RESP_EVENT( uint16_t Connection_Handle,
                                         uint8_t Format,
                                         uint8_t Event_Data_Length,
                                         const uint8_t* Handle_UUID_Pair );

tBleStatus ACI_ATT_FIND_BY_TYPE_VALUE_RESP_EVENT( uint16_t Connection_Handle,
                                                  uint8_t Num_of_Handle_Pair,
                                                  const Attribute_Group_Handle_Pair_t* Attribute_Group_Handle_Pair );

tBleStatus ACI_ATT_READ_BY_TYPE_RESP_EVENT( uint16_t Connection_Handle,
                                            uint8_t Handle_Value_Pair_Length,
                                            uint8_t Data_Length,
                                            const uint8_t* Handle_Value_Pair_Data );

tBleStatus ACI_ATT_READ_RESP_EVENT( uint16_t Connection_Handle,
                                    uint8_t Event_Data_Length,
                                    const uint8_t* Attribute_Value );

tBleStatus ACI_ATT_READ_BLOB_RESP_EVENT( uint16_t Connection_Handle,
                                         uint8_t Event_Data_Length,
                                         const uint8_t* Attribute_Value );

tBleStatus ACI_ATT_READ_MULTIPLE_RESP_EVENT( uint16_t Connection_Handle,
                                             uint8_t Event_Data_Length,
                                             const uint8_t* Set_Of_Values );

tBleStatus ACI_ATT_READ_BY_GROUP_TYPE_RESP_EVENT( uint16_t Connection_Handle,
                                                  uint8_t Attribute_Data_Length,
                                                  uint8_t Data_Length,
                                                  const uint8_t* Attribute_Data_List );

tBleStatus ACI_ATT_PREPARE_WRITE_RESP_EVENT( uint16_t Connection_Handle,
                                             uint16_t Attribute_Handle,
                                             uint16_t Offset,
                                             uint8_t Part_Attribute_Value_Length,
                                             const uint8_t* Part_Attribute_Value );

tBleStatus ACI_ATT_EXEC_WRITE_RESP_EVENT( uint16_t Connection_Handle );

tBleStatus ACI_GATT_INDICATION_EVENT( uint16_t Connection_Handle,
                                      uint16_t Attribute_Handle,
                                      uint8_t Attribute_Value_Length,
                                      const uint8_t* Attribute_Value );

tBleStatus ACI_GATT_NOTIFICATION_EVENT( uint16_t Connection_Handle,
                                        uint16_t Attribute_Handle,
                                        uint8_t Attribute_Value_Length,
                                        const uint8_t* Attribute_Value );

tBleStatus ACI_GATT_PROC_COMPLETE_EVENT( uint16_t Connection_Handle,
                                         uint8_t Error_Code );

tBleStatus ACI_GATT_ERROR_RESP_EVENT( uint16_t Connection_Handle,
                                      uint8_t Req_Opcode,
                                      uint16_t Attribute_Handle,
                                      uint8_t Error_Code );

tBleStatus ACI_GATT_DISC_READ_CHAR_BY_UUID_RESP_EVENT( uint16_t Connection_Handle,
                                                       uint16_t Attribute_Handle,
                                                       uint8_t Attribute_Value_Length,
                                                       const uint8_t* Attribute_Value );

tBleStatus ACI_GATT_WRITE_PERMIT_REQ_EVENT( uint16_t Connection_Handle,
                                            uint16_t Attribute_Handle,
                                            uint8_t Data_Length,
                                            const uint8_t* Data );

tBleStatus ACI_GATT_READ_PERMIT_REQ_EVENT( uint16_t Connection_Handle,
                                           uint16_t Attribute_Handle,
                                           uint16_t Offset );

tBleStatus ACI_GATT_READ_MULTI_PERMIT_REQ_EVENT( uint16_t Connection_Handle,
                                                 uint8_t Number_of_Handles,
                                                 const Handle_Item_t* Handle_Item );

tBleStatus ACI_GATT_TX_POOL_AVAILABLE_EVENT( uint16_t Connection_Handle,
                                             uint16_t Available_Buffers );

tBleStatus ACI_GATT_SERVER_CONFIRMATION_EVENT( uint16_t Connection_Handle );

tBleStatus ACI_GATT_PREPARE_WRITE_PERMIT_REQ_EVENT( uint16_t Connection_Handle,
                                                    uint16_t Attribute_Handle,
                                                    uint16_t Offset,
                                                    uint8_t Data_Length,
                                                    const uint8_t* Data );

tBleStatus ACI_GATT_EATT_BEARER_EVENT( uint8_t Channel_Index,
                                       uint8_t EAB_State,
                                       uint8_t Status );

tBleStatus ACI_GATT_MULT_NOTIFICATION_EVENT( uint16_t Connection_Handle,
                                             uint16_t Offset,
                                             uint16_t Data_Length,
                                             const uint8_t* Data );

tBleStatus ACI_GATT_NOTIFICATION_COMPLETE_EVENT( uint16_t Attr_Handle );

tBleStatus ACI_GATT_READ_EXT_EVENT( uint16_t Connection_Handle,
                                    uint16_t Offset,
                                    uint16_t Event_Data_Length,
                                    const uint8_t* Attribute_Value );

tBleStatus ACI_GATT_INDICATION_EXT_EVENT( uint16_t Connection_Handle,
                                          uint16_t Attribute_Handle,
                                          uint16_t Offset,
                                          uint16_t Attribute_Value_Length,
                                          const uint8_t* Attribute_Value );

tBleStatus ACI_GATT_NOTIFICATION_EXT_EVENT( uint16_t Connection_Handle,
                                            uint16_t Attribute_Handle,
                                            uint16_t Offset,
                                            uint16_t Attribute_Value_Length,
                                            const uint8_t* Attribute_Value );

tBleStatus HCI_RX_ACL_DATA_EVENT( uint16_t Connection_Handle,
                                  uint8_t Flags,
                                  uint16_t Data_Length,
                                  const uint8_t* Data );

tBleStatus HCI_RX_ISO_DATA_EVENT( uint16_t Connection_Handle,
                                  uint8_t Flags,
                                  uint16_t Data_Length,
                                  const uint8_t* Data );


#endif /* BLE_RAW_API_H__ */
