/*********************************************************************
 *
 * (C) Copyright Broadcom Corporation 2001-2016
 *
 **********************************************************************
 *
 * @filename ofdb_Groups_api.c
 *
 * @purpose Manage OF DPA Groups
 *
 * @component
 *
 * @comments contains the code to manage OF DPA Groups
 *
 * @create  03/25/2013
 *
 * @end
 *
 **********************************************************************/

#include <string.h>
#include <stdio.h>

#include "ofdb.h"
#include "ofdb_api.h"
#include "ofdpa_util.h"
#include "datapath.h"
#include "ofdpa_porting.h"
//#include "driver_util.h"

OFDPA_ERROR_t ofdbGroupAdd(ofdpaGroupEntry_t *group)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupTable_node_t group_node;
  ofdbGroupTable_node_t *dataPtr;

  memset(&group_node, 0, sizeof(group_node));

  memcpy(&group_node.group, group, sizeof(ofdpaGroupEntry_t));
  group_node.stats.groupAddUpTime = dpaUpTimeSeconds();

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbGroupTable_tree, &group_node);

  if (dataPtr == &group_node)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate Entry is found.
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Table entry already exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* New entry is added to the database.
    */
    ofdbGroupTableStatus->num_groups_in_database++;
    ofdbGroupTableStatus->groupTableEntryCount[OFDB_GROUP_TYPE(group->groupId)]++;

    if (OFDB_GROUP_TYPE(group->groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY)
    {
      ofdbTenantReferenceUpdate(OFDB_GROUP_TUNNELID(group->groupId), 1);
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupDelete(uint32_t groupId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupTable_node_t group_node;
  ofdbGroupTable_node_t *dataPtr;

  memset(&group_node, 0, sizeof(group_node));
  group_node.group.groupId = groupId;

  dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbGroupTable_tree, &group_node);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Table entry not found!\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }
  else
  {
    ofdbGroupTableStatus->num_groups_in_database--;
    ofdbGroupTableStatus->groupTableEntryCount[OFDB_GROUP_TYPE(groupId)]--;

    if (OFDB_GROUP_TYPE(groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY)
    {
      ofdbTenantReferenceUpdate(OFDB_GROUP_TUNNELID(groupId), 0);
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupGet(uint32_t groupId, ofdpaGroupEntry_t *group)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupTable_tree, &groupId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy(group, &dataPtr->group, sizeof(ofdpaGroupEntry_t));
  }

  return rc;
}


OFDPA_ERROR_t ofdbGroupDataUpdate(ofdpaGroupEntry_t *group)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupTable_tree, &group->groupId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy(&dataPtr->group,group, sizeof(ofdpaGroupEntry_t));
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupNextGet(uint32_t groupId, ofdpaGroupEntry_t *nextGroup)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupTable_tree, &groupId, AVL_NEXT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy(nextGroup, &dataPtr->group, sizeof(ofdpaGroupEntry_t));
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupTypeNextGet(uint32_t groupId,
                                   OFDPA_GROUP_ENTRY_TYPE_t groupType,
                                   ofdpaGroupEntry_t *nextGroup)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint8_t       typeArg = 0;

  if (OFDB_GROUP_TYPE(groupId) == groupType)
  {
    typeArg = 1;
  }

  while (rc == OFDPA_E_NONE)
  {
    rc = ofdbGroupNextGet(groupId, nextGroup);

    if (rc == OFDPA_E_NONE)
    {
      if (OFDB_GROUP_TYPE(nextGroup->groupId) == groupType)
      {
        break;
      }

      if (1 == typeArg)
      {
        rc = OFDPA_E_NOT_FOUND;
        break;
      }

      groupId = nextGroup->groupId;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupMPLSLabelSubTypeNextGet(uint32_t groupId,
                                               OFDPA_MPLS_LABEL_SUBTYPE_t subType,
                                               ofdpaGroupEntry_t *nextGroup)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint8_t       subTypeArg = 0;

  if (OFDB_GROUP_IS_MPLS_LABEL(groupId) && (OFDB_GROUP_MPLS_SUBTYPE(groupId) == subType))
  {
    subTypeArg = 1;
  }

  while (rc == OFDPA_E_NONE)
  {
    rc = ofdbGroupNextGet(groupId, nextGroup);

    if (rc == OFDPA_E_NONE)
    {
      if (OFDB_GROUP_IS_MPLS_LABEL(nextGroup->groupId) &&
          (OFDB_GROUP_MPLS_SUBTYPE(nextGroup->groupId) == subType))
      {
        break;
      }

      if (1 == subTypeArg)
      {
        rc = OFDPA_E_NOT_FOUND;
        break;
      }

      groupId = nextGroup->groupId;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupStatsGet(uint32_t groupId, ofdpaGroupEntryStats_t *groupStats)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupTable_node_t *dataPtr;
  ofdpaGroupEntryStats_t stats;
  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupTable_tree, &groupId, AVL_EXACT);

  if (dataPtr != 0)
  {

    groupStats->packet_count = 0;
    groupStats->byte_count = 0;
    groupStats->ptrGrpInst = dataPtr->group.ptrGrpInst ;
    groupStats->refCount = dataPtr->stats.refCount;
    groupStats->duration = dpaUpTimeSeconds() - dataPtr->stats.groupAddUpTime;
    groupStats->bucketCount = dataPtr->stats.bucketCount;
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupReferenceUpdate(uint32_t groupId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupTable_tree, &groupId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                        "Incrementing reference count!\r\n", 0);
      dataPtr->stats.refCount++;
    }
    else
    {
      if (dataPtr->stats.refCount > 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                          "Decrementing reference count!\r\n", 0);
        dataPtr->stats.refCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupBucketCountUpdate(uint32_t groupId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupTable_tree, &groupId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                        "Incrementing bucket count!\r\n", 0);
      dataPtr->stats.bucketCount++;
    }
    else
    {
      if (dataPtr->stats.bucketCount > 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                          "Decrementing bucket count!\r\n", 0);
        dataPtr->stats.bucketCount--;
      }
    }
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketRefEntryAdd(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupBucketRefTable_node_t bucket_node;
  ofdbGroupBucketRefTable_node_t *dataPtr;

  memset(&bucket_node, 0, sizeof(bucket_node));

  bucket_node.groupBucket.groupId = groupBucket->groupId;
  bucket_node.groupBucket.bucketIndex = groupBucket->bucketIndex;
  bucket_node.groupBucket.referenceGroupId = groupBucket->referenceGroupId;

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbGroupBucketRefTable_tree, &bucket_node);

  if (dataPtr == &bucket_node)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Bucket Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate Entry is found.
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Bucket entry already exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* New entry is added to the database.
    */
    ofdbGroupBucketCountUpdate(groupBucket->groupId, 1);
  }

  if (rc == OFDPA_E_NONE)
  {
    switch (OFDB_GROUP_TYPE(groupBucket->groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
      case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
        ofdbGroupReferenceUpdate(groupBucket->referenceGroupId, 1);
        break;

      default:
        break;
    }
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketDataEntryAdd(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupBucketDataTable_node_t bucket_node;
  ofdbGroupBucketDataTable_node_t *dataPtr;

  memset(&bucket_node, 0, sizeof(bucket_node));
  memcpy(&bucket_node.groupBucket, groupBucket, sizeof(ofdpaGroupBucketEntry_t));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree, &bucket_node);

  if (dataPtr == &bucket_node)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Bucket Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate Entry is found.
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Bucket entry already exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* New entry is added to the database.
    */
    ofdbGroupBucketCountUpdate(groupBucket->groupId, 1);
  }

  if (rc == OFDPA_E_NONE)
  {
    switch (OFDB_GROUP_TYPE(groupBucket->groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
      case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
      case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
      case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
        ofdbGroupReferenceUpdate(groupBucket->referenceGroupId, 1);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        {
        }
        break;

      default:
        break;
    }
  }

  /* update counter entry reference count */
  if (OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
  {
    switch (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->groupId))
    {
      case OFDPA_MPLS_L2_VPN_LABEL:
      case OFDPA_MPLS_L3_VPN_LABEL:
      case OFDPA_MPLS_TUNNEL_LABEL1:
      case OFDPA_MPLS_TUNNEL_LABEL2:
      case OFDPA_MPLS_SWAP_LABEL:
        if (groupBucket->bucketData.mplsLabel.colorBasedCountAction != 0)
        {
          ofdbColorBasedCounterReferenceUpdate(groupBucket->bucketData.mplsLabel.colorBasedCountId, 1);
        }
        break;
      case OFDPA_MPLS_INTERFACE:
        if (groupBucket->bucketData.mplsInterface.colorBasedCountAction != 0)
        {
          ofdbColorBasedCounterReferenceUpdate(groupBucket->bucketData.mplsInterface.colorBasedCountId, 1);
        }
        break;
      default:
        break;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupBucketEntryAdd(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  uint32_t  subType;

  switch (OFDB_GROUP_TYPE(groupBucket->groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      rc = _ofdbGroupBucketRefEntryAdd(groupBucket);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
      rc = _ofdbGroupBucketDataEntryAdd(groupBucket);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      subType = OFDB_GROUP_MPLS_SUBTYPE(groupBucket->groupId);

      switch (subType)
      {
        case OFDPA_MPLS_FAST_FAILOVER:
          rc = ofdbOamLogicalLivenessPortCreate(groupBucket->bucketData.mplsFastFailOver.watchPort, groupBucket->groupId);
          if (rc != OFDPA_E_NONE)
          {
            rc = OFDPA_E_FAIL;
          }
          else
          {
            rc = _ofdbGroupBucketDataEntryAdd(groupBucket);
          }
          break;
        case OFDPA_MPLS_L2_TAG:
          rc = _ofdbGroupBucketDataEntryAdd(groupBucket);
          break;
        case OFDPA_MPLS_L2_FLOOD:
        case OFDPA_MPLS_L2_MULTICAST:
        case OFDPA_MPLS_L2_LOCAL_FLOOD:
        case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
        case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
        case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        case OFDPA_MPLS_ECMP:
          rc = _ofdbGroupBucketRefEntryAdd(groupBucket);
          break;
        default:
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Invalid MPLS Group Subtype! SubType = 0x%x \r\n",
                         subType);
          rc = OFDPA_E_PARAM;
      }
      break;

    default:
      break;
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketRefEntryDelete(uint32_t groupId, uint32_t bucketIndex)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupBucketRefTable_node_t bucketNode;
  ofdbGroupBucketRefTable_node_t *dataPtr;

  bucketNode.groupBucket.groupId = groupId;
  bucketNode.groupBucket.bucketIndex = bucketIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketRefTable_tree, &bucketNode, AVL_EXACT);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Bucket entry not found!\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }
  else
  {
    switch (OFDB_GROUP_TYPE(dataPtr->groupBucket.groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
      case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
        ofdbGroupReferenceUpdate(dataPtr->groupBucket.referenceGroupId, 0);
        break;

      default:
        break;
    }

    dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbGroupBucketRefTable_tree, &bucketNode);

    ofdbGroupBucketCountUpdate(groupId, 0);
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketDataEntryDelete(uint32_t groupId, uint32_t bucketIndex)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupBucketDataTable_node_t bucketNode;
  ofdbGroupBucketDataTable_node_t *dataPtr;

  bucketNode.groupBucket.groupId = groupId;
  bucketNode.groupBucket.bucketIndex = bucketIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree, &bucketNode, AVL_EXACT);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Bucket entry not found!\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }
  else
  {
    /* update counter entry reference count */
    if (OFDB_GROUP_TYPE(dataPtr->groupBucket.groupId) == OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
    {
      switch (OFDB_GROUP_MPLS_SUBTYPE(dataPtr->groupBucket.groupId))
      {
        case OFDPA_MPLS_L2_VPN_LABEL:
        case OFDPA_MPLS_L3_VPN_LABEL:
        case OFDPA_MPLS_TUNNEL_LABEL1:
        case OFDPA_MPLS_TUNNEL_LABEL2:
        case OFDPA_MPLS_SWAP_LABEL:
          if (dataPtr->groupBucket.bucketData.mplsLabel.colorBasedCountAction != 0)
          {
            ofdbColorBasedCounterReferenceUpdate(dataPtr->groupBucket.bucketData.mplsLabel.colorBasedCountId, 0);
          }
          break;
        case OFDPA_MPLS_INTERFACE:
          if (dataPtr->groupBucket.bucketData.mplsInterface.colorBasedCountAction != 0)
          {
            ofdbColorBasedCounterReferenceUpdate(dataPtr->groupBucket.bucketData.mplsInterface.colorBasedCountId, 0);
          }
        default:
          break;
      }
    }

    switch (OFDB_GROUP_TYPE(dataPtr->groupBucket.groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
      case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
      case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
        ofdbGroupReferenceUpdate(dataPtr->groupBucket.referenceGroupId, 0);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
        {
          uint32_t subType;

          ofdbGroupReferenceUpdate(dataPtr->groupBucket.referenceGroupId, 0);

          subType = OFDB_GROUP_MPLS_SUBTYPE(dataPtr->groupBucket.groupId);
          if (subType == OFDPA_MPLS_FAST_FAILOVER)
          {
            rc = ofdbPortDelete(dataPtr->groupBucket.bucketData.mplsFastFailOver.watchPort);
            if (rc != OFDPA_E_NONE)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                "Error from ofdbPortDelete for FF watchport 0x%x, rc = %d\r\n",
                                 dataPtr->groupBucket.bucketData.mplsFastFailOver.watchPort, rc);
            }
          }
        }
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        {
        }
        break;

      default:
        break;
    }

    dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree, &bucketNode);

    ofdbGroupBucketCountUpdate(groupId, 0);
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupBucketEntryDelete(uint32_t groupId, uint32_t bucketIndex)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  uint32_t subType;

  switch (OFDB_GROUP_TYPE(groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      rc = _ofdbGroupBucketRefEntryDelete(groupId, bucketIndex);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
      rc = _ofdbGroupBucketDataEntryDelete(groupId, bucketIndex);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      subType = OFDB_GROUP_MPLS_SUBTYPE(groupId);

      switch (subType)
      {
        case OFDPA_MPLS_FAST_FAILOVER:
        case OFDPA_MPLS_L2_TAG:
          rc = _ofdbGroupBucketDataEntryDelete(groupId, bucketIndex);
          break;
        case OFDPA_MPLS_L2_FLOOD:
        case OFDPA_MPLS_L2_MULTICAST:
        case OFDPA_MPLS_L2_LOCAL_FLOOD:
        case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
        case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
        case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        case OFDPA_MPLS_ECMP:
          rc = _ofdbGroupBucketRefEntryDelete(groupId, bucketIndex);
          break;
        default:
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Invalid MPLS Group Subtype!\r\n", 0);
          rc = OFDPA_E_PARAM;
      }
      break;

    default:
      break;
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketRefEntryGet(uint32_t groupId, uint32_t bucketIndex,
                             ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupBucketRefTable_node_t *dataPtr;
  ofdbGroupBucketRefTable_node_t bucketNode;

  bucketNode.groupBucket.groupId = groupId;
  bucketNode.groupBucket.bucketIndex = bucketIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketRefTable_tree, &bucketNode, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    groupBucket->groupId = dataPtr->groupBucket.groupId;
    groupBucket->bucketIndex = dataPtr->groupBucket.bucketIndex;
    groupBucket->referenceGroupId = dataPtr->groupBucket.referenceGroupId;
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketDataEntryGet(uint32_t groupId, uint32_t bucketIndex,
                             ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupBucketDataTable_node_t *dataPtr;
  ofdbGroupBucketDataTable_node_t bucketNode;

  bucketNode.groupBucket.groupId = groupId;
  bucketNode.groupBucket.bucketIndex = bucketIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree, &bucketNode, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy(groupBucket, &dataPtr->groupBucket, sizeof(ofdpaGroupBucketEntry_t));
  }

  return rc;
}


OFDPA_ERROR_t ofdbGroupBucketEntryGet(uint32_t groupId, uint32_t bucketIndex,
                             ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  uint32_t subType;

  switch (OFDB_GROUP_TYPE(groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      rc = _ofdbGroupBucketRefEntryGet(groupId, bucketIndex, groupBucket);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
      rc = _ofdbGroupBucketDataEntryGet(groupId, bucketIndex, groupBucket);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      subType = OFDB_GROUP_MPLS_SUBTYPE(groupId);

      switch (subType)
      {
        case OFDPA_MPLS_FAST_FAILOVER:
        case OFDPA_MPLS_L2_TAG:
          rc = _ofdbGroupBucketDataEntryGet(groupId, bucketIndex, groupBucket);
          break;
        case OFDPA_MPLS_L2_FLOOD:
        case OFDPA_MPLS_L2_MULTICAST:
        case OFDPA_MPLS_L2_LOCAL_FLOOD:
        case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
        case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
        case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        case OFDPA_MPLS_ECMP:
          rc = _ofdbGroupBucketRefEntryGet(groupId, bucketIndex, groupBucket);
          break;
        default:
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Invalid MPLS Group Subtype!\r\n", 0);
          rc = OFDPA_E_PARAM;
      }
      break;

    default:
      break;
  }

  return rc;
}


static
OFDPA_ERROR_t _ofdbGroupBucketRefEntryModify(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupBucketRefTable_node_t *dataPtr;
  ofdbGroupBucketRefTable_node_t bucketNode;

  bucketNode.groupBucket.groupId = groupBucket->groupId;
  bucketNode.groupBucket.bucketIndex = groupBucket->bucketIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketRefTable_tree, &bucketNode, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;

    if (groupBucket->referenceGroupId != dataPtr->groupBucket.referenceGroupId)
    {
      ofdbGroupReferenceUpdate(groupBucket->referenceGroupId, 1);
      ofdbGroupReferenceUpdate(dataPtr->groupBucket.referenceGroupId, 0);
    }

    memset(&dataPtr->groupBucket, 0, sizeof(ofdbGroupBucketRefEntry_t));

    dataPtr->groupBucket.groupId = groupBucket->groupId;
    dataPtr->groupBucket.bucketIndex = groupBucket->bucketIndex;
    dataPtr->groupBucket.referenceGroupId = groupBucket->referenceGroupId;
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketDataEntryModify(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
 ofdbGroupBucketDataTable_node_t *dataPtr;
 ofdbGroupBucketDataTable_node_t bucketNode;

  bucketNode.groupBucket.groupId = groupBucket->groupId;
  bucketNode.groupBucket.bucketIndex = groupBucket->bucketIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree, &bucketNode, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;

    if (groupBucket->referenceGroupId != dataPtr->groupBucket.referenceGroupId)
    {
      ofdbGroupReferenceUpdate(groupBucket->referenceGroupId, 1);
      ofdbGroupReferenceUpdate(dataPtr->groupBucket.referenceGroupId, 0);
    }

    memset(&dataPtr->groupBucket, 0, sizeof(ofdpaGroupBucketEntry_t));
    memcpy(&dataPtr->groupBucket, groupBucket, sizeof(ofdpaGroupBucketEntry_t));
  }

  return rc;
}



OFDPA_BOOL ofdbGroupBucketDataEntryIsNeedUpdated(ofdpaGroupBucketEntry_t *groupBucket )
{
  OFDPA_BOOL rc = OFDPA_FALSE;
  ofdbGroupBucketDataTable_node_t *dataPtr;
  ofdbGroupBucketDataTable_node_t bucketNode;
  int result = 0;
  bucketNode.groupBucket.groupId = groupBucket->groupId;
  bucketNode.groupBucket.bucketIndex = groupBucket->bucketIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree, &bucketNode, AVL_EXACT);

  if (dataPtr != NULL)
  {
    printf("ofdbGroupBucketDataEntryIsNeedUpdated : bucketIndex=%d \r\n",groupBucket->bucketIndex);
    result = memcmp(&dataPtr->groupBucket, groupBucket, sizeof(ofdpaGroupBucketEntry_t));
    if(result != 0)
    {
      rc = OFDPA_TRUE;
    }
  }

  printf(" result=%d rc=%d\r\n",result,rc);
  return rc;
}

OFDPA_ERROR_t ofdbGroupBucketEntryModify(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;

  switch (OFDB_GROUP_TYPE(groupBucket->groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      rc = _ofdbGroupBucketRefEntryModify(groupBucket);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      rc = _ofdbGroupBucketDataEntryModify(groupBucket);
      break;

    default:
      break;
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketRefEntryFirstGet(uint32_t groupId,
                                           ofdpaGroupBucketEntry_t *firstGroupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbGroupBucketRefTable_node_t *dataPtr;
  ofdbGroupBucketRefTable_node_t bucketNode;

  bucketNode.groupBucket.groupId = groupId;
  bucketNode.groupBucket.bucketIndex = 0;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketRefTable_tree, &bucketNode, AVL_EXACT);

  if (dataPtr == NULL)
  {
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketRefTable_tree, &bucketNode, AVL_NEXT);
  }

  if ((dataPtr != NULL) && (groupId == dataPtr->groupBucket.groupId))
  {
    rc = OFDPA_E_NONE;

    firstGroupBucket->groupId = dataPtr->groupBucket.groupId;
    firstGroupBucket->bucketIndex = dataPtr->groupBucket.bucketIndex;
    firstGroupBucket->referenceGroupId = dataPtr->groupBucket.referenceGroupId;
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketDataEntryFirstGet(uint32_t groupId,
                                           ofdpaGroupBucketEntry_t *firstGroupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbGroupBucketDataTable_node_t *dataPtr;
  ofdbGroupBucketDataTable_node_t bucketNode;

  bucketNode.groupBucket.groupId = groupId;
  bucketNode.groupBucket.bucketIndex = 0;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree, &bucketNode, AVL_EXACT);

  if (dataPtr == NULL)
  {
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree, &bucketNode, AVL_NEXT);
  }

  if ((dataPtr != NULL) && (groupId == dataPtr->groupBucket.groupId))
  {
    rc = OFDPA_E_NONE;
    memcpy(firstGroupBucket, &dataPtr->groupBucket, sizeof(ofdpaGroupBucketEntry_t));
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupBucketEntryFirstGet(uint32_t groupId,
                                           ofdpaGroupBucketEntry_t *firstGroupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  uint32_t subType;

  switch (OFDB_GROUP_TYPE(groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      rc = _ofdbGroupBucketRefEntryFirstGet(groupId, firstGroupBucket);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
      rc = _ofdbGroupBucketDataEntryFirstGet(groupId, firstGroupBucket);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      subType = OFDB_GROUP_MPLS_SUBTYPE(groupId);

      switch (subType)
      {
        case OFDPA_MPLS_FAST_FAILOVER:
        case OFDPA_MPLS_L2_TAG:
          rc = _ofdbGroupBucketDataEntryFirstGet(groupId, firstGroupBucket);
          break;
        case OFDPA_MPLS_L2_FLOOD:
        case OFDPA_MPLS_L2_MULTICAST:
        case OFDPA_MPLS_L2_LOCAL_FLOOD:
        case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
        case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
        case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        case OFDPA_MPLS_ECMP:
          rc = _ofdbGroupBucketRefEntryFirstGet(groupId, firstGroupBucket);
          break;
        default:
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Invalid MPLS Group Subtype!\r\n", 0);
          rc = OFDPA_E_PARAM;
      }
      break;

    default:
      break;
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketRefEntryNextGet(uint32_t groupId, uint32_t bucketIndex,
                                 ofdpaGroupBucketEntry_t *nextGroupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbGroupBucketRefTable_node_t *dataPtr;
  ofdbGroupBucketRefTable_node_t bucketNode;

  bucketNode.groupBucket.groupId = groupId;
  bucketNode.groupBucket.bucketIndex = bucketIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketRefTable_tree, &bucketNode, AVL_NEXT);

  if ((dataPtr != NULL) && (groupId == dataPtr->groupBucket.groupId))
  {
    rc = OFDPA_E_NONE;

    nextGroupBucket->groupId = dataPtr->groupBucket.groupId;
    nextGroupBucket->bucketIndex = dataPtr->groupBucket.bucketIndex;
    nextGroupBucket->referenceGroupId = dataPtr->groupBucket.referenceGroupId;
  }

  return rc;
}

static
OFDPA_ERROR_t _ofdbGroupBucketDataEntryNextGet(uint32_t groupId, uint32_t bucketIndex,
                                 ofdpaGroupBucketEntry_t *nextGroupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbGroupBucketDataTable_node_t *dataPtr;
  ofdbGroupBucketDataTable_node_t bucketNode;

  bucketNode.groupBucket.groupId = groupId;
  bucketNode.groupBucket.bucketIndex = bucketIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree, &bucketNode, AVL_NEXT);

  if ((dataPtr != NULL) && (groupId == dataPtr->groupBucket.groupId))
  {
    rc = OFDPA_E_NONE;
    memcpy(nextGroupBucket, &dataPtr->groupBucket, sizeof(ofdpaGroupBucketEntry_t));
  }

  return rc;
}


OFDPA_ERROR_t ofdbGroupBucketEntryNextGet(uint32_t groupId, uint32_t bucketIndex,
                                 ofdpaGroupBucketEntry_t *nextGroupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  uint32_t subType;

  switch (OFDB_GROUP_TYPE(groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      rc = _ofdbGroupBucketRefEntryNextGet(groupId, bucketIndex, nextGroupBucket);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
      rc = _ofdbGroupBucketDataEntryNextGet(groupId, bucketIndex, nextGroupBucket);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      subType = OFDB_GROUP_MPLS_SUBTYPE(groupId);

      switch (subType)
      {
        case OFDPA_MPLS_FAST_FAILOVER:
        case OFDPA_MPLS_L2_TAG:
          rc = _ofdbGroupBucketDataEntryNextGet(groupId, bucketIndex, nextGroupBucket);
          break;
        case OFDPA_MPLS_L2_FLOOD:
        case OFDPA_MPLS_L2_MULTICAST:
        case OFDPA_MPLS_L2_LOCAL_FLOOD:
        case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
        case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
        case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        case OFDPA_MPLS_ECMP:
          rc = _ofdbGroupBucketRefEntryNextGet(groupId, bucketIndex, nextGroupBucket);
          break;
        default:
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Invalid MPLS Group Subtype!\r\n", 0);
          rc = OFDPA_E_PARAM;
      }
      break;

    default:
      break;
  }

  return rc;
}

OFDPA_ERROR_t ofdbGroupBucketValidate(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc;
  OFDB_ENTRY_FLAG_t flags;
  ofdpaGroupBucketEntry_t nextBucketEntry, tmpBucketEntry, workingGroupBucket;
  ofdpaFlowEntry_t aclFlow;
  int bucketIndex;
  uint32_t tmpVlanId;
  uint32_t tunnelId;
  ofdbPortInfo_t portInfo;
  uint32_t subType;

  if (groupBucket == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Null Group Bucket passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  if (OFDB_GROUP_TYPE(groupBucket->groupId) >= OFDPA_GROUP_ENTRY_TYPE_LAST)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Referenced group ID not valid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  if ((OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE)    ||
      (OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST)    ||
      (OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST)  ||
      (OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD)      ||
      (OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE)  ||
      (OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST)  ||
      (OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_L3_ECMP)       ||
      (OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)  ||
      (OFDB_GROUP_TYPE(groupBucket->groupId) == OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING))
  {
    /* Validate referenced Group Entry. Also check that the referenced group entry
       should have at least one bucket */

    rc = ofdbRefGroupValidate(groupBucket->referenceGroupId);

    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                        "Referenced Group not valid!\r\n", 0);
      return OFDPA_E_NOT_FOUND;
    }
  }

  switch (OFDB_GROUP_TYPE(groupBucket->groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
      /* The port id component of the groupId entry must match the
         Set Field value for output port id. */
      if (groupBucket->bucketData.l2Interface.outputPort != OFDB_GROUP_PORTID(groupBucket->groupId))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Port in Group Id does not match set field Port!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      if (groupBucket->bucketData.l2Interface.allowVlanTranslation != 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "L2 Interface Group bucket must set allowVlanTranslation "
                          "to 0. (allowVlanTranslation == %d)\r\n",
                           groupBucket->bucketData.l2Interface.allowVlanTranslation);
        return OFDPA_E_PARAM;
      }

      /* Validate pop VLAN */
      if ((groupBucket->bucketData.l2Interface.popVlanTag != 0) &&
          (groupBucket->bucketData.l2Interface.popVlanTag != 1))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Pop VLAN Tag should be 0 or 1!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
      /* validate source MAC: groupBucket->bucketData.l2Rewrite.dstMac ?? */

      /* validate source MAC: groupBucket->bucketData.l2Rewrite.srcMac */
      if (groupBucket->bucketData.l2Rewrite.srcMac.addr[0] & 0x01)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Source MAC not Unicast!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      /* Referenced Group Entry should be of type L2 Interface */
      if (OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Referenced Group Id not of type L2 Interface!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      if (groupBucket->bucketData.l2Rewrite.vlanId != 0)
      {
        /* Set Field value for VLAN Id should have OFDPA_VID_PRESENT. */
        if ((groupBucket->bucketData.l2Rewrite.vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                            "Set field VLAN does not have OFDPA_VID_PRESENT set.\r\n", 0);
          return OFDPA_E_PARAM;
        }
        /* The VLAN Id component of the referenced group entry must match the
           Set Field value for VLAN Id. */
        if ((groupBucket->bucketData.l2Rewrite.vlanId & OFDPA_VID_EXACT_MASK) != OFDB_GROUP_VLANID(groupBucket->referenceGroupId))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                            "VLAN in Referenced Group Id does not match set field VLAN.\r\n", 0);
          return OFDPA_E_PARAM;
        }
      }
      else
      {
        /*
         * VLAN ID is not rewritten, ensure that any Policy ACL flows that refer to this
         * group have a VLAN ID match that is the same as the L2 Interface VLAN ID.
         */
        tmpVlanId = OFDB_GROUP_VLANID(groupBucket->referenceGroupId);

        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_ACL_POLICY, &aclFlow);

        while (OFDPA_E_NONE == ofdbFlowNextGet(&aclFlow, &aclFlow, NULL, NULL))
        {
          if ((groupBucket->groupId == aclFlow.flowData.policyAclFlowEntry.groupID) &&
              (aclFlow.flowData.policyAclFlowEntry.match_criteria.vlanIdMask != OFDPA_VID_FIELD_MASK) &&
              ((aclFlow.flowData.policyAclFlowEntry.match_criteria.vlanId & OFDPA_VID_EXACT_MASK) != tmpVlanId))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "VLAN in Referenced L2 Interface Group does not match Policy ACL VLAN.\r\n", 0);
            return OFDPA_E_PARAM;
          }
        }
      }

      break;

    case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
      /* validate source MAC: groupBucket->bucketData.l3Interface.srcMac ?? */

      if (groupBucket->bucketData.l3Interface.srcMac.addr[0] & 0x01)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Source MAC not Unicast!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      /* Referenced Group Entry should be of type L2 Interface */
      if (OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Referenced Group Id not of type L2 Interface!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      /* Set Field value for VLAN Id should have OFDPA_VID_PRESENT. */
      if ((groupBucket->bucketData.l3Interface.vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Set field VLAN does not have OFDPA_VID_PRESENT set.\r\n", 0);
        return OFDPA_E_PARAM;
      }
      /* The VLAN Id component of the referenced group entry must match the
         Set Field value for VLAN Id. */
      if ((groupBucket->bucketData.l3Interface.vlanId & OFDPA_VID_EXACT_MASK) != OFDB_GROUP_VLANID(groupBucket->referenceGroupId))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "VLAN in Referenced Group Id does not match set field VLAN!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      break;

    case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:

      /* Referenced Group Entry should be of type L2 Interface or L2 Unfiltered Interface */
      if ((OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) &&
          (OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Referenced Group Id not of type L2 Interface or L2 Unfiltered Interface!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      /* Set Field value for VLAN Id should have OFDPA_VID_PRESENT. */
      if ((groupBucket->bucketData.l3Unicast.vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Set field VLAN does not have OFDPA_VID_PRESENT set.\r\n", 0);
        return OFDPA_E_PARAM;
      }
      /*
       * The VLAN Id component of the referenced group entry must match the Set Field
       * value for VLAN Id if, and only if, the referenced group is of type L2 Interface.
       */
      if ((OFDB_GROUP_TYPE(groupBucket->referenceGroupId) == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) &&
          ((groupBucket->bucketData.l3Unicast.vlanId & OFDPA_VID_EXACT_MASK) != OFDB_GROUP_VLANID(groupBucket->referenceGroupId)))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "VLAN in Referenced Group Id does not match set field VLAN!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      /* validate destination MAC: groupBucket->bucketData.l3Unicast.dstMac */
      if (groupBucket->bucketData.l3Unicast.dstMac.addr[0] & 0x01)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Destination MAC not Unicast!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      /* validate source MAC: groupBucket->bucketData.l3Unicast.srcMac */
      if (groupBucket->bucketData.l3Unicast.srcMac.addr[0] & 0x01)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Source MAC not Unicast!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:

      /* Referenced Group Entry should be of type L2 Interface */
      if (OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Referenced Group Id not of type L2 Interface!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      /* The VLAN id component of the referenced group entry must match the
         VLAN id of the Bucket entry. */
      if (OFDB_GROUP_VLANID(groupBucket->groupId) != OFDB_GROUP_VLANID(groupBucket->referenceGroupId))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "VLAN in Referenced Group Id does not match VLAN in Group Id!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
      /* Validate referenced Group Entry */

      /* Referenced Group Entry should be of type L2 Interface */
      if (OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Referenced Group Id not of type L2 Interface!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      /* The VLAN id component of the referenced group entry must match the
         VLAN id of the Bucket entry. */
      if (OFDB_GROUP_VLANID(groupBucket->groupId) != OFDB_GROUP_VLANID(groupBucket->referenceGroupId))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "VLAN in Referenced Group Id does not match VLAN in Group Id!\r\n", 0);
        return OFDPA_E_PARAM;
      }

      break;

    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:

      /* Referenced Group Entry should be of type L2/L3 Interface or MPLS L3 VPN */
      if (OFDB_GROUP_TYPE(groupBucket->referenceGroupId) == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
      {
        /* The VLAN id component of the referenced group entry must match the
           VLAN id of the Bucket entry. */
        if (OFDB_GROUP_VLANID(groupBucket->groupId) != OFDB_GROUP_VLANID(groupBucket->referenceGroupId))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                            "VLAN in Referenced L2 Group Id does not match VLAN in Group Id!\r\n", 0);
          return OFDPA_E_PARAM;
        }
      }
      else if (OFDB_GROUP_TYPE(groupBucket->referenceGroupId) == OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE)
      {
        /* The VLAN id component of the referenced group entry must be different from
           the VLAN id of the Bucket entry. */

        /* Get the bucket entry from the bucket table of the reference l3 interface group */
        memset(&tmpBucketEntry, 0, sizeof(tmpBucketEntry));
        (void)ofdbGroupBucketEntryGet(groupBucket->referenceGroupId, 0, &tmpBucketEntry);

        /* set field vlan of l3 interface group */
        tmpVlanId = (tmpBucketEntry.bucketData.l3Interface.vlanId & OFDPA_VID_EXACT_MASK);

        if (OFDB_GROUP_VLANID(groupBucket->groupId) == tmpVlanId)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                            "VLAN in Referenced L3 Group Id should not match VLAN in Group Id!\r\n", 0);
          return OFDPA_E_PARAM;
        }
        else
        {
          rc = OFDPA_E_NONE;
          bucketIndex = 0;

          memset(&nextBucketEntry, 0, sizeof(nextBucketEntry));

          rc = ofdbGroupBucketEntryFirstGet(groupBucket->groupId, &nextBucketEntry);

          while (rc == OFDPA_E_NONE)
          {
            /* The VLAN id component of the referenced group entry must be different from
               the VLAN id of the referenced group entry of all the other Bucket entries. */
            if (OFDB_GROUP_TYPE(nextBucketEntry.referenceGroupId) == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
            {
              if (tmpVlanId == OFDB_GROUP_VLANID(nextBucketEntry.referenceGroupId))
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                  "The VLAN Id component of the referenced group entry must be "
                                  "different from the VLAN id of the referenced group entry of "
                                  "all the other Bucket entries!\r\n", 0);
                return OFDPA_E_PARAM;
              }
            }
            else /* OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE */
            {
              /* Get the bucket entry from the bucket table of the reference group */
              memset(&tmpBucketEntry, 0, sizeof(tmpBucketEntry));
              (void)ofdbGroupBucketEntryGet(nextBucketEntry.referenceGroupId, 0, &tmpBucketEntry);

              if (tmpVlanId == (tmpBucketEntry.bucketData.l3Interface.vlanId & OFDPA_VID_EXACT_MASK))
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                  "The VLAN Id component of the referenced group entry must be "
                                  "different from the VLAN id of the referenced group entry of "
                                  "all the other Bucket entries!\r\n", 0);
                return OFDPA_E_PARAM;
              }
            }
            bucketIndex = nextBucketEntry.bucketIndex;

            rc = ofdbGroupBucketEntryNextGet(groupBucket->groupId, bucketIndex,
                                              &nextBucketEntry);
          }
        }
      }
      else if (!OFDB_GROUP_IS_MPLS_L3_VPN_LABEL(groupBucket->referenceGroupId))
      {
        return OFDPA_E_PARAM;
      }
      break;

    case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:

      /* Referenced Group Entry should be of type L3 Unicast */
      if ((OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST) &&
          (!OFDB_GROUP_IS_MPLS_L3_VPN_LABEL(groupBucket->referenceGroupId)) &&
          (!OFDB_GROUP_IS_MPLS_FAST_FAILOVER(groupBucket->referenceGroupId)))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Referenced Group Id not valid!\r\n", 0);
        return OFDPA_E_PARAM;
      }
      break;

     case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:

        return OFDPA_E_UNAVAIL;
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
    /* Validate Group Sub-type */
    subType = OFDB_GROUP_MPLS_SUBTYPE(groupBucket->groupId);

      if (OFDPA_MPLS_INTERFACE == subType)
      {
        /* Referenced Group Entry should be of type L2 Interface */
        if ((OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)&&
            (OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                             "Referenced Group Id not of type L2 Interface or L2 Unfiltered interface! Referenced Group = 0x%x\r\n",
                             groupBucket->referenceGroupId);
          return OFDPA_E_PARAM;
        }

        /* validate source MAC */
        if (groupBucket->bucketData.mplsInterface.srcMac.addr[0] & 0x01)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                            "Source MAC not Unicast!\r\n", 0);
          return OFDPA_E_PARAM;
        }

        /* Validate VLANID */

        /* Set Field value for VLAN Id should have OFDPA_VID_PRESENT. */
        if ((groupBucket->bucketData.mplsInterface.vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                            "Set field VLAN does not have OFDPA_VID_PRESENT set.\r\n", 0);
          return OFDPA_E_PARAM;
        }

        tmpVlanId = groupBucket->bucketData.mplsInterface.vlanId & OFDPA_VID_EXACT_MASK;

        if (!dpaVlanIsValid(tmpVlanId))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                             "Invalid VLAN %d!\r\n", tmpVlanId);
          return OFDPA_E_PARAM;
        }

        if ((OFDB_GROUP_TYPE(groupBucket->referenceGroupId) == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE))
        {
          if (tmpVlanId != OFDB_GROUP_VLANID(groupBucket->referenceGroupId))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "VLAN %d not equal to L2 Interface Group VLAN %d!\r\n",
                               tmpVlanId, OFDB_GROUP_VLANID(groupBucket->referenceGroupId));
            return OFDPA_E_PARAM;
          }
        }

        /* Validate lmepId */
        if (groupBucket->bucketData.mplsInterface.lmepIdAction != 0)
        {
          if (OFDPA_E_NONE != ofdbOamMepGet(groupBucket->bucketData.mplsInterface.lmepId, NULL, NULL))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Invalid lmepId = 0x%x\r\n",
                               groupBucket->bucketData.mplsInterface.lmepId);
            return OFDPA_E_PARAM;
          }
        }

        /* Validation for OAM_LM_TX_Count Action type with arguments LMEP_ID and Traffic
           Class to be done after adding support for OAM Data Plane Counter Table. */

        /* if colorBasedCountAction set, counter must exist in counter table */
        if (groupBucket->bucketData.mplsInterface.colorBasedCountAction != 0)
        {
          if (ofdbColorBasedCounterRefCountGet(groupBucket->bucketData.mplsInterface.colorBasedCountId, NULL) != OFDPA_E_NONE)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Entry specifies colorBasedCountAction but counter not found in table. "
                               "colorBasedCountAction = %d, colorBasedCountId = 0x%x\r\n",
                               groupBucket->bucketData.mplsInterface.colorBasedCountAction,
                               groupBucket->bucketData.mplsInterface.colorBasedCountId);
            return(0);
          }
        }
           
      }
      else
      {
        /* Validation common to all label groups */

        /* Note -- need to check for actions specified that are not allowed for a label type. */

        /* MPLS label field limited to 20 bits */
        if (groupBucket->bucketData.mplsLabel.mplsLabel & 0xfff00000ul)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "MPLS label value invalid. mplsLabel = 0x%x\r\n",
                             groupBucket->bucketData.mplsLabel.mplsLabel);
          return OFDPA_E_PARAM;
        }

        if (OFDPA_MPLS_SWAP_LABEL != subType)
        {
          if (0 == groupBucket->bucketData.mplsLabel.pushMplsHdr)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Push MPLS Header missing.\r\n", 0);
            return OFDPA_E_PARAM;
          }
        }

        /* Validate EXP. Limited to 3 bits */
        if ((groupBucket->bucketData.mplsLabel.mplsEXPAction != 0) &&
            (groupBucket->bucketData.mplsLabel.mplsEXP & 0xfffffff8ul))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                             "MPLS EXP invalid. EXP = 0x%x\r\n",
                             groupBucket->bucketData.mplsLabel.mplsEXP);
          return OFDPA_E_PARAM;
        }

        /* Validate TTL. Limited to 8 bits */
        if ((groupBucket->bucketData.mplsLabel.mplsTTLAction != 0) &&
            (groupBucket->bucketData.mplsLabel.mplsTTL & 0xffffff00ul))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                             "MPLS TTL invalid. TTL = 0x%x\r\n",
                             groupBucket->bucketData.mplsLabel.mplsTTL);
          return OFDPA_E_PARAM;
        }

        /* Validate MPLS Tunnel Label Remark table index */
        if (groupBucket->bucketData.mplsLabel.remarkTableIndexAction != 0)
        {
          if (groupBucket->bucketData.mplsLabel.mplsEXPAction != 0)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_VERBOSE,
                               "%d:set tc from field and remarktable action"
                               " can not be used simultaneously.\r\n", __LINE__);
            return OFDPA_E_PARAM;
          }
        }

        /* Validate lmepId */
        if (groupBucket->bucketData.mplsLabel.lmepIdAction != 0)
        {
          if (OFDPA_E_NONE != ofdbOamMepGet(groupBucket->bucketData.mplsLabel.lmepId, NULL, NULL))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Invalid lmepId = 0x%x\r\n",
                               groupBucket->bucketData.mplsLabel.lmepId);
            return OFDPA_E_PARAM;
          }
        }

        /* Validation for OAM_LM_TX_Count Action type with arguments LMEP_ID and Traffic
           Class to be done after adding support for OAM Data Plane Counter Table. */

        /* if colorBasedCountAction set, counter must exist in counter table */
        if (groupBucket->bucketData.mplsLabel.colorBasedCountAction != 0)
        {
          if (ofdbColorBasedCounterRefCountGet(groupBucket->bucketData.mplsLabel.colorBasedCountId, NULL) != OFDPA_E_NONE)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Entry specifies colorBasedCountAction but counter not found in table. "
                               "colorBasedCountAction = %d, colorBasedCountId = 0x%x\r\n",
                               groupBucket->bucketData.mplsLabel.colorBasedCountAction,
                               groupBucket->bucketData.mplsLabel.colorBasedCountId);
            return(0);
          }
        }

        switch (subType)
        {
          case OFDPA_MPLS_L2_VPN_LABEL:
          case OFDPA_MPLS_L3_VPN_LABEL:
            /* Referenced Group Entry should be of sub-type MPLS Interface or MPLS Label1 or MPLS Fast Failover */
            if ((!OFDB_GROUP_IS_MPLS_TUNNEL_LABEL1(groupBucket->referenceGroupId)) &&
                (!OFDB_GROUP_IS_MPLS_INTERFACE(groupBucket->referenceGroupId)) &&
                (!OFDB_GROUP_IS_MPLS_FAST_FAILOVER(groupBucket->referenceGroupId)))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Referenced Group Id not of type MPLS Interface or MPLS Tunnel Label1"
                                 " or MPLS Fast Failover! Referenced Group = 0x%x\r\n",
                                 groupBucket->referenceGroupId);
              return OFDPA_E_PARAM;
            }

            if (OFDPA_MPLS_L2_VPN_LABEL == subType)
            {
              if (0 == groupBucket->bucketData.mplsLabel.pushL2Hdr)
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                   "Push L2 Header action missing.\r\n", 0);
                return OFDPA_E_PARAM;
              }

              if (0 == groupBucket->bucketData.mplsLabel.pushCW)
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                   "Push Control Word action missing.\r\n", 0);
                return OFDPA_E_PARAM;
              }

              if ((0 != groupBucket->bucketData.mplsLabel.pushVlan) &&
                  (0x8100 != groupBucket->bucketData.mplsLabel.newTpid))
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                   "Invalid TPID for push VLAN action. TPID = 0x%04x\r\n",
                                   groupBucket->bucketData.mplsLabel.newTpid);
                return OFDPA_E_PARAM;
              }
            }

            if (0 == groupBucket->bucketData.mplsLabel.mplsBOS)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "VPN Label must be BOS.\r\n", 0);
              return OFDPA_E_PARAM;
            }

            break;

          case OFDPA_MPLS_TUNNEL_LABEL1:
          case OFDPA_MPLS_TUNNEL_LABEL2:
            if (subType == OFDPA_MPLS_TUNNEL_LABEL1)
            {
              /* Referenced Group Entry should be of type MPLS Interface or MPLS Label2*/
              if ((!OFDB_GROUP_IS_MPLS_TUNNEL_LABEL2(groupBucket->referenceGroupId)) &&
                  (!OFDB_GROUP_IS_MPLS_INTERFACE(groupBucket->referenceGroupId)))
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                   "Referenced Group Id not of type MPLS Interface or MPLS Tunnel Label2! Referenced Group = 0x%x\r\n",
                                   groupBucket->referenceGroupId);
                return OFDPA_E_PARAM;
              }
            }
            else if (subType == OFDPA_MPLS_TUNNEL_LABEL2)
            {
              /* Referenced Group Entry should be of type MPLS Interface */
              if (!OFDB_GROUP_IS_MPLS_INTERFACE(groupBucket->referenceGroupId))
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                   "Referenced Group Id not of type MPLS Interface! Referenced Group = 0x%x\r\n",
                                   groupBucket->referenceGroupId);
                return OFDPA_E_PARAM;
              }
            }

            break;

          case OFDPA_MPLS_SWAP_LABEL:
            /* Referenced Group Entry should be of sub-type MPLS Interface or MPLS Label1 or MPLS Fast Failover */
            if ((!OFDB_GROUP_IS_MPLS_TUNNEL_LABEL1(groupBucket->referenceGroupId)) &&
                (!OFDB_GROUP_IS_MPLS_INTERFACE(groupBucket->referenceGroupId)) &&
                (!OFDB_GROUP_IS_MPLS_FAST_FAILOVER(groupBucket->referenceGroupId)))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Referenced Group Id not of type MPLS Interface or MPLS Tunnel Label1"
                                 " or MPLS Fast Failover! Referenced Group = 0x%x\r\n",
                                 groupBucket->referenceGroupId);
              return OFDPA_E_PARAM;
            }

            break;

          default:
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Invalid MPLS Label Group Subtype! Subtype = %d\r\n", subType);
            return OFDPA_E_PARAM;
        }
      }

    break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
    /* Validate Group Sub-type */
    subType = OFDB_GROUP_MPLS_SUBTYPE(groupBucket->groupId);

    switch (subType)
    {
      case OFDPA_MPLS_L2_FLOOD:
      case OFDPA_MPLS_L2_MULTICAST:
        /* Validate Referenced Group Id*/
        switch (OFDB_GROUP_TYPE(groupBucket->referenceGroupId))
        {
          case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
            if ((OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_L2_VPN_LABEL) &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_SWAP_LABEL))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Referenced Group Id not of subtype MPLS L2 VPN Label or Swap Label! Referenced Group = 0x%x\r\n",
                                 groupBucket->referenceGroupId);
              return OFDPA_E_PARAM;
            }
            break;

          case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
            if ((OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_FAST_FAILOVER) &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_1_1_HEAD_END_PROTECT))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Referenced Group Id not of subtype MPLS Fast Failover or 1:1 Head End Protect! Referenced Group = 0x%x\r\n",
                                 groupBucket->referenceGroupId);
              return OFDPA_E_PARAM;
            }
            break;

          default:
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Referenced Group Id not of type MPLS! Referenced Group = 0x%x\r\n",
                               groupBucket->referenceGroupId);
            return OFDPA_E_PARAM;
            break;
        }

        break;

      case OFDPA_MPLS_L2_LOCAL_FLOOD:
      case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        /* Validate Referenced Group Id*/
        switch (OFDB_GROUP_TYPE(groupBucket->referenceGroupId))
        {
          case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
            if (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_L2_TAG)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Referenced Group Id not of subtype MPLS L2 Tag! Referenced Group = 0x%x\r\n",
                                 groupBucket->referenceGroupId);
              return OFDPA_E_PARAM;
            }
            break;

          case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
            break;

          default:
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Referenced Group Id invalid! Referenced Group = 0x%x\r\n",
                               groupBucket->referenceGroupId);
            return OFDPA_E_PARAM;
            break;
        }

        break;
      case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
      case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
        /* Validate Referenced Group Entry */
        break;

      case OFDPA_MPLS_FAST_FAILOVER:
        /* Validate Referenced Group Id*/
        switch (OFDB_GROUP_TYPE(groupBucket->referenceGroupId))
        {
          case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
            if ((OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_L2_VPN_LABEL) &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_L3_VPN_LABEL) &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_SWAP_LABEL)   &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_TUNNEL_LABEL1) &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_TUNNEL_LABEL2))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Referenced Group Id is not of valid type. Referenced Group = 0x%x\r\n",
                                 groupBucket->referenceGroupId);
              return OFDPA_E_PARAM;
            }
            break;

          default:
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Referenced Group Id not of type MPLS Label! Referenced Group = 0x%x\r\n",
                                 groupBucket->referenceGroupId);
              return OFDPA_E_PARAM;
              break;
          }

        /* Verify that group type of working and protection buckets is same. */
        memset(&workingGroupBucket, 0, sizeof(ofdpaGroupBucketEntry_t));
        if (OFDPA_E_NONE == ofdbGroupBucketEntryFirstGet(groupBucket->groupId, &workingGroupBucket))
        {
          if ((OFDB_GROUP_TYPE(groupBucket->referenceGroupId) != OFDB_GROUP_TYPE(workingGroupBucket.referenceGroupId)) ||
              (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDB_GROUP_MPLS_SUBTYPE(workingGroupBucket.referenceGroupId)))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "%d MPLS Fast Failover protection bucket's reference group type is"
                               " not same as working bucket's reference group type.\r\n",
                                __LINE__);
            return OFDPA_E_PARAM;
          }
        }

        /* Validate Watch Port - bucket should refer to logical port only */
        if (OFDB_PORT_TYPE(groupBucket->bucketData.mplsFastFailOver.watchPort) != OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                            "MPLS Fast Failover Bucket Data: incorrect watch port type. (%x)\r\n",
                             groupBucket->bucketData.mplsFastFailOver.watchPort);
          return OFDPA_E_PARAM;
        }
        break;

      case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        /* Validate Referenced Group Id*/
        switch (OFDB_GROUP_TYPE(groupBucket->referenceGroupId))
        {
          case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
            if ((OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_L2_VPN_LABEL) &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_L3_VPN_LABEL) &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_SWAP_LABEL)   &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_TUNNEL_LABEL1))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Referenced Group Id is not of valid type ! Referenced Group = 0x%x\r\n",
                                 groupBucket->referenceGroupId);
              return OFDPA_E_PARAM;
            }
            break;

          case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
            if ((OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_1_1_HEAD_END_PROTECT) &&
                (OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId) != OFDPA_MPLS_FAST_FAILOVER))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Referenced Group Id not of subtype MPLS 1:1 Head End Protect or Fast Failover! Referenced Group = 0x%x\r\n",
                                 groupBucket->referenceGroupId);
              return OFDPA_E_PARAM;
            }
            break;

          default:
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Referenced Group Id not of type MPLS! Referenced Group = 0x%x\r\n",
                               groupBucket->referenceGroupId);
            return OFDPA_E_PARAM;
            break;
        }
        break;

        case OFDPA_MPLS_ECMP:
          if ((!OFDB_GROUP_IS_MPLS_SWAP_LABEL(groupBucket->referenceGroupId)) &&
              (!OFDB_GROUP_IS_MPLS_FAST_FAILOVER(groupBucket->referenceGroupId)))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Referenced Group Id not of valid type! Referenced Group = 0x%x\r\n",
                               groupBucket->referenceGroupId);
            return OFDPA_E_PARAM;
          }

          rc = OFDPA_E_NONE;
          bucketIndex = 0;

        memset(&nextBucketEntry, 0, sizeof(nextBucketEntry));

        rc = ofdbGroupBucketEntryFirstGet(groupBucket->groupId, &nextBucketEntry);

          while (rc == OFDPA_E_NONE)
          {
            /* The type of the referenced group entry must be the same as other referenced group entries. */
            if ((OFDB_GROUP_TYPE(nextBucketEntry.referenceGroupId)         != OFDB_GROUP_TYPE(groupBucket->referenceGroupId)) ||
                (OFDB_GROUP_MPLS_SUBTYPE(nextBucketEntry.referenceGroupId) != OFDB_GROUP_MPLS_SUBTYPE(groupBucket->referenceGroupId)))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "The type of the referenced group entry must be the same "
                                 "as other referenced group entries!\r\n", 0);
              return OFDPA_E_PARAM;
            }
            bucketIndex = nextBucketEntry.bucketIndex;

          rc = ofdbGroupBucketEntryNextGet(groupBucket->groupId, bucketIndex,
                                            &nextBucketEntry);
        }
        break;

      case OFDPA_MPLS_L2_TAG:
        if (groupBucket->bucketData.mplsL2Tag.pushVlan != 0)
        {
          /* Validate VLANID */

          /* Set Field value for VLAN Id should have OFDPA_VID_PRESENT. */
          if ((groupBucket->bucketData.mplsL2Tag.vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                              "Set field VLAN does not have OFDPA_VID_PRESENT set.\r\n", 0);
            return OFDPA_E_PARAM;
          }

          tmpVlanId = groupBucket->bucketData.mplsL2Tag.vlanId & OFDPA_VID_EXACT_MASK;

          if (!dpaVlanIsValid(tmpVlanId))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Invalid VLAN %d!\r\n", tmpVlanId);
            return OFDPA_E_PARAM;
          }
        }

        break;

      default:
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Invalid MPLS Forwarding Group Subtype! Subtype = %d\r\n", subType);
      return OFDPA_E_PARAM;
    }

    break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
      /* The port id component of the groupId entry must match the
         Set Field value for output port id. */
      if (groupBucket->bucketData.l2UnfilteredInterface.outputPort != OFDB_GROUP_PORTID(groupBucket->groupId))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "Port in Group Id does not match set field Port!\r\n", 0);
        return OFDPA_E_PARAM;
      }
