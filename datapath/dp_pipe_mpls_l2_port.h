/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_mpls_l2_port.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/2/26
  Last Modified :
  Description   : flow table mpls l2 port  header
  Function List :
  History       :
  1.Date        : 2018/2/26
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




#ifndef INCLUDE_DP_PIPE_MPLS_L2_PORT_H
#define INCLUDE_DP_PIPE_MPLS_L2_PORT_H

#include "ofdpa_list.h"


#define OFDPA_FT_MPLS_L2_PORT_APLY_ACT_MAX		1
#define OFDPA_FT_MPLS_L2_PORT_WR_ACT_MAX			1


typedef struct ofdpaMplsL2PortMatchKey_s
{
  uint32_t mplsL2Port;

  uint32_t tunnelId;
  
  uint16_t etherType;

  uint8_t pad[6];
}ofdpaMplsL2PortMatchKey_t;



/** Mpls l2 port Flow Table Match */
typedef struct ofdpaMplsL2PortPipeMatch_s
{
	ofdpaMplsL2PortMatchKey_t	key;
	ofdpaMplsL2PortMatchKey_t	keyMask;
}ofdpaMplsL2PortPipeMatch_t;





typedef struct ofdpaMplsL2PortPipeNode_s 
{
  uint32_t                				priority;  	/* matching precedence of the flow entry */  
  uint32_t                				hard_time;
  uint32_t                				idle_time;			
  uint32_t                				flags;     	/* flags alter the way flow entries are managed*/
  uint64_t												recv_pkts;
  uint64_t												recv_bytes;
  uint64_t                				valid;  	  

	ofdpaMplsL2PortPipeMatch_t			match;
	ofdpaFlowTblInstruct_t					instructions;
}ofdpaMplsL2PortPipeNode_t;


typedef struct ofdpaMplsL2PortPipeNodeConfig_s
{
	ofdpaRWlock_t 							lock;

	ofdpaMplsL2PortPipeNode_t 	*entrys;
	uint32_t										max_entrys;
	uint32_t										count;			/* the number of current entrys*/
	int 												nodeSock;
	pthread_t 									nodeTid ;

}ofdpaMplsL2PortPipeNodeConfig_t;

#endif /* INCLUDE_DP_PIPE_MPLS_L2_PORT_H */





