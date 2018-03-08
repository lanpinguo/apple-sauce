/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_group.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/3/8
  Last Modified :
  Description   : group header
  Function List :
  History       :
  1.Date        : 2018/3/8
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



#ifndef INCLUDE_DP_PIPE_GROUP_H
#define INCLUDE_DP_PIPE_GROUP_H

#include "ofdpa_list.h"







typedef struct ofdpaGrpPipeNodeConfig_s
{
	struct ofdpa_list_head			entrys;
	uint32_t										max_entrys;
	int 												nodeSock;
	pthread_t 									nodeTid ;

  avlTree_t 									ofdbGroupTable_tree;
  avlTree_t 									ofdbGroupBucketRefTable_tree;
  avlTree_t 									ofdbGroupBucketDataTable_tree;

}ofdpaGrpPipeNodeConfig_t;

#endif /* INCLUDE_DP_PIPE_GROUP_H */




