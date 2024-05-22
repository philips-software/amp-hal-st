/**
  ******************************************************************************
  * @file    st_mac_buffer_management.c
  * @author  MCD-WBL Application Team
  * @brief   Mac Implementation App.
  *          This file provides latest MAC Implementation.
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


/**************************** Includes ***************************************/
#include <stdbool.h> // Must be part of a global type header.

#include "st_mac_802_15_4_core_config.h"
#include "st_mac_802_15_4_types.h"
    
#include "st_mac_queue.h"


#include "mac_temporary.h"


#include "st_mac_buffer_management.h" // Buffer management - to be group in global MAC includes


/**************************** Private Macro Definitions **********************/

#define m_IS_SMALL_BUFFER_c( bufferId ) \
                ( ( ( bufferId ) < g_Total_Number_Of_Small_Buffers_c ) ? 1 : 0 )

/**************************** Private type Definitions ***********************/
typedef struct {
    st_mac_queue_t freeLargeBufferQueue;
    st_mac_queue_t freeSmallBufferQueue;
} buffMgmt_freeBuffersQ_t;

/**************************** Private Enum Definitions ***********************/

/* None */

/**************************** Private Variable Definitions *******************/
static buffMgmt_freeBuffersQ_t m_freeBuffersQ;
static uint8_t m_bufferMgmtState;

/**************************** Public Variable Definitions ********************/


/**************************** Private Constant Definitions *******************/

/* None */

/**************************** Public Constant Definitions ********************/

/* None */

/**************************** Private Function Prototypes ********************/

/* None */

/**************************** Public Functions Definitions *******************/

uint8_t is_available_large_buffer( void ){
return ( (m_freeBuffersQ.freeLargeBufferQueue.current_capacity == 0 )?0:1 );
  }


uint8_t is_available_small_buffer( void ){
return ( ( m_freeBuffersQ.freeSmallBufferQueue.current_capacity == 0 )?0:1 );
  }

void buffMgmt_init ( void )
{
    Buffer_Id_t bufferId = 0;
    /* Initialize free buffer queue for large buffers */
    queueMgmt_Init(&m_freeBuffersQ.freeLargeBufferQueue,
                   g_Total_Number_Of_Large_Buffers_c);

    /* Initialize free buffer queue for small buffers */
    queueMgmt_Init(&m_freeBuffersQ.freeSmallBufferQueue,
                   g_Total_Number_Of_Small_Buffers_c);

    for ( ; bufferId < g_Total_Number_Of_Small_Buffers_c; bufferId++ ) {
        /* Append the buffer to free small buffer queue */
        queueMgmt_enqueue(&m_freeBuffersQ.freeSmallBufferQueue, bufferId);
    }

    for ( ; bufferId < g_Total_Number_Of_Buffers_c; bufferId++ ) {
        /* Append the buffer to free large buffer queue */
        queueMgmt_enqueue(&m_freeBuffersQ.freeLargeBufferQueue, bufferId);

    }

    m_bufferMgmtState = AVAILABLE_BUFFER;
}

void buffMgmt_reset ( void ) {
  

  Buffer_Id_t bufferId = 0;
  
  queueMgmt_Reset(&m_freeBuffersQ.freeLargeBufferQueue);
  queueMgmt_Reset(&m_freeBuffersQ.freeSmallBufferQueue);
  
  for ( ; bufferId < g_Total_Number_Of_Small_Buffers_c; bufferId++ ) {
      /* Append the buffer to free small buffer queue */
      queueMgmt_enqueue(&m_freeBuffersQ.freeSmallBufferQueue, bufferId);
  }

  for (  ; bufferId < g_Total_Number_Of_Buffers_c; bufferId++ ) {
        /* Append the buffer to free large buffer queue */
        queueMgmt_enqueue(&m_freeBuffersQ.freeLargeBufferQueue, bufferId);
  }
        
  m_bufferMgmtState = AVAILABLE_BUFFER;
    
}
/******************************************************************************/

