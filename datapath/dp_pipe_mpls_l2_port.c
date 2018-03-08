/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_mpls_l2_port.c
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/2/26
  Last Modified :
  Description   : mpls l2 port flow table instance
  Function List :
              executeMplsL2PortActOnPkt
              mplsL2PortEntryMatchFind
              getMplsL2PortSockFd
              mplsL2PortNodeSocketCreate
              mplsL2PortPipeFlowAdd
              mplsL2PortPipeFlowStatsGet
              mplsL2PortPipeInPktRecv
              mplsL2PortPktProcess
              mplsL2Port_pipe_init
              mplsL2Port_pipe_thread_core
  History       :
  1.Date        : 2018/2/26
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
#include "dp_pipe_mpls_l2_port.h"



ofdpaMplsL2PortPipeNodeConfig_t mplsL2Port_pipe_config;


int getMplsL2PortSockFd(void)
{
	return	mplsL2Port_pipe_config.nodeSock;
}






OFDPA_ERROR_t mplsL2PortPipeFlowStatsGet(ofdpaFlowEntry_t *flow,ofdpaFlowEntryStats_t *flowStats)
{
	ofdpaMplsL2PortPipeNode_t *pNode = NULL;

	

	if(flow == NULL || flowStats == NULL){
		return OFDPA_E_PARAM;
	}

	if(flow->priority >= mplsL2Port_pipe_config.max_entrys){
		return OFDPA_E_PARAM;
	}

	pNode = &mplsL2Port_pipe_config.entrys[flow->priority];
	if(pNode->valid){
		flowStats->receivedBytes = pNode->recv_bytes;
		flowStats->receivedPackets = pNode->recv_pkts;
	}
	else{
		return OFDPA_E_FAIL;
	}

	return OFDPA_E_NONE;

}



/*********************************************************************
*
* @purpose  Get information for the next flow after the specified flow.
*
* @param    flow @b{(input)} Flow information.
* @param    nextFlow @b{(output)} Configured information for next flow.
*
* @returns  OFDPA_E_NONE - Entry found.
* @returns  OFDPA_E_FAIL - Entry is not found.
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t mplsL2PortPipeFlowNextGet(ofdpaFlowEntry_t *flow,ofdpaFlowEntry_t *next)
{
	ofdpaMplsL2PortPipeNode_t *pNode = NULL;
	ofdpaMplsL2PortFlowEntry_t *flowData;
	int i;
	

	if(flow == NULL || next == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null flow_node passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}


	if((flow->priority + 1) >= mplsL2Port_pipe_config.max_entrys){

		return OFDPA_E_FAIL;

	}
	
	for(i = flow->priority + 1; i< mplsL2Port_pipe_config.max_entrys; i++){
		pNode = &mplsL2Port_pipe_config.entrys[i];
		flowData = &next->flowData.mplsL2PortFlowEntry;

		if(pNode->valid){

			/* Start instert*/
			next->priority		= 	pNode->priority; 		
			next->hard_time		= 	pNode->hard_time; 		
			next->idle_time		= 	pNode->idle_time ;		
			next->cookie			= 	pNode->flags;				

			return OFDPA_E_NONE;
		
		}
	}
	return OFDPA_E_FAIL;

}

