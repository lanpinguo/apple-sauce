/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2015
*
**********************************************************************
*
* @filename   dot1ag_cnfgr.c
*
* @purpose    Contains definitions to support the configurator framework
*             for dot1ag component
*
* @component  DOT1AG/Connectivity & Fault Management
*
* @comments   none
*
* @create     13-Feb-2009
*
* @end
*
**********************************************************************/

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ofdpa_porting.h"
//#include "driver_oam.h"
#include "dot1ag_exports.h"
#include "dot1ag_cnfgr.h"
#include "dot1ag_api.h"
#include "dot1ag_ctrl.h"
#include "dot1ag_y1731pm.h"
#include "dot1ag_pdu.h"
#include "dot1ag.h"
#include "dot1ag_procs.h"
#include "msgq.h"
#include "g8131.h"

dot1agCFM_t  dot1agCFM;
void        *dot1agTaskSyncSem;

static int dot1agCfmRmepObjKeyCompareFcn(void *a, void *b, size_t l)
{
  dot1agCfmRMepObjKey_t *pA = a;
  dot1agCfmRMepObjKey_t *pB = b;

  if (pA->dot1agCfmMdIndex > pB->dot1agCfmMdIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMdIndex < pB->dot1agCfmMdIndex)
  {
    return -1;
  }

  if (pA->dot1agCfmMaIndex > pB->dot1agCfmMaIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMaIndex < pB->dot1agCfmMaIndex)
  {
    return -1;
  }

  if (pA->dot1agCfmMepIdentifier > pB->dot1agCfmMepIdentifier)
  {
    return 1;
  }
  if (pA->dot1agCfmMepIdentifier < pB->dot1agCfmMepIdentifier)
  {
    return -1;
  }

  if (pA->dot1agCfmMepDbRMepIdentifier > pB->dot1agCfmMepDbRMepIdentifier)
  {
    return 1;
  }
  if (pA->dot1agCfmMepDbRMepIdentifier < pB->dot1agCfmMepDbRMepIdentifier)
  {
    return -1;
  }

  return 0;
}

static int dot1agCfmLtrObjKeyCompareFcn(void *a, void *b, size_t l)
{
  dot1agCfmLtrObjKey_t *pA = a;
  dot1agCfmLtrObjKey_t *pB = b;

  if (pA->dot1agCfmMdIndex > pB->dot1agCfmMdIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMdIndex < pB->dot1agCfmMdIndex)
  {
    return -1;
  }

  if (pA->dot1agCfmMaIndex > pB->dot1agCfmMaIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMaIndex < pB->dot1agCfmMaIndex)
  {
    return -1;
  }

  if (pA->dot1agCfmMepIdentifier > pB->dot1agCfmMepIdentifier)
  {
    return 1;
  }
  if (pA->dot1agCfmMepIdentifier < pB->dot1agCfmMepIdentifier)
  {
    return -1;
  }

  if (pA->dot1agCfmLtrSeqNumber > pB->dot1agCfmLtrSeqNumber)
  {
    return 1;
  }
  if (pA->dot1agCfmLtrSeqNumber < pB->dot1agCfmLtrSeqNumber)
  {
    return -1;
  }

  if (pA->dot1agCfmLtrReceiveOrder > pB->dot1agCfmLtrReceiveOrder)
  {
    return 1;
  }
  if (pA->dot1agCfmLtrReceiveOrder < pB->dot1agCfmLtrReceiveOrder)
  {
    return -1;
  }

  return 0;
}

static int dot1agCfmMepObjKeyCompareFcn(void *a, void *b, size_t l)
{
  dot1agCfmMepObjKey_t *pA = a;
  dot1agCfmMepObjKey_t *pB = b;

  if (pA->dot1agCfmMdIndex > pB->dot1agCfmMdIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMdIndex < pB->dot1agCfmMdIndex)
  {
    return -1;
  }

  if (pA->dot1agCfmMaIndex > pB->dot1agCfmMaIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMaIndex < pB->dot1agCfmMaIndex)
  {
    return -1;
  }

  if (pA->dot1agCfmMepIdentifier > pB->dot1agCfmMepIdentifier)
  {
    return 1;
  }
  if (pA->dot1agCfmMepIdentifier < pB->dot1agCfmMepIdentifier)
  {
    return -1;
  }

  if (pA->dot1agCfmMepIfIndex > pB->dot1agCfmMepIfIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMepIfIndex < pB->dot1agCfmMepIfIndex)
  {
    return -1;
  }

  return 0;
}

