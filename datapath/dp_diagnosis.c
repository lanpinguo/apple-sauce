/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_diagnosis.c
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/3/17
  Last Modified :
  Description   : diagnosis
  Function List :
  History       :
  1.Date        : 2018/3/17
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

#include <errno.h>
#define _GNU_SOURCE	/* for CPU_SET() */
#include <stdio.h>

#include <ctype.h>	// isprint()
#include <unistd.h>	// sysconf()
#include <sys/poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <libgen.h>
#include <signal.h>
#include <linux/kernel.h>
#include <arpa/inet.h>
#include <argp.h>
#include <regex.h>
#include <pthread.h>

#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include "ofdpa_datatypes.h"
#include "avl_api.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"
#include "datapath.h"
#include "dp_diagnosis.h"



static ofdpaDebugDiagConfig_t diag_config;


typedef struct tstPkt_s
{
	uint8_t pcb[RESERVED_BLOCK_SIZE];

	uint8_t pkt[64];

}tstPkt_t;

tstPkt_t tstPkt = {
	.pkt = {
		0x00, 0x0E, 0xC6, 0xA0, 0xA1, 0x3D, 0x54, 0xEE, 0x75, 0xC0, 0x5D, 0x61,
		0x81, 0x00, 0x00, 0x0A, 0x08, 0x00, 0x45, 0x00, 0x00, 0x28, 0x5A, 0x16, 
		0x40, 0x00, 0x80, 0x06, 0xE7, 0x08, 0xC0, 0xA8, 0x02, 0x69, 0x3D, 0x87,
		0xB9, 0x18, 0xD2, 0xAD, 0x01, 0xBB, 0x1B, 0x41, 0x27, 0x82, 0x93, 0x8F,
		0xA4, 0xE3, 0x50, 0x10, 0x03, 0xFC, 0xA2, 0x88, 0x00, 0x00
	}
};






OFDPA_ERROR_t dpDiagPktRecv(ofdpaPcbMsg_t *msg, struct timeval *timeout)
{
  int pipeInPktSockFd;
  ssize_t recvBytes;
  int rv;
  int flags = 0;

  pipeInPktSockFd = diag_config.nodeSock;
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

  recvBytes = recvfrom(pipeInPktSockFd, msg, sizeof(*msg), flags, 0, 0);

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
										"port manager rec %d\r\n",recvBytes);

  return OFDPA_E_NONE;
}


static OFDPA_ERROR_t diagCmdProcess(struct timeval *timeout)
{
	OFDPA_ERROR_t	rc;
  ofdpaPcbMsg_t msg;
	ofdpaPktCb_t *pcb;

	rc = dpDiagPktRecv(&msg, NULL);

	dump_pkt(&msg, sizeof(msg));

	pcb = &tstPkt;
	pcb->len = sizeof(tstPkt);
	pcb->pkt_len = sizeof(tstPkt.pkt);
	rc = dpPktPreParse(pcb);
	printf("\r\nTest Pkt:\r\n");
	dump_pkt(pcb, pcb->len);

	if(rc == OFDPA_E_NONE){
		dump_pcb(pcb);
		msg.dstObjectId = OFDPA_FLOW_TABLE_ID_INGRESS_PORT;
		msg.pcb = pcb;
		datapathPipeMsgSend(getPortMngSockFd(),&msg);
	}


  return OFDPA_E_NONE;
}



static void diag_thread_core(void * args)
{
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_NODE_DEBUG_DIAG, &diag_config.nodeSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}

	while(1){
	
		diagCmdProcess(NULL);

	}


}


int diag_init(int argc, char *argv[])
{


	diag_config.nodeTid = (pthread_t)dpaThreadCreate("dpDiag", 61, diag_thread_core, NULL);


	return 0;
}





