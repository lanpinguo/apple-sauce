/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2005-2015
*
**********************************************************************
* @filename  g8131_ctrl.h
*
* @purpose   G8131 Configurator file
*
* @component g8131
*
* @comments
*
* @create    6-NOV-2013
*
* @author    jianpiha
*
* @end
*
**********************************************************************/

#ifndef INCLUDE_G8131_CTRL_H
#define INCLUDE_G8131_CTRL_H

#include "g8131_sm.h"

typedef enum g8131_timer_type_s
{
  G8131_TIMER_TYPE_APS_CYCLE_INTERVAL = 0,/*Timer for all other APS packets sending*/
  G8131_TIMER_TYPE_HOLD_OFF_COUNT_DOWN,   /*Hold-off timer count down*/
  G8131_TIMER_TYPE_ALARM_CM,
  G8131_TIMER_TYPE_ALARM_PM,
  G8131_TIMER_TYPE_ALARM_NR,
  G8131_TIMER_TYPE_ALARM_TO,
}g8131_timer_type_t;

typedef void (*g8131_tmr_fn)(void *pParam);

typedef struct g8131_timer_node_s
{
  int           timeLeft;
  g8131_timer_type_t timeType;
  g8131_tmr_fn     func_p;
  void              *para;
}g8131_timer_node_t;

typedef struct g8131_timer_list_s
{
  g8131_timer_node_t         node;
  struct g8131_timer_list_s *next;
}g8131_timer_list_t;

/*Timer operation functions*/
void *g8131TimerCreate(g8131_timer_type_t tmType, uint32_t tmValue, void *pFunc, void *para);
void g8131TimerDelete(void *pNode);
OFDPA_ERROR_t g8131TimerUpdate(void *pNode, void * para, uint32_t tmValue, void *pFunc);
g8131_timer_list_t *g8131TimerGet(uint32_t grpId, g8131_timer_type_t tmType);

/*Timer related handling*/
void g8131TimerExpiryHdlr(timer_t timerCtrlBlk, void* ptrData);
void g8131TimerProcess(void);

/*Task related function*/
void g8131Task(void*);
OFDPA_ERROR_t g8131IssueCmd(uint32_t event, uint32_t intIfNum, void *data);
OFDPA_ERROR_t g8131IssueCmdSync(uint32_t event, uint32_t intIfNum, void *data);

OFDPA_ERROR_t g8131DispatchCmd(g8131MgmtMsg_t *pMsg);

#endif /* INCLUDE_G8131_CTRL_H */

