/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename     ofdb_remark_api.c
*
* @purpose      Openflow Remark Action table.
*
* @component    OFDB
*
* @comments     none
*
* @create       01/26/2015
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

uint32_t ofdbRemarkActionTableEntryCountGet(void)
{
  return ofdbAVLTrees.ofdbRemarkActionTable_tree.count;
}

uint32_t ofdbRemarkActionTableMaxCountGet(void)
{
  return dpaRemarkActionTableEntryCountGet();
}

OFDPA_ERROR_t ofdbRemarkActionAdd(ofdpaRemarkActionEntry_t *remarkEntry)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbRemarkActionTable_node_t remark_node;
  ofdbRemarkActionTable_node_t *dataPtr;

  memset(&remark_node, 0, sizeof(remark_node));
  memcpy(&remark_node.actions, &remarkEntry->actions, sizeof(ofdpaRemarkSetActions_t));
  remark_node.key.actionTableType = remarkEntry->actionTableType;
  remark_node.key.index = remarkEntry->index;
  remark_node.key.trafficClass = remarkEntry->trafficClass;
  remark_node.key.color = remarkEntry->color;

  dataPtr = avlInsertEntry (&ofdbAVLTrees.ofdbRemarkActionTable_tree, &remark_node);

  if (dataPtr == &remark_node)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Remark Action Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate Entry is found.
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Remark Action Table entry already exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  return rc;
}

OFDPA_ERROR_t ofdbRemarkActionDelete(ofdpaRemarkActionEntry_t *remarkEntry)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbRemarkActionTable_node_t remark_node;
  ofdbRemarkActionTable_node_t *dataPtr;

  memset(&remark_node, 0, sizeof(remark_node));
  remark_node.key.actionTableType = remarkEntry->actionTableType;
  remark_node.key.index = remarkEntry->index;
  remark_node.key.trafficClass = remarkEntry->trafficClass;
  remark_node.key.color = remarkEntry->color;

  dataPtr = avlDeleteEntry (&ofdbAVLTrees.ofdbRemarkActionTable_tree, &remark_node);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Remark Action Table entry not found!\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbRemarkActionEntryGet(ofdpaRemarkActionEntry_t *remarkEntry)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbRemarkActionTable_node_t *dataPtr;
  ofdbRemarkActionEntryKey_t remarkKey;

  if (remarkEntry == NULL)
  {
    return OFDPA_E_PARAM;
  }

  remarkKey.actionTableType = remarkEntry->actionTableType;
  remarkKey.index = remarkEntry->index;
  remarkKey.trafficClass = remarkEntry->trafficClass;
  remarkKey.color = remarkEntry->color;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbRemarkActionTable_tree, &remarkKey, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy (&remarkEntry->actions, &dataPtr->actions, sizeof(ofdpaRemarkSetActions_t));
  }

  return rc;
}

OFDPA_ERROR_t ofdbRemarkActionNextGet(ofdpaRemarkActionEntry_t *remarkEntry, ofdpaRemarkActionEntry_t *nextEntry)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbRemarkActionTable_node_t *dataPtr;
  ofdbRemarkActionEntryKey_t remarkKey;

  remarkKey.actionTableType = remarkEntry->actionTableType;
  remarkKey.index = remarkEntry->index;
  remarkKey.trafficClass = remarkEntry->trafficClass;
  remarkKey.color = remarkEntry->color;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbRemarkActionTable_tree, &remarkKey, AVL_NEXT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    nextEntry->actionTableType = dataPtr->key.actionTableType;
    nextEntry->index = dataPtr->key.index;
    nextEntry->color = dataPtr->key.color;
    nextEntry->trafficClass = dataPtr->key.trafficClass;
    memcpy (&nextEntry->actions, &dataPtr->actions, sizeof(ofdpaRemarkSetActions_t));
  }

  return rc;
}

OFDPA_ERROR_t ofdbRemarkActionEntryValidate(ofdpaRemarkActionEntry_t *remarkEntry)
{
  if (remarkEntry == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Null remarkEntry passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  /* Validate Action Table type */
  switch(remarkEntry->actionTableType)
  {
  case OFDPA_ACTION_TABLE_TYPE_MPLS_VPN_LABEL_REMARK:
  case OFDPA_ACTION_TABLE_TYPE_MPLS_TUNNEL_LABEL_REMARK:
    /* remarkData contains MPLS_TC value in case MPLS label table type */
    if (remarkEntry->actions.remarkData > OFDPA_MPLS_TC_MAX_VALUE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS_TC value out of range. MPLS_TC = %d\r\n",
                         remarkEntry->actions.remarkData);
      return OFDPA_E_PARAM;
    }
    break;
  default:
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Action Table type is not valid. actionTableType = %d\r\n", 
                         remarkEntry->actionTableType);
      return OFDPA_E_PARAM;
    }
  }

  /* Validate QOS color value */
  switch(remarkEntry->color)
  {
  case OFDPA_QOS_GREEN:
  case OFDPA_QOS_YELLOW:
  case OFDPA_QOS_RED:
    break;
  default:
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "QOS color value is not valid. color = %d\r\n", 
                         remarkEntry->color);
      return OFDPA_E_PARAM;
    }
  }

  /* Validate Traffic Class value */
  if (remarkEntry->trafficClass > OFDPA_TRAFFIC_CLASS_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "VLAN PCP value out of range. trafficClass = %d\r\n",
                       remarkEntry->trafficClass);
    return OFDPA_E_PARAM;
  }


  return OFDPA_E_NONE;
}
