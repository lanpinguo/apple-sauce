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
#include "dp_pipe_mpls_pcp_trust.h"



ofdpaMplsPcpTrustPipeNodeConfig_t mpls_pcp_trust_pipe_config;


int getMplsPcpTrustSockFd(void)
{
	return	mpls_pcp_trust_pipe_config.nodeSock;
}






OFDPA_ERROR_t mplsPcpTrustPipeFlowStatsGet(ofdpaFlowEntry_t *flow,ofdpaFlowEntryStats_t *flowStats)
{
	ofdpaMplsPcpTrustPipeNode_t *pNode = NULL;

	

	if(flow == NULL || flowStats == NULL){
		return OFDPA_E_PARAM;
	}

	if(flow->priority >= mpls_pcp_trust_pipe_config.max_entrys){
		return OFDPA_E_PARAM;
	}

	pNode = &mpls_pcp_trust_pipe_config.entrys[flow->priority];
	if(pNode->valid){
		flowStats->receivedBytes = pNode->recv_bytes;
		flowStats->receivedPackets = pNode->recv_pkts;
	}
	else{
		return OFDPA_E_FAIL;
	}

	return OFDPA_E_NONE;

}


OFDPA_ERROR_t mplsPcpTrustPipeFlowAdd(ofdbVlanFlowTable_node_t *flow_node)
{
	return OFDPA_E_NONE;
}




void *findMatchMplsPcpTrustNode(ofdpaMplsPcpTrustPipeNodeConfig_t *mpls_pcp_trust_pipe_config,ofdpaPktCb_t *pcb)
{
	ofdpaMplsPcpTrustPipeNode_t *pNode = NULL;
	int i;
	struct OFDPA_VLAN *vlan;
	
	vlan = (struct OFDPA_VLAN *)getFeild(pcb, FEILD_VLAN_0);
	if(vlan ==  NULL){
		return NULL;
	}
	
	for(i = 0; i < mpls_pcp_trust_pipe_config->max_entrys ; i++){
		pNode = &mpls_pcp_trust_pipe_config->entrys[i];

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



/* socket is created as a blocking socket */
static
int mplsPcpTrustNodeSocketCreate(	ofdpaMplsPcpTrustPipeNodeConfig_t *mpls_pcp_trust_pipe_config)
{
	struct PIPE_ENTRY_ADDR *pipe_entry;

  int sockfd;
  int retval;
  unsigned int rcvSize = (1024 * 256); /* bytes */



	pipe_entry = datapathPipeTransferEntryGet(OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST);

	if(pipe_entry == NULL){
		return -1;
	}
  
  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
    return -1;
  }

  
  unlink((const char *)pipe_entry->sockaddr.sun_path);     /* remove old socket file if it exists */

  retval = bind(sockfd, (const struct sockaddr *)&pipe_entry->sockaddr, pipe_entry->addrlen);
  if (retval < 0)
  {
    close(sockfd);
    return -1;
  }


  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvSize, sizeof(rcvSize)) == -1)
  {
    close(sockfd);
    return -1;
  }

	mpls_pcp_trust_pipe_config->nodeSock = sockfd;

  return sockfd;
}









OFDPA_ERROR_t  executeMplsPcpTrustActionOnPkt(ofdpaMplsPcpTrustPipeNode_t *pNode,ofdpaPktCb_t *pcb)
{	
	int i;
	OFDPA_ERROR_t rv;
	ofdpaActSetHolder_t	*pActSetHolder;
	ofdpaActHolder_t			*pHolder; 
	ofdpaAct_t 	*pAct;
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



static OFDPA_ERROR_t mplsPcpTrustPktProcess( ofdpaPktCb_t *pcb)
{
	OFDPA_ERROR_t rv;
	ofdpaPcbMsg_t msg;
	ofdpaMplsPcpTrustPipeNode_t *pNode = NULL;

	//dump_pcb(&pcb);
	//dump_pkt((const char *)pcb.this, pcb.len);
	pNode = findMatchMplsPcpTrustNode(&mpls_pcp_trust_pipe_config,pcb);
	if(pNode){
		pNode->recv_pkts++;
		pNode->recv_bytes += pcb->pkt_len;
		//printf("\r\nvlan ft match priority %d\r\n",pNode->priority);
		//printf("\r\nvlan ft recvPkts = %lld, recvBytes = %lld\r\n",pNode->recv_pkts,pNode->recv_bytes);

		executeMplsPcpTrustActionOnPkt(pNode,pcb);

		if(pNode->instructions.gotoTableId < OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST){
			/* the last ingress table, process the action set*/
			msg.dstObjectId = OFDPA_ING_ACT_EXECUTOR;
		}
		else{
			msg.dstObjectId = pNode->instructions.gotoTableId;
		}
		
		msg.pcb = pcb;
		rv = datapathPipeMsgSend(mpls_pcp_trust_pipe_config.nodeSock ,&msg);
		if(rv != OFDPA_E_NONE){
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
							 "Failed to send msg, rv = %d\r\n",rv);
		
			return OFDPA_E_PARAM;
		}
	}
	else{
		msg.dstObjectId = OFDPA_ING_ACT_EXECUTOR;
		msg.pcb = pcb;
		rv = datapathPipeMsgSend(mpls_pcp_trust_pipe_config.nodeSock ,&msg);
		if(rv != OFDPA_E_NONE){
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
							 "Failed to send msg, rv = %d\r\n",rv);
		
			return OFDPA_E_PARAM;
		}
	}
  return OFDPA_E_NONE;
}



static OFDPA_ERROR_t mplsPcpTrustPipeInPktRecv(struct timeval *timeout)
{
  int pipeInPktSockFd;
  ofdpaPcbMsg_t msg;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

	

  pipeInPktSockFd = getMplsPcpTrustSockFd();
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
										"mpls pcp trust ft rec %d\r\n",recvBytes);
	mplsPcpTrustPktProcess(msg.pcb);

  return OFDPA_E_NONE;
}



static void mplsPcpTrust_pipe_thread_core(void * arg)
{
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST, &mpls_pcp_trust_pipe_config.nodeSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}


	while(1){
	
		mplsPcpTrustPipeInPktRecv(NULL);

	}


}

/*****************************************************************************
 Prototype    : mplsPcpTrust_pipe_init
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
int mplsPcpTrust_pipe_init(int argc, char *argv[])
{
	
	mpls_pcp_trust_pipe_config.max_entrys = 4094;
	mpls_pcp_trust_pipe_config.entrys = calloc(mpls_pcp_trust_pipe_config.max_entrys,sizeof(ofdpaMplsPcpTrustPipeNode_t));
	
	if(mpls_pcp_trust_pipe_config.entrys == NULL){
		return OFDPA_E_INTERNAL;	
	}

	mpls_pcp_trust_pipe_config.nodeTid = (pthread_t)dpaThreadCreate("mplsPcpTrustFT", 62, mplsPcpTrust_pipe_thread_core, NULL);


	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/
