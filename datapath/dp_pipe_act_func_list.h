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

 uint64_t ofdpaActAllowVlanTrans(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActCpyMplsExpOutwards(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActCpyMplsTtlOutwards(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActIdentifyOutPort(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActIncClassBasedCounter(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActIncColorBasedCount(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActOamLmTxCount(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActPopVlan(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActPushL2Hdr(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActPushMplsCw(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActPushMplsHdr(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActPushVlan(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActPushVlan2(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetDstMac(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetEtherType(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetGrpId(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetLmepId(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetMetaDataMplsL2Port(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetMetaDataMplsType(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetMetaDataOvid(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetMetaDataTunId(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetMetaDataVrf(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetMplsBos(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetMplsExp(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetMplsLabel(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetMplsTtl(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetQosIndex(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetRemarkTableId(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetSrcMac(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetTpid(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetVlanId(void *this,void *pcb, uint64_t arg);
 uint64_t ofdpaActSetVlanId2(void *this,void *pcb, uint64_t arg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __DP_PIPE_ACT_FUNC_LIST_H__ */
