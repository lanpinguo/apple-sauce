/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
**********************************************************************
*
* @filename   y1731_pm.c
*
* @purpose    Y1731 PM
*
* @component  DOT1AG/Performance Management
*
* @comments   none
*
* @create     13-Dec-2013
*
**********************************************************************/

#include <stdlib.h>
#include "ofdpa_util.h"
#include "dot1ag_exports.h"
#include "dot1ag_ctrl.h"
#include "dot1ag_cnfgr.h"
#include "dot1ag_y1731pm.h"
#include "dot1ag.h"
#include "dot1ag_api.h"
#include "dot1ag_outcalls.h"
#include "dot1ag_pdu.h"
#include "dot1ag_procs.h"
#include "msgq.h"
#include "dot1ag_debug.h"

y1731PmCFM_t y1731PmCFM;
void              *y1731PmTaskSyncSem;

y1731ProLmBin_t y1731CurLmBin[OFDPA_DOT1AG_MAX_MEP_COUNT][MAX_Y1731_BIN_MODE];
y1731ProLmBin_t y1731HistoryLmBin[MAX_Y1731_HISTORY_BIN][OFDPA_DOT1AG_MAX_MEP_COUNT][MAX_Y1731_BIN_MODE];
uint32_t       y1731LmHistoryBinTail[OFDPA_DOT1AG_MAX_MEP_COUNT][MAX_Y1731_BIN_MODE];

y1731DmBin_t    y1731CurDmBin[MAX_DM_INSTANCE][MAX_Y1731_BIN_MODE];
y1731DmBin_t    y1731HistoryDmBin[MAX_Y1731_HISTORY_BIN][MAX_DM_INSTANCE][MAX_Y1731_BIN_MODE];
uint32_t       y1731DmHistoryBinTail[MAX_DM_INSTANCE][MAX_Y1731_BIN_MODE];

y1731CfmMepProlmTable_t          y1731CfmMepProlmTable[OFDPA_DOT1AG_MAX_MEP_COUNT];
y1731CfmMepOndemlmTable_t        y1731CfmMepOndemlmTable[OFDPA_DOT1AG_MAX_MEP_COUNT][8];
y1731CfmMepDmTable_t             y1731CfmMepDmTable[OFDPA_DOT1AG_MAX_MEP_COUNT][8];

OFDPA_ERROR_t y1731PmThreadCreate(void)
{
  DOT1AG_TRACE(CFM_PM, "y1731PmThreadCreate");
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_VERY_VERBOSE,
                     "=======================y1731PmThreadCreate.=======================\n", 0);

  /* Read/write lock to protect component data since processing occurs
   * on multiple threads (EFM-OAM thread, UI thread, NIM, dot1q, etc) */
  if (dpaRWlockInit(&y1731PmCFM.y1731PmCfgRWLock, NULL) == OFDPA_E_FAIL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to create read/write lock for Y1731 PM", 0);
    return OFDPA_E_FAIL;
  }

  /* Counting semaphore. Given whenever a message is added to any message queue
   * for the EFM-OAM thread. Taken when a message is read. */
  y1731PmCFM.y1731PmMsgQSema = dpaSemCreate("Y1731 MsQ Semaphore", DPA_SEM_COUNTING, 0);
  if (y1731PmCFM.y1731PmMsgQSema == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to create message queue semaphore for Y1731 PM.", 0);
    return OFDPA_E_FAIL;
  }

  /* create queue for events */
  y1731PmCFM.y1731PmEventQueue = msgqCreate(Y1731PM_QUEUE_EVENT, Y1731PM_EVENT_MSG_COUNT, sizeof(y1731PmMsg_t));
  if (y1731PmCFM.y1731PmEventQueue == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to create Event queue for Y1731 PM.", 0);
    return OFDPA_E_FAIL;
  }

  y1731PmTaskSyncSem = dpaSemCreate("Y1731 PM task sync semaphore", DPA_SEM_BINARY, 1);

  if (y1731PmTaskSyncSem == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS, "Failed to create Y1731 PM task syncing semaphore", 0);
    return OFDPA_E_FAIL;
  }

  y1731PmCFM.y1731PmTaskId = dpaThreadCreate(Y1731PM_TASK_NAME, 250, y1731PmTask, NULL);

  if (!y1731PmCFM.y1731PmTaskId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to create thread for Y1731 PM.", 0);
    return OFDPA_E_FAIL;
  }

  dpaSemTake(y1731PmTaskSyncSem, DPA_SEM_FOREVER);

  return OFDPA_E_NONE;
}

