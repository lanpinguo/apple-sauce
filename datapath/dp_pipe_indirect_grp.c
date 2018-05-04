/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_indirect_grp.c
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/3/27
  Last Modified :
  Description   : indirect group operation api
  Function List :
              dpGrpL2IntfBuktBuild
              dpGrpMplsIntfBuktBuild
              dpGrpMplsLabelBuktBuild
              indirectGroupAdd
              indirectGroupBucketEntryAdd
              indirectGroupBucketEntryDelete
              indirectGroupBucketEntryModify
              indirectGroupBucketProcess
              indirectGroupBucketsDeleteAll
              indirectGroupDelete
              indirectGrpPktProcess
  History       :
  1.Date        : 2018/3/27
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
#include "tpool.h"
#include "ofdpa_datatypes.h"
#include "ofdb.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"
#include "datapath.h"
#include "dp_pipe_group.h"


OFDPA_ERROR_t indirectGroupAdd(ofdpaGroupEntry_t *group)
{
	ofdpaGrpPipeNode_t * pNew;

	if(group == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null Group passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERY_VERBOSE,
										 "Add new indirect Group 0x%08x \r\n", group->groupId);

	pNew = dpGrpNodeMalloc(OFDPA_GRP_TYPE_INDIRECT,1);
	if(pNew){
		pNew->grpId = group->groupId;
		group->ptrGrpInst = pNew;
		
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERY_VERBOSE,
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
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_VERY_VERBOSE,
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




void indirectGrpPktProcess( dpGrpWork_t *work)
{
	OFDPA_ERROR_t rv;
	ofdpaPktCb_t *pPkt ;
	ofdpaGrpPipeNode_t * pGrp;
	ofdpaActArg_t arg = {.type = ACT_OP_TYPE_EXECUTE};
	ofdpaAct_t *pAct = NULL;
	ofdpaActBucket_t *pBucket = NULL;
	dpGrpWork_t newWork;
	ofdpaPcbMsg_t msg;
	int i;

	

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_VERY_VERBOSE,
										"Receive packet %p.\r\n",work->pPkt);

	pPkt = work->pPkt;
	pGrp = work->pGrp;

	pBucket = pGrp->actBukts[0];
	
	for(i = 0 ; i < pBucket->numAct; i++){
		pAct = &pBucket->act[i];
		if(pAct->act == NULL){
			break;
		}
		arg.data = pPkt;
		pAct->act(pAct, &arg);
	}

	if(pBucket->ptrRefGrpInst != NULL){
		newWork.pGrp = pBucket->ptrRefGrpInst;					
		newWork.pPkt = pPkt;
		rv = dpGrpSubWorkDispatch(&newWork);
		if (rv != OFDPA_E_NONE)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
												"Failed to dispatch new work. Error %d.\r\n", rv);
			return ;
		}
	}
	else{
		
		msg.dstObjectId = OFDPA_FLOW_TABLE_ID_EGRESS_VLAN;
		msg.pcb = pPkt;
		rv = datapathPipeMsgSend(grp_pipe_config.nodeSock ,&msg);
		if(rv != OFDPA_E_NONE){
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
							 "Failed to send msg, rv = %d\r\n",rv);
		}
	}



	
}

