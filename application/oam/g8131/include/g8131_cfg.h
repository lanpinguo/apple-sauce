/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2005-2015
*
**********************************************************************
* @filename  g8131_cfg.h
*
* @purpose   G8131 Configuration file
*
* @component g8131
*
* @comments
*
* @end
*
**********************************************************************/

#ifndef INCLUDE_G8131_CFG_H
#define INCLUDE_G8131_CFG_H

#include "ofdpa_datatypes.h"
#include "g8131_exports.h"
#include "g8131_ctrl.h"

#define G8131_APS_TIMER_SLICE     (100)  /*G8131 base timer, frequency is 1s.*/ 
#define G8131_APS_INTERVAL        (5000) /*Cycling sending one APS packet per 5s.*/
#define G8131_HOLD_OFF_DEF_VALUE  (0)    /*Default hold-off timer value: 0s range(0-100ms)*/
#define G8131_WTR_DEF_VALUE       (5*60*1000) /*Default WTR timer value: 5min range(5-12min)*/

#define G8131_APS_MAX_INTEVAL     (17000) /*For ALARM dFOP-CM & dFOP-TO, 3.25*5~3.5*5, (16.25~17.5)*/
#define G8131_APS_ALARM_NR        (1000)  /*For ALARM dFOP-NR, APSr continues more than 1 second*/

typedef struct pg_oper_data_s
{
  uint32_t grpId;

  uint32_t ffGroupId;                    /* fast failover group */
  uint32_t livenessPortIdWorking;        /* working path bucket identifier */
  uint32_t livenessPortIdProtection;     /* protection path bucket identifier */
  uint32_t livenessPortIdActive;         /* status indicating which is active */
  uint32_t primVidWorking;
  uint32_t primVidProtection;
  uint32_t lmepIdWorking;
  uint32_t lmepIdProtection;

  uint32_t holdOffTime;     /*milliseconds*/
  uint32_t wtrTime;         /*milliseconds*/
  OFDPA_BOOL revertiveMode;

  uint32_t smState;
  uint32_t lastLReq;        /*last local request*/
  uint32_t lastRReq;        /*last remote request*/
  uint32_t lastSReq;        /*last send request, it's used for continuing sending APS packets*/

  OFDPA_BOOL sfwState;      /*local SF-W exist flag: True/False*/
  OFDPA_BOOL sfpState;      /*local SF-P exist flag: True/False*/
  OFDPA_BOOL sdwState;      /*local SD-W exist flag: True/False*/
  OFDPA_BOOL sdpState;      /*local SD-P exist flag: True/False*/
 
  uint32_t wtrCntValue;     /*Used for WTR expiration caculation*/ 
  OFDPA_BOOL holdOffBlock;  /*Received events are blocked by hold-off-timer*/

  OFDPA_BOOL dfop_cm;       /*ALARM: dFOP-CM*/
  OFDPA_BOOL dfop_pm;       /*ALARM: dFOP-PM*/
  OFDPA_BOOL dfop_nr;       /*ALARM: dFOP-NR*/
  OFDPA_BOOL dfop_to;       /*ALARM: dFOP-TO*/

  g8131_timer_list_t *apsSendTmNode; /*For APS packet cycle sending*/
  g8131_timer_list_t *holdOffTmNode; /*For HOLD-OFF count-down*/
  g8131_timer_list_t *alarmCMTmNode; /*For ALARM dFOP-CM*/
  g8131_timer_list_t *alarmNRTmNode; /*For ALARM dFOP-NR*/
  g8131_timer_list_t *alarmTOTmNode; /*For ALARM dFOP-TO*/

  uint32_t  quickswitch;
  OFDPA_BOOL sfwStateFast;      /*local SF-W exist flag: True/False*/
  OFDPA_BOOL sfpStateFast;      /*local SF-P exist flag: True/False*/

  OFDPA_BOOL inUse;
  
} pgOperData_t;

typedef struct
{
  uint8_t enable;         /*permit/not-permit to switch. realization: just keep current state.*/

  pgOperData_t pgData[OFDPA_G8131_PG_MAX + 1];
} g8131Cfg_t;

void g8131UpdateLivenessPortState(pgOperData_t *pgOperData);

#endif
