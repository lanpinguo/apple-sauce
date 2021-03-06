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


#define ACT_PRINT_FMT_SPLIT_LINE	"| "



static char *mplsTypeSubTypeName[] =
{
  [OFDPA_MPLS_TYPE_VPWS         ] = "VPWS",
  [OFDPA_MPLS_TYPE_VPLS         ] = "VPLS",
  [OFDPA_MPLS_TYPE_OAM          ] = "OAM",
  [OFDPA_MPLS_TYPE_L3_UNICAST   ] = "L3 Route Unicast",
  [OFDPA_MPLS_TYPE_L3_MULTICAST ] = "L3 Route Multicast",
  [OFDPA_MPLS_TYPE_L3_PHP       ] = "L3 PHP",
};




uint64_t ofdpaActSetGrpId(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_ERROR_t rc = OFDPA_E_NONE;


	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"groupID = 0x%08x", (uint32_t)pObj->arg);

	}										 

	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		ofdpaGroupEntry_t group;
		
		rc = dpGroupGet((uint32_t)pObj->arg, &group);
		if(rc != OFDPA_E_NONE){
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
												 "group %d not found!\r\n", (uint32_t)pObj->arg);
			pPkt->meta_data.pGrpInst = NULL;
			return rc;
		}

		pPkt->meta_data.pGrpInst = group.ptrGrpInst;

	}	


	return rc;
}


uint64_t ofdpaActSetQosIndex(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"QosIndex = %d", (uint32_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetVlanId(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	ofdpaVlan_t *vlan = NULL;

	
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, \ 
											ACT_PRINT_FMT_SPLIT_LINE"newVlanId = 0x%04x (VLAN %d)", \
											REORDER16_B2L((uint16_t)pObj->arg), REORDER16_B2L((uint16_t)pObj->arg));

	}				

	
	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		vlan = DP_GET_FEILD(pPkt,FEILD_VLAN_0);
		if(vlan){
			vlan->vid = pObj->arg;
		}
		
	}	
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetVlanId2(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize,ACT_PRINT_FMT_SPLIT_LINE"newVlanId2 = 0x%04x (VLAN %d)",(uint16_t)pObj->arg, pObj->arg	& OFDPA_VID_EXACT_MASK);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActPushVlan(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, \
										ACT_PRINT_FMT_SPLIT_LINE"pushVlan(TPID = 0x%x)", \
										REORDER16_B2L((uint16_t)pObj->arg));
	}		



	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		ofdpaVlan_t *vlan = NULL;

		/* Update packet length*/
		pPkt->pkt_len += DP_VLAN_HDR_LEN;

		
		int newVlanBase = dpMallocMemFromPktPool(pPkt, DP_VLAN_HDR_LEN);
		if(newVlanBase){
			if(!DP_IS_FEILD_VALID(pPkt, FEILD_VLAN_0)){
				SET_FEILD_OFFSET(pPkt, FEILD_VLAN_0,newVlanBase);
				vlan = DP_GET_FEILD(pPkt,FEILD_VLAN_0);
			}
			else if(!DP_IS_FEILD_VALID(pPkt, FEILD_VLAN_1)) {
				SET_FEILD_OFFSET(pPkt, FEILD_VLAN_1,newVlanBase);
				vlan = DP_GET_FEILD(pPkt,FEILD_VLAN_1);
			}
			else{
				return OFDPA_E_INTERNAL;
			}

			if(vlan){
				vlan->type = (uint16_t)pObj->arg;
			}
		}
		else
		{
			return OFDPA_E_FAIL;
		}
	}	

	return OFDPA_E_NONE;
}

