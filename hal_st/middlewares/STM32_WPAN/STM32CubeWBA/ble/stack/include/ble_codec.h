/*****************************************************************************
 * @file    ble_codec.h
 *
 * @brief   This file contains the interface of the BLE stack regarding audio
 *          codec.
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

#ifndef BLE_CODEC_H__
#define BLE_CODEC_H__


#include <stdint.h>

/* Definition of Setup_Iso_Data_Path_Params */
typedef __PACKED_STRUCT  hci_le_setup_iso_data_path_params
{
  uint8_t Big_Handle;
  uint16_t Connection_Handle;
  uint8_t  Data_Path_Direction;
  uint8_t  Data_Path_ID;
  uint8_t  Codec_ID[5];
  uint8_t  Controller_Delay[3];
  uint8_t  Codec_Configuration_Length;
  uint8_t* pCodec_Configuration;
} hci_le_setup_iso_data_path_params;

/* Audio codec functions called by the BLE stack
 */
uint8_t BLE_CodecInit( void );

uint8_t BLE_ConfigureDataPath( uint8_t data_path_direction,
                               uint8_t data_pathID,
                               uint8_t vendor_specific_config_length,
                               const uint8_t* vendor_specific_config );

uint8_t BLE_ReadLocalSupportedCodecs( uint8_t* num_supported_std_codecs,
                                      uint8_t* std_codec,
                                      uint8_t* num_Supported_vs_Codecs,
                                      uint8_t* vs_Codec );

uint8_t BLE_ReadLocalSupportedCodecCapabilities(
                                         const uint8_t* codecID,
                                         uint8_t logical_transport_type,
                                         uint8_t direction,
                                         uint8_t* num_codec_capabilities,
                                         uint8_t* codec_capability );

uint8_t BLE_ReadLocalSupportedControllerDelay(
                                         const uint8_t* codec_id,
                                         uint8_t logical_transport_type,
                                         uint8_t direction,
                                         uint8_t codec_configuration_length,
                                         const uint8_t* codec_configuration,
                                         uint8_t* min_controller_delay,
                                         uint8_t* max_controller_delay );

uint8_t BLE_SetupIsoDataPath( uint16_t connection_handle,
                              hci_le_setup_iso_data_path_params*
                              iso_command_params );

uint8_t BLE_RemoveIsoDataPath( uint16_t connection_handle,
                               uint8_t direction);

uint8_t BLE_SendIsoDataOutToCodec( uint16_t iso_connection_handle,
                                   uint8_t pb_flag,
                                   uint8_t ts_flag,
                                   uint32_t timestamp,
                                   uint16_t PSN,
                                   uint8_t  packet_status_flag,
                                   uint16_t iso_data_load_length,
                                   uint32_t* iso_data );

uint8_t BLE_SendIsoDataInToCodec( uint16_t iso_connection_handle,
                                  uint8_t pb_flag,
                                  uint8_t ts_flag,
                                  uint32_t timestamp,
                                  uint16_t PSN,
                                  uint16_t iso_data_load_length,
                                  uint8_t* iso_data );

void BLE_LeSyncEvent( uint8_t group_id,
                      uint32_t next_anchor_point,
                      uint32_t time_stamp,
                      uint32_t next_sdu_delivery_timeout );

void BLE_IsochronousGroupEvent( uint16_t opcode,
                                uint8_t status,
                                uint8_t big_handle,
                                uint8_t cig_id,
                                uint16_t iso_interval,
                                uint8_t num_connection_handles,
                                uint16_t* connection_handle,
                                uint8_t* transport_latency_C_to_P,
                                uint8_t* transport_latency_P_to_C );

/* Audio codec functions exported by the BLE stack
 */
uint8_t BLE_SendIsoDataToLinkLayer( uint16_t iso_connection_handle,
                                 uint8_t pb_flag,
                                 uint8_t ts_flag,
                                 uint32_t timestamp,
                                 uint16_t PSN,
                                 uint16_t iso_data_load_length,
                                 uint8_t* iso_data );


void BLE_ResendIsoDataOutToCodec( uint16_t iso_connection_handle );

void BLE_CalibrationCallback(uint32_t TimeStamp);

void BLE_SynchronizationAudioClock(void);
#endif /* BLE_CODEC_H__ */
