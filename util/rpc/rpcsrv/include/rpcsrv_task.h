/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename   rpcsrv_task.h
*
* @purpose    RPC Server definitions.
*
* @component  rpcsrv
*
* @comments
*
* @create     19-Sep-2010
*
* @end
*
*********************************************************************/


#ifndef _RPCSRV_TASK_H
#define _RPCSRV_TASK_H

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include "ofdpa_datatypes.h"
#include "rpccom_support.h"
#include "rpc_common.h"

/* number of RPC server task startup args supported
 *
 * the following set of startup arguments are expected
 * in the argv[] array:
 *
 *  0: process instance number
 *  1: RPC server socket id
 */
#define RPCSRV_TASK_START_ARGS          2

/* RPC server task name */
#define RPCSRV_TASK_NAME_TMPL           "tRpcsrv.%05d"
#define RPCSRV_TASK_NAME_LEN_MAX        16

/* define well-known Unix-domain server socket address */
#define RPCSRV_SOCK_SRV_TMPL            "/tmp/fps.%05d"   /* must be 14 chars (per Stevens) */

/* define access permissions 0777 -> rwxrwxrwx
 * (allows Lighttpd running as special user to access server socket)
 */
#define RPCSRV_SOCK_SRV_MODE            (S_IRWXU | S_IRWXG | S_IRWXO)

#define RPCSRV_SOCK_UNINIT              (-1)
#define RPCSRV_SOCK_RETRY_WAIT          5       /* seconds */
#define RPCSRV_SOCK_MSG_MAX             sizeof(rpccomDevmsg_t)

#define RPCSRV_LOGMSG_THRESH            100     /* modulus for reporting repeat log msgs */

/* task statistics */
typedef struct
{
  uint32_t     msendok;                        /* sent msg success count */
  uint32_t     mrecvok;                        /* received msg success count */
  uint32_t     msendfail;                      /* failed msg send attempts */
  uint32_t     mrecvfail;                      /* failed msg recv attempts */
  uint32_t     mrecvtrunc;                     /* truncated recv msgs (not processed) */
  uint32_t     mrecvprocerr;                   /* recv msg processing error (not processed) */
  uint32_t     msockfail;                      /* failed socket creation attempts */
  uint32_t     mbindfail;                      /* failed socket bind attempts */
  uint32_t     mpermfail;                      /* failed socket permission change attempts */
} rpcsrvTaskStats_t;

/* task control fields */
typedef struct
{
  int                 inst;             /* task instance id */
  int                 sockid;           /* server socket id */
  int                 sockfd;           /* server socket file descriptor */
  BOOL                isbound;          /* flag: socket is bound to server address */
  rpcsrvTaskStats_t  *pstats;           /* ptr to task internal statistics */
  int                 srvlen;           /* server socket address struct len */
  struct sockaddr_un  srvaddr;          /* server socket address struct */
} rpcsrvTaskCtrl_t;


/* public data */
extern rpcsrvTaskStats_t rpcsrvTaskStats_g;

/* rpcsrv_sockmap.c */
extern OFDPA_ERROR_t rpcsrvSockmapDump(void);

/* rpcsrv_task.c */
extern void rpcsrvTask(void *arg);
extern OFDPA_ERROR_t rpcsrvTaskStatsDump(void);

#endif  /* _RPCSRV_TASK_H */
