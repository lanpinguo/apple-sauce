/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
**********************************************************************
*
* @filename   dot1ag_ctrl.c
*
* @purpose    Contains definitions to execution flow control procedures
*             for dot1ag component
*
* @component  DOT1AG/Connectivity & Fault Management
*
* @comments   none
*
* @create     13-Feb-2009
*
**********************************************************************/

#include <stdlib.h>
#include "msgq.h"
#include "ofdpa_porting.h"
#include "dot1ag_exports.h"
#include "dot1ag_ctrl.h"
#include "dot1ag_cnfgr.h"
#include "dot1ag_y1731pm.h"
#include "dot1ag.h"
#include "dot1ag_api.h"
#include "dot1ag_outcalls.h"
#include "dot1ag_pdu.h"
#include "dot1ag_procs.h"
#include "dot1ag_debug.h"
#include "dot1ag_sm.h"
//yjg add start
#include "g8131_api.h"
//yjg add end

#define DOT1AG_MAX_NODE 64

#define DOT1AG_IS_READY (((dot1agCFM.dot1agState == OFDPA_TRUE)) ? (OFDPA_TRUE) : (OFDPA_FALSE))

extern void *dot1agTaskSyncSem;
extern void dot1agSendCcmArray();

/* Dot1ag queue sync semaphore */
static void *dot1agQueueSyncSema = 0;
void dot1agCfmDefectsClear(dot1agCfmStackObj_t *pMPObj);
void dot1agCfmLbmTestTerminate(dot1agCfmStackObj_t *pMEPObj);

extern uint32_t dot1agMicrosecondsGet(void);

//yjg add start, 增加硬件RemoteCCM更新处理函数
extern uint32_t dot1agCentisecondsGet(void);
void dot1agUpdateRemoteCCM(dot1agCfmRMepObj_t  *rmepObj, uint32_t lmepId, OFDPA_BOOL faultOn);
//yjg add end

OFDPA_BOOL bFastCcmDebug = 0;

void setFastCcmDebugFlag(OFDPA_BOOL bFlag)
{
  bFastCcmDebug = bFlag;
}

/**************************************************************************
* @purpose  Create queue sync semaphore
*
* @param    none
*
* @comments Dot1ag queue sync semaphore allows all the messages in the
*           dot1ag event queue to get processed by putting a message on it
*           and blocking on dot1agQueueSync semaphore
*
* @end
*************************************************************************/
void dot1agQueueSyncSemaCreate(void)
{
  dot1agQueueSyncSema = dpaSemCreate("dot1ag sync semaphore", DPA_SEM_COUNTING, 0);
  if (dot1agQueueSyncSema == NULL)
  {
    return;
  }
}

/**************************************************************************
* @purpose  Take queue sync semaphore
*
* @param    none
*
* @comments
*
* @end
*************************************************************************/
void dot1agQueueSyncSemaTake(void)
{
  dpaSemTake(dot1agQueueSyncSema, DPA_SEM_FOREVER);
}

/**************************************************************************
* @purpose  Give queue sync semaphore
*
* @param    none
*
* @comments
*
* @end
*************************************************************************/
void dot1agQueueSyncSemaGive(void)
{
  dpaSemGive(dot1agQueueSyncSema);
}

/*********************************************************************
* @purpose  Notify enable RMEP Defect Status change event to all RMEP
*           SMs on this MEP.
*
*
* @returns  none
*
* @notes
*
* @end
*********************************************************************/
void dot1agMEPEnableRmepDefectNotify(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId)
{
  dot1agCfmStackObj_t   *pMPObj;
  uint32_t               rmepId;
  dot1agCfmSMEventMsg_t  eventMsg;
  dot1agCfmRMepObj_t    *rmepObjEntry = NULL;
  dot1agCfmMepObj_t     *pMEPObj      = NULL;

  /* Find the MEP object */
  pMEPObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, 0, AVL_NEXT);
  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL ||
      pMEPObj->mepData->maData == NULL ||
      pMEPObj->mepData->maData->mdData == NULL ||
      mdIndex != pMEPObj->mepData->maData->mdData->dot1agCfmMdIndex ||
      maIndex != pMEPObj->mepData->maData->dot1agCfmMaIndex ||
      mepId   != pMEPObj->mepData->dot1agCfmMepIdentifier)
  {
    return;
  }

  pMPObj = dot1agCfmStackObjGet(pMEPObj->mepData->dot1agCfmMepIfIndex,
                                pMEPObj->mepData->dot1agCfmMepPrimaryVid,
                                pMEPObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                pMEPObj->mepData->dot1agCfmMepDirection, AVL_EXACT);
  if (NULL == pMPObj)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Remote MEP defect notification failed because of invalid MEP in CFM Stack on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId, pMEPObj->mepData->dot1agCfmMepIfIndex);
    return;
  }

  rmepId = 0;
  while ((rmepObjEntry = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                             rmepId, AVL_NEXT)) != NULL)
  {
    if (rmepObjEntry->key.dot1agCfmMdIndex != mdIndex ||
        rmepObjEntry->key.dot1agCfmMaIndex != maIndex ||
        rmepObjEntry->key.dot1agCfmMepIdentifier != mepId)
    {
      break;
    }

    /* Pass enableRmepDefect to the RMEP SM */
    eventMsg.pRMEPObj = rmepObjEntry;
    eventMsg.pMepData = pMEPObj->mepData;
    eventMsg.event    = dot1agRmepNotEnableRmepDefect;
    eventMsg.pMPObj   = pMPObj;
    if ((eventMsg.pMepData != NULL) &&
        (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMepData->dot1agCfmMepIdentifier)))
    {
      (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
    }
    else
    {
      (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
    }

    rmepId = rmepObjEntry->key.dot1agCfmMepDbRMepIdentifier;
  }/* Loop through RMep DB for matching RMEP entries for this MEP */
}

/*********************************************************************
* @purpose  Notify MAC Status change event to all MEP CCIT SMs on this
*           interface in this VLAN. Notify enableRMEPDefect event too.
*
* @param    intIfnum  @b{(input)}  Interface on which MEP is configured
* @param    vlan      @b{(input)}  primary VID of the MA of which MEP
*                                  is part of
*
* @returns  none
*
* @notes
*
* @end
*********************************************************************/
void dot1agIntfMacStatusChangeNotify(uint32_t intIfNum, uint32_t vlanId)
{
  uint8_t                 mdLevel   = 0;
  Dot1agCfmMpDirection_t  direction = 0;
  dot1agCfmStackObj_t    *pMPObj    = NULL;
  dot1agCfmSMEventMsg_t   eventMsg;

  while ((pMPObj = dot1agCfmStackObjGet(intIfNum, vlanId, mdLevel,
                                        direction, AVL_NEXT)) != NULL)
  {
    if (intIfNum != pMPObj->key.dot1agCfmStackifIndex ||
        vlanId   != pMPObj->key.dot1agCfmStackVlanIdOrNone)
    {
      break;
    }

    if (pMPObj->operMode == OFDPA_TRUE)
    {
      if (pMPObj->mepData &&
          pMPObj->mepData->maData->dot1agCfmMaNetCcmInterval <= DOT1AG_CCM_interval10s)
      {
        eventMsg.pMPObj = pMPObj;
        eventMsg.event = dot1agCCIMacStatusChanged;
        if ((eventMsg.pMPObj->mepData != NULL) &&
            (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
        {
          (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
        }
        else
        {
          (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
        }
      }

//      if ((pMPObj->mepData) && (OFDPA_INTF_ISMASKBITSET(dot1agCFM.dot1agIntfVlanFwdMask[vlanId], intIfNum) == 0))
      if (pMPObj->mepData != NULL &&
          pMPObj->mepData->maData != NULL &&
          pMPObj->mepData->maData->mdData != NULL)
      {
        dot1agMEPEnableRmepDefectNotify(pMPObj->mepData->maData->mdData->dot1agCfmMdIndex,
                                        pMPObj->mepData->maData->dot1agCfmMaIndex,
                                        pMPObj->mepData->dot1agCfmMepIdentifier);
      }
    }
    mdLevel   = pMPObj->key.dot1agCfmStackMdLevel;
    direction = pMPObj->key.dot1agCfmStackDirection;
  }/* Loop through the CFM stack for MEPs */

  /* Loop through the RMEP stack and issue enableRmepDefect */
}

/*********************************************************************
* @purpose  Remove Remote mep entries given (mdIndex, maIndex, mepId)
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Delete all RMEP objects and clear all the associated
*           SMs
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agRmepDelete(uint32_t mdIndex, uint32_t maIndex,
                               uint32_t mepId)
{
  dot1agCfmRMepObj_t *pRMEPObj     = NULL;
  dot1agCfmRMepObj_t *pRMEPObjNext = NULL;
  OFDPA_ERROR_t       rc           = OFDPA_E_NONE;

  while ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                         0, AVL_NEXT)) != NULL &&
         rc == OFDPA_E_NONE)
  {
    /* Check if no more RMEPs exist for this (mdIndex, maIndex, mepId) combination */
    if (pRMEPObj->key.dot1agCfmMdIndex != mdIndex ||
        pRMEPObj->key.dot1agCfmMaIndex != maIndex ||
        pRMEPObj->key.dot1agCfmMepIdentifier != mepId)
    {
      break;
    }

#if OFDPA_NOT_SUPPORTED
    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    {
      /* Delete the Remote MEP entry from hardware */
      if ((pRMEPObj->hwExist == OFDPA_TRUE) &&
          (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId)))
      {
        if (dtlDot1agRemoteMEPDelete(mdIndex, maIndex, mepId,
                                     pRMEPObj->key.dot1agCfmMepDbRMepIdentifier) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to delete RMEP entry from hardware for mdIndex %d maIndex %d mepId %d rmepId %d\r\n",
                             mdIndex, maIndex, mepId, pRMEPObj->key.dot1agCfmMepDbRMepIdentifier);
          return OFDPA_E_FAIL;
        }
        pRMEPObj->hwExist = OFDPA_FALSE;
      }
      /* Stop any timers if running */
      if (pRMEPObj->dot1agCfmRmepVar.rmepTimer != NULL)
      {
        appTimerDelete(dot1agCFM.timerCB, pRMEPObj->dot1agCfmRmepVar.rmepTimer);
      }
    }
    else
#endif
    {
      if (pRMEPObj->hwExist == OFDPA_TRUE)
      {
        uint32_t rmepId = pRMEPObj->key.dot1agCfmMepDbRMepIdentifier;
        while ((pRMEPObjNext = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                                   rmepId, AVL_NEXT)) != NULL)
        {
          if (pRMEPObj->key.dot1agCfmMdIndex != mdIndex ||
              pRMEPObj->key.dot1agCfmMaIndex != maIndex ||
              pRMEPObj->key.dot1agCfmMepIdentifier != mepId)
          {
            return OFDPA_E_NONE;
          }
          if (pRMEPObjNext->hwExist)
          {
            rmepId = pRMEPObjNext->key.dot1agCfmMepDbRMepIdentifier;
            continue;
          }
          if (pRMEPObjNext->dot1agCfmRmepVar.rmepTimer != NULL)
          {
            dot1agTimerDelete(pRMEPObjNext->dot1agCfmRmepVar.rmepTimer);
            pRMEPObjNext->dot1agCfmRmepVar.rmepTimer = NULL;
          }
          rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_RMEP_TREE, &dot1agCFM.dot1agRMepTree,
                                       pRMEPObjNext);
          pRMEPObjNext = NULL;
        }

        if (pRMEPObjNext == NULL)
          return OFDPA_E_NONE;
      }
      /* Stop any timers if running */
      if (pRMEPObj->dot1agCfmRmepVar.rmepTimer != NULL)
      {
        dot1agTimerDelete(pRMEPObj->dot1agCfmRmepVar.rmepTimer);
        pRMEPObj->dot1agCfmRmepVar.rmepTimer = NULL;
      }
    }

    /* Remove the entry from the rmepList - RMEP Database */
    rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_RMEP_TREE, &dot1agCFM.dot1agRMepTree,
                                 pRMEPObj);
  }
  return rc;
}

/*********************************************************************
* @purpose  Link state changes event process on link up
*
* @param    intIfNum   @b{(input)) internal interface number
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments CFM stack objects involving this intIfNum are operationally
*           enabled
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agIntfUp(uint32_t intIfNum)
{
  dot1agCfmStackObj_t  *pMPObj = NULL;
  uint32_t             vlanId, mdLevel, direction;

  mdLevel   = 0;
  vlanId    = 0;
  direction = 0;
  while ((pMPObj = dot1agCfmStackObjGet(intIfNum, vlanId, mdLevel, direction, AVL_NEXT)) != NULL)
  {
    if (intIfNum != pMPObj->key.dot1agCfmStackifIndex)
    {
      break;
    }

    /* activate the MP */
    dot1agCfmMPActive(pMPObj);

    vlanId      = pMPObj->key.dot1agCfmStackVlanIdOrNone;
    mdLevel     = pMPObj->key.dot1agCfmStackMdLevel;
    direction   = pMPObj->key.dot1agCfmStackDirection;
  }/* End of cfm stack mp objects iterations */
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Link state changes event process L7_DOWN
*
* @param    intIfNum   @b{(input)) internal interface number
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments CFM stack objects involving this intIfNum are operationally
*           disabled
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agIntfDown(uint32_t intIfNum)
{
  dot1agCfmStackObj_t  *pMPObj = NULL;
  uint32_t             vlanId, mdLevel, direction;

  mdLevel   = 0;
  vlanId    = 0;
  direction = 0;

  while ((pMPObj = dot1agCfmStackObjGet(intIfNum, vlanId,
                                        mdLevel, direction, AVL_NEXT))
         != NULL)
  {
    if (intIfNum != pMPObj->key.dot1agCfmStackifIndex)
    {
      break;
    }

    /* De-activate the MP */
    if (!(pMPObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP
          && pMPObj->mepData != NULL))
    {
      dot1agCfmMPInActive(pMPObj);
    }

    vlanId      = pMPObj->key.dot1agCfmStackVlanIdOrNone;
    mdLevel     = pMPObj->key.dot1agCfmStackMdLevel;
    direction   = pMPObj->key.dot1agCfmStackDirection;
  }/* End of cfm stack mp objects iterations */
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Link state changes event process
*
* @param    intIfNum   @b{(input)) internal interface number
* @param    intIfEvent @b{(input)) interface event
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agIntfChangeProcess(uint32_t intIfNum, uint32_t intfEvent)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  if (!(DOT1AG_IS_READY))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Received an interface change callback while not ready to receive it\r\n", 0);
    rc = OFDPA_E_FAIL;
    return rc;
  }

  if (intfEvent == OFDPA_PORT_STATE_LINK_DOWN)
  {
#if OFDPA_NOT_SUPPORTED
    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    {
      rc = dot1agIntfDown(intIfNum);
    }
#endif
  }
  else
  {
    rc = dot1agIntfUp(intIfNum);
  }

#if OFDPA_NOT_SUPPORTED
  switch (intfEvent)
  {
    case L7_UP:
      rc = dot1agIntfUp(intIfNum);
      break;
    case L7_DOWN:
      if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
      {
        rc = dot1agIntfDown(intIfNum);
      }
      break;
    case L7_LAG_ACQUIRE:
    case L7_DETACH:
      rc = dot1agIntfDetach(intIfNum);
      break;
    case L7_LAG_RELEASE:
    case L7_ATTACH:
      rc = dot1agIntfAttach(intIfNum);
      break;
    case L7_FORWARDING:
    case L7_NOT_FORWARDING:
      rc = dot1agDot1sDisabledEventsProcess(intIfNum);
    default:
      break;
  }

  status.response.rc  = rc;
  nimEventStatusCallback(status);
#endif
  return rc;
}

/*********************************************************************
* @purpose  Link state changes callback
*
* @param    intIfNum   @b{(input)) internal interface number
* @param    intIfEvent @b{(input)) interface event
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agIntfChangeCallback(uint32_t intIfNum, uint32_t intfEvent)
{
  dot1agIntfChangeParms_t msg;
  OFDPA_ERROR_t rc;

  if (!(DOT1AG_IS_READY))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Received an interface change callback while not ready to receive it\r\n", 0);
    return OFDPA_E_FAIL;
  }

  msg.event = intfEvent;
  rc = dot1agIssueCmd(dot1agIntfChange, intIfNum, &msg);

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to send dot1ag interface change event, rc = %d\r\n", rc);
  }

  return rc;
}

/*********************************************************************
* @purpose  To process CFM create event generated from UI or during
*           system init
*
* @param    none
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmCreateProcess()
{
  dot1agCfmStackObj_t  *pMPObj = NULL;
  uint32_t             vlanId, mdLevel, direction, intIfNum;
  dot1agCfmSMEventMsg_t eventMsg;
//  uint32_t mode;
//  dot1agCfmMAObj_t      *maObjEntry;
//  dot1agCfmMAObjKey_t   maObjEntryKey;
//  uint32_t             mdIndex, maIndex, primVID;
//  dot1agCfmMdTable_t    *mdEntry = NULL;
//  dot1agCfmMaNetTable_t *maEntry = NULL;
//  uint8_t maId[OFDPA_DOT1AG_MAID_MAX_LEN];
//  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  intIfNum  = 0;
  mdLevel   = 0;
  vlanId    = 0;
  direction = 0;
#if OFDPA_NOT_SUPPORTED
  mdIndex   = 0;
  maIndex   = 0;
  mode = OFDPA_ENABLE;

  /* Install rules to trap CFM PDUs */
  rc = dtlDot1agCFMModeConfig(mode);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }
#endif

#if OFDPA_NOT_SUPPORTED
  memset(maId, 0x0, OFDPA_DOT1AG_MAID_MAX_LEN);
  memset(&maObjEntryKey, 0x0, sizeof(maObjEntryKey));

  /* create the domains in hardware if already configured */
  while ((maObjEntry = avlSearch(&dot1agCFM.dot1agMATree, &maObjEntryKey, AVL_NEXT))
         != NULL)
  {
    maEntry = maObjEntry->maData;
    mdEntry = maObjEntry->maData->mdData;
    primVID = maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId;
    mdIndex = mdEntry->dot1agCfmMdIndex;
    maIndex = maEntry->dot1agCfmMaIndex;
    if ((dot1agMAIDGet(mdEntry, maEntry, maId)) != OFDPA_E_NONE)
    {
      return OFDPA_E_FAIL;
    }

    if (dtlDot1agMAGroupAdd(mdIndex, maIndex, maId, primVID)
        != OFDPA_E_NONE)
    {
      return OFDPA_E_FAIL;
    }

    memcpy(&maObjEntryKey, &maObjEntry->key, sizeof(maObjEntryKey));
  } /* end of while */