void y1731PmTask(void *arg)
{
  y1731PmMsg_t eventMsg;

  dpaSemGive(y1731PmTaskSyncSem);

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_VERY_VERBOSE,
                     "=======================y1731PmTask.=======================\n", 0);
  if (OFDPA_E_NONE != dpaTimerAdd(y1731OneSecondTimerCallback, &(y1731PmCFM.oneSecondTimer),
                                  TIMER_TICK_1000MSEC, 1, NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to create Y1731 Timer.", 0);
    return;
  }

  /* Loop forever, processing incoming messages */
  while (OFDPA_TRUE)
  {
    /* Since we are reading from multiple queues, we cannot wait forever
     * on the message receive from each queue. Use a semaphore to indicate
     * whether any queue has data. Wait until data is available. */
    if (dpaSemTake(y1731PmCFM.y1731PmMsgQSema, DPA_SEM_FOREVER) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to take Y1731 PM message queue semaphore.", 0);
      continue;
    }
    memset(&eventMsg, 0, sizeof(eventMsg));

    if (msgqRcv(y1731PmCFM.y1731PmEventQueue, &eventMsg) == OFDPA_E_NONE)
    {
      /* Configuration actions are all done on the UI thread.
       * Synchronize that processing with event processing by
       * taking the same semaphore here. */
      switch (eventMsg.event)
      {
        case y1731PmEventOneSecondTimer:
          if (dpaRWlockWritelock(&y1731PmCFM.y1731PmCfgRWLock) == OFDPA_E_NONE)
          {
            y1731PmUpdate();
            dpaRWlockUnlock(&y1731PmCFM.y1731PmCfgRWLock);
          }
          break;

        default:
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                             "Invalid event(%d) message type received in the Y1731 PM thread.",
                             eventMsg.event);
          break;
      }
    }
  } /* end while true */

  return;
}

void y1731OneSecondTimerCallback(timer_t timerCtrlBlk, void *ptrData)
{
  y1731PmMsg_t  msg;
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  if (!y1731PmCFM.y1731PmEventQueue)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Failed to handle Y1731 PM Timer Expiry as Event Queue is not initialized.\n", 0);
  }


  msg.event = y1731PmEventOneSecondTimer;

  rc = msgqSnd(y1731PmCFM.y1731PmEventQueue, &msg);

  if (dpaSemGive(y1731PmCFM.y1731PmMsgQSema) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to release Y1731 PM Message Queue Semaphore.\n", 0);
  }

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to send Y1731 PM Timer event.\n", 0);
  }

  return;
}

#define Y1731PM_FLR_DIVIDE(LF, TF) ((TF != 0) ? ((uint32_t)(((uint64_t)LF * 100000) / (uint64_t)TF)) : 0)
#define PM_NA_VALUE 0xFFFFFFFF

OFDPA_ERROR_t y1731PmUpdate()
{
//  L7_clocktime  ct;
  uint32_t secs;

  DOT1AG_TRACE(CFM_PM, "Pm Update, tick %d", dpaUpTimeMilliSeconds());

  /*
    osapiSleepMSec(500);
    DOT1AG_TRACE(CFM_PM, "after sleep, tick is %d", dpaUpTimeMilliSeconds());
  */

//  osapiUTCTimeGet(&ct);

  secs = dpaUpTimeSeconds();
//  y1731PmProcessLmBin(ct.seconds);
//  y1731PmProcessDmBin(ct.seconds);

  y1731PmProcessLmBin(secs);
  y1731PmProcessDmBin(secs);

  return OFDPA_E_NONE;
}

void y1731PmProcessLmBin(uint32_t curTimeStamp)
{
  OFDPA_BOOL        is15mLmBinEnabled, is24hLmBinEnabled, isFixed, isEnabled;
  y1731CfmMepLmPrim proactiveLmPrim;
  OFDPA_BOOL        isLmPrimValid;
  uint32_t          i;
  uint8_t           start, end;
  uint32_t          N_FLR, F_FLR;
  OFDPA_BOOL        N_valid, F_valid;

  for (i = 0; i < OFDPA_DOT1AG_MAX_MEP_COUNT; i++)
  {
    isEnabled = y1731CfmMepProlmTable[i].prolmEnabled;
    if (!isEnabled)
    {
      continue;
    }
    is15mLmBinEnabled = (y1731CfmMepProlmTable[i].prolmBinEnable & Y1731_BIN_FLAG_15M) ? 1:0;
    is24hLmBinEnabled = (y1731CfmMepProlmTable[i].prolmBinEnable & Y1731_BIN_FLAG_24H) ? 1:0;
  
    if (is15mLmBinEnabled || is24hLmBinEnabled)
    {
      DOT1AG_TRACE(CFM_PM, "LM for idx %d", i);
      /* get lm pm bin primitives*/
      if (OFDPA_E_NONE != y1731PmGetLmBinPrim(i, &proactiveLmPrim, &isLmPrimValid))
      {
        continue;
      }
  
      /*calculate lm pm bin event, including N_FLR, F_FLR*/
      y1731PmGetLmBinEvent(isLmPrimValid, proactiveLmPrim.nNTF, proactiveLmPrim.nNLF, OFDPA_FALSE, &N_FLR, &N_valid);
      y1731PmGetLmBinEvent(isLmPrimValid, proactiveLmPrim.nFTF, proactiveLmPrim.nFLF, OFDPA_FALSE, &F_FLR, &F_valid);
  
      /*calulate loop range for 15m and 24h bin*/
      y1731PmCalcLoopRange(is15mLmBinEnabled, is24hLmBinEnabled, &start, &end);
      for (; start < end; start++)
      {
        if (!y1731CfmMepProlmTable[i].prolmCurBin[start].startTime)
          y1731CfmMepProlmTable[i].prolmCurBin[start].startTime = curTimeStamp;
  
        /*calculate lm pm bin result*/
        y1731PmCalcLmBin(i, start, N_FLR, N_valid, proactiveLmPrim.nNTF, proactiveLmPrim.nNLF,
                         F_FLR, F_valid, proactiveLmPrim.nFTF, proactiveLmPrim.nFLF);
  
        y1731PmCheckTimeIsFixedValue(start, curTimeStamp, &isFixed);
        /*to determine whether move current bin to history bin   */
        if (isFixed)
        {
          y1731PmMoveCurrentLmBintoHistory(i, start, curTimeStamp);
        }
      }
    }
  }
}

