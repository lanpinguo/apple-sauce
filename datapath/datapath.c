/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename     datapath.c
*
* @purpose      OF-DPA Datapath Manager Task
*
* @component    openflow
*
* @comments     none
*
* @create       04/04/2013
*
* @end
*
**********************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include "ofdpa_datatypes.h"
#include "ofdpa_util.h"
#include "ofdb_api.h"
#include "datapath.h"
#include <net/ethernet.h>
#include "port_manager.h"


int eventSocketFd = 0;

OFDPA_FLOW_TABLE_ID_t datapathFlowTableAgeList[] = 
{
  OFDPA_FLOW_TABLE_ID_INGRESS_PORT,
  OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST,
  OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST,
  OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST,
  OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST,
  OFDPA_FLOW_TABLE_ID_INJECTED_OAM,
  OFDPA_FLOW_TABLE_ID_VLAN,
  OFDPA_FLOW_TABLE_ID_VLAN_1,
  OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT,
  OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT,
  OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST,
  OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST,
  OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS,
  OFDPA_FLOW_TABLE_ID_L2_POLICER,
  OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS,
  OFDPA_FLOW_TABLE_ID_TERMINATION_MAC,
  OFDPA_FLOW_TABLE_ID_MPLS_0,
  OFDPA_FLOW_TABLE_ID_MPLS_1,
  OFDPA_FLOW_TABLE_ID_MPLS_2,
  OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT,
  OFDPA_FLOW_TABLE_ID_MPLS_L3_TYPE,
  OFDPA_FLOW_TABLE_ID_MPLS_TYPE,
  OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING,
  OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING,
  OFDPA_FLOW_TABLE_ID_BRIDGING,
  OFDPA_FLOW_TABLE_ID_ACL_POLICY,
  OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS,
  OFDPA_FLOW_TABLE_ID_EGRESS_VLAN,
  OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1,
  OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT,
  OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK,
  OFDPA_FLOW_TABLE_ID_EGRESS_TPID,
}; 

#define DATAPATH_FLOW_TABLE_AGE_LIST_NUM_ENTRIES  (sizeof(datapathFlowTableAgeList)/sizeof(OFDPA_FLOW_TABLE_ID_t))


struct PIPE_ENTRY_ADDR pipe_entrys[] = {
	{.key = OFDPA_PORT_MANAGER_ID , 						.sockPath = "/tmp/datapath_port_manager"},
	{.key = OFDPA_ING_ACT_EXECUTOR , 						.sockPath = "/tmp/datapath_ing_act_executor"},
	{.key = OFDPA_EGR_ACT_EXECUTOR , 						.sockPath = "/tmp/datapath_egr_act_executor"},
	{.key = OFDPA_FLOW_TABLE_ID_INGRESS_PORT , 	.sockPath = "/tmp/datapath_port_table"},
	{.key = OFDPA_FLOW_TABLE_ID_VLAN , 					.sockPath = "/tmp/datapath_vlan_table"},
	{.key = OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT ,	.sockPath = "/tmp/datapath_mpls_l2_port_table"},
	{.key = OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST ,.sockPath = "/tmp/datapath_mpls_pcp_trust_table"},
	{.key = OFDPA_INDIRECT_GRP					 ,			.sockPath = "/tmp/datapath_indirect_grp"},
	{.key = OFDPA_ALL_GRP ,											.sockPath = "/tmp/datapath_all_grp"},
	{.key = OFDPA_SELECT_GRP 				 ,					.sockPath = "/tmp/datapath_select_grp"},
	{.key = OFDPA_FAST_FAILOVER_GRP , 					.sockPath = "/tmp/datapath_fast_failover_grp"},
	{.key = OFDPA_INVALID_KEY , 								.sockPath = "INVALID"},
				
};


