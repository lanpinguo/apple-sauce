/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2016
*
**********************************************************************
*
* @filename     ofdpa_server.c
*
* @purpose      OF-DPA API implementation
*
* @component    OF-DPA
*
* @comments     These APIs may be accessed via RPC.
*
* @create       11 Apr 2013
*
* @end
*
**********************************************************************/
#include "ofdpa_api.h"
#include "ofdb_api.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/ethernet.h>
#include <sys/param.h>

#include "of_adpl_api.h"


OFDPA_ERROR_t ofdpaOfagentCxnAdd(ofdpaAgentCxnParam_t * cfg ,uint32_t *of_cxn_id)
{
	adplAgentCxnParam_t cxn_cfg;
	ADPL_ERROR_t rc;
		

  memcpy(cxn_cfg.controler_ip,cfg->controler_ip,sizeof(cxn_cfg.controler_ip));
	cxn_cfg.l4_port               = cfg->l4_port;
	cxn_cfg.periodic_echo_ms      = cfg->periodic_echo_ms;
	cxn_cfg.reset_echo_count      = cfg->reset_echo_count;
	cxn_cfg.pre_reset_echo_count  = cfg->pre_reset_echo_count;
	cxn_cfg.role                  = cfg->role;
	
	rc = ADPL_OfagentCxnAdd(&cxn_cfg,of_cxn_id);
	if ( rc == ADPL_E_NONE )
	{
		printf("\r\nAdd controlr successfully , cxn id :%d\r\n",*of_cxn_id);
    return OFDPA_E_NONE;
	}

  return OFDPA_E_FAIL;
}



OFDPA_ERROR_t ofdpaOfagentCxnRemove(uint32_t of_cxn_id)
{
	printf("\r\nRemove controlr successfully , cxn id :%d\r\n",of_cxn_id);
  return OFDPA_E_FAIL;
}

OFDPA_ERROR_t ofdpaFeatureSupported(OFDPA_FEATURE_t feature)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE, "\r\nfeature %d not supported\r\n",feature);
  return rc;
}

OFDPA_ERROR_t ofdpaFlowTableSupported(OFDPA_FLOW_TABLE_ID_t tableId)
{
  OFDPA_ERROR_t rc = OFDPA_E_UNAVAIL;
  uint32_t result;

  result = ofdbFlowTableSupported(tableId);
  if (result == 1)
  {
    rc = OFDPA_E_NONE;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE, "tableId = %d, result = %d, rc = %d.\r\n", tableId, result, rc);
  return(rc);
}


OFDPA_ERROR_t ofdpaFlowTableInfoGet(OFDPA_FLOW_TABLE_ID_t tableId, ofdpaFlowTableInfo_t *info)
{
	if (info == NULL)
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											"Null Info passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}

	if (ofdbFlowTableSupported(tableId) == 0)
	{
		return OFDPA_E_NOT_FOUND;
	}

	OFDB_READ_LOCK_TAKE;
	info->maxEntries = ofdbFlowTableMaxCountGet(tableId);
	info->numEntries = ofdbFlowTableEntryCountGet(tableId);
	OFDB_LOCK_GIVE;

	return OFDPA_E_NONE;
}



OFDPA_ERROR_t ofdpaFlowNextGet(ofdpaFlowEntry_t *flow, ofdpaFlowEntry_t *nextFlow)
{
	OFDPA_ERROR_t rc;
	ofdbFlowStatus_t flow_status;

	if ((flow == NULL) ||
			(nextFlow == NULL))
	{
		return OFDPA_E_PARAM;
	}

	OFDB_READ_LOCK_TAKE;
	rc = ofdbFlowNextGet(flow, nextFlow, NULL, &flow_status);
	OFDB_LOCK_GIVE;
	return(rc);
}



OFDPA_ERROR_t ofdpaFlowStatsGet(ofdpaFlowEntry_t *flow, ofdpaFlowEntryStats_t *flowStats)
	{
		OFDPA_ERROR_t rc;
		ofdbFlowStatus_t flow_status;
	
		if ((flow == NULL) ||
				(flowStats == NULL))
		{
			return OFDPA_E_PARAM;
		}
	
		OFDB_READ_LOCK_TAKE;
		rc = ofdbFlowGet(flow, NULL, &flow_status);
		OFDB_LOCK_GIVE;
		
		if (rc == OFDPA_E_NONE)
		{
			flowStats->durationSec = dpaUpTimeSeconds() - flow_status.flow_add_up_time;
	
			switch ( flow->tableId )
			{
					case OFDPA_FLOW_TABLE_ID_INGRESS_PORT :
							
							break;
					case OFDPA_FLOW_TABLE_ID_VLAN :
							rc = vlanPipeFlowStatsGet(flow,flowStats); 
							break;
					case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT :
							 
							break;
					case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT :
							rc = mplsL2PortPipeFlowStatsGet(flow,flowStats); 
							break;
					case OFDPA_FLOW_TABLE_ID_L2_POLICER :
							 
							break;
					case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC :
							 
							break;
					case OFDPA_FLOW_TABLE_ID_MPLS_0 :
							 
							break;
					case OFDPA_FLOW_TABLE_ID_MPLS_1 :
							
							break;
					case OFDPA_FLOW_TABLE_ID_MPLS_2 :
							 
							break;
					case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT :
							 
							break;
					case OFDPA_FLOW_TABLE_ID_MPLS_TYPE :
							 
							break;
					case OFDPA_FLOW_TABLE_ID_ACL_POLICY :
							break;
					case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN :
							 
							break;
					default:
							break;
			}
		}
		return(rc);
	}



OFDPA_ERROR_t ofdpaFlowAdd(ofdpaFlowEntry_t *flow)
{
	OFDPA_ERROR_t rc = OFDPA_E_NONE;
	uint32_t flowValid;
	uint64_t flowId;

	if ((flow == NULL) ||
			(dpFlowTableSupported(flow->tableId) == 0))
	{
		return OFDPA_E_PARAM;
	}


	switch (flow->tableId)
	{
		case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
			break;
	
		case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_VLAN:
			{
				rc = vlanPipeFlowAdd(flow);
				if(rc != OFDPA_E_NONE){
					OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "add VLAN pipe entry failed.\r\n", 0);
					return rc;
				}
			}
			break;
	
		case OFDPA_FLOW_TABLE_ID_VLAN_1:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
			{	
				rc = mplsL2PortPipeFlowAdd(flow);
				if(rc != OFDPA_E_NONE){
					OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "add mpls l2 port pipe entry failed.\r\n", 0);
					return rc;
				}			
			}
			break;
	
		case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_L2_POLICER:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_MPLS_0:
		case OFDPA_FLOW_TABLE_ID_MPLS_1:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_BRIDGING:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
			
			break;
	
	
		case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
			
			break;
	
		case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
			
			break;
	
		default:
			/* unknown table ID */
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
												 "Unknown flow table ID. (%d)\r\n",
												 flow->tableId);
			return(OFDPA_E_FAIL);
			break;
	
	}



	return(rc);
}

OFDPA_ERROR_t ofdpaFlowModify(ofdpaFlowEntry_t *flow)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  printf("\r\n ofdpaFlowModify not supported");
  return(rc);
}



