/*$Id: //dwh/bluetooth/DWC_ble154combo/firmware/rel/1.30a-SOW04PatchV2/firmware/public_inc/ll_error.h#1 $*/
/**
 ********************************************************************************
 * @file    error.h
 * @brief   This file contains error defines across BLE FW LL.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef ERROR_H_
#define ERROR_H_

/* Includes ---------------------------------------------------------------------*/

/* Exported  Defines -----------------------------------------------------------*/
#ifndef SUCCESS
#define		SUCCESS																			0x00
#endif
#define		UNKNOWN_HCI_COMMAND                                                         	0x01
#define		UNKNOWN_CONNECTION_IDENTIF                                                  	0x02
#define		HARDWARE_FAILURE                                                            	0x03
#define		PAGE_TIMEOUT                                                                	0x04
#define		AUTHENTICATION_FAILURE                                                      	0x05
#define		PIN_OR_KEY_MISSING                                                          	0x06
#define		MEMORY_CAPACITY_EXCEEDED                                                    	0x07
#define		CONNECTION_TIMEOUT                                                           	0x08
#define		CONNECTION_LIMIT_EXCEEDED                                                   	0x09
#define		SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED                           	0x0A
#define		ACL_CONNECTION_ALREADY_EXISTS                                               	0x0B
#define		COMMAND_DISALLOWED                                                          	0x0C
#define		CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES                                	0x0D
#define		CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS                                 	0x0E
#define		CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR                             	0x0F
#define		CONNECTION_ACCEPT_TIMEOUT_EXCEEDED                                          	0x10
#define		UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE                                      	0x11
#define		INVALID_HCI_COMMAND_PARAMETERS                                              	0x12
#define		REMOTE_USER_TERMINATED_CONNECTION                                           	0x13
#define		REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES                    	0x14
#define		REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF                        	0x15
#define		CONNECTION_TERMINATED_BY_LOCAL_HOST                                         	0x16
#define		REPEATED_ATTEMPTS                                                           	0x17
#define		PAIRING_NOT_ALLOWED                                                         	0x18
#define		UNKNOWN_LMP_PDU                                                             	0x19
#define		UNSUPPORTED_REMOTE_FEATURE                                                  	0x1A
#define		SCO_OFFSET_REJECTED                                                         	0x1B
#define		SCO_INTERVAL_REJECTED                                                       	0x1C
#define		SCO_AIR_MODE_REJECTED                                                       	0x1D
#define		INVALID_LMP_PARAMETERS_INVALID_LL_PARAMETERS                                  	0x1E
#define		UNSPECIFIED_ERROR                                                           	0x1F
#define		UNSUPPORTED_LMP_PARAMETER_VALUE_UNSUPPORTED_LL_PARAMETER_VALUE                	0x20
#define		ROLE_CHANGE_NOT_ALLOWED                                                     	0x21
#define		LMP_RESPONSE_TIMEOUT_LL_RESPONSE_TIMEOUT                                     	0x22
#define		LMP_ERROR_TRANSACTION_COLLISION                                             	0x23
#define		LMP_PDU_NOT_ALLOWED                                                         	0x24
#define		ENCRYPTION_MODE_NOT_ACCEPTABLE                                              	0x25
#define		LINK_KEY_CANNOT_BE_CHANGED                                                  	0x26
#define		REQUESTED_QOS_NOT_SUPPORTED                                                 	0x27
#define		INSTANT_PASSED                                                              	0x28
#define		PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED                                         	0x29
#define		DIFFERENT_TRANSACTION_COLLISION                                             	0x2A
#define		RESERVED_FOR_FUTURE_1                                                           0x2B
#define		QOS_UNACCEPTABLE_PARAMETER                                                  	0x2C
#define		QOS_REJECTED                                                                	0x2D
#define		CHANNEL_ASSESSMENT_NOT_SUPPORTED                                        	0x2E
#define		INSUFFICIENT_SECURITY                                                       	0x2F
#define		PARAMETER_OUT_OF_MANDATORY_RANGE                                            	0x30
#define		RESERVED_FOR_FUTURE_2                                                           0x31
#define		ROLE_SWITCH_PENDING                                                         	0x32
#define		RESERVED_FOR_FUTURE_3                                                           0x33
#define		RESERVED_SLOT_VIOLATION                                                     	0x34
#define		ROLE_SWITCH_FAILED                                                          	0x35
#define		EXTENDED_INQUIRY_RESPONSE_TOO_LARGE                                         	0x36
#define		SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST                                 	0x37
#define		HOST_BUSY_PAIRING                                                           	0x38
#define		CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND                        	0x39
#define		CONTROLLER_BUSY                                                             	0x3A
#define		UNACCEPTABLE_CONNECTION_PARAMETERS                                          	0x3B
#define		DIRECTED_ADVERTISING_TIMEOUT                                                	0x3C
#define		CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE                                    	0x3D
#define		CONNECTION_FAILED_TO_BE_ESTABLISHED                                         	0x3E
#define     SYNCHRONIZATION_TIMEOUT                                                         0X3E
#define		MAC_CONNECTION_FAILED                                                       	0x3F
#define		COARSE_CLOCK_ADJUSTMENT_REJECTED_BUT_WILL_TRY_TO_ADJUST_USING_CLOCK_DRAGGING	0x40