/* changed by yjg
      if (groupBucket->bucketData.l2UnfilteredInterface.allowVlanTranslation == 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                          "L2 Unfiltered Interface Group bucket must set "
                          "allowVlanTranslation. (allowVlanTranslation == %d)\r\n",
                           groupBucket->bucketData.l2Interface.allowVlanTranslation);
        return OFDPA_E_PARAM;
      }
*/
      break;

    default:
      /* Invalid Group ID */
      return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}

uint32_t ofdbGroupTableTotalEntryCountGet(void)
{
  return(ofdbGroupTableStatus->num_groups_in_database);
}

uint32_t ofdbGroupTableMaxCountGet(OFDPA_GROUP_ENTRY_TYPE_t groupType)
{
  uint32_t value = 0;

  if ((groupType >= OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) &&
      (groupType < OFDPA_GROUP_ENTRY_TYPE_LAST))
  {
    value = ofdbGroupTableStatus->groupTableMaxCount[groupType];
  }

  return(value);
}

uint32_t ofdbGroupTableEntryCountGet(OFDPA_GROUP_ENTRY_TYPE_t groupType)
{
  uint32_t value = 0;

  if ((groupType >= OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) &&
      (groupType < OFDPA_GROUP_ENTRY_TYPE_LAST))
  {
    value = ofdbGroupTableStatus->groupTableEntryCount[groupType];
  }

  return(value);
}