static int dot1agCfmMAObjKeyCompareFcn(void *a, void *b, size_t l)
{
  dot1agCfmMAObjKey_t *pA = a;
  dot1agCfmMAObjKey_t *pB = b;

  if (pA->dot1agCfmMdIndex > pB->dot1agCfmMdIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMdIndex < pB->dot1agCfmMdIndex)
  {
    return -1;
  }

  if (pA->dot1agCfmMaIndex > pB->dot1agCfmMaIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMaIndex < pB->dot1agCfmMaIndex)
  {
    return -1;
  }

  return 0;
}

static int dot1agCfmMAObjPrimVIDKeyCompareFcn(void *a, void *b, size_t l)
{
  dot1agCfmMAObjPrimVIDKey_t *pA = a;
  dot1agCfmMAObjPrimVIDKey_t *pB = b;

  if (pA->dot1agCfmMdIndex > pB->dot1agCfmMdIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmMdIndex < pB->dot1agCfmMdIndex)
  {
    return -1;
  }

  if (pA->dot1agCfmMaCompPrimaryVlanId > pB->dot1agCfmMaCompPrimaryVlanId)
  {
    return 1;
  }
  if (pA->dot1agCfmMaCompPrimaryVlanId < pB->dot1agCfmMaCompPrimaryVlanId)
  {
    return -1;
  }

  return 0;
}

static int dot1agCfmStackObjKeyCompareFcn(void *a, void *b, size_t l)
{
  dot1agCfmStackObjKey_t *pA = a;
  dot1agCfmStackObjKey_t *pB = b;

  if (pA->dot1agCfmStackifIndex > pB->dot1agCfmStackifIndex)
  {
    return 1;
  }
  if (pA->dot1agCfmStackifIndex < pB->dot1agCfmStackifIndex)
  {
    return -1;
  }

  if (pA->dot1agCfmStackVlanIdOrNone > pB->dot1agCfmStackVlanIdOrNone)
  {
    return 1;
  }
  if (pA->dot1agCfmStackVlanIdOrNone < pB->dot1agCfmStackVlanIdOrNone)
  {
    return -1;
  }

  if (pA->dot1agCfmStackMdLevel > pB->dot1agCfmStackMdLevel)
  {
    return 1;
  }
  if (pA->dot1agCfmStackMdLevel < pB->dot1agCfmStackMdLevel)
  {
    return -1;
  }

  if (pA->dot1agCfmStackDirection > pB->dot1agCfmStackDirection)
  {
    return 1;
  }
  if (pA->dot1agCfmStackDirection < pB->dot1agCfmStackDirection)
  {
    return -1;
  }

  return 0;
}

static int dot1agCfmSlmObjKeyCompareFcn(void *a, void *b, size_t l)
{
  dot1agCfmSlmObjKey_t *pA = a;
  dot1agCfmSlmObjKey_t *pB = b;

  if (pA->dot1agCfmLmepIdentifier > pB->dot1agCfmLmepIdentifier)
  {
    return 1;
  }
  if (pA->dot1agCfmLmepIdentifier < pB->dot1agCfmLmepIdentifier)
  {
    return -1;
  }

  if (pA->dot1agCfmRmepIdentifier > pB->dot1agCfmRmepIdentifier)
  {
    return 1;
  }
  if (pA->dot1agCfmRmepIdentifier < pB->dot1agCfmRmepIdentifier)
  {
    return -1;
  }

  if (pA->dot1agCfmTestId > pB->dot1agCfmTestId)
  {
    return 1;
  }
  if (pA->dot1agCfmTestId < pB->dot1agCfmTestId)
  {
    return -1;
  }

  return 0;
}

static int dot1agCfmClientMPDBObjKeyCompareFcn(void *a, void *b, size_t l)
{
  dot1agCfmClientMPDBObjKey_t *pA = a;
  dot1agCfmClientMPDBObjKey_t *pB = b;

  if (pA->serverLmepId > pB->serverLmepId)
  {
    return 1;
  }
  if (pA->serverLmepId < pB->serverLmepId)
  {
    return -1;
  }

  if (pA->clientLmepId > pB->clientLmepId)
  {
    return 1;
  }
  if (pA->clientLmepId < pB->clientLmepId)
  {
    return -1;
  }

  return 0;
}

