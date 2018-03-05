/*********************************************************************
*
* Broadcom Proprietary and Confidential. (C) Copyright 2016 Broadcom.
* All rights reserved.
*
**********************************************************************
*
* @filename     ofdb_counters_api.c
*
* @purpose      Openflow tables related to class and color based counters.
*
* @component    OFDB
*
* @comments     none
*
* @create       09/13/2016
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
//#include "driver_counter.h"

uint32_t ofdbClassBasedCounterTableEntryCountGet(void)
{
  return ofdbAVLTrees.ofdbClassBasedCounterTable_tree.count;
}

uint32_t ofdbClassBasedCounterTableMaxCountGet(void)
{
  return ofdbClassBasedCounterTableStatus->tableMaxCount;
}

OFDPA_ERROR_t ofdbClassBasedCounterAdd(uint32_t counterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbClassBasedCounterTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.counterId = counterId;

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbClassBasedCounterTable_tree, &node);

  if (dataPtr == &node)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Table full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Table entry already exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  return rc;
}

OFDPA_ERROR_t ofdbClassBasedCounterDelete(uint32_t counterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbClassBasedCounterTable_node_t *dataPtr;

  dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbClassBasedCounterTable_tree, &counterId);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Entry not found.\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbClassBasedCounterGet(uint32_t counterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbClassBasedCounterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbClassBasedCounterTable_tree, &counterId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbClassBasedCounterIdNextGet(uint32_t counterId, uint32_t *nextCounterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbClassBasedCounterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbClassBasedCounterTable_tree, &counterId, AVL_NEXT);
  if (dataPtr != NULL)
  {
    *nextCounterId = dataPtr->counterId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbClassBasedCounterStatsGet(uint32_t counterId, ofdpaClassBasedCounterStats_t *stats)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbClassBasedCounterTable_node_t *dataPtr;
  int i;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbClassBasedCounterTable_tree, &counterId, AVL_EXACT);

  if (dataPtr != 0)
  {
    stats->refCount = dataPtr->refCount;

    if (dataPtr->refCount > 0)
    {
      rc = OFDPA_E_NONE;
    }
    else
    {
      /* Set the maximum field value (the unsigned equivalent of -1) */
      for (i = 0; i <= OFDPA_TRAFFIC_CLASS_MAX_VALUE; i++)
      {
        stats->tcCounter[i].byteCount = UINT64_MAX;
        stats->tcCounter[i].packetCount = UINT64_MAX;
      }
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbClassBasedCounterRefCountGet(uint32_t counterId, uint32_t *refCount)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbClassBasedCounterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbClassBasedCounterTable_tree, &counterId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (refCount != NULL)
    {
      *refCount = dataPtr->refCount;
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbClassBasedCounterReferenceUpdate(uint32_t counterId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbClassBasedCounterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbClassBasedCounterTable_tree, &counterId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                        "Incrementing reference count.\r\n", 0);
      dataPtr->refCount++;
    }
    else
    {
      if (dataPtr->refCount > 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                          "Decrementing reference count.\r\n", 0);
        dataPtr->refCount--;
      }
    }
  }

  return rc;
}

uint32_t ofdbColorBasedCounterTableEntryCountGet(void)
{
  return ofdbAVLTrees.ofdbColorBasedCounterTable_tree.count;
}

uint32_t ofdbColorBasedCounterTableMaxCountGet(void)
{
  return ofdbColorBasedCounterTableStatus->tableMaxCount;
}

OFDPA_ERROR_t ofdbColorBasedCounterAdd(uint32_t counterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbColorBasedCounterTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.counterId = counterId;

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbColorBasedCounterTable_tree, &node);

  if (dataPtr == &node)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Table full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Table entry already exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  return rc;
}

OFDPA_ERROR_t ofdbColorBasedCounterDelete(uint32_t counterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbColorBasedCounterTable_node_t *dataPtr;

  dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbColorBasedCounterTable_tree, &counterId);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Entry not found.\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbColorBasedCounterGet(uint32_t counterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbColorBasedCounterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbColorBasedCounterTable_tree, &counterId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbColorBasedCounterIdNextGet(uint32_t counterId, uint32_t *nextCounterId)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbColorBasedCounterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbColorBasedCounterTable_tree, &counterId, AVL_NEXT);
  if (dataPtr != NULL)
  {
    *nextCounterId = dataPtr->counterId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbColorBasedCounterStatsGet(uint32_t counterId, ofdpaColorBasedCounterStats_t *stats)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbColorBasedCounterTable_node_t *dataPtr;
  int i;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbColorBasedCounterTable_tree, &counterId, AVL_EXACT);

  if (dataPtr != 0)
  {
    stats->refCount = dataPtr->refCount;

    if (dataPtr->refCount > 0)
    {
      rc = OFDPA_E_NONE;
    }
    else
    {
      /* Set the maximum field value (the unsigned equivalent of -1) */
      for (i = 0; i < OFDPA_QOS_RESERVED; i++)
      {
        stats->colorCounter[i].byteCount = UINT64_MAX;
        stats->colorCounter[i].packetCount = UINT64_MAX;
      }
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbColorBasedCounterRefCountGet(uint32_t counterId, uint32_t *refCount)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbColorBasedCounterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbColorBasedCounterTable_tree, &counterId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (refCount != NULL)
    {
      *refCount = dataPtr->refCount;
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbColorBasedCounterReferenceUpdate(uint32_t counterId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbColorBasedCounterTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbColorBasedCounterTable_tree, &counterId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                        "Incrementing reference count.\r\n", 0);
      dataPtr->refCount++;
    }
    else
    {
      if (dataPtr->refCount > 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                          "Decrementing reference count.\r\n", 0);
        dataPtr->refCount--;
      }
    }
  }

  return rc;
}
