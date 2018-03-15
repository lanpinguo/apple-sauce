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

#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include "ofdpa_datatypes.h"
#include "avl_api.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"
#include "datapath.h"
#include "dp_port_manager.h"




uint8_t ifname[64] = "netmap:eth1";


ofdpaPortMngConfig_t		portMng;




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
			datapathPipeMsgSend(portMng.nodeSock,&msg);
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


int dpPortIsValid(uint32_t portNum)
{
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != 0) &&
      !(dataPtr->flags & DP_PORT_DELETED))
    return 1;
  else
    return 0;
}


OFDPA_ERROR_t dpPortGet(uint32_t portNum, ofdpaPortInfo_t *portInfo, DP_ENTRY_FLAG_t *flags)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  /* see if we found an entry */
  if (dataPtr != NULL)
  {
    if (portInfo != NULL)
    {
      memcpy(portInfo, &dataPtr->portInfo, sizeof(ofdpaPortInfo_t));
    }
    if (flags != NULL)
    {
      *flags = dataPtr->flags;
    }

    rc = OFDPA_E_NONE;
  }

  return rc;
}


OFDPA_ERROR_t dpPortAdd(uint32_t port, ofdpaPortInfo_t *portInfo)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdpaPortTable_node_t portNode;
  ofdpaPortTable_node_t *dataPtr;

  memset(&portNode, 0, sizeof(portNode));

  portNode.portNum = port;
  memcpy(&portNode.portInfo, portInfo, sizeof(portNode.portInfo));

  portNode.port_add_up_time = dpaUpTimeSeconds();
  portNode.flags = DP_PORT_ADDED;

  dataPtr = avlInsertEntry(&portMng.ofdbPortTable_tree, &portNode);

  if (dataPtr == &portNode)
  {
    /* Table is full
    */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                      "Port Table FULL!\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* duplicate entry found, check if marked deleted */
    if (dataPtr->flags & DP_PORT_DELETED)
    {
      /* entry was marked for delete, change flag and update info */
      dataPtr->flags = DP_PORT_ADDED;
      dataPtr->port_add_up_time = dpaUpTimeSeconds();
      memcpy(&dataPtr->portInfo, portInfo, sizeof(portNode.portInfo));
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                        "Port entry already exists!\r\n", 0);
      rc = OFDPA_E_EXISTS;
    }
  }

  if (rc == OFDPA_E_NONE)
  {
    /* Notify client  */
    datapathEventNotificationSend(NULL);
  }

  return rc;
}

OFDPA_ERROR_t dpPortDelete(uint32_t portNum)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;
  ofdpaPortInfo_t *portInfo;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if (dataPtr != 0)
  {
    /* check if already deleted */
    if (!(dataPtr->flags & DP_PORT_DELETED))
    {
      /* make sure no tenant entry references are still configured */
      if (dataPtr->portInfo.tunnelPortStatus.tenantCount != 0)
      {
        rc = OFDPA_E_FAIL;
      }
      else
      {
        dataPtr->flags = DP_PORT_DELETED;

        /* notify client */
        datapathEventNotificationSend(NULL);

        rc = OFDPA_E_NONE;
      }
    }
    else
    {
      /* entry already marked deleted, return success */
      rc = OFDPA_E_NONE;
    }
  }

  return rc;
}

OFDPA_ERROR_t dpPortNextGet(uint32_t portNum, uint32_t *nextPortNum)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdpaPortTable_node_t *dataPtr;
  uint32_t searchPortNum;

  /* search for next entry that is NOT marked deleted */
  searchPortNum = portNum;

  while (NULL != (dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &searchPortNum, AVL_NEXT)))
  {
    if (dataPtr->flags & DP_PORT_DELETED)
    {
      searchPortNum = dataPtr->portNum;
    }
    else
    {
      break;
    }
  }

  /* see if we found an entry */
  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    if (nextPortNum != NULL)
    {
      *nextPortNum = dataPtr->portNum;
    }
  }

  return rc;
}

