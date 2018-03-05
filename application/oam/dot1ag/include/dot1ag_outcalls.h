/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2007-2015
*
**********************************************************************
* @filename  dot1ag_outcalls.h
*
* @purpose   Prototypes of dot1ag outcalls
*
* @component dot1ag
*
* @comments
*
* @create    02/26/2009
*
* @end
*
**********************************************************************/
#ifndef DOT1AG_OUTCALLS_H
#define DOT1AG_OUTCALLS_H

#include "ofdpa_datatypes.h"

/* Begin Function Prototypes */
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
OFDPA_BOOL dot1agIsValidIntfType(uint32_t sysIntfType);

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
OFDPA_BOOL dot1agIsValidIntf(uint32_t intIfNum);

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
OFDPA_ERROR_t dot1agValidIntfNextGet(uint32_t *pIntIfNum);

/*********************************************************************
* @purpose  Checks if the interface/VLAN ID can be enabled for dot1ag
*
* @param    intIfNum  @b{(input)} Internal Interface Number
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
OFDPA_BOOL dot1agIntfCanBeEnabled(uint32_t intIfNum);

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
OFDPA_BOOL dot1agIntfCanBeOperational(uint32_t intIfNum);

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
                                         uint32_t mepId, uint32_t fngDefect);

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
OFDPA_BOOL dot1agCfmIntfVlanIsForwarding(uint32_t intIfNum, uint32_t vlanId);

/* End Function Prototypes */
#endif /* DOT1AG_OUTCALLS_H */