#endif
  while ((pMPObj = dot1agCfmStackObjGet(intIfNum, vlanId,
                                        mdLevel, direction, AVL_NEXT))
         != NULL)
  {
    if (dot1agIntfCanBeOperational(pMPObj->key.dot1agCfmStackifIndex) == OFDPA_TRUE ||
        (pMPObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP &&
         pMPObj->mepData != NULL))
    {
      /* Activate each MP */
      dot1agCfmMPActive(pMPObj);
    }
    intIfNum    = pMPObj->key.dot1agCfmStackifIndex;
    vlanId      = pMPObj->key.dot1agCfmStackVlanIdOrNone;
    mdLevel     = pMPObj->key.dot1agCfmStackMdLevel;
    direction   = pMPObj->key.dot1agCfmStackDirection;
  }
  dot1agCFM.dot1agCfmLtmrVar.nextLtmTransId = 1;

  /* LTRT SM */
  eventMsg.event = dot1agLTRTBegin;
  /* Need to revisit */
  (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To process CFM delete event generated from UI or during
*           system init
*
* @param    none
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmDeleteProcess()
{
  dot1agCfmStackObj_t   *pMPObj = NULL;
  uint32_t               vlanId, mdLevel, direction, intIfNum;
  dot1agCfmSMEventMsg_t  eventMsg;
//  uint32_t mode;
//  dot1agCfmMAObj_t      *maObjEntry;
  dot1agCfmMAObjKey_t    maObjEntryKey;
//  uint32_t             mdIndex, maIndex;

//  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  intIfNum  = 0;
  mdLevel   = 0;
  vlanId    = 0;
  direction = 0;

//  mode = OFDPA_DISABLE;
  memset(&maObjEntryKey, 0x0, sizeof(maObjEntryKey));

  /* Clearing LTR cache */
  if (dot1agLTRCacheClearProcess() != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to clear LTR cache\r\n", 0);
  }

  /* Clearing SLM cache */
  if (dot1agSLMCacheClearProcess() != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to clear SLM cache\r\n", 0);
  }

  while ((pMPObj = dot1agCfmStackObjGet(intIfNum, vlanId,
                                        mdLevel, direction, AVL_NEXT))
         != NULL)
  {
    if (pMPObj->operMode == OFDPA_TRUE)
    {
      /* De-activate each MP */
      dot1agCfmMPInActive(pMPObj);
      dot1agCfmDefectsClear(pMPObj);
    }
    intIfNum    = pMPObj->key.dot1agCfmStackifIndex;
    vlanId      = pMPObj->key.dot1agCfmStackVlanIdOrNone;
    mdLevel     = pMPObj->key.dot1agCfmStackMdLevel;
    direction   = pMPObj->key.dot1agCfmStackDirection;
  }

#if OFDPA_NOT_SUPPORTED
  /*Delete the entries from hardware if already configured*/
  while ((maObjEntry = avlSearch(&dot1agCFM.dot1agMATree, &maObjEntryKey, AVL_NEXT))
         != NULL)
  {
    mdIndex = maObjEntry->maData->mdData->dot1agCfmMdIndex;
    maIndex = maObjEntry->maData->dot1agCfmMaIndex;
    rc = dtlDot1agMAGroupDelete(mdIndex, maIndex);
    if (rc != OFDPA_E_NONE)
    {
      return OFDPA_E_FAIL;
    }
    memcpy(&maObjEntryKey, &maObjEntry->key, sizeof(maObjEntryKey));
  }

  /* Remove rules installed to trap CFM PDUs */
  rc = dtlDot1agCFMModeConfig(mode);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }
#endif

  eventMsg.event = dot1agLTRTNotMepActive;
  /* Revisit */
  (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To Make MP operationally active
*
* @param    pMPObj   @b{(input)} Reference to maintenance point object
*
* @returns  none
*
* @comments
*
* @end
*********************************************************************/
void  dot1agCfmMPActive(dot1agCfmStackObj_t *pMPObj)
{
#if OFDPA_NOT_SUPPORTED
  OFDPA_ERROR_t          result    = OFDPA_E_NONE;
#endif
  dot1agCfmSMEventMsg_t  eventMsg;
  uint32_t               rmepId    = 0;
  dot1agCfmMdTable_t    *mdEntry   = NULL;
  dot1agCfmMaNetTable_t *maEntry   = NULL;
  dot1agCfmMepTable_t   *mepEntry  = NULL;
  dot1agCfmRMepObj_t    *pRMEPObj  = NULL;
  OFDPA_BOOL             hwEnabled = OFDPA_FALSE;
  uint32_t               pri;

  if (pMPObj->operMode == OFDPA_TRUE)
  {
    /* Already active */
    return;
  }

  if ((pMPObj->mepData != NULL && pMPObj->mepData->dot1agCfmMepActive != OFDPA_TRUE) ||
      DOT1AG_CFG->dot1agCfmAdminMode != OFDPA_TRUE)
  {
    /* MEP is admin disabled */
    return;
  }

  pMPObj->operMode = OFDPA_TRUE;
  eventMsg.pMPObj = pMPObj;

  /* Initialize all state machines */
  if (pMPObj->mepData)
  {
    mepEntry = pMPObj->mepData;

    if ((eventMsg.pMPObj->mepData != NULL) &&
        (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
    {
      hwEnabled = OFDPA_TRUE;
    }
    else
    {
      hwEnabled = OFDPA_FALSE;
    }

    if (OFDPA_TRUE == hwEnabled)
    {
      /* Hardware enabled Dot1AG
       * install hardware entry only MEP operMode is active
       */
      if (NULL == mepEntry->maData ||
          NULL == mepEntry->maData->mdData)
      {
        return;
      }

      maEntry = mepEntry->maData;
      mdEntry = maEntry->mdData;
//      ccmInterval = CCMtime(maEntry->dot1agCfmMaNetCcmInterval, hwEnabled);
      rmepId = 0;

      /* Disable CCM transmission when MEP is in disabled state */
      if (mepEntry->dot1agCfmMepCCIVar.cciEnabled == OFDPA_FALSE)
      {
//        ccmInterval = DOT1AG_CCM_intervalInvalid;
      }
#if OFDPA_NOT_SUPPORTED
      if (nimGetIntfType(pMPObj->key.dot1agCfmStackifIndex, &sysIntfType)
          == OFDPA_E_NONE)
      {
        /* Hardware may not be aware of LAG by the time
         * application configures registers in hardware
         */
        if (L7_LAG_INTF == sysIntfType)
          osapiSleep(1);
      }
      if (dtlDot1agMEPAdd(pMPObj->key.dot1agCfmStackifIndex,
                          mdEntry->dot1agCfmMdIndex,
                          maEntry->dot1agCfmMaIndex,
                          mdEntry->dot1agCfmMdMdLevel,
                          mepEntry->dot1agCfmMepIdentifier,
                          mepEntry->dot1agCfmMepPrimaryVid,
                          ccmInterval) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to add MEP entry to hardware or mdIndex %d "
                           "maIndex %d mepId %d\r\n", mdEntry->dot1agCfmMdIndex,
                           maEntry->dot1agCfmMaIndex, mepEntry->dot1agCfmMepIdentifier);
        return;
      }
#endif
      /* Install RMEP entries in hardware
       * corresponding for this MEP
       */
      while ((pRMEPObj = dot1agCfmRMepObjGet(mdEntry->dot1agCfmMdIndex,
                                             maEntry->dot1agCfmMaIndex,
                                             mepEntry->dot1agCfmMepIdentifier,
                                             rmepId, AVL_NEXT)) != NULL)
      {
        /* Check if no more RMEPs exist for this (mdIndex, maIndex, mepId)
         * combination
         */
        if (pRMEPObj->key.dot1agCfmMdIndex       != mdEntry->dot1agCfmMdIndex ||
            pRMEPObj->key.dot1agCfmMaIndex       != maEntry->dot1agCfmMaIndex ||
            pRMEPObj->key.dot1agCfmMepIdentifier != mepEntry->dot1agCfmMepIdentifier)
        {
          break;
        }
        else if (pRMEPObj->key.dot1agCfmMepDbRMepIdentifier == rmepId)
        {
          break;
        }
        rmepId = pRMEPObj->key.dot1agCfmMepDbRMepIdentifier;
#if OFDPA_NOT_SUPPORTED
        if (dtlDot1agRemoteMEPAdd(pMPObj->key.dot1agCfmStackifIndex,
                                  mdEntry->dot1agCfmMdIndex,
                                  maEntry->dot1agCfmMaIndex,
                                  mdEntry->dot1agCfmMdMdLevel,
                                  mepEntry->dot1agCfmMepIdentifier,
                                  pRMEPObj->key.dot1agCfmMepDbRMepIdentifier,
                                  mepEntry->dot1agCfmMepPrimaryVid,
                                  ccmInterval) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to add RMEP  entry to hardware for mdIndex %d "
                             "maIndex %d mepId %d rmepId %d\r\n", mdEntry->dot1agCfmMdIndex,
                             maEntry->dot1agCfmMaIndex, mepEntry->dot1agCfmMepIdentifier,
                             pRMEPObj->key.dot1agCfmMepDbRMepIdentifier);
          return;
        }
#endif
        pRMEPObj->hwExist = OFDPA_TRUE;
      } /* end of while */
    }
    else                                /* dot1agIsMepHwEnabled */
    {
      /* Connectivity Check Initiator state Machine */
      pMPObj->mepData->dot1agCfmMepCCIState = DOT1AG_MEP_CCI_IDLE;
      /* 802.1ag d8-1 20.10.2 */
      pMPObj->mepData->dot1agCfmMepCCIVar.cciSentCCMs = 1;
      eventMsg.event = dot1agCCIBegin;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
      if (pMPObj->mepData->dot1agCfmMepCCIVar.cciEnabled == OFDPA_TRUE)
      {
        eventMsg.event = dot1agCCIEnabled;
        (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
      }

      /* MEP Connectivity Check Receiver state Machine */
      pMPObj->mepData->dot1agCfmMepCCRState = DOT1AG_MEP_CCR_RESET;
      eventMsg.event = dot1agCCRBegin;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      /* RMEP Error state Machine */
      pMPObj->mepData->dot1agCfmRmepErrState = DOT1AG_RMEP_ERRCCM_IDLE;
      eventMsg.pMepData = pMPObj->mepData;
      eventMsg.event = dot1agRmepErrBegin;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      /* xConnect state Machine */
      pMPObj->mepData->dot1agCfmMepXConState = DOT1AG_MEP_XCON_IDLE;
      eventMsg.event = dot1agXConMepBegin;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      /* y1731 */
      pMPObj->mepData->y1731DefectState[Y1731_DEFECT_TYPE_RDI] = Y1731_MEP_DEFECT_NO_DEFECT;
      pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_RDI].y1731DefCcmDefect = OFDPA_FALSE;
      pMPObj->mepData->y1731DefectState[Y1731_DEFECT_TYPE_LOC] = Y1731_MEP_DEFECT_NO_DEFECT;
      pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_LOC].y1731DefCcmDefect = OFDPA_FALSE;

      pMPObj->mepData->y1731DefectState[Y1731_DEFECT_TYPE_UNM] = Y1731_MEP_DEFECT_IDLE;
      eventMsg.event = y1731dUNMMepBegin;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      pMPObj->mepData->y1731DefectState[Y1731_DEFECT_TYPE_UNP] = Y1731_MEP_DEFECT_IDLE;
      eventMsg.event = y1731dUNPMepBegin;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      pMPObj->mepData->y1731DefectState[Y1731_DEFECT_TYPE_MMG] = Y1731_MEP_DEFECT_IDLE;
      eventMsg.event = y1731dMMGMepBegin;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      pMPObj->mepData->y1731DefectState[Y1731_DEFECT_TYPE_UNL] = Y1731_MEP_DEFECT_IDLE;
      eventMsg.event = y1731dUNLMepBegin;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      pMPObj->mepData->y1731DefectState[Y1731_DEFECT_TYPE_UNPTR] = Y1731_MEP_DEFECT_IDLE;
      eventMsg.event = y1731dUNPtrMepBegin;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
    }

    /* Loopback Initiator state Machine */
    pMPObj->mepData->dot1agCfmMepTransmitLbmStatus = OFDPA_TRUE;
    pMPObj->mepData->dot1agCfmMepLbiState = DOT1AG_LBI_IDLE;
    eventMsg.event = dot1agLBIBegin;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    /* Loopback Initiator Responder state Machine */
    pMPObj->mepData->dot1agCfmMepLbiVar.nextLbmTransId = 0;
    pMPObj->mepData->dot1agCfmMepLBIRState = DOT1AG_LBIR_IDLE;
    eventMsg.event = dot1agLBIRBegin;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    /* FNG State Machine */
    pMPObj->mepData->dot1agCfmMepSMFngState = DOT1AG_FNG_RESET;
    eventMsg.event = dot1agFNGBegin;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
    MAdefectIndicationNotify(pMPObj->mepData);

    /* PROLM State Machine */
    pMPObj->mepData->y1731CfmMepProlmState = Y1731_PROLM_DISABLE;
    pMPObj->mepData->y1731CfmMepProlmEntry = NULL;
    for (pri = 0; pri < 8; pri++)
    {
      pMPObj->mepData->y1731CfmOndemLMMState[pri] = Y1731_ONDEMLMM_IDLE;
      pMPObj->mepData->y1731CfmLMRState[pri] = Y1731_LMR_IDLE;
      pMPObj->mepData->y1731CfmMepOndemlmEntry[pri] = NULL;
      pMPObj->mepData->y1731CfmMepDmEntry[pri] = NULL;
    }

    pMPObj->mepData->y1731CfmMepAisSendState = Y1731_AIS_SEND_STOP;
    eventMsg.event = y1731AisSendBegin;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    pMPObj->mepData->y1731CfmMepAisRcvState = Y1731_AIS_RCV_IDLE;
    eventMsg.event = y1731AisRcvBegin;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    pMPObj->mepData->y1731CfmMepLckSendState = Y1731_LCK_SEND_STOP;
    eventMsg.event = y1731LckSendBegin;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    pMPObj->mepData->y1731CfmMepLckRcvState = Y1731_LCK_RCV_IDLE;
    eventMsg.event = y1731LckRcvBegin;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    pMPObj->mepData->y1731CfmMepCsfSendState = Y1731_CSF_SEND_STOP;
    eventMsg.event = y1731CsfSendBegin;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    pMPObj->mepData->y1731CfmMepCsfRcvState = Y1731_CSF_RCV_IDLE;
    eventMsg.event = y1731CsfRcvBegin;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    /* Apply LBM/LBR policy on port.
     * For hardware MEPs (with MEP Id 1-512), trap LBRs to CPU
     * For software MEPs (with MEP Id 513-8191), trap LBMs and LBRs to CPU
     */
    DOT1AG_TRACE(CFM_LBM, "Applying %s policy to port %d",
                 (OFDPA_TRUE == hwEnabled) ? "LBR":"LBM, LBR",
                 pMPObj->key.dot1agCfmStackifIndex);

#if OFDPA_NOT_SUPPORTED
    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    {
      result = dtlDot1agLBMPolicyApply(pMPObj->key.dot1agCfmStackifIndex,
                                       (OFDPA_TRUE == hwEnabled) ?
                                       OFDPA_DOT1AG_LBR_TRAP_TO_CPU :
                                       OFDPA_DOT1AG_LBM_LBR_TRAP_TO_CPU);
    }
    else
    {
      result = OFDPA_E_NONE;
    }

    if (OFDPA_E_NONE != result)
    {
      if (mepEntry)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to apply LBM/LBR policy on interface %d for MEP %d\r\n",
                           pMPObj->key.dot1agCfmStackifIndex,
                           mepEntry->dot1agCfmMepIdentifier);
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to apply LBM/LBR policy on interface %d\r\n",
                           pMPObj->key.dot1agCfmStackifIndex);
      }
    }
#endif
  }
  else
  {
    /* Loopback Responder state Machine */
    pMPObj->dot1agCfmLbrMpState = DOT1AG_LBR_IDLE;
    eventMsg.event = dot1agLBRBegin;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    /* Apply LBM, LBR and LTR policies on port */
#if OFDPA_NOT_SUPPORTED
    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    {
      result = dtlDot1agLBMPolicyApply(pMPObj->key.dot1agCfmStackifIndex,
                                       OFDPA_DOT1AG_LBM_LBR_TRAP_TO_CPU);
    }
    else
    {
      result = OFDPA_E_NONE;
    }

    if (OFDPA_E_NONE != result)
    {
      if (mepEntry)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to apply LBM, LBR, LTR policies on interface %d "
                           "for MEP %d\r\n", pMPObj->key.dot1agCfmStackifIndex,
                           mepEntry->dot1agCfmMepIdentifier);
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to apply LBM, LBR, LTR policies on interface %d\r\n",
                           pMPObj->key.dot1agCfmStackifIndex);
      }
    }
#endif
  }
}