OFDPA_ERROR_t dpPortMacGet(uint32_t portNum, ofdpaMacAddr_t *mac)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (mac != NULL)
    {
      memcpy(mac, &dataPtr->portInfo.hwAddr, OFDPA_MAC_ADDR_LEN);
    }
  }

  return rc;
}



OFDPA_ERROR_t dpPortInfoSet(uint32_t portNum, ofdpaPortInfo_t *portInfo)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (portInfo != NULL)
    {
      memcpy(&dataPtr->portInfo,portInfo ,sizeof(ofdpaPortInfo_t));
    }
  }

  return rc;
}


OFDPA_ERROR_t dpPortNameGet(uint32_t portNum, ofdpa_buffdesc *name)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (name != NULL)
    {
      strncpy(name->pstart, dataPtr->portInfo.name, name->size);
      name->size = strlen(dataPtr->portInfo.name) + 1;
    }
  }

  return rc;
}


OFDPA_ERROR_t dpPortNameSet(uint32_t portNum, ofdpa_buffdesc *name)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (name != NULL)
    {
      strncpy(dataPtr->portInfo.name,name->pstart , 
	  	(name->size > OFDPA_PORT_NAME_STRING_SIZE?OFDPA_PORT_NAME_STRING_SIZE:name->size));
    }
  }

  return rc;
}


/* BEGIN: Added by HuShouqiang, 2016/3/17   问题单号:is00099 */


OFDPA_ERROR_t dpPortEnableGet(uint32_t  portNum, int *enable)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;



  return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpPortEnableSet(uint32_t  portNum, int enable)
{
	OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
	
	
	
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpPortDescpGet(uint32_t portNum, ofdpa_buffdesc *descp)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (descp != NULL)
    {
      strncpy(descp->pstart, dataPtr->portInfo.descp, descp->size);
      descp->size = strlen(dataPtr->portInfo.descp) + 1;
    }
  }

  return rc;
}


OFDPA_ERROR_t dpPortDescpSet(uint32_t portNum, ofdpa_buffdesc *descp)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (descp != NULL)
    {
      strncpy(dataPtr->portInfo.name,descp->pstart , 
	  	(descp->size > OFDPA_PORT_NAME_STRING_SIZE?OFDPA_PORT_NAME_STRING_SIZE:descp->size));
    }
  }

  return rc;
}

OFDPA_ERROR_t dpPortLastChgTSGet(uint32_t portNum, uint32_t *ts)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (ts != NULL)
    {
      	*ts = dataPtr->portInfo.last_chg;
    }
  }

  return rc;
}


/* END:   Added by HuShouqiang, 2016/3/17 */



OFDPA_ERROR_t dpPortDurationGet(uint32_t  portNum, uint32_t *duration)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (duration != NULL)
    {
      *duration = dpaUpTimeSeconds() - dataPtr->port_add_up_time;
    }
  }

  return rc;
}

OFDPA_ERROR_t dpPortStateGet(uint32_t  portNum, OFDPA_PORT_STATE_t *state)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (state != NULL)
    {
      *state = dataPtr->portInfo.state;
    }
  }

  return rc;
}

