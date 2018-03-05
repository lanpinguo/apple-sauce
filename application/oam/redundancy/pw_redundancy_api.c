/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
* @filename  PW_REDUN_api.c
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

#include "ofdb_api.h"
#include "pw_redundancy_include.h"
#include "pw_redundancy_cfg.h"
#include "pw_redundancy_api.h"
#include "pw_redundancy_exports.h"
#include "pw_redundancy_sm.h"

extern pwRedunCfg_t      *pwRedunCfg;
extern ofdpaRWlock_t    pwRedunRWLock;



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
OFDPA_ERROR_t pwRedunCreate(uint32_t grpId,
                            uint32_t ffGroupId,
                            uint32_t livenessPortIdWorking,
                            uint32_t livenessPortIdProtection,          
                            uint32_t lmepIdWorking,
                            uint32_t lmepIdProtection)
{
  OFDPA_ERROR_t status = OFDPA_E_FAIL;
  pwRedunOperData_t  *pgNode;

  PW_REDUN_PG_CHECK(grpId);

  dpaRWlockWritelock(&pwRedunRWLock);

  if (pwRedunCfg->pgData[grpId].inUse == OFDPA_FALSE)
  {
    pgNode = &pwRedunCfg->pgData[grpId];

    memset(pgNode, 0, sizeof(*pgNode));

    pgNode->grpId = grpId;

    pgNode->ffGroupId                = ffGroupId;
    pgNode->livenessPortIdWorking    = livenessPortIdWorking;
    pgNode->livenessPortIdProtection = livenessPortIdProtection;

    pgNode->lmepIdWorking            = lmepIdWorking;
    pgNode->lmepIdProtection         = lmepIdProtection;
    pgNode->livenessPortIdActive     = livenessPortIdWorking;


    pgNode->wtrTime        = 0;
    pgNode->revertiveMode  = OFDPA_FALSE;
 
    pgNode->inUse         = OFDPA_TRUE;

    /* update liveness port state */
    PwRedunUpdateLivenessPortState(&pwRedunCfg->pgData[grpId]);

    status = OFDPA_E_NONE;
  }
  else
  {
    /*The G8131 table entry already in use */
    status = OFDPA_E_EXISTS;
  }
  dpaRWlockUnlock(&pwRedunRWLock);

  _PW_REDUN_RETURN_CODE_CHECK(status);


  dpaRWlockWritelock(&pwRedunRWLock);

  
  ofdbBfdStateGet(pgNode->lmepIdWorking, &pgNode->stateWorking);
  ofdbBfdStateGet(pgNode->lmepIdProtection, &pgNode->stateProtection);
  pwRedunSwitch(grpId);

  /*leishenghua add ，更新一下liveness port状态*/
  PwRedunUpdateLivenessPortState(&pwRedunCfg->pgData[grpId]);
  
  dpaRWlockUnlock(&pwRedunRWLock);


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
OFDPA_ERROR_t pwRedunDelete(uint32_t grpId)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;

  PW_REDUN_PG_CHECK(grpId);

  dpaRWlockWritelock(&pwRedunRWLock);

  memset(&(pwRedunCfg->pgData[grpId]), 0, sizeof(pwRedunCfg->pgData[0]));

  dpaRWlockUnlock(&pwRedunRWLock);

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
OFDPA_ERROR_t pwRedunCommonGet(uint32_t  grpId,
                               uint32_t  itemType,
                               uint32_t *itemValue)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;

  PW_REDUN_PG_CHECK(grpId);

  dpaRWlockReadlock(&pwRedunRWLock);
  if (pwRedunCfg->pgData[grpId].inUse == OFDPA_TRUE)
  {
    switch (itemType)
    {
      case PW_REDUN_CMN_TYPE_HOLDOFF_VALUE:
        *itemValue = pwRedunCfg->pgData[grpId].holdOffTime;
        break;

      case PW_REDUN_CMN_TYPE_WTR_VALUE:
        *itemValue = pwRedunCfg->pgData[grpId].wtrTime;
        break;

      case PW_REDUN_CMN_TYPE_REVERT_MODE:
        *itemValue = pwRedunCfg->pgData[grpId].revertiveMode;
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

  dpaRWlockUnlock(&pwRedunRWLock);

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
OFDPA_ERROR_t pwRedunCommonSet(uint32_t  grpId,
                               uint32_t  itemType,
                               uint32_t  itemValue)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;

  PW_REDUN_PG_CHECK(grpId);

  dpaRWlockReadlock(&pwRedunRWLock);
  if (pwRedunCfg->pgData[grpId].inUse == OFDPA_TRUE)
  {
    switch (itemType)
    {
      case PW_REDUN_CMN_TYPE_HOLDOFF_VALUE:
         pwRedunCfg->pgData[grpId].holdOffTime = itemValue;
        break;

      case PW_REDUN_CMN_TYPE_WTR_VALUE:
        pwRedunCfg->pgData[grpId].wtrTime = itemValue;
        break;

      case PW_REDUN_CMN_TYPE_REVERT_MODE:
        pwRedunCfg->pgData[grpId].revertiveMode = itemValue;
		/*tbd触发返回*/
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

  dpaRWlockUnlock(&pwRedunRWLock);

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
OFDPA_ERROR_t pwRedunFaultReport(uint32_t lmepid, int oamStatus)
{
  OFDPA_ERROR_t  status = OFDPA_E_NONE;
  uint32_t       grpId;
  OFDPA_BOOL     foundFlag = OFDPA_FALSE;
  uint32_t stateChange = 1;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_VERBOSE,
					 "lmepid 0x%x oamStatus %d\r\n", lmepid, oamStatus);

  /*Re-use foundFlag to search for corresponding groupId*/
  foundFlag = OFDPA_FALSE;

  dpaRWlockReadlock(&pwRedunRWLock);
  for (grpId = OFDPA_PW_REDUN_MIN_ID; grpId <= OFDPA_PW_REDUN_MAX_ID; grpId++)
  {
    if ((pwRedunCfg->pgData[grpId].lmepIdWorking!= lmepid) &&
        (pwRedunCfg->pgData[grpId].lmepIdProtection!= lmepid))
    {
      continue;
    }

    foundFlag = OFDPA_TRUE;

    break;
  }

  if (OFDPA_FALSE == foundFlag)
  {
    dpaRWlockUnlock(&pwRedunRWLock);
    return OFDPA_E_FAIL;
  }


  if (pwRedunCfg->pgData[grpId].lmepIdWorking == lmepid)
  {
    /* event on working */
    if (oamStatus == 0) /*Down*/
    {
		if(pwRedunCfg->pgData[grpId].stateWorking ==0)
		{
			stateChange = 0;
		}
		else
		{
			pwRedunCfg->pgData[grpId].stateWorking = 0;
		}	
    }
    else
    {
		if(pwRedunCfg->pgData[grpId].stateWorking !=0)
		{
			stateChange = 0;
		}
		else
		{
			pwRedunCfg->pgData[grpId].stateWorking = 1;
		}
    }
  }
  else
  {
    /* event on protection */
    if (oamStatus == 0) /*Down*/
    {
		if(pwRedunCfg->pgData[grpId].stateProtection==0)
		{
			stateChange = 0;
		}
		else
		{
			pwRedunCfg->pgData[grpId].stateProtection = 0;
		}	

    }
    else
    {
		if(pwRedunCfg->pgData[grpId].stateProtection !=0)
		{
			stateChange = 0;
		}
		else
		{
			pwRedunCfg->pgData[grpId].stateProtection = 1;
		}	

    }
  }


  if(stateChange)
  {
	  /*触发倒换*/
	  pwRedunSwitch(grpId);	  
  }

  /*倒换完成再释放信号量*/
  dpaRWlockUnlock(&pwRedunRWLock);
  
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
OFDPA_ERROR_t pwRedunGet(uint32_t grpId,	pwRedunOperData_t  *pgNode)
{
  OFDPA_ERROR_t status = OFDPA_E_FAIL;

  PW_REDUN_PG_CHECK(grpId);

  dpaRWlockWritelock(&pwRedunRWLock);

  if (pwRedunCfg->pgData[grpId].inUse == OFDPA_TRUE)
  {

    memcpy(pgNode, &pwRedunCfg->pgData[grpId], sizeof(*pgNode));

    status = OFDPA_E_NONE;
  }
  else
  {
    /*The G8131 table entry already in use */
    status = OFDPA_E_NOT_FOUND;
  }
  dpaRWlockUnlock(&pwRedunRWLock);


  return status;
}



/* END:   Added by Hushouqiang, 2016/5/10 */