/*********************************************************************
* @purpose  To Make MP operationally inactive
*
* @param    pMPObj   @b{(input)} Reference to maintenance point object
*
* @returns  none
*
* @comments
*
* @end
*********************************************************************/
void  dot1agCfmMPInActive(dot1agCfmStackObj_t *pMPObj)
{
#if OFDPA_NOT_SUPPORTED
  OFDPA_ERROR_t          result    = OFDPA_E_NONE;
#endif
  dot1agCfmSMEventMsg_t  eventMsg;
  uint32_t               rmepId    = 0;
  dot1agCfmMdTable_t    *mdEntry   = NULL;
  dot1agCfmMaNetTable_t *maEntry   = NULL;
  dot1agCfmMepTable_t   *mepEntry  = NULL;
  dot1agCfmRMepObj_t    *pRMEPObj  = NULL;
//  dot1dTpFdbData_t data;
//  uint16_t shortVal;
//  uint8_t macBuff[L7_FDB_KEY_SIZE];
//  L7_enetMacAddr_t mpMacAddr;
  OFDPA_BOOL             hwEnabled = OFDPA_FALSE;

  if (pMPObj->operMode == OFDPA_FALSE)
  {
    /* Not active */
    return;
  }

  pMPObj->operMode = OFDPA_FALSE;

  /* Reset all the SMs */
  eventMsg.pMPObj = pMPObj;

  if (pMPObj->mepData != NULL)
  {
    mepEntry = pMPObj->mepData;

    if ((eventMsg.pMPObj->mepData != NULL) &&
        (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
    {
      hwEnabled = OFDPA_TRUE;
    }
    else
    {
      hwEnabled = OFDPA_FALSE;
    }

    if (OFDPA_TRUE == hwEnabled)
    {
      /* Hardware enabled dot1ag
       * Remove entries from hardware
       */
      maEntry = mepEntry->maData;
      mdEntry = maEntry->mdData;
      rmepId = 0;

      DOT1AG_TRACE(CFM_HW_EVENTS, "Removing hardware entries for MEP and RMEP"
                                  " with Vlan %d MEPId %d MA %d MD %d",
                   mepEntry->dot1agCfmMepPrimaryVid,
                   mepEntry->dot1agCfmMepIdentifier,
                   maEntry->dot1agCfmMaIndex, mdEntry->dot1agCfmMdMdLevel);

      /* Remove Hardware RMEP entries corresponding to MEP
       */
      while ((pRMEPObj = dot1agCfmRMepObjGet(mdEntry->dot1agCfmMdIndex,
                                             maEntry->dot1agCfmMaIndex,
                                             mepEntry->dot1agCfmMepIdentifier,
                                             rmepId, AVL_NEXT)) != NULL)
      {
        /* Check if no more RMEPs exist for this (mdIndex, maIndex, mepId) combination */
        if (pRMEPObj->key.dot1agCfmMdIndex != mdEntry->dot1agCfmMdIndex ||
            pRMEPObj->key.dot1agCfmMaIndex != maEntry->dot1agCfmMaIndex ||
            pRMEPObj->key.dot1agCfmMepIdentifier != mepEntry->dot1agCfmMepIdentifier)
        {
          break;
        }
        else if (pRMEPObj->key.dot1agCfmMepDbRMepIdentifier == rmepId)
        {
          break;
        }
        rmepId = pRMEPObj->key.dot1agCfmMepDbRMepIdentifier;
        if (pRMEPObj->hwExist == OFDPA_TRUE)
        {
#if OFDPA_NOT_SUPPORTED
          if (dtlDot1agRemoteMEPDelete(mdEntry->dot1agCfmMdIndex,
                                       maEntry->dot1agCfmMaIndex,
                                       mepEntry->dot1agCfmMepIdentifier,
                                       pRMEPObj->key.dot1agCfmMepDbRMepIdentifier)
              != OFDPA_E_NONE)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                               "Failed to delete RMEP  entry from hardware for mdIndex %d maIndex %d mepId %d rmepId %d\r\n",
                               mdEntry->dot1agCfmMdIndex, maEntry->dot1agCfmMaIndex,
                               mepEntry->dot1agCfmMepIdentifier, pRMEPObj->key.dot1agCfmMepDbRMepIdentifier);
            return;

          }
          pRMEPObj->hwExist = OFDPA_FALSE;
#endif
        }

        DOT1AG_TRACE(CFM_HW_EVENTS, "Deleted hardware entry for RMEP with Vlan "
                                    "%d MEPId %d MA %d MD %d",
                     mepEntry->dot1agCfmMepPrimaryVid,
                     mepEntry->dot1agCfmMepIdentifier,
                     maEntry->dot1agCfmMaIndex, mdEntry->dot1agCfmMdMdLevel);
      }/* end of while*/

      DOT1AG_TRACE(CFM_HW_EVENTS, "Deleting hardware entry for MEP with Vlan "
                                  "%d MEPId %d MA %d MD %d", mepEntry->dot1agCfmMepPrimaryVid,
                   mepEntry->dot1agCfmMepIdentifier, maEntry->dot1agCfmMaIndex,
                   mdEntry->dot1agCfmMdMdLevel);
#if OFDPA_NOT_SUPPORTED
      /* Remove the MEP entry from hardware */
      if (dtlDot1agMEPDelete(mdEntry->dot1agCfmMdIndex,
                             maEntry->dot1agCfmMaIndex,
                             mepEntry->dot1agCfmMepIdentifier)
          != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete MEP entry to hardware or mdIndex %d maIndex %d mepId %d\r\n",
                           mdEntry->dot1agCfmMdIndex, maEntry->dot1agCfmMaIndex,
                           mepEntry->dot1agCfmMepIdentifier);
        return;
      }
#endif
#if OFDPA_NOT_SUPPORTED
      shortVal = mepEntry->dot1agCfmMepPrimaryVid;
      memcpy(macBuff, &shortVal, L7_MFDB_VLANID_LEN);
      memcpy(mpMacAddr.addr, pMPObj->mpMacAddress, sizeof(mpMacAddr.addr));
      memcpy(&macBuff[L7_MFDB_VLANID_LEN], mpMacAddr.addr, OFDPA_MAC_ADDR_LEN);
      memset(&data, 0x0, sizeof(data));
      if (fdbFind(macBuff, L7_MATCH_EXACT, &data) == OFDPA_E_NONE &&
          memcmp(macBuff, data.dot1dTpFdbAddress, L7_FDB_KEY_SIZE) == 0)
      {
        /* Delete static L2 entry (Maintenance point's MAC address)
         * with L3 bit set
         */
        DOT1AG_TRACE(CFM_LBM, "Deleting static L2 entry with L3 bit set"
                              " from hardware for %.2x:%.2x:%.2x:%.2x:%.2x:%.2x:"
                              " Vlan %d", mpMacAddr.addr[0], mpMacAddr.addr[1],
                     mpMacAddr.addr[2], mpMacAddr.addr[3], mpMacAddr.addr[4],
                     mpMacAddr.addr[5], mepEntry->dot1agCfmMepPrimaryVid);

        if (dtlFdbMacAddrDelete(mpMacAddr.addr, pMPObj->key.dot1agCfmStackifIndex,
                                mepEntry->dot1agCfmMepPrimaryVid, L7_FDB_ADDR_FLAG_DOT1AG_STATIC,
                                NULL) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to delete static L2 entry from hardware\r\n", 0);
        }
        else
        {
          fdbDelete(mpMacAddr.addr, mepEntry->dot1agCfmMepPrimaryVid);
        }
      }
      else
      {
        /* L2 entry with L3 bit is already deleted */
        DOT1AG_TRACE(CFM_LBM, "Static L2 entry with L3 bit set is already "
                              "deleted in hardware for %.2x:%.2x:%.2x:%.2x:%.2x:%.2x:"
                              " Vlan %d", mpMacAddr.addr[0], mpMacAddr.addr[1],
                     mpMacAddr.addr[2], mpMacAddr.addr[3], mpMacAddr.addr[4],
                     mpMacAddr.addr[5], mepEntry->dot1agCfmMepPrimaryVid);
      }
#endif
    }
    else
    {
      /* Connectivity Check Initiator state Machine */
      eventMsg.event = dot1agCCINotMepActive;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      /* MEP Connectivity Check Receiver state Machine */
      eventMsg.event = dot1agCCRNotMepActive;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      /* RMEP Error state Machine */
      eventMsg.event = dot1agRmepErrNotMepActive;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      /* xConnect state Machine */
      eventMsg.event = dot1agXConNotNMepActive;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      eventMsg.event = y1731dUNMNotNMepActive;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      eventMsg.event = y1731dUNPNotNMepActive;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      eventMsg.event = y1731dMMGNotNMepActive;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      eventMsg.event = y1731dUNLNotNMepActive;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      eventMsg.event = y1731dUNPtrNotNMepActive;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    }
    /* Loopback Initiator state Machine */
    eventMsg.event = dot1agLBINotMepActive;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    /* Loopback Initiator Responder state Machine */
    eventMsg.event = dot1agLBIRNotMepActive;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    /* FNG State Machine */
    eventMsg.event = dot1agFNGNotMepActive;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    /* PROLM State Machine */
    eventMsg.event = y1731PROLMNotMepActive;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    eventMsg.event = y1731AisSendNotMepActive;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    eventMsg.event = y1731AisRcvNotMepActive;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    eventMsg.event = y1731LckSendNotMepActive;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    eventMsg.event = y1731LckRcvNotMepActive;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    eventMsg.event = y1731CsfSendNotMepActive;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    eventMsg.event = y1731CsfRcvNotMepActive;
    eventMsg.pMepData = pMPObj->mepData;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    /* RMEP State Machine  - Delete all RMEP entries for
       this  MEPID only for software enabled dot1ag*/
    if (OFDPA_FALSE == hwEnabled)
    {
      if (dot1agRmepDelete(pMPObj->mepData->maData->mdData->dot1agCfmMdIndex,
                           pMPObj->mepData->maData->dot1agCfmMaIndex,
                           pMPObj->mepData->dot1agCfmMepIdentifier) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to delete RMEP entries for mdIndex %d maIndex %d mepId %d\r\n",
                           pMPObj->mepData->maData->mdData->dot1agCfmMdIndex, pMPObj->mepData->maData->dot1agCfmMaIndex,
                           pMPObj->mepData->dot1agCfmMepIdentifier);
      }
      MAdefectIndicationNotify(pMPObj->mepData);
    }

#if OFDPA_NOT_SUPPORTED
    /* Remove LBM/LBR policies on port */
    DOT1AG_TRACE(CFM_LBM, "Removing %s policy from port %d",
                 (OFDPA_TRUE == hwEnabled) ? "LBR":"LBM, LBR",
                 pMPObj->key.dot1agCfmStackifIndex);

    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    {
      result = dtlDot1agLBMPolicyRemove(pMPObj->key.dot1agCfmStackifIndex,
                                        (OFDPA_TRUE == hwEnabled) ?
                                        OFDPA_DOT1AG_LBR_TRAP_TO_CPU :
                                        OFDPA_DOT1AG_LBM_LBR_TRAP_TO_CPU);
    }
    else
    {
      result = OFDPA_E_NONE;
    }

    if (OFDPA_E_NONE != result)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to delete LBM policy on interface %d for MEP %d\r\n",
                         pMPObj->key.dot1agCfmStackifIndex,
                         pMPObj->mepData->dot1agCfmMepIdentifier);
    }
#endif
  }
  else
  {
    /* Loopback Responder state Machine */
    eventMsg.event = dot1agLBRNotMepActive;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

#if OFDPA_NOT_SUPPORTED
    /* Remove LBM, LBR and LTR policies on port */
    DOT1AG_TRACE(CFM_LBM, "Removing LBM, LBR and LTR policies from port %d",
                 pMPObj->key.dot1agCfmStackifIndex);
    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    {
      result = dtlDot1agLBMPolicyRemove(pMPObj->key.dot1agCfmStackifIndex,
                                        OFDPA_DOT1AG_LBM_LBR_TRAP_TO_CPU);
    }
    else
    {
      result = OFDPA_E_NONE;
    }

    if (OFDPA_E_NONE != result)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to remove LBM, LBR and LTR policies on interface %d",
                         pMPObj->key.dot1agCfmStackifIndex);

    }
#endif
  }
  /* Clear RMEP data base associated with this MP */
  /* Clear LTR data base associated with this MP */
}

/*********************************************************************
* @purpose  Compares transmit path of CFM objects.
*
* @param    pFirstOamMapData  @b{(input)} first OAM map data
* @param    pSecondOamMapData  @b{(input)} second OAM map data
*
* @returns  -1 - second object is one label below first object
*            1 - second object is one label above first object
*            0 - for all other cases.
*
* @comments
*
* @end
*********************************************************************/
int dot1agCfmCompareOamMapData(ofdbOamLmepData_t *pFirstOamMapData, ofdbOamLmepData_t *pSecondOamMapData)
{
  int result = 0, i = 0;
  int transmitPathNotSame = 0;

  if ((pFirstOamMapData->numLabels == (pSecondOamMapData->numLabels + 1)) ||
      (pFirstOamMapData->numLabels == (pSecondOamMapData->numLabels - 1)))
  {
    /* Compare MPLS headers */
    if ((pFirstOamMapData->oamType != OFDB_OAM_TYPE_SECTION) && 
        (pFirstOamMapData->numLabels == (pSecondOamMapData->numLabels + 1)))
    {
      for (i = 1; i < pSecondOamMapData->numLabels; i++)
      {
        if (pFirstOamMapData->mplsHeader[i] != pSecondOamMapData->mplsHeader[i])
        {
          transmitPathNotSame = 1;
          break;
        }
      }
      if (transmitPathNotSame != 1)
      {
        result = -1;
      }
    }
    else if (pFirstOamMapData->numLabels == (pSecondOamMapData->numLabels - 1))
    {
      for (i = 1; i < pFirstOamMapData->numLabels; i++)
      {
        if (pFirstOamMapData->mplsHeader[i] != pSecondOamMapData->mplsHeader[i])
        {
          transmitPathNotSame = 1;
          break;
        }
      }
      if (transmitPathNotSame != 1)
      {
        result = 1;
      }
    }
  }
  return result;
}

/*********************************************************************
* @purpose  To create a entry in Client MP DB and also update existing
*           entries in Client MP DB.
*
* @param    pMpObj  @b{(input)} CFM object.
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/

OFDPA_ERROR_t dot1agCfmClientMPDBEntryAdd(dot1agCfmStackObj_t *pMpObj)
{
  ofdbOamLmepData_t       *pFirstOamMapData = NULL, *pSecondOamMapData = NULL;
  uint32_t                 intIfNum, vlanId, direction, level;
  uint32_t                 mhfIntIfNum, mhfVlanId, mhfDirection, mhfLevel;
  dot1agCfmStackObj_t     *pSecondMPObj = NULL;
  dot1agCfmStackObj_t     *pMhfSecondMPObj = NULL;
  OFDPA_DOT1AG_MP_type_t   secondMPType;
  OFDPA_DOT1AG_MP_type_t   mhfSecondMPType;
  int8_t                   megLevel = 0;
  dot1agCfmClientMPDBObj_t clientMPDBEntry;
  OFDPA_ERROR_t            rc = OFDPA_E_NONE;

  /* Retrieve Injected Oam Mapping entry for incoming MP object*/
  rc = ofdbInjectedOamLmepIdMapDataGet(pMpObj->ofdpaLocalMpId, &pFirstOamMapData);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  /* Run through CFM stack objects */
  intIfNum = vlanId = direction = level = 0;
  while ((pSecondMPObj = dot1agCfmStackObjGet(intIfNum, vlanId, level, direction, AVL_NEXT)) != NULL)
  {
    megLevel = 0;

    /* Retrieve Injected Oam Mapping entry for second MP*/
    if (ofdbInjectedOamLmepIdMapDataGet(pSecondMPObj->ofdpaLocalMpId, &pSecondOamMapData) == OFDPA_E_NONE)
    {
      if ((dot1agMpTypeGet(pSecondMPObj, &secondMPType) == OFDPA_E_NONE) &&
          (OFDPA_DOT1AG_MP_TYPE_BHH == secondMPType))
      {
        /*
          If first MEP is a BHH Section MEP, then compare refGroupIds of Injected OAM entries corresponding to 
          first and second MEPs and if refGroupIds are same, then compare the transmit paths.
          If first MEP is a BHH Section MEP then dot1agCfmCompareOamMapData API will either return 1 or 0 as
          Section MEP can never be a client.
        */
        if ((pFirstOamMapData->oamType == OFDB_OAM_TYPE_SECTION) &&
            (pMpObj->ofdpaLocalMpId != pSecondMPObj->ofdpaLocalMpId))
        {
          if (ofdbInjectedOamCompareRefGroupId(pMpObj->ofdpaLocalMpId, pSecondMPObj->ofdpaLocalMpId))
          {
            megLevel = dot1agCfmCompareOamMapData(pFirstOamMapData, pSecondOamMapData);
          }
        }
        else
        {
          /* Section MEP can never be a client */
          if (pSecondOamMapData->oamType != OFDB_OAM_TYPE_SECTION)
          {
            /* Compare transmit path of first and second CFM objects */
            megLevel = dot1agCfmCompareOamMapData(pFirstOamMapData, pSecondOamMapData);
          }
        }

        /*
          megLevel = 1 means second CFM object has one and only one MPLS label in addition to transmit path of first CFM object
          megLevel = -1 means second CFM object has one and only one MPLS label in less to transmit path of first CFM object
          megLevel = 0 means MPLS label stack of first and second CFM objects are not neither superset nor subset of each other.
        */
        if (megLevel == 1)
        {
          if (NULL == pSecondMPObj->mepData) /* Second MP object is a MIP */
          {
            clientMPDBEntry.key.serverLmepId     = pMpObj->mepData->dot1agCfmMepIdentifier;
            clientMPDBEntry.key.clientLmepId     = pSecondMPObj->ofdpaLocalMpId;
            clientMPDBEntry.serverOfdpaLocalMpId = pMpObj->ofdpaLocalMpId;
            clientMPDBEntry.pClientMPObj         = pSecondMPObj;
            clientMPDBEntry.txDirSameAsServer    = OFDPA_TRUE;
            dpaRWlockWritelock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
            rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_CLIENT_MP_DB_TREE, &dot1agCFM.dot1agClientMPDBTree, &clientMPDBEntry);
            dpaRWlockUnlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);

            /* Retrieve the other MHF and add an entry into Second MP DB */
            mhfIntIfNum = mhfVlanId = mhfDirection = mhfLevel = 0;
            while ((pMhfSecondMPObj = dot1agCfmStackObjGet(mhfIntIfNum, mhfVlanId, mhfLevel, mhfDirection, AVL_NEXT)) != NULL)
            {
              if ((pMhfSecondMPObj->key.dot1agCfmStackMdLevel == pSecondMPObj->key.dot1agCfmStackMdLevel) &&
                  (pMhfSecondMPObj->ofdpaLocalMpId != pSecondMPObj->ofdpaLocalMpId))
              {
                if ((dot1agMpTypeGet(pMhfSecondMPObj, &mhfSecondMPType) == OFDPA_E_NONE) &&
                    (OFDPA_DOT1AG_MP_TYPE_BHH == mhfSecondMPType))
                {
                  clientMPDBEntry.key.serverLmepId     = pMpObj->mepData->dot1agCfmMepIdentifier;
                  clientMPDBEntry.key.clientLmepId     = pMhfSecondMPObj->ofdpaLocalMpId;
                  clientMPDBEntry.serverOfdpaLocalMpId = pMpObj->ofdpaLocalMpId;
                  clientMPDBEntry.pClientMPObj         = pMhfSecondMPObj;
                  clientMPDBEntry.txDirSameAsServer    = OFDPA_FALSE;
                  dpaRWlockWritelock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
                  rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_CLIENT_MP_DB_TREE, &dot1agCFM.dot1agClientMPDBTree, &clientMPDBEntry);
                  dpaRWlockUnlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
                  break;
                }
              }

              mhfIntIfNum  = pMhfSecondMPObj->key.dot1agCfmStackifIndex;
              mhfVlanId    = pMhfSecondMPObj->key.dot1agCfmStackVlanIdOrNone;
              mhfLevel     = pMhfSecondMPObj->key.dot1agCfmStackMdLevel;
              mhfDirection = pMhfSecondMPObj->key.dot1agCfmStackDirection; 
            }
          }
          else /* Second MP object is a MEP */
          {
            clientMPDBEntry.key.serverLmepId     = pMpObj->mepData->dot1agCfmMepIdentifier;
            clientMPDBEntry.key.clientLmepId     = pSecondMPObj->mepData->dot1agCfmMepIdentifier;
            clientMPDBEntry.serverOfdpaLocalMpId = pMpObj->ofdpaLocalMpId;
            clientMPDBEntry.pClientMPObj         = pSecondMPObj;
            clientMPDBEntry.txDirSameAsServer    = OFDPA_TRUE;
            dpaRWlockWritelock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
            rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_CLIENT_MP_DB_TREE, &dot1agCFM.dot1agClientMPDBTree, &clientMPDBEntry);
            dpaRWlockUnlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
          }
        }
        else if (megLevel == -1)
        {
          clientMPDBEntry.key.serverLmepId     = pSecondMPObj->mepData->dot1agCfmMepIdentifier;
          clientMPDBEntry.key.clientLmepId     = pMpObj->mepData->dot1agCfmMepIdentifier;
          clientMPDBEntry.serverOfdpaLocalMpId = pSecondMPObj->ofdpaLocalMpId;
          clientMPDBEntry.pClientMPObj         = pMpObj;
          dpaRWlockWritelock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
          rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_CLIENT_MP_DB_TREE, &dot1agCFM.dot1agClientMPDBTree, &clientMPDBEntry);
          dpaRWlockUnlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
        }
      }
    }

    intIfNum  = pSecondMPObj->key.dot1agCfmStackifIndex;
    vlanId    = pSecondMPObj->key.dot1agCfmStackVlanIdOrNone;
    level     = pSecondMPObj->key.dot1agCfmStackMdLevel;
    direction = pSecondMPObj->key.dot1agCfmStackDirection;
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To delete a entry in Client MP DB.
*
* @param    pMpObj  @b{(input)} CFM object.
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmClientMPDBEntryDelete(dot1agCfmStackObj_t *pMpObj)
{
  dot1agCfmClientMPDBObj_t clientMPDBEntry, *pClientMPDBObj = NULL;
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  /* Run through ClientMPDB and delete entries with any of the key parameters matching the CFM object */
  clientMPDBEntry.key.serverLmepId = 0;
  clientMPDBEntry.key.clientLmepId = 0;
  while ((pClientMPDBObj = dot1agCfmClientMPDBObjGet(clientMPDBEntry.key, AVL_NEXT)) != NULL)
  {
    if ((pClientMPDBObj->key.serverLmepId == pMpObj->mepData->dot1agCfmMepIdentifier) ||
        (pClientMPDBObj->key.clientLmepId == pMpObj->mepData->dot1agCfmMepIdentifier))
    {
      dpaRWlockWritelock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
      rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_CLIENT_MP_DB_TREE, &dot1agCFM.dot1agClientMPDBTree, pClientMPDBObj);
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
    }

    clientMPDBEntry.key.serverLmepId = pClientMPDBObj->key.serverLmepId;
    clientMPDBEntry.key.clientLmepId = pClientMPDBObj->key.clientLmepId;
  }

  return rc;
}