OFDPA_ERROR_t dpPortStateSet(uint32_t  portNum, OFDPA_PORT_STATE_t state)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdpaPortTable_node_t *dataPtr;
  dpaEventMsg_t   eventMsg = {.msgType = DPA_EVENT_NOTIFICATION_MSG };

  
  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if (dataPtr != NULL)
  {
    if (state == dataPtr->portInfo.state)
    {
      /* no change in state */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERY_VERBOSE,
                        "Port state set called with unchanged data. (portNum = %d [0x%x], state = 0x%x)\r\n",
                         portNum, portNum, state);
    }
    else
    {
      /* something changed, determine what and update OFDB event flags */
      if ((state & OFDPA_PORT_STATE_LINK_DOWN) != (dataPtr->portInfo.state & OFDPA_PORT_STATE_LINK_DOWN)) 
      {
        /* port link state transitioned */
/* BEGIN: Added by Hushouqiang, 2016/7/23   问题单号:P10008 */
        eventMsg.msgType  = DPA_EVENT_PORT_STATUS_MSG;
        eventMsg.msgSubType = (state & OFDPA_PORT_STATE_LINK_DOWN);
        eventMsg.cookie[0] = portNum;
/* END:   Added by Hushouqiang, 2016/7/23 */
        if ((state & OFDPA_PORT_STATE_LINK_DOWN) != 0)
        {
          DP_ENTRY_FLAG_SET(dataPtr->flags, DP_PORT_LINK_UP);
          DP_ENTRY_FLAG_UNSET(dataPtr->flags, DP_PORT_LINK_DOWN);
        }
        else
        {
          DP_ENTRY_FLAG_SET(dataPtr->flags, DP_PORT_LINK_DOWN);
          DP_ENTRY_FLAG_UNSET(dataPtr->flags, DP_PORT_LINK_UP);
        }
      }

      if ((state & OFDPA_PORT_STATE_BLOCKED) != (dataPtr->portInfo.state & OFDPA_PORT_STATE_BLOCKED)) 
      {
        /* port liveness transitioned */
        if ((state & OFDPA_PORT_STATE_BLOCKED) != 0)
        {
          DP_ENTRY_FLAG_SET(dataPtr->flags, DP_PORT_BLOCKED);
          DP_ENTRY_FLAG_UNSET(dataPtr->flags, DP_PORT_NOT_BLOCKED);
        }
        else
        {
          DP_ENTRY_FLAG_SET(dataPtr->flags, DP_PORT_NOT_BLOCKED);
          DP_ENTRY_FLAG_UNSET(dataPtr->flags, DP_PORT_BLOCKED);
        }
      }

      if ((state & OFDPA_PORT_STATE_LIVE) != (dataPtr->portInfo.state & OFDPA_PORT_STATE_LIVE)) 
      {
        /* port liveness transitioned */
        if ((state & OFDPA_PORT_STATE_LIVE) != 0)
        {
          DP_ENTRY_FLAG_SET(dataPtr->flags, DP_PORT_LIVE);
          DP_ENTRY_FLAG_UNSET(dataPtr->flags, DP_PORT_NOT_LIVE);
        }
        else
        {
          DP_ENTRY_FLAG_SET(dataPtr->flags, DP_PORT_NOT_LIVE);
          DP_ENTRY_FLAG_UNSET(dataPtr->flags, DP_PORT_LIVE);
        }
      }

      dataPtr->portInfo.state = state;
	  
	  /* BEGIN: Added by HuShouqiang, 2016/3/17   问题单号:P10001 */
		dataPtr->portInfo.last_chg = time(NULL);
	  /* END:   Added by HuShouqiang, 2016/3/17 */


	  /* Notify client. */
      datapathEventNotificationSend(&eventMsg);
    }

    rc = OFDPA_E_NONE;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                      "Port table entry not found. (portNum = %d [0x%x], state = 0x%x)\r\n",
                       portNum, portNum, state);
  }

  return rc;
}

OFDPA_ERROR_t dpPortConfigSet(uint32_t portNum, OFDPA_PORT_CONFIG_t config)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdpaPortTable_node_t *dataPtr;
  OFDPA_PORT_TYPE_t portType;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if (dataPtr != NULL)
  {
    rc = OFDPA_E_NONE;
    dataPtr->portInfo.config = config;

    /* handle any updates to port state due to config */
    portType = DP_PORT_TYPE(portNum);
    switch (portType)
    {
#if 0 /* need to determine if admin config can influence liveness */
      case OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT:
        if ((config & OFDPA_PORT_CONFIG_DOWN) == 0)
        {
          /* port is adminstratively enabled */
          if ((dataPtr->portInfo.state & OFDPA_PORT_STATE_LIVE) != 0)
          {
            /* unset liveness status */
            ofdbPortStateSet(portNum, (dataPtr->portInfo.state & ~OFDPA_PORT_STATE_LIVE));
          }
        }
        else
        {
          /* port is adminstratively disabled */
          if ((dataPtr->portInfo.state & OFDPA_PORT_STATE_LIVE) == 0)
          {
            /* set liveness status */
            ofdbPortStateSet(portNum, (dataPtr->portInfo.state | OFDPA_PORT_STATE_LIVE));
          }
        }
#endif
        break;

      default:
        /* no updates needed for port types not handled above, continue */
        break;
    }
  }

  return rc;
}