OFDPA_ERROR_t y1731PmGetLmBinPrim(uint16_t lmIdx, y1731CfmMepLmPrim *lmPrim, OFDPA_BOOL *isValid)
{
  memcpy(lmPrim, &y1731CfmMepProlmTable[lmIdx].lmPrim, sizeof(*lmPrim));
  memset(&y1731CfmMepProlmTable[lmIdx].lmPrim, 0, sizeof(y1731CfmMepProlmTable[lmIdx].lmPrim));

#ifdef Y1731_PM_SIM_GET
  lmPrim->nNTF = 100000;
  lmPrim->nNLF = ((uint32_t)rand()) %100;
  lmPrim->nFTF = 200000;
  lmPrim->nFLF = ((uint32_t)rand()) %200;
#endif
  *isValid = OFDPA_TRUE;
  return OFDPA_E_NONE;
}

void y1731PmGetLmBinEvent(OFDPA_BOOL isFrameValid, uint32_t totalFrame, uint32_t lostFrame, OFDPA_BOOL defectSecond,
                          uint32_t *frameLossRate, OFDPA_BOOL *valid)
{
  /* when TF > 0, DS = false, then it is valid measurement */
  if (isFrameValid && (totalFrame != 0) && (!defectSecond ))
  {
    *frameLossRate = Y1731PM_FLR_DIVIDE(lostFrame, totalFrame);    /* FLR unit is 0.001% */
    *valid = OFDPA_TRUE;
  }
  else
  {
    *frameLossRate = 0;
    *valid = OFDPA_FALSE;
  }
}

void y1731PmCalcLmBin(uint16_t lmIdx, uint8_t binMode, uint32_t N_FLR, OFDPA_BOOL N_valid, uint32_t N_TF, uint32_t N_LF,
                      uint32_t F_FLR, OFDPA_BOOL F_valid, uint32_t F_TF, uint32_t F_LF)
{
  y1731ProLmBin_t *y1731CurLmBin;

  y1731CurLmBin = &y1731CfmMepProlmTable[lmIdx].prolmCurBin[binMode];
  if (N_valid)
  {
    /*initialize FLR*/
    if (y1731CurLmBin->N_FLR_Total_Num == 0)
    {
      y1731CurLmBin->xN_FLR = N_FLR;
      y1731CurLmBin->mN_FLR = N_FLR;
      y1731CurLmBin->aN_FLR = Y1731PM_FLR_DIVIDE(N_LF, N_TF);
      y1731CurLmBin->Total_N_TF = N_TF;
      y1731CurLmBin->Total_N_LF = N_LF;
    }
    else
    {
      if (N_FLR > y1731CurLmBin->xN_FLR)
      {
        y1731CurLmBin->xN_FLR = N_FLR;
      }

      if (N_FLR < y1731CurLmBin->mN_FLR)
      {
        y1731CurLmBin->mN_FLR = N_FLR;
      }

      y1731CurLmBin->Total_N_TF += N_TF;
      y1731CurLmBin->Total_N_LF += N_LF;

      y1731CurLmBin->aN_FLR = Y1731PM_FLR_DIVIDE(y1731CurLmBin->Total_N_LF,
                                                 y1731CurLmBin->Total_N_TF);
    }

    y1731CurLmBin->N_FLR_Total_Num++;

    /*set valid flag*/
    y1731CurLmBin->isValid = OFDPA_TRUE;
  }

  if (F_valid)
  {
    /* initialize FLR */
    if (y1731CurLmBin->F_FLR_Total_Num == 0)
    {
      y1731CurLmBin->xF_FLR = F_FLR;
      y1731CurLmBin->mF_FLR = F_FLR;
      y1731CurLmBin->aF_FLR = Y1731PM_FLR_DIVIDE(F_LF, F_TF);
      y1731CurLmBin->Total_F_TF = F_TF;
      y1731CurLmBin->Total_F_LF = F_LF;
    }
    else
    {
      if (F_FLR > y1731CurLmBin->xF_FLR)
      {
        y1731CurLmBin->xF_FLR = F_FLR;
      }

      if (F_FLR < y1731CurLmBin->mF_FLR)
      {
        y1731CurLmBin->mF_FLR = F_FLR;
      }

      y1731CurLmBin->Total_F_TF += F_TF;
      y1731CurLmBin->Total_F_LF += F_LF;

      y1731CurLmBin->aF_FLR = Y1731PM_FLR_DIVIDE(y1731CurLmBin->Total_F_LF,
                                                 y1731CurLmBin->Total_F_TF);
    }

    y1731CurLmBin->F_FLR_Total_Num++;

    /* set valid flag*/
    y1731CurLmBin->isValid = OFDPA_TRUE;
  }
  if (y1731CurLmBin->aF_FLR >= 100000 || y1731CurLmBin->aN_FLR >= 100000)
  {
    y1731CurLmBin->suspectFlag = OFDPA_TRUE;
  }
}

