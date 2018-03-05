/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename  rpcsrv_support.h
*
* @purpose   RPC server wrapper support header file.
*
* @component rpcsrv
*
* @comments
*
* @create    19-Sep-2010
*
* @end
*
**********************************************************************/

#ifndef _RPCSRV_SUPPORT_H
#define _RPCSRV_SUPPORT_H

#include <string.h>
#include <stdint.h>
#include "ofdpa_datatypes.h"
#include "rpccom_support.h"

#define RPCSRV_API_RC_LEN_MAX        16      /* bytes: longest return value supported */

/* function table entry RPC server function signature */
typedef void (*rpcsrvFn_t) (uint32_t numParms,
                            ofdpa_buffdesc *pparm[],
                            uint8_t *rcbuf);

/* function table entry definition */
typedef struct
{
  rpcsrvFn_t srvfn;                     /* function to call */
  uint32_t   numParms;                  /* number of parameters to pass */
  uint16_t   parmSize[RPC_DEVMSG_PARMS_MAX]; /* expected size of each parameter */
} rpcsrvFtableEnt_t;


/* Group-specific declarations follow */

/* OFDPA group -- server side */
extern rpcsrvFtableEnt_t *rpcsrv_ofdpa_ftable_g[];
extern void rpcsrvOfdpaFtableInit(void);

#endif /* _RPCSRV_SUPPORT_H */