ofdpaTblPipeNode_t pipe_tbl_nodes[256];



 void dump_pcb(ofdpaPktCb_t *pcb)
{
	char *feild_name[] = {
		"FEILD_DMAC",	
		"FEILD_SMAC",			
		"FEILD_VLAN_0",
		"FEILD_VLAN_1",
		"FEILD_L3_TYPE",	
		"FEILD_MPLS_0",		
		"FEILD_MPLS_1",		
		"FEILD_MPLS_2",		
		"FEILD_CW", 			
		"FEILD_DIP", 			
		"FEILD_SIP", 			
		"FEILD_L4_TYPE",	
		"FEILD_L4_DP", 		
		"FEILD_L4_SP", 		
		"FEILD_DATA", 	
		"FEILD_MAX",
	};

	char *format = "\r\n %-15s = %d";
	int i = 0;

	

	printf("\r\n %-15s = %016llx","port",pcb->port);
	for(i = 0; i <= FEILD_MAX; i++){	
		printf(format,feild_name[i],\
			(pcb->feilds[i].offset >= RESERVED_BLOCK_SIZE) ? \
			(pcb->feilds[i].offset - RESERVED_BLOCK_SIZE) : pcb->feilds[i].offset);
	}
}


 
 
 
 
 /* Check the payload of the packet for errors (use it for debug).
	* Look for consecutive ascii representations of the size of the packet.
	*/
 void
 dump_pkt(const char *_p, int len)
 {
	 char buf[128];
	 int i, j, i0;
	 const unsigned char *p = (const unsigned char *)_p;
 
	 /* hexdump routine */
	 for (i = 0; i < len; ) {
		 memset(buf, sizeof(buf), ' ');
		 sprintf(buf, "%5d: ", i);
		 i0 = i;
		 for (j=0; j < 16 && i < len; i++, j++)
			 sprintf(buf+7+j*3, "%02x ", (uint8_t)(p[i]));
		 i = i0;
		 for (j=0; j < 16 && i < len; i++, j++)
			 sprintf(buf+7+j + 48, "%c",
				 isprint(p[i]) ? p[i] : '.');
		 printf("%s\n", buf);
	 }
 }


 
 
 OFDPA_ERROR_t dpAddAct2Holder(ofdpaActHolder_t * pHolder , ofdpaAct_t *pAct)
 {
	 int i;
 
	 
	 if(pHolder == NULL || pAct == NULL){
	 
		 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												"Null pBukt or pAct passed!\r\n", 0);
		 return OFDPA_E_PARAM;
	 }
 
	 for(i = 0; i < pHolder->maxNum ; i++){
		 if(pHolder->act[i].act){
			 continue;
		 }
		 pHolder->act[i] = *pAct;
		 pHolder->numAct++;
		 return OFDPA_E_NONE;
	 }
	 return OFDPA_E_FULL;  
 
 }
 
 ofdpaActHolder_t *dpActHolderMalloc(uint32_t numAct)
 {
	 ofdpaActHolder_t * pHolder = NULL;
 
 
	 pHolder = calloc(1,sizeof(ofdpaActHolder_t) + numAct * sizeof(ofdpaAct_t));
 
	 if(pHolder){
		 pHolder->maxNum = numAct;
		 pHolder->numAct = 0;
	 }
 
	 return pHolder;
 }
 
 ofdpaActSetHolder_t *dpActSetHolderMalloc(uint32_t numAct)
 {
	 ofdpaActSetHolder_t * pHolder = NULL;
 
 
	 pHolder = calloc(1,sizeof(ofdpaActSetHolder_t) + numAct * sizeof(ofdpaAct_t));
 
	 if(pHolder){
		 pHolder->actHolder.maxNum = numAct;
		 pHolder->actHolder.numAct = 0;
	 }
 
	 return pHolder;
 }

 
 
 
