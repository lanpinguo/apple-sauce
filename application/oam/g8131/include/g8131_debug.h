/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2002-2015
*
**********************************************************************
*
* @filename g8131_debug.h
*
* @purpose   defines of g8131 debug routines
*
* @component g8131
*
* @comments
*
* @create 14-11-2013
*
* @end
*
**********************************************************************/
#ifndef G8131_DEBUG_H
#define G8131_DEBUG_H
#include <stdio.h>

#define G8131_DEBUG_PRINTF printf

#define G8131_MAX_DEBUG_MSG_SIZE 512

#define G8131_TRACE(traceFlag, __fmt__, __args__...) /*lint -save -e506 -e774 -e737*/\
{ \
  if ((g8131debugTraceFlags & (traceFlag)) || (traceFlag == G8131_DEBUG_ERROR))\
  {\
    char  __buf1__[G8131_MAX_DEBUG_MSG_SIZE];    \
    char  __buf2__[G8131_MAX_DEBUG_MSG_SIZE];    \
    ofdpaTimespec time;                                \
    dpaUpTime(&time);                             \
    (void)snprintf (__buf1__, G8131_MAX_DEBUG_MSG_SIZE, __fmt__, ## __args__);  \
    (void)snprintf (__buf2__, G8131_MAX_DEBUG_MSG_SIZE,                    \
               "%02d:%02d:%02d G8131: In %s() %d - %s",                             \
               time.hours, time.minutes, time.seconds,\
               __FUNCTION__,__LINE__,__buf1__);       \
    G8131_DEBUG_PRINTF("%s\r\n", __buf2__); \
  }\
}/*lint -restore*/

extern uint32_t g8131debugTraceFlags;

#endif /* G8131_DEBUG_H */
