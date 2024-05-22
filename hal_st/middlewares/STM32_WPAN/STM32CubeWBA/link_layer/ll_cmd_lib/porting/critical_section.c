/*$Id: //dwh/bluetooth/DWC_ble154combo/rtl/rel/1.20a-sow04/RCE/DWC_ble154combo/firmware/shrd_utils/src/porting/mps2/critical_section.c#1 $*/
/**
 ********************************************************************************
 * @file    critical_section.c
 * @brief   PLEASE UPDATE WITH A BRIEF FILE DESCRIPTION
 ******************************************************************************
 * @copy
 *
 *COPYRIGHT "2020" Synopsys, Inc. This Synopsys "product" and all associated documentation 
 *are proprietary to Synopsys, Inc. and may only be used pursuant to the terms and 
 *conditions of a written license agreement with Synopsys, Inc. All other use, 
 *reproduction, modification, or distribution of the Synopsys "product" or the associated 
 *documentation is strictly prohibited.
 *
 *
 * THE ENTIRE NOTICE ABOVE MUST BE REPRODUCED ON ALL AUTHORIZED COPIES.
 *
 * <h2><center>&copy;(C) COPYRIGHT 2020 SYNOPSYS, INC.</center></h2>
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

#include "bsp.h"

extern void ll_sys_enable_irq(void);
extern void ll_sys_disable_irq(void);
extern void ll_sys_enable_specific_irq(uint8_t isr_type);
extern void ll_sys_disable_specific_irq(uint8_t isr_type);

void enable_irq(void) 
{
  ll_sys_enable_irq();
}

void disable_irq(void) 
{
  ll_sys_disable_irq();
}


/**
 * @brief this function is used to enable a specific ISR
 * @param[in]  isr_type that holds specific ISR to be enabled by this function
 * 				BIT[0] for LL_HIGH_ISR
 * 				BIT[1] for LL_LOW_ISR
 * 				BIT[2] for SYS_LOW_ISR
 * @return None
 */
void enable_specific_irq (uint8_t isr_type)
{
  ll_sys_enable_specific_irq(isr_type);
  
}


/**
 * @brief this function is used to disable a specific ISR
 * @param[in]  isr_type that holds specific ISR to be disabled by this function
 * 				BIT[0] for LL_HIGH_ISR
 * 				BIT[1] for LL_LOW_ISR
 * 				BIT[2] for SYS_LOW_ISR
 * @return None
 */
void disable_specific_irq (uint8_t isr_type)
{
  ll_sys_disable_specific_irq(isr_type);  
}