OFDPA_ERROR_t dpBindBukt2Grp(ofdpaGrpPipeNode_t *pGrp, ofdpaActBucket_t *pBukt)
{
	uint32_t index;

	if(pGrp == NULL || pBukt == NULL){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											"Null pointer passed, pGrp = %p, pBukt = %p!\r\n", pGrp,pBukt);
		return OFDPA_E_PARAM;
	}


	for(index = 0; index < pGrp->numActBukt; index++){
		if(pGrp->actBukts[index]){
			continue;
		}
		pGrp->actBukts[index] = pBukt;
		return OFDPA_E_NONE;
	}
	return OFDPA_E_FULL;
}
 
 
 
 
ofdpaGrpPipeNode_t *dpGrpNodeMalloc(ofdpaGrpType_e type, uint32_t numActBukt)
{
	ofdpaGrpPipeNode_t *pNode = NULL;

	pNode = (ofdpaGrpPipeNode_t *)calloc(1,sizeof(ofdpaGrpPipeNode_t) + numActBukt * sizeof(ofdpaActBucket_t *));
	if(pNode){
		pNode->this				 = (void*)pNode;
		pNode->grpType 		 = type;
		pNode->numActBukt = numActBukt;
	}

	return pNode;
}
 
 OFDPA_ERROR_t dpAddAct2Bukt(ofdpaActBucket_t * pBukt , ofdpaAct_t *pAct)
 {
	 int i;
 
	 
	 if(pBukt == NULL || pAct == NULL){
	 
		 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												"Null pBukt or pAct passed!\r\n", 0);
		 return OFDPA_E_PARAM;
	 }
 
	 for(i = 0; i < pBukt->maxNum; i++){
		 if(pBukt->act[i].act){
			 continue;
		 }
		 pBukt->act[i] = *pAct;
		 pBukt->numAct++;
		 return OFDPA_E_NONE;
	 }
	 return OFDPA_E_FULL;  
 
 }
 
 
 
 ofdpaActBucket_t *dpGrpBucketMalloc(uint32_t numAct)
 {
	 ofdpaActBucket_t * pBukt = NULL;
 
 
	 pBukt = calloc(1,sizeof(ofdpaActBucket_t) + numAct * sizeof(ofdpaAct_t));
 
	 if(pBukt){
		 pBukt->maxNum = numAct;
		 pBukt->numAct = 0;
	 }
 
	 return pBukt;
 }


/*********************************************************************
* @purpose  Create a socket to be used to send event
*           notifications to the client.
*
* @param    void
*
* @returns  OFDPA_E_NONE if socket created successfully
*           OFDPA_E_FAIL otherwise
*
* @notes
*
* @end
*********************************************************************/
int datapathEventSocketCreate(void)
{
  struct sockaddr_un  eventsockaddr;
  int  addrlen, sockfd;
  int  retval;
  int  nonblocking = 1;
  unsigned int sndSize = (1024 * 1024); /* bytes */

  if (0 > (sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                       "Failed to create UNIX datagram socket for OFDPA server. Error %d (%s).",
                       sockfd, strerror(errno));
    return OFDPA_E_FAIL;
  }



  /* Socket is nonblocking. If socket send buffer is full, we immediately drop the message. */
  retval = ioctl(sockfd, FIONBIO, (void *)&nonblocking);
  if (retval < 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                       "Failed to set server socket to non-blocking. Error %d (%s).",
                       errno, strerror(errno));
    close(sockfd);
    return OFDPA_E_FAIL;
  }

  memset(&eventsockaddr, 0, sizeof(eventsockaddr));
  eventsockaddr.sun_family = AF_UNIX;
  snprintf(eventsockaddr.sun_path, sizeof(eventsockaddr.sun_path) - 1, OFDPA_EVENT_SERVER_ADDR);
  addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(eventsockaddr.sun_path);
  unlink((const char *)eventsockaddr.sun_path);     /* remove old socket file if it exists */
  retval = bind(sockfd, (const struct sockaddr *)&eventsockaddr, addrlen);
  if (retval < 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                       "Failed to bind UNIX socket with fd %d to %s. Error %d (%s).",
                       sockfd, eventsockaddr.sun_path, errno, strerror(errno));
    close(sockfd);
    return OFDPA_E_FAIL;
  }

  eventSocketFd = sockfd;

  

  retval = setsockopt(sockfd, SOL_SOCKET,SO_SNDBUF,&sndSize, sizeof(sndSize));
  if (retval < 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                       "Failed to set server socket send buffer to 1m . Error %d (%s).",
                       errno, strerror(errno));
  }
  


  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Send an event notification to the client.