/*********************************************************************
* @purpose  Updates Client MP DB when a MIP is created or deleted.
*
* @param    pMPObj  @b{(input)} CFM object.
*           isMipDelete   @b{(input)} true if MIP is deleted
*                                     false if MIP is created
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmClientMPDBUpdate(dot1agCfmStackObj_t *pMPObj, OFDPA_BOOL isMipDelete)
{
  ofdbOamLmepData_t       *pServerOamMapData = NULL, *pOamMapData = NULL;
  uint32_t                 mhfIntIfNum, mhfVlanId, mhfDirection, mhfLevel;
  dot1agCfmStackObj_t     *pMhfMPObj = NULL, *pTmpMhfMPObj = NULL;
  OFDPA_DOT1AG_MP_type_t   mhfMPType;
  int8_t                   megLevel = 0;
  dot1agCfmClientMPDBObj_t clientMPDBEntry, *pClientMPDBObj = NULL;
  OFDPA_ERROR_t            rc = OFDPA_E_NONE;

  /* Retrieve the other MHF present in the same MEG domain*/
  mhfIntIfNum = mhfVlanId = mhfDirection = mhfLevel = 0;
  while ((pTmpMhfMPObj = dot1agCfmStackObjGet(mhfIntIfNum, mhfVlanId, mhfLevel, mhfDirection, AVL_NEXT)) != NULL)
  {
    if ((pTmpMhfMPObj->key.dot1agCfmStackMdLevel == pMPObj->key.dot1agCfmStackMdLevel) &&
        (pTmpMhfMPObj->ofdpaLocalMpId != pMPObj->ofdpaLocalMpId))
    {
      if ((dot1agMpTypeGet(pTmpMhfMPObj, &mhfMPType) == OFDPA_E_NONE) &&
          (OFDPA_DOT1AG_MP_TYPE_BHH == mhfMPType))
      {
        pMhfMPObj = pTmpMhfMPObj;
        break;
      }
    }

    mhfIntIfNum  = pTmpMhfMPObj->key.dot1agCfmStackifIndex;
    mhfVlanId    = pTmpMhfMPObj->key.dot1agCfmStackVlanIdOrNone;
    mhfLevel     = pTmpMhfMPObj->key.dot1agCfmStackMdLevel;
    mhfDirection = pTmpMhfMPObj->key.dot1agCfmStackDirection; 
  }

  if (pMhfMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* Retrieve Injected Oam Mapping entry for incoming MP object*/
  rc = ofdbInjectedOamLmepIdMapDataGet(pMPObj->ofdpaLocalMpId, &pOamMapData);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  /* Run through ClientMPDB */
  clientMPDBEntry.key.serverLmepId = 0;
  clientMPDBEntry.key.clientLmepId = 0;
  while ((pClientMPDBObj = dot1agCfmClientMPDBObjGet(clientMPDBEntry.key, AVL_NEXT)) != NULL)
  {
    megLevel = 0;

    if (isMipDelete == OFDPA_FALSE)
    {
      /* Retrieve Injected Oam Mapping entry for server MEP*/
      if (ofdbInjectedOamLmepIdMapDataGet(pClientMPDBObj->serverOfdpaLocalMpId, &pServerOamMapData) == OFDPA_E_NONE)
      {
        /*
          If oamType of pServerOamMapData is a BHH Section, then compare refGroupIds of Injected OAM entries corresponding to 
          server MEP and incoming CFM object and if refGroupIds are same, then compare the transmit paths.
          If oamType of pServerOamMapData is a BHH Section, then dot1agCfmCompareOamMapData API will either return 1 or 0 as
          Section MEP can never be a client.
        */
        if ((pServerOamMapData->oamType == OFDB_OAM_TYPE_SECTION) &&
            (pClientMPDBObj->serverOfdpaLocalMpId != pMPObj->ofdpaLocalMpId))
        {
          if (ofdbInjectedOamCompareRefGroupId(pClientMPDBObj->serverOfdpaLocalMpId, pMPObj->ofdpaLocalMpId))
          {
            megLevel = dot1agCfmCompareOamMapData(pServerOamMapData, pOamMapData);
          }
        }
        else
        {
          /* Compare transmit path of first and second CFM objects */
          megLevel = dot1agCfmCompareOamMapData(pServerOamMapData, pOamMapData);
        }

        /*
          megLevel = 1 means incoming CFM object has one and only one MPLS label in addition to transmit path of server CFM object
          megLevel = -1 means incoming CFM object has one and only one MPLS label in less to transmit path of server CFM object
          megLevel = 0 means MPLS label stack of first and second CFM objects are not neither superset nor subset of each other.
        */
        if (megLevel == 1)
        {
          clientMPDBEntry.key.serverLmepId     = pClientMPDBObj->key.serverLmepId;
          clientMPDBEntry.key.clientLmepId     = pMPObj->ofdpaLocalMpId;
          clientMPDBEntry.serverOfdpaLocalMpId = pClientMPDBObj->serverOfdpaLocalMpId;
          clientMPDBEntry.pClientMPObj         = pMPObj;
          clientMPDBEntry.txDirSameAsServer    = OFDPA_TRUE;
          dpaRWlockWritelock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
          rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_CLIENT_MP_DB_TREE, &dot1agCFM.dot1agClientMPDBTree, &clientMPDBEntry);
          dpaRWlockUnlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
    
          clientMPDBEntry.key.serverLmepId     = pClientMPDBObj->key.serverLmepId;
          clientMPDBEntry.key.clientLmepId     = pMhfMPObj->ofdpaLocalMpId;
          clientMPDBEntry.serverOfdpaLocalMpId = pClientMPDBObj->serverOfdpaLocalMpId;
          clientMPDBEntry.pClientMPObj         = pMhfMPObj;
          clientMPDBEntry.txDirSameAsServer    = OFDPA_FALSE;
          dpaRWlockWritelock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
          rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_CLIENT_MP_DB_TREE, &dot1agCFM.dot1agClientMPDBTree, &clientMPDBEntry);
          dpaRWlockUnlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
        }
      }
    }
    else
    {
      if ((pClientMPDBObj->key.clientLmepId == pMPObj->ofdpaLocalMpId) ||
          (pClientMPDBObj->key.clientLmepId == pMhfMPObj->ofdpaLocalMpId))
      {
        dpaRWlockWritelock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
        rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_CLIENT_MP_DB_TREE, &dot1agCFM.dot1agClientMPDBTree, pClientMPDBObj);
        dpaRWlockUnlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
      }
    }

    clientMPDBEntry.key.serverLmepId = pClientMPDBObj->key.serverLmepId;
    clientMPDBEntry.key.clientLmepId = pClientMPDBObj->key.clientLmepId;
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To create an operational maintenance point and send events
*           to various state machines
*
* @param    intIfNum  @b{(input)} Index to indentify an interface
* @param    primVID   @b{(input)} VLAN ID associated with the MA
* @param    mdLevel   @b{(input)} Domain level associated with the MP
* @param    direction @b{(input)} Direction of MP
* @param    pMEPObj   @b{(input)} Reference to MEP data structure. Use
*                                 NULL for MIPs
*
* @returns  none
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMPInit(uint32_t intIfNum, uint32_t primVID,
                              uint8_t mdLevel, uint32_t direction,
                              uint8_t *macAddress,
                              dot1agCfmMepTable_t *pMEPObj,
                              uint32_t localMpId)
{
	return 0;
}

/*********************************************************************
* @purpose  To remove an operational maintenance point and send events
*           to various state machines
*
* @param    pMPObj   @b{(input)} Reference to maintenance point object
*
* @returns  none
*
* @comments
*
* @end
*********************************************************************/
void  dot1agCfmMPDestroy(dot1agCfmStackObj_t *pMPObj)
{
	return 0;
}

/*********************************************************************
* @purpose  To process MIP create event
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    mdLevel   @b{(input)} Domain level associated with the MIP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments We do not use mdIndex as in standard implementation there
*           can be multiple domains for a given level. Currently we
*           have only one domain per level.
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMIPCreateProcess(uint32_t mdIndex, uint32_t mdLevel,
                                        uint32_t intIfNum, uint8_t *macAddr)
{
  dot1agCfmMAObj_t *maObjEntry;
  uint32_t          maIndex;
  uint32_t          primVID;
  uint32_t          mdIdx;

  /* Find all the MAs associated with this mdIndex, mdLevel */
  /* Obtain the domain entry. */
  for (mdIdx = 0; mdIdx < dpaDot1agMaintenanceDomainsMaxGet(); mdIdx++)
  {
    if (DOT1AG_MD_CFG(mdIdx).active == DOT1AG_ROW_ACTIVE &&
        DOT1AG_MD_CFG(mdIdx).dot1agCfmMdMdLevel >= mdLevel)
    {
      mdIndex = DOT1AG_MD_CFG(mdIdx).dot1agCfmMdIndex;
      maIndex = 0;
      while ((maObjEntry = dot1agActiveMAGet(mdIndex, maIndex, AVL_NEXT)) != NULL)
      {
        /* Check if any such MA exists */
        if (maObjEntry->key.dot1agCfmMdIndex != mdIndex)
        {
          break;
        }
        maIndex = maObjEntry->key.dot1agCfmMaIndex;
        primVID = maObjEntry->maData->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId;
        /* Go ahead and try to create a CFM MP stack object */
        dot1agCfmMPInit(intIfNum, primVID, mdLevel, DOT1AG_MP_DIRECTION_UP, macAddr, NULL, 0);
      }/* End of MIP iterations */
    }/* End of matching domain check */
  }/* End of domain iterations */
  return OFDPA_E_NONE;
}

/*********************************************************************
 * @purpose  To process MIP delete event generated from UI or during
*           system init
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    mdLevel   @b{(input)} Domain level associated with the MIP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMIPDeleteProcess(uint32_t mdIndex, uint32_t mdLevel,
                                        uint32_t intIfNum, uint32_t primVID)
{
  dot1agCfmMAObj_t    *maObjEntry;
  uint32_t             maIndex;
  OFDPA_ERROR_t        rc     = OFDPA_E_NONE;
  uint32_t             mdIdx, vid;
  dot1agCfmStackObj_t *pMPObj = NULL;

  /* Find all the MAs associated with this mdIndex, mdLevel */
  /* Obtain the domain entry. */
  for (mdIdx = 0; mdIdx < dpaDot1agMaintenanceDomainsMaxGet(); mdIdx++)
  {
    if (DOT1AG_MD_CFG(mdIdx).active != DOT1AG_ROW_INACTIVE &&
        DOT1AG_MD_CFG(mdIdx).dot1agCfmMdMdLevel >= mdLevel)
    {
      if (primVID != 0)
      {
        /* check if an MA for this VID exists exists at a higher level domain.
           In that case do not delete MIP */
        if ((pMPObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel,
                                           DOT1AG_MP_DIRECTION_UP, AVL_EXACT)) != NULL)
        {
          /* Kick start any MIP related events here and clean up oper data */
          dot1agCfmMPDestroy(pMPObj);
        }
        break;
      }
      else
      {
        mdIndex = DOT1AG_MD_CFG(mdIdx).dot1agCfmMdIndex;
        maIndex = 0;
        while ((maObjEntry = dot1agActiveMAGet(mdIndex, maIndex,
                                               AVL_NEXT))
               != NULL)
        {
          /* Check if any such MA exists */
          if (maObjEntry->key.dot1agCfmMdIndex != mdIndex)
          {
            break;
          }
          maIndex = maObjEntry->key.dot1agCfmMaIndex;
          vid = maObjEntry->maData->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId;

          /* check if a MP object exists */
          if ((pMPObj = dot1agCfmStackObjGet(intIfNum, vid, mdLevel,
                                             DOT1AG_MP_DIRECTION_UP, AVL_EXACT)) != NULL)
          {
            /* Kick start any MIP related events here and clean up oper data */
            if (pMPObj->mepData == NULL)
            {
              dot1agCfmMPDestroy(pMPObj);
            }
          }
        }/* End of MA iterations */
      }
    }/* End of matching domain check */
  }/* End of domain iterations */
  return rc;
}

/*********************************************************************
* @purpose  To process MEP create event generated from UI or during
*           system init
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMEPCreateProcess(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t intIfNum, uint32_t localMpId)
{
  dot1agCfmMdTable_t *mdEntry;
  dot1agCfmMepObj_t  *mepObj = NULL;

  if (dot1agActiveMAGet(mdIndex, maIndex,
                        AVL_EXACT) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "MEP create process failed because of invalid domain %d maIndex %d\r\n",
                       mdIndex, maIndex);
    return OFDPA_E_FAIL;
  }

  if ((mdEntry = dot1agMdEntryGet(mdIndex, -1)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "MEP create process failed because of invalid domain %d\r\n", mdIndex);
    return OFDPA_E_FAIL;
  }

  if (((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId,
                                   intIfNum, AVL_EXACT)) == NULL) ||
      (mepObj->mepData == NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "MEP create process failed because of invalid mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId, intIfNum);
    return OFDPA_E_FAIL;
  }

  /* Go ahead and create a CFM MP stack object */
  dot1agCfmMPInit(intIfNum, mepObj->mepData->dot1agCfmMepPrimaryVid, mdEntry->dot1agCfmMdMdLevel,
                  mepObj->mepData->dot1agCfmMepDirection, mepObj->mepData->dot1agCfmMepMacAddress,
                  mepObj->mepData, localMpId);

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To process MEP delete event generated from UI or during
*           system init
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMEPDeleteProcess(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t intIfNum)
{
  dot1agCfmMdTable_t  *mdEntry;
  dot1agCfmMepObj_t   *mepObjEntry;
  dot1agCfmMepTable_t *mepEntry = NULL;
  dot1agCfmStackObj_t *pMPObj   = NULL;
  OFDPA_ERROR_t        rc       = OFDPA_E_NONE;

  /* Obtain the domain entry */
  if (((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) == NULL) ||
      (mepObjEntry->mepData == NULL) ||
      (mepObjEntry->mepData->maData == NULL) ||
      (mepObjEntry->mepData->maData->mdData == NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "MEP delete process failed because of invalid domain %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId, intIfNum);
    return OFDPA_E_FAIL;
  }

  mepEntry = mepObjEntry->mepData;
  mdEntry = mepObjEntry->mepData->maData->mdData;

  /* Generate any SM events for this MEP destruction */
  if ((pMPObj = dot1agCfmStackObjGet(intIfNum, mepEntry->dot1agCfmMepPrimaryVid,
                                     mdEntry->dot1agCfmMdMdLevel,
                                     mepEntry->dot1agCfmMepDirection,
                                     AVL_EXACT)) != NULL)
  {
  printf("\r\n dot1agCfmMEPDeleteProcess: ifnum:%d, vid:%d, level:%d, direction:%d \r\n", 
    intIfNum, mepEntry->dot1agCfmMepPrimaryVid, mdEntry->dot1agCfmMdMdLevel, mepEntry->dot1agCfmMepDirection);
    dot1agCfmMPDestroy(pMPObj);
  }

  /* Delete the RMEP Entries for the MEP only for hw Enabled dot1ag
   *  otherwise entries would have been deleted in dot1agCfmMPInActive
   */
  if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
  {
    if (dot1agRmepDelete(mdIndex, maIndex, mepId) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Error deleting RMep Entries from database for domain %d maIndex %d mepId %d \r\n",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_FAIL;
    }
  }

  /* Delete tree nodes */
  if (mepEntry->dot1agCfmMepCCIVar.cciTimer != NULL)
  {
    dot1agTimerDelete(mepEntry->dot1agCfmMepCCIVar.cciTimer);
    mepEntry->dot1agCfmMepCCIVar.cciTimer = NULL;
  }
  rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_MEP_OBJ_TREE,
                               &dot1agCFM.dot1agMepTree, mepObjEntry);
  if (rc == OFDPA_E_NONE)
  {
    /* Reset table index */
    dot1agCFM.dot1agMEPTableIndex = mepEntry - &(DOT1AG_MEP_TABLE(0));
    /* Wipe previous config */
    memset(mepEntry, 0x0, sizeof(*mepEntry));
    mepEntry->active = DOT1AG_ROW_INACTIVE;
  }
  else
  {
    /* Do not delete domain as operation is incomplete. This gives
       user another chance to try and delete the domain */
    mepEntry->active = DOT1AG_ROW_ACTIVE;
  }
  return rc;
}

/*********************************************************************
* @purpose  To set/reset MEP active mode generated from UI
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
* @param    mode      @b{(input)} Active mode set by the UI
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMEPActiveModeProcess(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t intIfNum,
                                            OFDPA_BOOL mode)
{
  dot1agCfmMdTable_t  *mdEntry;
  dot1agCfmMepObj_t   *mepObjEntry;
  dot1agCfmMepTable_t *mepEntry = NULL;
  dot1agCfmStackObj_t *pMPObj   = NULL;
  OFDPA_ERROR_t        rc       = OFDPA_E_NONE;

  /* Obtain the domain entry */
  if (((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) == NULL) ||
      (mepObjEntry->mepData == NULL) ||
      (mepObjEntry->mepData->maData == NULL) ||
      (mepObjEntry->mepData->maData->mdData == NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "MEP Activation failed. Invalid MEP obj domain %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId, intIfNum);
    return OFDPA_E_FAIL;
  }

  mepEntry = mepObjEntry->mepData;
  mdEntry = mepObjEntry->mepData->maData->mdData;

  if ((mode == OFDPA_TRUE) &&
      (dot1agIntfCanBeOperational(intIfNum) == OFDPA_FALSE) &&
      (mepEntry->dot1agCfmMepDirection == DOT1AG_MP_DIRECTION_DOWN))
  {
    /* Can not be activate at this point of time */
    return OFDPA_E_NONE;
  }
  /* Generate any SM events for this MEP destruction */
  if ((pMPObj = dot1agCfmStackObjGet(intIfNum, mepEntry->dot1agCfmMepPrimaryVid,
                                     mdEntry->dot1agCfmMdMdLevel,
                                     mepEntry->dot1agCfmMepDirection,
                                     AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "MEP Activation failed. Invalid stack obj intf %d primVID %d level %d direction %d\r\n",
                       intIfNum, mepEntry->dot1agCfmMepPrimaryVid,
                       mdEntry->dot1agCfmMdMdLevel,
                       mepEntry->dot1agCfmMepDirection);
    return OFDPA_E_FAIL;
  }

  if (mode == OFDPA_TRUE)
  {
    dot1agCfmMPActive(pMPObj);
  }
  else if (mode == OFDPA_FALSE)
  {
    dot1agCfmMPInActive(pMPObj);
  }
  return rc;
}

/*********************************************************************
* @purpose  To clear remote mep entries on a given mpid generated from UI
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRMEPDeleteProcess(uint32_t mdIndex, uint32_t maIndex,
                                         uint32_t mepId, uint32_t intIfNum)
{
  dot1agCfmMepObj_t   *mepObjEntry;
  dot1agCfmMepTable_t *mepEntry = NULL;
  OFDPA_ERROR_t        rc       = OFDPA_E_NONE;

  /* Obtain the domain entry */
  if (((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) == NULL) ||
      (mepObjEntry->mepData == NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "MEP delete process failed. Invalid mep obj domain %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId, intIfNum);
    return OFDPA_E_FAIL;
  }

  mepEntry = mepObjEntry->mepData;

  /* RMEP State Machine  - Delete all RMEP entries for this  MEPID*/
  if (dot1agRmepDelete(mdIndex, maIndex, mepId) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to delete RMEP entries for mdIndex %d maIndex %d mepId %d\r\n",
                       mdIndex, maIndex, mepId);
  }
  MAdefectIndicationNotify(mepEntry);

  return rc;
}

/*********************************************************************
* @purpose  To process MA create event generated from UI or during
*           system init
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMACreateProcess(uint32_t mdIndex, uint32_t maIndex)
{
#if OFDPA_NOT_SUPPORTED
  uint32_t               primVID;
  dot1agCfmMdTable_t    *mdEntry;
  dot1agCfmMaNetTable_t *maEntry          = NULL;
  dot1agCfmMAObj_t      *maObjEntry       = NULL;
  uint32_t               intIfNum, mdLevel, tmpLevel;
  NIM_INTF_MASK_t        portMask;
  int                    numPlatformPorts = -1;
  uint8_t                maId[OFDPA_DOT1AG_MAID_MAX_LEN];

  memset(maId, 0x0, OFDPA_DOT1AG_MAID_MAX_LEN);

  if ((mdEntry = dot1agMdEntryGet(mdIndex, -1)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "MA create process failed. Invalid domain %d\r\n", mdIndex);
    return OFDPA_E_FAIL;
  }

  /* Obtain the domain entry */
  if ((maObjEntry = dot1agActiveMAGet(mdIndex, maIndex,
                                      AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "MA create process failed. Invalid domain %d maIndex %d\r\n",
                       mdIndex, maIndex);
    return OFDPA_E_FAIL;
  }

  if (NULL == maObjEntry->maData)
  {
    return OFDPA_E_FAIL;
  }

  /* Find MIPs for this service at this or lower level and initialize them */
  maEntry = maObjEntry->maData;
  primVID = maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId;

  /* create a group in hardware*/
  if (DOT1AG_CFG->dot1agCfmAdminMode == OFDPA_TRUE)
  {
    if ((dot1agMAIDGet(mdEntry, maEntry, maId)) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                         "Error reading MAID for maintenance domain %d\r\n", mdIndex);
      return OFDPA_E_FAIL;
    }

    if (dtlDot1agMAGroupAdd( mdIndex, maIndex, maId, primVID)
        != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                         "Error creating domain %d association %d in hardware\r\n",
                         mdIndex, maIndex);
      return OFDPA_E_FAIL;
    }
  }

  mdLevel = mdEntry->dot1agCfmMdMdLevel;

  if (dot1qVlanEgressPortsGet(primVID, &portMask) == OFDPA_E_NONE)
  {
    numPlatformPorts = dpaPlatformNumPortsGet();

    if (-1 == numPlatformPorts)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to get number of ports.\r\n", 0);
      return OFDPA_E_FAIL;
    }

    for (intIfNum = 1; intIfNum <= numPlatformPorts; intIfNum++)
    {
      if (OFDPA_INTF_ISMASKBITSET(portMask, intIfNum))
      {
        for (tmpLevel = OFDPA_DOT1AG_MD_LEVEL_MIN; tmpLevel <= mdLevel; tmpLevel++)
        {
          if (OFDPA_INTF_ISMASKBITSET(DOT1AG_MIP_TABLE(tmpLevel).mipMode, intIfNum))
          {
            /* Go ahead and try to create a CFM MP stack object */
            dot1agCfmMPInit(intIfNum, primVID, tmpLevel,
                            DOT1AG_MP_DIRECTION_UP, NULL);
          }
        } /* End of MIP level check */
      } /* End of VLAN memer check */
    } /* End of interfaces loop */
  } /* End of vlan port mask get */
