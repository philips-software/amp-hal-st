/*
 *  Copyright (c) 2018, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements the OpenThread platform abstraction for the alarm.
 *
 */

#include "stm32_timer.h"
#include OPENTHREAD_PROJECT_CORE_CONFIG_FILE
#include "openthread/platform/alarm-milli.h"
#if OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE
#include "openthread/platform/alarm-micro.h"
#endif
#include "platform_wba.h"

#define OT_INSTANCE_NUMBER                1
#define OT_ALARM_MILLI_MAX_EVENT          1
#define OT_ALARM_MICRO_MAX_EVENT          1
#define NO_STORED_INSTANCE                255

#define FROM_MS_TO_US(ms)   (ms/1000)
#define MIN(A, B) (((A) < (B)) ? (A) : (B))

extern void APP_THREAD_ScheduleAlarm(void);
extern void APP_THREAD_ScheduleUsAlarm(void);
extern void ral_set_ot_base_slp_time_value(uint32_t time);
extern uint64_t ral_cnvert_slp_tim_to_ot_tim(uint32_t time);
extern uint32_t get_current_time(void);

static void ms_alarm_timer_cbk(void *arg);
static void us_alarm_timer_cbk(void *arg);
#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
static uint32_t instance_idx = 0;
#endif
static UTIL_TIMER_Time_t sCounter_ms = 0;
static uint32_t sCounter_steps = 0;

typedef struct ms_alarm_struct
{
  uint32_t expires;
  uint8_t sIsRunning;
} ms_alarm_struct;

typedef struct csl_alarm_struct
{
  otInstance *aInstance;
  uint32_t expires;
  uint8_t sIsRunning;
}csl_alarm_struct;

#define OT_ALARM_INVALID_IDX (-1)
ms_alarm_struct milli_alarm_struct_st[OT_ALARM_MILLI_MAX_EVENT];
csl_alarm_struct micro_alarm_struct_st[OT_INSTANCE_NUMBER];
uint8_t milli_alarm_pending;
uint8_t micro_alarm_pending;

static UTIL_TIMER_Object_t ms_alarm_process_tmr;
static os_timer_id csl_timer_id;

#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
uint8_t set_tmr_index(otInstance *aInstance)
{
  uint8_t i = 0, inst_num = NO_STORED_INSTANCE;
  for (i = 0; i < OT_ALARM_MILLI_MAX_EVENT; i++)
  {
    if (NULL == alarm_struct_st[i].aInstance){
      milli_alarm_struct_st[i].aInstance = aInstance;
      inst_num = i;
      break;
    }
    else if(aInstance == milli_alarm_struct_st[i].aInstance){
      inst_num = i;
      break;
    }
  }
  return inst_num;
}
#endif

void arcAlarmInit(void)
{
  uint8_t i;

  milli_alarm_pending = 0U;
  UTIL_TIMER_Create(&ms_alarm_process_tmr, 0xFFFFFFFF, UTIL_TIMER_ONESHOT, &ms_alarm_timer_cbk, 0);
  for (i = 0U; i < OT_ALARM_MILLI_MAX_EVENT ; i++){
    milli_alarm_struct_st[i].sIsRunning = 0;
  }

#if OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE
  micro_alarm_pending = 0;
  csl_timer_id = os_timer_create((void (*)())us_alarm_timer_cbk, os_timer_once, NULL);
  for (i = 0U; i < OT_INSTANCE_NUMBER ; i++){
    micro_alarm_struct_st[i].sIsRunning = 0;
  }
#endif /* OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE */

  sCounter_ms = UTIL_TIMER_GetCurrentTime();
  sCounter_steps = get_current_time();
  ral_set_ot_base_slp_time_value(sCounter_steps);
}

/* Used by RCP for CSL latency transport (UART, SPI etc..) offset calculation */
uint32_t otPlatTimeGet(void)
{
  return otPlatAlarmMicroGetNow();
}

uint32_t otPlatAlarmMilliGetNow(void)
{
  UTIL_TIMER_Time_t curr_timer_ms = UTIL_TIMER_GetCurrentTime();
  return (uint32_t)(curr_timer_ms - sCounter_ms);
}

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
  instance_idx = set_tmr_index(aInstance);
    if(instance_idx == NO_STORED_INSTANCE)while(1);