OFDPA_ERROR_t dpaGroupBucketEntryDelete(uint32_t groupId, uint32_t bucketIndex)
{
  OFDPA_ERROR_t rc;
  OFDPA_ERROR_t driverRc = OFDPA_E_NONE;
  ofdpaGroupEntryStats_t groupStats;
  ofdpaGroupBucketEntry_t groupBucket;
  ofdbPortInfo_t portInfo;
  OFDB_ENTRY_FLAG_t flags;
  uint32_t subType;

  /* Check if there are no references to the Group */
  rc = ofdbGroupStatsGet(groupId, &groupStats);

  if (rc == OFDPA_E_NONE)
  {
    if ((groupStats.refCount == 0) || (groupStats.bucketCount > 1))
    {
      memset(&groupBucket, 0, sizeof(groupBucket));
      rc = ofdbGroupBucketEntryGet(groupId, bucketIndex, &groupBucket);
      if (OFDPA_E_NONE != rc)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                           "Failed to find entry in Software; rc = %d!\r\n", rc);
        /* BEGIN: Modified by Hushouqiang, 2016/12/26   问题单号:DEBUG_PROTECTION */
        /*OFDB_LOCK_GIVE;*/
        /* END:   Modified by Hushouqiang, 2016/12/26 */
        return rc;
      }

      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                         "Deleting Bucket from Hardware\r\n", 0);

      switch (OFDB_GROUP_TYPE(groupId))
      {
      case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
        break;
      case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
        break;
      case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
        break;
      case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
        break;
      case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
        break;
      case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:

        break;
      case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
        break;
      case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        break;
      case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
        /* Validate Group Sub-type */
        subType = OFDB_GROUP_MPLS_SUBTYPE(groupId);

        switch (subType)
        {
          case OFDPA_MPLS_INTERFACE:

            break;
          case OFDPA_MPLS_L2_VPN_LABEL:
          case OFDPA_MPLS_L3_VPN_LABEL:
          case OFDPA_MPLS_TUNNEL_LABEL1:
          case OFDPA_MPLS_TUNNEL_LABEL2:
          case OFDPA_MPLS_SWAP_LABEL:
            break;
          default:
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                           "Invalid MPLS Label Group Subtype!\r\n", 0);
            driverRc = OFDPA_E_PARAM;
        }
        break;

      case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
        /* Validate Group Sub-type */
        subType = OFDB_GROUP_MPLS_SUBTYPE(groupId);

        switch (subType)
        {
          case OFDPA_MPLS_ECMP:
            break;
          case OFDPA_MPLS_FAST_FAILOVER:
            break;
          case OFDPA_MPLS_L2_FLOOD:
          case OFDPA_MPLS_L2_MULTICAST:
          case OFDPA_MPLS_L2_LOCAL_FLOOD:
          case OFDPA_MPLS_L2_LOCAL_MULTICAST:
          case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
          case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
          case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
          case OFDPA_MPLS_L2_TAG:
            break;
          default:
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                           "Invalid MPLS Forwarding Group Subtype!\r\n", 0);
            driverRc = OFDPA_E_PARAM;
        }
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
        break;

      default:
        /* Invalid Group ID */
        return OFDPA_E_PARAM;
      }

      if (OFDPA_E_NONE != driverRc)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                           "Failed to delete Bucket from Hardware; driverRc = %d!\r\n", driverRc);
      }

      rc = ofdbGroupBucketEntryDelete(groupId, bucketIndex);

      if (OFDPA_E_NONE != rc)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                           "Failed to delete Bucket from Software; rc = %d!\r\n", rc);
      }

      if ((rc == OFDPA_E_NONE) && (driverRc == OFDPA_E_NONE))
      {
        rc = OFDPA_E_NONE;
      }
      else
      {
        rc = OFDPA_E_FAIL;
      }
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                         "Cannot delete the only bucket of a referenced Group.", 0);
      rc = OFDPA_E_FAIL;
    }
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Stats get failed: rc = %d!\r\n", rc);
  }

  return rc;
}