#define		UNKNOWN_ADVERTISING_IDENTIFIER													0x42
#define		LIMIT_REACHED																	0x43
#define     OPERATION_CANCELLED_BY_HOST														0x44
#define		PACKET_TOO_LONG																	0x45


#if ((MAX_NUM_CNCRT_STAT_MCHNS > 1) && !((SUPPORT_MASTER_CONNECTION) || (SUPPORT_SLAVE_CONNECTION)||(SUPPORT_LE_EXTENDED_ADVERTISING)))
#error define at least either master or slave role
#endif

#if (SUPPORT_MASTER_CONNECTION || SUPPORT_SLAVE_CONNECTION || SUPPORT_LE_EXTENDED_ADVERTISING) && (MAX_NUM_CNCRT_STAT_MCHNS == 0)
#error define at least one state machine in order to support slave or master role or extended advertising
#endif /* SUPPORT_MASTER_CONNECTION || SUPPORT_SLAVE_CONNECTION */

#if ((SUPPORT_LE_ENCRYPTION) && (!SUPPORT_MASTER_CONNECTION && !SUPPORT_SLAVE_CONNECTION))
#error Encryption feature shall be enabled only when the master role or slave role is enabled
#endif /* #error */

#if ((SUPPORT_LE_PERIODIC_ADVERTISING) && (!SUPPORT_LE_EXTENDED_ADVERTISING))
#error Since controller supports periodic advertising feature, then it shall support Extended advertising feature
#endif /* #error */

#if (SUPPORT_PERIODIC_SYNC_TRANSFER && (!SUPPORT_MASTER_CONNECTION && !SUPPORT_SLAVE_CONNECTION))
#error Sending PAST IND shall be enabled only when the master role or slave role is enabled
#endif /* #error */

#if (SUPPORT_PERIODIC_SYNC_TRANSFER && (!SUPPORT_LE_PERIODIC_ADVERTISING))
#error Sending PAST IND shall be enabled only when the Periodic ADV is enabled
#endif /* #error */

#if ((SUPPORT_LE_POWER_CONTROL) && (!SUPPORT_MASTER_CONNECTION && !SUPPORT_SLAVE_CONNECTION))
#error LE Power Control feature shall be enabled only when the master role or slave role is enabled
#endif /* #error */

#if ((SUPPORT_BRD_ISOCHRONOUS || SUPPORT_SYNC_ISOCHRONOUS) && (!SUPPORT_LE_PERIODIC_ADVERTISING))
#error Broadcast Isochronous feature shall be enabled only when periodic advertisng is enabled
#endif /* #error */

#if (SUPPORT_PERIODIC_ADV_ADI && (!SUPPORT_LE_PERIODIC_ADVERTISING))
#error Periodic Advertising ADI feature shall be enabled only when periodic advertisng is enabled
#endif /* #error */

#if ((SUPPORT_CHANNEL_CLASSIFICATION) && (!SUPPORT_MASTER_CONNECTION && !SUPPORT_SLAVE_CONNECTION))
#error Channel Classification feature shall be enabled only when the master role or slave role is enabled 
#endif /* #error */


/* Exported macros ------------------------------------------------------------*/

/* Exported types -------------------------------------------------------------*/

/* Exported functions ---------------------------------------------------------*/

#endif /* ERROR_H_ */

/*****END OF FILE****/