Buffer_Id_t buffMgmt_allocateBuffer ( uint8_t size )
{
  Buffer_Id_t bufferId = NO_AVAILABLE_BUFFER;
  ENTER_CRITICAL_REGION();

  if ( m_bufferMgmtState != NO_AVAILABLE_BUFFER ) {
    if ( size <= g_Large_Buffer_Size_c ) {  /* Allocate buffer only if size requested is less than or equal to  maximum
                                               size that can be allocated */
      if ( size <= g_Small_Buffer_Size_c ) { /* Allocate small buffer if size is less than small buffer size and if
                                                small buffer is available allocate from small buffer pool/ */
        bufferId = queueMgmt_dequeue(&m_freeBuffersQ.freeSmallBufferQueue);
      }
      if (bufferId == (uint8_t) NO_AVAILABLE_BUFFER) { /* If size is greater than small buffer size or no free small buffer is
                                           available, allocate a buffer from large buffer pool if available */
        bufferId = queueMgmt_dequeue(&m_freeBuffersQ.freeLargeBufferQueue);
      }
    } /* size <= g_Large_Buffer_Size_c */
    if ( bufferId == NO_AVAILABLE_BUFFER ) { /* Store Buffer Mngt State */
        m_bufferMgmtState = NO_AVAILABLE_BUFFER;
    }
  }
  LEAVE_CRITICAL_REGION();
  return bufferId;
} /* End Of buffMgmt_allocateBuffer */


/******************************************************************************/
void buffMgmt_freeBuffer ( Buffer_Id_t bufferId, uint8_t location ) {

    ENTER_CRITICAL_REGION();
    if ( bufferId < g_Total_Number_Of_Buffers_c ) {
      queueMgmt_releaseBufferFromOwner(bufferId);
      if (m_IS_SMALL_BUFFER_c( bufferId )) { /* Append the buffer into free small buffer queue */
        queueMgmt_enqueue(&m_freeBuffersQ.freeSmallBufferQueue, bufferId);
      }
      else {/* Append the buffer into free large buffer queue */
        queueMgmt_enqueue(&m_freeBuffersQ.freeLargeBufferQueue, bufferId);
      }
      // Buffer has been released - indicate availability
      m_bufferMgmtState = AVAILABLE_BUFFER;
    }
    else { //mustn't occur : may we have a mem leaks - Buffer no more accessible.
      }
    LEAVE_CRITICAL_REGION();
}

/******************************************************************************/
uint8_t* buffMgmt_getBufferPointer ( Buffer_Id_t bufferId )
{
    uint8_t *p_buffer_pointer;
    uint16_t offset_to_large_buffers;

    if (bufferId >= g_Total_Number_Of_Buffers_c) {
#if ( g_DIAGNOSTICS_CLUSTER_ENABLE_d == 1 )
    mac_ext_PacketBufferAllocateFailuresHandler();
#endif     /* g_DIAGNOSTICS_CLUSTER_ENABLE_d */
        /* If the buffer id is not valid abort operation */
        return g_NULL_c;
    }

    if (m_IS_SMALL_BUFFER_c( bufferId )) {
        /* Evaluate the base address of the buffer */
        p_buffer_pointer = (uint8_t*) ga_Heap + (bufferId
                * g_Small_Buffer_Size_c);
    }
    else {
        /* Evaluate the base address of the buffer */
        offset_to_large_buffers = g_Total_Number_Of_Small_Buffers_c *
            g_Small_Buffer_Size_c;

        p_buffer_pointer = (uint8_t*) ga_Heap +
            (offset_to_large_buffers + (bufferId - g_Total_Number_Of_Small_Buffers_c) *
             g_Large_Buffer_Size_c);
    }
    return p_buffer_pointer;
}

/******************************************************************************/

uint8_t buffMgmt_getNumberofFreeBuffers ( uint8_t bufferSize )
{
    uint8_t freeBuffers;

    if (bufferSize <= g_Small_Buffer_Size_c) {
        freeBuffers = m_freeBuffersQ.freeSmallBufferQueue.current_capacity;
    }
    else {
        freeBuffers = m_freeBuffersQ.freeLargeBufferQueue.current_capacity;
    }
    return freeBuffers;
}

uint8_t buffMgmt_isBufferIdValid (Buffer_Id_t bufferId)
{
    if (bufferId >= g_Total_Number_Of_Buffers_c) {
        return g_FALSE_c;
    }
    return g_TRUE_c;
}

bool IsSmallBuffer(st_mac_queue_t* pQueueName)
{
  return (pQueueName == &m_freeBuffersQ.freeSmallBufferQueue);
}

bool IsLargeBuffer(st_mac_queue_t* pQueueName)
{
  return (pQueueName == &m_freeBuffersQ.freeLargeBufferQueue);
}

/**************************** Private Function Definitions *******************/