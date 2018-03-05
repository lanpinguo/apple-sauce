/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2016
*
**********************************************************************
*
* @filename     ofdb_oam_api.c
*
* @purpose      Openflow tables related to OAM.
*
* @component    OFDB
*
* @comments     none
*
* @create       11/09/2014
*
* @end
*
**********************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ofdb_api.h"
#include "ofdb.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"
#include "datapath.h"
#include "dot1ag_exports.h"
#include "dot1ag_api.h"
#include "dot1ag_pdu.h"
#include "g8131_exports.h"
#include "g8131_api.h"
//#include "driver_group.h"
#include "pw_redundancy_api.h"
#include "pw_redundancy_cfg.h"

#define OFDPA_DOT1AG_BHH_BASE_PRIMVID  10000

#if 0
static uint32_t  ofdbOamEventMapping[OFDB_OAM_EVENT_MAX_TYPE] =
{
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointPortDown = 0 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointPortUp = 1 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceDown = 2 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceUp = 3 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceTestingToUp = 4 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceUnknownToUp = 5 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceDormantToUp = 6 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceNotPresentToUp = 7 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceLLDownToUp = 8 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceTesting = 9 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceUnkonwn = 10 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceDormant = 11 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceNotPresent = 12 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointInterfaceLLDown = 13 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupCCMxcon = 14 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupCCMError = 15 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupRemote = 16 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupCCMTimeout = 17 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventBHHLBTimeout = 18 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventBHHLBDiscoveryUpdate = 19 */
  (OFDB_OAM_EVENT_LOC | OFDB_OAM_EVENT_PRESENT), /* bcmOAMEventBHHCCMTimeout = 20  */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventBHHCCMState = 21  */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointCCMTimeout = 22  */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointCCMTimein = 23  */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointCCMTimeoutEarly = 24  */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointRemote = 25  */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointRemoteUp = 26 */
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointDmStatistics = 27 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEndpointLmStatistics = 28 */ 
  (OFDB_OAM_EVENT_RDI | OFDB_OAM_EVENT_PRESENT), /* bcmOAMEventBHHCCMRdi = 29 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventBHHCCMUnknownMegLevel = 30 */ 
  (OFDB_OAM_EVENT_MMG | OFDB_OAM_EVENT_PRESENT), /* bcmOAMEventBHHCCMUnknownMegId = 31 */ 
  (OFDB_OAM_EVENT_UNM | OFDB_OAM_EVENT_PRESENT), /* bcmOAMEventBHHCCMUnknownMepId = 32 */ 
  (OFDB_OAM_EVENT_UNP | OFDB_OAM_EVENT_PRESENT), /* bcmOAMEventBHHCCMUnknownPeriod = 33 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventBHHCCMUnknownPriority = 34 */ 
  OFDB_OAM_EVENT_RDI ,                /* bcmOAMEventBHHCCMRdiClear = 35 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventBHHCCMUnknownMegLevelClear = 36 */
  OFDB_OAM_EVENT_MMG ,                /* bcmOAMEventBHHCCMUnknownMegIdClear = 37 */ 
  OFDB_OAM_EVENT_UNM ,                /* bcmOAMEventBHHCCMUnknownMepIdClear = 38 */ 
  OFDB_OAM_EVENT_UNP ,                /* bcmOAMEventBHHCCMUnknownPeriodClear = 39 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventBHHCCMUnknownPriorityClear = 40 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventBHHRawData = 41 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEthLmDmRawData = 42 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventMplsLmDmRawData = 43 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupMACStatus = 44 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupCCMxconClear = 45 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupCCMErrorClear = 46 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupRemoteClear = 47 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupCCMTimeoutClear = 48 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventGroupMACStatusClear = 49 */ 
  OFDB_OAM_EVENT_LOC ,                /* bcmOAMEventBHHCCMTimeoutClear = 50 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventBhhPmCounterRollover = 51 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventEthLmDmPmCounterRollover = 52 */ 
  OFDB_OAM_EVENT_NONE,                /* bcmOAMEventMplsLmDmPmCounterRollover = 53 */
};
#endif

uint32_t ofdbOamBhhVidGet(uint32_t megIndex, ofdpaOamMegConfig_t *config);

OFDPA_ERROR_t ofdbOamLogicalLivenessPortCreate(uint32_t portId, uint32_t ffGroupId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbPortInfo_t portInfo;

  /* Assure port ID is proper type */
  if (OFDB_PORT_TYPE(portId) != OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT)
  {
    rc = OFDPA_E_ERROR;
  }
  else
  {
    /* check if port entry already exists */
    if (!ofdbPortIsValid(portId))
    {
      memset(&portInfo, 0, sizeof(portInfo));

      /* generate a port name for the logical liveness port */
      snprintf(portInfo.name, sizeof(portInfo.name), "LiveP-%08x", portId);
      /* initialize port as administratively up */
      portInfo.config &= ~OFDPA_PORT_CONFIG_DOWN;
      /* initialize port state as up */
      portInfo.state &= ~OFDPA_PORT_STATE_LINK_DOWN;
      /* initialize port as live */
      portInfo.state |= OFDPA_PORT_STATE_LIVE;
      portInfo.ffGroupId = ffGroupId;

      rc = ofdbPortAdd(portId, &portInfo);

      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Error creating Logical Liveness Port in port table. (livenessLogicalPortId: 0x%x)\r\n",
                           portId);
        rc = OFDPA_E_FAIL;
      }
    }
    else
    {
      
      if(ffGroupId != OFDPA_INVALID_GROUP_ID)
      {
        memset(&portInfo, 0, sizeof(portInfo));

        /* generate a port name for the logical liveness port */
        snprintf(portInfo.name, sizeof(portInfo.name), "LiveP-%08x", portId);
        /* initialize port as administratively up */
        portInfo.config &= ~OFDPA_PORT_CONFIG_DOWN;
        /* initialize port state as up */
        portInfo.state &= ~OFDPA_PORT_STATE_LINK_DOWN;
        /* initialize port as live */
        portInfo.state |= OFDPA_PORT_STATE_LIVE;

        portInfo.ffGroupId = ffGroupId;
        rc = ofdbPortInfoSet(portId, &portInfo);

        if (rc != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Error modifying Logical Liveness Port in port table. (livenessLogicalPortId: 0x%x)\r\n",
                             portId);
          rc = OFDPA_E_FAIL;
        }
      }


    }
  }

  return(rc);
}

int ofdbOamMegTypeIsValid(OFDPA_OAM_MEG_TYPE_t megType)
{
  int result;

  switch (megType)
  {
    case OFDPA_OAM_MEG_TYPE_ETHERNET:
    case OFDPA_OAM_MEG_TYPE_G8113_1:
      result = 1;
      break;
    default:
      result = 0;
      break;
  }

  return(result);
}

int ofdbOamMegConfigValidate(uint32_t megIndex, ofdpaOamMegConfig_t *config)
{
  uint32_t primVid;

  primVid = ofdbOamBhhVidGet(megIndex, config);
  if(primVid > 0xffff)
  {
	 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE, 
	 	"OAM MEG config error: megIndex = %d > %d. \r\n", megIndex, (0xffff-OFDPA_DOT1AG_BHH_BASE_PRIMVID));
	return 0;
  }
	
  return 1;
}

static ofdbOamMegTable_node_t *ofdbOamMegNodeGet(uint32_t megIndex)
{
  return(avlSearch(&ofdbAVLTrees.ofdbOamMegTable_tree, &megIndex, AVL_EXACT));
}

static ofdbOamMepTable_node_t *ofdbOamMepNodeGet(uint32_t lmepId)
{
  return(avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT));
}

static ofdbOamMepTable_node_t *ofdbOamMepNodeNextGet(uint32_t lmepId)
{
  return(avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_NEXT));
}

static ofdbOamMipTable_node_t *ofdbOamMipNodeGet(uint32_t localMpId)
{
  return(avlSearch(&ofdbAVLTrees.ofdbOamMipTable_tree, &localMpId, AVL_EXACT));
}

static ofdbOamMipTable_node_t *ofdbOamMipNodeNextGet(uint32_t localMpId)
{
  return(avlSearch(&ofdbAVLTrees.ofdbOamMipTable_tree, &localMpId, AVL_NEXT));
}

static ofdbOamRemoteMpTable_node_t *ofdbOamRemoteMpNodeGet(uint32_t remoteMpId)
{
  return(avlSearch(&ofdbAVLTrees.ofdbOamRemoteMpTable_tree, &remoteMpId, AVL_EXACT));
}

static ofdbOamRemoteMpTable_node_t *ofdbOamRemoteMpNodeNextGet(uint32_t remoteMpId)
{
  return(avlSearch(&ofdbAVLTrees.ofdbOamRemoteMpTable_tree, &remoteMpId, AVL_NEXT));
}

#if 1
static ofdbBfdTable_node_t *ofdbBfdNodeGet(uint32_t lmepId)
{
  return(avlSearch(&ofdbAVLTrees.ofdbBfdTable_tree, &lmepId, AVL_EXACT));
}

static ofdbY1564Table_node_t *ofdbY1564NodeGet(uint32_t lmepId)
{
  return(avlSearch(&ofdbAVLTrees.ofdbY1564Table_tree, &lmepId, AVL_EXACT));
}

static ofdbLbTable_node_t *ofdbLoopbackNodeGet(uint32_t lmepId)
{
  return(avlSearch(&ofdbAVLTrees.ofdbLbTable_tree, &lmepId, AVL_EXACT));
}


#endif


/*根据LMEPID 查找OAM 测试类型*/
OFDPA_ERROR_t ofdbOamTypeGet(uint32_t lmepId, OFDB_OAM_TYPE_t* pOamtype)
{
	OFDB_OAM_TYPE_t oamtype = OFDB_OAM_TYPE_OAM_MEP;
	OFDPA_ERROR_t rc  = OFDPA_E_NOT_FOUND;
	
	if(ofdbBfdNodeGet(lmepId) != NULL)
	{
		oamtype = OFDB_OAM_TYPE_BFD;		
		rc =  OFDPA_E_NONE;
	}
	else if(ofdbOamMepNodeGet(lmepId) != NULL)
	{
		oamtype = OFDB_OAM_TYPE_OAM_MEP;
		rc =  OFDPA_E_NONE;
	}
	else if(ofdbOamMipNodeGet(lmepId) != NULL)
	{
		oamtype = OFDB_OAM_TYPE_OAM_MIP;
		rc =  OFDPA_E_NONE;
	}
	else if(ofdbY1564NodeGet(lmepId) != NULL)
	{
		oamtype = OFDB_OAM_TYPE_Y1564;
		rc =  OFDPA_E_NONE;
	}
	else if(ofdbLoopbackNodeGet(lmepId) != NULL)
	{
		oamtype = OFDB_OAM_TYPE_Y1564_LB;
		rc =  OFDPA_E_NONE;
	}
	else if(ofdbLmepidGet(lmepId, &oamtype) == OFDPA_E_NONE)
	{
		rc =  OFDPA_E_NONE;
	}
	
	if(pOamtype != NULL)
	{
		*pOamtype = oamtype;
	}
	
	return rc;

}


uint32_t ofdbOamBhhVidGet(uint32_t megIndex, ofdpaOamMegConfig_t *config)
{
  uint32_t primVid;

  /* the VLAN ID used to configure the MA will be a generated number for BHH and a configured number if Ethernet */
  if (config->megType == OFDPA_OAM_MEG_TYPE_ETHERNET)
  {
    primVid = config->primVid;
  }
  else
  {
    primVid = OFDPA_DOT1AG_BHH_BASE_PRIMVID + megIndex;
  }

  return(primVid);
}

OFDPA_OAM_MEG_TYPE_t ofdbMegTypeFromVidGet(uint32_t primVid)
{
  if (dpaVlanIsValid(primVid))
  {
    return OFDPA_OAM_MEG_TYPE_ETHERNET;
  }
  else if (primVid >= OFDPA_DOT1AG_BHH_BASE_PRIMVID)
  {
    return OFDPA_OAM_MEG_TYPE_G8113_1;
  }
  else
  {
    return -1;
  }
}

static OFDPA_ERROR_t mplsTpLabelHeaderPrepare(uint32_t label, uint8_t exp, uint8_t bos,
                                              uint8_t ttl, ofdbOamLmepData_t *oamLmepData)
{
  OFDPA_ERROR_t  rc = OFDPA_E_NONE;

  if (oamLmepData->numLabels < 3)
  {
    mplsTpLabelPrepare(label, exp, bos, (0 == ttl) ? 255 : ttl,
                       (uint8_t *)&oamLmepData->mplsHeader[oamLmepData->numLabels++]);
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "Max allowed labels exceeded. No of labels in current label stack = %d ",
                     oamLmepData->numLabels);
    rc = OFDPA_E_FULL;
  }
  return rc;
}

static OFDPA_ERROR_t ofdbInjectedOamLmepIdMapL2InterfaceGet(uint32_t groupId,
                                                            ofdbInjectedOamLmepIdDataMappingTable_node_t *mapNode)
{
  OFDPA_ERROR_t                                    rc;
  ofdpaGroupBucketEntry_t                          bucketEntry;

  rc = ofdbGroupBucketEntryFirstGet(groupId, &bucketEntry);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Could not retrieve bucket entry for groupId = 0x%08x.\r\n",
                       groupId);
    return(OFDPA_E_NOT_FOUND);
  }

  if (OFDB_GROUP_TYPE(groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
  {
    mapNode->oamLmepData.ifNum = bucketEntry.bucketData.l2Interface.outputPort;
    mapNode->oamLmepData.popVlanTag = bucketEntry.bucketData.l2Interface.popVlanTag;
  }
  else if (OFDB_GROUP_TYPE(groupId) == OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE)
  {
    mapNode->oamLmepData.ifNum = bucketEntry.bucketData.l2UnfilteredInterface.outputPort;
    mapNode->oamLmepData.popVlanTag = 0;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Group entry reference chain did not end at L2 Interface or L2 Unfiltered Interface group. (GroupId = 0x%08x)\r\n",
                       groupId);
    return(OFDPA_E_ERROR);
  }

  return(OFDPA_E_NONE);
}

static OFDPA_ERROR_t ofdbInjectedOamLmepIdMapMplsInterfaceGet(uint32_t groupId,
                                                              ofdbInjectedOamLmepIdDataMappingTable_node_t *mapNode)
{
  OFDPA_ERROR_t                                    rc;
  ofdpaGroupBucketEntry_t                          bucketEntry;
  uint32_t                                         currentGroupId;
  OFDPA_GROUP_ENTRY_TYPE_t                         currentGroupType;

  currentGroupId = groupId;

  rc = ofdbGroupBucketEntryFirstGet(currentGroupId, &bucketEntry);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Could not retrieve bucket entry for currentGroupId = 0x%08x.\r\n",
                       currentGroupId);
    return(OFDPA_E_ERROR);
  }

  memcpy(&mapNode->oamLmepData.destMac, &bucketEntry.bucketData.mplsInterface.dstMac, sizeof(mapNode->oamLmepData.destMac));
  memcpy(&mapNode->oamLmepData.srcMac, &bucketEntry.bucketData.mplsInterface.srcMac, sizeof(mapNode->oamLmepData.srcMac));
  mapNode->oamLmepData.vlanId = bucketEntry.bucketData.mplsInterface.vlanId & OFDPA_VID_EXACT_MASK;

  /* move to next group in reference chain */
  currentGroupId = bucketEntry.referenceGroupId;
  currentGroupType = OFDB_GROUP_TYPE(currentGroupId);

  if ((currentGroupType == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) || (currentGroupType == OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE))
  {
    rc = ofdbInjectedOamLmepIdMapL2InterfaceGet(currentGroupId, mapNode);
  }
  else
  {
    return(OFDPA_E_ERROR);
  }

  return rc;
}

static OFDPA_ERROR_t ofdbInjectedOamLmepIdMapTunnelLabelGet(uint32_t groupId,
                                                            ofdbInjectedOamLmepIdDataMappingTable_node_t *mapNode)
{
  OFDPA_ERROR_t                                    rc;
  ofdpaGroupBucketEntry_t                          bucketEntry;
  uint32_t                                         currentGroupId;

  currentGroupId = groupId;

  rc = ofdbGroupBucketEntryFirstGet(currentGroupId, &bucketEntry);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Could not retrieve bucket entry for currentGroupId = 0x%08x.\r\n",
                       currentGroupId);
    return(OFDPA_E_ERROR);
  }

  rc = mplsTpLabelHeaderPrepare(bucketEntry.bucketData.mplsLabel.mplsLabel,
                                bucketEntry.bucketData.mplsLabel.mplsEXP,
                                0,
                                bucketEntry.bucketData.mplsLabel.mplsTTL,
                                &mapNode->oamLmepData);

  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  /* move to next group in reference chain */
  currentGroupId = bucketEntry.referenceGroupId;

  rc = ofdbGroupBucketEntryFirstGet(currentGroupId, &bucketEntry);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Could not retrieve bucket entry for currentGroupId = 0x%08x.\r\n",
                       currentGroupId);
    return(OFDPA_E_ERROR);
  }

  if (OFDB_GROUP_IS_MPLS_TUNNEL_LABEL2(currentGroupId))
  {
    rc = mplsTpLabelHeaderPrepare(bucketEntry.bucketData.mplsLabel.mplsLabel,
                                  bucketEntry.bucketData.mplsLabel.mplsEXP,
                                  0,
                                  bucketEntry.bucketData.mplsLabel.mplsTTL,
                                  &mapNode->oamLmepData);

    if (rc != OFDPA_E_NONE)
    {
      return rc;
    }

    /* move to next group in reference chain */
    currentGroupId = bucketEntry.referenceGroupId;

    rc = ofdbGroupBucketEntryFirstGet(currentGroupId, &bucketEntry);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Could not retrieve bucket entry for currentGroupId = 0x%08x.\r\n",
                         currentGroupId);
      return(OFDPA_E_ERROR);
    }
  }

  if (OFDB_GROUP_IS_MPLS_INTERFACE(currentGroupId))
  {
    /* move to next group in reference chain */
    rc = ofdbInjectedOamLmepIdMapMplsInterfaceGet(currentGroupId, mapNode);
  }
  else
  {
    return(OFDPA_E_ERROR);
  }

  return rc;
}

static OFDPA_ERROR_t ofdbInjectedOamLmepIdMapMPLSFFOLabelGet(uint32_t groupId,
                                                             ofdbInjectedOamLmepIdDataMappingTable_node_t *mapNode)
{
  OFDPA_ERROR_t                                    rc;
  ofdpaGroupBucketEntry_t                          bucketEntry;
  uint32_t                                         currentGroupId;
  uint32_t                                         operGroupId;
  int                                              failoverValue = 0;
  uint32_t                                         tempBucketIndex;

  currentGroupId = groupId;

  //rc = driverMplsFastFailoverGet(currentGroupId, &failoverValue);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  rc = ofdbGroupBucketEntryFirstGet(currentGroupId, &bucketEntry);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Could not retrieve bucket entry for currentGroupId = 0x%08x.\r\n",
                       currentGroupId);
    return(OFDPA_E_ERROR);
  }

  if (0 == failoverValue)
  {
    /* working path */
    operGroupId = bucketEntry.referenceGroupId;
  }
  else
  {
    /* protection path */
    tempBucketIndex = bucketEntry.bucketIndex;

    rc = ofdbGroupBucketEntryNextGet(currentGroupId, tempBucketIndex, &bucketEntry);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Could not retrieve bucket entry for currentGroupId = 0x%08x.\r\n",
                         currentGroupId);
      return(OFDPA_E_ERROR);
    }

    operGroupId = bucketEntry.referenceGroupId;
  }

  if (OFDB_GROUP_IS_MPLS_TUNNEL_LABEL1(operGroupId))
  {
    rc = ofdbInjectedOamLmepIdMapTunnelLabelGet(operGroupId, mapNode);
    if (rc != OFDPA_E_NONE)
    {
      return rc;
    }
  }
  else
  {
    rc = OFDPA_E_ERROR;
  }
  return rc;
}

