/*********************************************************************
 *
 * (C) Copyright Broadcom Corporation 2000-2015
 *
 **********************************************************************
 * @filename  g8131_sm.c
 *
 * @purpose   Header file for CFM State Machines
 *
 * @component G8131
 *
 * @comments  none
 *
 * @create    28/10/2013
 *
 * @end
 **********************************************************************/

#include <string.h>
#include "ofdb_api.h"
#include "driver_group.h"
#include "datapath.h"

#include "pw_redundancy_cfg.h"

extern pwRedunCfg_t      *pwRedunCfg;
extern ofdpaRWlock_t    pwRedunRWLock;


void PwRedunUpdateLivenessPortState(pwRedunOperData_t *pgOperData)
{
  OFDPA_PORT_STATE_t workingPortState;
  OFDPA_PORT_STATE_t protectionPortState;

  // OFDB_WRITE_LOCK_TAKE;
  if ((ofdbPortStateGet(pgOperData->livenessPortIdWorking, &workingPortState) == OFDPA_E_NONE) &&
      (ofdbPortStateGet(pgOperData->livenessPortIdProtection, &protectionPortState) == OFDPA_E_NONE))
  {
    if (pgOperData->livenessPortIdActive == pgOperData->livenessPortIdWorking)
    {
      /* new path is working */
      workingPortState    |= OFDPA_PORT_STATE_LIVE;
      protectionPortState &= ~OFDPA_PORT_STATE_LIVE;
    }
    else
    {
      /* new path is protection */
      protectionPortState |= OFDPA_PORT_STATE_LIVE;
      workingPortState    &= ~OFDPA_PORT_STATE_LIVE;
    }

    ofdbPortStateSet(pgOperData->livenessPortIdWorking, workingPortState);
    ofdbPortStateSet(pgOperData->livenessPortIdProtection, protectionPortState);
  }
  else
  {
    //G8131_TRACE(G8131_DEBUG_SM, "Failed retrieving port state from OFDB.");
  }
  // OFDB_WRITE_LOCK_GIVE;
}

 
OFDPA_ERROR_t pwRedunSwitch(uint32_t grpId)
{
  OFDPA_ERROR_t   status;
  uint32_t        failOverValue;
  OFDB_ENTRY_FLAG_t flags;
  ofdbPortInfo_t portInfo;
  dpaEventMsg_t   eventMsg = {.msgType = DPA_EVENT_PW_REDUN_STATUS_MSG};
  uint32_t switchover = 0;

	/*leishenghua modify 20170424, 先更新ffgroup id，再倒换*/
	
	/* BEGIN: Added by Hushouqiang, 2016/9/28	问题单号:P10012 */
	if(pwRedunCfg->pgData[grpId].ffGroupId == OFDPA_INVALID_GROUP_ID)
	{
	  /*
	   * validation logic assures the workingLivenessPortId and the protectionLivenessPortId
	   * contain the same ffGroupId so using either one to retrieve the ffGroupId works
	   */
	   
	  /*OFDB_READ_LOCK_TAKE;*/
	  status = ofdbPortGet(pwRedunCfg->pgData[grpId].livenessPortIdActive, &portInfo, &flags);
	  /*OFDB_LOCK_GIVE;*/
	  if (status == OFDPA_E_NONE)
	  {
		if (flags & OFDB_PORT_DELETED)
		{
		  status = OFDPA_E_NOT_FOUND;
		}
	  }
	  if (status != OFDPA_E_NONE)
	  {
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
						   "Liveness port not found. portId = 0x%x\r\n",
						   pwRedunCfg->pgData[grpId].livenessPortIdActive);
		return OFDPA_E_NONE;
	  }
		
	  pwRedunCfg->pgData[grpId].ffGroupId = portInfo.ffGroupId;
	  
	}
	/* END:   Added by Hushouqiang, 2016/9/28 */
	
	

	/*当前工作在主*/
	if (pwRedunCfg->pgData[grpId].livenessPortIdActive == pwRedunCfg->pgData[grpId].livenessPortIdWorking)
	{
		if(pwRedunCfg->pgData[grpId].stateWorking == 0 && pwRedunCfg->pgData[grpId].stateProtection)
		{
			
			failOverValue = 1;
			status = driverMplsFastFailoverSet(pwRedunCfg->pgData[grpId].ffGroupId, failOverValue);
			if (OFDPA_E_NONE != status)
			{
			  	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                     "Unable to set MPLS FFO state. rv = %dd\r\n",
                     status);
				return status;
			}

			switchover = 1;
			pwRedunCfg->pgData[grpId].livenessPortIdActive = pwRedunCfg->pgData[grpId].livenessPortIdProtection;
		}
	}
	else
	{
		/*当前工作在备*/
		if((pwRedunCfg->pgData[grpId].stateWorking  && pwRedunCfg->pgData[grpId].stateProtection == 0)
			|| (pwRedunCfg->pgData[grpId].stateWorking  && pwRedunCfg->pgData[grpId].stateProtection
			&& pwRedunCfg->pgData[grpId].wtrTime == 0 && pwRedunCfg->pgData[grpId].revertiveMode ==OFDPA_TRUE))
		{
			failOverValue = 0;
			status = driverMplsFastFailoverSet(pwRedunCfg->pgData[grpId].ffGroupId, failOverValue);
			if (OFDPA_E_NONE != status)
			{
			  	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                     "Unable to set MPLS FFO state. rv = %dd\r\n",
                     status);
				return status;
			}

			switchover = 1;
			pwRedunCfg->pgData[grpId].livenessPortIdActive = pwRedunCfg->pgData[grpId].livenessPortIdWorking;

		}
	}
	


	if(switchover)
	{

	  pwRedunCfg->pgData[grpId].switchCount++;
	  /*tbd*/
	  /*pwRedunCfg->pgData[grpId].lastSwitchTime*/
		
	  PwRedunUpdateLivenessPortState(&pwRedunCfg->pgData[grpId]);

	  /*更新oam mep，???*/
	  /* Update LMEP Map data */
	  status = ofdbInjectedOamLmepIdMapDataUpdate(pwRedunCfg->pgData[grpId].ffGroupId);

	  if (OFDPA_E_NONE != status)
	  {
	  	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Unable to update LMEP Map data. rv = %d\r\n",
                         status);
	  }
	  
	  /* BEGIN: Added by Hushouqiang, 2016/10/16   问题单号:P10013 */
	  /* Notify client. */
	  eventMsg.msgSubType = failOverValue;/*ofdbOamEventMapping[event_type] ;*/

	  eventMsg.cookie[0] = grpId;
	  eventMsg.cookie[1] = pwRedunCfg->pgData[grpId].livenessPortIdActive;

	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "DPA_EVENT_PW_REDUN_STATUS_MSG grpid %d failOverValue %d\r\n",
                         grpId, failOverValue);
	  datapathEventNotificationSend(&eventMsg);
	  /* END:   Added by Hushouqiang, 2016/10/16 */
	}

  return status;
}