OFDPA_ERROR_t dpaGroupBucketsDeleteAll(uint32_t groupId)
{
  OFDPA_ERROR_t rc, rc1;
  ofdpaGroupBucketEntry_t nextBucketEntry;
  int tempBucketIndex = 0;

  /* Delete all buckets from the corresponding Group Bucket Table */

  memset(&nextBucketEntry, 0, sizeof(nextBucketEntry));
  rc = ofdbGroupBucketEntryFirstGet(groupId, &nextBucketEntry);

  while (rc == OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                       "Deleting Bucket\r\n", 0);

    rc1 = dpaGroupBucketEntryDelete(groupId, nextBucketEntry.bucketIndex);

    if (OFDPA_E_NONE != rc1)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                         "Failed to delete Bucket; rc = %d!\r\n", rc1);
      return OFDPA_E_FAIL;
    }

    tempBucketIndex = nextBucketEntry.bucketIndex;

    rc = ofdbGroupBucketEntryNextGet(groupId, tempBucketIndex,
                                         &nextBucketEntry);
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaGroupAdd(ofdpaGroupEntry_t *group)
{
  OFDPA_ERROR_t rc;
  uint32_t vlanId;
  uint32_t tunnelId;
  ofdpaTunnelTenantConfig_t tenantConfig;
  ofdpaTunnelTenantStatus_t tenantStatus;
  uint32_t portId;
  uint32_t groupId;
  ofdpaGroupEntry_t nextGroup;
  OFDPA_GROUP_ENTRY_TYPE_t groupType;
  uint32_t portNum;
  ofdbPortInfo_t portInfo;
  OFDB_ENTRY_FLAG_t portFlags;
  uint32_t subType;

  if (group == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Group passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "Add Group : 0x%08x!\r\n", group->groupId);


  groupType = OFDB_GROUP_TYPE(group->groupId);

  OFDB_WRITE_LOCK_TAKE;

  /* check if there is room for another entry in this groupId */
  if (ofdbGroupTableEntryCountGet(groupType) >= ofdbGroupTableMaxCountGet(groupType))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Group 0x%x max count (%d) is not larger than entry count (%d).\r\n",
                       group->groupId, ofdbGroupTableEntryCountGet(groupType),
                       ofdbGroupTableMaxCountGet(groupType));
    OFDB_LOCK_GIVE;
    return OFDPA_E_FULL;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                     "GroupId = 0x%x, Type = %d\r\n",
                     group->groupId, OFDB_GROUP_TYPE(group->groupId));

  switch (groupType)
  {
  case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
  case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                       "Group VLANID = %d, Port = %d\r\n",
                       OFDB_GROUP_VLANID(group->groupId), OFDB_GROUP_PORTID(group->groupId));

    /* Validate VLANID */
    vlanId = OFDB_GROUP_VLANID(group->groupId);

    if (groupType == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
    {
      if (!dpaVlanIsValid(vlanId))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                           "Invalid VLAN %d for L2 Interface Group!\r\n", vlanId);
        OFDB_LOCK_GIVE;
        return OFDPA_E_PARAM;
      }
    }
    else
    {
      if (vlanId != 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                           "Invalid VLAN %d for L2 Unfiltered Interface Group!\r\n", vlanId);
        OFDB_LOCK_GIVE;
        return OFDPA_E_PARAM;
      }
    }

    /* Validate PortID */
    portId = OFDB_GROUP_PORTID(group->groupId);

    if (!ofdbPortIsValid(portId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                         "Invalid port %d!\r\n", portId);
      OFDB_LOCK_GIVE;
      return OFDPA_E_NOT_FOUND;
    }

    if (groupType == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
    {
      /* Check whether the output port is not used in any L2 Unfiltered Interface group */
      rc = OFDPA_E_NONE;
      groupId = 0;

      while (rc == OFDPA_E_NONE)
      {
        rc = ofdbGroupTypeNextGet(groupId, OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE, &nextGroup);

        if (rc == OFDPA_E_NONE)
        {
          if (OFDB_GROUP_PORTID(group->groupId) == OFDB_GROUP_PORTID(nextGroup.groupId))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                               "Port Id already used in L2 Unfiltered Interface group %x!\r\n",
                               nextGroup.groupId);
            OFDB_LOCK_GIVE;
            return OFDPA_E_PARAM;
          }
          groupId = nextGroup.groupId;
        }
      }
    }
    else
    {
      /* Check whether the output port is not used in any L2 Interface group */
      rc = OFDPA_E_NONE;
      groupId = 0;

      while (rc == OFDPA_E_NONE)
      {
        rc = ofdbGroupTypeNextGet(groupId, OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE, &nextGroup);

        if (rc == OFDPA_E_NONE)
        {
          if (OFDB_GROUP_PORTID(group->groupId) == OFDB_GROUP_PORTID(nextGroup.groupId))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                               "Port Id already used in L2 Interface group %x!\r\n",
                               nextGroup.groupId);
            OFDB_LOCK_GIVE;
            return OFDPA_E_PARAM;
          }
          groupId = nextGroup.groupId;
        }
      }
    }

    /* check constraints between group and VXLAN Access Port configuration */
    /* no L2 Interface Group entry may match on a port if that port is configured as a tunnel access port that includes all packets on that port */
    /* no L2 Interface Group entry may match on a port/VLAN if that port/VLAN is configured as a tunnel access port for the same port/vlan */

    /* iterate over all logical port entries
       if an entry is an access port and the ports match
       if the access port is for untagged, L2 Interface Group entry rejected
       else access port is for tagged, if VLAN matches, L2 Interface Group rejected */
    if (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN)) 
    {
      portNum = 0;
      portNum = OFDB_PORT_TYPE_SET(portNum, OFDPA_PORT_TYPE_LOGICAL_TUNNEL);
      while ((ofdbPortNextGet(portNum, &portNum) == OFDPA_E_NONE) &&
             (OFDB_PORT_TYPE(portNum) == OFDPA_PORT_TYPE_LOGICAL_TUNNEL))
      {
        if (ofdbPortGet(portNum, &portInfo, &portFlags) == OFDPA_E_NONE)
        {
          if ((portFlags & OFDB_PORT_DELETED) == 0)
          {
            if ((portInfo.tunnelPortConfig.type == OFDPA_TUNNEL_PORT_TYPE_ACCESS) &&
                (portInfo.tunnelPortConfig.configData.access.physicalPortNum == portId))
            {
              if (portInfo.tunnelPortConfig.configData.access.untagged)
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                   "Tunnel access port configured to match all traffic on same physical port. groupId = %x, portId = %d, portNum = %x\r\n",
                                   group->groupId, portId, portNum);
                OFDB_LOCK_GIVE;
                return OFDPA_E_ERROR;
              }
              else
              {
                if (portInfo.tunnelPortConfig.configData.access.vlanId == vlanId)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Tunnel access port configured to match same VLAN traffic on same physical port. groupId = %x, portId = %d, vlanId = %d, portNum = %x\r\n",
                                     group->groupId, portId, vlanId, portNum);
                  OFDB_LOCK_GIVE;
                  return OFDPA_E_ERROR;
                }
              }
            }
         }
        }
      }
    }
    break;

  case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
  case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
    /* Validate VLANID */
    vlanId = OFDB_GROUP_VLANID(group->groupId);

    if (!dpaVlanIsValid(vlanId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                         "Invalid VLAN %d!\r\n", vlanId);
      OFDB_LOCK_GIVE;
      return OFDPA_E_PARAM;
    }

    break;

  case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    /* Validate VLANID */
    vlanId = OFDB_GROUP_VLANID(group->groupId);

    if (!dpaVlanIsValid(vlanId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                         "Invalid VLAN %d!\r\n", vlanId);
      OFDB_LOCK_GIVE;
      return OFDPA_E_PARAM;
    }

    rc = OFDPA_E_NONE;
    groupId = 0;

    while (rc == OFDPA_E_NONE)
    {
      rc = ofdbGroupNextGet(groupId, &nextGroup);
      if (rc == OFDPA_E_NONE)
      {
        if ((OFDB_GROUP_TYPE(nextGroup.groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD) &&
            (OFDB_GROUP_VLANID(nextGroup.groupId) == OFDB_GROUP_VLANID(group->groupId)))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                             "Found same VLAN Flood Group entry!\r\n", 0);
          OFDB_LOCK_GIVE;
          return OFDPA_E_PARAM;
        }

        groupId = nextGroup.groupId;
      }
    }

    break;

  case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
    if (OFDPA_E_NONE == (rc = ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN))) 
    {
      /* validate tunnel ID */
      tunnelId = OFDB_GROUP_TUNNELID(group->groupId);
      if (ofdbTenantGet(tunnelId, &tenantConfig, &tenantStatus) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                           "No tenant with matching tunnel ID found. (%d)\r\n", tunnelId);
        OFDB_LOCK_GIVE;
        return OFDPA_E_PARAM;
      }

      /* if added group is subtype OFDPA_L2_OVERLAY_MULTICAST_MULTICAST_TUNNEL or
         OFDPA_L2_OVERLAY_FLOOD_MULTICAST_TUNNEL, check if tenant has MCAST info */
      if ((OFDB_GROUP_TUNNEL_SUBTYPE(group->groupId) == OFDPA_L2_OVERLAY_FLOOD_MULTICAST_TUNNEL) ||
          (OFDB_GROUP_TUNNEL_SUBTYPE(group->groupId) == OFDPA_L2_OVERLAY_MULTICAST_MULTICAST_TUNNEL))
      {
        if (tenantConfig.mcastIp == 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                             "Specified tenant has not configured for multicast. (%d)\r\n", tunnelId);
          OFDB_LOCK_GIVE;
          return OFDPA_E_PARAM;
        }
      }

      /* there can be only one group entry of subtype OFDPA_L2_OVERLAY_FLOOD_MULTICAST_TUNNEL or
         OFDPA_L2_OVERLAY_FLOOD_UNICAST_TUNNEL for the tenant */
      if ((OFDB_GROUP_TUNNEL_SUBTYPE(group->groupId) == OFDPA_L2_OVERLAY_FLOOD_MULTICAST_TUNNEL) ||
          (OFDB_GROUP_TUNNEL_SUBTYPE(group->groupId) == OFDPA_L2_OVERLAY_FLOOD_UNICAST_TUNNEL))
      {
        groupId = 0;
        groupId = OFDB_GROUP_TYPE_SET(groupId, OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY);
        groupId = OFDB_GROUP_TUNNELID_SET(groupId, tunnelId);

        if ((rc = ofdbGroupGet(groupId, &nextGroup)) != OFDPA_E_NONE)
        {
          rc = ofdbGroupNextGet(groupId, &nextGroup);
        }
        while (rc == OFDPA_E_NONE)
        {
          if ((OFDB_GROUP_TYPE(nextGroup.groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY) &&
              (OFDB_GROUP_TUNNELID(nextGroup.groupId) == tunnelId))
          {
            if ((OFDB_GROUP_TUNNEL_SUBTYPE(nextGroup.groupId) == OFDPA_L2_OVERLAY_FLOOD_MULTICAST_TUNNEL) ||
                (OFDB_GROUP_TUNNEL_SUBTYPE(nextGroup.groupId) == OFDPA_L2_OVERLAY_FLOOD_UNICAST_TUNNEL))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                                 "Overlay flood group entry already exists for tenant. (tunnelId = %d)\r\n",
                                 tunnelId);
              OFDB_LOCK_GIVE;
              return OFDPA_E_PARAM;
            }

            groupId = nextGroup.groupId;
            rc = ofdbGroupNextGet(groupId, &nextGroup);
          }
          else
          {
            /* indexed beyond entries for type and tunnel ID, no need to search further */
            rc = OFDPA_E_NOT_FOUND;
          }
        }
      }
    }
    break;

  case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
  case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
  case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
  case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
    break;

  case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:

    /* Validate Group Sub-type */
    subType = OFDB_GROUP_MPLS_SUBTYPE(group->groupId);

    switch (subType)
    {
      case OFDPA_MPLS_INTERFACE:
      case OFDPA_MPLS_L2_VPN_LABEL:
      case OFDPA_MPLS_L3_VPN_LABEL:
      case OFDPA_MPLS_TUNNEL_LABEL1:
      case OFDPA_MPLS_TUNNEL_LABEL2:
      case OFDPA_MPLS_SWAP_LABEL:
        break;
      default:
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Invalid MPLS Label Group Subtype!\r\n", 0);
        OFDB_LOCK_GIVE;
        return OFDPA_E_PARAM;
    }

    break;

  case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:

    /* Validate Group Sub-type */
    subType = OFDB_GROUP_MPLS_SUBTYPE(group->groupId);

    switch (subType)
    {
      case OFDPA_MPLS_ECMP:
        if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_FEATURE_MPLSECMP))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                             "MPLS ECMP not supported.\r\n", 0);
          OFDB_LOCK_GIVE;
          return OFDPA_E_UNAVAIL;
        }
        /* fall through */
      case OFDPA_MPLS_FAST_FAILOVER:
      case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
      case OFDPA_MPLS_L2_TAG:
      case OFDPA_MPLS_L2_FLOOD:
      case OFDPA_MPLS_L2_MULTICAST:
      case OFDPA_MPLS_L2_LOCAL_FLOOD:
      case OFDPA_MPLS_L2_LOCAL_MULTICAST:
      case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
      case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
        break;
      default:
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Invalid MPLS Forwarding Group Subtype!\r\n", 0);
        OFDB_LOCK_GIVE;
        return OFDPA_E_PARAM;
    }

    break;

  default:
    /* Invalid Group ID */
    OFDB_LOCK_GIVE;
    return OFDPA_E_PARAM;
  }

  rc = ofdbGroupAdd(group);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Failed to add Software Group entry !\r\n", 0);
    OFDB_LOCK_GIVE;
    return rc;
  }

	

	switch ( OFDB_GROUP_TYPE(group->groupId) )
	{
			case	OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE :
			case	OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE 	:
			case	OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST 	:
			case	OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE :
			case	OFDPA_GROUP_ENTRY_TYPE_L3_ECMP			:
			case	OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY 	:
			case	OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL 	:
			case	OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE	:
				rc = indirectGroupAdd(group);
				break;
			case	OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING	 :
				rc = indirectGroupAdd(group);
				break;
			case	OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST :
			case	OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD 		:
			case	OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST :
				break;
	    default:
				break;
	}

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Failed to create datapath Group entry !\r\n", 0);
    ofdbGroupDelete(group->groupId);
  }

	rc = ofdbGroupDataUpdate(group);
	if (rc != OFDPA_E_NONE)
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Failed to update software Group entry, rc = %d !\r\n", rc);
		rc = indirectGroupDelete(group);											 
		rc |= ofdbGroupDelete(group->groupId);
	}


  OFDB_LOCK_GIVE;

  return rc;
}