OFDPA_ERROR_t ofdbInjectedOamLmepIdMapEntryAdd(ofdpaInjectedOamFlowEntry_t *flowData)
{
  OFDPA_ERROR_t                                 rc             = OFDPA_E_NONE;
  ofdbInjectedOamLmepIdDataMappingTable_node_t  mapNode;
  ofdbInjectedOamLmepIdDataMappingTable_node_t *dataPtr;
  uint32_t                                      currentGroupId = 0;
  ofdbOamMepTable_node_t                       *mepNode;
  ofdbOamMipTable_node_t                       *mipNode;
  ofdbOamMegTable_node_t                       *megNode;
  uint32_t                                      megIndex;

  OFDB_OAM_TYPE_t oamtype = OFDB_OAM_TYPE_OAM_MEP;

  memset(&mapNode, 0, sizeof(mapNode));
  mapNode.lmepId = flowData->match_criteria.lmepId;

  if (OFDPA_E_NONE != ofdbFlowInjectedOamEntryTypeGet(flowData, &mapNode.oamLmepData.oamType))
  {
    /* This should not happen as the entry has already passed validation */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "The match criteria are not a valid combination.\r\n", 0);
    return OFDPA_E_ERROR;
  }

  switch (mapNode.oamLmepData.oamType)
  {
    case OFDB_OAM_TYPE_ETHERNET_UP:
      mapNode.oamLmepData.tunnelId = flowData->tunnelId;
      mapNode.oamLmepData.mplsL2Port = flowData->mplsL2Port;
      mapNode.oamLmepData.oamLmTxCountAction = flowData->oamLmTxCountAction;

      break;
    case OFDB_OAM_TYPE_ETHERNET_DOWN:

      rc = ofdbInjectedOamLmepIdMapL2InterfaceGet(flowData->groupId, &mapNode);
      if (rc != OFDPA_E_NONE)
      {
        return rc;
      }

      currentGroupId = flowData->groupId;

      break;

    case OFDB_OAM_TYPE_SECTION:
    case OFDB_OAM_TYPE_LSP:
      mapNode.oamLmepData.mplsDataFirstNibble = flowData->mplsDataFirstNibble;
      mapNode.oamLmepData.mplsAchChannel = flowData->mplsAchChannel;

      rc = mplsTpLabelHeaderPrepare(13, flowData->mplsGalTC, 1, flowData->mplsGalTTL,
                                    &mapNode.oamLmepData);

      if (rc != OFDPA_E_NONE)
      {
        return rc;
      }

      currentGroupId = flowData->groupId;

      if (OFDB_GROUP_IS_MPLS_INTERFACE(currentGroupId))
      {
        rc = ofdbInjectedOamLmepIdMapMplsInterfaceGet(currentGroupId, &mapNode);
        if (rc != OFDPA_E_NONE)
        {
          return rc;
        }
      }

      if (OFDB_OAM_TYPE_LSP == mapNode.oamLmepData.oamType)
      {
        rc = mplsTpLabelHeaderPrepare(flowData->mplsLabel, flowData->mplsTC, flowData->mplsBOS,
                                      flowData->mplsTTL, &mapNode.oamLmepData);

        if (rc != OFDPA_E_NONE)
        {
          return rc;
        }

      }

      break;

    case OFDB_OAM_TYPE_PW_VCCV_TYPE_1_OR_3:
    case OFDB_OAM_TYPE_PW_VCCV_TYPE_4:
      mapNode.oamLmepData.mplsDataFirstNibble = flowData->mplsDataFirstNibble;
      mapNode.oamLmepData.mplsAchChannel = flowData->mplsAchChannel;
      mapNode.oamLmepData.pushCW = flowData->pushCW;

      if (OFDB_OAM_TYPE_PW_VCCV_TYPE_4 == mapNode.oamLmepData.oamType)
      {
        rc = mplsTpLabelHeaderPrepare(13, flowData->mplsGalTC, 1, flowData->mplsGalTTL,
                                      &mapNode.oamLmepData);

        if (rc != OFDPA_E_NONE)
        {
          return rc;
        }
      }

      rc = mplsTpLabelHeaderPrepare(flowData->mplsLabel, flowData->mplsTC, flowData->mplsBOS,
                                    flowData->mplsTTL, &mapNode.oamLmepData);

      if (rc != OFDPA_E_NONE)
      {
        return rc;
      }

      currentGroupId = flowData->groupId;

      if (OFDB_GROUP_IS_MPLS_TUNNEL_LABEL1(currentGroupId))
      {
        rc = ofdbInjectedOamLmepIdMapTunnelLabelGet(currentGroupId, &mapNode);
        if (rc != OFDPA_E_NONE)
        {
          return rc;
        }
      }
      else if (OFDB_GROUP_IS_MPLS_FAST_FAILOVER(currentGroupId))
      {
        rc = ofdbInjectedOamLmepIdMapMPLSFFOLabelGet(currentGroupId, &mapNode);
        if (rc != OFDPA_E_NONE)
        {
          return rc;
        }
      }

      break;

    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERY_VERBOSE,
                         "Called with subtype group entry not used in mapping entry. (groupId = 0x%08x)\r\n",
                         flowData->groupId);
      return(OFDPA_E_NONE);
      break;
  }

	rc = ofdbOamTypeGet(flowData->match_criteria.lmepId, &oamtype); 
	if (OFDPA_E_NONE != rc)
	{
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
						 "Failed get lmepId %d type. (rc = %d)\r\n",
						 flowData->match_criteria.lmepId, rc);
	}

    if(oamtype == OFDB_OAM_TYPE_OAM_MEP || oamtype == OFDB_OAM_TYPE_OAM_MIP)
	{
	  mepNode = ofdbOamMepNodeGet(flowData->match_criteria.lmepId);
	  if (mepNode == NULL)
	  {
	    mipNode = ofdbOamMipNodeGet(flowData->match_criteria.lmepId);
	    if (NULL == mipNode)
	    {
	      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
	                         "Failed retrieving MP entry for lmpId = %d.\r\n",
	                         flowData->match_criteria.lmepId);
	      return(OFDPA_E_NOT_FOUND);
	    }
	    megIndex = mipNode->config.megIndex;
	  }
	  else
	  {
	    megIndex = mepNode->config.megIndex;
	    mapNode.oamLmepData.livenessLogicalPortId = mepNode->config.mlp.u.livenessLogicalPortId;
	  }

	  megNode = ofdbOamMegNodeGet(megIndex);
	  if (megNode == NULL)
	  {
	    /* this should not happen since a MEP node was found using this megIndex, the MEG entry should be present */
	    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
	                       "Failed retrieving MEG entry for lmpId = %d. (megIndex = %d)\r\n",
	                       flowData->match_criteria.lmepId, megIndex);
	    return(OFDPA_E_INTERNAL);
	  }

	  mapNode.oamLmepData.megLevel = megNode->config.level;
	  
	}

  /* completed getting all the information for the entry */
  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbInjectedOamLmepIdDataMappingTable_tree, &mapNode);

  if (dataPtr == &mapNode)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Entry exists. (lmepId = %d)\r\n",
                       mapNode.lmepId);
    rc = OFDPA_E_EXISTS;
  }
  else                                  /* successful */
  {
    if (0 != currentGroupId)            /* not all entries reference groups */
    {
      /* Update group reference count */
      ofdbGroupReferenceUpdate(currentGroupId, 1);
    }
  }
  return(rc);
}

OFDPA_ERROR_t ofdbInjectedOamLmepIdMapEntryDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t                                       rc;
  ofdbInjectedOamLmepIdDataMappingTable_node_t       *dataPtr;
  ofdpaFlowEntry_t                                    flow;
  ofdbFlowStatus_t                                    flowStatus;
  OFDB_OAM_TYPE_ID_t                                  oamType;

  /* Get the OAM type first. Need this to update group references */
  dataPtr = avlSearch(&ofdbAVLTrees.ofdbInjectedOamLmepIdDataMappingTable_tree, &lmepId, AVL_EXACT);
  if (dataPtr == 0)
  {
    /* Not found. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Could not find entry in mapping table.\r\n", 0);
    return OFDPA_E_NOT_FOUND;
  }

  oamType = dataPtr->oamLmepData.oamType;

  dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbInjectedOamLmepIdDataMappingTable_tree,
                           &lmepId);

  ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_INJECTED_OAM, &flow);
  flow.flowData.injectedOamFlowEntry.match_criteria.lmepId = lmepId;

  if ((rc = ofdbFlowGet(&flow, NULL, &flowStatus)) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Could not find entry in ofdb.\r\n", 0);
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDB_OAM_TYPE_ETHERNET_UP != oamType)
  {
    /* Update group reference count */
    ofdbGroupReferenceUpdate(flow.flowData.injectedOamFlowEntry.groupId, 0);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbInjectedOamLmepIdMapDataGet(uint32_t lmepId, ofdbOamLmepData_t **oamMapData)
{
  OFDPA_ERROR_t                                    rc = OFDPA_E_NOT_FOUND;
  ofdbInjectedOamLmepIdDataMappingTable_node_t    *dataPtr;

  if (oamMapData == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbInjectedOamLmepIdDataMappingTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    *oamMapData = &dataPtr->oamLmepData;
    rc = OFDPA_E_NONE;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbInjectedOamLmepIdMapDataUpdate(uint32_t ffGroupId)
{
  OFDPA_ERROR_t     rv = OFDPA_E_NONE;
  ofdpaFlowEntry_t  flowEntry;
  ofdbFlowStatus_t  flow_status;

  ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_INJECTED_OAM, &flowEntry);

  while ((OFDPA_E_NONE == ofdbFlowNextGet(&flowEntry, &flowEntry, NULL, &flow_status)) &&
         (OFDPA_FLOW_TABLE_ID_INJECTED_OAM == flowEntry.tableId))
  {
    if (OFDB_GROUP_IS_MPLS_FAST_FAILOVER(flowEntry.flowData.injectedOamFlowEntry.groupId) &&
      (flowEntry.flowData.injectedOamFlowEntry.groupId == ffGroupId))
    {
      rv = ofdbInjectedOamLmepIdMapEntryDelete(flowEntry.flowData.injectedOamFlowEntry.match_criteria.lmepId);

      if (OFDPA_E_NONE != rv)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                          "ofdbInjectedOamLmepIdMapEntryDelete() failed for lmep id %d rv = %d.\r\n",
                          flowEntry.flowData.injectedOamFlowEntry.match_criteria.lmepId, rv);

        return rv;
      }

      rv = ofdbInjectedOamLmepIdMapEntryAdd(&flowEntry.flowData.injectedOamFlowEntry);

      if (OFDPA_E_NONE != rv)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                          "ofdbInjectedOamLmepIdMapEntryAdd() failed for lmep id %d rv = %d.\r\n",
                          flowEntry.flowData.injectedOamFlowEntry.match_criteria.lmepId, rv);

        return rv;
      }
    }
  }
  return(rv);
}

OFDPA_ERROR_t ofdbOamMegConfig(ofdbOamMegTable_node_t *node)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdpaOamMegConfig_t *config;
  uint32_t mdIndex = 0;
  uint32_t maIndex = 0;
  uint32_t primVid;

  config = &node->config;

  do
  {
    rc = dot1agMdMdIndexGet(NULL, config->level, &mdIndex);

    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Error retrieving mdIndex. (rc = %d)\r\n", rc);
      break;
    }

    rc = dot1agMdMaNextIndexGet(mdIndex, &maIndex);
    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Error retrieving maIndex. (rc = %d)\r\n", rc);
      break;
    }

    primVid = ofdbOamBhhVidGet(node->megIndex, config);

    rc = dot1agMaCreate(mdIndex, maIndex, config->megId, primVid);
    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Error from dot1agMegCreate(). (rc = %d)\r\n", rc);
      break;
    }
    else
    {
      rc = dot1agMaCCMIntervalSet(mdIndex, maIndex, config->ccmInterval);
      if (OFDPA_E_NONE != rc)
      {
        dot1agMaDelete(mdIndex, maIndex);
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Error from dot1agMaCCMIntervalSet(). (rc = %d)\r\n", rc);
        break;
      }
      else
      {
        node->megInfo.dot1agMdIndex = mdIndex;
        node->megInfo.dot1agMaIndex = maIndex;
        node->megInfo.primVid = primVid;
      }
    }
  } while (0);

  return(rc);
}

OFDPA_ERROR_t ofdbOamMegUnconfig(ofdbOamMegTable_node_t *node)
{
  OFDPA_ERROR_t rc;

  rc = dot1agMaDelete(node->megInfo.dot1agMdIndex, node->megInfo.dot1agMaIndex);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error returned from dot1agMaDelete(). rc = %d, mdIndex = %d, maIndex = %d\r\n",
                       rc, node->megInfo.dot1agMdIndex, node->megInfo.dot1agMaIndex);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamMegAdd(uint32_t megIndex, ofdpaOamMegConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMegTable_node_t node, *dataPtr;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(&node, 0, sizeof(node));

  node.megIndex = megIndex;
  memcpy(&node.config, config, sizeof(node.config));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbOamMegTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
//    rc = OFDPA_E_EXISTS;
	rc = OFDPA_E_NONE; /*add by wangjing,解决MEG和MEP一起下发问题*/
    printf("ofdbOamMegAdd: MEG ADD exist megindex:%d \r\n", node.megIndex);
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMegTable_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. megIndex = %d\r\n",
                         megIndex);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /* configure MEG in OAM subsystem */
      rc = ofdbOamMegConfig(dataPtr);

      if (rc != OFDPA_E_NONE)
      {
        avlDeleteEntry(&ofdbAVLTrees.ofdbOamMegTable_tree, dataPtr);
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMegDelete(uint32_t megIndex)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMegTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.megIndex = megIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMegTable_tree, &node, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (dataPtr->status.refCount == 0)
    {
      rc = ofdbOamMegUnconfig(dataPtr);
      avlDeleteEntry(&ofdbAVLTrees.ofdbOamMegTable_tree, dataPtr);
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Attempted to delete OAM MEG entry with refCount not 0. megIndex = %d, refCount = %d\r\n",
                         megIndex, dataPtr->status.refCount);
      rc = OFDPA_E_FAIL;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMegModify(uint32_t megIndex, ofdpaOamMegConfig_t *newConfig)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMegTable_node_t node, *dataPtr;

  if (newConfig == NULL)
  {
    /* this should never happen; if it does, something is wrong with the AVL library */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "Empty config data. megIndex = %d\r\n",
                       megIndex);  
    return OFDPA_E_PARAM;
  }

  memset(&node, 0, sizeof(node));
  node.megIndex = megIndex;
  
  /* retrieve pointer to node in AVL tree */
  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMegTable_tree, &node, AVL_EXACT);

  if (dataPtr == NULL)
  {
    /* this should never happen; if it does, something is wrong with the AVL library */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "Failed to find AVL tree entry just added. megIndex = %d\r\n",
                       megIndex);
    rc = OFDPA_E_INTERNAL;
  }
  else
  {
    /* Only ccmInterval modification is supported */
    if (dataPtr->config.ccmInterval != newConfig->ccmInterval)
    {
      rc = dot1agMaCCMIntervalSet(dataPtr->megInfo.dot1agMdIndex, dataPtr->megInfo.dot1agMaIndex, newConfig->ccmInterval);
      if (OFDPA_E_NONE != rc)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Error from dot1agMaCCMIntervalSet(). (rc = %d)\r\n", rc);
      }
      dataPtr->config.ccmInterval = newConfig->ccmInterval;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMegGet(uint32_t megIndex, ofdpaOamMegConfig_t *config, ofdpaOamMegStatus_t *status, ofdbOamMegInfo_t *info)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMegTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.megIndex = megIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMegTable_tree, &node, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (config)
    {
      memcpy(config, &dataPtr->config, sizeof(dataPtr->config));
    }
    if (status)
    {
      memcpy(status, &dataPtr->status, sizeof(dataPtr->status));
    }
    if (info)
    {
      memcpy(info, &dataPtr->megInfo, sizeof(dataPtr->megInfo));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMegNextGet(uint32_t megIndex, uint32_t *nextMegIndex)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMegTable_node_t node, *dataPtr;

  if (nextMegIndex == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(&node, 0, sizeof(node));

  node.megIndex = megIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMegTable_tree, &node, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextMegIndex = dataPtr->megIndex;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMegReferenceUpdate(uint32_t megIndex, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMegTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.megIndex = megIndex;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMegTable_tree, &node, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->status.refCount++;
    }
    else
    {
      if (dataPtr->status.refCount > 0)
      {
        dataPtr->status.refCount--;
      }
    }
  }

  return rc;
}

int ofdbOamMepIdIsValid(uint16_t mepId)
{
  int result = 0;

  if ((mepId > 0) && (mepId <= 8191))
  {
    result = 1;
  }

  return(result);
}

int ofdbOamMepConfigValidate(uint32_t lmepId, ofdpaOamMepConfig_t *config)
{
  uint32_t lmepIdIter = 0;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMegTable_node_t *megNodeIter = NULL;

  if (0 == lmepId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "lmepId cannot be zero.\r\n", 0);
    return(0);
  }

  /* create Logical Liveness Port entry if specified */
  if (config->mlp.u.livenessLogicalPortId != 0)
  {
    if (OFDB_PORT_TYPE(config->mlp.u.livenessLogicalPortId) != OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Port identifier is wrong type for livenessLogicalPortId = 0x%x. (lmepId = %d)\r\n",
                         config->mlp.u.livenessLogicalPortId, lmepId);
      return(0);
    }
  }

  if ((megNode = ofdbOamMegNodeGet(config->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, config->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  if (megNode->config.megType == OFDPA_OAM_MEG_TYPE_ETHERNET)
  {
    while (NULL != (mepDataPtr = ofdbOamMepNodeNextGet(lmepIdIter)))
    {
      if (((megNodeIter = ofdbOamMegNodeGet(mepDataPtr->config.megIndex)) != NULL) &&
          (megNodeIter->config.megType == OFDPA_OAM_MEG_TYPE_ETHERNET))
      {
        if ((config->ifIndex == mepDataPtr->config.ifIndex) &&
            (memcmp(config->macAddress.addr, mepDataPtr->config.macAddress.addr, sizeof(config->macAddress.addr))) != 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "MEP MAC address is already assigned to lmep %d on same interface\r\n",
                             mepDataPtr->lmepId);
          return(0);
        }
      }
      lmepIdIter = mepDataPtr->lmepId;
    }
  }

  return 1;
}


OFDPA_ERROR_t ofdbOamMepConfig(ofdbOamMepTable_node_t *node)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *config;
  ofdbOamMegTable_node_t *megNode;
  Dot1agCfmMpDirection_t dot1agDirection;
  int mepCreated = 0;

  config = &node->config;

  /* parameter mapping */
  switch (config->direction)
  {
    case OFDPA_MP_DIRECTION_DOWN:
      dot1agDirection = DOT1AG_MP_DIRECTION_DOWN;
      break;
    case OFDPA_MP_DIRECTION_UP:
      dot1agDirection = DOT1AG_MP_DIRECTION_UP;
      break;
    default:
      rc = OFDPA_E_PARAM;
      break;
  }

  if (rc == OFDPA_E_NONE)
  {
    do
    {
      if ((megNode = ofdbOamMegNodeGet(config->megIndex)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                           node->lmepId, config->megIndex);
        rc = OFDPA_E_NOT_FOUND;
        break;
      }

      rc = dot1agMepCreate(config->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                           config->mepId, dot1agDirection, config->macAddress.addr, node->lmepId);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Error from dot1agMepCreate(). rc = %d\r\n",
                           rc);
        rc = OFDPA_E_FAIL;
        break;
      }
      mepCreated = 1;

      rc = dot1agMepCcmLtmPrioritySet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, config->mepId, config->ifIndex, config->ccmPriority);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Error from dot1agMepCcmLtmPrioritySet(). rc = %d\r\n",
                           rc);
        rc = OFDPA_E_FAIL;
        break;
      }

      rc = dot1agMepActiveModeSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, config->mepId, config->ifIndex, OFDPA_TRUE);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Error from dot1agMepActiveModeSet(). rc = %d\r\n",
                           rc);
        rc = OFDPA_E_FAIL;
        break;
      }

      rc = dot1agMepCCIEnabledSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, config->mepId, config->ifIndex, OFDPA_TRUE);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Error from dot1agMepCCIEnabledSet(). rc = %d\r\n",
                           rc);
        rc = OFDPA_E_FAIL;
        break;
      }
    } while (0);
  }

  if ((rc != OFDPA_E_NONE) && mepCreated)
  {
    dot1agMepDelete(config->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, config->mepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamMepUnconfig(ofdbOamMepTable_node_t *node)
{
  OFDPA_ERROR_t           rc;
  ofdpaOamMepConfig_t    *config;
  ofdbOamMegTable_node_t *megNode;

  config = &node->config;

  if ((megNode = ofdbOamMegNodeGet(config->megIndex)) == NULL)
  {
    return(OFDPA_E_NOT_FOUND);
  }

  rc = dot1agMepDelete(config->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, config->mepId);

  return(rc);
}

OFDPA_ERROR_t ofdbOamMepAdd(uint32_t lmepId, ofdpaOamMepConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t node, *dataPtr;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Check if localMpId already used by a MIP */
  if (OFDPA_E_NONE == ofdbOamMipGet(lmepId, NULL, NULL))
  {
    return OFDPA_E_EXISTS;
  }

  memset(&node, 0, sizeof(node));

  node.lmepId = lmepId;
  memcpy(&node.config, config, sizeof(node.config));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbOamMepTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. lmepId = %d\r\n",
                         lmepId);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /* configure MEP in OAM subsystem */
      rc = ofdbOamMepConfig(dataPtr);

      if (rc == OFDPA_E_NONE)
      {
        ofdbOamMegReferenceUpdate(config->megIndex, 1);
      }
      else
      {
        /* clean up MEP table entry */
        avlDeleteEntry(&ofdbAVLTrees.ofdbOamMepTable_tree, dataPtr);
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMepDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMepTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (dataPtr->status.refCount != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Attempted to delete OAM MEP entry with refCount not 0. "
                         "lmepId = %d, refCount = %d\r\n",
                         lmepId, dataPtr->status.refCount);
      rc = OFDPA_E_FAIL;
    }
    else
    {
      rc = ofdbOamMepUnconfig(dataPtr);
      ofdbOamMegReferenceUpdate(dataPtr->config.megIndex, 0);

      dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbOamMepTable_tree, dataPtr);
      if (dataPtr == 0)
      {
        /* this should never happen; if it does, something is wrong with the AVL library */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete AVL tree entry just found. lmepId = %d\r\n",
                           lmepId);
        rc = OFDPA_E_INTERNAL;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMepModify(uint32_t lmepId, ofdpaOamMepConfig_t *newConfig)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t node, *dataPtr;
  ofdbOamMegTable_node_t *megNode;  
  ofdbInjectedOamLmepIdDataMappingTable_node_t *injectedOamLmepMapNode = NULL;

  if (newConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(&node, 0, sizeof(node));
  node.lmepId = lmepId;

  /* successful insertion, retrieve pointer to node in AVL tree */
  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &node, AVL_EXACT);
  if (dataPtr == NULL)
  {
    /* this should never happen; if it does, something is wrong with the AVL library */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "Failed to find AVL tree entry added before. lmepId = %d\r\n",
                       lmepId);
    rc = OFDPA_E_INTERNAL;
  }
  else
  {
    if (memcmp(&dataPtr->config.mlp, &newConfig->mlp, sizeof(mlpConfig_t)) != 0)
    {
      memcpy(&dataPtr->config.mlp, &newConfig->mlp, sizeof(mlpConfig_t));
      /* Update injected OAM LMEP_ID mapping table */
      injectedOamLmepMapNode = avlSearch(&ofdbAVLTrees.ofdbInjectedOamLmepIdDataMappingTable_tree, &lmepId, AVL_EXACT);
      if (injectedOamLmepMapNode != NULL)
      {
        injectedOamLmepMapNode->oamLmepData.livenessLogicalPortId = newConfig->mlp.u.livenessLogicalPortId;
      }       
    }
    if (dataPtr->config.ccmPriority != newConfig->ccmPriority)
    {
      if ((megNode = ofdbOamMegNodeGet(dataPtr->config.megIndex)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                           lmepId, dataPtr->config.megIndex);
        return OFDPA_E_ERROR;
      }
      
      rc = dot1agMepCcmLtmPrioritySet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, dataPtr->config.mepId, dataPtr->config.ifIndex, newConfig->ccmPriority);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Error from dot1agMepCcmLtmPrioritySet(). rc = %d\r\n",
                           rc);
        rc = OFDPA_E_FAIL;
        return rc;
      }      
      dataPtr->config.ccmPriority = newConfig->ccmPriority;      
    }

    if (dataPtr->config.mepId != newConfig->mepId)
    {
      dataPtr->config.mepId = newConfig->mepId;      
    }

  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMepGet(uint32_t lmepId, ofdpaOamMepConfig_t *config, ofdpaOamMepStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMepTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (config)
    {
      memcpy(config, &dataPtr->config, sizeof(dataPtr->config));
    }
    if (status)
    {
      memcpy(status, &dataPtr->status, sizeof(dataPtr->status));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMepNextGet(uint32_t lmepId, uint32_t *nextLmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMepTable_node_t *dataPtr;

  if (nextLmepId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextLmepId = dataPtr->lmepId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMepReferenceUpdate(uint32_t lmepId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMepTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->status.refCount++;
    }
    else
    {
      if (dataPtr->status.refCount > 0)
      {
        dataPtr->status.refCount--;
      }
    }
  }

  return rc;
}


OFDPA_ERROR_t ofdbOamMepTimeoutUpdate(uint32_t lmepId, uint32_t timeout)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMepTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    dataPtr->status.timeout = timeout;
  }

  return rc;
}


