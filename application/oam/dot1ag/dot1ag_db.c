/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
**********************************************************************
*
* @filename   dot1ag_db.c
*
* @purpose    Contains definitions to CFM database access procedures
*
* @component  DOT1AG/Connectivity & Fault Management
*
* @comments   none
*
* @create     16-Feb-2009
*
**********************************************************************/

#include "ofdpa_porting.h"
#include "dot1ag_exports.h"
#include "dot1ag_ctrl.h"
#include "dot1ag_cnfgr.h"
#include "dot1ag.h"
#include "dot1ag_db.h"

/*********************************************************************
* @purpose  Insert a node from AVL tree
*
* @param    dot1agDBId @b{(input)} enum to identify the tree
* @param    pTree      @b{(input)} Reference to the AVL tree whose node
*                                  is to be insert
* @param    pEntry     @b{(input)} Pointer to the node which needs to be
*                                  inserted
*
* @returns  OFDPA_E_NONE   Node inserted from AVL tree
* @returns  OFDPA_E_FAIL   Failed to insert the node
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agDBTreeEntryAdd(DOT1AG_DB_ID_t dot1agDBId, avlTree_t *pTree,
                                   void *pEntry)
{
  void          *pEntryDup = NULL;
  OFDPA_ERROR_t  rc        = OFDPA_E_NONE;

  if (pEntry == NULL)
  {
    return OFDPA_E_FAIL;
  }

  dpaSemTake(pTree->semId, DPA_SEM_FOREVER);

  pEntryDup = avlInsertEntry(pTree, pEntry);
  if (pEntryDup == pEntry)
  { /* no more room in table */
    rc = OFDPA_E_FULL;
  }
  else if (pEntryDup != NULL)
  { /* found existing entry */
    rc = OFDPA_E_EXISTS;
  }

  dpaSemGive(pTree->semId);

  return rc;
}

/*********************************************************************
* @purpose  Delete a node from AVL tree
*
* @param    dot1agDBId @b{(input)} enum to identify the tree
* @param    pTree      @b{(input)} Reference to the AVL tree whose node
*                                  is to be deleted
* @param    pEntry     @b{(input)} Pointer to the node which needs to be
*                                  deleted
*
* @returns  OFDPA_E_NONE   Node removed from AVL tree
* @returns  OFDPA_E_FAIL   Failed to delete the node
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agDBTreeEntryDelete(DOT1AG_DB_ID_t dot1agDBId, avlTree_t *pTree,
                                      void *pEntry)
{
  void *pData = NULL;

  dpaSemTake(pTree->semId, DPA_SEM_FOREVER);
  pData = avlDeleteEntry(pTree, pEntry);
  dpaSemGive(pTree->semId);

  if (pData == NULL)
  {
    /* Entry does not exist */
    return OFDPA_E_FAIL;
  }

  if (pData == pEntry)
  {
    /* Entry deleted */
    return OFDPA_E_NONE;
  }
  return OFDPA_E_FAIL;
}

/*********************************************************************
* @purpose  Obtain a valid MA object with search keys
*           mdIndex and maIndex
*
* @param    mdIndex    @b{(input)} Index value to Identify an MD
* @param    maIndex    @b{(input)} Index value to identify an MA. Use
*                                  NULL if search is by primVID
* @param    flag       @b{(input)} Use AVL_EXACT - Obtain exact matching
*                                                  entry
*                                      AVL_NEXT - Obtain next valid
*                                                 entry
*
*
* @returns  NULL         No such ma exists
* @returns  dot1agCfmMAObj_t * pointer to ma mib view object entry
*
* @notes
*
* @end
**********************************************************************/
dot1agCfmMAObj_t *dot1agActiveMAGet(uint32_t mdIndex, uint32_t maIndex,
                                    uint32_t flag)
{
  dot1agCfmMAObj_t    *maObjEntry = NULL;
  dot1agCfmMAObjKey_t  maObjEntryKey;

  memset(&maObjEntryKey, 0x0, sizeof(maObjEntryKey));
  maObjEntryKey.dot1agCfmMdIndex = mdIndex;
  maObjEntryKey.dot1agCfmMaIndex = maIndex;
  maObjEntry = avlSearch(&dot1agCFM.dot1agMATree, &maObjEntryKey, flag);
  return maObjEntry;
}

