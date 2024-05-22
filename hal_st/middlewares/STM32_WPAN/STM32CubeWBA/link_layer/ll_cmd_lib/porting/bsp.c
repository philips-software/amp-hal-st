/*$Id: //dwh/bluetooth/DWC_ble154combo/firmware/rel/1.21a-SOW07PatchV6/firmware/shrd_utils/src/porting/emsk/bsp.c#1 $*/
/**
 ********************************************************************************
 * @file    bsp.c
 * @brief   Link Layer BSP for WBA
 ******************************************************************************
 * @copy
 *
 *(C) COPYRIGHT 2022 SYNOPSYS, Inc. This Synopsys "product" and all associated documentation 
 *are proprietary to Synopsys, Inc. and may only be used pursuant to the terms and 
 *conditions of a written license agreement with Synopsys, Inc. All other use, 
 *reproduction, modification, or distribution of the Synopsys "product" or the associated 
 *documentation is strictly prohibited.
 *
 *
 * THE ENTIRE NOTICE ABOVE MUST BE REPRODUCED ON ALL AUTHORIZED COPIES.
 *
 * <h2><center>&copy; (C) (C) COPYRIGHT 2022 SYNOPSYS, INC.</center></h2>
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
 
#include <bsp.h>
#include <bsp_arch.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <arm_acle.h>
#include <common_cntrlrs.h>

#define DWC_BLE_LL_0_SIZE  (0xBFFF)   /*!< 2.4 GHz RADIO peripheral 4 Kbytes */
#define TIMER_FREQUENCY_HZ (32000u)

extern void ll_sys_init(void);
extern void ll_sys_delay_us(uint32_t delay);
extern void ll_sys_assert(uint8_t condition);
extern void ll_sys_radio_ack_ctrl(uint8_t enable);
extern void ll_sys_radio_wait_for_busclkrdy(void);
extern void ll_sys_get_rng(uint8_t *ptr_rnd, uint32_t len);

extern void ll_sys_setup_radio_intr(void (*intr_cb)());
extern void ll_sys_setup_radio_sw_low_intr(void (*intr_cb)());
extern void ll_sys_radio_sw_low_intr_trigger(uint8_t priority);

extern void ll_sys_radio_evt_not(uint8_t start);
extern void ll_sys_rco_clbr_not(uint8_t start);
extern void ll_sys_request_temperature(void);

extern void ll_sys_enable_os_context_switch(void);
extern void ll_sys_disable_os_context_switch(void);

extern void LINKLAYER_DEBUG_SIGNAL_SET(Debug_GPIO_t gpio);
extern void LINKLAYER_DEBUG_SIGNAL_RESET(Debug_GPIO_t gpio);
extern void LINKLAYER_DEBUG_SIGNAL_TOGGLE(Debug_GPIO_t gpio);

extern void ll_sys_schldr_timing_update_not(Evnt_timing_t * p_evnt_timing);

int bsp_init ()
{
	ll_sys_init();
        return 0;
}

void bsp_delay_us(uint32_t delay)
{
	ll_sys_delay_us(delay);
}

void bsp_delay_ms(uint32_t delay)
{
	bsp_delay_us(delay*1000);
}

int bsp_intr_enable(uint32_t intrNum, void (*intr_cb)())
{
	return 0;
}

/* Not used in the STM32WBA as the LL interrupt is the most important priority 
   on the system. LL interrupt priority define only one time at init. 
*/
int bsp_intr_set_pri(uint32_t intrNum, void (*intr_cb)(), int32_t intpri)
{
  switch(intrNum)
  {
    case DWC_BLE_LL_INTR_NUM:
        ll_sys_setup_radio_intr(intr_cb);
        break;
        
    case DWC_BLE_LL_LOW_ISR_INTR_NUM:
        ll_sys_setup_radio_sw_low_intr(intr_cb);
        break;
    
  }
  return 0;
}

#if LOGGER_ENABLE
inline void bsp_assert_log(uint8_t condition, uint8_t severity, const char *ptr_func_name,  const int line)
{
	uint8_t logStr[70] = {0};
        
        if(severity == SEVERITY_HIGH)
        {
          if (condition == 0) {

                  sprintf((char *)logStr, " \n\rASSERTION IN FUN:%-25s  LINE:%d ",
                          ptr_func_name, line);
                  logger_write(logStr, 70);
          }
          ll_sys_assert(condition);
        }
}
#else
void bsp_assert(uint8_t condition, uint8_t severity)
{
  if(severity == SEVERITY_HIGH)
  {
    ll_sys_assert(condition);
  }
}
#endif /* LOGGER_ENABLE */


int bsp_is_ptr_in_ble_mem(void* inp_ptr)
{
	if(((uint32_t)inp_ptr >= DWC_BLE_LL_0_BASEADDR) && ((uint32_t)inp_ptr <= (DWC_BLE_LL_0_BASEADDR + DWC_BLE_LL_0_SIZE)))
	{
		return 1;
	}
	return 0;
}

int ll_intf_is_ptr_in_ble_mem(void* inp_ptr)
{
  return bsp_is_ptr_in_ble_mem(inp_ptr);
}

/* WFI (wait for interrupt) suspends execution until one of the following events occurs:
	- an exception
	- an interrupt becomes pending, which would preempt if PRIMASK was clear */
void bsp_mcu_slp(void)
{
    /* Not implemented for FPGA validation */	
	return;
}
/* Exit implementation for baremetal */
__WEAK void exit(int reason)
{
	LL_UNUSED(reason);
	while(1);
}

