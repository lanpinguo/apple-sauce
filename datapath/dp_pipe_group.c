/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_grp.c
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/2/23
  Last Modified :
  Description   : group table l2 interface instance

  History       :
  1.Date        : 2018/2/23
    Author      : lanpinguo
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include "ofdpa_datatypes.h"
#include "ofdb.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"
#include "datapath.h"
#include "dp_pipe_group.h"


#define DP_ADD_ACTION_TO_BUCKET(pBukt,pAct) \
{ \
	rv = dpAddAct2Bukt(pBukt,pAct); \
	if(rv != OFDPA_E_NONE){ \
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC, \
											 "Add action failed, rv = %d!\r\n", rv); \
		return rv; \
	} \
}while(0)

ofdpaGrpPipeNodeConfig_t grp_pipe_config;


int getGrpSockFd(void)
{
	return	grp_pipe_config.nodeSock;
}


OFDPA_ERROR_t indirectGroupAdd(ofdpaGroupEntry_t *group)
{
	ofdpaGrpPipeNode_t * pNew;

	if(group == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null Group passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "Add new indirect Group 0x%08x \r\n", group->groupId);

	pNew = dpGrpNodeMalloc(OFDPA_GRP_TYPE_INDIRECT,1);
	if(pNew){
		pNew->grpId = group->groupId;
		group->ptrGrpInst = pNew;
		
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Add new node to list!\r\n", 0);
		ofdpa_list_add(&pNew->list,&grp_pipe_config.entrys);
		
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERBOSE,
											 "New group 0x%08x node added!\r\n", group->groupId);
		return OFDPA_E_NONE;
	}

	return OFDPA_E_FAIL;
}

OFDPA_ERROR_t indirectGroupDelete(uint32_t groupId)
{
	return OFDPA_E_NONE;

}