/*********************************************************************
* @purpose  Obtain a valid MA object with search keys
*           mdIndex and primVID
*
* @param    mdIndex    @b{(input)} Index value to Identify an MD
* @param    primVID    @b{(input)} Primary VID associated with this
*                                  MA.
* @param    flag       @b{(input)} Use AVL_EXACT - Obtain exact matching
*                                                  entry
*                                      AVL_NEXT - Obtain next valid
*                                                 entry
*
*
* @returns  NULL         No such ma exists
* @returns  dot1agCfmMAObjPrimVIDKey_t * pointer to ma primVID view
*                                        object entry
*
* @notes
*
* @end
**********************************************************************/
dot1agCfmMAObjPrimVID_t *dot1agActivePrimVIDMAGet(uint32_t mdIndex, uint32_t primVID,
                                                  uint32_t flag)
{
  dot1agCfmMAObjPrimVID_t    *maObjPrimVID = NULL;
  dot1agCfmMAObjPrimVIDKey_t  maObjPrimVIDKey;

  memset(&maObjPrimVIDKey, 0x0, sizeof(maObjPrimVIDKey));
  maObjPrimVIDKey.dot1agCfmMdIndex = mdIndex;
  maObjPrimVIDKey.dot1agCfmMaCompPrimaryVlanId = primVID;
  maObjPrimVID = avlSearch(&dot1agCFM.dot1agMAPrimVIDTree, &maObjPrimVIDKey, flag);
  return maObjPrimVID;
}

/*********************************************************************
* @purpose  Obtain a valid MEP object with search keys mdIndex, maIndex
*           mepId and intIfNum.
*
* @param    mdIndex    @b{(input)} Index value to Identify an MD
* @param    maIndex    @b{(input)} Index value to identify an MA
* @param    mepId      @b{(input)} Index value to identify an MEP
* @param    intIfNum   @b{(input)} Interface on which MEP is to be
*                                  found
* @param    flag       @b{(input)} Use AVL_EXACT - Obtain exact matching
*                                                  entry
*                                      AVL_NEXT - Obtain next valid
*                                                 entry
*
*
* @returns  NULL          No such MEP exists
* @returns  dot1agCfmMepObj_t * pointer to mep object entry
*
* @notes
*
* @end
**********************************************************************/
dot1agCfmMepObj_t *dot1agActiveMEPGet(uint32_t mdIndex, uint32_t maIndex, uint32_t mepId,
                                      uint32_t intIfNum, uint32_t flag)
{
  dot1agCfmMepObj_t    *mepObjEntry = NULL;
  dot1agCfmMepObjKey_t  mepObjEntryKey;

  memset(&mepObjEntryKey, 0x0, sizeof(mepObjEntryKey));
  mepObjEntryKey.dot1agCfmMdIndex       = mdIndex;
  mepObjEntryKey.dot1agCfmMaIndex       = maIndex;
  mepObjEntryKey.dot1agCfmMepIdentifier = mepId;
  mepObjEntryKey.dot1agCfmMepIfIndex    = intIfNum;

  mepObjEntry = avlSearch(&dot1agCFM.dot1agMepTree, &mepObjEntryKey, flag);
  return mepObjEntry;
}

/*********************************************************************
* @purpose  Obtain a valid domain with search keys mdIndex
*           or mdName or mdLevel in that order.
*
* @param    mdIndex    @b{(input)} Index value used by SNMP. Use
*                                  NULL for other UI's
* @param    mdName     @b{(input)} Maintenance Domain name. Can be
*                                  NULLPTR
* @param    mdLevel    @b{(input)} Maintenance level. Use -1 when
*                                  this is not search key
* @param    active     @b{(input)} state of the entry which u want to
*                                  search. Can be DOT1AG_ROW_ACTIVE or
*                                  DOT1AG_ROW_INACTIVE_PENDING
*
*
* @returns  NULL
* @returns  dot1agCfmMdTable_t * pointer to domain entry
*
* @notes    This API is not protected by r/w lock. Take config locks
*           before accessing this API
*
* @end
**********************************************************************/
dot1agCfmMdTable_t *dot1agActiveMdGet(uint32_t mdIndex, uint8_t *mdName,
                                      int mdLevel, uint32_t active)
{
  uint32_t            md_idx;
  dot1agCfmMdTable_t  *md = NULL;

  for (md_idx = 0; md_idx < dpaDot1agMaintenanceDomainsMaxGet(); md_idx++)
  {
    if ((DOT1AG_MD_CFG(md_idx).active == active) &&
        ((mdIndex != 0 && DOT1AG_MD_CFG(md_idx).dot1agCfmMdIndex == mdIndex) ||
         (mdName != NULL && strcmp(DOT1AG_MD_CFG(md_idx).dot1agCfmMdName, mdName) == 0) ||
         (mdLevel != -1 && DOT1AG_MD_CFG(md_idx).dot1agCfmMdMdLevel == mdLevel)))
    {
      md = &DOT1AG_MD_CFG(md_idx);
      break;
    }
  } /* End of domain loop */
  return md;
}

