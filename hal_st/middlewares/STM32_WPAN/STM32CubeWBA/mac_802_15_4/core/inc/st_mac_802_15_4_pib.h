/**
  ******************************************************************************
  * @file    st_mac_802_15_4_pib.h
  * @author  MCD-WBL Application Team
  * @brief   MAC PIB attributes manager service.
  *          This file provide service headers to manage MAC Pib attributes
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ST_MAC_802_15_4_PIB_H
#define ST_MAC_802_15_4_PIB_H

#include "stdint.h"

/**
  * @brief  This function returns the pib attributes len required due to sub
            layer implementation choice.
  * @param  mac_cntx_ptr :
  * @param  pib_attr_id  :
  * @retval None
  */

uint8_t MAC_PIB_get_attribute_len(void* mac_cntx_ptr,uint8_t pib_attr_id);

#endif /* _ST_MAC_802_15_4_ERRORS_H_ */
