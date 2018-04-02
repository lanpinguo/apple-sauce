/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_indirect_grp.h
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/3/27
  Last Modified :
  Description   : dp_pipe_indirect_grp.c header file
  Function List :
  History       :
  1.Date        : 2018/3/27
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

#ifndef __DP_PIPE_INDIRECT_GRP_H__
#define __DP_PIPE_INDIRECT_GRP_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

 OFDPA_ERROR_t dpGrpL2IntfBuktBuild(ofdpaL2InterfaceGroupBucketData_t       * pData,ofdpaActBucket_t	**ppBucket);
 OFDPA_ERROR_t dpGrpMplsIntfBuktBuild(ofdpaMPLSInterfaceGroupBucketData_t       * pData,ofdpaActBucket_t	**ppBucket);
 OFDPA_ERROR_t dpGrpMplsLabelBuktBuild(ofdpaMPLSLabelGroupBucketData_t       * pData, ofdpaActBucket_t	**ppBucket);
 OFDPA_ERROR_t indirectGroupAdd(ofdpaGroupEntry_t *group);
 OFDPA_ERROR_t indirectGroupBucketEntryAdd(ofdpaGroupBucketEntry_t *groupBucket);
 OFDPA_ERROR_t indirectGroupBucketEntryDelete(uint32_t groupId, uint32_t bucketIndex);
 OFDPA_ERROR_t indirectGroupBucketEntryModify(ofdpaGroupBucketEntry_t *groupBucket);
 OFDPA_ERROR_t indirectGroupBucketsDeleteAll(uint32_t groupId);
 OFDPA_ERROR_t indirectGroupDelete(uint32_t groupId);
 void indirectGrpPktProcess( dpGrpWork_t *work);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __DP_PIPE_INDIRECT_GRP_H__ */