/*********************************************************************
* @purpose  Obtain a valid domain with search keys mdIndex
*           or mdLevel in that order.
*
* @param    mdIndex    @b{(input)} Index value used by SNMP. Use
*                                  NULL when this is not search key
* @param    mdLevel    @b{(input)} Maintenance level. Use -1 when
*                                  this is not search key
*
*
* @returns  NULL
* @returns  dot1agCfmMdTable_t * pointer to domain entry
*
* @notes    This API is not protected by r/w lock. Take config locks
*           before accessing this API
*
* @end
**********************************************************************/
dot1agCfmMdTable_t *dot1agMdEntryGet(uint32_t mdIndex,
                                     int mdLevel)
{
  uint32_t            md_idx;
  dot1agCfmMdTable_t  *md = NULL;

  for (md_idx = 0; md_idx < dpaDot1agMaintenanceDomainsMaxGet(); md_idx++)
  {
    if ((DOT1AG_MD_CFG(md_idx).active != DOT1AG_ROW_INACTIVE) &&
        ((mdIndex != 0 && DOT1AG_MD_CFG(md_idx).dot1agCfmMdIndex == mdIndex) ||
         (mdLevel != -1 && DOT1AG_MD_CFG(md_idx).dot1agCfmMdMdLevel == mdLevel)))
    {
      md = &DOT1AG_MD_CFG(md_idx);
      break;
    }
  } /* End of domain loop */
  return md;
}

/*********************************************************************
* @purpose  Obtain a valid domain LTR entry on a a given MEP
*
* @param    mdIndex    @b{(input)} Index value to Identify an MD
* @param    maIndex    @b{(input)} Index value to identify an MA
* @param    mepId      @b{(input)} Index value to identify an MEP
* @param    ltrSeqNumber      @b{(input)} Index value to identify an MEP
* @param    ltrReceiveOrder   @b{(input)} Index value to identify an MEP
* @param    flag       @b{(input)} Use AVL_EXACT - Obtain exact matching
*                                                  entry
*                                      AVL_NEXT - Obtain next valid
*                                                 entry
*
*
* @returns  NULL
* @returns  dot1agCfgmLtrObj_s * pointer to ltr entry
*
* @notes
*
* @end
**********************************************************************/
dot1agCfmLtrObj_t *dot1agCfmLtrObjGet(uint32_t mdIndex,
                                      uint32_t maIndex,
                                      uint32_t mepId,
                                      uint32_t ltrSeqNumber,
                                      uint32_t ltrReceiveOrder,
                                      uint32_t flag)
{
  dot1agCfmLtrObj_t    *ltrmepObjEntry = NULL;
  dot1agCfmLtrObjKey_t  ltrmepObjEntryKey;

  memset(&ltrmepObjEntryKey, 0x0, sizeof(ltrmepObjEntryKey));
  ltrmepObjEntryKey.dot1agCfmMdIndex         = mdIndex;
  ltrmepObjEntryKey.dot1agCfmMaIndex         = maIndex;
  ltrmepObjEntryKey.dot1agCfmMepIdentifier   = mepId;
  ltrmepObjEntryKey.dot1agCfmLtrSeqNumber    = ltrSeqNumber;
  ltrmepObjEntryKey.dot1agCfmLtrReceiveOrder = ltrReceiveOrder;

  ltrmepObjEntry = avlSearch(&dot1agCFM.dot1agLtrTree,
                             &ltrmepObjEntryKey, flag);
  return ltrmepObjEntry;
}

