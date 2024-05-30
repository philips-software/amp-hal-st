/**
 ******************************************************************************
 * @file    svc_ctl.c
 * @author  MCD Application Team
 * @brief   BLE Controller
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "common_blesvc.h"
#include "cmsis_compiler.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
#if (BLE_CFG_SVC_MAX_NBR_CB > 0)
SVC_CTL_p_EvtHandler_t SVCCTL__SvcHandlerTab[BLE_CFG_SVC_MAX_NBR_CB];
#endif
uint8_t NbreOfRegisteredHandler;
} SVCCTL_EvtHandler_t;

typedef struct
{
#if (BLE_CFG_CLT_MAX_NBR_CB > 0)
SVC_CTL_p_EvtHandler_t SVCCTL_CltHandlerTable[BLE_CFG_CLT_MAX_NBR_CB];
#endif
uint8_t NbreOfRegisteredHandler;
} SVCCTL_CltHandler_t;

typedef struct
{
#if (BLE_CFG_MAX_NBR_CB > 0)
SVC_CTL_p_EvtHandler_t SVCCTL_HandlerTable[BLE_CFG_MAX_NBR_CB];
#endif
uint8_t NbreOfRegisteredHandler;
} SVCCTL_Handler_t;

/* Private defines -----------------------------------------------------------*/
#define SVCCTL_EGID_EVT_MASK   0xFF00
#define SVCCTL_GATT_EVT_TYPE   0x0C00

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SVCCTL_EvtHandler_t SVCCTL_EvtHandler;
SVCCTL_CltHandler_t SVCCTL_CltHandler;
SVCCTL_Handler_t SVCCTL_Handler;

/* Private functions ----------------------------------------------------------*/
/* Weak functions -------------------------------------------------------------*/

/* Functions Definition ------------------------------------------------------*/

void SVCCTL_Init( void )
{

  /**
   * Initialize the number of registered Handler
   */
  SVCCTL_EvtHandler.NbreOfRegisteredHandler = 0;
  SVCCTL_CltHandler.NbreOfRegisteredHandler = 0;
  SVCCTL_Handler.NbreOfRegisteredHandler = 0;

  return;
}

/**
 * @brief  BLE Controller initialization
 * @param  None
 * @retval None
 */
void SVCCTL_RegisterSvcHandler( SVC_CTL_p_EvtHandler_t pfBLE_SVC_Service_Event_Handler )
{
#if (BLE_CFG_SVC_MAX_NBR_CB > 0)
  SVCCTL_EvtHandler.SVCCTL__SvcHandlerTab[SVCCTL_EvtHandler.NbreOfRegisteredHandler] = pfBLE_SVC_Service_Event_Handler;
  SVCCTL_EvtHandler.NbreOfRegisteredHandler++;
#else
  (void)(pfBLE_SVC_Service_Event_Handler);
#endif

  return;
}

/**
 * @brief  BLE Controller initialization
 * @param  None
 * @retval None
 */
void SVCCTL_RegisterCltHandler( SVC_CTL_p_EvtHandler_t pfBLE_SVC_Client_Event_Handler )
{
#if (BLE_CFG_CLT_MAX_NBR_CB > 0)
  SVCCTL_CltHandler.SVCCTL_CltHandlerTable[SVCCTL_CltHandler.NbreOfRegisteredHandler] = pfBLE_SVC_Client_Event_Handler;
  SVCCTL_CltHandler.NbreOfRegisteredHandler++;
#else
  (void)(pfBLE_SVC_Client_Event_Handler);
#endif

  return;
}

/**
 * @brief  BLE Controller initialization
 * @param  None
 * @retval None
 */
