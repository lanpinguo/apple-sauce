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



OFDPA_ERROR_t dpActSetGrpId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetQosIndex(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetVlanId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActSetVlanId2(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActPushVlan(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetMetaDataOvid(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetMetaDataTunId(void *this,void *pcb, uint64_t arg)
{
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", arg);

	pPkt = (ofdpaPktCb_t *)pcb;
	pPkt->meta_data.tunnelId = (uint32_t)arg;
	
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetMetaDataVrf(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetMetaDataMplsL2Port(void *this,void *pcb, uint64_t arg)
{
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", arg);

	pPkt = (ofdpaPktCb_t *)pcb;


	pPkt->meta_data.mplsL2Port = (uint32_t)arg;

										 
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetMetaDataMplsType(void *this,void *pcb, uint64_t arg)
{
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", arg);

	pPkt = (ofdpaPktCb_t *)pcb;
	pPkt->meta_data.mplsType = (uint32_t)arg;
	
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActIncClassBasedCounter(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActIdentifyOutPort(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActPopVlan(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActAllowVlanTrans(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActOamLmTxCount(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActSetDstMac(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetSrcMac(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetLmepId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActIncColorBasedCount(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActPushL2Hdr(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetTpid(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActPushMplsHdr(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetEtherType(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActPushMplsCw(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetMplsLabel(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActSetMplsBos(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetMplsTtl(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActSetMplsExp(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}


OFDPA_ERROR_t dpActCpyMplsExpOutwards(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActSetRemarkTableId(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

OFDPA_ERROR_t dpActCpyMplsTtlOutwards(void *this,void *pcb, uint64_t arg)
{
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%s!\r\n", __FUNCTION__);
	return OFDPA_E_NONE;
}

/*
***************************************** end of file *************************************************
*/