/*********************************************************************
* @purpose  Obtain a valid domain RMEP entry on a a given MEP
*
* @param    mdIndex    @b{(input)} Index value to Identify an MD
* @param    maIndex    @b{(input)} Index value to identify an MA
* @param    mepId      @b{(input)} Index value to identify an MEP
* @param    rmepId     @b{(input)} Index value to identify an RMEP
* @param    flag       @b{(input)} Use AVL_EXACT - Obtain exact matching
*                                                  entry
*                                      AVL_NEXT - Obtain next valid
*                                                 entry
*
*
* @returns  NULL
* @returns  dot1agCfgmLtrObj_s * pointer to ltr entry
*
* @notes
*
* @end
**********************************************************************/
dot1agCfmRMepObj_t *dot1agCfmRMepObjGet(uint32_t mdIndex,
                                        uint32_t maIndex,
                                        uint32_t mepId,
                                        uint32_t rmepId,
                                        uint32_t flag)
{
  dot1agCfmRMepObj_t    *rmepObjEntry = NULL;
  dot1agCfmRMepObjKey_t  rmepObjEntryKey;

  memset(&rmepObjEntryKey, 0x0, sizeof(rmepObjEntryKey));
  rmepObjEntryKey.dot1agCfmMdIndex       = mdIndex;
  rmepObjEntryKey.dot1agCfmMaIndex       = maIndex;
  rmepObjEntryKey.dot1agCfmMepIdentifier = mepId;
  rmepObjEntryKey.dot1agCfmMepDbRMepIdentifier = rmepId;

  rmepObjEntry = avlSearch(&dot1agCFM.dot1agRMepTree,
                           &rmepObjEntryKey, flag);
  return rmepObjEntry;
}

/*********************************************************************
* @purpose  Obtain a valid CFM Mainteance Point(MP)
*
* @param    intIfNum   @b{(input)} Interface on which MEP is to be
*                                  found
* @param    primVID    @b{(input)} Primary VLAN associated with the MA
* @param    mdLevel    @b{(input)} Level of the maintenance domain to
*                                  which this MP belongs to
* @param    direction  @b{(input)} Direction of the MP
* @param    flag       @b{(input)} Use AVL_EXACT - Obtain exact matching
*                                                  entry
*                                      AVL_NEXT - Obtain next valid
*                                                 entry
*
*
* @returns  NULL
* @returns  dot1agCfmStackObj_s * pointer to ltr entry
*
* @notes
*
* @end
**********************************************************************/
dot1agCfmStackObj_t *dot1agCfmStackObjGet(uint32_t intIfNum, uint16_t primVID,
                                          uint8_t mdLevel, Dot1agCfmMpDirection_t direction,
                                          uint32_t flag)
{
  dot1agCfmStackObj_t    *mpObjEntry = NULL;
  dot1agCfmStackObjKey_t  mpObjEntryKey;

  memset(&mpObjEntryKey, 0x0, sizeof(mpObjEntryKey));
  mpObjEntryKey.dot1agCfmStackifIndex      = intIfNum;
  mpObjEntryKey.dot1agCfmStackVlanIdOrNone = primVID;
  mpObjEntryKey.dot1agCfmStackMdLevel      = mdLevel;
  mpObjEntryKey.dot1agCfmStackDirection    = direction;

  mpObjEntry = avlSearch(&dot1agCFM.dot1agCfmStackTree,
                         &mpObjEntryKey, flag);
  return mpObjEntry;
}

