#ifndef MAC_TEMPORARY_H
#define MAC_TEMPORARY_H


//#include "cmsis_iccarm.h" // Must be remove
#include "cmsis_compiler.h"

#include "string.h"
#include "bsp.h"

#ifndef ASSERT
/*
 * Define Two assertion severity levels HIGH for conditions that requires system reset and LOW for conditions that can be bypassed
 * */
#define SEVERITY_LOW		0
#define SEVERITY_HIGH		1

#if LOGGER_ENABLE
	#define ASSERT(condition, severity)			bsp_assert_log(condition, severity, __func__, __LINE__)
#else
        #define ASSERT(condition, severity)			bsp_assert(condition, severity)
#endif /* LOGGER_ENABLE */
#endif /* ASSERT*/

// Debug Buffer Management 

#define ST_MAC_HANDLE_INCOMING_MAC_CMD 0x00
#define ST_MAC_HANDLE_OUTGOING_MAC_MSG 0x01

#define MAC_WRAPPER_LOG 0x00

/* Maybe we need to change this for WBA2 */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define MAC_MEMSET  ble_memset
#define MAC_MEMCPY  ble_memcpy
#else
#define MAC_MEMCPY  memcpy
#define MAC_MEMSET  memset
#endif

// MUST BE REMOVED WHEN FULL INTEGRATION IS SCHEDULED
#define COMMON_RF_M_BEGIN     do {

#define COMMON_RF_M_END       } while(0)

#ifndef ENTER_CRITICAL_REGION
#define ENTER_CRITICAL_REGION()                   COMMON_RF_M_BEGIN uint32_t _primask = __get_PRIMASK( ); \
                                                                    __disable_irq( )
#endif /* ENTER_CRITICAL_REGION */

#ifndef LEAVE_CRITICAL_REGION
#define LEAVE_CRITICAL_REGION()                   __set_PRIMASK( _primask ); COMMON_RF_M_END
#endif /* LEAVE_CRITICAL_REGION */
                                                                      
                                                                      
 // BYTES UTILS 
                                                                      /* Write bytes to memory macro like function */
#define MAC_WRITE_2_BYTES(pckt,pos,var)     \
	do {\
		((uint8_t *)pckt)[pos] = (uint8_t)(var);\
		((uint8_t *)pckt)[pos + 1] = (uint8_t)(((var) & 0xff00) >> 8);\
	} while(0)

#define MAC_WRITE_3_BYTES(pckt,pos,var)     \
	do {\
		((uint8_t *)pckt)[pos] = (uint8_t)(var);\
		((uint8_t *)pckt)[pos + 1] = (uint8_t) ((var) >> 8);\
                ((uint8_t *)pckt)[pos + 2] = (uint8_t) ((var) >> 16);\
	} while(0)

#define MAC_WRITE_4_BYTES(pckt,pos,var)     \
	do {\
		((uint8_t *)pckt)[pos] = (uint8_t)(var);\
		((uint8_t *)pckt)[pos + 1] = (uint8_t) ((var) >> 8);\
                ((uint8_t *)pckt)[pos + 2] = (uint8_t) ((var) >> 16);\
                ((uint8_t *)pckt)[pos + 3] = (uint8_t) ((var) >> 24);\
	} while(0)
    
#endif /* MAC_TEMPORARY_H */