/*********************************************************************
* @purpose  Start Dot1ag task
*
* @param    none
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes    none
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agStartTask(void)
{
  /* Read/write lock to protect component data since processing occurs
   * on multiple threads (DOT1AG thread, UI thread, NIM, dot1q, etc) */
  if (dpaRWlockInit(&(dot1agCFM.dot1agCfgRWLock), NULL) == OFDPA_E_FAIL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to create read/write lock for CFM-OAM", 0);
    return OFDPA_E_FAIL;
  }

  /* Read/write lock to protect component data since processing occurs
   * on multiple threads (DOT1AG thread, UI thread, NIM, dot1q, etc) */
  if (dpaRWlockInit(&(dot1agCFM.dot1agCfgClientMPDBRWLock), NULL) == OFDPA_E_FAIL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to create read/write lock for Client MP DB", 0);
    return OFDPA_E_FAIL;
  }

  dot1agTaskSyncSem = dpaSemCreate("dot1ag task sync semaphore", DPA_SEM_BINARY, 1);

  if (dot1agTaskSyncSem == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS, "Failed to create dot1ag task syncing semaphore", 0);
    return OFDPA_E_FAIL;
  }

  dot1agCFM.dot1agTaskId =  dpaThreadCreate(DOT1AG_TASK_NAME, 200, dot1agTask, NULL);

  if (!dot1agCFM.dot1agTaskId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to create dot1ag task", 0);
    return OFDPA_E_FAIL;
  }

  dpaSemTake(dot1agTaskSyncSem, DPA_SEM_FOREVER);

  return(OFDPA_E_NONE);
}

/*********************************************************************
 * @purpose  Enable dot1ag CFM and pre-create 8 maintenance domains.
 *
 * @returns  OFDPA_E_FAIL
 * @returns  OFDPA_E_NONE
 *
 * @end
 *********************************************************************/
static OFDPA_ERROR_t dot1agOamPreCfg(void)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  uint32_t      mdIndex;
  uint32_t      mdLevel;
  uint8_t       strDomainName[OFDPA_DOT1AG_MD_NAME_LEN];

  dot1agCfmModeSet(OFDPA_TRUE);

  /* pre-create all the domains */
  for (mdLevel = OFDPA_DOT1AG_MD_LEVEL_MIN; mdLevel <= OFDPA_DOT1AG_MD_LEVEL_MAX; mdLevel++)
  {
    /* Check whether the domain entry is full. */
    dot1agMdTableNextIndexGet(&mdIndex);
    if (0 == mdIndex)
    {
      return OFDPA_E_FAIL;
    }

    /* Check whether the special level domain exist. */
    memset(strDomainName, 0,  OFDPA_DOT1AG_MD_NAME_LEN);
    snprintf(strDomainName, OFDPA_DOT1AG_MD_NAME_LEN, "md%d", mdIndex);
    if (dot1agMdIsValid(0, mdLevel, strDomainName) != OFDPA_E_NONE)
    {
      rc = dot1agMdCreate(mdIndex, mdLevel, strDomainName);
      if ((rc != OFDPA_E_NONE) && (rc != OFDPA_E_EXISTS))
      {
        return OFDPA_E_FAIL;
      }
    }
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  System Initialization for dot1ag component
*
* @param    none
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if other failure
*
* @comments  none
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agInit(void)
{
	return 0;
}

uint32_t dot1agMicrosecondsGet(void)
{
  int rc;
  struct timespec tp;

  rc = clock_gettime(CLOCK_MONOTONIC, &tp);
  if (rc < 0)
  {
    return(0);
  }
  return((tp.tv_sec * 1000000) + (tp.tv_nsec / 1000));
}

uint32_t dot1agCentisecondsGet(void)
{
  int             rc;
  struct timespec tp;

  rc = clock_gettime(CLOCK_MONOTONIC, &tp);
  if (rc < 0)
  {
    return(0);
  }
  return((tp.tv_sec * 100) + (tp.tv_nsec / 10000000));
}

void dot1agTimerHandler(timer_t timerCtrlBlk, void* ptrData)
{
  static uint32_t lastTime = 0;
  uint32_t        currTime;
  uint32_t        timeGap;

  currTime = dot1agMicrosecondsGet();
  timeGap  = currTime - lastTime;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_TOO_VERBOSE,
                     "currTime = %u lastTime = %u timeGap = %u\n", currTime, lastTime, timeGap);
  if (!lastTime)
  {
    lastTime = currTime;
    return;
  }

  if (dot1agIssueCmd(dot1agCfmTimerTick, 0, NULL) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to send dot1ag timer tick event", 0);
  }

  lastTime = currTime;
}
