/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : dp_pipe_act_func_list.c
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/2/26
  Last Modified :
  Description   : action function list
  Function List :
              dpActAllowVlanTrans
              dpActIdentifyOutPort
              dpActPopVlan
  History       :
  1.Date        : 2018/2/26
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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include "ofdpa_datatypes.h"
#include "ofdb.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"
#include "datapath.h"




typedef struct dpActionFuncMap_s
{
	OFDPA_FLOW_ACTION_TYPE_t 	type;
	ofdpaAct_f								func;
}dpActionFuncMap_t;



dpActionFuncMap_t	actMap[OFDPA_ActCount] = {

	[OFDPA_ActAllowVlanTrans]			      = {.type = OFDPA_ActAllowVlanTrans        , .func = dpActAllowVlanTrans },
	[OFDPA_ActCpyMplsExpOutwards]       = {.type = OFDPA_ActCpyMplsExpOutwards    , .func = dpActCpyMplsExpOutwards },
	[OFDPA_ActCpyMplsTtlOutwards]       = {.type = OFDPA_ActCpyMplsTtlOutwards    , .func = dpActCpyMplsTtlOutwards },
	[OFDPA_ActIdentifyOutPort]          = {.type = OFDPA_ActIdentifyOutPort       , .func = dpActIdentifyOutPort },
	[OFDPA_ActIncClassBasedCounter]     = {.type = OFDPA_ActIncClassBasedCounter  , .func = dpActIncClassBasedCounter },
	[OFDPA_ActIncColorBasedCount]       = {.type = OFDPA_ActIncColorBasedCount    , .func = dpActIncColorBasedCount },
	[OFDPA_ActOamLmTxCount]             = {.type = OFDPA_ActOamLmTxCount          , .func = dpActOamLmTxCount },
	[OFDPA_ActPopVlan]                  = {.type = OFDPA_ActPopVlan               , .func = dpActPopVlan },
	[OFDPA_ActPushL2Hdr]                = {.type = OFDPA_ActPushL2Hdr             , .func = dpActPushL2Hdr },
	[OFDPA_ActPushMplsCw]               = {.type = OFDPA_ActPushMplsCw            , .func = dpActPushMplsCw },
	[OFDPA_ActPushMplsHdr]              = {.type = OFDPA_ActPushMplsHdr           , .func = dpActPushMplsHdr },
	[OFDPA_ActPushVlan]                 = {.type = OFDPA_ActPushVlan              , .func = dpActPushVlan },
	[OFDPA_ActPushVlan2] 								= {.type = OFDPA_ActPushVlan2							, .func = dpActPushVlan2 },
	[OFDPA_ActSetDstMac]                = {.type = OFDPA_ActSetDstMac             , .func = dpActSetDstMac },
	[OFDPA_ActSetEtherType]             = {.type = OFDPA_ActSetEtherType          , .func = dpActSetEtherType },
	[OFDPA_ActSetGrpId]                 = {.type = OFDPA_ActSetGrpId              , .func = dpActSetGrpId },
	[OFDPA_ActSetLmepId]                = {.type = OFDPA_ActSetLmepId             , .func = dpActSetLmepId },
	[OFDPA_ActSetMetaDataMplsL2Port]    = {.type = OFDPA_ActSetMetaDataMplsL2Port , .func = dpActSetMetaDataMplsL2Port},
	[OFDPA_ActSetMetaDataMplsType]      = {.type = OFDPA_ActSetMetaDataMplsType   , .func = dpActSetMetaDataMplsType },
	[OFDPA_ActSetMetaDataOvid]          = {.type = OFDPA_ActSetMetaDataOvid       , .func = dpActSetMetaDataOvid },
	[OFDPA_ActSetMetaDataTunId]         = {.type = OFDPA_ActSetMetaDataTunId      , .func = dpActSetMetaDataTunId },
	[OFDPA_ActSetMetaDataVrf]           = {.type = OFDPA_ActSetMetaDataVrf        , .func = dpActSetMetaDataVrf },
	[OFDPA_ActSetMplsBos]               = {.type = OFDPA_ActSetMplsBos            , .func = dpActSetMplsBos },
	[OFDPA_ActSetMplsExp]               = {.type = OFDPA_ActSetMplsExp            , .func = dpActSetMplsExp },
	[OFDPA_ActSetMplsLabel]             = {.type = OFDPA_ActSetMplsLabel          , .func = dpActSetMplsLabel },
	[OFDPA_ActSetMplsTtl]               = {.type = OFDPA_ActSetMplsTtl            , .func = dpActSetMplsTtl },
	[OFDPA_ActSetQosIndex]              = {.type = OFDPA_ActSetQosIndex           , .func = dpActSetQosIndex },
	[OFDPA_ActSetRemarkTableId]         = {.type = OFDPA_ActSetRemarkTableId      , .func = dpActSetRemarkTableId },
	[OFDPA_ActSetSrcMac]                = {.type = OFDPA_ActSetSrcMac             , .func = dpActSetSrcMac },
	[OFDPA_ActSetTpid]                  = {.type = OFDPA_ActSetTpid               , .func = dpActSetTpid },
	[OFDPA_ActSetVlanId]                = {.type = OFDPA_ActSetVlanId             , .func = dpActSetVlanId },
	[OFDPA_ActSetVlanId2]               = {.type = OFDPA_ActSetVlanId2            , .func = dpActSetVlanId2 },

};



