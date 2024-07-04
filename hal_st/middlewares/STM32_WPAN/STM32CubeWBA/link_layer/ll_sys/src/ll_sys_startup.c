/**
  ******************************************************************************
  * @file    ll_sys_startup.c
  * @author  MCD Application Team
  * @brief   Link Layer IP system interface startup module
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

#include "ll_sys.h"
#include "ll_intf.h"
#include "ll_sys_startup.h"
#include "common_types.h"

static void ll_sys_dependencies_init(void);
#ifdef BLE
static void ll_sys_event_missed_cb( ble_buff_hdr_t* ptr_evnt_hdr )
{

}

/**
  * @brief  Initialize the Link Layer IP BLE controller
  * @param  None
  * @retval None
  */
void ll_sys_ble_cntrl_init(hst_cbk hostCallback)
{
  const struct hci_dispatch_tbl* p_hci_dis_tbl;
  hci_get_dis_tbl( &p_hci_dis_tbl );

  ll_intf_init(p_hci_dis_tbl);

  ll_intf_rgstr_hst_cbk(hostCallback);

  ll_intf_rgstr_hst_cbk_ll_queue_full( ll_sys_event_missed_cb );

  ll_sys_dependencies_init();
}
#endif /* BLE */

/**
  * @brief  Initialize the Link Layer ressources for startup.
  *         This includes: - Deep Sleep feature ressources
  *                        - Link Layer background task
  * @param  None
  * @retval None
  */
static void ll_sys_dependencies_init(void)
{
  ll_sys_status_t dp_slp_status = LL_SYS_ERROR;

  /* Deep sleep feature initialization */
  dp_slp_status = ll_sys_dp_slp_init();
  ll_sys_assert(dp_slp_status == LL_SYS_OK);

  /* Background task initialization */
  ll_sys_bg_process_init();
}
