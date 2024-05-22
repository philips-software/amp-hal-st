/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    uart.c
  * @author  MCD Application Team
  * @brief   This file implements the OpenThread platform abstraction for CLI
  *          UART communication.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "platform_wba.h"
#include "stdio.h"

#ifndef OPENTHREAD_RCP /* RCP cannot be used with CLI at same time */
#if (OT_CLI_USE == 1)

/**
 * The UART driver calls this method to notify OpenThread that bytes have been received.
 *
 * @param[in]  aBuf        A pointer to the received bytes.
 * @param[in]  aBufLength  The number of bytes received.
 *
 */
extern void otPlatUartReceived(const uint8_t *aBuf, uint16_t aBufLength);

extern void APP_THREAD_ScheduleUART(void);

/* Extern variables  ---------------------------------------------------------*/
#ifndef OT_CLI_UART_HANDLER
extern UART_HandleTypeDef huart1;
#define OT_CLI_UART_HANDLER huart1
#endif


/* Private function prototypes -----------------------------------------------*/
static void otUart_TxCpltCallback(UART_HandleTypeDef *huart);
static void otUart_RxCpltCallback(UART_HandleTypeDef *huart);
static void processTransmit(void);
static void processReceive(void);



/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE   1024  /* Data size to read (in bytes) */
#define CLI_BUFFER_NB 3

#define CLI_ECHO /* command echo on UART, comment to disable */

typedef struct {
  char cli_buffer[BUFFER_SIZE];
  uint16_t size;
} cli_payload_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t g_buffer[BUFFER_SIZE] = {0};
volatile uint8_t buffer_index;
UART_HandleTypeDef *oTCLIuart = &OT_CLI_UART_HANDLER;

/*
 * Circular buffers index:
 * - uart_buf_idx buffer index to be printed to the UART (0..CLI_BUFFER_NB)
 * - ot_buf_idx index to be filled by info from OT       (0..CLI_BUFFER_NB)
 */
uint8_t uart_buf_idx, ot_buf_idx;
uint8_t otUartRX = 0;
cli_payload_t cli_cmd_buffer[CLI_BUFFER_NB];

/* Whether OT UART task is currently scheduled (one each for RX/TX */
bool otUART_TX_Schdl = FALSE;
bool otUART_RX_Schdl = FALSE;

/*
 * UART used for commands input/output
 * The selection among those available is done in this order:
 * USART1 --> Error (no UART found)
 *
 */


otError otPlatUartEnable(void)
{
  otError error = OT_ERROR_NONE;

  uart_buf_idx = 0U;
  ot_buf_idx = 0U;
  buffer_index = 0U;

  /*
   * It is assumed that UART1 MSP Init is done by
   * CubeMX layer. As such, no need to call
   * MX_USART1_UART_Init & HAL_UART_MspInit.
   */

  /* register callbacks */
  oTCLIuart->TxCpltCallback = otUart_TxCpltCallback;
  oTCLIuart->RxCpltCallback = otUart_RxCpltCallback;
  HAL_UART_Receive_IT(oTCLIuart, &otUartRX, 1);

  for (uint8_t i = 0; i < CLI_BUFFER_NB; i++)
  {
    cli_cmd_buffer[i].size = 0U;
  }

  return error;
}

otError otPlatUartDisable(void)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatUartFlush(void)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

/* Execution of TX or forwarding of RX to OT stack*/
void arcUartProcess(void)
{
  if (otUART_RX_Schdl == TRUE)
    processReceive();

  if (otUART_TX_Schdl == TRUE)
    processTransmit();
}