OFDPA_ERROR_t ofdpaGroupDelete(uint32_t groupId)
{
  OFDPA_ERROR_t rc, driverRc;
  ofdpaGroupEntryStats_t groupStats;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                     "Group ID: 0x%x enter\r\n",
                     groupId);
  /* Check if there are no references to the Group */
  OFDB_WRITE_LOCK_TAKE;
  rc = ofdbGroupStatsGet(groupId, &groupStats);

  if (rc == OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                       "Group ID: 0x%x - Group Reference Count = %d\r\n",
                       groupId, groupStats.refCount);

    if (groupStats.refCount == 0)
    {
      /* Delete all buckets from the corresponding Group Bucket Table */
      rc = dpaGroupBucketsDeleteAll(groupId);

      if (OFDPA_E_NONE != rc)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                           "Group ID: 0x%x - Failed to delete all Buckets; rc = %d!\r\n",
                           groupId, rc);
      }
      else
      {
        rc = ofdbGroupDelete(groupId);

        driverRc = OFDPA_E_NONE;

        if (OFDB_GROUP_TYPE(groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST)
        {
        }
        else if (OFDB_GROUP_TYPE(groupId) == OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST)
        {
        }
        else if (OFDB_GROUP_TYPE(groupId) == OFDPA_GROUP_ENTRY_TYPE_L3_ECMP)
        {
        }
        else if (OFDB_GROUP_TYPE(groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD)
        {
        }
        else if (OFDB_GROUP_TYPE(groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY)
        {
          if (OFDPA_E_NONE == (driverRc = ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN)))   
          {
          }
        }
        else if (OFDB_GROUP_IS_MPLS_ECMP(groupId))
        {
        }
        else if (OFDB_GROUP_IS_MPLS_FAST_FAILOVER(groupId))
        {
        }

        if ( driverRc != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                             "Group ID: 0x%x - Failed to delete Hardware Group entry !\r\n",
                             groupId);
          rc = OFDPA_E_FAIL;
        }
      }
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                         "Group ID: 0x%x - Group Reference Count not equal to 0, cannot delete Group \r\n",
                         groupId);
      rc = OFDPA_E_FAIL;
    }
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Group ID: 0x%x - Stats get failed: rc = %d!\r\n",
                       groupId, rc);
  }

  OFDB_LOCK_GIVE;
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                     "Group ID: 0x%x - end; rc = %d\r\n",
                     groupId, rc);
  return rc;
}

