/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
* @filename  g8131_api.c
*
* @purpose   G8131 APIs
*
* @component g8131
*
* @comments  none
*
* @end
**********************************************************************/
#include <string.h>
#include "g8131_include.h"
#include "g8131_api.h"
#include "ofdb_api.h"
//#include "driver_group.h"
#include "datapath.h"

extern g8131Cfg_t      *g8131Cfg;
extern ofdpaRWlock_t    g8131RWLock;

static OFDPA_ERROR_t g8131PgGlbParamCheck(uint32_t itemType,
                                          uint32_t itemValue)
{
  switch (itemType)
  {
    case G8131_GLB_CMN_TYPE_ENABLE:
      G8131_PG_BOOL_VALUE_CHECK(itemValue);
      break;

      /*Ignore other parameters' validation.*/
    default:
      break;
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Create G8131 Protection group
*
* @param    grpId                 @b{(input)} identifier for protection group entry
* @param    ffGroupId             @b{(input)} fastfailover group ID associated with this entry
* @param    bucketIndexWorking    @b{(input)} bucket index within fastfailover group for working path
* @param    bucketIndexProtection @b{(input)} bucket index within fastfailover group for protection path

*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments  none
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgCreate(uint32_t grpId,
                            uint32_t ffGroupId,
                            uint32_t livenessPortIdWorking,
                            uint32_t livenessPortIdProtection,
                            uint32_t primVidWorking,
                            uint32_t primVidProtection,
                            uint32_t lmepIdWorking,
                            uint32_t lmepIdProtection)
{
  OFDPA_ERROR_t status = OFDPA_E_FAIL;
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  pgOperData_t  *pgNode;
  g8131ReqMsg_t msg;
  ofdbPortInfo_t portInfo;
  OFDB_ENTRY_FLAG_t flags;
  uint32_t        failOverValue;
  ofdpaOamMepConfig_t config;

  G8131_PG_CHECK(grpId);

  dpaRWlockWritelock(&g8131RWLock);

  if (g8131Cfg->pgData[grpId].inUse == OFDPA_FALSE)
  {
    pgNode = &g8131Cfg->pgData[grpId];

    memset(pgNode, 0, sizeof(*pgNode));

    pgNode->grpId = grpId;

    pgNode->ffGroupId                = ffGroupId;
    pgNode->livenessPortIdWorking    = livenessPortIdWorking;
    pgNode->livenessPortIdProtection = livenessPortIdProtection;
    pgNode->primVidWorking           = primVidWorking;
    pgNode->primVidProtection        = primVidProtection;
    pgNode->lmepIdWorking            = lmepIdWorking;
    pgNode->lmepIdProtection         = lmepIdProtection;
    pgNode->livenessPortIdActive     = livenessPortIdWorking;

    pgNode->smState       = G8131_STATE_A_NR_NULL;
    pgNode->lastLReq      = G8131_LR_M_CLEAR;
    pgNode->lastRReq      = G8131_RR_AA_NR_NULL;
    pgNode->lastSReq      = G8131_RR_AA_NR_NULL;

    pgNode->sfwState      = OFDPA_FALSE;
    pgNode->sfpState      = OFDPA_FALSE;
    pgNode->sdwState      = OFDPA_FALSE;
    pgNode->sdpState      = OFDPA_FALSE;

    pgNode->wtrCntValue   = 0;
    pgNode->holdOffBlock  = OFDPA_FALSE;

    pgNode->holdOffTime    = G8131_HOLD_OFF_DEF_VALUE;
    pgNode->wtrTime        = G8131_WTR_DEF_VALUE;
    pgNode->revertiveMode  = OFDPA_FALSE;

    pgNode->apsSendTmNode = NULL;
    pgNode->holdOffTmNode = NULL;
    pgNode->alarmCMTmNode = NULL;
    pgNode->alarmNRTmNode = NULL;
    pgNode->alarmTOTmNode = NULL;

	pgNode->quickswitch = 1;
    pgNode->sfwStateFast = OFDPA_FALSE;
    pgNode->sfpStateFast = OFDPA_FALSE;

    pgNode->inUse         = OFDPA_TRUE;

    /* BEGIN: Modified by hushouqiang, 2017/1/19 */
    if(ffGroupId != OFDPA_INVALID_GROUP_ID)
    {
      /* Get Fast failover group state */
    }
    /* END:   Modified by hushouqiang, 2017/1/19 */
    /* update liveness port state */
    g8131UpdateLivenessPortState(&g8131Cfg->pgData[grpId]);

    status = OFDPA_E_NONE;
  }
  else
  {
    /*The G8131 table entry already in use */
    status = OFDPA_E_EXISTS;
  }
  dpaRWlockUnlock(&g8131RWLock);

  _G8131_RETURN_CODE_CHECK(status);

  /*Trigger the state machine.*/
  memset(&msg, 0, sizeof(msg));
  msg.grpId = grpId;
  msg.request = G8131_LR_M_CLEAR;
  status = g8131IssueCmd(G8131_EVENT_PG_CREATE, 0, &msg);
  _G8131_RETURN_CODE_CHECK(status);

  /*leishenghua modify 20170119, 创建保护组后，重新生成endpoint,
   解决先创建endpoint 后创建保护组，告警丢失问题*/
  rc = ofdbOamMepGet(lmepIdWorking,&config,NULL);
  if (rc == OFDPA_E_NONE)
  {
  }
  
  
  rc = ofdbOamMepGet(lmepIdProtection,&config,NULL);
  if (rc == OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       " megIndex = %d\r\n",config.megIndex);
  }
  

  return status;
}

/*********************************************************************
*
* @purpose  Delete G8131 Protection group
*
* @param    grpId        @b{(input)} protection group ID
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments  none
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgDelete(uint32_t grpId)
{
  OFDPA_ERROR_t status;
  g8131ReqMsg_t msg;

#if 0
  /*Trigger the state machine with G8131_EVENT_REQUEST(G8131_LR_A_LOCKOUT).*/
  memset(&msg, 0, sizeof(msg));
  msg.grpId   = grpId;
  msg.request = G8131_LR_A_LOCKOUT;
  status = g8131IssueCmd(G8131_EVENT_REQUEST, 0, &msg);
  _G8131_RETURN_CODE_CHECK(status);
#endif
  /*Trigger the state machine with event G8131_EVENT_PG_DELETE.*/
  memset(&msg, 0, sizeof(msg));
  msg.grpId = grpId;
  /* BEGIN: Modified by Hushouqiang, 2017/1/16   问题单号:DEBUG_PROTECTION */
  status = g8131IssueCmdSync(G8131_EVENT_PG_DELETE, 0, &msg);
  /* END:   Modified by Hushouqiang, 2017/1/16 */
  _G8131_RETURN_CODE_CHECK(status);

  return status;
}

