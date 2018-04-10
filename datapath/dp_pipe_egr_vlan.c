/********************************************************************************/
/*   @@BEGAIN_INTERNAL_LEGAL@@                                          		*/
/*                                                                      		*/
/*                   Copyright(C) Description                           		*/
/*      Pure Science & Technology Development Co.,Ltd. Beijing, China   		*/
/*     Unpublished work-rights reserved under the China Copyright Act.  		*/
/*     Use,duplication, or disclosure by the government is subject to   		*/
/*     restrictions set forth in the CDTT commercial license agreement. 		*/
/*                                                                      		*/
/*   @@END_INTERNAL_LEGAL@@                                             		*/
/***********************************************************************		*/
/*   Filename 	:dp_pipe_vlan.c                                    			    */
/*   Author    	:HuShouqiang                                         			  */
/*   Date       :2018-02-03                                           			*/
/*   Version   	:1.0                                                    		*/
/*   Purpose    :                                                       		*/
/********************************************************************************/
/********************************************************************************/
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
#include "dp_pipe_egr_vlan.h"



ofdpaEgrVlanPipeNodeConfig_t egr_vlan_pipe_config;


int getEgrVlanSockFd(void)
{
	return	egr_vlan_pipe_config.nodeSock;
}






OFDPA_ERROR_t egrVlanPipeFlowStatsGet(ofdpaFlowEntry_t *flow,ofdpaFlowEntryStats_t *flowStats)
{
	ofdpaEgrVlanPipeNode_t *pNode = NULL;

	
	if(flow == NULL || flowStats == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "null paramater %d !\r\n", __LINE__);
		return OFDPA_E_PARAM;
	}

	if(flow->priority >= egr_vlan_pipe_config.max_entrys){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "wrong paramater %d !\r\n", __LINE__);
		return OFDPA_E_PARAM;
	}

	pNode = &egr_vlan_pipe_config.entrys[flow->priority];
	
	if(pNode->valid){
		flowStats->receivedBytes = pNode->recv_bytes;
		flowStats->receivedPackets = pNode->recv_pkts;
		flowStats->durationSec = dpaUpTimeSeconds() - pNode->up_ts;
		
	}
	else{
		return OFDPA_E_FAIL;
	}

	return OFDPA_E_NONE;

}


OFDPA_ERROR_t egrVlanPipeFlowAdd(ofdpaFlowEntry_t *flow_node)
{
	OFDPA_ERROR_t	rv;
	ofdpaEgrVlanPipeNode_t *pNode = NULL;
	uint16_t	vlanId;
	ofdpaEgressVlanFlowEntry_t *flowData;
	ofdpaAct_t						act;
	ofdpaActHolder_t			*pHolder;
	int i;


	
	if(flow_node == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null flow_node passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}

	if(flow_node->priority >= egr_vlan_pipe_config.max_entrys){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Priority is out of range !\r\n", 0);
		return OFDPA_E_PARAM;
	}

	pNode = &egr_vlan_pipe_config.entrys[flow_node->priority];
	if(pNode->valid){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Node is already exists !\r\n", 0);
		return OFDPA_E_EXISTS;
	}


	flowData = &flow_node->flowData.egressVlanFlowEntry;


	/* Start instert*/
	pNode->priority 		= flow_node->priority;
	pNode->hard_time 		= flow_node->hard_time;
	pNode->idle_time 		= flow_node->idle_time;
	pNode->flags				= flow_node->cookie;
	pNode->up_ts				= dpaUpTimeSeconds();
	//pNode->match.key.inPort 		= (1<<(flowData->match_criteria.inPort - 1));
	//pNode->match.keyMask.inPort = OFDPA_FT_VLAN_IN_PORT_MASK;
	vlanId 							= (flowData->match_criteria.vlanId) & 0xFFF; 
	pNode->match.key.vlanId	= REORDER16_L2B(vlanId);
	vlanId 							= flowData->match_criteria.vlanIdMask;
	pNode->match.keyMask.vlanId	= REORDER16_L2B(vlanId);
	pNode->instructions.gotoTableId = flowData->gotoTableId;

	/* Apply actions */
	pNode->instructions.apply_action = dpActHolderMalloc(OFDPA_FT_VLAN_APLY_ACT_MAX);
	if(pNode->instructions.apply_action  == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "memory malloc failed!\r\n", 0);
		return OFDPA_E_INTERNAL;
	}

	pHolder = pNode->instructions.apply_action;