OFDPA_ERROR_t ofdpaGroupNextGet(uint32_t groupId, ofdpaGroupEntry_t *nextGroup)
{
  OFDPA_ERROR_t rc;

  if (nextGroup == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Next Group passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbGroupNextGet(groupId, nextGroup);
  OFDB_LOCK_GIVE;

  return rc;
}

OFDPA_ERROR_t ofdpaGroupTypeNextGet(uint32_t groupId,
                                    OFDPA_GROUP_ENTRY_TYPE_t groupType,
                                    ofdpaGroupEntry_t *nextGroup)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  if (nextGroup == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Next Group passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbGroupTypeNextGet(groupId, groupType, nextGroup);
  OFDB_LOCK_GIVE;

  return rc;
}

OFDPA_ERROR_t ofdpaGroupStatsGet(uint32_t groupId, ofdpaGroupEntryStats_t *groupStats)
{
  OFDPA_ERROR_t rc;

  if (groupStats == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Stats passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbGroupStatsGet(groupId, groupStats);
  OFDB_LOCK_GIVE;

  return rc;
}

OFDPA_ERROR_t ofdpaGroupBucketEntryAdd(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc;
  ofdpaGroupEntryStats_t groupStats;
  ofdbPortInfo_t portInfo;
  OFDB_ENTRY_FLAG_t flags;
  uint32_t subType;

  if (groupBucket == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Group Bucket passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_WRITE_LOCK_TAKE;
  if (OFDPA_E_NONE != ofdbGroupStatsGet(groupBucket->groupId, &groupStats))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Group (0x%x) not present in Group Table!\r\n", groupBucket->groupId);
    OFDB_LOCK_GIVE;
    return OFDPA_E_NOT_FOUND;
  }

	/* for fast instert, save the time in searching group's instance */
	groupBucket->ptrGrpInst = groupStats.ptrGrpInst;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "Add Group bucket to obj (%p) !\r\n", groupBucket->ptrGrpInst);


  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                     "Group Bucket count = %d\r\n", groupStats.bucketCount);

  /* check if there is room for another bucket entry in this Group */
  if (groupStats.bucketCount >= ofdbGroupBucketTableMaxCountGet(groupBucket->groupId))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Group 0x%x max count (%d) is not larger than entry count (%d).\r\n",
                       groupBucket->groupId, groupStats.bucketCount,
                       ofdbGroupBucketTableMaxCountGet(groupBucket->groupId));
    OFDB_LOCK_GIVE;
    return OFDPA_E_FULL;
  }

  rc = ofdbGroupBucketValidate(groupBucket);

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Bucket validation failed; rc = %d!\r\n", rc);
    OFDB_LOCK_GIVE;
    return rc;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                     "Adding Bucket in Software\r\n", 0);

  rc = ofdbGroupBucketEntryAdd(groupBucket);

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Failed to add Bucket in software database; rc = %d!\r\n", rc);
    OFDB_LOCK_GIVE;
    return rc;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                     "Adding Bucket in Datapath\r\n", 0);

  switch (OFDB_GROUP_TYPE(groupBucket->groupId))
	{
		case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
		case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
		case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
		case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
		case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
		case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
			rc = indirectGroupBucketEntryAdd(groupBucket);
		  break;
		case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
		case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
		case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
		case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
		  break;

		case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
		  break;

		case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
		  /* Validate Group Sub-type */
		  subType = OFDB_GROUP_MPLS_SUBTYPE(groupBucket->groupId);

		  switch (subType)
		  {
		    case OFDPA_MPLS_ECMP:
		      break;
		    case OFDPA_MPLS_FAST_FAILOVER:
		      break;
		    case OFDPA_MPLS_L2_FLOOD:
		    case OFDPA_MPLS_L2_MULTICAST:
		    case OFDPA_MPLS_L2_LOCAL_FLOOD:
		    case OFDPA_MPLS_L2_LOCAL_MULTICAST:
		    case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
		    case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
		    case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
		    case OFDPA_MPLS_L2_TAG:
		      rc = OFDPA_E_NONE;
		      break;

		    default:
		      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
		                     "Invalid MPLS Forwarding Group Subtype!\r\n", 0);
		      rc = OFDPA_E_PARAM;
		  }

			rc = indirectGroupBucketEntryAdd(groupBucket);
		  break;

		default:
		  /* Invalid Group ID */
		  /* this should never happen since group ID has been validated above */
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_ALWAYS,
		                     "Unexpected Group ID in driver switch statement\r\n", 0);
		  rc = OFDPA_E_PARAM;
	}

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Failed to add Bucket in Hardware; rc = %d!\r\n", rc);

    ofdbGroupBucketEntryDelete(groupBucket->groupId, groupBucket->bucketIndex);
  }

  OFDB_LOCK_GIVE;

  
  return rc;
}

OFDPA_ERROR_t ofdpaGroupBucketEntryDelete(uint32_t groupId, uint32_t bucketIndex)
{
  OFDPA_ERROR_t rc;

  OFDB_WRITE_LOCK_TAKE;
  rc = dpaGroupBucketEntryDelete(groupId, bucketIndex);
  OFDB_LOCK_GIVE;

	
  return rc;
}

OFDPA_ERROR_t ofdpaGroupBucketsDeleteAll(uint32_t groupId)
{
  OFDPA_ERROR_t rc;

  OFDB_WRITE_LOCK_TAKE;
  rc = dpaGroupBucketsDeleteAll(groupId);
  OFDB_LOCK_GIVE;

  return(rc);
}



OFDPA_ERROR_t ofdpaGroupBucketEntryModify(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc;
  ofdpaGroupBucketEntry_t groupBucketEntry;
  uint32_t  subType;

  if (groupBucket == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Group Bucket passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_WRITE_LOCK_TAKE;
  memset(&groupBucketEntry, 0, sizeof(groupBucketEntry));
  if (OFDPA_E_NONE != ofdbGroupBucketEntryGet(groupBucket->groupId, groupBucket->bucketIndex,
                                              &groupBucketEntry))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Group Bucket not found!\r\n", 0);
    OFDB_LOCK_GIVE;
    return OFDPA_E_NOT_FOUND;
  }

  switch (OFDB_GROUP_TYPE(groupBucket->groupId))
  {
  case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    if (groupBucket->bucketData.l2Interface.popVlanTag ==
          groupBucketEntry.bucketData.l2Interface.popVlanTag)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE,
                         "New and old Bucket parameters are the same!\r\n", 0);
      OFDB_LOCK_GIVE;
      return OFDPA_E_PARAM;
    }


    rc = ofdbGroupBucketValidate(groupBucket);
    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                         "Bucket validation failed; rc = %d!\r\n", rc);
      OFDB_LOCK_GIVE;
      return rc;
    }



    break;

  case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
    if ((!memcmp(&groupBucket->bucketData.l3Unicast.srcMac,
                 &groupBucketEntry.bucketData.l3Unicast.srcMac,
                 sizeof(groupBucket->bucketData.l3Unicast.srcMac))) &&
          (!memcmp(&groupBucket->bucketData.l3Unicast.dstMac,
                 &groupBucketEntry.bucketData.l3Unicast.dstMac,
                 sizeof(groupBucket->bucketData.l3Unicast.dstMac))) &&
          (groupBucket->bucketData.l3Unicast.vlanId == groupBucketEntry.bucketData.l3Unicast.vlanId) &&
          (groupBucket->referenceGroupId == groupBucketEntry.referenceGroupId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE,
                         "New and old Bucket parameters are the same!\r\n", 0);
      OFDB_LOCK_GIVE;
      return OFDPA_E_PARAM;
    }

    rc = ofdbGroupBucketValidate(groupBucket);
    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                         "Bucket validation failed; rc = %d!\r\n", rc);
      OFDB_LOCK_GIVE;
      return rc;
    }


    break;

  case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
#ifdef NOT_SUPPORTED
    if ((!memcmp(&groupBucket->bucketData.l3Interface.srcMac,
                 &groupBucketEntry.bucketData.l3Interface.srcMac,
                 sizeof(groupBucket->bucketData.l3Interface.srcMac))) &&
        (groupBucket->bucketData.l3Interface.vlanId == groupBucketEntry.bucketData.l3Interface.vlanId) &&
        (groupBucket->referenceGroupId == groupBucketEntry.referenceGroupId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE,
                         "New and old Bucket parameters are the same!\r\n", 0);
      OFDB_LOCK_GIVE;
      return OFDPA_E_FAIL;
    }

    rc = ofdbGroupBucketValidate(groupBucket);
    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                         "Bucket validation failed; rc = %d!\r\n", rc);
      OFDB_LOCK_GIVE;
      return rc;
    }

    nextGroupId = 0;
    /* Find groups referring this Group. Will be of type L3 Multicast. */
    result = ofdbRefGroupNextGet(groupBucket->groupId, &nextGroupId,
                                 OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST);

    while (result == OFDPA_E_NONE)
    {
      result = ofdbRefGroupNextGet(groupBucket->groupId, &nextGroupId,
                                   OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST);
    }


    nextGroupId = 0;
    result = ofdbRefGroupNextGet(groupBucket->groupId, &nextGroupId,
                                 OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST);

    while (result == OFDPA_E_NONE)
    {
      result = ofdbRefGroupNextGet(groupBucket->groupId, &nextGroupId,
                                   OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST);
    }

