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
/*   Filename 	:port.c                                           			    */
/*   Author    	:HuShouqiang                                         			  */
/*   Date       :2018-01-31                                           			*/
/*   Version   	:1.0                                                    		*/
/*   Purpose    :                                                       		*/
/********************************************************************************/
/********************************************************************************/

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
#include "ofdpa_util.h"

#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>

#include "ofdpa_api.h"
#include "of_adpl_api.h"
#include "sal_thread.h"
#include "datapath.h"





uint8_t ifname[64] = "netmap:eth1";

uint32_t pipeSockFd = 0;





/* Check the payload of the packet for errors (use it for debug).
 * Look for consecutive ascii representations of the size of the packet.
 */
static void
dump_payload(const char *_p, int len, struct netmap_ring *ring, int cur)
{
	char buf[128];
	int i, j, i0;
	const unsigned char *p = (const unsigned char *)_p;

	/* get the length in ASCII of the length of the packet. */

	printf("ring %p cur %5d [buf %6d flags 0x%04x len %5d]\n",
		ring, cur, ring->slot[cur].buf_idx,
		ring->slot[cur].flags, len);

	dump_pkt(p,len);


}


uint32_t getPktLen(ofdpaPktCb_t *pcb)
{
	int i;
	int len;

	if(pcb == NULL){
		return 0;
	}

	return pcb->pkt_len;
}


void *getFeild(ofdpaPktCb_t *pcb, int type)
{
	if((type >= FEILD_MAX) || (pcb == NULL)){
		return NULL;
	}

	if(pcb->feilds[type].offset < RESERVED_BLOCK_SIZE){
		return NULL;
	}

	return (void *) ((char *)pcb->this + pcb->feilds[type].offset);
}

void *getFeildVlan0(ofdpaPktCb_t *pcb)
{
	return (void *) ((char *)pcb + RESERVED_BLOCK_SIZE + 12);
}

void *getFeildVlan1(ofdpaPktCb_t *pcb)
{
	return (void *) ((char *)pcb + RESERVED_BLOCK_SIZE + 16);
}


