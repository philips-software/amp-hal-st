/*$Id: //dwh/bluetooth/DWC_ble154combo/firmware/rel/1.30a-SOW04PatchV2/firmware/public_inc/mem_intf.h#1 $*/
/**
 ********************************************************************************
 * @file    mem_intf.h
 * @brief   This file contains all the functions prototypes for the mem_intf.c.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MEM_INTF_H
#define MEM_INTF_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Defination ----------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @addtogroup Memory_Interface_Exported_Functions
 * @{
 */

/**
 * @brief  Coping memory from position to another.
 * @param  ptr_dstntion	: pointer to the destination array where the content is to be copied.
 * @param  ptr_src	: pointer to the source of data to be copied.
 * @param  n 		: the number of bytes to be copied.
 * @retval pointer to destination.
 */
void *ble_memcpy(
	void *ptr_dstntion,
	const void *ptr_src,
	uint16_t n);

/**
 * @brief  Setting a certian block of memory with a certain value.
 * @param  ptr_mem	: pointer to the block of memory to fill.
 * @param  value	: the value to be set. The value is passed as an int.
 * @param  n 		: the number of bytes to be set to the value.
 * @retval pointer to destination.
 */
void *ble_memset(
	void *ptr_mem,
	uint8_t value,
	uint16_t n);

/**
 * @brief  comparing a certain block of memory with another.
 * @param  ptr_dstntion	: pointer to the destination array where the content is to be compared.
 * @param  ptr_src	: pointer to the source of data to be compared.
 * @param  n 		: the number of bytes to be compared.
 * @retval  	< 0 ptr_dstntion is less than ptr_src.
 * 		> 0 ptr_src is less than ptr_dstntion.
 * 		  0 ptr_dstntion is equal to ptr_src.
 */
int8_t ble_memcmp(
	const void *ptr_dstntion,
	const void *ptr_src,
	uint16_t n);

/**
 * @}
 */
#endif /* MEM_INTF_H */

/*****END OF FILE****/