void y1731PmDumpLmBin(y1731ProLmBin_t *prolmBin)
{
  printf("time       %10u, %10u, valid %4u, suspect %4u\n",
         prolmBin->startTime,
         prolmBin->endTime,
         prolmBin->isValid,
         prolmBin->suspectFlag);
  printf("              ----MIN,----AVERAGE,    ----MAX\n");
  printf("N_FLR:     %10u, %10u, %10u\n",
         prolmBin->mN_FLR,
         prolmBin->aN_FLR,
         prolmBin->xN_FLR);
  printf("F_FLR:     %10u, %10u, %10u\n",
         prolmBin->mF_FLR,
         prolmBin->aF_FLR,
         prolmBin->xF_FLR);
  printf("FLR_NUM:   NE%8u, FE%8u\n",
         prolmBin->N_FLR_Total_Num,
         prolmBin->F_FLR_Total_Num);
  printf("NEAR_END:  TF%8u, LF%8u\n",
         prolmBin->Total_N_TF,
         prolmBin->Total_N_LF);
  printf("FAR_END:   TF%8u, LF%8u\n",
         prolmBin->Total_F_TF,
         prolmBin->Total_F_LF);
  /*
    printf("time:     [%10d, %10d]\n",
    prolmBin->startTime,
    prolmBin->endTime);
    printf("valid:     %10d\n",
    prolmBin->isValid);
    printf("suspect:   %10d\n",
    prolmBin->suspectFlag);
  */
}

void y1731PmDumpCurLmBin(uint16_t lmIdx, uint8_t binMode)
{
  dot1agCfmStackObj_t     *pMEPObj;
  y1731CfmMepProlmTable_t *prolmEntry;
  y1731ProLmBin_t         *prolmBin;

  prolmEntry = &y1731CfmMepProlmTable[lmIdx];
  prolmBin = &prolmEntry->prolmCurBin[binMode];

  pMEPObj = prolmEntry->pMPObj;
  if (pMEPObj == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP for LM %d", lmIdx);
    return;
  }

  printf("\nLM for idx %d, mode %d on (intf %d, level %d, mepId %d):\n",
         lmIdx, binMode,
         pMEPObj->key.dot1agCfmStackifIndex,
         pMEPObj->key.dot1agCfmStackMdLevel,
         (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);
  y1731PmDumpLmBin(prolmBin);

}

OFDPA_ERROR_t y1731PmDumpLm(uint32_t intIfNum, uint32_t mdIndex,
                            uint32_t maIndex, uint32_t mepId,
                            uint32_t priorityClass, uint8_t binMode)
{
  dot1agCfmMepObj_t       *mepObj;
  dot1agCfmStackObj_t     *pMEPObj;
  y1731CfmMepProlmTable_t *prolmEntry;
  y1731ProLmBin_t         *prolmBin;
  uint32_t                 binTail, i, num;

  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP %d", mepId);
    return OFDPA_E_ERROR;
  }

  if (mepObj->mepData == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "Invalid mepData %d", 0);
    return OFDPA_E_ERROR;
  }

  if (mepObj->mepData->y1731CfmMepProlmEntry == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get LM %d", mepId);
    return OFDPA_E_NONE;
  }

  pMEPObj = mepObj->mepData->y1731CfmMepProlmEntry->pMPObj;
  if (pMEPObj == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP for LM %d", mepId);
    return OFDPA_E_ERROR;
  }

  prolmEntry = mepObj->mepData->y1731CfmMepProlmEntry;
  printf("\nLM for mode %d on (intf %d, level %d, mepId %d):\n",
         binMode,
         pMEPObj->key.dot1agCfmStackifIndex,
         pMEPObj->key.dot1agCfmStackMdLevel,
         (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);
  prolmBin = &prolmEntry->prolmCurBin[binMode];
  printf("----------CURRENT BIN------------------------\n");
  y1731PmDumpLmBin(prolmBin);
  binTail = prolmEntry->prolmHistoryBinTail[binMode];
  num = (binTail < MAX_Y1731_HISTORY_BIN) ? binTail : MAX_Y1731_HISTORY_BIN;
  for (i = 0; i < num; i++)
  {
    printf("----------HISTORY BIN %6d-----------------\n", binTail-i-1);
    prolmBin = &prolmEntry->prolmHistoryBin[(binTail-i-1)%MAX_Y1731_HISTORY_BIN][binMode];
    y1731PmDumpLmBin(prolmBin);
  }
  return OFDPA_E_NONE;
}

void y1731PmMoveCurrentLmBintoHistory(uint16_t lmIdx, uint8_t binMode, uint32_t curTimeStamp)
{
  uint8_t historyInd;

  //get oldest history bin index
  historyInd = y1731CfmMepProlmTable[lmIdx].prolmHistoryBinTail[binMode] % MAX_Y1731_HISTORY_BIN;

  /*move current bin to the oldest history bin*/
  y1731CfmMepProlmTable[lmIdx].prolmCurBin[binMode].endTime = curTimeStamp;
  memcpy(&y1731CfmMepProlmTable[lmIdx].prolmHistoryBin[historyInd][binMode],
         &y1731CfmMepProlmTable[lmIdx].prolmCurBin[binMode],
         sizeof(y1731CfmMepProlmTable[lmIdx].prolmHistoryBin[historyInd][binMode]));

  DOT1AG_TRACE(CFM_PM, "idx is %d", y1731CfmMepProlmTable[lmIdx].prolmHistoryBinTail[binMode]);
  y1731PmDumpCurLmBin(lmIdx, binMode);
  /*clear current bin*/
  y1731PmClearCurrentLmBin(lmIdx, binMode);

  /*set current bin startTime */
  y1731CfmMepProlmTable[lmIdx].prolmCurBin[binMode].startTime = curTimeStamp;

  /*notify upper layer application lm bin end timestamp*/
  //CbHandler.ReportBinEndTimestampHandler(LM_BIN, mepIdx, binMode, curTimeStamp);

  /*increase HistoryBinTail*/
  y1731CfmMepProlmTable[lmIdx].prolmHistoryBinTail[binMode]++;
}

