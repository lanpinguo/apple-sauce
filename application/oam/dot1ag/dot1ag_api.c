/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2015
*
**********************************************************************
*
* @filename   dot1ag_api.c
*
* @purpose    Contains definitions to support the management objects
*             access for dot1ag component
*
* @component  DOT1AG/Connectivity & Fault Management
*
* @comments   none
*
* @create     16-Feb-2009
*
* @end
*
**********************************************************************/

#include "ofdpa_porting.h"
#include "ofdb_api.h"
#include "dot1ag.h"
#include "dot1ag_pdu.h"
#include "dot1ag_db.h"
#include "dot1ag_debug.h"
#include "dot1ag_ctrl.h"
#include "dot1ag_pdu.h"
#include "dot1ag_api.h"
#include "dot1ag_outcalls.h"
#include "dot1ag_procs.h"

/*********************************************************************
* @purpose  Takes semaphore to help synchronizing messages in queue
*
* @param    @b{(input)} None
*
* @returns  OFDPA_E_NONE  if synchronization is successful
* @returns  OFDPA_E_FAIL  otherwise
*
* @comments
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agApiQueueSynchronize()
{
  OFDPA_ERROR_t      rc = OFDPA_E_NONE;
  dot1agMsgCmdData_t msg;

  /* Post a message to synchronize the queue */
  memset(&msg, 0, sizeof(msg));
  if (dot1agIssueCmd(dot1agQueueSynchronize, 0, &msg) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "Failed to synchronizing DOT1AG queue.", 0);
    rc = OFDPA_E_FAIL;
  }
  else
  {
    dot1agQueueSyncSemaTake();
  }

  return rc;
}

/*********************************************************************
* @purpose  To find out if more maintenance domains can be created and
*           what is the next valid mdIndex available
*
* @param    mdIndex    @b{(output)} Index value used by SNMP. If no
*                                   more domains can be created then
*                                   this value is 0
*
* @returns  none
*
* @notes    none
*
* @end
**********************************************************************/
void dot1agMdTableNextIndexGet(uint32_t *mdIndex)
{
  uint32_t idx, maxIndex = 0, activeCount = 0;

  *mdIndex = 0;
  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  for (idx = 0; idx < dpaDot1agMaintenanceDomainsMaxGet(); idx++)
  {
    if (DOT1AG_MD_CFG(idx).active == DOT1AG_ROW_ACTIVE ||
        DOT1AG_MD_CFG(idx).active == DOT1AG_ROW_INACTIVE_PENDING)
    {
      if (DOT1AG_MD_CFG(idx).dot1agCfmMdIndex > maxIndex)
      {
        maxIndex = DOT1AG_MD_CFG(idx).dot1agCfmMdIndex;
      }
      activeCount++;
    }
  } /* End of domain loop iterations */

  if (activeCount != dpaDot1agMaintenanceDomainsMaxGet())
  {
    /* New domain index can be 1 greater than already configured
       domain index or 1 if none configured */
    *mdIndex = maxIndex + 1;
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
}

/*********************************************************************
* @purpose  Create a maintenance domain.
*
* @param    mdIndex    @b{(input)} Index value used by SNMP.
* @param    mdLevel    @b{(input)} Domain level to be set
* @param    mdName     @b{(input)} Maintenance Domain name.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_REQUEST_DENIED      Domain exists that has only one of
*                                       the given name or level
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdCreate(uint32_t mdIndex, int mdLevel,
                             uint8_t *mdName)
{
  uint32_t           validmdIndex, idx, freeIdx = 0;
  OFDPA_BOOL         freeIdxFound               = OFDPA_FALSE;
  OFDPA_ERROR_t      rc                         = OFDPA_E_FAIL;
  dot1agMsgCmdData_t msg;

  if ((mdName == 0) ||
      (strlen(mdName) > OFDPA_DOT1AG_MD_NAME_LEN) ||
      (mdLevel < OFDPA_DOT1AG_MD_LEVEL_MIN) ||
      (mdLevel > OFDPA_DOT1AG_MD_LEVEL_MAX) ||
      (mdIndex == 0))
  {
    return OFDPA_E_ERROR;
  }

  /* Obtain a valid index */
  dot1agMdTableNextIndexGet(&validmdIndex);
  if (mdIndex != validmdIndex)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  for (idx = 0; idx < dpaDot1agMaintenanceDomainsMaxGet(); idx++)
  {
    if (DOT1AG_MD_CFG(idx).active == DOT1AG_ROW_ACTIVE ||
        DOT1AG_MD_CFG(idx).active == DOT1AG_ROW_INACTIVE_PENDING)
    {
      /* Invalid index being used. Can be cause because of
         of multiple user access */
      if (strcmp(DOT1AG_MD_CFG(idx).dot1agCfmMdName, mdName) == 0 &&
          DOT1AG_MD_CFG(idx).dot1agCfmMdMdLevel == mdLevel)
      {
        /* Domain already configured */
        dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
        return OFDPA_E_EXISTS;
      }
      else if (strcmp(DOT1AG_MD_CFG(idx).dot1agCfmMdName, mdName) == 0 ||
               DOT1AG_MD_CFG(idx).dot1agCfmMdMdLevel == mdLevel)
      {
        /* Domain already exists that has one of the name or level matches */
        dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
        return OFDPA_E_REQUEST_DENIED;
      }
    }
    else if (freeIdxFound == OFDPA_FALSE)
    {
      freeIdxFound = OFDPA_TRUE;
      freeIdx = idx;
    }
  } /* End of domain loop iterations */

  if (freeIdxFound == OFDPA_TRUE)
  {
    msg.mdIndex = mdIndex;
    if (dot1agIssueCmd(dot1agCfmMDCreate, 0, &msg) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to create MD for mdIndex %d",
                         mdIndex);
    }

    DOT1AG_MD_CFG(freeIdx).dot1agCfmMdIndex  = mdIndex;
    /* Domain name */
    /* Default supported name type */
    DOT1AG_MD_CFG(freeIdx).dot1agCfmMdFormat = DOT1AG_MD_NAME_TYPE_charString;
    strncpy(DOT1AG_MD_CFG(freeIdx).dot1agCfmMdName, mdName,
            OFDPA_DOT1AG_MD_NAME_LEN);
    DOT1AG_MD_CFG(freeIdx).dot1agCfmMdName[OFDPA_DOT1AG_MD_NAME_LEN - 1] = '\0';
    DOT1AG_MD_CFG(freeIdx).dot1agCfmMdMdLevel = mdLevel;
    DOT1AG_MD_CFG(freeIdx).dot1agCfmMdMaNextIndex = 1; /* Initialize ma index */
    DOT1AG_MD_CFG(freeIdx).active = DOT1AG_ROW_ACTIVE;
    rc = OFDPA_E_NONE;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Delete specified maintenance domain
*
* @param    mdIndex    @b{(input)} Index value used by SNMP. Use
*                                  0 for other UI's
* @param    mdLevel    @b{(input)} Domain level
* @param    mdName     @b{(input)} Maintenance Domain name. This has to
*                                  be unique for the CLI/Web to address
*                                  domains using name instead of index as
*                                  in SNMP.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_REQUEST_DENIED      Domain exists that has only one of
*                                  the given name or level
* @returns  OFDPA_E_NOT_FOUND           Domain with a given name and
*                                  level does not exist
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdDelete(uint32_t mdIndex, int mdLevel,
                             uint8_t *mdName)
{
  uint32_t            idx;
  dot1agCfmMdTable_t *mdEntry;
  dot1agMsgCmdData_t  msg;
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;

  if (mdIndex == 0 && (mdName == 0 || mdLevel == -1))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  if (mdIndex == 0)
  {
    for (idx = 0; idx < dpaDot1agMaintenanceDomainsMaxGet(); idx++)
    {
      if (DOT1AG_MD_CFG(idx).active == DOT1AG_ROW_ACTIVE)
      {
        /* Invalid index being used. Can be cause because of
           of multiple user access */
        if (strcmp(DOT1AG_MD_CFG(idx).dot1agCfmMdName, mdName) == 0 &&
            DOT1AG_MD_CFG(idx).dot1agCfmMdMdLevel == mdLevel)
        {
          /* Domain to be delted is found */
          mdIndex = DOT1AG_MD_CFG(idx).dot1agCfmMdIndex;
          break;
        }
        else if (strcmp(DOT1AG_MD_CFG(idx).dot1agCfmMdName, mdName) == 0 ||
                 DOT1AG_MD_CFG(idx).dot1agCfmMdMdLevel == mdLevel)
        {
          /* Domain already exists that has one of the name or level matches */
          dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
          return OFDPA_E_REQUEST_DENIED;
        }
      }
    } /* End of domain loop iterations */
    if (mdIndex == 0)
    {
      /* Domain with a given name and level does not exists */
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_NOT_FOUND;
    }
  }

  if ((mdEntry = dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE)) != 0)
  {
    rc = OFDPA_E_NONE;
    mdEntry->active = DOT1AG_ROW_INACTIVE_PENDING;
    /* Post a message to queue so that md is delete completely and
       activeState is set to DOT1AG_ROW_INACTIVE */
    msg.mdIndex = mdEntry->dot1agCfmMdIndex;
    if (dot1agIssueCmd(dot1agCfmMDDelete, 0, &msg) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to delete MD for mdIndex %d",
                         mdEntry->dot1agCfmMdIndex);
      mdEntry->active = DOT1AG_ROW_ACTIVE;
      rc = OFDPA_E_FAIL;
    }
  } /* End of domain loop iterations */
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Verify if a give maintenance domain exists
*
* @param    mdIndex    @b{(input)} Index value used by SNMP.
* @param    mdLevel    @b{(input)} Domain level. This has to be unique
*                                  for the CLI/Web to address domains using
*                                  name instead of index as in SNMP.
* @param    mdName     @b{(input)} Maintenance Domain name. This has to
*                                  be unique for the CLI/Web to address
*                                  domains using name instead of index as
*                                  in SNMP.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdIsValid(uint32_t mdIndex, int mdLevel,
                              uint8_t *mdName)
{
  uint32_t idx;

  if ((mdIndex == 0) &&
      ((mdName == 0) || (strlen(mdName) > OFDPA_DOT1AG_MD_NAME_LEN)) &&
      ((mdLevel == -1) || (mdLevel < OFDPA_DOT1AG_MD_LEVEL_MIN) || (mdLevel > OFDPA_DOT1AG_MD_LEVEL_MAX)))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  for (idx = 0; idx < dpaDot1agMaintenanceDomainsMaxGet(); idx++)
  {
    if (DOT1AG_MD_CFG(idx).active == DOT1AG_ROW_ACTIVE)
    {
      if (((mdIndex != 0) && (DOT1AG_MD_CFG(idx).dot1agCfmMdIndex == mdIndex)) ||
          (((mdName != 0) && (strcmp(DOT1AG_MD_CFG(idx).dot1agCfmMdName, mdName) == 0)) &&
           ((mdLevel != -1) && (DOT1AG_MD_CFG(idx).dot1agCfmMdMdLevel == mdLevel))))
      {
        dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
        return OFDPA_E_NONE;
      }
    }
  } /* End of domain loop iterations */

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return OFDPA_E_FAIL;
}

/*********************************************************************
* @purpose  Obtain configured maintenance domain name given mdIndex
*
* @param    mdIndex    @b{(input)} Index value used by SNMP.
* @param    mdName     @b{(output)} Maintenance Domain name
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdNameGet(uint32_t mdIndex, uint8_t *mdName)
{
  dot1agCfmMdTable_t *mdEntry;
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;

  if (mdName == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mdEntry = dot1agActiveMdGet(mdIndex, mdName, -1, DOT1AG_ROW_ACTIVE)) != 0)
  {
    rc = OFDPA_E_NONE;
    strncpy(mdName, mdEntry->dot1agCfmMdName, OFDPA_DOT1AG_MD_NAME_LEN);
  } /* End of domain loop iterations */
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain configured maintenance domain level given mdIndex
*
* @param    mdIndex    @b{(input)} Index value used by SNMP.
* @param    mdLevel    @b{(output)} Maintenance Domain level
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdLevelGet(uint32_t mdIndex, int *mdLevel)
{
  dot1agCfmMdTable_t *mdEntry;
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;

  if (mdIndex == 0 || mdLevel == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mdEntry = dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE))
      != 0)
  {
    rc = OFDPA_E_NONE;
    *mdLevel = mdEntry->dot1agCfmMdMdLevel;
  } /* End of domain loop iterations */
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain configured maintenance domain mdIndex given mdLevel
*           or mdName
*
* @param    mdName     @b{(input)} Maintenance Domain name. Can be
*                                  0PTR, if so use mdLevel as input
* @param    mdLevel    @b{(input)} Maintenance Domain level. Can be -1
*                                  of so use mdName as key
* @param    mdIndex    @b{(output)} Index value used by SNMP.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdMdIndexGet(uint8_t *mdName, int mdLevel,
                                 uint32_t *mdIndex)
{
  dot1agCfmMdTable_t *mdEntry;
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;

  if (mdIndex == 0 || (mdLevel == -1 && mdName == 0))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mdEntry = dot1agActiveMdGet(0, mdName, mdLevel, DOT1AG_ROW_ACTIVE))
      != 0)
  {
    rc = OFDPA_E_NONE;
    *mdIndex = mdEntry->dot1agCfmMdIndex;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain number of MA's configured in a given maintenance domain
*           or mdName
*
* @param    mdIndex    @b{(input)}  Index for MD identification
* @param    mdCount    @b{(output)} Number of maintenance associations
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdMaCountGet(uint32_t mdIndex, uint32_t *maCount)
{
  uint32_t prevMdIndex, maIndex;

  if (mdIndex == 0 || (maCount == 0))
  {
    return OFDPA_E_ERROR;
  }

  *maCount = maIndex = 0;
  prevMdIndex = mdIndex;

  while (dot1agMaNextGet(&mdIndex, &maIndex) == OFDPA_E_NONE)
  {
    if (mdIndex != prevMdIndex)
    {
      break;
    }

    *maCount = *maCount + 1;
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Obtain valid maIndex to be used to create a new
*           maintenance association for this domain mdIndex
*
* @param    mdIndex      @b{(input)}  Index value used by SNMP.
* @param    maNextIndex  @b{(output)} New maintenance Assoc index.
*                                     0 if no new MAs can be
*                                     created
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdMaNextIndexGet(uint32_t mdIndex, uint32_t *maNextIndex)
{
  dot1agCfmMdTable_t *mdEntry;
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;

  if (maNextIndex == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mdEntry = dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE))
      != 0)
  {
    rc = OFDPA_E_NONE;
    if (DOT1AG_MA_NET_TABLE(dot1agCFM.dot1agMATableIndex).active == DOT1AG_ROW_INACTIVE)
    {
      *maNextIndex = mdEntry->dot1agCfmMdMaNextIndex;
    }
    else
    {
      *maNextIndex = 0;
      rc = OFDPA_E_FULL;
    }
  } /* End of domain loop iterations */
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain a next valid mdIndex value given a maintenance domain
*
* @param    nextMdIndex    @b{(input/output)} domain index value. Use 0
*                                         to obatain the first valid
*                                         value.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdIndexNextGet(uint32_t *nextMdIndex)
{
  uint32_t idx, nextKey;

  if (nextMdIndex == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  nextKey = *nextMdIndex;
  for (idx = 0; idx < dpaDot1agMaintenanceDomainsMaxGet(); idx++)
  {
    if (DOT1AG_MD_CFG(idx).active == DOT1AG_ROW_ACTIVE)
    {
      if (((DOT1AG_MD_CFG(idx).dot1agCfmMdIndex) > (*nextMdIndex)) &&
          ((nextKey == (*nextMdIndex)) ||
           (((DOT1AG_MD_CFG(idx).dot1agCfmMdIndex) < nextKey))))
      {
        nextKey = (DOT1AG_MD_CFG(idx).dot1agCfmMdIndex);
      }
    }
  } /* End of domain loop iterations */

  if (nextKey != (*nextMdIndex))
  {
    *nextMdIndex = nextKey;
    dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
    return OFDPA_E_NONE;
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return OFDPA_E_FAIL;
}

/*********************************************************************
* @purpose  Create a maintenance association
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    maid       @b{(input)} Maintenance assoc identifier
* @param    primVID    @b{(input)} VLAN ID associated with the MA.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_FULL
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMaCreate(uint32_t mdIndex, uint32_t maIndex,
                             uint8_t *maid, uint32_t primVID)
{
  uint32_t                 prevFreeIdx;
  OFDPA_ERROR_t            rc = OFDPA_E_ERROR;
  dot1agCfmMaNetTable_t   *maEntry;
  dot1agCfmMAObj_t         maObjEntry;
  dot1agCfmMAObjPrimVID_t  maObjPrimVID;
  dot1agCfmMAObjPrimVID_t *pmaObjPrimVID = 0;
  dot1agCfmMdTable_t      *mdEntry       = 0;
  dot1agMsgCmdData_t       msg;

  if (mdIndex == 0 || maIndex == 0 || maid == NULL)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  if ((mdEntry = dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE)) != 0)
  {
    /* Verify if passed maIndex is a valid value */
    if (mdEntry->dot1agCfmMdMaNextIndex != maIndex)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_ERROR;
    }

    if (DOT1AG_MA_NET_TABLE(dot1agCFM.dot1agMATableIndex).active != DOT1AG_ROW_INACTIVE)
    {
      /* Table management issue */
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_FULL;
    }

    maEntry = &DOT1AG_MA_NET_TABLE(dot1agCFM.dot1agMATableIndex);

    maEntry->mdData           = mdEntry;
    maEntry->dot1agCfmMaIndex = mdEntry->dot1agCfmMdMaNextIndex;

    memcpy(maEntry->dot1agCfmMaid, maid, sizeof(maEntry->dot1agCfmMaid));

    maEntry->dot1agCfmMaNetCcmInterval = FD_DOT1AG_CCM_INTERVAL;
    maEntry->pktChanged = OFDPA_TRUE;

    maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId = primVID;
    maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompNumberOfVids  = 1; /* Always 1 */
    maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompMhfCreation   = DOT1AG_defMHFnone;
    maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompIdPermission  = DOT1AG_sendIdNone;

    /* Insert entry into AVL tree */
    if (primVID != 0)
    {
      /* Primary VID view */
      memset(&maObjPrimVID, 0x0, sizeof(maObjPrimVID));
      maObjPrimVID.key.dot1agCfmMdIndex = mdEntry->dot1agCfmMdIndex;
      maObjPrimVID.key.dot1agCfmMaCompPrimaryVlanId = primVID;
      maObjPrimVID.maData = maEntry;
      rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_MA_OBJ_PRIM_VID_TREE, &dot1agCFM.dot1agMAPrimVIDTree, &maObjPrimVID);
    }
    else
    {
      rc = OFDPA_E_NONE;
    }

    if (rc == OFDPA_E_NONE)
    {
      /* MIB view */
      memset(&maObjEntry, 0x0, sizeof(maObjEntry));
      maObjEntry.key.dot1agCfmMdIndex = mdEntry->dot1agCfmMdIndex;
      maObjEntry.key.dot1agCfmMaIndex = maEntry->dot1agCfmMaIndex;
      maObjEntry.maData = maEntry;
      rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_MA_OBJ_TREE, &dot1agCFM.dot1agMATree, &maObjEntry);
      if (rc != OFDPA_E_NONE)
      {
        /* remove earlier added primVID view node */
        pmaObjPrimVID = dot1agActivePrimVIDMAGet(mdEntry->dot1agCfmMdIndex,
                                                 primVID, AVL_EXACT);
        if (pmaObjPrimVID != 0)
        {
          dot1agDBTreeEntryDelete(DOT1AG_DB_ID_MA_OBJ_PRIM_VID_TREE,
                                  &dot1agCFM.dot1agMAPrimVIDTree, pmaObjPrimVID);
        }
      }
      else
      {
        mdEntry->dot1agCfmMdMaNextIndex++; /* Auto increment the nextValidIndex maID */
        maEntry->active = DOT1AG_ROW_ACTIVE;

        /* point TableIndex to next free index */
        prevFreeIdx = dot1agCFM.dot1agMATableIndex;

        while (DOT1AG_MA_NET_TABLE(dot1agCFM.dot1agMATableIndex).active != DOT1AG_ROW_INACTIVE)
        {
          dot1agCFM.dot1agMATableIndex++;

          if (dot1agCFM.dot1agMATableIndex == dpaOamMegTableEntryCountGet())
          {
            dot1agCFM.dot1agMATableIndex = 0;
          }

          if (prevFreeIdx == dot1agCFM.dot1agMATableIndex)
          {
            break;
          }
        }/* End of MA table loop */

        msg.mdIndex = mdEntry->dot1agCfmMdIndex;
        msg.maIndex = maEntry->dot1agCfmMaIndex;
        if (dot1agIssueCmd(dot1agCfmMACreate, 0, &msg) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to send create MA message. (mdIndex = %d, maIndex = %d)",
                             mdEntry->dot1agCfmMdIndex, maEntry->dot1agCfmMaIndex);
          rc = OFDPA_E_FAIL;
        }
      } /* End of MIB view tree node add success check */
    } /* End of primVID tree node add success check */
  } /* End of domain get */

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Delete a maintenance association given (mdIndex)
*           and (maIndex)
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_NOT_FOUND
* @returns  OFDPA_E_REQUEST_DENIED
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMaDelete(uint32_t mdIndex, uint32_t maIndex)
{
  dot1agCfmMAObj_t      *maObjEntry;
  dot1agCfmMAObjKey_t    maObjEntryKey;
  dot1agCfmMaNetTable_t *maEntry    = 0;
  dot1agCfmMdTable_t    *mdEntry    = 0;
  dot1agMsgCmdData_t     msg;
  OFDPA_BOOL             foundEntry = OFDPA_FALSE;

  if (mdIndex == 0 || (maIndex == 0))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mdEntry = dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE)) != 0)
  {
    /* User entered a valid maID. Use MIB view */
    maObjEntryKey.dot1agCfmMdIndex = mdEntry->dot1agCfmMdIndex;
    maObjEntryKey.dot1agCfmMaIndex = 0;

    while ((maObjEntry = avlSearch(&dot1agCFM.dot1agMATree, &maObjEntryKey, AVL_NEXT)) != NULL)
    {
      if (maObjEntry->key.dot1agCfmMdIndex != mdIndex)
      {
        break;
      }

      maEntry = maObjEntry->maData;
      maObjEntryKey.dot1agCfmMaIndex = maObjEntry->key.dot1agCfmMaIndex;

      if (maIndex == maEntry->dot1agCfmMaIndex)
      {
        foundEntry = OFDPA_TRUE;
        break;
      }
    }

    if (foundEntry == OFDPA_FALSE)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_NOT_FOUND;
    }

    if (maEntry != 0 && maEntry->active == DOT1AG_ROW_ACTIVE)
    {
      maEntry->active = DOT1AG_ROW_INACTIVE_PENDING;
      /* Post a message to queue so that ma is delete completely and
         activeState is set to DOT1AG_ROW_INACTIVE */
      msg.mdIndex = mdEntry->dot1agCfmMdIndex;
      msg.maIndex = maEntry->dot1agCfmMaIndex;
      if (dot1agIssueCmd(dot1agCfmMADelete, 0, &msg) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to delete MA. mdIndex %d maIndex %d",
                           mdEntry->dot1agCfmMdIndex, maEntry->dot1agCfmMaIndex);
        maEntry->active = DOT1AG_ROW_ACTIVE; /* Undo delete */
      }
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_NONE;
    }
  } /* End of domain get */

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return OFDPA_E_FAIL;
}

