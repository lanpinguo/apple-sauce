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
#include "dp_pipe_vlan.h"



ofdpaVlanPipeNodeConfig_t vlan_pipe_config;


int getVlanSockFd(void)
{
	return	vlan_pipe_config.nodeSock;
}






OFDPA_ERROR_t vlanPipeFlowStatsGet(ofdpaFlowEntry_t *flow,ofdpaFlowEntryStats_t *flowStats)
{
	ofdpaVlanPipeNode_t *pNode = NULL;

	

	if(flow == NULL || flowStats == NULL){
		return OFDPA_E_PARAM;
	}

	if(flow->priority >= vlan_pipe_config.max_entrys){
		return OFDPA_E_PARAM;
	}

	pNode = &vlan_pipe_config.entrys[flow->priority];
	if(pNode->valid){
		flowStats->receivedBytes = pNode->recv_bytes;
		flowStats->receivedPackets = pNode->recv_pkts;
	}
	else{
		return OFDPA_E_FAIL;
	}

	return OFDPA_E_NONE;

}


OFDPA_ERROR_t vlanPipeFlowAdd(ofdbVlanFlowTable_node_t *flow_node)
{
	OFDPA_ERROR_t	rv;
	ofdpaVlanPipeNode_t *pNode = NULL;
	uint16_t	vlanId;
	ofdpaVlanFlowEntry_t *flowData;
	ofdpaAct_t						act;
	ofdpaActHolder_t			*pHolder;

	if(flow_node == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Null flow_node passed!\r\n", 0);
		return OFDPA_E_PARAM;
	}

	if(flow_node->priority >= vlan_pipe_config.max_entrys){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Priority is out of range !\r\n", 0);
		return OFDPA_E_PARAM;
	}

	pNode = &vlan_pipe_config.entrys[flow_node->priority];
	if(pNode->valid){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "Node is already exists !\r\n", 0);
		return OFDPA_E_EXISTS;
	}


	/* Start instert*/
	pNode->priority 		= flow_node->priority;
	pNode->hard_time 		= flow_node->hard_time;
	pNode->idle_time 		= flow_node->idle_time;
	pNode->flags				= flow_node->flags;
	pNode->match.inPort = (1<<(flow_node->vlanFlowEntry.match_criteria.inPort - 1));
	vlanId 							= (flow_node->vlanFlowEntry.match_criteria.vlanId) & 0xFFF; 
	pNode->match.vlanId	= REORDER16_L2B(vlanId);
	vlanId 							= flow_node->vlanFlowEntry.match_criteria.vlanIdMask;
	pNode->match.vlanIdMask	= REORDER16_L2B(vlanId);
	pNode->instructions.gotoTableId = flow_node->vlanFlowEntry.gotoTableId;

	/* Apply actions */
	pNode->instructions.apply_action = dpActHolderMalloc(OFDPA_FT_VLAN_APLY_ACT_MAX);
	if(pNode->instructions.apply_action  == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "memory malloc failed!\r\n", 0);
		return OFDPA_E_INTERNAL;
	}

	pHolder = pNode->instructions.apply_action;
	flowData = &flow_node->vlanFlowEntry;
	if (flowData->setVlanIdAction)
	{
		act.act = dpActSetVlanId;
		act.arg = flowData->newVlanId;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);

	}
	if (flowData->popVlanAction)
	{
		act.act = dpActPopVlan;
		act.arg = 0;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}
	if (flowData->pushVlan2Action)
	{
		act.act = dpActPushVlan;
		act.arg = flowData->newTpid2;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);

	}
	if (flowData->setVlanId2Action)
	{
		act.act = dpActSetVlanId2;
		act.arg = flowData->newVlanId2;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}
	if (flowData->ovidAction)
	{
		act.act = dpActSetMetaDataOvid;
		act.arg = flowData->ovid;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}
	if (flowData->vrfAction)
	{
		act.act = dpActSetMetaDataVrf;
		act.arg = flowData->vrf;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}
	if (flowData->mplsL2PortAction)
	{
		act.act = dpActSetMetaDataMplsL2Port;
		act.arg = flowData->mplsL2Port;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}
	if (flowData->tunnelIdAction)
	{
		act.act = dpActSetMetaDataTunId;
		act.arg = flowData->tunnelId;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}
	if (flowData->mplsTypeAction)
	{
		act.act = dpActSetMetaDataMplsType;
		act.arg = flowData->mplsType;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}
	

	/* Write actions*/
	
	pNode->instructions.write_action = dpActSetHolderMalloc(OFDPA_FT_VLAN_WR_ACT_MAX);
	if(pNode->instructions.write_action  == NULL){
	
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "memory malloc failed!\r\n", 0);
		return OFDPA_E_INTERNAL;
	}

	pHolder =	&pNode->instructions.write_action->actHolder;
	if (flowData->classBasedCountAction)
	{
		act.act = dpActIncClassBasedCounter;
		act.arg = flowData->classBasedCountId;
		DP_FT_ADD_ACTION_TO_HOLDER(pHolder,&act);
	}

	pNode->valid = 1;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "vlan pipe flow add here!\r\n", 0);


	return OFDPA_E_NONE;

}