/*********************************************************************
*
* @purpose  Get G8131 Protection group information per-group.
*
* @param    grpId         @b{(input)}
* @param    itemKey       @b{(input)}
* @param    itemValue     @b{(output)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgCommonGet(uint32_t  grpId,
                               uint32_t  itemType,
                               uint32_t *itemValue)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;

  G8131_PG_CHECK(grpId);

  dpaRWlockReadlock(&g8131RWLock);
  if (g8131Cfg->pgData[grpId].inUse == OFDPA_TRUE)
  {
    switch (itemType)
    {
      case G8131_CMN_TYPE_HOLDOFF_VALUE:
        *itemValue = g8131Cfg->pgData[grpId].holdOffTime;
        break;

      case G8131_CMN_TYPE_WTR_VALUE:
        *itemValue = g8131Cfg->pgData[grpId].wtrTime;
        break;

      case G8131_CMN_TYPE_REVERT_MODE:
        *itemValue = g8131Cfg->pgData[grpId].revertiveMode;
        break;

      case G8131_CMN_TYPE_LAST_LR:
        *itemValue = g8131Cfg->pgData[grpId].lastLReq;
        break;

      case G8131_CMN_TYPE_LAST_RR:
        *itemValue = g8131Cfg->pgData[grpId].lastRReq;
        break;

      case G8131_CMN_TYPE_LAST_SR:
        *itemValue = g8131Cfg->pgData[grpId].lastSReq;
        break;

      case G8131_CMN_TYPE_LOCAL_SF_W_EXIST:
        *itemValue = g8131Cfg->pgData[grpId].sfwState;
        break;

      case G8131_CMN_TYPE_LOCAL_SF_P_EXIST:
        *itemValue = g8131Cfg->pgData[grpId].sfpState;
        break;

      case G8131_CMN_TYPE_LOCAL_SD_W_EXIST:
        *itemValue = g8131Cfg->pgData[grpId].sdwState;
        break;

      case G8131_CMN_TYPE_LOCAL_SD_P_EXIST:
        *itemValue = g8131Cfg->pgData[grpId].sdpState;
        break;

      case G8131_CMN_TYPE_ALARM_CM:
        *itemValue = (uint32_t)g8131Cfg->pgData[grpId].dfop_cm;
        break;

      case G8131_CMN_TYPE_ALARM_PM:
        *itemValue = (uint32_t)g8131Cfg->pgData[grpId].dfop_pm;
        break;

      case G8131_CMN_TYPE_ALARM_NR:
        *itemValue = (uint32_t)g8131Cfg->pgData[grpId].dfop_nr;
        break;

      case G8131_CMN_TYPE_ALARM_TO:
        *itemValue = (uint32_t)g8131Cfg->pgData[grpId].dfop_to;
        break;

      default:
        /*Configuration parameter not supported.*/
        status = OFDPA_E_FAIL;
        break;
    }
  }
  else
  {
    status = OFDPA_E_NOT_FOUND;
  }

  dpaRWlockUnlock(&g8131RWLock);

  return status;
}

