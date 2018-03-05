/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2014
*
**********************************************************************
*
* @filename   rpcsrv_util.c
*
* @purpose    RPC Server component utility functions
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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "rpcsrv_util.h"

static uint32_t rpcsrvDebugMsgLvl_g = 0;

void rpcsrvPrintf(unsigned char *format, ...)
{
  unsigned char buf[200];
  va_list       ap;

  if (RPCSRV_MSGLVL_ACTIVE(RPCSRV_MSGLVL_4))
  {
    va_start(ap, format);
    if (vsnprintf(buf, sizeof(buf), format, ap) >= sizeof(buf))
    {
      buf[sizeof(buf) - 1] = '\0';
    }
    va_end(ap);
    fwrite(buf, sizeof(buf[0]), strlen(buf), stdout);
    fflush(stdout);
  }
}

/*********************************************************************
* @purpose  Get RPC Server debug message level
*
* @param    void
*
* @returns  message level value (0-n)
*
* @comments Messages are defined according to interest level.  Higher
*           values generally display more debug messages.
*
* @comments This function returns the operational msgLvl value.
*
* @end
*********************************************************************/
uint32_t rpcsrvMsgLvlGet(void)
{
  return rpcsrvDebugMsgLvl_g;
}

/*********************************************************************
* @purpose  Set RPC Server debug message level
*
* @param    msgLvl      @b{(input)} Message level (0 = off, >0 = on)
*
* @returns  void
*
* @comments Messages are defined according to interest level.  Higher
*           values generally display more debug messages.
*
* @comments This function sets the operational msgLvl value.
*
* @end
*********************************************************************/
void rpcsrvMsgLvlSet(uint32_t msgLvl)
{
  rpcsrvDebugMsgLvl_g = msgLvl;
}

/*********************************************************************
* @purpose  Display a list of debug commands available for this component
*
* @param    void
*
* @returns  void
*
* @end
*********************************************************************/
void rpcsrvDebugHelp(void)
{
  uint32_t msgLvlReqd = RPCSRV_MSGLVL_ALWAYS;    /* always display output */

  /* display header banner */
  RPCSRV_PRT(msgLvlReqd, "\nThe following debug commands are available:\n\n");
  RPCSRV_PRT(msgLvlReqd, "  - rpcsrvInfoShow\n");
  RPCSRV_PRT(msgLvlReqd, "  - rpcsrvTaskStatsDump(instance ID)\n");
  RPCSRV_PRT(msgLvlReqd, "\n\n");
}

/*********************************************************************
* @purpose  Show the component operational data
*
* @param    none
*
* @returns  none
*
* @comments
*
* @end
*********************************************************************/
void rpcsrvInfoShow(void)
{
  uint32_t msgLvlReqd = RPCSRV_MSGLVL_ALWAYS;    /* always display output */

  RPCSRV_PRT(msgLvlReqd, "\nRPC Server operational data:\n\n");

  RPCSRV_PRT(msgLvlReqd, "  msgLvl=%u\n", rpcsrvMsgLvlGet());

  RPCSRV_PRT(msgLvlReqd, "\n\n");
}
