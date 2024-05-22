/**
  ******************************************************************************
  * @file    scheduling_intf.c
  * @author  MCD Application Team
  * @brief   Link Layer IP system interface synchronisation module 
  *          for sequencer use
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

#include "os_wrapper.h"
#include "bsp.h"

#define MW_UNUSED(x) ((void)(x))

extern void ll_sys_schedule_bg_process(void);
extern void ll_sys_schedule_bg_process_isr(void);
extern void LINKLAYER_PLAT_EnableIRQ(void);
extern void LINKLAYER_PLAT_DisableIRQ(void);
extern void LINKLAYER_PLAT_EnableSpecificIRQ(uint8_t isr_type);
extern void LINKLAYER_PLAT_DisableSpecificIRQ(uint8_t isr_type);

/**
 * @brief Link Layer synchronisation functions.
 */

void os_start(void){
    /* Not used with sequencer utility */
}

void os_disable_isr(void)
{
  LINKLAYER_PLAT_DisableIRQ();
}

void os_enable_isr(void)
{
  LINKLAYER_PLAT_EnableIRQ();
}

/*  ==== Mutex Functions ====  */

os_mutex_id os_rcrsv_mutex_create(void)
{
  /* Not used with sequencer utility */
  return 0;
}

int32_t os_rcrsv_mutex_delete(os_mutex_id mutex_id)
{
  /* Not used with sequencer utility */
  return 0;
}

int32_t os_rcrsv_mutex_wait(os_mutex_id mutex_id,uint32_t millisec)
{
  MW_UNUSED(mutex_id);
  MW_UNUSED(millisec);
  LINKLAYER_PLAT_DisableSpecificIRQ(LL_HIGH_ISR_ONLY | LL_LOW_ISR_ONLY);
  return 0;
}

int32_t os_rcrsv_mutex_release(os_mutex_id mutex_id)
{
  MW_UNUSED(mutex_id);
  LINKLAYER_PLAT_EnableSpecificIRQ(LL_HIGH_ISR_ONLY | LL_LOW_ISR_ONLY);
  return 0;
}

/*  ==== Semaphore Functions ====  */

os_semaphore_id os_semaphore_create(int32_t max_count,
	int32_t initial_count)
{
  /* Not used with sequencer utility */
  MW_UNUSED(max_count);
  MW_UNUSED(initial_count);
  return 0;
}

int32_t os_semaphore_wait(os_semaphore_id semaphore_id, uint32_t millisec)
{
  /* Not used with sequencer utility */
  MW_UNUSED(semaphore_id);
  MW_UNUSED(millisec);
  return 0;
}

int32_t os_semaphore_wait_isr(os_semaphore_id semaphore_id, uint32_t millisec)
{
  /* Not used with sequencer utility */
  MW_UNUSED(semaphore_id);
  MW_UNUSED(millisec);

  return 0;
}

int32_t os_semaphore_release(os_semaphore_id semaphore_id)
{
  MW_UNUSED(semaphore_id);
  ll_sys_schedule_bg_process();
  return 0;
}

int32_t os_semaphore_release_isr(os_semaphore_id semaphore_id)
{
  MW_UNUSED(semaphore_id);
  ll_sys_schedule_bg_process_isr();
  return 0;
}

int32_t os_semaphore_delete(os_semaphore_id semaphore_id)
{
  /* Not used with sequencer utility */
  MW_UNUSED(semaphore_id);
  return 0;
}
