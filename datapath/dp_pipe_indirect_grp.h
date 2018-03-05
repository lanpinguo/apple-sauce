/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_grp_l2_if.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/2/23
  Last Modified :
  Description   : group table l2 interface header file
  Function List :
  History       :
  1.Date        : 2018/2/23
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



#ifndef INCLUDE_DP_PIPE_GRP_L2_IF_H
#define INCLUDE_DP_PIPE_GRP_L2_IF_H

#include "ofdpa_list.h"







typedef struct ofdpaIndirectGrpPipeNodeConfig_s
{
	struct ofdpa_list_head			entrys;
	uint32_t										max_entrys;
	int 												nodeSock;
	pthread_t 									nodeTid ;

}ofdpaIndirectGrpPipeNodeConfig_t;

#endif /* INCLUDE_DP_PIPE_GRP_L2_IF_H */