uint64_t ofdpaActPushVlan2(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"newTpid2 = 0x%x", (uint16_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetMetaDataOvid(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"ovid = %d", (uint16_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMetaDataTunId(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", pObj->arg);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"tunnelId = 0x%x", (uint32_t)pObj->arg);

	}										 

	pPkt = (ofdpaPktCb_t *)arg->data;
	pPkt->meta_data.tunnelId = (uint32_t)pObj->arg;
	
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMetaDataVrf(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"vrf = %d", (uint32_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMetaDataMplsL2Port(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", pObj->arg);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize,  ACT_PRINT_FMT_SPLIT_LINE"mplsL2Port = 0x%x", (uint32_t)pObj->arg);

	}										 


	pPkt = (ofdpaPktCb_t *)arg->data;


	pPkt->meta_data.mplsL2Port = (uint32_t)pObj->arg;

										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMetaDataMplsType(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	ofdpaPktCb_t *pPkt;

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "arg = %lld \r\n", pObj->arg);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"mplsType = %d (%s)", (uint32_t)pObj->arg, mplsTypeSubTypeName[(uint32_t)pObj->arg] );

	}										 

	pPkt = (ofdpaPktCb_t *)arg->data;
	pPkt->meta_data.mplsType = (uint32_t)pObj->arg;
	
	return OFDPA_E_NONE;
}

uint64_t ofdpaActIncClassBasedCounter(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"classBasedCountId = %d", (uint32_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActIdentifyOutPort(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;
		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"outputPort = %d", (uint32_t)pObj->arg);
	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActPopVlan(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"popVlanAction");

	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActAllowVlanTrans(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;
		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"allowVlanTranslation ");
	}										 
	return OFDPA_E_NONE;
}


uint64_t ofdpaActOamLmTxCount(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);

	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"oamLmTxCountAction = %d ", (uint32_t)pObj->arg);

	}										 

	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetDstMac(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;
		ofdpaMacAddr_t *pMac;
		pMac = (ofdpaMacAddr_t *)&pObj->arg;
		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"dstMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ", 
										pMac->addr[0],
										pMac->addr[1],
										pMac->addr[2],
										pMac->addr[3],
										pMac->addr[4],
										pMac->addr[5]);
	}

	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		ofdpaMacAddr_t *pMac = NULL;
		
		/* Update packet length*/

		if(DP_IS_FEILD_VALID(pPkt, FEILD_DMAC)){
			pMac = DP_GET_FEILD_ADDR(pPkt, FEILD_DMAC);
		}
		else{
			return OFDPA_E_INTERNAL;
		}
	
		if(pMac){
			memcpy(pMac,&pObj->arg,sizeof(ofdpaMacAddr_t)); 
		}
	}
	
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetSrcMac(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;
		ofdpaMacAddr_t *pMac;
		pMac = (ofdpaMacAddr_t *)&pObj->arg;
		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"srcMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ", 
										pMac->addr[0],
										pMac->addr[1],
										pMac->addr[2],
										pMac->addr[3],
										pMac->addr[4],
										pMac->addr[5]);
	}										 

	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		ofdpaMacAddr_t *pMac = NULL;
		
		/* Update packet length*/

		if(DP_IS_FEILD_VALID(pPkt, FEILD_SMAC)){
			pMac = DP_GET_FEILD_ADDR(pPkt, FEILD_SMAC);
		}
		else{
			return OFDPA_E_INTERNAL;
		}
	
		if(pMac){
			memcpy(pMac,&pObj->arg,sizeof(ofdpaMacAddr_t)); 
		}
	}


	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetLmepId(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"lmepIdAction = %d ", (uint32_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActIncColorBasedCount(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActPushL2Hdr(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_ERROR_t rc = OFDPA_E_NONE;
	int newL2HdrBase;

	
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"pushL2Hdr ");

	}		
	
	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		int pktStart = GET_PKT_START(pPkt);
		CLEAN_FEILD_ALL(pPkt);
		/* Update packet length*/
		UPDATE_DATA_OFFSET(pPkt, pktStart, pPkt->pkt_len);
		pPkt->pkt_len += DP_L2_HDR_LEN;
		
		newL2HdrBase = dpMallocMemFromPktPool(pPkt, DP_L2_HDR_LEN);
		if(newL2HdrBase){
			SET_FEILD_OFFSET(pPkt, FEILD_DMAC,newL2HdrBase);
			SET_FEILD_OFFSET(pPkt, FEILD_SMAC,newL2HdrBase + 6);
			SET_FEILD_OFFSET(pPkt, FEILD_L3_TYPE,newL2HdrBase + 12);

		}
		else
		{
			return OFDPA_E_FAIL;
		}
	}	


	return rc;
}


uint64_t ofdpaActSetTpid(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	return OFDPA_E_NONE;
}

uint64_t ofdpaActPushMplsHdr(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize,\
										ACT_PRINT_FMT_SPLIT_LINE"pushMplsHdr(0x%x) ",
										REORDER16_B2L((uint16_t)pObj->arg));

	}	

	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		uint16_t *type = NULL;

		/* Update packet length*/
		pPkt->pkt_len += DP_MPLS_HDR_LEN;

		
		int newMplsBase = dpMallocMemFromPktPool(pPkt, DP_MPLS_HDR_LEN);
		if(newMplsBase){
			if(!DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_0)){
				SET_FEILD_OFFSET(pPkt, FEILD_MPLS_0,newMplsBase);
			}
			else if(!DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_1)){
				SET_FEILD_OFFSET(pPkt, FEILD_MPLS_1,newMplsBase);
			}
			else if(!DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_2)){
				SET_FEILD_OFFSET(pPkt, FEILD_MPLS_2,newMplsBase);
			}			
			else{
				return OFDPA_E_INTERNAL;
			}

			/* Update ethernet type*/
			type = DP_GET_FEILD_ADDR(pPkt, FEILD_L3_TYPE);
			if(type){
				*type = (uint16_t)pObj->arg;
			}
			else{
				return OFDPA_E_INTERNAL;
			}
		}
		else
		{
			return OFDPA_E_FAIL;
		}
	}	
	
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetEtherType(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	return OFDPA_E_NONE;
}