uint32_t ofdbGroupBucketTableMaxCountGet(uint32_t groupId)
{
  uint32_t value = 0;
  OFDPA_GROUP_ENTRY_TYPE_t groupType = OFDB_GROUP_TYPE(groupId);

  if ((groupType >= OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) &&
      (groupType < OFDPA_GROUP_ENTRY_TYPE_LAST))
  {
    if (OFDB_GROUP_IS_MPLS_FAST_FAILOVER(groupId))
    {
      value = dpaGroupSubtypeBucketMaxSizeGet(groupType, OFDPA_MPLS_FAST_FAILOVER);
    }
		else  if (OFDB_GROUP_IS_MPLS_L2_TAG(groupId))
    {
      value = dpaGroupSubtypeBucketMaxSizeGet(groupType, OFDPA_MPLS_L2_TAG);
    }
    else
    {
      value = ofdbGroupBucketTableStatus->groupBucketTableMaxCount[groupType];
    }
  }

  return(value);
}

OFDPA_ERROR_t ofdbRefGroupNextGet(uint32_t referredGroupId, uint32_t *nextReferringGroupId,
                                  OFDPA_GROUP_ENTRY_TYPE_t referringGroupType)
{
  OFDPA_ERROR_t result, result1;
  ofdpaGroupEntry_t nextGroup;
  ofdpaGroupBucketEntry_t nextBucketEntry;
  uint32_t groupId, tempBucketIndex;

  if (nextReferringGroupId == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Null Group passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  groupId = *nextReferringGroupId;

  result = ofdbGroupNextGet(groupId, &nextGroup);

  while (result == OFDPA_E_NONE)
  {
    if (OFDB_GROUP_TYPE(nextGroup.groupId) == referringGroupType)
    {
      memset(&nextBucketEntry, 0, sizeof(nextBucketEntry));

      result1 = ofdbGroupBucketEntryFirstGet(nextGroup.groupId, &nextBucketEntry);

      while (result1 == OFDPA_E_NONE)
      {
        if (nextBucketEntry.referenceGroupId == referredGroupId)
        {
          *nextReferringGroupId = nextGroup.groupId;
          return OFDPA_E_NONE;
        }
        tempBucketIndex = nextBucketEntry.bucketIndex;

        result1 = ofdbGroupBucketEntryNextGet(nextGroup.groupId, tempBucketIndex,
                                              &nextBucketEntry);
      }
    }
    groupId = nextGroup.groupId;
    result = ofdbGroupNextGet(groupId, &nextGroup);
  }
  return OFDPA_E_FAIL;
}

OFDPA_ERROR_t ofdbRefGroupValidate(uint32_t referredGroupId)
{
  ofdpaGroupEntryStats_t refGroupStats;

  if (OFDPA_E_NONE != ofdbGroupStatsGet(referredGroupId, &refGroupStats))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Referenced Group not found in Group table!\r\n", 0);
    return OFDPA_E_NOT_FOUND;
  }

  if (refGroupStats.bucketCount <= 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Referenced Group Id should have at least 1 bucket!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
 *
 * @purpose get Init L2VpnLabel Arrary according to Term Tunnel Label
 *
 * @param uint32_t    label         @b{(input)} Term Tunnel Label
 * @param uint32_t   *isProt        @b{output)} Is this tunnel protection tunnel
 * @param uint32_t   *l2vpnLabels   @b{output)} Init L2VpnLabel Arrary
 * @param uint32_t   *entryCount    @b{output)} entryCount of Init L2VpnLabel Arrary
 *
 * @returns OFDPA_ERROR_t
 *
 * @notes
 *  There is some related field must be the same
    for Init Label entry in MPLS Flow table and Term Label entry in group table.
    Now lmepId is the key for OAM_LM_TX_Count and OAM_LM_RX_Count.
    Eventually color-based counter for LSP/PW should be utilized.
 * @internal
 *
 * @end
 *
 *********************************************************************/
