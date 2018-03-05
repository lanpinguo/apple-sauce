/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpcclt_api.h
*
* @purpose   RPC client API header file.
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

#ifndef _RPCCLT_API_H
#define _RPCCLT_API_H

#include <stdio.h>
#include <sys/types.h>
#include "rpc_common.h"

/* number of startup args supported
 *
 * the following set of startup arguments are expected
 * in the argv[] array:
 *
 *  0: requesting process name
 *  1: process instance number
 *  2: RPC client socket id
 *  3: RPC server socket id
 *  4: log file name (fully-qualified path) - or NULL ptr if not used
 *  5: config file name (fully-qualified path) - or NULL ptr if not used
 *  6: config parm message level token name - or NULL ptr if not used
 *
 * the message level can be modified at any time by writing to the specified
 * config file the token string followed by the message level value with a
 * single space in between, for example:
 *
 *      debugMsgLvl 4
 *
 */
typedef enum
{
  RPCCLT_START_ARGS_REQ_PROC         = 0,
  RPCCLT_START_ARGS_INST             = 1,
  RPCCLT_START_ARGS_SOCKID           = 2,
  RPCCLT_START_ARGS_SRVRSOCKID       = 3,
  RPCCLT_START_ARGS_LOGFILE          = 4,
  RPCCLT_START_ARGS_CFGFILE          = 5,
  RPCCLT_START_ARGS_CFGPARM          = 6,
  RPCCLT_START_ARGS_TOTAL                          /* total number of RPC client start args */
} RPCCLT_START_ARGS_t;

/* RPC return codes */
typedef enum
{
  RPCCLT_RC_OK                       = 0,
  RPCCLT_RC_ERR                      = -1,
  RPCCLT_RC_ERR2                     = -2,         /* special use -- function specific */
  RPCCLT_RC_TIMEOUT                  = -3          /* timeout waiting for action to complete */
} RPCCLT_RC_t;

/* debug message levels supported (for logprintf, etc.)
 * - higher numbers represent increasing verbosity in the output log
 * - the numeric values do not necessarily need to be densely-packed,
 *     just increasing (e.g. 0, 10, 20, 30...)
 */
#define RPCCLT_MSGLVL_ALWAYS         0             /* output always emitted (no way to inhibit) */
#define RPCCLT_MSGLVL_ACTIVE         1             /* msgs to be seen whenever output is active (enabled) */
#define RPCCLT_MSGLVL_EXCEPT         2             /* more exceptions/errors without extra clutter */
#define RPCCLT_MSGLVL_BASIC          3             /* basic informational msgs */
#define RPCCLT_MSGLVL_DETAIL         4             /* more detailed information */
#define RPCCLT_MSGLVL_MSGDATA        5             /* message data contents, etc. */
#define RPCCLT_MSGLVL_VERBOSE        6             /* everything else (very verbose!) */

#define RPCCLT_SOCKNAME_MAX          16            /* max 14 chars allowed in socket name path */
#define RPCCLT_FILENAME_MAX          128           /* max length of filename path (log file, etc.) */
#define RPCCLT_CFGPARM_MAX           128           /* max length of config parm string */

#define RPCCLT_SHMID_UNALLOC         -10           /* init value indicating shared memory area unallocated */

/* internal debug counters */
typedef struct rpccltCtrs_s
{
  uint32_t msendok;                    /* sent msg success count */
  uint32_t mrecvok;                    /* received msg success count */
  uint32_t mnowait;                    /* nowait option usage count */
  uint32_t msendfail;                  /* failed msg send attempts */
  uint32_t mrecvfail;                  /* failed msg recv attempts */
  uint32_t mrecvseqerr;                /* msg recv sequence number mismatch (not processed) */
  uint32_t mrecvtimeout;               /* msg recv timed out (not processed) */
  uint32_t mrecvtrunc;                 /* truncated recv msgs (not processed) */
  uint32_t mconnfail;                  /* failed connection attempts */
} rpccltCtrs_t;

/* proc globals for managing resources, etc. */
typedef struct rpccltProcParms_s
{
  int inst;                          /* specific instance identifier for the invoking process */
  pid_t pid;                         /* process id of self */
  uint32_t msgseq;                   /* message sequence number (init to current system time, 0 is valid value) */
  int singlesrvr;                    /* single server mode (normally off) */
  int debugMsgLvl;                   /* debug flag -- turn on for extra debug msgs (normally 0) */
  FILE *logfd;                       /* log file descriptor (log file shared among all procs) */

#if RPC_USE_SHMEM
  /* shared memory variables */
  int shmid;                         /* shared memory segment id */
  int shmsize;                       /* shared memory segment allocation size */
  int8_t *shmdata;                   /* pointer to shared memory data area */
#endif /* RPC_USE_SHMEM */

  /* socket information */
  int8_t cliaddr[RPCCLT_SOCKNAME_MAX]; /* name of CLI socket file */
  int sockid;                        /* RPC client-side socket id */
  int srvrsockid;                    /* RPC server-side socket id */
  int sockfd;                        /* Unix-domain datagram socket to switchdrvr device */
  int connected;                     /* socket connection established with server */
  int clienttimout;                  /* how long to wait for select reply from server */
  rpccltCtrs_t ctrs;                 /* debug counters */

  /* startup parameters */
  int8_t logfile[RPCCLT_FILENAME_MAX+1];         /* log file name */
  int8_t cfgfile[RPCCLT_FILENAME_MAX+1];         /* config file name */
  int8_t cfgmsglvlname[RPCCLT_CFGPARM_MAX+1];    /* debug message level config parm token name */

} rpccltProcParms_t;

/* initialize communication path with switchdrvr device
 *
 * the following set of startup arguments are expected
 * in the argv[] array:
 *
 *  0: requesting process name
 *  1: process instance number
 *  2: RPC client socket id
 *  3: RPC server socket id
 *  4: log file name (fully-qualified path) - or NULL ptr if not used
 *  5: config file name (fully-qualified path) - or NULL ptr if not used
 *  6: config parm message level token name - or NULL ptr if not used
 *
 * the message level can be modified at any time by writing to the specified
 * config file the token string followed by the message level value with a
 * single space in between, for example:
 *
 * debugMsgLvl 4
 *
 */
extern int rpccltCommSetup(int argc, char *argv[]);

/* de-initialize communication path with switchdrvr device */
extern int rpccltCommTeardown(void);

/* output a message to the RPC client process log file */
extern void rpccltLogprintf(rpccltProcParms_t *proc, int msglvl, int8_t *fmt, ...);

/* retrieves proc ptr */
extern rpccltProcParms_t *rpccltProcPtrGet(void);

/* set rpc timeout value */
extern void rpccltTimeoutSet(int val);

#endif /* _RPCCLT_API_H */
