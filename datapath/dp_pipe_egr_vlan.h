/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_vlan.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/4/9
  Last Modified :
  Description   : data struct definition for egress vlan table
  Function List :
  History       :
  1.Date        : 2018/4/9
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



#ifndef INCLUDE_DP_PIPE_VLAN_H
#define INCLUDE_DP_PIPE_VLAN_H

#include "ofdpa_list.h"



#define OFDPA_FT_VLAN_IN_PORT_MASK		0xFFFFFFFFFFFFFFFFUL



typedef struct ofdpaEgrVlanMatchKey_s
{
  uint64_t    inPort;
  uint16_t    vlanId;

  uint8_t pad[6];
}ofdpaEgrVlanMatchKey_t;




/** VLAN Flow Table Match */
typedef struct ofdpaEgrVlanPipeMatch_s
{
	ofdpaEgrVlanMatchKey_t key;
	ofdpaEgrVlanMatchKey_t keyMask;
}ofdpaEgrVlanPipeMatch_t;






typedef struct ofdpaEgrVlanPipeNode_s 
{
  uint32_t                				priority;  	/* matching precedence of the flow entry */  
  uint32_t                				hard_time;
  uint32_t                				idle_time;			
  uint32_t                				flags;     	/* flags alter the way flow entries are managed*/
  uint32_t                				up_ts;     	/*first add up time stamp*/
  uint64_t												recv_pkts;
  uint64_t												recv_bytes;
  uint64_t                				valid;  	  

	ofdpaEgrVlanPipeMatch_t				 		match;
	ofdpaFlowTblInstruct_t					instructions;
}ofdpaEgrVlanPipeNode_t;


typedef struct ofdpaEgrVlanPipeNodeConfig_s
{
	ofdpaRWlock_t 							lock;

	ofdpaEgrVlanPipeNode_t 				*entrys;
	uint32_t										max_entrys;
	uint32_t										count;			/* the number of current entrys*/
	int 												nodeSock;
	pthread_t 									nodeTid ;

}ofdpaEgrVlanPipeNodeConfig_t;

#endif /* INCLUDE_DP_PIPE_VLAN_H */




