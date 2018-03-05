/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_indirect_grp.c
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
#include "dp_pipe_indirect_grp.h"


#define DP_ADD_ACTION_TO_BUCKET(pBukt,pAct) \
{ \
	rv = dpAddAct2Bukt(pBukt,pAct); \
	if(rv != OFDPA_E_NONE){ \
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC, \
											 "Add action failed, rv = %d!\r\n", rv); \
		return rv; \
	} \
}while(0)

ofdpaIndirectGrpPipeNodeConfig_t indirect_grp_pipe_config;


int getIndirectGrpSockFd(void)
{
	return	indirect_grp_pipe_config.nodeSock;
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
		ofdpa_list_add(&pNew->list,&indirect_grp_pipe_config.entrys);
		
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
	action.act = dpActIdentifyOutPort;
	action.arg = pData->outputPort;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	
	if(pData->popVlanTag){
		action.act = dpActPopVlan;
		action.arg = 0;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}
	
	if(pData->allowVlanTranslation){
		action.act = dpActAllowVlanTrans;
		action.arg = 0;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	
	}

	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpGrpMplsIntfBuktBuild(ofdpaMPLSInterfaceGroupBucketData_t       * pData,ofdpaActBucket_t	**ppBucket)
{
  OFDPA_ERROR_t rv;
	ofdpaAct_t				action;



	*ppBucket = dpGrpBucketMalloc(6);
	if(*ppBucket == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Bucket Malloc failed!\r\n", 0);
		return OFDPA_E_PARAM;
	}

	if(pData->oamLmTxCountAction){
		action.act = dpActOamLmTxCount;
		action.arg = 0;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}
		
	
	action.act = dpActSetSrcMac;
	action.arg = *(uint64_t *)&pData->srcMac;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = dpActSetDstMac;
	action.arg = *(uint64_t *)&pData->dstMac;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = dpActSetVlanId;
	action.arg = pData->vlanId;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);


	
	if(pData->lmepIdAction){
		action.act = dpActSetLmepId;
		action.arg = pData->lmepId;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	
	}
	
	if(pData->colorBasedCountAction){
		action.act = dpActIncColorBasedCount;
		action.arg = pData->colorBasedCountId;
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
	
	action.act = dpActPushL2Hdr;
	action.arg = 0;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);


	action.act = dpActPushVlan;
	action.arg = 0;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);


	action.act = dpActSetTpid;
	action.arg = pData->newTpid;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = dpActPushMplsHdr;
	action.arg = pData->newTpid;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = dpActSetEtherType;
	action.arg = pData->mplsEtherType;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	
	action.act = dpActPushMplsCw;
	action.arg = 0;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = dpActSetMplsLabel;
	action.arg = pData->mplsLabel;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	action.act = dpActSetMplsBos;
	action.arg = pData->mplsBOS;
	DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);

	
	if(pData->mplsEXPAction){
		action.act = dpActSetMplsExp;
		action.arg = pData->mplsEXP;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->mplsCopyEXPOutwards){
		action.act = dpActCpyMplsExpOutwards;
		action.arg = pData->mplsCopyEXPOutwards;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	
	if(pData->remarkTableIndexAction){
		action.act = dpActSetRemarkTableId;
		action.arg = pData->remarkTableIndex;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->mplsTTLAction){
		action.act = dpActSetMplsTtl;
		action.arg = pData->mplsTTL;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->mplsCopyTTLOutwards){
		action.act = dpActCpyMplsTtlOutwards;
		action.arg = pData->mplsCopyTTLOutwards;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->lmepIdAction){
		action.act = dpActSetLmepId;
		action.arg = pData->lmepId;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->oamLmTxCountAction){
		action.act = dpActOamLmTxCount;
		action.arg = 0;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	if(pData->colorBasedCountAction){
		action.act = dpActIncColorBasedCount;
		action.arg = pData->colorBasedCountId;
		DP_ADD_ACTION_TO_BUCKET(*ppBucket,&action);
	}

	return OFDPA_E_NONE;
}


OFDPA_ERROR_t indirectGroupBucketEntryAdd(ofdpaGroupBucketEntry_t *groupBucket)
{
  OFDPA_ERROR_t rv;
  uint32_t subType;
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

	

  pipeInPktSockFd = getIndirectGrpSockFd();
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



static void indirect_grp_pipe_thread_core(void * arg)
{
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_INDIRECT_GRP, &indirect_grp_pipe_config.nodeSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}
	
	OFDPA_INIT_LIST_HEAD(&indirect_grp_pipe_config.entrys);

	while(1){
	
		indirectGrpPipeInPktRecv(NULL);

	}


}

/*****************************************************************************
 Prototype    : indirect_grp_pipe_init
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
int indirect_grp_pipe_init(int argc, char *argv[])
{
	
	indirect_grp_pipe_config.max_entrys = 4094;

	indirect_grp_pipe_config.nodeTid = (pthread_t)dpaThreadCreate("indirectGrpTask", 62, indirect_grp_pipe_thread_core, NULL);


	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/
