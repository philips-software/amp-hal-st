/**
 ******************************************************************************
 * @file    entropy.c
 * @author  MCD Application Team
 * @brief   This file includes the platform abstraction for entropy generation.
 *
 *   This file provides firmware functions to manage the hw random  generator
 *   present on the device.
 *
 *   Apart from the function RandomInit() which need to be called at the
 *   initialization of the Application, all the other functions
 *   (eg :otPlatRandomGet() and otPlatSecureRandomGet(..)) are requested by the
 *   OpenThread middleware.
 *   The OpenThread requested functions are defined in the following file :
 *   Middlewares/Third_Party/Thread/openthread/include/platform/random.h
 *
 *
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


#include <stdlib.h>
#include <stdio.h>

#include "openthread/platform/entropy.h"
#include "openthread/platform/radio.h"
#include "platform.h"

otError otPlatEntropyGet(uint8_t *aOutput, uint16_t aOutputLength)
{
  otError error = OT_ERROR_NONE;
  uint32_t status = 0;
#if ENHANCED_RX_WHILE_CSMA_BACKOFF_DELAY
  status = mac_gen_rnd_num(aOutput, aOutputLength, FALSE);
#else
  status = mac_gen_rnd_num(aOutput, aOutputLength, TRUE);
#endif

  if (status != 0)
    error = OT_ERROR_FAILED;
  return error;
}
