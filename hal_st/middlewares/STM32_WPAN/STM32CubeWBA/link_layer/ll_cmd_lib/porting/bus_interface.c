/*$Id: //dwh/bluetooth/DWC_ble154combo/rtl/rel/1.20a-sow04/RCE/DWC_ble154combo/firmware/shrd_utils/src/porting/mps2/bus_interface.c#1 $*/
/**
 ********************************************************************************
 * @file    bus_interface.c
 * @brief   This component for interfacing the bus with UART .
 ******************************************************************************
 * @copy
 *
 * THIS SOFTWARE AND THE ASSOCIATED  DOCUMENTATION ARE PROPRIETARY TO SYNOPSYS,
 * INC. THIS SOFTWARE MAY ONLY BE USED IN ACCORDANCE WITH THE TERMS AND
 * CONDITIONS OF A WRITTEN LICENSE AGREEMENT WITH SYNOPSYS, INC. ALL OTHER USE,
 * REPRODUCTION, OR DISTRIBUTION OF THIS SOFTWARE IS STRICTLY PROHIBITED.
 *
 * THE ENTIRE NOTICE ABOVE MUST BE REPRODUCED ON ALL AUTHORIZED COPIES.
 *
 * <h2><center>&copy; (C) COPYRIGHT 2020 SYNOPSYS, INC.</center></h2>
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
 *   <td><b> when </b></td>
 *   <td><b> who </b></td>
 *   <td><b> what, where, why </b></td>
 * </tr>
 * <tr>
 *   <td>  Create the file</td>
 * </tr>
 * </table>\n
 */

//#include "stm32_adv_trace.h"
#include "cmsis_compiler.h"
#include "bsp.h"
#include <stdio.h>
#include <assert.h>

#ifdef USE_HCI_TRANSPORT
#include "stm32wbaxx_hal.h"
#endif /* USE_HCI_TRANSPORT */

#ifdef USE_HCI_TRANSPORT
static ble_ll_bus * op_callback;
static uint8_t *g_read_bus_buffer;
static uint8_t *g_write_bus_buffer;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
#endif /* USE_HCI_TRANSPORT */

volatile uint16_t lgr_locked =0;

#ifdef USE_HCI_TRANSPORT

/**
  * @brief  Rx Transfer completed callback.
  * @param husart USART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
  
    LL_UNUSED(huart);
    if (NULL != op_callback)
        op_callback->read(g_read_bus_buffer);
}


/**
  * @brief Tx Transfer completed callback.
  * @param husart USART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1){
    if (NULL != op_callback)
	op_callback->write(g_write_bus_buffer);
  }

  else if(huart->Instance == USART2){
        lgr_locked = 0;
  }
}

/**
 * @brief   Bus initialization
 *
 * @param  void
 * @retval void
 *
 */
void bus_init(ble_ll_bus * op)
{
    op_callback = op;
}

/**
 * @brief   Bus read
 *
 * @param   *buffer: pointer to the buffer of the data
 * @param   size: size of bytes read from the buffer
 *
 * @retval void
 */
void bus_read(uint8_t *buffer, uint16_t size)
{
    g_read_bus_buffer = buffer;
#if defined(OPENTHREAD_FTD) || defined(OPENTHREAD_MTD)
    HAL_UART_Receive_IT(&huart1, buffer, size);
#else
    HAL_UART_Receive_DMA(&huart1, buffer, size);
#endif
    return;
}

/**
 * @brief   Bus write
 *
 * @param   *buffer: pointer to the buffer of the data
 * @param   size: size of bytes written to the buffer
 *
 * @retval void
 */
void bus_write(uint8_t *buffer, uint16_t size)
{
    g_write_bus_buffer = buffer;
#if defined(OPENTHREAD_FTD) || defined(OPENTHREAD_MTD)
    HAL_UART_Transmit_IT(&huart1, buffer, size);
#else
    HAL_UART_Transmit_DMA(&huart1, buffer, size);
#endif
    return;
}

#endif /* USE_HCI_TRANSPORT */

__WEAK void app_logger_write(uint8_t *buffer, uint32_t size)
{
  (void)*buffer;
  (void)size;
}

void logger_write(uint8_t *buffer, uint32_t size)
{
  lgr_locked = 1;
  app_logger_write(buffer, size);
  return;
}

void logger_init()
{
  /* Intentionally empty */

}

/**
 * @}
 */
/******************* (C) COPYRIGHT 2020 SYNOPSYS, INC. *****END OF FILE****/
