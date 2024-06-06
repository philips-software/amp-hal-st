
/**
  ******************************************************************************
  * @file    svc_ctl.h
  * @author  MCD Application Team
  * @brief   Header for ble_controller.c module
  ******************************************************************************
  * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */


/**
 * The BLE Controller supports the application to handle services and clients.
 * It provides an API to initialize the BLE core Device and a handler mechanism to rout the GATT/GAP events to the
 * application. When the ble_controller is used (recommended), the application shall register a callback for each
 * Service and each Client implemented. This is already done with the Services and Clients provided in that delivery.
 *  + A GATT event is relevant to only one Service and/or one Client. When a GATT event is received, it is notified to
 *    the registered handlers to the BLE controller. When no registered handler acknowledges positively the GATT event,
 *    it is reported to the application.
 *  + A GAP event is not relevant to either a Service or a Client. It is sent to the application
 *  + In case the application does not want to take benefit from the ble_controller, it could bypass it. In that case,
 *  the application shall:
 *    - call  SVCCTL_Init() to initialize the BLE core device (or implement on its own what is inside that function
 *    - implement TLHCI_UserEvtRx() which is the notification from the HCI layer to report all events (GATT/GAP).
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SVCCTL_H
#define __SVCCTL_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
#define PACKED(d) __PACKED_STRUCT     
typedef PACKED(struct) _hci_uart_pckt
{
  uint8_t type;
  uint8_t data[1];
} hci_uart_pckt;

typedef PACKED(struct) _hci_event_pckt
{
  uint8_t         evt;
  uint8_t         plen;
  uint8_t         data[1];
} hci_event_pckt;

typedef PACKED(struct) _evt_le_meta_event
{
  uint8_t         subevent;
  uint8_t         data[1];
} evt_le_meta_event;

/**
 * Vendor specific event for BLE core.
 */
typedef PACKED(struct) _evt_blecore_aci
{
  uint16_t ecode; /**< One of the BLE core event codes. */
  uint8_t  data[1];
} evt_blecore_aci;


/* ------------------------------------------------------------------------- */

#define ADV_IND                                        0
#define ADV_DIRECT_IND                                 1
#define ADV_SCAN_IND                                   2
#define ADV_NONCONN_IND                                3
#define SCAN_RSP                                       4
#define HIGH_DUTY_CYCLE_DIRECTED_ADV                   1
#define LOW_DUTY_CYCLE_DIRECTED_ADV                    4


  typedef enum
  {
    SVCCTL_EvtNotAck,
    SVCCTL_EvtAckFlowEnable,
    SVCCTL_EvtAckFlowDisable,
  } SVCCTL_EvtAckStatus_t;

  typedef enum
  {
    SVCCTL_UserEvtFlowDisable,
    SVCCTL_UserEvtFlowEnable,
  } SVCCTL_UserEvtFlowStatus_t;

  typedef SVCCTL_EvtAckStatus_t (*SVC_CTL_p_EvtHandler_t)(void *p_evt);

  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/

  /* Exported functions ------------------------------------------------------- */
  /**
   * @brief  It initializes the BLE core Driver and sends some commands to initialize the BLE core device
   *         It shall be called before any BLE operation
   *
   * @param  None
   * @retval None
   */
  void SVCCTL_Init( void );

  /**
   * @brief  This API registers a handler to be called when a GATT user event is received from the BLE core device. When
   *         a Service is created, it shall register a callback to be notified when a GATT event is received from the
   *         BLE core device. When a GATT event is received, it shall be checked in the handler if the GATT events belongs
   *         to the Service or not. The handler shall return the correct status depending on the result. As soon as one
   *         Service handler registered acknowledges positively the GATT event, the ble_controller stops calling the
   *         registered Service handlers.
   *         This handler is called in the TL_BLE_HCI_UserEvtProc() context
   *
   * @param  pfBLE_SVC_Service_Event_Handler: This is the Service handler that the ble_controller calls to report a GATT
   *         event received. If the GATT event belongs to that Service, the callback shall return positively with
   *         SVCCTL_EvtAckFlowEnable.
   * @retval None
   */
  void SVCCTL_RegisterSvcHandler( SVC_CTL_p_EvtHandler_t pfBLE_SVC_Service_Event_Handler );

  /**
   * @brief  This API registers a handler to be called when a GATT user event is received from the BLE core device. When
   *         a Client is created, it shall register a callback to be notified when a GATT event is received from the
   *         BLE core device. When a GATT event is received, it shall be checked in the handler if the GATT events belongs
   *         to the Client or not. The handler shall return the correct status depending on the result. As soon as one
   *         Client handler registered acknowledges positively the GATT event, the ble_controller stops calling the
   *         registered Client handlers.
   *         This handler is called in the TL_BLE_HCI_UserEvtProc() context
   *
   * @param  pfBLE_SVC_Client_Event_Handler: This is the Client handler that the ble_controller calls to report a GATT
   *         event received. If the GATT event belongs to that Client, the callback shall return positively with
   *         SVCCTL_EvtAckFlowEnable.
   * @retval None
   */
  void SVCCTL_RegisterCltHandler( SVC_CTL_p_EvtHandler_t pfBLE_SVC_Client_Event_Handler );

  /**
   * @brief  This API registers a handler to be called when a GAP user event is received from the BLE core device. When
   *         a Profile is created, it could register a callback to be notified when a GAP event is received from the
   *         BLE core device.
   *         As soon as one handler registered acknowledges positively the event, the ble_controller stops calling the
   *         registered handlers.
   *         This handler is called in the TL_BLE_HCI_UserEvtProc() context
   *
   * @param  pfBLE_SVC_Event_Handler: This is the  handler that the ble_controller calls to report a GAP
   *         event received.
   * @retval None
   */
void SVCCTL_RegisterHandler( SVC_CTL_p_EvtHandler_t pfBLE_SVC_Event_Handler );

  /**
   * @brief  This API is used to resume the User Event Flow that has been stopped in return of SVCCTL_UserEvtRx()
   *
   * @param  None
   * @retval None
   */
  void SVCCTL_ResumeUserEventFlow( void );


  /**
   * @brief This callback is triggered when either
   *          + a GAP event is received from the BLE core device.
   *          + a GATT event that has not been positively acknowledged by the registered handler is received from the
   *            BLE core device.
   *        The event is returned in a HCI packet. The full HCI packet is stored in a single buffer and is available when
   *        this callback is triggered. However, an ACI event may be longer than a HCI packet and could be fragmented over
   *        several HCI packets. The HCI layer only handles HCI packets so when an ACI packet is split over several HCI
   *        packets, this callback is triggered for each HCI fragment. It is the responsibility of the application to
   *        reassemble the ACI event.
   *        This callback is triggered in the TL_BLE_HCI_UserEvtProc() context
   *
   * @param  pckt: The user event received from the BLE core device
   * @retval None
   */
  SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification( void *pckt );

  /**
   * @brief
   *
   *
   * @param  pckt: The user event received from the BLE core device
   * @retval SVCCTL_UserEvtFlowStatus_t: SVCCTL_UserEvtFlowEnable when the packet has been processed
   *         SVCCTL_UserEvtFlowDisable otherwise (the packet is kept in the queue)
   */
  SVCCTL_UserEvtFlowStatus_t SVCCTL_UserEvtRx( void *pckt );

#ifdef __cplusplus
}
#endif

#endif /*__SVCCTL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