void bsp_control_hclk(uint8_t enable)
{
	/**
	 * This function should enable/disable the bus clock.
	 *
	 * For reference, refer to this function implementation in the bsp module of
	 * the EMSK board where the bus clock is controlled by a GPIO signal.
	 */
	LL_UNUSED(enable);
}

void bsp_control_aclk(uint8_t enable)
{
	/**
	 * This function should enable/disable the active clock.
	 *
	 * For reference, refer to this function implementation in the bsp module of
	 * the EMSK board where the active clock is controlled by a GPIO signal.
	 */
  ll_sys_radio_ack_ctrl(enable);
}

/**
 * @brief  wait for bus clock ready signal
 *
 * A platform that has more accurate information about the readiness of the bus clock should implement this function to
 * avoid redundant delay while reading sleep timer .
 *
 * @note this function will be called only if @ref USE_SOC_ACCURATE_BUS_CLK_READY_API is set to one otherwise LL FW will wait for a change in sleep timer reading.
 *
 * @param None.
 *
 * @retval None.
 */
void bsp_wait_for_busclkrdy()
{
  ll_sys_radio_wait_for_busclkrdy();
}

/**
 * @brief used to assert/trigger the low priority interrupt from the SW.
 *
 * @param priority: if 1 then this SW ISR should be treated as if it was High priority HW ISR
 *
 * @retval None.
 */
void bsp_switch_to_lw_isr(uint8_t priority) {
  ll_sys_radio_sw_low_intr_trigger(priority);
}

uint32_t gen_rnd_num(uint8_t *ptr_rnd, uint32_t len){
  ll_sys_get_rng(ptr_rnd, len);
  return 0;
}

void bsp_evnt_not(EvntNotiState enable)
{
  uint8_t start = 0;
    
  switch(enable)
  {
    case EVNT_START:
      start = 1;
      break;
        
    case EVNT_END:
      break;
        
    case EVNT_NOT_SPECIIFED:
      /* Unspecified radio event specification not supported */
      ll_sys_assert(0);
      break;
  }
  
  ll_sys_radio_evt_not(start);
}

/**
 * @brief	Notification that LL FW will start or end RCO Calibration .
 *
 *@note this is an optional wrapper that used to inform the upper layer of the state of RCO calibration.
 *@note the upper layer may ignore this wrapper
 * @param	enable: EVNT_START , RCO calibration will be started
 * 				  : Evnt_END     RCO calibration has  completed
  *
 * @retval None.
 */
void bsp_rco_clbr_not(EvntNotiState enable)
{
	uint8_t start = 0;
    
  switch(enable)
  {
    case EVNT_START:
      start = 1;
      break;
        
    case EVNT_END:
      break;
        
    case EVNT_NOT_SPECIIFED:
      /* Unspecified radio event specification not supported */
      ll_sys_assert(0);
      break;
  }
	
	ll_sys_rco_clbr_not(start);
}

/**
 * @brief used to start temperature calculations
 * if the upper layer has informed the link layer by the existence of temperature sensor through @ref ll_intf_set_temperature_sensor_stat() or llhwc_cmn_set_temperature_sensor_stat()  . 
 * New temperature will be requested in the following cases:
 * 1- at initialization when the @ref ll_intf_set_temperature_sensor_state or llhwc_cmn_set_temperature_sensor_stat is called
 * 2- before any radio event
 * Once temperature is ready the upper layer should call @ref ll_intf_set_temperature_value() or llhwc_cmn_set_temperature_value() to inform Link Layer with new Temperature value
 * @retval None.
 */
void bsp_request_temperature()
{
    ll_sys_request_temperature();
}

void bsp_debug_gpio_set(Debug_GPIO_t gpio)
{
	LINKLAYER_DEBUG_SIGNAL_SET(gpio);
}
void bsp_debug_gpio_clear(Debug_GPIO_t gpio)
{
	LINKLAYER_DEBUG_SIGNAL_RESET(gpio);
}
void bsp_debug_gpio_toggle(Debug_GPIO_t gpio)
{
	LINKLAYER_DEBUG_SIGNAL_TOGGLE(gpio);
}

/**
 * @brief a function to inform the upper layer by state of periodic calibration state.
 *
 * @param state[in]: Value from @ref PhyClbrState, NO_PHY_CLBR_NEEDED  when the calibration is completed , PHY_CLBR_PNDING_OR_RUNNING when calibration is started or about to start from background task
 *
 * @retval None.
 */
void bsp_set_phy_clbr_state(PhyClbrState state)
{
  switch(state)
  {
    case PHY_CLBR_PNDING_OR_RUNNING:
      ll_sys_disable_os_context_switch();
      break;
        
    case PHY_CLBR_NOT_RUNNING:
      ll_sys_enable_os_context_switch();
      break;
        
    case EVNT_NOT_SPECIIFED:
      /* Unspecified calibration state not supported */
      ll_sys_assert(0);
      break;
  }
}  

  /**
 * @brief a function to notify the upper layer to switch the clock.
 *
 * @param evnt_timing[in]: Evnt_timing_t pointer to structure contains drift time , execution time and scheduling time
 *
 * @retval None.
 */
void bsp_evnt_schldr_timing_update_not(Evnt_timing_t * p_evnt_timing)
{
  ll_sys_schldr_timing_update_not(p_evnt_timing);
}
