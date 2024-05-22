/**
 ******************************************************************************
 * @file    ble.h
 * @author  MCD Application Team
 * @brief   BLE interface
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLE_H
#define __BLE_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "ble_conf.h"
#include "ble_dbg_conf.h"
  
/**< core */
#include "ble_core.h"
#include "ble_bufsize.h"
#include "ble_defs.h"
#include "ble_std.h"

#include "svc/Inc/svc_ctl.h"

#include "svc/Inc/uuid.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /*__BLE_H */
