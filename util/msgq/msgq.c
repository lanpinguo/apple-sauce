/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2000-2014
*
**********************************************************************
*
* @filename   msgq.c
*
* @purpose    File contains all the api functions for a generic message
*             queue
*
* @component  message queue
*
* @comments
*
* @create     10/30/2014
*
* @end
*
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "msgq.h"
#include "ofdpa_util.h"

typedef struct msgqDesc_s
{
  void          *qSem;
  char           nameString[MSGQ_NAME_SIZE];
  unsigned int   headId;
  unsigned int   tailId;
  unsigned int   max_cnt;
  unsigned int   msg_cnt;
  unsigned int   msgSize;
  void          *msg;
} msgqDesc_t;

/*
 * Function:
 *      msgqCreate
 * Purpose:
 *      create a message queue.
 * Parameters:
 *      name      - A string that fits into a buffer of length
 *                  MSGQ_NAME_SIZE, including terminating NULL
 *                  to identify the queue for debugging 
 *      count     - Maximum number of messages the queue can hold
 *      msgSize   - the size in octets of each message
 * Returns:
 *      NULL or the pointer
 * Notes:
 */
void *msgqCreate(char *name, unsigned int count, unsigned int msgSize)
{
  msgqDesc_t *qDesc;

  qDesc = (msgqDesc_t *)malloc(sizeof(msgqDesc_t));

  if (NULL == qDesc)
  {
    return NULL;
  }

  qDesc->msg = malloc(msgSize * count);
  if (NULL == qDesc->msg)
  {
    free(qDesc);
    return NULL;
  }

  qDesc->qSem = dpaSemCreate(name, DPA_SEM_BINARY, 1);

  strncpy(qDesc->nameString, name, sizeof(qDesc->nameString));
  qDesc->nameString[MSGQ_NAME_SIZE-1] = '\0';

  qDesc->headId = 0;
  qDesc->tailId = 0;
  qDesc->msg_cnt = 0;
  qDesc->max_cnt = count;
  qDesc->msgSize = msgSize;

  memset(qDesc->msg, 0, (msgSize * count));

  return qDesc;
}

/*
 * Function:
 *      msgqDestroy
 * Purpose:
 *      Destroy a message queue
 * Parameters:
 *      msg - The message queue pointer
 * Returns:
 *      None
 * Notes:
 */
int msgqDestroy(void *msgq)
{
  msgqDesc_t *qDesc;

  if (NULL == msgq)
  {
    return -2;
  }

  qDesc = msgq;

  dpaSemTake(qDesc->qSem, DPA_SEM_FOREVER);

  dpaSemDestroy(qDesc->qSem);

  if (NULL != qDesc->msg)
  {
    free(qDesc->msg);
  }
  free(qDesc);

  return 0;
}

/*
 * Function:
 *      msgqSnd
 * Purpose:
 *      Send a information into the message queue
 * Parameters:
 *      msgq    - The message queue handle
 *      buff    - Message data
 * Returns:
 *      -1 if queue is full
 *      -2 if either parameter is NULL
 *       0 if no error
 * Notes:
 */
int msgqSnd(void *msgq, void *buff)
{
  int rc = 0;
  msgqDesc_t *qDesc;
  unsigned int buffPos = 0;

  if (NULL == msgq || NULL == buff)
  {
    return -2;
  }

  qDesc = msgq;

  dpaSemTake(qDesc->qSem, DPA_SEM_FOREVER);

  if (qDesc->max_cnt == qDesc->msg_cnt)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_BASIC, 
                       "Message queue %s is full. msg_cnt = %d\r\n", 
                       qDesc->nameString, qDesc->msg_cnt);
    rc = -1;
  }
  else
  {
    buffPos = qDesc->tailId * qDesc->msgSize;
    memcpy((((char *)qDesc->msg) + buffPos), buff, qDesc->msgSize);

    qDesc->tailId = (qDesc->tailId + qDesc->max_cnt + 1) % qDesc->max_cnt;
    qDesc->msg_cnt++;
  }

  dpaSemGive(qDesc->qSem);

  return(rc);
}

/*
 * Function:
 *      msgqRcv
 * Purpose:
 *      Receive a message from a queue
 * Parameters:
 *      msg    - message queue handle
 *      buff   - storage for message
 * Returns:
 *      -1 if there are no messages in the queue
 *      -2 if either parameter is NULL
 *       0 if no error
 * Notes:
 */
int msgqRcv(void *msgq, void *buff)
{
  unsigned int buffPos = 0;
  int rc = 0;
  msgqDesc_t *qDesc;

  if (NULL == msgq || NULL == buff)
  {
    return -2;
  }

  qDesc = msgq;

  dpaSemTake(qDesc->qSem, DPA_SEM_FOREVER);

  if (0 == qDesc->msg_cnt)
  {
    rc = -1;
  }
  else
  {
    buffPos = qDesc->headId * qDesc->msgSize;
    memcpy(buff, (((char *)qDesc->msg) + buffPos), qDesc->msgSize);

    qDesc->msg_cnt--;

    qDesc->headId = (qDesc->headId + qDesc->max_cnt + 1) % qDesc->max_cnt;
  }

  dpaSemGive(qDesc->qSem);
  return(rc);
}



/*
 * Function:
 *      msgqRcv
 * Purpose:
 *      Receive a message from a queue
 * Parameters:
 *      msg    - message queue handle
 *      buff   - storage for message
 * Returns:
 *      -1 if there are no messages in the queue
 *      -2 if either parameter is NULL
 *       0 if no error
 * Notes:
 */
int msgqCnt(void *msgq, int *count)
{
  unsigned int buffPos = 0;
  int rc = 0;
  msgqDesc_t *qDesc;

  if (NULL == msgq || NULL == count)
  {
    return -2;
  }

  qDesc = msgq;

  dpaSemTake(qDesc->qSem, DPA_SEM_FOREVER);
  *count = qDesc->msg_cnt; 
  dpaSemGive(qDesc->qSem);
  return(rc);
}


/*
 * Function:
 *      msgqFlush
 * Purpose:
 *      Flushes all messages out of the message queue
 * Parameters:
 *      msgq    - Message queue handle
 * Returns:
 *      -1 if NULL pointer passed
 *       0 if successful
 * Notes:
 */
int msgqFlush(void *msgq)
{
  msgqDesc_t *qDesc;

  if (NULL == msgq)
  {
    return -1;
  }

  qDesc = msgq;

  dpaSemTake(qDesc->qSem, DPA_SEM_FOREVER);

  memset(qDesc->msg, 0x0, (qDesc->msgSize * qDesc->msg_cnt));
  qDesc->tailId = 0;
  qDesc->headId = 0;
  qDesc->msg_cnt = 0;

  dpaSemGive(qDesc->qSem);
  return 0;
}

