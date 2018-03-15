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
	int i;



	*ppBucket = dpGrpBucketMalloc(OFDPA_GRP_L2_INTF_BUKT_ACT_MAX);
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

OFDPA_ERROR_t dpGrpMplsIntfBuktBuild(ofdpaMPLSInterfaceGroupBucketData_t       * pData,ofdpaActBucket_t	**ppBucket)
{
  OFDPA_ERROR_t rv;
	ofdpaAct_t				action;
	int i;


	*ppBucket = dpGrpBucketMalloc(OFDPA_GRP_MPLS_INTF_BUKT_ACT_MAX);
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
	int i;



	*ppBucket = dpGrpBucketMalloc(OFDPA_GRP_MPLS_LABEL_BUKT_ACT_MAX);
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
		
		pBukt->ptrRefGrpInst = NULL;
  }
  else{
		pBukt->ptrRefGrpInst = group.ptrGrpInst;
  }

	

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


OFDPA_ERROR_t dpGroupDelete(uint32_t groupId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdbGroupTable_node_t group_node;
  ofdbGroupTable_node_t *dataPtr;

  memset(&group_node, 0, sizeof(group_node));
  group_node.group.groupId = groupId;

  dataPtr = avlDeleteEntry(&grp_pipe_config.ofdbGroupTable_tree, &group_node);

  if (dataPtr == NULL)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Group Table entry not found!\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }
  else
  {
    grp_pipe_config.grpStatus->num_groups_in_database--;
    grp_pipe_config.grpStatus->groupTableEntryCount[OFDB_GROUP_TYPE(groupId)]--;

  }

  return rc;
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


OFDPA_ERROR_t dpGroupBucketEntryDelete(uint32_t groupId, uint32_t bucketIndex)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;

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
  uint32_t subType;
	ofdbGroupTable_node_t grpNode;
	ofdpaGrpPipeNode_t * pGrpPipeNode;
	ofdpaGrpPipeNode_t * pRefGrpPipeNode;
	ofdpaActBucket_t		*pBucket;
	int	i,supported = 0;
	

	if(groupBucket == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "NULL pointer!\r\n",0);
		return OFDPA_E_PARAM;
	}



	rc = dpGroupGet(groupId, &grpNode);
	if(rc != OFDPA_E_NONE){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Group not found, rc = %d!\r\n",rc);
		return rc;
	}



	
	groupBucket->groupId 		= grpNode.group.groupId;
	groupBucket->ptrGrpInst = grpNode.group.ptrGrpInst;

	pGrpPipeNode = grpNode.group.ptrGrpInst;

	if(bucketIndex >= pGrpPipeNode->numActBukt){
		return OFDPA_E_PARAM;
	}

	pBucket = pGrpPipeNode->actBukts[bucketIndex];


  switch (OFDB_GROUP_TYPE(groupId))
	{
		case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
			for(i = 0 ; i < pBucket->numAct; i++){
				groupBucket->bucketData.l2Interface.actions[i] = pBucket->act[i];
				groupBucket->bucketData.l2Interface.act_cnt++;
			}
			supported = 1;
			break;

			break;
		case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
			subType = OFDB_GROUP_MPLS_SUBTYPE(groupId);
			switch ( subType )
			{
			    case OFDPA_MPLS_INTERFACE :
						for(i = 0 ; i < pBucket->numAct; i++){
							groupBucket->bucketData.mplsInterface.actions[i] = pBucket->act[i];
							groupBucket->bucketData.mplsInterface.act_cnt++;
						}
						supported = 1;
		        break;
			    case OFDPA_MPLS_L2_VPN_LABEL :
					case OFDPA_MPLS_L3_VPN_LABEL :
					case OFDPA_MPLS_TUNNEL_LABEL1:
					case OFDPA_MPLS_TUNNEL_LABEL2:
					case OFDPA_MPLS_SWAP_LABEL:
						for(i = 0 ; i < pBucket->numAct; i++){
							groupBucket->bucketData.mplsLabel.actions[i] = pBucket->act[i];
							groupBucket->bucketData.mplsLabel.act_cnt++;
						}
						supported = 1;
					  break;
			    default:
			      break;
			}
			break;
		default:
			break;
	}

	if(supported){
		pRefGrpPipeNode = pBucket->ptrRefGrpInst;
		if(pRefGrpPipeNode){
			groupBucket->referenceGroupId = pRefGrpPipeNode->grpId;
		}
		else{
			groupBucket->referenceGroupId = 0;
		}
	}
 return rc;
}




OFDPA_ERROR_t dpGroupBucketEntryFirstGet(uint32_t groupId,
																					 ofdpaGroupBucketEntry_t *firstGroupBucket)
{
	return dpGroupBucketEntryGet(groupId,0,firstGroupBucket);
}


OFDPA_ERROR_t dpGroupBucketEntryNextGet(uint32_t groupId, uint32_t bucketIndex,
																 ofdpaGroupBucketEntry_t *nextGroupBucket)
{
	return dpGroupBucketEntryGet(groupId,bucketIndex + 1,nextGroupBucket);
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