OFDPA_ERROR_t ofdbOamMepTimeoutGet(uint32_t lmepId, uint32_t *timeout)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMepTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    *timeout = dataPtr->status.timeout;
  }

  return rc;
}




int ofdbOamMipConfigValidate(uint32_t localMpId, ofdpaOamMipConfig_t *config)
{
  if (0 == localMpId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "localMpId cannot be zero.\r\n", 0);
    return 0;
  }

  if ((OFDPA_MP_DIRECTION_DOWN != config->direction) &&
      (OFDPA_MP_DIRECTION_UP   != config->direction))
  {
    return 0;
  }

  return 1;
}


OFDPA_ERROR_t ofdbOamMipConfig(ofdbOamMipTable_node_t *node)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMipConfig_t    *config;
  ofdbOamMegTable_node_t *megNode;

  config = &node->config;

  megNode = ofdbOamMegNodeGet(config->megIndex);
  if (megNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. localMpId = %d, megIndex = %d\r\n",
                       node->localMpId, config->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agMipCreate(config->ifIndex, megNode->megInfo.primVid, megNode->config.level,
                       config->direction, config->macAddress.addr, config->mipIdTlvData,
                       node->localMpId);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agMipCreate(). rc = %d\r\n",
                       rc);
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMipUnconfig(ofdbOamMipTable_node_t *node)
{
  ofdpaOamMipConfig_t    *config;
  ofdbOamMegTable_node_t *megNode;

  config = &node->config;

  megNode = ofdbOamMegNodeGet(config->megIndex);
  if (NULL == megNode)
  {
    return OFDPA_E_NOT_FOUND;
  }

  return dot1agMipDelete(config->ifIndex, megNode->megInfo.primVid, megNode->config.level, config->direction);
}

OFDPA_ERROR_t ofdbOamMipAdd(uint32_t localMpId, ofdpaOamMipConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMipTable_node_t node, *dataPtr;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Check if localMpId already used by a MEP */
  if (OFDPA_E_NONE == ofdbOamMepGet(localMpId, NULL, NULL))
  {
    return OFDPA_E_EXISTS;
  }

  memset(&node, 0, sizeof(node));

  node.localMpId = localMpId;
  memcpy(&node.config, config, sizeof(node.config));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbOamMipTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = ofdbOamMipNodeGet(localMpId);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. localMpId = %d\r\n",
                         localMpId);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /* configure MIP in OAM subsystem */
      rc = ofdbOamMipConfig(dataPtr);

      if (rc == OFDPA_E_NONE)
      {
        ofdbOamMegReferenceUpdate(config->megIndex, 1);
      }
    }

    if (rc != OFDPA_E_NONE)
    {
      /* clean up MIP table entry */
      avlDeleteEntry(&ofdbAVLTrees.ofdbOamMipTable_tree, dataPtr);
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMipDelete(uint32_t localMpId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMipTable_node_t *dataPtr;

  dataPtr = ofdbOamMipNodeGet(localMpId);

  if (dataPtr != 0)
  {
    if (dataPtr->status.refCount != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Attempted to delete OAM MIP entry with refCount not 0. "
                         "localMpId = %d, refCount = %d\r\n",
                         localMpId, dataPtr->status.refCount);
      rc = OFDPA_E_FAIL;
    }
    else
    {
      rc = ofdbOamMipUnconfig(dataPtr);
      ofdbOamMegReferenceUpdate(dataPtr->config.megIndex, 0);

      dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbOamMipTable_tree, dataPtr);
      if (dataPtr == 0)
      {
        /* this should never happen; if it does, something is wrong with the AVL library */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete AVL tree entry just found. localMpId = %d\r\n",
                           localMpId);
        rc = OFDPA_E_INTERNAL;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMipGet(uint32_t localMpId, ofdpaOamMipConfig_t *config, ofdpaOamMipStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMipTable_node_t *dataPtr;

  dataPtr = ofdbOamMipNodeGet(localMpId);

  if (dataPtr != 0)
  {
    if (config)
    {
      memcpy(config, &dataPtr->config, sizeof(dataPtr->config));
    }
    if (status)
    {
      memcpy(status, &dataPtr->status, sizeof(dataPtr->status));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMipNextGet(uint32_t localMpId, uint32_t *nextMipId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMipTable_node_t *dataPtr;

  if (nextMipId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = ofdbOamMipNodeNextGet(localMpId);

  if (dataPtr != NULL)
  {
    *nextMipId = dataPtr->localMpId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMipReferenceUpdate(uint32_t localMpId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMipTable_node_t *dataPtr;

  dataPtr = ofdbOamMipNodeGet(localMpId);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->status.refCount++;
    }
    else
    {
      if (dataPtr->status.refCount > 0)
      {
        dataPtr->status.refCount--;
      }
    }
  }

  return rc;
}

int ofdbOamRemoteMpConfigValidate(uint32_t rmpId, ofdpaOamRemoteMpConfig_t *config)
{
  return 1;
}

OFDPA_ERROR_t ofdbOamRemoteMpConfig(ofdbOamRemoteMpTable_node_t *node)
{
  OFDPA_ERROR_t                 rc = OFDPA_E_NONE;
  ofdpaOamRemoteMpConfig_t     *config;
  ofdbOamMepTable_node_t       *mepNode;
  ofdbOamMegTable_node_t       *megNode;

  config = &node->config;

  if (rc == OFDPA_E_NONE)
  {
    do
    {
      if ((mepNode = ofdbOamMepNodeGet(node->config.lmepId)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "MEP entry not found. rmpId = %d, lmepId = %d\r\n",
                           node->rmpId, node->config.lmepId);
        rc = OFDPA_E_NOT_FOUND;
        break;
      }

      if ((megNode = ofdbOamMegNodeGet(mepNode->config.megIndex)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "MEG entry not found. rmpId = %d, lmepId = %d, megIndex = %d\r\n",
                           node->rmpId, node->config.lmepId, mepNode->config.megIndex);
        rc = OFDPA_E_NOT_FOUND;
        break;
      }

      rc = dot1agRMepEntryAdd(mepNode->config.ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                              mepNode->config.mepId, config->mepId, config->macAddress.addr);
      if (rc != OFDPA_E_NONE)
      {
        rc = OFDPA_E_FAIL;
        break;
      }
#if 0
      if (config->groupId != 0)
      {
        rc = ofdbOamBhhPrimVlanMapEntryAdd(config->groupId);
        if (rc == OFDPA_E_NOT_FOUND)
        {
          /* entry will not be added if other needed configuration is not present, this is not an error */
          rc = OFDPA_E_NONE;
        }
      }
#endif
    } while (0);
  }

  return(rc);
}



OFDPA_ERROR_t ofdbOamRemoteMpCfgFind(ofdbOamRemoteMpTable_node_t *node)
{
  OFDPA_ERROR_t                 rc = OFDPA_E_NONE;
  ofdpaOamRemoteMpConfig_t     *config;
  ofdbOamMepTable_node_t       *mepNode;
  ofdbOamMegTable_node_t       *megNode;

  config = &node->config;

  if (rc == OFDPA_E_NONE)
  {
    do
    {
      if ((mepNode = ofdbOamMepNodeGet(node->config.lmepId)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "MEP entry not found. rmpId = %d, lmepId = %d\r\n",
                           node->rmpId, node->config.lmepId);
        rc = OFDPA_E_NOT_FOUND;
        break;
      }

      if ((megNode = ofdbOamMegNodeGet(mepNode->config.megIndex)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "MEG entry not found. rmpId = %d, lmepId = %d, megIndex = %d\r\n",
                           node->rmpId, node->config.lmepId, mepNode->config.megIndex);
        rc = OFDPA_E_NOT_FOUND;
        break;
      }

	  if(dot1agCfmRMepObjGet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
	  				mepNode->config.mepId, config->mepId, AVL_EXACT) == NULL)
	  {
		rc = OFDPA_E_NOT_FOUND;
        break;
	  }
      
    } while (0);
	
  }

  return(rc);
}


OFDPA_ERROR_t ofdbOamRemoteMpUnconfig(ofdbOamRemoteMpTable_node_t *node)
{
  OFDPA_ERROR_t                 rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t       *mepNode;
  ofdbOamMegTable_node_t       *megNode;

  if (rc == OFDPA_E_NONE)
  {
    do
    {
      if ((mepNode = ofdbOamMepNodeGet(node->config.lmepId)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "MEP entry not found. rmpId = %d, lmepId = %d\r\n",
                           node->rmpId, node->config.lmepId);
        rc = OFDPA_E_NOT_FOUND;
        break;
      }

      if ((megNode = ofdbOamMegNodeGet(mepNode->config.megIndex)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "MEG entry not found. rmpId = %d, lmepId = %d, megIndex = %d\r\n",
                           node->rmpId, node->config.lmepId, mepNode->config.megIndex);
        rc = OFDPA_E_NOT_FOUND;
        break;
      }

      rc = dot1agRMepEntryDelete(mepNode->config.ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                 mepNode->config.mepId, node->rmpId);
      if (rc != OFDPA_E_NONE)
      {
        rc = OFDPA_E_FAIL;
        break;
      }
#if 0
      if (node->config.groupId != 0)
      {
        rc = ofdbOamBhhPrimVlanMapEntryDelete(node->config.groupId);
        if (rc == OFDPA_E_NOT_FOUND)
        {
          /* entry will not be added if other needed configuration is not present, this is not an error */
          rc = OFDPA_E_NONE;
        }
      }
#endif
    } while (0);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamRemoteMpAdd(uint32_t rmpId, ofdpaOamRemoteMpConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamRemoteMpTable_node_t node, *dataPtr;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /*暂时先不考虑localmep与remotemep openflowmpid同时全局唯一,仅LOCALmep全局唯一 ADD BY WANGJING */
  /* Check if localMpId already used by a MEP or MIP
  if ((OFDPA_E_NONE == ofdbOamMepGet(rmpId, NULL, NULL)) ||
      (OFDPA_E_NONE == ofdbOamMipGet(rmpId, NULL, NULL)))
  {
    return OFDPA_E_EXISTS;
  }*/

  memset(&node, 0, sizeof(node));

  node.rmpId = rmpId;
  memcpy(&node.config, config, sizeof(node.config));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbOamRemoteMpTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamRemoteMpTable_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. rmpId = %d\r\n",
                         rmpId);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /* configure MP in OAM subsystem */
      rc = ofdbOamRemoteMpConfig(dataPtr);

      if (rc == OFDPA_E_NONE)
      {
        ofdbOamMepReferenceUpdate(config->lmepId, 1);
      }
    }

    if (rc != OFDPA_E_NONE)
    {
      /* clean up table entry */
      avlDeleteEntry(&ofdbAVLTrees.ofdbOamRemoteMpTable_tree, dataPtr);
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamRemoteMpDelete(uint32_t remoteMpId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamRemoteMpTable_node_t *dataPtr;

  dataPtr = ofdbOamRemoteMpNodeGet(remoteMpId);

  if (dataPtr != 0)
  {
    rc = ofdbOamRemoteMpUnconfig(dataPtr);
    ofdbOamMepReferenceUpdate(dataPtr->config.lmepId, 0);
    avlDeleteEntry(&ofdbAVLTrees.ofdbOamRemoteMpTable_tree, dataPtr);
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamRemoteMpFind(uint32_t rmpId, ofdpaOamRemoteMpConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamRemoteMpTable_node_t node, *dataPtr;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }


  memset(&node, 0, sizeof(node));

  node.rmpId = rmpId;
  memcpy(&node.config, config, sizeof(node.config));

  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamRemoteMpTable_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. rmpId = %d\r\n",
                         rmpId);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /* configure MP in OAM subsystem */
      rc = ofdbOamRemoteMpCfgFind(dataPtr);     
    }

    
  }

  return rc;
}


OFDPA_ERROR_t ofdbOamRemoteMpGet(uint32_t remoteMpId, ofdpaOamRemoteMpConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamRemoteMpTable_node_t *dataPtr;

  dataPtr = ofdbOamRemoteMpNodeGet(remoteMpId);

  if (dataPtr != 0)
  {
    if (config)
    {
      memcpy(config, &dataPtr->config, sizeof(dataPtr->config));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamRemoteMpNextGet(uint32_t remoteMpId, uint32_t *nextRemoteMpId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamRemoteMpTable_node_t *dataPtr;

  if (nextRemoteMpId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = ofdbOamRemoteMpNodeNextGet(remoteMpId);

  if (dataPtr != 0)
  {
    *nextRemoteMpId = dataPtr->rmpId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMepCCMDatabaseEntryGet(uint32_t lmepId, 
                                            uint32_t remoteMepId,
                                            ofdpaOamCcmDatabaseEntry_t *dbEntry)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NOT_FOUND;
  ofdpaOamMepConfig_t     *mepConfig = NULL;
  ofdbOamMegTable_node_t  *megNode = NULL;
  ofdbOamMepTable_node_t  *mepDataPtr = NULL;
  ofdbOamRemoteMpTable_node_t *rMepDataPtr = NULL;
  uint32_t                mepId; 
  
  mepDataPtr = ofdbOamMepNodeGet(lmepId);

  if (mepDataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;
  mepId     = mepConfig->mepId;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  rMepDataPtr = ofdbOamRemoteMpNodeGet(remoteMepId);

  if (rMepDataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "RMEP entry for remote mepId = %d not found\r\n",
                       remoteMepId);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agOamMepCCMDatabaseEntryGet(megNode->megInfo.dot1agMdIndex, 
                                       megNode->megInfo.dot1agMaIndex, 
                                       mepId, rMepDataPtr->config.mepId, dbEntry);
  
  return(rc);
}

OFDPA_ERROR_t ofdbOamMepCCMDatabaseEntryNextGet(uint32_t lmepId, 
                                                uint32_t remoteMepId,
                                                uint32_t *nextRemoteMepId)
{
  ofdpaOamMepConfig_t         *mepConfig = NULL;
  ofdbOamMegTable_node_t      *megNode = NULL;
  ofdbOamMepTable_node_t      *mepDataPtr = NULL;
  ofdbOamRemoteMpTable_node_t *rMepDataPtr = NULL;
  uint32_t                     rMepIdIter = remoteMepId;
  
  mepDataPtr = ofdbOamMepNodeGet(lmepId);

  if (mepDataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  while (NULL != (rMepDataPtr = ofdbOamRemoteMpNodeNextGet(rMepIdIter)))
  {
    if (rMepDataPtr->config.lmepId == lmepId)
    {
      *nextRemoteMepId = rMepDataPtr->rmpId;
       return OFDPA_E_NONE;
    }
    rMepIdIter = rMepDataPtr->rmpId;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                     "Next RMEP entry for remote mepId = %d not found\r\n",
                     remoteMepId);

  return OFDPA_E_NOT_FOUND;
}

static ofdbOamMlpGroupTable_node_t *ofdbOamMLPGroupNodeGet(uint32_t index)
{
  return(avlSearch(&ofdbAVLTrees.ofdbOamMLPGroupTable_tree, &index, AVL_EXACT));
}

int ofdbOamMLPGroupConfigValidate(uint32_t index, ofdpaOamMLPGroupConfig_t *config)
{
  if ((index < OFDPA_G8131_PG_MIN_ID) || (index > OFDPA_G8131_PG_MAX_ID))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MLP index out of range. index = %d\r\n",
                       index);
    return(0);
  }
  /* per ITU-T G8131 hold-off time is 0 to 10 seconds expressed in 100ms increments */
  if (config->holdOffTime > 100)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MLP hold-off timer out of range. holdOffTime = %d\r\n",
                       config->holdOffTime);
    return(0);
  }
  /* per ITU-T G8131 wait-to-restore time is 5 to 12 minutes expressed in 1-minute steps */
  if ((config->wtrTime > 12) || (config->wtrTime < 0))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MLP wait-to-restore time out of range. wtrTime = %d\r\n",
                       config->wtrTime);
    return(0);
  }

  return(1);
}

OFDPA_ERROR_t ofdbOamMlpGroupConfig(uint32_t index, ofdpaMlpheadendcfg_t *mlpcfg)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  OFDPA_ERROR_t rc1 = OFDPA_E_NONE;
  ofdbOamMlpGroupTable_node_t *mlpGroupPtr;
  uint32_t lmepId;
  ofdbOamMepTable_node_t *lmepEntryPtr;
  uint32_t workingLivenessPortId = 0;
  uint32_t protectionLivenessPortId = 0;
  ofdbOamMegTable_node_t *megNode;
  uint32_t workingPrimVid = 0;
  uint32_t protectionPrimVid = 0;
  uint32_t workingLmepId = 0;
  uint32_t protectionLmepId = 0;
  ofdbPortInfo_t portInfo;
  OFDB_ENTRY_FLAG_t flags;
  ofdbOamLmepData_t *oamMapData;
  uint32_t ffGroupId = OFDPA_INVALID_GROUP_ID;

  mlpGroupPtr = ofdbOamMLPGroupNodeGet(index);

  if (mlpGroupPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Failed to retrieve MLP group entry. index = %d\r\n",
                       index);
    rc = OFDPA_E_NOT_FOUND;
  }
  else
  {
    if (mlpGroupPtr->config.mlpArchitecture == OFDPA_MLP_ARCH_1_TO_1)
    {
        lmepEntryPtr = ofdbOamMepNodeGet(mlpcfg->lmepid);
        if (lmepEntryPtr != NULL)
        {
           /*保护组创建时更新ROLE及LIVENESSPORT信息   ADD BY WANGJING*/
           lmepEntryPtr->config.mlp.mlpRole = mlpcfg->role;
           lmepEntryPtr->config.mlp.u.livenessLogicalPortId = mlpcfg->livenessport;

           OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "mlpcfg update role = %d, livenessort = %x\r\n",
                               lmepEntryPtr->config.mlp.mlpRole,
                               lmepEntryPtr->config.mlp.u.livenessLogicalPortId);
        }

        /* Added by JiaoYong, 2016/7/9 更新injectOAM里的信息,否则后续保护倒换触发
        的时候会使用默认的livenessLogicalPortId，保护倒换失败*/
        if (ofdbInjectedOamLmepIdMapDataGet(mlpcfg->lmepid, &oamMapData) == OFDPA_E_NONE)
	    {
            oamMapData->livenessLogicalPortId = mlpcfg->livenessport;
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "update InjectedOam livenessLogicalPortId = %x\r\n",
                               oamMapData->livenessLogicalPortId);
        }
          
      /*
       * if there are 2 head end MEP associations with the Protection Group,
       * we can configure it in the APS subsystem
       */
      if (mlpGroupPtr->status.refCount == 2)
      {
        /* retrieve each MLP <> MEP association record */
        lmepId = 0;
        while (ofdbOamMepProtectionNextGet(index, lmepId, &lmepId) == OFDPA_E_NONE)
        {
          lmepEntryPtr = ofdbOamMepNodeGet(lmepId);
          if (lmepEntryPtr == NULL)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Failed to retrieve MEP entry. index = %d, lmepId = %d\r\n",
                               index, lmepId);
            rc = OFDPA_E_NOT_FOUND;
            break;
          }

          megNode = ofdbOamMegNodeGet(lmepEntryPtr->config.megIndex);
          if (megNode == NULL)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Failed to retrieve MEG node. megIndex = %d\r\n",
                               lmepEntryPtr->config.megIndex);
            rc = OFDPA_E_NOT_FOUND;
            break;
          }          

          if (lmepEntryPtr->config.mlp.mlpRole == OFDPA_MLP_ROLE_PRIMARY)
          {
            workingLivenessPortId = lmepEntryPtr->config.mlp.u.livenessLogicalPortId;
            workingPrimVid = ofdbOamBhhVidGet(lmepEntryPtr->config.megIndex, &megNode->config);
            workingLmepId = lmepEntryPtr->lmepId;
          }
          else
          {
            protectionLivenessPortId = lmepEntryPtr->config.mlp.u.livenessLogicalPortId;
            protectionPrimVid = ofdbOamBhhVidGet(lmepEntryPtr->config.megIndex, &megNode->config);
            protectionLmepId = lmepEntryPtr->lmepId;
          }
        }

        if ((workingLivenessPortId == 0) || (protectionLivenessPortId == 0) || (workingPrimVid == 0) || (protectionPrimVid == 0))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                             "Failed to find MEP entry for working and/or protection. "
                             "index = %d, workingLivenessPortId = 0x%x, protectionLivenessPortId = 0x%x, "
                             "workingPrimVid = %d, protectionPrimVid = %d\r\n",
                             index, workingLivenessPortId, protectionLivenessPortId,
                             workingPrimVid, protectionPrimVid);
          rc = OFDPA_E_ERROR;
        }

