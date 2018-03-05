/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2015
*
**********************************************************************
*
* @filename   dot1ag_outcalls.c
*
* @purpose    Contains definitions to APIs used to interact with
*             external components
*
* @component  dot1ag
*
* @comments   none
*
* @create     26-Feb-2009
*
* @end
*
**********************************************************************/

#include "ofdb_api.h"
#include "dot1ag.h"
#include "dot1ag_debug.h"

/*********************************************************************
* @purpose  Determine if the interface type is valid for dot1ag
*
* @param    sysIntfType  @b{(input)} interface type
*
* @returns  OFDPA_TRUE   Valid interface type
* @returns  OFDPA_FALSE
*
* @comments none
*
* @end
*********************************************************************/
OFDPA_BOOL dot1agIsValidIntfType(uint32_t sysIntfType)
{
  switch (sysIntfType)
  {
    case OFDPA_PORT_TYPE_PHYSICAL:
      return OFDPA_TRUE;
    default:
      break;
  }
  return OFDPA_FALSE;
}

/*********************************************************************
* @purpose  Determine if the interface is valid for dot1ag
*
* @param    intIfNum  @b{(input)} internal interface number
*
* @returns  OFDPA_TRUE   Supported interface
* @returns  OFDPA_FALSE
*
* @comments none
*
* @end
*********************************************************************/
OFDPA_BOOL dot1agIsValidIntf(uint32_t intIfNum)
{
  OFDPA_ERROR_t  rc = OFDPA_FALSE;

  //OFDB_WRITE_LOCK_TAKE;
  if ((OFDB_PORT_TYPE(intIfNum) != OFDPA_PORT_TYPE_PHYSICAL) ||
      (!ofdbPortIsValid(intIfNum)))
  {
    rc = OFDPA_FALSE;
  }
  else
  {
    rc = OFDPA_TRUE;
  }
  //OFDB_WRITE_LOCK_GIVE;
  return rc;
}

/*********************************************************************
* @purpose  Get the next interface that is valid for dot1ag
*
* @param    *pIntIfNum  @b{(input/output)} internal interface number
*
* @returns  OFDPA_E_NONE, if a valid interface is found
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments none
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agValidIntfNextGet(uint32_t *pIntIfNum)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;

  //OFDB_WRITE_LOCK_TAKE;
  while (ofdbPortNextGet(*pIntIfNum, pIntIfNum) == OFDPA_E_NONE)
  {
    if (OFDB_PORT_TYPE(*pIntIfNum) == OFDPA_PORT_TYPE_PHYSICAL)
    {
      rc = OFDPA_E_NONE;
      break;
    }
    else
    {
      continue;
    }
  }
  //OFDB_WRITE_LOCK_GIVE;
  return rc;
}

/*********************************************************************
* @purpose  Checks if the interface/VLAN ID can be enabled for dot1ag
*
* @param    intIfNum  @b{(input)} Internal Interface Number
* @param    vlanId    @b{(input)} VLAN ID
*
* @returns  OFDPA_TRUE, if interface can be enabled
* @returns  OFDPA_FALSE, if interface cannot be enabled
*
* @notes    Acceptability criteria
*              1. Must not be the mirror dest port   &&
*              2. Must not be a LAG member           &&
*              3. Must not be enabled for routing    &&
*              4. Must be in the Forwarding state
*           This function only checks the above criteria.  All other
*           checking (i.e. validity of interface number) can be done
*           in dot1agIntfCheck() or dot1agIntfValidate().  This function
*           can be used to determine when an interface can be configured,
*           but, not enabled.
*
*
* @end
*********************************************************************/
OFDPA_BOOL dot1agIntfCanBeEnabled(uint32_t intIfNum)
{
  if (OFDPA_TRUE == dot1agIsValidIntf(intIfNum))
  {
    return OFDPA_TRUE;
  }
  else
  {
    return OFDPA_FALSE;
  }
}

/*********************************************************************
* @purpose  Checks if an MP on this intIfNum can be operational
*           for dot1ag
*
* @param    intIfNum  @b{(input)} Internal Interface Number
*
* @returns  OFDPA_TRUE, if MP can be operational
* @returns  OFDPA_FALSE, if MP cannot be operational
*
* @notes    Acceptability criteria
*              1. Must not be the mirror dest port   &&
*              2. Must not be a LAG member           &&
*              3. Must not be enabled for routing    &&
*              4. Must be in the Forwarding state
*           This function only checks the above criteria.  All other
*           checking (i.e. validity of interface number) can be done
*           in dot1agIntfCheck() or dot1agIntfValidate().  This function
*           can be used to determine when an interface can be configured,
*           but, not enabled.
*
*
* @end
*********************************************************************/
OFDPA_BOOL dot1agIntfCanBeOperational(uint32_t intIfNum)
{
  if (dot1agIntfCanBeEnabled(intIfNum) == OFDPA_TRUE)
  {
    return OFDPA_TRUE;
  }

  return OFDPA_FALSE;
}

/*********************************************************************
* @purpose  Transmit Fault Alarm
*
* @param    mdIndex   @b{(input)} MD whose FNGSM is reporting a defect
* @param    maIndex   @b{(input)} MA whose FNGSM is reporting a defect
* @param    mepId     @b{(input)} MEP whose FNGSM is reporting a defect
* @param    fngDefect   @b{(input)} Defect to be reported
*
* @returns  none
*
* @notes    802.1ag d8-1 20.34.1
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmFaultNotification(uint32_t mdIndex, uint32_t maIndex,
                                         uint32_t mepId, uint32_t fngDefect)
{
  /* Call SNMP Trap manager routine to report the defect to NMS */
  DOT1AG_TRACE(CFM_SM_FNG, "CFM Fault: MD Index = %d, MA Index = %d, MEP Id = %d, Defect = %d\r\n",
               mdIndex, maIndex, mepId, fngDefect);
//  trapMgrDot1agCfmFaultNotification(mdIndex, maIndex, mepId, fngDefect);
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  To find out if an interface is in forwarding in a VLAN
*
* @param    intIfNum @b{(input)} Interface number
* @param    vlanId   @b{(input)} VLAN Id of the MA
*
* @returns  none
*
* @notes
*
* @end
**********************************************************************/
OFDPA_BOOL dot1agCfmIntfVlanIsForwarding(uint32_t intIfNum, uint32_t vlanId)
{
#if OFDPA_NOT_SUPPORTED
  uint32_t     activeState;
  uint32_t     mstID;

  if (dot1sModeGet() == OFDPA_TRUE)
  {
    /* If this port is not in the forwarding state, it can be configured */
    /* but not enabled */
    mstID = dot1sVlanToMstiGet(vlanId);
    if (dot1sMstiPortStateGet(mstID, intIfNum) != L7_DOT1S_FORWARDING &&
        dot1sMstiPortStateGet(mstID, intIfNum) != L7_DOT1S_MANUAL_FWD)
    {
      return OFDPA_FALSE;
    }
  }
  else /* dot1d Support */
  {
    /* If this port is not in the active state (up & forwarding), it can be */
    /* configured but not enabled */
    if ( (nimGetIntfActiveState(intIfNum, &activeState) != OFDPA_E_NONE) ||
         (activeState != L7_ACTIVE) )
      return OFDPA_FALSE;
  }
  return OFDPA_TRUE;
#endif
  /* NOTE: This function call us within a OFDB_WRITE_LOCK_TAKE call. DO NOT TAKE SEMA AGAIN */
  return OFDPA_TRUE;
}