*
* @returns  OFDPA_E_NONE if message sent.
*           OFDPA_E_FAIL if message not sent or partially sent.
*
* @notes    The socket is a nonblocking socket.
*
* @end
*********************************************************************/
OFDPA_ERROR_t datapathEventNotificationSend(dpaEventMsg_t *pMsg)
{
  uint32_t  addrlen;
  ssize_t   bytesSent;
  struct sockaddr_un  sockaddr;
  int flags = 0;
  dpaEventMsg_t msg;
  ssize_t msgLen;

  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sun_family = AF_UNIX;
  snprintf(sockaddr.sun_path, sizeof(sockaddr.sun_path) - 1, OFDPA_EVENT_CLIENT_ADDR);
  addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(sockaddr.sun_path);

  msgLen = sizeof(msg);
  
  if(pMsg == NULL)
  {
    memset(&msg, 0, sizeof(msg));
    msg.msgType = DPA_EVENT_NOTIFICATION_MSG;
  }
  else
  {
    memcpy(&msg, pMsg, sizeof(msg));
  }

  if(eventSocketFd == 0)
  {
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
						 "Failed to send event notification to client on "
						 "socket with fd %d. msg %d.\r\n",
						 eventSocketFd, msg.msgType);
	  return OFDPA_E_FAIL;

  }
  
  bytesSent = sendto(eventSocketFd, &msg, msgLen, flags, (struct sockaddr*) &sockaddr, addrlen);

  if (bytesSent != msgLen)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
                       "Failed to send event notification to client on "
                       "socket with fd %d. Error (%d, %s).",
                       eventSocketFd, errno, strerror(errno));
    return OFDPA_E_FAIL;
  }

  return OFDPA_E_NONE;
}

int datapathFlowsAge(uint32_t currentUpTime, uint32_t flowLimit)
{
  return 0;
}

/*********************************************************************
*
* @purpose  The OF-DPA Datapath Manager Task.
*
* @returns  void
*
* @comments none
*
* @end
*
*********************************************************************/
void datapathTask(void *arg)
{

}


int generateSockUnAddr(struct sockaddr_un * sockaddr,char * path)
{
  uint32_t  addrlen;

  memset(sockaddr, 0, sizeof(struct sockaddr_un));
  sockaddr->sun_family = AF_UNIX;
  snprintf(sockaddr->sun_path, sizeof(sockaddr->sun_path) - 1, path);
  addrlen = offsetof(struct sockaddr_un, sun_path) + strlen(sockaddr->sun_path);

	return addrlen;

}



/*********************************************************************
* @purpose  Create a socket to be used to send event
*           notifications to the client.
*
* @param    void
*
* @returns  OFDPA_E_NONE if socket created successfully
*           OFDPA_E_FAIL otherwise
*
* @notes
*
* @end
*********************************************************************/
int dpPipeNodeSocketsAddrCreate(void)
{
	uint32_t 	i;


	for(i = 0; i < sizeof(pipe_entrys)/sizeof(struct PIPE_ENTRY_ADDR); i++){
		if(pipe_entrys[i].key == OFDPA_INVALID_KEY){
			break;
		}
		pipe_entrys[i].addrlen = generateSockUnAddr(&pipe_entrys[i].sockaddr, pipe_entrys[i].sockPath);

	}

	return 0;
}


struct PIPE_ENTRY_ADDR* datapathPipeTransferEntryGet(uint64_t key)
{
	uint32_t 	i;
	
	for(i = 0; i < sizeof(pipe_entrys)/sizeof(struct PIPE_ENTRY_ADDR); i++){
		if(pipe_entrys[i].key == key){
			return &pipe_entrys[i];
		}
	}

	return NULL;

}


/* socket is created as a blocking socket */
OFDPA_ERROR_t  dpPipeNodeSocketCreate(	uint64_t nodeKey , int *sockId)
{
	struct PIPE_ENTRY_ADDR *pipe_entry;

  int sockfd;
  int retval;
  unsigned int rcvSize = (1024 * 256); /* bytes */



	pipe_entry = datapathPipeTransferEntryGet(nodeKey);

	if(pipe_entry == NULL){
		return OFDPA_E_PARAM;
	}
  
  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sockfd < 0)
  {
    return OFDPA_E_UNAVAIL;
  }

  
  unlink((const char *)pipe_entry->sockaddr.sun_path);     /* remove old socket file if it exists */

  retval = bind(sockfd, (const struct sockaddr *)&pipe_entry->sockaddr, pipe_entry->addrlen);
  if (retval < 0)
  {
    close(sockfd);
    return OFDPA_E_INTERNAL;
  }


  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvSize, sizeof(rcvSize)) == -1)
  {
    close(sockfd);
    return OFDPA_E_INTERNAL;
  }

	*sockId = sockfd;

  return OFDPA_E_NONE;
}