/* BEGIN: Modified by Hushouqiang, 2016/10/13   问题单号:P10012 */
        /*do not get logical port's info in  the procesure of creating mlp*/
        if (rc == OFDPA_E_NONE)
        {
          /*
           * validation logic assures the workingLivenessPortId and the protectionLivenessPortId
           * contain the same ffGroupId so using either one to retrieve the ffGroupId works
           */
           
          rc1 = ofdbPortGet(workingLivenessPortId, &portInfo, &flags);
          if (rc1 == OFDPA_E_NONE)
          {
            if (flags & OFDB_PORT_DELETED)
            {
              rc1 = OFDPA_E_NOT_FOUND;
            }
          }
          if (rc1 != OFDPA_E_NONE)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Liveness port not found. portId = 0x%x\r\n",
                               workingLivenessPortId);
          }
          else
          {
            ffGroupId = portInfo.ffGroupId;
          }
        }
        
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "ffGroupId = 0x%x\r\n",
                           ffGroupId);
        if (rc == OFDPA_E_NONE)
        {
          /* use default value :OFDPA_INVALID_GROUP_ID */
          /* generate the primVid from the MEG */
          rc = g8131PgCreate(index, ffGroupId,
                             workingLivenessPortId, protectionLivenessPortId,
                             workingPrimVid, protectionPrimVid,
                             workingLmepId, protectionLmepId);

/* END:   Modified by Hushouqiang, 2016/10/13 */
          if (rc == OFDPA_E_NONE)
          {
            do
            {
              /* G.8131 component expects hold off time in milliseconds */
              rc = g8131PgCommonSet(index, G8131_CMN_TYPE_HOLDOFF_VALUE, mlpGroupPtr->config.holdOffTime * 100);
              if (rc != OFDPA_E_NONE)
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                   "Error setting holdoff time. rc = %d, index = %d\r\n",
                                   rc, index);
                break;
              }

              /* G.8131 component expects WTR time in milliseconds - convert minutes to milliseconds */
              rc = g8131PgCommonSet(index, G8131_CMN_TYPE_WTR_VALUE, mlpGroupPtr->config.wtrTime * 60 * 1000);
              if (rc != OFDPA_E_NONE)
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                   "Error setting WTR time. rc = %d, index = %d\r\n",
                                   rc, index);
                break;
              }

              rc = g8131PgCommonSet(index, G8131_CMN_TYPE_REVERT_MODE, (uint32_t)mlpGroupPtr->config.revertiveMode);
              if (rc != OFDPA_E_NONE)
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                   "Error setting revertive mode. rc = %d, index = %d\r\n",
                                   rc, index);
                break;
              }
            } while (0);

            if (rc == OFDPA_E_NONE)
            {
              /* BEGIN: Modified by Hushouqiang, 2016/10/13   问题单号:P10012 */
              /* do nothing */
              #if 0
              /* increment reference count to FF group that caused the working and protection liveness ports to be created */
              if (ofdbGroupReferenceUpdate(portInfo.ffGroupId, 1) != OFDPA_E_NONE)
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                   "Error while incrementing reference count in FF Group associated with Liveness port. workingLivenessPortId = 0x%x ffGroupId = 0x%x\r\n",
                                   workingLivenessPortId, portInfo.ffGroupId);
                rc = OFDPA_E_INTERNAL;
              }
              mlpGroupPtr->status.ffGroupId = portInfo.ffGroupId;
              #endif
              /* END:   Modified by Hushouqiang, 2016/10/13 */
            }
            else
            {
              /* something went wrong setting PG parameters, remove PG from OAM subsystem */
              g8131PgDelete(index);
            }
          } /* END if PG created successfully */
        }
      }
    }
    else
    {
      /* other MLP Architectures not yet implemented */
      rc = OFDPA_E_UNAVAIL;
    }
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamMlpGroupUnconfig(uint32_t index)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMlpGroupTable_node_t *mlpGroupPtr;

  mlpGroupPtr = ofdbOamMLPGroupNodeGet(index);

  if (mlpGroupPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Failed to retrieve MLP group entry. index = %d\r\n",
                       index);
    rc = OFDPA_E_NOT_FOUND;
  }
  else
  {
    /*
     * if there fewer than 2 head end MEP associations with the Protection Group,
     * we should unconfigure it in the APS subsystem
     */
    if (mlpGroupPtr->status.refCount < 2)
    {
/*by hushouqiang , 2016/10/21 start*/      
#if 0
      /* decrement reference count to FF group that caused the working and protection liveness ports to be created */
      if (ofdbGroupReferenceUpdate(mlpGroupPtr->status.ffGroupId, 0) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Error while decrementing reference count in FF Group associated with MLP group. index = %d ffGroupId = 0x%x\r\n",
                           index, mlpGroupPtr->status.ffGroupId);
      }
#endif
/*by hushouqiang , 2016/10/21 end*/      
      rc = g8131PgDelete(index);
    }

    /* dealing with tail end MEPs not yet implemented */
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamMLPGroupAdd(uint32_t index, ofdpaOamMLPGroupConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMlpGroupTable_node_t node, *dataPtr;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(&node, 0, sizeof(node));

  node.index = index;
  memcpy(&node.config, config, sizeof(node.config));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbOamMLPGroupTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMLPGroupDelete(uint32_t index)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMlpGroupTable_node_t *dataPtr;

  dataPtr = ofdbOamMLPGroupNodeGet(index);

  if (dataPtr != 0)
  {
    if (dataPtr->status.refCount == 0)
    {
      avlDeleteEntry(&ofdbAVLTrees.ofdbOamMLPGroupTable_tree, dataPtr);
      rc = OFDPA_E_NONE;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Attempted to delete OAM MLP Group entry with a refCount not 0. index = %d, "
                         "refCount = %d\r\n",
                         index, dataPtr->status.refCount);
      rc = OFDPA_E_FAIL;
    }
  }

  return rc;
}

static void ofdbMLPStatusGet(uint32_t index, ofdpaOamMLPGroupStatus_t *status)
{
  g8131PgCommonGet(index, G8131_CMN_TYPE_LAST_LR, &status->lastLReq);
  g8131PgCommonGet(index, G8131_CMN_TYPE_LAST_RR, &status->lastRReq);
  g8131PgCommonGet(index, G8131_CMN_TYPE_LAST_SR, &status->lastSReq);

  g8131PgCommonGet(index, G8131_CMN_TYPE_LOCAL_SF_W_EXIST, &status->sfwState);
  g8131PgCommonGet(index, G8131_CMN_TYPE_LOCAL_SF_P_EXIST, &status->sfpState);
  g8131PgCommonGet(index, G8131_CMN_TYPE_LOCAL_SD_W_EXIST, &status->sdwState);
  g8131PgCommonGet(index, G8131_CMN_TYPE_LOCAL_SD_P_EXIST, &status->sdpState);

  g8131PgCommonGet(index, G8131_CMN_TYPE_ALARM_CM, &status->dfop_cm);
  g8131PgCommonGet(index, G8131_CMN_TYPE_ALARM_PM, &status->dfop_pm);
  g8131PgCommonGet(index, G8131_CMN_TYPE_ALARM_NR, &status->dfop_nr);
  g8131PgCommonGet(index, G8131_CMN_TYPE_ALARM_TO, &status->dfop_to);

  g8131PgCommonGet(index, G8131_CMN_TYPE_LAST_W_STATUS, &status->workingStatus);
  g8131PgCommonGet(index, G8131_CMN_TYPE_LAST_P_STATUS, &status->standbyStatus);
}

OFDPA_ERROR_t ofdbOamMLPGroupGet(uint32_t index, ofdpaOamMLPGroupConfig_t *config, ofdpaOamMLPGroupStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMlpGroupTable_node_t *dataPtr;

  dataPtr = ofdbOamMLPGroupNodeGet(index);

  if (dataPtr != 0)
  {
    if (config)
    {
      memcpy(config, &dataPtr->config, sizeof(dataPtr->config));
    }
    if (status)
    {
      /* retrieve latest status from the OAM subsystem */
      ofdbMLPStatusGet(index, status);

      status->refCount = dataPtr->status.refCount;
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMLPGroupNextGet(uint32_t index, uint32_t *nextIndex)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMlpGroupTable_node_t *dataPtr;

  if (nextIndex == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMLPGroupTable_tree, &index, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextIndex = dataPtr->index;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMLPGroupReferenceUpdate(uint32_t index, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMlpGroupTable_node_t *dataPtr;

  dataPtr = ofdbOamMLPGroupNodeGet(index);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;

    if (increment != 0)
    {
      dataPtr->status.refCount++;
    }
    else
    {
      if (dataPtr->status.refCount > 0)
      {
        dataPtr->status.refCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMLPGroupRequest(uint32_t index, OFDPA_PROTECTION_SWITCH_REQUEST_TYPE_t request)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMlpGroupTable_node_t *dataPtr;
  uint32_t g8131Request;

  switch (request)
  {
    case OFDPA_PROTECTION_SWITCH_REQUEST_TYPE_MANUAL_LOCK:
      g8131Request = G8131_REQUEST_TYPE_MANUAL_LOCK;
      break;
    case OFDPA_PROTECTION_SWITCH_REQUEST_TYPE_FORCE_SWITCH:
      g8131Request = G8131_REQUEST_TYPE_FORCE_SWITCH;
      break;
    case OFDPA_PROTECTION_SWITCH_REQUEST_TYPE_MANUAL_SWITCH_P:
      g8131Request = G8131_REQUEST_TYPE_MANUAL_SWITCH_P;
      break;
    case OFDPA_PROTECTION_SWITCH_REQUEST_TYPE_MANUAL_SWITCH:
      g8131Request = G8131_REQUEST_TYPE_MANUAL_SWITCH;
      break;
    case OFDPA_PROTECTION_SWITCH_REQUEST_TYPE_CLEAR:
      g8131Request = G8131_REQUEST_TYPE_CLEAR;
      break;
    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Invalid request type value. request = %d\r\n",
                         request);
      /* BEGIN: Modified by Hushouqiang, 2016/10/16   问题单号:P10012 */
      /* unknown request, Just return ok*/
      return OFDPA_E_NONE;
      /* END:   Modified by Hushouqiang, 2016/10/16 */
      break;
  }

  dataPtr = ofdbOamMLPGroupNodeGet(index);

  if (dataPtr != NULL)
  {
    rc = g8131PgRequest(index, g8131Request);
  }

  return rc;
}

static ofdbOamMLPGroupMepList_node_t *ofdbOamMepProtectionNodeGet(uint32_t mlpGroupIndex, uint32_t lmepId)
{
  ofdbOamMLPGroupMepList_node_t node;

  memset(&node, 0, sizeof(node));
  node.mlpGroupIndex = mlpGroupIndex;
  node.lmepId = lmepId;

  return(avlSearch(&ofdbAVLTrees.ofdbOamMLPGroupMepTable_tree, &node, AVL_EXACT));
}

static ofdbOamMLPGroupMepList_node_t *ofdbOamMepProtectionNodeNextGet(uint32_t mlpGroupIndex, uint32_t lmepId)
{
  ofdbOamMLPGroupMepList_node_t node;

  memset(&node, 0, sizeof(node));
  node.mlpGroupIndex = mlpGroupIndex;
  node.lmepId = lmepId;

  return(avlSearch(&ofdbAVLTrees.ofdbOamMLPGroupMepTable_tree, &node, AVL_NEXT));
}

int ofdbOamMepProtectionValidate(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg)
{
  ofdbOamMepTable_node_t *mepNode, *existingMepNode;
  ofdbOamMlpGroupTable_node_t *mlpGroupNode;
  ofdbOamMLPGroupMepList_node_t *mlpGroupMepListNode;
  uint32_t searchLmepId;
  /*ofdbPortInfo_t newMepLivenessPortInfo, existingMepLivenessPortInfo;
  OFDB_ENTRY_FLAG_t flags;*/


  /* make sure table entries for MEP and MLP Group exist */
  mepNode = ofdbOamMepNodeGet(lmepId);
  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Failed to retrieve MEP entry. lmepId = %d\r\n",
                       lmepId);
    return(0);
  }

  /* check that MEP has MLP role set */
  if ((mepNode->config.mlp.mlpRole != OFDPA_MLP_ROLE_PRIMARY) && (mepNode->config.mlp.mlpRole != OFDPA_MLP_ROLE_BACKUP))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry not configured with valid MLP role. lmepId = %d, mlpRole = %d\r\n",
                       lmepId, mepNode->config.mlp.mlpRole);
    return(0);
  }

  mlpGroupNode = ofdbOamMLPGroupNodeGet(mlpGroupIndex);
  if (mlpGroupNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Failed to retrieve MLP Group entry. mlpGroupIndex = %d\r\n",
                       mlpGroupIndex);
    return(0);
  }

  if (mlpGroupNode->status.refCount >= 2)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Protection group already has two paths. mlpGroupIndex = %d\r\n",
                       mlpGroupIndex);
    return(0);
  }

  if (mlpGroupNode->config.mlpArchitecture == OFDPA_MLP_ARCH_1_TO_1)
  {
    /* Modified by JiaoYong, 2016/7/9 创建mep时下发的role和livenessLogicalPortId
    都是默认值，而不是控制器下发的值，因此不能做校验，后续会在ofdbOamMlpGroupConfig()
    里更新这两个值*/    
    #if 0
    if ((ofdbPortGet(mepNode->config.mlp.u.livenessLogicalPortId, &newMepLivenessPortInfo, &flags) != OFDPA_E_NONE) || (flags & OFDB_PORT_DELETED))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Liveness port used by LMEP not created. lmepId = %d, livenessLogicalPortId = 0x%x\r\n",
                         lmepId, mepNode->config.mlp.u.livenessLogicalPortId);
      return 0;
    }
    #endif
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Only protection groups using 1:1 protection supported.\r\n", 0);
    return 0;
  }

  /* find other lmep using this protection group, if any */
  if (mlpGroupNode->status.refCount == 1)
  {
    searchLmepId = 0;
    while (((mlpGroupMepListNode = ofdbOamMepProtectionNodeNextGet(mlpGroupIndex, searchLmepId)) != NULL) &&
           (mlpGroupMepListNode->mlpGroupIndex == mlpGroupIndex))
    {
      if (mlpGroupMepListNode->lmepId != lmepId)
      {
        existingMepNode = ofdbOamMepNodeGet(mlpGroupMepListNode->lmepId);
        if (existingMepNode != NULL)
        {
          /* check if existing lmep is serving the same MLP role as new lmep */
          if (mlpcfg->role == existingMepNode->config.mlp.mlpRole)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Existing protection group member has same role as new member.\r\n", 0);
            return(0);
          }

          /* Modified by JiaoYong, 2016/7/9 创建mep时下发的role和livenessLogicalPortId
         都是默认值，而不是控制器下发的值，因此不能做校验，后续会在ofdbOamMlpGroupConfig()
         里更新这两个值*/ 
          #if 0
          /* check that all lmeps in protection group refer to liveness ports containing the same ffGroupId value */
          if ((ofdbPortGet(existingMepNode->config.mlp.u.livenessLogicalPortId, &existingMepLivenessPortInfo, &flags) == OFDPA_E_NONE) &&
              !(flags & OFDB_PORT_DELETED))
          {
            if (newMepLivenessPortInfo.ffGroupId != existingMepLivenessPortInfo.ffGroupId)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "New and existing member MEPs refer to liveness ports from different Fast Failover Groups.\r\n"
                                 "\tnew:      lmepId = %d, livenessPort = 0x%x, ffGroupId = 0x%x\r\n",
                                 "\texisting: lmepId = %d, livenessPort = 0x%x, ffGroupId = 0x%x\r\n",
                                 lmepId, mepNode->config.mlp.u.livenessLogicalPortId, newMepLivenessPortInfo.ffGroupId,
                                 mlpGroupMepListNode->lmepId, existingMepNode->config.mlp.u.livenessLogicalPortId, existingMepLivenessPortInfo.ffGroupId);
              return(0);
            }
          }
          else
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Liveness port used by existing LMEP not created. lmepId = %d, livenessLogicalPortId = 0x%x\r\n",
                               mlpGroupMepListNode->lmepId, existingMepNode->config.mlp.u.livenessLogicalPortId);
            return 0;
          }
          #endif
        }
        else
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                             "Failed to retrieve data for existing member MEP. lmepId = %d\r\n",
                             mlpGroupMepListNode->lmepId);
          return(0);
        }
      }
      searchLmepId = mlpGroupMepListNode->lmepId;
    }
  }

  return(1);
}