#if 0	
	for(i = 0; i < flowData->apply_cnt; i++){
		act = flowData->apply_actions[i];
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}
#endif	

	/* Write actions*/
	
	pNode->instructions.write_action = dpActSetHolderMalloc(OFDPA_FT_VLAN_WR_ACT_MAX);
	if(pNode->instructions.write_action  == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "memory malloc failed!\r\n", 0);
		return OFDPA_E_INTERNAL;
	}

	pHolder =	&pNode->instructions.write_action->actHolder;

#if 0
	for(i = 0; i < flowData->write_cnt; i++){
		act = flowData->write_actions[i];
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}
#endif
	pNode->valid = 1;
	egr_vlan_pipe_config.count++;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "vlan pipe flow add here!\r\n", 0);


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
OFDPA_ERROR_t egrVlanPipeFlowNextGet(ofdpaFlowEntry_t *flow,ofdpaFlowEntry_t *next)
{
	ofdpaEgrVlanPipeNode_t *pNode = NULL;
	ofdpaEgressVlanFlowEntry_t *flowData;
	int i,j;
	

	if(flow == NULL || next == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null flow_node passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}


	if((flow->priority + 1) >= egr_vlan_pipe_config.max_entrys){

		return OFDPA_E_FAIL;

	}

	for(i = flow->priority + 1 ; i < egr_vlan_pipe_config.max_entrys; i++ ){
	
		pNode = &egr_vlan_pipe_config.entrys[i];
		flowData = &next->flowData.vlanFlowEntry;

		if(pNode->valid){

			/* Start instert*/
			next->priority		= 	pNode->priority; 		
			next->hard_time		= 	pNode->hard_time; 		
			next->idle_time		= 	pNode->idle_time ;		
			next->cookie			= 	pNode->flags;				
			//flowData->match_criteria.inPort 		= pNode->match.key.inPort;
			flowData->match_criteria.vlanId 		= REORDER16_B2L(pNode->match.key.vlanId);
			flowData->match_criteria.vlanIdMask = REORDER16_B2L(pNode->match.keyMask.vlanId);
			flowData->gotoTableId 							= pNode->instructions.gotoTableId ;

#if 0		
			for(j = 0,flowData->apply_cnt = 0; j < pNode->instructions.apply_action->numAct ; j++){
				flowData->apply_actions[j] = pNode->instructions.apply_action->act[j];
				flowData->apply_cnt++;
			}


			for(j = 0 , flowData->write_cnt = 0; j < pNode->instructions.write_action->actHolder.numAct ; j++){
				flowData->write_actions[j] = pNode->instructions.write_action->actHolder.act[j];
				flowData->write_cnt++;
			}
#endif
			return OFDPA_E_NONE;
		}
	}
	return OFDPA_E_FAIL;

}



OFDPA_ERROR_t egrVlanPktKeyCreate(ofdpaPktCb_t *pcb,ofdpaEgrVlanMatchKey_t *key)
{
	uint16_t	 *pEtherType;
	struct OFDPA_VLAN *vlan;
	
	vlan = (struct OFDPA_VLAN *)getFeild(pcb, FEILD_VLAN_0);
	if(vlan ==  NULL){
		return OFDPA_E_PARAM;
	}


	if((pcb ==  NULL)||(key ==  NULL)){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null flow_node passed!\r\n", 0);
		return OFDPA_E_FAIL;
	}

	

	key->inPort = pcb->port;
	key->vlanId = vlan->vid;
	

	return OFDPA_E_NONE;
}