#endif /* NOT_SUPPORTED */
  case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
    
    subType = OFDB_GROUP_MPLS_SUBTYPE(groupBucket->groupId);
    
    switch (subType)
    {
      case OFDPA_MPLS_FAST_FAILOVER:
        rc = ofdbGroupBucketValidate(groupBucket);
        if (OFDPA_E_NONE != rc)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                             "Bucket validation failed; rc = %d!\r\n", rc);
          OFDB_LOCK_GIVE;
          return rc;
        }

        /* Check whether the bucket is needed update*/
        if(ofdbGroupBucketDataEntryIsNeedUpdated(groupBucket) == OFDPA_FALSE)
        {
          OFDB_LOCK_GIVE;
          return OFDPA_E_NONE;
        }

        
        rc = ofdbOamLogicalLivenessPortCreate(
          groupBucket->bucketData.mplsFastFailOver.watchPort, groupBucket->groupId);
        if (rc != OFDPA_E_NONE)
        {
          rc = OFDPA_E_FAIL;
        }
        break;
      default:
        break;
    }
    break;
  case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
  case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
  case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
  case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
  case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
  case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
  case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
  case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Group Modify not supported; \r\n", 0);
    /* fall through to default case */

  default:
    /* Invalid Group ID */
    OFDB_LOCK_GIVE;
    return OFDPA_E_PARAM;
  }

  rc = ofdbGroupBucketEntryModify(groupBucket);
  OFDB_LOCK_GIVE;

  return rc;
}

OFDPA_ERROR_t ofdpaGroupBucketEntryGet(uint32_t groupId, uint32_t bucketIndex,
                                       ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc;

  if (groupBucket == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Group Bucket passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbGroupBucketEntryGet(groupId, bucketIndex, groupBucket);
  OFDB_LOCK_GIVE;

  return rc;
}

OFDPA_ERROR_t ofdpaGroupBucketEntryFirstGet(uint32_t groupId,
                                            ofdpaGroupBucketEntry_t *firstGroupBucket)
{
  OFDPA_ERROR_t rc;

  if (firstGroupBucket == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null First Group Bucket passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbGroupBucketEntryFirstGet(groupId, firstGroupBucket);
  OFDB_LOCK_GIVE;

  return rc;
}

OFDPA_ERROR_t ofdpaGroupBucketEntryNextGet(uint32_t groupId, uint32_t bucketIndex,
                                           ofdpaGroupBucketEntry_t *nextBucketEntry)
{
  OFDPA_ERROR_t rc;

  if (nextBucketEntry == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Next Group Bucket passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbGroupBucketEntryNextGet(groupId, bucketIndex, nextBucketEntry);
  OFDB_LOCK_GIVE;

  return rc;
}

OFDPA_ERROR_t ofdpaGroupTableTotalEntryCountGet(uint32_t *entryCount)
{
  if (entryCount == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                      "Null parameter passed.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  *entryCount = ofdbGroupTableTotalEntryCountGet();
  OFDB_LOCK_GIVE;

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaGroupTableInfoGet(uint32_t groupId, ofdpaGroupTableInfo_t *info)
{
  OFDPA_GROUP_ENTRY_TYPE_t groupType = OFDB_GROUP_TYPE(groupId);

  if (info == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                      "Null Info passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  if ((groupType < OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) ||
      (groupType >= OFDPA_GROUP_ENTRY_TYPE_LAST))
  {
    return OFDPA_E_NOT_FOUND;
  }

  OFDB_READ_LOCK_TAKE;
  info->maxGroupEntries = ofdbGroupTableMaxCountGet(groupType);
  info->numGroupEntries = ofdbGroupTableEntryCountGet(groupType);
  info->maxBucketEntries = ofdbGroupBucketTableMaxCountGet(groupId);
  OFDB_LOCK_GIVE;

  return OFDPA_E_NONE;
}


OFDPA_ERROR_t ofdpaPortNextGet(uint32_t portNum, uint32_t *nextPortNum)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  if (nextPortNum == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Next Port Number passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;

  /*GETNEXT时如果是LIVENESSPORT或管理端口，则继续取下一个*/
  do
  {
      if ((rc = ofdbPortNextGet(portNum, nextPortNum)) == OFDPA_E_NONE)
      {
          portNum = *nextPortNum;
      }
  }while (rc == OFDPA_E_NONE && 
    (OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT == OFDB_PORT_TYPE(*nextPortNum) ||
     OFDPA_PORT_TYPE_OUTBAND_PORT == OFDB_PORT_TYPE(*nextPortNum) ||
      OFDPA_PORT_TYPE_INBAND_PORT == OFDB_PORT_TYPE(*nextPortNum)));
  
  OFDB_LOCK_GIVE;

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_TOO_VERBOSE,
                       "Next Port get failed; rc = %d!\r\n", rc);
  }
  return rc;
}

OFDPA_ERROR_t ofdpaPortMacGet(uint32_t portNum, ofdpaMacAddr_t *mac)
{
  OFDPA_ERROR_t rc;

  if (mac == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null MAC passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbPortMacGet(portNum, mac);
  OFDB_LOCK_GIVE;

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "MAC get failed; rc = %d!\r\n", rc);
  }
  return rc;
}

OFDPA_ERROR_t ofdpaPortNameGet(uint32_t portNum, ofdpa_buffdesc *name)
{
  OFDPA_ERROR_t rc;

  if ((name == NULL) || (name->pstart == NULL) || (name->size < OFDPA_PORT_NAME_STRING_SIZE))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null name passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbPortNameGet(portNum, name);
  OFDB_LOCK_GIVE;

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Port Name get failed; rc = %d!\r\n", rc);
  }
  return rc;
}

OFDPA_ERROR_t ofdpaPortStateGet(uint32_t portNum, uint32_t *state)
{
  OFDPA_ERROR_t rc;

  if (state == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null State passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbPortStateGet(portNum, state);
  OFDB_LOCK_GIVE;

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Port State get failed; rc = %d!\r\n", rc);
  }
  return rc;
}

OFDPA_ERROR_t ofdpaPortConfigSet(uint32_t portNum, OFDPA_PORT_CONFIG_t config)
{
  OFDPA_ERROR_t rc;

  OFDB_WRITE_LOCK_TAKE;
  if (!ofdbPortIsValid(portNum))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Invalid port. (inPort:%d)\r\n",
                       portNum);
    OFDB_LOCK_GIVE;
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDB_PORT_TYPE(portNum) == OFDPA_PORT_TYPE_PHYSICAL)
  {
//    rc = driverPortConfigSet(portNum, config);
//
//    if (OFDPA_E_NONE != rc)
//    {
//      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
//                         "Port Config set failed in hardware. rc = %d\r\n", rc);
//      OFDB_LOCK_GIVE;
//      return rc;
//    }
  }

  rc = ofdbPortConfigSet(portNum, config);

  OFDB_LOCK_GIVE;

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Port Config set failed; rc = %d!\r\n", rc);
  }
  return rc;
}

OFDPA_ERROR_t ofdpaPortConfigGet(uint32_t portNum, uint32_t *config)
{
  OFDPA_ERROR_t rc;

  if (config == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE,
                       "Null pointer.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbPortConfigGet(portNum, config);
  OFDB_LOCK_GIVE;

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Port Config get failed; rc = %d!\r\n", rc);
  }
  return rc;
}

OFDPA_ERROR_t ofdpaPortMaxSpeedGet(uint32_t portNum, uint32_t *maxSpeed)
{
  OFDPA_ERROR_t rc;

  if (maxSpeed == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE,
                       "Null pointer.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;
  rc = ofdbPortMaxSpeedGet(portNum, maxSpeed);
  OFDB_LOCK_GIVE;

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Port Max Speed get failed; rc = %d!\r\n", rc);
  }
  return rc;
}

OFDPA_ERROR_t ofdpaPortCurrSpeedGet(uint32_t portNum, uint32_t *currSpeed)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  if (currSpeed == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE,
                       "Null pointer.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  if (OFDB_PORT_TYPE(portNum) != OFDPA_PORT_TYPE_PHYSICAL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERY_VERBOSE,
                       "portNum specifies non-physical port type. (portNum = 0x%x)\r\n",
                       portNum);
    return OFDPA_E_UNAVAIL;
  }