OFDPA_ERROR_t ofdbOamMepProtectionAdd(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMLPGroupMepList_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.mlpGroupIndex = mlpGroupIndex;
  node.lmepId = lmepId;

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbOamMLPGroupMepTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    ofdbOamMepReferenceUpdate(lmepId, 1);
    ofdbOamMLPGroupReferenceUpdate(mlpGroupIndex, 1);

    rc = ofdbOamMlpGroupConfig(mlpGroupIndex, mlpcfg);

    if (rc != OFDPA_E_NONE)
    {
      /* undo table changes */
      avlDeleteEntry(&ofdbAVLTrees.ofdbOamMLPGroupMepTable_tree, &node);
      ofdbOamMepReferenceUpdate(lmepId, 0);
      ofdbOamMLPGroupReferenceUpdate(mlpGroupIndex, 0);

      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error from ofdbOamMlpGroupConfig(). mlpGroupIndex = %d, rc = %d\r\n",
                         mlpGroupIndex, rc);
      rc = OFDPA_E_FAIL;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMepProtectionDelete(uint32_t mlpGroupIndex, uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMLPGroupMepList_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));

  node.mlpGroupIndex = mlpGroupIndex;
  node.lmepId = lmepId;

  dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbOamMLPGroupMepTable_tree, &node);

  if (dataPtr != 0)
  {
    ofdbOamMepReferenceUpdate(lmepId, 0);
    ofdbOamMLPGroupReferenceUpdate(mlpGroupIndex, 0);

    rc = ofdbOamMlpGroupUnconfig(mlpGroupIndex);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error from ofdbOamMlpGroupUnconfig(). mlpGroupIndex = %d, rc = %d\r\n",
                         mlpGroupIndex, rc);
      rc = OFDPA_E_FAIL;
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMepProtectionGet(uint32_t mlpGroupIndex, uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMLPGroupMepList_node_t *dataPtr;

  dataPtr = ofdbOamMepProtectionNodeGet(mlpGroupIndex, lmepId);

  if (dataPtr != 0)
  {
    rc = OFDPA_E_NONE;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamMepProtectionNextGet(uint32_t mlpGroupIndex, uint32_t lmepId, uint32_t *nextLmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamMLPGroupMepList_node_t *dataPtr;

  dataPtr = ofdbOamMepProtectionNodeNextGet(mlpGroupIndex, lmepId);

  if (dataPtr != 0)
  {
    *nextLmepId = dataPtr->lmepId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}

uint32_t ofdbOamDataCounterTableEntryCountGet(void)
{
  return ofdbAVLTrees.ofdbOamDataCounterTable_tree.count;
}

uint32_t ofdbOamDataCounterTableMaxCountGet(void)
{
  return dpaDataPlaneCounterTableEntryCountGet();
}

OFDPA_ERROR_t ofdbOamDataCounterAdd(uint32_t lmepId, uint8_t trafficClass)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamDataCounterTable_node_t node;
  ofdbOamDataCounterTable_node_t *dataPtr;

  memset(&node, 0, sizeof(node));
  node.lmepId = lmepId;
  node.trafficClass = trafficClass;

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbOamDataCounterTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Data Plane Counter Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate Entry is found.
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Data Plane Counter Table entry already exists!\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamDataCounterDelete(uint32_t lmepId, uint8_t trafficClass)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamDataCounterTable_node_t node;
  ofdbOamDataCounterTable_node_t *dataPtr;

  memset(&node, 0, sizeof(node));
  node.lmepId = lmepId;
  node.trafficClass = trafficClass;

  dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbOamDataCounterTable_tree, &node);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Data Plane Counter Table entry not found!\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamDataCounterStatusGet(uint32_t lmepId, uint8_t trafficClass, ofdpaOamDataCounterStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamDataCounterTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.lmepId = lmepId;
  node.trafficClass = trafficClass;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamDataCounterTable_tree, &node, AVL_EXACT);
  if (dataPtr != NULL)
  {
    if (status != NULL)
    {
      rc = OFDPA_E_NONE;
      memcpy(status, &dataPtr->status, sizeof(ofdpaOamDataCounterStatus_t));
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamDataCounterGet(ofdpaOamDataCounterIndex_t index, ofdpaOamDataCounterStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbOamDataCounterTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.lmepId = index.lmepId;
  node.trafficClass = index.trafficClass;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamDataCounterTable_tree, &node, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy(status, &dataPtr->status, sizeof(ofdpaOamDataCounterStatus_t));
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamDataCounterNextGet(ofdpaOamDataCounterIndex_t index, ofdpaOamDataCounterIndex_t *nextIndex,
                                        ofdpaOamDataCounterStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbOamDataCounterTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.lmepId = index.lmepId;
  node.trafficClass = index.trafficClass;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamDataCounterTable_tree, &node, AVL_NEXT);
  if (dataPtr != NULL)
  {
    if (nextIndex != NULL)
    {
      nextIndex->lmepId = dataPtr->lmepId;
      nextIndex->trafficClass = dataPtr->trafficClass;
      if (status != NULL)
      {
        rc = OFDPA_E_NONE;
        memcpy(status, &dataPtr->status, sizeof(ofdpaOamDataCounterStatus_t));
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamDataCounterReferenceUpdate(uint32_t lmepId, uint8_t trafficClass, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbOamDataCounterTable_node_t node, *dataPtr;

  memset(&node, 0, sizeof(node));
  node.lmepId = lmepId;
  node.trafficClass = trafficClass;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamDataCounterTable_tree, &node, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->status.refCount++;
    }
    else
    {
      if (dataPtr->status.refCount > 0)
      {
        dataPtr->status.refCount--;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbOamDataCounterEntryValidate(uint32_t lmepId, uint8_t trafficClass)
{
  ofdpaOamDataCounterIndex_t index;
  ofdpaOamDataCounterIndex_t nextIndex;
  ofdpaOamDataCounterStatus_t status;
  int count = 0;


  index.lmepId = lmepId;
  index.trafficClass = trafficClass;

  if (OFDPA_E_NONE == ofdbOamDataCounterGet(index, &status))
  {
    return OFDPA_E_EXISTS;
  }

  /*  There are only 8 counters available for 15 traffic classes.
      So allow upto 8 traffic classes per LMEP. */

  index.trafficClass = 0;
  if (OFDPA_E_NONE == ofdbOamDataCounterGet(index, &status))
  {
    count++;
  }

  while (OFDPA_E_NONE == ofdbOamDataCounterNextGet(index, &nextIndex, &status))
  {
    if (nextIndex.lmepId != index.lmepId)
    {
      break;
    }
    count++;
    memcpy(&nextIndex, &index, sizeof(nextIndex));
  }

  if (count >= 8)
  {
    return OFDPA_E_FULL;
  }
  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbOamLbAdd(uint32_t lmepId, ofdpaOamLbConfig_t *lbConfig)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  uint32_t intIfNum;

  if (lbConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  memcpy(&mepDataPtr->lbConfig, lbConfig, sizeof(mepDataPtr->lbConfig));
  mepConfig = &mepDataPtr->config;

  
  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  if (dot1agMepParamGet(megNode->megInfo.dot1agMdIndex,
                        megNode->megInfo.dot1agMaIndex,
                        mepConfig->mepId, 0,
                        MEP_PARAM_dot1agCfmMepIfIndex, &intIfNum) != OFDPA_E_NONE)
  {
    return OFDPA_E_ERROR;
  }

  rc = dot1agCfmMepTransmitLbmVlanPrioSet(megNode->megInfo.dot1agMdIndex,
                                          megNode->megInfo.dot1agMaIndex,
                                          mepConfig->mepId, intIfNum,
                                          lbConfig->lbPriority);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmLbmVlanPrioSet(). rc = %d\r\n",
                       rc);
    return (rc);
  }

  rc = dot1agCfmMepTransmitLbmVlanDropEnableSet(megNode->megInfo.dot1agMdIndex,
                                                megNode->megInfo.dot1agMaIndex,
                                                mepConfig->mepId, intIfNum,
                                                lbConfig->lbDropEligibility);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmLbmVlanDropEnableSet(). rc = %d\r\n",
                       rc);
    return (rc);
  }

  if (lbConfig->lbTestPatternTlvPresent == OFDPA_TRUE)
  {
    rc = dot1agCfmMepTransmitLbmTestTlvSet(megNode->megInfo.dot1agMdIndex,
                                           megNode->megInfo.dot1agMaIndex,
                                           mepConfig->mepId, intIfNum,
                                           lbConfig->lbTestPatternTlvType);

    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error from dot1agCfmMepTransmitLbmTestTlvSet(). rc = %d\r\n",
                         rc);
      return (rc);
    }
  }

  if (lbConfig->lbDiscovery == OFDPA_OAM_LB_DISCOVERY_NONE)
  {
    rc = dot1agCfmMepTransmitLbmPacketLengthSet(megNode->megInfo.dot1agMdIndex,
                                                megNode->megInfo.dot1agMaIndex,
                                                mepConfig->mepId, intIfNum,
                                                lbConfig->lbPacketLength);

    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error from dot1agCfmMepTransmitLbmPacketLengthSet(). rc = %d\r\n",
                         rc);
      return (rc);
    }
  }

  if (megNode->config.megType == OFDPA_OAM_MEG_TYPE_G8113_1)
  {
    rc = dot1agCfmMepTransmitLbmTtlSet(megNode->megInfo.dot1agMdIndex,
                                       megNode->megInfo.dot1agMaIndex,
                                       mepConfig->mepId, intIfNum,
                                       lbConfig->lbTtl);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error from dot1agCfmLbmTtlSet(). rc = %d\r\n",
                         rc);
      return (rc);
    }
  }

  rc = dot1agCfmMepTransmitLbmIntervalSet(megNode->megInfo.dot1agMdIndex,
                                          megNode->megInfo.dot1agMaIndex,
                                          mepConfig->mepId, intIfNum,
                                          lbConfig->lbPeriod);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmLbmIntervalSet(). rc = %d\r\n",
                       rc);
    return (rc);
  }

  rc = dot1agCfmMepTransmitLbmDiscoverySet(megNode->megInfo.dot1agMdIndex,
                                           megNode->megInfo.dot1agMaIndex,
                                           mepConfig->mepId, intIfNum,
                                           lbConfig->lbDiscovery);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmMepTransmitLbmDiscoverySet(). rc = %d\r\n",
                       rc);
    return (rc);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamLbStart(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdbOamRemoteMpTable_node_t *rmpNode = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);
  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }
  
  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  if ((rmpNode = ofdbOamRemoteMpNodeGet(mepDataPtr->lbConfig.lbDestMpId)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Remote MP node not found. lmepId = %d, lbDestMpId = %d\r\n",
                       mepDataPtr->lmepId, mepDataPtr->lbConfig.lbDestMpId);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agCfmLbmSend(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                        mepConfig->mepId, 0,
                        rmpNode->config.mepId,
                        mepDataPtr->lbConfig.lbNbrPacketsToSend);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmLbmSend(). rc = %d\r\n",
                       rc);
  }

  /* Added by JiaoYong, 2017/1/6 */
  mepDataPtr->lbConfig.lbState = OFDPA_LB_STATE_STARTED;
  
  return (rc);
}

OFDPA_ERROR_t ofdbOamLbStop(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);
  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }
  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agCfmLbmStop(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                        mepConfig->mepId);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmLbmSend(). rc = %d\r\n",
                       rc);
  }

  /* Added by JiaoYong, 2017/1/6 */
  mepDataPtr->lbConfig.lbState = OFDPA_LB_STATE_STOPPED;

  return (rc);
}

OFDPA_ERROR_t ofdbOamLbGet(uint32_t lmepId, ofdpaOamLbConfig_t *lbConfig, ofdpaOamLbStatus_t *lbStatus)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t  *mepDataPtr = NULL;
  ofdpaOamMepConfig_t     *mepConfig = NULL;
  ofdbOamMegTable_node_t  *megNode = NULL;
  uint32_t                intIfNum, lbrIn, lbrInOutOfOrder, lbrBadMsdu;
  OFDPA_BOOL              resultOk = OFDPA_FALSE;

  if (lbConfig == NULL || lbStatus == NULL) {
    return OFDPA_E_PARAM;
  }

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);
  if (mepDataPtr == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Modified by JiaoYong, 2017/1/6 
    如果已经删除或者未创建，直接返回OK
  */  
  if((OFDPA_LB_STATE_NULL == mepDataPtr->lbConfig.lbState)
    || (OFDPA_LB_STATE_CREATED == mepDataPtr->lbConfig.lbState))
  {    
    
	memcpy(lbConfig, &mepDataPtr->lbConfig, sizeof(mepDataPtr->lbConfig));
    return OFDPA_E_NONE;
  }
  
  mepConfig = &mepDataPtr->config;
  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }
  if (dot1agMepParamGet(megNode->megInfo.dot1agMdIndex,
                        megNode->megInfo.dot1agMaIndex,
                        mepConfig->mepId, 0,
                        MEP_PARAM_dot1agCfmMepIfIndex, &intIfNum) != OFDPA_E_NONE)
  {
    return OFDPA_E_ERROR;
  }

  if (dot1agMepParamGet(megNode->megInfo.dot1agMdIndex,
                        megNode->megInfo.dot1agMaIndex,
                        mepConfig->mepId, 0,
                        MEP_PARAM_dot1agCfmMepLbrIn, &lbrIn) != OFDPA_E_NONE)
  {
    return OFDPA_E_ERROR;
  }

  if (lbrIn > 0)
  {
    resultOk = OFDPA_TRUE;
  }

  if (dot1agMepParamGet(megNode->megInfo.dot1agMdIndex,
                        megNode->megInfo.dot1agMaIndex,
                        mepConfig->mepId, 0,
                        MEP_PARAM_dot1agCfmMepLbrInOutOfOrder, &lbrInOutOfOrder) != OFDPA_E_NONE)
  {
    return OFDPA_E_ERROR;
  }

  if (dot1agMepParamGet(megNode->megInfo.dot1agMdIndex,
                        megNode->megInfo.dot1agMaIndex,
                        mepConfig->mepId, 0,
                        MEP_PARAM_dot1agCfmMepLbrBadMsdu, &lbrBadMsdu) != OFDPA_E_NONE)
  {
    return OFDPA_E_ERROR;
  }

  lbStatus->lbResultOk = resultOk;
  lbStatus->lbrIn = lbrIn;
  lbStatus->lbrInOutOfOrder = lbrInOutOfOrder;
  lbStatus->lbrBadMsdu = lbrBadMsdu;
  memcpy(lbConfig, &mepDataPtr->lbConfig, sizeof(mepDataPtr->lbConfig));

  return (rc);
}
/*****************************************************************************
 函 数 名  : ofdbOamLbDelete
 功能描述  : Delete LB
 输入参数  : uint32_t lmepId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年1月6日
    作    者   : JiaoYong
    修改内容   : 新生成函数

*****************************************************************************/
OFDPA_ERROR_t ofdbOamLbDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);
  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Added by JiaoYong, 2017/1/6 */
  if(OFDPA_LB_STATE_STARTED == mepDataPtr->lbConfig.lbState)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "LB can't be deleted before stopped for lmepId = %d\r\n",
                       lmepId);
    return OFDPA_E_REQUEST_DENIED;
  }
  
  memset(&mepDataPtr->lbConfig, 0, sizeof(mepDataPtr->lbConfig));
  memset(&mepDataPtr->lbStatus, 0, sizeof(mepDataPtr->lbStatus));
  mepDataPtr->lbConfig.lbState = OFDPA_LB_STATE_NULL;
  
  return (rc);
}

OFDPA_ERROR_t ofdbOamLbConfigValidate(uint32_t lmepId, ofdpaOamLbConfig_t *lbConfig)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;

  if (lbConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Modified by JiaoYong, 2017/1/6 
    如果已经创建，直接返回错误
  */
  if(mepDataPtr->lbConfig.lbState)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "LB already exist for lmepId = %d, lbState = %d\r\n",
                       lmepId, mepDataPtr->lbConfig.lbState);
    return OFDPA_E_EXISTS;
  }
  
  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  if (lbConfig->lbTestPatternTlvPresent == OFDPA_FALSE)
  {
    /* Number of loopback messages */
    if ((lbConfig->lbNbrPacketsToSend < OFDPA_DOT1AG_NUM_LBM_MIN) ||
        (lbConfig->lbNbrPacketsToSend > OFDPA_DOT1AG_NUM_LBM_MAX))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Invalid number of messages %d\r\n", lbConfig->lbNbrPacketsToSend);
      return OFDPA_E_PARAM;
    }
  }
  else
  {
    if ((lbConfig->lbTestPatternTlvType < OFDPA_OAM_LB_TEST_TLV_NULL) ||
        (lbConfig->lbTestPatternTlvType > OFDPA_OAM_LB_TEST_TLV_PRBS_CRC32))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Invalid test tlv type for LBM message", 0);
      return OFDPA_E_PARAM;
    }
  }

  if ((lbConfig->lbDiscovery < OFDPA_OAM_LB_DISCOVERY_NONE) ||
      (lbConfig->lbDiscovery > OFDPA_OAM_LB_DISCOVERY_ICC_MIPID))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Invalid lbm discovery type \r\n", 0);
    return OFDPA_E_PARAM;
  }

  if ((lbConfig->lbPeriod < 0) ||
      (lbConfig->lbPeriod > 600000) ||
      ((lbConfig->lbPeriod % 100) != 0))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "valid lbm period range is 0 to 600 seconds with a granularity of 100ms\r\n", 0);
    return OFDPA_E_PARAM;
  }

  /* Validate vlan priority or  mpls tc */
  if (lbConfig->lbPriority < 0 || lbConfig->lbPriority > 7)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Invalid vlan priority or mpls tc for mepId %d\r\n", lmepId);
    return OFDPA_E_PARAM;
  }

  if (megNode->config.megType == OFDPA_OAM_MEG_TYPE_G8113_1)
  {
    if (lbConfig->lbDiscovery == OFDPA_OAM_LB_DISCOVERY_NONE)
    {
      if ((lbConfig->lbDropEligibility < 0) || (lbConfig->lbDropEligibility > 1))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Drop elegibility must be in range of 0 - 1\r\n", 0);
        return OFDPA_E_PARAM;
      }
    }
    else
    {
      if (lbConfig->lbDropEligibility != 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Drop elegibility must be 0 for loopback discovery\r\n", 0);
        return OFDPA_E_PARAM;
      }
    }
  }
  else
  {
    if ((lbConfig->lbTtl < 0) || (lbConfig->lbTtl > 255))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Ttl must be in range of 1 - 255 for BHH MP \r\n", 0);
      return OFDPA_E_PARAM;
    }
  }

  if (lbConfig->lbDiscovery == OFDPA_OAM_LB_DISCOVERY_NONE)
  {
    /* Modified by JiaoYong, 2016/6/30 
     pMEPObj->mepData->dot1agCfmMepTransmitLbmPacketLength - (data - dot1agPktFromCpu.payLoad) - 4
     代码里要求BHH的必须大于70
    */    
    if ((lbConfig->lbPacketLength > OFDPA_DOT1AG_MAX_ETHERNET_FRAME_SIZE)
        || (lbConfig->lbPacketLength < 70))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "packet length should be less than 1500 and more than 70\r\n", 0);
      return OFDPA_E_PARAM;
    }
  }

  /* Modified by JiaoYong, 2017/5/24 
    BUGID=SDN0002066, save after all pass
  */  
  memcpy(&mepDataPtr->lbConfig, lbConfig, sizeof(mepDataPtr->lbConfig));

  return (rc);
}

OFDPA_ERROR_t ofdbOamProLmConfig(ofdbOamMepTable_node_t *node)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  OFDPA_CCM_INTERVAL_t    period;

  mepConfig = &node->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       node->lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_CCM_INTERVAL_Invalid == node->pmProLmConfig.period)
  {
    period = OFDPA_CCM_INTERVAL_100ms; /* Default period is 100ms */
  }
  else
  {
    period = node->pmProLmConfig.period;
  }

  rc = dot1agMepProlmCreate(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                            mepConfig->mepId, 0, node->pmProLmConfig.proLmDestMpId, node->pmProLmConfig.trafficClass,
                            node->pmProLmConfig.toolType, node->pmProLmConfig.testId, period);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agMepProlmCreate(). rc = %d\r\n",
                       rc);
    rc = OFDPA_E_FAIL;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProLmUnconfig(ofdbOamMepTable_node_t *node)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;

  mepConfig = &node->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agMepProlmDelete(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, mepConfig->mepId);

  return(rc);
}

