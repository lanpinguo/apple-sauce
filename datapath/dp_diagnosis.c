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
#include "dp_port_manager.h"

static int portPipeInSock;
static pthread_t diagTid ;

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


static OFDPA_ERROR_t diagCmdProcess(struct timeval *timeout)
{
	OFDPA_ERROR_t	rc;
  ofdpaPcbMsg_t msg;
	ofdpaPktCb_t *pcb;

	rc = dpPortMngPktRecv(&msg, NULL);

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

	while(1){
	
		diagCmdProcess(NULL);

	}


}


int diag_init(int argc, char *argv[])
{


	diagTid = (pthread_t)dpaThreadCreate("dpDiag", 61, diag_thread_core, NULL);


	return 0;
}





