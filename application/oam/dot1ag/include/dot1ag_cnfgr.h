/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
* ********************************************************************
*
*  @filename       dot1ag_cnfgr.h
*  @purpose        Dot1ag functions prototypes
*  @component      Dot1ag
*  @comments       none
*  @create         11/19/2008
*  @end
*
***********************************************************************/

#ifndef INCLUDE_DOT1AG_CNFGR_H
#define INCLUDE_DOT1AG_CNFGR_H

#include "dot1ag_exports.h"

/* CFM mgmt event message */
typedef struct dot1agMsgCmdData_s
{
  uint32_t mdIndex;
  uint32_t maIndex;
  uint32_t mepIndex;
  uint32_t rmepIndex;
  uint32_t intIfNum;
  uint32_t uintVal;
  uint32_t localMpId;
  uint8_t  srcMac[OFDPA_MAC_ADDR_LEN];
} dot1agMsgCmdData_t;

typedef struct dot1agIntfChangeParms_s
{
  uint32_t event;
} dot1agIntfChangeParms_t;

typedef struct dot1agMsg_s
{
  uint32_t event;
  uint32_t intf;
  union
  {
    dot1agMsgCmdData_t      dot1agKeys;
    uint32_t                vlanData;
    dot1agIntfChangeParms_t dot1agIntfChangeParms;
    dot1agEventInfo_t       dot1agHwEvent;
  } data;
} dot1agMgmtMsg_t;

#define DOT1AG_TASK_NAME        "dot1agTask"
#define DOT1AG_QUEUE_EVENT      "dot1agEventQueue"
#define DOT1AG_QUEUE_PDU        "dot1agPduQueue"

void dot1agTimerHandler(timer_t timerCtrlBlk, void *ptrData);
OFDPA_ERROR_t dot1agInit(void);
OFDPA_ERROR_t dot1agStartTask(void);

#endif  /* INCLUDE_DOT1AG_CNFGR_H */