OFDPA_ERROR_t ofdbOamProLmEnabledSet(uint32_t lmepId, OFDPA_BOOL mode)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    if (mode == OFDPA_TRUE && mepDataPtr->pmProLmStatus.proLmState == OFDPA_OAM_PM_STATE_RUNNING)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Proactive LM session is already enabled for lmepId = %d, \r\n", lmepId);
      return OFDPA_E_FAIL;
    }
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agMepProlmEnabledSet(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                mepConfig->mepId, mode);

  if (rc == OFDPA_E_NONE)
  {
    if (mode == OFDPA_TRUE)
    {
      mepDataPtr->pmProLmStatus.proLmState = OFDPA_OAM_PM_STATE_RUNNING;
    }
    else
    {
      mepDataPtr->pmProLmStatus.proLmState = OFDPA_OAM_PM_STATE_STOPPED;
    }
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "Failed enabling Pro LM for lmepId = %d rc = %d\r\n",
                       lmepId, rc);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProLmAdd(uint32_t lmepId, ofdpaOamProLmConfig_t *proLmConfig)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  if (proLmConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    if (mepDataPtr->pmProLmStatus.proLmState == OFDPA_OAM_PM_STATE_RUNNING)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Proactive LM session is already enabled for lmepId = %d, \r\n", lmepId);
      return OFDPA_E_FAIL;
    }

    memcpy(&mepDataPtr->pmProLmConfig, proLmConfig, sizeof(mepDataPtr->pmProLmConfig));
    rc = ofdbOamProLmConfig(mepDataPtr);
    if (rc != OFDPA_E_NONE)
    {
      memset(&mepDataPtr->pmProLmConfig, 0x0, sizeof(mepDataPtr->pmProLmConfig));
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProLmDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Disabled PM Proactive LM for lmepId = %d\r\n",
                       lmepId);

    rc = ofdbOamProLmUnconfig(mepDataPtr);

    if (rc == OFDPA_E_NONE)
    {
      memset(&mepDataPtr->pmProLmConfig, 0x0, sizeof(mepDataPtr->pmProLmConfig));
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProLmGet(uint32_t lmepId, ofdpaOamProLmConfig_t *proLmConfig, ofdpaOamProLmStatus_t *proLmStatus)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    if (proLmConfig)
    {
      memcpy(proLmConfig, &mepDataPtr->pmProLmConfig, sizeof(mepDataPtr->pmProLmConfig));
    }
    if (proLmStatus)
    {
      memcpy(proLmStatus, &mepDataPtr->pmProLmStatus, sizeof(mepDataPtr->pmProLmStatus));
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProLmCountersGet(uint32_t lmepId, ofdpaOamProLmCounters_t *proLmCounters)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  y1731CfmMepProlmTable_t prolmEntry;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agMepY1731ProLmCountersGet(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                      mepConfig->mepId, mepDataPtr->pmProLmConfig.trafficClass, &prolmEntry);

  if (rc == OFDPA_E_NONE)
  {
    proLmCounters->bin_15min.mN_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_15M].mN_FLR;
    proLmCounters->bin_15min.aN_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_15M].aN_FLR;
    proLmCounters->bin_15min.xN_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_15M].xN_FLR;
    proLmCounters->bin_15min.mF_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_15M].mF_FLR;
    proLmCounters->bin_15min.aF_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_15M].aF_FLR;
    proLmCounters->bin_15min.xF_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_15M].xF_FLR;
    proLmCounters->bin_24hr.mN_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_24H].mN_FLR;
    proLmCounters->bin_24hr.aN_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_24H].aN_FLR;
    proLmCounters->bin_24hr.xN_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_24H].xN_FLR;
    proLmCounters->bin_24hr.mF_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_24H].mF_FLR;
    proLmCounters->bin_24hr.aF_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_24H].aF_FLR;
    proLmCounters->bin_24hr.xF_FLR = prolmEntry.prolmCurBin[OFDPA_OAM_BIN_MODE_24H].xF_FLR;
  }
  return(rc);
}

OFDPA_ERROR_t ofdbOamProDmStart(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdbOamRemoteMpTable_node_t *rmpNode = NULL;
  OFDPA_CCM_INTERVAL_t    period;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP node not found. lmepId = %d\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if (mepDataPtr->pmProDmStatus.proDmState == OFDPA_OAM_PM_STATE_RUNNING)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Proactive DM already running for lmepId = %d\r\n", lmepId);
    return OFDPA_E_FAIL;
  }

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  if ((rmpNode = ofdbOamRemoteMpNodeGet(mepDataPtr->pmProDmConfig.proDmDestMpId)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Remote MP node not found. lmepId = %d, proDmDestMpId = %d\r\n",
                       mepDataPtr->lmepId, mepDataPtr->pmProDmConfig.proDmDestMpId);
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_CCM_INTERVAL_Invalid == mepDataPtr->pmProDmConfig.period)
  {
    period = OFDPA_CCM_INTERVAL_100ms; /* Default period is 100ms */
  }
  else
  {
    period = mepDataPtr->pmProDmConfig.period;
  }

  rc = dot1agCfmDmmStart(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                         mepConfig->mepId, rmpNode->config.macAddress.addr, rmpNode->config.mepId,
                         mepDataPtr->pmProDmConfig.trafficClass, period,
                         mepDataPtr->pmProDmConfig.frameLen, OFDPA_OAM_DM_TYPE_PROACTIVE, 0);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmDmmStart(). rc = %d\r\n",
                       rc);
  }
  else
  {
    mepDataPtr->pmProDmStatus.proDmState = OFDPA_OAM_PM_STATE_RUNNING;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProDmStop(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP node not found. lmepId = %d\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agCfmDmmStop(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                        mepConfig->mepId, mepDataPtr->pmProDmConfig.trafficClass);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmOndemLmmStop(). rc = %d\r\n",
                       rc);
  }
  else
  {
    mepDataPtr->pmProDmStatus.proDmState = OFDPA_OAM_PM_STATE_STOPPED;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProDmAdd(uint32_t lmepId, ofdpaOamProDmConfig_t *proDmConfig)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  if (proDmConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    memcpy(&mepDataPtr->pmProDmConfig, proDmConfig, sizeof(mepDataPtr->pmProDmConfig));
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Enabled PM Proactive DM for lmepId = %d\r\n",
                       lmepId);
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProDmDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    memset(&mepDataPtr->pmProDmConfig, 0x0, sizeof(mepDataPtr->pmProDmConfig));
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Disabled PM Proactive DM for lmepId = %d\r\n",
                       lmepId);
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProDmGet(uint32_t lmepId, ofdpaOamProDmConfig_t *proDmConfig, ofdpaOamProDmStatus_t *proDmStatus)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    if (proDmConfig)
    {
      memcpy(proDmConfig, &mepDataPtr->pmProDmConfig, sizeof(mepDataPtr->pmProDmConfig));
    }
    if (proDmStatus)
    {
      memcpy(proDmStatus, &mepDataPtr->pmProDmStatus, sizeof(mepDataPtr->pmProDmStatus));
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProDmCountersGet(uint32_t lmepId, ofdpaOamProDmCounters_t *proDmCounters)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  y1731CfmMepDmTable_t    dmEntry;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agMepY1731ProDmCountersGet(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                      mepConfig->mepId, mepDataPtr->pmProDmConfig.trafficClass, &dmEntry);

  if (rc == OFDPA_E_NONE)
  {
    proDmCounters->bin_15min.mB_FD  = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_15M].mB_FD;
    proDmCounters->bin_15min.aB_FD  = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_15M].aB_FD;
    proDmCounters->bin_15min.xB_FD  = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_15M].xB_FD;
    proDmCounters->bin_15min.mN_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_15M].mN_FDV;
    proDmCounters->bin_15min.aN_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_15M].aN_FDV;
    proDmCounters->bin_15min.xN_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_15M].xN_FDV;
    proDmCounters->bin_15min.mF_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_15M].mF_FDV;
    proDmCounters->bin_15min.aF_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_15M].aF_FDV;
    proDmCounters->bin_15min.xF_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_15M].xF_FDV;
    proDmCounters->bin_24hr.mB_FD  = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_24H].mB_FD;
    proDmCounters->bin_24hr.aB_FD  = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_24H].aB_FD;
    proDmCounters->bin_24hr.xB_FD  = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_24H].xB_FD;
    proDmCounters->bin_24hr.mN_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_24H].mN_FDV;
    proDmCounters->bin_24hr.aN_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_24H].aN_FDV;
    proDmCounters->bin_24hr.xN_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_24H].xN_FDV;
    proDmCounters->bin_24hr.mF_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_24H].mF_FDV;
    proDmCounters->bin_24hr.aF_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_24H].aF_FDV;
    proDmCounters->bin_24hr.xF_FDV = dmEntry.dmCurBin[OFDPA_OAM_BIN_MODE_24H].xF_FDV;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdLmStart(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdbOamRemoteMpTable_node_t *rmpNode = NULL;
  OFDPA_CCM_INTERVAL_t    period;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP node not found. lmepId = %d\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if (mepDataPtr->pmOdLmStatus.odLmState == OFDPA_OAM_PM_STATE_RUNNING)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "OnDemand LM already running for lmepId = %d\r\n", lmepId);
    return OFDPA_E_FAIL;
  }

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  if ((rmpNode = ofdbOamRemoteMpNodeGet(mepDataPtr->pmOdLmConfig.odLmDestMpId)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Remote MP node not found. lmepId = %d, odLmDestMpId = %d\r\n",
                       mepDataPtr->lmepId, mepDataPtr->pmOdLmConfig.odLmDestMpId);
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_CCM_INTERVAL_Invalid == mepDataPtr->pmOdLmConfig.period)
  {
    period = OFDPA_CCM_INTERVAL_100ms; /* Default period is 100ms */
  }
  else
  {
    period = mepDataPtr->pmOdLmConfig.period;
  }

  rc = dot1agCfmOndemLmmStart(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                              mepConfig->mepId, rmpNode->config.macAddress.addr, rmpNode->config.mepId,
                              mepDataPtr->pmOdLmConfig.trafficClass, mepDataPtr->pmOdLmConfig.trafficClass,
                              period, 0, mepDataPtr->pmOdLmConfig.toolType, mepDataPtr->pmOdLmConfig.testId);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmOndemLmmStart(). rc = %d\r\n",
                       rc);
  }
  else
  {
    mepDataPtr->pmOdLmStatus.odLmState = OFDPA_OAM_PM_STATE_RUNNING;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdLmStop(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP node not found. lmepId = %d\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agCfmOndemLmmStop(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                             mepConfig->mepId, mepDataPtr->pmOdLmConfig.trafficClass);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmOndemLmmStop(). rc = %d\r\n",
                       rc);
  }
  else
  {
    mepDataPtr->pmOdLmStatus.odLmState = OFDPA_OAM_PM_STATE_STOPPED;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdLmAdd(uint32_t lmepId, ofdpaOamOdLmConfig_t *odLmConfig)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  if (odLmConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    memcpy(&mepDataPtr->pmOdLmConfig, odLmConfig, sizeof(mepDataPtr->pmOdLmConfig));
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdLmDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    memset(&mepDataPtr->pmOdLmConfig, 0x0, sizeof(mepDataPtr->pmOdLmConfig));
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdLmGet(uint32_t lmepId, ofdpaOamOdLmConfig_t *odLmConfig, ofdpaOamOdLmStatus_t *odLmStatus)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    if (odLmConfig)
    {
      memcpy(odLmConfig, &mepDataPtr->pmOdLmConfig, sizeof(mepDataPtr->pmOdLmConfig));
    }
    if (odLmStatus)
    {
      memcpy(odLmStatus, &mepDataPtr->pmOdLmStatus, sizeof(mepDataPtr->pmOdLmStatus));
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdLmCountersGet(uint32_t lmepId, ofdpaOamOdLmSnapshot_t *odLmCounters)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  y1731CfmMepOndemlmTable_t odlmEntry;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agMepY1731OdLmCountersGet(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                     mepConfig->mepId, mepDataPtr->pmOdLmConfig.trafficClass, &odlmEntry);

  if (rc == OFDPA_E_NONE)
  {
    odLmCounters->tNTFCnt = odlmEntry.lmPrim.nNTF;
    odLmCounters->tNLFCnt = odlmEntry.lmPrim.nNLF;
    odLmCounters->tNFLR   = odlmEntry.ondemlmBin.tNFLRCnt;
    odLmCounters->tFTFCnt = odlmEntry.lmPrim.nFTF;
    odLmCounters->tFLFCnt = odlmEntry.lmPrim.nFLF;
    odLmCounters->tFFLR   = odlmEntry.ondemlmBin.tFFLRCnt;
    odLmCounters->sLMCnt  = odlmEntry.ondemlmBin.sLMCnt;
    odLmCounters->uLMCnt  = odlmEntry.ondemlmBin.uLMCnt;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdDmStart(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdbOamRemoteMpTable_node_t *rmpNode = NULL;
  OFDPA_CCM_INTERVAL_t    period;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP node not found. lmepId = %d\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if (mepDataPtr->pmOdDmStatus.odDmState == OFDPA_OAM_PM_STATE_RUNNING)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "OnDemand DM already running for lmepId = %d\r\n", lmepId);
    return OFDPA_E_FAIL;
  }

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  if ((rmpNode = ofdbOamRemoteMpNodeGet(mepDataPtr->pmOdDmConfig.odDmDestMpId)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Remote MP node not found. lmepId = %d, odDmDestMpId = %d\r\n",
                       mepDataPtr->lmepId, mepDataPtr->pmOdDmConfig.odDmDestMpId);
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_CCM_INTERVAL_Invalid == mepDataPtr->pmOdDmConfig.period)
  {
    period = OFDPA_CCM_INTERVAL_100ms; /* Default period is 100ms */
  }
  else
  {
    period = mepDataPtr->pmOdDmConfig.period;
  }

  rc = dot1agCfmDmmStart(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                         mepConfig->mepId, rmpNode->config.macAddress.addr, rmpNode->config.mepId,
                         mepDataPtr->pmOdDmConfig.trafficClass, period,
                         mepDataPtr->pmOdDmConfig.frameLen, OFDPA_OAM_DM_TYPE_ONDEMAND, 0);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmDmmStart(). rc = %d\r\n",
                       rc);
  }
  else
  {
    mepDataPtr->pmOdDmStatus.odDmState = OFDPA_OAM_PM_STATE_RUNNING;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdDmStop(uint32_t lmepId)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP node not found. lmepId = %d\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agCfmDmmStop(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                        mepConfig->mepId, mepDataPtr->pmOdDmConfig.trafficClass);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmDmmStop(). rc = %d\r\n",
                       rc);
  }
  else
  {
    mepDataPtr->pmOdDmStatus.odDmState = OFDPA_OAM_PM_STATE_STOPPED;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdDmAdd(uint32_t lmepId, ofdpaOamOdDmConfig_t *odDmConfig)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  if (odDmConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    memcpy(&mepDataPtr->pmOdDmConfig, odDmConfig, sizeof(mepDataPtr->pmOdDmConfig));
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdDmDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    memset(&mepDataPtr->pmOdDmConfig, 0x0, sizeof(mepDataPtr->pmOdDmConfig));
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdDmGet(uint32_t lmepId, ofdpaOamOdDmConfig_t *odDmConfig, ofdpaOamOdDmStatus_t *odDmStatus)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    if (odDmConfig)
    {
      memcpy(odDmConfig, &mepDataPtr->pmOdDmConfig, sizeof(mepDataPtr->pmOdDmConfig));
    }
    if (odDmStatus)
    {
      memcpy(odDmStatus, &mepDataPtr->pmOdDmStatus, sizeof(mepDataPtr->pmOdDmStatus));
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamOdDmCountersGet(uint32_t lmepId, ofdpaOamOdDmSnapshot_t *odDmCounters)
{
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  y1731CfmMepDmTable_t   dmEntry;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agMepY1731OdDmCountersGet(mepConfig->ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                     mepConfig->mepId, mepDataPtr->pmOdDmConfig.trafficClass, &dmEntry);

  if (rc == OFDPA_E_NONE)
  {
    odDmCounters->tN_FD = dmEntry.dmPrim.tNFD.second + (dmEntry.dmPrim.tNFD.ns/1000000000);
    odDmCounters->tF_FD = dmEntry.dmPrim.tFFD.second + (dmEntry.dmPrim.tFFD.ns/1000000000);
    odDmCounters->tB_FD = dmEntry.dmPrim.tBFD.second + (dmEntry.dmPrim.tBFD.ns/1000000000);
    odDmCounters->elapsedTime = (dmEntry.dmEndTime - dmEntry.dmStartTime)/10;
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamProLmConfigValidate(uint32_t lmepId, ofdpaOamProLmConfig_t *config)
{
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Verify if MEP with lmepId exists or not*/
  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Local MEP with Id %d not found.\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Validate tool type */
  if (config->toolType > OFDPA_OAM_PM_TOOL_TYPE_SLM)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "tool type is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  megNode = ofdbOamMegNodeGet(mepDataPtr->config.megIndex);

  if (megNode != NULL)
  {
    if ((megNode->config.megType == OFDPA_OAM_MEG_TYPE_ETHERNET) &&
        (config->toolType == OFDPA_OAM_PM_TOOL_TYPE_CCM))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "CCM based LM is not supported on Ethernet MEG. lmepId = %d, megIndex = %d\r\n",
                         mepDataPtr->lmepId, mepDataPtr->config.megIndex);
      return OFDPA_E_PARAM;
    }
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       mepDataPtr->lmepId, mepDataPtr->config.megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  /* Validate period */
  if (config->period > OFDPA_CCM_INTERVAL_10min)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "period is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbOamProDmConfigValidate(uint32_t lmepId, ofdpaOamProDmConfig_t *config)
{
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Verify if MEP with lmepId exists or not*/
  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Local MEP with Id %d not found.\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Validate period */
  if (config->period > OFDPA_CCM_INTERVAL_10min)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "period is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  /* Validate frameLen */
  if ((config->frameLen < OFDPA_OAM_PM_FRAME_LEN_MIN) || (config->frameLen > OFDPA_OAM_PM_FRAME_LEN_MAX))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "frame length is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbOamOdLmConfigValidate(uint32_t lmepId, ofdpaOamOdLmConfig_t *config)
{
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Verify if MEP with lmepId exists or not*/
  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Local MEP with Id %d not found.\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Validate tool type */
  if ((config->toolType == OFDPA_OAM_PM_TOOL_TYPE_CCM) || (config->toolType > OFDPA_OAM_PM_TOOL_TYPE_SLM))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "tool type is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  /* Validate period */
  if (config->period > OFDPA_CCM_INTERVAL_10min)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "period is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  /* Validate role */
  if (config->role > OFDPA_OAM_PM_ROLE_BOTH)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "role is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbOamOdDmConfigValidate(uint32_t lmepId, ofdpaOamOdDmConfig_t *config)
{
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Verify if MEP with lmepId exists or not*/
  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Local MEP with Id %d not found.\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Validate period */
  if (config->period > OFDPA_CCM_INTERVAL_10min)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "period is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  /* Validate frameLen */
  if ((config->frameLen < OFDPA_OAM_PM_FRAME_LEN_MIN) || (config->frameLen > OFDPA_OAM_PM_FRAME_LEN_MAX))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "frame length is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  /* Validate role */
  if (config->role > OFDPA_OAM_PM_ROLE_BOTH)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "role is invalid.\r\n", 0);
    return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbOamLtConfigValidate(uint32_t lmepId, ofdpaOamLtConfig_t *config)
{
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Verify if MEP with lmepId exists or not*/
  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Local MEP with Id %d not found.\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       mepDataPtr->lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }
  else
  {
    if (megNode->config.megType != OFDPA_OAM_MEG_TYPE_ETHERNET)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Link trace session initiation failed due to incorrect MEG type for lmepId = %d \r\n",
                         lmepId);
      return OFDPA_E_FAIL;
    }
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbOamLtmSend(ofdbOamMepTable_node_t *mepDataPtr)
{
  OFDPA_ERROR_t            rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t      *mepConfig = NULL;
  ofdbOamRemoteMpTable_node_t *rmpNode = NULL;
  ofdbOamMegTable_node_t   *megNode = NULL;

  mepConfig = &mepDataPtr->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       mepDataPtr->lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  if ((rmpNode = ofdbOamRemoteMpNodeGet(mepDataPtr->ltConfig.ltDestMpId)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Remote MP node not found. lmepId = %d, ltDestMpId = %d\r\n",
                       mepDataPtr->lmepId, mepDataPtr->ltConfig.ltDestMpId);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agCfmLtmSend(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, mepDataPtr->config.mepId,
                        rmpNode->config.macAddress.addr, 0, mepDataPtr->ltConfig.ltTtl);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from dot1agCfmLtmSend(). rc = %d\r\n",
                       rc);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamLtStart(uint32_t lmepId, ofdpaOamLtConfig_t *ltConfig)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  if (ltConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* perform validity checks */
  rc = ofdbOamLtConfigValidate(lmepId, ltConfig);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE, "OAM Link Trace configuration did not pass validation.\r\n", 0);
    return rc;
  }

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    memcpy(&mepDataPtr->ltConfig, ltConfig, sizeof(mepDataPtr->ltConfig));

    rc = ofdbOamLtmSend(mepDataPtr);
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamLtGet(uint32_t lmepId, ofdpaOamLtConfig_t *ltConfig, ofdpaOamLtStatus_t *ltStatus)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  mepDataPtr = avlSearch(&ofdbAVLTrees.ofdbOamMepTable_tree, &lmepId, AVL_EXACT);

  if (mepDataPtr != 0)
  {
    if (ltConfig)
    {
      memcpy(ltConfig, &mepDataPtr->ltConfig, sizeof(mepDataPtr->ltConfig));
    }
    if (ltStatus)
    {
      memcpy(ltStatus, &mepDataPtr->ltStatus, sizeof(mepDataPtr->ltStatus));
    }
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamLtResultNextGet(uint32_t lmepId, uint32_t sequenceNum, uint32_t receiveOrder, ofdpaOamLtrEntry_t *ltReply)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepNode = NULL;
  ofdpaOamMepConfig_t    *mepConfig = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  dot1agCfmLtrObj_t linkTraceReply;

  mepNode = ofdbOamMepNodeGet(lmepId);

  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  mepConfig = &mepNode->config;

  if ((megNode = ofdbOamMegNodeGet(mepConfig->megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepConfig->megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agMepLtResultNextGet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, lmepId, sequenceNum, receiveOrder, &linkTraceReply);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error from ofdbOamLtResultNextGet(). rc = %d\r\n",
                       rc);
  }
  else
  {
    ltReply->ttl                     = linkTraceReply.dot1agCfmLtrTtl;
    ltReply->forwarded               = linkTraceReply.dot1agCfmLtrForwarded;
    ltReply->terminalMep             = linkTraceReply.dot1agCfmLtrTerminalMep;
    ltReply->relayAction             = linkTraceReply.dot1agCfmLtrRelay;
    ltReply->ingressAction           = linkTraceReply.dot1agCfmLtrIngress;
    ltReply->ingressPortIdSubtype    = linkTraceReply.dot1agCfmLtrIngressPortIdSubtype;
    ltReply->egressAction            = linkTraceReply.dot1agCfmLtrEgress;
    ltReply->egressPortIdSubtype     = linkTraceReply.dot1agCfmLtrEgressPortIdSubtype;

    memcpy(&ltReply->lastEgressId, &(linkTraceReply.dot1agCfmLtrLastEgressIdentifier), OFDPA_LTR_EGRESS_ID_SIZE);
    memcpy(ltReply->nextEgressId, &(linkTraceReply.dot1agCfmLtrNextEgressIdentifier), OFDPA_LTR_EGRESS_ID_SIZE);
    memcpy(&ltReply->ingressMac, &(linkTraceReply.dot1agCfmLtrIngressMac), OFDPA_MAC_ADDR_LEN);
    memcpy(&ltReply->ingressPortId, &(linkTraceReply.dot1agCfmLtrIngressPortId), sizeof(uint32_t));
    memcpy(&ltReply->egressMac, &(linkTraceReply.dot1agCfmLtrEgressMac), OFDPA_MAC_ADDR_LEN);
    memcpy(&ltReply->egressPortId, &(linkTraceReply.dot1agCfmLtrEgressPortId), sizeof(uint32_t));
    if ((NULL != ltReply->organizationSpecificTlv.pstart) && (ltReply->organizationSpecificTlv.size <= OFDPA_ORG_SPECIFIC_TLV_SIZE))
    {
      memcpy(ltReply->organizationSpecificTlv.pstart, &(linkTraceReply.dot1agCfmLtrOrganizationSpecificTlv), OFDPA_ORG_SPECIFIC_TLV_SIZE);
    }
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamCsfEnable(uint32_t lmepId, ofdpaOamCsfConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepNode = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  y1731CfmPeriod_t y1731CsfPeriod;

  /* translate interval/period parameter */
  switch (config->csfPeriod)
  {
    case OFDPA_CCM_INTERVAL_1s:
      y1731CsfPeriod = Y1731_CFM_PERIOD_1PPS;
      break;
    case OFDPA_CCM_INTERVAL_1min:
      y1731CsfPeriod = Y1731_CFM_PERIOD_1PPM;
      break;
    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Unsupported csfPeriod specified. lmepId = %d, csfPeriod = %d\r\n",
                         lmepId, config->csfPeriod);
      return OFDPA_E_PARAM;
      break;
  }

  mepNode = ofdbOamMepNodeGet(lmepId);

  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  if ((megNode = ofdbOamMegNodeGet(mepNode->config.megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepNode->config.megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  /* apply configuration to OAM component */
  do
  {
    rc = dot1agCfmMepAisLckCsfClientLevelSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                             mepNode->config.mepId, mepNode->config.ifIndex,
                                             Y1731_DEFECT_FUNCTION_CSF, megNode->config.level);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error setting client level. rc = %d, lmepId = %d\r\n",
                         rc, lmepId);
      break;
    }
    rc = dot1agCfmMepAisLckCsfPrioritySet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                          mepNode->config.mepId, mepNode->config.ifIndex,
                                          Y1731_DEFECT_FUNCTION_CSF, config->csfPriority);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error setting priority. rc = %d, lmepId = %d\r\n",
                         rc, lmepId);
      break;
    }
    rc = dot1agCfmMepAisLckCsfIntervalSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                          mepNode->config.mepId, mepNode->config.ifIndex,
                                          Y1731_DEFECT_FUNCTION_CSF, y1731CsfPeriod);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error setting interval. rc = %d, lmepId = %d\r\n",
                         rc, lmepId);
      break;
    }
    rc = dot1agCfmMepAisLckCsfModeSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                          mepNode->config.mepId, mepNode->config.ifIndex,
                                          Y1731_DEFECT_FUNCTION_CSF, OFDPA_TRUE);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error setting mode. rc = %d, lmepId = %d\r\n",
                         rc, lmepId);
      break;  /* in case code is inserted after this clause */
    }
  } while (0);

  if (rc == OFDPA_E_NONE)
  {
    /* store configuration in mepNode */
    memcpy(&mepNode->csfConfig, config, sizeof(mepNode->csfConfig));
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamCsfDisable(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepNode = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;

  mepNode = ofdbOamMepNodeGet(lmepId);

  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  if ((megNode = ofdbOamMegNodeGet(mepNode->config.megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepNode->config.megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  /* apply configuration to OAM component */
  rc = dot1agCfmMepAisLckCsfModeSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                    mepNode->config.mepId, mepNode->config.ifIndex,
                                    Y1731_DEFECT_FUNCTION_CSF, OFDPA_FALSE);

  /* clear configuration in mepNode */
  memset(&mepNode->csfConfig, 0, sizeof(mepNode->csfConfig));

  return(rc);
}

static OFDPA_ERROR_t ofdbOamAisConfigValidate(uint32_t lmepId, ofdpaOamAisConfig_t *aisConfig)
{
  ofdbOamMepTable_node_t *mepDataPtr = NULL;
  ofdbOamMegTable_node_t *megDataPtr = NULL;
  uint32_t megId;

  if (aisConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Verify if MEP with lmepId exists or not */
  mepDataPtr = ofdbOamMepNodeGet(lmepId);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Local MEP with Id %d not found.\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Validate client meg level */
  if (aisConfig->aisClientMegLevel > OFDPA_MDL_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "client meg level is invalid (%d)\r\n", aisConfig->aisClientMegLevel);
    return OFDPA_E_PARAM;
  }

  /* Validate period */
  if (aisConfig->aisPeriod != OFDPA_CCM_INTERVAL_1s && aisConfig->aisPeriod != OFDPA_CCM_INTERVAL_1min)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "period is invalid (%d)\r\n", aisConfig->aisPeriod);
    return OFDPA_E_PARAM;
  }

  megId = mepDataPtr->config.megIndex;
  /* Verify if MEG with megId exists or not */
  megDataPtr = ofdbOamMegNodeGet(megId);
  if (NULL == megDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "MEG with Id %d not found.\r\n", megId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Validate priority. Used for PCP in Ethernet OAM and TC value in MPLS OAM */
  if (megDataPtr->config.megType == OFDPA_OAM_MEG_TYPE_ETHERNET &&
      aisConfig->aisPriority > OFDPA_VLAN_PCP_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Priority is invalid (%d)\r\n", aisConfig->aisPriority);
    return OFDPA_E_PARAM;
  }
  else if (megDataPtr->config.megType == OFDPA_OAM_MEG_TYPE_G8113_1 &&
      aisConfig->aisPriority > OFDPA_TRAFFIC_CLASS_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Traffic Class is invalid (%d)\r\n", aisConfig->aisPriority);
    return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbOamAisEnable(uint32_t lmepId, ofdpaOamAisConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepNode = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  y1731CfmPeriod_t y1731AisPeriod;

  /* validate AIS config */
  rc = ofdbOamAisConfigValidate(lmepId, config);
  if (OFDPA_E_NONE != rc)
  {
    return rc;
  }

  /* translate interval/period parameter */
  y1731AisPeriod = Y1731_AIS_PERIOD_FIELD_PARSE(config->aisPeriod);

  mepNode = ofdbOamMepNodeGet(lmepId);
  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  if ((megNode = ofdbOamMegNodeGet(mepNode->config.megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepNode->config.megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  /* apply configuration to OAM component */
  do
  {
    rc = dot1agCfmMepAisLckCsfClientLevelSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                             mepNode->config.mepId, mepNode->config.ifIndex,
                                             Y1731_DEFECT_FUNCTION_AIS, config->aisClientMegLevel);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error setting client level. rc = %d, lmepId = %d\r\n",
                         rc, lmepId);
      break;
    }
    rc = dot1agCfmMepAisLckCsfPrioritySet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                          mepNode->config.mepId, mepNode->config.ifIndex,
                                          Y1731_DEFECT_FUNCTION_AIS, config->aisPriority);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error setting priority. rc = %d, lmepId = %d\r\n",
                         rc, lmepId);
      break;
    }
    rc = dot1agCfmMepAisLckCsfIntervalSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                          mepNode->config.mepId, mepNode->config.ifIndex,
                                          Y1731_DEFECT_FUNCTION_AIS, y1731AisPeriod);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error setting interval. rc = %d, lmepId = %d\r\n",
                         rc, lmepId);
      break;
    }
    rc = dot1agCfmMepAisLckCsfModeSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                      mepNode->config.mepId, mepNode->config.ifIndex,
                                      Y1731_DEFECT_FUNCTION_AIS, OFDPA_TRUE);
    if (rc != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Error setting mode. rc = %d, lmepId = %d\r\n",
                         rc, lmepId);
      break;  /* in case code is inserted after this clause */
    }
  } while (0);

  if (rc == OFDPA_E_NONE)
  {
    /* store configuration in mepNode */
    memcpy(&mepNode->aisConfig, config, sizeof(mepNode->aisConfig));
  }

  return(rc);
}

