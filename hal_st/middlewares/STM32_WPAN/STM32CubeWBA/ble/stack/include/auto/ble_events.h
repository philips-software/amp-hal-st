/*****************************************************************************
 * @file    ble_events.h
 * @author  MDG
 * @brief   STM32WBA BLE API (event callbacks)
 *          Auto-generated file: do not edit!
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */

#ifndef BLE_EVENTS_H__
#define BLE_EVENTS_H__


#include "ble_types.h"

/* HCI events */

/**
 * @brief HCI_DISCONNECTION_COMPLETE_EVENT
 * This event occurs when a connection is terminated. The status parameter
 * indicates if the disconnection was successful or not. The reason parameter
 * indicates the reason for the disconnection if the disconnection was
 * successful. If the disconnection was not successful, the value of the reason
 * parameter shall be ignored by the Host.
 * Note: if the connection is terminated by the remote device, the reason
 * parameter of this event is set to the reason specified by the remote device
 * only if it has an allowed value, otherwise the reason is forced to Remote
 * User Terminated Connection error code (0x13). Allowed remote reason values
 * are: Authentication Failure error code (0x05), Other End Terminated
 * Connection error codes (0x13 to 0x15), Unsupported Remote Feature error code
 * (0x1A), and Unacceptable Connection Parameters error code (0x3B).
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.5].
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Reason Reason for disconnection (see Bluetooth spec. [Vol 1, Part F]
 *        Error Codes).
 * @return Value indicating success or error code.
 */
tBleStatus hci_disconnection_complete_event( uint8_t Status,
                                             uint16_t Connection_Handle,
                                             uint8_t Reason );

/**
 * @brief HCI_ENCRYPTION_CHANGE_EVENT
 * The Encryption Change event is used to indicate that the change of the
 * encryption mode has been completed. The Connection_Handle will be a
 * Connection_Handle for an ACL connection. The Encryption_Enabled event
 * parameter specifies the new Encryption_Enabled parameter for the
 * Connection_Handle specified by the Connection_Handle event parameter. This
 * event will occur on both devices to notify the Hosts when Encryption has
 * changed for the specified Connection_Handle between two devices. Note: This
 * event shall not be generated if encryption is paused or resumed; during a
 * role switch, for example.
 * The meaning of the Encryption_Enabled parameter depends on whether the Host
 * has indicated support for Secure Connections in the
 * Secure_Connections_Host_Support parameter. When
 * Secure_Connections_Host_Support is 'disabled' or the Connection_Handle
 * refers to an LE link, the Controller shall only use Encryption_Enabled
 * values 0x00 (OFF) and 0x01 (ON).
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.8].
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Encryption_Enabled Link Level Encryption.
 *        Values:
 *        - 0x00: Link Level Encryption OFF
 *        - 0x01: Link Level Encryption is ON with AES-CCM
 * @return Value indicating success or error code.
 */
tBleStatus hci_encryption_change_event( uint8_t Status,
                                        uint16_t Connection_Handle,
                                        uint8_t Encryption_Enabled );

/**
 * @brief HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE_EVENT
 * The Read Remote Version Information Complete event is used to indicate the
 * completion of the process obtaining the version information of the remote
 * Controller specified by the Connection_Handle event parameter. The
 * Connection_Handle shall be for an ACL connection.
 * The Version event parameter defines the specification version of the LE
 * Controller.
 * The Manufacturer_Name event parameter indicates the manufacturer of the
 * remote Controller. The Subversion event parameter is controlled by the
 * manufacturer and is implementation dependent. The Subversion event parameter
 * defines the various revisions that each version of the Bluetooth hardware
 * will go through as design processes change and errors are fixed. This allows
 * the software to determine what Bluetooth hardware is being used and, if
 * necessary, to work around various bugs in the hardware.
 * When the Connection_Handle is associated with an LE-U logical link, the
 * Version event parameter shall be Link Layer VersNr parameter, the
 * Manufacturer_Name event parameter shall be the CompId parameter, and the
 * Subversion event parameter shall be the SubVersNr parameter.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.12].
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Version Version of the Current LMP in the remote Controller
 * @param Manufacturer_Name Manufacturer Name of the remote Controller
 * @param Subversion Subversion of the LMP in the remote Controller
 * @return Value indicating success or error code.
 */
tBleStatus hci_read_remote_version_information_complete_event( uint8_t Status,
                                                               uint16_t Connection_Handle,
                                                               uint8_t Version,
                                                               uint16_t Manufacturer_Name,
                                                               uint16_t Subversion );

/**
 * @brief HCI_HARDWARE_ERROR_EVENT
 * The Hardware Error event is used to indicate some implementation specific
 * type of hardware failure for the controller. This event is used to notify
 * the Host that a hardware failure has occurred in the Controller.
 * 
 * @param Hardware_Code Hardware Error Event code.
 *        Error code 0 is not used.
 *        Error code 1 is bluecore act2 error detected (only for STM32WB).
 *        Error code 2 is bluecore time overrun error detected (only for
 *        STM32WB).
 *        Error code 3 is internal FIFO full.
 *        Error code 4 is ISR delay error detected (only for STM32WB and only
 *        from cut 2.2).
 *        Values:
 *        - 0x01: event_act2 error
 *        - 0x02: event_time_overrun error
 *        - 0x03: event_fifo_full error
 *        - 0x04: event_isr_delay_error
 * @return Value indicating success or error code.
 */
tBleStatus hci_hardware_error_event( uint8_t Hardware_Code );

/**
 * @brief HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT
 * The Number Of Completed Packets event is used by the Controller to indicate
 * to the Host how many HCI Data Packets have been completed (transmitted or
 * flushed) for each Connection_Handle since the previous Number Of Completed
 * Packets event was sent to the Host. This means that the corresponding buffer
 * space has been freed in the Controller. Based on this information, and the
 * HC_Total_Num_ACL_Data_Packets and HC_Total_Num_Synchronous_Data_Packets
 * return parameter of the Read_Buffer_Size command, the Host can determine for
 * which Connection_Handles the following HCI Data Packets should be sent to
 * the Controller. The Number Of Completed Packets event must not be sent
 * before the corresponding Connection Complete event. While the Controller has
 * HCI data packets in its buffer, it must keep sending the Number Of Completed
 * Packets event to the Host at least periodically, until it finally reports
 * that all the pending ACL Data Packets have been transmitted or flushed.
 * 
 * @param Number_of_Handles The number of Connection_Handles and
 *        Num_HCI_Data_Packets parameters pairs contained in this event
 * @param Handle_Packets_Pair_Entry See @ref Handle_Packets_Pair_Entry_t
 * @return Value indicating success or error code.
 */
tBleStatus hci_number_of_completed_packets_event( uint8_t Number_of_Handles,
                                                  const Handle_Packets_Pair_Entry_t* Handle_Packets_Pair_Entry );

/**
 * @brief HCI_DATA_BUFFER_OVERFLOW_EVENT
 * This event is used to indicate that the Controller's data buffers have been
 * overflowed. This can occur if the Host has sent more packets than allowed.
 * 
 * @param Link_Type Type of data whose buffers overflowed.
 *        Values:
 *        - 0x00: Synchronous Data packet buffers
 *        - 0x01: ACL Data packet buffers
 *        - 0x02: ISO Data packet buffers
 * @return Value indicating success or error code.
 */
tBleStatus hci_data_buffer_overflow_event( uint8_t Link_Type );

/**
 * @brief HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT
 * This event is used to indicate to the Host that the encryption key was
 * refreshed on the given Connection_Handle. The Controller sends this event
 * when the encryption key has been refreshed due to encryption being started
 * or resumed.
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @return Value indicating success or error code.
 */
tBleStatus hci_encryption_key_refresh_complete_event( uint8_t Status,
                                                      uint16_t Connection_Handle );

/**
 * @brief HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_EVENT
 * This event is used to indicate that a packet containing a valid MIC on the
 * Connection_Handle was not received within the LE Authenticated Payload
 * Timeout.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.75].
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @return Value indicating success or error code.
 */
tBleStatus hci_authenticated_payload_timeout_expired_event( uint16_t Connection_Handle );

/* HCI LE meta events */