OFDPA_ERROR_t dpGrpL2IntfBuktBuild(ofdpaL2InterfaceGroupBucketData_t       * pData,ofdpaActBucket_t	**ppBucket)
{
  OFDPA_ERROR_t rv;
	ofdpaAct_t				action;



	*ppBucket = dpGrpBucketMalloc(3);
	if(*ppBucket == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Bucket Malloc failed!\r\n", 0);
		return OFDPA_E_PARAM;
	}
	action.act = ofdpaActIdentifyOutPort;
	action.arg = pData->outputPort;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	
	if(pData->popVlanTag){
		action.act = ofdpaActPopVlan;
		action.arg = 0;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}
	
	if(pData->allowVlanTranslation){
		action.act = ofdpaActAllowVlanTrans;
		action.arg = 0;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	
	}

	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpGrpMplsIntfBuktBuild(ofdpaMPLSInterfaceGroupBucketData_t       * pData,ofdpaActBucket_t	**ppBucket)
{
  OFDPA_ERROR_t rv;
	ofdpaAct_t				action;
	int i;


	*ppBucket = dpGrpBucketMalloc(6);
	if(*ppBucket == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Bucket Malloc failed!\r\n", 0);
		return OFDPA_E_PARAM;
	}


	for(i = 0; i < pData->act_cnt; i++){
		action = pData->actions[i];
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}


	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpGrpMplsLabelBuktBuild(ofdpaMPLSLabelGroupBucketData_t       * pData, ofdpaActBucket_t	**ppBucket)
{
  OFDPA_ERROR_t rv;
	ofdpaAct_t				action;



	*ppBucket = dpGrpBucketMalloc(16);
	if(*ppBucket == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Bucket Malloc failed!\r\n", 0);
		return OFDPA_E_PARAM;
	}
	
	action.act = ofdpaActPushL2Hdr;
	action.arg = 0;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);


	action.act = ofdpaActPushVlan;
	action.arg = 0;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);


	action.act = ofdpaActSetTpid;
	action.arg = pData->newTpid;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = ofdpaActPushMplsHdr;
	action.arg = pData->newTpid;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = ofdpaActSetEtherType;
	action.arg = pData->mplsEtherType;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	
	action.act = ofdpaActPushMplsCw;
	action.arg = 0;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = ofdpaActSetMplsLabel;
	action.arg = pData->mplsLabel;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = ofdpaActSetMplsBos;
	action.arg = pData->mplsBOS;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	
	if(pData->mplsEXPAction){
		action.act = ofdpaActSetMplsExp;
		action.arg = pData->mplsEXP;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->mplsCopyEXPOutwards){
		action.act = ofdpaActCpyMplsExpOutwards;
		action.arg = pData->mplsCopyEXPOutwards;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	
	if(pData->remarkTableIndexAction){
		action.act = ofdpaActSetRemarkTableId;
		action.arg = pData->remarkTableIndex;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->mplsTTLAction){
		action.act = ofdpaActSetMplsTtl;
		action.arg = pData->mplsTTL;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->mplsCopyTTLOutwards){
		action.act = ofdpaActCpyMplsTtlOutwards;
		action.arg = pData->mplsCopyTTLOutwards;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->lmepIdAction){
		action.act = ofdpaActSetLmepId;
		action.arg = pData->lmepId;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->oamLmTxCountAction){
		action.act = ofdpaActOamLmTxCount;
		action.arg = 0;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->colorBasedCountAction){
		action.act = ofdpaActIncColorBasedCount;
		action.arg = pData->colorBasedCountId;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	return OFDPA_E_NONE;
}


OFDPA_ERROR_t indirectGroupBucketEntryAdd(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rv;
  uint32_t subType;
  ofdpaGroupEntry_t group;
	ofdpaActBucket_t	*pBukt = NULL;

	if(groupBucket == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null Group bucket passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}

	if(groupBucket->ptrGrpInst == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null Group passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}



  switch (OFDB_GROUP_TYPE(groupBucket->groupId))
	{
		case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
			 rv = dpGrpL2IntfBuktBuild(&groupBucket->bucketData.l2Interface,&pBukt);
			break;
		case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
			subType = OFDB_GROUP_MPLS_SUBTYPE(groupBucket->groupId);
			switch ( subType )
			{
			    case OFDPA_MPLS_INTERFACE :
						rv = dpGrpMplsIntfBuktBuild(&groupBucket->bucketData.mplsInterface,&pBukt);
		        break;
			    case OFDPA_MPLS_L2_VPN_LABEL :
					case OFDPA_MPLS_L3_VPN_LABEL :
					case OFDPA_MPLS_TUNNEL_LABEL1:
					case OFDPA_MPLS_TUNNEL_LABEL2:
					case OFDPA_MPLS_SWAP_LABEL:
						rv = dpGrpMplsLabelBuktBuild(&groupBucket->bucketData.mplsLabel,&pBukt);
					  break;
			    default:
			      break;
			}
			break;
		case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
		case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
		case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
		case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
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
		      break;

		    default:
		      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
		                     "Invalid MPLS Forwarding Group Subtype!\r\n", 0);
		  }

		  break;

		default:
		  /* Invalid Group ID */
		  /* this should never happen since group ID has been validated above */
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_ALWAYS,
		                     "Unexpected Group ID in driver switch statement\r\n", 0);
		  return OFDPA_E_UNAVAIL;                   
	}


  if (OFDPA_E_NONE != dpGroupGet(groupBucket->referenceGroupId, &group))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Group (0x%x) not present in Group Table!\r\n", groupBucket->referenceGroupId);
  }

	
	pBukt->ptrRefGrpInst = group.ptrGrpInst;

	rv = dpBindBukt2Grp(groupBucket->ptrGrpInst,pBukt);
	if(rv != OFDPA_E_NONE){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Add bucket failed, rv = %d!\r\n", rv);
	}

	return rv;
}




OFDPA_ERROR_t indirectGroupBucketEntryDelete(uint32_t groupId, uint32_t bucketIndex)
{

	return OFDPA_E_NONE;
}