void y1731PmProcessDmBin(uint32_t curTimeStamp)
{
  OFDPA_BOOL        is15mDmBinEnabled, is24hDmBinEnabled, isEnabled, isValid, isFixed;
  y1731CfmMepDmPrim dmPrim;
  OFDPA_BOOL        isDmPrimValid;
  uint32_t          N_FD, F_FD, B_FD, i, j;
  uint8_t           start, end;

  for (i = 0; i < MAX_DM_INSTANCE; i++)
  {
    for (j = 0; j < 8; j++)
    {
      isEnabled = y1731CfmMepDmTable[i][j].dmEnabled;
      if (!isEnabled)
      {
        continue;
      }
      DOT1AG_TRACE(CFM_PM, "DM for idx %d, pri %d", i, j);
      is15mDmBinEnabled = (y1731CfmMepDmTable[i][j].dmBinEnable & Y1731_BIN_FLAG_15M) ? 1:0;
      is24hDmBinEnabled = (y1731CfmMepDmTable[i][j].dmBinEnable & Y1731_BIN_FLAG_24H) ? 1:0;

      if (is15mDmBinEnabled || is24hDmBinEnabled)
      {
        /* get dm pm bin primitives */
        if (OFDPA_E_NONE != y1731PmGetDmBinPrim(i, j, &dmPrim, &isDmPrimValid))
        {
          continue;
        }

        /*calculate dm pm event N_FD, F_FD, B_FD*/
        y1731PmGetDmBinEvent(&dmPrim, &N_FD, &F_FD, &B_FD, &isValid);

        /*calulate loop range for 15m and 24h bin*/
        y1731PmCalcLoopRange(is15mDmBinEnabled, is24hDmBinEnabled, &start, &end);
        for (; start < end; start++)
        {
          if (!y1731CfmMepDmTable[i][j].dmCurBin[start].startTime)
            y1731CfmMepDmTable[i][j].dmCurBin[start].startTime = curTimeStamp;
          /*calculate internal lm pm bin result*/
          y1731PmCalcDmBin(i, j, start, N_FD, F_FD, B_FD);

          y1731PmCheckTimeIsFixedValue(start, curTimeStamp, &isFixed);
          /*to determine whether move current bin to history bin   */
          if (isFixed)
          {
            y1731PmMoveCurrentDmBintoHistory(i, j, start, curTimeStamp);
          }
        }
      }
    }
  }

}

OFDPA_ERROR_t y1731PmGetDmBinPrim(uint16_t dmIdx, uint8_t priority, y1731CfmMepDmPrim *dmPrim, OFDPA_BOOL *isValid)
{
  memcpy(dmPrim, &y1731CfmMepDmTable[dmIdx][priority].dmPrim, sizeof(*dmPrim));
#ifdef Y1731_PM_SIM_GET
  dmPrim->tNFD.ns = (20 - (((uint32_t)rand()) %40))*1000;
  dmPrim->tFFD.ns = (20 - (((uint32_t)rand()) %40))*1000;
  dmPrim->tBFD.ns = (((uint32_t)rand()) %40)*100000;
#endif
  *isValid = OFDPA_TRUE;
  return OFDPA_E_NONE;
}

void y1731PmGetDmBinEvent(y1731CfmMepDmPrim *dmPrim, uint32_t *N_FD, uint32_t *F_FD, uint32_t *B_FD, OFDPA_BOOL *isValid)
{
  *isValid = OFDPA_FALSE;

  *N_FD = dmPrim->tNFD.ns;
  *F_FD = dmPrim->tFFD.ns;
  *B_FD = dmPrim->tBFD.ns;
  *isValid = OFDPA_TRUE;
}