/*********************************************************************
*
* @purpose  Set G8131 Protection group information per-group.
*
* @param    grpId         @b{(input)}
* @param    itemKey       @b{(input)}
* @param    itemValue     @b{(output)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgCommonSet(uint32_t  grpId,
                               uint32_t  itemType,
                               uint32_t  itemValue)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;

  G8131_PG_CHECK(grpId);

  dpaRWlockReadlock(&g8131RWLock);
  if (g8131Cfg->pgData[grpId].inUse == OFDPA_TRUE)
  {
    switch (itemType)
    {
      case G8131_CMN_TYPE_HOLDOFF_VALUE:
         g8131Cfg->pgData[grpId].holdOffTime = itemValue;
        break;

      case G8131_CMN_TYPE_WTR_VALUE:
        g8131Cfg->pgData[grpId].wtrTime = itemValue;
        break;

      case G8131_CMN_TYPE_REVERT_MODE:
        g8131Cfg->pgData[grpId].revertiveMode = itemValue;
        break;

      default:
        /*Configuration parameter not supported.*/
        status = OFDPA_E_FAIL;
        break;
    }
  }
  else
  {
    status = OFDPA_E_NOT_FOUND;
  }

  dpaRWlockUnlock(&g8131RWLock);

  return status;
}

/*********************************************************************
*
* @purpose  Send request to g8131 Protection group
*
* @param    grpId        @b{(input)}
* @param    request      @b{(input)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments MS/MS-P/CLEAR/FS/LOCK supported
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgRequest(uint32_t grpId,
                             uint32_t req)
{
  OFDPA_ERROR_t status = OFDPA_E_FAIL;
  g8131SMRequest_t g8131Req;
  g8131ReqMsg_t    msg;

  G8131_PG_CHECK(grpId);
  G8131_PG_LOCAL_REQUEST_CHECK(req);

  switch (req)
  {
    case G8131_REQUEST_TYPE_CLEAR:
      g8131Req = G8131_LR_M_CLEAR;
      break;

    case G8131_REQUEST_TYPE_FORCE_SWITCH:
      g8131Req = G8131_LR_B_FS;
      break;

    case G8131_REQUEST_TYPE_MANUAL_SWITCH:
      g8131Req = G8131_LR_L_MS;
      break;

    case G8131_REQUEST_TYPE_MANUAL_SWITCH_P:
      g8131Req = G8131_LR_K_MS_P;
      break;

    case G8131_REQUEST_TYPE_MANUAL_LOCK:
      g8131Req = G8131_LR_A_LOCKOUT;
      break;

      /*Just for test purpose.*/
    default:
      g8131Req = req;
      break;
  }

  dpaRWlockReadlock(&g8131RWLock);

  if (g8131Cfg->pgData[grpId].inUse == OFDPA_TRUE)
  {
    memset(&msg, 0, sizeof(msg));
    msg.grpId   = grpId;
    msg.request = g8131Req;
    status = g8131IssueCmd(G8131_EVENT_REQUEST, 0, &msg);
  }
  else
  {
    status = OFDPA_E_NOT_FOUND;
  }

  dpaRWlockUnlock(&g8131RWLock);

  _G8131_RETURN_CODE_CHECK(status);

  return status;
}

