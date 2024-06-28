/*$Id: //dwh/bluetooth/DWC_ble154combo/firmware/rel/1.30a-SOW04PatchV2/firmware/public_inc/power_table.h#1 $*/
/**
 ******************************************************************************
 * @file    power_table.h
 * @brief   This file contains APIs prototypes related to configuring the used power table and the external power amplifier (EPA) parameters
 ******************************************************************************
 */


#ifndef POWER_TABLE_H_
#define POWER_TABLE_H_

#include "common_types.h"

/* Private typedef -----------------------------------------------------------*/

typedef uint8_t (*epa_enable_cb_t)(uint8_t epa_enable);

/**
 * @brief Structure of one element of the power table.
 */
typedef struct {
const uint8_t vddh_pa;			/* VDDHPA supply volatge level */
const uint8_t internal_pa_code; /* Internal PA code */
const uint8_t epa_bypass; 		/* External PA Bypass 1: enabled, 0 : disabled */
const int8_t tx_pwr; 			/* TX Power level in dBm. */
} power_table_entry;

/**
 * @brief Structure of the power table ID.
 */
typedef struct _power_table_id_t{
	const power_table_entry *ptr_tx_power_table;	/* Pointer to the TX_Power table corresponding to the associated "power_table_id". */
	const uint8_t tx_power_levels_count;			/* Number of TX_Power levels in the selected power table. */
	const uint8_t g_vdd_ldo_value_1; 				/* Value to be set in PHY register (address: 0x63) to select the TX_Power mode */
	const uint8_t g_vdd_ldo_value_2;				/* Value to be set in PHY register (address: 0xEA) to select the TX_Power mode */
	const uint8_t power_table_id;					/* TX_Power table ID */
} power_table_id_t;


extern const power_table_id_t ll_tx_power_tables[];
extern const uint8_t num_of_supported_power_tables;



/* APIs ----------------------------------------------------------------------*/
/** @ingroup power_control_functions
 * @{
 */
/**
 * @brief	Used to initialize the EPA parameters. Called during the initialization.
 * 			Note: If an EPA is in use, then a valid callback function shall be passed to LL FW, otherwise the LL FW returns error status.
 *
 * @param  use_epa : [in] 1: External PA exists. 0: There is no External PA.
 * @param  cbk	   : [in] Callback function to actually enable and disable the EPA.
 *
 * @retval	Status : FW returns an error code, in case:
 * 					 1. "use_epa" has wrong value
 * 					 2. An EPA is in use and its enable callback function is set to NULL.
 */
uint8_t ll_tx_pwr_if_epa_init(uint8_t use_epa, epa_enable_cb_t cbk);

/**
 * @brief Used to specify the used power table and its size based on the selected power mode.
 *
 * @param  tx_power_mode : [in] Parameter indicating the selected TX_Power mode
 *
 * @retval Status 		 : 0: SUCCESS. Otherwise: Error code.
 */
uint8_t ll_tx_pwr_if_select_tx_power_mode(uint8_t tx_power_table_id);

/**
 * @}
 */

#endif /* POWER_TABLE_H_ */