OFDPA_ERROR_t indirectGroupBucketsDeleteAll(uint32_t groupId)
{

	return OFDPA_E_NONE;
}



OFDPA_ERROR_t indirectGroupBucketEntryModify(ofdpaGroupBucketEntry_t *groupBucket)
{
	return OFDPA_E_NONE;
}






static OFDPA_ERROR_t indirectGrpPktProcess( ofdpaPcbMsg_t *msg)
{
	ofdpaGrpPipeNode_t * pGrp;
	int i;

	
	/* Check whether dst group instance is right */
	if(msg->pGroupInst == NULL){
		return OFDPA_E_UNAVAIL;
	}

	pGrp = msg->pGroupInst;
	if(pGrp->this != pGrp){
		return OFDPA_E_PARAM;
	}

	if(pGrp->actBukts[0] == NULL){

		return OFDPA_E_INTERNAL;
	}

	for(i = 0 ; i < pGrp->actBukts[0]->numAct; i++){
		if(pGrp->actBukts[0]->act[i].act == NULL){
			break;
		}
		pGrp->actBukts[0]->act[i].act(pGrp, msg->pcb, pGrp->actBukts[0]->act[i].arg);
	}

	
  return OFDPA_E_NONE;
}



static OFDPA_ERROR_t indirectGrpPipeInPktRecv(struct timeval *timeout)
{
  int pipeInPktSockFd;
  ofdpaPcbMsg_t msg;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

	

  pipeInPktSockFd = getGrpSockFd();
  if (pipeInPktSockFd < 0)
  {
    return OFDPA_E_FAIL;
  }

  if (timeout)
  {
    if ((timeout->tv_sec == 0) && (timeout->tv_usec == 0))
    {
      /* set socket to non-blocking for this read */
      flags |= MSG_DONTWAIT;
    }
    else
    {
      /* blocking socket with a timeout */
      rv = setsockopt(pipeInPktSockFd, SOL_SOCKET, SO_RCVTIMEO, (char *)timeout,
                      sizeof(struct timeval));
      if (rv < 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                          "Failed to set packet receive timeout. Error %d.\r\n", rv);
        return OFDPA_E_FAIL;
      }
    }
  }
  else
  {
    /* blocking socket with no timeout. Make sure there is no timeout configured
     * on the socket from previous call. */
    rv = setsockopt(pipeInPktSockFd, SOL_SOCKET, SO_RCVTIMEO, NULL, 0);
  }

  recvBytes = recvfrom(pipeInPktSockFd, &msg, sizeof(msg), flags, 0, 0);

  if (recvBytes < 0)
  {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
    {
      /* Normal if no packets waiting to be received and caller didn't block. */
      return OFDPA_E_TIMEOUT;
    }
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to receive packet. recvfrom() returned %d. errno %s.\r\n",
                      recvBytes, strerror(errno));
    return OFDPA_E_FAIL;
  }

	indirectGrpPktProcess(&msg);

  return OFDPA_E_NONE;
}



static void grp_pipe_thread_core(void * arg)
{
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_INDIRECT_GRP, &grp_pipe_config.nodeSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}
	
	OFDPA_INIT_LIST_HEAD(&grp_pipe_config.entrys);

	while(1){
	
		indirectGrpPipeInPktRecv(NULL);

	}


}



int dpGroupBucketRefTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbGroupBucketRefTable_node_t *pA = a;
  ofdbGroupBucketRefTable_node_t *pB = b;

  if (pA->groupBucket.groupId > pB->groupBucket.groupId)
  {
    return 1;
  }
  if (pA->groupBucket.groupId < pB->groupBucket.groupId)
  {
    return -1;
  }

  if (pA->groupBucket.bucketIndex > pB->groupBucket.bucketIndex)
  {
    return 1;
  }
  if (pA->groupBucket.bucketIndex < pB->groupBucket.bucketIndex)
  {
    return -1;
  }
  return 0;
}

int dpGroupBucketDataTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbGroupBucketDataTable_node_t *pA = a;
  ofdbGroupBucketDataTable_node_t *pB = b;

  if (pA->groupBucket.groupId > pB->groupBucket.groupId)
  {
    return 1;
  }
  if (pA->groupBucket.groupId < pB->groupBucket.groupId)
  {
    return -1;
  }

  if (pA->groupBucket.bucketIndex > pB->groupBucket.bucketIndex)
  {
    return 1;
  }
  if (pA->groupBucket.bucketIndex < pB->groupBucket.bucketIndex)
  {
    return -1;
  }
  return 0;
}


OFDPA_ERROR_t dpGroupAdd(ofdpaGroupEntry_t *group)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupTable_node_t group_node;
  ofdbGroupTable_node_t *dataPtr;



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
                       "Failed to add Group entry !\r\n", 0);
    return rc;                   
  }



  memset(&group_node, 0, sizeof(group_node));

  memcpy(&group_node.group, group, sizeof(ofdpaGroupEntry_t));
  group_node.stats.groupAddUpTime = dpaUpTimeSeconds();

  dataPtr = avlInsertEntry(&grp_pipe_config.ofdbGroupTable_tree, &group_node);

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
    grp_pipe_config.grpStatus->num_groups_in_database++;
    grp_pipe_config.grpStatus->groupTableEntryCount[OFDB_GROUP_TYPE(group->groupId)]++;

  }

  return rc;
}


OFDPA_ERROR_t dpGroupReferenceUpdate(uint32_t groupId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&grp_pipe_config.ofdbGroupTable_tree, &groupId, AVL_EXACT);
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

OFDPA_ERROR_t dpGroupBucketCountUpdate(uint32_t groupId, uint32_t increment)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&grp_pipe_config.ofdbGroupTable_tree, &groupId, AVL_EXACT);
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

OFDPA_ERROR_t dpGroupBucketEntryAdd(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  uint32_t  subType;



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

  if (OFDPA_E_NONE == rc)
  {
		dpGroupReferenceUpdate(groupBucket->referenceGroupId, 1);
		dpGroupBucketCountUpdate(groupBucket->groupId, 1);
  }
  else{
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
                       "Failed to add Bucket in Hardware; rc = %d!\r\n", rc);
  }




  return rc;
}





OFDPA_ERROR_t dpGroupDataUpdate(ofdpaGroupEntry_t *group)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&grp_pipe_config.ofdbGroupTable_tree, &group->groupId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy(&dataPtr->group,group, sizeof(ofdpaGroupEntry_t));
  }

  return rc;
}


OFDPA_ERROR_t dpGroupGet(uint32_t groupId, ofdpaGroupEntry_t *group)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&grp_pipe_config.ofdbGroupTable_tree, &groupId, AVL_EXACT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy(group, &dataPtr->group, sizeof(ofdpaGroupEntry_t));
  }

  return rc;
}


OFDPA_ERROR_t dpGroupStatsGet(uint32_t groupId, ofdpaGroupEntryStats_t *groupStats)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupTable_node_t *dataPtr;
  ofdpaGroupEntryStats_t stats;
  dataPtr = avlSearch(&grp_pipe_config.ofdbGroupTable_tree, &groupId, AVL_EXACT);

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



OFDPA_ERROR_t dpRefGroupValidate(uint32_t referredGroupId)
{
  ofdpaGroupEntryStats_t refGroupStats;

  if (OFDPA_E_NONE != dpGroupStatsGet(referredGroupId, &refGroupStats))
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



OFDPA_ERROR_t dpGroupBucketValidate(ofdpaGroupBucketEntry_t *groupBucket)
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

    rc = dpRefGroupValidate(groupBucket->referenceGroupId);

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
      break;

    default:
      /* Invalid Group ID */
      return OFDPA_E_PARAM;
  }

  return OFDPA_E_NONE;
}



