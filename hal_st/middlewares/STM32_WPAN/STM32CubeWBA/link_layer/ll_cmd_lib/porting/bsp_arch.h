/*$Id: //dwh/bluetooth/DWC_ble154combo/rtl/rel/1.21a-sow07/RCE/DWC_ble154combo/firmware/shrd_utils/src/porting/emsk/bsp_arch.h#1 $*/
/**
 ********************************************************************************
 * @file    bsp_arch.h
 * @brief  basic definition for porting a bsp
 ******************************************************************************
 * @copy
 *
 *COPYRIGHT "2021" Synopsys, Inc. This Synopsys "product" and all associated documentation 
 *are proprietary to Synopsys, Inc. and may only be used pursuant to the terms and 
 *conditions of a written license agreement with Synopsys, Inc. All other use, 
 *reproduction, modification, or distribution of the Synopsys "product" or the associated 
 *documentation is strictly prohibited.
 *
 *
 * THE ENTIRE NOTICE ABOVE MUST BE REPRODUCED ON ALL AUTHORIZED COPIES.
 *
 * <h2><center>&copy; (C) COPYRIGHT "2021" Synopsys, INC.</center></h2>
 * <h2><center>&copy; 	ALL RIGHTS RESERVED</center></h2>
 *
 * \n\n<b>References</b>\n
 * -Documents folder .
 *
 * <b>Edit History For File</b>\n
 *  This section contains comments describing changes made to this file.\n
 *  Notice that changes are listed in reverse chronological order.\n
 * <table border>
 * <tr>
 *   <td><b> PLEASE UPDATE with when date </b></td>
 *   <td><b> PLEASE UPDATE with who made the changes </b></td>
 *   <td><b> PLEASE UPDATE WITH what, where, why the changes were made</b></td>
 * </tr>
 * </table>\n
 */

#ifndef ARM_SRC_BSP_ARCH_H_
#define ARM_SRC_BSP_ARCH_H_

#ifdef STM32WBA52xx
#include "stm32wba52xx.h"
#else
/* Assume STM32WBA55xx */
#include "stm32wba55xx.h"
#endif

/* Place values by any value = 0 for Linux */
#define DWC_BLE_LL_0_BASEADDR 		    RADIO_BASE  /* 2.4 GHz RADIO peripheral base address */
#define DWC_BLE_LL_INTR_NUM		    (1)  /* 2.4GHz RADIO global interrupt */
/* LL Interrupt Priority */
#define DWC_BLE_LL_INTR_PRIO     (2)
#if DWC_BLE_LL_INTR_PRIO < 2
#error "BLE LL IRQ Priority must be greater than or equal 2"
#endif
/* SW Interrupt (an interrupt that is triggered by SW) */
#define DWC_BLE_LL_LOW_ISR_INTR_NUM 	(2) /* If the interrupt number == 0xFF: indicates that the interrupt is not initialized to a valid value. */
#define DWC_BLE_LL_LOW_ISR_INTR_PRIO	(10)
#if (DWC_BLE_LL_LOW_ISR_INTR_PRIO <= DWC_BLE_LL_INTR_PRIO)
#error "Low ISR priority must be greater than BLE LL IRQ Priority"
#endif
#if defined(__MW_START__)
#define SYS_INTERRUPT _Interrupt
#else
#define SYS_INTERRUPT
#endif
#define ALLOW_MCU_SLEEP                 0
#define FW_PROFILING                    0
#define BOARD_CPU_CLOCK					(32000000UL)
#endif /* ARM_SRC_BSP_ARCH_H_ */
