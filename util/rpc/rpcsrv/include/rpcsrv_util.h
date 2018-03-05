/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename   rpcsrv_util.h
*
* @purpose    RPC Server component utility function prototypes
*
* @component  rpcsrv
*
* @comments   none
*
* @create     19-Sep-2010
*
* @end
*
**********************************************************************/

#ifndef _RPCSRV_UTIL_H
#define _RPCSRV_UTIL_H

#include <stdint.h>


/****************************************
*
*  DEBUGGING MACROS, FUNCTION PROTOTYPES
*
*****************************************/

/* Debug message levels (higher values are more verbose)*/
#define RPCSRV_MSGLVL_ALWAYS  0                 /* use whenever 'printf' is desired */
#define RPCSRV_MSGLVL_1       1
#define RPCSRV_MSGLVL_2       2
#define RPCSRV_MSGLVL_3       3
#define RPCSRV_MSGLVL_4       4
#define RPCSRV_MSGLVL_5       5

#define RPCSRV_PRT(mlvl, fmt, args...) \
  { \
    if (rpcsrvMsgLvlGet() >= (mlvl)) \
    { \
      printf(fmt, ##args); \
    } \
  }

#define RPCSRV_MSGLVL_ACTIVE(mlvl) \
  (rpcsrvMsgLvlGet() >= (mlvl))

/*
**********************************************************************
*                    FUNCTION PROTOTYPES
**********************************************************************
*/

uint32_t rpcsrvMsgLvlGet(void);
void rpcsrvMsgLvlSet(uint32_t msgLvl);
void rpcsrvDebugHelp(void);
void rpcsrvInfoShow(void);
void rpcsrvPrintf(unsigned char *format, ...);

#endif /* _RPCSRV_UTIL_H */