void *findMatchVlanNode(struct ofdpaVlanPipeNodeConfig_s *vlan_pipe_config,ofdpaPktCb_t *pcb)
{
	ofdpaVlanPipeNode_t *pNode = NULL;
	int i;
	struct OFDPA_VLAN *vlan;
	
	vlan = (struct OFDPA_VLAN *)getFeild(pcb, FEILD_VLAN_0);
	if(vlan ==  NULL){
		return NULL;
	}
	
	for(i = 0; i < vlan_pipe_config->max_entrys ; i++){
		pNode = &vlan_pipe_config->entrys[i];

		if(pNode->valid){
		
			if(pNode->match.inPort != pcb->port){
				continue;												 
			}
			
			if((vlan->vid & pNode->match.vlanIdMask) == pNode->match.vlanId){
				return pNode;
			}
			else {
				continue;
			}
		}
	}

	return NULL;

}











OFDPA_ERROR_t  executeActionOnPkt(ofdpaVlanPipeNode_t *pNode,ofdpaPktCb_t *pcb)
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
			ofdpa_list_add(&pActSetHolder->head, &pcb->action_set);
		}
	}

	
	return OFDPA_E_NONE;

}



static OFDPA_ERROR_t vlanPktProcess( ofdpaPktCb_t *pcb)
{
	OFDPA_ERROR_t rv;
	ofdpaPcbMsg_t msg;
	ofdpaVlanPipeNode_t *pNode = NULL;

	//dump_pcb(&pcb);
	//dump_pkt((const char *)pcb.this, pcb.len);
	pNode = findMatchVlanNode(&vlan_pipe_config,pcb);
	if(pNode){
		pNode->recv_pkts++;
		pNode->recv_bytes += pcb->pkt_len;
		//printf("\r\nvlan ft match priority %d\r\n",pNode->priority);
		//printf("\r\nvlan ft recvPkts = %lld, recvBytes = %lld\r\n",pNode->recv_pkts,pNode->recv_bytes);

		executeActionOnPkt(pNode,pcb);

		if(pNode->instructions.gotoTableId < OFDPA_FLOW_TABLE_ID_VLAN){
			/* the last ingress table, process the action set*/
			msg.dstObjectId = OFDPA_ING_ACT_EXECUTOR;
		}
		else{
			msg.dstObjectId = pNode->instructions.gotoTableId;
		}
		msg.pcb = pcb;
		rv = datapathPipeMsgSend(vlan_pipe_config.nodeSock ,&msg);
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



static OFDPA_ERROR_t vlanPipeInPktRecv(struct timeval *timeout)
{
  int pipeInPktSockFd;
  ofdpaPcbMsg_t msg;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

	

  pipeInPktSockFd = getVlanSockFd();
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
										"vlan ft rec %d\r\n",recvBytes);
	vlanPktProcess(msg.pcb);

  return OFDPA_E_NONE;
}



static void vlan_pipe_thread_core(void * arg)
{
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_FLOW_TABLE_ID_VLAN, &vlan_pipe_config.nodeSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}

	while(1){
	
		vlanPipeInPktRecv(NULL);

	}


}

/*****************************************************************************
 Prototype    : vlan_pipe_init
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
int vlan_pipe_init(int argc, char *argv[])
{
	
	vlan_pipe_config.max_entrys = 4094;
	vlan_pipe_config.entrys = calloc(vlan_pipe_config.max_entrys,sizeof(ofdpaVlanPipeNode_t));
	
	if(vlan_pipe_config.entrys == NULL){
		return OFDPA_E_INTERNAL;	
	}

	vlan_pipe_config.nodeTid = (pthread_t)dpaThreadCreate("vlanFT", 62, vlan_pipe_thread_core, NULL);


	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/
