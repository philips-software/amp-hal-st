/*****************************************************************************
 * @file    ble_hal_aci.h
 * @brief   STM32WBA BLE API (HAL_ACI)
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

#ifndef BLE_HAL_ACI_H__
#define BLE_HAL_ACI_H__


#include "ble_types.h"

/**
 * @brief ACI_HAL_GET_FW_BUILD_NUMBER
 * This command returns the build number associated with the firmware version
 * currently running
 * 
 * @param[out] Build_Number Build number of the firmware.
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_get_fw_build_number( uint16_t* Build_Number );

/**
 * @brief ACI_HAL_WRITE_CONFIG_DATA
 * This command writes a value to a configure data structure. It is useful to
 * setup directly some parameters for the BLE stack.
 * Refer to Annex for details on the different parameters that can be
 * configured.
 * 
 * @param Offset Offset of the element in the configuration data structure
 *        which has to be written.
 *        Values:
 *        - 0x00: CONFIG_DATA_PUBLIC_ADDRESS_OFFSET;
 *          Bluetooth public address; 6 bytes
 *        - 0x08: CONFIG_DATA_ER_OFFSET;
 *          Encryption root key; 16 bytes
 *        - 0x18: CONFIG_DATA_IR_OFFSET;
 *          Identity root key; 16 bytes
 *        - 0x2E: CONFIG_DATA_RANDOM_ADDRESS_OFFSET;
 *          Static Random Address; 6 bytes
 *        - 0x34: CONFIG_DATA_GAP_ADD_REC_NBR_OFFSET;
 *          GAP service additional record number; 1 byte
 *        - 0x35: CONFIG_DATA_SC_KEY_TYPE_OFFSET;
 *          Secure Connections key type; 1 byte
 *        - 0xB0: CONFIG_DATA_SMP_MODE_OFFSET;
 *          SMP mode; 1 byte
 *        - 0xC0: CONFIG_DATA_LL_SCAN_CHAN_MAP_OFFSET [only for STM32WB];
 *          LL scan channel map; 1 byte
 *        - 0xC1: CONFIG_DATA_LL_BG_SCAN_MODE_OFFSET [only for STM32WB];
 *          LL background scan mode; 1 byte
 *        - 0xC2: CONFIG_DATA_LL_RSSI_GOLDEN_RANGE_OFFSET [only for STM32WBA];
 *          LL RSSI golden range; 2 bytes
 *        - 0xC3: CONFIG_DATA_LL_RPA_MODE_OFFSET [only for STM32WB];
 *          LL RPA mode; 1 byte
 *        - 0xC4: CONFIG_DATA_LL_RX_ACL_CTRL_OFFSET [only for STM32WBA];
 *          LL RX ACL control; 2 bytes
 *        - 0xD1: CONFIG_DATA_LL_MAX_DATA_EXT_OFFSET [only for STM32WB full
 *          stack];
 *          LL maximum data length extension; 8 bytes
 * @param Length Length of data to be written
 * @param Value Data to be written
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_write_config_data( uint8_t Offset,
                                      uint8_t Length,
                                      const uint8_t* Value );

/**
 * @brief ACI_HAL_READ_CONFIG_DATA
 * This command requests the value in the configure data structure. The number
 * of read bytes changes for different Offset.
 * 
 * @param Offset Offset of the element in the configuration data structure
 *        which has to be read.
 *        Values:
 *        - 0x00: CONFIG_DATA_PUBLIC_ADDRESS_OFFSET;
 *          Bluetooth public address; 6 bytes
 *        - 0x08: CONFIG_DATA_ER_OFFSET;
 *          Encryption root key used to derive LTK (legacy) and CSRK; 16 bytes
 *        - 0x18: CONFIG_DATA_IR_OFFSET
 *          Identity root key used to derive DHK (legacy) and IRK; 16 bytes
 *        - 0x2E: CONFIG_DATA_RANDOM_ADDRESS_OFFSET;
 *          Static Random Address; 6 bytes
 *        - 0xC2: CONFIG_DATA_LL_RSSI_GOLDEN_RANGE_OFFSET [only for STM32WBA];
 *          LL RSSI golden range (two 8-bit signed values in dBm - byte #0 is
 *          the lower value, byte #1 is the upper value); 2 bytes
 * @param[out] Data_Length Length of Data in octets
 * @param[out] Data Data field associated with Offset parameter
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_read_config_data( uint8_t Offset,
                                     uint8_t* Data_Length,
                                     uint8_t* Data );

/**
 * @brief ACI_HAL_SET_TX_POWER_LEVEL
 * This command sets the TX power level of the device. By controlling the PA
 * level, that determines the output power level (dBm) at the IC pin.
 * When the system starts up or reboots, the default TX power level is used,
 * which is the maximum value. Once this command is given, the output power
 * changes instantly, regardless if there is Bluetooth communication going on
 * or not. For example, for debugging purpose, the device can be set to
 * advertise all the time. By using this command, one can then observe the
 * evolution of the TX signal strength.
 * The system keeps the last received TX power level from the command, i.e. the
 * 2nd command overwrites the previous TX power level. The new TX power level
 * remains until another ACI_HAL_SET_TX_POWER_LEVEL command, or the system
 * reboots. However, note that the advertising extensions commands allow, per
 * advertising set, to override the value of TX power determined by
 * ACI_HAL_SET_TX_POWER_LEVEL command (e.g. see ACI_GAP_ADV_SET_CONFIGURATION).
 * Refer to Annex for the dBm corresponding values of PA_Level parameter.
 * 
 * @param En_High_Power Enable High Power mode - Deprecated and ignored
 *        Values:
 *        - 0x00: Standard Power
 *        - 0x01: High Power
 * @param PA_Level Power amplifier output level.
 *        Values:
 *        - 0x00 ... 0x23
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_set_tx_power_level( uint8_t En_High_Power,
                                       uint8_t PA_Level );

/**
 * @brief ACI_HAL_LE_TX_TEST_PACKET_NUMBER
 * This command returns the number of packets sent in Direct Test Mode.
 * When the Direct TX test is started, a 16-bit counter is used to count how
 * many packets have been transmitted.
 * This command can be used to check how many packets have been sent during the
 * Direct TX test.
 * The counter starts from 0 and counts upwards. The counter can wrap and start
 * from 0 again. The counter is not cleared until the next Direct TX test
 * starts.
 * 
 * @param[out] Number_Of_Packets Number of packets sent during the last Direct
 *        TX test.
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_le_tx_test_packet_number( uint32_t* Number_Of_Packets );

/**
 * @brief ACI_HAL_TONE_START
 * This command starts a carrier frequency, i.e. a tone, on a specific channel.
 * The frequency sine wave at the specific channel may be used for debugging
 * purpose only. The channel ID is a parameter from 0x00 to 0x27 for the 40 BLE
 * channels, e.g. 0x00 for 2.402 GHz, 0x01 for 2.404 GHz etc.
 * This command should not be used when normal Bluetooth activities are
 * ongoing.
 * The tone should be stopped by ACI_HAL_TONE_STOP command.
 * 
 * @param RF_Channel BLE Channel ID, from 0x00 to 0x27 meaning (2.402 +
 *        0.002*0xXX) GHz
 *        Device will continuously emit 0s, that means that the tone will be at
 *        the channel center frequency minus the maximum frequency deviation
 *        (250 kHz).
 *        Values:
 *        - 0x00 ... 0x27
 * @param Freq_offset Frequency Offset for tone channel
 *        Values:
 *        - 0x00 ... 0xFF
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_tone_start( uint8_t RF_Channel,
                               uint8_t Freq_offset );

/**
 * @brief ACI_HAL_TONE_STOP
 * This command is used to stop the previously started ACI_HAL_TONE_START
 * command.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_tone_stop( void );

/**
 * @brief ACI_HAL_SET_RADIO_ACTIVITY_MASK
 * This command set the bitmask associated to
 * ACI_HAL_END_OF_RADIO_ACTIVITY_EVENT.
 * Only the radio activities enabled in the mask will be reported to
 * application by ACI_HAL_END_OF_RADIO_ACTIVITY_EVENT
 * 
 * @param Radio_Activity_Mask Bitmask of radio events
 *        Flags:
 *        - 0x0001: Idle
 *        - 0x0002: Advertising
 *        - 0x0004: Peripheral connection
 *        - 0x0008: Scanning
 *        - 0x0020: Central connection
 *        - 0x0040: TX test mode
 *        - 0x0080: RX test mode
 *        - 0x0200: Periodic advertising [only for STM32WBA]
 *        - 0x0400: Periodic sync [only for STM32WBA]
 *        - 0x0800: Iso broadcast [only for STM32WBA]
 *        - 0x1000: Iso sync [only for STM32WBA]
 *        - 0x2000: Iso peripheral connection [only for STM32WBA]
 *        - 0x4000: Iso central connection [only for STM32WBA]
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_set_radio_activity_mask( uint16_t Radio_Activity_Mask );

/**
 * @brief ACI_HAL_SET_EVENT_MASK
 * This command is used to enable/disable the generation of HAL events. If the
 * bit in the Event_Mask is set to a one, then the event associated with that
 * bit will be enabled.
 * 
 * @param Event_Mask Mask to enable/disable generation of HAL events
 *        Flags:
 *        - 0x00000000: No events specified (Default)
 *        - 0x00000001: ACI_HAL_SCAN_REQ_REPORT_EVENT [only for STM32WB]
 *        - 0x00000002: ACI_HAL_SYNC_EVENT [only for STM32WBA]
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_set_event_mask( uint32_t Event_Mask );

/**
 * @brief ACI_HAL_GET_LINK_STATUS_V2
 * This command returns the status of the Bluetooth Low Energy links managed by
 * the device.
 * 
 * @param[out] Link_Status Array of link status (up to 20 links plus 2 ISO
 *        streams). Each link status is 1 byte.
 *        Values:
 *        - 0x00: Idle
 *        - 0x01: Advertising
 *        - 0x02: Peripheral connection
 *        - 0x03: Scanning
 *        - 0x05: Central connection
 *        - 0x06: TX test mode
 *        - 0x07: RX test mode
 *        - 0x09: Periodic advertising
 *        - 0x0A: Periodic sync
 *        - 0x0B: Iso broadcast
 *        - 0x0C: Iso sync
 *        - 0x0D: Iso peripheral connection
 *        - 0x0E: Iso central connection
 * @param[out] Link_Connection_Handle Array of connection handles (2 bytes).
 *        Valid only if the corresponding link status is "connected"
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_get_link_status_v2( uint8_t* Link_Status,
                                       uint16_t* Link_Connection_Handle );

/**
 * @brief ACI_HAL_GET_PM_DEBUG_INFO_V2
 * This command is used to retrieve TX, RX and total buffer count allocated for
 * ACL packets.
 * 
 * @param[out] Allocated_For_TX MBlocks allocated for TXing
 * @param[out] Allocated_For_RX MBlocks allocated for RXing
 * @param[out] Allocated_MBlocks Overall allocated MBlocks
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_get_pm_debug_info_v2( uint16_t* Allocated_For_TX,
                                         uint16_t* Allocated_For_RX,
                                         uint16_t* Allocated_MBlocks );

/**
 * @brief ACI_HAL_SET_PERIPHERAL_LATENCY
 * This command is used to disable/enable the Peripheral latency feature during
 * a connection. Note that, by default, the Peripheral latency is enabled at
 * connection time.
 * 
 * @param Enable Enable/disable Peripheral latency.
 *        Values:
 *        - 0x00: Peripheral latency is disabled
 *        - 0x01: Peripheral latency is enabled
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_set_peripheral_latency( uint8_t Enable );

/**
 * @brief ACI_HAL_SET_SYNC_EVENT_CONFIG
 * This command is used to configure ACI_HAL_SYNC_EVENT.
 * 
 * @param Group_Id Identifier of the CIG or BIG.
 *        Values:
 *        - 0x00 ... 0xEF
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_set_sync_event_config( uint8_t Group_Id,
                                          uint8_t Enable_Sync,
                                          uint8_t Enable_Cb_Trigger,
                                          uint8_t Trigger_Source );

/**
 * @brief ACI_HAL_READ_RSSI
 * This command returns the value of the RSSI.
 * 
 * @param[out] RSSI RSSI (signed integer).
 *        Units: dBm.
 *        Values:
 *        - 127: RSSI not available
 *        - -127 ... 20
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_read_rssi( uint8_t* RSSI );

/**
 * @brief ACI_HAL_CONTINUOUS_TX_START
 * This command starts the continuous transmit test mode.
 * 
 * @param RF_Channel Index of the BLE RF channel used for test mode.
 *        Values:
 *        - 0x00 ... 0x27
 * @param PHY PHY used for test mode.
 *        Values:
 *        - 0x01: LE 1M PHY
 *        - 0x02: LE 2M PHY
 * @param Pattern Data pattern used for test mode.
 *        Values:
 *        - 0x00: Pseudo-Random bit sequence 9
 *        - 0x01: Pattern of alternating bits '11110000'
 *        - 0x02: Pattern of alternating bits '10101010'
 *        - 0x03: Pseudo-Random bit sequence 15
 *        - 0x04: Pattern of All '1' bits
 *        - 0x05: Pattern of All '0' bits
 *        - 0x06: Pattern of alternating bits '00001111'
 *        - 0x07: Pattern of alternating bits '0101'
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_continuous_tx_start( uint8_t RF_Channel,
                                        uint8_t PHY,
                                        uint8_t Pattern );

/**
 * @brief ACI_HAL_EAD_ENCRYPT_DECRYPT
 * This command encrypts or decrypts data following the Encrypted Advertising
 * Data scheme.
 * When encryption mode is selected, In_Data shall only contain the Payload
 * field to encrypt. The command adds the Randomizer and MIC fields in the
 * result. The result data length (Out_Data_Length) is equal to the input
 * length plus 9.
 * When decryption mode is selected, In_Data shall contain the full Encrypted
 * Data (Randomizer + Payload + MIC). The result data length (Out_Data_Length)
 * is equal to the input length minus 9.
 * If the decryption fails, the returned status is BLE_STATUS_FAILED, otherwise
 * it is BLE_STATUS_SUCCESS.
 * Note: on STM32WB, the In_Data_Length value must not exceed
 * (BLE_CMD_MAX_PARAM_LEN - 27) i.e. 228 for BLE_CMD_MAX_PARAM_LEN default
 * value.
 * 
 * @param Mode EAD operation mode: encryption or decryption.
 *        Values:
 *        - 0x00: Encryption
 *        - 0x01: Decryption
 * @param Key Session key used for EAD operation (in Little Endian format).
 * @param IV Initialization vector used for EAD operation (in Little Endian
 *        format).
 * @param In_Data_Length Length of input data
 * @param In_Data Input data
 * @param[out] Out_Data_Length Length of result data
 * @param[out] Out_Data Result data
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_ead_encrypt_decrypt( uint8_t Mode,
                                        const uint8_t* Key,
                                        const uint8_t* IV,
                                        uint16_t In_Data_Length,
                                        const uint8_t* In_Data,
                                        uint16_t* Out_Data_Length,
                                        uint8_t* Out_Data );


#endif /* BLE_HAL_ACI_H__ */
