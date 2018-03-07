/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_act_func_list.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/3/1
  Last Modified :
  Description   : dp_pipe_act_func_list.c header file
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

#ifndef __DP_PIPE_ACT_FUNC_LIST_H__
#define __DP_PIPE_ACT_FUNC_LIST_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

 uint64_t dpActAllowVlanTrans(void *this,void *pcb, uint64_t arg);
 uint64_t dpActCpyMplsExpOutwards(void *this,void *pcb, uint64_t arg);
 uint64_t dpActCpyMplsTtlOutwards(void *this,void *pcb, uint64_t arg);
 uint64_t dpActIdentifyOutPort(void *this,void *pcb, uint64_t arg);
 uint64_t dpActIncClassBasedCounter(void *this,void *pcb, uint64_t arg);
 uint64_t dpActIncColorBasedCount(void *this,void *pcb, uint64_t arg);
 uint64_t dpActOamLmTxCount(void *this,void *pcb, uint64_t arg);
 uint64_t dpActPopVlan(void *this,void *pcb, uint64_t arg);
 uint64_t dpActPushL2Hdr(void *this,void *pcb, uint64_t arg);
 uint64_t dpActPushMplsCw(void *this,void *pcb, uint64_t arg);
 uint64_t dpActPushMplsHdr(void *this,void *pcb, uint64_t arg);
 uint64_t dpActPushVlan(void *this,void *pcb, uint64_t arg);
 uint64_t dpActPushVlan2(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetDstMac(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetEtherType(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetGrpId(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetLmepId(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetMetaDataMplsL2Port(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetMetaDataMplsType(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetMetaDataOvid(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetMetaDataTunId(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetMetaDataVrf(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetMplsBos(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetMplsExp(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetMplsLabel(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetMplsTtl(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetQosIndex(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetRemarkTableId(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetSrcMac(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetTpid(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetVlanId(void *this,void *pcb, uint64_t arg);
 uint64_t dpActSetVlanId2(void *this,void *pcb, uint64_t arg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __DP_PIPE_ACT_FUNC_LIST_H__ */