#endif
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To process MA Delete event generated from UI or during
*           system init
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMADeleteProcess(uint32_t mdIndex, uint32_t maIndex)
{
  dot1agCfmMepObj_t       *mepObjEntry;
  dot1agCfmMaNetTable_t   *maEntry      = NULL;
  dot1agCfmMAObj_t        *maObjEntry   = NULL;
  dot1agCfmMAObjPrimVID_t *maObjPrimVID = NULL;
  uint32_t                 primVID, mdLevel, mdIdx;
  uint32_t                 maLevel      = OFDPA_DOT1AG_MD_LEVEL_MIN;
  dot1agCfmMdTable_t      *mdEntry;
  dot1agCfmStackObj_t     *pMPObj       = NULL;
  OFDPA_BOOL               entryFound   = OFDPA_FALSE;
  uint32_t                 tmpLevel, tmpVID, intIfNum;
  Dot1agCfmMpDirection_t   direction;
  uint8_t                  maId[OFDPA_DOT1AG_MAID_MAX_LEN];
  OFDPA_ERROR_t            rc           = OFDPA_E_NONE;

  memset(maId, 0x0, OFDPA_DOT1AG_MAID_MAX_LEN);
  /* Obtain the domain entry */
  if ((mdEntry = dot1agMdEntryGet(mdIndex, -1)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "MA delete process failed. Invalid domain %d\r\n", mdIndex);
    return OFDPA_E_FAIL;
  }

  mdLevel = mdEntry->dot1agCfmMdMdLevel;
  /* Obtain the domain entry */
  if ((maObjEntry = dot1agActiveMAGet(mdIndex, maIndex,
                                      AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "MA delete process failed. Invalid domain %d maIndex %d\r\n",
                       mdIndex, maIndex);
    return OFDPA_E_FAIL;
  }

  maEntry = maObjEntry->maData;
  primVID = maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId;


  /* Obtain the domain entry */
  if ((maObjPrimVID = dot1agActivePrimVIDMAGet(mdIndex, primVID, AVL_EXACT)) == NULL)
  {
    /* Delete tree nodes for NULL vlanId */
    if (primVID == 0)
    {
      if ((rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_MA_OBJ_TREE,
                                        &dot1agCFM.dot1agMATree, maObjEntry))
          != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to delete MA %d for primVID %d\r\n",
                           maEntry->dot1agCfmMaIndex, primVID);
        return OFDPA_E_FAIL;
      }
      /* Reset table index */
      dot1agCFM.dot1agMATableIndex = maEntry - &(DOT1AG_MA_NET_TABLE(0));
      /* Wipe previous config */
      memset(maEntry, 0x0, sizeof(*maEntry));
      maEntry->active = DOT1AG_ROW_INACTIVE;
      return OFDPA_E_NONE;
    }
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to retrieve Active PrimVIDMA DB for MaIndex %d MaIndex %d for primVID %d\r\n",
                       mdIndex, maEntry->dot1agCfmMaIndex, primVID);
    return OFDPA_E_FAIL;
  }

  /* Purge all the MEP's associated with this MA */
  while ((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex, 0,
                                           0, AVL_NEXT)) != NULL)
  {
    if (mepObjEntry->key.dot1agCfmMdIndex != mdIndex ||
        mepObjEntry->key.dot1agCfmMaIndex != maIndex)
    {
      break;
    }

    if (dot1agCfmMEPDeleteProcess(mdIndex, mepObjEntry->key.dot1agCfmMaIndex,
                                  mepObjEntry->key.dot1agCfmMepIdentifier,
                                  mepObjEntry->key.dot1agCfmMepIfIndex)
        != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to delete MEP %d maIndex %d mepId %d intf %d\r\n",
                         mdIndex,
                         mepObjEntry->key.dot1agCfmMaIndex, mepObjEntry->key.dot1agCfmMepIdentifier,
                         mepObjEntry->key.dot1agCfmMepIfIndex);
      rc = OFDPA_E_FAIL;
      break;
    }
  }/* End of loop of MEP objects for a given MD, MA */

  /* purge all the MIPs associated with this MA.
     Call MIPCreateProcess again to create MIPs in case this primVID is present in another MA
  */
  for (mdIdx = 0; mdIdx < dpaDot1agMaintenanceDomainsMaxGet(); mdIdx++)
  {
    if (DOT1AG_MD_CFG(mdIdx).active != DOT1AG_ROW_INACTIVE &&
        DOT1AG_MD_CFG(mdIdx).dot1agCfmMdMdLevel != mdLevel)
    {
      if (dot1agActivePrimVIDMAGet(DOT1AG_MD_CFG(mdIdx).dot1agCfmMdIndex,
                                   primVID, AVL_EXACT) != NULL)
      {
        /* found this primVID in another domain */
        entryFound = OFDPA_TRUE;
        /* Remember the maximum level at which this MA is present */
        if (maLevel < DOT1AG_MD_CFG(mdIdx).dot1agCfmMdMdLevel)
        {
          maLevel = DOT1AG_MD_CFG(mdIdx).dot1agCfmMdMdLevel;
        }
      }
    }/* End of check if MA present in another domain/level */
  }/* End of mdIndex loop */

  intIfNum = 0;
  tmpLevel = 0;
  tmpVID = 0;
  direction = 0;
  while ((pMPObj = dot1agCfmStackObjGet(intIfNum, tmpVID, tmpLevel,
                                        direction, AVL_NEXT)) != NULL)
  {
    intIfNum  = pMPObj->key.dot1agCfmStackifIndex;
    tmpVID    = pMPObj->key.dot1agCfmStackVlanIdOrNone;
    tmpLevel  = pMPObj->key.dot1agCfmStackMdLevel;
    direction = pMPObj->key.dot1agCfmStackDirection;

    if (pMPObj->key.dot1agCfmStackVlanIdOrNone == primVID &&
        pMPObj->mepData == NULL)
    {
      if (entryFound == OFDPA_FALSE || /* no such MA is present at any other level */
          maLevel < tmpLevel) /* An MA exists but at a lesser level than MIP level */
      {
        dot1agCfmMPDestroy(pMPObj);
      }
    }
  }/* End of MIP obj iterations */

  if (rc == OFDPA_E_NONE)
  {
    /* Delete tree nodes */
    if ((rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_MA_OBJ_TREE,
                                      &dot1agCFM.dot1agMATree, maObjEntry))
        == OFDPA_E_NONE)
    {
      if ((rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_MA_OBJ_PRIM_VID_TREE,
                                        &dot1agCFM.dot1agMAPrimVIDTree, maObjPrimVID))
          == OFDPA_E_NONE)
      {
        /* Reset table index */
        dot1agCFM.dot1agMATableIndex = maEntry - &(DOT1AG_MA_NET_TABLE(0));
        /* Wipe previous config */
        memset(maEntry, 0x0, sizeof(*maEntry));
        maEntry->active = DOT1AG_ROW_INACTIVE;
      } /* Delete PrimVID view obj */
    }/* Delete MIB view obj */
  }

  if (rc != OFDPA_E_NONE)
  {
    /* Do not delete domain as operation is incomplete. This gives
       user another chance to try and delete the domain */
    maEntry->active = DOT1AG_ROW_ACTIVE;
  }
  else
  {
    /* Hardware Enabled Dot1ag delete the entry from hardware*/
    if (DOT1AG_CFG->dot1agCfmAdminMode == OFDPA_TRUE)
    {
#if OFDPA_NOT_SUPPORTED
      rc = dtlDot1agMAGroupDelete(mdIndex, maIndex);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Error deleting domain %d in hardware\r\n", mdIndex);
        return OFDPA_E_FAIL;
      }
#endif
    }
  }

  return rc;
}

/*********************************************************************
* @purpose  To process to reinstall entries in hardware with changed
* parameters
*
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
***********************************************************************/
OFDPA_ERROR_t dot1agCfmMPEntryReinstall(uint32_t mdIndex, uint32_t maIndex)
{
  dot1agCfmMepObj_t     *mepObjEntry   = NULL;
  dot1agCfmMaNetTable_t *maEntry       = NULL;
  dot1agCfmMAObj_t      *maObjEntry    = NULL;
  dot1agCfmMdTable_t    *mdEntry;
  dot1agCfmMepTable_t   *mepEntry      = NULL;
  dot1agCfmRMepObj_t    *pRMEPObj      = NULL;
//  uint32_t               ccmInterval = 0;
  uint32_t               rmepId        = 0;
  uint32_t               mepId         = 0, intfNum = 0;
  dot1agCfmStackObj_t   *pMPObj;
  OFDPA_ERROR_t          rc            = OFDPA_E_NONE;

  /* Obtain the domain entry */
  if ((mdEntry = dot1agMdEntryGet(mdIndex, -1)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "MA delete process failed. Invalid domain %d\r\n", mdIndex);
    return OFDPA_E_FAIL;
  }

  /* Obtain the domain entry */
  if ((maObjEntry = dot1agActiveMAGet(mdIndex, maIndex,
                                      AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "MA delete process failed. Invalid domain %d maIndex %d\r\n",
                       mdIndex, maIndex);
    return OFDPA_E_FAIL;
  }

  maEntry = maObjEntry->maData;
  /* Reinstall entries only is CFM admin mode is OFDPA_TRUE */
  if (DOT1AG_CFG->dot1agCfmAdminMode == OFDPA_FALSE)
  {
    return OFDPA_E_NONE;
  }
  /* Purge all the MEP's associated with this MA */
  while ((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex, 0,
                                           0, AVL_NEXT)) != NULL)
  {
    if (mepObjEntry->key.dot1agCfmMdIndex != mdIndex ||
        mepObjEntry->key.dot1agCfmMaIndex != maIndex)
    {
      break;
    }
    if ((mepObjEntry->key.dot1agCfmMepIdentifier == mepId) &&
        (mepObjEntry->key.dot1agCfmMepIfIndex == intfNum))
    {
      break;
    }
    mepId = mepObjEntry->key.dot1agCfmMepIdentifier;
    intfNum = mepObjEntry->key.dot1agCfmMepIfIndex;
    mepEntry = mepObjEntry->mepData;
    mepEntry->mepPktChanged = OFDPA_TRUE;

//    ccmInterval = CCMtime(maEntry->dot1agCfmMaNetCcmInterval, OFDPA_TRUE);

    /* Disable CCM transmission if active MEP (MEPActive = OFDPA_TRUE)
     * is disabled (CCIenabled = OFDPA_FALSE). Do not update dot1ag
     * structures as the previously set ccm interval still needs
     * to be available for re-enabling CCM transmission when MEP
     * is enabled (CCIenabled = OFDPA_TRUE and MEPActive = OFDPA_TRUE).
     */
    if (mepEntry->dot1agCfmMepCCIVar.cciEnabled == OFDPA_FALSE)
    {
//      ccmInterval = DOT1AG_CCM_intervalInvalid;
    }

    rmepId = 0;
    /* Check if stack object exists even before installing Hardware entries */
    if ((pMPObj = dot1agCfmStackObjGet(mepEntry->dot1agCfmMepIfIndex,
                                       mepEntry->dot1agCfmMepPrimaryVid,
                                       mdEntry->dot1agCfmMdMdLevel,
                                       mepEntry->dot1agCfmMepDirection,
                                       AVL_EXACT)) == NULL)
    {
      /* MEP stack object not created yet
       * Case : when switch reboots MEP database is created
       * Entries are not yet installed in hardware
       */
      continue;

    }

    if (pMPObj->mepData)
    {
      pMPObj->mepData->mepPktChanged = OFDPA_TRUE;
    }

    /* Remove Hardware RMEP entries corresponding to MEP
     */
    while ((pRMEPObj = dot1agCfmRMepObjGet(mdEntry->dot1agCfmMdIndex,
                                           maEntry->dot1agCfmMaIndex,
                                           mepEntry->dot1agCfmMepIdentifier,
                                           0, AVL_NEXT)) != NULL)
    {
      /* Check if no more RMEPs exist for this (mdIndex, maIndex, mepId) combination */
      if (pRMEPObj->key.dot1agCfmMdIndex != mdEntry->dot1agCfmMdIndex ||
          pRMEPObj->key.dot1agCfmMaIndex != maEntry->dot1agCfmMaIndex ||
          pRMEPObj->key.dot1agCfmMepIdentifier != mepEntry->dot1agCfmMepIdentifier)
      {
        break;
      }

      else if (pRMEPObj->key.dot1agCfmMepDbRMepIdentifier == rmepId)
      {
        break;
      }

      rmepId = pRMEPObj->key.dot1agCfmMepDbRMepIdentifier;
      if (pRMEPObj->hwExist == OFDPA_TRUE)
      {
#if OFDPA_NOT_SUPPORTED
        if (dtlDot1agRemoteMEPDelete(mdEntry->dot1agCfmMdIndex,
                                     maEntry->dot1agCfmMaIndex,
                                     mepEntry->dot1agCfmMepIdentifier,
                                     pRMEPObj->key.dot1agCfmMepDbRMepIdentifier)
            != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to delete RMEP  entry from hardware for mdIndex %d "
                             "maIndex %d mepId %d rmepId %d\r\n", mdEntry->dot1agCfmMdIndex,
                             maEntry->dot1agCfmMaIndex, mepEntry->dot1agCfmMepIdentifier,
                             pRMEPObj->key.dot1agCfmMepDbRMepIdentifier);
          return OFDPA_E_FAIL;
        }
        if (dtlDot1agRemoteMEPAdd(mepEntry->dot1agCfmMepIfIndex,
                                  mdEntry->dot1agCfmMdIndex,
                                  maEntry->dot1agCfmMaIndex,
                                  mdEntry->dot1agCfmMdMdLevel,
                                  mepEntry->dot1agCfmMepIdentifier,
                                  pRMEPObj->key.dot1agCfmMepDbRMepIdentifier,
                                  mepEntry->dot1agCfmMepPrimaryVid,
                                  ccmInterval) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to add RMEP  entry to hardware for mdIndex %d "
                             "maIndex %d mepId %d rmepId %d\r\n", mdEntry->dot1agCfmMdIndex,
                             maEntry->dot1agCfmMaIndex, mepEntry->dot1agCfmMepIdentifier,
                             pRMEPObj->key.dot1agCfmMepDbRMepIdentifier);
          return OFDPA_E_FAIL;
        }
#endif
      }
    }/*end of while */

    /* Remove the MEP entry and reinstall in hardware only for hardware MEPs */
    if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepEntry->dot1agCfmMepIdentifier))
    {
#if OFDPA_NOT_SUPPORTED
      if (dtlDot1agMEPDelete(mdEntry->dot1agCfmMdIndex,
                             maEntry->dot1agCfmMaIndex,
                             mepEntry->dot1agCfmMepIdentifier)
          != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete MEP entry to hardware or mdIndex %d "
                           "maIndex %d mepId %d\r\n", mdEntry->dot1agCfmMdIndex,
                           maEntry->dot1agCfmMaIndex, mepEntry->dot1agCfmMepIdentifier);
        return OFDPA_E_FAIL;
      }
      if (dtlDot1agMEPAdd(mepEntry->dot1agCfmMepIfIndex,
                          mdEntry->dot1agCfmMdIndex,
                          maEntry->dot1agCfmMaIndex,
                          mdEntry->dot1agCfmMdMdLevel,
                          mepEntry->dot1agCfmMepIdentifier,
                          mepEntry->dot1agCfmMepPrimaryVid,
                          ccmInterval) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to add MEP entry to hardware or mdIndex %d "
                           "maIndex %d mepId %d\r\n", mdEntry->dot1agCfmMdIndex,
                           maEntry->dot1agCfmMaIndex, mepEntry->dot1agCfmMepIdentifier);
        return OFDPA_E_FAIL;
      }
#endif
    }
  }/* End of loop of MEP objects for a given MD, MA */

  return rc;
}

/*********************************************************************
* @purpose  To process MD Create event generated from UI or during
*           system init
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMDCreateProcess(uint32_t mdIndex)
{
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To process MD Delete event generated from UI or during
*           system init
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMDDeleteProcess(uint32_t mdIndex)
{
  dot1agCfmMdTable_t *mdEntry;
//  dot1agCfmMaNetTable_t *maEntry     = NULL;
  dot1agCfmMAObj_t   *maObjEntry;
//  uint32_t             primVID, mdLevel, maIndex, intIfNum;
  uint32_t            mdLevel, maIndex, intIfNum;
  int                 numPlatformPorts = -1;
  OFDPA_ERROR_t       rc               = OFDPA_E_NONE;

  /* Obtain the domain entry */
  if ((mdEntry = dot1agMdEntryGet(mdIndex, -1)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "MD delete process failed. Invalid domain %d\r\n", mdIndex);
    return OFDPA_E_FAIL;
  }

  mdLevel = mdEntry->dot1agCfmMdMdLevel;

  /* Purge all the MA's associated with this MD */
  maIndex = 0;
  while ((maObjEntry = dot1agActiveMAGet(mdIndex, maIndex,
                                         AVL_NEXT))
         != NULL)
  {
    if (maObjEntry->key.dot1agCfmMdIndex != mdIndex)
    {
      break;
    }
    maIndex = maObjEntry->key.dot1agCfmMaIndex;
//    maEntry = maObjEntry->maData;
//    primVID = maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId;

    if (dot1agCfmMADeleteProcess(mdIndex, maIndex) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to delete MA for mdIndex %d maIndex %d\r\n", mdIndex,
                         maIndex);
      rc = OFDPA_E_FAIL;
      break;
    }
  }

  if (rc == OFDPA_E_NONE)
  {
    /* Clear mipMode mask for this level. MIP object deletion is taken care in
       MA delete process */
    numPlatformPorts = dpaPlatformNumPortsGet();

    if (-1 == numPlatformPorts)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to get number of ports.\r\n", 0);
      return OFDPA_E_FAIL;
    }

    for (intIfNum = 1; intIfNum <= numPlatformPorts; intIfNum++)
    {
      OFDPA_INTF_CLRMASKBIT(DOT1AG_MIP_TABLE(mdLevel).mipMode, intIfNum);
    }/* End of interface iterations */

    /* Wipe previous config */
    memset(mdEntry, 0x0, sizeof(*mdEntry));
    mdEntry->active = DOT1AG_ROW_INACTIVE;
  }
  else
  {
    /* Do not delete domain as operation is incomplete. This gives
       user another chance to try and delete the domain */
    mdEntry->active = DOT1AG_ROW_ACTIVE;
  }
  return rc;
}

