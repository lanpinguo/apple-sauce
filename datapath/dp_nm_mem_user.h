/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_nm_mem_user.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/4/28
  Last Modified :
  Description   : dp_nm_mem_user.c header file
  Function List :
  History       :
  1.Date        : 2018/4/28
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

#ifndef __DP_NM_MEM_USER_H__
#define __DP_NM_MEM_USER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

 OFDPA_ERROR_t dpNetmapMemFree(uint32_t index);
 void * dpNetmapMemMalloc(u_int len, uint32_t *start, uint32_t *index);
 OFDPA_ERROR_t dpNetmapMemPoolInit(void* base,uint32_t size, uint32_t objSize);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __DP_NM_MEM_USER_H__ */