/*********************************************************************
* @purpose  Send an pkt to the next pipe.
*
* @returns  OFDPA_E_NONE if message sent.
*           OFDPA_E_FAIL if message not sent or partially sent.
*
* @notes    The socket is a nonblocking socket.
*
* @end
*********************************************************************/
OFDPA_ERROR_t datapathPipeMsgSend(int srcSockFd, ofdpaPcbMsg_t *msg)
{
	struct PIPE_ENTRY_ADDR *pipe_entry;
  ssize_t   bytesSent;
  int flags = 0;

  ssize_t msgLen;


	if(msg == NULL){
		return OFDPA_E_PARAM;
	}

	pipe_entry = datapathPipeTransferEntryGet(msg->dstObjectId);

	if(pipe_entry == NULL){
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
						 "Failed to get target object fd "
						 "with dstObjectId = 0x%016llx. \r\n",msg->dstObjectId);

		return OFDPA_E_PARAM;
	}


	
  msgLen = sizeof(ofdpaPcbMsg_t);
  

  if(srcSockFd == 0)
  {
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
						 "Failed to send pkt cb info to port ft on "
						 "socket with fd %d. \r\n",
						 srcSockFd);
	  return OFDPA_E_FAIL;

  }
  
  bytesSent = sendto(srcSockFd, msg, msgLen, flags, (struct sockaddr*) &pipe_entry->sockaddr, pipe_entry->addrlen);

  if (bytesSent != msgLen)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
                       "Failed to send pkt cb msg to client on "
                       "socket with fd %d. Error (%d, %s).",
                       srcSockFd, errno, strerror(errno));
    return OFDPA_E_FAIL;
  }

  return OFDPA_E_NONE;
}


uint32_t dpFlowTableSupported(OFDPA_FLOW_TABLE_ID_t tableId)
{
  uint32_t valid = 1;

	if (tableId >= OFDPA_FLOW_TABLE_ID_END){
		return OFDPA_E_PARAM;	
	}


  if(pipe_tbl_nodes[tableId].valid){
		valid = pipe_tbl_nodes[tableId].valid;
	}

  return(valid);
}


uint32_t dpFlowTableMaxCountGet(OFDPA_FLOW_TABLE_ID_t tableId)
{
  uint32_t value = 0;

	if (tableId >= OFDPA_FLOW_TABLE_ID_END){
		return OFDPA_E_PARAM;	
	}


  if(pipe_tbl_nodes[tableId].ops.flowTableMaxCountGet){
		value = pipe_tbl_nodes[tableId].ops.flowTableMaxCountGet();
	}
	
  return(value);

}

uint32_t dpFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_t tableId)
{
  uint32_t value = 0;

	if (tableId >= OFDPA_FLOW_TABLE_ID_END){
		return OFDPA_E_PARAM;	
	}


  if(pipe_tbl_nodes[tableId].ops.flowTableEntryCountGet){
		value = pipe_tbl_nodes[tableId].ops.flowTableEntryCountGet();
	}
  return(value);
}





OFDPA_ERROR_t dpFlowAdd(ofdpaFlowEntry_t *flow)
{
	OFDPA_ERROR_t rc = OFDPA_E_NONE;



	if (flow == NULL)
	{
		return OFDPA_E_PARAM;
	}

	if (flow->tableId >= OFDPA_FLOW_TABLE_ID_END){
		return OFDPA_E_PARAM;	
	}


  if(pipe_tbl_nodes[flow->tableId].ops.flowAdd){
		rc = pipe_tbl_nodes[flow->tableId].ops.flowAdd(flow);
	}


	return(rc);
}




