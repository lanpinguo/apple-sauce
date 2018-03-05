/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
* ********************************************************************
*
*  @filename       dot1ag_ctrl.h
*  @purpose        Dot1ah event flow control procedures functions
*                  prototypes
*  @component      Dot1ag
*  @comments       none
*  @create         02/14/2009
*  @end
*
***********************************************************************/
#ifndef INCLUDE_DOT1AG_CTRL_H
#define INCLUDE_DOT1AG_CTRL_H

#include "dot1ag_proto.h"
#include "dot1ag_cnfgr.h"
#include "dot1ag.h"
#include "dot1ag_db.h"

void dot1agQueueSyncSemaCreate(void);
void dot1agQueueSyncSemaTake(void);
void dot1agQueueSyncSemFree (void);
OFDPA_ERROR_t dot1agCfmCreateProcess(void);
OFDPA_ERROR_t dot1agCfmDeleteProcess(void);
OFDPA_ERROR_t dot1agCfmMPInit(uint32_t intIfNum, uint32_t primVID,
                              uint8_t mdLevel, uint32_t direction,
                              uint8_t *macAddress,
                              dot1agCfmMepTable_t *pMEPObj,
                              uint32_t localMpId);
void  dot1agCfmMPDestroy(dot1agCfmStackObj_t *pMPObj);
OFDPA_ERROR_t dot1agCfmMIPCreateProcess(uint32_t mdIndex, uint32_t mdLevel,
                                        uint32_t intIfNum, uint8_t *macAddr);
OFDPA_ERROR_t dot1agCfmMIPDeleteProcess(uint32_t mdIndex, uint32_t mdLevel,
                                        uint32_t intIfNum, uint32_t primVID);
OFDPA_ERROR_t dot1agCfmMEPCreateProcess(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t intIfNum,
                                        uint32_t localMpId);
OFDPA_ERROR_t dot1agCfmMEPDeleteProcess(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t intIfNum);
OFDPA_ERROR_t dot1agCfmMACreateProcess(uint32_t mdIndex, uint32_t maIndex);
OFDPA_ERROR_t dot1agCfmMADeleteProcess(uint32_t mdIndex, uint32_t maIndex);
OFDPA_ERROR_t dot1agCfmMDCreateProcess(uint32_t mdIndex);
OFDPA_ERROR_t dot1agCfmMDDeleteProcess(uint32_t mdIndex);
void  dot1agCfmMPActive(dot1agCfmStackObj_t *pMPObj);
void  dot1agCfmMPInActive(dot1agCfmStackObj_t *pMPObj);
OFDPA_ERROR_t dot1agIntfChangeProcess(uint32_t intIfNum, uint32_t intfEvent);
OFDPA_ERROR_t dot1agIntfChangeCallback(uint32_t intIfNum, uint32_t intfEvent);
OFDPA_ERROR_t dot1agIntfUp(uint32_t intIfNum);
OFDPA_ERROR_t dot1agIntfDown(uint32_t intIfNum);
OFDPA_ERROR_t dot1agVlanChangeCallback(uint32_t vlanId,
                                       uint32_t intIfNum, uint32_t event);
void dot1agIntfMacStatusChangeNotify(uint32_t intIfNum,
                                     uint32_t vlanId);
OFDPA_ERROR_t dot1agFillMsg(void *data, dot1agMgmtMsg_t *msg);
OFDPA_ERROR_t dot1agIssueCmd(uint32_t event, uint32_t intIfNum, void *data);
OFDPA_ERROR_t dot1agCfmUIEventProcess(dot1agMgmtMsg_t *pMsg);
void dot1agTask(void *arg);
OFDPA_ERROR_t dot1agLTRCacheClearProcess(void);
OFDPA_ERROR_t dot1agCCMEventHandlerCallback(dot1agEventInfo_t *eventInfo);

OFDPA_ERROR_t dot1agCfmRMEPEntryAddProcess(uint32_t mdIndex,
                                           uint32_t maIndex,
                                           uint32_t mepId,
                                           uint32_t rmepId,
                                           uint32_t intfNum,
                                           uint8_t *srcMac);

OFDPA_ERROR_t dot1agCfmRMEPEntryDeleteProcess(uint32_t mdIndex,
                                              uint32_t maIndex,
                                              uint32_t mepId,
                                              uint32_t rmepId,
                                              uint32_t intIfNum);
OFDPA_ERROR_t dot1agCfmMPEntryReinstall(uint32_t mdIndex,
                                        uint32_t maIndex);
OFDPA_ERROR_t dot1agCfmRMEPDeleteProcess(uint32_t mdIndex, uint32_t maIndex,
                                         uint32_t mepId, uint32_t intIfNum);
OFDPA_ERROR_t dot1agCfmMEPProlmCreateProcess(uint32_t mdIndex,
                                             uint32_t maIndex,
                                             uint32_t mepId,
                                             uint32_t intfNum,
                                             uint32_t priorityClass);
OFDPA_ERROR_t dot1agMepOndemLmmStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepIndex, uint32_t intIfNum, uint32_t priority);
OFDPA_ERROR_t dot1agMepOndemLmmStopProcess(uint32_t mdIndex, uint32_t maIndex,
                                           uint32_t mepIndex, uint32_t intIfNum, uint32_t priority);
OFDPA_ERROR_t dot1agMepDmmStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepIndex, uint32_t intIfNum, uint32_t priority);
OFDPA_ERROR_t dot1agMepDmmStopProcess(uint32_t mdIndex, uint32_t maIndex,
                                      uint32_t mepIndex, uint32_t intIfNum, uint32_t priority);

OFDPA_ERROR_t dot1agMepAisStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepIndex, uint32_t intIfNum,
                                       uint32_t startFlag);

OFDPA_ERROR_t dot1agMepLckStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepIndex, uint32_t intIfNum,
                                       uint32_t startFlag);

OFDPA_ERROR_t dot1agMepCsfStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepIndex, uint32_t intIfNum,
                                       uint32_t startFlag);

OFDPA_ERROR_t dot1agSLMCacheClearProcess(void);

OFDPA_ERROR_t dot1agTimerUpdate(void *pNode, void *para, uint32_t tmValue, dot1ag_tmr_fn pFunc);
void *dot1agTimerNodeAdd(uint32_t tmValue, dot1ag_tmr_fn pFunc, void *para);
void dot1agTimerDelete(void *pNode);
void dot1agTimerProcess(dot1ag_timer_list_t timerList);
OFDPA_ERROR_t dot1agCfmClientMPDBEntryAdd(dot1agCfmStackObj_t *pServerMpObj);
OFDPA_ERROR_t dot1agCfmClientMPDBEntryDelete(dot1agCfmStackObj_t *pServerMpObj);
OFDPA_ERROR_t dot1agCfmClientMPDBUpdate(dot1agCfmStackObj_t *pClientMpObj, OFDPA_BOOL isMipDelete);
#endif
