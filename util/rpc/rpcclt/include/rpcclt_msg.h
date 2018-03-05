/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpcclt_msg.h
*
* @purpose   RPC client messaging header file.
*
* @component rpcclt
*
* @comments
*
* @create    10-Sep-2011
*
* @end
*
**********************************************************************/

#ifndef _RPCCLT_MSG_H
#define _RPCCLT_MSG_H

#include "rpc_common.h"

/* init the operational msg level for debug logging (0=off)
 *
 * - activates the various RPCCLT_MSGLVL_x values at or
 *     below this setting
 *
 * - can be modified during run-time by setting a designated
 *     configuration parameter to the desired message level,
 *     for example:
 *
 *          debugMsgLvl 4
 */
#define RPCCLT_DEBUG_MSGLVL_INIT        1             /* change as needed (normally 0 or 1) */

#define RPCCLT_FD_UNINIT                (FILE *)-1
#define RPCCLT_SOCK_UNINIT              -1
#define RPCCLT_SOCK_WAIT                30            /* seconds */
#define RPCCLT_SOCK_RECV_TIMEOUT        7             /* seconds */

#define RPCCLT_SOCK_CLI_TMPL            "/tmp/fpc.%05d"   /* must be exactly 14 chars (per Stevens) */
#define RPCCLT_SOCK_SRV_TMPL            "/tmp/fps.%05d"   /* must be exactly 14 chars (per Stevens) */

#define RPCCLT_LOG_DATESIZE             255

/* shared memory header structure */
typedef struct
{
  int  maxparms;                             /* maximum number of parms in this msg hdr */
  int  numparms;                             /* actual number of parms in this msg */
  int  maxdata;                              /* total bytes of parm data in shared mem */
  int  useddata;                             /* number of bytes of parm data in use */
  int  parmoffset[RPC_DEVMSG_PARMS_MAX];  /* parameter offsets in msg data */
} rpccltShmemHdr_t;

/* shared memory data structure template */
typedef struct
{
  rpccltShmemHdr_t   hdr;               /* fixed-size shared memory header */
  uint8_t            parmDatastart;     /* first byte of parameter data area */
} rpccltShmem_t;

/* NOTE: for shared memory parm data overlay structure, use rpcParmEnt_t directly */

typedef enum
{
  RPCCLT_DEVMSG_FLG_NONE                = 0,
  RPCCLT_DEVMSG_FLG_NOWAIT              = (1 << 0)    /* don't wait for reply msg */
} rpccltDevmsgFlg_e;

/* container struct for device messages */
typedef struct rpccltDevmsg_s
{
  rpccltDevmsgFlg_e  mflags;            /* message processing flags */
  int32_t            mlen;              /* length of msg contents (incl any null terminator) */
  int32_t            bufsiz;            /* total size of msg buffer (don't -1 when setting) */
  int8_t            *bufptr;            /* ptr to msg buffer */
} rpccltDevmsg_t;

/* reads config file to obtain desired debug message level */
extern int rpccltDebugMsglvlCfgRead(void);

/* updates current debug message level from user-configured value */
extern void rpccltDebugMsglvlUpdate(void);

/* gets debug message level */
extern int rpccltDebugMsglvlGet(void);

/* sets debug message level */
extern void rpccltDebugMsglvlSet(int msglvl);

/* open the log file */
extern void rpccltDebugLogFileOpen(void);

/* close the log file */
extern void rpccltDebugLogFileClose(void);

/* send a message to switchdrvr device and optionally wait for reply
 *
 * returns:
 *    0 - success (including send with NOWAIT option)
 *   -1 - error, message not sent
 *   -2 - error, message sent but response not received
 */
extern int rpccltDevmsgSend(rpccltDevmsg_t *msg);

/* create a shared memory area (if one does not already exist) */
extern int rpccltShmemCreate(void);

/* delete the shared memory area (if one exists) */
extern int rpccltShmemDelete(void);

/* (re)initialize shared memory area for message parm data usage */
extern void rpccltShmemParmInit(void);

/* allocate parm data area from shared memory block */
extern rpcParmEnt_t *rpccltShmemParmAreaGet(int parmlen);

/* return an offset relative to the shared memory base address */
extern int rpccltShmemAddrNormalize(int8_t *shmaddr);

/* return an absolute pointer to the shared memory address for this client */
extern int8_t *rpccltShmemAddrAbsolute(int relOffset);

#endif /* _RPCCLT_MSG_H */
