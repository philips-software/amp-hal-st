/**
 ******************************************************************************
 * @file    misc.c
 * @author  MCD Application Team
 * @brief   This file manages the implementation of specific ot commands.
 *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]

  @endverbatim
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2021 STMicroelectronics</center></h2>
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

#include <platform/misc.h>
#include "platform_wba.h"

static void ot_assert_failed(char const* file, uint32_t line);


void otPlatReset(otInstance *aInstance)
{
  (void)aInstance;
//  /*----------------------------*/
//  /* Reset the stack            */
//  /*----------------------------*/
//  DeinitThreadStack();
//  InitThreadStack();
  otSysRequestPseudoReset();
}

otPlatResetReason otPlatGetResetReason(otInstance *aInstance)
{
  (void)aInstance;

  return OT_PLAT_RESET_REASON_POWER_ON; 
}

void otPlatAssertFail(const char *aFilename, int aLineNumber)
{
  //OT_UNUSED_VARIABLE(aFilename);
  //OT_UNUSED_VARIABLE(aLineNumber);
  ot_assert_failed(aFilename, aLineNumber);
}

void otPlatWakeHost(void)
{
 
}


/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
static void ot_assert_failed(char const* file, uint32_t line)
{
  /* Prevent unused argument(s) compilation warning */
  (void)file;
  (void)line;

  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* SW breakpoint for debugging purpose */
  __asm("BKPT #0\n") ;

  /* Infinite loop */
  while (1)
  {
  }
}