static char *mplsTypeSubTypeName[] =
{
  [OFDPA_MPLS_TYPE_VPWS         ] = "VPWS",
  [OFDPA_MPLS_TYPE_VPLS         ] = "VPLS",
  [OFDPA_MPLS_TYPE_OAM          ] = "OAM",
  [OFDPA_MPLS_TYPE_L3_UNICAST   ] = "L3 Route Unicast",
  [OFDPA_MPLS_TYPE_L3_MULTICAST ] = "L3 Route Multicast",
  [OFDPA_MPLS_TYPE_L3_PHP       ] = "L3 PHP",
};



ofdpaAct_f dpActGetFuncFromType(OFDPA_FLOW_ACTION_TYPE_t t)
{

	if(t >= OFDPA_ActCount){
		return NULL;
	}

	return actMap[t].func;

}

uint64_t dpActSetGrpId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActSetQosIndex(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActSetVlanId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " newVlanId = 0x%04x (VLAN %d)", (uint16_t)arg, arg	& OFDPA_VID_EXACT_MASK);

	}										 
	return OFDPA_E_NONE;
}

uint64_t dpActSetVlanId2(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize," newVlanId2 = 0x%04x (VLAN %d)",(uint16_t)arg, arg	& OFDPA_VID_EXACT_MASK);

	}										 
	return OFDPA_E_NONE;
}

uint64_t dpActPushVlan(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t dpActPushVlan2(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " newTpid2 = 0x%x", (uint16_t)arg);

	}										 
	return OFDPA_E_NONE;
}

uint64_t dpActSetMetaDataOvid(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " ovid = %d", (uint16_t)arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t dpActSetMetaDataTunId(void *this,void *pcb, uint64_t arg)
{
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", arg);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize,  " tunnelId = 0x%x", (uint32_t)arg);

	}										 

	pPkt = (ofdpaPktCb_t *)pcb;
	pPkt->meta_data.tunnelId = (uint32_t)arg;
	
	return OFDPA_E_NONE;
}


uint64_t dpActSetMetaDataVrf(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " vrf = %d", (uint32_t)arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t dpActSetMetaDataMplsL2Port(void *this,void *pcb, uint64_t arg)
{
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", arg);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize,  " mplsL2Port = 0x%x", (uint32_t)arg);

	}										 


	pPkt = (ofdpaPktCb_t *)pcb;


	pPkt->meta_data.mplsL2Port = (uint32_t)arg;

										 
	return OFDPA_E_NONE;
}


uint64_t dpActSetMetaDataMplsType(void *this,void *pcb, uint64_t arg)
{
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", arg);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " mplsType = %d (%s)", (uint32_t)arg, mplsTypeSubTypeName[(uint32_t)arg] );

	}										 

	pPkt = (ofdpaPktCb_t *)pcb;
	pPkt->meta_data.mplsType = (uint32_t)arg;
	
	return OFDPA_E_NONE;
}

uint64_t dpActIncClassBasedCounter(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " classBasedCountId = %d", (uint32_t)arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t dpActIdentifyOutPort(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t dpActPopVlan(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		dpActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " popVlanAction");

	}										 
	return OFDPA_E_NONE;
}


uint64_t dpActAllowVlanTrans(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActOamLmTxCount(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t dpActSetDstMac(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActSetSrcMac(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActSetLmepId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t dpActIncColorBasedCount(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t dpActPushL2Hdr(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActSetTpid(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t dpActPushMplsHdr(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActSetEtherType(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActPushMplsCw(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActSetMplsLabel(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t dpActSetMplsBos(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActSetMplsTtl(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActSetMplsExp(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t dpActCpyMplsExpOutwards(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t dpActSetRemarkTableId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t dpActCpyMplsTtlOutwards(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/
