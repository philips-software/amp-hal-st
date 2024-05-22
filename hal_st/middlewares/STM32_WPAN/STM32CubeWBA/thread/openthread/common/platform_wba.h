/*
 * platfrom_wba.h
 *
 *  Created on: May 4, 2021
 *      Author: frq02098
 */

#ifndef PLATFORM_WBA_H_
#define PLATFORM_WBA_H_

#include <stdint.h>
#include <stdbool.h>
#include "openthread/instance.h"
#include "os_wrapper.h"

extern os_mutex_id g_ll_lock;
#define LL_LOCK() os_rcrsv_mutex_wait(g_ll_lock,0xffffffff)
#define LL_UNLOCK() os_rcrsv_mutex_release(g_ll_lock)

#define NO_EVT_TIME 0xFFFFFFFF

// Global OpenThread instance structure
extern otInstance *sInstance;


/**
 * This function initializes the alarm service used by OpenThread.
 *
 */
void arcAlarmInit(void);

/**
 * This function retrieves the time remaining until the alarm fires.
 *
 * @param[out]  aTimeval  A pointer to timer value.
 *
 */
void arcAlarmUpdateTimeout(int32_t *aTimeout);

/**
 * @brief     This function performs alarm driver processing.
 * @param[in] aInstance  A pointer to OT instance.
 * @retval    None
 */
void arcAlarmProcess(otInstance *aInstance);

/**
 * @brief     This function performs us alarm driver processing.
 * @param[in] aInstance  A pointer to OT instance.
 * @retval    None
 */
void arcUsAlarmProcess(otInstance *aInstance);

/**
 * @brief     This function returns the next alarm event.
 * @param[in] None
 * @retval    Relative time until next alarm in us. (0xFFFFFFFF if none)
 */
uint64_t arcAlarmGetRemainingTimeNextEvent(void);

/**
 * This function initializes the radio service used by OpenThread.
 *
 */
void arcRadioInit(void);

/**
 * This function performs radio driver processing.
 *
 */
void arcRadioProcess(otInstance *aInstance);

/**
 * This function initializes the random number service used by OpenThread.
 *
 */
void arcRandomInit(void);

/**
 * This function performs UART driver processing.
 *
 */
void arcUartProcess(void);

/**
 * This function initializes the UART driver.
 *
 */
otError otPlatUartEnable(void);

/**
 * This function is called by the OT stack to print information from the stack
 * (for example return of CLI commands).
 */
int CliUartOutput(void *aContext, const char *aFormat, va_list aArguments);

/**
 * This function initializes the NCP, definition in examples/apps/ncp.c.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 *
 */
void otAppNcpInit(otInstance *aInstance);

/**
 * This function performs all platform-specific initialization of OpenThread's drivers.
 *
 * @note This function is not called by the OpenThread library. Instead, the system/RTOS should call this function
 *       when initialization of OpenThread's drivers is most appropriate.
 *
 * @param[in]  argc  Number of arguments in @p argv.
 * @param[in]  argv  Argument vector.
 *
 */
void otSysInit(int argc, char *argv[]);

void otSysRequestPseudoReset(void);

void otlogTimestamp(uint8_t * pData, uint16_t * piSize);

#endif /* PLATFORM_WBA_H_ */