/*********************************************************************
*
* @purpose  Set G8131 Protection group global information.
*
* @param    itemKey      @b{(input)}
* @param    itemValue    @b{(input)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgGlbCommonSet(uint32_t itemKey,
                                  uint32_t itemValue)
{
  OFDPA_ERROR_t status;

  status = g8131PgGlbParamCheck(itemKey, itemValue);
  _G8131_RETURN_CODE_CHECK(status);

  dpaRWlockWritelock(&g8131RWLock);
  switch (itemKey)
  {
    case G8131_GLB_CMN_TYPE_ENABLE:
      g8131Cfg->enable = itemValue;
      break;

    default:
      status = OFDPA_E_FAIL;
      break;
  }
  dpaRWlockUnlock(&g8131RWLock);
  _G8131_RETURN_CODE_CHECK(status);

  return status;
}

/*********************************************************************
*
* @purpose  Get G8131 Protection group global information.
*
* @param    itemKey      @b{(input)}
* @param    itemValue    @b{(output)}
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgGlbCommonGet(uint32_t  itemKey,
                                  uint32_t *itemValue)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;

  dpaRWlockReadlock(&g8131RWLock);
  switch (itemKey)
  {
    case G8131_GLB_CMN_TYPE_ENABLE:
      *itemValue = g8131Cfg->enable;
      break;

    default:
      status = OFDPA_E_FAIL;
      break;
  }
  dpaRWlockUnlock(&g8131RWLock);
  _G8131_RETURN_CODE_CHECK(status);

  return status;
}

/*********************************************************************
* @purpose  G8131 protection group auto switch 
*          
* @param    livenessPortId  @b{(input)}    livenessPortId
* @param    oamStatus       @b{(output)}   oamStatus - 0: Down 1: UP
*
* @returns  OFDPA_E_NONE  
* @returns  OFDPA_E_FAIL  
*
* @comments OAM module will call this function to report fault information.
*           Presently this is only a boolean that indicates the path associated
*           with livenessPortId is down.
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgFaultReport(uint32_t livenessPortId, int oamStatus)
{
  OFDPA_ERROR_t  status = OFDPA_E_FAIL;
  uint32_t       grpId;
  OFDPA_BOOL     foundFlag = OFDPA_FALSE;
  uint32_t       request;
  g8131ReqMsg_t  msg;

  G8131_TRACE(G8131_DEBUG_CONFIG, "livenessPortId 0x%x oamStatus %d", livenessPortId, oamStatus);

  if (OFDB_PORT_TYPE(livenessPortId) != OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT)
  {	    
	  return OFDPA_E_FAIL;
  }
  
  /*Re-use foundFlag to search for corresponding groupId*/
  foundFlag = OFDPA_FALSE;

  dpaRWlockReadlock(&g8131RWLock);
  for (grpId = OFDPA_G8131_PG_MIN_ID; grpId <= OFDPA_G8131_PG_MAX_ID; grpId++)
  {
    if ((g8131Cfg->pgData[grpId].livenessPortIdWorking    != livenessPortId) &&
        (g8131Cfg->pgData[grpId].livenessPortIdProtection != livenessPortId))
    {
      continue;
    }

    foundFlag = OFDPA_TRUE;

    break;
  }

  if (OFDPA_FALSE == foundFlag)
  {
    dpaRWlockUnlock(&g8131RWLock);
    return OFDPA_E_FAIL;
  }

  memset(&msg, 0, sizeof(msg));
  msg.grpId   = grpId;
  if (g8131Cfg->pgData[grpId].livenessPortIdWorking == livenessPortId)
  {
    /* event on working */
    if (oamStatus == 0) /*Down*/
    {
      request = G8131_LR_C_SF;
    }
    else
    {
      request = G8131_LR_D_SF_DIS;
    }
  }
  else
  {
    /* event on protection */
    if (oamStatus == 0) /*Down*/
    {
      request = G8131_LR_E_SF_P;
    }
    else
    {
      request = G8131_LR_F_SF_P_DIS;
    }
  }

  dpaRWlockUnlock(&g8131RWLock);

  msg.request = request;

  /*leishenghua modify 20160906 解决保护倒换超时问题，
  	取消消息发送，直接调用倒换函数*/
  /*status = g8131IssueCmd(G8131_EVENT_REQUEST, 0, &msg);*/
  status = g8131IssueCmdSync(G8131_EVENT_REQUEST, 0, &msg);
  _G8131_RETURN_CODE_CHECK(status);

  return status;
}