OFDPA_ERROR_t ofdbOamAisDisable(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepNode = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;

  mepNode = ofdbOamMepNodeGet(lmepId);

  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  if ((megNode = ofdbOamMegNodeGet(mepNode->config.megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepNode->config.megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  /* apply configuration to OAM component */
  rc = dot1agCfmMepAisLckCsfModeSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                    mepNode->config.mepId, mepNode->config.ifIndex,
                                    Y1731_DEFECT_FUNCTION_AIS, OFDPA_FALSE);

  /* clear configuration in mepNode */
  memset(&mepNode->aisConfig, 0, sizeof(mepNode->aisConfig));

  return(rc);
}

OFDPA_ERROR_t ofdbOamAisConfigGet(uint32_t lmepId, ofdpaOamAisConfig_t *config)
{
  ofdbOamMepTable_node_t *mepNode = NULL;

  mepNode = ofdbOamMepNodeGet(lmepId);
  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  if (mepNode->aisConfig.aisPeriod == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "AIS config is disabled for current MEP entry (lmepId = %d)\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* get configuration from mepNode */
  memcpy(config, &mepNode->aisConfig, sizeof(mepNode->aisConfig));

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbOamLckConfigValidate(uint32_t lmepId, ofdpaOamLckConfig_t *lckConfig)
{
  ofdbOamMepTable_node_t *mepDataPtr = NULL;

  if (lckConfig == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Verify if MEP with lmepId exists or not */
  mepDataPtr = ofdbOamMepNodeGet(lmepId);

  if (NULL == mepDataPtr)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Local MEP with Id %d not found.\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* Validate client meg level */
  if (lckConfig->lckClientMegLevel > OFDPA_MDL_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "client meg level is invalid (%d)\r\n", lckConfig->lckClientMegLevel);
    return OFDPA_E_PARAM;
  }

  /* Validate period */
  if (lckConfig->lckPeriod != OFDPA_CCM_INTERVAL_1s && lckConfig->lckPeriod != OFDPA_CCM_INTERVAL_1min)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "period is invalid (%d)\r\n", lckConfig->lckPeriod);
    return OFDPA_E_PARAM;
  }

  /* Validate trafficClass */
  if (lckConfig->lckTrafficClass > OFDPA_VLAN_PCP_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Traffic Class is invalid (%d)\r\n", lckConfig->lckTrafficClass);
    return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbOamLckEnable(uint32_t lmepId, ofdpaOamLckConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepNode = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;
  y1731CfmPeriod_t y1731LckPeriod;

  /* validate LCK config */
  rc = ofdbOamLckConfigValidate(lmepId, config);
  if (OFDPA_E_NONE != rc)
  {
    return rc;
  }

  /* translate interval/period parameter */
  y1731LckPeriod = Y1731_LCK_PERIOD_FIELD_PARSE(config->lckPeriod);

  mepNode = ofdbOamMepNodeGet(lmepId);
  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  if ((megNode = ofdbOamMegNodeGet(mepNode->config.megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepNode->config.megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  /* apply LCK configuration to OAM component */
  rc = dot1agCfmMepAisLckCsfClientLevelSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                           mepNode->config.mepId, mepNode->config.ifIndex,
                                           Y1731_DEFECT_FUNCTION_LCK, config->lckClientMegLevel);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error setting client level. rc = %d, lmepId = %d\r\n",
                       rc, lmepId);
    return rc;
  }
  rc = dot1agCfmMepAisLckCsfPrioritySet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                        mepNode->config.mepId, mepNode->config.ifIndex,
                                        Y1731_DEFECT_FUNCTION_LCK, config->lckTrafficClass);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error setting priority. rc = %d, lmepId = %d\r\n",
                       rc, lmepId);
    return rc;
  }
  rc = dot1agCfmMepAisLckCsfIntervalSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                        mepNode->config.mepId, mepNode->config.ifIndex,
                                        Y1731_DEFECT_FUNCTION_LCK, y1731LckPeriod);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error setting interval. rc = %d, lmepId = %d\r\n",
                       rc, lmepId);
    return rc;
  }
  rc = dot1agCfmMepAisLckCsfModeSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                    mepNode->config.mepId, mepNode->config.ifIndex,
                                    Y1731_DEFECT_FUNCTION_LCK, OFDPA_TRUE);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Error setting mode. rc = %d, lmepId = %d\r\n",
                       rc, lmepId);
    return rc;
  }

  /* store configuration in mepNode */
  memcpy(&mepNode->lckConfig, config, sizeof(mepNode->lckConfig));

  return(rc);
}

OFDPA_ERROR_t ofdbOamLckDisable(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbOamMepTable_node_t *mepNode = NULL;
  ofdbOamMegTable_node_t *megNode = NULL;

  mepNode = ofdbOamMepNodeGet(lmepId);

  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  if ((megNode = ofdbOamMegNodeGet(mepNode->config.megIndex)) == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEG node not found. lmepId = %d, megIndex = %d\r\n",
                       lmepId, mepNode->config.megIndex);
    return OFDPA_E_NOT_FOUND;
  }

  /* apply configuration to OAM component */
  rc = dot1agCfmMepAisLckCsfModeSet(megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex,
                                    mepNode->config.mepId, mepNode->config.ifIndex,
                                    Y1731_DEFECT_FUNCTION_LCK, OFDPA_FALSE);

  /* clear configuration in mepNode */
  memset(&mepNode->lckConfig, 0, sizeof(mepNode->lckConfig));

  return(rc);
}

OFDPA_ERROR_t ofdbOamLckConfigGet(uint32_t lmepId, ofdpaOamLckConfig_t *config)
{
  ofdbOamMepTable_node_t *mepNode = NULL;

  mepNode = ofdbOamMepNodeGet(lmepId);
  if (mepNode == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "MEP entry for lmepId = %d not found\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  if (mepNode->lckConfig.lckPeriod == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Lock Signal config is disabled for current MEP entry (lmepId = %d)\r\n",
                       lmepId);
    return OFDPA_E_NOT_FOUND;
  }

  /* get lock signal configuration from mepNode */
  memcpy(config, &mepNode->lckConfig, sizeof(mepNode->lckConfig));

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdbInjectedOamRefGroupIdGet(uint32_t lmepId, uint32_t *groupId)
{
  ofdbInjectedOamFlowTable_node_t flow_node;
  ofdbInjectedOamFlowTable_node_t *injectedOamDataPtr = NULL;

  memset(&flow_node, 0, sizeof(flow_node));
  flow_node.key.lmepId = lmepId;
  injectedOamDataPtr = avlSearch(&ofdbAVLTrees.ofdbInjectedOamFlowTable_tree, &flow_node.key, AVL_EXACT);

  if (injectedOamDataPtr != NULL)
  {
    *groupId = injectedOamDataPtr->injectedOamFlowEntry.groupId;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERY_VERBOSE,
                      "fail to find injected OAM flow for lmepId %d!\r\n", lmepId);
    return OFDPA_E_NOT_FOUND;
  }
  return OFDPA_E_NONE;
}

uint32_t ofdbInjectedOamCompareRefGroupId(uint32_t serverMpId, uint32_t clientMpId)
{
  OFDPA_ERROR_t rc1 = OFDPA_E_NONE, rc2 = OFDPA_E_NONE;
  uint32_t serverRefGroupId = 0, clientRefGroupId = 0;

  rc1 = ofdbInjectedOamRefGroupIdGet(serverMpId, &serverRefGroupId);

  if (rc1 == OFDPA_E_NONE)
  {
    rc2 = ofdbInjectedOamRefGroupIdGet(clientMpId, &clientRefGroupId);

    if (rc2 == OFDPA_E_NONE)
    {
      if (serverRefGroupId == clientRefGroupId)
      {
        return 1;
      }
    }
  }

  return 0;
}


/* BEGIN: Modified by hushouqiang, 2017/10/23 bug: SDN00002496*/
#ifdef LEGACY_SWITCH_SUPPORT
typedef struct ofdbOamMegEventEntry_s
{
  uint32_t         megId;               /* key */
  uint32_t         endpoint;
  uint32_t         cur_event;
  void            *next;                /* required by AVL */
} ofdbOamMegEventEntry_t;

static avlTree_t ofdpOamMegEventAvlTree = {
  .semId = NULL,
};

