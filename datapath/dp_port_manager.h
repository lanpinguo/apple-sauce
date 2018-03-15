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
/*   Filename 	:port.h                                           			    */
/*   Author    	:HuShouqiang                                         			  */
/*   Date       :2018-01-31                                           			*/
/*   Version   	:1.0                                                    		*/
/*   Purpose    :                                                       		*/
/********************************************************************************/
/********************************************************************************/

#ifndef INCLUDE_PORT_H
#define INCLUDE_PORT_H

typedef enum
{
  DP_ENTRY_HARD_TIMEOUT         = 1 << 0,
  DP_ENTRY_IDLE_TIMEOUT         = 1 << 1,

  DP_PORT_ADDED                 = 1 << 2,
  DP_PORT_DELETED               = 1 << 3,

  DP_PORT_LINK_UP               = 1 << 4,
  DP_PORT_LINK_DOWN             = 1 << 5,
  DP_PORT_BLOCKED               = 1 << 6,
  DP_PORT_NOT_BLOCKED           = 1 << 7,
  DP_PORT_LIVE                  = 1 << 8,
  DP_PORT_NOT_LIVE              = 1 << 9,

} DP_ENTRY_FLAG_t;

/*
** The Port Table.
*/

#define DP_PORT_TYPE(p)         (((p) & OFDPA_INPORT_TYPE_MASK) >> 16)
#define DP_PORT_INDEX(p)         ((p) & OFDPA_INPORT_INDEX_MASK)
#define DP_PORT_TYPE_SET(p, x)  (((p) & ~OFDPA_INPORT_TYPE_MASK)  | (((x) & 0xffffu) << 16))
#define DP_PORT_INDEX_SET(p, x) (((p) & ~OFDPA_INPORT_INDEX_MASK) | ((x) & 0xffffu))

#define DP_MPLS_L2_PORT_TYPE(p)         (((p) & OFDPA_MPLS_L2_PORT_TYPE_MASK) >> 16)
#define DP_MPLS_L2_PORT_INDEX(p)         ((p) & OFDPA_MPLS_L2_PORT_INDEX_MASK)
#define DP_MPLS_L2_PORT_TYPE_SET(p, x)  (((p) & ~OFDPA_MPLS_L2_PORT_TYPE_MASK)  | (((x) & 0xffffu) << 16))
#define DP_MPLS_L2_PORT_INDEX_SET(p, x) (((p) & ~OFDPA_MPLS_L2_PORT_INDEX_MASK) | ((x) & 0xffffu))

#define DP_TUNNELID_TYPE(p)         (((p) & OFDPA_TUNNEL_ID_TYPE_MASK) >> 16)
#define DP_TUNNELID_INDEX(p)         ((p) & OFDPA_TUNNEL_ID_INDEX_MASK)
#define DP_TUNNELID_TYPE_SET(p, x)  (((p) & ~OFDPA_TUNNEL_ID_TYPE_MASK)  | (((x) & 0xffffu) << 16))
#define DP_TUNNELID_INDEX_SET(p, x) (((p) & ~OFDPA_TUNNEL_ID_INDEX_MASK) | ((x) & 0xffffu))


/* set of flags that indicate a port state change event */
#define DP_ENTRY_PORT_STATE_EVENT_FLAGS (DP_PORT_LINK_UP | DP_PORT_LINK_DOWN | DP_PORT_BLOCKED | DP_PORT_NOT_BLOCKED | DP_PORT_LIVE | DP_PORT_NOT_LIVE)

#define DP_ENTRY_FLAG_SET(__storage__, __flag__)   (__storage__ |= __flag__)
#define DP_ENTRY_FLAG_UNSET(__storage__, __flag__) (__storage__ &= ~__flag__)


/* Port Queue information
*/

typedef struct
{
  uint32_t minRate;
  uint32_t maxRate;
  uint32_t weight;
} ofdpaPortQueueInfo_t;


/* Port Status information.
*/
typedef struct
{
  /* status items */
  ofdpaMacAddr_t hwAddr;
  char name[OFDPA_PORT_NAME_STRING_SIZE];
  /*Description,added by Hushouqiang 20160316*/
  char descp[OFDPA_PORT_DESCP_STRING_SIZE];

  uint32_t maxSpeed;

  OFDPA_PORT_CONFIG_t config;
  OFDPA_PORT_STATE_t state;

/* BEGIN: Added by HuShouqiang, 2016/3/17   ÎÊÌâµ¥ºÅ:P10001 */
	/*last-change date-and-time in calendar time*/
	uint32_t last_chg;

/* END:   Added by HuShouqiang, 2016/3/17 */

  uint32_t  schedMode;  /* port congestion scheduler mode */

  OFDPA_PORT_FEATURE_t advertised;
  OFDPA_PORT_FEATURE_t supported;

  ofdpaPortQueueInfo_t *portQueueInfo;

  /* flag indicating a physical is used in an untagged tunnel access port */
  uint32_t physicalPortOnUntaggedAccessPort;

  ofdpaTunnelPortConfig_t tunnelPortConfig;
  ofdpaTunnelPortStatus_t tunnelPortStatus;

  uint32_t ffGroupId;  /* used to store fast failover group that caused a liveness port to be created */

  uint32_t ulIpAddr;//management ip
  uint32_t ulIpMask;//management mask
} ofdpaPortInfo_t;

/* OpenFlow Port AVL Tree.
*/
typedef struct
{
  /* index */
  uint32_t           portNum;
  ofdpaPortInfo_t     portInfo;
  uint32_t           port_add_up_time;
  DP_ENTRY_FLAG_t  	flags;
  void              *next;              /* This field must be the last element in the AVL Tree Node */
} ofdpaPortTable_node_t;

typedef struct ofdpaPortMngConfig_s
{
	int 												nodeSock;
	pthread_t 									nodeTid ;

	avlTree_t 									ofdbPortTable_tree;

}ofdpaPortMngConfig_t;




#endif /* INCLUDE_PORT_H */
/*!  @}
 */