/*********************************************************************
* @purpose  Obtain next valid maintenance association given
*           (mdIndex)
*
* @param    mdIndex    @b{(input/output)} Index used for MD indentification
*                                         used by SNMP.
* @param    maIndex    @b{(input/output)} Index for MA identification used by
*                                         SNMP.
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use 0 as input to obtain first valid entry
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMaNextGet(uint32_t *mdIndex, uint32_t *maIndex)
{
  OFDPA_ERROR_t     rc = OFDPA_E_FAIL;
  dot1agCfmMAObj_t *maObj;

  /* Validate input */
  if (mdIndex == 0 || maIndex == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((maObj = dot1agActiveMAGet(*mdIndex, *maIndex,
                                 AVL_NEXT)) != 0)
  {
    rc = OFDPA_E_NONE;
    if (NULL != maObj->maData &&
        NULL != maObj->maData->mdData)
    {
      *mdIndex = maObj->maData->mdData->dot1agCfmMdIndex;
      *maIndex = maObj->maData->dot1agCfmMaIndex;
    }
    else
    {
      rc = OFDPA_E_ERROR;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain maintenance association name given mdIndex and
*           maIndex
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    maName     @b{(output)} Maintenance association name
* @param    maNameFormat @b{(output)} Maintenance association name format
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaidGet(uint32_t mdIndex, uint32_t maIndex, uint8_t *maid)
{
  OFDPA_ERROR_t     rc = OFDPA_E_FAIL;
  dot1agCfmMAObj_t *maObj;

  /* Validate input */
  if (maid == NULL)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((maObj = dot1agActiveMAGet(mdIndex, maIndex, AVL_EXACT)) != 0)
  {
    rc = OFDPA_E_NONE;
    memcpy(maid, maObj->maData->dot1agCfmMaid, sizeof(maObj->maData->dot1agCfmMaid));
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain primary vlan associated with an MA
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    primVID    @b{(output)} Primary VLAN associated with the MA
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaPrimVLANGet(uint32_t mdIndex, uint32_t maIndex,
                                  uint32_t *primVID)
{
  OFDPA_ERROR_t     rc = OFDPA_E_FAIL;
  dot1agCfmMAObj_t *maObj;

  /* Validate input */
  if (primVID == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((maObj = dot1agActiveMAGet(mdIndex, maIndex,
                                 AVL_EXACT)) != 0)
  {
    rc = OFDPA_E_NONE;
    *primVID = maObj->maData->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain maIndex given primary vlan associated with an MA
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    primVID    @b{(input)} Primary VLAN associated with the MA
* @param    maIndex    @b{(output)} Index for MA identification used by
*                                  SNMP.
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaIndexGet(uint32_t mdIndex,  uint32_t primVID,
                               uint32_t *maIndex)
{
  OFDPA_ERROR_t            rc = OFDPA_E_FAIL;
  dot1agCfmMAObjPrimVID_t *maObj;

  /* Validate input */
  if (maIndex == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((maObj = dot1agActivePrimVIDMAGet(mdIndex, primVID,
                                        AVL_EXACT)) != 0)
  {
    rc = OFDPA_E_NONE;
    *maIndex = maObj->maData->dot1agCfmMaIndex;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain CCM interval associated with this MA
*
* @param    mdIndex     @b{(input)} Index used for MD indentification
* @param    maIndex     @b{(input)} Index for MA identification
* @param    ccmInterval @b{(output)} Continuity Check Interval
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaCCMIntervalGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t *ccmInterval)
{
  OFDPA_ERROR_t     rc = OFDPA_E_FAIL;
  dot1agCfmMAObj_t *maObj;

  /* Validate input */
  if (ccmInterval == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((maObj = dot1agActiveMAGet(mdIndex, maIndex,
                                 AVL_EXACT)) != 0)
  {
    rc = OFDPA_E_NONE;
    *ccmInterval = maObj->maData->dot1agCfmMaNetCcmInterval;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set CCM interval associated with this MA
*
* @param    mdIndex     @b{(input)} Index used for MD indentification
* @param    maIndex     @b{(input)} Index for MA identification
* @param    ccmInterval @b{(input)} Continuity Check Interval
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaCCMIntervalSet(uint32_t mdIndex, uint32_t maIndex,
                                     Dot1agCfmCcmInterval_t ccmInterval)
{
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;
  dot1agCfmMAObj_t   *maObj;
  dot1agMsgCmdData_t  msg;
  int                 timeRemaining;
  int                 timerInterval;

  /* Validate CCM interval */
  if (ccmInterval < OFDPA_DOT1AG_CCM_INTERVAL_MIN ||
      ccmInterval > OFDPA_DOT1AG_CCM_INTERVAL_MAX)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  maObj = dot1agActiveMAGet(mdIndex, maIndex, AVL_EXACT);
  if (maObj != 0)
  {
    rc = OFDPA_E_NONE;
    if (ccmInterval != maObj->maData->dot1agCfmMaNetCcmInterval)
    {
      maObj->maData->dot1agCfmMaNetCcmInterval = ccmInterval;

      msg.mdIndex  = mdIndex;
      msg.maIndex  = maIndex;
      maObj->maData->pktChanged = OFDPA_TRUE;
      if (dot1agIssueCmd(dot1agCfmCCMIntervalSet, 0, &msg) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to reinstall MEP/RMEP hardware entries for changed CCM Interval %d mdIndex %d maIndex %d",
                           ccmInterval, mdIndex, maIndex);
        rc = OFDPA_E_FAIL;
      }
    }
  }

  /* If the free running timer interval is greater than the new CCM interval, decrease the
   * timer interval to match the CCM interval. */
  if ((DOT1AG_CCM_intervalInvalid != ccmInterval) &&
      (OFDPA_E_NONE == dpaTimerGet(dot1agCFM.timerCB, &timeRemaining, &timerInterval)))
  {
    if (timerInterval > CCMtime(ccmInterval, 0))
    {
      if (OFDPA_E_NONE != dpaTimerSet(dot1agCFM.timerCB, timeRemaining, CCMtime(ccmInterval, 0)))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to modify Dot1ag Timer", 0);
      }
    }
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Verify if an entry with given values exists in MEP Table
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is configured
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes This functions ignores interface number
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepIsValid(uint32_t mdIndex, uint32_t maIndex,
                               uint32_t mepId, uint32_t intIfNum)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  /* Ignore interface number, as it's not a key in SNMP MIB and is used
   *  as a key in application. */
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, 0,
                                   AVL_NEXT)) != 0)
  {
    if ((NULL != mepObj->mepData) &&
        (NULL != mepObj->mepData->maData) &&
        (NULL != mepObj->mepData->maData->mdData))
    {
      if ((mdIndex == mepObj->mepData->maData->mdData->dot1agCfmMdIndex) &&
          (maIndex == mepObj->mepData->maData->dot1agCfmMaIndex) &&
          (mepId == mepObj->mepData->dot1agCfmMepIdentifier) &&
          (intIfNum == mepObj->mepData->dot1agCfmMepIfIndex))
      {
        rc = OFDPA_E_NONE;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain next valid maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes  Use 0 values to obtain the first valid entry
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepNextGet(uint32_t *mdIndex, uint32_t *maIndex,
                               uint32_t *mepId, uint32_t *intIfNum)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (intIfNum == 0 || mdIndex == 0 ||
      maIndex == 0 || mepId == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(*mdIndex, *maIndex, *mepId, *intIfNum,
                                   AVL_NEXT)) != 0)
  {
    if ((NULL != mepObj->mepData) &&
        (NULL != mepObj->mepData->maData) &&
        (NULL != mepObj->mepData->maData->mdData) &&
        (*intIfNum == mepObj->mepData->dot1agCfmMepIfIndex))
    {
      rc = OFDPA_E_NONE;
      *mdIndex  = mepObj->mepData->maData->mdData->dot1agCfmMdIndex;
      *maIndex  = mepObj->mepData->maData->dot1agCfmMaIndex;
      *mepId    = mepObj->mepData->dot1agCfmMepIdentifier;
      *intIfNum = mepObj->mepData->dot1agCfmMepIfIndex;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain direction of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP. Use 0 for
*                                  other UI's
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    direction  @b{(output)} Direction of MEP (Up or Down)
*
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepDirectionGet(uint32_t mdIndex, uint32_t maIndex,
                                    uint32_t mepId, uint32_t intIfNum,
                                    uint32_t *direction)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (direction == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_NEXT)) != 0)
  {
    if ((mepObj->mepData != NULL) &&
        (intIfNum == mepObj->mepData->dot1agCfmMepIfIndex))
    {
      rc = OFDPA_E_NONE;
      *direction  = mepObj->mepData->dot1agCfmMepDirection;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain active mode of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)}  Active mode of MEP (OFDPA_TRUE or
*                                    OFDPA_FALSE)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepActiveModeGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum,
                                     OFDPA_BOOL *mode)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (mode == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      *mode  = mepObj->mepData->dot1agCfmMepActive;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set active mode of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(input)} Active mode of MEP (OFDPA_TRUE or OFDPA_FALSE)
*
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepActiveModeSet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum,
                                     OFDPA_BOOL mode)
{
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;
  dot1agMsgCmdData_t  msg;

  /* Validate input */
  if (mode != OFDPA_TRUE && mode != OFDPA_FALSE)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
   
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      if (mode != mepObj->mepData->dot1agCfmMepActive)
      {
        mepObj->mepData->dot1agCfmMepActive = mode;
        /* Post a message to queue so that mep create event is passed to state machines */
        msg.mdIndex  = mdIndex;
        msg.maIndex  = maIndex;
        msg.mepIndex = mepId;
        msg.intIfNum = intIfNum;
        msg.uintVal  = mode;
        if (dot1agIssueCmd(dot1agCfmMEPActiveMode, 0, &msg) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to set MEP active mode. mdIndex %d maIndex %d mepIndex %d intf %d mode %d",
                             mdIndex, maIndex, mepId, intIfNum, mode);
          rc = OFDPA_E_FAIL;
        }
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain CCI enabled flag of maintenance end point on an
*           interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)}  CCI enabled flag of MEP (OFDPA_TRUE or
*                                    OFDPA_FALSE)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepCCIEnabledGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum,
                                     OFDPA_BOOL *mode)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (mode == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      *mode  = mepObj->mepData->dot1agCfmMepCCIVar.cciEnabled;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set CCI enabled flag of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)}  CCI enabled flag of MEP (OFDPA_TRUE or
*                                    OFDPA_FALSE)
*
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepCCIEnabledSet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum,
                                     OFDPA_BOOL mode)
{
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;
  dot1agMsgCmdData_t  msg;

  /* Validate input */
  if (mode != OFDPA_TRUE && mode != OFDPA_FALSE)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      if (mode != mepObj->mepData->dot1agCfmMepCCIVar.cciEnabled)
      {
        mepObj->mepData->dot1agCfmMepCCIVar.cciEnabled = mode;
        /* Post a message to queue so that mep create event is passed to state machines */
        msg.mdIndex  = mdIndex;
        msg.maIndex  = maIndex;
        msg.mepIndex = mepId;
        msg.intIfNum = intIfNum;
        msg.uintVal  = mode;
        if (dot1agIssueCmd(dot1agCfmMEPCciEnabled, 0, &msg) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to set CciEnabled mode. mdIndex %d maIndex %d mepIndex %d intf %d mode %d",
                             mdIndex, maIndex, mepId, intIfNum, mode);
          rc = OFDPA_E_FAIL;
        }
  
        /* If MEP is active and disabled (MEPActive = OFDPA_TRUE,
         * CCIenabled = OFDPA_FALSE) disable CCM transmission.
         */
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(msg.mepIndex))
        {
          if (mepObj->mepData->dot1agCfmMepActive)
          {
            if (dot1agIssueCmd(dot1agCfmCCMIntervalSet, 0, &msg) != OFDPA_E_NONE)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                                 "Failed to reinstall MEP/RMEP hardware entries for"
                                 " changed CCM Interval %d mdIndex %d maIndex %d",
                                 mepObj->mepData->maData->dot1agCfmMaNetCcmInterval,
                                 mdIndex, maIndex);
              rc = OFDPA_E_FAIL;
            }
          }
        }
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Clear RMEP entries learned on a maintenance end point
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agRmepClear(uint32_t mdIndex, uint32_t maIndex,
                              uint32_t mepId)
{
  OFDPA_ERROR_t       rc       = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;
  dot1agMsgCmdData_t  msg;
  uint32_t            intIfNum = 0;

  /* Validate input */
  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_NEXT);
  if ((mepObj == 0) ||
      (mepObj->mepData == NULL) ||
      (mepObj->mepData->maData == NULL) ||
      (mepObj->mepData->maData->mdData == NULL) ||
      (mepObj != 0 && (mdIndex != mepObj->mepData->maData->mdData->dot1agCfmMdIndex ||
                       maIndex != mepObj->mepData->maData->dot1agCfmMaIndex ||
                       mepId   != mepObj->mepData->dot1agCfmMepIdentifier)))
  {
    dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
    return rc;
  }

  intIfNum = mepObj->key.dot1agCfmMepIfIndex;

  rc = OFDPA_E_NONE;

  /* Post a message to queue so that rmep entries learned on a given mep would be cleared */
  msg.mdIndex  = mdIndex;
  msg.maIndex  = maIndex;
  msg.mepIndex = mepId;
  msg.intIfNum = intIfNum;
  if (dot1agIssueCmd(dot1agCfmRMEPDelete, 0, &msg) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to clear RMEP entries. mdIndex %d maIndex %d mepIndex %d",
                       mdIndex, maIndex, mepId);
    rc = OFDPA_E_FAIL;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain maintenance end point on an interface parameter
*           defined by section 17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    paramCode  @b{(input)} MEP object parameter defined by
*                                  Dot1agCfmMepEntry_t
* @param    val       @b{(output)} Param value
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepParamGet(uint32_t mdIndex, uint32_t maIndex,
                                uint32_t mepId, uint32_t intIfNum,
                                uint32_t paramCode, void *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if (intIfNum == 0)
  {
    mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_NEXT);
    if ((mepObj == 0) ||
        (mepObj->mepData == NULL) ||
        (mepObj->mepData->maData == NULL) ||
        (mepObj->mepData->maData->mdData == NULL) ||
        (mepObj != 0 && (mdIndex != mepObj->mepData->maData->mdData->dot1agCfmMdIndex ||
                         maIndex != mepObj->mepData->maData->dot1agCfmMaIndex ||
                         mepId   != mepObj->mepData->dot1agCfmMepIdentifier)))
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return rc;
    }
    intIfNum = mepObj->key.dot1agCfmMepIfIndex;
  }
  else
  {
    mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                AVL_EXACT);
  }

  if (mepObj  != 0)
  {
    rc = OFDPA_E_NONE;
    switch (paramCode)
    {
      case MEP_PARAM_dot1agCfmMepIfIndex:
        *((uint32_t *)val)  = mepObj->key.dot1agCfmMepIfIndex;
        break;
      case MEP_PARAM_dot1agCfmMepDirection:
        *((uint32_t *)val)  = mepObj->mepData->dot1agCfmMepDirection;
        break;
      case MEP_PARAM_dot1agCfmMepPrimaryVid:
        *((uint32_t *)val)  = mepObj->mepData->dot1agCfmMepPrimaryVid;
        break;
      case MEP_PARAM_dot1agCfmMepActive:
        *((uint32_t *)val)  = mepObj->mepData->dot1agCfmMepActive;
        break;
      case MEP_PARAM_dot1agCfmMepFngState:
        *((uint32_t *)val)  = mepObj->mepData->dot1agCfmMepSMFngState;
        break;
      case MEP_PARAM_dot1agCfmMepCciEnabled:
        *((uint32_t *)val)  = mepObj->mepData->dot1agCfmMepCCIVar.cciEnabled;
        break;
      case MEP_PARAM_dot1agCfmMepCcmLtmPriority:
        *((uint32_t *)val)  = mepObj->mepData->dot1agCfmMepCcmLtmPriority;
        break;
      case MEP_PARAM_dot1agCfmMepMacAddress:
        memcpy(val, mepObj->mepData->dot1agCfmMepMacAddress, sizeof(mepObj->mepData->dot1agCfmMepMacAddress));
        break;
      case MEP_PARAM_dot1agCfmMepLowPrDef:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmGblMepVar.lowestAlarmPri;
        break;
      case MEP_PARAM_dot1agCfmMepFngAlarmTime:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepFngAlarmTime;
        break;
      case MEP_PARAM_dot1agCfmMepFngResetTime:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepFngResetTime;
        break;
      case MEP_PARAM_dot1agCfmMepHighestPrDefect:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepFngVar.highestDefect;
        break;
      case MEP_PARAM_dot1agCfmMepDefects:
        *((uint32_t *)val) = 0;
        if (mepObj->mepData->dot1agCfmMepFngVar.someRDIDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= DOT1AG_MEP_DEFECT_BIT_bDefRDICCM;
        }

        if (mepObj->mepData->dot1agCfmMepFngVar.someMacStatusDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= DOT1AG_MEP_DEFECT_BIT_bDefMACstatus;
        }

        if (mepObj->mepData->dot1agCfmMepFngVar.someRMEPCCMDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= DOT1AG_MEP_DEFECT_BIT_bDefRemoteCCM;
        }

        if (mepObj->mepData->dot1agCfmRmepErrVar.errorCcmDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= DOT1AG_MEP_DEFECT_BIT_bDefErrorCCM;
        }

        if (mepObj->mepData->dot1agCfmMepXConVar.xconCcmDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= DOT1AG_MEP_DEFECT_BIT_bDefXconCCM;
        }
        break;
      case MEP_PARAM_dot1agCfmMepErrorCcmLastFailure:
        memcpy(val,
               mepObj->mepData->dot1agCfmRmepErrVar.errorCcmLastFailure,
               OFDPA_DOT1AG_CCM_FRAME_LEN);
        break;
      case MEP_PARAM_dot1agCfmMepXconCcmLastFailure:
        memcpy(val,
               mepObj->mepData->dot1agCfmMepXConVar.xconCcmLastFailure,
               OFDPA_DOT1AG_CCM_FRAME_LEN);
        break;
      case MEP_PARAM_dot1agCfmMepCcmSequenceErrors:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepCcmSequenceErrors;
        }
        break;
      case MEP_PARAM_dot1agCfmMepCciSentCcms:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepCCIVar.cciSentCCMs;
        }
        break;
      case MEP_PARAM_dot1agCfmMepNextLbmTransId:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepLbiVar.nextLbmTransId;
        break;
      case MEP_PARAM_dot1agCfmMepLbrIn:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepLbrIn;
        break;
      case MEP_PARAM_dot1agCfmMepLbrInOutOfOrder:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepLbrInOutOfOrder;
        break;
      case MEP_PARAM_dot1agCfmMepLbrBadMsdu:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepLbrBadMsdu;
        break;
      case MEP_PARAM_dot1agCfmMepLtmNextSeqNumber:
        *((uint32_t *)val) = dot1agCFM.dot1agCfmLtmrVar.nextLtmTransId;
        break;
      case MEP_PARAM_dot1agCfmMepUnexpLtrIn:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepUnexpLtrIn;
        break;
      case MEP_PARAM_dot1agCfmMepLbrOut:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepLbrOut;
        }
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmStatus:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepTransmitLbmStatus;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmDestMacAddress:
        strncpy((uint8_t *)val, mepObj->mepData->dot1agCfmMepTransmitLbmDestMacAddress,
                OFDPA_MAC_ADDR_LEN);
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmDestMepId:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepTransmitLbmDestMepId;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmDestIsMepId:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepTransmitLbmDestIsMepId;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmMessages:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepLbiVar.lbmsToSend;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmDataTlv:
        rc = OFDPA_NOT_IMPLEMENTED_YET;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmVlanPriority:
        rc = OFDPA_NOT_IMPLEMENTED_YET;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmVlanDropEnable:
        rc = OFDPA_NOT_IMPLEMENTED_YET;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmResultOK:
        *((OFDPA_BOOL *)val) = mepObj->mepData->dot1agCfmMepTransmitLbmResultOk;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLbmSeqNumber:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepLbiVar.nextLbmTransId;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLtmStatus:
        if (dot1agCFM.dot1agCfmLtmrVar.ltitTimer != 0)
        {
          *((uint32_t *)val) = OFDPA_FALSE;
        }
        else
        {
          *((uint32_t *)val) = OFDPA_TRUE;
        }
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLtmFlags:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepTransmitLtmFlags;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLtmTargetMacAddress:
        strncpy((uint8_t *)val,
                mepObj->mepData->dot1agCfmMepTransmitLtmTargetMacAddress,
                OFDPA_MAC_ADDR_LEN);
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLtmTargetMepId:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepTransmitLtmTargetMepId;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLtmTargetIsMepId:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepTransmitLtmTargetIsMepId;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLtmTtl:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepTransmitLtmTtl;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLtmResult:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepTransmitLtmResult;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLtmSeqNumber:
        *((uint32_t *)val) = mepObj->mepData->dot1agCfmMepTransmitLtmSeqNumber;
        break;
      case MEP_PARAM_dot1agCfmMepTransmitLtmEgressIdentifier:
        /* Get base MAC address (could be BIA or LAA) */
        memset(val, 0x00, DOT1AG_EGRESS_ID_LEN);
        memcpy((((uint8_t *)val) + 2), mepObj->mepData->dot1agCfmMepMacAddress, sizeof(mepObj->mepData->dot1agCfmMepMacAddress));
        break;
      case MEP_PARAM_y1731CfmMepDefects:
        *((uint32_t *)val) = 0;
        if (mepObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_RDI].y1731DefCcmDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= Y1731_MEP_DEFECT_BIT_bDefRDICCM;
        }

        if (mepObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_LOC].y1731DefCcmDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= Y1731_MEP_DEFECT_BIT_bDefLOCCCM;
        }

        if (mepObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNPTR].y1731DefCcmDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= Y1731_MEP_DEFECT_BIT_bDefUNPtrCCM;
        }

        if (mepObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNP].y1731DefCcmDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= Y1731_MEP_DEFECT_BIT_bDefUNPCCM;
        }

        if (mepObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNM].y1731DefCcmDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= Y1731_MEP_DEFECT_BIT_bDefUNMCCM;
        }

        if (mepObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_MMG].y1731DefCcmDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= Y1731_MEP_DEFECT_BIT_bDefMMGCCM;
        }

        if (mepObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNL].y1731DefCcmDefect == OFDPA_TRUE)
        {
          *((uint32_t *)val) |= Y1731_MEP_DEFECT_BIT_bDefUNLCCM;
        }
        break;

      case MEP_PARAM_y1731CfmMepAisTx:
        *((uint32_t *)val) = mepObj->mepData->y1731CfmMepAisSendState;
        break;
      case MEP_PARAM_y1731CfmMepAisRx:
        *((uint32_t *)val) = mepObj->mepData->y1731CfmMepAisRcvState;
        break;
      case MEP_PARAM_y1731CfmMepLckTx:
        *((uint32_t *)val) = mepObj->mepData->y1731CfmMepLckSendState;
        break;
      case MEP_PARAM_y1731CfmMepLckRx:
        *((uint32_t *)val) = mepObj->mepData->y1731CfmMepLckRcvState;
        break;
      case MEP_PARAM_y1731CfmMepCsfTx:
        *((uint32_t *)val) = mepObj->mepData->y1731CfmMepCsfSendState;
        break;
      case MEP_PARAM_y1731CfmMepCsfRx:
        *((uint32_t *)val) = mepObj->mepData->y1731CfmMepCsfRcvState;
        break;

      default:
        rc = OFDPA_E_FAIL;
        break;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain priority to be used in LTMs, CCMs sent by the
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val       @b{(output)} Priority value for CCMs and LTMs
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepCcmLtmPriorityGet(uint32_t mdIndex, uint32_t maIndex,
                                         uint32_t mepId, uint32_t intIfNum,
                                         uint32_t *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    { 
      rc = OFDPA_E_NONE;
      *val  = mepObj->mepData->dot1agCfmMepCcmLtmPriority;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set priority to be used in LTMs, CCMs sent by the
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Priority value for CCMs and LTMs
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepCcmLtmPrioritySet(uint32_t mdIndex, uint32_t maIndex,
                                         uint32_t mepId, uint32_t intIfNum,
                                         uint32_t val)
{
  OFDPA_ERROR_t   rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val > OFDPA_DOT1P_MAX_PRIORITY)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    { 
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepCcmLtmPriority = val;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain MAC Address of the
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    macAddr    @b{(output)} MAC Addr of the MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepMacAddressGet(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t intIfNum,
                                        uint8_t *macAddr)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (macAddr == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE; 
      memcpy(macAddr, mepObj->mepData->dot1agCfmMepMacAddress, sizeof(mepObj->mepData->dot1agCfmMepMacAddress));
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain "dot1agCfmMepLowPrDef -
*           An integer value specifying the lowest priority defect
*           that is allowed to generate fault alarm" of
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val       @b{(output)} Lowest priority defect value
*                                  Dot1agCfmLowestAlarmPri_t
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLowPrDefGet(uint32_t mdIndex, uint32_t maIndex,
                                      uint32_t mepId, uint32_t intIfNum,
                                      uint32_t *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      *val  = mepObj->mepData->dot1agCfmGblMepVar.lowestAlarmPri;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set "dot1agCfmMepLowPrDef -
*           An integer value specifying the lowest priority defect
*           that is allowed to generate fault alarm" of
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val       @b{(input)} Lowest priority defect value
*                                  Dot1agCfmLowestAlarmPri_t
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLowPrDefSet(uint32_t mdIndex, uint32_t maIndex,
                                      uint32_t mepId, uint32_t intIfNum,
                                      uint32_t val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val > DOT1AG_ALARM_PRIORITY_MAX || val < DOT1AG_ALARM_PRIORITY_MIN)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      if (mepObj->mepData->dot1agCfmGblMepVar.lowestAlarmPri != val)
      {
        mepObj->mepData->dot1agCfmGblMepVar.lowestAlarmPri = val;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain dot1agCfmMepFngAlarmTime -
*           The time that defects must be present
*           before a Fault Alarm is issued (fngAlarmTime. 20.33.3)
*           (default 250 = 2.5s)
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val    @b{(output)}    Time interval expressed as t*10 where
*                                  t is Time interval in seconds
*                                  ex: 2.5s = 250
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepFngAlarmTimeGet(uint32_t mdIndex, uint32_t maIndex,
                                          uint32_t mepId, uint32_t intIfNum,
                                          uint32_t *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      *val  = mepObj->mepData->dot1agCfmMepFngAlarmTime;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepFngAlarmTime -
*          The time that defects must be present
*          before a Fault Alarm is issued (fngAlarmTime. 20.33.3)
*          (default 250 = 2.5s)
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Time interval expressed as t*100
*                                  where t is Time interval in seconds
*                                  ex: 2.5s = 250
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepFngAlarmTimeSet(uint32_t mdIndex, uint32_t maIndex,
                                          uint32_t mepId, uint32_t intIfNum,
                                          uint32_t val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val < OFDPA_DOT1AG_FNG_ALARM_TIME_MIN ||
      val > OFDPA_DOT1AG_FNG_ALARM_TIME_MAX)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      if (mepObj->mepData->dot1agCfmMepFngAlarmTime != val)
      {
        mepObj->mepData->dot1agCfmMepFngAlarmTime = val;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepFngResetTime -
*          The time that defects must be absent before resetting a
*          Fault Alarm (fngResetTime, 20.33.4) (default 10s).
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Time interval expressed as t*100
*                                  where t is Time interval in seconds
*                                  ex: 2.5s = 250
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepFngResetTimeGet(uint32_t mdIndex, uint32_t maIndex,
                                          uint32_t mepId, uint32_t intIfNum,
                                          uint32_t *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      *val = mepObj->mepData->dot1agCfmMepFngResetTime;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepFngResetTime -
*          The time that defects must be absent before resetting a
*          Fault Alarm (fngResetTime, 20.33.4) (default 10s).
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Time interval expressed as t*100
*                                  where t is Time interval in seconds
*                                  ex: 2.5s = 250
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepFngResetTimeSet(uint32_t mdIndex, uint32_t maIndex,
                                          uint32_t mepId, uint32_t intIfNum,
                                          uint32_t val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val < OFDPA_DOT1AG_FNG_ALARM_RESET_TIME_MIN ||
      val > OFDPA_DOT1AG_FNG_ALARM_RESET_TIME_MAX)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      if (mepObj->mepData->dot1agCfmMepFngResetTime != val)
      {
        mepObj->mepData->dot1agCfmMepFngResetTime = val;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmInterval -
*          Set the period between LBM messages for ETH over transport
*          OAM
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    interval   @b{(input)} A value indicating interval between
*                                  LBM messages
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmIntervalSet(uint32_t mdIndex, uint32_t maIndex,
                                                 uint32_t mepId, uint32_t intIfNum,
                                                 uint32_t interval)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmInterval = interval;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmResultOk -
*          A Boolean flag set to true by the bridge port to indicate
*          that LBM is transmitted.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag       @b{(input)} A value of OFDPA_TRUE indicates LBMs
*                                  can be sent by the MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmResultOkSet(uint32_t mdIndex, uint32_t maIndex,
                                               uint32_t mepId, uint32_t intIfNum,
                                               OFDPA_BOOL flag)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (flag != OFDPA_FALSE && flag != OFDPA_TRUE)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmResultOk = flag;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set tdot1agCfmMepTransmitLbmStatus -
*          A Boolean flag set to true by the bridge port to indicate
*          that another LBM may be transmitted.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag       @b{(input)} A value of OFDPA_TRUE indicates LBMs
*                                  can be sent by the MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmStatusSet(uint32_t mdIndex, uint32_t maIndex,
                                               uint32_t mepId, uint32_t intIfNum,
                                               OFDPA_BOOL flag)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (flag != OFDPA_FALSE && flag != OFDPA_TRUE)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmStatus = flag;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Stop dot1agCfmMepTransmitLbmMessages -
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmMessagesStop(uint32_t mdIndex, uint32_t maIndex,
                                                  uint32_t mepId, uint32_t intIfNum)
{
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;
  dot1agMsgCmdData_t  msg;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    rc = OFDPA_E_NONE;
    msg.mdIndex  = mdIndex;
    msg.maIndex  = maIndex;
    msg.mepIndex = mepId;
    msg.intIfNum = intIfNum;

    if (dot1agIssueCmd(dot1agCfmLBMStop, 0, &msg) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to stop LBM for mdIndex %d maIndex %d mepIndex %d intf %d",
                         mdIndex, maIndex, mepId, intIfNum);
      rc = OFDPA_E_FAIL;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}
/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmMessages -
*          The number of Loopback messages to be transmitted.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Number of LBMs to be sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmMessagesSet(uint32_t mdIndex, uint32_t maIndex,
                                                 uint32_t mepId, uint32_t intIfNum,
                                                 uint32_t val)
{
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;
  dot1agMsgCmdData_t  msg;

  if (val < OFDPA_DOT1AG_NUM_LBM_MIN ||
      val > OFDPA_DOT1AG_NUM_LBM_MAX)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepLbiVar.lbmsToSend = val;
      if (val != 0)
      {
        /* Post a message to queue so that mep create event is passed to state machines */
        msg.mdIndex  = mdIndex;
        msg.maIndex  = maIndex;
        msg.mepIndex = mepId;
        msg.intIfNum = intIfNum;
        msg.uintVal  = val;
        if (dot1agIssueCmd(dot1agCfmLBMSend, 0, &msg) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to send LBM for mdIndex %d maIndex %d mepIndex %d intf %d",
                             mdIndex, maIndex, mepId, intIfNum);
          rc = OFDPA_E_FAIL;
        }
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmDestMacAddress
*          dot1agCfmMepTransmitLbmDestIsMepId -
*          The Target MAC Address Field to be transmitted: A unicast
*          destination MAC address. This address will be used if the
*          value of the column dot1agCfmMepTransmitLbmDestIsMepId is
*          'false'.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    dmac       @b{(input)} DMAC for the MEP to which LBM is
*                                  sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmDestMacAddressSet(uint32_t mdIndex,
                                                       uint32_t maIndex,
                                                       uint32_t mepId,
                                                       uint32_t intIfNum,
                                                       uint8_t *dmac)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (dmac == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      memcpy(mepObj->mepData->dot1agCfmMepTransmitLbmDestMacAddress,
             dmac, OFDPA_MAC_ADDR_LEN);
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmDestIsMepId -
*
*          True indicates that MEPID of the target MEP is used for
*          Loopback transmission. False indicates that unicast destination
*          MAC address of the target MEP is used for Loopback transmission.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag       @b{(input)} OFDPA_TRUE/OFDPA_FALSE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmDestIsMepIdSet(uint32_t mdIndex,
                                                    uint32_t maIndex,
                                                    uint32_t mepId,
                                                    uint32_t intIfNum,
                                                    OFDPA_BOOL flag)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (flag != OFDPA_FALSE && flag != OFDPA_TRUE)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmDestIsMepId = flag;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmDestMepId,
*          dot1agCfmMepTransmitLbmDestIsMepId -
*          The Maintenance association End Point Identifier of another
*          MEP in the same Maintenance Association to which the LBM is
*          to be sent.This address will be used if the value of the column
*          dot1agCfmMepTransmitLbmDestIsMepId is 'true'.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    destMepid  @b{(input)} Desstination MEPID to which LBM is
*                                  to be sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmDestMepIdSet(uint32_t mdIndex, uint32_t maIndex,
                                                  uint32_t mepId, uint32_t intIfNum,
                                                  uint32_t destMepId)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (destMepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmDestMepId   = destMepId;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmVlanPrio,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    vlanPrio   @b{(input)} vlan priority to be set in LBM header
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmVlanPrioSet(uint32_t mdIndex, uint32_t maIndex,
                                                 uint32_t mepId, uint32_t intIfNum,
                                                 uint32_t vlanPrio)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (vlanPrio < 0 || vlanPrio > 7)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmVlanPriority = vlanPrio;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmVlanDropEnable,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    dropEnable @b{(input)} Vlan Drop Enable to be set in LBM header
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmVlanDropEnableSet(uint32_t mdIndex, uint32_t maIndex,
                                                       uint32_t mepId, uint32_t intIfNum,
                                                       OFDPA_BOOL dropEnable)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmVlanDropEnable = dropEnable;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmPacketLength,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    tlvType    @b{(input)} Tlv Type to be set
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmPacketLengthSet(uint32_t mdIndex, uint32_t maIndex,
                                                     uint32_t mepId, uint32_t intIfNum,
                                                     uint32_t packetLength)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmPacketLength = packetLength;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmTestTlvType,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    tlvType    @b{(input)} Tlv Type to be set
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmTestTlvSet(uint32_t mdIndex, uint32_t maIndex,
                                                uint32_t mepId, uint32_t intIfNum,
                                                OFDPA_OAM_LB_TEST_TLV_TYPE_t tlvType)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmTestTlvPresent = OFDPA_TRUE;
      mepObj->mepData->dot1agCfmMepTransmitLbmTestTlvType = tlvType;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmTtl,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    Ttl        @b{(input)} Ttl to bet set in LBM sent to MIP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmTtlSet(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t intIfNum,
                                            uint32_t Ttl)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmTtl = Ttl;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmDiscovery
*
* @param    mdIndex     @b{(input)} Index used for MD indentification
* @param    maIndex     @b{(input)} Index for MA identification
* @param    mepId       @b{(input)} Index to identify a MEP
* @param    intIfNum    @b{(input)} Interface on which mep is to be
*                                   created
* @param    lbDiscovery @b{(input)} lbDiscovery test mode to be set.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmDiscoverySet(uint32_t mdIndex, uint32_t maIndex,
                                                  uint32_t mepId, uint32_t intIfNum,
                                                  OFDPA_OAM_LB_DISCOVERY_TYPE_t lbDiscovery)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLbmDiscovery = lbDiscovery;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Stop LBM on a give MEP
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    destMac    @b{(input)} MAC address of MP to which LBM is
*                                  sent
* @param    destMepid  @b{(input)} Destination MEPID to which LBM is
*                                  to be sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLbmStop(uint32_t mdIndex, uint32_t maIndex,
                               uint32_t mepId)
{
  uint32_t   intIfNum;

  if (dot1agMepParamGet(mdIndex, maIndex, mepId, 0,
                        MEP_PARAM_dot1agCfmMepIfIndex, &intIfNum) != OFDPA_E_NONE)
  {
    return OFDPA_E_ERROR;
  }

  if (dot1agCfmMepTransmitLbmMessagesStop(mdIndex, maIndex,
                                          mepId, intIfNum) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose Send an LBM on a give MEP
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    destMac    @b{(input)} MAC address of MP to which LBM is
*                                  sent
* @param    destMepid  @b{(input)} Destination MEPID to which LBM is
*                                  to be sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLbmSend(uint32_t mdIndex, uint32_t maIndex,
                               uint32_t mepId, uint8_t *destMac,
                               uint32_t destMepId, uint32_t msgCount)
{
  uint32_t   intIfNum;
  OFDPA_BOOL transmitStatus = OFDPA_FALSE, sendPing = OFDPA_FALSE;

  /* Validate Input */
  if (destMac == 0 && destMepId == 0)
  {
    return OFDPA_E_PARAM;
  }

  if (dot1agMepParamGet(mdIndex, maIndex, mepId, 0,
                        MEP_PARAM_dot1agCfmMepIfIndex, &intIfNum) != OFDPA_E_NONE)
  {
    return OFDPA_E_ERROR;
  }

  /* Check if TransmitLbmStatus is false. If so step back */
  if (dot1agMepParamGet(mdIndex, maIndex, mepId, intIfNum,
                        MEP_PARAM_dot1agCfmMepTransmitLbmStatus,
                        &transmitStatus) != OFDPA_E_NONE ||
      transmitStatus == OFDPA_FALSE)
  {
    /* set resultOk to false */
    if (dot1agCfmMepTransmitLbmResultOkSet(mdIndex, maIndex,
                                           mepId, intIfNum, OFDPA_FALSE) != OFDPA_E_NONE)
    {
      return OFDPA_E_FAIL;
    }
    return OFDPA_E_REQUEST_DENIED;
  }

  /* set the transmission status to false, to disallow other entities */
  if (dot1agCfmMepTransmitLbmStatusSet(mdIndex, maIndex,
                                       mepId, intIfNum, OFDPA_FALSE) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  /* Set MAC Address to which LBM is to be sent */
  if (destMac != 0)
  {
    if (dot1agCfmMepTransmitLbmDestMacAddressSet(mdIndex, maIndex,
                                                 mepId, intIfNum, destMac) == OFDPA_E_NONE)
    {
      if (dot1agCfmMepTransmitLbmDestIsMepIdSet(mdIndex, maIndex,
                                                mepId, intIfNum, OFDPA_FALSE) == OFDPA_E_NONE)
      {
        sendPing = OFDPA_TRUE;
      }
    }
  }
  else
  {
    if (dot1agCfmMepTransmitLbmDestMepIdSet(mdIndex, maIndex,
                                            mepId, intIfNum, destMepId) == OFDPA_E_NONE)
    {
      if (dot1agCfmMepTransmitLbmDestIsMepIdSet(mdIndex, maIndex,
                                                mepId, intIfNum, OFDPA_TRUE) == OFDPA_E_NONE)
      {
        sendPing = OFDPA_TRUE;
      }
    }
  }

  if (sendPing == OFDPA_TRUE &&
      dot1agCfmMepTransmitLbmMessagesSet(mdIndex, maIndex,
                                         mepId, intIfNum, msgCount) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  if (dot1agCfmMepTransmitLbmResultOkSet(mdIndex, maIndex,
                                         mepId, intIfNum, OFDPA_TRUE) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  /* set the transmission status to true, to allow other entities */
  if (dot1agCfmMepTransmitLbmStatusSet(mdIndex, maIndex,
                                       mepId, intIfNum, OFDPA_TRUE) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose Obtain dot1agCfmMepCcmSequenceErrors - The total number of
*          out-of-sequence CCMs received from all remote MEPs.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  CCM out of sequence rx pkt count
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepCcmSequenceErrors(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t intIfNum,
                                            uint32_t *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      *val = mepObj->mepData->dot1agCfmMepCcmSequenceErrors;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Obtain dot1agCfmMepCcmSequenceErrors - The total number of
*          out-of-sequence CCMs received from all remote MEPs.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  CCM out of sequence rx pkt count
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepCcmSequenceErrorsGet(uint32_t mdIndex, uint32_t maIndex,
                                               uint32_t mepId, uint32_t intIfNum,
                                               uint32_t *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      *val = mepObj->mepData->dot1agCfmMepCcmSequenceErrors;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Obtain dot1agCfmMepLbrIn -
*          Total number of valid, in-order Loopback Replies received.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  LBR rx count
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLbrInGet(uint32_t mdIndex, uint32_t maIndex,
                                   uint32_t mepId, uint32_t intIfNum,
                                   uint32_t *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      *val = mepObj->mepData->dot1agCfmMepLbrIn;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Obtain dot1agCfmMepLbrInOutOfOrder -
*          The total number of valid, out-of-order Loopback Replies
*          received.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} LBR rx out-of-order count
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLbrInOutOfOrderGet(uint32_t mdIndex, uint32_t maIndex,
                                             uint32_t mepId, uint32_t intIfNum,
                                             uint32_t *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    rc = OFDPA_E_NONE;
    *val = mepObj->mepData->dot1agCfmMepLbrInOutOfOrder;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Obtain dot1agCfmMepLbrBadMsdu -
*          The total number of LBRs received whose
*          mac_service_data_unit did not match (except for the OpCode)
*          that of the corresponding LBM (20.2.3).
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Lbr rx count with mismatch
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLbrBadMsduGet(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t intIfNum,
                                        uint32_t *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      *val = mepObj->mepData->dot1agCfmMepLbrInOutOfOrder;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLtmTargetMacAddress
*          The Target MAC Address Field to be transmitted: A unicast
*          destination MAC address. This address will be used if the
*          value of the column dot1agCfmMepTransmitLtmTargetIsMepId is
*          'false'.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    dmac       @b{(input)} DMAC for the MEP to which LTM is
*                                  sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmTargetMacAddressSet(uint32_t mdIndex,
                                                         uint32_t maIndex,
                                                         uint32_t mepId,
                                                         uint32_t intIfNum,
                                                         uint8_t *dmac)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (dmac == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      memcpy(mepObj->mepData->dot1agCfmMepTransmitLtmTargetMacAddress,
             dmac, OFDPA_MAC_ADDR_LEN);
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepTransmitLtmTargetMepId
*           An indication of the Target MAC Address Field to be
*           transmitted: The Maintenance association End Point Identifier
*           of another MEP in the same Maintenance Association
*           This address will be used if the value of the column
*           dot1agCfmMepTransmitLtmTargetIsMepId is 'true'.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    destMepId  @b{(input)} MEPID for the MEP to which LTM is
*                                  sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmTargetMepIdSet(uint32_t mdIndex,
                                                    uint32_t maIndex,
                                                    uint32_t mepId,
                                                    uint32_t intIfNum,
                                                    uint32_t destMepId)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (destMepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLtmTargetMepId = destMepId;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepTransmitLtmTargetIsMepId
*           True indicates that MEPID of the target MEP is used for
*           Linktrace transmission. False indicates that unicast
*           destination MAC address of the target MEP is used for
*           Loopback transmission.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag  @b{(input)}  OFDPA_TRUE/OFDPA_FALSE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmTargetIsMepIdSet(uint32_t mdIndex,
                                                      uint32_t maIndex,
                                                      uint32_t mepId,
                                                      uint32_t intIfNum,
                                                      OFDPA_BOOL flag)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (flag != OFDPA_FALSE && flag != OFDPA_TRUE)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLtmTargetIsMepId = flag;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepTransmitLtmTtl -
*           The LTM TTL field. Default value, if not specified, is 64.
*           The TTL field indicates the number of hops remaining to the
*           LTM.  Decremented by 1 by each Linktrace Responder that
*           handles the LTM.  The value returned in the LTR is one less
*           than that received in the LTM.  If the LTM TTL is 0 or 1, the
*           LTM is not forwarded to the next hop, and if 0, no LTR is
*           generated.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  TTL for the LTM message
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmTtlSet(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t intIfNum,
                                            uint32_t val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (val > OFDPA_DOT1AG_LTM_TTL_MAX)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      mepObj->mepData->dot1agCfmMepTransmitLtmTtl = val;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLtmFlags - The flags field for LTMs
*          transmitted by the MEP. Transmission is initiated by writing
*          a value to the dot1agCfmMepTransmitLtmFlags object
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag       @b{(input)} A value of 0x01 to start transmission
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmFlagsSet(uint32_t mdIndex, uint32_t maIndex,
                                              uint32_t mepId, uint32_t intIfNum,
                                              uint8_t flag)
{
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t  *mepObj;
  dot1agMsgCmdData_t  msg;

  /* Validate input */
  if (flag != DOT1AG_MEP_XMIT_LTM_FLAG_DEFVAL)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      if (dot1agCFM.dot1agCfmLtmrVar.ltitTimer != 0)
      {
        dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
        return OFDPA_E_REQUEST_DENIED;
      }
  
      mepObj->mepData->dot1agCfmMepTransmitLtmFlags = flag;
      mepObj->mepData->dot1agCfmMepTransmitLtmResult = OFDPA_FALSE;
      /* Post a message to queue so that mep create event is passed to state machines */
      msg.mdIndex  = mdIndex;
      msg.maIndex  = maIndex;
      msg.mepIndex = mepId;
      msg.intIfNum = intIfNum;
      msg.uintVal  = flag;
      if (dot1agIssueCmd(dot1agCfmLTMSend, 0, &msg) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to send LTM for mdIndex %d maIndex %d mepIndex %d intf %d",
                           mdIndex, maIndex, mepId, intIfNum);
        rc = OFDPA_E_FAIL;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose Send an LTM on a give MEP
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    destMac    @b{(input)} MAC address of MP to which LTM is
*                                  sent
* @param    destMepid  @b{(input)} Destination MEPID to which LTM is
*                                  to be sent
* @param    ttl        @b{(input)} Max hops to be used in LTM
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLtmSend(uint32_t mdIndex, uint32_t maIndex,
                               uint32_t mepId, uint8_t *destMac,
                               uint32_t destMepId, uint8_t ttl)
{
  uint32_t   intIfNum;
  OFDPA_BOOL transmitStatus = OFDPA_FALSE, sendTraceroute = OFDPA_FALSE;

  /* Validate Input */
  if (destMac == 0 && destMepId == 0)
  {
    return OFDPA_E_ERROR;
  }

  if (dot1agMepParamGet(mdIndex, maIndex, mepId, 0,
                        MEP_PARAM_dot1agCfmMepIfIndex, &intIfNum) != OFDPA_E_NONE)
  {
    return OFDPA_E_ERROR;
  }

  /* Check if TransmitLbmStatus is false. If so step back */
  if (dot1agMepParamGet(mdIndex, maIndex, mepId, intIfNum,
                        MEP_PARAM_dot1agCfmMepTransmitLtmStatus,
                        &transmitStatus) != OFDPA_E_NONE ||
      transmitStatus == OFDPA_FALSE)
  {
    return OFDPA_E_REQUEST_DENIED;
  }

  if (dot1agCfmMepTransmitLtmTtlSet(mdIndex, maIndex,
                                    mepId, intIfNum, ttl) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  /* Set MAC Address to which LBM is to be sent */
  if (destMac != 0)
  {
    if (dot1agCfmMepTransmitLtmTargetMacAddressSet(mdIndex, maIndex,
                                                   mepId, intIfNum, destMac) == OFDPA_E_NONE)
    {
      if (dot1agCfmMepTransmitLtmTargetIsMepIdSet(mdIndex, maIndex,
                                                  mepId, intIfNum, OFDPA_FALSE) == OFDPA_E_NONE)
      {
        sendTraceroute = OFDPA_TRUE;
      }
    }
  }
  else
  {
    if (dot1agCfmMepTransmitLtmTargetMepIdSet(mdIndex, maIndex,
                                              mepId, intIfNum, destMepId) == OFDPA_E_NONE)
    {
      if (dot1agCfmMepTransmitLtmTargetIsMepIdSet(mdIndex, maIndex,
                                                  mepId, intIfNum, OFDPA_TRUE) == OFDPA_E_NONE)
      {
        sendTraceroute = OFDPA_TRUE;
      }
    }
  }

  if (sendTraceroute == OFDPA_TRUE &&
      dot1agCfmMepTransmitLtmFlagsSet(mdIndex, maIndex,
                                      mepId, intIfNum, DOT1AG_MEP_XMIT_LTM_FLAG_DEFVAL) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose Clear Traceroute Cache
*
* @param    none
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmTraceRouteCacheClear()
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agMsgCmdData_t msg;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  /* Link Trace is in progress. So deny the request */
  if (dot1agCFM.dot1agCfmLtmrVar.ltitTimer != 0)
  {
    dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
    return OFDPA_E_REQUEST_DENIED;
  }
  rc = OFDPA_E_NONE;

  /* Post a message to queue so that traceroute cache clear event is passed to state machines */
  memset(&msg, 0x00, sizeof(msg));
  if (dot1agIssueCmd(dot1agCfmLTRCacheClear, 0, &msg) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to clear traceroute cache", 0);
    rc = OFDPA_E_FAIL;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfModeSet
*           AIS/LCK/CSF are set as enable/disable based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(input)} AIS/LCK/CSF mode {enable, disable}
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfModeSet(uint32_t mdIndex,
                                           uint32_t maIndex,
                                           uint32_t mepId,
                                           uint32_t intIfNum,
                                           uint32_t defectType,
                                           OFDPA_BOOL   mode)
{
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;
  uint32_t            event;
  dot1agCfmMepObj_t  *mepObj;
  dot1agMsgCmdData_t  msg;

  /* Validate input */
  if (mepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  if ((defectType >  Y1731_DEFECT_FUNCTION_CSF) ||
      (defectType <  Y1731_DEFECT_FUNCTION_AIS))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      switch (defectType)
      {
        case Y1731_DEFECT_FUNCTION_AIS:
          mepObj->mepData->y1731CfmMepAisSendVar.aisEnable = mode;
          event = dot1agCfmDefectAis;
          break;
        case Y1731_DEFECT_FUNCTION_LCK:
          mepObj->mepData->y1731CfmMepLckSendVar.lckEnable = mode;
          event = dot1agCfmDefectLck;
          break;
        case Y1731_DEFECT_FUNCTION_CSF:
          mepObj->mepData->y1731CfmMepCsfSendVar.csfEnable = mode;
          event = dot1agCfmDefectCsf;
          break;
        default:
          rc = OFDPA_E_ERROR;
          break;
      }
    }
  }

  if (rc != OFDPA_E_NONE)
  {
    /* Release the lock and return */
    dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
    return rc;
  }

  /* Post a message to queue so that mep create event is passed to state machines */
  if ((Y1731_DEFECT_FUNCTION_LCK == defectType) ||
      (Y1731_DEFECT_FUNCTION_AIS == defectType) ||
      ((Y1731_DEFECT_FUNCTION_CSF == defectType) && (OFDPA_FALSE == mode)))
  {
    msg.mdIndex  = mdIndex;
    msg.maIndex  = maIndex;
    msg.mepIndex = mepId;
    msg.intIfNum = intIfNum;
    if (OFDPA_FALSE == mode)
      msg.uintVal = OFDPA_FALSE;
    else
      msg.uintVal = OFDPA_TRUE;
    if (dot1agIssueCmd(event, 0, &msg) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to create MEP for mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      rc = OFDPA_E_FAIL;
    }
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfModeSet
*           AIS/LCK/CSF moded (enable/disable) are based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)} AIS/LCK/CSF mode {enable, disable}
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfModeGet(uint32_t mdIndex,
                                           uint32_t maIndex,
                                           uint32_t mepId,
                                           uint32_t intIfNum,
                                           uint32_t defectType,
                                           OFDPA_BOOL   *mode)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (mepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  if ((defectType >  Y1731_DEFECT_FUNCTION_CSF) ||
      (defectType <  Y1731_DEFECT_FUNCTION_AIS))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      switch (defectType)
      {
        case Y1731_DEFECT_FUNCTION_AIS:
          *mode = mepObj->mepData->y1731CfmMepAisSendVar.aisEnable;
          break;
        case Y1731_DEFECT_FUNCTION_LCK:
          *mode = mepObj->mepData->y1731CfmMepLckSendVar.lckEnable;
          break;
        case Y1731_DEFECT_FUNCTION_CSF:
          *mode = mepObj->mepData->y1731CfmMepCsfSendVar.csfEnable;
          break;
        default:
          rc = OFDPA_E_ERROR;
          break;
      }
    }
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfClientLevelSet
*           AIS/LCK/CSF client level is set based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    clientLevel@b{(input)} AIS/LCK/CSF client level
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfClientLevelSet(uint32_t mdIndex,
                                                  uint32_t maIndex,
                                                  uint32_t mepId,
                                                  uint32_t intIfNum,
                                                  uint32_t defectType,
                                                  uint8_t  clientLevel)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (mepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  if ((defectType >  Y1731_DEFECT_FUNCTION_CSF) ||
      (defectType <  Y1731_DEFECT_FUNCTION_AIS))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      switch (defectType)
      {
        case Y1731_DEFECT_FUNCTION_AIS:
          mepObj->mepData->y1731CfmMepAisSendVar.aisClientLevel = clientLevel;
          break;
        case Y1731_DEFECT_FUNCTION_LCK:
          mepObj->mepData->y1731CfmMepLckSendVar.lckClientLevel = clientLevel;
          break;
        case Y1731_DEFECT_FUNCTION_CSF:
          mepObj->mepData->y1731CfmMepCsfSendVar.csfClientLevel = clientLevel;
          break;
        default:
          rc = OFDPA_E_ERROR;
          break;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfClientLevelGet
*           AIS/LCK/CSF client level is get based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    clientLevel@b{(output)} AIS/LCK/CSF client level
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfClientLevelGet(uint32_t mdIndex,
                                                  uint32_t maIndex,
                                                  uint32_t mepId,
                                                  uint32_t intIfNum,
                                                  uint32_t defectType,
                                                  uint8_t *clientLevel)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (mepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  if ((defectType >  Y1731_DEFECT_FUNCTION_CSF) ||
      (defectType <  Y1731_DEFECT_FUNCTION_AIS))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      switch (defectType)
      {
        case Y1731_DEFECT_FUNCTION_AIS:
          *clientLevel = mepObj->mepData->y1731CfmMepAisSendVar.aisClientLevel;
          break;
        case Y1731_DEFECT_FUNCTION_LCK:
          *clientLevel = mepObj->mepData->y1731CfmMepLckSendVar.lckClientLevel;
          break;
        case Y1731_DEFECT_FUNCTION_CSF:
          *clientLevel = mepObj->mepData->y1731CfmMepCsfSendVar.csfClientLevel;
          break;
        default:
          rc = OFDPA_E_ERROR;
          break;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfPrioritySet
*           AIS/LCK/CSF priority is set based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    priority   @b{(input)} AIS/LCK/CSF priority
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfPrioritySet(uint32_t mdIndex,
                                               uint32_t maIndex,
                                               uint32_t mepId,
                                               uint32_t intIfNum,
                                               uint32_t defectType,
                                               uint8_t  priority)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (mepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  if ((defectType >  Y1731_DEFECT_FUNCTION_CSF) ||
      (defectType <  Y1731_DEFECT_FUNCTION_AIS))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      switch (defectType)
      {
        case Y1731_DEFECT_FUNCTION_AIS:
          mepObj->mepData->y1731CfmMepAisSendVar.aisPriority = priority;
          break;
        case Y1731_DEFECT_FUNCTION_LCK:
          mepObj->mepData->y1731CfmMepLckSendVar.lckPriority = priority;
          break;
        case Y1731_DEFECT_FUNCTION_CSF:
          mepObj->mepData->y1731CfmMepCsfSendVar.csfPriority = priority;
          break;
        default:
          rc = OFDPA_E_ERROR;
          break;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfPriorityGet
*           AIS/LCK/CSF priority is get based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    clientLevel@b{(output)} AIS/LCK/CSF priority
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfPriorityGet(uint32_t mdIndex,
                                               uint32_t maIndex,
                                               uint32_t mepId,
                                               uint32_t intIfNum,
                                               uint32_t defectType,
                                               uint8_t *priority)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (mepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  if ((defectType >  Y1731_DEFECT_FUNCTION_CSF) ||
      (defectType <  Y1731_DEFECT_FUNCTION_AIS))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      switch (defectType)
      {
        case Y1731_DEFECT_FUNCTION_AIS:
          *priority = mepObj->mepData->y1731CfmMepAisSendVar.aisPriority;
          break;
        case Y1731_DEFECT_FUNCTION_LCK:
          *priority = mepObj->mepData->y1731CfmMepLckSendVar.lckPriority;
          break;
        case Y1731_DEFECT_FUNCTION_CSF:
          *priority = mepObj->mepData->y1731CfmMepCsfSendVar.csfPriority;
          break;
        default:
          rc = OFDPA_E_ERROR;
          break;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfIntervalSet
*           AIS/LCK/CSF interval is set based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    interval   @b{(input)} AIS/LCK/CSF interval
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfIntervalSet(uint32_t mdIndex,
                                               uint32_t maIndex,
                                               uint32_t mepId,
                                               uint32_t intIfNum,
                                               uint32_t defectType,
                                               uint32_t interval)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (mepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  if ((defectType >  Y1731_DEFECT_FUNCTION_CSF) ||
      (defectType <  Y1731_DEFECT_FUNCTION_AIS))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      switch (defectType)
      {
        case Y1731_DEFECT_FUNCTION_AIS:
          mepObj->mepData->y1731CfmMepAisSendVar.aisInterval = interval;
          break;
        case Y1731_DEFECT_FUNCTION_LCK:
          mepObj->mepData->y1731CfmMepLckSendVar.lckInterval = interval;
          break;
        case Y1731_DEFECT_FUNCTION_CSF:
          mepObj->mepData->y1731CfmMepCsfSendVar.csfInterval = interval;
          break;
        default:
          rc = OFDPA_E_ERROR;
          break;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfIntervalGet
*           AIS/LCK/CSF interval is get based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    interval   @b{(output)} AIS/LCK/CSF interval
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfIntervalGet(uint32_t mdIndex,
                                               uint32_t maIndex,
                                               uint32_t mepId,
                                               uint32_t intIfNum,
                                               uint32_t defectType,
                                               uint32_t *interval)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  /* Validate input */
  if (mepId >  OFDPA_DOT1AG_MEPID_MAX)
  {
    return OFDPA_E_ERROR;
  }

  if ((defectType >  Y1731_DEFECT_FUNCTION_CSF) ||
      (defectType <  Y1731_DEFECT_FUNCTION_AIS))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum,
                                   AVL_EXACT)) != 0)
  {
    if (mepObj->mepData != NULL)
    {
      rc = OFDPA_E_NONE;
      switch (defectType)
      {
        case Y1731_DEFECT_FUNCTION_AIS:
          *interval = mepObj->mepData->y1731CfmMepAisSendVar.aisInterval;
          break;
        case Y1731_DEFECT_FUNCTION_LCK:
          *interval = mepObj->mepData->y1731CfmMepLckSendVar.lckInterval;
          break;
        case Y1731_DEFECT_FUNCTION_CSF:
          *interval = mepObj->mepData->y1731CfmMepCsfSendVar.csfInterval;
          break;
        default:
          rc = OFDPA_E_ERROR;
          break;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Create a maintenance end point on an interface given
*           (mdIndex) and (maIndex) and mepId
*           and direction
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    direction  @b{(input)} Direction of MEP to be created(Up/Down)
* @param    macAddress @b{(input)} MAC address for PDUs sent from this MEP
* @param    lmepId     @b{(input)} unique identifier of this MEP entry in the OF-DPA MEP table
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_REQUEST_DENIED      No software and hardware mix of MEPs
*                                       is allowed for a given port and VLAN
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepCreate(uint32_t intIfNum, uint32_t mdIndex,
                              uint32_t maIndex, uint32_t mepId,
                              Dot1agCfmMpDirection_t direction,
                              uint8_t *macAddress, uint32_t lmepId)
{
  dot1agCfmMAObj_t      *maObjEntry;
  dot1agCfmMAObjKey_t    maObjEntryKey;
  dot1agCfmMepObj_t     *pMepObj;
  dot1agCfmMepObj_t      mepObj;
  dot1agCfmMepObjKey_t   mepObjKey;
  dot1agCfmMaNetTable_t *maEntry  = 0;
  dot1agCfmMdTable_t    *mdEntry  = 0;
  dot1agCfmMepTable_t   *mepEntry = 0;
  dot1agMsgCmdData_t     msg;
  OFDPA_ERROR_t          rc       = OFDPA_E_NONE;
  uint32_t               prevFreeIdx;
  uint32_t               primVID;
  uint32_t               tmpMdIndex, tmpMaIndex, tmpMepId, tmpIntf;

  /* Validate input */
  if (mdIndex == 0 ||
      maIndex == 0 ||
      (direction != DOT1AG_MP_DIRECTION_UP && direction != DOT1AG_MP_DIRECTION_DOWN) ||
      (mepId < OFDPA_DOT1AG_MEPID_MIN  || mepId > OFDPA_DOT1AG_MEPID_MAX))
  {
    return OFDPA_E_ERROR;
  }

  tmpMdIndex = mdIndex;
  tmpMaIndex = maIndex;
  tmpMepId = tmpIntf = 0;
  while (dot1agMepNextGet(&tmpMdIndex, &tmpMaIndex, &tmpMepId, &tmpIntf) == OFDPA_E_NONE)
  {
    if (tmpMdIndex != mdIndex || tmpMaIndex != maIndex)
    {
      break;
    }

    /* Return error, if MEP is already configured on the requested interface or
       if the requested MEP id is already configured in a given MA
    */
    if (tmpMepId == mepId || tmpIntf == intIfNum)
    {
      return OFDPA_E_ERROR;
    }
  }

  if (OFDPA_E_NONE != dot1agMaPrimVLANGet(mdIndex, maIndex, &primVID))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Failed to get VLAN associated with MA %d", maIndex);
    return OFDPA_E_ERROR;
  }

  /* Modified by JiaoYong, 2016/4/1 ADD BHH MPLS-TP OAM*/  
  #if 0  
  /* Currently only Ethernet can be supported in HW */
  if ((OFDPA_OAM_MEG_TYPE_ETHERNET != ofdbMegTypeFromVidGet(primVID)) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "MEP ID %d, type not supported in HW", mepId);
    return OFDPA_E_ERROR;
  }
  #else
  /* Currently only Ethernet can be supported in HW */
  if (((OFDPA_OAM_MEG_TYPE_ETHERNET != ofdbMegTypeFromVidGet(primVID)) 
       && (OFDPA_OAM_MEG_TYPE_G8113_1 != ofdbMegTypeFromVidGet(primVID))) 
       && (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId)))
  {
    printf("\r\nMEP ID %d, type not supported in HW\n", mepId);
    return OFDPA_E_ERROR;
  }
  #endif

  /* Hardware MEP configuration is not allowed if done earlier in software
   * for the same port and VLAN. Same applies to software MEP configuration.
   */
  memset(&mepObjKey, 0x0, sizeof(mepObjKey));
  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  pMepObj = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjKey, AVL_NEXT);
  while (pMepObj != 0)
  {
    if ((pMepObj->key.dot1agCfmMepIfIndex == intIfNum) &&
        ((pMepObj->mepData != NULL) &&
         (pMepObj->mepData->dot1agCfmMepPrimaryVid == primVID)))
    {
      if (((OFDPA_TRUE == dot1agIsMepHwEnabled(mepId)) &&
           (OFDPA_FALSE == dot1agIsMepHwEnabled(pMepObj->key.dot1agCfmMepIdentifier))) ||
          ((OFDPA_FALSE == dot1agIsMepHwEnabled(mepId)) &&
           (OFDPA_TRUE == dot1agIsMepHwEnabled(pMepObj->key.dot1agCfmMepIdentifier))))
      {
        dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
        return OFDPA_E_REQUEST_DENIED;
      }
    }
    memcpy(&mepObjKey, &pMepObj->key, sizeof(mepObjKey));
    pMepObj = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjKey, AVL_NEXT);
  }

  if ((mdEntry = dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE)) != 0)
  {
    maObjEntryKey.dot1agCfmMdIndex = mdEntry->dot1agCfmMdIndex;
    maObjEntryKey.dot1agCfmMaIndex = maIndex;

    maObjEntry = avlSearch(&dot1agCFM.dot1agMATree, &maObjEntryKey, AVL_EXACT);
    if (maObjEntry == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_ERROR;
    }
    maEntry = maObjEntry->maData;
    primVID = maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId;

    if (maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId == 0)
    {
      /* MA not yet ready for MEP creations */
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_ERROR;
    }

    memset(&mepObjKey, 0x0, sizeof(mepObjKey));
    mepObjKey.dot1agCfmMdIndex = mdEntry->dot1agCfmMdIndex;
    mepObjKey.dot1agCfmMaIndex = maIndex;

    rc = OFDPA_E_NONE;
    pMepObj = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjKey, AVL_NEXT);
    while (pMepObj != 0)
    {
      if (pMepObj->key.dot1agCfmMdIndex != mdEntry->dot1agCfmMdIndex ||
          pMepObj->key.dot1agCfmMaIndex != maIndex)
      {
        /* no MEP entry exists for this MD, MA*/
        break;
      }

      if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to create a HW MEP with mepId: %d "
                           "for mdIndex %d maIndex %d as a MEP with id:%d already exists on %d",
                           mepId, pMepObj->key.dot1agCfmMdIndex, pMepObj->key.dot1agCfmMaIndex,
                           pMepObj->key.dot1agCfmMepIdentifier, pMepObj->key.dot1agCfmMepIfIndex);

        rc =  OFDPA_E_ERROR;
        break;
      }

      if (pMepObj->key.dot1agCfmMepIdentifier == mepId ||
          pMepObj->key.dot1agCfmMepIfIndex == intIfNum)
      {
        rc = OFDPA_E_EXISTS;
        break;
      }

      /* 802.1ag d8-1  22.2.2 - If any MEPs are configured on an MA, then in any given Bridge,
         either all of those MEPs are Up MEPs, or  all of them are Down MEPs; a Bridge refuses
         to create an Up (Down) MEPs if a Down (Up) MEP already is configured in that MA in that
         Bridge. */
      if ((pMepObj->mepData != NULL) &&
          (pMepObj->mepData->dot1agCfmMepDirection != direction))
      {
        rc = OFDPA_E_ERROR;
      }

      memcpy(&mepObjKey, &pMepObj->key, sizeof(mepObjKey));
      pMepObj = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjKey, AVL_NEXT);
    } /* End of loop thru Mep tree */

    /* A new MEP can be created */
    if (rc == OFDPA_E_NONE)
    {
      if (DOT1AG_MEP_TABLE(dot1agCFM.dot1agMEPTableIndex).active != DOT1AG_ROW_INACTIVE)
      {
        /* Table management issue */
        dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
        return OFDPA_E_FULL;
      }
      mepEntry = &DOT1AG_MEP_TABLE(dot1agCFM.dot1agMEPTableIndex);

      /* Fill in the MEP table */
      mepEntry->maData                  = maEntry;
      mepEntry->dot1agCfmMepIdentifier  = mepId;
      mepEntry->dot1agCfmMepIfIndex     = intIfNum;
      mepEntry->dot1agCfmMepDirection   = direction;
      mepEntry->dot1agCfmMepPrimaryVid  = primVID;
      memcpy(&mepEntry->dot1agCfmMepMacAddress, macAddress, sizeof(mepEntry->dot1agCfmMepMacAddress));

      /* Default MEP r/w params */
      mepEntry->dot1agCfmMepFngAlarmTime  = FD_DOT1AG_FNG_ALARM_TIME;
      mepEntry->dot1agCfmMepFngResetTime  = FD_DOT1AG_FNG_ALARM_RESET_TIME;
      mepEntry->dot1agCfmMepCciEnabled    = FD_DOT1AG_MEP_CCI_ENABLED_MODE;
      mepEntry->dot1agCfmMepActive        = FD_DOT1AG_MEP_ACTIVE_MODE;
      mepEntry->dot1agCfmGblMepVar.lowestAlarmPri  = FD_DOT1AG_FNG_LOWEST_ALARM_PRI;
      mepEntry->mepPktChanged = OFDPA_TRUE;

      /* Default MEP AIS params */
      mepEntry->y1731CfmMepAisSendState               = Y1731_AIS_SEND_STOP;
      mepEntry->y1731CfmMepAisSendVar.aisEnable       = OFDPA_FALSE;
      mepEntry->y1731CfmMepAisSendVar.aisClientLevel  = 0;
      mepEntry->y1731CfmMepAisSendVar.aisPriority     = 0;
      mepEntry->y1731CfmMepAisSendVar.aisInterval     = Y1731_CFM_PERIOD_1PPS;
      mepEntry->y1731CfmMepAisSendVar.aisSendTimer    = 0;

      mepEntry->y1731CfmMepAisRcvState                = Y1731_AIS_RCV_IDLE;
      mepEntry->y1731CfmMepAisRcvVar.aisRcvTimer      = 0;

      /* Default MEP LCK params */
      mepEntry->y1731CfmMepLckSendState               = Y1731_LCK_SEND_STOP;
      mepEntry->y1731CfmMepLckSendVar.lckEnable       = OFDPA_FALSE;
      mepEntry->y1731CfmMepLckSendVar.lckClientLevel  = 0;
      mepEntry->y1731CfmMepLckSendVar.lckPriority     = 0;
      mepEntry->y1731CfmMepLckSendVar.lckInterval     = Y1731_CFM_PERIOD_1PPS;
      mepEntry->y1731CfmMepLckSendVar.lckSendTimer    = 0;

      mepEntry->y1731CfmMepLckRcvState                = Y1731_LCK_RCV_IDLE;
      mepEntry->y1731CfmMepLckRcvVar.lckRcvTimer      = 0;

      /* Default MEP CSF params */
      mepEntry->y1731CfmMepCsfSendState               = Y1731_CSF_SEND_STOP;
      mepEntry->y1731CfmMepCsfSendVar.csfEnable       = OFDPA_FALSE;
      mepEntry->y1731CfmMepCsfSendVar.csfClientLevel  = 0;
      mepEntry->y1731CfmMepCsfSendVar.csfPriority     = 0;
      mepEntry->y1731CfmMepCsfSendVar.csfInterval     = Y1731_CFM_PERIOD_1PPS;
      mepEntry->y1731CfmMepCsfSendVar.csfSendTimer    = 0;

      mepEntry->y1731CfmMepCsfRcvState                = Y1731_CSF_RCV_IDLE;
      mepEntry->y1731CfmMepCsfRcvVar.csfRcvTimer      = 0;

      /* Insert node in MIB view */
      memset(&mepObj, 0x0, sizeof(mepObj));
      mepObj.key.dot1agCfmMdIndex       = mdEntry->dot1agCfmMdIndex;
      mepObj.key.dot1agCfmMaIndex       = maIndex;
      mepObj.key.dot1agCfmMepIdentifier = mepId;
      mepObj.key.dot1agCfmMepIfIndex    = intIfNum;
      mepObj.mepData = mepEntry;

      rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_MEP_OBJ_TREE, &dot1agCFM.dot1agMepTree, &mepObj);
      if (rc == OFDPA_E_NONE)
      {
        mepEntry->active  |= DOT1AG_ROW_ACTIVE;
        /* point TableIndex to next free index */
        prevFreeIdx = dot1agCFM.dot1agMEPTableIndex;

        while (DOT1AG_MEP_TABLE(dot1agCFM.dot1agMEPTableIndex).active != DOT1AG_ROW_INACTIVE)
        {
          dot1agCFM.dot1agMEPTableIndex++;

          if (dot1agCFM.dot1agMEPTableIndex == dpaOamMepTableEntryCountGet())
          {
            dot1agCFM.dot1agMEPTableIndex = 0;
          }

          if (prevFreeIdx == dot1agCFM.dot1agMEPTableIndex)
          {
            break;
          }
        }/* End of MEP table loop */

        /* Post a message to queue so that mep create event is passed to state machines */
        msg.mdIndex  = mdEntry->dot1agCfmMdIndex;
        msg.maIndex  = maEntry->dot1agCfmMaIndex;
        msg.mepIndex = mepId;
        msg.intIfNum = intIfNum;
        msg.localMpId = lmepId;
        memcpy(msg.srcMac, macAddress, sizeof(msg.srcMac));

        if (dot1agIssueCmd(dot1agCfmMEPCreate, 0, &msg) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to create MEP for mdIndex %d maIndex %d mepIndex %d vid %d intf %d",
                             mdEntry->dot1agCfmMdIndex, maEntry->dot1agCfmMaIndex,
                             mepId, primVID, intIfNum);
          rc = OFDPA_E_FAIL;
          mepEntry->active  = DOT1AG_ROW_INACTIVE;
        }
      }/* End of DB entry add check */
    }
  }/* End of active domain get check */
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Delete a maintenance end point on an interface given
*           (mdIndex) and (maIndex) and mepId
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  deleted
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP. Use 0 for
*                                  other UI's
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP. Use 0 for other UI's
* @param    mepId      @b{(input)} Index used to identify a MEP.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepDelete(uint32_t intIfNum, uint32_t mdIndex,
                              uint32_t maIndex, uint32_t mepId)
{
  dot1agCfmMepObj_t    *pMepObj;
  dot1agCfmMepObjKey_t  mepObjKey;
  dot1agCfmMdTable_t   *mdEntry  = 0;
  dot1agCfmMepTable_t  *mepEntry = 0;
  dot1agMsgCmdData_t    msg;
  OFDPA_ERROR_t         rc       = OFDPA_E_FAIL;

  /* Validate input */
  if (mdIndex == 0 ||
      maIndex == 0 ||
      (mepId < OFDPA_DOT1AG_MEPID_MIN  || mepId > OFDPA_DOT1AG_MEPID_MAX))
  {
    return OFDPA_E_ERROR;
  }
  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mdEntry = dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE)) != 0)
  {
    memset(&mepObjKey, 0x0, sizeof(mepObjKey));
    mepObjKey.dot1agCfmMdIndex       = mdEntry->dot1agCfmMdIndex;
    mepObjKey.dot1agCfmMaIndex       = maIndex;
    mepObjKey.dot1agCfmMepIdentifier = mepId;
    mepObjKey.dot1agCfmMepIfIndex    = intIfNum;

    pMepObj = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjKey, AVL_EXACT);
    if ((pMepObj != 0) &&
        (pMepObj->mepData != NULL))
    {
      rc = OFDPA_E_NONE;
      mepEntry = pMepObj->mepData;
      mepEntry->active |= DOT1AG_ROW_INACTIVE_PENDING;
      msg.mdIndex  = mdEntry->dot1agCfmMdIndex;
      msg.maIndex  = maIndex;
      msg.mepIndex = mepId;
      msg.intIfNum = intIfNum;
      /* Post a message to queue so that mep is delete completely and
         activeState is set to DOT1AG_ROW_INACTIVE */
      if (dot1agIssueCmd(dot1agCfmMEPDelete, 0, &msg) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete MEP for mdIndex %d maIndex %d mepIndex %d vid %d intf %d",
                           mdEntry->dot1agCfmMdIndex, maIndex,
                           mepId, mepEntry->dot1agCfmMepPrimaryVid, intIfNum);
        mepEntry->active &= ~(DOT1AG_ROW_INACTIVE_PENDING);
        rc = OFDPA_E_FAIL;
      }
    }
  }/* End of active domain get */
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Verify if an entry with given values exists in LTR Table
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    ltrSeqNumber    @b{(input)} Index used to identify the
*                                       remote MEP which sent the LTR
* @param    ltrReceiveOrder @b{(input)} An index to distinguish
*                                       among multiple LTRs with the same
*                                       LTR Transaction Identifier field
*                                       value.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTREntryIsValid(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepId, uint32_t ltrSeqNumber,
                                       uint32_t ltrReceiveOrder)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmLtrObj_t *ltrObj;

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((ltrObj = dot1agCfmLtrObjGet(mdIndex, maIndex, mepId, ltrSeqNumber,
                                   ltrReceiveOrder, AVL_EXACT))
      != 0)
  {
    if (ltrObj->key.dot1agCfmLtrReceiveOrder != 0)
    {
      rc = OFDPA_E_NONE;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  Obtain Linktrace replies received by a specific MEP in
*           response to a linktrace message. As defined by section
*           17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
 *                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    ltrSeqNumber    @b{(input)} Index used to identify the
*                                       remote MEP which sent the LTR
* @param    ltrReceiveOrder @b{(input)} An index to distinguish
*                                       among multiple LTRs with the same
*                                       LTR Transaction Identifier field
*                                       value.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes  Use 0 to obtain first valid entry
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTREntryNextGet(uint32_t *mdIndex, uint32_t *maIndex,
                                       uint32_t *mepId, uint32_t *ltrSeqNumber,
                                       uint32_t *ltrReceiveOrder)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmLtrObj_t *ltrObj;

  /* Validate input */
  if (mdIndex == 0 || maIndex == 0 ||
      mepId == 0 || ltrSeqNumber == 0 ||
      ltrReceiveOrder == 0)
  {
    return OFDPA_E_ERROR;
  }

  DOT1AG_TRACE(CFM_LTR, "Retrieving next LTR values for "
                        "MD[%d] MA[%d] MEPID[%d] SEQ[%d] RCV[%d]", *mdIndex,
               *maIndex, *mepId, *ltrSeqNumber, *ltrReceiveOrder);

  if (ofdpaFeatureSupported(OFDPA_DOT1AG_LTR_SORT_FEATURE_ID) != OFDPA_E_NONE)
  {
    dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
    while ((ltrObj = dot1agCfmLtrObjGet(*mdIndex, *maIndex, *mepId, *ltrSeqNumber,
                                        *ltrReceiveOrder, AVL_NEXT))
           != 0)
    {
      *mdIndex = ltrObj->key.dot1agCfmMdIndex;
      *maIndex = ltrObj->key.dot1agCfmMaIndex;
      *mepId = ltrObj->key.dot1agCfmMepIdentifier;
      *ltrSeqNumber = ltrObj->key.dot1agCfmLtrSeqNumber;
      *ltrReceiveOrder = ltrObj->key.dot1agCfmLtrReceiveOrder;
      if (ltrObj->key.dot1agCfmLtrReceiveOrder == 0)
      {
        /* Skip this entry as the receive order of zero is used
         * for internal purpose only */
        continue;
      }
      else
      {
        rc = OFDPA_E_NONE;
        break;
      }
    }
    dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  }
  else /* LTR entries returned are in decrementing order of TTL values */
  {
    dot1agCfmLtrObj_t *ltrObj;
    dot1agCfmLtrObjKey_t array[DOT1AG_LTR_ENTRIES_MAX];
    dot1agCfmLtrObjKey_t arraySorted[DOT1AG_LTR_ENTRIES_MAX];
    uint32_t md, ma, mep, ltrSeqNo, ltrRx, i = 0, j, k = 1, l = 0;
    uint32_t dot1agCfmLtrTtl[DOT1AG_LTR_ENTRIES_MAX] = {0};
//    uint32_t dot1agCfmLtrTtlSorted[DOT1AG_LTR_ENTRIES_MAX] = {0}, temp;
    uint32_t temp;
    int entryCountStart = 0, entryCountEnd = 1;
    OFDPA_BOOL firstValidValue = OFDPA_FALSE, ltrEntriesPresent = OFDPA_FALSE;

    memset(array, 0, sizeof(array));
    memset(arraySorted, 0, sizeof(arraySorted));
    md = ma = mep = ltrSeqNo = ltrRx = 0;

    dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
    while ((ltrObj = dot1agCfmLtrObjGet(md, ma, mep, ltrSeqNo,
                                        ltrRx, AVL_NEXT)) != 0)
    {
      array[i].dot1agCfmMdIndex = md = ltrObj->key.dot1agCfmMdIndex;
      array[i].dot1agCfmMaIndex = ma = ltrObj->key.dot1agCfmMaIndex;
      array[i].dot1agCfmMepIdentifier = mep = ltrObj->key.dot1agCfmMepIdentifier;
      array[i].dot1agCfmLtrSeqNumber = ltrSeqNo = ltrObj->key.dot1agCfmLtrSeqNumber;
      array[i].dot1agCfmLtrReceiveOrder = ltrRx = ltrObj->key.dot1agCfmLtrReceiveOrder;
      dot1agCfmLtrTtl[i] = ltrObj->dot1agCfmLtrTtl;
      i++;
      ltrEntriesPresent = OFDPA_TRUE;
    }
    dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

    /* Return failure if LTR entries are not present */
    if (OFDPA_FALSE == ltrEntriesPresent)
      return OFDPA_E_FAIL;

    /* Sort values in array based on decrementing TTL vlaues
     * List starts with 'n' entries with TTL value 63, then
     * next 'n' entries with TTL value 62 and so on.
     */
    for (i = 0; i < DOT1AG_LTR_ENTRIES_MAX; i++)
    {
      if (0 == array[i].dot1agCfmLtrReceiveOrder)
        continue;

      if ((firstValidValue == OFDPA_FALSE) && (array[i].dot1agCfmLtrReceiveOrder != 0))
      {
        entryCountStart = i;
        k = array[i].dot1agCfmLtrSeqNumber;
        firstValidValue = OFDPA_TRUE;
      }

      if (OFDPA_DOT1AG_LTM_DEFAULT_TTL == dot1agCfmLtrTtl[i])
        continue;

      for (j = i + 1; j < DOT1AG_LTR_ENTRIES_MAX; j++)
      {
        if (dot1agCfmLtrTtl[i] < dot1agCfmLtrTtl[j])
        {
          dot1agCfmLtrObjKey_t tempDot1ag;
          temp = dot1agCfmLtrTtl[i];
          dot1agCfmLtrTtl[i] = dot1agCfmLtrTtl[j];
          dot1agCfmLtrTtl[j] = temp;
          memcpy(&tempDot1ag, &array[i], sizeof(tempDot1ag));
          memcpy(&array[i], &array[j], sizeof(array[i]));
          memcpy(&array[j], &tempDot1ag, sizeof(array[j]));
        }
      }
    }

    /* Track the start and end of the non-0 LTR values
     * to help sort the TTL entries list
     */
    for (i = 0; i < DOT1AG_LTR_ENTRIES_MAX; i++)
    {
      if (array[i].dot1agCfmLtrReceiveOrder != 0)
      {
        entryCountEnd = i;
      }
    }

    /* Sorting and storing LTR entries based on sequence numbers */
    for (i = entryCountStart; i <= entryCountEnd; i++)
    {
      for (j = entryCountStart; j <= entryCountEnd; j++)
      {
        if ((array[j].dot1agCfmLtrSeqNumber == k) && (array[j].dot1agCfmLtrReceiveOrder != 0))
        {
          memcpy(&arraySorted[l], &array[j], sizeof(arraySorted[l]));
//          dot1agCfmLtrTtlSorted[l++] = dot1agCfmLtrTtl[j];
        }
      }
      k++;
    }

    if (((*mdIndex == 0) &&
         (*maIndex == 0) &&
         (*mepId == 0) &&
         (*ltrReceiveOrder == 0)) ||
        (*ltrSeqNumber == 0))
    {
      *mdIndex = arraySorted[0].dot1agCfmMdIndex;
      *maIndex = arraySorted[0].dot1agCfmMaIndex;
      *mepId = arraySorted[0].dot1agCfmMepIdentifier;
      *ltrSeqNumber = arraySorted[0].dot1agCfmLtrSeqNumber;
      *ltrReceiveOrder = arraySorted[0].dot1agCfmLtrReceiveOrder;
      if (*ltrReceiveOrder != 0)
      {
        rc = OFDPA_E_NONE;
      }
    }
    else
    {
      for (i = 0; i < 1; i++)
      {
        if (*ltrSeqNumber != arraySorted[i].dot1agCfmLtrSeqNumber)
          continue;

        if ((*mdIndex == arraySorted[i].dot1agCfmMdIndex) &&
            (*maIndex == arraySorted[i].dot1agCfmMaIndex) &&
            (*mepId == arraySorted[i].dot1agCfmMepIdentifier))
        {
          if (*ltrReceiveOrder == 0)
          {
            /*Return the next value in the sorted list */
          }
          else
          {
            if (*ltrReceiveOrder != arraySorted[i].dot1agCfmLtrReceiveOrder)
              continue;

            i++;
            if (i >= l)
            {
              return rc;
            }
          }
          if ((*mdIndex == arraySorted[i].dot1agCfmMdIndex) &&
              (*maIndex == arraySorted[i].dot1agCfmMaIndex) &&
              (*mepId == arraySorted[i].dot1agCfmMepIdentifier) &&
              (*ltrSeqNumber == arraySorted[i].dot1agCfmLtrSeqNumber) &&
              (*ltrReceiveOrder == arraySorted[i].dot1agCfmLtrReceiveOrder))
          {
            return rc;
          }
          *mdIndex = arraySorted[i].dot1agCfmMdIndex;
          *maIndex = arraySorted[i].dot1agCfmMaIndex;
          *mepId = arraySorted[i].dot1agCfmMepIdentifier;
          *ltrSeqNumber = arraySorted[i].dot1agCfmLtrSeqNumber;
          *ltrReceiveOrder = arraySorted[i].dot1agCfmLtrReceiveOrder;
          rc = OFDPA_E_NONE;
        }
      }
    }
  }

  DOT1AG_TRACE(CFM_LTR, "Returning LTR values : "
                        "MD[%d] MA[%d] MEPID[%d] SEQ[%d] RCV[%d]", *mdIndex,
               *maIndex, *mepId, *ltrSeqNumber, *ltrReceiveOrder);
  return rc;
}

/*********************************************************************
* @purpose  Obtain Linktrace replies received by a specific MEP in
*           response to a linktrace message. As
*           defined by section 17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    ltrSeqNumber    @b{(input)} Index used to identify the
*                                       remote MEP which sent the LTR
* @param    ltrReceiveOrder @b{(input)} An index to distinguish
*                                       among multiple LTRs with the same
*                                       LTR Transaction Identifier field
*                                       value.
* @param    paramCode  @b{(input)} LTR parameter defined by
*                                  dot1agCfmLtrEntry_t
* @param    val       @b{(output)} Param value
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTRParamGet(uint32_t mdIndex, uint32_t maIndex,
                                   uint32_t mepId, uint32_t ltrSeqNumber,
                                   uint32_t ltrReceiveOrder, uint32_t paramCode,
                                   void *val)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmLtrObj_t *ltrObj;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((ltrObj = dot1agCfmLtrObjGet(mdIndex, maIndex, mepId, ltrSeqNumber,
                                   ltrReceiveOrder, AVL_EXACT))
      != 0)
  {
    if (ltrObj->key.dot1agCfmLtrReceiveOrder == 0) /* ltrReceiveOrder is used for internal table
                                                      management */
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_FAIL;
    }
    rc = OFDPA_E_NONE;
    switch (paramCode)
    {
      case LTR_PARAM_dot1agCfmLtrTtl:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrTtl;
        break;
      case LTR_PARAM_dot1agCfmLtrForwarded:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrForwarded;
        break;
      case LTR_PARAM_dot1agCfmLtrTerminalMep:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrTerminalMep;
        break;
      case LTR_PARAM_dot1agCfmLtrLastEgressIdentifier:
        memcpy(val, ltrObj->dot1agCfmLtrLastEgressIdentifier, DOT1AG_EGRESS_ID_LEN);
        break;
      case LTR_PARAM_dot1agCfmLtrNextEgressIdentifier:
        memcpy(val, ltrObj->dot1agCfmLtrNextEgressIdentifier, DOT1AG_EGRESS_ID_LEN);
        break;
      case LTR_PARAM_dot1agCfmLtrRelay:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrRelay;
        break;
      case LTR_PARAM_dot1agCfmLtrChassisIdLen:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrChassisIdLen;
        break;
      case LTR_PARAM_dot1agCfmLtrChassisIdSubtype:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrChassisIdSubtype;
        break;
      case LTR_PARAM_dot1agCfmLtrChassisId:
        memcpy(val, ltrObj->dot1agCfmLtrChassisId, ltrObj->dot1agCfmLtrChassisIdLen);
        break;
      case LTR_PARAM_dot1agCfmLtrManAddressDomainLen:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrManAddressDomainLen;
        break;
      case LTR_PARAM_dot1agCfmLtrManAddressDomain:
        memcpy(val, ltrObj->dot1agCfmLtrManAddressDomain, ltrObj->dot1agCfmLtrManAddressDomainLen);
        break;
      case LTR_PARAM_dot1agCfmLtrManAddressLen:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrManAddressLen;
        break;
      case LTR_PARAM_dot1agCfmLtrManAddress:
        memcpy(val, ltrObj->dot1agCfmLtrManAddress, ltrObj->dot1agCfmLtrManAddressLen);
        break;
      case LTR_PARAM_dot1agCfmLtrIngress:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrIngress;
        break;
      case LTR_PARAM_dot1agCfmLtrIngressMac:
        memcpy(val, ltrObj->dot1agCfmLtrIngressMac, OFDPA_MAC_ADDR_LEN);
        break;
      case LTR_PARAM_dot1agCfmLtrIngressPortIdLen:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrIngressPortIdLen;
        break;
      case LTR_PARAM_dot1agCfmLtrIngressPortIdSubtype:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrIngressPortIdSubtype;
        break;
      case LTR_PARAM_dot1agCfmLtrIngressPortId:
        memcpy(val, ltrObj->dot1agCfmLtrIngressPortId, ltrObj->dot1agCfmLtrIngressPortIdLen);
        break;
      case LTR_PARAM_dot1agCfmLtrEgress:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrEgress;
        break;
      case LTR_PARAM_dot1agCfmLtrEgressMac:
        memcpy(val, ltrObj->dot1agCfmLtrEgressMac, OFDPA_MAC_ADDR_LEN);
        break;
      case LTR_PARAM_dot1agCfmLtrEgressPortIdLen:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrEgressPortIdLen;
        break;
      case LTR_PARAM_dot1agCfmLtrEgressPortIdSubtype:
        *((uint32_t *)val) = ltrObj->dot1agCfmLtrEgressPortIdSubtype;
        break;
      case LTR_PARAM_dot1agCfmLtrEgressPortId:
        memcpy(val, ltrObj->dot1agCfmLtrEgressPortId, ltrObj->dot1agCfmLtrEgressPortIdLen);
        break;
      case LTR_PARAM_dot1agCfmLtrOrganizationSpecificTlv:
        memcpy(val, ltrObj->dot1agCfmLtrOrganizationSpecificTlv, DOT1AG_LTR_ORG_TLV_LEN);
        break;
      default:
        rc = OFDPA_E_FAIL;
        break;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  Verify if an entry with given values exists in RMEP Table
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    rmepId     @b{(input)} Index used to identify the
*                                       remote MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRMepEntryIsValid(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t rmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if (dot1agCfmRMepObjGet(mdIndex, maIndex, mepId, rmepId,
                          AVL_EXACT) != 0)
  {
    rc = OFDPA_E_NONE;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  Obtain RMep entries collected by a specific MEP.
*           As defined by section 17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    rmepId     @b{(input)} Index used to identify the
*                                       remote MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes Use 0 to obtain next valid entry
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRMepEntryNextGet(uint32_t *mdIndex, uint32_t *maIndex,
                                        uint32_t *mepId, uint32_t *rmepId)
{
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;
  dot1agCfmRMepObj_t *rmepObj;

  /* Validate input */
  if (mdIndex == 0 || maIndex == 0 ||
      mepId == 0 || rmepId == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((rmepObj = dot1agCfmRMepObjGet(*mdIndex, *maIndex, *mepId, *rmepId,
                                     AVL_NEXT)) != 0)
  {
    rc = OFDPA_E_NONE;
    *mdIndex = rmepObj->key.dot1agCfmMdIndex;
    *maIndex = rmepObj->key.dot1agCfmMaIndex;
    *mepId   = rmepObj->key.dot1agCfmMepIdentifier;
    *rmepId  = rmepObj->key.dot1agCfmMepDbRMepIdentifier;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain RMep entries collected by a specific MEP.
*           As defined by section 17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    rmepId     @b{(input)} Index used to identify the
*                                       remote MEP
* @param    paramCode  @b{(input)} RMep parameter defined by
*                                  dot1agCfmMepDbEntry_t
* @param    val       @b{(output)} Param value
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRMepParamGet(uint32_t mdIndex, uint32_t maIndex,
                                    uint32_t mepId, uint32_t rmepId,
                                    uint32_t paramCode, void *val)
{
  OFDPA_ERROR_t       rc = OFDPA_E_FAIL;
  dot1agCfmRMepObj_t *rmepObj;
  uint32_t            time;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((rmepObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId, rmepId,
                                     AVL_EXACT)) != 0)
  {
    rc = OFDPA_E_NONE;
    switch (paramCode)
    {
      case RMEP_PARAM_dot1agCfmMepDbRMepState:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = rmepObj->dot1agCfmMepDbRMepState;
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbRMepFailedOkTime:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = rmepObj->dot1agCfmMepDbRMepState;
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbMacAddress:
        memcpy(val, rmepObj->dot1agCfmRmepVar.rMepMacAddress, OFDPA_MAC_ADDR_LEN);
        break;
      case RMEP_PARAM_dot1agCfmMepDbRdi:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = rmepObj->dot1agCfmMepDbRdi;
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbPortStatusTlv:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = rmepObj->dot1agCfmMepDbPortStatusTlv;
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbInterfaceStatusTlv:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = rmepObj->dot1agCfmMepDbInterfaceStatusTlv;
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbChassisIdLen:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = rmepObj->dot1agCfmMepDbChassisIdLen;
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbChassisIdSubtype:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = rmepObj->dot1agCfmMepDbChassisIdSubtype;
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbChassisId:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          memcpy(val, rmepObj->dot1agCfmMepDbChassisId, rmepObj->dot1agCfmMepDbChassisIdLen);
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbManAddressDomainLen:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = rmepObj->dot1agCfmMepDbManAddressDomainLen;
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbManAddressDomain:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          memcpy(val, rmepObj->dot1agCfmMepDbManAddressDomain, rmepObj->dot1agCfmMepDbManAddressDomainLen);
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbManAddressLen:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          *((uint32_t *)val) = rmepObj->dot1agCfmMepDbManAddressLen;
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbManAddress:
        if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
        {
          rc = OFDPA_E_UNAVAIL;
        }
        else
        {
          memcpy(val, rmepObj->dot1agCfmMepDbManAddress, rmepObj->dot1agCfmMepDbManAddressLen);
        }
        break;
      case RMEP_PARAM_dot1agCfmMepDbCcmExpiryTime:
        time = 0;
#if OFDPA_NOT_SUPPORTED
        if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_DOT1AG_MPLS_TP_CCM_SUPPORT))
        {
          if (OFDPA_TRUE == dot1agIsMepHwEnabled(mepId))
          {
            if (rmepObj->dot1agCfmRmepVar.rmepTimer != 0)
            {
              if (appTimerTimeLeftGet(dot1agCFM.timerCB,
                                      rmepObj->dot1agCfmRmepVar.rmepTimer, &time) != OFDPA_E_NONE)
              {
                time = 0;
              }
            }
          }
          else
          {
            if (rmepObj->dot1agCfmRmepState != DOT1AG_RMEP_FAILED && rmepObj->dot1agCfmRmepVar.rmepTimer != 0)
            {
              if (appTimerTimeLeftGet(dot1agCFM.timerCB,
                                      rmepObj->dot1agCfmRmepVar.rmepTimer, &time) != OFDPA_E_NONE)
              {
                time = 0;
              }
            }
          }
        }
#endif
        memcpy(val, &time, sizeof(time));
        break;
      default:
        rc = OFDPA_E_FAIL;
        break;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Verify if an entry with given values exists in CFM Stack
*
* @param    intIfNum   @b{(input)} Interface on which MP is configured
* @param    primVID    @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel    @b{(input)} Domain level
* @param    direction  @b{(output)} Direction of MP (Up or Down)
*                                   For MIP it is always Up
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmStackEntryIsValid(uint32_t intIfNum, uint32_t primVID,
                                         int mdLevel,
                                         Dot1agCfmMpDirection_t direction)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if (dot1agCfmStackObjGet(intIfNum, primVID, mdLevel, direction,
                           AVL_EXACT) != 0)
  {
    rc = OFDPA_E_NONE;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain CFM MP Stack entries configured
*           As defined by section 17.5 802.1ag d8-1
*
*
* @param    intIfNum   @b{(input)} Interface on which MP is configured
* @param    primVID    @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel    @b{(input)} Domain level to be set
* @param    direction  @b{(output)} Direction of MP (Up or Down)
*                                   For MIP it is always Up
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes Use 0 to obtain next valid entry
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmStackEntryNextGet(uint32_t *intIfNum, uint32_t *primVID,
                                         int *mdLevel,
                                         Dot1agCfmMpDirection_t *direction)
{
  OFDPA_ERROR_t        rc = OFDPA_E_FAIL;
  dot1agCfmStackObj_t *mpObj;

  /* Validate input */
  if (intIfNum == 0 || primVID == 0 ||
      mdLevel == 0 || direction == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((mpObj = dot1agCfmStackObjGet(*intIfNum, *primVID, *mdLevel, *direction,
                                    AVL_NEXT)) != 0)
  {
    rc = OFDPA_E_NONE;
    *intIfNum   = mpObj->key.dot1agCfmStackifIndex;
    *primVID    = mpObj->key.dot1agCfmStackVlanIdOrNone;
    *mdLevel    = mpObj->key.dot1agCfmStackMdLevel;
    *direction  = mpObj->key.dot1agCfmStackDirection;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain CFM MP Stack entries configured
*           As defined by section 17.5 802.1ag d8-1
*
* @param    intIfNum   @b{(input)} Interface on which MP is configured
* @param    primVID    @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel    @b{(input)} Domain level to be set
* @param    direction  @b{(output)} Direction of MP (Up or Down)
* @param    paramCode  @b{(input)} CFM Stack MP parameter defined by
*                                  dot1agCfmStackEntry_t
* @param    val        @b{(output)} Param value
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_NOT_FOUND
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmStackParamGet(uint32_t intIfNum, uint32_t primVID,
                                     int mdLevel, Dot1agCfmMpDirection_t direction,
                                     uint32_t paramCode, void *val)
{
  OFDPA_ERROR_t            rc             = OFDPA_E_FAIL;
  dot1agCfmStackObj_t     *mpObj;
  dot1agCfmMdTable_t      *mdEntry        = 0;
  dot1agCfmMAObjPrimVID_t *maPrimVIDEntry = 0;

  /* Validate input */
  if (val == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  mpObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel, direction, AVL_EXACT);
  if (NULL != mpObj)
  {
    rc = OFDPA_E_NONE;
    switch (paramCode)
    {
      case CFM_MP_PARAM_dot1agCfmStackMdIndex:
        if (mpObj->mepData != 0)
        {
          *((uint32_t *)val) = mpObj->mepData->maData->mdData->dot1agCfmMdIndex;
        }
        else
        { /* MIP */
          mdEntry = dot1agActiveMdGet(0, 0, mdLevel, DOT1AG_ROW_ACTIVE);
          if (NULL != mdEntry)
          {
            *((uint32_t *)val) = mdEntry->dot1agCfmMdIndex;
          }
          else
          {
            rc = OFDPA_E_FAIL;
          }
        }
        break;
      case CFM_MP_PARAM_dot1agCfmStackMaIndex:
        if (mpObj->mepData != 0)
        {
          *((uint32_t *)val) = mpObj->mepData->maData->dot1agCfmMaIndex;
        }
        else
        {                               /* MIP */
          rc = OFDPA_E_FAIL;
          mdEntry = dot1agActiveMdGet(0, 0, mdLevel, DOT1AG_ROW_ACTIVE);
          if (NULL != mdEntry)
          {
            maPrimVIDEntry = (dot1agCfmMAObjPrimVID_t *)dot1agActivePrimVIDMAGet(mdEntry->dot1agCfmMdIndex,
                                                                                 primVID, AVL_EXACT);
            if (maPrimVIDEntry != 0)
            {
              *((uint32_t *)val) = maPrimVIDEntry->maData->dot1agCfmMaIndex;
              rc = OFDPA_E_NONE;
            }
          }/* End of Get Active MD */
        }
        break;
      case CFM_MP_PARAM_dot1agCfmStackMepId:
        if (mpObj->mepData != 0)
        {
          *((uint32_t *)val) = mpObj->mepData->dot1agCfmMepIdentifier;
        }
        else
        {
          *((uint32_t *)val) = 0;
        }
        break;
      case CFM_MP_PARAM_dot1agCfmStackMacAddress:
        memcpy(val, mpObj->mpMacAddress, sizeof(mpObj->mpMacAddress));
        break;
      case CFM_MP_PARAM_dot1agCfmStackMepOperMode:
        if (mpObj->mepData != 0)
        {
          *((uint32_t *)val) = mpObj->operMode;
        }
        else
        {
          *((uint32_t *)val) = 0;
        }
        break;
      case CFM_MP_PARAM_dot1agCfmStackMipIdTlvData:
        if (NULL == mpObj->mepData)     /* Only valid for MIP */
        {
          memcpy(val, mpObj->mipIdTlvData, sizeof(mpObj->mipIdTlvData));
        }
        else
        {
          rc = OFDPA_E_FAIL;
        }
        break;
      default:
        rc = OFDPA_E_FAIL;
        break;
    }
  }
  else
  {
    /* Entry with given values is not present in CFM Stack */
    rc = OFDPA_E_NOT_FOUND;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Create a maintenance domain intermediate point
*
* @param    intIfNum     @b{(input)} Interface on which mep is to be
*                                    created/deleted
* @param    primVID      @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel      @b{(input)} MD level of the MD for which MIP
*                                    is to be created/deleted.
* @param    direction    @b{(input)} Direction of MP (Up or Down)
* @param    macAddr      @b{(input)} MAC address for Ethernet MIP
* @param    mipIdTlvData @b{(input)} MIP ID TLV for BHH
* @param    localMpId    @b{(input)} OF-DPA MP ID
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_REQUEST_DENIED
*
* @notes    Refer to 802.1ag d8-1 section 22.2.3  -- not following this
*           because in OF-DPA, non-Ethernet MIPs cannot be created automatically.
*           This API will cause a local MIP to be created as a result of
*           a configuration action.
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipCreate(uint32_t intIfNum, uint32_t primVID, int mdLevel,
                              Dot1agCfmMpDirection_t direction,
                              uint8_t *macAddr, uint8_t *mipIdTlvData, uint32_t localMpId)
{
  dot1agCfmStackObj_t *pMPObj = NULL;
  OFDPA_ERROR_t rc;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  pMPObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel, direction, AVL_EXACT);
  if (NULL != pMPObj)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "MP already exists for this intIfNum (%d), primVID (%d), mdLevel (%d) and direction (%d).\r\n",
                       intIfNum, primVID, mdLevel, direction);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    rc = dot1agCfmMPInit(intIfNum, primVID, mdLevel, direction, macAddr, NULL, localMpId);
    if (OFDPA_E_NONE == rc)
    {
      pMPObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel, direction, AVL_EXACT);
      if (NULL == pMPObj)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                           "MP creation failed for this intIfNum (%d), primVID (%d), mdLevel (%d) and direction (%d).\r\n",
                           intIfNum, primVID, mdLevel, direction);
        rc = OFDPA_E_FAIL;
      }
      else
      {
        memcpy(pMPObj->mipIdTlvData, mipIdTlvData, sizeof(pMPObj->mipIdTlvData));
      }
    }
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  Delete a maintenance domain intermediate point
*
* @param    intIfNum     @b{(input)} Interface on which mep is to be
*                                    created/deleted
* @param    primVID      @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel      @b{(input)} MD level of the MD for which MIP
*                                    is to be created/deleted.
* @param    direction    @b{(input)} Direction of MP (Up or Down)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_REQUEST_DENIED
*
* @notes    Refer to 802.1ag d8-1 section 22.2.3  -- not following this
*           because in OF-DPA, non-Ethernet MIPs cannot be created automatically.
*           This API will cause a local MIP to be deleted as a result of
*           a configuration action.
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipDelete(uint32_t intIfNum, uint32_t primVID, int mdLevel,
                              Dot1agCfmMpDirection_t direction)
{
  dot1agCfmStackObj_t *pMPObj = NULL;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  pMPObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel, direction, AVL_EXACT);
  if (NULL != pMPObj)
  {
    dot1agCfmMPDestroy(pMPObj);
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Enable/Disable a maintenance intermediate point on an
*           interface for a given domain level
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created/deleted
* @param    mdLevel    @b{(input)} MD level of the MD for which MIP
*                                  is to be created/deleted.
* @param    macAddr    @b{(input)} MAC address for MIP
* @param    mode       @b{(input)} OFDPA_ENABLE /OFDPA_DISABLE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Refer to 802.1ag d8-1 section 22.2.3
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipModeSet(uint32_t intIfNum, int mdLevel,
                               uint8_t *macAddr, uint32_t mode)
{
  OFDPA_ERROR_t       rc       = OFDPA_E_NONE;
  uint32_t            prevMode = OFDPA_DISABLE;
  uint32_t            event;
  dot1agMsgCmdData_t  msg;
  dot1agCfmMdTable_t *mdEntry;
  uint32_t            lvl;

  /* Validate input */
  if (mdLevel < OFDPA_DOT1AG_MD_LEVEL_MIN ||
      mdLevel > OFDPA_DOT1AG_MD_LEVEL_MAX ||
      (mode != OFDPA_ENABLE  && mode != OFDPA_DISABLE))
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mdEntry = dot1agActiveMdGet(0, 0, mdLevel, DOT1AG_ROW_ACTIVE)) != 0)
  {
    /* Check if any MIP already exists, disable it if it is configured */
    if (mode == OFDPA_ENABLE )
    {
      for (lvl = OFDPA_DOT1AG_MD_LEVEL_MIN; lvl <= OFDPA_DOT1AG_MD_LEVEL_MAX; lvl++)
      {
        if (OFDPA_INTF_ISMASKBITSET(DOT1AG_MIP_TABLE(lvl).mipMode, intIfNum))
        {
          if (mdLevel != lvl)
          {
            event = dot1agCfmMIPDelete;
            OFDPA_INTF_CLRMASKBIT(DOT1AG_MIP_TABLE(lvl).mipMode, intIfNum);
            /* Post a message to queue so that mip delete event is passed to state machines */
            memset(&msg, 0x0, sizeof(msg));
            msg.mdIndex   = mdEntry->dot1agCfmMdIndex;
            msg.intIfNum  = intIfNum;
            msg.uintVal   = lvl;
            if (dot1agIssueCmd(event, 0, &msg) != OFDPA_E_NONE)
            {
              rc = OFDPA_E_FAIL;
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to issue delete MIP command for mdIndex %d intf %d mode %d",
                                 mdEntry->dot1agCfmMdIndex, intIfNum, mode);
            }
          }
          break;
        }
      }/* Loop through all mdLevels */
    }

    prevMode = OFDPA_INTF_ISMASKBITSET(DOT1AG_MIP_TABLE(mdLevel).mipMode, intIfNum) ? OFDPA_ENABLE  : OFDPA_DISABLE;
    if (prevMode != mode)
    {
      if (mode == OFDPA_ENABLE)
      {
        event = dot1agCfmMIPCreate;
        OFDPA_INTF_SETMASKBIT(DOT1AG_MIP_TABLE(mdLevel).mipMode, intIfNum);
      }
      else
      {
        event = dot1agCfmMIPDelete;
        OFDPA_INTF_CLRMASKBIT(DOT1AG_MIP_TABLE(mdLevel).mipMode, intIfNum);
      }

      /* Post a message to queue so that mip create event is passed to state machines */
      memset(&msg, 0x0, sizeof(msg));
      msg.mdIndex   = mdEntry->dot1agCfmMdIndex;
      msg.intIfNum  = intIfNum;
      msg.uintVal   = mdLevel;
      memcpy(msg.srcMac, macAddr, sizeof(msg.srcMac));

      if (dot1agIssueCmd(event, 0, &msg) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to issue MIP mode set. mdIndex %d intf %d mode %d",
                           mdEntry->dot1agCfmMdIndex, intIfNum, mode);
        rc = OFDPA_E_FAIL;
      }
    }
  }
  else
  {
    rc = OFDPA_E_FAIL;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Enable/Disable a maintenance intermediate point on an
*           interface for a given domain level
*
* @param    intIfNum   @b{(input)} Interface on which mip is to be
*                                  created/deleted
* @param    mdLevel    @b{(input)} MD level of the MD for which MEP
*                                  is to be created/deleted.
* @param    mode       @b{(output)} OFDPA_ENABLE /OFDPA_DISABLE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Refer to 802.1ag d8-1 section 22.2.3
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipModeGet(uint32_t intIfNum, int mdLevel,
                               uint32_t *mode)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  /* Validate input */
  if (mdLevel < OFDPA_DOT1AG_MD_LEVEL_MIN || mdLevel > OFDPA_DOT1AG_MD_LEVEL_MAX ||
      mode == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if (dot1agActiveMdGet(0, 0, mdLevel,
                        DOT1AG_ROW_ACTIVE) != 0)
  {
    *mode = OFDPA_INTF_ISMASKBITSET(DOT1AG_MIP_TABLE(mdLevel).mipMode, intIfNum)
      ? OFDPA_ENABLE  : OFDPA_DISABLE;
  }
  else
  {
    rc = OFDPA_E_FAIL;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Enable/Disable CFM
*
* @param    mode       @b{(input)} OFDPA_ENABLE /OFDPA_DISABLE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Not standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmModeSet(uint32_t mode)
{
  OFDPA_ERROR_t      rc = OFDPA_E_NONE;
  uint32_t           event;
  dot1agMsgCmdData_t msg;

  /* Validate input */
  if (mode != OFDPA_ENABLE  && mode != OFDPA_DISABLE)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if (DOT1AG_CFG->dot1agCfmAdminMode != mode)
  {
    if (mode == OFDPA_ENABLE )
    {
      event = dot1agCfmCreate;
    }
    else
    {
      event = dot1agCfmDelete;
    }
    DOT1AG_CFG->dot1agCfmAdminMode = mode;

    /* Post a message to queue so that CFM create event is processed and
       corresponding MPs are created and SMs are started */
    memset(&msg, 0x0, sizeof(msg));
    if (dot1agIssueCmd(event, 0, &msg) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to set CFM admin mode %d",
                         mode);
      rc = OFDPA_E_FAIL;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Enable/Disable mode of CFM
*
* @param    mode       @b{(output)} OFDPA_ENABLE /OFDPA_DISABLE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmModeGet(uint32_t *mode)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  /* Validate input */
  if (mode == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  *mode = DOT1AG_CFG->dot1agCfmAdminMode;
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Obtain archive hold time. The time interval after which
*           RMEPs in RMEP_FAILED state are removed
*
* @param    timerVal       @b{(output)}
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmArchiveHoldTimeGet(uint32_t *timerVal)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  /* Validate input */
  if (timerVal == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  *timerVal = DOT1AG_CFG->dot1agCfmArchiveHoldTime;
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set archive hold time. The time interval after which
*           RMEPs in RMEP_FAILED state are removed
*
* @param    timerVal       @b{(input)}
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmArchiveHoldTimeSet(uint32_t timerVal)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  /* Validate input */
  if (timerVal < OFDPA_DOT1AG_CCM_ARCHIVE_HOLD_TIME_MIN ||
      timerVal > OFDPA_DOT1AG_CCM_ARCHIVE_HOLD_TIME_MAX)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  DOT1AG_CFG->dot1agCfmArchiveHoldTime = timerVal;
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  get one entry for Pro LM
*
* @param    mepId      @b{(input)} Index used to identify a MEP.
*
*
* @returns  Prolm Entry
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepLmPriBitmapGet(uint32_t intIfNum, uint32_t mdIndex,
                                      uint32_t maIndex, uint32_t mepId, uint32_t *lmPriBitmap)
{
  OFDPA_ERROR_t      rc = OFDPA_E_NONE;
  dot1agCfmMepObj_t *mepObj;

  *lmPriBitmap = 0;
  if ((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0)
  {
    if ((mepObj->mepData != NULL) &&
        (mepObj->mepData->y1731CfmMepProlmEntry != NULL) &&
        (mepObj->mepData->y1731CfmMepProlmEntry->prolmEnabled))
    {
      *lmPriBitmap |= (1 << mepObj->mepData->dot1agCfmMepCcmLtmPriority);
    }
    /* TODO: for Ondem LM */
  }
  return rc;
}

/*********************************************************************
* @purpose  get one entry for Pro LM
*
* @param    mepId      @b{(input)} Index used to identify a MEP.
*
*
* @returns  Prolm Entry
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
y1731CfmMepProlmTable_t *dot1agMepProlmEntryGet(uint32_t intIfNum, uint32_t mdIndex,
                                                uint32_t maIndex, uint32_t mepId)
{
  dot1agCfmMepObj_t       *mepObj;
  uint32_t                 idx;
  y1731CfmMepProlmTable_t *mepProlmEntry = 0;

  if (((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0) && 
      (mepObj->mepData != NULL))
  {
    idx = mepObj->mepData - &(DOT1AG_MEP_TABLE(0));
    mepProlmEntry = &y1731CfmMepProlmTable[idx];
  }
  return mepProlmEntry;
}

/*********************************************************************
* @purpose  Create a PROLM on one MEP
*
* @param    intIfNum       @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex        @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex        @b{(input)} Index for MA identification used by
*                                  SNMP. Use 0 for other UI's
* @param    mepId          @b{(input)} Index to identify a MEP
* @param    destMac        @b{(input)} MAC address of MP to which LM is
*                                      measured
* @param    destMepid      @b{(input)} Destination MEPID to which LM is
*                                      measured
* @param    priorityClass  @b{(input)} Priority class (CoS) of the traffic that CCM-based LM tool should monitor
* @param    toolType       @b{(input)} Tool Type to specify CCM, LMM or SLM based LM
* @param    testId         @b{(input)} TestId to be used for SLM based LM.
*                                      can be set to 0 for non-SLM based LM
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepProlmCreate(uint32_t intIfNum, uint32_t mdIndex,
                                   uint32_t maIndex, uint32_t mepId,
                                   uint8_t *destMac, uint32_t destMepId,
                                   uint32_t priorityClass, uint8_t toolType,
                                   uint32_t testId, uint32_t controlPeriod)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;
  OFDPA_BOOL         lmDestIsMepId = OFDPA_TRUE;

  /* Validate input */
  if (priorityClass > OFDPA_DOT1P_MAX_PRIORITY)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if (((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0) &&
      (mepObj->mepData != NULL))
  {
    rc = OFDPA_E_NONE;
    mepObj->mepData->y1731CfmMepProlmEntry = 0;
    mepObj->mepData->y1731CfmMepProlmEntry = dot1agMepProlmEntryGet(intIfNum, mdIndex, maIndex, mepId);
    if (mepObj->mepData->y1731CfmMepProlmEntry == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_ERROR;
    }
    memset(mepObj->mepData->y1731CfmMepProlmEntry, 0x0, sizeof(*mepObj->mepData->y1731CfmMepProlmEntry));

    if (destMac != 0)
    {
      memcpy(mepObj->mepData->y1731CfmMepProlmEntry->prolmDestMacAddress, destMac, OFDPA_MAC_ADDR_LEN);
      lmDestIsMepId = OFDPA_FALSE;
    }
    mepObj->mepData->y1731CfmMepProlmEntry->prolmDestIsMepId = lmDestIsMepId;
    mepObj->mepData->y1731CfmMepProlmEntry->prolmDestMepId = destMepId;
    mepObj->mepData->y1731CfmMepProlmEntry->prolmPriorityClass = priorityClass;
    mepObj->mepData->y1731CfmMepProlmEntry->prolmSlmTestId = testId;
    mepObj->mepData->y1731CfmMepProlmEntry->prolmControlPeriod = controlPeriod;
    mepObj->mepData->y1731CfmMepProlmVar.prolmToolType = toolType;
    mepObj->mepData->y1731CfmMepProlmVar.active = DOT1AG_ROW_ACTIVE;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Delete PROLM on one MEP
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP. Use 0 for other UI's
* @param    mepId      @b{(input)} Index to identify a MEP
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepProlmDelete(uint32_t intIfNum, uint32_t mdIndex,
                                   uint32_t maIndex, uint32_t mepId)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmMepObj_t *mepObj;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if (((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0) &&
      (mepObj->mepData != NULL))
  {
    rc = OFDPA_E_NONE;

    mepObj->mepData->y1731CfmMepProlmEntry = 0;
    mepObj->mepData->y1731CfmMepProlmVar.active = DOT1AG_ROW_INACTIVE;
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Set PROLM flag of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)}  CCI enabled flag of MEP (OFDPA_TRUE or
*                                    OFDPA_FALSE)
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepProlmEnabledSet(uint32_t intIfNum, uint32_t mdIndex,
                                       uint32_t maIndex, uint32_t mepId,
                                       OFDPA_BOOL mode)
{
  OFDPA_ERROR_t        rc     = OFDPA_E_FAIL;
  dot1agCfmMepObj_t   *mepObj;
  dot1agCfmStackObj_t *pMPObj = 0;
  dot1agMsgCmdData_t   msg;
  dot1agCfmSlmObj_t   *pSlmEntry = NULL, slmEntry;

  /* Validate input */
  if (mode != OFDPA_TRUE && mode != OFDPA_FALSE)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if (((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0) &&
      (mepObj->mepData != NULL))
  {
    rc = OFDPA_E_NONE;
    if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                       mepObj->mepData->dot1agCfmMepPrimaryVid,
                                       mepObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                       mepObj->mepData->dot1agCfmMepDirection, AVL_EXACT)) == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "fail to get CfmStackObj. mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_ERROR;
    }

    if (mepObj->mepData->y1731CfmMepProlmEntry == 0)
    {
      rc = OFDPA_E_FAIL;
    }
    else if (mode != mepObj->mepData->y1731CfmMepProlmVar.prolmEnabled)
    {
      mepObj->mepData->y1731CfmMepProlmEntry->prolmBinEnable = 0;
      if (mode == OFDPA_TRUE)
      {
        mepObj->mepData->y1731CfmMepProlmVar.prolmEnabled = mode;
        mepObj->mepData->y1731CfmMepProlmEntry->prolmEnabled = mode;
        memset(mepObj->mepData->y1731CfmMepProlmEntry->prolmCurBin, 0,
               sizeof(mepObj->mepData->y1731CfmMepProlmEntry->prolmCurBin));
        memset(mepObj->mepData->y1731CfmMepProlmEntry->prolmHistoryBin, 0,
               sizeof(mepObj->mepData->y1731CfmMepProlmEntry->prolmHistoryBin));
        memset(mepObj->mepData->y1731CfmMepProlmEntry->prolmHistoryBinTail, 0,
               sizeof(mepObj->mepData->y1731CfmMepProlmEntry->prolmHistoryBinTail));
        mepObj->mepData->y1731CfmMepProlmEntry->prolmBinEnable = (Y1731_BIN_FLAG_15M | Y1731_BIN_FLAG_24H);
        mepObj->mepData->y1731CfmMepProlmEntry->saved = OFDPA_FALSE;

        if (mepObj->mepData->y1731CfmMepProlmVar.prolmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM)
        {
          /* Creates a new entry in the SLM tree. */
          memset(&slmEntry, 0x0, sizeof(slmEntry));

          slmEntry.key.dot1agCfmLmepIdentifier  = mepObj->mepData->dot1agCfmMepIdentifier;
          slmEntry.key.dot1agCfmRmepIdentifier  = mepObj->mepData->y1731CfmMepProlmEntry->prolmDestMepId;
          slmEntry.key.dot1agCfmTestId          = mepObj->mepData->y1731CfmMepProlmEntry->prolmSlmTestId;

          if (dot1agSlmEntryInsert(&slmEntry) != OFDPA_E_NONE)
          {
            dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
            return OFDPA_E_FAIL;
          }
        }
      }
      else
      {
        if (mepObj->mepData->y1731CfmMepProlmVar.prolmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM)
        {
          /* Deletes entry from the SLM tree. */
          memset(&slmEntry, 0x0, sizeof(slmEntry));

          slmEntry.key.dot1agCfmLmepIdentifier  = mepObj->mepData->dot1agCfmMepIdentifier;
          slmEntry.key.dot1agCfmRmepIdentifier  = mepObj->mepData->y1731CfmMepProlmEntry->prolmDestMepId;
          slmEntry.key.dot1agCfmTestId          = mepObj->mepData->y1731CfmMepProlmEntry->prolmSlmTestId;

          pSlmEntry = dot1agCfmSlmObjGet(slmEntry.key.dot1agCfmLmepIdentifier, slmEntry.key.dot1agCfmRmepIdentifier,
                                         slmEntry.key.dot1agCfmTestId, AVL_EXACT);
          if (NULL != pSlmEntry)
          {
            if (dot1agSlmEntryDelete(pSlmEntry) != OFDPA_E_NONE)
            {
              dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
              return OFDPA_E_FAIL;
            }
          }
          else
          {
            dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
            return OFDPA_E_FAIL;
          }
        }
      }
      mepObj->mepData->y1731CfmMepProlmEntry->pMPObj = pMPObj;

      /* Post a message to queue so that mep create event is passed to state machines */
      msg.mdIndex  = mdIndex;
      msg.maIndex  = maIndex;
      msg.mepIndex = mepId;
      msg.intIfNum = intIfNum;
      msg.uintVal  = mode;
      if (dot1agIssueCmd(dot1agCfmMEPProlmEnabled, 0, &msg) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to set ProlmEnable. mdIndex %d maIndex %d mepIndex %d intf %d mode %d",
                           mdIndex, maIndex, mepId, intIfNum, mode);
        rc = OFDPA_E_FAIL;
      }
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

  
/*********************************************************************
* @purpose  Get CCM database entry corresponding to mepId and remote mepId 
*
* @param    mdIndex     @b{(input)} Index used for MD indentification
*                                   used by SNMP.
* @param    maIndex     @b{(input)} Index for MA identification used by SNMP.
* @param    mepId       @b{(input)} Index to identify a MEP
* @param    remoteMepId @b{(input)} Index of corresponding rmep entry
* @param    dbEntry     @b{(output)}Structure to return entry details
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_NOT_FOUND
* @returns  OFDPA_E_INTERNAL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agOamMepCCMDatabaseEntryGet(uint32_t mdIndex, uint32_t maIndex, 
                                              uint32_t mepId, uint32_t remoteMepId,
                                              ofdpaOamCcmDatabaseEntry_t *dbEntry)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NOT_FOUND;
  dot1agCfmRMepObj_t      *rmepObj;
  dot1agCfmRMepObjKey_t   rmepObjKey;

  rmepObjKey.dot1agCfmMdIndex             = mdIndex;
  rmepObjKey.dot1agCfmMaIndex             = maIndex;
  rmepObjKey.dot1agCfmMepIdentifier       = mepId;
  rmepObjKey.dot1agCfmMepDbRMepIdentifier = remoteMepId;

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);

  rmepObj = avlSearch(&dot1agCFM.dot1agRMepTree, &rmepObjKey, AVL_EXACT);
  if (rmepObj != 0)
  {
    rc = OFDPA_E_NONE;

    if (dbEntry != NULL)
    {
      dbEntry->lastUpdateTime = rmepObj->dot1agCfmMepDbRMepFailedOkTime;
      memcpy (dbEntry->remoteMepMacAddr.addr, 
              rmepObj->dot1agCfmRmepVar.rMepMacAddress, 
              sizeof(dbEntry->remoteMepMacAddr.addr));

      switch(rmepObj->dot1agCfmRmepState)
      {
        case DOT1AG_RMEP_IDLE: 
          dbEntry->remoteMepState = OFDPA_OAM_RMEP_STATE_IDLE;
          break; 
      
        case DOT1AG_RMEP_START:
          dbEntry->remoteMepState = OFDPA_OAM_RMEP_STATE_START;
          break; 
      
        case DOT1AG_RMEP_OK:
          dbEntry->remoteMepState = OFDPA_OAM_RMEP_STATE_OK;
          break; 
      
        case DOT1AG_RMEP_FAILED:
          dbEntry->remoteMepState = OFDPA_OAM_RMEP_STATE_FAILED;
          break; 
      
        case DOT1AG_RMEP_DEFAULT:
          dbEntry->remoteMepState = 0;
          rc = OFDPA_E_INTERNAL;
          break; 
      
        default: 
          dbEntry->remoteMepState = 0;
          rc = OFDPA_E_INTERNAL;
          break; 
      }
    }
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  Find CCM database entry corresponding to mepId and remote mepId 
*           and return next valid remote mepId 
*
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                       used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification 
*                                       used by SNMP.
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    remoteMepId     @b{(input)} Index of corresponding rmep entry
* @param    nextRemoteMepId @b{(output)}Index of next rmep entry
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_NOT_FOUND
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agOamMepCCMDatabaseEntryNextGet(uint32_t mdIndex, uint32_t maIndex, 
                                                  uint32_t mepId, uint32_t remoteMepId,
                                                  uint32_t *nextRemoteMepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NOT_FOUND;
  dot1agCfmRMepObj_t      *rmepObj;
  dot1agCfmRMepObjKey_t   rmepObjKey;

  rmepObjKey.dot1agCfmMdIndex             = mdIndex;
  rmepObjKey.dot1agCfmMaIndex             = maIndex;
  rmepObjKey.dot1agCfmMepIdentifier       = mepId;
  rmepObjKey.dot1agCfmMepDbRMepIdentifier = remoteMepId;

  *nextRemoteMepId = 0;

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  rmepObj = avlSearch(&dot1agCFM.dot1agRMepTree, &rmepObjKey, AVL_NEXT);
  if (rmepObj != 0)
  {
    if ((rmepObj->key.dot1agCfmMdIndex == mdIndex) &&
        (rmepObj->key.dot1agCfmMaIndex == maIndex) &&
        (rmepObj->key.dot1agCfmMepIdentifier == mepId))
    {
      *nextRemoteMepId = rmepObj->key.dot1agCfmMepDbRMepIdentifier;
      rc = OFDPA_E_NONE;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  get one entry for Ondem LM
*
* @param    primVID    @b{(input)} VLAN ID associated with the MA. Can be
*                                  0 when used with SNMP.
*
*
* @returns  Ondemlm Entry
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
y1731CfmMepOndemlmTable_t *dot1agMepOndemlmEntryGet(uint32_t intIfNum, uint16_t primVID,
                                                    uint8_t mdLevel, Dot1agCfmMpDirection_t direction,
                                                    uint32_t controlPriority)
{
  dot1agCfmStackObj_t       *pMPObj          = 0;
  uint32_t                   idx;
  y1731CfmMepOndemlmTable_t *mepOndemlmEntry = 0;

  if (((pMPObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel, direction, AVL_EXACT)) != 0) &&
      (pMPObj->mepData != NULL))
  {
    idx = pMPObj->mepData - &(DOT1AG_MEP_TABLE(0));
    mepOndemlmEntry = &y1731CfmMepOndemlmTable[idx][controlPriority];
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "idx %d, entry addr 0x%x, 0x%x",
                       idx, mepOndemlmEntry, *mepOndemlmEntry);
  }
  return mepOndemlmEntry;
}

/*********************************************************************
* @purpose  start Ondem LM on MEP
*
*
* @param    intIfNum        @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification used by
*                                  SNMP. Use 0 for other UI's
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    destMac         @b{(input)} MAC address of MP to which LBM is
*                                  sent
* @param    destMepid       @b{(input)} Destination MEPID to which LBM is
*                                  to be sent
* @param    controlPriority @b{(input)} LMM/LMR frame priority, 0 -7, which will be carried in LMM/LMR frames
* @param    priorityClass   @b{(input)} Priority class (CoS) of the traffic that CCM-based LM tool should monitor
* @param    controlPeriod   @b{(input)} could be 100ms (by default), 10ms, 1s. It will not carried in LMM/LMR frames
* @param    msgCount        @b{(input)}
* @param    toolType        @b{(input)} Tool Type to specify CCM, LMM or SLM based LM
* @param    testId          @b{(input)} TestId to be used for SLM based LM.
*                                       can be set to 0 for non-SLM based LM
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmOndemLmmStart(uint32_t intIfNum, uint32_t mdIndex,
                                     uint32_t maIndex, uint32_t mepId,
                                     uint8_t *destMac, uint32_t destMepId,
                                     uint32_t controlPriority, uint32_t priorityClass,
                                     uint32_t controlPeriod, uint32_t msgCount,
                                     uint8_t toolType, uint32_t testId)
{
  OFDPA_ERROR_t        rc     = OFDPA_E_FAIL;
  dot1agCfmMepObj_t   *mepObj;
  dot1agMsgCmdData_t   msg;
  dot1agCfmStackObj_t *pMPObj = 0;
  OFDPA_BOOL           lmDestIsMepId = OFDPA_TRUE;
  dot1agCfmSlmObj_t    slmEntry;

  /* Validate Input */
  if (destMac == 0 && destMepId == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if (((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0) &&
      (mepObj->mepData != NULL))
  {
    rc = OFDPA_E_NONE;
    if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                       mepObj->mepData->dot1agCfmMepPrimaryVid,
                                       mepObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                       mepObj->mepData->dot1agCfmMepDirection, AVL_EXACT)) == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "fail to get CfmStackObj. mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_ERROR;
    }

    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "start OndemLMM: intf %d, {%d, %d, %d}, pri %d ",
                       intIfNum, mdIndex, maIndex, mepId, controlPriority);
    if (mepObj->mepData->y1731CfmOndemLMMState[controlPriority] != Y1731_ONDEMLMM_IDLE)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "OndemLMM ongoing. mdIndex %d maIndex %d mepIndex %d, pri %d ",
                         mdIndex, maIndex, mepId, controlPriority);
      return OFDPA_E_ERROR;
    }
    if (mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority] == 0)
    {
      mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority] = dot1agMepOndemlmEntryGet(intIfNum,
                                                                                           mepObj->mepData->dot1agCfmMepPrimaryVid,
                                                                                           mepObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                                                                           mepObj->mepData->dot1agCfmMepDirection, controlPriority);
    }
    if (mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority] == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "fail to allocate OndemLM entry. mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_ERROR;
    }
    memset(mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority], 0x0,
           sizeof(*mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]));

    /* Set MAC Address to which LMM is to be sent */
    if (destMac != 0)
    {
      memcpy(mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmDestMacAddress,
             destMac, OFDPA_MAC_ADDR_LEN);
      lmDestIsMepId = OFDPA_FALSE;
    }
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmDestIsMepId = lmDestIsMepId;
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmDestMepId = destMepId;
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmControlPeriod = controlPeriod;
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmControlPriority = controlPriority;
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmPriorityClass = priorityClass;
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmToSend = msgCount;
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->pMPObj = pMPObj;
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmToolType = toolType;
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmSlmTestId = testId;
    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmEnabled = OFDPA_TRUE;

    mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority]->ondemlmStartTime = dpaUpTimeSeconds();
    mepObj->mepData->y1731CfmOndemLMMState[controlPriority] = Y1731_ONDEMLMM_IDLE;

    if (toolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM)
    {
     /* Creates a new entry in the SLM tree. */
      memset(&slmEntry, 0x0, sizeof(slmEntry));

      slmEntry.key.dot1agCfmLmepIdentifier  = pMPObj->mepData->dot1agCfmMepIdentifier;
      slmEntry.key.dot1agCfmRmepIdentifier  = destMepId;
      slmEntry.key.dot1agCfmTestId          = testId;

      if (dot1agSlmEntryInsert(&slmEntry) != OFDPA_E_NONE)
      {
        dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
        return OFDPA_E_FAIL;
      }
    }

    /* Post a message to queue so that mep create event is passed to state machines */
    msg.mdIndex  = mdIndex;
    msg.maIndex  = maIndex;
    msg.mepIndex = mepId;
    msg.intIfNum = intIfNum;
    msg.uintVal  = controlPriority;
    if (dot1agIssueCmd(dot1agMepOndemLMMStart, 0, &msg) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to start OndemLMM. mdIndex %d maIndex %d mepIndex %d intf %d",
                         mdIndex, maIndex, mepId, intIfNum);
      rc = OFDPA_E_FAIL;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  stop Ondem LM on MEP
*
*
* @param    intIfNum        @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification used by
*                                  SNMP. Use 0 for other UI's
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    controlPriority @b{(input)} LMM/LMR frame priority, 0 -7, which will be carried in LMM/LMR frames
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmOndemLmmStop(uint32_t intIfNum, uint32_t mdIndex,
                                    uint32_t maIndex, uint32_t mepId,
                                    uint32_t controlPriority)
{
  OFDPA_ERROR_t        rc     = OFDPA_E_FAIL;
  dot1agCfmMepObj_t   *mepObj;
  dot1agMsgCmdData_t   msg;
  dot1agCfmStackObj_t *pMPObj = 0;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if (((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0) &&
      (mepObj->mepData != NULL))
  {
    rc = OFDPA_E_NONE;
    if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                       mepObj->mepData->dot1agCfmMepPrimaryVid,
                                       mepObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                       mepObj->mepData->dot1agCfmMepDirection, AVL_EXACT)) == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "fail to get CfmStackObj. mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_ERROR;
    }

    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "stop OndemLMM: intf %d, {%d, %d, %d}, pri %d ",
                       intIfNum, mdIndex, maIndex, mepId, controlPriority);
    if (mepObj->mepData->y1731CfmMepOndemlmEntry[controlPriority] == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "OndemLMM not started yet. mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_NONE;
    }

    /* Post a message to queue so that mep create event is passed to state machines */
    msg.mdIndex  = mdIndex;
    msg.maIndex  = maIndex;
    msg.mepIndex = mepId;
    msg.intIfNum = intIfNum;
    msg.uintVal  = controlPriority;
    if (dot1agIssueCmd(dot1agMepOndemLMMStop, 0, &msg) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to stop OndemLMM. mdIndex %d maIndex %d mepIndex %d intf %d",
                         mdIndex, maIndex, mepId, intIfNum);
      rc = OFDPA_E_FAIL;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  get one entry for DM
*
* @param    primVID    @b{(input)} VLAN ID associated with the MA. Can be
*                                  0 when used with SNMP.
*
*
* @returns  DM Entry
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
y1731CfmMepDmTable_t * dot1agMepDmEntryGet(uint32_t intIfNum, uint16_t primVID,
                                           uint8_t mdLevel, Dot1agCfmMpDirection_t direction,
                                           uint32_t controlPriority)
{
  dot1agCfmStackObj_t  *pMPObj     = 0;
  uint32_t              idx;
  y1731CfmMepDmTable_t *mepDmEntry = 0;

  if (((pMPObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel, direction, AVL_EXACT)) != 0) &&
      (pMPObj->mepData))
  {
    idx = pMPObj->mepData - &(DOT1AG_MEP_TABLE(0));
    mepDmEntry = &y1731CfmMepDmTable[idx][controlPriority];
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "idx %d, entry addr 0x%x, 0x%x",
                       idx, mepDmEntry, *mepDmEntry);
  }

  return mepDmEntry;
}

/*********************************************************************
* @purpose  start DM on MEP
*
*
* @param    intIfNum        @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification used by
*                                  SNMP. Use 0 for other UI's
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    destMac         @b{(input)} MAC address of MP to which LBM is
*                                  sent
* @param    destMepid       @b{(input)} Destination MEPID to which LBM is
*                                  to be sent
* @param    controlPriority @b{(input)} LMM/LMR frame priority, 0 -7, which will be carried in LMM/LMR frames
* @param    controlPeriod   @b{(input)} could be 100ms (by default), 10ms, 1s. It will not carried in LMM/LMR frames
* @param    dmType          @b{(input)} ProDM or OnDemDM
* @param    msgCount        @b{(input)}
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmDmmStart(uint32_t intIfNum, uint32_t mdIndex,
                                uint32_t maIndex, uint32_t mepId,
                                uint8_t *destMac, uint32_t destMepId,
                                uint32_t controlPriority, uint32_t controlPeriod,
                                uint32_t frameLen, uint32_t dmType, uint32_t msgCount)
{
  OFDPA_ERROR_t        rc     = OFDPA_E_FAIL;
  dot1agCfmMepObj_t   *mepObj;
  dot1agMsgCmdData_t   msg;
  dot1agCfmStackObj_t *pMPObj = 0;

  /* Validate Input */
  if (destMac == 0 && destMepId == 0)
  {
    return OFDPA_E_ERROR;
  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if (((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0) &&
      (mepObj->mepData != NULL))
  {
    rc = OFDPA_E_NONE;
    if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                       mepObj->mepData->dot1agCfmMepPrimaryVid,
                                       mepObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                       mepObj->mepData->dot1agCfmMepDirection, AVL_EXACT)) == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "fail to get CfmStackObj. mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_ERROR;
    }

    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "start DMM: intf %d, {%d, %d, %d}, pri %d, type %d ",
                       intIfNum, mdIndex, maIndex, mepId, controlPriority, dmType);
    if (mepObj->mepData->y1731CfmDMMState[controlPriority] != Y1731_DMM_IDLE)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "DMM ongoing. mdIndex %d maIndex %d mepIndex %d, pri %d ",
                         mdIndex, maIndex, mepId, controlPriority);
      return OFDPA_E_ERROR;
    }
    if (mepObj->mepData->y1731CfmMepDmEntry[controlPriority] == 0)
    {
      mepObj->mepData->y1731CfmMepDmEntry[controlPriority] = dot1agMepDmEntryGet(intIfNum,
                                                                                 mepObj->mepData->dot1agCfmMepPrimaryVid,
                                                                                 mepObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                                                                 mepObj->mepData->dot1agCfmMepDirection, controlPriority);
    }
    if (mepObj->mepData->y1731CfmMepDmEntry[controlPriority] == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "fail to allocate DM entry. mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_ERROR;
    }
    memset(mepObj->mepData->y1731CfmMepDmEntry[controlPriority], 0x0,
           sizeof(*mepObj->mepData->y1731CfmMepDmEntry[controlPriority]));

    /* Set MAC Address to which DMM is to be sent */
    if (destMac != 0)
    {
      memcpy(mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmDestMacAddress,
             destMac, OFDPA_MAC_ADDR_LEN);
      mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmDestIsMepId = OFDPA_FALSE;
    }
    else
    {
      mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmDestMepId = destMepId;
      mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmDestIsMepId = OFDPA_TRUE;
    }
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmControlPeriod = controlPeriod;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmControlPriority = controlPriority;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmType = dmType;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmToSend = msgCount;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->pMPObj = pMPObj;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmPacketLength = frameLen;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmEnabled = OFDPA_TRUE;
    if (mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmType == y1731DmTypeProactive)
    {
      mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmBinEnable = (Y1731_BIN_FLAG_15M | Y1731_BIN_FLAG_24H);
    }
    else
    {
      mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmStartTime = dpaUpTimeMilliSeconds();
      mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmEndTime = 0;
    }
    memset(mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmCurBin, 0,
           sizeof(mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmCurBin));
    memset(mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmHistoryBin, 0,
           sizeof(mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmHistoryBin));
    memset(mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmHistoryBinTail, 0,
           sizeof(mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmHistoryBinTail));

    mepObj->mepData->y1731CfmDMMState[controlPriority] = Y1731_DMM_IDLE;

    /* Post a message to queue so that mep create event is passed to state machines */
    msg.mdIndex  = mdIndex;
    msg.maIndex  = maIndex;
    msg.mepIndex = mepId;
    msg.intIfNum = intIfNum;
    msg.uintVal  = controlPriority;
    if (dot1agIssueCmd(dot1agMepDMMStart, 0, &msg) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to start DMM. mdIndex %d maIndex %d mepIndex %d intf %d",
                         mdIndex, maIndex, mepId, intIfNum);
      rc = OFDPA_E_FAIL;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/*********************************************************************
* @purpose  stop DM on MEP
*
*
* @param    intIfNum        @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification used by
*                                  SNMP. Use 0 for other UI's
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    controlPriority @b{(input)} LMM/LMR frame priority, 0 -7, which will be carried in LMM/LMR frames
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmDmmStop(uint32_t intIfNum, uint32_t mdIndex,
                               uint32_t maIndex, uint32_t mepId,
                               uint32_t controlPriority)
{
  OFDPA_ERROR_t        rc     = OFDPA_E_FAIL;
  dot1agCfmMepObj_t   *mepObj;
  dot1agMsgCmdData_t   msg;
  dot1agCfmStackObj_t *pMPObj = 0;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if (((mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT)) != 0) &&
      (mepObj->mepData != NULL))
  {
    rc = OFDPA_E_NONE;
    if ((pMPObj = dot1agCfmStackObjGet(intIfNum,
                                       mepObj->mepData->dot1agCfmMepPrimaryVid,
                                       mepObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                       mepObj->mepData->dot1agCfmMepDirection, AVL_EXACT)) == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "fail to get CfmStackObj. mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_ERROR;
    }

    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "stop DMM: intf %d, {%d, %d, %d}, pri %d ",
                       intIfNum, mdIndex, maIndex, mepId, controlPriority);
    if (mepObj->mepData->y1731CfmMepDmEntry[controlPriority] == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "DMM not started yet. mdIndex %d maIndex %d mepIndex %d",
                         mdIndex, maIndex, mepId);
      return OFDPA_E_NONE;
    }

    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmEnabled = OFDPA_FALSE;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmType = y1731DmTypeInvalid;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmBinEnable = 0;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmStartTime = 0;
    mepObj->mepData->y1731CfmMepDmEntry[controlPriority]->dmEndTime = 0;

    /* Post a message to queue so that mep create event is passed to state machines */
    msg.mdIndex  = mdIndex;
    msg.maIndex  = maIndex;
    msg.mepIndex = mepId;
    msg.intIfNum = intIfNum;
    msg.uintVal  = controlPriority;
    if (dot1agIssueCmd(dot1agMepDMMStop, 0, &msg) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                         "Failed to stop DMM. mdIndex %d maIndex %d mepIndex %d intf %d",
                         mdIndex, maIndex, mepId, intIfNum);
      rc = OFDPA_E_FAIL;
    }
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return rc;
}

/* Debug Routines */
uint32_t dot1agDebugProlm(uint32_t mepId, OFDPA_BOOL mode, uint32_t pri, uint8_t toolType, uint32_t testId)
{

  uint8_t dmac[]  = {0x00, 0x80, 0xcc, 0xdd, 0xee, 0xaa};
  dot1agMepProlmCreate (4, 7, 1, mepId, dmac, 0, pri, toolType, testId, 4);
  dot1agMepProlmEnabledSet(4, 7, 1, mepId, mode);
  return 0;
}

uint32_t dot1agDebugOndemlmStart(uint32_t pri, uint32_t count, uint8_t toolType, uint32_t testId)
{
  uint8_t dmac[]  = {0x00, 0x80, 0xcc, 0xdd, 0xee, 0xaa};

  dot1agCfmOndemLmmStart(4, 7, 1, 651,
                         dmac, 0,
                         pri, pri,
                         DOT1AG_CCM_interval1s, count, toolType, testId);
  return 0;
}

uint32_t dot1agDebugOndemlmStartRmep(uint32_t rmepId, uint32_t pri, uint32_t count, uint8_t toolType, uint32_t testId)
{
  dot1agCfmOndemLmmStart(4, 7, 1, 651,
                         0, rmepId,
                         pri, 7,
                         DOT1AG_CCM_interval1s, count, toolType, testId);
  return 0;
}

uint32_t dot1agDebugOndemlmStop(uint32_t pri)
{
  dot1agCfmOndemLmmStop(4, 7, 1, 651, pri);
  return 0;
}

uint32_t dot1agDebugProDmStart(uint32_t mepId, uint32_t rmepId, uint32_t pri, uint32_t interval)
{
  dot1agCfmDmmStart(4, 7, 1, mepId,
                    0, rmepId,
                    pri, interval,
                    64, y1731DmTypeProactive, 0xFFFFFFFF);
  return 0;
}

uint32_t dot1agDebugOndemDmStart(uint32_t pri, uint32_t count)
{
  uint8_t dmac[]  = {0x00, 0x80, 0xcc, 0xdd, 0xee, 0xaa};

  dot1agCfmDmmStart(4, 7, 1, 651,
                    dmac, 0,
                    pri, DOT1AG_CCM_interval1s,
                    64, y1731DmTypeOndemand, count);
  return 0;
}

uint32_t dot1agDebugOndemDmStartRmep(uint32_t rmepId, uint32_t pri, uint32_t count)
{
  dot1agCfmDmmStart(4, 7, 1, 651,
                    0, 601,
                    pri, DOT1AG_CCM_interval1s,
                    64, y1731DmTypeOndemand, count);
  return 0;
}

uint32_t dot1agDebugProDmStop(uint32_t mepId, uint32_t pri)
{
  dot1agCfmDmmStop(4, 7, 1, mepId, pri);
  return 0;
}

uint32_t dot1agDebugDomainCreate(uint32_t mdIndex, int mdLevel)
{
  uint8_t tempName[10];

  sprintf(tempName, "test%d", mdIndex);
  return dot1agMdCreate(mdIndex, mdLevel, tempName);
}

uint32_t dot1agDebugDomainDelete(uint32_t mdIndex)
{
   return dot1agMdDelete(mdIndex, -1, 0);
}

uint32_t dot1agDebugMepCreate()
{
  static uint32_t mdIndex = 1, maIndex = 1;
  uint32_t        mepIndex, intIfNum, mdLevel, primVID;
  uint32_t        direction;
  uint8_t         macAddress[OFDPA_MAC_ADDR_LEN] = {00, 00, 00, 12, 34, 56};
  uint32_t        lmepId;

  maIndex = 1;
  mepIndex = 1;
  intIfNum = 1;
  mdLevel = 1;
  primVID = 10;
  direction = DOT1AG_MP_DIRECTION_UP;
  lmepId = 1;

  dot1agMdCreate(mdIndex, mdLevel, "domain 1");
  dot1agMaCreate(mdIndex, maIndex, "ma 1", primVID);
  dot1agMaCreate(mdIndex, maIndex+1, "ma 2", primVID+1);
  dot1agMaCreate(mdIndex, maIndex+2, "ma 3", primVID+2);
  dot1agMepCreate(intIfNum, mdIndex, maIndex, mepIndex, direction, macAddress, lmepId);

  mdIndex++;
  return 0;
}

uint32_t dot1agDebugMepDelete()
{
//  uint32_t mepIndex, intIfNum, mdLevel, primVID;
//  uint32_t direction;
  uint32_t mdIndex, maIndex;

  mdIndex = 1;
  maIndex = 1;
//  mepIndex = 1;
//  intIfNum = 1;
//  mdLevel = 1;
//  primVID = 10;
//  direction = DOT1AG_MP_DIRECTION_UP;

  dot1agMaDelete(mdIndex, maIndex);
  return 0;
}

void dot1agDebugDumpDomains()
{
  uint32_t idx;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  for (idx = 0; idx < dpaDot1agMaintenanceDomainsMaxGet(); idx++)
  {
    if (DOT1AG_MD_CFG(idx).active == DOT1AG_ROW_ACTIVE ||
        DOT1AG_MD_CFG(idx).active ==  DOT1AG_ROW_INACTIVE_PENDING)
    {
      printf("Domain Index: %u, Name: %s, Level: %d, Active: %d\r\n",
             DOT1AG_MD_CFG(idx).dot1agCfmMdIndex,
             DOT1AG_MD_CFG(idx).dot1agCfmMdName,
             DOT1AG_MD_CFG(idx).dot1agCfmMdMdLevel,
             DOT1AG_MD_CFG(idx).active);
    }
  } /* End of domain loop iterations */

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
}

void dot1agDebugDumpMA(unsigned long ulUserID)
{
  dot1agCfmMAObj_t    *maObjEntry;
  dot1agCfmMAObjKey_t  maObjEntryKey;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  memset(&maObjEntryKey, 0x0, sizeof(maObjEntryKey));
  maObjEntry = avlSearch(&dot1agCFM.dot1agMATree, &maObjEntryKey, AVL_NEXT);
  while (maObjEntry != 0)
  {
    printf("MA  mdIndex: %d maIndex: %d active: %d\r\n",
           maObjEntry->key.dot1agCfmMdIndex, maObjEntry->key.dot1agCfmMaIndex,
           maObjEntry->maData->active);

    memcpy(&maObjEntryKey, &maObjEntry->key, sizeof(maObjEntryKey));
    maObjEntry = avlSearch(&dot1agCFM.dot1agMATree, &maObjEntryKey, AVL_NEXT);
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
}

void dot1agDebugDumpMEP(unsigned long ulUserID)
{
  dot1agCfmMepObj_t    *mepObjEntry;
  dot1agCfmMepObjKey_t  mepObjEntryKey;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  memset(&mepObjEntryKey, 0x0, sizeof(mepObjEntryKey));
  mepObjEntry = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjEntryKey, AVL_NEXT);
  while ((mepObjEntry != 0) &&
         (mepObjEntry->mepData != NULL))
  {
    printf("MEP  mdIndex: %d maIndex: %d mepID: %d intIfNum: %d mepObj %8p direction: %s vid: %d active: %d\r\n",
           mepObjEntry->key.dot1agCfmMdIndex, mepObjEntry->key.dot1agCfmMaIndex,
           mepObjEntry->key.dot1agCfmMepIdentifier, mepObjEntry->key.dot1agCfmMepIfIndex,
           mepObjEntry->mepData,
           (mepObjEntry->mepData->dot1agCfmMepDirection == DOT1AG_MP_DIRECTION_UP?"UP":"DOWN"),
           mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
           mepObjEntry->mepData->active);

    memcpy(&mepObjEntryKey, &mepObjEntry->key, sizeof(mepObjEntryKey));
    mepObjEntry = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjEntryKey, AVL_NEXT);
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
}

void dot1agDebugDumpCFMStack(unsigned long ulUserID)
{
  dot1agCfmStackObj_t    *mpObj;
  dot1agCfmStackObjKey_t  mpObjKey;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  memset(&mpObjKey, 0x0, sizeof(mpObjKey));
  mpObj = avlSearch(&dot1agCFM.dot1agCfmStackTree, &mpObjKey, AVL_NEXT);
  while (mpObj != 0)
  {
    printf("CFM MP %p intIfNum: %d vlanId: %d mdLevel: %d direction: %s  operMode: %d  "
           "macAddr: %02x:%02x:%02x:%02x:%02x:%02x  mepObj %p MEPID: %d\r\n",
           mpObj,
           mpObj->key.dot1agCfmStackifIndex, mpObj->key.dot1agCfmStackVlanIdOrNone,
           mpObj->key.dot1agCfmStackMdLevel,
           (mpObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP?"UP":"DOWN"),
           mpObj->operMode,
           mpObj->mpMacAddress[0], mpObj->mpMacAddress[1], mpObj->mpMacAddress[2],
           mpObj->mpMacAddress[3], mpObj->mpMacAddress[4], mpObj->mpMacAddress[5],
           mpObj->mepData,
           (mpObj->mepData) ? mpObj->mepData->dot1agCfmMepIdentifier : -1);

    memcpy(&mpObjKey, &mpObj->key, sizeof(mpObjKey));
    mpObj = avlSearch(&dot1agCFM.dot1agCfmStackTree, &mpObjKey, AVL_NEXT);
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
}


void dot1agDebugDumpClientMPDBObj(unsigned long ulUserID)
{
  dot1agCfmClientMPDBObj_t clientMPDBEntry, *pClientMPDBObj = NULL;


  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  printf("---------------------- Client MP DB Start ----------------------\n");
  printf("ServerLmep - ClientLmep - ServerLocalMP - TxDirSameAsServer - ClientMPObj\n");

  clientMPDBEntry.key.serverLmepId = 0;
  clientMPDBEntry.key.clientLmepId = 0;
  while ((pClientMPDBObj = dot1agCfmClientMPDBObjGet(clientMPDBEntry.key, AVL_NEXT)) != NULL)
  {
	  printf("%-10d - %-10d - %-13d - %-17s - %-11p\n",
			 pClientMPDBObj->key.serverLmepId, pClientMPDBObj->key.clientLmepId,
			 pClientMPDBObj->serverOfdpaLocalMpId,
			 ((pClientMPDBObj->txDirSameAsServer == 1) ? "TRUE":"FALSE"),
			 pClientMPDBObj->pClientMPObj);
	  printf("\n");

    clientMPDBEntry.key.serverLmepId = pClientMPDBObj->key.serverLmepId;
    clientMPDBEntry.key.clientLmepId = pClientMPDBObj->key.clientLmepId;

  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
}



int dot1agDebugLbmSend(uint32_t mdIndex, uint32_t maIndex,
                       uint32_t mepId, uint32_t destMepId,
                       uint32_t msgCount)
{
  uint8_t destMac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

  return dot1agCfmLbmSend(mdIndex, maIndex, mepId, destMac, destMepId,
                          msgCount);
}

void dot1agDebugDumpRMEP(unsigned long ulUserID)
{
  dot1agCfmRMepObj_t    *rmepObj;
  dot1agCfmRMepObjKey_t  rmepObjKey;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  memset(&rmepObjKey, 0x0, sizeof(rmepObjKey));
  rmepObj = avlSearch(&dot1agCFM.dot1agRMepTree, &rmepObjKey, AVL_NEXT);
  while (rmepObj != 0)
  {
    printf("RMEP Entry %p mdIndex: %d maIndex: %d mepId: %d rmepId: %d  rmepAddr: %02X:%02X:%02X:%02X:%02X:%02X dot1agCfmRmepState: %d hwExist %d\r\n",
           rmepObj,
           rmepObj->key.dot1agCfmMdIndex, rmepObj->key.dot1agCfmMaIndex,
           rmepObj->key.dot1agCfmMepIdentifier, rmepObj->key.dot1agCfmMepDbRMepIdentifier,
           rmepObj->dot1agCfmRmepVar.rMepMacAddress[0],
           rmepObj->dot1agCfmRmepVar.rMepMacAddress[1],
           rmepObj->dot1agCfmRmepVar.rMepMacAddress[2],
           rmepObj->dot1agCfmRmepVar.rMepMacAddress[3],
           rmepObj->dot1agCfmRmepVar.rMepMacAddress[4],
           rmepObj->dot1agCfmRmepVar.rMepMacAddress[5],
           rmepObj->dot1agCfmRmepState,
           rmepObj->hwExist);

    memcpy(&rmepObjKey, &rmepObj->key, sizeof(rmepObjKey));
    rmepObj = avlSearch(&dot1agCFM.dot1agRMepTree, &rmepObjKey, AVL_NEXT);
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
}

void dot1agDebugDumpCFMStackSM(unsigned long ulUserID)
{
  dot1agCfmStackObj_t    *mpObj;
  dot1agCfmStackObjKey_t  mpObjKey;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);

  memset(&mpObjKey, 0x0, sizeof(mpObjKey));
  mpObj = avlSearch(&dot1agCFM.dot1agCfmStackTree, &mpObjKey, AVL_NEXT);
  while (mpObj != 0)
  {
    if (mpObj->mepData)
    {
      printf("CFM MEP %p intIfNum: %d vlanId: %d mdLevel: %d direction: %s  operMode: %d MEPID: %d\r\n",
             mpObj,
             mpObj->key.dot1agCfmStackifIndex, mpObj->key.dot1agCfmStackVlanIdOrNone,
             mpObj->key.dot1agCfmStackMdLevel,
             (mpObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP?"UP":"DOWN"),
             mpObj->operMode, mpObj->mepData->dot1agCfmMepIdentifier);
      printf("dot1agCfmMepCCIState: %d\ndot1agCfmMepCCRState: %d\ndot1agCfmRmepErrState: %d\ndot1agCfmMepXConState: %d\n"
             "dot1agCfmMepLbiState: %d\ndot1agCfmMepLBIRState: %d\ndot1agCfmMepLtirState: %d\ndot1agCfmMepSMFngState: %d\r\n",
             mpObj->mepData->dot1agCfmMepCCIState,
             mpObj->mepData->dot1agCfmMepCCRState,
             mpObj->mepData->dot1agCfmRmepErrState,
             mpObj->mepData->dot1agCfmMepXConState,
             mpObj->mepData->dot1agCfmMepLbiState,
             mpObj->mepData->dot1agCfmMepLBIRState,
             mpObj->mepData->dot1agCfmMepLtirState,
             mpObj->mepData->dot1agCfmMepSMFngState);

    }
    memcpy(&mpObjKey, &mpObj->key, sizeof(mpObjKey));
    mpObj = avlSearch(&dot1agCFM.dot1agCfmStackTree, &mpObjKey, AVL_NEXT);
  }
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
}

/*********************************************************************
* @purpose  Obtain Maintenance Domain Format
*
* @param    mdIndex  @b{(input)}
*           mdFormat @b{(output)}
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdFormatGet(uint32_t mdIndex, uint32_t *mdFormat)
{
  *mdFormat = DOT1AG_MD_NAME_TYPE_charString;
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Verify if a give maintenance Aossication exists
*
* @param    mdIndex    @b{(input)} index value used by SNMP.
*           maIndex    @b{(input)} index value used by SNMP.
*
* @returns  OFDPA_TRUE
* @returns  OFDPA_FALSE
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_BOOL dot1agMaIndexIsValid(uint32_t mdIndex, uint32_t maIndex)
{
  uint32_t idx;

  if (mdIndex == 0 || maIndex == 0)
  {
    return OFDPA_FALSE;
  }

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  for (idx = 0; idx < dpaDot1agMaintenanceDomainsMaxGet(); idx++)
  {
    if (DOT1AG_MD_CFG(idx).active == DOT1AG_ROW_ACTIVE)
    {
      if (DOT1AG_MD_CFG(idx).dot1agCfmMdIndex == mdIndex)
      {
        if (dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE)
            != 0)
        {
          /* Obtain the domain entry */
          if (dot1agActiveMAGet(mdIndex, maIndex,
                                AVL_EXACT) != 0)
          {
            dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
            return OFDPA_TRUE;
          }
        }
        break;
      }
    }
  } /* End of domain loop iterations */

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return OFDPA_FALSE;
}

/*********************************************************************
* @purpose  Obtain a next valid mip value given a maintenance domain
*
* @param    mdIndex  @b{(input/output)} Index value used by SNMP
*                                       Use 0 to obtain first valid
*                                       value.
*           ifIndex  @b{(input/output)} Interface Index used by SNMP.
*                                       Use 0 to obtain first valid
*                                       value.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipNextGet(uint32_t *mdIndex, uint32_t *ifIndex)
{
  int  mdLevel;

  if (mdIndex == 0 || ifIndex == 0)
  {
    return OFDPA_E_ERROR;
  }

  if (dot1agMdIsValid(*mdIndex, -1, 0) != OFDPA_E_NONE)
  {
    if (dot1agMdIndexNextGet(mdIndex) != OFDPA_E_NONE)
    {
      return OFDPA_E_FAIL;
    }

    *ifIndex = 0;
  }

  if (dot1agMdLevelGet(*mdIndex, &mdLevel) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  do
  {
    if (dot1agValidIntfNextGet(ifIndex) == OFDPA_E_NONE)
    {
      return OFDPA_E_NONE;
    }
    else if (dot1agMdIndexNextGet(mdIndex) != OFDPA_E_NONE)
    {
      break;
    }
    else
    {
      if (dot1agMdLevelGet(*mdIndex, &mdLevel) != OFDPA_E_NONE)
      {
        return OFDPA_E_FAIL;
      }
      *ifIndex = 0;
    }
  } while (1);

  return OFDPA_E_FAIL;
}

/*********************************************************************
* @purpose  Add a maintenance end point on an interface given
*           (mdIndex) and (maIndex) mepId rmepId
*           and direction
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP. Use 0 for other UI's
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    rmepId     @b{(input)} Index to identify a remote MEP
* @param    srcMac     @b{(input)} MAC address of remote MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agRMepEntryAdd(uint32_t intIfNum, uint32_t mdIndex,
                                 uint32_t maIndex, uint32_t mepId,
                                 uint32_t rmepId, uint8_t *srcMac)
{
  dot1agCfmMAObj_t      *maObjEntry;
  dot1agCfmMAObjKey_t    maObjEntryKey;
  dot1agCfmMepObj_t     *pMepObj;
  dot1agCfmMepObjKey_t   mepObjKey;
  dot1agCfmRMepObj_t    *pRMEPObj;
  dot1agCfmMaNetTable_t *maEntry = 0;
  dot1agCfmMdTable_t    *mdEntry = 0;
  dot1agMsgCmdData_t     msg;
  OFDPA_ERROR_t          rc      = OFDPA_E_NONE;
  uint32_t               primVID;

  /* Validate input */
  if (mdIndex == 0 ||
      maIndex == 0 ||
      (mepId < OFDPA_DOT1AG_MEPID_MIN  || mepId > OFDPA_DOT1AG_MEPID_MAX) ||
      (rmepId < OFDPA_DOT1AG_MEPID_MIN  || rmepId > OFDPA_DOT1AG_MEPID_MAX))
  {
    return OFDPA_E_ERROR;
  }

  /* Validate Input */
  if (srcMac == 0)
  {
    return OFDPA_E_ERROR;
  }

  /* Validate the srcMac address */
  if (dot1agIsRestrictedMacAddress(srcMac) == OFDPA_TRUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_VERBOSE,
                       "Failed to create MEP remote entry mdIndex %d maIndex %d mepIndex %d rmepindex %d - Invalid MAC Address %02X:%02X:%02X:%02X:%02X:%02X",
                       mdIndex, maIndex,
                       mepId, rmepId,
                       srcMac[0], srcMac[1], srcMac[2],
                       srcMac[3], srcMac[4], srcMac[5]);
    return OFDPA_E_FAIL;

  }

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mdEntry = dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE)) != 0)
  {
    /* User entered a valid maID. Use MIB view */
    maObjEntryKey.dot1agCfmMdIndex = mdEntry->dot1agCfmMdIndex;
    maObjEntryKey.dot1agCfmMaIndex = maIndex;

    maObjEntry = avlSearch(&dot1agCFM.dot1agMATree, &maObjEntryKey, AVL_EXACT);
    if (maObjEntry == 0)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_ERROR;
    }
    maEntry = maObjEntry->maData;
    primVID = maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId;

    if (maEntry->dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT].dot1agCfmMaCompPrimaryVlanId == 0)
    {
      /* MA not yet ready for MEP creations */
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_ERROR;
    }

    memset(&mepObjKey, 0x0, sizeof(mepObjKey));
    mepObjKey.dot1agCfmMdIndex = mdEntry->dot1agCfmMdIndex;
    mepObjKey.dot1agCfmMaIndex = maIndex;
    mepObjKey.dot1agCfmMepIdentifier = mepId;
    mepObjKey.dot1agCfmMepIfIndex    = intIfNum;

    /* A new MEP can be created */
    pMepObj = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjKey, AVL_EXACT);
    if (pMepObj != 0)
    {
      rc = OFDPA_E_NONE;

      /* Delete the RMEP entry if already configured */
      if ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                          rmepId, AVL_EXACT)) != 0)
      {
        memset(&msg, 0x0, sizeof(msg));
        msg.mdIndex  = mdEntry->dot1agCfmMdIndex;
        msg.maIndex  = maIndex;
        msg.mepIndex = mepId;
        msg.rmepIndex = rmepId;
        msg.intIfNum = intIfNum;

        /* Post a message to queue so that rmep is delete completely */
        if (dot1agIssueCmd(dot1agCfmRMEPEntryDelete, 0, &msg) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to delete RMEP for mdIndex %d maIndex %d mepIndex %d rmepId %d vid %d intf %d",
                             mdEntry->dot1agCfmMdIndex, maIndex,
                             mepId, rmepId, primVID, intIfNum);
          rc = OFDPA_E_FAIL;
        }
      }

      /* post a message to queue to create remote MEP entry*/
      memset(&msg, 0x0, sizeof(msg));
      msg.mdIndex   = mdEntry->dot1agCfmMdIndex;
      msg.maIndex   = maEntry->dot1agCfmMaIndex;
      msg.mepIndex  = mepId;
      msg.rmepIndex = rmepId;
      msg.intIfNum  = intIfNum;

      memcpy(msg.srcMac, srcMac, OFDPA_MAC_ADDR_LEN);

      if (dot1agIssueCmd(dot1agCfmRMEPEntryAdd, 0, &msg) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Failed to create MEP remote entry mdIndex %d maIndex %d mepIndex %d rmepindex %d vid %d intf %d",
                           mdEntry->dot1agCfmMdIndex, maEntry->dot1agCfmMaIndex,
                           mepId, rmepId, primVID, intIfNum);
        rc = OFDPA_E_FAIL;
      }
    }
    else
    {
      rc = OFDPA_E_FAIL;
    }
  }/* End of active domain get check */
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Delete remote  maintenance end point on an interface given
*           (mdIndex) and (maIndex), mepId and rmepId
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  deleted
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP. Use 0 for
*                                  other UI's
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP. Use 0 for other UI's
* @param    mepId      @b{(input)} Index used to identify a MEP.
* @param    rmepId      @b{(input)} Index used to identify a MEP.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agRMepEntryDelete(uint32_t intIfNum, uint32_t mdIndex,
                                    uint32_t maIndex, uint32_t mepId,
                                    uint32_t rmepId)
{
  dot1agCfmMepObj_t    *pMepObj;
  dot1agCfmMepObjKey_t  mepObjKey;
  dot1agCfmRMepObj_t   *pRMEPObj;
  dot1agCfmMdTable_t   *mdEntry  = 0;
  dot1agCfmMepTable_t  *mepEntry = 0;
  dot1agMsgCmdData_t    msg;
  OFDPA_ERROR_t         rc       = OFDPA_E_FAIL;

  /* Validate input */
  if (mdIndex == 0 ||
      maIndex == 0 ||
      (mepId < OFDPA_DOT1AG_MEPID_MIN  || mepId > OFDPA_DOT1AG_MEPID_MAX))
  {
    return OFDPA_E_ERROR;
  }
  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  if ((mdEntry = dot1agActiveMdGet(mdIndex, 0, -1, DOT1AG_ROW_ACTIVE)) != 0)
  {
    memset(&mepObjKey, 0x0, sizeof(mepObjKey));
    mepObjKey.dot1agCfmMdIndex       = mdEntry->dot1agCfmMdIndex;
    mepObjKey.dot1agCfmMaIndex       = maIndex;
    mepObjKey.dot1agCfmMepIdentifier = mepId;
    mepObjKey.dot1agCfmMepIfIndex    = intIfNum;

    pMepObj = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjKey, AVL_EXACT);
    if ((pMepObj != 0) &&
        (pMepObj->mepData != NULL))
    {
      rc = OFDPA_E_NONE;
      if ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                          rmepId, AVL_EXACT)) != 0)
      {
        mepEntry = pMepObj->mepData;
        msg.mdIndex  = mdEntry->dot1agCfmMdIndex;
        msg.maIndex  = maIndex;
        msg.mepIndex = mepId;
        msg.rmepIndex = rmepId;
        msg.intIfNum = intIfNum;
        /* Post a message to queue so that rmep is delete completely */
        if (dot1agIssueCmd(dot1agCfmRMEPEntryDelete, 0, &msg) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                             "Failed to delete RMEP for mdIndex %d maIndex %d mepIndex %d rmepId %d vid %d intf %d",
                             mdEntry->dot1agCfmMdIndex, maIndex,
                             mepId, rmepId, mepEntry->dot1agCfmMepPrimaryVid, intIfNum);
          rc = OFDPA_E_FAIL;
        }
      }
    }
  }/* End of active domain get */
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose  Determine if the specified MAC address is a member of the
*           restricted set.
*
* @param  *macaddr - pointer to mac address
*
* @returns OFDPA_TRUE if restricted mac, OFDPA_FALSE otherwise
*
* @end
*
*********************************************************************/
OFDPA_BOOL dot1agIsRestrictedMacAddress(uint8_t *macaddr)
{
  OFDPA_BOOL    rc;
  uint32_t  i;

  /* --------------------------- */
  /* Check for 00:00:00:00:00:00 */
  /* --------------------------- */
  rc = OFDPA_TRUE;
  for (i = 0; i < 6; i++)
  {
    if (macaddr[i] != 0x00)
    {
      rc = OFDPA_FALSE;
      break;
    }
  }
  if (rc == OFDPA_TRUE)
  {
    return OFDPA_TRUE;
  }

  /* --------------------------- */
  /* Check for FF:FF:FF:FF:FF:FF */
  /* --------------------------- */
  rc = OFDPA_TRUE;
  for (i = 0; i < 6; i++)
  {
    if (macaddr[i] != 0xFF)
    {
      rc = OFDPA_FALSE;
      break;
    }
  }
  if (rc == OFDPA_TRUE)
  {
    return OFDPA_TRUE;
  }

  /* ------------------------------- */
  /* Check for Multicast Mac Address */
  /* ------------------------------- */
  if ((macaddr[0] & 0x01) == 0x01)
  {
    rc = OFDPA_TRUE;
  }
  if (rc == OFDPA_TRUE)
  {
    return OFDPA_TRUE;
  }

  return OFDPA_FALSE;
}

/***************************************************************************
* @purpose  Returns the status of hardware enabled MEPs
*
* @param    mepId      @b{(input)} Index to identify a MEP
*
* @returns  OFDPA_TRUE    If device supports hardware enabled dot1ag and MEP Id
*                         is within the supported range
* @returns  OFDPA_FALSE   Otherwise
*
* @notes
*
* @end
*****************************************************************************/
OFDPA_BOOL dot1agIsMepHwEnabled(uint32_t mepId)
{
  static int mepCount = -1;

  if (mepCount < 0)
  {
    mepCount = dpaOamMepTableEntryCountGet();
  }

  if ((OFDPA_DOT1AG_MEPID_MIN <= mepId) && (mepCount >= mepId))
  {
    return OFDPA_TRUE;
  }

  /* Modified by JiaoYong, 2016/7/19 返回true，不让CC状态机运转*/  
  //return OFDPA_FALSE;
  return OFDPA_TRUE;
}

/***************************************************************************
* @purpose  Determine if MEP is configured on the interface
*
* @param    intIfNum   @b{(input)} intIfNum to check if MEP is configured
*
* @returns  OFDPA_E_NONE If MEP is configured on the interface
*                        is within the supported range
* @returns  OFDPA_E_FAIL Otherwise
*
* @notes
*
* @end
*****************************************************************************/
OFDPA_ERROR_t dot1agIsMepConfigOnIntf(uint32_t intIfNum)
{
  dot1agCfmMepObj_t    *mepObjEntry;
  dot1agCfmMepObjKey_t  mepObjEntryKey;

  dpaRWlockWritelock(&dot1agCFM.dot1agCfgRWLock);
  memset(&mepObjEntryKey, 0x0, sizeof(mepObjEntryKey));
  mepObjEntry = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjEntryKey, AVL_NEXT);
  while ((mepObjEntry != 0) &&
         (mepObjEntry->mepData != NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_TOO_VERBOSE,
                       "\nMEP  mdIndex: %d maIndex: %d mepID: %d intIfNum: %d direction: %s vid: %d active: %d",
                       mepObjEntry->key.dot1agCfmMdIndex, mepObjEntry->key.dot1agCfmMaIndex,
                       mepObjEntry->key.dot1agCfmMepIdentifier, mepObjEntry->key.dot1agCfmMepIfIndex,
                       (mepObjEntry->mepData->dot1agCfmMepDirection == DOT1AG_MP_DIRECTION_UP?"UP":"DOWN"),
                       mepObjEntry->mepData->dot1agCfmMepPrimaryVid,
                       mepObjEntry->mepData->active);

    if (mepObjEntry->key.dot1agCfmMepIfIndex == intIfNum)
    {
      dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
      return OFDPA_E_NONE;
    }

    memcpy(&mepObjEntryKey, &mepObjEntry->key, sizeof(mepObjEntryKey));
    mepObjEntry = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjEntryKey, AVL_NEXT);
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);

  return OFDPA_E_FAIL;
}

void dot1agApsRcvCallBackRegister(void *callback)
{
  dot1agCFM.apsPduRcvCallBack = callback;
}

/*********************************************************************
* @purpose  Obtain maintenance end point on Y1731 PM
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731PmGet(uint32_t mdIndex, uint32_t maIndex,
                                  uint32_t mepId, uint32_t intIfNum)
{
  OFDPA_ERROR_t        rc  = OFDPA_E_FAIL;
  OFDPA_BOOL           ena = OFDPA_TRUE;
  uint32_t             pri;
  dot1agCfmMepObj_t   *mepObj;
  dot1agCfmStackObj_t *pMEPObj;

  mepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT);
  if ((mepObj == NULL) || 
      (mepObj->mepData == NULL))
  {
    return OFDPA_E_ERROR;
  }

  if (mepObj->mepData->y1731CfmMepProlmEntry != 0)
  {
    pMEPObj = mepObj->mepData->y1731CfmMepProlmEntry->pMPObj;
    if (pMEPObj == 0)
    {
      return OFDPA_E_ERROR;
    }

    dot1agMepCcmLtmPriorityGet(mdIndex, maIndex, mepId, intIfNum, &pri);
    ena = mepObj->mepData->y1731CfmMepProlmEntry->prolmEnabled;
    printf("\n MepId %d, intf %d, level %d, pri %d, proactive LM %d",
           (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0,
           pMEPObj->key.dot1agCfmStackifIndex,
           pMEPObj->key.dot1agCfmStackMdLevel,
           pri, ena);

    rc = OFDPA_E_NONE;
  }

  for (pri = 0; pri <= OFDPA_DOT1P_MAX_PRIORITY; pri++)
  {
    if (mepObj->mepData->y1731CfmMepDmEntry[pri] != 0)
    {
      pMEPObj = mepObj->mepData->y1731CfmMepDmEntry[pri]->pMPObj;
      if (pMEPObj == 0)
      {
        return OFDPA_E_ERROR;
      }

      ena = mepObj->mepData->y1731CfmMepDmEntry[pri]->dmEnabled;
      printf("\n MepId %d, intf %d, level %d, pri %d, proactive DM %d",
             (pMEPObj->mepData) ? (pMEPObj->mepData->dot1agCfmMepIdentifier) : 0,
             pMEPObj->key.dot1agCfmStackifIndex,
             pMEPObj->key.dot1agCfmStackMdLevel,
             pri, ena);

      rc = OFDPA_E_NONE;
    }
  }

  if (rc == OFDPA_E_NONE)
  {
    printf("\r\n");
  }

  return rc;
}

/*********************************************************************
* @purpose  Obtain maintenance end point on Y1731 PM
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731ProLmGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint32_t      pri;
  uint32_t      binMode;

  printf("\n\n =========================== LM BIN @ {%d, %d, %d, %d}: ====== \n",
         intIfNum, mdIndex, maIndex, mepId);
  dot1agMepCcmLtmPriorityGet(mdIndex, maIndex, mepId, intIfNum, &pri);
  for (binMode = 0; binMode < MAX_Y1731_BIN_MODE; binMode++)
  {
    y1731PmDumpLm(intIfNum, mdIndex, maIndex, mepId, pri, binMode);
  }

  return rc;
}

/*********************************************************************
* @purpose  Obtain maintenance end point on Y1731 PM
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731ProDmGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint32_t      pri;
  uint32_t      binMode;

  printf("\n\n =========================== DM BIN @ {%d, %d, %d, %d}: ====== \n",
         intIfNum, mdIndex, maIndex, mepId);
  for (pri = 0; pri <= OFDPA_DOT1P_MAX_PRIORITY; pri++)
  {
    for (binMode = 0; binMode < MAX_Y1731_BIN_MODE; binMode++)
    {
      y1731PmDumpDm(intIfNum, mdIndex, maIndex, mepId, pri, binMode);
    }
  }

  return rc;
}

/*********************************************************************
* @purpose  Retrieve Proactive LM counters.
*
* @param    mdIndex       @b{(input)} Index used for MD indentification
*                                     used by SNMP.
* @param    maIndex       @b{(input)} Index for MA identification used by
*                                     SNMP.
* @param    mepId         @b{(input)} Index to identify a MEP
* @param    intIfNum      @b{(input)} Interface on which mep is to be
*                                     created
* @param    proLmCounters @b{(output)} Proactive LM counters.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731ProLmCountersGet(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex,
                                             uint32_t mepId, uint32_t pri, y1731CfmMepProlmTable_t *proLmCounters)
{
  return y1731PmGetProLmCounters(intIfNum, mdIndex, maIndex, mepId, proLmCounters);
}

/*********************************************************************
* @purpose  Retrieve Proactive DM counters.
*
* @param    mdIndex       @b{(input)} Index used for MD indentification
*                                     used by SNMP.
* @param    maIndex       @b{(input)} Index for MA identification used by
*                                     SNMP.
* @param    mepId         @b{(input)} Index to identify a MEP
* @param    intIfNum      @b{(input)} Interface on which mep is to be
*                                     created
* @param    proDmCounters @b{(output)} Proactive DM counters.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731ProDmCountersGet(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex,
                                             uint32_t mepId, uint32_t pri, y1731CfmMepDmTable_t *proDmCounters)
{
  return y1731PmGetProDmCounters(intIfNum, mdIndex, maIndex, mepId, pri, proDmCounters);
}

/*********************************************************************
* @purpose  Retrieve OnDemand LM counters.
*
* @param    mdIndex       @b{(input)} Index used for MD indentification
*                                     used by SNMP.
* @param    maIndex       @b{(input)} Index for MA identification used by
*                                     SNMP.
* @param    mepId         @b{(input)} Index to identify a MEP
* @param    intIfNum      @b{(input)} Interface on which mep is to be
*                                     created
* @param    odLmCounters @b{(output)} OnDemand LM counters.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731OdLmCountersGet(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t pri, y1731CfmMepOndemlmTable_t *odLmCounters)
{
  return y1731PmGetOdLmCounters(intIfNum, mdIndex, maIndex, mepId, pri, odLmCounters);
}

/*********************************************************************
* @purpose  Retrieve OnDemand DM counters.
*
* @param    mdIndex       @b{(input)} Index used for MD indentification
*                                     used by SNMP.
* @param    maIndex       @b{(input)} Index for MA identification used by
*                                     SNMP.
* @param    mepId         @b{(input)} Index to identify a MEP
* @param    intIfNum      @b{(input)} Interface on which mep is to be
*                                     created
* @param    odDmCounters @b{(output)} onDemand DM counters.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731OdDmCountersGet(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t pri, y1731CfmMepDmTable_t *odDmCounters)
{
  return y1731PmGetOdDmCounters(intIfNum, mdIndex, maIndex, mepId, pri, odDmCounters);
}

/*********************************************************************
* @purpose  Retrieve next link trace records.
*
* @param    mdIndex        @b{(input)}  Index used for MD indentification
* @param    maIndex        @b{(input)}  Index for MA identification
* @param    mepId          @b{(input)}  Index to identify a MEP
* @param    ltrSeqNumber    @b{(input)} Index used to identify the
*                                       remote MEP which sent the LTR
* @param    ltrReceiveOrder @b{(input)} An index to distinguish
*                                       among multiple LTRs with the same
*                                       LTR Transaction Identifier field
*                                       value.
* @param    linkTraceReply @b{(output)} link trace session result.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepLtResultNextGet(uint32_t mdIndex, uint32_t maIndex, uint32_t mepId,
                                       uint32_t ltrSeqNumber, uint32_t ltrReceiveOrder,
                                       dot1agCfmLtrObj_t *linkTraceReply)
{
  return dot1agCfmLtrResultNextGet(mdIndex, maIndex, mepId, ltrSeqNumber, ltrReceiveOrder, linkTraceReply);
}

/*********************************************************************
* @purpose  Updating Client MP DB with the MEP created.
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agAddMepToClientMPDB(uint32_t intIfNum, uint32_t mdIndex,
                                       uint32_t maIndex, uint32_t mepId)
{
  dot1agCfmMepObj_t   *pServerMepObj = NULL;
  dot1agCfmStackObj_t *pServerMPObj = NULL;
  OFDPA_ERROR_t        rc = OFDPA_E_FAIL;

  pServerMepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT);

  if (pServerMepObj != NULL &&
      pServerMepObj->mepData != NULL)
  {
    pServerMPObj = dot1agCfmStackObjGet(intIfNum, pServerMepObj->mepData->dot1agCfmMepPrimaryVid,
                                        pServerMepObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                        pServerMepObj->mepData->dot1agCfmMepDirection, AVL_EXACT);
    if (pServerMPObj != NULL)
    {
      rc = dot1agCfmClientMPDBEntryAdd(pServerMPObj);
    }
  }
  return rc;
}

/*********************************************************************
* @purpose  Updating Client MP DB with the MIP created.
*
* @param    intIfNum     @b{(input)} Interface on which mep is to be
*                                    created/deleted
* @param    primVID      @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel      @b{(input)} MD level of the MD for which MIP
*                                    is to be created/deleted.
* @param    direction    @b{(input)} Direction of MP (Up or Down)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_FAIL
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agAddMipToClientMPDB(uint32_t intIfNum, uint32_t primVID, int mdLevel,
                                       Dot1agCfmMpDirection_t direction)
{

  dot1agCfmStackObj_t *pServerMPObj = NULL;
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;

  pServerMPObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel, direction, AVL_EXACT);

  if (pServerMPObj != NULL)
  {
    rc = dot1agCfmClientMPDBUpdate(pServerMPObj, OFDPA_FALSE);
  }

  return rc;
}

/*********************************************************************
* @purpose  Updating Client MP DB with the MEP deleted.
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agDeleteMepFromClientMPDB(uint32_t intIfNum, uint32_t mdIndex,
                                          uint32_t maIndex, uint32_t mepId)
{
  dot1agCfmMepObj_t   *pServerMepObj = NULL;
  dot1agCfmStackObj_t *pServerMPObj = NULL;
  OFDPA_ERROR_t        rc = OFDPA_E_FAIL;

  pServerMepObj = dot1agActiveMEPGet(mdIndex, maIndex, mepId, intIfNum, AVL_EXACT);

  if (pServerMepObj != NULL &&
      pServerMepObj->mepData != NULL)
  {
    pServerMPObj = dot1agCfmStackObjGet(intIfNum, pServerMepObj->mepData->dot1agCfmMepPrimaryVid,
                                        pServerMepObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                        pServerMepObj->mepData->dot1agCfmMepDirection, AVL_EXACT);
    if (pServerMPObj != NULL)
    {
      rc = dot1agCfmClientMPDBEntryDelete(pServerMPObj);
    }
  }
  return rc;
}

/*********************************************************************
* @purpose  Updating Client MP DB with the MIP deleted.
*
* @param    intIfNum     @b{(input)} Interface on which mep is to be
*                                    created/deleted
* @param    primVID      @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel      @b{(input)} MD level of the MD for which MIP
*                                    is to be created/deleted.
* @param    direction    @b{(input)} Direction of MP (Up or Down)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_FAIL
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agDeleteMipFromClientMPDB(uint32_t intIfNum, uint32_t primVID, int mdLevel,
                                       Dot1agCfmMpDirection_t direction)
{

  dot1agCfmStackObj_t *pServerMPObj = NULL;
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;

  pServerMPObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel, direction, AVL_EXACT);

  if (pServerMPObj != NULL)
  {
    rc = dot1agCfmClientMPDBUpdate(pServerMPObj, OFDPA_TRUE);
  }

  return rc;
}
