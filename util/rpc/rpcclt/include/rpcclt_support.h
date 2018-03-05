/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpcclt_support.h
*
* @purpose   RPC client wrapper support header file.
*
* @component luaweb
*
* @comments
*
* @end
*
**********************************************************************/

#ifndef _RPCCLT_SUPPORT_H
#define _RPCCLT_SUPPORT_H

#include "ofdpa_datatypes.h"
#include "rpcclt_api.h"
#include "rpcclt_msg.h"
#include "rpccom_support.h"
#include "rpc_common.h"

/* predefined macros to manipulate the output parms mask field */
#define RPCCLT_OUTPARMSMASK_FLAG_GET(_ptr, _pos) \
  ((_ptr)->outParmsMask & (0x80000000 >> (_pos)))
#define RPCCLT_OUTPARMSMASK_FLAG_SET(_ptr, _pos) \
  ((_ptr)->outParmsMask | (0x80000000 >> (_pos)))
#define RPCCLT_OUTPARMSMASK_FLAG_CLR(_ptr, _pos) \
  ((_ptr)->outParmsMask & ~(0x80000000 >> (_pos)))

typedef void * rpccltVaParm_t;                         /* type used for variable arg pointers */

/* input parameter description definition */
typedef struct
{
  int16_t            fieldSize;                        /* message field size for parm */
} rpccltParmDesc_t;

/* function table entry definition */
typedef struct
{
  void                  *cltfn;         /* client function name (generic, for identification only) */
  RPCCOM_DEVMSG_FLAGS_t  flags;         /* message flags */
  uint32_t               outParmsMask;  /* bit mask denoting output parameters */
  int32_t                numParms;      /* number of input parameters */
  rpccltParmDesc_t       parmIdx[RPC_DEVMSG_PARMS_MAX]; /* index list of each input parameter */
} rpccltFtableEnt_t;

/* function prototypes -- client side */
extern OFDPA_ERROR_t rpccltMsgAlloc(rpccltProcParms_t *proc,
                              rpccomDevmsg_t **ppmsg);
extern void rpccltMsgFree(rpccltProcParms_t *proc,
                          rpccomDevmsg_t **ppmsg);
extern void rpccltMsgBegin(rpccltProcParms_t *proc,
                           rpccomDevmsg_t *pmsg, RPCCOM_DEVMSG_GRP_t group,
                           uint32_t type, RPCCOM_DEVMSG_FLAGS_t flags);
extern void rpccltMsgParmAdd(rpccltProcParms_t *proc,
                             rpccomDevmsg_t *pmsg, int32_t fieldSize,
                             uint8_t *parmVal, BOOL isBufferArg);
extern void rpccltMsgEnd(rpccltProcParms_t *proc, rpccomDevmsg_t *pmsg);
extern OFDPA_ERROR_t rpccltMsgSend(rpccltProcParms_t *proc, rpccomDevmsg_t *pmsg);
extern void rpccltMsgDump(rpccltProcParms_t *proc, rpccomDevmsg_t *pmsg, int8_t *title);
extern void rpccltMsgFlagsConvert(RPCCOM_DEVMSG_FLAGS_t flags,
                                  rpccltDevmsgFlg_e *msgflags);
extern void rpccltSuppWrapperCommon(int fid, va_list args, RPCCOM_DEVMSG_GRP_t msgGrp, rpccltFtableEnt_t **ftable);


/* Group-specific declarations follow */

/* OFDPA group -- client side */
extern rpccltFtableEnt_t   *rpcclt_ofdpa_ftable_g[];
extern void rpccltOfdpaFtableInit(void);
extern void rpccltSuppWrapperOfdpa(int fid, ...);

#endif /* _RPCCLT_SUPPORT_H */