OFDPA_ERROR_t dpFlowNextGet(ofdpaFlowEntry_t *flow, ofdpaFlowEntry_t *nextFlow)
{
	OFDPA_ERROR_t rc = OFDPA_E_FAIL;
	ofdbFlowStatus_t flow_status;

	if ((flow == NULL) ||
			(nextFlow == NULL))
	{
		return OFDPA_E_PARAM;
	}
			
	if (flow->tableId >= OFDPA_FLOW_TABLE_ID_END){
		return OFDPA_E_PARAM; 
	}
	
	
	if(pipe_tbl_nodes[flow->tableId].ops.flowNextGet){
		rc = pipe_tbl_nodes[flow->tableId].ops.flowNextGet(flow,nextFlow);
	}
	
	return(rc);
}



OFDPA_ERROR_t dpFlowStatsGet(ofdpaFlowEntry_t *flow, ofdpaFlowEntryStats_t *flowStats)
{
	OFDPA_ERROR_t rc;

	if ((flow == NULL) ||
			(flowStats == NULL))
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "null pointer %d !\r\n", __LINE__);
		return OFDPA_E_PARAM;
	}

	if (flow->tableId >= OFDPA_FLOW_TABLE_ID_END){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "wrong paramater %d !\r\n", __LINE__);
		return OFDPA_E_PARAM; 
	}
	
	
	if(pipe_tbl_nodes[flow->tableId].ops.flowStatsGet){
		rc = pipe_tbl_nodes[flow->tableId].ops.flowStatsGet(flow,flowStats);
	}



	
	return(rc);
}



uint32_t dpFlowVlanEntryValidate(ofdpaVlanFlowEntry_t *flowData)
{
  /* if you get here, you are valid */
  return(1);
}






OFDPA_ERROR_t dpFlowEntryPrint(ofdpaFlowEntry_t *flow, ofdpaPrettyPrintBuf_t *buf)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;



	if((flow == NULL) || (buf == NULL)){
		return OFDPA_E_PARAM;
	}

	if (flow->tableId >= OFDPA_FLOW_TABLE_ID_END){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
											 "wrong paramater %d !\r\n", __LINE__);
		return OFDPA_E_PARAM; 
	}

	if(pipe_tbl_nodes[flow->tableId].valid){

		if(pipe_tbl_nodes[flow->tableId].ops.flowEntryPrint){
			rc = pipe_tbl_nodes[flow->tableId].ops.flowEntryPrint(flow,buf);
		}

		return(rc);
	}

	APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, buf->len, "Unable to decode flow entry for table ID %d ", flow->tableId);
	return OFDPA_E_NOT_FOUND;


}