OFDPA_ERROR_t mplsL2PortPipeFlowAdd(ofdpaFlowEntry_t *flow_node)
{
	OFDPA_ERROR_t	rv;
	ofdpaMplsL2PortPipeNode_t *pNode = NULL;
	ofdpaMplsL2PortFlowEntry_t *flowData;
	ofdpaAct_t						act;
	ofdpaActHolder_t			*pHolder;

	if(flow_node == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null flow_node passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}

	if(flow_node->priority >= mplsL2Port_pipe_config.max_entrys){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Priority is out of range !\r\n", 0);
		return OFDPA_E_PARAM;
	}

	pNode = &mplsL2Port_pipe_config.entrys[flow_node->priority];
	if(pNode->valid){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Node is already exists !\r\n", 0);
		return OFDPA_E_EXISTS;
	}
	
	flowData = &flow_node->flowData.mplsL2PortFlowEntry;


	/* Start instert*/
	pNode->priority 		= flow_node->priority;
	pNode->hard_time 		= flow_node->hard_time;
	pNode->idle_time 		= flow_node->idle_time;
	pNode->flags				= flow_node->cookie;
	pNode->match.key.mplsL2Port 		= flowData->match_criteria.mplsL2Port;
	pNode->match.keyMask.mplsL2Port = flowData->match_criteria.mplsL2PortMask;
	pNode->match.key.etherType			= REORDER16_L2B(flowData->match_criteria.etherType);
	pNode->match.keyMask.etherType	= REORDER16_L2B(flowData->match_criteria.etherTypeMask);
	pNode->match.key.tunnelId				= flowData->match_criteria.tunnelId;
	pNode->match.keyMask.tunnelId		= 0xFFFFFFFF;
	pNode->instructions.gotoTableId = flowData->gotoTableId;

	/* Apply actions */
	pNode->instructions.apply_action = dpActHolderMalloc(OFDPA_FT_MPLS_L2_PORT_APLY_ACT_MAX);
	if(pNode->instructions.apply_action  == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "memory malloc failed!\r\n", 0);
		return OFDPA_E_INTERNAL;
	}

	pHolder = pNode->instructions.apply_action;
	if (flowData->qosIndexAction)
	{
		act.act = ofdpaActSetQosIndex;
		act.arg = flowData->qosIndex;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);

	}


	/* Write actions*/
	
	pNode->instructions.write_action = dpActSetHolderMalloc(OFDPA_FT_MPLS_L2_PORT_WR_ACT_MAX);
	if(pNode->instructions.write_action  == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "memory malloc failed!\r\n", 0);
		return OFDPA_E_INTERNAL;
	}

	pHolder =	&pNode->instructions.write_action->actHolder;
	if (flowData->groupId)
	{
		act.act = ofdpaActSetGrpId;
		act.arg = flowData->groupId;
		rv = dpAddAct2Holder(pHolder,&act);
		if(rv != OFDPA_E_NONE){
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "Add action failed, rv = %d !\r\n", rv);
			return rv;
		}
	}

	pNode->valid = 1;
	mplsL2Port_pipe_config.count++;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "mplsL2Port pipe flow add here!\r\n", 0);


	return OFDPA_E_NONE;

}


OFDPA_ERROR_t mplsL2PortMatchKeyCreate(ofdpaPktCb_t *pcb,ofdpaMplsL2PortMatchKey_t *key)
{
	uint16_t	 *pEtherType;


	if((pcb ==  NULL)||(key ==  NULL)){
		return OFDPA_E_FAIL;
	}

	
	pEtherType = (uint16_t *)getFeild(pcb, FEILD_L3_TYPE);
	if(pEtherType ==  NULL){
		return OFDPA_E_FAIL;
	}

	key->etherType 	= *pEtherType;
	key->mplsL2Port = pcb->meta_data.mplsL2Port;
	key->tunnelId 	= pcb->meta_data.tunnelId;


	return OFDPA_E_NONE;
}


void *mplsL2PortEntryMatchFind(struct ofdpaMplsL2PortPipeNodeConfig_s *mplsL2Port_pipe_config,ofdpaPktCb_t *pcb)
{
	OFDPA_ERROR_t rv;
	ofdpaMplsL2PortPipeNode_t *pNode = NULL;
	ofdpaMplsL2PortMatchKey_t	pktKey = {.pad = {0}};
	uint64_t	*pKey, *pKeyMask, *pPktKey;
	int i,j;
	uint64_t unmatch = 0;


	
	rv = mplsL2PortMatchKeyCreate(pcb,&pktKey);
	if(rv != OFDPA_E_NONE){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "create search key failed!\r\n", 0);
		return OFDPA_E_INTERNAL;
	}

	pPktKey = (uint64_t*)&pktKey;
	
	for(i = 0; i < mplsL2Port_pipe_config->max_entrys ; i++){
		pNode = &mplsL2Port_pipe_config->entrys[i];

		if(pNode->valid){

			pKey 			= (uint64_t*)&pNode->match.key;
			pKeyMask 	= (uint64_t*)&pNode->match.keyMask;
			unmatch 	= 0;
			for(j = 0 ; j < sizeof(ofdpaMplsL2PortMatchKey_t)/sizeof(uint64_t); j++){

				printf("i = %d, j = %d, in = %016x , Key:Mask = %016x:%016x\r\n"
								,i,j,pPktKey[j],pKey[j],pKeyMask[j]);
								
				if((pPktKey[j] & pKeyMask[j]) ^ pKey[j]){
					unmatch = 1;
					break;
				}

			}

			if(unmatch){
				continue;
			}
			
			/* Found match node, return */
			return pNode;
			
		}
	}

	return NULL;

}