//  OFDB_READ_LOCK_TAKE;
//  rc = driverPortCurrSpeedGet(portNum, currSpeed);
//  OFDB_LOCK_GIVE;
//
//  if (OFDPA_E_NONE != rc)
//  {
//    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
//                       "Port Current Speed get failed; rc = %d!\r\n", rc);
//  }
	*currSpeed = 1000*1000;
  return rc;
}

OFDPA_ERROR_t ofdpaPortFeatureGet(uint32_t portNum, ofdpaPortFeature_t *feature)
{
  OFDPA_ERROR_t rc;
  ofdbPortInfo_t portInfo;
  OFDB_ENTRY_FLAG_t flags = 0;
  OFDPA_PORT_FEATURE_t peer, curr;

  if (feature == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE,
                       "Null pointer.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  OFDB_READ_LOCK_TAKE;

  if ((ofdbPortGet(portNum, &portInfo, &flags) != OFDPA_E_NONE) ||
      (flags & OFDB_PORT_DELETED))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Port entry not found or marked deleted. (portNum:%d)\r\n",
                       portNum);
    rc = OFDPA_E_NOT_FOUND;
  }
  else
  {
    /* feature data is supported for physical ports, otherwise return all 0 */
    if (OFDB_PORT_TYPE(portNum) == OFDPA_PORT_TYPE_PHYSICAL)
    {
        feature->supported = 0;
        feature->advertised = 0;
        feature->curr = 0;
        feature->peer = 0;

        rc = OFDPA_E_NONE;
    }
    else
    {
      feature->advertised = 0;
      feature->supported = 0;
      feature->curr = 0;
      feature->peer = 0;

      rc = OFDPA_E_NONE;
    }
  }

  OFDB_LOCK_GIVE;
  return rc;
}

OFDPA_ERROR_t ofdpaPortAdvertiseFeatureSet(uint32_t portNum, OFDPA_PORT_FEATURE_t advertise)
{
  OFDPA_ERROR_t rc;

  /* feature data is only valid for physical ports */
  if (OFDB_PORT_TYPE(portNum) != OFDPA_PORT_TYPE_PHYSICAL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Port is not a physical port. %x\r\n", portNum);
    return OFDPA_E_PARAM;
  }

  OFDB_WRITE_LOCK_TAKE;
  if (!ofdbPortIsValid(portNum))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Invalid port. (inPort:%d)\r\n",
                       portNum);
    OFDB_LOCK_GIVE;
    return OFDPA_E_NOT_FOUND;
  }
//  rc = driverPortCapabilityAdvertSet(portNum, advertise);
//
//  if (OFDPA_E_NONE != rc)
//  {
//    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
//                       "Port Advertise Feature Set in hardware failed; rc = %d!\r\n", rc);
//    OFDB_LOCK_GIVE;
//    return rc;
//  }

  rc = ofdbPortAdvertiseFeatureSet(portNum, advertise);

  OFDB_LOCK_GIVE;

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Port Advertise Feature Set failed; rc = %d!\r\n", rc);
  }
  return rc;
}

OFDPA_ERROR_t ofdpaPortStatsClear(uint32_t portNum)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  if (OFDB_PORT_TYPE(portNum) != OFDPA_PORT_TYPE_PHYSICAL)
  {
    return OFDPA_E_UNAVAIL;
  }

  OFDB_WRITE_LOCK_TAKE;
  if (!ofdbPortIsValid(portNum))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Invalid port. (inPort:%d)\r\n",
                       portNum);
    OFDB_LOCK_GIVE;
    return OFDPA_E_NOT_FOUND;
  }

  //rc = driverPortStatsClear(portNum);

  OFDB_LOCK_GIVE;

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Port Stats clear failed; rc = %d!\r\n", rc);
    rc = OFDPA_E_FAIL;
  }

  return rc;
}

OFDPA_ERROR_t ofdpaPortStatsGet(uint32_t portNum, ofdpaPortStats_t *stats)
{
  OFDPA_ERROR_t rc;

  if (OFDB_PORT_TYPE(portNum) != OFDPA_PORT_TYPE_PHYSICAL)
  {
    return OFDPA_E_UNAVAIL;
  }

  if (stats == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Null Stats passed!\r\n", 0);
    return OFDPA_E_PARAM;
  }

  memset(stats, 0, sizeof(ofdpaPortStats_t));

  OFDB_READ_LOCK_TAKE;

  if ((rc = ofdbPortDurationGet(portNum, &stats->duration_seconds)) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Invalid port. (inPort:%d)\r\n",
                       portNum);
  }
  else
  {
    if (OFDB_PORT_TYPE(portNum) == OFDPA_PORT_TYPE_PHYSICAL)
    {
//      if (OFDPA_E_NONE != (rc = driverPortStatsGet(portNum, stats)))
//      {
//        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
//                           "Physical port stats get failed; rc = %d.\r\n", rc);
//        rc = OFDPA_E_FAIL;
//      }
    }
  }

  OFDB_LOCK_GIVE;

  return rc;
}