/*********************************************************************
* @purpose  Obtain a valid CFM Mainteance End Point(MEP) if it exists
*
* @param    intIfNum   @b{(input)} Interface on which MEP is to be
*                                  found
* @param    primVID    @b{(input)} Primary VLAN associated with the MA
* @param    mdLevel    @b{(input)} Level of the maintenance domain to
*                                  which this MP belongs to
* @param    direction  @b{(input)} Direction of the MP. Optional and
*                                  can be NULL
*
*
* @returns  NULL
* @returns  dot1agCfmStackObj_s * pointer to StackObj entry
*
* @notes
*
* @end
**********************************************************************/
dot1agCfmStackObj_t *dot1agCfmMEPExists(uint32_t intIfNum,
                                        uint16_t primVID,
                                        uint8_t mdLevel,
                                        Dot1agCfmMpDirection_t direction)
{
  uint32_t                flag;
  dot1agCfmStackObj_t    *mpObjEntry = NULL;
  dot1agCfmStackObjKey_t  mpObjEntryKey;

  memset(&mpObjEntryKey, 0x0, sizeof(mpObjEntryKey));
  mpObjEntryKey.dot1agCfmStackifIndex      = intIfNum;
  mpObjEntryKey.dot1agCfmStackVlanIdOrNone = primVID;
  mpObjEntryKey.dot1agCfmStackMdLevel      = mdLevel;
  mpObjEntryKey.dot1agCfmStackDirection    = direction;

  flag = (direction == 0) ? AVL_NEXT : AVL_EXACT;

  mpObjEntry = avlSearch(&dot1agCFM.dot1agCfmStackTree,
                         &mpObjEntryKey, flag);
  if (flag == AVL_NEXT && mpObjEntry != NULL)
  {
    if (mpObjEntry->key.dot1agCfmStackifIndex != intIfNum ||
        mpObjEntry->key.dot1agCfmStackVlanIdOrNone != primVID ||
        mpObjEntry->key.dot1agCfmStackMdLevel != mdLevel)
    {
      return NULL;
    }
  }
  return mpObjEntry;
}

/*********************************************************************
* @purpose  Obtain a valid SLM LM counter entry for a given MEP-RMEP-TestId
*
* @param    lmepId    @b{(input)} Index value to Identify an LMEP
* @param    rmepId    @b{(input)} Index value to identify an RMEP
* @param    testId    @b{(input)} Index value to identify an TestId
* @param    flag       @b{(input)} Use AVL_EXACT - Obtain exact matching
*                                                  entry
*                                      AVL_NEXT - Obtain next valid
*                                                 entry
*
*
* @returns  NULL
* @returns  dot1agCfgmSlmObj_s * pointer to slm entry
*
* @notes
*
* @end
**********************************************************************/
dot1agCfmSlmObj_t *dot1agCfmSlmObjGet(uint32_t lmepId,
                                      uint32_t rmepId,
                                      uint32_t testId,
                                      uint32_t flag)
{
  dot1agCfmSlmObj_t    *slmObjEntry = NULL;
  dot1agCfmSlmObjKey_t  slmObjEntryKey;

  memset(&slmObjEntryKey, 0x0, sizeof(slmObjEntryKey));
  slmObjEntryKey.dot1agCfmLmepIdentifier = lmepId;
  slmObjEntryKey.dot1agCfmRmepIdentifier = rmepId;
  slmObjEntryKey.dot1agCfmTestId         = testId;

  slmObjEntry = avlSearch(&dot1agCFM.dot1agSlmTree,
                             &slmObjEntryKey, flag);
  return slmObjEntry;
}

/*********************************************************************
* @purpose  Obtain a valid Client MP DB entry
*
* @param    clientMPDBObjEntryKey @b{(input)} Client MP DB Key.
* @param    flag                  @b{(input)} Use AVL_EXACT - Obtain
*                                             exact matching entry
*                                             AVL_NEXT - Obtain next
*                                             valid entry
*
* @returns  NULL
* @returns  dot1agCfgmClientMPDBObj_s * pointer to Client MP DB entry
*
* @notes
*
* @end
**********************************************************************/
dot1agCfmClientMPDBObj_t *dot1agCfmClientMPDBObjGet(dot1agCfmClientMPDBObjKey_t  clientMPDBObjEntryKey,
                                                    uint32_t flag)
{
  dot1agCfmClientMPDBObj_t    *pClientMPDBObjEntry = NULL;
  dpaRWlockReadlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
  pClientMPDBObjEntry = avlSearch(&dot1agCFM.dot1agClientMPDBTree,
                                  &clientMPDBObjEntryKey, flag);
  dpaRWlockUnlock(&dot1agCFM.dot1agCfgClientMPDBRWLock);
  return pClientMPDBObjEntry;
}