OFDPA_ERROR_t  executeMplsL2PortActOnPkt(ofdpaMplsL2PortPipeNode_t *pNode,ofdpaPktCb_t *pcb)
{	
	int i;
	OFDPA_ERROR_t rv;
	ofdpaActSetHolder_t	*pActSetHolder;
	ofdpaActHolder_t			*pHolder; 

	/* apply-action */
	pHolder = pNode->instructions.apply_action;
	if(pHolder){
		for(i = 0; i < pHolder->numAct ; i++){
			if(pHolder->act[i].act == NULL){
				break;
			}
			rv = pHolder->act[i].act(pNode, pcb, pHolder->act[i].arg);
			if(rv != OFDPA_E_NONE){
				OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
													 "execute action faild, index=%d,rv=%d",i,rv);
			}
			/* Update pkt len */
		}
	}
	

	/* write-action */
	pActSetHolder = pNode->instructions.write_action;
	if(pActSetHolder){
		if(pActSetHolder->actHolder.act[0].act){
			/*printf("\r\n head = %p, head->prev = %p , head->next = %p \r\n",
				&pcb->action_set,pcb->action_set.prev,pcb->action_set.next);*/
			ofdpa_list_add_tail(&pActSetHolder->head, &pcb->action_set);
			
			/*printf("\r\n new = %p, new->prev = %p , new->next = %p \r\n",
				&pActSetHolder->head, pActSetHolder->head.prev, &pcb->action_set.next);
			printf("\r\n head = %p, head->prev = %p , head->next = %p \r\n",
				&pcb->action_set,pcb->action_set.prev,pcb->action_set.next);*/
		}
	}

	
	return OFDPA_E_NONE;

}



static OFDPA_ERROR_t mplsL2PortPktProcess( ofdpaPktCb_t *pcb)
{
	OFDPA_ERROR_t rv;
	ofdpaPcbMsg_t msg;
	ofdpaMplsL2PortPipeNode_t *pNode = NULL;

	//dump_pcb(&pcb);
	//dump_pkt((const char *)pcb.this, pcb.len);
	pNode = mplsL2PortEntryMatchFind(&mplsL2Port_pipe_config,pcb);
	if(pNode){
		pNode->recv_pkts++;
		pNode->recv_bytes += pcb->pkt_len;
		//printf("\r\nvlan ft match priority %d\r\n",pNode->priority);
		//printf("\r\nvlan ft recvPkts = %lld, recvBytes = %lld\r\n",pNode->recv_pkts,pNode->recv_bytes);

		executeMplsL2PortActOnPkt(pNode,pcb);

		if(pNode->instructions.gotoTableId < OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT){
			/* the last ingress table, process the action set*/
			msg.dstObjectId = OFDPA_ING_ACT_EXECUTOR;
		}
		else{
			msg.dstObjectId = pNode->instructions.gotoTableId;
		}
		
		msg.pcb = pcb;
		rv = datapathPipeMsgSend(mplsL2Port_pipe_config.nodeSock ,&msg);
		if(rv != OFDPA_E_NONE){
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
							 "Failed to send msg, rv = %d\r\n",rv);
		
			return OFDPA_E_PARAM;
		}

	}
	else {
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
						 "NO MACTCH in talbe [%d]\r\n",OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT);
	}
  return OFDPA_E_NONE;
}



