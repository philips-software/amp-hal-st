
 /**
  ******************************************************************************
  * @file    logging_stm32wba.c
  * @author  MCD Application Team
  * @brief   This file contains all the defines and structures used for the
  *          communication between the two core M0 and M4.
  *          This file is shared between the code running on M4 and the code
  *          running on M0.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/**
 * @file
 *   This file implements the OpenThread platform abstraction for logging.
 *
 */
//#include "stm32wbaxx_hal.h"
#include "app_conf.h"
#include "utilities_conf.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "logging.h"
#include "platform_wba.h"
#define LOG_PARSE_BUFFER_SIZE  356U

/* JCA ADDED */
#define USE_PRINTF 0U

#define LOG_TIMESTAMP_ENABLE 0U
#define LOG_REGION_ENABLE 0U
#define LOG_RTT_COLOR_ENABLE 0U

#if (LOG_RTT_COLOR_ENABLE == 1U)
#define RTT_COLOR_CODE_DEFAULT "\x1b[0m"
#define RTT_COLOR_CODE_RED     "\x1b[0;91m"
#define RTT_COLOR_CODE_GREEN   "\x1b[0;92m"
#define RTT_COLOR_CODE_YELLOW  "\x1b[0;93m"
#define RTT_COLOR_CODE_BLUE    "\x1b[0;94m"
#define RTT_COLOR_CODE_MAGENTA "\x1b[0;95m"
#define RTT_COLOR_CODE_CYAN    "\x1b[0;96m"

#else // LOG_RTT_COLOR_ENABLE == 1U
#define RTT_COLOR_CODE_DEFAULT ""
#define RTT_COLOR_CODE_RED     ""
#define RTT_COLOR_CODE_GREEN   ""
#define RTT_COLOR_CODE_YELLOW  ""
#define RTT_COLOR_CODE_BLUE    ""
#define RTT_COLOR_CODE_MAGENTA ""
#define RTT_COLOR_CODE_CYAN    ""
#endif // LOG_RTT_COLOR_ENABLE == 1U
/* END JCA ADDED */

/*   OT Log Levels :                                  OT_LOG_LEVEL_NONE  OT_LOG_LEVEL_CRIT   OT_LOG_LEVEL_WARN     OT_LOG_LEVEL_NOTE   OT_LOG_LEVEL_INFO   OT_LOG_LEVEL_DEBG */
const Log_Verbose_Level_t otLevelToVerboseTable[] = { LOG_VERBOSE_INFO,  LOG_VERBOSE_ERROR,  LOG_VERBOSE_WARNING,  LOG_VERBOSE_INFO,   LOG_VERBOSE_INFO,   LOG_VERBOSE_DEBUG };

extern uint32_t otPlatAlarmMilliGetNow(void);

static uint8_t debugLevel = 1;

uint8_t getDebugLevel(void)
{
  return debugLevel;
}

void setDebugLevel(uint8_t aDebugLevel)
{
  debugLevel = aDebugLevel;
}

#if (LOG_RTT_COLOR_ENABLE == 1U)
/**
 * Function for getting color of a given level log.
 *
 * @param[in]  aLogLevel The log level.
 *
 * @returns  String with a log level color value.
 */
static inline const char *levelToString(otLogLevel aLogLevel)
{
    switch (aLogLevel)
    {
    case OT_LOG_LEVEL_CRIT:
        return RTT_COLOR_CODE_RED;

    case OT_LOG_LEVEL_WARN:
        return RTT_COLOR_CODE_YELLOW;

    case OT_LOG_LEVEL_INFO:
        return RTT_COLOR_CODE_GREEN;

    case OT_LOG_LEVEL_DEBG:
    default:
        return RTT_COLOR_CODE_DEFAULT;
    }
}

/**
 * Function for printing log level.
 *
 * @param[inout]  aLogString  Pointer to log buffer.
 * @param[in]     aMaxSize    Maximum size of log buffer.
 * @param[in]     aLogLevel   Log level.
 *
 * @returns  Number of bytes successfully written to the log buffer.
 */
