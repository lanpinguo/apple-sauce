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
              ofdpaActAllowVlanTrans
              ofdpaActIdentifyOutPort
              ofdpaActPopVlan
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






static char *mplsTypeSubTypeName[] =
{
  [OFDPA_MPLS_TYPE_VPWS         ] = "VPWS",
  [OFDPA_MPLS_TYPE_VPLS         ] = "VPLS",
  [OFDPA_MPLS_TYPE_OAM          ] = "OAM",
  [OFDPA_MPLS_TYPE_L3_UNICAST   ] = "L3 Route Unicast",
  [OFDPA_MPLS_TYPE_L3_MULTICAST ] = "L3 Route Multicast",
  [OFDPA_MPLS_TYPE_L3_PHP       ] = "L3 PHP",
};




uint64_t ofdpaActSetGrpId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetQosIndex(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetVlanId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " newVlanId = 0x%04x (VLAN %d)", (uint16_t)arg, arg	& OFDPA_VID_EXACT_MASK);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetVlanId2(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize," newVlanId2 = 0x%04x (VLAN %d)",(uint16_t)arg, arg	& OFDPA_VID_EXACT_MASK);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActPushVlan(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActPushVlan2(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " newTpid2 = 0x%x", (uint16_t)arg);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetMetaDataOvid(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " ovid = %d", (uint16_t)arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMetaDataTunId(void *this,void *pcb, uint64_t arg)
{
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", arg);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize,  " tunnelId = 0x%x", (uint32_t)arg);

	}										 

	pPkt = (ofdpaPktCb_t *)pcb;
	pPkt->meta_data.tunnelId = (uint32_t)arg;
	
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMetaDataVrf(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " vrf = %d", (uint32_t)arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMetaDataMplsL2Port(void *this,void *pcb, uint64_t arg)
{
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", arg);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize,  " mplsL2Port = 0x%x", (uint32_t)arg);

	}										 


	pPkt = (ofdpaPktCb_t *)pcb;


	pPkt->meta_data.mplsL2Port = (uint32_t)arg;

										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMetaDataMplsType(void *this,void *pcb, uint64_t arg)
{
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", arg);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " mplsType = %d (%s)", (uint32_t)arg, mplsTypeSubTypeName[(uint32_t)arg] );

	}										 

	pPkt = (ofdpaPktCb_t *)pcb;
	pPkt->meta_data.mplsType = (uint32_t)arg;
	
	return OFDPA_E_NONE;
}

uint64_t ofdpaActIncClassBasedCounter(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " classBasedCountId = %d", (uint32_t)arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActIdentifyOutPort(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActPopVlan(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	if((pcb == NULL) && (this != NULL)){
		ofdpaActionFuncOpt_t *pOps = this;

		return snprintf(pOps->buf, pOps->bufSize, " popVlanAction");

	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActAllowVlanTrans(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActOamLmTxCount(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetDstMac(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetSrcMac(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetLmepId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActIncColorBasedCount(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActPushL2Hdr(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetTpid(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActPushMplsHdr(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetEtherType(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActPushMplsCw(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMplsLabel(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetMplsBos(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMplsTtl(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMplsExp(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActCpyMplsExpOutwards(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetRemarkTableId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActCpyMplsTtlOutwards(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/