void y1731PmCalcDmBin(uint16_t dmIdx, uint8_t priority, uint8_t binMode, uint32_t N_FD, uint32_t F_FD, uint32_t B_FD)
{
  uint32_t      N_FDV, F_FDV;
  uint32_t      N_FD_pre, F_FD_pre;
  y1731DmBin_t *y1731CurDmBin;

  y1731CurDmBin = &y1731CfmMepDmTable[dmIdx][priority].dmCurBin[binMode];

  if (y1731CurDmBin->Total_Num == 0)
  {
    /*initialize FD*/
    y1731CurDmBin->xB_FD = B_FD;
    y1731CurDmBin->mB_FD = B_FD;
    y1731CurDmBin->aB_FD = B_FD;

    y1731CurDmBin->Total_B_FD = B_FD;
  }
  else
  {
    if (B_FD > y1731CurDmBin->xB_FD)
    {
      y1731CurDmBin->xB_FD = B_FD;
    }

    if (B_FD < y1731CurDmBin->mB_FD)
    {
      y1731CurDmBin->mB_FD = B_FD;
    }

    y1731CurDmBin->Total_B_FD += B_FD;

    y1731CurDmBin->aB_FD = y1731CurDmBin->Total_B_FD / (y1731CurDmBin->Total_Num + 1);
  }

  if (y1731CurDmBin->Total_Num == 0)
  {
    y1731CurDmBin->xN_FDV = PM_NA_VALUE;
    y1731CurDmBin->mN_FDV = PM_NA_VALUE;
    y1731CurDmBin->aN_FDV = PM_NA_VALUE;
    y1731CurDmBin->xF_FDV = PM_NA_VALUE;
    y1731CurDmBin->mF_FDV = PM_NA_VALUE;
    y1731CurDmBin->aF_FDV = PM_NA_VALUE;
  }
  else
  {
    N_FD_pre = y1731CurDmBin->N_FD_Pre;
    F_FD_pre = y1731CurDmBin->F_FD_Pre;

    /* calculate the FDV */
    y1731PmCalcDmFDV(N_FD_pre, N_FD, &N_FDV);
    y1731PmCalcDmFDV(F_FD_pre, F_FD, &F_FDV);

    /*initialize *N_FDV, *F_FDV*/
    if (y1731CurDmBin->Total_Num == 1)
    {
      y1731CurDmBin->xN_FDV = N_FDV;
      y1731CurDmBin->mN_FDV = N_FDV;
      y1731CurDmBin->aN_FDV = N_FDV;
      y1731CurDmBin->xF_FDV = F_FDV;
      y1731CurDmBin->mF_FDV = F_FDV;
      y1731CurDmBin->aF_FDV = F_FDV;

      y1731CurDmBin->Total_N_FDV = N_FDV;
      y1731CurDmBin->Total_F_FDV = F_FDV;
    }
    else
    {
      if (N_FDV > y1731CurDmBin->xN_FDV)
      {
        y1731CurDmBin->xN_FDV = N_FDV;
      }

      if (N_FDV < y1731CurDmBin->mN_FDV)
      {
        y1731CurDmBin->mN_FDV = N_FDV;
      }
      y1731CurDmBin->Total_N_FDV += N_FDV;
      y1731CurDmBin->aN_FDV = (y1731CurDmBin->Total_N_FDV) / (y1731CurDmBin->Total_Num);

      if (F_FDV > y1731CurDmBin->xF_FDV)
      {
        y1731CurDmBin->xF_FDV = F_FDV;
      }

      if (F_FDV <  y1731CurDmBin->mF_FDV)
      {
        y1731CurDmBin->mF_FDV = F_FDV;
      }
      y1731CurDmBin->Total_F_FDV += F_FDV;
      y1731CurDmBin->aF_FDV = (y1731CurDmBin->Total_F_FDV) / (y1731CurDmBin->Total_Num);
    }
  }

  /*save current N_FD and F_FD as previous N_FD and F_FD*/
  y1731CurDmBin->N_FD_Pre = N_FD;
  y1731CurDmBin->F_FD_Pre = F_FD;

  /*total FD number increase*/
  y1731CurDmBin->Total_Num++;

  /*set valid flag*/
  if (B_FD)
  {
    y1731CurDmBin->isValid = OFDPA_TRUE;
  }

  return;
}

void y1731PmCalcDmFDV(uint32_t FD_pre, uint32_t FD, uint32_t *FDV)
{
  /*
   *  calculate the FDV:
   *   FDV = abs(FD - FD_pre)
   */
  int iFDV;

  iFDV = FD - FD_pre;
  if (iFDV < 0)
  {
    iFDV = 0 - iFDV;
  }
  *FDV = iFDV;
  DOT1AG_TRACE(CFM_PM, "FDV 0x%8x, 0x%8x -> %d", FD_pre, FD, *FDV);

  /* set max with two seconds */
  if (*FDV > (2 * 1000000000))
  {
    *FDV = 0;
  }
}

void y1731PmDumpDmBin(y1731DmBin_t *dmBin)
{
  printf("time   %10u, %10u, valid %4u\n",
         dmBin->startTime,
         dmBin->endTime,
         dmBin->isValid);
  printf("                   MIN,    AVERAGE,        MAX\n");
  printf("B_FD:       %10u, %10u, %10u\n",
         dmBin->mB_FD,
         dmBin->aB_FD,
         dmBin->xB_FD);
  printf("N_FDV:      %10u, %10u, %10u\n",
         dmBin->mN_FDV,
         dmBin->aN_FDV,
         dmBin->xN_FDV);
  printf("F_FDV:      %10u, %10u, %10u\n",
         dmBin->mF_FDV,
         dmBin->aF_FDV,
         dmBin->xF_FDV);
  /*
    printf("FD_NUM:     %10d\n",
    y1731CfmMepDmTable[dmIdx][priority].dmCurBin[binMode].nFDM);
  */
  printf("T_Num:      %10u\n",
         dmBin->Total_Num);
  printf("T_B_FD:     %10u\n",
         dmBin->Total_B_FD);
  printf("T_N_FDV:    %10u\n",
         dmBin->Total_N_FDV);
  printf("T_F_FDV:    %10u\n",
         dmBin->Total_F_FDV);

}