OFDPA_ERROR_t ofdbGroupInitL2VpnLabelsGetFromTermTunnelLabel(uint32_t label, uint32_t *isProt, uint32_t *l2vpnLabels, uint32_t *entryCount)
{
  OFDPA_ERROR_t rc, rc2;
  uint32_t groupId = 0, tunnelGroupId = 0, failoverGroupId = 0;
  uint32_t found;
  int      bucketIndex;
  uint32_t lmepId = 0;
  ofdpaFlowEntry_t        flowEntry;
  ofdpaGroupEntry_t       nextGroup;
  ofdpaGroupBucketEntry_t nextBucketEntry;

  *entryCount = 0;
  *isProt = 0;
  /* find lmepId according to ingress label */
  ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_MPLS_1, &flowEntry);
  while ((OFDPA_E_NONE       == ofdbFlowNextGet(&flowEntry, &flowEntry, NULL, NULL)) &&
         (OFDPA_FLOW_TABLE_ID_MPLS_1 == flowEntry.tableId))
  {
    if ((label  == flowEntry.flowData.mplsFlowEntry.match_criteria.mplsLabel) &&
        (flowEntry.flowData.mplsFlowEntry.lmepId != 0))
    {
      lmepId = flowEntry.flowData.mplsFlowEntry.lmepId;
      break;
    }
  }
  if (lmepId == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERY_VERBOSE,
                      "fail to find lmepId according to ingress label %d!\r\n", label);
    return OFDPA_E_NOT_FOUND;
  }

  /* find tunnel group according to lmepId */
  rc = OFDPA_E_NONE;
  groupId = 0;
  found = 0;
  while (rc == OFDPA_E_NONE)
  {
    rc = ofdbGroupNextGet(groupId, &nextGroup);
    if (rc == OFDPA_E_NONE)
    {
      if (OFDB_GROUP_IS_MPLS_TUNNEL_LABEL1(nextGroup.groupId))
      {
          rc2 = OFDPA_E_NONE;
          bucketIndex = 0;

          memset(&nextBucketEntry, 0, sizeof(nextBucketEntry));

          rc2 = ofdbGroupBucketEntryFirstGet(nextGroup.groupId, &nextBucketEntry);
          while (rc2 == OFDPA_E_NONE)
          {
            if (nextBucketEntry.bucketData.mplsLabel.lmepId == lmepId)
            {
              found = 1;
              break;
            }
            bucketIndex = nextBucketEntry.bucketIndex;

            rc2 = ofdbGroupBucketEntryNextGet(nextGroup.groupId, bucketIndex,
                                              &nextBucketEntry);
          }
      }
      groupId = nextGroup.groupId;
      if (found)
      {
        tunnelGroupId = groupId;
        break;
      }
    }
  }
  if (found == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERY_VERBOSE,
                      "fail to find tunnel group according to lmepId %d!\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* find failover group according to tunnel group */
  rc = OFDPA_E_NONE;
  groupId = 0;
  found = 0;
  while (rc == OFDPA_E_NONE)
  {
    rc = ofdbGroupTypeNextGet(groupId, OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING, &nextGroup);
    if (rc == OFDPA_E_NONE)
    {
      if (OFDB_GROUP_IS_MPLS_FAST_FAILOVER(nextGroup.groupId))
      {
          rc2 = OFDPA_E_NONE;
          bucketIndex = 0;

          memset(&nextBucketEntry, 0, sizeof(nextBucketEntry));

          rc2 = ofdbGroupBucketEntryFirstGet(nextGroup.groupId, &nextBucketEntry);
          while (rc2 == OFDPA_E_NONE)
          {
            if (nextBucketEntry.referenceGroupId == tunnelGroupId)
            {
              found = 1;
              if (nextBucketEntry.bucketIndex == 1)
              {
                *isProt = 1;
              }
              break;
            }
            bucketIndex = nextBucketEntry.bucketIndex;

            rc2 = ofdbGroupBucketEntryNextGet(nextGroup.groupId, bucketIndex,
                                              &nextBucketEntry);
          }
      }
      groupId = nextGroup.groupId;
      if (found)
      {
        failoverGroupId = groupId;
        break;
      }
    }
  }
  if (found == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERY_VERBOSE,
                      "fail to find failover group according to tunnelGroupId 0x%x!\r\n", tunnelGroupId);
  }

  /* find l2 vpn group according to tunnel group and failover group */
  rc = OFDPA_E_NONE;
  groupId = 0;
  found = 0;
  while (rc == OFDPA_E_NONE)
  {
    rc = ofdbGroupTypeNextGet(groupId, OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL, &nextGroup);
    if (rc == OFDPA_E_NONE)
    {
      if (OFDB_GROUP_IS_MPLS_L2_VPN_LABEL(nextGroup.groupId))
      {
          rc2 = OFDPA_E_NONE;
          bucketIndex = 0;

          memset(&nextBucketEntry, 0, sizeof(nextBucketEntry));

          rc2 = ofdbGroupBucketEntryFirstGet(nextGroup.groupId, &nextBucketEntry);
          while (rc2 == OFDPA_E_NONE)
          {
            if ((nextBucketEntry.referenceGroupId == tunnelGroupId) ||
                (nextBucketEntry.referenceGroupId == failoverGroupId))
            {
              found = 1;
              l2vpnLabels[*entryCount] = nextBucketEntry.bucketData.mplsLabel.mplsLabel;
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_TOO_VERBOSE,
                                 "found init l2vpnLable 0x%x according to tunnelGroupId 0x%x and failoverGroupId 0x%x!\r\n",
                                 l2vpnLabels[*entryCount], tunnelGroupId, failoverGroupId);
              *entryCount = *entryCount + 1;
            }
            bucketIndex = nextBucketEntry.bucketIndex;

            rc2 = ofdbGroupBucketEntryNextGet(nextGroup.groupId, bucketIndex,
                                              &nextBucketEntry);
          }
      }
      groupId = nextGroup.groupId;
    }
  }
  if (found == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERY_VERBOSE,
                      "fail to find l2vpn group according to tunnelGroupId 0x%x and failoverGroupId 0x%x!\r\n", tunnelGroupId, failoverGroupId);
    return OFDPA_E_NOT_FOUND;
  }

  return OFDPA_E_NONE;
}