/**
 * @brief HCI_LE_CONNECTION_COMPLETE_EVENT
 * The LE Connection Complete event indicates to both of the Hosts forming the
 * connection that a new connection has been created. Upon the creation of the
 * connection a Connection_Handle shall be assigned by the Controller, and
 * passed to the Host in this event. If the connection establishment fails this
 * event shall be provided to the Host that had issued the LE_Create_Connection
 * command.
 * This event indicates to the Host which issued a LE_Create_Connection command
 * and received a Command Status event if the connection establishment failed
 * or was successful.
 * The Central_Clock_Accuracy parameter is only valid for a Peripheral. On a
 * Central, this parameter is set to 0x00.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.1].
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Role Role of the local device in the connection.
 *        Values:
 *        - 0x00: Central
 *        - 0x01: Peripheral
 * @param Peer_Address_Type The address type of the peer device.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 * @param Peer_Address Public Device Address or Random Device Address of the
 *        peer device
 * @param Conn_Interval Connection interval used on this connection.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Latency Maximum Peripheral latency for the connection in number
 *        of connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Supervision_Timeout Supervision timeout for the LE Link.
 *        It shall be a multiple of 10 ms and larger than (1 +
 *        connPeripheralLatency) * connInterval * 2.
 *        Time = N * 10 ms.
 *        Values:
 *        - 0x000A (100 ms)  ... 0x0C80 (32000 ms)
 * @param Central_Clock_Accuracy Central clock accuracy. Only valid for a
 *        Peripheral.
 *        Values:
 *        - 0x00: 500 ppm
 *        - 0x01: 250 ppm
 *        - 0x02: 150 ppm
 *        - 0x03: 100 ppm
 *        - 0x04: 75 ppm
 *        - 0x05: 50 ppm
 *        - 0x06: 30 ppm
 *        - 0x07: 20 ppm
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_connection_complete_event( uint8_t Status,
                                             uint16_t Connection_Handle,
                                             uint8_t Role,
                                             uint8_t Peer_Address_Type,
                                             const uint8_t* Peer_Address,
                                             uint16_t Conn_Interval,
                                             uint16_t Conn_Latency,
                                             uint16_t Supervision_Timeout,
                                             uint8_t Central_Clock_Accuracy );

/**
 * @brief HCI_LE_ADVERTISING_REPORT_EVENT
 * The LE Advertising Report event indicates that a device or multiple devices
 * have responded to an active scan or received some information during a
 * passive scan. The Controller may queue these advertising reports and send
 * information from multiple devices in one LE Advertising Report event.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.2].
 * Note: in the current BLE stack version, only one report is sent per event
 * (Num_Reports = 1).
 * 
 * @param Num_Reports Number of responses in this event.
 *        Values:
 *        - 0x01
 * @param Advertising_Report See @ref Advertising_Report_t
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_advertising_report_event( uint8_t Num_Reports,
                                            const Advertising_Report_t* Advertising_Report );

/**
 * @brief HCI_LE_CONNECTION_UPDATE_COMPLETE_EVENT
 * The LE Connection Update Complete event is used to indicate that the
 * Controller process to update the connection has completed.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.3].
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Conn_Interval Connection interval used on this connection.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Latency Maximum Peripheral latency for the connection in number
 *        of connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Supervision_Timeout Supervision timeout for the LE Link.
 *        It shall be a multiple of 10 ms and larger than (1 +
 *        connPeripheralLatency) * connInterval * 2.
 *        Time = N * 10 ms.
 *        Values:
 *        - 0x000A (100 ms)  ... 0x0C80 (32000 ms)
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_connection_update_complete_event( uint8_t Status,
                                                    uint16_t Connection_Handle,
                                                    uint16_t Conn_Interval,
                                                    uint16_t Conn_Latency,
                                                    uint16_t Supervision_Timeout );

/**
 * @brief HCI_LE_READ_REMOTE_FEATURES_COMPLETE_EVENT
 * This event is used to indicate the completion of the process of the
 * Controller obtaining the used features of the remote Bluetooth device
 * specified by the Connection_Handle event parameter.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.4].
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param LE_Features Bit Mask List of used LE features. For details see LE
 *        Link Layer specification.
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_read_remote_features_complete_event( uint8_t Status,
                                                       uint16_t Connection_Handle,
                                                       const uint8_t* LE_Features );

/**
 * @brief HCI_LE_LONG_TERM_KEY_REQUEST_EVENT
 * This event indicates that the Central is attempting to encrypt or re-encrypt
 * the link and is requesting the Long Term Key from the Host.
 * See Bluetooth spec. v.5.4 [Vol 6, Part B, 5.1.3] and [Vol 4, Part E,
 * 7.7.65.5].
 * 
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Random_Number 64-bit random number
 * @param Encrypted_Diversifier 16-bit encrypted diversifier
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_long_term_key_request_event( uint16_t Connection_Handle,
                                               const uint8_t* Random_Number,
                                               uint16_t Encrypted_Diversifier );

/**
 * @brief HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_EVENT
 * This event indicates to the Central's Host or the Peripheral's Host that the
 * remote device is requesting a change in the connection parameters using the
 * Connection Update procedure. The Host replies either with the
 * HCI_LE_Remote_Connection_Parameter_Request_Reply command or the
 * HCI_LE_Remote_Connection_Parameter_Request_Negative_Reply command.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.6].
 * 
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Interval_Min Minimum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Interval_Max Maximum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Max_Latency Maximum Peripheral latency for the connection in number
 *        of connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Timeout Supervision timeout for the LE Link.
 *        It shall be a multiple of 10 ms and larger than (1 +
 *        connPeripheralLatency) * connInterval * 2.
 *        Time = N * 10 ms.
 *        Values:
 *        - 0x000A (100 ms)  ... 0x0C80 (32000 ms)
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_remote_connection_parameter_request_event( uint16_t Connection_Handle,
                                                             uint16_t Interval_Min,
                                                             uint16_t Interval_Max,
                                                             uint16_t Max_Latency,
                                                             uint16_t Timeout );

/**
 * @brief HCI_LE_DATA_LENGTH_CHANGE_EVENT
 * This event notifies the Host of a change to either the maximum Payload
 * length or the maximum transmission time of packets in either direction. The
 * values reported are the maximum that will actually be used on the connection
 * following the change, except that on the LE Coded PHY a packet taking up to
 * 2704 us to transmit may be sent even though the corresponding parameter has
 * a lower value.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.7] and [Vol 6, Part B,
 * 4.5.10].
 * 
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param MaxTxOctets The maximum number of payload octets in a Link Layer
 *        packet that the local Controller will send on this connection
 *        (connEffectiveMaxTxOctets).
 *        Values:
 *        - 0x001B ... 0x00FB
 * @param MaxTxTime The maximum time that the local Controller will take to
 *        send a Link Layer packet on this connection (connEffectiveMaxTxTime).
 *        Values:
 *        - 0x0148 ... 0x4290
 * @param MaxRxOctets The maximum number of payload octets in a Link Layer
 *        packet that the local Controller expects to receive on this
 *        connection (connEffectiveMaxRxOctets).
 *        Values:
 *        - 0x001B ... 0x00FB
 * @param MaxRxTime The maximum time that the local Controller expects to take
 *        to receive a Link Layer packet on this connection
 *        (connEffectiveMaxRxTime).
 *        Values:
 *        - 0x0148 ... 0x4290
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_data_length_change_event( uint16_t Connection_Handle,
                                            uint16_t MaxTxOctets,
                                            uint16_t MaxTxTime,
                                            uint16_t MaxRxOctets,
                                            uint16_t MaxRxTime );

/**
 * @brief HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT
 * This event is generated when local P-256 key generation is complete.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.8].
 * 
 * @param Status Status error code.
 * @param Local_P256_Public_Key Local P-256 public key.
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_read_local_p256_public_key_complete_event( uint8_t Status,
                                                             const uint8_t* Local_P256_Public_Key );

/**
 * @brief HCI_LE_GENERATE_DHKEY_COMPLETE_EVENT
 * This event indicates that LE Diffie Hellman key generation has been
 * completed by the Controller.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.9].
 * 
 * @param Status Status error code.
 * @param DHKey Diffie Hellman Key
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_generate_dhkey_complete_event( uint8_t Status,
                                                 const uint8_t* DHKey );

/**
 * @brief HCI_LE_ENHANCED_CONNECTION_COMPLETE_EVENT
 * This event indicates to both of the Hosts forming the connection that a new
 * connection has been created. Upon the creation of the connection a
 * Connection_Handle shall be assigned by the Controller, and passed to the
 * Host in this event. If the connection establishment fails, this event shall
 * be provided to the Host that had issued the LE_Create_Connection command.
 * If this event is unmasked and LE Connection Complete event is unmasked, only
 * the LE Enhanced Connection Complete event is sent when a new connection has
 * been completed.
 * This event indicates to the Host that issued a LE_Create_Connection command
 * and received a Command Status event if the connection establishment failed
 * or was successful.
 * The Central_Clock_Accuracy parameter is only valid for a Peripheral. On a
 * Central, this parameter is set to 0x00.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.10].
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Role Role of the local device in the connection.
 *        Values:
 *        - 0x00: Central
 *        - 0x01: Peripheral
 * @param Peer_Address_Type Address type
 *        0x00 Public Device Address
 *        0x01 Random Device Address
 *        0x02 Public Identity Address (Corresponds to Resolved Private
 *        Address)
 *        0x03 Random (Static) Identity Address (Corresponds to Resolved
 *        Private Address)
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 *        - 0x02: Public Identity Address
 *        - 0x03: Random (Static) Identity Address
 * @param Peer_Address Public Device Address, Random Device Address, Public
 *        Identity Address or Random (static) Identity Address of the device to
 *        be connected.
 * @param Local_Resolvable_Private_Address Resolvable Private Address being
 *        used by the local device for this connection.
 *        This is only valid when the Own_Address_Type is set to 0x02 or 0x03.
 *        For other Own_Address_Type values, the Controller shall return all
 *        zeros.
 * @param Peer_Resolvable_Private_Address Resolvable Private Address being used
 *        by the peer device for this connection.
 *        This is only valid for Peer_Address_Type 0x02 and 0x03. For other
 *        Peer_Address_Type values, the Controller shall return all zeros.
 * @param Conn_Interval Connection interval used on this connection.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Latency Maximum Peripheral latency for the connection in number
 *        of connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Supervision_Timeout Supervision timeout for the LE Link.
 *        It shall be a multiple of 10 ms and larger than (1 +
 *        connPeripheralLatency) * connInterval * 2.
 *        Time = N * 10 ms.
 *        Values:
 *        - 0x000A (100 ms)  ... 0x0C80 (32000 ms)
 * @param Central_Clock_Accuracy Central clock accuracy. Only valid for a
 *        Peripheral.
 *        Values:
 *        - 0x00: 500 ppm
 *        - 0x01: 250 ppm
 *        - 0x02: 150 ppm
 *        - 0x03: 100 ppm
 *        - 0x04: 75 ppm
 *        - 0x05: 50 ppm
 *        - 0x06: 30 ppm
 *        - 0x07: 20 ppm
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_enhanced_connection_complete_event( uint8_t Status,
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

/**
 * @brief HCI_LE_DIRECTED_ADVERTISING_REPORT_EVENT
 * This event indicates that directed advertisements have been received where
 * the advertiser is using a resolvable private address for the InitA field in
 * the ADV_DIRECT_IND PDU and the Scanning_Filter_Policy is equal to 0x02 or
 * 0x03, see HCI_LE_Set_Scan_Parameters.
 * Direct_Address_Type and Direct_Address specify the address the directed
 * advertisements are being directed to. Address_Type and Address specify the
 * address of the advertiser sending the directed advertisements.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.11].
 * 
 * @param Num_Reports Number of responses in this event.
 *        Values:
 *        - 0x01
 * @param Direct_Advertising_Report See @ref Direct_Advertising_Report_t
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_directed_advertising_report_event( uint8_t Num_Reports,
                                                     const Direct_Advertising_Report_t* Direct_Advertising_Report );

/**
 * @brief HCI_LE_PHY_UPDATE_COMPLETE_EVENT
 * This event is used to indicate that the Controller has changed the
 * transmitter PHY or receiver PHY in use.
 * If the Controller changes the transmitter PHY, the receiver PHY, or both
 * PHYs, this event shall be issued.
 * If an LE_Set_PHY command was sent and the Controller determines that neither
 * PHY will change as a result, it issues this event immediately.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.12].
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param TX_PHY Transmitter PHY in use.
 *        Values:
 *        - 0x01: The transmitter PHY for the connection is LE 1M
 *        - 0x02: The transmitter PHY for the connection is LE 2M
 *        - 0x03: The transmitter PHY for the connection is LE Coded (not
 *          supported on STM32WB)
 * @param RX_PHY Receiver PHY in use.
 *        Values:
 *        - 0x01: The receiver PHY for the connection is LE 1M
 *        - 0x02: The receiver PHY for the connection is LE 2M
 *        - 0x03: The receiver PHY for the connection is LE Coded (not
 *          supported on STM32WB)
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_phy_update_complete_event( uint8_t Status,
                                             uint16_t Connection_Handle,
                                             uint8_t TX_PHY,
                                             uint8_t RX_PHY );

/**
 * @brief HCI_LE_EXTENDED_ADVERTISING_REPORT_EVENT
 * The HCI_LE_Extended_Advertising_Report event indicates that a Bluetooth
 * device has responded to an active scan or has broadcast advertisements that
 * were received during a passive scan.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.13].
 * 
 * @param Num_Reports Number of responses in this event.
 *        Values:
 *        - 0x01
 * @param Event_Type Event type.
 *        Flags:
 *        - 0x0001: Connectable advertising
 *        - 0x0002: Scannable advertising
 *        - 0x0004: Directed advertising
 *        - 0x0008: Scan response
 *        - 0x0010: Legacy advertising PDUs used
 *        - 0x0020: Incomplete, more data to come
 *        - 0x0040: Incomplete, data truncated, no more to come
 * @param Address_Type Address type of the advertising device.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 *        - 0x02: Public Identity Address (corresponds to Resolved Private
 *          Address)
 *        - 0x03: Random (static) Identity Address (corresponds to Resolved
 *          Private Address)
 *        - 0xFF: No address provided (anonymous advertisement)
 * @param Address Public Device Address, Random Device Address, Public Identity
 *        Address, or Random (static) Identity Address of the advertising
 *        device.
 * @param Primary_PHY Primary advertising PHY.
 *        Values:
 *        - 0x01: Advertiser PHY is LE 1M
 * @param Secondary_PHY Secondary advertising PHY.
 *        Values:
 *        - 0x00: No packets on the secondary advertising physical channel
 *        - 0x01: Advertiser PHY is LE 1M
 *        - 0x02: Advertiser PHY is LE 2M
 *        - 0x03: Advertiser PHY is LE Coded
 * @param Advertising_SID Value of the Advertising SID subfield in the ADI
 *        field of the PDU or, for scan responses, in the ADI field of the
 *        original scannable.
 *        Values:
 *        - 0xFF: No ADI field provided
 *        - 0x00 ... 0x0F: Advertising SID subfield
 * @param TX_Power Tx Power (signed integer).
 *        Units: dBm.
 *        Values:
 *        - 127: Tx power information not available
 *        - -127 ... 20: Tx power
 * @param RSSI RSSI (signed integer).
 *        Units: dBm.
 *        Values:
 *        - 127: RSSI not available
 *        - -127 ... 20
 * @param Periodic_Adv_Interval Interval of the periodic advertising.
 *        Values:
 *        - 0x0000: No periodic advertising
 * @param Direct_Address_Type Target device address type.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 *        - 0x02: Public Identity Address (Corresponds to Resolved Private
 *          Address)
 *        - 0x03: Random (static) Identity Address (Corresponds to Resolved
 *          Private Address)
 *        - 0xFE: Random Device Address (Controller unable to resolve)
 * @param Direct_Address Public Device Address, Random Device Address, Public
 *        Identity Address, or Random (static) Identity Address of the target
 *        device.
 * @param Data_Length Length of Data
 * @param Data Octets of advertising or scan response data formatted as defined
 *        in Bluetooth spec. v.5.4 [Vol 3, Part C, 11].
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_extended_advertising_report_event( uint8_t Num_Reports,
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

/**
 * @brief HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_EVENT
 * This event indicates that the Controller has received the first periodic
 * advertising packet from an advertiser after the
 * HCI_LE_Periodic_Advertising_Create_Sync command has been sent to the
 * Controller.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.14].
 * 
 * @param Status Periodic advertising sync status.
 *        Values:
 *        - 0x00: Periodic advertising sync successful.
 *        - 0x01 ... 0xFF: Periodic advertising sync failed (see "Status error
 *          codes" section).
 * @param Sync_Handle Handle identifying the periodic advertising train.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Advertising_SID Value of the Advertising SID subfield in the ADI
 *        field of the PDU or, for scan responses, in the ADI field of the
 *        original scannable.
 *        Values:
 *        - 0xFF: No ADI field provided
 *        - 0x00 ... 0x0F: Advertising SID subfield
 * @param Advertiser_Address_Type Advertiser address type.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 *        - 0x02: Public Identity Address (Corresponds to Resolved Private
 *          Address)
 *        - 0x03: Random (static) Identity Address (Corresponds to Resolved
 *          Private Address)
 * @param Advertiser_Address Public Device Address, Random Device Address,
 *        Public Identity Address, or Random (static) Identity Address of the
 *        advertiser.
 * @param Advertiser_PHY Advertiser PHY.
 *        Values:
 *        - 0x01: Advertiser PHY is LE 1M
 *        - 0x02: Advertiser PHY is LE 2M
 *        - 0x03: Advertiser PHY is LE Coded
 * @param Periodic_Advertising_Interval Periodic advertising interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0xFFFF (81918.75 ms)
 * @param Advertiser_Clock_Accuracy Advertiser clock accuracy.
 *        Values:
 *        - 0x00: 500 ppm
 *        - 0x01: 250 ppm
 *        - 0x02: 150 ppm
 *        - 0x03: 100 ppm
 *        - 0x04: 75 ppm
 *        - 0x05: 50 ppm
 *        - 0x06: 30 ppm
 *        - 0x07: 20 ppm
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_periodic_advertising_sync_established_event( uint8_t Status,
                                                               uint16_t Sync_Handle,
                                                               uint8_t Advertising_SID,
                                                               uint8_t Advertiser_Address_Type,
                                                               const uint8_t* Advertiser_Address,
                                                               uint8_t Advertiser_PHY,
                                                               uint16_t Periodic_Advertising_Interval,
                                                               uint8_t Advertiser_Clock_Accuracy );

/**
 * @brief HCI_LE_PERIODIC_ADVERTISING_REPORT_EVENT
 * This event indicates that the Controller has received a periodic
 * advertisement.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.15].
 * 
 * @param Sync_Handle Handle identifying the periodic advertising train.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param TX_Power Tx Power (signed integer).
 *        Units: dBm.
 *        Values:
 *        - 127: Tx power information not available
 *        - -127 ... 20: Tx power
 * @param RSSI RSSI (signed integer).
 *        Units: dBm.
 *        Values:
 *        - 127: RSSI not available
 *        - -127 ... 20
 * @param CTE_Type Constant Tone Extension type.
 *        Values:
 *        - 0x00: AoA CTE
 *        - 0x01: AoD CTE with 1 us slots
 *        - 0x02: AoD CTE with 2 us slots
 *        - 0xFF: No CTE
 * @param Data_Status Data status.
 *        Values:
 *        - 0x00: Data complete
 *        - 0x01: Data incomplete, more data to come
 *        - 0x02: Data incomplete, data truncated, no more to come
 *        - 0xFF: Failed to receive an AUX_SYNC_SUBEVENT_IND PDU
 * @param Data_Length Length of Data
 * @param Data Octets of advertising or scan response data formatted as defined
 *        in Bluetooth spec. v.5.4 [Vol 3, Part C, 11].
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_periodic_advertising_report_event( uint16_t Sync_Handle,
                                                     uint8_t TX_Power,
                                                     uint8_t RSSI,
                                                     uint8_t CTE_Type,
                                                     uint8_t Data_Status,
                                                     uint8_t Data_Length,
                                                     const uint8_t* Data );

/**
 * @brief HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST_EVENT
 * This event indicates that the Controller has not received a Periodic
 * Advertising packet from the train identified by Sync_Handle within the
 * timeout period.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.16].
 * 
 * @param Sync_Handle Handle identifying the periodic advertising train.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_periodic_advertising_sync_lost_event( uint16_t Sync_Handle );

/**
 * @brief HCI_LE_SCAN_TIMEOUT_EVENT
 * This event indicates that scanning has ended because the duration has
 * expired.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.17].
 * 
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_scan_timeout_event( void );

/**
 * @brief HCI_LE_ADVERTISING_SET_TERMINATED_EVENT
 * This event indicates that the Controller has terminated advertising in the
 * advertising sets specified by the Advertising_Handle parameter.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.18].
 * 
 * @param Status Status error code.
 * @param Advertising_Handle Used to identify an advertising set.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Num_Completed_Ext_Adv_Events Number of completed extended advertising
 *        events transmitted by the Controller.
 *        Values:
 *        - 0x00 ... 0xFF
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_advertising_set_terminated_event( uint8_t Status,
                                                    uint8_t Advertising_Handle,
                                                    uint16_t Connection_Handle,
                                                    uint8_t Num_Completed_Ext_Adv_Events );

/**
 * @brief HCI_LE_SCAN_REQUEST_RECEIVED_EVENT
 * This event indicates that a SCAN_REQ PDU or an AUX_SCAN_REQ PDU has been
 * received by the advertiser. The request contains a device address from a
 * scanner that is allowed by the advertising filter policy. The advertising
 * set is identified by Advertising_Handle.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.19].
 * 
 * @param Advertising_Handle Used to identify an advertising set.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Scanner_Address_Type Scanner address type.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 *        - 0x02: Public Identity Address (corresponds to Resolved Private
 *          Address)
 *        - 0x03: Random (static) Identity Address (corresponds to Resolved
 *          Private Address)
 * @param Scanner_Address Public Device Address, Random Device Address, Public
 *        Identity Address, or Random (static) Identity Address of the scanner
 *        device.
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_scan_request_received_event( uint8_t Advertising_Handle,
                                               uint8_t Scanner_Address_Type,
                                               const uint8_t* Scanner_Address );

/**
 * @brief HCI_LE_CHANNEL_SELECTION_ALGORITHM_EVENT
 * This event indicates which channel selection algorithm is used on a data
 * physical channel connection.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.20].
 * 
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Channel_Selection_Algorithm LE Channel Selection Algorithm.
 *        Values:
 *        - 0x00: Algorithm #1 is used
 *        - 0x01: Algorithm #2 is used
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_channel_selection_algorithm_event( uint16_t Connection_Handle,
                                                     uint8_t Channel_Selection_Algorithm );

/**
 * @brief HCI_LE_CONNECTIONLESS_IQ_REPORT_EVENT
 * This event is used by the Controller to report IQ information from the
 * Constant Tone Extension of a received advertising packet forming part of the
 * periodic advertising train identified by Sync_Handle and to report IQ
 * information from the Constant Tone Extension of a received Test Mode packet.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.21].
 * 
 * @param Sync_Handle Handle identifying the periodic advertising train.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Channel_Index Index of the channel on which the packet was received.
 *        Note: 0x25 to 0x27 can be used only for packets generated during test
 *        modes.
 *        Values:
 *        - 0x00 ... 0x27
 * @param RSSI RSSI (signed integer).
 *        Units: 0.1 dBm.
 *        Values:
 *        - -1270 ... 200
 * @param RSSI_Antenna_ID ID of the antenna on which the RSSI is measured.
 * @param CTE_Type Constant Tone Extension type.
 *        Values:
 *        - 0x00: AoA CTE
 *        - 0x01: AoD CTE with 1 us slots
 *        - 0x02: AoD CTE with 2 us slots
 * @param Slot_Durations Slot Durations.
 *        Values:
 *        - 0x01: Switching and sampling slots are 1 us each
 *        - 0x02: Switching and sampling slots are 2 us each
 * @param Packet_Status Packet status.
 *        Values:
 *        - 0x00: CRC was correct
 *        - 0x01: CRC was incorrect and the Length and CTETime fields of the
 *          packet were used to determine sampling points
 *        - 0x02: CRC was incorrect but the Controller has determined the
 *          position and length of the CTE in some other way
 *        - 0xFF: Insufficient resources to sample
 * @param Periodic_Event_Counter Value of paEventCounter for the reported
 *        packet.
 * @param Sample_Count Total number of sample pairs
 * @param IQ_Sample I/Q sample pairs
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_connectionless_iq_report_event( uint16_t Sync_Handle,
                                                  uint8_t Channel_Index,
                                                  uint16_t RSSI,
                                                  uint8_t RSSI_Antenna_ID,
                                                  uint8_t CTE_Type,
                                                  uint8_t Slot_Durations,
                                                  uint8_t Packet_Status,
                                                  uint16_t Periodic_Event_Counter,
                                                  uint8_t Sample_Count,
                                                  const uint16_t* IQ_Sample );

/**
 * @brief HCI_LE_CONNECTION_IQ_REPORT_EVENT
 * This event is used by the Controller to report the IQ samples from the
 * Constant Tone Extension of a received packet.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.22].
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param RX_PHY Receiver PHY.
 *        Values:
 *        - 0x01: The receiver PHY for the connection is LE 1M
 *        - 0x02: The receiver PHY for the connection is LE 2M
 * @param Data_Channel_Index Index of the data channel on which the Data
 *        Physical Channel PDU was received.
 *        Values:
 *        - 0x00 ... 0x24
 * @param RSSI RSSI (signed integer).
 *        Units: 0.1 dBm.
 *        Values:
 *        - -1270 ... 200
 * @param RSSI_Antenna_ID ID of the antenna on which the RSSI is measured.
 * @param CTE_Type Constant Tone Extension type.
 *        Values:
 *        - 0x00: AoA CTE
 *        - 0x01: AoD CTE with 1 us slots
 *        - 0x02: AoD CTE with 2 us slots
 * @param Slot_Durations Slot Durations.
 *        Values:
 *        - 0x01: Switching and sampling slots are 1 us each
 *        - 0x02: Switching and sampling slots are 2 us each
 * @param Packet_Status Packet status.
 *        Values:
 *        - 0x00: CRC was correct
 *        - 0x01: CRC was incorrect and the Length and CTETime fields of the
 *          packet were used to determine sampling points
 *        - 0x02: CRC was incorrect but the Controller has determined the
 *          position and length of the CTE in some other way
 *        - 0xFF: Insufficient resources to sample
 * @param Connection_Event_Counter Value of paEventCounter for the reported
 *        packet.
 * @param Sample_Count Total number of sample pairs
 * @param IQ_Sample I/Q sample pairs
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_connection_iq_report_event( uint16_t Connection_Handle,
                                              uint8_t RX_PHY,
                                              uint8_t Data_Channel_Index,
                                              uint16_t RSSI,
                                              uint8_t RSSI_Antenna_ID,
                                              uint8_t CTE_Type,
                                              uint8_t Slot_Durations,
                                              uint8_t Packet_Status,
                                              uint16_t Connection_Event_Counter,
                                              uint8_t Sample_Count,
                                              const uint16_t* IQ_Sample );

/**
 * @brief HCI_LE_CTE_REQUEST_FAILED_EVENT
 * This event is used by the Controller to report an issue following a request
 * to a peer device to reply with a packet containing an LL_CTE_RSP PDU and a
 * Constant Tone Extension. It shall be generated if the packet containing the
 * LL_CTE_RSP PDU sent in response did not contain a Constant Tone Extension or
 * if the peer rejected the request. It shall not be generated if the packet
 * containing the LL_CTE_RSP PDU had a CRC error or if the procedure response
 * timeout timer expired.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.23].
 * 
 * @param Status CTE request failed status.
 *        Values:
 *        - 0x00: LL_CTE_RSP PDU received successfully but without a CTE field
 *        - 0x01 ... 0xFF: Peer rejected the request (see "Status error codes"
 *          section)
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_cte_request_failed_event( uint8_t Status,
                                            uint16_t Connection_Handle );

/**
 * @brief HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_EVENT
 * This event is used by the Controller to report that it has received periodic
 * advertising synchronization information from the device referred to by the
 * Connection_Handle parameter and either successfully synchronized to the
 * periodic advertising train or timed out while attempting to synchronize. The
 * Status will be zero if it successfully synchronized and non-zero otherwise.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.24].
 * 
 * @param Status Periodic advertising sync status.
 *        Values:
 *        - 0x00: Periodic advertising sync successful.
 *        - 0x01 ... 0xFF: Periodic advertising sync failed (see "Status error
 *          codes" section).
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Service_Data Service data.
 * @param Sync_Handle Handle identifying the periodic advertising train.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Advertising_SID Value of the Advertising SID subfield in the ADI
 *        field of the PDU or, for scan responses, in the ADI field of the
 *        original scannable.
 *        Values:
 *        - 0xFF: No ADI field provided
 *        - 0x00 ... 0x0F: Advertising SID subfield
 * @param Advertiser_Address_Type Advertiser address type.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 *        - 0x02: Public Identity Address (Corresponds to Resolved Private
 *          Address)
 *        - 0x03: Random (static) Identity Address (Corresponds to Resolved
 *          Private Address)
 * @param Advertiser_Address Public Device Address, Random Device Address,
 *        Public Identity Address, or Random (static) Identity Address of the
 *        advertiser.
 * @param Advertiser_PHY Advertiser PHY.
 *        Values:
 *        - 0x01: Advertiser PHY is LE 1M
 *        - 0x02: Advertiser PHY is LE 2M
 *        - 0x03: Advertiser PHY is LE Coded
 * @param Periodic_Advertising_Interval Periodic advertising interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0xFFFF (81918.75 ms)
 * @param Advertiser_Clock_Accuracy Advertiser clock accuracy.
 *        Values:
 *        - 0x00: 500 ppm
 *        - 0x01: 250 ppm
 *        - 0x02: 150 ppm
 *        - 0x03: 100 ppm
 *        - 0x04: 75 ppm
 *        - 0x05: 50 ppm
 *        - 0x06: 30 ppm
 *        - 0x07: 20 ppm
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_periodic_advertising_sync_transfer_received_event( uint8_t Status,
                                                                     uint16_t Connection_Handle,
                                                                     uint16_t Service_Data,
                                                                     uint16_t Sync_Handle,
                                                                     uint8_t Advertising_SID,
                                                                     uint8_t Advertiser_Address_Type,
                                                                     const uint8_t* Advertiser_Address,
                                                                     uint8_t Advertiser_PHY,
                                                                     uint16_t Periodic_Advertising_Interval,
                                                                     uint8_t Advertiser_Clock_Accuracy );

/**
 * @brief HCI_LE_CIS_ESTABLISHED_EVENT
 * This event indicates that a CIS has been established, was considered lost
 * before being established, or, on the Central, was rejected by the
 * Peripheral. It is generated by the Controller in the Central and Peripheral.
 * The Connection_Handle parameter shall be set to the value provided in the
 * HCI_LE_Create_CIS command on the Central and in the HCI_LE_CIS_Request event
 * on the Peripheral.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.25].
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param CIG_Sync_Delay The maximum time, in microseconds, for transmission of
 *        PDUs of all CISes in a CIG event.
 *        Values:
 *        - 0x0000EA ... 0x7FFFFF
 * @param CIS_Sync_Delay The maximum time, in microseconds, for transmission of
 *        PDUs of the specified CIS in a CIG event.
 *        Values:
 *        - 0x0000EA ... 0x7FFFFF
 * @param PHY_C_To_P PHY used for transmission from the Central to the
 *        Peripheral.
 *        Values:
 *        - 0x01: The transmitter PHY of packets from the Central is LE 1M
 *        - 0x02: The transmitter PHY of packets from the Central is LE 2M
 *        - 0x03: The transmitter PHY of packets from the Central is LE Coded
 * @param PHY_P_To_C PHY used for transmission from the Peripheral to the
 *        Central.
 *        Values:
 *        - 0x01: The transmitter PHY of packets from the Peripheral is LE 1M
 *        - 0x02: The transmitter PHY of packets from the Peripheral is LE 2M
 *        - 0x03: The transmitter PHY of packets from the Peripheral is LE
 *          Coded
 * @param NSE Number of subevents in each interval of each BIS in the BIG.
 *        Values:
 *        - 0x01 ... 0x1F
 * @param BN_C_To_P Burst number for Central to Peripheral.
 *        Values:
 *        - 0x00: No isochronous data from the Central to the Peripheral
 *        - 0x01 ... 0x0F
 * @param BN_P_To_C Burst number for Peripheral to Central.
 *        Values:
 *        - 0x00: No isochronous data from the Peripheral to the Central
 *        - 0x01 ... 0x0F
 * @param FT_C_To_P Flush timeout in multiples of ISO_Interval for each payload
 *        sent from the Central to Peripheral.
 *        Values:
 *        - 0x01 ... 0xFF
 * @param FT_P_To_C Flush timeout in multiples of ISO_Interval for each payload
 *        sent from the Peripheral to Central.
 *        Values:
 *        - 0x01 ... 0xFF
 * @param Max_PDU_C_To_P Maximum size, in octets, of the payload from the
 *        Central to the Peripheral.
 *        Values:
 *        - 0x0000 ... 0x00FB
 * @param Max_PDU_P_To_C Maximum size, in octets, of the payload from the
 *        Peripheral to the Central.
 *        Values:
 *        - 0x0000 ... 0x00FB
 * @param ISO_Interval Time between consecutive BIG anchor points.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0004 (5.00 ms)  ... 0x0C80 (4000.00 ms)
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_cis_established_event( uint8_t Status,
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

/**
 * @brief HCI_LE_CIS_REQUEST_EVENT
 * This event indicates that a Controller has received a request to establish a
 * CIS. If the Controller receives such a request while the HCI_LE_CIS_Request
 * event is masked away, it shall reject it. Otherwise the Controller shall
 * assign a connection handle for the requested CIS and send the handle in the
 * CIS_Connection_Handle parameter of the event.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.26].
 * 
 * @param ACL_Connection_Handle Connection handle of an ACL connection.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param CIS_Connection_Handle Connection handle of a CIS.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param CIG_ID CIG identifier.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param CIS_ID CIS identifier.
 *        Values:
 *        - 0x00 ... 0xEF
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_cis_request_event( uint16_t ACL_Connection_Handle,
                                     uint16_t CIS_Connection_Handle,
                                     uint8_t CIG_ID,
                                     uint8_t CIS_ID );

/**
 * @brief HCI_LE_CREATE_BIG_COMPLETE_EVENT
 * This event indicates that the HCI_LE_Create_BIG command has completed.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.27].
 * 
 * @param BIG_Handle BIG identifier.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param BIG_Sync_Delay Maximum time in microseconds for transmission of PDUs
 *        of all BISes in a BIG event.
 *        Values:
 *        - 0x0000EA ... 0x7FFFFF
 * @param Transport_Latency_BIG Transport latency, in microseconds.
 *        Values:
 *        - 0x0000EA ... 0x7FFFFF
 * @param PHY PHY used to create the BIG.
 *        Values:
 *        - 0x01: The PHY used to create the BIG is LE 1M
 *        - 0x02: The PHY used to create the BIG is LE 2M
 *        - 0x03: The PHY used to create the BIG is LE Coded
 * @param NSE Number of subevents in each interval of each BIS in the BIG.
 *        Values:
 *        - 0x01 ... 0x1F
 * @param BN Number of new payloads in each interval for each BIS.
 *        Values:
 *        - 0x01 ... 0x07
 * @param PTO Offset used for pre-transmissions.
 *        Values:
 *        - 0x00 ... 0x0F
 * @param IRC Number of times the scheduled payloads are transmitted in a given
 *        event.
 *        Values:
 *        - 0x01 ... 0x0F
 * @param Max_PDU Maximum size, in octets, of payload.
 *        Values:
 *        - 0x0001 ... 0x00FB
 * @param ISO_Interval Time between consecutive BIG anchor points.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0004 (5.00 ms)  ... 0x0C80 (4000.00 ms)
 * @param Num_BIS Total number of BISes in the BIG.
 *        Values:
 *        - 0x01 ... 0x1F
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_create_big_complete_event( uint8_t Status,
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
                                             const uint16_t* Connection_Handle );

/**
 * @brief HCI_LE_TERMINATE_BIG_COMPLETE_EVENT
 * This event indicates that the transmission of all the BISes in the BIG are
 * terminated.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.28].
 * 
 * @param BIG_Handle BIG identifier.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Reason See "Status error codes" section.
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_terminate_big_complete_event( uint8_t BIG_Handle,
                                                uint8_t Reason );

/**
 * @brief HCI_LE_BIG_SYNC_ESTABLISHED_EVENT
 * This event indicates that the HCI_LE_BIG_Create_Sync command has completed.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.29].
 * 
 * @param BIG_Handle BIG identifier.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Transport_Latency_BIG Transport latency, in microseconds.
 *        Values:
 *        - 0x0000EA ... 0x7FFFFF
 * @param NSE Number of subevents in each interval of each BIS in the BIG.
 *        Values:
 *        - 0x01 ... 0x1F
 * @param BN Number of new payloads in each interval for each BIS.
 *        Values:
 *        - 0x01 ... 0x07
 * @param PTO Offset used for pre-transmissions.
 *        Values:
 *        - 0x00 ... 0x0F
 * @param IRC Number of times the scheduled payloads are transmitted in a given
 *        event.
 *        Values:
 *        - 0x01 ... 0x0F
 * @param Max_PDU Maximum size, in octets, of payload.
 *        Values:
 *        - 0x0001 ... 0x00FB
 * @param ISO_Interval Time between consecutive BIG anchor points.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0004 (5.00 ms)  ... 0x0C80 (4000.00 ms)
 * @param Num_BIS Total number of BISes in the BIG.
 *        Values:
 *        - 0x01 ... 0x1F
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_big_sync_established_event( uint8_t Status,
                                              uint8_t BIG_Handle,
                                              const uint8_t* Transport_Latency_BIG,
                                              uint8_t NSE,
                                              uint8_t BN,
                                              uint8_t PTO,
                                              uint8_t IRC,
                                              uint16_t Max_PDU,
                                              uint16_t ISO_Interval,
                                              uint8_t Num_BIS,
                                              const uint16_t* Connection_Handle );

/**
 * @brief HCI_LE_BIG_SYNC_LOST_EVENT
 * This event indicates that the Controller has not received any PDUs on a BIG
 * within the timeout period BIG_Sync_Timeout or the BIG has been terminated by
 * the remote device.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.30].
 * 
 * @param BIG_Handle BIG identifier.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Reason See "Status error codes" section.
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_big_sync_lost_event( uint8_t BIG_Handle,
                                       uint8_t Reason );

/**
 * @brief HCI_LE_REQUEST_PEER_SCA_COMPLETE_EVENT
 * This event indicates that the HCI_LE_Request_Peer_SCA command has been
 * completed.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.31].
 * 
 * @param Status Peer clock accuracy request status.
 *        Values:
 *        - 0x00: The Peer_Clock_Accuracy parameter is successfully received
 *        - 0x01 ... 0xFF: The reception of Peer_Clock_Accuracy parameter
 *          failed (see "Status error codes" section)
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Peer_Clock_Accuracy Sleep clock accuracy of the peer.
 *        Values:
 *        - 0x00: 251 ppm to 500 ppm
 *        - 0x01: 151 ppm to 250 ppm
 *        - 0x02: 101 ppm to 150 ppm
 *        - 0x03: 76 ppm to 100 ppm
 *        - 0x04: 51 ppm to 75 ppm
 *        - 0x05: 31 ppm to 50 ppm
 *        - 0x06: 21 ppm to 30 ppm
 *        - 0x07: 0 ppm to 20 ppm
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_request_peer_sca_complete_event( uint8_t Status,
                                                   uint16_t Connection_Handle,
                                                   uint8_t Peer_Clock_Accuracy );

/**
 * @brief HCI_LE_PATH_LOSS_THRESHOLD_EVENT
 * This event is used to report a path loss threshold crossing on the ACL
 * connection identified by the Connection_Handle parameter.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.32].
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Current_Path_Loss Current path loss (always zero or positive).
 *        Units: dB.
 *        Values:
 *        - 255: Unavailable
 *        - 0 ... 254
 * @param Zone_Entered Entered zone (shall be ignored if Current_Path_Loss is
 *        set to 0xFF).
 *        Values:
 *        - 0x00: Entered low zone
 *        - 0x01: Entered middle zone
 *        - 0x02: Entered high zone
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_path_loss_threshold_event( uint16_t Connection_Handle,
                                             uint8_t Current_Path_Loss,
                                             uint8_t Zone_Entered );

/**
 * @brief HCI_LE_TRANSMIT_POWER_REPORTING_EVENT
 * This event is used to report the transmit power level on the ACL connection
 * identified by the Connection_Handle parameter.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.33].
 * 
 * @param Status Status error code.
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Reason Event reason.
 *        Values:
 *        - 0x00: Local transmit power changed
 *        - 0x01: Remote transmit power changed
 *        - 0x02: HCI_LE_Read_Remote_Transmit_Power_Level command completed
 * @param PHY Applying PHY.
 *        Values:
 *        - 0x01: LE 1M PHY
 *        - 0x02: LE 2M PHY
 *        - 0x03: LE Coded PHY with S=8 data coding
 *        - 0x04: LE Coded PHY with S=2 data coding
 * @param TX_Power_Level Transmit power level (signed integer).
 *        Units: dBm.
 *        Values:
 *        - 126: Remote device is not managing power levels on this PHY.
 *        - 127: Transmit power level is not available
 *        - -127 ... 20
 * @param TX_Power_Level_Flag Indicates whether the transmit power level has
 *        reached its minimum and/or maximum level. Shall be ignored if the
 *        TX_Power_Level parameter is set to 0x7E or 0x7F.
 *        Values:
 *        - 0x00: Transmit power level is at minimum level
 *        - 0x01: Transmit power level is at maximum level
 * @param Delta Change in transmit power level (signed integer - positive
 *        indicates increased power, negative indicates decreased power, zero
 *        indicates unchanged).
 *        Units: dB.
 *        Values:
 *        - 127: Change is not available or is out of range
 *        - -127 ... 126
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_transmit_power_reporting_event( uint8_t Status,
                                                  uint16_t Connection_Handle,
                                                  uint8_t Reason,
                                                  uint8_t PHY,
                                                  uint8_t TX_Power_Level,
                                                  uint8_t TX_Power_Level_Flag,
                                                  uint8_t Delta );

/**
 * @brief HCI_LE_BIGINFO_ADVERTISING_REPORT_EVENT
 * This event indicates that the Controller has received an Advertising PDU
 * that contained a BIGInfo field. If the Controller also generates an
 * HCI_LE_Periodic_Advertising_Report event, the
 * HCI_LE_BIGInfo_Advertising_Report event shall immediately follow that event.
 * See Bluetooth spec. v.5.4 [Vol 4, Part E, 7.7.65.34].
 * 
 * @param Sync_Handle Handle identifying the periodic advertising train.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Num_BIS Total number of BISes in the BIG.
 *        Values:
 *        - 0x01 ... 0x1F
 * @param NSE Number of subevents in each interval of each BIS in the BIG.
 *        Values:
 *        - 0x01 ... 0x1F
 * @param ISO_Interval Time between consecutive BIG anchor points.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0004 (5.00 ms)  ... 0x0C80 (4000.00 ms)
 * @param BN Number of new payloads in each interval for each BIS.
 *        Values:
 *        - 0x01 ... 0x07
 * @param PTO Offset used for pre-transmissions.
 *        Values:
 *        - 0x00 ... 0x0F
 * @param IRC Number of times the scheduled payloads are transmitted in a given
 *        event.
 *        Values:
 *        - 0x01 ... 0x0F
 * @param Max_PDU Maximum size, in octets, of payload.
 *        Values:
 *        - 0x0001 ... 0x00FB
 * @param SDU_Interval Interval, in microseconds, of periodic SDUs.
 *        Values:
 *        - 0x0000FF ... 0x0FFFFF
 * @param Max_SDU Maximum size of an SDU in octets.
 *        Values:
 *        - 0x0001 ... 0x0FFF
 * @param PHY PHY of the Advertising PDU.
 *        Values:
 *        - 0x01: The BIG is transmitted on the LE 1M PHY
 *        - 0x02: The BIG is transmitted on the LE 2M PHY
 *        - 0x03: The BIG is transmitted on the LE Coded PHY
 * @param Framing Format of the CIS Data PDUs.
 *        Values:
 *        - 0x00: Unframed
 *        - 0x01: Framed
 * @param Encryption Encryption mode of the BISes.
 *        Values:
 *        - 0x00: Unencrypted
 *        - 0x01: Encrypted
 * @return Value indicating success or error code.
 */
