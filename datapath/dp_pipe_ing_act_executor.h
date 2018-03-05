/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_ing_act_executor.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/3/1
  Last Modified :
  Description   : ingress action set executor header
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


#ifndef INCLUDE_DP_PIPE_ING_ACT_EXECUTOR_H
#define INCLUDE_DP_PIPE_ING_ACT_EXECUTOR_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#include "ofdpa_list.h"



typedef struct  ofdpaIngActExecutorPipeNodeConfig_s
{

	uint32_t										max_entrys;
	int 												nodeSock;
	pthread_t 									nodeTid ;

} ofdpaIngActExecutorPipeNodeConfig_t;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INCLUDE_DP_PIPE_ING_ACT_EXECUTOR_H */