/*********************************************************************
* @purpose  To send an LTM message on MEP
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
static OFDPA_ERROR_t dot1agCfmLTMSendProcess(uint32_t mdIndex, uint32_t maIndex,
                                             uint32_t mepIndex, uint32_t intIfNum)
{
  dot1agCfmStackObj_t *pMPObj;
  dot1agCfmMepObj_t   *mepObjEntry;

  /* Obtain the MEP entry */
  if ((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex,
                                        mepIndex, intIfNum,
                                        AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate LTM. Invalid MEP on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (NULL == mepObjEntry->mepData ||
      NULL == mepObjEntry->mepData->maData ||
      NULL == mepObjEntry->mepData->maData->mdData)
  {
    return OFDPA_E_FAIL;
  }

  if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                     mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
                                     mepObjEntry->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                     mepObjEntry->mepData->dot1agCfmMepDirection, AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate LTM. Invalid MEP in CFM Stack on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (pMPObj->mepData == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "LTM cannot be initiated from MIP. mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (pMPObj->operMode == OFDPA_FALSE)
  {
    pMPObj->mepData->dot1agCfmMepTransmitLtmResult = OFDPA_FALSE;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate LTM, as the MEP is non-operational. mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  /* Check if any LTM session is in progress */
  if (dot1agCFM.dot1agCfmLtmrVar.ltitTimer != NULL)
  {
    pMPObj->mepData->dot1agCfmMepTransmitLtmResult = OFDPA_FALSE;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_VERBOSE, "Failed to initiate LTM. Link Trace session in progress, try after 5s\r\n", 0);
    return OFDPA_E_FAIL;
  }

  if (OFDPA_E_NONE != dpaTimerAdd(dot1agCfmltitTimerCallback, &dot1agCFM.dot1agCfmLtmrVar.ltitTimer,
                                  DOT1AG_CFM_LTM_DELAY_TIMER_DEFAULT_MS, 0, &dot1agCFM))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to initialize Dot1ag LTIt Timer\r\n", 0);
    return OFDPA_E_ERROR;
  }

  if (xmitLTM(pMPObj) != OFDPA_E_NONE)
  {
    pMPObj->mepData->dot1agCfmMepTransmitLtmResult = OFDPA_FALSE;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to send LTM on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Clear Traceroute Cache
*
* @param    None
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Clear Traceroute Cache
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agLTRCacheClearProcess()
{
  OFDPA_ERROR_t             rc = OFDPA_E_NONE;

  if (dot1agCFM.dot1agCfmLtmrVar.ltitTimer != NULL)
  {
    rc = OFDPA_E_FAIL;
  }

  avlPurgeAvlTree(&dot1agCFM.dot1agLtrTree, DOT1AG_LTR_ENTRIES_MAX);

  return rc;
}
#if OFDPA_NOT_SUPPORTED
/*********************************************************************
* @purpose   This function is used to send timer events
*
* @param     timerCtrlBlk    @b{(input)}   Timer Control Block
* @param     ptrData         @b{(input)}   Ptr to passed data
*
* @returns   None
*
* @notes     None
* @end
*********************************************************************/
void dot1agTimerExpiryHdlr(L7_APP_TMR_CTRL_BLK_t timerCtrlBlk, void* ptrData)
{
  if (dot1agIssueCmd(dot1agCfmTimerTick, NULL, NULL) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to send dot1ag timer tick event\r\n", 0);
  }
}
#endif

//yjg add start, 增加硬件RemoteCCM更新处理函数

/*********************************************************************
* @purpose  update RmepState and report fault to APS
*
* @param    *rmepObj   pointer to the rmep object
* @param    lmepId   lmepid
* @param    faultOn   TRUE fault occer, FALSE fault clear
*
* @notes
* @notes
* @end
*********************************************************************/
void dot1agUpdateRemoteCCM(dot1agCfmRMepObj_t  *rmepObj, uint32_t lmepId, OFDPA_BOOL faultOn)
{
	ofdbOamLmepData_t *oamMapData;

    DOT1AG_TRACE(CFM_HW_EVENTS, 
                 "dot1agUpdateRemoteCCM: faultOn=%d, rmepObj->dot1agCfmRmepVar.rMepCcmDefect=%d",
                 faultOn, 
                 rmepObj->dot1agCfmRmepVar.rMepCcmDefect);
    /*printf("\r\ndot1agUpdateRemoteCCM: lmepid:%d faulton:%d, maindex:%d, rmepdef:%d\r\n",
        lmepId, faultOn, rmepObj->key.dot1agCfmMaIndex, rmepObj->dot1agCfmRmepVar.rMepCcmDefect);*/

	/*leishenghua modify 20170120, 只要有事件就上报, 创建PG时会二次上报事件*/
#if 0	
	if(faultOn == rmepObj->dot1agCfmRmepVar.rMepCcmDefect)//状态未改变
	{
		return;
	}
#endif

	if (ofdbInjectedOamLmepIdMapDataGet(lmepId, &oamMapData) == OFDPA_E_NONE)
	{
		rmepObj->dot1agCfmMepDbRMepFailedOkTime = dot1agCentisecondsGet();

		if (faultOn == OFDPA_FALSE)//恢复
		{
            DOT1AG_TRACE(CFM_HW_EVENTS, "dot1agUpdateRemoteCCM: livenessLogicalPortId=0x%x, oamStatus=%d",
               oamMapData->livenessLogicalPortId, 1);

		    /* report fault cleared to APS component */
		    /* retrieve the livenessPortId associated with this mep */
			g8131PgFaultReport(oamMapData->livenessLogicalPortId, 1);  /* up */
		  	rmepObj->dot1agCfmRmepVar.rMepCcmDefect = OFDPA_FALSE;
			rmepObj->dot1agCfmRmepState = DOT1AG_RMEP_OK;
		}
		else
		{
            DOT1AG_TRACE(CFM_HW_EVENTS, "dot1agUpdateRemoteCCM: livenessLogicalPortId=0x%x, oamStatus=%d",
               oamMapData->livenessLogicalPortId, 0);
            
			g8131PgFaultReport(oamMapData->livenessLogicalPortId, 0);  /* down */
		  	rmepObj->dot1agCfmRmepVar.rMepCcmDefect = OFDPA_TRUE;
			rmepObj->dot1agCfmRmepState = DOT1AG_RMEP_FAILED;
		}
	}

	return;
}
//yjg add end

/*********************************************************************
* @purpose  Get CCM event and handle fault notification
*
* @param    *eventInfo   pointer to the event information
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_NOT_FOUND, if MEG, MA, MD or RMEP not found
* @returns  OFDPA_E_FAIL, if other failure
*
* @notes
* @notes
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCCMEventHandler(dot1agEventInfo_t *eventInfo)
{
  OFDPA_ERROR_t              rc        = OFDPA_E_NONE;
  dot1agCfmMepObj_t         *pMEPObj   = NULL;
  dot1agCfmMdTable_t        *mdEntry   = NULL;
  dot1agCfmRMepObj_t        *pRMEPObj  = NULL;
  ofdpaOamMegConfig_t        megConfig;
  ofdbOamMegInfo_t           megInfo;
  ofdpaOamMepConfig_t        lmepConfig;
  ofdpaOamRemoteMpConfig_t   rmepConfig;
  OFDPA_DOT1AG_EVENT_TYPE_t  eventType;
  uint32_t                   maIndex, mdIndex, rmepId, mepId;
  uint32_t                   tempMepId = 0, tempIntfNum = 0;

  eventType = eventInfo->eventType;

  /*DOT1AG_TRACE(CFM_HW_EVENTS, "Event: %s megIndex:%d lmepId:%d rmepId:%d",
               dot1agDebugHwEventNames[eventType],
               eventInfo->megIndex, eventInfo->lmepId, eventInfo->rmepId);*/
  DOT1AG_TRACE(CFM_HW_EVENTS,"dot1agCCMEventHandler Event: %s megIndex:%d lmepId:%d rmepId:%d faulton:%d\r\n",
               dot1agDebugHwEventNames[eventType],
               eventInfo->megIndex, eventInfo->lmepId, eventInfo->rmepId, eventInfo->faultOn);

  rc = ofdbOamMegGet(eventInfo->megIndex, &megConfig, NULL, &megInfo);
  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Failed to retrieve MEG info for MEG index %d.\r\n", eventInfo->megIndex);
    return rc;
  }

  mdIndex = megInfo.dot1agMdIndex;
  maIndex = megInfo.dot1agMaIndex;

  if (OFDPA_E_NONE == ofdbOamMepGet(eventInfo->lmepId, &lmepConfig, NULL))
  {
    mepId = lmepConfig.mepId;
  }
  else
  {
    mepId = ~0;
  }

  if (OFDPA_E_NONE == ofdbOamRemoteMpGet(eventInfo->rmepId, &rmepConfig))
  {
    rmepId = rmepConfig.mepId;
  }
  else
  {
    rmepId = ~0;
  }

  if (dot1agActiveMAGet(mdIndex, maIndex,
                        AVL_EXACT) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Invalid association maIndex %d domain mdIndex %d\r\n",
                       mdIndex, maIndex);
    return OFDPA_E_FAIL;
  }


  if ((mdEntry = dot1agMdEntryGet(mdIndex, -1)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Invalid domain %d\r\n", mdIndex);
    return OFDPA_E_FAIL;
  }

  /* For hardware enabled dot1ag, chip cannot provide information on which LMEP
   * the defects(defXconCCM, defErrorCCM) has occured, so Evaluate the MADefectIndication variable and
   * execute FNG SM for all ACTIVE MEPS on this maIndex and mdIndex.
   */
  while ((pMEPObj = dot1agActiveMEPGet(mdIndex, maIndex, tempMepId,
                                       tempIntfNum, AVL_NEXT)) != NULL)
  {
    if (pMEPObj->key.dot1agCfmMdIndex != mdIndex ||
        pMEPObj->key.dot1agCfmMaIndex != maIndex)
    {
      break;
    }

    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
        "dot1agCCMEventHandler rmepId: %d mepid:%d pMEPOBJMEPID:%d lmepId:%d faultOn %d \r\n",
               rmepId, mepId, pMEPObj->key.dot1agCfmMepIdentifier, eventInfo->lmepId, eventInfo->faultOn);
    if ((rmepId != ~0) && (mepId != ~0) && (mepId == pMEPObj->key.dot1agCfmMepIdentifier) &&
        ((eventType == OFDPA_DOT1AG_EVENT_defIntfStatusDefect) ||
         (eventType == OFDPA_DOT1AG_EVENT_defRemoteCCM) ||  //added by yjg
         (eventType == OFDPA_DOT1AG_EVENT_defPortStatusDefect)))
    {
    
      if ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                          rmepId, AVL_EXACT)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to Get RMEP entry from RMEP database for mdIndex %d maIndex %d mepId %d rmepId %d\r\n",
                           mdIndex, maIndex, mepId, rmepId);
        return rc;
      }
    }
#if 0
    else if (eventType == OFDPA_DOT1AG_EVENT_defRemoteCCM && eventInfo->faultOn == OFDPA_FALSE)
    {
        if ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, 0, 0, AVL_NEXT)) != NULL)
        {
            
        printf("GetNEXT RMEP entry from RMEP database for mdIndex %d maIndex %d mepId %d rmepId %d\r\n",
                           pRMEPObj->key.dot1agCfmMdIndex, pRMEPObj->key.dot1agCfmMaIndex, 
                           pRMEPObj->key.dot1agCfmMepIdentifier, pRMEPObj->key.dot1agCfmMepDbRMepIdentifier);
            if (pRMEPObj->key.dot1agCfmMdIndex != mdIndex ||
              pRMEPObj->key.dot1agCfmMaIndex != maIndex)
            {
              /*如果不是指定MA下的RMEP,则该RMEP无效*/
              pRMEPObj = NULL;
            }
        }        
    }
#endif
    switch(eventType)
    {
      case OFDPA_DOT1AG_EVENT_defXconCCM:
        if (pMEPObj->mepData != NULL)
        {
          pMEPObj->mepData->dot1agCfmMepXConVar.xconCcmDefect = eventInfo->faultOn;
        }
        break;
      case OFDPA_DOT1AG_EVENT_defErrorCCM:
        if (pMEPObj->mepData != NULL)
        {
          pMEPObj->mepData->dot1agCfmRmepErrVar.errorCcmDefect = eventInfo->faultOn;
        }
        break;
      case OFDPA_DOT1AG_EVENT_defRemoteCCM:
        if (pMEPObj->mepData != NULL)
        {
          pMEPObj->mepData->dot1agCfmMepFngVar.someRMEPCCMDefect = eventInfo->faultOn;
		  //yjg add start, 增加硬件RemoteCCM处理函数
          if (pRMEPObj != NULL)
          {
		    dot1agUpdateRemoteCCM(pRMEPObj,eventInfo->lmepId,eventInfo->faultOn);
          }
		  //yjg add end
        }
        break;
      case OFDPA_DOT1AG_EVENT_defRDICCM:
        if (pMEPObj->mepData != NULL)
        {
          pMEPObj->mepData->dot1agCfmMepFngVar.someRDIDefect = eventInfo->faultOn;
        }
        break;
      case OFDPA_DOT1AG_EVENT_defIntfStatusDefect:
        if (pRMEPObj != NULL)
        {
          pRMEPObj->dot1agCfmRmepVar.rMepInterfaceStatusDefect = OFDPA_TRUE;
        }
        else
        {
          rc = OFDPA_E_FAIL;
        }
        break;
      case OFDPA_DOT1AG_EVENT_defPortStatusDefect:
        if (pRMEPObj != NULL)
        {
          pRMEPObj->dot1agCfmRmepVar.rMepPortStatusDefect = OFDPA_TRUE;
        }
        else
        {
          rc = OFDPA_E_FAIL;
        }
        break;
      case OFDPA_DOT1AG_EVENT_defMACstatus:
        break;
      default:
        rc = OFDPA_E_FAIL;
        break;
    }
    if (rc == OFDPA_E_NONE &&
        pMEPObj->mepData != NULL)
    {
      MAdefectIndicationNotify(pMEPObj->mepData);
    }

    tempMepId = pMEPObj->key.dot1agCfmMepIdentifier;
    tempIntfNum = pMEPObj->key.dot1agCfmMepIfIndex;
  }/* End of loop of MEP objects for a given MD, MA */
  return rc;
}

/*********************************************************************
* @purpose  Save the data in a message to a shared memory
*
* @param    event   @b{(input)} event type
* @param    *data   @b{(input)} pointer to data
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments Once the message is serviced, this variable size data will
*           be retrieved
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agFillMsg(void *data, dot1agMgmtMsg_t *msg)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  switch (msg->event)
  {
    case dot1agCfmCreate:
    case dot1agCfmDelete:
    case dot1agCfmMDCreate:
    case dot1agCfmMDDelete:
    case dot1agCfmMACreate:
    case dot1agCfmMADelete:
    case dot1agCfmMEPCreate:
    case dot1agCfmMEPDelete:
    case dot1agCfmMIPCreate:
    case dot1agCfmMIPDelete:
    case dot1agCfmMEPActiveMode:
    case dot1agCfmMEPCciEnabled:
    case dot1agCfmRMEPEntryAdd:
    case dot1agCfmRMEPEntryDelete:
    case dot1agCfmRMEPDelete:
    case dot1agCfmLBMSend:
    case dot1agCfmLBMStop:
    case dot1agCfmLTMSend:
    case dot1agCfmLTRCacheClear:
    case dot1agCfmCCMIntervalSet:
    case dot1agCfmMEPProlmCreate:
    case dot1agCfmMEPProlmDelete:
    case dot1agCfmMEPProlmEnabled:
    case dot1agMepOndemLMMStart:
    case dot1agMepOndemLMMStop:
    case dot1agMepDMMStart:
    case dot1agMepDMMStop:
    case dot1agCfmDefectAis:
    case dot1agCfmDefectLck:
    case dot1agCfmDefectCsf:
      memcpy(&msg->data.dot1agKeys, data, sizeof(msg->data.dot1agKeys));
      break;
      /* Interface Events */
    case dot1agIntfChange:
      memcpy(&msg->data.dot1agIntfChangeParms, data, sizeof(msg->data.dot1agIntfChangeParms));
      break;

      /* Only for synchronizing messages in the queue */
    case dot1agQueueSynchronize:
      break;

    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Invalid mgmt event! Event: %u\r\n", msg->event);
      rc = OFDPA_E_FAIL;
      break;
  }
  return rc;
}

/*********************************************************************
* @purpose  Send a command to dot1ag mgmtevent queue
*
* @param    event      @b{(input)} event type
* @param    intIfNum   @b{(input)} internal interface number
* @param    *data      @b{(input)} pointer to data
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments Command is queued for service
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agIssueCmd(uint32_t event, uint32_t intIfNum, void *data)
{
  uint32_t        currTime = 0;
  uint32_t        durTime;
  dot1agMgmtMsg_t eventmsg;
  OFDPA_ERROR_t   rc;

  if (bFastCcmDebug)
  {
    currTime = dot1agMicrosecondsGet();
  }

  /* copy event, intIfNum and data ptr to msg struct */
  eventmsg.event = event;
  eventmsg.intf  = intIfNum;

  if (data != NULL)
  {
    if (dot1agHwEventReceived == event)
    {
      memcpy(&eventmsg.data.dot1agHwEvent, data, sizeof(eventmsg.data.dot1agHwEvent));
    }
    else if (dot1agFillMsg(data, &eventmsg) != OFDPA_E_NONE)
    {
      return OFDPA_E_FAIL;
    }
  }

  if (event == dot1agCfmTimerTick)
  {
    if (dpaSemTake(dot1agCFM.dot1agTimerMsgSem, 0) == -1)
    {
      return OFDPA_E_NONE;
    }
  }

  DOT1AG_TRACE(CFM_EVENTS, "Event: %d (%s)\n", event,  dot1agDebugEventNameGet(event));

  /* send message */
  rc = msgqSnd(dot1agCFM.dot1agEventQueue, &eventmsg);
  if (rc == -1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "dot1agEventQueue is full.\n", 0);
  }

  if (bFastCcmDebug)
  {
    durTime = dot1agMicrosecondsGet() - currTime;
    if (durTime >= 1000)
    {
      printf("----%s %d used %6d us on %15u for %d\n",
             __FUNCTION__, __LINE__, durTime, dpaUpTimeSeconds(), event);
    }
  }
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to send to dot1ag event queue! Event: %u, interface: %d\r\n", event, intIfNum);
  }
  else
  {
    dpaSemGive(dot1agCFM.dot1agMsgQSema);
  }
  if (bFastCcmDebug)
  {
    durTime = dot1agMicrosecondsGet() - currTime;
    if (durTime >= 1000)
    {
      printf("----%s %d used %6d us on %15u for %d\n",
             __FUNCTION__, __LINE__, durTime, dpaUpTimeSeconds(), event);
    }
  }

  return rc;
}

/*********************************************************************
* @purpose  Route the event to a handling function and grab the parms
*
* @param    pMsg   @b{(input)} message containing event and interface
*                             number
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
 **********************************************************************/