OFDPA_ERROR_t dpGroupNextGet(uint32_t groupId, ofdpaGroupEntry_t *nextGroup)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdbGroupTable_node_t *dataPtr;

  dataPtr = avlSearch(&grp_pipe_config.ofdbGroupTable_tree, &groupId, AVL_NEXT);
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    memcpy(nextGroup, &dataPtr->group, sizeof(ofdpaGroupEntry_t));
  }

  return rc;
}

OFDPA_ERROR_t dpGroupTypeNextGet(uint32_t groupId,
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
    rc = dpGroupNextGet(groupId, nextGroup);

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


 


 
 OFDPA_ERROR_t dpGroupBucketEntryGet(uint32_t groupId, uint32_t bucketIndex,
															ofdpaGroupBucketEntry_t *groupBucket)
 {
	 OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
	
 
	 return rc;
 }




OFDPA_ERROR_t dpGroupBucketEntryFirstGet(uint32_t groupId,
																					 ofdpaGroupBucketEntry_t *firstGroupBucket)
{
	OFDPA_ERROR_t rc = OFDPA_E_FAIL;

	return rc;
}


OFDPA_ERROR_t dpGroupBucketEntryNextGet(uint32_t groupId, uint32_t bucketIndex,
																 ofdpaGroupBucketEntry_t *nextGroupBucket)
{
	OFDPA_ERROR_t rc = OFDPA_E_FAIL;

	return rc;
}


 
 uint32_t dpGroupTableTotalEntryCountGet(void)
 {
	 return(grp_pipe_config.grpStatus->num_groups_in_database);
 }
 
 uint32_t dpGroupTableMaxCountGet(OFDPA_GROUP_ENTRY_TYPE_t groupType)
 {
	 uint32_t value = 0;
 
	 if ((groupType >= OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) &&
			 (groupType < OFDPA_GROUP_ENTRY_TYPE_LAST))
	 {
		 value = grp_pipe_config.grpStatus->groupTableMaxCount[groupType];
	 }
 
	 return(value);
 }
 
 uint32_t dpGroupTableEntryCountGet(OFDPA_GROUP_ENTRY_TYPE_t groupType)
 {
	 uint32_t value = 0;
 
	 if ((groupType >= OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) &&
			 (groupType < OFDPA_GROUP_ENTRY_TYPE_LAST))
	 {
		 value = grp_pipe_config.grpStatus->groupTableEntryCount[groupType];
	 }
 
	 return(value);
 }
 
 uint32_t dpGroupBucketTableMaxCountGet(uint32_t groupId)
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
			 value = grp_pipe_config.grpBuktStatus->groupBucketTableMaxCount[groupType];
		 }
	 }
 
	 return(value);
 }