OFDPA_ERROR_t ofdpaPortEventNextGet(ofdpaPortEvent_t *eventData)
{
  OFDPA_ERROR_t rc;

  if (eventData == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* retrieving an event will update status in the port table, so take the WRITE lock */
  OFDB_WRITE_LOCK_TAKE;
  rc = ofdbPortEventNextGet(eventData);
  OFDB_LOCK_GIVE;

  return(rc);
}

OFDPA_ERROR_t ofdpaPortEventGet(ofdpaPortEvent_t *eventData)
{
  OFDPA_ERROR_t rc;

  if (eventData == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* retrieving an event will update status in the port table, so take the WRITE lock */
  OFDB_WRITE_LOCK_TAKE;
  rc = ofdbPortEventGet(eventData);
  OFDB_LOCK_GIVE;

  return(rc);
}





/*****************************************************************************
 Prototype    : ofdpaPortQueueSchedSet
 Description  : This routine set the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                uint32_t mode       
                int32_t *weights  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdpaPortQueueSchedSet(uint32_t port, int32_t mode,int32_t *weights)
{
  return ofdbPortQueueSchedSet(port, mode,  weights);
}

/*****************************************************************************
 Prototype    : ofdpaPortQueueWeightModify
 Description  : This routine modify the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                uint32_t queueId       
                int32_t  weight  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdpaPortQueueWeightModify(uint32_t port,uint32_t queueId, int32_t weight)
{
  return ofdbPortQueueWeightModify(port, queueId, weight);
}


/*****************************************************************************
 Prototype    : ofdpaPortQueueSchedGet
 Description  : Get the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                int32_t *mode       
                int32_t *weights  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdpaPortQueueSchedGet(uint32_t port, int32_t *mode,int32_t *weights)
{
  return ofdbPortQueueSchedGet(port, mode,  weights);
}



/*****************************************************************************
 Prototype    : ofdpaPortDiscardModeSet
 Description  : This routine set the congestion avoidance mode of COS queue on a port.
                
 Input        : uint32_t mode       
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdpaPortDiscardModeSet(uint32_t mode)
{
  return ofdbPortDiscardModeSet(mode);
}




/*****************************************************************************
 Prototype    : ofdpaPortDiscardModeGet
 Description  : Get the congestion avoidance mode of COS queue on a port.
                
 Input        : uint32_t *mode       
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdpaPortDiscardModeGet(uint32_t *mode)
{
  return ofdbPortDiscardModeGet(mode);
}




/*****************************************************************************
 Prototype    : ofdpaPortDiscardConfig
 Description  : This routine configure the congestion avoidance parameters
                of COS queue on a port.
 Input        : uint32_t port     
                uint32_t cosq     
                uint32_t color    
                int drop_start  
                int drop_slope  
                int drop_end    
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdpaPortDiscardConfig(uint32_t port, uint32_t cosq,uint32_t color,
  int drop_start, int drop_slope,int drop_end)
{
  return ofdbPortDiscardConfig(port, cosq, color, drop_start, drop_slope, drop_end);
}




/*****************************************************************************
 Prototype    : ofdpaPortDiscardConfigGet
 Description  : Get the congestion avoidance parameters
                of COS queue on a port.
 Input        : uint32_t port     
                uint32_t cosq     
                uint32_t color    
                int *drop_start  
                int *drop_slope  
                int *drop_end    
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  ofdpaPortDiscardConfigGet(uint32_t port, uint32_t cosq,uint32_t color,
  int *drop_start, int *drop_slope,int *drop_end)
{
  return ofdbPortDiscardConfigGet(port, cosq, color, drop_start, drop_slope, drop_end);
}

	
	OFDPA_ERROR_t ofdpaNumQueuesGet(uint32_t portNum, uint32_t *numQueues)
	{
		OFDPA_ERROR_t result = OFDPA_E_NONE;
	
		if (numQueues == NULL)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Null numQueues passed!\r\n", 0);
			return OFDPA_E_PARAM;
		}
	
		OFDB_READ_LOCK_TAKE;
	
		if (!ofdbPortIsValid(portNum))
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Invalid input port %u.\r\n",
												 portNum);
			OFDB_LOCK_GIVE;
			return OFDPA_E_NOT_FOUND;
		}
	
		result = dpaCosQueuesMaxGet(numQueues);
	
		OFDB_LOCK_GIVE;
	
		return result;
	}
	
	OFDPA_ERROR_t ofdpaQueueStatsGet(uint32_t portNum, uint32_t queueId, ofdpaPortQueueStats_t *stats)
	{
		OFDPA_ERROR_t rc;
	
		if (stats == NULL)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Null stats passed!\r\n", 0);
			return OFDPA_E_PARAM;
		}
	
		OFDB_READ_LOCK_TAKE;
	
		if (!ofdbPortIsValid(portNum))
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Invalid input port %u.\r\n",
												 portNum);
			OFDB_LOCK_GIVE;
			return OFDPA_E_NOT_FOUND;
		}
	
		rc = ofdbPortQueueIdIsValid(portNum, queueId);
		if (rc != OFDPA_E_NONE)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Invalid port queue %u; rc = %d.\r\n", queueId, rc);
			OFDB_LOCK_GIVE;
			return OFDPA_E_PARAM;
		}
	
	
//		rc = driverPortQueueStatsGet(portNum, queueId, stats);
//		if (rc != OFDPA_E_NONE)
//		{
//			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
//												 "Could not get port queue statistics.; rc = %d\r\n", rc);
//			OFDB_LOCK_GIVE;
//			return rc;
//		}
	
		/* Queues will initialize during port initialization.
			 Hence duration since Queues are up would be same as port uptime */
		rc = ofdbPortDurationGet(portNum, &(stats->duration_seconds));
		OFDB_LOCK_GIVE;
	
		if (rc != OFDPA_E_NONE)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Could not get the duration since the port queue is initialized!!!\r\n", 0);
			return rc;
		}
	
		return rc;
	}
	
	OFDPA_ERROR_t ofdpaQueueStatsClear(uint32_t portNum, uint32_t queueId)
	{
		OFDPA_ERROR_t rc = OFDPA_E_NONE;
	
		OFDB_WRITE_LOCK_TAKE;
	
		if (!ofdbPortIsValid(portNum))
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Invalid input port %u.\r\n",
												 portNum);
			OFDB_LOCK_GIVE;
			return OFDPA_E_NOT_FOUND;
		}
	
		rc = ofdbPortQueueIdIsValid(portNum, queueId);
		if (rc != OFDPA_E_NONE)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Invalid port queue %u; rc = %d.\r\n", queueId, rc);
			OFDB_LOCK_GIVE;
			return OFDPA_E_PARAM;
		}
	
		//rc = driverPortQueueStatsClear(portNum, queueId);
	
		OFDB_LOCK_GIVE;
	
		if (rc != OFDPA_E_NONE)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Could not get port queue statistics; rc = %d\r\n", rc);
			return rc;
		}
	
		return rc;
	}
	
	OFDPA_ERROR_t ofdpaQueueRateSet(uint32_t portNum, uint32_t queueId, uint32_t minRate, uint32_t maxRate)
	{
		OFDPA_ERROR_t rc;
	
		OFDB_WRITE_LOCK_TAKE;
	
		if (!ofdbPortIsValid(portNum))
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Invalid input port %u.\r\n",
												 portNum);
			OFDB_LOCK_GIVE;
			return OFDPA_E_NOT_FOUND;
		}
	
		rc = ofdbPortQueueIdIsValid(portNum, queueId);
		if (rc != OFDPA_E_NONE)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Invalid port queue %u; rc = %d.\r\n", queueId, rc);
			OFDB_LOCK_GIVE;
			return OFDPA_E_PARAM;
		}
	
		if (minRate > maxRate)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Min queue rate must be less than max queue rate.\r\n", 0);
			OFDB_LOCK_GIVE;
			return OFDPA_E_PARAM;
		}
	
		if ((minRate < 1 || minRate > 1000) || (maxRate < 1 || maxRate > 1000))
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Min and Max queue rates must range between 1-1000%.\r\n", 0);
			OFDB_LOCK_GIVE;
			return OFDPA_E_PARAM;
		}
	
//		rc = driverPortQueueMinMaxRateSet(portNum, queueId, minRate, maxRate);
//		if (OFDPA_E_NONE != rc)
//		{
//			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
//												 "Port Queue rate set in hardware failed; rc = %d!\r\n", rc);
//			OFDB_LOCK_GIVE;
//			return rc;
//		}
	
		rc = ofdbPortQueueMinMaxRateSet(portNum, queueId, minRate, maxRate);
	
		OFDB_LOCK_GIVE;
	
		if (OFDPA_E_NONE != rc)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Port Queue rate set failed; rc = %d!\r\n", rc);
		}
	
		return rc;
	}
	
	OFDPA_ERROR_t ofdpaQueueRateGet(uint32_t portNum, uint32_t queueId, uint32_t *minRate, uint32_t *maxRate)
	{
		OFDPA_ERROR_t rc;
	
		if (minRate == NULL)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Null min rate passed!\r\n", 0);
			return OFDPA_E_PARAM;
	
		}
	
		if (maxRate == NULL)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Null max rate passed!\r\n", 0);
			return OFDPA_E_PARAM;
		}
	
		OFDB_READ_LOCK_TAKE;
	
		if (!ofdbPortIsValid(portNum))
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Invalid input port %u.\r\n",
												 portNum);
			OFDB_LOCK_GIVE;
			return OFDPA_E_NOT_FOUND;
		}
	
		rc = ofdbPortQueueIdIsValid(portNum, queueId);
		if (rc != OFDPA_E_NONE)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Invalid port queue %u; rc = %d.\r\n", queueId, rc);
			OFDB_LOCK_GIVE;
			return OFDPA_E_PARAM;
		}
	
		rc = ofdbPortQueueMinMaxRateGet(portNum, queueId, minRate, maxRate);
	
		OFDB_LOCK_GIVE;
	
		if (OFDPA_E_NONE != rc)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Port Queue rate get failed; rc = %d!\r\n", rc);
		}
	
		return rc;
	}