OFDPA_ERROR_t dpGroupBucketEntryPrint(ofdpaGroupBucketEntry_t *bucketEntry, ofdpaPrettyPrintBuf_t *buf)
{
#if 1
  OFDPA_ERROR_t rc;
  OFDPA_GROUP_ENTRY_TYPE_t groupType;
  uint32_t subType;
  uint32_t count = 0;

  rc = ofdpaGroupTypeGet(bucketEntry->groupId, &groupType);
  if (rc != OFDPA_E_NONE)
  {
    rc = OFDPA_E_ERROR;
  }
  else
  {
    APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "bucketIndex = %d: ", bucketEntry->bucketIndex);

    switch (groupType)
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "outputPort = ");
        ofdpaPortDecode(buf->data, OFDPA_PRETTY_MAX_LEN, &count, bucketEntry->bucketData.l2Interface.outputPort);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "popVlanTag = %d ", bucketEntry->bucketData.l2Interface.popVlanTag);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "allowVlanTranslation = %d ", bucketEntry->bucketData.l2Interface.allowVlanTranslation);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "outputPort = ");
        ofdpaPortDecode(buf->data, OFDPA_PRETTY_MAX_LEN, &count, bucketEntry->bucketData.l2UnfilteredInterface.outputPort);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "allowVlanTranslation = %d ", bucketEntry->bucketData.l2UnfilteredInterface.allowVlanTranslation);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.l3Interface.vlanId, bucketEntry->bucketData.l3Interface.vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "srcMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[0],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[1],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[2],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[3],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[4],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[5]);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.l3Unicast.vlanId, bucketEntry->bucketData.l3Unicast.vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "srcMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[0],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[1],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[2],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[3],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[4],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[5]);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "dstMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[0],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[1],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[2],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[3],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[4],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[5]);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.l2Rewrite.vlanId, bucketEntry->bucketData.l2Rewrite.vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "srcMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[0],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[1],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[2],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[3],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[4],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[5]);
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "dstMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[0],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[1],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[2],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[3],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[4],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[5]);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "outputPort = ");
        ofdpaPortDecode(buf->data, OFDPA_PRETTY_MAX_LEN, &count, bucketEntry->bucketData.l2Overlay.outputPort);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
      case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
        rc = ofdpaGroupMplsSubTypeGet(bucketEntry->groupId, &subType);
        if (rc != OFDPA_E_NONE)
        {
          rc = OFDPA_E_ERROR;
        }
        else
        {
          switch (subType)
          {
            case OFDPA_MPLS_INTERFACE:
              {
              	int i;
              	ofdpaAct_t	*pAct;
								ofdpaActionFuncOpt_t	ops;
								
	              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
	              
								for(i = 0; i < bucketEntry->bucketData.mplsInterface.act_cnt ; i ++){
									pAct = &bucketEntry->bucketData.mplsInterface.actions[i];
									if (count < OFDPA_PRETTY_MAX_LEN) 																														 
									{ 
										if(pAct->act){
											ops.buf = &buf->data[count];
											ops.bufSize = OFDPA_PRETTY_MAX_LEN - count;
											count += (pAct->act)(&ops,NULL,pAct->arg);
										}
									} 																																											 
									if (count >= OFDPA_PRETTY_MAX_LEN)																														 
									{ 																																											 
										buf->data[OFDPA_PRETTY_MAX_LEN - 1] = '\0'; 																												
										return OFDPA_E_FULL;																																	 
									} 																																											 
								}

#if 0
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "oamLmTxCountAction = %d ", bucketEntry->bucketData.mplsInterface.oamLmTxCountAction);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.mplsInterface.vlanId, bucketEntry->bucketData.mplsInterface.vlanId & OFDPA_VID_EXACT_MASK);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "lmepIdAction = %d ", bucketEntry->bucketData.mplsInterface.lmepIdAction);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "lmepId = %d ", bucketEntry->bucketData.mplsInterface.lmepId);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "srcMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[0],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[1],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[2],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[3],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[4],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[5]);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "dstMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[0],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[1],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[2],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[3],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[4],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[5]);
#endif
							}
              break;

            case OFDPA_MPLS_L2_VPN_LABEL:
            case OFDPA_MPLS_L3_VPN_LABEL:
            case OFDPA_MPLS_TUNNEL_LABEL1:
            case OFDPA_MPLS_TUNNEL_LABEL2:
            case OFDPA_MPLS_SWAP_LABEL:
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "pushL2Hdr = %d ", bucketEntry->bucketData.mplsLabel.pushL2Hdr);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "pushVlan = %d ", bucketEntry->bucketData.mplsLabel.pushVlan);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "newTpid = 0x%04x ", bucketEntry->bucketData.mplsLabel.newTpid);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "pushMplsHdr = %d ", bucketEntry->bucketData.mplsLabel.pushMplsHdr);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "pushCW = %d ", bucketEntry->bucketData.mplsLabel.pushCW);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "mplsLabel = 0x%x ", bucketEntry->bucketData.mplsLabel.mplsLabel);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "mplsBOS = %d ", bucketEntry->bucketData.mplsLabel.mplsBOS);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "mplsEXPAction = %d ", bucketEntry->bucketData.mplsLabel.mplsEXPAction);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "mplsEXP = %d ", bucketEntry->bucketData.mplsLabel.mplsEXP);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "mplsCopyEXPOutwards = %d ", bucketEntry->bucketData.mplsLabel.mplsCopyEXPOutwards);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "remarkTableIndexAction = %d ", bucketEntry->bucketData.mplsLabel.remarkTableIndexAction);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "RemarkTableIndex = %d ", bucketEntry->bucketData.mplsLabel.remarkTableIndex);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "mplsTTLAction = %d ", bucketEntry->bucketData.mplsLabel.mplsTTLAction);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "mplsTTL = %d ", bucketEntry->bucketData.mplsLabel.mplsTTL);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "mplsCopyTTLOutwards = %d ", bucketEntry->bucketData.mplsLabel.mplsCopyTTLOutwards);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "lmepIdAction = %d ", bucketEntry->bucketData.mplsLabel.lmepIdAction);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "lmepId = %d ", bucketEntry->bucketData.mplsLabel.lmepId);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "oamLmTxCountAction = %d ", bucketEntry->bucketData.mplsLabel.oamLmTxCountAction);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "colorBasedCountAction = %d ", bucketEntry->bucketData.mplsLabel.colorBasedCountAction);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "colorBasedCountId = %d ", bucketEntry->bucketData.mplsLabel.colorBasedCountId);
              break;

            default:
              count += printf("Unknown group sub-type (0x%x)", subType);
              break;
          }
        }
        break;

      case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
        rc = ofdpaGroupMplsSubTypeGet(bucketEntry->groupId, &subType);
        if (rc != OFDPA_E_NONE)
        {
          rc = OFDPA_E_ERROR;
        }
        else
        {
          switch (subType)
          {
            /* these subtypes are MPLS L2 Replication entries, all have same bucket actions */
            case OFDPA_MPLS_L2_FLOOD:
            case OFDPA_MPLS_L2_MULTICAST:
            case OFDPA_MPLS_L2_LOCAL_FLOOD:
            case OFDPA_MPLS_L2_LOCAL_MULTICAST:
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              break;

            case OFDPA_MPLS_FAST_FAILOVER:
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "watchPort = ");
              ofdpaPortDecode(buf->data, OFDPA_PRETTY_MAX_LEN, &count, bucketEntry->bucketData.mplsFastFailOver.watchPort);
              break;

            case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              break;

            case OFDPA_MPLS_ECMP:
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              break;

            case OFDPA_MPLS_L2_TAG:
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "pushVlan = %d ", bucketEntry->bucketData.mplsL2Tag.pushVlan);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "newTpid = 0x%04x ", bucketEntry->bucketData.mplsL2Tag.newTpid);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "popVlan = %d ", bucketEntry->bucketData.mplsL2Tag.popVlan);
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.mplsL2Tag.vlanId, bucketEntry->bucketData.mplsL2Tag.vlanId & OFDPA_VID_EXACT_MASK);
              break;

            default:
              APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "Unknown group sub-type (0x%x)", subType);
              break;
          }
        }
        break;

      default:
        APPEND_BUFFER_CHECK_SIZE(buf->data, OFDPA_PRETTY_MAX_LEN, count, "Unknown Group type (0x%x)", groupType);
        break;
    }
  }
  return(rc);