OFDPA_ERROR_t dot1agDispatchCmd(dot1agMgmtMsg_t *pMsg)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  DOT1AG_TRACE(CFM_EVENTS, "Event: %d (%s)\n", pMsg->event,  dot1agDebugEventNameGet(pMsg->event));
  switch (pMsg->event)
  {
    /* Put this case first as it will be the most time sensitive */
    case dot1agCfmTimerTick:
      dot1agTimerProcess(dot1agCFM.timerList);
      dpaSemGive(dot1agCFM.dot1agTimerMsgSem);
      break;

    case dot1agHwEventReceived:
      dot1agCCMEventHandler(&pMsg->data.dot1agHwEvent);
      break;

    case dot1agCfmAdminModeEnable:
      break;

    case dot1agCfmAdminModeDisable:
      break;

    case dot1agCfmMDCreate:
      rc = dot1agCfmMDCreateProcess(pMsg->data.dot1agKeys.mdIndex);
      break;
    case dot1agCfmMDDelete:
      rc = dot1agCfmMDDeleteProcess(pMsg->data.dot1agKeys.mdIndex);
      break;
    case dot1agCfmMACreate:
      rc = dot1agCfmMACreateProcess(pMsg->data.dot1agKeys.mdIndex,
                                    pMsg->data.dot1agKeys.maIndex);
      break;
    case dot1agCfmCCMIntervalSet:
      rc = dot1agCfmMPEntryReinstall(pMsg->data.dot1agKeys.mdIndex,
                                     pMsg->data.dot1agKeys.maIndex);
      break;
    case dot1agCfmMADelete:
      rc = dot1agCfmMADeleteProcess(pMsg->data.dot1agKeys.mdIndex,
                                    pMsg->data.dot1agKeys.maIndex);
      break;
    case dot1agCfmMEPCreate:
      rc = dot1agCfmMEPCreateProcess(pMsg->data.dot1agKeys.mdIndex,
                                     pMsg->data.dot1agKeys.maIndex,
                                     pMsg->data.dot1agKeys.mepIndex,
                                     pMsg->data.dot1agKeys.intIfNum,
                                     pMsg->data.dot1agKeys.localMpId);
      break;
    case dot1agCfmMEPDelete:
      rc = dot1agCfmMEPDeleteProcess(pMsg->data.dot1agKeys.mdIndex,
                                     pMsg->data.dot1agKeys.maIndex,
                                     pMsg->data.dot1agKeys.mepIndex,
                                     pMsg->data.dot1agKeys.intIfNum);
      break;
    case dot1agCfmMIPCreate:
      rc = dot1agCfmMIPCreateProcess(pMsg->data.dot1agKeys.mdIndex,
                                     pMsg->data.dot1agKeys.uintVal,
                                     pMsg->data.dot1agKeys.intIfNum,
                                     pMsg->data.dot1agKeys.srcMac);
      break;
    case dot1agCfmMIPDelete:
      rc = dot1agCfmMIPDeleteProcess(pMsg->data.dot1agKeys.mdIndex,
                                     pMsg->data.dot1agKeys.uintVal,
                                     pMsg->data.dot1agKeys.intIfNum,
                                     0);
      break;
    case dot1agCfmCreate:
      rc = dot1agCfmCreateProcess();
      break;
    case dot1agCfmDelete:
      rc = dot1agCfmDeleteProcess();
      break;
    case dot1agCfmMEPActiveMode:
      rc = dot1agCfmMEPActiveModeProcess(pMsg->data.dot1agKeys.mdIndex,
                                         pMsg->data.dot1agKeys.maIndex,
                                         pMsg->data.dot1agKeys.mepIndex,
                                         pMsg->data.dot1agKeys.intIfNum,
                                         pMsg->data.dot1agKeys.uintVal);
      break;
    case dot1agCfmRMEPEntryAdd:
      rc = dot1agCfmRMEPEntryAddProcess(pMsg->data.dot1agKeys.mdIndex,
                                        pMsg->data.dot1agKeys.maIndex,
                                        pMsg->data.dot1agKeys.mepIndex,
                                        pMsg->data.dot1agKeys.rmepIndex,
                                        pMsg->data.dot1agKeys.intIfNum,
                                        pMsg->data.dot1agKeys.srcMac);
      break;
    case dot1agCfmRMEPEntryDelete:
      rc = dot1agCfmRMEPEntryDeleteProcess(pMsg->data.dot1agKeys.mdIndex,
                                           pMsg->data.dot1agKeys.maIndex,
                                           pMsg->data.dot1agKeys.mepIndex,
                                           pMsg->data.dot1agKeys.rmepIndex,
                                           pMsg->data.dot1agKeys.intIfNum);
      break;
    case dot1agCfmRMEPDelete:
      rc = dot1agCfmRMEPDeleteProcess(pMsg->data.dot1agKeys.mdIndex,
                                      pMsg->data.dot1agKeys.maIndex,
                                      pMsg->data.dot1agKeys.mepIndex,
                                      pMsg->data.dot1agKeys.intIfNum);
      break;
    case dot1agCfmMEPCciEnabled:
    case dot1agCfmMEPProlmEnabled:
    case dot1agCfmLBMSend:
    case dot1agCfmLBMStop:
      rc = dot1agCfmUIEventProcess(pMsg);
      break;
    case dot1agCfmLTMSend:
      rc = dot1agCfmLTMSendProcess(pMsg->data.dot1agKeys.mdIndex,
                                   pMsg->data.dot1agKeys.maIndex,
                                   pMsg->data.dot1agKeys.mepIndex,
                                   pMsg->data.dot1agKeys.intIfNum);
      break;
    case dot1agCfmLTRCacheClear:
      rc = dot1agLTRCacheClearProcess();
      break;
    case dot1agIntfChange:
      rc = dot1agIntfChangeProcess(pMsg->intf, pMsg->data.dot1agIntfChangeParms.event);
      break;
    case dot1agQueueSynchronize:
      dot1agQueueSyncSemaGive();
      break;

    case dot1agCfmMEPProlmCreate:
      rc = dot1agCfmMEPProlmCreateProcess(pMsg->data.dot1agKeys.mdIndex,
                                          pMsg->data.dot1agKeys.maIndex,
                                          pMsg->data.dot1agKeys.mepIndex,
                                          pMsg->data.dot1agKeys.intIfNum,
                                          pMsg->data.dot1agKeys.uintVal);
      break;
    case dot1agMepOndemLMMStart:
      rc = dot1agMepOndemLmmStartProcess(pMsg->data.dot1agKeys.mdIndex,
                                         pMsg->data.dot1agKeys.maIndex,
                                         pMsg->data.dot1agKeys.mepIndex,
                                         pMsg->data.dot1agKeys.intIfNum,
                                         pMsg->data.dot1agKeys.uintVal);
      break;
    case dot1agMepOndemLMMStop:
      rc = dot1agMepOndemLmmStopProcess(pMsg->data.dot1agKeys.mdIndex,
                                        pMsg->data.dot1agKeys.maIndex,
                                        pMsg->data.dot1agKeys.mepIndex,
                                        pMsg->data.dot1agKeys.intIfNum,
                                        pMsg->data.dot1agKeys.uintVal);
      break;
    case dot1agMepDMMStart:
      rc = dot1agMepDmmStartProcess(pMsg->data.dot1agKeys.mdIndex,
                                    pMsg->data.dot1agKeys.maIndex,
                                    pMsg->data.dot1agKeys.mepIndex,
                                    pMsg->data.dot1agKeys.intIfNum,
                                    pMsg->data.dot1agKeys.uintVal);
      break;
    case dot1agMepDMMStop:
      rc = dot1agMepDmmStopProcess(pMsg->data.dot1agKeys.mdIndex,
                                   pMsg->data.dot1agKeys.maIndex,
                                   pMsg->data.dot1agKeys.mepIndex,
                                   pMsg->data.dot1agKeys.intIfNum,
                                   pMsg->data.dot1agKeys.uintVal);
      break;
    case dot1agCfmDefectAis:
      rc = dot1agMepAisStartProcess(pMsg->data.dot1agKeys.mdIndex,
                                    pMsg->data.dot1agKeys.maIndex,
                                    pMsg->data.dot1agKeys.mepIndex,
                                    pMsg->data.dot1agKeys.intIfNum,
                                    pMsg->data.dot1agKeys.uintVal);
      break;
    case dot1agCfmDefectLck:
      rc = dot1agMepLckStartProcess(pMsg->data.dot1agKeys.mdIndex,
                                    pMsg->data.dot1agKeys.maIndex,
                                    pMsg->data.dot1agKeys.mepIndex,
                                    pMsg->data.dot1agKeys.intIfNum,
                                    pMsg->data.dot1agKeys.uintVal);
      break;
    case dot1agCfmDefectCsf:
      rc = dot1agMepCsfStartProcess(pMsg->data.dot1agKeys.mdIndex,
                                    pMsg->data.dot1agKeys.maIndex,
                                    pMsg->data.dot1agKeys.mepIndex,
                                    pMsg->data.dot1agKeys.intIfNum,
                                    pMsg->data.dot1agKeys.uintVal);
      break;
    case dot1agCfmSLMCacheClear:
      rc = dot1agSLMCacheClearProcess();
      break;
    default:
      rc = OFDPA_E_FAIL;
  }

  return rc;
}

/*********************************************************************
* @purpose  This routine processes the UI management events.
*
* @param    pMsg   @b{(input)} message containing event and interface
*                             number
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments This routine triggers the appropriate SMs also if needed.
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmUIEventProcess(dot1agMgmtMsg_t *pMsg)
{
  dot1agCfmSMEventMsg_t  smEventMsg;
  dot1agCfmStackObj_t   *pMPObj      = NULL;
  dot1agCfmMepObj_t     *mepObjEntry = NULL;
  OFDPA_ERROR_t          rc          = OFDPA_E_NONE;
  OFDPA_BOOL             hwEnabled   = OFDPA_FALSE;

  memset(&smEventMsg, 0, sizeof(smEventMsg));

  /* Obtain the MEP entry */
  if ((mepObjEntry = dot1agActiveMEPGet(pMsg->data.dot1agKeys.mdIndex,
                                        pMsg->data.dot1agKeys.maIndex,
                                        pMsg->data.dot1agKeys.mepIndex,
                                        pMsg->data.dot1agKeys.intIfNum,
                                        AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to process UI event. mdIndex %d  maIndex %d mepIndex %d intf %d\r\n",
                       pMsg->data.dot1agKeys.mdIndex,
                       pMsg->data.dot1agKeys.maIndex,
                       pMsg->data.dot1agKeys.mepIndex,
                       pMsg->data.dot1agKeys.intIfNum);
    return OFDPA_E_FAIL;
  }

  if (NULL == mepObjEntry->mepData ||
      NULL == mepObjEntry->mepData->maData ||
      NULL == mepObjEntry->mepData->maData->mdData)
  {
    return OFDPA_E_FAIL;
  }

  /* Check if MP is not yet ready */
  if (pMsg->event == dot1agCfmMEPCciEnabled ||
      pMsg->event == dot1agCfmMEPProlmEnabled ||
      pMsg->event == dot1agCfmLBMSend ||
      pMsg->event == dot1agCfmLBMStop)
  {
    if ((pMPObj = dot1agCfmStackObjGet(pMsg->data.dot1agKeys.intIfNum,
                                       mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
                                       mepObjEntry->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                       mepObjEntry->mepData->dot1agCfmMepDirection, AVL_EXACT)) == NULL)
    {
      return OFDPA_E_NONE;
    }
    else if (pMPObj->operMode == OFDPA_FALSE)
    {
      return OFDPA_E_NONE;
    }
  }

  if (NULL == pMPObj)
  {
    return OFDPA_E_NONE;
  }

  smEventMsg.pMPObj = pMPObj;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    hwEnabled = OFDPA_TRUE;
  }
  else
  {
    hwEnabled = OFDPA_FALSE;
  }

  /* Call the SM dispatcher if needed.*/
  switch (pMsg->event)
  {
    case dot1agCfmMEPCciEnabled:
      if (pMsg->data.dot1agKeys.uintVal == OFDPA_TRUE)
      {
        smEventMsg.event = dot1agCCIEnabled;
      }
      else
      {
        smEventMsg.event = dot1agCCINotEnabled;
      }
      (void)dot1agCfmStateMachineClassifier(&smEventMsg, hwEnabled);
      break;
    case dot1agCfmMEPProlmEnabled:
      if (pMsg->data.dot1agKeys.uintVal == OFDPA_TRUE)
      {
        smEventMsg.event = y1731PROLMEnabled;
      }
      else
      {
        smEventMsg.event = y1731PROLMNotEnabled;
      }
      (void)dot1agCfmStateMachineClassifier(&smEventMsg, hwEnabled);
      break;
    case dot1agCfmLBMSend:
      smEventMsg.event = dot1agLBILBMsToSend;
      (void)dot1agCfmStateMachineClassifier(&smEventMsg, hwEnabled);
      break;
    case dot1agCfmLBMStop:
      (void)dot1agCfmLbmTestTerminate(pMPObj);
      break;
    default:
      rc = OFDPA_E_FAIL;
      break;
  }
  return rc;
}

/*********************************************************************
* @purpose  dot1ag task which serves the request queue
*
* @param    void *
*
* @returns  void
*
* @comments User-interface writes and PAE PDUs are serviced off
*           of the dot1agQueue
*
* @end
**********************************************************************/
void dot1agTask(void *arg)
{
  dot1agMgmtMsg_t  eventMsg;
  dot1agPduMsg_t   pduMsg;

  dpaSemGive(dot1agTaskSyncSem);

  do
  {
    dpaSemTake(dot1agCFM.dot1agMsgQSema, DPA_SEM_FOREVER);
    if (msgqRcv(dot1agCFM.dot1agEventQueue, &eventMsg) != OFDPA_E_NONE)
    {
      if (msgqRcv(dot1agCFM.dot1agPduQueue, &pduMsg) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                           "Failed to receive PDU event msg\r\n", 0);
      }
      else
      {
        dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
        (void)dot1agPduReceiveProcess(&pduMsg);
        dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      }
    }
    else
    {

	  if(dot1agCfmTimerTick != eventMsg.event)
	  {
	  	  if(dot1agHwEventReceived == eventMsg.event)
	  	  {
		      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_VERY_VERBOSE,
					 "dot1agEventQueue rcv lmepid(%d) event(%d) faultOn(%d) send...\r\n",
					 eventMsg.data.dot1agHwEvent.lmepId, eventMsg.data.dot1agHwEvent.eventType, 
					 eventMsg.data.dot1agHwEvent.faultOn);
	  	  }	  
	  } 
      dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
      (void)dot1agDispatchCmd(&eventMsg);
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
    }
  } while (1); /* Eternal loop */
}

/*********************************************************************
* @purpose  Get CCM event and handle fault notification
*
* @param    *eventInfo   pointer to the event information
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if other failure
*
* @notes
* @notes
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCCMEventHandlerCallback(dot1agEventInfo_t *eventInfo)
{
  OFDPA_ERROR_t rc = dot1agIssueCmd(dot1agHwEventReceived, 0, eventInfo);

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to send dot1ag HW event, rc = %d\r\n", rc);
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To add remote mep entries on a given mpid rmepid
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    rmepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
* @param    srcMac     @b{(input)} MAC address of remote MEP
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRMEPEntryAddProcess(uint32_t mdIndex,
                                           uint32_t maIndex,
                                           uint32_t mepId,
                                           uint32_t rmepId,
                                           uint32_t intfNum,
                                           uint8_t *srcMac)
{
  dot1agCfmStackObj_t   *pMPObj;
  dot1agCfmMepObj_t     *mepObjEntry;
  dot1agCfmMAObj_t      *maObjEntry = NULL;
  dot1agCfmMdTable_t    *mdEntry    = NULL;
  dot1agCfmMepTable_t   *mepEntry   = NULL;
  dot1agCfmRMepObj_t    *pRMEPObj   = NULL;
  dot1agCfmRMepObj_t     rmepObj;
  dot1agCfmSMEventMsg_t  eventMsg;
  OFDPA_BOOL             hwExist    = OFDPA_FALSE;
//  uint32_t             ccmInterval = 0, rmepInterval;

  if ((maObjEntry = dot1agActiveMAGet(mdIndex, maIndex,
                                      AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Remote MEP entry creation failed because of invalid domain %d maIndex %d\r\n",
                       mdIndex, maIndex);
    return OFDPA_E_FAIL;
  }

  if ((mdEntry = dot1agMdEntryGet(mdIndex, -1)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Remote MEP entry creation failed because of invalid domain %d\r\n", mdIndex);
    return OFDPA_E_FAIL;
  }

  /* Obtain the domain entry */
  if ((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex,
                                        mepId, intfNum, AVL_EXACT))
      == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "MEP delete process failed.Invalid mep obj domain %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId, intfNum);
    return OFDPA_E_FAIL;
  }

  if (NULL == mepObjEntry->mepData ||
      NULL == mepObjEntry->mepData->maData ||
      NULL == mepObjEntry->mepData->maData->mdData)
  {
    return OFDPA_E_FAIL;
  }

  mepEntry = mepObjEntry->mepData;

  pMPObj = dot1agCfmStackObjGet(mepObjEntry->mepData->dot1agCfmMepIfIndex,
                                mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
                                mepObjEntry->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                mepObjEntry->mepData->dot1agCfmMepDirection, AVL_EXACT);
  if (NULL == pMPObj)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Remote MEP entry creation failed because of invalid MEP in CFM Stack on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId, mepObjEntry->mepData->dot1agCfmMepIfIndex);
    return OFDPA_E_FAIL;
  }

#if OFDPA_NOT_SUPPORTED
  if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
  {
    ccmInterval = CCMtime(mepEntry->maData->dot1agCfmMaNetCcmInterval, OFDPA_TRUE);
  }
  else
  {
    ccmInterval = CCMtime(mepEntry->maData->dot1agCfmMaNetCcmInterval, OFDPA_FALSE);
  }

  rmepInterval = DOT1AG_RMEP_INTERVAL_GET(ccmInterval);

  if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
    if (DOT1AG_CFG->dot1agCfmAdminMode == OFDPA_TRUE)
    {
      if (dot1agIntfCanBeOperational(intfNum) == OFDPA_TRUE)
      {
        if (dtlDot1agRemoteMEPAdd(intfNum, mdIndex, maIndex, mdEntry->dot1agCfmMdMdLevel,
                                  mepId, rmepId, primVID, ccmInterval)!= OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to add RMEP  entry to hardware for mdIndex %d maIndex %d mepId %d rmepId %d\r\n",
                             mdIndex, maIndex, mepId, rmepId);
          return OFDPA_E_FAIL;
        }
        hwExist = OFDPA_TRUE;
      }
    }
#endif
  /* Check if any RMEP Obj exists for the received CCM MEP,
     if not create one */
  if ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                      rmepId, AVL_EXACT)) == NULL)
  {
    /* Add a new RMEP entry */
    memset(&rmepObj, 0x0, sizeof(rmepObj));
    rmepObj.key.dot1agCfmMdIndex             = mdIndex;
    rmepObj.key.dot1agCfmMaIndex             = maIndex;
    rmepObj.key.dot1agCfmMepIdentifier       = mepId;
    rmepObj.key.dot1agCfmMepDbRMepIdentifier = rmepId;
    rmepObj.dot1agCfmRmepState               = DOT1AG_RMEP_IDLE;
    rmepObj.hwExist = hwExist;

    memcpy(rmepObj.dot1agCfmRmepVar.rMepMacAddress, srcMac, OFDPA_MAC_ADDR_LEN);
    if (dot1agDBTreeEntryAdd(DOT1AG_DB_ID_RMEP_TREE, &dot1agCFM.dot1agRMepTree,
                             &rmepObj) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to add RMEP entry to RMEP database for mdIndex %d maIndex %d mepId %d rmepId %d\r\n",
                         mdIndex, maIndex, mepId, rmepId);
      return OFDPA_E_FAIL;

    }
    if ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                        rmepId, AVL_EXACT)) == NULL)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to get RMEP entry from RMEP database for mdIndex %d maIndex %d mepId %d rmepId %d\r\n",
                         mdIndex, maIndex, mepId, rmepId);
      return OFDPA_E_FAIL;
    }

    eventMsg.event    = dot1agRmepBegin;
    eventMsg.pRMEPObj = pRMEPObj;
    eventMsg.pMepData = mepEntry;
    eventMsg.pMPObj   = pMPObj;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);

    eventMsg.event    = dot1agRmepWhileZero;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To clear remote mep entries on a given mpid generated from UI
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    rmepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRMEPEntryDeleteProcess(uint32_t mdIndex,
                                              uint32_t maIndex,
                                              uint32_t mepId,
                                              uint32_t rmepId,
                                              uint32_t intfNum)
{
  dot1agCfmMepObj_t   *mepObjEntry;
  dot1agCfmMepTable_t *mepEntry = NULL;
  dot1agCfmMdTable_t  *mdEntry  = NULL;
  dot1agCfmRMepObj_t  *pRMEPObj = NULL;
  OFDPA_ERROR_t        rc       = OFDPA_E_NONE;

  if (dot1agActiveMAGet(mdIndex, maIndex,
                        AVL_EXACT) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Remote MEP entry creation failed because of invalid domain %d maIndex %d\r\n",
                       mdIndex, maIndex);
    return OFDPA_E_FAIL;
  }

  if ((mdEntry = dot1agMdEntryGet(mdIndex, -1)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Remote MEP entry creation failed because of invalid domain %d\r\n",
                       mdIndex);
    return OFDPA_E_FAIL;
  }
  /* Obtain the domain entry */
  if (((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intfNum, AVL_EXACT)) == NULL) ||
      (mepObjEntry->mepData == NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "MEP delete process failed. Invalid mep obj domain %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepId, intfNum);
    return OFDPA_E_FAIL;
  }

  mepEntry = mepObjEntry->mepData;


  if ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                      rmepId, AVL_EXACT)) != NULL)
  {
    /* Delete the Remote MEP entry from hardware */
    if (pRMEPObj->hwExist == OFDPA_TRUE)
    {
#if OFDPA_NOT_SUPPORTED
      if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
        if (dtlDot1agRemoteMEPDelete(mdIndex, maIndex, mepId, rmepId)
            != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to delete RMEP entry from hardware for mdIndex %d maIndex %d mepId %d rmepId %d\r\n",
                             mdIndex, maIndex, mepId, rmepId);
          return OFDPA_E_FAIL;
        }
#endif
      pRMEPObj->hwExist = OFDPA_FALSE;
    }

    if (pRMEPObj->dot1agCfmRmepVar.rmepTimer != NULL)
    {
      dot1agTimerDelete(pRMEPObj->dot1agCfmRmepVar.rmepTimer);
      pRMEPObj->dot1agCfmRmepVar.rmepTimer = NULL;
    }
    /* Remove the entry from the rmepList - RMEP Database */
    rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_RMEP_TREE, &dot1agCFM.dot1agRMepTree,
                                 pRMEPObj);

  }

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to delete RMEP entry RMEP from database for mdIndex %d maIndex %d mepId %d rmepId %d\r\n",
                       mdIndex, maIndex, mepId, rmepId);
    return rc;
  }
  MAdefectIndicationNotify(mepEntry);

  return rc;
}

