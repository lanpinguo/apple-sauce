/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2014
*
**********************************************************************
*
* @filename     ofdb_drop_api.c
*
* @purpose      Openflow Drop Status table.
*
* @component    OFDB
*
* @comments     none
*
* @create       11/18/2014
*
* @end
*
**********************************************************************/
#include <string.h>
#include <stdio.h>

#include "ofdb_api.h"
#include "ofdb.h"
#include "ofdpa_util.h"
#include "datapath.h"
#include "ofdpa_porting.h"

uint32_t ofdbDropStatusTableEntryCountGet(void)
{
  return ofdbAVLTrees.ofdbDropStatusTable_tree.count;
}

uint32_t ofdbDropStatusTableMaxCountGet(void)
{
  return dpaDropStatusTableEntryCountGet();
}

OFDPA_ERROR_t ofdbDropStatusAdd(ofdpaDropStatusEntry_t *dropEntry)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbDropStatusTable_node_t drop_node;
  ofdbDropStatusTable_node_t *dataPtr;

  memset(&drop_node, 0, sizeof(drop_node));
  memcpy(&drop_node.dropEntry, dropEntry, sizeof(ofdpaDropStatusEntry_t));

  dataPtr = avlInsertEntry (&ofdbAVLTrees.ofdbDropStatusTable_tree, &drop_node);

  if (dataPtr == &drop_node)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Drop Status Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate Entry is found.
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Drop Status Table entry already exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  return rc;
}

OFDPA_ERROR_t ofdbDropStatusDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbDropStatusTable_node_t drop_node;
  ofdbDropStatusTable_node_t *dataPtr;

  memset(&drop_node, 0, sizeof(drop_node));
  drop_node.dropEntry.lmepId = lmepId;

  dataPtr = avlDeleteEntry (&ofdbAVLTrees.ofdbDropStatusTable_tree, &drop_node);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Drop Status Table entry not found!\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbDropStatusGet(uint32_t lmepId, ofdpaDropStatusEntry_t *dropEntry)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbDropStatusTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbDropStatusTable_tree, &lmepId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy (dropEntry, &dataPtr->dropEntry, sizeof (ofdpaDropStatusEntry_t));
  }

  return rc;
}

OFDPA_ERROR_t ofdbDropStatusNextGet(uint32_t lmepId, ofdpaDropStatusEntry_t *nextDropEntry)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbDropStatusTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbDropStatusTable_tree, &lmepId, AVL_NEXT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy (nextDropEntry, &dataPtr->dropEntry, sizeof (ofdpaDropStatusEntry_t));
  }

  return rc;
}

OFDPA_ERROR_t ofdbDropStatusActionGet(uint32_t lmepId, uint8_t *dropAction)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbDropStatusTable_node_t *dataPtr;

  /* The default action on a Check Drop-Status action lookup miss is "Do not drop." */
  *dropAction = 0;
  dataPtr = avlSearch(&ofdbAVLTrees.ofdbDropStatusTable_tree, &lmepId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    *dropAction = dataPtr->dropEntry.dropAction;
  }

  return rc;
}

OFDPA_ERROR_t ofdbDropStatusEntryValidate(ofdpaDropStatusEntry_t *dropEntry)
{
  if (dropEntry == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Null dropEntry passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  /* Validate Drop Status type */
  if (dropEntry->type != OFDPA_DROP_TABLE_STATUS_LOCK)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Drop Status type is not valid. type = 0x%x\r\n", 
                       dropEntry->type);
    return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}
