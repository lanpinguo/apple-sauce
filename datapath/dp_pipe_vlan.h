/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_vlan.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/2/20
  Last Modified :
  Description   : data struct definition for vlan table
  Function List :
  History       :
  1.Date        : 2018/2/20
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



typedef struct ofdpaVlanMatchKey_s
{
  uint64_t    inPort;
  uint16_t    vlanId;

  uint8_t pad[6];
}ofdpaVlanMatchKey_t;




/** VLAN Flow Table Match */
typedef struct ofdpaVlanPipeMatch_s
{
	ofdpaVlanMatchKey_t key;
	ofdpaVlanMatchKey_t keyMask;
}ofdpaVlanPipeMatch_t;






typedef struct ofdpaVlanPipeNode_s 
{
  uint32_t                				priority;  	/* matching precedence of the flow entry */  
  uint32_t                				hard_time;
  uint32_t                				idle_time;			
  uint32_t                				flags;     	/* flags alter the way flow entries are managed*/
  uint32_t                				up_ts;     	/*first add up time stamp*/
  uint64_t												recv_pkts;
  uint64_t												recv_bytes;
  uint64_t                				valid;  	  

	ofdpaVlanPipeMatch_t				 		match;
	ofdpaFlowTblInstruct_t					instructions;
}ofdpaVlanPipeNode_t;


typedef struct ofdpaVlanPipeNodeConfig_s
{

	ofdpaVlanPipeNode_t 				*entrys;
	uint32_t										max_entrys;
	uint32_t										count;			/* the number of current entrys*/
	int 												nodeSock;
	pthread_t 									nodeTid ;

}ofdpaVlanPipeNodeConfig_t;

#endif /* INCLUDE_DP_PIPE_VLAN_H */