#endif
  uint32_t alarm_expire_ms = (uint32_t)(aT0 + aDt);

  milli_alarm_struct_st[0].expires = alarm_expire_ms;
  milli_alarm_struct_st[0].sIsRunning = 1;

  /* Schedule Alarm */
  APP_THREAD_ScheduleAlarm();
}

void otPlatAlarmMilliStop(otInstance *aInstance)
{
  milli_alarm_struct_st[0].sIsRunning = 0;
  milli_alarm_pending = 0;
}

static void MilliAlarmRemoveFirstAlarm(void)
{
  milli_alarm_struct_st[0].sIsRunning = 0;
}

#if OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE
void otPlatAlarmMicroStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
  (void)(aInstance);

  uint32_t curr_time= get_current_time();
  uint32_t alarm_expire_us = (uint32_t)(aT0 + aDt);

  micro_alarm_struct_st[0].sIsRunning = 1;
  micro_alarm_struct_st[0].expires = alarm_expire_us;

  /* Schedule Alarm */
  os_timer_start_in_us(csl_timer_id, alarm_expire_us - ral_cnvert_slp_tim_to_ot_tim(curr_time));
}

void otPlatAlarmMicroStop(otInstance *aInstance)
{
  micro_alarm_struct_st[0].sIsRunning = 0;
  os_timer_stop(csl_timer_id);
}

uint32_t otPlatAlarmMicroGetNow(void)
{
  uint32_t steps = get_current_time();
  return (uint32_t) ral_cnvert_slp_tim_to_ot_tim(steps);
}

static uint32_t arcMicroAlarmGetRemainingTime(void)
{
  uint32_t rem_time;

  if (micro_alarm_struct_st[0].sIsRunning == 1)
  {
    uint32_t alarm_time = micro_alarm_struct_st[0].expires;
    uint32_t curr_time = otPlatAlarmMicroGetNow();
    rem_time = (alarm_time < curr_time) ? 0 : (alarm_time - curr_time);
  }
  else
  {
    rem_time = NO_EVT_TIME;
  }
  return rem_time;
}

#endif /* OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE */

static uint32_t arcMilliAlarmGetRemainingTime(void)
{
  uint32_t rem_time;

  if (milli_alarm_struct_st[0].sIsRunning == 1)
  {
    uint32_t alarm_time = milli_alarm_struct_st[0].expires;
    uint32_t curr_time = otPlatAlarmMilliGetNow();
    rem_time = (alarm_time < curr_time) ? 0 : (alarm_time - curr_time);
  }
  else
  {
    rem_time = NO_EVT_TIME;
  }
  return rem_time;
}

/**
 * @brief     This function returns the next alarm event in us.
 * @param[in] None
 * @retval    Relative time until next alarm in us. (NO_EVT_TIME if none)
 */
uint64_t arcAlarmGetRemainingTimeNextEvent(void)
{
#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
#error "Option not supported for arcAlarmRemainingTime()"
#endif
  uint64_t retval = (uint64_t)arcMilliAlarmGetRemainingTime();
  /* Convert in us */
  retval = FROM_MS_TO_US(retval);
#if OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE
  uint64_t alarm_expires_us = (uint64_t)arcMicroAlarmGetRemainingTime();
  retval = MIN(retval, alarm_expires_us);
#endif /* #if OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE */
  return retval;
}

static void ms_alarm_timer_cbk(void *arg)
{
  UTIL_TIMER_Stop(&ms_alarm_process_tmr);
  APP_THREAD_ScheduleAlarm();
}

static void us_alarm_timer_cbk(void *arg)
{
  APP_THREAD_ScheduleUsAlarm();
}

void arcAlarmProcess(otInstance *aInstance)
{
  uint32_t nxt_evt_time = arcMilliAlarmGetRemainingTime();
  if (nxt_evt_time == 0) {
    MilliAlarmRemoveFirstAlarm();
    otPlatAlarmMilliFired(aInstance);
    //APP_THREAD_ScheduleAlarm();
  } else if (nxt_evt_time < NO_EVT_TIME) {
    /* Start timer for next event */
    UTIL_TIMER_StartWithPeriod(&ms_alarm_process_tmr, nxt_evt_time);
  }
}

void arcUsAlarmProcess(otInstance *aInstance)
{
  if (micro_alarm_struct_st[0].sIsRunning == 1)
  {
    micro_alarm_struct_st[0].sIsRunning = 0;
    otPlatAlarmMicroFired(aInstance);
  }
}