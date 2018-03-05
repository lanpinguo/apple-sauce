/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2000-2014
*
**********************************************************************
*
* @filename   msgq.h
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

#ifndef _MSG_Q_H
#define _MSG_Q_H

/* string storage size for msgQ name, terminating NULL included */
#define MSGQ_NAME_SIZE 32

void *msgqCreate(char *name, unsigned int count, unsigned int msgSize);
int msgqDestroy(void *msgq);

int msgqSnd(void *msgq, void *buff);
int msgqRcv(void *msgq, void *buff);
int msgqFlush(void *msgq);
int msgqCnt(void *msgq, int *count);

#endif /* _MSG_Q_H */
