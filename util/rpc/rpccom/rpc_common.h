/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpc_common.h
*
* @purpose   RPC common header file.
*
* @component rpcclt, rpcsrv
*
* @comments
*
* @create    19-Sep-2010
*
* @end
*
**********************************************************************/

#ifndef _RPC_COMMON_H
#define _RPC_COMMON_H

#include <stdarg.h>
#include <stdint.h>

#define FD_RPCSRV_SINGLE_SRVR_MODE 0

/* specify whether RPC server-side uses one socket for all external processes or not (normally FALSE) */
#define RPC_SINGLE_SRVR_MODE         FD_RPCSRV_SINGLE_SRVR_MODE

/* define the path where the log files reside on the target */
#define RPC_LOGFILE_PATH             "/tmp/logs"

#define RPC_PARM_DATA_AREA_MAX (128 * 1024)

/* define whether to use shared memory in the build or not */
#define RPC_USE_SHMEM                0             /* define as 1 to use shared memory */

#define RPC_SHMEM_KEY_BASE           13680         /* just some unique base number */
#define RPC_SHMEM_SIZE               RPC_PARM_DATA_AREA_MAX     /* entire shmem area (hdr + data) */
#define RPC_SHMEM_PERMS              0660

#define RPC_DEVMSG_IDX_RETVAL        0       /* return value always in parm index 0 */
#define RPC_DEVMSG_IDX_FIRSTPARM     1       /* first fn parm always in parm index 1 */

#define RPC_DEVMSG_PARMS_MAX         (20 + 1)      /* max parms in a single msg (plus return value) */
#define RPC_DEVMSG_ALIGN_SIZE        sizeof(parmlen_t)  /* data alignment byte boundary */
#define RPC_DEVMSG_PARM_BUF_MAX      260     /* bytes (includes a little extra) */

/* this shared memory threshold is normally set to the
 * parm buffer max value, but may be reduced to force
 * shared memory usage for smaller sized parm data
 */
#define RPC_DEVMSG_PARM_SHMEM_TRIGGER    260 /* bytes */

#define RPC_DEVMSG_PARM_SHMEM_THRESH \
  min(RPC_DEVMSG_PARM_BUF_MAX, RPC_DEVMSG_PARM_SHMEM_TRIGGER)

#define RPC_DEVMSG_PARM_SHMEM_ADDR_NORMALIZE(_saddr, _sbase) \
  ((int8_t *)(_saddr) - (int8_t *)(_sbase))

#if RPC_USE_SHMEM
  /* calculate the maximum msg data area (in bytes) as:
   *   max parms * (align size + parmlen_t size + parm buf max)
   *
   * then round up to the next 1 KB increment
   */
  #define RPC_DEVMSG_DATA_MAX \
    ( (((RPC_DEVMSG_PARMS_MAX * \
        (RPC_DEVMSG_ALIGN_SIZE + sizeof(parmlen_t) + RPC_DEVMSG_PARM_BUF_MAX)) \
       + 0x3ff) & ~0x3ff) \
    )
#else
    /* max size of all RPC parameter data */
  #define RPC_DEVMSG_DATA_MAX        RPC_PARM_DATA_AREA_MAX
#endif /* RPC_USE_SHMEM */

/* align msg data pointer to next boundary */
#define RPC_DEVMSG_PTR_ALIGN(_p) \
  ((uint8_t *) \
    ((((unsigned long)(_p)) + (unsigned long)(RPC_DEVMSG_ALIGN_SIZE - 1)) & \
     (unsigned long)(~(RPC_DEVMSG_ALIGN_SIZE - 1))))

/* buffer descriptor structure initializer definition */
#define RPC_BUFFDESC_NULL_INIT       { 0, (int8_t *)0 }

#ifndef BOOL
typedef enum
{
  FALSE = 0,
  TRUE  = 1,
} BOOL;
#endif

/* socket RPC device message header structure */
typedef struct
{
  uint32_t totLen;                      /* total length of msg contents(incl this field) */
  uint32_t group;                       /* msg group identifier */
  uint32_t type;                        /* msg type identifier (per group) */
  uint32_t seq;                         /* msg send sequence number */
  uint32_t flags;                       /* various message flags (see DEVMSG_FLAGS_t) */
  int32_t  rpcRes;                      /* RPC execution result */
  int32_t  shmemKeyPartial;             /* add this to shmem key base to get key */

  uint32_t maxParms;                    /* maximum number of parms in this msg hdr*/
  uint32_t numParms;                    /* actual number of parms in this msg */
  uint32_t parmOffset[RPC_DEVMSG_PARMS_MAX]; /* parameter offsets in msg data */
} rpcDevmsgHdr_t;

/* specific type for parm length field w/in data */
typedef unsigned int parmlen_t;

/* RPC parameter entry template */
typedef struct
{
  parmlen_t len;                        /* length of parameter data */
  uint8_t   dataStart;                  /* start of parameter data (of len bytes) */
} rpcParmEnt_t;

#endif /* _RPC_COMMON_H */