#endif
	return OFDPA_E_FAIL;
}


OFDPA_ERROR_t dpFlowTblPipeNodeRegister(OFDPA_FLOW_TABLE_ID_t					tableId,ofdpaTblPipeNodeOps_t *ops)
{

	if ((tableId >= OFDPA_FLOW_TABLE_ID_END) ||
			(ops == NULL))
	{
		return OFDPA_E_PARAM;
	}

	if(pipe_tbl_nodes[tableId].valid){
		return OFDPA_E_EXISTS;
	}
	
	pipe_tbl_nodes[tableId].tableId = tableId;
	
	pipe_tbl_nodes[tableId].ops = *ops;

	pipe_tbl_nodes[tableId].valid = 1;
	
	return OFDPA_E_NONE;
}


int datapathInit(void)
{
  printf("starting datapathInit ...\r\n");

	memset(pipe_tbl_nodes,0,sizeof(ofdpaTblPipeNode_t));
	
	(void)dpPipeNodeSocketsAddrCreate();
	(void)port_manager_init(0, NULL);
	(void)port_pipe_init(0, NULL);
	(void)vlan_pipe_init(0, NULL);
	(void)mplsL2Port_pipe_init(0, NULL);
	(void)mplsPcpTrust_pipe_init(0, NULL);
	(void)ingActExecutor_pipe_init(0, NULL);
	(void)group_pipe_init(0, NULL);

  return 0;
}