static inline uint16_t logLevelPrint(char *aLogString, uint16_t aMaxSize,
                                otLogLevel aLogLevel)
{
    return (uint16_t)(snprintf(aLogString, aMaxSize, "%s ", levelToString(aLogLevel)));
}
#endif //LOG_RTT_COLOR_ENABLE

#if (LOG_TIMESTAMP_ENABLE == 1U)
/**
 * Function for printing actual timestamp.
 *
 * @param[inout]  aLogString Pointer to the log buffer.
 * @param[in]     aMaxSize   Maximum size of the log buffer.
 *
 * @returns  Number of bytes successfully written to the log buffer.
 */
static inline uint16_t logTimestamp(char *aLogString, uint16_t aMaxSize)
{
    return (uint16_t)(snprintf(aLogString, aMaxSize, "%s[%010lu]", RTT_COLOR_CODE_DEFAULT,
                    (unsigned long)otPlatAlarmMilliGetNow()));
}
#endif //LOG_TIMESTAMP_ENABLE


void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char * aFormat, ...)
{
  /*
   * Note that the support for log region is removed. The OT core will always emit all logs with `OT_LOG_REGION_CORE`
   * as @p aLogRegion
   */
  (void)(aLogRegion);

#if ( (USE_PRINTF == 1U) || (CFG_LOG_SUPPORTED != 0) )
#ifdef DYNAMIC_MODE
  // To be updated
  if ( ( getDebugLevel() != 0) || ( aLogLevel == 6 ) )
#else /* DYNAMIC_MODE */
  if ( getDebugLevel() != 0 )
#endif /* DYNAMIC_MODE */
  {
    va_list paramList;
    va_start(paramList, aFormat);
    
#if (USE_PRINTF == 1U)    
    uint16_t length = 0;
    char logString[LOG_PARSE_BUFFER_SIZE + 1U];

#if (LOG_TIMESTAMP_ENABLE == 1U)
    length += logTimestamp(logString, LOG_PARSE_BUFFER_SIZE);
#endif /* (LOG_TIMESTAMP_ENABLE == 1U) */

#if (LOG_RTT_COLOR_ENABLE == 1U)
    /* Add level information */
    length += logLevelPrint(&logString[length], (LOG_PARSE_BUFFER_SIZE - length), aLogLevel);
#endif /* (LOG_RTT_COLOR_ENABLE == 1U) */

    /* Parse user string */
    length += vsnprintf(&logString[length], (LOG_PARSE_BUFFER_SIZE - length), aFormat, paramList);
    logString[length++] = '\n';
    logString[length++] = '\r';
    logString[length++] = 0U;
    va_end(paramList);

    printf("%s\r\n", logString);
#else /* (USE_PRINTF == 1U) */
    Log_Module_PrintWithArg( otLevelToVerboseTable[aLogLevel], (Log_Region_t)LOG_REGION_THREAD, aFormat, paramList );
#endif /* (USE_PRINTF == 1U) */
  }
#else /* (USE_PRINTF == 1U) || (CFG_LOG_SUPPORTED != 0) */
  /* No log possible, abort */
  UNUSED(aLogLevel);
  UNUSED(aFormat);
#endif /* (USE_PRINTF == 1U) || (CFG_LOG_SUPPORTED != 0) */
}



/**
 * Function for printing actual timestamp.
 *
 * @param[inout]  pData Pointer to the log buffer.
 * @param[in]     piSize   Size of the timestamp insert.
 *
 *
 */
void otlogTimestamp(uint8_t * pData, uint16_t * piSize)
{
  uint16_t ByeWritten = 0;
  ByeWritten = snprintf((char*)pData, LOG_PARSE_BUFFER_SIZE, "[%010lu] ",
                    (unsigned long)otPlatAlarmMilliGetNow());
  *piSize = ByeWritten;
}