static int
receive_packets(struct netmap_ring *ring, u_int limit, int dump, uint64_t *bytes)
{
	u_int cur, rx, n;
	uint64_t b = 0;
	ofdpaPktCb_t *pcb;
	struct OFDPA_VLAN *vlan;
	struct OFDPA_L3_TYPE *l3_type;
	uint16_t l3_type_offset = RESERVED_BLOCK_SIZE + 12;
	uint64_t	drop = DROP_NONE;
	uint64_t	*port = 0;
	ofdpaPcbMsg_t msg;

	
	if (bytes == NULL)
		bytes = &b;

	cur = ring->cur;
	n = nm_ring_space(ring);
	if (n < limit)
		limit = n;
	for (rx = 0; rx < limit; rx++) {
		struct netmap_slot *slot = &ring->slot[cur];
		char *p = NETMAP_BUF(ring, slot->buf_idx);

		*bytes += slot->len;
		
		/* pkt pre-parse */
		pcb 			= (ofdpaPktCb_t *)p;
		pcb->this = pcb;
		pcb->len 	= slot->len;
		pcb->pkt_len 	= slot->len - RESERVED_BLOCK_SIZE;
		port 			= (uint64_t *)p;
		/*port that the pkt comes from*/
		*port = (1<<2);

		OFDPA_INIT_LIST_HEAD(&pcb->action_set);
		
		
		vlan = (struct OFDPA_VLAN *)getFeildVlan0(pcb);

		if(vlan){
			//printf("\r\ntype : %04x\r\n",vlan->type);
			if((vlan->type == VLAN_TYPE)){
				l3_type_offset = RESERVED_BLOCK_SIZE + 16;
				SET_FEILD_OFFSET(pcb,FEILD_VLAN_0,RESERVED_BLOCK_SIZE + 12);
				vlan = (struct OFDPA_VLAN *)getFeildVlan1(pcb);
				SET_FEILD_OFFSET(pcb,FEILD_VLAN_1,\
					(vlan->type ^ VLAN_TYPE) ? 0 : (l3_type_offset = RESERVED_BLOCK_SIZE + 20, RESERVED_BLOCK_SIZE + 16));
			}
		}
		SET_FEILD_OFFSET(pcb,FEILD_L3_TYPE,l3_type_offset);

		l3_type = getFeild(pcb, FEILD_L3_TYPE);


		//printf("\r\n eth_type: %04x\r\n",l3_type->type);
		
		//printf("\r\n mpls mask: %08x\r\n",REORDER32_L2B(1<<8));

		if(l3_type->type == IP_TYPE) {

		}
		else if(l3_type->type == MPLS_TYPE){
			struct OFDPA_MPLS *mpls;

			SET_FEILD_OFFSET(pcb, FEILD_MPLS_0, l3_type_offset + 2);
			mpls = getFeild(pcb, FEILD_MPLS_0);
			//printf("\r\n mpls0: %08x\r\n",*(uint32_t*)mpls);
			if(IS_MPLS_BOS(mpls)){
				SET_FEILD_OFFSET(pcb, FEILD_CW, l3_type_offset + 6);
			}
			else{
				SET_FEILD_OFFSET(pcb, FEILD_MPLS_1, l3_type_offset + 6);
				mpls = getFeild(pcb, FEILD_MPLS_1);
				//printf("\r\n mpls1: %08x\r\n",*(uint32_t*)mpls);
				if(IS_MPLS_BOS(mpls)){
					SET_FEILD_OFFSET(pcb, FEILD_CW, l3_type_offset + 10);
				}
				else {
					SET_FEILD_OFFSET(pcb, FEILD_MPLS_2, l3_type_offset + 10);
					mpls = getFeild(pcb, FEILD_MPLS_2);
					//printf("\r\n mpls2: %08x",*(uint32_t*)mpls);
					SET_FEILD_OFFSET(pcb, FEILD_CW, l3_type_offset + 14);
					if(!IS_MPLS_BOS(mpls)){
						drop = DROP_ERROR_FORMAT;
					}
				}
			}

		}
		else{
			drop = DROP_UNSUPPORTED;
		}
		
		if (dump)
			dump_payload(p, slot->len, ring, cur);

		//printf("\r\nl3_type_offset : %d\r\n",l3_type_offset);

		if(drop == DROP_NONE){
			//dump_pcb(pcb);
			msg.dstObjectId = OFDPA_FLOW_TABLE_ID_INGRESS_PORT;
			msg.pcb = pcb;
			datapathPipeMsgSend(pipeSockFd,&msg);
		}


		cur = nm_ring_next(ring, cur);
	}
	ring->head = ring->cur = cur;

	return (rx);
}






void port_thread_core(void *argv)
{
	struct nm_desc 		*nmd;
	struct pollfd 		pfd = { .events = POLLIN };
	struct netmap_if 	*nifp;
	struct netmap_ring 	*rxring;

	int m;
	int	rv;
	int i;

	

	
	nmd = nm_open(ifname, NULL, 0, NULL);
	if (nmd == NULL) {
		D("Unable to open %s: %s", ifname, strerror(errno));
		printf("Unable to open %s: %s", ifname, strerror(errno));
		goto out;
	}

	pfd.fd = nmd->fd;
	nifp = nmd->nifp;

	while(1) {
		rv = poll(&pfd, 1, 2 * 1000) ;
		if ((rv < 0) || (pfd.revents & POLLERR)) {

		
			printf("error rv=%d(%s)!!\r\n",rv,strerror(errno));
			goto out;
		}


		
		for (i = nmd->first_rx_ring; i <= nmd->last_rx_ring; i++) {

			rxring = NETMAP_RXRING(nifp, i);
			/* compute free space in the ring */
			m = rxring->head + rxring->num_slots - rxring->tail;
			if (m >= (int) rxring->num_slots)
				m -= rxring->num_slots;
			if (nm_ring_empty(rxring))
				continue;

			m = receive_packets(rxring, 512, 0, NULL);
		}



	}

out:
	printf("error!!\r\n");
	return;	
	
}




int port_manager_init(int argc, char *argv[])
{
	pthread_t pipeTid ;
	OFDPA_ERROR_t rv;

	rv = dpPipeNodeSocketCreate(OFDPA_PORT_MANAGER_ID, &pipeSockFd);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}
	
	printf("\r\nStarting portTask.\r\n");

	pipeTid = (pthread_t)dpaThreadCreate("portTask", 61, port_thread_core, NULL);

	if (0 == pipeTid)
	{
	  printf("Failed starting portTask.\r\n");
	  return OFDPA_E_FAIL;
	}
	
	return OFDPA_E_NONE;
}








