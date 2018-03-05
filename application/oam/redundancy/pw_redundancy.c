/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2005-2015
*
**********************************************************************
* @filename  g8131.c
*
* @purpose   G8131 module initilization
*
* @component G8131
*
* @comments
*
* @end
*
**********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "ofdpa_util.h"
#include "driver_oam.h"
#include "msgq.h"
#include "ofdpa_porting.h"
#include "pw_redundancy_cfg.h"
#include "pw_redundancy_api.h"

pwRedunCfg_t    *pwRedunCfg = NULL;
ofdpaRWlock_t  pwRedunRWLock;


/*********************************************************************
* @purpose  Get CCM event and handle fault notification
*
* @param    *eventInfo   pointer to the event information
*
* @returns  OFDPA_E_NONE, if success
* @returns  OFDPA_E_FAIL, if other failure
*
* @notes
* @notes
* @end
*********************************************************************/
OFDPA_ERROR_t pwRedunEventHandlerCallback(pwRedunEventInfo_t *eventInfo)
{

  OFDPA_ERROR_t rc;

  if(eventInfo == NULL)
  {
	return OFDPA_E_PARAM;
  }
  
  rc = pwRedunFaultReport(eventInfo->lmepId, (eventInfo->faultOn==OFDPA_TRUE?0:1));

  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_BASIC, "Failed to send dot1ag HW event, rc = %d\r\n", rc);
  }

  return OFDPA_E_NONE;
}


OFDPA_ERROR_t pwRedunTunnelIdGet(int32_t grpId, uint32_t *tunnelId)
{
	uint32_t ffGroupId = 0;
	
	/*通过fastfailover groupid 查找l2 port*/

	ffGroupId = pwRedunCfg->pgData[grpId].ffGroupId;

	return driverVpwsTunnelIdGet(ffGroupId, tunnelId);
	
}

OFDPA_ERROR_t pwRedunGroupGet(uint32_t inPortNum, uint32_t pktvlan, uint32_t *ffGroupId)
{
	int unit = 0;	
	int rv = 0;
	int grpId = 0;
	uint32_t 	tunnelId = 0;
	uint32_t inport = 0;
	uint32_t vlan = 0;
	OFDPA_ERROR_t rc = OFDPA_E_NONE;


	for (grpId = OFDPA_PW_REDUN_MIN_ID; grpId <= OFDPA_PW_REDUN_MAX_ID; grpId++)
	{
	  if ((pwRedunCfg->pgData[grpId].inUse))
	  {
		if(pwRedunTunnelIdGet(grpId, &tunnelId) == OFDPA_E_NONE)
		{
			/*通过tunnelid 查找提取的inport 和vlan*/
			if(driverVpwsCustomerPortVlanGet(tunnelId, &inport, &vlan) == OFDPA_E_NONE)
			{
	  			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_VERY_VERBOSE, "inport %d vlan %d."
						   "pktvlan = %d .\r\n", inport, vlan, pktvlan);				
				if(inport == inPortNum)
				{
					if(vlan == 0 || vlan == pktvlan)
					{
						break;
					}
				}
			}
		}
	  }
	
	}

	if(grpId > OFDPA_PW_REDUN_MAX_ID)
	{
		return OFDPA_E_NOT_FOUND;
	}

	*ffGroupId = pwRedunCfg->pgData[grpId].ffGroupId;
	return OFDPA_E_NONE;

}


OFDPA_ERROR_t pwRedunInit(void)
{
  OFDPA_ERROR_t rc;
  
  /* Allocate the memory for the configuration storage data. */
  pwRedunCfg = malloc(sizeof(*pwRedunCfg));

  if (NULL == pwRedunCfg)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                       "Failed to allocate memory for g8131 cfg file\r\n", 0);
    return OFDPA_E_ERROR;
  }

  if (OFDPA_E_FAIL == dpaRWlockInit(&pwRedunRWLock, NULL))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS,
                       "Failed to create g8131RWLock\r\n", 0);
    return OFDPA_E_ERROR;
  }
  
  memset(pwRedunCfg, 0, sizeof(*pwRedunCfg));
  pwRedunCfg->enable        = OFDPA_TRUE;


  rc = driverBfdEventCallbackRegister(pwRedunEventHandlerCallback);
  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_ALWAYS, "\r\nError registering to receive OAM events from driver, rc = %d.\r\n", rc);
    return rc;
  }

#ifdef ARP_FOR_PW_PROTECT
  pwRedunArpStart();
#endif

  return OFDPA_E_NONE;
  
}