OFDPA_ERROR_t group_database_init(void)
{
	int ii;


	
  /* Group Table Global Status.
   */
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "Allocating %u bytes for  Group Table Status Data.\r\n",
                     sizeof(ofdpaGroupTableStatus_t));

  if (!(grp_pipe_config.grpStatus = malloc (sizeof(ofdpaGroupTableStatus_t))))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to allocate memory for  Group Table Status Data.\r\n", 0);
    return -1;
  }
  

  memset(grp_pipe_config.grpStatus, 0, sizeof(ofdpaGroupTableStatus_t));

  /* Group Table AVL Tree.
   */

  for (ii = OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE; ii < OFDPA_GROUP_ENTRY_TYPE_LAST; ii++)
  {
    grp_pipe_config.grpStatus->groupTableMaxCount[ii] = dpaGroupMaxSizeGet(ii);
    grp_pipe_config.grpStatus->group_database_size += dpaGroupMaxSizeGet(ii);

    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group count for type %d = %d.\r\n",
                       ii, grp_pipe_config.grpStatus->groupTableMaxCount[ii]);
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group_table_size = %d.\r\n",
                     grp_pipe_config.grpStatus->group_database_size);

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating  Group Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&grp_pipe_config.ofdbGroupTable_tree,
                               grp_pipe_config.grpStatus->group_database_size,
                               sizeof(ofdbGroupTable_node_t),
                               0x10,
                               avlCompareULong32,
                              0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create  Group Table AVL Tree.\r\n", 0);
    return -1;
  }


  /* Group Table Global Status.
   */
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "Allocating %u bytes for  Group Bucket Table Status Data.\r\n",
                     sizeof(ofdpaGroupBucketTableStatus_t));

  if (!(grp_pipe_config.grpBuktStatus = malloc (sizeof(ofdpaGroupBucketTableStatus_t))))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to allocate memory for  Group Bucket Table Status Data.\r\n", 0);
    return -1;
  }

  memset(grp_pipe_config.grpBuktStatus, 0, sizeof(ofdpaGroupBucketTableStatus_t));

  /* Group Bucket Table AVL Trees.
   */
	for (ii = OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE; ii < OFDPA_GROUP_ENTRY_TYPE_LAST; ii++)
	{
		if((ii == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
				|| (ii == OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
				//|| (ii == OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING)
				|| (ii == OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE)
				|| (ii == OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST)
				|| (ii == OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD))
				
		 {
			 grp_pipe_config.grpBuktStatus->groupBucketTableMaxCount[ii] = dpaGroupBucketMaxSizeGet(ii);
			 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group bucket count for type %d = %d.\r\n",
													ii, grp_pipe_config.grpBuktStatus->groupBucketTableMaxCount[ii]);
	 
			 switch(ii)
			 {
				 case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
				 case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
				 case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
				 case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
				 case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
				 case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
				 case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
					 grp_pipe_config.grpBuktStatus->group_bucket_data_database_size +=
						 (dpaGroupMaxSizeGet(ii) * grp_pipe_config.grpBuktStatus->groupBucketTableMaxCount[ii]);
					 break;
	 
				 case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
				 case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
				 case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
				 case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
					 grp_pipe_config.grpBuktStatus->group_bucket_ref_database_size +=
						 (dpaGroupMaxSizeGet(ii) * grp_pipe_config.grpBuktStatus->groupBucketTableMaxCount[ii]);
					 break;
	 
				 case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
					 /* Divide equally into data and reference table as this Group has both types  */
					 grp_pipe_config.grpBuktStatus->group_bucket_data_database_size +=
						 ((dpaGroupMaxSizeGet(ii)/2) * grp_pipe_config.grpBuktStatus->groupBucketTableMaxCount[ii]);
	 
					 grp_pipe_config.grpBuktStatus->group_bucket_ref_database_size +=
						 ((dpaGroupMaxSizeGet(ii)/2) * grp_pipe_config.grpBuktStatus->groupBucketTableMaxCount[ii]);
	 
					 break;
	 
					 /* coverity[dead_error_begin] */
				 default:
					 break;
			 }
		 }
	}


  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group_bucket_ref_table_size = %d.\r\n",
                     grp_pipe_config.grpBuktStatus->group_bucket_ref_database_size);

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group_bucket_data_table_size = %d.\r\n",
                     grp_pipe_config.grpBuktStatus->group_bucket_data_database_size);




}




/*****************************************************************************
 Prototype    : group_pipe_init
 Description  : this is vlan pipe init
 Input        : int argc      
                char *argv[]  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2018/2/18
    Author       : lanpinguo
    Modification : Created function

*****************************************************************************/
int group_pipe_init(int argc, char *argv[])
{
	OFDPA_ERROR_t rc;
	

	rc = group_database_init();
	if(rc != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to initialize  Group Table AVL Tree.\r\n", rc);
		return rc;
	}
	grp_pipe_config.max_entrys = 4094;
	grp_pipe_config.nodeTid = (pthread_t)dpaThreadCreate("indirectGrpTask", 62, grp_pipe_thread_core, NULL);


	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/
