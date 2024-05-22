/** @file st_mac_802_1584_core_config.h
 *
 * @brief Exports the configurable parameters of MAC PHY
 *
 */

#ifndef MAC_STACK_CONFIGURATION_H
#define MAC_STACK_CONFIGURATION_H

#include "stdint.h" // Must be retrieve from common type header file

/**************************** Public Macro Definitions ***********************/


/** @brief  Defines the size of small buffer */
#define g_SMALL_BUFFER_SIZE_c  ( (uint8_t)60 )

/** @brief  Defines the size of large buffer */
#define g_LARGE_BUFFER_SIZE_c  ( (uint8_t)190 )

/** @brief  Defines the number of small buffer */
#define g_TOTAL_NUMBER_OF_SMALL_BUFFERS_c  ( (uint8_t)0 )

/** @brief Default value for channels supported */
#define  m_DEFAULT_PHY_CHANNELS_SUPPORTED_c           (0x07FFF800)

/** @brief  Defines the number of large buffer */
#define g_TOTAL_NUMBER_OF_LARGE_BUFFERS_c  ( (uint8_t)15 )

/** @brief  Defines the number of total buffer */
#define g_TOTAL_NUMBER_OF_BUFFERS_c \
   (uint8_t)( g_TOTAL_NUMBER_OF_SMALL_BUFFERS_c + g_TOTAL_NUMBER_OF_LARGE_BUFFERS_c )

#define TIMESTAMP_LEN sizeof(uint64_t)
 
#define QUEUE_MAX_LEN         10              // Define QUEUE_MAX_LEN  
     
#define  CCA_THRESHOLD        -75
     
/** @brief Internal Incoming mac Command ID */

#define ST_MAC_ASSOCIATION_REQ_INT_CMD_ID     		0x00
#define ST_MAC_DISASSOCIATION_REQ_INT_CMD_ID  		0x01
#define ST_MAC_GET_REQ_INT_CMD_ID             		0x02
#define ST_MAC_RESET_REQ_INT_CMD_ID           		0x03
#define ST_MAC_RX_ENABLE_REQ_INT_CMD_ID       		0x04
#define ST_MAC_SCAN_REQ_INT_CMD_ID            		0x05
#define ST_MAC_SET_REQ_INT_CMD_ID             		0x06
#define ST_MAC_START_REQ_INT_CMD_ID           		0x07
#define ST_MAC_POLL_REQ_INT_CMD_ID            		0x08
#define ST_MAC_ASSOCIATE_RES_INT_CMD_ID       		0x09
#define ST_MAC_ORPHAN_RES_INT_CMD_ID          		0x0A
#define ST_MAC_DATA_REQ_INT_CMD_ID            		0x0B
#define ST_MAC_PURGE_REQ_INT_CMD_ID           		0x0C
#define ST_MAC_BEACON_REQ_INT_CMD_ID          		0x0D 
#define ST_MAC_GET_PWR_INFO_TABLE_REQ_INT_CMD_ID        0x0E
#define ST_MAC_SET_PWR_INFO_TABLE_REQ_INT_CMD_ID        0x0F  
     
#define ST_MAC_MAX_INT_IN_CMD_ID              		0x10  

/** @brief Internal outgoing mac Command ID */
     
#define ST_MAC_RESET_CNF_INT_MSG_ID           		0x00
#define ST_MAC_SET_CNF_INT_MSG_ID             		0x01  
#define ST_MAC_SCAN_CNF_INT_MSG_ID            		0x02
#define ST_MAC_BEACON_IND_INT_MSG_ID          		0x03
#define ST_MAC_START_CNF_INT_MSG_ID           		0x04
#define ST_MAC_ASSOCIATION_IND_INT_MSG_ID     		0x05
#define ST_MAC_GET_CNF_INT_MSG_ID             		0x06
#define ST_MAC_DATA_CNF_INT_MSG_ID            		0x07
#define ST_MAC_DATA_IND_INT_MSG_ID            		0x08
#define ST_MAC_ASSOCIATION_CNF_INT_MSG_ID     		0x09
#define ST_MAC_COM_STATUS_IND_INT_MSG_ID      		0x0A
#define ST_MAC_POLL_CNF_INT_MSG_ID            		0x0B
#define ST_MAC_POLL_IND_INT_MSG_ID            		0x0C     
#define ST_MAC_PURGE_CNF_INT_MSG_ID           		0x0D
#define ST_MAC_ORPHAN_IND_INT_MSG_ID          		0x0E
#define ST_MAC_RX_ENABLE_CNF_INT_CMD_ID       		0x0F
#define ST_MAC_DISASSOCIATION_CNF_INT_CMD_ID  		0x10
#define ST_MAC_DISASSOCIATION_IND_INT_CMD_ID  		0x11  
#define ST_MAC_BEACONREQ_IND_INT_CMD_ID       		0x12
#define ST_MAC_BEACON_CNF_INT_MSG_ID          		0x13
#define ST_MAC_GET_PWR_INFO_TABLE_CNF_INT_CMD_ID        0x14
#define ST_MAC_SET_PWR_INFO_TABLE_CNF_INT_CMD_ID        0x15
#define ST_MAC_SYNC_LOSS_IND_INT_MSG_ID                 0x16
     
     
    

     
     

     


/*------------------------ Security Related Macros ------------------------*/

/**************************** Public Type Definitions ************************/

/* None */

/**************************** Public Enum Definitions ************************/

/* None */

/**************************** Public Variable Declaration ********************/

extern uint8_t ga_Heap[]; /* Common Buffer pool holding the buffers */
extern uint8_t ga_Buffer_Link_Array[]; /* link array for queue management*/

/**************************** Public Constant Declaration ********************/

/** @brief  Interface to access m_SMALL_BUFFER_SIZE_c from lib */
extern const uint8_t g_Small_Buffer_Size_c;

/** @brief  Interface to access m_LARGE_BUFFER_SIZE_c from lib */
extern const uint8_t g_Large_Buffer_Size_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_SMALL_BUFFERS_c from lib */
extern const uint8_t g_Total_Number_Of_Small_Buffers_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_LARGE_BUFFERS_c from lib */
extern const uint8_t g_Total_Number_Of_Large_Buffers_c;

/** @brief  Interface to access m_TOTAL_NUMBER_OF_BUFFERS_c from lib */
extern const uint8_t g_Total_Number_Of_Buffers_c;

/******************************************************************************/

/******************************************************************************/


#endif  /*MAC_STACK_CONFIGURATION_H */
