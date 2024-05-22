/**
  ******************************************************************************
  * @file    st_mac_802_15_4_dbg_svc.h
  * @author  MCD Application Team
  * @brief   Contains STM32WB specificities requested to debug the 802.15.4
  *          MAC interface.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


#ifndef _ST_MAC_802_15_4_DBG_H_
#define _ST_MAC_802_15_4_DBG_H_

/* DBG Gpio Plan - MB1801A*/

/* DBG_GPIO2 : GPIOA, GPIO_PIN_9  : GPIO39 -> CN4.19 - D12 */
/* DBG_GPIO1 : GPIOA, GPIO_PIN_10 : GPIO55 -> CN4.37 - D11 */
/* DBG_GPIO3 : GPIOB, GPIO_PIN_5  : GPIO54 -> CN4.35 - D10 */
/* DBG_GPIO4 : GPIOB, GPIO_PIN_7  : GPIO52 -> CN4.33 - D09 */
/* DBG_GPIO5 : GPIOB, GPIO_PIN_13 : GPIO49 -> CN4.29 - D08 */

#define TRACE_DBG_INACTIVE
   
#define TRACE_DBG_GPIO1_SET             ST_setDbgGpio_1()
#define TRACE_DBG_GPIO1_RESET           ST_resetDbgGpio_1()

#define TRACE_DBG_GPIO2_SET             ST_setDbgGpio_2()
#define TRACE_DBG_GPIO2_RESET           ST_resetDbgGpio_2()

#define TRACE_DBG_GPIO3_SET             ST_setDbgGpio_3()
#define TRACE_DBG_GPIO3_RESET           ST_resetDbgGpio_3() 

#define TRACE_DBG_GPIO4_SET             ST_setDbgGpio_4()
#define TRACE_DBG_GPIO4_RESET           ST_resetDbgGpio_4()

#define TRACE_DBG_GPIO5_SET             ST_setDbgGpio_5()
#define TRACE_DBG_GPIO5_RESET           ST_resetDbgGpio_5()

   
#define TRACE_RADIO_IRQ_SET     TRACE_DBG_GPIO2_SET
#define TRACE_RADIO_IRQ_RESET   TRACE_DBG_GPIO2_RESET

#define TRACE_FRAME_TX_SET     TRACE_DBG_INACTIVE
#define TRACE_FRAME_TX_RESET   TRACE_DBG_INACTIVE

#define TRACE_RADIO_PRDC_CALIB_SET     TRACE_DBG_INACTIVE
#define TRACE_RADIO_PRDC_CALIB_RESET   TRACE_DBG_INACTIVE

#define TRACE_TX_START_SET     TRACE_DBG_GPIO4_SET
#define TRACE_TX_START_RESET   TRACE_DBG_GPIO4_RESET

#define TRACE_CSMA_SET     TRACE_DBG_GPIO5_SET
#define TRACE_CSMA_RESET   TRACE_DBG_GPIO5_RESET


#define TRACE_MAC_PERSIST_TIMER_SET     TRACE_DBG_GPIO1_SET
#define TRACE_MAC_PERSIST_TIMER_RESET   TRACE_DBG_GPIO1_RESET

   
void ST_setDbgGpio_1();
void ST_resetDbgGpio_1();
void ST_setDbgGpio_2();
void ST_resetDbgGpio_2();
void ST_setDbgGpio_3();
void ST_resetDbgGpio_3();
void ST_setDbgGpio_4();
void ST_resetDbgGpio_4();
void ST_setDbgGpio_5();
void ST_resetDbgGpio_5();

#endif /* _ST_MAC_802_15_4_DBG_H_ */