OFDPA_ERROR_t dpPortConfigGet(uint32_t portNum, OFDPA_PORT_CONFIG_t  *config)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (config != NULL)
    {
      *config = dataPtr->portInfo.config;
    }
  }

  return rc;
}

OFDPA_ERROR_t dpPortAdvertiseFeatureSet(uint32_t portNum, OFDPA_PORT_FEATURE_t advertise)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    dataPtr->portInfo.advertised = advertise;
  }

  return rc;
}


OFDPA_ERROR_t dpPortMaxSpeedGet(uint32_t portNum, uint32_t  *maxSpeed)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;

  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &portNum, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if (maxSpeed != NULL)
    {
      *maxSpeed = dataPtr->portInfo.maxSpeed;
    }
  }

  return rc;
}

OFDPA_ERROR_t dpPortEventNextGet(ofdpaPortEvent_t *eventData)
{
  OFDPA_ERROR_t rc;
  ofdpaPortTable_node_t *dataPtr;
  int portFound;
  uint32_t scanPortNum;

  if (eventData == NULL)
  {
    return OFDPA_E_PARAM;
  }

  portFound = 0;
  scanPortNum = eventData->portNum;

  /* find port table entry that has been marked for an event */
  while ((!portFound) &&
         ((dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &scanPortNum, AVL_NEXT)) != NULL))
  {
    if (dataPtr->flags != 0)
    {
      eventData->portNum = dataPtr->portNum;
      eventData->eventMask = 0;

      if (dataPtr->flags & DP_PORT_ADDED)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_CREATE;
      }
      else if (dataPtr->flags & DP_PORT_DELETED)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_DELETE;
      }

      if (dataPtr->flags & DP_ENTRY_PORT_STATE_EVENT_FLAGS)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_STATE;
        eventData->state = dataPtr->portInfo.state;
      }

      if (dataPtr->flags & DP_PORT_DELETED)
      {
        /* remove deleted port node now that its deletion has been reported */
        if (dataPtr->portInfo.portQueueInfo != NULL)
        {
          free(dataPtr->portInfo.portQueueInfo);
        }
        avlDeleteEntry(&portMng.ofdbPortTable_tree, dataPtr);
      }
      else
      {
        /* clear the event flags now that they have been reported */
        dataPtr->flags = 0;
      }

      portFound = 1;
    }
    else
    {
      scanPortNum = dataPtr->portNum;
    }
  }

  if (portFound)
  {
    rc = OFDPA_E_NONE;
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return(rc);
}

OFDPA_ERROR_t dpPortEventGet(ofdpaPortEvent_t *eventData)
{
  OFDPA_ERROR_t rc;
  ofdpaPortTable_node_t *dataPtr;
  int portFound;
  uint32_t scanPortNum;

  if (eventData == NULL)
  {
    return OFDPA_E_PARAM;
  }

  portFound = 0;
  scanPortNum = eventData->portNum;

  /* find port table entry that has been marked for an event */
  if ((dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &scanPortNum, AVL_EXACT)) != NULL)
  {
    if (dataPtr->flags != 0)
    {
      eventData->portNum = dataPtr->portNum;
      eventData->eventMask = 0;

      if (dataPtr->flags & DP_PORT_ADDED)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_CREATE;
      }
      else if (dataPtr->flags & DP_PORT_DELETED)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_DELETE;
      }

      if (dataPtr->flags & DP_ENTRY_PORT_STATE_EVENT_FLAGS)
      {
        eventData->eventMask |= OFDPA_EVENT_PORT_STATE;
        eventData->state = dataPtr->portInfo.state;
      }

      if (dataPtr->flags & DP_PORT_DELETED)
      {
        /* remove deleted port node now that its deletion has been reported */
        if (dataPtr->portInfo.portQueueInfo != NULL)
        {
          free(dataPtr->portInfo.portQueueInfo);
        }
        avlDeleteEntry(&portMng.ofdbPortTable_tree, dataPtr);
      }
      else
      {
        /* clear the event flags now that they have been reported */
        dataPtr->flags = 0;
      }

      portFound = 1;
    }
  }

  if (portFound)
  {
    rc = OFDPA_E_NONE;
  }
  else
  {
    rc = OFDPA_E_NOT_FOUND;
  }

  return(rc);
}