/*********************************************************************
* @purpose  To clear the cfm defects on hardware meps
*
* @param    pMPObj   @b{(input)} Reference to maintenance point object
*
* @returns  none
*
* @comments
*
* @end
*
*********************************************************************/
void dot1agCfmDefectsClear(dot1agCfmStackObj_t *pMPObj)
{
  if (pMPObj->mepData != NULL)
  {
    pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_RDI].y1731DefCcmDefect   = OFDPA_FALSE;
    pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_LOC].y1731DefCcmDefect   = OFDPA_FALSE;
    pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNPTR].y1731DefCcmDefect = OFDPA_FALSE;
    pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNP].y1731DefCcmDefect   = OFDPA_FALSE;
    pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNM].y1731DefCcmDefect   = OFDPA_FALSE;
    pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_MMG].y1731DefCcmDefect   = OFDPA_FALSE;
    pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNL].y1731DefCcmDefect   = OFDPA_FALSE;

    pMPObj->mepData->dot1agCfmMepFngVar.someRMEPCCMDefect   = OFDPA_FALSE;
    pMPObj->mepData->dot1agCfmMepXConVar.xconCcmDefect      = OFDPA_FALSE;
    pMPObj->mepData->dot1agCfmRmepErrVar.errorCcmDefect     = OFDPA_FALSE;
    pMPObj->mepData->dot1agCfmMepFngVar.someRDIDefect       = OFDPA_FALSE;
    pMPObj->mepData->dot1agCfmMepFngVar.someMacStatusDefect = OFDPA_FALSE;
    pMPObj->mepData->dot1agCfmGblMepVar.maDefectIndication  = OFDPA_FALSE;
    pMPObj->mepData->dot1agCfmGblMepVar.presentRDI          = OFDPA_FALSE;
    pMPObj->mepData->dot1agCfmMepFngVar.highestDefect       = DOT1AG_DEFECT_none;
    pMPObj->mepData->dot1agCfmMepFngVar.highestDefectPri    = DOT1AG_DEFECT_PRIORITY_none;
  }
}

void dot1agTimerDelete(void *pNode)
{
  dot1ag_timer_list_t *pDel = pNode;
  dot1ag_timer_list_t *cur = dot1agCFM.timerList.next;

  if (dot1agCFM.timerList.next == pDel)
  {
    dot1agCFM.timerList.next = pDel->next;
    free(pDel);
    return;
  }
  for (; cur; cur = cur->next)
  {
    if (cur->next == pDel)
    {
      cur->next = pDel->next;
      free(pDel);
      break;
    }
  }
  if (!cur)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to Del timer node.\r\n", 0);
  }
}

OFDPA_ERROR_t dot1agTimerUpdate(void *pNode, void *para, uint32_t tmValue, dot1ag_tmr_fn pFunc)
{
  dot1ag_timer_list_t *p = pNode;
  dot1ag_timer_list_t *cur = dot1agCFM.timerList.next;

  if (!pNode || !para)
  {
    return OFDPA_E_FAIL;
  }

  if (dot1agCFM.timerList.next == pNode)
  {
    p->node.func_p   = pFunc;
    p->node.para     = para;
    p->node.timeLeft = tmValue * DOT1AG_TIMER_INTERVAL;
    return OFDPA_E_NONE;
  }
  for (; cur; cur = cur->next)
  {
    if (cur->next == pNode)
    {
      p->node.func_p   = pFunc;
      p->node.para     = para;
      p->node.timeLeft = tmValue * DOT1AG_TIMER_INTERVAL;
      return OFDPA_E_NONE;
    }
  }
  return OFDPA_E_FAIL;
}

void *dot1agTimerNodeAdd(uint32_t tmValue, dot1ag_tmr_fn pFunc, void *para)
{
  dot1ag_timer_list_t *p     = dot1agCFM.timerList.next;
  dot1ag_timer_list_t *pNode = NULL;

  /* timer list is empty */
  if (!p)
  {
    pNode = malloc(sizeof(*pNode));
    if (!pNode)
    {
      return NULL;
    }
    memset(pNode, 0, sizeof(*pNode));
    pNode->node.para         = para;
    pNode->node.func_p       = pFunc;
    pNode->node.timeLeft     = tmValue * DOT1AG_TIMER_INTERVAL;
    dot1agCFM.timerList.next = pNode;
    return pNode;
  }
  for (; p; p = p->next)
  {
    /* add the same node */
    if ((p->node.para == para) &&
        (p->node.func_p == pFunc))
    {
      return p;
    }
    else if (!p->next)  /* the list last node*/
    {
      pNode = malloc(sizeof(*pNode));
      if (!pNode)
      {
        return NULL;
      }
      memset(pNode, 0, sizeof(*pNode));
      pNode->node.para     = para;
      pNode->node.func_p   = pFunc;
      pNode->node.timeLeft = tmValue * DOT1AG_TIMER_INTERVAL;
      p->next              = pNode;
      return pNode;
    }
  }
  return pNode;
}

void dot1agTimerProcess(dot1ag_timer_list_t timerList)
{
  dot1ag_timer_list_t *p        = timerList.next;
  static uint32_t      prevTime = 0;
  uint32_t             currTime;
  uint32_t             durTime;

  currTime = dot1agMicrosecondsGet();
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_TOO_VERBOSE,
                     "currTime = 0x%u prevTime = 0x%u timeGap = 0x%u\n",
                     currTime, prevTime, (currTime - prevTime));
  if (!prevTime)
  {
    prevTime = currTime;
    return;
  }

#if 0
  if ((currTime - prevTime) >= 8000)
  {
    printf("----dot1agTimerProcess: gap %6u us on %15u\n", currTime - prevTime, dpaUpTimeSeconds());
  }
#endif

  durTime = currTime - prevTime;
  for (; p; p = p->next)
  {
    if (p->node.timeLeft <= 0) /*dead node, skip*/
    {
      continue;
    }
    p->node.timeLeft = p->node.timeLeft - durTime;
    if (p->node.timeLeft <= 0)
    {
      p->node.func_p(p->node.para);
    }
  }

  prevTime = currTime;

  if (bFastCcmDebug)
  {
    durTime = dot1agMicrosecondsGet() - currTime;
    if (durTime >= 1500)
    {
      printf("----%s %d used %6d us on %15u\n", __FUNCTION__, __LINE__, durTime, dpaUpTimeSeconds());
    }
  }

  /* Here call the TX chain (if any) */
  dot1agSendCcmArray();

  if (bFastCcmDebug)
  {
    durTime = dot1agMicrosecondsGet() - currTime;
    if (durTime >= 2500)
    {
      printf("----%s %d used %6d us on %15u\n", __FUNCTION__, __LINE__, durTime, dpaUpTimeSeconds());
    }
  }
}

/*********************************************************************
* @purpose  To create pro lm on a given mpid
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMEPProlmCreateProcess(uint32_t mdIndex,
                                             uint32_t maIndex,
                                             uint32_t mepId,
                                             uint32_t intfNum,
                                             uint32_t priorityClass)
{
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "dot1agCfmMEPProlmCreateProcess: mep obj domain %d maIndex %d mepId %d\r\n",
                       mdIndex, maIndex, mepId);
    return OFDPA_E_FAIL;
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To start ondem lm on a given mpid
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepOndemLmmStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepIndex, uint32_t intIfNum, uint32_t priority)
{
  dot1agCfmSMEventMsg_t  smEventMsg;
  dot1agCfmStackObj_t   *pMPObj      = NULL;
  dot1agCfmMepObj_t     *mepObjEntry = NULL;
  OFDPA_ERROR_t          rc          = OFDPA_E_NONE;
  OFDPA_BOOL             hwEnabled   = OFDPA_FALSE;

  memset(&smEventMsg, 0, sizeof(smEventMsg));

  if ((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex,
                                        mepIndex, intIfNum,
                                        AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate LMM. Invalid MEP on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (NULL == mepObjEntry->mepData ||
      NULL == mepObjEntry->mepData->maData ||
      NULL == mepObjEntry->mepData->maData->mdData)
  {
    return OFDPA_E_FAIL;
  }

  if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                     mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
                                     mepObjEntry->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                     mepObjEntry->mepData->dot1agCfmMepDirection, AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate LMM. Invalid MEP in CFM Stack on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (pMPObj->mepData == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "LMM cannot be initiated from MIP. mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  smEventMsg.pMPObj = pMPObj;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    hwEnabled = OFDPA_TRUE;
  }
  else
  {
    hwEnabled = OFDPA_FALSE;
  }

  /* Call the SM dispatcher if needed.*/
  smEventMsg.event = y1731ONDEMLMMStart;
  smEventMsg.priority = priority;
  (void)dot1agCfmStateMachineClassifier(&smEventMsg, hwEnabled);

  return rc;
}

/*********************************************************************
* @purpose  To stop ondem lm on a given mpid
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepOndemLmmStopProcess(uint32_t mdIndex, uint32_t maIndex,
                                           uint32_t mepIndex, uint32_t intIfNum, uint32_t priority)
{
  dot1agCfmSMEventMsg_t  smEventMsg;
  dot1agCfmStackObj_t   *pMPObj      = NULL;
  dot1agCfmMepObj_t     *mepObjEntry = NULL;
  OFDPA_ERROR_t          rc          = OFDPA_E_NONE;
  OFDPA_BOOL             hwEnabled   = OFDPA_FALSE;

  memset(&smEventMsg, 0, sizeof(smEventMsg));

  if ((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex,
                                        mepIndex, intIfNum,
                                        AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate LMM. Invalid MEP on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (NULL == mepObjEntry->mepData ||
      NULL == mepObjEntry->mepData->maData ||
      NULL == mepObjEntry->mepData->maData->mdData)
  {
    return OFDPA_E_FAIL;
  }

  if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                     mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
                                     mepObjEntry->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                     mepObjEntry->mepData->dot1agCfmMepDirection, AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate LMM. Invalid MEP in CFM Stack on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (pMPObj->mepData == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "LMM cannot be initiated from MIP. mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  smEventMsg.pMPObj = pMPObj;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    hwEnabled = OFDPA_TRUE;
  }
  else
  {
    hwEnabled = OFDPA_FALSE;
  }

  /* Call the SM dispatcher if needed.*/
  smEventMsg.event = y1731ONDEMLMMStop;
  smEventMsg.priority = priority;
  (void)dot1agCfmStateMachineClassifier(&smEventMsg, hwEnabled);

  return rc;
}

/*********************************************************************
* @purpose  To start dm on a given mpid
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepDmmStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepIndex, uint32_t intIfNum, uint32_t priority)
{
  dot1agCfmSMEventMsg_t  smEventMsg;
  dot1agCfmStackObj_t   *pMPObj      = NULL;
  dot1agCfmMepObj_t     *mepObjEntry = NULL;
  OFDPA_ERROR_t          rc          = OFDPA_E_NONE;
  OFDPA_BOOL             hwEnabled   = OFDPA_FALSE;

  memset(&smEventMsg, 0, sizeof(smEventMsg));

  if ((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex,
                                        mepIndex, intIfNum,
                                        AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate DMM. Invalid MEP on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (NULL == mepObjEntry->mepData ||
      NULL == mepObjEntry->mepData->maData ||
      NULL == mepObjEntry->mepData->maData->mdData)
  {
    return OFDPA_E_FAIL;
  }

  if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                     mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
                                     mepObjEntry->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                     mepObjEntry->mepData->dot1agCfmMepDirection, AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate DMM. Invalid MEP in CFM Stack on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (pMPObj->mepData == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "DMM cannot be initiated from MIP. mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  smEventMsg.pMPObj = pMPObj;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    hwEnabled = OFDPA_TRUE;
  }
  else
  {
    hwEnabled = OFDPA_FALSE;
  }

  /* Call the SM dispatcher if needed.*/
  smEventMsg.event = y1731DMMStart;
  smEventMsg.priority = priority;
  (void)dot1agCfmStateMachineClassifier(&smEventMsg, hwEnabled);

  return rc;
}

/*********************************************************************
* @purpose  To stop dm on a given mpid
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepDmmStopProcess(uint32_t mdIndex, uint32_t maIndex,
                                      uint32_t mepIndex, uint32_t intIfNum, uint32_t priority)
{
  dot1agCfmSMEventMsg_t  smEventMsg;
  dot1agCfmStackObj_t   *pMPObj      = NULL;
  dot1agCfmMepObj_t     *mepObjEntry = NULL;
  OFDPA_ERROR_t          rc          = OFDPA_E_NONE;
  OFDPA_BOOL             hwEnabled   = OFDPA_FALSE;

  memset(&smEventMsg, 0, sizeof(smEventMsg));

  if ((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex,
                                        mepIndex, intIfNum,
                                        AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate DMM. Invalid MEP on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (NULL == mepObjEntry->mepData ||
      NULL == mepObjEntry->mepData->maData ||
      NULL == mepObjEntry->mepData->maData->mdData)
  {
    return OFDPA_E_FAIL;
  }

  if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                     mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
                                     mepObjEntry->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                     mepObjEntry->mepData->dot1agCfmMepDirection, AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to initiate DMM. Invalid MEP in CFM Stack on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (pMPObj->mepData == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "DMM cannot be initiated from MIP. mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  smEventMsg.pMPObj = pMPObj;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    hwEnabled = OFDPA_TRUE;
  }
  else
  {
    hwEnabled = OFDPA_FALSE;
  }

  /* Call the SM dispatcher if needed.*/
  smEventMsg.event = y1731DMMStop;
  smEventMsg.priority = priority;
  (void)dot1agCfmStateMachineClassifier(&smEventMsg, hwEnabled);

  return rc;
}

/*********************************************************************
* @purpose  To start AIS on a given mpid
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
* @param    startFlag @b{(input)} Start or stop AIS
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepAisStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepIndex, uint32_t intIfNum,
                                       uint32_t startFlag)
{
  dot1agCfmMepObj_t     *mepObjEntry = NULL;
  dot1agCfmSMEventMsg_t  smEventMsg;
  OFDPA_BOOL             hwEnabled   = OFDPA_FALSE;
  OFDPA_ERROR_t          rc          = OFDPA_E_NONE;

  if (((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex, mepIndex, intIfNum, AVL_EXACT)) == NULL) ||
      (mepObjEntry->mepData == NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Invalid MEP on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  smEventMsg.pMepData = mepObjEntry->mepData;

  if ((smEventMsg.pMepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMepData->dot1agCfmMepIdentifier)))
  {
    hwEnabled = OFDPA_TRUE;
  }
  else
  {
    hwEnabled = OFDPA_FALSE;
  }

  /* Call the SM dispatcher if needed.*/
  if (startFlag)
  {
    smEventMsg.event = y1731AisSendStart;
  }
  else
  {
    smEventMsg.event = y1731AisSendStop;
  }
  (void)dot1agCfmStateMachineClassifier(&smEventMsg, hwEnabled);

  return rc;
}

/*********************************************************************
* @purpose  To start LCK on a given mpid
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
* @param    startFlag @b{(input)} Start or stop LCK
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepLckStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepIndex, uint32_t intIfNum,
                                       uint32_t startFlag)
{
  dot1agCfmSMEventMsg_t  smEventMsg;
  dot1agCfmStackObj_t   *pMPObj      = NULL;
  dot1agCfmMepObj_t     *mepObjEntry = NULL;
  OFDPA_ERROR_t          rc          = OFDPA_E_NONE;
  OFDPA_BOOL             hwEnabled   = OFDPA_FALSE;

  memset(&smEventMsg, 0, sizeof(smEventMsg));
  if ((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex,
                                        mepIndex, intIfNum,
                                        AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Invalid MEP on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (NULL == mepObjEntry->mepData ||
      NULL == mepObjEntry->mepData->maData ||
      NULL == mepObjEntry->mepData->maData->mdData)
  {
    return OFDPA_E_FAIL;
  }

  if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                     mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
                                     mepObjEntry->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                     mepObjEntry->mepData->dot1agCfmMepDirection, AVL_EXACT)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Failed to initiate DMM. Invalid MEP in CFM Stack on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  if (pMPObj->mepData == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "DMM cannot be initiated from MIP. mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  smEventMsg.pMPObj = pMPObj;

  if ((smEventMsg.pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(smEventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    hwEnabled = OFDPA_TRUE;
  }
  else
  {
    hwEnabled = OFDPA_FALSE;
  }

  /* Call the SM dispatcher if needed.*/
  if (startFlag)
  {
    smEventMsg.event = y1731LckSendStart;
  }
  else
  {
    smEventMsg.event = y1731LckSendStop;
  }
  (void)dot1agCfmStateMachineClassifier(&smEventMsg, hwEnabled);

  return rc;
}

/*********************************************************************
* @purpose  To start CSF on a given mpid
*
* @param    mdIndex   @b{(input)} Index to indentify an MD
* @param    maIndex   @b{(input)} Index to indentify an MA
* @param    mepId     @b{(input)} Index to indentify an MEP
* @param    intIfNum  @b{(input)} Index to indentify an interface
* @param    startFlag @b{(input)} Start or stop CSF
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepCsfStartProcess(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepIndex, uint32_t intIfNum,
                                       uint32_t startFlag)
{
  dot1agCfmMepObj_t *mepObjEntry = NULL;

  if (((mepObjEntry = dot1agActiveMEPGet(mdIndex, maIndex, mepIndex, intIfNum, AVL_EXACT)) == NULL) ||
      (mepObjEntry->mepData == NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Invalid MEP on mdIndex %d maIndex %d mepId %d intf %d\r\n",
                       mdIndex, maIndex, mepIndex, intIfNum);
    return OFDPA_E_FAIL;
  }

  CSFTriggerHandler(mepObjEntry->mepData);

  return OFDPA_TRUE;
}

/*********************************************************************
* @purpose  This routine stops LBM test
*
* @param    pMPObj  @b{(input)} pmpobj on which the test has to be stopped
*
* @returns
*
* @comments
*
* @end
**********************************************************************/
void dot1agCfmLbmTestTerminate(dot1agCfmStackObj_t *pMEPObj)
{
  OFDPA_ERROR_t          rc  = OFDPA_E_NONE;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    return;
  }

/* Modified by JiaoYong, 2016/6/27 去掉LB stop对TVL的依赖*/
#if 0
  /* Terminate is valid only for Test */
  if (pMEPObj->mepData->dot1agCfmMepTransmitLbmTestTlvPresent != OFDPA_TRUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_VERBOSE,
                       "Stop is valid only for LBMs with test tlv  \r\n", 0);
    return;
  }
#endif 

  if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiActive == OFDPA_FALSE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_VERBOSE,
                       "LBM test is not running \r\n", 0);
    return;
  }

  /* Start LBI Timer for 5 seconds. */
  if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer != NULL)
  {
    rc = dot1agTimerUpdate(pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer, pMEPObj,
                           DOT1AG_CFM_LBI_TIMER_DEFAULT_MS, dot1agCfmLbiTimerCallback);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to stop LBM test, error in configuring Lbi timer \r\n", 0);
      return;
    }
  }
  else
  {
    pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer = dot1agTimerNodeAdd(DOT1AG_CFM_LBI_TIMER_DEFAULT_MS,
                                                                       dot1agCfmLbiTimerCallback, pMEPObj);
    if (pMEPObj->mepData->dot1agCfmMepLbiVar.lbiTimer == NULL)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to stop LBM test, error in configuring Lbi timer \r\n", 0);
      return;
    }
  }
}

/*********************************************************************
* @purpose  Clear Syntheic Loss Measurement LM Counters Cache
*
* @param    None
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Clear Syntheic Loss Measurement LM Counters Cache
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agSLMCacheClearProcess()
{
  OFDPA_ERROR_t             rc = OFDPA_E_NONE;

  avlPurgeAvlTree(&dot1agCFM.dot1agSlmTree, DOT1AG_SLM_ENTRIES_MAX);

  return rc;
}