tBleStatus hci_le_biginfo_advertising_report_event( uint16_t Sync_Handle,
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

/* ACI GAP events */

/**
 * @brief ACI_GAP_LIMITED_DISCOVERABLE_EVENT
 * This event is generated by the controller when the limited discoverable mode
 * ends due to timeout. The timeout is 180 seconds.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_limited_discoverable_event( void );

/**
 * @brief ACI_GAP_PAIRING_COMPLETE_EVENT
 * This event is generated when the pairing process has completed successfully
 * or a pairing procedure timeout has occurred or the pairing has failed. This
 * is to notify the application that we have paired with a remote device so
 * that it can take further actions or to notify that a timeout has occurred so
 * that the upper layer can decide to disconnect the link.
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Status Pairing status
 *        Values:
 *        - 0x00: Success
 *        - 0x01: SMP timeout
 *        - 0x02: Pairing failed
 *        - 0x03: Encryption failed
 * @param Reason Pairing reason error code (valid in case of pairing failed
 *        status)
 *        Values:
 *        - 0x02: OOB_NOT_AVAILABLE
 *        - 0x03: AUTH_REQ_CANNOT_BE_MET
 *        - 0x04: CONFIRM_VALUE_FAILED
 *        - 0x05: PAIRING_NOT_SUPPORTED
 *        - 0x06: INSUFF_ENCRYPTION_KEY_SIZE
 *        - 0x07: CMD_NOT_SUPPORTED
 *        - 0x08: UNSPECIFIED_REASON
 *        - 0x09: VERY_EARLY_NEXT_ATTEMPT
 *        - 0x0A: SM_INVALID_PARAMS
 *        - 0x0B: SMP_SC_DHKEY_CHECK_FAILED
 *        - 0x0C: SMP_SC_NUMCOMPARISON_FAILED
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_pairing_complete_event( uint16_t Connection_Handle,
                                           uint8_t Status,
                                           uint8_t Reason );

/**
 * @brief ACI_GAP_PASS_KEY_REQ_EVENT
 * This event is generated by the Security manager to the application when a
 * passkey is required for pairing. When this event is received, the
 * application has to respond with the ACI_GAP_PASS_KEY_RESP command.
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_pass_key_req_event( uint16_t Connection_Handle );

/**
 * @brief ACI_GAP_AUTHORIZATION_REQ_EVENT
 * This event is generated by the Security manager to the application when the
 * application has set that authorization is required for reading/writing of
 * attributes. This event will be generated as soon as the pairing is complete.
 * When this event is received, ACI_GAP_AUTHORIZATION_RESP command should be
 * used to respond by the application.
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_authorization_req_event( uint16_t Connection_Handle );

/**
 * @brief ACI_GAP_PERIPHERAL_SECURITY_INITIATED_EVENT
 * This event is generated when the Peripheral Security Request is successfully
 * sent to the Central.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_peripheral_security_initiated_event( void );

/**
 * @brief ACI_GAP_BOND_LOST_EVENT
 * This event is generated when a pairing request is issued in response to a
 * Peripheral Security Request from a Central which has previously bonded with
 * the Peripheral. When this event is received, the upper layer has to issue
 * the command ACI_GAP_ALLOW_REBOND in order to allow the Peripheral to
 * continue the pairing process with the Central.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_bond_lost_event( void );

/**
 * @brief ACI_GAP_PROC_COMPLETE_EVENT
 * This event is sent by the GAP to the upper layers when a procedure
 * previously started has been terminated by the upper layer or has completed
 * for any other reason
 * 
 * @param Procedure_Code Terminated procedure.
 *        Values:
 *        - 0x01: GAP_LIMITED_DISCOVERY_PROC
 *        - 0x02: GAP_GENERAL_DISCOVERY_PROC
 *        - 0x08: GAP_AUTO_CONNECTION_ESTABLISHMENT_PROC
 *        - 0x10: GAP_GENERAL_CONNECTION_ESTABLISHMENT_PROC
 *        - 0x20: GAP_SELECTIVE_CONNECTION_ESTABLISHMENT_PROC
 *        - 0x40: GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC
 *        - 0x80: GAP_OBSERVATION_PROC
 * @param Status Status error code.
 * @param Data_Length Length of Data in octets
 * @param Data Procedure Specific Data:
 *        - For Name Discovery Procedure: the name of the peer device if the
 *        procedure completed successfully.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_proc_complete_event( uint8_t Procedure_Code,
                                        uint8_t Status,
                                        uint8_t Data_Length,
                                        const uint8_t* Data );

/**
 * @brief ACI_GAP_ADDR_NOT_RESOLVED_EVENT
 * This event is sent only by a privacy enabled peripheral with a non-empty
 * bonded device list. The event is sent to the application when the peripheral
 * is unsuccessful in resolving the resolvable address of the peer device after
 * connecting to it.
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_addr_not_resolved_event( uint16_t Connection_Handle );

/**
 * @brief ACI_GAP_NUMERIC_COMPARISON_VALUE_EVENT
 * This event is sent only during SC Pairing, when Numeric Comparison
 * Association model is selected, in order to show the Numeric Value generated,
 * and to ask for Confirmation to the User. When this event is received, the
 * application has to respond with the ACI_GAP_NUMERIC_COMPARISON_RESP command.
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Numeric_Value Generated numeric value.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_numeric_comparison_value_event( uint16_t Connection_Handle,
                                                   uint32_t Numeric_Value );

/**
 * @brief ACI_GAP_KEYPRESS_NOTIFICATION_EVENT
 * This event is sent only during SC Pairing, when Keypress Notifications are
 * supported, in order to show the input type signaled by the peer device,
 * having Keyboard only I/O capabilities. When this event is received, no
 * action is required to the User.
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Notification_Type Type of Keypress input notified/signaled by peer
 *        device (having Keyboard only I/O capabilities.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_keypress_notification_event( uint16_t Connection_Handle,
                                                uint8_t Notification_Type );

/* ACI GATT/ATT events */

/**
 * @brief ACI_GATT_ATTRIBUTE_MODIFIED_EVENT
 * This event is generated to the application by the GATT server when a client
 * modifies any attribute on the server, as consequence of one of the following
 * GATT procedures:
 * - write without response
 * - signed write without response
 * - write characteristic value
 * - write long characteristic value
 * - reliable write.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attr_Handle Handle of the attribute that was modified.
 * @param Offset Bits 14-0: offset from which the write has been performed by
 *        the peer device. Bit 15 is used as flag: when set to 1 it indicates
 *        that more data are to come (fragmented event in case of long
 *        attribute data).
 * @param Attr_Data_Length Length of Attr_Data in octets
 * @param Attr_Data The modified value
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_attribute_modified_event( uint16_t Connection_Handle,
                                              uint16_t Attr_Handle,
                                              uint16_t Offset,
                                              uint16_t Attr_Data_Length,
                                              const uint8_t* Attr_Data );

/**
 * @brief ACI_GATT_PROC_TIMEOUT_EVENT
 * This event is generated by the client/server to the application on a GATT
 * timeout (30 seconds). This is a critical event that should not happen during
 * normal operating conditions. It is an indication of either a major
 * disruption in the communication link or a mistake in the application which
 * does not provide a reply to GATT procedures. After this event, the GATT
 * channel is closed and no more GATT communication can be performed. The
 * application is expected to issue an ACI_GAP_TERMINATE to disconnect from the
 * peer device. It is important to leave a 100 ms blank window before sending
 * the ACI_GAP_TERMINATE, since immediately after this event, system could save
 * important information in non-volatile memory.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_proc_timeout_event( uint16_t Connection_Handle );

/**
 * @brief ACI_ATT_EXCHANGE_MTU_RESP_EVENT
 * This event is generated in response to an Exchange MTU request. See
 * ACI_GATT_EXCHANGE_CONFIG.
 * 
 * @param Connection_Handle Connection handle for which the event applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Server_RX_MTU Attribute server receive MTU size
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_exchange_mtu_resp_event( uint16_t Connection_Handle,
                                            uint16_t Server_RX_MTU );

/**
 * @brief ACI_ATT_FIND_INFO_RESP_EVENT
 * This event is generated in response to a Find Information Request. See
 * ACI_ATT_FIND_INFO_REQ and Find Information Response in Bluetooth Core spec.
 * This event is also generated in response to ACI_GATT_DISC_ALL_CHAR_DESC
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Format Format of the handle-UUID pairs
 * @param Event_Data_Length Length of Handle_UUID_Pair in octets
 * @param Handle_UUID_Pair A sequence of handle-uuid pairs. if format=1, each
 *        pair is:[2 octets for handle, 2 octets for UUIDs], if format=2, each
 *        pair is:[2 octets for handle, 16 octets for UUIDs]
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_find_info_resp_event( uint16_t Connection_Handle,
                                         uint8_t Format,
                                         uint8_t Event_Data_Length,
                                         const uint8_t* Handle_UUID_Pair );

/**
 * @brief ACI_ATT_FIND_BY_TYPE_VALUE_RESP_EVENT
 * This event is generated in response to a ACI_ATT_FIND_BY_TYPE_VALUE_REQ
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Num_of_Handle_Pair Number of attribute, group handle pairs
 * @param Attribute_Group_Handle_Pair See @ref Attribute_Group_Handle_Pair_t
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_find_by_type_value_resp_event( uint16_t Connection_Handle,
                                                  uint8_t Num_of_Handle_Pair,
                                                  const Attribute_Group_Handle_Pair_t* Attribute_Group_Handle_Pair );

/**
 * @brief ACI_ATT_READ_BY_TYPE_RESP_EVENT
 * This event is generated in response to a ACI_ATT_READ_BY_TYPE_REQ. See
 * ACI_GATT_FIND_INCLUDED_SERVICES and ACI_GATT_DISC_ALL_CHAR_DESC.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Handle_Value_Pair_Length The size of each attribute handle-value pair
 * @param Data_Length Length of Handle_Value_Pair_Data in octets
 * @param Handle_Value_Pair_Data Attribute Data List as defined in Bluetooth
 *        Core spec. A sequence of handle-value pairs: [2 octets for Attribute
 *        Handle, (Handle_Value_Pair_Length - 2 octets) for Attribute Value]
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_read_by_type_resp_event( uint16_t Connection_Handle,
                                            uint8_t Handle_Value_Pair_Length,
                                            uint8_t Data_Length,
                                            const uint8_t* Handle_Value_Pair_Data );

/**
 * @brief ACI_ATT_READ_RESP_EVENT
 * This event is generated in response to a Read Request. See
 * ACI_GATT_READ_CHAR_VALUE.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Event_Data_Length Length of following data
 * @param Attribute_Value The value of the attribute.
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_read_resp_event( uint16_t Connection_Handle,
                                    uint8_t Event_Data_Length,
                                    const uint8_t* Attribute_Value );

/**
 * @brief ACI_ATT_READ_BLOB_RESP_EVENT
 * This event can be generated during a read long characteristic value
 * procedure. See ACI_GATT_READ_LONG_CHAR_VALUE.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Event_Data_Length Length of following data
 * @param Attribute_Value Part of the attribute value.
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_read_blob_resp_event( uint16_t Connection_Handle,
                                         uint8_t Event_Data_Length,
                                         const uint8_t* Attribute_Value );

/**
 * @brief ACI_ATT_READ_MULTIPLE_RESP_EVENT
 * This event is generated in response to a Read Multiple Characteristic Values
 * Request or a Read Multiple Variable Length Characteristic Values Request.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Event_Data_Length Length of following data
 * @param Set_Of_Values A set of two or more values.
 *        A concatenation of attribute values for each of the attribute handles
 *        in the request in the order that they were requested.
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_read_multiple_resp_event( uint16_t Connection_Handle,
                                             uint8_t Event_Data_Length,
                                             const uint8_t* Set_Of_Values );

/**
 * @brief ACI_ATT_READ_BY_GROUP_TYPE_RESP_EVENT
 * This event is generated in response to a Read By Group Type Request. See
 * ACI_GATT_DISC_ALL_PRIMARY_SERVICES.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Data_Length The size of each attribute data
 * @param Data_Length Length of Attribute_Data_List in octets
 * @param Attribute_Data_List Attribute Data List as defined in Bluetooth Core
 *        spec. A sequence of attribute handle, end group handle, attribute
 *        value tuples: [2 octets for Attribute Handle, 2 octets End Group
 *        Handle, (Attribute_Data_Length - 4 octets) for Attribute Value]
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_read_by_group_type_resp_event( uint16_t Connection_Handle,
                                                  uint8_t Attribute_Data_Length,
                                                  uint8_t Data_Length,
                                                  const uint8_t* Attribute_Data_List );

/**
 * @brief ACI_ATT_PREPARE_WRITE_RESP_EVENT
 * This event is generated in response to a ACI_ATT_PREPARE_WRITE_REQ.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Handle The handle of the attribute to be written
 * @param Offset The offset of the first octet to be written.
 * @param Part_Attribute_Value_Length Length of Part_Attribute_Value in octets
 * @param Part_Attribute_Value The value of the attribute to be written
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_prepare_write_resp_event( uint16_t Connection_Handle,
                                             uint16_t Attribute_Handle,
                                             uint16_t Offset,
                                             uint8_t Part_Attribute_Value_Length,
                                             const uint8_t* Part_Attribute_Value );

/**
 * @brief ACI_ATT_EXEC_WRITE_RESP_EVENT
 * This event is generated in response to an Execute Write Request.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @return Value indicating success or error code.
 */
tBleStatus aci_att_exec_write_resp_event( uint16_t Connection_Handle );

/**
 * @brief ACI_GATT_INDICATION_EVENT
 * This event is generated when an indication is received from the server.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Handle The handle of the attribute
 * @param Attribute_Value_Length Length of Attribute_Value in octets
 * @param Attribute_Value The current value of the attribute
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_indication_event( uint16_t Connection_Handle,
                                      uint16_t Attribute_Handle,
                                      uint8_t Attribute_Value_Length,
                                      const uint8_t* Attribute_Value );

/**
 * @brief ACI_GATT_NOTIFICATION_EVENT
 * This event is generated when a notification is received from the server.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Handle The handle of the attribute
 * @param Attribute_Value_Length Length of Attribute_Value in octets
 * @param Attribute_Value The current value of the attribute
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_notification_event( uint16_t Connection_Handle,
                                        uint16_t Attribute_Handle,
                                        uint8_t Attribute_Value_Length,
                                        const uint8_t* Attribute_Value );

/**
 * @brief ACI_GATT_PROC_COMPLETE_EVENT
 * This event is generated when a GATT client procedure completes either with
 * error or successfully.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Error_Code Indicates whether the procedure completed with an error or
 *        was successful (see "Status error codes" section)
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_proc_complete_event( uint16_t Connection_Handle,
                                         uint8_t Error_Code );

/**
 * @brief ACI_GATT_ERROR_RESP_EVENT
 * This event is generated when an Error Response is received from the server.
 * The error response can be given by the server at the end of one of the GATT
 * discovery procedures. This does not mean that the procedure ended with an
 * error, but this error event is part of the procedure itself.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Req_Opcode The request that generated this error response
 * @param Attribute_Handle The attribute handle that generated this error
 *        response
 * @param Error_Code The reason why the request has generated an error response
 *        (ATT error codes)
 *        Values:
 *        - 0x01: Invalid handle
 *        - 0x02: Read not permitted
 *        - 0x03: Write not permitted
 *        - 0x04: Invalid PDU
 *        - 0x05: Insufficient authentication
 *        - 0x06: Request not supported
 *        - 0x07: Invalid offset
 *        - 0x08: Insufficient authorization
 *        - 0x09: Prepare queue full
 *        - 0x0A: Attribute not found
 *        - 0x0B: Attribute not long
 *        - 0x0C: Insufficient encryption key size
 *        - 0x0D: Invalid attribute value length
 *        - 0x0E: Unlikely error
 *        - 0x0F: Insufficient encryption
 *        - 0x10: Unsupported group type
 *        - 0x11: Insufficient resources
 *        - 0x12: Database Out Of Sync
 *        - 0x13: Value Not Allowed
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_error_resp_event( uint16_t Connection_Handle,
                                      uint8_t Req_Opcode,
                                      uint16_t Attribute_Handle,
                                      uint8_t Error_Code );

/**
 * @brief ACI_GATT_DISC_READ_CHAR_BY_UUID_RESP_EVENT
 * This event can be generated during a "Discover Characteristics By UUID"
 * procedure or a "Read using Characteristic UUID" procedure.
 * The attribute value will be a service declaration as defined in Bluetooth
 * spec. v.5.4 [Vol 3, Part G, 3.3.1], when a "Discover Characteristics By
 * UUID" has been started. It will be the value of the Characteristic if a*
 * "Read using Characteristic UUID" has been performed.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Handle The handle of the attribute
 * @param Attribute_Value_Length Length of Attribute_Value in octets
 * @param Attribute_Value The attribute value will be a service declaration as
 *        defined in Bluetooth spec. v.5.4 [Vol 3, Part G, 3.3.1], when a
 *        "Discover Characteristics By UUID" has been started.
 *        It will be the value of the Characteristic if a "Read using
 *        Characteristic UUID" has been performed.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_disc_read_char_by_uuid_resp_event( uint16_t Connection_Handle,
                                                       uint16_t Attribute_Handle,
                                                       uint8_t Attribute_Value_Length,
                                                       const uint8_t* Attribute_Value );

/**
 * @brief ACI_GATT_WRITE_PERMIT_REQ_EVENT
 * This event is given to the application when a write request, write command
 * or signed write command is received by the server from the client. This
 * event will be given to the application only if the event bit for this event
 * generation is set when the characteristic was added.
 * When this event is received, the application has to check whether the value
 * being requested for write can be allowed to be written and respond with the
 * command ACI_GATT_WRITE_RESP.
 * The details of the parameters of the command can be found. Based on the
 * response from the application, the attribute value will be modified by the
 * stack. If the write is rejected by the application, then the value of the
 * attribute will not be modified. In case of a write REQ, an error response
 * will be sent to the client, with the error code as specified by the
 * application.
 * In case of write/signed write commands, no response is sent to the client
 * but the attribute is not modified.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Handle The handle of the attribute
 * @param Data_Length Length of Data field
 * @param Data The data that the client has requested to write
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_write_permit_req_event( uint16_t Connection_Handle,
                                            uint16_t Attribute_Handle,
                                            uint8_t Data_Length,
                                            const uint8_t* Data );

/**
 * @brief ACI_GATT_READ_PERMIT_REQ_EVENT
 * This event is given to the application when a read request or read blob
 * request is received by the server from the client. This event will be given
 * to the application only if the event bit for this event generation is set
 * when the characteristic was added.
 * On receiving this event, the application can update the value of the handle
 * if it desires and when done, it must send the ACI_GATT_ALLOW_READ command to
 * indicate to the stack that it can send the response to the client.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Handle The handle of the attribute
 * @param Offset Contains the offset from which the read has been requested
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_read_permit_req_event( uint16_t Connection_Handle,
                                           uint16_t Attribute_Handle,
                                           uint16_t Offset );

/**
 * @brief ACI_GATT_READ_MULTI_PERMIT_REQ_EVENT
 * This event is given to the application when a read multiple request or read
 * by type request is received by the server from the client. This event will
 * be given to the application only if the event bit for this event generation
 * is set when the characteristic was added.
 * On receiving this event, the application can update the values of the
 * handles if it desires and when done, it must send the ACI_GATT_ALLOW_READ
 * command to indicate to the stack that it can send the response to the
 * client.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Handle_Item See @ref Handle_Item_t
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_read_multi_permit_req_event( uint16_t Connection_Handle,
                                                 uint8_t Number_of_Handles,
                                                 const Handle_Item_t* Handle_Item );

/**
 * @brief ACI_GATT_TX_POOL_AVAILABLE_EVENT
 * Each time BLE stack raises the error code BLE_STATUS_INSUFFICIENT_RESOURCES
 * (0x64), the ACI_GATT_TX_POOL_AVAILABLE_EVENT event is generated as soon as
 * there are at least two buffers available for notifications or write
 * commands.
 * 
 * @param Connection_Handle Not used.
 * @param Available_Buffers Number of buffers available.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_tx_pool_available_event( uint16_t Connection_Handle,
                                             uint16_t Available_Buffers );

/**
 * @brief ACI_GATT_SERVER_CONFIRMATION_EVENT
 * This event is generated when the client has sent the confirmation to a
 * previously sent indication
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_server_confirmation_event( uint16_t Connection_Handle );

/**
 * @brief ACI_GATT_PREPARE_WRITE_PERMIT_REQ_EVENT
 * This event is given to the application when a prepare write request is
 * received by the server from the client. This event will be given to the
 * application only if the event bit for this event generation is set when the
 * characteristic was added.
 * When this event is received, the application has to check whether the value
 * being requested for write can be allowed to be written and respond with the
 * command ACI_GATT_WRITE_RESP. Based on the response from the application, the
 * attribute value will be modified by the stack.
 * If the write is rejected by the application, then the value of the attribute
 * will not be modified and an error response will be sent to the client, with
 * the error code as specified by the application.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Handle The handle of the attribute
 * @param Offset The offset from which the prepare write has been requested
 * @param Data_Length Length of Data field
 * @param Data The data that the client has requested to write
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_prepare_write_permit_req_event( uint16_t Connection_Handle,
                                                    uint16_t Attribute_Handle,
                                                    uint16_t Offset,
                                                    uint8_t Data_Length,
                                                    const uint8_t* Data );

/**
 * @brief ACI_GATT_EATT_BEARER_EVENT
 * This event informs the application of a change in status of the Enhanced ATT
 * bearer handled by the specified L2CAP channel.
 * 
 * @param Channel_Index Index of the connection-oriented channel for which the
 *        primitive applies.
 * @param EAB_State Enhanced ATT bearer state.
 *        Values:
 *        - 0x00: Enhanced ATT bearer created
 *        - 0x01: Enhanced ATT bearer terminated
 * @param Status Status error code.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_eatt_bearer_event( uint8_t Channel_Index,
                                       uint8_t EAB_State,
                                       uint8_t Status );

/**
 * @brief ACI_GATT_MULT_NOTIFICATION_EVENT
 * This event is generated when a Multiple Handle Value notification is
 * received from the server.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Offset Bits 14-0: offset in octets from which Attribute_Value data
 *        starts. Bit 15 is used as flag: when set to 1 it indicates that more
 *        data are to come (fragmented event in case of long attribute data).
 * @param Data_Length Length of Data in bytes
 * @param Data List of "Handle Length Value" tuples as defined in Bluetooth
 *        Core specification
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_mult_notification_event( uint16_t Connection_Handle,
                                             uint16_t Offset,
                                             uint16_t Data_Length,
                                             const uint8_t* Data );

/**
 * @brief ACI_GATT_NOTIFICATION_COMPLETE_EVENT
 * This event is generated on server side after the transmission of all
 * notifications linked with a local update of a characteristic value (if it is
 * enabled at the creation of the characteristic with
 * GATT_NOTIFY_NOTIFICATION_COMPLETION mask and if the characteristic supports
 * notifications).
 * 
 * @param Attr_Handle Handle of the updated characteristic value
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_notification_complete_event( uint16_t Attr_Handle );

/**
 * @brief ACI_GATT_READ_EXT_EVENT
 * When it is enabled with ACI_GATT_SET_EVENT_MASK, this event is generated
 * instead of ACI_ATT_READ_RESP_EVENT / ACI_ATT_READ_BLOB_RESP_EVENT /
 * ACI_ATT_READ_MULTIPLE_RESP_EVENT.
 * This event should be used instead of those events when ATT_MTU >
 * (BLE_EVT_MAX_PARAM_LEN - 4) i.e. ATT_MTU > 251 for BLE_EVT_MAX_PARAM_LEN
 * default value.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Offset Bits 14-0: offset in octets from which Attribute_Value data
 *        starts. Bit 15 is used as flag: when set to 1 it indicates that more
 *        data are to come (fragmented event in case of long attribute data).
 * @param Event_Data_Length Length of following data
 * @param Attribute_Value The value of the attribute(s).
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_read_ext_event( uint16_t Connection_Handle,
                                    uint16_t Offset,
                                    uint16_t Event_Data_Length,
                                    const uint8_t* Attribute_Value );

/**
 * @brief ACI_GATT_INDICATION_EXT_EVENT
 * When it is enabled with ACI_GATT_SET_EVENT_MASK and when an indication is
 * received from the server, this event is generated instead of
 * ACI_GATT_INDICATION_EVENT.
 * This event should be used instead of ACI_GATT_INDICATION_EVENT when ATT_MTU
 * > (BLE_EVT_MAX_PARAM_LEN - 4) i.e. ATT_MTU > 251 for BLE_EVT_MAX_PARAM_LEN
 * default value.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Handle The handle of the attribute
 * @param Offset Bits 14-0: offset in octets from which Attribute_Value data
 *        starts. Bit 15 is used as flag: when set to 1 it indicates that more
 *        data are to come (fragmented event in case of long attribute data).
 * @param Attribute_Value_Length Length of Attribute_Value in octets
 * @param Attribute_Value The current value of the attribute
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_indication_ext_event( uint16_t Connection_Handle,
                                          uint16_t Attribute_Handle,
                                          uint16_t Offset,
                                          uint16_t Attribute_Value_Length,
                                          const uint8_t* Attribute_Value );

/**
 * @brief ACI_GATT_NOTIFICATION_EXT_EVENT
 * When it is enabled with ACI_GATT_SET_EVENT_MASK and when a notification is
 * received from the server, this event is generated instead of
 * ACI_GATT_NOTIFICATION_EVENT.
 * This event should be used instead of ACI_GATT_NOTIFICATION_EVENT when
 * ATT_MTU > (BLE_EVT_MAX_PARAM_LEN - 4) i.e. ATT_MTU > 251 for
 * BLE_EVT_MAX_PARAM_LEN default value.
 * 
 * @param Connection_Handle Specifies the ATT bearer for which the event
 *        applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF: Unenhanced ATT bearer (the parameter is the
 *          connection handle)
 *        - 0xEA00 ... 0xEA3F: Enhanced ATT bearer (the LSB-byte of the
 *          parameter is the connection-oriented channel index)
 * @param Attribute_Handle The handle of the attribute
 * @param Offset Bits 14-0: offset in octets from which Attribute_Value data
 *        starts. Bit 15 is used as flag: when set to 1 it indicates that more
 *        data are to come (fragmented event in case of long attribute data).
 * @param Attribute_Value_Length Length of Attribute_Value in octets
 * @param Attribute_Value The current value of the attribute
 * @return Value indicating success or error code.
 */
tBleStatus aci_gatt_notification_ext_event( uint16_t Connection_Handle,
                                            uint16_t Attribute_Handle,
                                            uint16_t Offset,
                                            uint16_t Attribute_Value_Length,
                                            const uint8_t* Attribute_Value );

/* ACI L2CAP events */

/**
 * @brief ACI_L2CAP_CONNECTION_UPDATE_RESP_EVENT
 * This event is generated when the Central responds to the connection update
 * request packet with a connection update response packet.
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Result Result field from the response packet.
 *        Values:
 *        - 0x0000: Connection Parameters accepted
 *        - 0x0001: Connection Parameters rejected
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_connection_update_resp_event( uint16_t Connection_Handle,
                                                   uint16_t Result );

/**
 * @brief ACI_L2CAP_PROC_TIMEOUT_EVENT
 * This event is generated when the Central does not respond to the connection
 * update request packet with a connection update response packet or a command
 * reject packet within 30 seconds.
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Data_Length Length of following data
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_proc_timeout_event( uint16_t Connection_Handle,
                                         uint8_t Data_Length,
                                         const uint8_t* Data );

/**
 * @brief ACI_L2CAP_CONNECTION_UPDATE_REQ_EVENT
 * The event is given by the L2CAP layer when a connection update request is
 * received from the Peripheral. The upper layer which receives this event has
 * to respond by sending a ACI_L2CAP_CONNECTION_PARAMETER_UPDATE_RESP command.
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Identifier This is the identifier which associate the request to the
 *        response.
 * @param L2CAP_Length Length of the L2CAP connection update request.
 * @param Interval_Min Minimum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Interval_Max Maximum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Latency Maximum Peripheral latency for the connection in number of
 *        connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Timeout_Multiplier Defines connection timeout parameter in the
 *        following manner: Timeout Multiplier * 10ms.
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_connection_update_req_event( uint16_t Connection_Handle,
                                                  uint8_t Identifier,
                                                  uint16_t L2CAP_Length,
                                                  uint16_t Interval_Min,
                                                  uint16_t Interval_Max,
                                                  uint16_t Latency,
                                                  uint16_t Timeout_Multiplier );

/**
 * @brief ACI_L2CAP_COMMAND_REJECT_EVENT
 * This event is generated upon receipt of a valid Command Reject packet (e.g.
 * when the Central responds to the Connection Update Request packet with a
 * Command Reject packet).
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Identifier This is the identifier which associate the request to the
 *        response.
 * @param Reason Reason
 * @param Data_Length Length of following data
 * @param Data Data field associated with Reason
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_command_reject_event( uint16_t Connection_Handle,
                                           uint8_t Identifier,
                                           uint16_t Reason,
                                           uint8_t Data_Length,
                                           const uint8_t* Data );

/**
 * @brief ACI_L2CAP_COC_CONNECT_EVENT
 * This event is generated when receiving a valid Credit Based Connection
 * Request packet.
 * See Bluetooth spec. v.5.4 [Vol 3, Part A].
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param SPSM Simplified Protocol/Service Multiplexer.
 *        Values:
 *        - 0x0001 ... 0x00FF
 * @param MTU Maximum Transmission Unit.
 *        Values:
 *        - 23 ... 65535
 * @param MPS Maximum payload size (in octets).
 *        Values:
 *        - 23 ... 65533
 * @param Initial_Credits Number of K-frames that can be received on the
 *        created channel(s) by the L2CAP layer entity sending this packet.
 *        Values:
 *        - 0 ... 65535
 * @param Channel_Number Number of channels to be created. If this parameter is
 *        set to 0, it requests the creation of one LE credit based connection-
 *        oriented channel. Otherwise, it requests the creation of one or more
 *        enhanced credit based connection-oriented channels.
 *        Values:
 *        - 0 ... 5
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_connect_event( uint16_t Connection_Handle,
                                        uint16_t SPSM,
                                        uint16_t MTU,
                                        uint16_t MPS,
                                        uint16_t Initial_Credits,
                                        uint8_t Channel_Number );

/**
 * @brief ACI_L2CAP_COC_CONNECT_CONFIRM_EVENT
 * This event is generated when receiving a valid Credit Based Connection
 * Response packet.
 * See Bluetooth spec. v.5.4 [Vol 3, Part A].
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param MTU Maximum Transmission Unit.
 *        Values:
 *        - 23 ... 65535
 * @param MPS Maximum payload size (in octets).
 *        Values:
 *        - 23 ... 65533
 * @param Initial_Credits Number of K-frames that can be received on the
 *        created channel(s) by the L2CAP layer entity sending this packet.
 *        Values:
 *        - 0 ... 65535
 * @param Result This parameter indicates the outcome of the request. A value
 *        of 0x0000 indicates success while a non-zero value indicates the
 *        request is refused.
 *        Values:
 *        - 0x0000 ... 0x000C
 * @param Channel_Number Number of created channels. It is the length of
 *        Channel_Index_List.
 *        Values:
 *        - 0 ... 5
 * @param Channel_Index_List List of channel indexes for which the primitive
 *        applies.
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_connect_confirm_event( uint16_t Connection_Handle,
                                                uint16_t MTU,
                                                uint16_t MPS,
                                                uint16_t Initial_Credits,
                                                uint16_t Result,
                                                uint8_t Channel_Number,
                                                const uint8_t* Channel_Index_List );

/**
 * @brief ACI_L2CAP_COC_RECONF_EVENT
 * This event is generated when receiving a valid Credit Based Reconfigure
 * Request packet.
 * See Bluetooth spec. v.5.4 [Vol 3, Part A].
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param MTU Maximum Transmission Unit.
 *        Values:
 *        - 23 ... 65535
 * @param MPS Maximum payload size (in octets).
 *        Values:
 *        - 23 ... 65533
 * @param Channel_Number Number of created channels. It is the length of
 *        Channel_Index_List.
 *        Values:
 *        - 1 ... 5
 * @param Channel_Index_List List of channel indexes for which the primitive
 *        applies.
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_reconf_event( uint16_t Connection_Handle,
                                       uint16_t MTU,
                                       uint16_t MPS,
                                       uint8_t Channel_Number,
                                       const uint8_t* Channel_Index_List );

/**
 * @brief ACI_L2CAP_COC_RECONF_CONFIRM_EVENT
 * This event is generated when receiving a valid Credit Based Reconfigure
 * Response packet.
 * See Bluetooth spec. v.5.4 [Vol 3, Part A].
 * 
 * @param Connection_Handle Handle of the connection where this event occurred.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Result This parameter indicates the outcome of the request. A value
 *        of 0x0000 indicates success while a non-zero value indicates the
 *        request is refused.
 *        Values:
 *        - 0x0000 ... 0x000C
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_reconf_confirm_event( uint16_t Connection_Handle,
                                               uint16_t Result );

/**
 * @brief ACI_L2CAP_COC_DISCONNECT_EVENT
 * This event is generated when a connection-oriented channel is disconnected
 * following an L2CAP channel termination procedure.
 * See Bluetooth spec. v.5.4 [Vol 3, Part A].
 * 
 * @param Channel_Index Index of the connection-oriented channel for which the
 *        primitive applies.
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_disconnect_event( uint8_t Channel_Index );

/**
 * @brief ACI_L2CAP_COC_FLOW_CONTROL_EVENT
 * This event is generated when receiving a valid Flow Control Credit signaling
 * packet.
 * See Bluetooth spec. v.5.4 [Vol 3, Part A].
 * 
 * @param Channel_Index Index of the connection-oriented channel for which the
 *        primitive applies.
 * @param Credits Number of credits the receiving device can increment,
 *        corresponding to the number of K-frames that can be sent to the peer
 *        device sending the Flow Control Credit packet.
 *        Values:
 *        - 1 ... 65535
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_flow_control_event( uint8_t Channel_Index,
                                             uint16_t Credits );

/**
 * @brief ACI_L2CAP_COC_RX_DATA_EVENT
 * This event is generated when receiving a valid K-frame packet on a
 * connection-oriented channel.
 * See Bluetooth spec. v.5.4 [Vol 3, Part A].
 * Note: for the first K-frame of the SDU, the Information data contains the
 * L2CAP SDU Length coded on two octets followed by the K-frame information
 * payload. For the next K-frames of the SDU, the Information data only
 * contains the K-frame information payload.
 * 
 * @param Channel_Index Index of the connection-oriented channel for which the
 *        primitive applies.
 * @param Length Length of Data (in octets)
 * @param Data Information data
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_rx_data_event( uint8_t Channel_Index,
                                        uint16_t Length,
                                        const uint8_t* Data );

/**
 * @brief ACI_L2CAP_COC_TX_POOL_AVAILABLE_EVENT
 * Each time ACI_L2CAP_COC_TX_DATA raises the error code
 * BLE_STATUS_INSUFFICIENT_RESOURCES (0x64), the
 * ACI_L2CAP_COC_TX_POOL_AVAILABLE_EVENT event is generated as soon as there is
 * a free buffer available for sending K-frames.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_l2cap_coc_tx_pool_available_event( void );

/* ACI HAL events */

/**
 * @brief ACI_HAL_END_OF_RADIO_ACTIVITY_EVENT
 * This event is generated when the device completes a radio activity and
 * provide information when a new radio activity will be performed.
 * Information provided includes type of radio activity and absolute time in
 * system ticks when a new radio activity is schedule, if any. Application can
 * use this information to schedule user activities synchronous to selected
 * radio activities. A command ACI_HAL_SET_RADIO_ACTIVITY_MASK is provided to
 * enable radio activity events of user interests, by default no events are
 * enabled.
 * User should take into account that enabling radio events in application with
 * intense radio activity could lead to a fairly high rate of events generated.
 * Application use cases includes synchronizing notification with connection
 * interval, switching antenna at the end of advertising or performing flash
 * erase operation while radio is idle.
 * 
 * @param Last_State Completed radio event
 *        Values:
 *        - 0x00: Idle
 *        - 0x01: Advertising
 *        - 0x02: Peripheral connection
 *        - 0x03: Scanning
 *        - 0x05: Central connection
 *        - 0x06: TX test mode
 *        - 0x07: RX test mode
 *        - 0x09: Periodic advertising (only for STM32WBA)
 *        - 0x0A: Periodic sync (only for STM32WBA)
 *        - 0x0B: Iso broadcast (only for STM32WBA)
 *        - 0x0C: Iso sync (only for STM32WBA)
 *        - 0x0D: Iso peripheral connection (only for STM32WBA)
 *        - 0x0E: Iso central connection (only for STM32WBA)
 * @param Next_State Incoming radio event
 *        Values:
 *        - 0x00: Idle
 *        - 0x01: Advertising
 *        - 0x02: Peripheral connection
 *        - 0x03: Scanning
 *        - 0x05: Central connection
 *        - 0x06: TX test mode
 *        - 0x07: RX test mode
 *        - 0x09: Periodic advertising (only for STM32WBA)
 *        - 0x0A: Periodic sync (only for STM32WBA)
 *        - 0x0B: Iso broadcast (only for STM32WBA)
 *        - 0x0C: Iso sync (only for STM32WBA)
 *        - 0x0D: Iso peripheral connection (only for STM32WBA)
 *        - 0x0E: Iso central connection (only for STM32WBA)
 * @param Next_State_SysTime 32bit absolute current time expressed in internal
 *        time units.
 * @param Last_State_Slot Slot number of completed radio event
 *        Values:
 *        - 0xFF: Idle
 *        - 0x00 ... 0x07
 * @param Next_State_Slot Slot number of incoming radio event
 *        Values:
 *        - 0xFF: Idle
 *        - 0x00 ... 0x07
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_end_of_radio_activity_event( uint8_t Last_State,
                                                uint8_t Next_State,
                                                uint32_t Next_State_SysTime,
                                                uint8_t Last_State_Slot,
                                                uint8_t Next_State_Slot );

/**
 * @brief ACI_HAL_SCAN_REQ_REPORT_EVENT
 * This event is reported to the application after a scan request is received
 * and a scan response is scheduled to be transmitted.
 * Note: RSSI in this event is valid only when privacy is not used.
 * 
 * @param RSSI RSSI (signed integer).
 *        Units: dBm.
 *        Values:
 *        - 127: RSSI not available
 *        - -127 ... 20
 * @param Peer_Address_Type Address type
 *        0x00 Public Device Address
 *        0x01 Random Device Address
 *        0x02 Public Identity Address (Corresponds to Resolved Private
 *        Address)
 *        0x03 Random (Static) Identity Address (Corresponds to Resolved
 *        Private Address)
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 *        - 0x02: Public Identity Address
 *        - 0x03: Random (Static) Identity Address
 * @param Peer_Address Public Device Address or Random Device Address of the
 *        peer device
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_scan_req_report_event( uint8_t RSSI,
                                          uint8_t Peer_Address_Type,
                                          const uint8_t* Peer_Address );

/**
 * @brief ACI_HAL_FW_ERROR_EVENT
 * This event is generated to report firmware error information.
 * 
 * @param FW_Error_Type FW Error type
 *        Values:
 *        - 0x01: L2CAP recombination failure
 *        - 0x02: GATT unexpected peer message
 *        - 0x03: NVM level warning
 *        - 0x04: COC RX data length too large
 * @param Data_Length Length of Data in octets
 * @param Data The error event info
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_fw_error_event( uint8_t FW_Error_Type,
                                   uint8_t Data_Length,
                                   const uint8_t* Data );

/**
 * @brief ACI_HAL_SYNC_EVENT
 * This event is generated after the execution of CIG or BIG is completed on
 * hardware.
 * 
 * @param Group_Id Identifier of the CIG or BIG.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Next_Anchor_Point Timestamp of the next ISO anchor point
 * @param Time_Stamp Timestamp of the calibration signal
 * @param Next_Sdu_Delivery_Timeout Timestamp before which the SDU must be
 *        provided to the LL for being handled in the next ISO event
 * @return Value indicating success or error code.
 */
tBleStatus aci_hal_sync_event( uint8_t Group_Id,
                               uint32_t Next_Anchor_Point,
                               uint32_t Time_Stamp,
                               uint32_t Next_Sdu_Delivery_Timeout );

/* HCI ACL Data events */

/**
 * @brief HCI_RX_ACL_DATA_EVENT
 * This event is generated in "LL only" mode when an ACL data packet is
 * received.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus hci_rx_acl_data_event( uint16_t Connection_Handle,
                                  uint8_t  PB_Flag,
                                  uint16_t  Data_Length,
                                  uint8_t*  Data );


#endif /* BLE_EVENTS_H__ */
