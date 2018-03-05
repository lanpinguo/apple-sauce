/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2002-2015
*
**********************************************************************
*
* @filename dot1ag_debug.h
*
* @purpose   defines of dot1ag debug routines
*
* @component dog1ag
*
* @comments
*
* @create 14-Mar-2009
*
* @end
*
**********************************************************************/
#ifndef DOT1AG_DEBUG_H
#define DOT1AG_DEBUG_H

#include <stdio.h>

#define DOT1AG_MAX_DEBUG_MSG_SIZE 512

#define DOT1AG_TRACE(traceFlag,__fmt__, __args__...)  {                 \
    if (debugTraceFlags & (traceFlag))                                  \
    {                                                                   \
      uint8_t  __buf1__[DOT1AG_MAX_DEBUG_MSG_SIZE];                     \
      ofdpaTimespec time;                                               \
      dpaUpTime(&time);                                                 \
      snprintf(__buf1__, DOT1AG_MAX_DEBUG_MSG_SIZE, __fmt__, ## __args__); \
      printf("%02d:%02d:%02d DOT1AG: In %s() %d - %s\r\n",              \
             time.hours, time.minutes, time.seconds,                    \
             __FUNCTION__,__LINE__,__buf1__);                           \
    }                                                                   \
  }

extern uint32_t debugTraceFlags;
extern const char *dot1agDebugHwEventNames[];

const char *dot1agDebugEventNameGet(uint32_t event);
void dot1agDebugTraceFlagSet(OFDPA_BOOL mode, DOT1AG_TRACE_FLAGS_t flags);
void    dot1agDebugPacketTraceFlagGet(OFDPA_BOOL *transmitFlag,OFDPA_BOOL *receiveFlag);
OFDPA_ERROR_t dot1agDebugPacketTraceFlagSet(OFDPA_BOOL transmitFlag,OFDPA_BOOL receiveFlag);
void    dot1agDebugPacketRxTrace(uint32_t intIfNum, uint32_t svid, void *bufHandle);
void dot1agDebugDumpClientMPDB(uint32_t serverLmepId, uint32_t clientLmepId);

#endif /* DOT1AG_DEBUG_H */

