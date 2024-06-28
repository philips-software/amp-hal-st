/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    power_table.c
  * @author  MCD Application Team
  * @brief   This file contains supported power tables
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "power_table.h"

/* Private typedef -----------------------------------------------------------*/

/* VDD_LDO values to be written in PHY registers to select the TX_Power mode.
 * g_vdd_ldo_value_1 : is to be set in PHY register address 0x63.
 * g_vdd_ldo_value_2 : is to be set in PHY register address 0xEA. */

typedef enum _vdd_ldo_value_e {
  VDD_LDO_VALUE_1_ID_0  = 0x70,
  VDD_LDO_VALUE_2_ID_0  = 0x00,
} vdd_ldo_value_e;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

const power_table_entry ll_power_table_id_0[] = {
    {0x02, 0x02, 0x01, -20},        /* Actual_Power = -20.4 dBm, Loc_of_power_level -20 dBm */
    {0x02, 0x03, 0x01, -19},        /* Actual_Power = -19.4 dBm, Loc_of_power_level -19 dBm */
    {0x02, 0x04, 0x01, -18},        /* Actual_Power = -18.3 dBm, Loc_of_power_level -18 dBm */
    {0x02, 0x05, 0x01, -17},        /* Actual_Power = -17.3 dBm, Loc_of_power_level -17 dBm */
    {0x02, 0x06, 0x01, -16},        /* Actual_Power = -16.4 dBm, Loc_of_power_level -16 dBm */
    {0x02, 0x07, 0x01, -15},        /* Actual_Power = -15.4 dBm, Loc_of_power_level -15 dBm */
    {0x02, 0x08, 0x01, -14},        /* Actual_Power = -14.3 dBm, Loc_of_power_level -14 dBm */
    {0x02, 0x09, 0x01, -13},        /* Actual_Power = -13.3 dBm, Loc_of_power_level -13 dBm */
    {0x02, 0x0A, 0x01, -12},        /* Actual_Power = -12.3 dBm, Loc_of_power_level -12 dBm */
    {0x02, 0x0B, 0x01, -11},        /* Actual_Power = -11.4 dBm, Loc_of_power_level -11 dBm */
    {0x02, 0x0C, 0x01, -10},        /* Actual_Power = -10.4 dBm, Loc_of_power_level -10 dBm */
    {0x02, 0x0D, 0x01, -9},         /* Actual_Power = -9.5 dBm, Loc_of_power_level -9 dBm */
    {0x02, 0x0E, 0x01, -8},         /* Actual_Power = -8.4 dBm, Loc_of_power_level -8 dBm */
    {0x00, 0x0F, 0x01, -7},         /* Actual_Power = -7.5 dBm, Loc_of_power_level -7 dBm */
    {0x00, 0x10, 0x01, -6},         /* Actual_Power = -6.5 dBm, Loc_of_power_level -6 dBm */
    {0x00, 0x11, 0x01, -5},         /* Actual_Power = -5.4 dBm, Loc_of_power_level -5 dBm */
    {0x00, 0x12, 0x01, -4},         /* Actual_Power = -4.5 dBm, Loc_of_power_level -4 dBm */
    {0x00, 0x13, 0x01, -3},         /* Actual_Power = -3.5 dBm, Loc_of_power_level -3 dBm */
    {0x02, 0x14, 0x01, -2},         /* Actual_Power = -2.4 dBm, Loc_of_power_level -2 dBm */
    {0x00, 0x15, 0x01, -1},         /* Actual_Power = -1.5 dBm, Loc_of_power_level -1 dBm */
    {0x02, 0x16, 0x01, 0},          /* Actual_Power = -0.3 dBm, Loc_of_power_level 0 dBm */
    {0x00, 0x17, 0x01, 1},          /* Actual_Power = 0.9 dBm, Loc_of_power_level 1 dBm */
    {0x00, 0x18, 0x01, 2},          /* Actual_Power = 2.3 dBm, Loc_of_power_level 2 dBm */
    {0x02, 0x18, 0x01, 3},          /* Actual_Power = 2.8 dBm, Loc_of_power_level 3 dBm */
    {0x00, 0x19, 0x01, 4},          /* Actual_Power = 3.9 dBm, Loc_of_power_level 4 dBm */
    {0x02, 0x19, 0x01, 5},          /* Actual_Power = 4.8 dBm, Loc_of_power_level 5 dBm */
    {0x03, 0x19, 0x01, 6},          /* Actual_Power = 5.6 dBm, Loc_of_power_level 6 dBm */

                /* 10 dBm */
    {0x05, 0x19, 0x01, 7},          /* Actual_Power = 6.9 dBm, Loc_of_power_level 7 dBm */
    {0x06, 0x19, 0x01, 8},          /* Actual_Power = 7.5 dBm, Loc_of_power_level 8 dBm */
    {0x08, 0x19, 0x01, 9},          /* Actual_Power = 8.5 dBm, Loc_of_power_level 9 dBm */
    {0x0D, 0x19, 0x01, 10},          /* Actual_Power = 10 dBm, Loc_of_power_level 10 dBm */
};

/* Supported TX_Power tables. */
const power_table_id_t ll_tx_power_tables[] = {
    {ll_power_table_id_0, sizeof(ll_power_table_id_0)/sizeof(ll_power_table_id_0[0]), VDD_LDO_VALUE_1_ID_0, VDD_LDO_VALUE_2_ID_0, 0x00},
};

/* Number of supported TX_Power tables. */
const uint8_t num_of_supported_power_tables = sizeof(ll_tx_power_tables)/sizeof(ll_tx_power_tables[0]);

/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
