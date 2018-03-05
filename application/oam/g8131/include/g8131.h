/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename  g8131.h
*
* @purpose   G8131
*
* @component g8131
*
* @comments  none
*
* @create    6-NOV-2013
*
* @end
**********************************************************************/

#ifndef INCLUDE_G8131_H
#define INCLUDE_G8131_H

typedef enum
{
  G8131_EVENT_START = 0,
  G8131_EVENT_PG_CREATE,
  G8131_EVENT_PG_DELETE,
  G8131_EVENT_REQUEST,
  G8131_EVENT_PDU_RECEIVE,
  G8131_EVENT_ALARM_DFOP_CM,/*Receiption of APSw(APS frame from working transport entity). Cleared on receipt of no APSw during K*5s.*/
  G8131_EVENT_ALARM_DFOP_PM,/*Receiption of APSb(incompatible 'B' bit value. Cleared on receipt of expAPS event.*/
  G8131_EVENT_ALARM_DFOP_NR,/*Receiption of ABSr(incompatible 'request singal' value over 1s. Cleared on receipt of expAPS event.*/
  G8131_EVENT_ALARM_DFOP_TO,/*Receiption of no expAPS during K*5s, Cleared on receipt of expAPS event.*/
  G8131_EVENT_TIMER_TICK,
  G8131_EVENT_END
} g8131ControlEvents_t;

/*  Function Prototypes */
OFDPA_ERROR_t oamG8131Init(void);

#endif