void y1731PmDumpCurDmBin(uint16_t dmIdx, uint8_t priority, uint8_t binMode)
{
  dot1agCfmStackObj_t  *pMEPObj;
  y1731CfmMepDmTable_t *dmEntry;
  y1731DmBin_t         *dmBin;

  dmEntry = &y1731CfmMepDmTable[dmIdx][priority];
  dmBin = &dmEntry->dmCurBin[binMode];

  pMEPObj = dmEntry->pMPObj;
  if (pMEPObj == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP for DM %d, pri %d", dmIdx, priority);
    return;
  }

  printf("\nDM for idx %d, pri %d, mode %d on (intf %d, level %d, mepId %d):\n",
         dmIdx, priority, binMode,
         pMEPObj->key.dot1agCfmStackifIndex,
         pMEPObj->key.dot1agCfmStackMdLevel,
         (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);
  y1731PmDumpDmBin(dmBin);
}

OFDPA_ERROR_t y1731PmDumpDm(uint32_t intIfNum, uint32_t mdIndex,
                            uint32_t maIndex, uint32_t mepId,
                            uint32_t pri, uint8_t binMode)
{
  dot1agCfmMepObj_t    *mepObj;
  dot1agCfmStackObj_t  *pMEPObj;
  y1731CfmMepDmTable_t *dmEntry;
  y1731DmBin_t         *dmBin;
  uint32_t              binTail, i, num;

  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP %d", mepId);
    return OFDPA_E_ERROR;
  }

  if (mepObj->mepData == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "Invalid mepData %d", 0);
    return OFDPA_E_ERROR;
  }

  if (mepObj->mepData->y1731CfmMepDmEntry[pri] == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get DM %d", mepId);
    return OFDPA_E_NONE;
  }

  pMEPObj = mepObj->mepData->y1731CfmMepDmEntry[pri]->pMPObj;
  if (pMEPObj == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP for DM %d", mepId);
    return OFDPA_E_ERROR;
  }

  dmEntry = mepObj->mepData->y1731CfmMepDmEntry[pri];
  printf("\nDM for pri %d, mode %d on (intf %d, level %d, mepId %d):\n",
         pri, binMode,
         pMEPObj->key.dot1agCfmStackifIndex,
         pMEPObj->key.dot1agCfmStackMdLevel,
         (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0);
  dmBin = &dmEntry->dmCurBin[binMode];
  printf("----------CURRENT BIN------------------------\n");
  y1731PmDumpDmBin(dmBin);
  binTail = dmEntry->dmHistoryBinTail[binMode];
  num = (binTail < MAX_Y1731_HISTORY_BIN) ? binTail : MAX_Y1731_HISTORY_BIN;
  for (i = 0; i < num; i++)
  {
    printf("----------HISTORY BIN %6d-----------------\n", binTail-i-1);
    dmBin = &dmEntry->dmHistoryBin[(binTail-i-1)%MAX_Y1731_HISTORY_BIN][binMode];
    y1731PmDumpDmBin(dmBin);
  }
  return OFDPA_E_NONE;
}

void y1731PmMoveCurrentDmBintoHistory(uint16_t dmIdx, uint8_t priority, uint8_t binMode, uint32_t curTimeStamp)
{
  uint8_t historyInd;

  //get oldest history bin index
  historyInd = y1731CfmMepDmTable[dmIdx][priority].dmHistoryBinTail[binMode] % MAX_Y1731_HISTORY_BIN;

  /*move current bin to the oldest history bin*/
  y1731CfmMepDmTable[dmIdx][priority].dmCurBin[binMode].endTime = curTimeStamp;
  memcpy(&y1731CfmMepDmTable[dmIdx][priority].dmHistoryBin[historyInd][binMode],
         &y1731CfmMepDmTable[dmIdx][priority].dmCurBin[binMode],
         sizeof(y1731CfmMepDmTable[dmIdx][priority].dmHistoryBin[historyInd][binMode]));

  DOT1AG_TRACE(CFM_PM, "idx is %d", y1731CfmMepDmTable[dmIdx][priority].dmHistoryBinTail[binMode]);
  y1731PmDumpCurDmBin(dmIdx, priority, binMode);
  /*clear current bin*/
  y1731PmClearCurrentDmBin(dmIdx, priority, binMode);

  /*set current bin startTime */
  y1731CfmMepDmTable[dmIdx][priority].dmCurBin[binMode].startTime = curTimeStamp;

  /*notify upper layer application lm bin end timestamp*/
  //CbHandler.ReportBinEndTimestampHandler(DM_BIN, mepIdx, binMode, curTimeStamp);

  /*increase HistoryBinTail*/
  y1731CfmMepDmTable[dmIdx][priority].dmHistoryBinTail[binMode]++;
}

OFDPA_BOOL y1731BinIntervalDebugEnable = 0;

void y1731PmSetBinIntervalDebug (OFDPA_BOOL enable)
{
  y1731BinIntervalDebugEnable = enable;
}

OFDPA_ERROR_t y1731PmCheckTimeIsFixedValue(uint8_t binMode, uint32_t timeStamp, OFDPA_BOOL *isFixed)
{
  /*calculate if the startTime is fixed value*/
  *isFixed = OFDPA_FALSE;
  switch(binMode)
  {
    case Y1731_BIN_MODE_15M:
      /*the timeStamp is fixed value if it equals to values as 00:15:00, 00:30:00, 00:45:00, 01:00:00, etc*/
      if ((0 == y1731BinIntervalDebugEnable  && 0 == timeStamp % 900) ||
          (1 == y1731BinIntervalDebugEnable  && 0 == timeStamp % 60))
      {
        *isFixed = OFDPA_TRUE;
      }
      break;

    case Y1731_BIN_MODE_24H:
      /*the timeStamp is fixed value if it equals to values as 00:00:00, etc*/
      if ((0 == y1731BinIntervalDebugEnable  && 0 == timeStamp % 86400) ||
          (1 == y1731BinIntervalDebugEnable  && 0 == timeStamp % 300))
      {
        *isFixed = OFDPA_TRUE;
      }
      break;

    default:
      return OFDPA_E_ERROR;
  }

  return OFDPA_E_NONE;
}