static OFDPA_ERROR_t mplsL2PortPipeInPktRecv(struct timeval *timeout)
{
  int pipeInPktSockFd;
  ofdpaPcbMsg_t msg;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

	

  pipeInPktSockFd = getMplsL2PortSockFd();
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

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
										"mpls l2 port ft rec %d\r\n",recvBytes);
	mplsL2PortPktProcess(msg.pcb);

  return OFDPA_E_NONE;
}



static void mplsL2Port_pipe_thread_core(void * arg)
{
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT, &mplsL2Port_pipe_config.nodeSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}
	

	while(1){
	
		mplsL2PortPipeInPktRecv(NULL);

	}


}


uint32_t mplsL2PortFtMaxCountGet(void)
{
	return mplsL2Port_pipe_config.max_entrys;
}

uint32_t mplsL2PortFtEntryCountGet(void)
{
	return mplsL2Port_pipe_config.count;
}




OFDPA_ERROR_t mplsl2PortFlowEntryPrint(ofdpaFlowEntry_t *flow, ofdpaPrettyPrintBuf_t *buf)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint32_t count = 0, mdlLevel = 0;
	ofdpaMplsL2PortFlowEntry_t *flowData;
	ofdpaMplsL2PortFlowMatch_t *match;



	if((flow == NULL) || (buf == NULL)){
		return OFDPA_E_PARAM;
	}

	flowData = &flow->flowData.mplsL2PortFlowEntry;
	match = &flowData->match_criteria;

	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " mplsL2Port:mask = 0x%08x:0x%08x", match->mplsL2Port, match->mplsL2PortMask);
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " tunnelId = 0x%x", match->tunnelId);

	if (OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST == flowData->gotoTableId)
	{
		APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " etherType:mask = 0x%04x:0x%04x", match->etherType, match->etherTypeMask);
	}

	/* instructions */
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "%s", " |");
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
	/* Apply actions */
	if (0 != flowData->qosIndexAction)
	{
		APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " QOS Index = %d", flowData->qosIndex);
	}
	/* Write actions */
	if (flowData->groupId != 0)
	{
		APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " groupId = 0x%08x", flowData->groupId);
	}
		
	/* configuration data common to all flow entries */
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " | ");
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "priority = %d ", flow->priority);
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "hard_time = %d ", flow->hard_time);
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "idle_time = %d ", flow->idle_time);
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "cookie = %llu", (unsigned long long int)flow->cookie);
		

	buf->len = count;
	
	return(rc);
}



/*****************************************************************************
 Prototype    : mplsL2Port_pipe_init
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
int mplsL2Port_pipe_init(int argc, char *argv[])
{
	OFDPA_ERROR_t rv;
	ofdpaTblPipeNodeOps_t ops;

	
	
	ops.flowAdd 									= mplsL2PortPipeFlowAdd;
	ops.flowNextGet 							= mplsL2PortPipeFlowNextGet;
	ops.flowStatsGet 							= mplsL2PortPipeFlowStatsGet;
	ops.flowTableEntryCountGet		= mplsL2PortFtEntryCountGet;
	ops.flowTableMaxCountGet			= mplsL2PortFtMaxCountGet;
	ops.flowEntryPrint						= mplsl2PortFlowEntryPrint;
	rv = dpFlowTblPipeNodeRegister(OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT, &ops);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to register node %d.\r\n",rv);
		return OFDPA_E_INTERNAL;
	}

	
	mplsL2Port_pipe_config.max_entrys = 4094;
	mplsL2Port_pipe_config.entrys = calloc(mplsL2Port_pipe_config.max_entrys,sizeof(ofdpaMplsL2PortPipeNode_t));
	
	if(mplsL2Port_pipe_config.entrys == NULL){
		return OFDPA_E_INTERNAL;	
	}





	mplsL2Port_pipe_config.nodeTid = (pthread_t)dpaThreadCreate("mplsL2PortFT", 62, mplsL2Port_pipe_thread_core, NULL);


	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/

