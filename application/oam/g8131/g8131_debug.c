/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
* @filename  g8131_debug.c
*
* @purpose   g8131 debug utilities
*
* @component g8131
*
* @comments  none
*
* @create    14-Nov-2013
*
* @end
*
**********************************************************************/
#include "g8131_include.h"

uint32_t g8131debugTraceFlags = G8131_DEBUG_ERROR;

void g8131DebugTraceEnable(g8131_trace_flag_t flags)
{
  g8131debugTraceFlags |= flags;
}

void g8131DebugTraceClear(g8131_trace_flag_t flags)
{
  g8131debugTraceFlags &= ~flags;
}

void g8131DebugTraceFlagShow(void)
{
  printf("g8131debugTraceFlags = 0x%x\r\n", g8131debugTraceFlags);
}
