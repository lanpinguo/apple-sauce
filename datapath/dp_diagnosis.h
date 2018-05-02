/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_diagnosis.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/5/2
  Last Modified :
  Description   : dp_diagnosis.c header file
  Function List :
  History       :
  1.Date        : 2018/5/2
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

#ifndef __DP_DIAGNOSIS_H__
#define __DP_DIAGNOSIS_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

 typedef struct ofdpaDebugDiagConfig_s
 {
	 int												 nodeSock;
	 pthread_t									 nodeTid ;
 
 }ofdpaDebugDiagConfig_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __DP_DIAGNOSIS_H__ */
