/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpcclt_support_ofdpa.c
*
* @purpose   RPC client OFDPA wrapper support functions.
*
* @component OFDPA client
*
* @comments
*
* @create    27-Mar-2013
*
* @end
*
**********************************************************************/

#include <stdlib.h>
#include "rpc_common.h"
#include "rpcclt_ofdpa.h"
#include "rpcclt_support.h"

/*
 * NOTE: All variable args passed to this function must be of type rpccltVaParm_t.
 */
void rpccltSuppWrapperOfdpa(int fid, ...)
{
  static BOOL           ftableIsInit = FALSE;
  rpccltProcParms_t    *proc         = rpccltProcPtrGet();
  RPCCOM_DEVMSG_GRP_t   msgGrp       = RPCCOM_DEVMSG_GRP_OFDPA;
  rpccltFtableEnt_t   **ftable       = rpcclt_ofdpa_ftable_g;
  va_list               ap;

  /* initialize the client-side RPC function table */
  if (ftableIsInit == FALSE)
  {
    rpccltOfdpaFtableInit();
    ftableIsInit = TRUE;
    rpccltLogprintf(proc, RPCCLT_MSGLVL_ACTIVE,
                    "%s (inst=%d grp=%d): Initialized RPC client OFDPA function table.",
                    __FUNCTION__, proc->inst, msgGrp);
  }

  /* invoke common function to handle the RPC transaction */
  va_start(ap, fid);
  rpccltSuppWrapperCommon(fid, ap, msgGrp, ftable);
  va_end(ap);
}
