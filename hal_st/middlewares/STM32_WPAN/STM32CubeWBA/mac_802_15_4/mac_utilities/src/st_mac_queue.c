/**
  ******************************************************************************
  * @file    stmac_queue.c
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


/**************************** Includes ****************************************/

#include <string.h>

#include "st_mac_queue.h"
#include "st_mac_buffer_management.h"
    
#include "st_mac_802_15_4_core_config.h"

#include "mac_temporary.h"


/**************************** Private Macro Definitions ***********************/



/**************************** Private type Definitions ************************/

/* None */

/**************************** Private Enum Definitions ************************/

/* None */

/**************************** Private Variable Definitions ********************/

/* None */

/**************************** Public Variable Definitions *********************/

/* None */

/**************************** Private Constant Definitions ********************/

/* None */

/**************************** Public Constant Definitions *********************/

/* None */

/**************************** Private Function Prototypes *********************/

/* None */

/**************************** Public Functions Definitions ********************/


st_mac_queue_t* queueOnwer[g_TOTAL_NUMBER_OF_BUFFERS_c];


void queueMgmt_Init ( st_mac_queue_t* pQueueName, uint8_t queueSize )
{
  /* Initializing the head and tail of the queue to unkonwn values */
  pQueueName->head = pQueueName->tail = QUEUE_UNKNOWN_LINK;

  /* Initializing the total capacity of the queue to the queue size*/
  pQueueName->total_capacity = queueSize;

  /* Assigning the current capacity of the queue to zero */
  pQueueName->current_capacity = QUEUE_EMPTY;
}


void queueMgmt_Reset ( st_mac_queue_t* pQueueName  ) {
  if( NULL != pQueueName) {
    /* Reset the head and tail of the queue to unkonwn values */
    pQueueName->head = pQueueName->tail = QUEUE_UNKNOWN_LINK;
    /* Reset the current capacity of the queue to zero */
    pQueueName->current_capacity = QUEUE_EMPTY;
  }
}

/******************************************************************************/

uint8_t queueMgmt_enqueue ( st_mac_queue_t* pQueueName, uint8_t bufferId ) {
  uint8_t enqueue_status = QUEUE_FULL;

  ENTER_CRITICAL_REGION();

  if (QUEUE_EMPTY == pQueueName->current_capacity) { /* Checking if there is any buffer id in the queue */
    /* The head and the tail point to the same id when enqueue is done for the first time */
    pQueueName->head = pQueueName->tail = bufferId;
    queueOnwer[bufferId] = pQueueName;
    /* The next link is pointing to unknown when only one buffer id is enqueued into the queue */
    ga_Buffer_Link_Array[bufferId] = QUEUE_UNKNOWN_LINK;
    pQueueName->current_capacity++;
    enqueue_status = QUEUE_SUCCESS;
  } else if (pQueueName->total_capacity == pQueueName->current_capacity) { /* Checking if the queue is full */
    enqueue_status = QUEUE_FULL;

  } else { /* If its not the first buffer and the queue is not full */
    ga_Buffer_Link_Array[pQueueName->tail] = bufferId;  /* The tail next will point to the new buffer id */
    queueOnwer[bufferId] = pQueueName;

    pQueueName->tail = bufferId; /* The tail points to the new buffer id */

    ga_Buffer_Link_Array[pQueueName->tail] = QUEUE_UNKNOWN_LINK; /* The new tail next will point to a unknown link */
    pQueueName->current_capacity++;
    enqueue_status = QUEUE_SUCCESS;
  }
  /* Incrementing the queue capacity */

  LEAVE_CRITICAL_REGION();
  return enqueue_status;
}

/******************************************************************************/

uint8_t queueMgmt_dequeue ( st_mac_queue_t* pQueueName ) {

  uint8_t dequeue_id = QUEUE_FAIL;   /* Assigning the current buffer id to a local variable to be returned if
  the queue is not empty */
  ENTER_CRITICAL_REGION();

  uint8_t bufferId = pQueueName->head;

  if (QUEUE_EMPTY != pQueueName->current_capacity) { /* Checkif the current queue has not buffer id's */

    pQueueName->current_capacity--; /* Decrementing the current number of buffers held by the queue */

    if (QUEUE_EMPTY == pQueueName->current_capacity) { /* Checking if there are not buffer id's in the queue */
      pQueueName->head = pQueueName->tail = QUEUE_UNKNOWN_LINK;/* If the queue has no buffer id's head and tail will point to unknown link */
    } else {
      /* Making the head point to the next buffer id in the queue */
      pQueueName->head = ga_Buffer_Link_Array[pQueueName->head];
    }
    queueOnwer[bufferId] = NULL;
    dequeue_id = bufferId;
  } else { /* the current queue has not buffer id's */
    dequeue_id = QUEUE_FAIL;
  }

  LEAVE_CRITICAL_REGION();
  return dequeue_id;
}