void y1731PmCalcLoopRange(OFDPA_BOOL is15mBinEnabled, OFDPA_BOOL is24hBinEnabled, uint8_t *start, uint8_t *end)
{
  if (is15mBinEnabled)
  {
    if (is24hBinEnabled)
    {
      *start = 0;
      *end = 2;
    }
    else
    {
      *start = 0;
      *end = 1;
    }
  }
  else if (is24hBinEnabled)
  {
    *start = 1;
    *end = 2;
  }
  else
  {
    *start = 0;
    *end = 0;
  }
}

void y1731PmClearCurrentLmBin(uint16_t lmIdx, uint8_t binMode)
{
  memset(&y1731CfmMepProlmTable[lmIdx].prolmCurBin[binMode],
         0,
         sizeof(y1731CfmMepProlmTable[lmIdx].prolmCurBin[binMode]));
}
void y1731PmClearCurrentDmBin(uint16_t dmIdx, uint8_t priority, uint8_t binMode)
{
  memset(&y1731CfmMepDmTable[dmIdx][priority].dmCurBin[binMode],
         0,
         sizeof(y1731CfmMepDmTable[dmIdx][priority].dmCurBin[binMode]));
}

OFDPA_ERROR_t y1731PmGetProLmCounters(uint32_t intIfNum, uint32_t mdIndex,
                                      uint32_t maIndex, uint32_t mepId,
                                      y1731CfmMepProlmTable_t *prolmEntry)
{
  dot1agCfmMepObj_t       *mepObj;

  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP %d", mepId);
    return OFDPA_E_NOT_FOUND;
  }

  if (mepObj->mepData == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "Invalid mepData %d", 0);
    return OFDPA_E_ERROR;
  }

  if (mepObj->mepData->y1731CfmMepProlmEntry == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get LM %d", mepId);
    return OFDPA_E_NOT_FOUND;
  }

  memcpy(prolmEntry, mepObj->mepData->y1731CfmMepProlmEntry, sizeof(y1731CfmMepProlmTable_t));

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t y1731PmGetProDmCounters(uint32_t intIfNum, uint32_t mdIndex,
                                      uint32_t maIndex, uint32_t mepId,
                                      uint32_t pri, y1731CfmMepDmTable_t *dmEntry)
{
  dot1agCfmMepObj_t    *mepObj;

  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP %d", mepId);
    return OFDPA_E_NOT_FOUND;
  }

  if (mepObj->mepData == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "Invalid mepData %d", 0);
    return OFDPA_E_ERROR;
  }

  if (mepObj->mepData->y1731CfmMepDmEntry[pri] == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get DM %d", mepId);
    return OFDPA_E_NOT_FOUND;
  }

  memcpy(dmEntry, mepObj->mepData->y1731CfmMepDmEntry[pri], sizeof(y1731CfmMepDmTable_t));

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t y1731PmGetOdLmCounters(uint32_t intIfNum, uint32_t mdIndex,
                                     uint32_t maIndex, uint32_t mepId,
                                     uint32_t priorityClass, y1731CfmMepOndemlmTable_t *odlmEntry)
{
  dot1agCfmMepObj_t       *mepObj;

  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP %d", mepId);
    return OFDPA_E_NOT_FOUND;
  }

  if (mepObj->mepData == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "Invalid mepData %d", 0);
    return OFDPA_E_ERROR;
  }

  if (mepObj->mepData->y1731CfmMepOndemlmEntry[priorityClass] == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get LM %d", mepId);
    return OFDPA_E_NOT_FOUND;
  }

  memcpy(odlmEntry, mepObj->mepData->y1731CfmMepOndemlmEntry[priorityClass], sizeof(y1731CfmMepOndemlmTable_t));

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t y1731PmGetOdDmCounters(uint32_t intIfNum, uint32_t mdIndex,
                                     uint32_t maIndex, uint32_t mepId,
                                     uint32_t pri, y1731CfmMepDmTable_t *dmEntry)
{
  dot1agCfmMepObj_t    *mepObj;

  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get MP %d", mepId);
    return OFDPA_E_NOT_FOUND;
  }

  if (mepObj->mepData == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "Invalid mepData %d", 0);
    return OFDPA_E_ERROR;
  }

  if (mepObj->mepData->y1731CfmMepDmEntry[pri] == NULL)
  {
    DOT1AG_TRACE(CFM_PM, "fail to get DM %d", mepId);
    return OFDPA_E_NOT_FOUND;
  }

  if (mepObj->mepData->y1731CfmMepDmEntry[pri]->dmStartTime)
  {
    mepObj->mepData->y1731CfmMepDmEntry[pri]->dmEndTime = dpaUpTimeMilliSeconds();
  }
  memcpy(dmEntry, mepObj->mepData->y1731CfmMepDmEntry[pri], sizeof(y1731CfmMepDmTable_t));

  return OFDPA_E_NONE;
}
