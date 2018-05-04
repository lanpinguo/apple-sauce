/********************************************************************************/
/*   @@BEGAIN_INTERNAL_LEGAL@@                                          		*/
/*                                                                      		*/
/*                   Copyright(C) Description                           		*/
/* Raisecom  Science & Technology Development Co.,Ltd. Beijing, China   		*/
/*     Unpublished work-rights reserved under the China Copyright Act.  		*/
/*     Use,duplication, or disclosure by the government is subject to   		*/
/*     restrictions set forth in the CDTT commercial license agreement. 		*/
/*                                                                      		*/
/*   @@END_INTERNAL_LEGAL@@                                             		*/
/***********************************************************************		*/
/*   Filename 	:datapath_flowtable_vlan.c                                           			    */
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

static int portPipeInSock;
static pthread_t portPipeTid ;




static OFDPA_ERROR_t portPipeInPktProcess(struct timeval *timeout)
{
  int pipeInPktSockFd;
  ofdpaPcbMsg_t msg;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

  pipeInPktSockFd = portPipeInSock;
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

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_VERY_VERBOSE,
										"port ft rec %d\r\n",recvBytes);
	msg.dstObjectId = OFDPA_FLOW_TABLE_ID_VLAN;
	datapathPipeMsgSend(portPipeInSock,&msg);


  return OFDPA_E_NONE;
}



static void port_pipe_thread_core(void * args)
{
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_FLOW_TABLE_ID_INGRESS_PORT, &portPipeInSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}

	while(1){
	
		portPipeInPktProcess(NULL);

	}


}


int port_pipe_init(int argc, char *argv[])
{


	portPipeTid = (pthread_t)dpaThreadCreate("portFT", 61, port_pipe_thread_core, NULL);


	return 0;
}