void *egrVlanEntryMatchFind(ofdpaEgrVlanMatchKey_t *searchKey )
{
	int i,j;
	ofdpaEgrVlanPipeNode_t *pNode = NULL;
	uint64_t	*pEntryKey, *pEntryKeyMask, *key;
	uint64_t unmatch = 0;


	
	key = (uint64_t*)searchKey;
	
	for(i = 0; i < egr_vlan_pipe_config.max_entrys ; i++){
		pNode = &egr_vlan_pipe_config.entrys[i];

		if(pNode->valid){

			pEntryKey 			= (uint64_t*)&pNode->match.key;
			pEntryKeyMask 	= (uint64_t*)&pNode->match.keyMask;
			unmatch 	= 0;
			for(j = 0 ; j < sizeof(ofdpaEgrVlanMatchKey_t)/sizeof(uint64_t); j++){

				/*printf("i = %d, j = %d, in = %016llx , Key:Mask = %016llx:%016llx\r\n"
								,i,j,key[j],pEntryKey[j],pEntryKeyMask[j]);*/
								
				if((key[j] & pEntryKeyMask[j]) ^ pEntryKey[j]){
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











OFDPA_ERROR_t  egrVlanExecuteActionOnPkt(ofdpaEgrVlanPipeNode_t *pNode,ofdpaPktCb_t *pcb)
{	
	int i;
	OFDPA_ERROR_t rv;
	ofdpaActSetHolder_t	*pActSetHolder;
	ofdpaActHolder_t			*pHolder; 
	ofdpaAct_t	*pAct;
	ofdpaActArg_t arg = {.type = ACT_OP_TYPE_EXECUTE};
	

	/* apply-action */
	pHolder = pNode->instructions.apply_action;
	if(pHolder){
		for(i = 0; i < pHolder->numAct ; i++){
			pAct = &pHolder->act[i];
			if(pAct->act == NULL){
				break;
			}
			arg.data = pcb;
			rv = pAct->act(pAct,&arg);
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
			ofdpa_list_add(&pActSetHolder->head, &pcb->action_set);
		}
	}

	
	return OFDPA_E_NONE;

}



static OFDPA_ERROR_t egrVlanPktProcess( ofdpaPktCb_t *pcb)
{
	OFDPA_ERROR_t rv;
	ofdpaPcbMsg_t msg;
	ofdpaEgrVlanPipeNode_t *pNode = NULL;
	ofdpaEgrVlanMatchKey_t	pktKey = {.pad = {0}};


	dump_pkt(pcb->this, pcb->len);
	dump_pcb(pcb);


	rv = egrVlanPktKeyCreate(pcb,&pktKey);
	if(rv != OFDPA_E_NONE){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "create search key failed!\r\n", 0);
		return OFDPA_E_INTERNAL;
	}

	pNode = egrVlanEntryMatchFind(&pktKey);
	if(pNode){
		pNode->recv_pkts++;
		pNode->recv_bytes += pcb->pkt_len;
		//printf("\r\nvlan ft match priority %d\r\n",pNode->priority);
		//printf("\r\nvlan ft recvPkts = %lld, recvBytes = %lld\r\n",pNode->recv_pkts,pNode->recv_bytes);

		egrVlanExecuteActionOnPkt(pNode,pcb);

		if(pNode->instructions.gotoTableId < OFDPA_FLOW_TABLE_ID_VLAN){
			/* the last ingress table, process the action set*/
			msg.dstObjectId = OFDPA_ING_ACT_EXECUTOR;
		}
		else{
			msg.dstObjectId = pNode->instructions.gotoTableId;
		}
		msg.pcb = pcb;
		rv = datapathPipeMsgSend(egr_vlan_pipe_config.nodeSock ,&msg);
		if(rv != OFDPA_E_NONE){
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
							 "Failed to send msg, rv = %d\r\n",rv);
		
			return OFDPA_E_PARAM;
		}

	}
	else {
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
						 "NO MACTCH in talbe [%d]\r\n",OFDPA_FLOW_TABLE_ID_VLAN);
	}
  return OFDPA_E_NONE;
}



static OFDPA_ERROR_t egrVlanPipeInPktRecv(struct timeval *timeout)
{
  int pipeInPktSockFd;
  ofdpaPcbMsg_t msg;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

	

  pipeInPktSockFd = getEgrVlanSockFd();
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
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
                      "Failed to receive packet. recvfrom() returned %d. errno %s.\r\n",
                      recvBytes, strerror(errno));
    return OFDPA_E_FAIL;
  }

	
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
										"egress vlan ft rec %d\r\n",recvBytes);
	egrVlanPktProcess(msg.pcb);

  return OFDPA_E_NONE;
}



static void egr_vlan_pipe_thread_core(void * arg)
{
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_FLOW_TABLE_ID_EGRESS_VLAN, &egr_vlan_pipe_config.nodeSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}

	while(1){
	
		egrVlanPipeInPktRecv(NULL);

	}


}



uint32_t egrVlanFtMaxCountGet(void)
{
	return egr_vlan_pipe_config.max_entrys;
}

uint32_t egrVlanFtEntryCountGet(void)
{
	return egr_vlan_pipe_config.count;
}