/*****************************************************************************
 Prototype    : ofdbPortQueueSchedSet
 Description  : This routine set the schedule policy of COS queue on a port.
                
 Input        : uint32_t port       
                uint32_t mode       
                int32_t *weights  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2017/3/22
    Author       : HuShouqiang
    Modification : Created function

*****************************************************************************/
OFDPA_ERROR_t  dpPortQueueSchedSet(uint32_t port, int32_t mode,int32_t *weights)
{ 
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdpaPortTable_node_t *dataPtr;
  int i ;
  uint32_t numQueues;



  dataPtr = avlSearch(&portMng.ofdbPortTable_tree, &port, AVL_EXACT);

  if ((dataPtr != NULL) &&
      !(dataPtr->flags & DP_PORT_DELETED))
  {
    rc = OFDPA_E_NONE;
    if(mode != -1)
    {
      dataPtr->portInfo.schedMode = mode;
    }

  
    if((dataPtr->portInfo.schedMode & 
      (DP_QOS_QUEUE_SCHED_MODE_WRR | DP_QOS_QUEUE_SCHED_MODE_WDRR | DP_QOS_QUEUE_SCHED_MODE_WFQ)) != 0 )
    {
      
      rc = dpaCosQueuesMaxGet(&numQueues);
      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                           "Could not get max queues!!!\r\n", 0);
        return rc;
      }

      
      for(i = 0 ; i < numQueues ; i ++ )
      {
        if(weights != NULL)  
        {
          if(weights[i] == 0)
          {
            if((dataPtr->portInfo.schedMode & DP_QOS_QUEUE_SCHED_MODE_SP) == 0 )
            {
              /* NOT "sp + wxx" mode use the default value*/
              weights[i] = DEFAULT_PORT_Q_WEIGHT;
            }
          }
          /* record in database*/
          dataPtr->portInfo.portQueueInfo[i].weight = weights[i];
        }
        else
        {
          /* when switch scheduler mode, then set weight to default value */
          
          if((dataPtr->portInfo.schedMode & DP_QOS_QUEUE_SCHED_MODE_SP) == 0 )
          {
            /* NOT "sp + wxx" mode use the default value*/
            
            dataPtr->portInfo.portQueueInfo[i].weight = DEFAULT_PORT_Q_WEIGHT;
          }
          else
          {
            dataPtr->portInfo.portQueueInfo[i].weight = 0;
          }
        }        
      }

    }

    rc = OFDPA_E_NONE;
  }

  return rc;

}

int port_manager_init(int argc, char *argv[])
{
	OFDPA_ERROR_t rv;


  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Port Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&portMng.ofdbPortTable_tree,
                               dpaPortTableEntryCountGet() * 2,
                               sizeof(ofdpaPortTable_node_t),
                               0x10,
                               avlCompareULong32,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Port Table AVL Tree.\r\n", 0);
    return -1;
  }



	rv = dpPipeNodeSocketCreate(OFDPA_PORT_MANAGER_ID, &portMng.nodeSock);
	if(rv != OFDPA_E_NONE){
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_DATAPATH, OFDPA_DEBUG_ALWAYS,
                      "Failed to create socket %d. errno %s.\r\n",rv);
		return;
	}
	
	printf("\r\nStarting portTask.\r\n");

	portMng.nodeTid = (pthread_t)dpaThreadCreate("portTask", 61, port_thread_core, NULL);

	if (0 == portMng.nodeTid)
	{
	  printf("Failed starting portTask.\r\n");
	  return OFDPA_E_FAIL;
	}
	
	return OFDPA_E_NONE;
}








