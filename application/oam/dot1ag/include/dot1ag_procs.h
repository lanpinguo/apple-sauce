/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2015
*
**********************************************************************
*
* @filename   dot1ag_procs.h
*
* @purpose    Contains signatures of methods used by various SMs
*
* @component  DOT1AG/Connectivity & Fault Management
*
* @comments   none
*
* @create     03-Mar-2009
*
* @end
*
**********************************************************************/
#ifndef INCLUDE_DOT1AG_PROCS_H
#define INCLUDE_DOT1AG_PROCS_H

#include "dot1ag_proto.h"
#include "dot1ag_pdu.h"

/* CCR Methods */
OFDPA_ERROR_t  MEPprocessEqualCCM(dot1agControlPkt_t *dot1agPkt,
                            dot1agCfmStackObj_t *pMPObj);
OFDPA_ERROR_t MEPprocessLowCCM(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);

/* LBIR Methods */
OFDPA_ERROR_t ProcessLBR(dot1agControlPkt_t *dot1agPkt,
                   dot1agCfmStackObj_t *pMPObj);
/* LBR Methods */
OFDPA_ERROR_t ProcessLBM(dot1agControlPkt_t *dot1agPkt,
                   dot1agCfmStackObj_t *pMPObj);
OFDPA_ERROR_t xmitLBR(dot1agControlPkt_t *dot1agPkt,
                dot1agCfmStackObj_t *pMPObj);
/* LTIR Methods */
OFDPA_ERROR_t ProcessLTR(dot1agControlPkt_t *dot1agPkt,
                   dot1agCfmStackObj_t *pMPObj);

/* LTMR Methods */
OFDPA_ERROR_t ProcessLTM(dot1agControlPkt_t *dot1agPkt,
                   dot1agCfmStackObj_t *pMPObj);

uint32_t CCMtime(uint32_t ccmInterval, OFDPA_BOOL hwEnabled);

OFDPA_ERROR_t xmitFaultAlarm(uint32_t mdIndex, uint32_t maIndex,
                       uint32_t mepId, uint32_t fngDefect);
OFDPA_ERROR_t xmitLBM(dot1agCfmStackObj_t *pMEPObj);
OFDPA_ERROR_t addCCMToArray(dot1agCfmStackObj_t *pMEPObj);
OFDPA_ERROR_t xmitLTM(dot1agCfmStackObj_t *pMEPObj);
OFDPA_ERROR_t ProcessLTM(dot1agControlPkt_t *dot1agPkt,
                   dot1agCfmStackObj_t *pMPObj);
OFDPA_ERROR_t ProcessLTMEgressIdFind(dot1agControlPkt_t *dot1agPkt,
                               dot1agCfmStackObj_t *rxpMPObj,
                               uint32_t *txIntIfnum);
void ProcessLTMRxDownMEP(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);
void ProcessLTMRxDownMHFTxUpMEP(dot1agControlPkt_t *dot1agPkt,
                                dot1agCfmStackObj_t *pMPObj);
void ProcessLTMRxUpMEP(dot1agControlPkt_t *dot1agPkt,
                       dot1agCfmStackObj_t *pMPObj);
void ProcessLTMRxUpMHF(dot1agControlPkt_t *dot1agPkt,
                       dot1agCfmStackObj_t *pMPObj);
OFDPA_ERROR_t ForwardLTM(dot1agControlPkt_t *dot1agPkt,
                   dot1agCfmStackObj_t *txpMPObj);
OFDPA_ERROR_t enqueLTR(dot1agControlPkt_t *dot1agPkt, dot1agCfmStackObj_t *pMPObj,
                 OFDPA_BOOL ltmFwdFlag, Dot1agCfmMPType_t ltrMPType,
                 Dot1agCfmMPType_t rxMPType, uint32_t ingressIntIfNum,
                 uint32_t egressIntIfNum);

void MAdefectIndicationNotify(dot1agCfmMepTable_t  *mepData);

/* LTRT Methods */
void clearPendingLTRs();
void xmitOldestLTR();
OFDPA_ERROR_t dot1agMAIDGet(dot1agCfmMdTable_t *mdEntry,
                        dot1agCfmMaNetTable_t *maEntry,
                        uint8_t *maid);

OFDPA_ERROR_t xmitProLMM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority);
OFDPA_ERROR_t xmitOndemLMM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority);
OFDPA_ERROR_t xmitLMM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority, OFDPA_BOOL lmDestIsMepId,
                      uint8_t *lmDestMacAddress, Dot1agCfmLmmOperationType_t lmmTypeFlags);
OFDPA_ERROR_t xmitProSLM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority);
OFDPA_ERROR_t xmitOndemSLM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority);
OFDPA_ERROR_t xmitSLM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority, uint32_t lmDestMepId,
                      OFDPA_BOOL lmDestIsMepId, uint8_t *lmDestMacAddress, uint32_t slmTestId);
OFDPA_ERROR_t ProcessSLM(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);
OFDPA_ERROR_t xmitSLR(dot1agControlPkt_t *dot1agPkt,
                      dot1agCfmStackObj_t *pMPObj,
                      dot1agCfmSlmObj_t *pSlmEntry);
OFDPA_ERROR_t dot1agParseSLR(dot1agControlPkt_t *dot1agPkt);
OFDPA_ERROR_t ProcessLMM(dot1agControlPkt_t *dot1agPkt,
                   dot1agCfmStackObj_t *pMPObj);
OFDPA_ERROR_t xmitLMR(dot1agControlPkt_t *dot1agPkt,
                dot1agCfmStackObj_t *pMPObj);
OFDPA_ERROR_t dot1agParseLMR(dot1agControlPkt_t *dot1agPkt);
OFDPA_ERROR_t xmitDMM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority);

OFDPA_ERROR_t ProcessDMM(dot1agControlPkt_t *dot1agPkt,
                   dot1agCfmStackObj_t *pMPObj);

OFDPA_ERROR_t xmitDMR(dot1agControlPkt_t *dot1agPkt,
                dot1agCfmStackObj_t *pMPObj);

OFDPA_ERROR_t dot1agParseDMR(dot1agControlPkt_t *dot1agPkt);

OFDPA_BOOL IsAISTriggered(dot1agCfmMepTable_t *pServerMEPObj);

OFDPA_ERROR_t xmitAIS(dot1agCfmStackObj_t *pMPObj, dot1agCfmMepTable_t *pServerMEPData);

void AISTriggerHandler(dot1agCfmMepTable_t *pServerMEPObj);

OFDPA_ERROR_t dot1agCheckAIS(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg, uint32_t *interval);

OFDPA_ERROR_t xmitLCK(dot1agCfmStackObj_t *pServerMEPObj, dot1agCfmStackObj_t *pClientMEPObj);

OFDPA_ERROR_t xmitClientLCK(dot1agCfmStackObj_t *pServerMEPObj);

OFDPA_ERROR_t dot1agCheckLCK(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg, uint32_t *interval);

OFDPA_ERROR_t xmitClientCSF(dot1agCfmStackObj_t *pMPObj, uint32_t csfType);

void CSFTriggerHandler(dot1agCfmMepTable_t *pClientMEPObj);

OFDPA_ERROR_t dot1agCheckCSF(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg,
                             uint32_t *interval, uint32_t *type);

#endif /* INCLUDE_DOT1AG_PROCS_H */