/* BEGIN: Added by Hushouqiang, 2016/5/10   问题单号:P10006 */

/*********************************************************************
*
* @purpose  Get G8131 Protection group
*
* @param    grpId                 @b{(input)} identifier for protection group entry
* @param    pgNode             @b{(input)} fastfailover group node pointer associated with this entry

*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if failure
*
* @comments  none
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgGet(uint32_t grpId,pgOperData_t  *pgNode)
{
  OFDPA_ERROR_t status = OFDPA_E_FAIL;

  G8131_PG_CHECK(grpId);

  dpaRWlockWritelock(&g8131RWLock);

  if (g8131Cfg->pgData[grpId].inUse == OFDPA_TRUE)
  {

    memcpy(pgNode, &g8131Cfg->pgData[grpId], sizeof(*pgNode));

    status = OFDPA_E_NONE;
  }
  else
  {
    /*The G8131 table entry already in use */
    status = OFDPA_E_NOT_FOUND;
  }
  dpaRWlockUnlock(&g8131RWLock);

  _G8131_RETURN_CODE_CHECK(status);

  return status;
}



/*********************************************************************
* @purpose  G8131 protection group auto switch 
*          
* @param    livenessPortId  @b{(input)}    livenessPortId
* @param    oamStatus       @b{(output)}   oamStatus - 0: Down 1: UP
*
* @returns  OFDPA_E_NONE  
* @returns  OFDPA_E_FAIL  
*
* @comments OAM module will call this function to report fault information.
*           Presently this is only a boolean that indicates the path associated
*           with livenessPortId is down.
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgFfgroupidGet(uint32_t lmepid, uint32_t *ffgroupid, uint32_t *work, uint32_t *quickswitch)
{
  OFDPA_ERROR_t  status = OFDPA_E_NONE;
  uint32_t       grpId;
  OFDPA_BOOL     foundFlag = OFDPA_FALSE;

  if(ffgroupid == NULL || work == NULL)
  {
	  return OFDPA_E_FAIL;
  }
  
  /*Re-use foundFlag to search for corresponding groupId*/
  foundFlag = OFDPA_FALSE;

  /*减少处理时间*/
  /*dpaRWlockReadlock(&g8131RWLock);*/
  
  for (grpId = OFDPA_G8131_PG_MIN_ID; grpId <= OFDPA_G8131_PG_MAX_ID; grpId++)
  {
  
    if(g8131Cfg->pgData[grpId].ffGroupId == OFDPA_INVALID_GROUP_ID)
    {
		continue;
	}
	
    if ((g8131Cfg->pgData[grpId].lmepIdWorking    != lmepid) &&
        (g8131Cfg->pgData[grpId].lmepIdProtection != lmepid))
    {
      continue;
    }

    foundFlag = OFDPA_TRUE;

    break;
  }

  if (OFDPA_FALSE == foundFlag)
  {
    /*dpaRWlockUnlock(&g8131RWLock);*/
    return OFDPA_E_FAIL;
  }

  

  *quickswitch = g8131Cfg->pgData[grpId].quickswitch;
 
   if(g8131Cfg->pgData[grpId].sfpStateFast == OFDPA_TRUE
   	&& g8131Cfg->pgData[grpId].sfwStateFast == OFDPA_TRUE)
   {
		*quickswitch = 0;
   }
   else  if(g8131Cfg->pgData[grpId].sfpStateFast == OFDPA_FALSE
   	&& g8131Cfg->pgData[grpId].sfwStateFast == OFDPA_FALSE)
   {
		*quickswitch = 0;
   }
   else  if(g8131Cfg->pgData[grpId].sfpStateFast == OFDPA_TRUE
   	&& g8131Cfg->pgData[grpId].sfwStateFast == OFDPA_FALSE)
   {
		*work = 0;	/*备故障*/
   }
   else  if(g8131Cfg->pgData[grpId].sfpStateFast == OFDPA_FALSE
   	&& g8131Cfg->pgData[grpId].sfwStateFast == OFDPA_TRUE)
   {
		*work = 1;  /*主故障*/
   }


  *ffgroupid = g8131Cfg->pgData[grpId].ffGroupId;

  /*dpaRWlockUnlock(&g8131RWLock);*/

 
  return status;
}