/******************************************************************************/

uint8_t queueMgmt_dequeueSpecBuffer ( st_mac_queue_t* pQueueName, uint8_t bufferId ) {
  uint8_t prev;
  uint8_t current;

  uint8_t dequeue_specbuff_status = QUEUE_FAIL;

  ENTER_CRITICAL_REGION();
  /* Variables holding the queue head */
  current = prev = pQueueName->head;

  /* Checking if there is any buffer id in the queue */
  if (QUEUE_EMPTY != pQueueName->current_capacity) {

    /* Loop through the queue starting from the head to tail till getting the
    specified buffer id */
    while ((current != bufferId) && (current != 0xFF)) {
      /* Making the variable to hold the current buffer id */
      prev = current;
      /* Making the variable to hold the next buffer id in the queue */
      current = ga_Buffer_Link_Array[current];
    }
    if (current == 0xFF) {
      /* Reached the queue tail without finding the buffer id, return failure */
      dequeue_specbuff_status = QUEUE_FAIL;
    } else {
      /* Decrementing the current number of buffers held by the queue */
      pQueueName->current_capacity--;
      queueOnwer[bufferId] = NULL;

      if (current == pQueueName->head) {                            /*If buffer id is present in the Queue as head element */

        /* Making the head point to the next buffer id in the queue */
        pQueueName->head = ga_Buffer_Link_Array[pQueueName->head];
        if (!(pQueueName->current_capacity)) { /* Checking if there is any buffer id in the queue */
          /* If the queue has no buffer id's tail will point to unknown link */
          pQueueName->tail = QUEUE_UNKNOWN_LINK;
        }
        dequeue_specbuff_status = QUEUE_HEAD;

      } else if (current == pQueueName->tail) {                   /* If buffer id is present in the Queue as tail element */

        ga_Buffer_Link_Array[prev] = QUEUE_UNKNOWN_LINK; /* Buffer link array at prev index was pointing to current (buffer ID),
        make it to point to FF */
        pQueueName->tail = prev;/* Making the tail point to last buffer id in the queue */
        dequeue_specbuff_status =  QUEUE_SUCCESS;
      }  else {                                                   /* The element is in between the head and tail in the queue */
        /* Making the buffer link array to hold the next link */
        ga_Buffer_Link_Array[prev] = ga_Buffer_Link_Array[current];
        dequeue_specbuff_status =  QUEUE_SUCCESS;
      }
    }
  } else { /* Queue is empty - buffer ID can't be retrieve from */
    dequeue_specbuff_status = QUEUE_FAIL;
  }
  LEAVE_CRITICAL_REGION();
  return dequeue_specbuff_status;
}

uint8_t queueMgmt_isQueueFull (st_mac_queue_t* pQueueName) {
  uint8_t is_queue_status = QUEUE_FALSE;
  ENTER_CRITICAL_REGION();
  if (pQueueName->current_capacity == pQueueName->total_capacity) {
    is_queue_status =  QUEUE_TRUE;
  }
  LEAVE_CRITICAL_REGION();
  return is_queue_status;
}


void queueMgmt_releaseBufferFromOwner(uint8_t bufferId) {
  if (queueOnwer[bufferId] != NULL) {
    queueMgmt_dequeueSpecBuffer(queueOnwer[bufferId],bufferId);
    queueOnwer[bufferId] = NULL;
  }
}

void queueMgmt_resetQueueOwner( void ) {
  memset(queueOnwer,(int) NULL, g_TOTAL_NUMBER_OF_BUFFERS_c);
}

st_mac_queue_t * queueMgmt_getQueue(uint8_t bufferId) {
  return queueOnwer[bufferId];
}


void queueMgmt_cleanUp( st_mac_queue_t* pQueue ) {
  Buffer_Id_t bufferId;

  /* Free up one by one all the buffers from the queue */
  while (Is_Msg_Available(*pQueue)) {
    bufferId = Dequeue(*pQueue);

    if (INVALID_BUFFER_ID != bufferId) {
      /* Put back the buffer in the Buffer management's free buffer queue */
      buffMgmt_freeBuffer(bufferId, 129);
    }
  }
}


/**************************** Private Function Definitions ********************/

/* None */