OFDPA_BOOL ofdbOamEventIsNew(
                                        uint32_t flags,
                                        uint32_t event_type,
                                        uint32_t group,
                                        uint32_t endpoint
                                      )
{
  OFDPA_ERROR_t rc;
  OFDPA_BOOL result = OFDPA_TRUE;
  ofdbOamMegEventEntry_t recordEntry;
  ofdbOamMegEventEntry_t *pRecordEntry = NULL;

  if(ofdpOamMegEventAvlTree.semId == NULL)
  {
    /* assumes the max meg count is 64 */
    rc = avlAllocAndCreateAvlTree(&ofdpOamMegEventAvlTree, 64, sizeof(ofdbOamMegEventEntry_t), 0x10,
                                  avlCompareULong32, 0);
    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS, "avlAllocAndCreateAvlTree for ofdpOamMegEventAvlTree failed, rc = %d.\r\n", rc);
      return OFDPA_TRUE;
    }
  }

  pRecordEntry = avlSearch(&ofdpOamMegEventAvlTree, &group, AVL_EXACT);


  if (NULL != pRecordEntry)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_TOO_VERBOSE,
      "old event type = %d, endpoint = %d, group id = %d \r\n", 
      pRecordEntry->cur_event, pRecordEntry->endpoint,pRecordEntry->megId);

    
    if((pRecordEntry->endpoint == endpoint )
      && (pRecordEntry->cur_event == event_type))
    {
      result =  OFDPA_FALSE;
      goto exit;
    }

    /* if the previous event is cc timeout, then do not send unknown mepid event.
     bcmOAMEventBHHCCMTimeout = 20,bcmOAMEventBHHCCMUnknownMepId = 32,*/
    if((pRecordEntry->cur_event == 20)
      && (event_type == 32))
    {
      result =  OFDPA_FALSE;
      goto exit;
    }


    /* update record */
    pRecordEntry->endpoint = endpoint; 
    pRecordEntry->cur_event = event_type;

  }
  else
  {
    recordEntry.megId = group;
    recordEntry.endpoint = endpoint;
    recordEntry.cur_event = event_type;
    pRecordEntry = avlInsertEntry(&ofdpOamMegEventAvlTree, &recordEntry);
    if (NULL != pRecordEntry)
    {
      if (pRecordEntry == &recordEntry)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS, "Failed to insert AVL entry.\r\n", 0);
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS, "Duplicate entry.\r\n", 0);
      }
    }
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_TOO_VERBOSE, " Insert entry ,new event type = %d, group id = %d.\r\n", event_type,group);
    return result;
  }
exit:  
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_TOO_VERBOSE,
    "new event type = %d, endpoint = %d, group id = %d, result = %d.\r\n", 
    pRecordEntry->cur_event,pRecordEntry->endpoint,pRecordEntry->megId,result);
  
  return result;
}


#endif

OFDPA_ERROR_t ofdbOamEventNotify(
                                        uint32_t flags,
                                        uint32_t event_type,
                                        uint32_t group,
                                        uint32_t endpoint
                                      )
{
  dpaEventMsg_t   eventMsg = {.msgType = DPA_EVENT_OAM_STATUS_MSG};

  
  if(event_type < OFDB_OAM_EVENT_MAX_TYPE)
  {
    eventMsg.msgSubType = event_type;/*ofdbOamEventMapping[event_type] ;*/

    eventMsg.cookie[0] = group;
    eventMsg.cookie[1] = endpoint;
    /*printf("event:%d --- %x \r\n",eventMsg.msgSubType,eventMsg.cookie[0]);*/

#ifdef LEGACY_SWITCH_SUPPORT
    if(ofdbOamEventIsNew( flags,event_type,group,endpoint) == OFDPA_FALSE)
    {
      return OFDPA_E_NONE;
    }
#endif
    
    /* Notify client. */
    datapathEventNotificationSend(&eventMsg);
  }

  return 0;
}

/* END:   Modified by hushouqiang, 2017/10/23 */


#if 1

int ofdbBfdConfigValidate(uint32_t lmepId, ofdpaBfdConfig_t *config)
{
  if (0 == lmepId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "lmepId cannot be zero.\r\n", 0);
    return(0);
  }

  /* create Logical Liveness Port entry if specified */
  if (config->mlp.u.livenessLogicalPortId != 0)
  {
    if (OFDB_PORT_TYPE(config->mlp.u.livenessLogicalPortId) != OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Port identifier is wrong type for livenessLogicalPortId = 0x%x. (lmepId = %d)\r\n",
                         config->mlp.u.livenessLogicalPortId, lmepId);
      return(0);
    }
  }
  return 1;
}



OFDPA_ERROR_t ofdbBfdAdd(uint32_t lmepId, ofdpaBfdConfig_t *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbBfdTable_node_t node, *dataPtr;

  if (config == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* Check if localMpId already used by a MIP and mep
   MEP and Mip check bfd*/
  if (OFDPA_E_NONE == ofdbOamMipGet(lmepId, NULL, NULL))
  {
    return OFDPA_E_EXISTS;
  }

  if (OFDPA_E_NONE == ofdbOamMepGet(lmepId, NULL, NULL))
  {
    return OFDPA_E_EXISTS;
  }

  memset(&node, 0, sizeof(node));

  node.lmepId = lmepId;
  memcpy(&node.config, config, sizeof(node.config));

  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbBfdTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbBfdTable_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. lmepId = %d\r\n",
                         lmepId);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /*nothing to do*/
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbBfdDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbBfdTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbBfdTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (dataPtr->status.refCount != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Attempted to delete OAM MEP entry with refCount not 0. "
                         "lmepId = %d, refCount = %d\r\n",
                         lmepId, dataPtr->status.refCount);
      rc = OFDPA_E_FAIL;
    }
    else
    {

      dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbBfdTable_tree, dataPtr);
      if (dataPtr == 0)
      {
        /* this should never happen; if it does, something is wrong with the AVL library */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete AVL tree entry just found. lmepId = %d\r\n",
                           lmepId);
        rc = OFDPA_E_INTERNAL;
      }
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbBfdGet(uint32_t lmepId, ofdpaBfdConfig_t *config, ofdpaBfdStatus_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbBfdTable_node_t *dataPtr;
  uint32_t endpoint;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbBfdTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (config)
    {
      memcpy(config, &dataPtr->config, sizeof(dataPtr->config));
    }
    if (status)
    {

#ifdef INCLUDE_BFD    
		if(driverBfdEpIdMapGet(lmepId, &endpoint) == OFDPA_E_NONE)
		{
			driverBfdStatUpdate(endpoint, &dataPtr->status);
		}    
#endif

      memcpy(status, &dataPtr->status, sizeof(dataPtr->status));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}


OFDPA_ERROR_t ofdbBfdNextGet(uint32_t lmepId, uint32_t *nextLmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbBfdTable_node_t *dataPtr;

  if (nextLmepId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbBfdTable_tree, &lmepId, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextLmepId = dataPtr->lmepId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}


OFDPA_ERROR_t ofdbBfdStateGet(uint32_t lmepId, uint32_t *bfdState)
{
	OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
	ofdbBfdTable_node_t *dataPtr;
	uint32_t endpoint;
	
#ifdef INCLUDE_BFD    
	if(driverBfdEpIdMapGet(lmepId, &endpoint) == OFDPA_E_NONE)
	{
		driverBfdStateUpdate(endpoint);
	}    
#endif

	dataPtr = avlSearch(&ofdbAVLTrees.ofdbBfdTable_tree, &lmepId, AVL_EXACT);
	
	if (dataPtr != 0)
	{
		/*结合localstate和故障字判断线路状态*/
		if(dataPtr->status.localState == ofdpa_bfdStateUp
			&&dataPtr->status.remoteDiag != ofdpa_bfdDiagCodeConcatPathDown
			&&dataPtr->status.remoteDiag != ofdpa_bfdDiagCodeRevConcatPathDown)
		{
			*bfdState = 1;
		}
		else
		{
			*bfdState = 0;
		}
	}

	return rc;

}

OFDPA_ERROR_t ofdbBfdReferenceUpdate(uint32_t lmepId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbBfdTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbBfdTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->status.refCount++;
    }
    else
    {
      if (dataPtr->status.refCount > 0)
      {
        dataPtr->status.refCount--;
      }
    }
  }

  return rc;
}

#endif

#ifdef INCLUDE_BFD

OFDPA_ERROR_t ofdbBfdEventNotify(
                                        uint32_t flags,
                                        uint32_t event_type,
                                        uint32_t endpoint,
                                        ofdpaBfdStatus_t *bfdStatus
                                      )
{

  uint32_t lmepId = 0;
  ofdbBfdTable_node_t *dataPtr;
  OFDB_BFD_EVENT_TYPE_t bfdEvent = OFDB_BFD_EVENT_NONE;

  driverBfdLocalMepIdMapGet(endpoint, &lmepId);

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
					 "lmepid= %d(endpoint %d)  event_type = 0x%x\r\n",
					 lmepId, endpoint, event_type);

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbBfdTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0 && bfdStatus != NULL)
  {

#if 1
	  dpaEventMsg_t   eventMsg = {.msgType = DPA_EVENT_BFD_STATUS_MSG};
	  if(dataPtr->status.localState != bfdStatus->localState)   
	  {
	    if(bfdStatus->localState == ofdpa_bfdStateUp)
	    {
			bfdEvent = OFDB_BFD_EVENT_UP;
			dataPtr->status.sessionUpCount++;
			/*dataPtr->status.stSessionUpTime;*/
		}
		else
		{
			bfdEvent = OFDB_BFD_EVENT_DOWN;
			/*dataPtr->status.stSessionDownTime;*/
			dataPtr->status.lastCommLostDiag = bfdStatus->localDiag;
		}
		
	  	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
						 "lmepid= %d(endpoint %d)  bfdEvent = %d\r\n",
						 lmepId, endpoint, bfdEvent);
		
	    eventMsg.msgSubType = bfdEvent;/*ofdbOamEventMapping[event_type] ;*/

	    eventMsg.cookie[0] = lmepId;
	    eventMsg.cookie[1] = endpoint;
	    /*printf("event:%x --- %x \r\n",eventMsg.msgSubType,eventMsg.cookie[0]);*/
	    /* Notify client. */
	    datapathEventNotificationSend(&eventMsg);

				
	  }
#endif

  	dataPtr->status.negoDetectMult = bfdStatus->negoDetectMult;
  	dataPtr->status.negoTxInter = bfdStatus->negoTxInter;
  	dataPtr->status.negoRxInter = bfdStatus->negoRxInter;	
  	dataPtr->status.negoEchoInter = bfdStatus->negoEchoInter;	
  	dataPtr->status.remoteDisc = bfdStatus->remoteDisc;	
	dataPtr->status.remoteState = bfdStatus->remoteState;	
	dataPtr->status.remoteDiag = bfdStatus->remoteDiag;
	
  	dataPtr->status.localState = bfdStatus->localState;	
  	dataPtr->status.localDiag = bfdStatus->localDiag;	

  	dataPtr->status.ctrlPktIn = bfdStatus->ctrlPktIn;
  	dataPtr->status.ctrlPktOut= bfdStatus->ctrlPktOut;
  	dataPtr->status.ctrlPktDrop = bfdStatus->ctrlPktDrop;	
  }
  else
  {
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
						 "lmepid= %d(endpoint %d)  event_type = 0x%x\r\n",
						 lmepId, endpoint, event_type);
  }
	
  return 0;	
  
}


#endif



#if 1
static ofdbPwRedunTable_node_t *ofdbPwRedunNodeGet(uint32_t index)
{
  return(avlSearch(&ofdbAVLTrees.ofdbPwRedunTable_tree, &index, AVL_EXACT));
}

int ofdbPwRedunConfigValidate(uint32_t index, ofdpaPwRedunConfig_t *config)
{
  if ((index < OFDPA_G8131_PG_MIN_ID) || (index > OFDPA_G8131_PG_MAX_ID))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MLP index out of range. index = %d\r\n",
                       index);
    return(0);
  }
 
  /* per ITU-T G8131 wait-to-restore time is 5 to 12 minutes expressed in 1-minute steps */
  if ((config->wtrTime > 12) || (config->wtrTime < 0))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                       "MLP wait-to-restore time out of range. wtrTime = %d\r\n",
                       config->wtrTime);
    return(0);
  }

  return(1);
}

OFDPA_ERROR_t ofdbPwRedunConfig(uint32_t index, ofdpaMlpheadendcfg_t *mlpcfg)
{
	return 0;
}

OFDPA_ERROR_t ofdbPwRedunUnconfig(uint32_t index)
{
	return 0;
}

OFDPA_ERROR_t ofdbPwRedunAdd(uint32_t index, ofdpaPwRedunConfig_t *config)
{
	return 0;
}

OFDPA_ERROR_t ofdbPwRedunDelete(uint32_t index)
{
	return 0;
}

static void ofdbPwRedunStatusGet(uint32_t index, ofdpaPwRedunStatus_t *status)
{
	return 0;
}

OFDPA_ERROR_t ofdbPwRedunGet(uint32_t index, ofdpaPwRedunConfig_t *config, ofdpaPwRedunStatus_t *status)
{
	return 0;
}

OFDPA_ERROR_t ofdbPwRedunNextGet(uint32_t index, uint32_t *nextIndex)
{
	return 0;
}

OFDPA_ERROR_t ofdbPwRedunReferenceUpdate(uint32_t index, uint32_t increment)
{
	return 0;
}


static ofdbPwRedunMepList_node_t *ofdbBfdProtectionNodeGet(uint32_t mlpGroupIndex, uint32_t lmepId)
{
	return 0;
}


static ofdbPwRedunMepList_node_t *ofdbBfdProtectionNodeNextGet(uint32_t mlpGroupIndex, uint32_t lmepId)
{
	return 0;
}


int ofdbBfdProtectionValidate(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg)
{
	return 0;
}


OFDPA_ERROR_t ofdbBfdProtectionAdd(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg)
{
	return 0;
}


OFDPA_ERROR_t ofdbBfdProtectionDelete(uint32_t mlpGroupIndex, uint32_t lmepId)
{
	return 0;
}


OFDPA_ERROR_t ofdbBfdProtectionGet(uint32_t mlpGroupIndex, uint32_t lmepId)
{
	return 0;
}


OFDPA_ERROR_t ofdbBfdProtectionNextGet(uint32_t mlpGroupIndex, uint32_t lmepId, uint32_t *nextLmepId)
{
	return 0;
}



#endif


#if 1

int ofdbY1564ConfigValidate(uint32_t lmepId)
{
  if (0 == lmepId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "lmepId cannot be zero.\r\n", 0);
    return(0);
  }

  return 1;
}


OFDPA_ERROR_t ofdbY1564Add(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbY1564Table_node_t node, *dataPtr;
  OFDB_OAM_TYPE_t oamType = OFDB_OAM_TYPE_OAM_MEP;
  
  /* Check if localMpId already used by a MIP and mep
   MEP and Mip check bfd*/
  if (OFDPA_E_NOT_FOUND != ofdbOamTypeGet(lmepId, &oamType))
  {
    /*支持重复配置*/
    if(oamType == OFDB_OAM_TYPE_Y1564)
    {
		return OFDPA_E_NONE;
	}
    return OFDPA_E_EXISTS;
  }


  memset(&node, 0, sizeof(node));

  node.lmepId = lmepId;
  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbY1564Table_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbY1564Table_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. lmepId = %d\r\n",
                         lmepId);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /*nothing to do*/
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbY1564Delete(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbY1564Table_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbY1564Table_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0)
  {
  
    {

      dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbY1564Table_tree, dataPtr);
      if (dataPtr == 0)
      {
        /* this should never happen; if it does, something is wrong with the AVL library */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete AVL tree entry just found. lmepId = %d\r\n",
                           lmepId);
        rc = OFDPA_E_INTERNAL;
      }
    }
  }

  return rc;
}



OFDPA_ERROR_t ofdbY1564Get(uint32_t lmepId, ofdpaY1564Status_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbY1564Table_node_t *dataPtr;
  uint32_t endpoint;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbY1564Table_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0)
  {

    if (status)
    {
      memcpy(status, &dataPtr->status, sizeof(dataPtr->status));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}


OFDPA_ERROR_t ofdbY1564NextGet(uint32_t lmepId, uint32_t *nextLmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbY1564Table_node_t *dataPtr;

  if (nextLmepId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbY1564Table_tree, &lmepId, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextLmepId = dataPtr->lmepId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}



OFDPA_ERROR_t ofdbY1564ReferenceUpdate(uint32_t lmepId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbY1564Table_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbY1564Table_tree, &lmepId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->status.refCount++;
    }
    else
    {
      if (dataPtr->status.refCount > 0)
      {
        dataPtr->status.refCount--;
      }
    }
  }

  return rc;
}


int ofdbLoopbackConfigValidate(uint32_t lmepId)
{
  if (0 == lmepId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "lmepId cannot be zero.\r\n", 0);
    return(0);
  }

  return 1;
}


OFDPA_ERROR_t ofdbLoopbackAdd(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbLbTable_node_t node, *dataPtr;

  /* Check if localMpId already used by a MIP and mep
   MEP and Mip check bfd*/
  if (OFDPA_E_NOT_FOUND != ofdbOamTypeGet(lmepId, NULL))
  {
    return OFDPA_E_EXISTS;
  }


  memset(&node, 0, sizeof(node));

  node.lmepId = lmepId;
  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbLbTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbLbTable_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. lmepId = %d\r\n",
                         lmepId);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /*nothing to do*/
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbLoopbackDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbLbTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbLbTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0)
  {
  
    {

      dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbLbTable_tree, dataPtr);
      if (dataPtr == 0)
      {
        /* this should never happen; if it does, something is wrong with the AVL library */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete AVL tree entry just found. lmepId = %d\r\n",
                           lmepId);
        rc = OFDPA_E_INTERNAL;
      }
    }
  }

  return rc;
}



OFDPA_ERROR_t ofdbLoopbackGet(uint32_t lmepId, ofdpaY1564Status_t *status)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbLbTable_node_t *dataPtr;
  uint32_t endpoint;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbLbTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0)
  {

    if (status)
    {
      memcpy(status, &dataPtr->status, sizeof(dataPtr->status));
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}


OFDPA_ERROR_t ofdbLoopbackNextGet(uint32_t lmepId, uint32_t *nextLmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbLbTable_node_t *dataPtr;

  if (nextLmepId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbLbTable_tree, &lmepId, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextLmepId = dataPtr->lmepId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}



OFDPA_ERROR_t ofdbLoopbackReferenceUpdate(uint32_t lmepId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbLbTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbLbTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (increment != 0)
    {
      dataPtr->status.refCount++;
    }
    else
    {
      if (dataPtr->status.refCount > 0)
      {
        dataPtr->status.refCount--;
      }
    }
  }

  return rc;
}



OFDPA_ERROR_t ofdbLmepidAdd(uint32_t lmepId, OFDB_OAM_TYPE_t oamtype)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbLmepidTable_node_t node, *dataPtr;
  OFDB_OAM_TYPE_t oldType = OFDB_OAM_TYPE_OAM_MEP;
  
  rc = ofdbLmepidGet(lmepId, &oldType);
  if(rc == OFDPA_E_NONE )
  {
    if(oamtype == oldType)
    {
		return rc;
    }
	else
	{		
		/*类型变化，先删除后添加*/
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "lmepId type change for %d to %d.\r\n", oldType, oamtype);
		ofdbLmepidDelete(lmepId);
	}
  }
 

  memset(&node, 0, sizeof(node));

  node.lmepId = lmepId;
  node.type = oamtype;
  dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbLmepidTable_tree, &node);

  if (dataPtr == &node)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }
  else
  {
    /* successful insertion, retrieve pointer to node in AVL tree */
    dataPtr = avlSearch(&ofdbAVLTrees.ofdbLmepidTable_tree, &node, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* this should never happen; if it does, something is wrong with the AVL library */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Failed to find AVL tree entry just added. lmepId = %d\r\n",
                         lmepId);
      rc = OFDPA_E_INTERNAL;
    }
    else
    {
      /*nothing to do*/
    }
  }

  return rc;
}

OFDPA_ERROR_t ofdbLmepidDelete(uint32_t lmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbLmepidTable_node_t *dataPtr;
  


  dataPtr = avlSearch(&ofdbAVLTrees.ofdbLmepidTable_tree, &lmepId, AVL_EXACT);
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
							 "ofdbLmepidDelete. lmepId = %d dataPtr = %x\r\n",
							 lmepId, dataPtr);

  if (dataPtr != 0)
  {      
      dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbLmepidTable_tree, dataPtr);
      if (dataPtr == 0)
      {
        /* this should never happen; if it does, something is wrong with the AVL library */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Failed to delete AVL tree entry just found. lmepId = %d\r\n",
                           lmepId);
        rc = OFDPA_E_INTERNAL;
      }    
  }

  return rc;
}



OFDPA_ERROR_t ofdbLmepidGet(uint32_t lmepId, OFDB_OAM_TYPE_t *oamType)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbLmepidTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbLmepidTable_tree, &lmepId, AVL_EXACT);

  if (dataPtr != 0)
  {
    if (oamType != NULL)
    {
      *oamType = dataPtr->type;
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}


OFDPA_ERROR_t ofdbLmepidNextGet(uint32_t lmepId, uint32_t *nextLmepId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbLmepidTable_node_t *dataPtr;

  if (nextLmepId == NULL)
  {
    return OFDPA_E_PARAM;
  }

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbLmepidTable_tree, &lmepId, AVL_NEXT);

  if (dataPtr != 0)
  {
    *nextLmepId = dataPtr->lmepId;
    rc = OFDPA_E_NONE;
  }

  return rc;
}



#endif



