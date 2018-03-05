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

#ifndef INCLUDE_PW_REDUN_CFG_H
#define INCLUDE_PW_REDUN_CFG_H

#include "ofdpa_datatypes.h"
#include "pw_redundancy_include.h"


typedef struct pw_redun_oper_data_s
{
  uint32_t grpId;

  uint32_t ffGroupId;                    /* fast failover group */
  uint32_t livenessPortIdWorking;        /* working path bucket identifier */
  uint32_t livenessPortIdProtection;     /* protection path bucket identifier */
  uint32_t livenessPortIdActive;         /* status indicating which is active */
  uint32_t stateWorking;
  uint32_t stateProtection;
  uint32_t lmepIdWorking;
  uint32_t lmepIdProtection;

  uint32_t holdOffTime;     /*milliseconds*/
  uint32_t wtrTime;         /*milliseconds*/
  OFDPA_BOOL revertiveMode;
  uint32_t switchCount;
  uint32_t lastSwitchTime;

 
  OFDPA_BOOL inUse;

} pwRedunOperData_t;

typedef struct
{
  uint8_t enable;         /*permit/not-permit to switch. realization: just keep current state.*/

  pwRedunOperData_t pgData[OFDPA_PW_REDUN_MAX + 1];
} pwRedunCfg_t;

void pwRedunUpdateLivenessPortState(pwRedunOperData_t *pgOperData);

#endif