void SVCCTL_RegisterHandler( SVC_CTL_p_EvtHandler_t pfBLE_SVC_Event_Handler )
{
#if (BLE_CFG_MAX_NBR_CB > 0)
  SVCCTL_Handler.SVCCTL_HandlerTable[SVCCTL_Handler.NbreOfRegisteredHandler] = pfBLE_SVC_Event_Handler;
  SVCCTL_Handler.NbreOfRegisteredHandler++;
#else
  (void)(pfBLE_SVC_Event_Handler);
#endif

  return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_UserEvtRx( void *pckt )
{
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  SVCCTL_EvtAckStatus_t event_notification_status;
  SVCCTL_UserEvtFlowStatus_t return_status;
#if (BLE_CFG_SVC_MAX_NBR_CB > 0) || (BLE_CFG_CLT_MAX_NBR_CB > 0)
  uint8_t index;
#endif

  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;
  event_notification_status = SVCCTL_EvtNotAck;

  switch (event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
    {
      blecore_evt = (evt_blecore_aci*) event_pckt->data;

      switch ((blecore_evt->ecode) & SVCCTL_EGID_EVT_MASK)
      {
        case SVCCTL_GATT_EVT_TYPE:
#if (BLE_CFG_SVC_MAX_NBR_CB > 0)
          /* For Service event handler */
          for (index = 0; index < SVCCTL_EvtHandler.NbreOfRegisteredHandler; index++)
          {
            event_notification_status = SVCCTL_EvtHandler.SVCCTL__SvcHandlerTab[index](pckt);
            /**
             * When a GATT event has been acknowledged by a Service, there is no need to call the other registered handlers
             * a GATT event is relevant for only one Service
             */
            if (event_notification_status != SVCCTL_EvtNotAck)
            {
              /**
               *  The event has been managed. The Event processing should be stopped
               */
              break;
            }
          }
#endif
#if (BLE_CFG_CLT_MAX_NBR_CB > 0)
          /* For Client event handler */
          event_notification_status = SVCCTL_EvtNotAck;
          for(index = 0; index <SVCCTL_CltHandler.NbreOfRegisteredHandler; index++)
          {
            event_notification_status = SVCCTL_CltHandler.SVCCTL_CltHandlerTable[index](pckt);
            /**
             * When a GATT event has been acknowledged by a Client, there is no need to call the other registered handlers
             * a GATT event is relevant for only one Client
             */
            if (event_notification_status != SVCCTL_EvtNotAck)
            {
              /**
               *  The event has been managed. The Event processing should be stopped
               */
              break;
            }
          }
#endif
          break;

        default:
#if (BLE_CFG_MAX_NBR_CB > 0)
          /* For event handler */
          for(index = 0; index <SVCCTL_Handler.NbreOfRegisteredHandler; index++)
          {
            event_notification_status = SVCCTL_Handler.SVCCTL_HandlerTable[index](pckt);
            /**
             * When a GATT event has been acknowledged by a Client, there is no need to call the other registered handlers
             * a GATT event is relevant for only one Client
             */
            if (event_notification_status != SVCCTL_EvtNotAck)
            {
              /**
               *  The event has been managed. The Event processing should be stopped
               */
              break;
            }
          }
#endif
          break;
      }
    }
      break; /* HCI_HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE_SPECIFIC */

    default:
#if (BLE_CFG_MAX_NBR_CB > 0)
      /* For event handler */
      for(index = 0; index <SVCCTL_Handler.NbreOfRegisteredHandler; index++)
      {
        event_notification_status = SVCCTL_Handler.SVCCTL_HandlerTable[index](pckt);
      }
#endif
      break;
  }

  /**
   * When no registered handlers (either Service or Client) has acknowledged the GATT event, it is reported to the application
   * a GAP event is always reported to the application.
   */
  switch (event_notification_status)
  {
    case SVCCTL_EvtNotAck:
      /**
       *  The event has NOT been managed.
       *  It shall be passed to the application for processing
       */
      return_status = SVCCTL_App_Notification(pckt);
      break;

    case SVCCTL_EvtAckFlowEnable:
      return_status = SVCCTL_UserEvtFlowEnable;
      break;

    case SVCCTL_EvtAckFlowDisable:
      return_status = SVCCTL_UserEvtFlowDisable;
      break;

    default:
      return_status = SVCCTL_UserEvtFlowEnable;
      break;
  }

  return (return_status);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