/*********************************************************************
* @purpose  G8131 protection group auto switch 
*          
* @param    livenessPortId  @b{(input)}    livenessPortId
* @param    oamStatus       @b{(output)}   oamStatus - 0: Down 1: UP
*
* @returns  OFDPA_E_NONE  
* @returns  OFDPA_E_FAIL  
*
* @comments OAM module will call this function to report fault information.
*           Presently this is only a boolean that indicates the path associated
*           with livenessPortId is down.
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PgUpdateStateFast(uint32_t lmepid, int fault)
{
  OFDPA_ERROR_t  status = OFDPA_E_NONE;
  uint32_t       grpId;
  OFDPA_BOOL     foundFlag = OFDPA_FALSE;

  /*Re-use foundFlag to search for corresponding groupId*/
  foundFlag = OFDPA_FALSE;

  /*减少处理时间*/
  /*dpaRWlockReadlock(&g8131RWLock);*/
  
  for (grpId = OFDPA_G8131_PG_MIN_ID; grpId <= OFDPA_G8131_PG_MAX_ID; grpId++)
  {
  
    if(g8131Cfg->pgData[grpId].ffGroupId == OFDPA_INVALID_GROUP_ID)
    {
		continue;
	}
	
    if ((g8131Cfg->pgData[grpId].lmepIdWorking    != lmepid) &&
        (g8131Cfg->pgData[grpId].lmepIdProtection != lmepid))
    {
      continue;
    }

    foundFlag = OFDPA_TRUE;

    break;
  }

  if (OFDPA_FALSE == foundFlag)
  {
    /*dpaRWlockUnlock(&g8131RWLock);*/
    return OFDPA_E_FAIL;
  }

  
 
  if (g8131Cfg->pgData[grpId].lmepIdWorking == lmepid)
  {
    g8131Cfg->pgData[grpId].sfwStateFast = fault?OFDPA_TRUE:OFDPA_FALSE;
	
  }
  else
  {
  	g8131Cfg->pgData[grpId].sfpStateFast = fault?OFDPA_TRUE:OFDPA_FALSE;
  }


  /*dpaRWlockUnlock(&g8131RWLock);*/

 
  return status;
}




/* END:   Added by Hushouqiang, 2016/5/10 */