static void processTransmit(void)
{
  HAL_StatusTypeDef HalError;

  /* Check size, processTransmit can be scheduled where as CLI output already
  printed in previous schedule */
  if (cli_cmd_buffer[uart_buf_idx].size == 0)
  {
    return;
  }
  /* Take current buffer index, send it to the UART */
  ot_buf_idx = uart_buf_idx++;
  uart_buf_idx %= CLI_BUFFER_NB;

  /* Wait for uart to be ready */
  while(oTCLIuart->gState != HAL_UART_STATE_READY);

  HalError = HAL_UART_Transmit_DMA(oTCLIuart, (uint8_t*)&cli_cmd_buffer[ot_buf_idx].cli_buffer[0],
                       cli_cmd_buffer[ot_buf_idx].size); 
  if (HalError != HAL_UART_ERROR_NONE)
  {
    APP_DBG("CLI UART ERROR 0x%x, buff idx : %d, buff size : %d \r\n",HalError, ot_buf_idx, cli_cmd_buffer[ot_buf_idx].size);
  }

  cli_cmd_buffer[ot_buf_idx].size = 0; /* Reset buffer */
}

static void processReceive(void)
{
  LL_LOCK();
  if (buffer_index > 0){
    otPlatUartReceived(g_buffer, buffer_index);
    buffer_index = 0;
    memset(g_buffer, 0x0, BUFFER_SIZE);
  }
  otUART_RX_Schdl = FALSE;
  LL_UNLOCK();
  return;
}

#if defined(CLI_ECHO)
static void processEcho(uint8_t *cmd, uint8_t cmd_size)
{
  /* Fill the buffer with CLI output */
  uint16_t size = cli_cmd_buffer[uart_buf_idx].size;

  memcpy(&cli_cmd_buffer[uart_buf_idx].cli_buffer[size], cmd, cmd_size);
  cli_cmd_buffer[uart_buf_idx].size += cmd_size ;

  /* Schedule the task if not */
  APP_THREAD_ScheduleUART();
  otUART_TX_Schdl = TRUE;
}
#endif /* CLI_ECHO */

static void otUart_TxCpltCallback(UART_HandleTypeDef *huart)
{
  UNUSED(huart);
  /* Re-schedule task if current buffer being filled by OT is non-empty */
  if (cli_cmd_buffer[uart_buf_idx].size)
    APP_THREAD_ScheduleUART();
  else
    otUART_TX_Schdl = FALSE;
}

static void otUart_RxCpltCallback(UART_HandleTypeDef *huart)
{
  UNUSED(huart);
  HAL_UART_Receive_IT(oTCLIuart, &otUartRX, 1);
  g_buffer[buffer_index] = otUartRX;
  buffer_index++;

  /* If ENTER, forward the command if non-empty*/
  if ( (otUartRX == '\r') && (buffer_index > 1)) {

#ifdef CLI_ECHO
    if (otUartRX == '\r')
      g_buffer[buffer_index++] = '\n';

    processEcho(g_buffer, buffer_index);
#endif /* CLI_ECHO */
    if (otUART_RX_Schdl == FALSE) {
      otUART_RX_Schdl = TRUE;
      APP_THREAD_ScheduleUART();
    }
  }
}

int CliUartOutput(void *aContext, const char *aFormat, va_list aArguments)
{
  UNUSED(aContext);
  uint16_t ret,size;

  /* Fill the buffer with CLI output */
  size = cli_cmd_buffer[uart_buf_idx].size;

  ret = (uint16_t)vsnprintf(&cli_cmd_buffer[uart_buf_idx].cli_buffer[size],
                            BUFFER_SIZE, aFormat, aArguments);

  /* Do not print "> ", allign with WB for CubeMonitorRF */
  if ((strncmp(&cli_cmd_buffer[uart_buf_idx].cli_buffer[size], "> ", 2) != 0)&&(ret != 0))
  {
    cli_cmd_buffer[uart_buf_idx].size += ret;

    /* Schedule the task if not */
    APP_THREAD_ScheduleUART();
    otUART_TX_Schdl = TRUE;
  }

  return ret;
}

#endif /* OT_CLI_USE */
#endif /* OPENTHREAD_RCP */