OFDPA_ERROR_t egrVlanFlowEntryPrint(ofdpaFlowEntry_t *flow, ofdpaPrettyPrintBuf_t *buf)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint32_t count = 0, mdlLevel = 0;
	ofdpaEgressVlanFlowEntry_t *flowData;
	ofdpaEgressVlanFlowMatch_t *match;
	int i;
	ofdpaActPrintBuf_t	ops;
	ofdpaActArg_t	arg = {.type = ACT_OP_TYPE_PRETTY_PRINT};
	ofdpaAct_t *pAct;

	static char *mplsTypeSubTypeName[] =
	{
		[OFDPA_MPLS_TYPE_VPWS 				] = "VPWS",
		[OFDPA_MPLS_TYPE_VPLS 				] = "VPLS",
		[OFDPA_MPLS_TYPE_OAM					] = "OAM",
		[OFDPA_MPLS_TYPE_L3_UNICAST 	] = "L3 Route Unicast",
		[OFDPA_MPLS_TYPE_L3_MULTICAST ] = "L3 Route Multicast",
		[OFDPA_MPLS_TYPE_L3_PHP 			] = "L3 PHP",
	};



	if((flow == NULL) || (buf == NULL)){
		return OFDPA_E_PARAM;
	}


	
	flowData = &flow->flowData.vlanFlowEntry;
	match = &flowData->match_criteria;

	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " inPort = ");
	//ofdpaPortDecode(buf->data, OFDPA_PRETTY_MAX_LEN, &count, match->inPort);
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " vlanId:mask = 0x%04x:0x%04x (VLAN %d)", match->vlanId, match->vlanIdMask, match->vlanId & OFDPA_VID_EXACT_MASK);

	/* instructions */
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " |");
	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

#if 0
	for(i = 0; i < flowData->apply_cnt ; i ++){
		if (count < OFDPA_PRETTY_MAX_LEN) 																														 
		{ 
			pAct = &flowData->apply_actions[i];
			if(pAct->act){
				ops.buf = &buf->data[count];
				ops.bufSize = OFDPA_PRETTY_MAX_LEN - count;
				arg.data = &ops;
				count += pAct->act(pAct,&arg);
			}
		} 																																											 
		if (count >= OFDPA_PRETTY_MAX_LEN)																														 
		{ 																																											 
			buf->data[OFDPA_PRETTY_MAX_LEN - 1] = '\0'; 																												
			return OFDPA_E_FULL;																																	 
		} 																																											 
	}
	
	for(i = 0; i < flowData->write_cnt ; i ++){
		if (count < OFDPA_PRETTY_MAX_LEN) 																														 
		{ 
			pAct = &flowData->write_actions[i];
			if(pAct->act){
				ops.buf = &buf->data[count];
				ops.bufSize = OFDPA_PRETTY_MAX_LEN - count;
				arg.data = &ops;
				count += pAct->act(pAct,&arg);
			}
		} 																																											 
		if (count >= OFDPA_PRETTY_MAX_LEN)																														 
		{ 																																											 
			buf->data[OFDPA_PRETTY_MAX_LEN - 1] = '\0'; 																												
			return OFDPA_E_FULL;																																	 
		} 																																											 
	}

#endif
		
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
 Prototype    : egr_vlan_pipe_init
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
int egr_vlan_pipe_init(int argc, char *argv[])
{
	OFDPA_ERROR_t rv;
	ofdpaTblPipeNodeOps_t ops;

	

	ops.flowAdd 									= egrVlanPipeFlowAdd;
	ops.flowNextGet 							= egrVlanPipeFlowNextGet;
	ops.flowStatsGet 							= egrVlanPipeFlowStatsGet;
	ops.flowTableEntryCountGet		= egrVlanFtEntryCountGet;
	ops.flowTableMaxCountGet			= egrVlanFtMaxCountGet;
	ops.flowEntryPrint						= egrVlanFlowEntryPrint;
	rv = dpFlowTblPipeNodeRegister(OFDPA_FLOW_TABLE_ID_EGRESS_VLAN, &ops);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to register node %d.\r\n",rv);
		return OFDPA_E_INTERNAL;
	}

	egr_vlan_pipe_config.max_entrys = 4094;
	egr_vlan_pipe_config.entrys = calloc(egr_vlan_pipe_config.max_entrys,sizeof(ofdpaEgrVlanPipeNode_t));
	
	if(egr_vlan_pipe_config.entrys == NULL){
		return OFDPA_E_INTERNAL;	
	}


	egr_vlan_pipe_config.nodeTid = (pthread_t)dpaThreadCreate("egrVlanFT", 62, egr_vlan_pipe_thread_core, NULL);


	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/