uint64_t ofdpaActPushCw(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"pushCW ");

	}		

	
	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		uint16_t *type = NULL;

		/* Update packet length*/
		pPkt->pkt_len += DP_MPLS_CW_LEN;

		
		int newMplsBase = dpMallocMemFromPktPool(pPkt, DP_MPLS_CW_LEN);
		if(newMplsBase){
			if(!DP_IS_FEILD_VALID(pPkt, FEILD_CW)){
				SET_FEILD_OFFSET(pPkt, FEILD_CW,newMplsBase);
			}
			else{
				return OFDPA_E_INTERNAL;
			}

		}
		else
		{
			return OFDPA_E_FAIL;
		}
	}	
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMplsLabel(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, \
										ACT_PRINT_FMT_SPLIT_LINE"mplsLabel = %d(0x%x) ",\
										REORDER32_B2L((uint32_t)pObj->arg)>>12,
										REORDER32_B2L((uint32_t)pObj->arg)>>12);

	}		

	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		ofdpaMpls_t *mpls = NULL;
		
		/* Update packet length*/

		if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_2)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_2);
		}
		else if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_1)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_1);
		}
		else if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_0)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_0);
		} 		
		else{
			return OFDPA_E_INTERNAL;
		}
	
		if(mpls){
			mpls->mpls_head &=	REORDER32_L2B(~(0xFFFFF << 12)) ; 
			mpls->mpls_head |=	(uint32_t)pObj->arg; 
		}
	}
	
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetMplsBos(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, \
										ACT_PRINT_FMT_SPLIT_LINE"mplsBOS = %d ",\
										REORDER32_B2L(((uint32_t)pObj->arg))>>8);

	}										 


	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		ofdpaMpls_t *mpls = NULL;
		
		/* Update packet length*/

		if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_2)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_2);
		}
		else if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_1)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_1);
		}
		else if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_0)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_0);
		} 		
		else{
			return OFDPA_E_INTERNAL;
		}
	
		if(mpls){
			mpls->mpls_head &=	REORDER32_L2B(~(1 << 8)) ; 
			mpls->mpls_head |=	(uint32_t)pObj->arg; 
		}
	}
	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMplsTtl(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize,\
										ACT_PRINT_FMT_SPLIT_LINE"mplsTTL = %d ",\
										REORDER32_B2L((uint32_t)pObj->arg) & 0xFF);

	}										 


	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		ofdpaMpls_t *mpls = NULL;
		
		/* Update packet length*/

		if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_2)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_2);
		}
		else if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_1)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_1);
		}
		else if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_0)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_0);
		} 		
		else{
			return OFDPA_E_INTERNAL;
		}
	
		if(mpls){
			mpls->mpls_head &=	REORDER32_L2B(~0xFF) ; 
			mpls->mpls_head |=	(uint32_t)pObj->arg; 
		}
	}


	return OFDPA_E_NONE;
}


uint64_t ofdpaActSetMplsExp(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize,\
										ACT_PRINT_FMT_SPLIT_LINE"mplsEXP = %d ",\
										REORDER32_B2L(((uint32_t)pObj->arg))>>9);

	}	
	
	if(ACT_OP_TYPE_EXECUTE == arg->type){
		ofdpaPktCb_t *pPkt = arg->data;
		ofdpaMpls_t *mpls = NULL;
		
		/* Update packet length*/

		if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_2)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_2);
		}
		else if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_1)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_1);
		}
		else if(DP_IS_FEILD_VALID(pPkt, FEILD_MPLS_0)){
			mpls = DP_GET_FEILD_ADDR(pPkt, FEILD_MPLS_0);
		} 		
		else{
			return OFDPA_E_INTERNAL;
		}
	
		if(mpls){
			mpls->mpls_head &=	REORDER32_L2B(~(0x7 << 9)) ; 
			mpls->mpls_head |=	(uint32_t)pObj->arg; 
		}
	}

	return OFDPA_E_NONE;
}


uint64_t ofdpaActCpyMplsExpOutwards(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"CopyMplsExpOutwards ");

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetRemarkTableId(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"RemarkTableId = %d ",(uint32_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActCpyMplsTtlOutwards(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"CopyMplsTtlOutwards ");

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetVlanPcp(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"vlanPCP = %d ",(uint8_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetVlanDei(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"vlanDEI = %d ",(uint8_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}
uint64_t ofdpaActSetDscp(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"ipDSCP = %d ",(uint8_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetMplsExpRemarkTable(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"mplsExpRemarkTable = %d ",(uint32_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}

uint64_t ofdpaActSetPcpPriRemarkTable(void *this,ofdpaActArg_t *arg)
{
	ofdpaAct_t *pObj = this;
	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_API, OFDPA_DEBUG_BASIC,
										 "%p!\r\n", this);
	if(ACT_OP_TYPE_PRETTY_PRINT == arg->type){
		ofdpaActPrintBuf_t *pBuf = arg->data;

		return snprintf(pBuf->buf, pBuf->bufSize, ACT_PRINT_FMT_SPLIT_LINE"vlanPcpPriRemarkTable = %d ",(uint32_t)pObj->arg);

	}										 
	return OFDPA_E_NONE;
}





/*
***************************************** end of file *************************************************
*/
