
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
#include "dp_pipe_ing_act_executor.h"



ofdpaIngActExecutorPipeNodeConfig_t ing_act_executor_pipe_config;


int getIngActExecutorSockFd(void)
{
	return	ing_act_executor_pipe_config.nodeSock;
}








OFDPA_ERROR_t  executeIngActSetOnPkt(ofdpaPktCb_t *pcb)
{	
	int i;
	OFDPA_ERROR_t rv;
	ofdpaActSetHolder_t	*pActSetHolder;

	

	/* write-action */

#if 1	

	for(;;){
		if(!ofdpa_list_empty(&pcb->action_set)){
			pActSetHolder = ofdpa_list_first_entry(&pcb->action_set,ofdpaActSetHolder_t,head);
			if(pActSetHolder != NULL){
				OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
								 "get action %d \r\n",pActSetHolder->actHolder.numAct);
				ofdpa_list_del_init(&pActSetHolder->head);
			}
			else{
				OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
								 "Wrong pointer %p \r\n",pActSetHolder);
				break;
			}
		}
		else{
			break;
		}

	}

#else	
	ofdpa_list_for_each_entry(pActSetHolder,&pcb->action_set,ofdpaActSetHolder_t,head){

		if(pActSetHolder != NULL){
			printf("\r\n head = %p, head->prev = %p , head->next = %p \r\n",
				&pcb->action_set,pcb->action_set.prev,pcb->action_set.next);
			printf("\r\n cur = %p , cur->prev = %p, cur->next = %p \r\n",
				&pActSetHolder->head,pActSetHolder->head.prev,pActSetHolder->head.next);
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
							 "get action %d \r\n",pActSetHolder->actHolder.numAct);
			ofdpa_list_del(&pActSetHolder->head);
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
							 "delete action %p \r\n",pActSetHolder);
		}

	}
#endif
	
	return OFDPA_E_NONE;

}



static OFDPA_ERROR_t ingActExecutorPktProcess( ofdpaPktCb_t *pcb)
{
	OFDPA_ERROR_t rv;
	ofdpaPcbMsg_t msg;


	executeIngActSetOnPkt(pcb);

#if 0
	msg.dstObjectId = OFDPA_INVALID_KEY;
	msg.pcb = pcb;
	rv = datapathPipeMsgSend(ing_act_executor_pipe_config.nodeSock ,&msg);
	if(rv != OFDPA_E_NONE){
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_BASIC,
						 "Failed to send msg, rv = %d\r\n",rv);
	
		return OFDPA_E_PARAM;
	}
#endif
  return OFDPA_E_NONE;
}



static OFDPA_ERROR_t ingActExecutorPipeInPktRecv(struct timeval *timeout)
{
  int pipeInPktSockFd;
  ofdpaPcbMsg_t msg;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

	

  pipeInPktSockFd = getIngActExecutorSockFd();
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

	ingActExecutorPktProcess(msg.pcb);

  return OFDPA_E_NONE;
}



static void ingActExecutor_pipe_thread_core(void * arg)
{
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_ING_ACT_EXECUTOR, &ing_act_executor_pipe_config.nodeSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}

	while(1){
	
		ingActExecutorPipeInPktRecv(NULL);

	}


}

/*****************************************************************************
 Prototype    : ingActExecutor_pipe_init
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
int ingActExecutor_pipe_init(int argc, char *argv[])
{
	
	ing_act_executor_pipe_config.nodeTid = (pthread_t)dpaThreadCreate("mplsPcpTrustFT", 62, ingActExecutor_pipe_thread_core, NULL);

	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/

