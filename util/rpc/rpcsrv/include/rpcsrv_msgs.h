/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename   rpcsrv_msgs.h
*
* @purpose    RPC Server socket message handler header fie.
*
* @component  rpcsrv
*
* @comments   None
*
* @create     19-Sep-2010
*
* @end
*
*********************************************************************/


#ifndef _RPCSRV_MSGS_H
#define _RPCSRV_MSGS_H

#include <sys/un.h>
#include <stdint.h>
#include "ofdpa_datatypes.h"
#include "rpcsrv_support.h"

extern OFDPA_ERROR_t rpcsrvMsgRxProcess(struct sockaddr_un *pcliaddr,
                                        uint8_t *buf, int bufsiz,
                                        int *pmlen);
extern void rpcsrvMsgApiRxProcess(RPCCOM_DEVMSG_GRP_t msgGrp, rpcsrvFtableEnt_t **ftable,
                                  uint8_t *buf, int bufsiz, int *pmlen);
extern void rpcsrvMsgOfdpaRxProcess(uint8_t *buf, int bufsiz, int *pmlen);

#endif  /* _RPCSRV_MSGS_H */
