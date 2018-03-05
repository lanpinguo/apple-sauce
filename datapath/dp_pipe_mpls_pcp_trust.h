/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_mpls_pcp_trust.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/3/1
  Last Modified :
  Description   : mpls pcp trust table header
  Function List :
  History       :
  1.Date        : 2018/3/1
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


#ifndef INCLUDE_DP_PIPE_MPLS_PCP_TRUST_H
#define INCLUDE_DP_PIPE_MPLS_PCP_TRUST_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#include "ofdpa_list.h"


#define OFDPA_FT_MPLS_PCP_TRUST_APLY_ACT_MAX		8
#define OFDPA_FT_MPLS_PCP_TRUST_WR_ACT_MAX			1

/** VLAN Flow Table Match */
typedef struct  ofdpaMplsPcpTrustPipeMatch_s
{
  /** OpenFlow Ingress Port number */
  uint64_t    inPort;
  uint16_t    vlanId;
  uint16_t    vlanIdMask;

} ofdpaMplsPcpTrustPipeMatch_t;






typedef struct  ofdpaMplsPcpTrustPipeNode_s 
{
  uint32_t                				priority;  	/* matching precedence of the flow entry */  
  uint32_t                				hard_time;
  uint32_t                				idle_time;			
  uint32_t                				flags;     	/* flags alter the way flow entries are managed*/
  uint64_t												recv_pkts;
  uint64_t												recv_bytes;
  uint64_t                				valid;  	  

	ofdpaMplsPcpTrustPipeMatch_t		match;
	ofdpaFlowTblInstruct_t					instructions;
} ofdpaMplsPcpTrustPipeNode_t;


typedef struct  ofdpaMplsPcpTrustPipeNodeConfig_s
{

	ofdpaMplsPcpTrustPipeNode_t 				*entrys;
	uint32_t										max_entrys;
	int 												nodeSock;
	pthread_t 									nodeTid ;

} ofdpaMplsPcpTrustPipeNodeConfig_t;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INCLUDE_DP_PIPE_MPLS_PCP_TRUST_H */

