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

 uint64_t ofdpaActAllowVlanTrans(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActCpyMplsExpOutwards(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActCpyMplsTtlOutwards(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActIdentifyOutPort(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActIncClassBasedCounter(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActIncColorBasedCount(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActOamLmTxCount(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActPopVlan(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActPushL2Hdr(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActPushCw(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActPushMplsHdr(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActPushVlan(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActPushVlan2(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetDstMac(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetEtherType(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetGrpId(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetLmepId(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMetaDataMplsL2Port(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMetaDataMplsType(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMetaDataOvid(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMetaDataTunId(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMetaDataVrf(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMplsBos(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMplsExp(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMplsLabel(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMplsTtl(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetQosIndex(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetRemarkTableId(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetSrcMac(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetTpid(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetVlanId(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetVlanId2(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetVlanPcp(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetVlanDei(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetDscp(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetMplsExpRemarkTable(void *this,ofdpaActArg_t *arg);
 uint64_t ofdpaActSetPcpPriRemarkTable(void *this,ofdpaActArg_t *arg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __DP_PIPE_ACT_FUNC_LIST_H__ */
