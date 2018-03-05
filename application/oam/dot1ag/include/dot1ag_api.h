/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2015
*
**********************************************************************
*
* @filename   dot1ag_api.h
*
* @purpose    Contains signations of procedures to support management objects
*             access for dot1ag component
*
* @component  DOT1AG/Connectivity & Fault Management
*
* @comments   none
*
* @create     19-Feb-2009
*
* @end
*
**********************************************************************/
#ifndef INCLUDE_DOT1AG_API_H
#define INCLUDE_DOT1AG_API_H

#include "ofdpa_datatypes.h"
#include "dot1ag_exports.h"

/*********************************************************************
* @purpose  Synchronize with dot1ag queue.
*
* @param    @b{(input)} None
*
* @returns  OFDPA_E_NONE  if synchronization is successful
* @returns  OFDPA_E_FAIL  otherwise
*
* @comments
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agApiQueueSynchronize();

/*********************************************************************
* @purpose  To find out if more maintenance domains can be created and
*           what is the next valid mdIndex available
*
* @param    mdIndex    @b{(output)} Index value used by SNMP. If no
*                                   more domains can be created then
*                                   this value is NULL
*
* @returns  none
*
* @notes    none
*
* @end
**********************************************************************/
void dot1agMdTableNextIndexGet(uint32_t *mdIndex);

/*********************************************************************
* @purpose  Create a maintenance domain with specified name and level.
*           Optionally you can provide the mdIndex value as well.
*
* @param    mdIndex    @b{(input)} Index value used by SNMP. Use
*                                  NULL for other UI's
* @param    mdLevel    @b{(input)} Domain level to be set
* @param    mdName     @b{(input)} Maintenance Domain name. This has to
*                                  be unique for the CLI/Web to address
*                                  domains using name instead of index as
*                                  in SNMP.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_REQUEST_DENIED      Domain exists that has only one of
*                                  the given name or level
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdCreate(uint32_t mdIndex, int mdLevel,
                             uint8_t *mdName);

/*********************************************************************
* @purpose  Delete specified maintenance domain
*
* @param    mdIndex    @b{(input)} Index value used by SNMP. Use
*                                  NULL for other UI's
* @param    mdLevel    @b{(input)} Domain level
* @param    mdName     @b{(input)} Maintenance Domain name. This has to
*                                  be unique for the CLI/Web to address
*                                  domains using name instead of index as
*                                  in SNMP.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_REQUEST_DENIED      Domain exists that has only one of
*                                  the given name or level
* @returns  OFDPA_E_NOT_FOUND           Domain with a given name and
*                                  level does not exist
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdDelete(uint32_t mdIndex, int mdLevel,
                             uint8_t *mdName);

/*********************************************************************
* @purpose  Verify if a give maintenance domain exists
*
* @param    mdIndex    @b{(input)} Index value used by SNMP.
* @param    mdLevel    @b{(input)} Domain level. This has to be unique
*                                  for the CLI/Web to address domains using
*                                  name instead of index as in SNMP.
* @param    mdName     @b{(input)} Maintenance Domain name. This has to
*                                  be unique for the CLI/Web to address
*                                  domains using name instead of index as
*                                  in SNMP.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdIsValid(uint32_t mdIndex, int mdLevel,
                              uint8_t *mdName);

/*********************************************************************
* @purpose  Obtain configured maintenance domain name given mdIndex
*
* @param    mdIndex    @b{(input)} Index value used by SNMP.
* @param    mdName     @b{(output)} Maintenance Domain name
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdNameGet(uint32_t mdIndex, uint8_t *mdName);

/*********************************************************************
* @purpose  Obtain configured maintenance domain level given mdIndex
*
* @param    mdIndex    @b{(input)} Index value used by SNMP.
* @param    mdLevel    @b{(output)} Maintenance Domain level
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdLevelGet(uint32_t mdIndex, int *mdLevel);

/*********************************************************************
* @purpose  Obtain configured maintenance domain mdIndex given mdLevel
*           or mdName
*
* @param    mdName     @b{(input)} Maintenance Domain name. Can be
*                                  NULLPTR, if so use mdLevel as input
* @param    mdLevel    @b{(input)} Maintenance Domain level. Can be -1
*                                  of so use mdName as key
* @param    mdIndex    @b{(output)} Index value used by SNMP.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdMdIndexGet(uint8_t *mdName, int mdLevel,
                                 uint32_t *mdIndex);

/*********************************************************************
* @purpose  Obtain number of MA's configured in a given maintenance domain
*           or mdName
*
* @param    mdIndex    @b{(input)}  Index for MD identification
* @param    mdCount    @b{(output)} Number of maintenance associations
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdMaCountGet(uint32_t mdIndex, uint32_t *maCount);

/*********************************************************************
* @purpose  Obtain valid maIndex to be used to create a new
*           maintenance association for this domain mdIndex
*
* @param    mdIndex      @b{(input)}  Index value used by SNMP.
* @param    maNextIndex  @b{(output)} New maintenance Assoc index.
*                                     NULL if no new MAs can be
*                                     created
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdMaNextIndexGet(uint32_t mdIndex, uint32_t *maNextIndex);

/*********************************************************************
* @purpose  Obtain a next valid mdIndex value given a maintenance domain
*
* @param    nextMdIndex    @b{(input/output)} domain index value. Use NULL
*                                         to obatain the first valid
*                                         value.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdIndexNextGet(uint32_t *nextMdIndex);

/*********************************************************************
* @purpose  Create a maintenance association given (mdIndex)
*           and (maIndex or maName) and primVID
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    maid       @b{(input)} Maintenance assoc identifier
* @param    primVID    @b{(input)} VLAN ID associated with the MA.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_FULL
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMaCreate(uint32_t mdIndex, uint32_t maIndex,
                             uint8_t *maid, uint32_t primVID);

/*********************************************************************
* @purpose  Delete a maintenance association given (mdIndex)
*           and (maIndex)
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_NOT_FOUND
* @returns  OFDPA_E_REQUEST_DENIED
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMaDelete(uint32_t mdIndex, uint32_t maIndex);

/*********************************************************************
* @purpose  Obtain next valid maintenance association given
*           (mdIndex and mdName)
*
* @param    mdIndex    @b{(input/output)} Index used for MD indentification
*                                         used by SNMP.
* @param    maIndex    @b{(input/output)} Index for MA identification used by
*                                         SNMP.
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use NULL as input to obtain first valid entry
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMaNextGet(uint32_t *mdIndex, uint32_t *maIndex);

/*********************************************************************
* @purpose  Obtain maintenance association name given mdIndex and
*           maIndex
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    maName     @b{(output)} Maintenance association name
* @param    maNameFormat @b{(output)} Maintenance association name format
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaidGet(uint32_t mdIndex, uint32_t maIndex, uint8_t *maid);

/*********************************************************************
* @purpose  Obtain primary vlan associated with an MA
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    primVID    @b{(output)} Primary VLAN associated with the MA
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaPrimVLANGet(uint32_t mdIndex, uint32_t maIndex,
                                  uint32_t *primVID);

/*********************************************************************
* @purpose  Obtain maIndex given primary vlan associated with an MA
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    primVID    @b{(input)} Primary VLAN associated with the MA
* @param    maIndex    @b{(output)} Index for MA identification used by
*                                  SNMP.
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaIndexGet(uint32_t mdIndex,  uint32_t primVID,
                               uint32_t *maIndex);

/*********************************************************************
* @purpose  Obtain CCM interval associated with this MA
*
* @param    mdIndex     @b{(input)} Index used for MD indentification
* @param    maIndex     @b{(input)} Index for MA identification
* @param    ccmInterval @b{(output)} Continuity Check Interval
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaCCMIntervalGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t *ccmInterval);

/*********************************************************************
* @purpose  Set CCM interval associated with this MA
*
* @param    mdIndex     @b{(input)} Index used for MD indentification
* @param    maIndex     @b{(input)} Index for MA identification
* @param    ccmInterval @b{(input)} Continuity Check Interval
*
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMaCCMIntervalSet(uint32_t mdIndex, uint32_t maIndex,
                                     Dot1agCfmCcmInterval_t ccmInterval);

/*********************************************************************
* @purpose  Verify if an entry with given values exists in MEP Table
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is configured
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes This functions ignores interface number
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepIsValid(uint32_t mdIndex, uint32_t maIndex,
                               uint32_t mepId, uint32_t intIfNum);

/*********************************************************************
* @purpose  Obtain next valid maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes  Use NULL values to obtain the first valid entry
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepNextGet(uint32_t *mdIndex, uint32_t *maIndex,
                               uint32_t *mepId, uint32_t *intIfNum);

/*********************************************************************
* @purpose  Obtain direction of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP. Use NULL for
*                                  other UI's
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    direction  @b{(output)} Direction of MEP (Up or Down)
*
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepDirectionGet(uint32_t mdIndex, uint32_t maIndex,
                                    uint32_t mepId, uint32_t intIfNum,
                                    uint32_t *direction);

/*********************************************************************
* @purpose  Obtain active mode of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)}  Active mode of MEP (OFDPA_TRUE or
*                                    OFDPA_FALSE)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepActiveModeGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum,
                                     OFDPA_BOOL *mode);

/*********************************************************************
* @purpose  Set active mode of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(input)} Active mode of MEP (OFDPA_TRUE or OFDPA_FALSE)
*
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepActiveModeSet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum,
                                     OFDPA_BOOL mode);

/*********************************************************************
* @purpose  Obtain CCI enabled flag of maintenance end point on an
*           interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)}  CCI enabled flag of MEP (OFDPA_TRUE or
*                                    OFDPA_FALSE)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepCCIEnabledGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum,
                                     OFDPA_BOOL *mode);

/*********************************************************************
* @purpose  Set CCI enabled flag of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)}  CCI enabled flag of MEP (OFDPA_TRUE or
*                                    OFDPA_FALSE)
*
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepCCIEnabledSet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum,
                                     OFDPA_BOOL mode);

/*********************************************************************
* @purpose  Clear RMEP entries learned on a maintenance end point
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agRmepClear(uint32_t mdIndex, uint32_t maIndex,
                              uint32_t mepId);

/*********************************************************************
* @purpose  Obtain Fault Notification Genreator SM status
*           of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val       @b{(output)} Fault Notification Genreator SM
*                                  status Dot1agCfmFngState_t
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepFngStateGet(uint32_t mdIndex, uint32_t maIndex,
                                   uint32_t mepId, uint32_t intIfNum,
                                   uint32_t *val);

/*********************************************************************
* @purpose  Obtain maintenance end point on an interface parameter
*           defined by section 17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    paramCode  @b{(input)} MEP object parameter defined by
*                                  Dot1agCfmMepEntry_t
* @param    val       @b{(output)} Param value
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepParamGet(uint32_t mdIndex, uint32_t maIndex,
                                uint32_t mepId, uint32_t intIfNum,
                                uint32_t paramCode, void *val);

/*********************************************************************
* @purpose  Obtain priority to be used in LTMs, CCMs sent by the
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val       @b{(output)} Priority value for CCMs and LTMs
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepCcmLtmPriorityGet(uint32_t mdIndex, uint32_t maIndex,
                                         uint32_t mepId, uint32_t intIfNum,
                                         uint32_t *val);

/*********************************************************************
* @purpose  Set priority to be used in LTMs, CCMs sent by the
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Priority value for CCMs and LTMs
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepCcmLtmPrioritySet(uint32_t mdIndex, uint32_t maIndex,
                                         uint32_t mepId, uint32_t intIfNum,
                                         uint32_t val);

/*********************************************************************
* @purpose  Obtain MAC Address of the
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    macAddr    @b{(output)} MAC Addr of the MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepMacAddressGet(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t intIfNum,
                                        uint8_t *macAddr);

/*********************************************************************
* @purpose  Obtain "dot1agCfmMepLowPrDef -
*           An integer value specifying the lowest priority defect
*           that is allowed to generate fault alarm" of
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val       @b{(output)} Lowest priority defect value
*                                  Dot1agCfmLowestAlarmPri_t
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLowPrDefGet(uint32_t mdIndex, uint32_t maIndex,
                                      uint32_t mepId, uint32_t intIfNum,
                                      uint32_t *val);

/*********************************************************************
* @purpose  Set "dot1agCfmMepLowPrDef -
*           An integer value specifying the lowest priority defect
*           that is allowed to generate fault alarm" of
*           maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val       @b{(input)} Lowest priority defect value
*                                  Dot1agCfmLowestAlarmPri_t
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLowPrDefSet(uint32_t mdIndex, uint32_t maIndex,
                                      uint32_t mepId, uint32_t intIfNum,
                                      uint32_t val);

/*********************************************************************
* @purpose  Obtain dot1agCfmMepFngAlarmTime -
*           The time that defects must be present
*           before a Fault Alarm is issued (fngAlarmTime. 20.33.3)
*           (default 250 = 2.5s)
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val    @b{(output)}    Time interval expressed as t*10 where
*                                  t is Time interval in seconds
*                                  ex: 2.5s = 250
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepFngAlarmTimeGet(uint32_t mdIndex, uint32_t maIndex,
                                          uint32_t mepId, uint32_t intIfNum,
                                          uint32_t *val);

/*********************************************************************
* @purpose Set dot1agCfmMepFngAlarmTime -
*          The time that defects must be present
*          before a Fault Alarm is issued (fngAlarmTime. 20.33.3)
*          (default 250 = 2.5s)
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Time interval expressed as t*100
*                                  where t is Time interval in seconds
*                                  ex: 2.5s = 250
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepFngAlarmTimeSet(uint32_t mdIndex, uint32_t maIndex,
                                          uint32_t mepId, uint32_t intIfNum,
                                          uint32_t val);

/*********************************************************************
* @purpose Set dot1agCfmMepFngResetTime -
*          The time that defects must be absent before resetting a
*          Fault Alarm (fngResetTime, 20.33.4) (default 10s).
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Time interval expressed as t*100
*                                  where t is Time interval in seconds
*                                  ex: 2.5s = 250
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepFngResetTimeGet(uint32_t mdIndex, uint32_t maIndex,
                                          uint32_t mepId, uint32_t intIfNum,
                                          uint32_t *val);

/*********************************************************************
* @purpose Set dot1agCfmMepFngResetTime -
*          The time that defects must be absent before resetting a
*          Fault Alarm (fngResetTime, 20.33.4) (default 10s).
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Time interval expressed as t*100
*                                  where t is Time interval in seconds
*                                  ex: 2.5s = 250
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepFngResetTimeSet(uint32_t mdIndex, uint32_t maIndex,
                                          uint32_t mepId, uint32_t intIfNum,
                                          uint32_t val);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmInterval -
*          Set the period between LBM messages for ETH over transport
*          OAM
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    interval   @b{(input)} A value indicating interval between
*                                  LBM messages
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmIntervalSet(uint32_t mdIndex, uint32_t maIndex,
                                                 uint32_t mepId, uint32_t intIfNum,
                                                 uint32_t interval);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmResultOk -
*          A Boolean flag set to true by the bridge port to indicate
*          that another LBM may be transmitted.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag       @b{(input)} A value of OFDPA_TRUE indicates LBMs
*                                  can be sent by the MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmResultOkSet(uint32_t mdIndex, uint32_t maIndex,
                                                 uint32_t mepId, uint32_t intIfNum,
                                                 OFDPA_BOOL flag);

/*********************************************************************
* @purpose Set tdot1agCfmMepTransmitLbmStatus -
*          A Boolean flag set to true by the bridge port to indicate
*          that another LBM may be transmitted.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag       @b{(input)} A value of OFDPA_TRUE indicates LBMs
*                                  can be sent by the MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmStatusSet(uint32_t mdIndex, uint32_t maIndex,
                                               uint32_t mepId, uint32_t intIfNum,
                                               OFDPA_BOOL flag);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmMessages -
*          The number of Loopback messages to be transmitted.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Number of LBMs to be sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmMessagesSet(uint32_t mdIndex, uint32_t maIndex,
                                                 uint32_t mepId, uint32_t intIfNum,
                                                 uint32_t val);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmDestMacAddress
*          dot1agCfmMepTransmitLbmDestIsMepId -
*          The Target MAC Address Field to be transmitted: A unicast
*          destination MAC address. This address will be used if the
*          value of the column dot1agCfmMepTransmitLbmDestIsMepId is
*          'false'.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    dmac       @b{(input)} DMAC for the MEP to which LBM is
*                                  sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmDestMacAddressSet(uint32_t mdIndex,
                                                       uint32_t maIndex,
                                                       uint32_t mepId,
                                                       uint32_t intIfNum,
                                                       uint8_t *dmac);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmDestIsMepId -
*
*          True indicates that MEPID of the target MEP is used for
*          Loopback transmission. False indicates that unicast destination
*          MAC address of the target MEP is used for Loopback transmission.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag       @b{(input)} OFDPA_TRUE/OFDPA_FALSE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmDestIsMepIdSet(uint32_t mdIndex,
                                                    uint32_t maIndex,
                                                    uint32_t mepId,
                                                    uint32_t intIfNum,
                                                    OFDPA_BOOL flag);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmDestMepId,
*          dot1agCfmMepTransmitLbmDestIsMepId -
*          The Maintenance association End Point Identifier of another
*          MEP in the same Maintenance Association to which the LBM is
*          to be sent.This address will be used if the value of the column
*          dot1agCfmMepTransmitLbmDestIsMepId is 'true'.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    destMepid  @b{(input)} Desstination MEPID to which LBM is
*                                  to be sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmDestMepIdSet(uint32_t mdIndex, uint32_t maIndex,
                                                  uint32_t mepId, uint32_t intIfNum,
                                                  uint32_t destMepId);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmVlanPrio,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    vlanPrio   @b{(input)} vlan priority to be set in LBM header
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmVlanPrioSet(uint32_t mdIndex, uint32_t maIndex,
                                                 uint32_t mepId, uint32_t intIfNum,
                                                 uint32_t vlanPrio);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmVlanDropEnable,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    dropEnable @b{(input)} Vlan Drop Enable to be set in LBM header
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmVlanDropEnableSet(uint32_t mdIndex, uint32_t maIndex,
                                                       uint32_t mepId, uint32_t intIfNum,
                                                       OFDPA_BOOL dropEnable);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbPacketLength,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    packetLength    @b{(input)} packet length to be set
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmPacketLengthSet(uint32_t mdIndex, uint32_t maIndex,
                                                     uint32_t mepId, uint32_t intIfNum,
                                                     uint32_t packetLength);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmDiscovery
*
* @param    mdIndex     @b{(input)} Index used for MD indentification
* @param    maIndex     @b{(input)} Index for MA identification
* @param    mepId       @b{(input)} Index to identify a MEP
* @param    intIfNum    @b{(input)} Interface on which mep is to be
*                                   created
* @param    lbDiscovery @b{(input)} lbDiscovery test mode to be set.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmDiscoverySet(uint32_t mdIndex, uint32_t maIndex,
                                                  uint32_t mepId, uint32_t intIfNum,
                                                  OFDPA_OAM_LB_DISCOVERY_TYPE_t lbDiscovery);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmTestTlvType,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    tlvType    @b{(input)} Tlv Type to be set
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmTestTlvSet(uint32_t mdIndex, uint32_t maIndex,
                                                uint32_t mepId, uint32_t intIfNum,
                                                OFDPA_OAM_LB_TEST_TLV_TYPE_t tlvType);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLbmTtl,
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    Ttl        @b{(input)} Ttl to bet set in LBM sent to MIP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLbmTtlSet(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t intIfNum,
                                            uint32_t Ttl);

/*********************************************************************
* @purpose Send an LBM on a give MEP
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    destMac    @b{(input)} MAC address of MP to which LBM is
*                                  sent
* @param    destMepid  @b{(input)} Destination MEPID to which LBM is
*                                  to be sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLbmSend(uint32_t mdIndex, uint32_t maIndex,
                               uint32_t mepId,
                               uint8_t *destMac, uint32_t destMepId,
                               uint32_t msgCount);

/*********************************************************************
* @purpose Stop an LBM on a give MEP
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLbmStop(uint32_t mdIndex, uint32_t maIndex,
                               uint32_t mepId);

/*********************************************************************
* @purpose Obtain dot1agCfmMepHighestPrDefect -
*          The highest priority defect that has been present since the
*          MEPs Fault Notification Generator State Machine was last in
*          the FNG_RESET state.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Highest priorty defect value
*                                  Dot1agCfmHighestDefectPri_t
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepHighestPrDefectGet(uint32_t mdIndex, uint32_t maIndex,
                                             uint32_t mepId, uint32_t intIfNum,
                                             uint32_t *val);

/*********************************************************************
* @purpose Obtain dot1agCfmMepDefects - MEP defects
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} MEP defect
*                                  Dot1agCfmMepDefects_t
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepDefectsGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum,
                                     uint32_t *val);

/*********************************************************************
* @purpose Obtain dot1agCfmMepCcmSequenceErrors - The total number of
*          out-of-sequence CCMs received from all remote MEPs.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  CCM out of sequence rx pkt count
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepCcmSequenceErrors(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t intIfNum,
                                            uint32_t *val);

/*********************************************************************
* @purpose Obtain dot1agCfmMepCcmSequenceErrors - The total number of
*          out-of-sequence CCMs received from all remote MEPs.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  CCM out of sequence rx pkt count
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepCcmSequenceErrorsGet(uint32_t mdIndex, uint32_t maIndex,
                                               uint32_t mepId, uint32_t intIfNum,
                                               uint32_t *val);

/*********************************************************************
* @purpose Obtain dot1agCfmMepCciSentCcms - The total number of
*          Continuity Check messages transmitted.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  CCM pkt tx count
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepCciSentCcmsGet(uint32_t mdIndex, uint32_t maIndex,
                                         uint32_t mepId, uint32_t intIfNum,
                                         uint32_t *val);

/*********************************************************************
* @purpose Obtain dot1agCfmMepNextLbmTransId -
*          Next sequence number/transaction identifier to be sent in a
*          Loopback message. This sequence number can be zero because
*          it wraps around.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  Next LBM transaction Identifier
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepNextLbmTransIdGet(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t intIfNum,
                                            uint32_t *val);

/*********************************************************************
* @purpose Obtain dot1agCfmMepLbrIn -
*          Total number of valid, in-order Loopback Replies received.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  LBR rx count
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLbrInGet(uint32_t mdIndex, uint32_t maIndex,
                                   uint32_t mepId, uint32_t intIfNum,
                                   uint32_t *val);

/*********************************************************************
* @purpose Obtain dot1agCfmMepLbrInOutOfOrder -
*          The total number of valid, out-of-order Loopback Replies
*          received.
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} LBR rx out-of-order count
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLbrInOutOfOrderGet(uint32_t mdIndex, uint32_t maIndex,
                                             uint32_t mepId, uint32_t intIfNum,
                                             uint32_t *val);

/*********************************************************************
* @purpose Obtain dot1agCfmMepLbrBadMsdu -
*          The total number of LBRs received whose
*          mac_service_data_unit did not match (except for the OpCode)
*          that of the corresponding LBM (20.2.3).
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)} Lbr rx count with mismatch
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepLbrBadMsduGet(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t intIfNum,
                                        uint32_t *val);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLtmTargetMacAddress
*          The Target MAC Address Field to be transmitted: A unicast
*          destination MAC address. This address will be used if the
*          value of the column dot1agCfmMepTransmitLtmTargetIsMepId is
*          'false'.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    dmac       @b{(input)} DMAC for the MEP to which LTM is
*                                  sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmTargetMacAddressSet(uint32_t mdIndex,
                                                         uint32_t maIndex,
                                                         uint32_t mepId,
                                                         uint32_t intIfNum,
                                                         uint8_t *dmac);

/*********************************************************************
* @purpose  Set dot1agCfmMepTransmitLtmTargetMepId
*           An indication of the Target MAC Address Field to be
*           transmitted: The Maintenance association End Point Identifier
*           of another MEP in the same Maintenance Association
*           This address will be used if the value of the column
*           dot1agCfmMepTransmitLtmTargetIsMepId is 'true'.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    destMepId  @b{(input)} MEPID for the MEP to which LTM is
*                                  sent
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmTargetMepIdSet(uint32_t mdIndex,
                                                    uint32_t maIndex,
                                                    uint32_t mepId,
                                                    uint32_t intIfNum,
                                                    uint32_t destMepId);

/*********************************************************************
* @purpose  Set dot1agCfmMepTransmitLtmTargetIsMepId
*           True indicates that MEPID of the target MEP is used for
*           Linktrace transmission. False indicates that unicast
*           destination MAC address of the target MEP is used for
*           Loopback transmission.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag  @b{(input)}  OFDPA_TRUE/OFDPA_FALSE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmTargetIsMepIdSet(uint32_t mdIndex,
                                                      uint32_t maIndex,
                                                      uint32_t mepId,
                                                      uint32_t intIfNum,
                                                      OFDPA_BOOL flag);

/*********************************************************************
* @purpose  Set dot1agCfmMepTransmitLtmTtl -
*           The LTM TTL field. Default value, if not specified, is 64.
*           The TTL field indicates the number of hops remaining to the
*           LTM.  Decremented by 1 by each Linktrace Responder that
*           handles the LTM.  The value returned in the LTR is one less
*           than that received in the LTM.  If the LTM TTL is 0 or 1, the
*           LTM is not forwarded to the next hop, and if 0, no LTR is
*           generated.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    val        @b{(input)}  TTL for the LTM message
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmTtlSet(uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t intIfNum,
                                            uint32_t val);

/*********************************************************************
* @purpose Set dot1agCfmMepTransmitLtmFlags - The flags field for LTMs
*          transmitted by the MEP. Transmission is initiated by writing
*          a value to the dot1agCfmMepTransmitLtmFlags object
*
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    flag       @b{(input)} A value of 0x01 to start transmission
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmMepTransmitLtmFlagsSet(uint32_t mdIndex, uint32_t maIndex,
                                              uint32_t mepId, uint32_t intIfNum,
                                              uint8_t flag);

/*********************************************************************
* @purpose Send an LTM on a give MEP
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    destMac    @b{(input)} MAC address of MP to which LTM is
*                                  sent
* @param    destMepid  @b{(input)} Destination MEPID to which LTM is
*                                  to be sent
* @param    ttl        @b{(input)} Max hops to be used in LTM
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLtmSend(uint32_t mdIndex, uint32_t maIndex,
                               uint32_t mepId, uint8_t *destMac,
                               uint32_t destMepId, uint8_t ttl);

/*********************************************************************
* @purpose Clear Traceroute Cache
*
* @param    none
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_REQUEST_DENIED
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmTraceRouteCacheClear();

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfModeSet
*           AIS/LCK/CSF are set as enable/disable based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(input)} AIS/LCK/CSF mode {enable, disable}
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfModeSet(uint32_t mdIndex,
                                           uint32_t maIndex,
                                           uint32_t mepId,
                                           uint32_t intIfNum,
                                           uint32_t defectType,
                                           OFDPA_BOOL mode);

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfModeSet
*           AIS/LCK/CSF moded (enable/disable) are based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)} AIS/LCK/CSF mode {enable, disable}
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfModeGet(uint32_t mdIndex,
                                           uint32_t maIndex,
                                           uint32_t mepId,
                                           uint32_t intIfNum,
                                           uint32_t defectType,
                                           OFDPA_BOOL *mode);

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfClientLevelSet
*           AIS/LCK/CSF client level is set based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    clientLevel@b{(input)} AIS/LCK/CSF client level
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfClientLevelSet(uint32_t mdIndex,
                                                  uint32_t maIndex,
                                                  uint32_t mepId,
                                                  uint32_t intIfNum,
                                                  uint32_t defectType,
                                                  uint8_t  clientLevel);

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfClientLevelGet
*           AIS/LCK/CSF client level is get based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    clientLevel@b{(output)} AIS/LCK/CSF client level
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfClientLevelGet(uint32_t mdIndex,
                                                  uint32_t maIndex,
                                                  uint32_t mepId,
                                                  uint32_t intIfNum,
                                                  uint32_t defectType,
                                                  uint8_t *clientLevel);

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfPrioritySet
*           AIS/LCK/CSF priority is set based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    priority   @b{(input)} AIS/LCK/CSF priority
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfPrioritySet(uint32_t mdIndex,
                                               uint32_t maIndex,
                                               uint32_t mepId,
                                               uint32_t intIfNum,
                                               uint32_t defectType,
                                               uint8_t  priority);

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfPriorityGet
*           AIS/LCK/CSF priority is get based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    clientLevel@b{(output)} AIS/LCK/CSF priority
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfPriorityGet(uint32_t mdIndex,
                                               uint32_t maIndex,
                                               uint32_t mepId,
                                               uint32_t intIfNum,
                                               uint32_t defectType,
                                               uint8_t *priority);

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfIntervalSet
*           AIS/LCK/CSF interval is set based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    interval   @b{(input)} AIS/LCK/CSF interval
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfIntervalSet(uint32_t mdIndex,
                                               uint32_t maIndex,
                                               uint32_t mepId,
                                               uint32_t intIfNum,
                                               uint32_t defectType,
                                               uint32_t interval);

/*********************************************************************
* @purpose  Set dot1agCfmMepAisLckCsfIntervalGet
*           AIS/LCK/CSF interval is get based on the assigned MEP.
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    interval   @b{(output)} AIS/LCK/CSF interval
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*******************************************************************/
OFDPA_ERROR_t dot1agCfmMepAisLckCsfIntervalGet(uint32_t mdIndex,
                                               uint32_t maIndex,
                                               uint32_t mepId,
                                               uint32_t intIfNum,
                                               uint32_t defectType,
                                               uint32_t *interval);

/*********************************************************************
* @purpose  Create a maintenance end point on an interface given
*           (mdIndex) and (maIndex) and mepId
*           and direction
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    direction  @b{(input)} Direction of MEP to be created(Up/Down)
* @param    macAddress @b{(input)} MAC address for PDUs sent from this MEP
* @param    lmepId     @b{(input)} unique identifier of this MEP entry in the OF-DPA MEP table
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_REQUEST_DENIED      No software and hardware mix of MEPs
*                                       is allowed for a given port and VLAN
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepCreate(uint32_t intIfNum, uint32_t mdIndex,
                              uint32_t maIndex, uint32_t mepId,
                              Dot1agCfmMpDirection_t direction,
                              uint8_t *macAddress, uint32_t lmepId);

/*********************************************************************
* @purpose  Delete a maintenance end point on an interface given
*           (mdIndex) and (maIndex) and mepId
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  deleted
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP. Use NULL for
*                                  other UI's
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP. Use NULL for other UI's
* @param    mepId      @b{(input)} Index used to identify a MEP.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepDelete(uint32_t intIfNum, uint32_t mdIndex,
                              uint32_t maIndex, uint32_t mepId);

/*********************************************************************
* @purpose  Verify if an entry with given values exists in LTR Table
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    ltrSeqNumber    @b{(input)} Index used to identify the
*                                       remote MEP which sent the LTR
* @param    ltrReceiveOrder @b{(input)} An index to distinguish
*                                       among multiple LTRs with the same
*                                       LTR Transaction Identifier field
*                                       value.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTREntryIsValid(uint32_t mdIndex, uint32_t maIndex,
                                       uint32_t mepId, uint32_t ltrSeqNumber,
                                       uint32_t ltrReceiveOrder);

/*********************************************************************
* @purpose  Obtain Linktrace replies received by a specific MEP in
*           response to a linktrace message. As
*           defined by section 17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    ltrSeqNumber    @b{(input)} Index used to identify the
*                                       remote MEP which sent the LTR
* @param    ltrReceiveOrder @b{(input)} An index to distinguish
*                                       among multiple LTRs with the same
*                                       LTR Transaction Identifier field
*                                       value.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes  Use NULL to obtain first valid entry
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTREntryNextGet(uint32_t *mdIndex, uint32_t *maIndex,
                                       uint32_t *mepId, uint32_t *ltrSeqNumber,
                                       uint32_t *ltrReceiveOrder);

/*********************************************************************
* @purpose  Obtain Linktrace replies received by a specific MEP in
*           response to a linktrace message. As
*           defined by section 17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    ltrSeqNumber    @b{(input)} Index used to identify the
*                                       remote MEP which sent the LTR
* @param    ltrReceiveOrder @b{(input)} An index to distinguish
*                                       among multiple LTRs with the same
*                                       LTR Transaction Identifier field
*                                       value.
* @param    paramCode  @b{(input)} LTR parameter defined by
*                                  dot1agCfmLtrEntry_t
* @param    val       @b{(output)} Param value
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTRParamGet(uint32_t mdIndex, uint32_t maIndex,
                                   uint32_t mepId, uint32_t ltrSeqNumber,
                                   uint32_t ltrReceiveOrder, uint32_t paramCode,
                                   void *val);

/*********************************************************************
* @purpose  Verify if an entry with given values exists in RMEP Table
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    rmepId     @b{(input)} Index used to identify the
*                                       remote MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRMepEntryIsValid(uint32_t mdIndex, uint32_t maIndex,
                                        uint32_t mepId, uint32_t rmepId);

/*********************************************************************
* @purpose  Obtain RMep entries collected by a specific MEP.
*           As defined by section 17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    rmepId     @b{(input)} Index used to identify the
*                                       remote MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes Use NULL to obtain next valid entry
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRMepEntryNextGet(uint32_t *mdIndex, uint32_t *maIndex,
                                        uint32_t *mepId, uint32_t *rmepId);

/*********************************************************************
* @purpose  Obtain RMep entries collected by a specific MEP.
*           As defined by section 17.5 802.1ag d8-1
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} VLAN ID associated with the MA.
* @param    rmepId     @b{(input)} Index used to identify the
*                                       remote MEP
* @param    paramCode  @b{(input)} RMep parameter defined by
*                                  dot1agCfmMepDbEntry_t
* @param    val       @b{(output)} Param value
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRMepParamGet(uint32_t mdIndex, uint32_t maIndex,
                                    uint32_t mepId, uint32_t rmepId,
                                    uint32_t paramCode, void *val);

/*********************************************************************
* @purpose  Verify if an entry with given values exists in CFM Stack
*
* @param    intIfNum   @b{(input)} Interface on which MP is configured
* @param    primVID    @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel    @b{(input)} Domain level
* @param    direction  @b{(output)} Direction of MP (Up or Down)
*                                   For MIP it is always Up
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmStackEntryIsValid(uint32_t intIfNum, uint32_t primVID,
                                         int mdLevel,
                                         Dot1agCfmMpDirection_t direction);

/*********************************************************************
* @purpose  Obtain CFM MP Stack entries configured
*           As defined by section 17.5 802.1ag d8-1
*
*
* @param    intIfNum   @b{(input)} Interface on which MP is configured
* @param    primVID    @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel    @b{(input)} Domain level to be set
* @param    direction  @b{(output)} Direction of MP (Up or Down)
*                                   For MIP it is always Up
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes Use NULL to obtain next valid entry
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmStackEntryNextGet(uint32_t *intIfNum, uint32_t *primVID,
                                         int *mdLevel,
                                         Dot1agCfmMpDirection_t *direction);

/*********************************************************************
* @purpose  Obtain CFM MP Stack entries configured
*           As defined by section 17.5 802.1ag d8-1
*
* @param    intIfNum   @b{(input)} Interface on which MP is configured
* @param    primVID    @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel    @b{(input)} Domain level to be set
* @param    direction  @b{(output)} Direction of MP (Up or Down)
*                                   For MIP it is always Up
* @param    paramCode  @b{(input)} CFM Stack MP parameter defined by
*                                  dot1agCfmStackEntry_t
* @param    val       @b{(output)} Param value
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_NOT_FOUND
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmStackParamGet(uint32_t intIfNum, uint32_t primVID,
                                     int mdLevel, Dot1agCfmMpDirection_t direction,
                                     uint32_t paramCode, void *val);

/*********************************************************************
* @purpose  Create a maintenance domain intermediate point
*
* @param    intIfNum     @b{(input)} Interface on which mep is to be
*                                    created/deleted
* @param    primVID      @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel      @b{(input)} MD level of the MD for which MIP
*                                    is to be created/deleted.
* @param    direction    @b{(input)} Direction of MP (Up or Down)
* @param    macAddr      @b{(input)} MAC address for Ethernet MIP
* @param    mipIdTlvData @b{(input)} MIP ID TLV for BHH
* @param    localMpId    @b{(input)} OF-DPA MP ID
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_REQUEST_DENIED
*
* @notes    Refer to 802.1ag d8-1 section 22.2.3  -- not following this
*           because in OF-DPA, non-Ethernet MIPs cannot be created automatically.
*           This API will cause a local MIP to be created as a result of
*           a configuration action.
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipCreate(uint32_t intIfNum, uint32_t primVID, int mdLevel,
                              Dot1agCfmMpDirection_t direction,
                              uint8_t *macAddr, uint8_t *mipIdTlvData, uint32_t localMpId);

/*********************************************************************
* @purpose  Delete a maintenance domain intermediate point
*
* @param    intIfNum     @b{(input)} Interface on which mep is to be
*                                    created/deleted
* @param    primVID      @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel      @b{(input)} MD level of the MD for which MIP
*                                    is to be created/deleted.
* @param    direction    @b{(input)} Direction of MP (Up or Down)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_REQUEST_DENIED
*
* @notes    Refer to 802.1ag d8-1 section 22.2.3  -- not following this
*           because in OF-DPA, non-Ethernet MIPs cannot be created automatically.
*           This API will cause a local MIP to be deleted as a result of
*           a configuration action.
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipDelete(uint32_t intIfNum, uint32_t primVID, int mdLevel,
                              Dot1agCfmMpDirection_t direction);

/*********************************************************************
* @purpose  Enable/Disable a maintenance intermediate point on an
*           interface for a given domain level
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created/deleted
* @param    mdLevel    @b{(input)} MD level of the MD for which MIP
*                                  is to be created/deleted.
* @param    macAddr    @b{(input)} MAC address for MIP
* @param    mode       @b{(input)} OFDPA_ENABLE /OFDPA_DISABLE
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Refer to 802.1ag d8-1 section 22.2.3
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipModeSet(uint32_t intIfNum, int mdLevel,
                               uint8_t *macAddr, uint32_t mode);

/*********************************************************************
* @purpose  Enable/Disable a maintenance intermediate point on an
*           interface for a given domain level
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created/deleted
* @param    mdLevel    @b{(input)} MD level of the MD for which MEP
*                                  is to be created/deleted.
* @param    mode       @b{(output)} OFDPA_ENABLE /OFDPA_DISABLE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Refer to 802.1ag d8-1 section 22.2.3
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipModeGet(uint32_t intIfNum, int mdLevel,
                               uint32_t *mode);

/*********************************************************************
* @purpose  Enable/Disable CFM
*
* @param    mode       @b{(input)} OFDPA_ENABLE /OFDPA_DISABLE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Not standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmModeSet(uint32_t mode);

/*********************************************************************
* @purpose  Enable/Disable mode of CFM
*
* @param    mode       @b{(output)} OFDPA_ENABLE /OFDPA_DISABLE
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmModeGet(uint32_t *mode);

/*********************************************************************
* @purpose  Obtain archive hold time. The time interval after which
*           RMEPs in RMEP_FAILED state are removed
*
* @param    timerVal       @b{(output)}
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmArchiveHoldTimeGet(uint32_t *timerVal);

/*********************************************************************
* @purpose  Set archive hold time. The time interval after which
*           RMEPs in RMEP_FAILED state are removed
*
* @param    timerVal       @b{(input)}
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmArchiveHoldTimeSet(uint32_t timerVal);

/*********************************************************************
* @purpose  Obtain Maintenance Domain Format
*
* @param    mdIndex  @b{(input)}
*           mdFormat @b{(output)}
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    standard config param
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMdFormatGet(uint32_t mdIndex, uint32_t *mdFormat);

/*********************************************************************
* @purpose  Verify if a give maintenance Aossication exists
*
* @param    mdIndex    @b{(input)} index value used by SNMP.
*           maIndex    @b{(input)} index value used by SNMP.
*
* @returns  OFDPA_TRUE
* @returns  OFDPA_FALSE
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_BOOL dot1agMaIndexIsValid(uint32_t mdIndex, uint32_t maIndex);

/*********************************************************************
* @purpose  Obtain a next valid mip value given a maintenance domain
*
* @param    mdIndex  @b{(input/output)} Index value used by SNMP
*                                       Use NULL to obtain first valid
*                                       value.
*           ifIndex  @b{(input/output)} Interface Index used by SNMP.
*                                       Use NULL to obtain first valid
*                                       value.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMipNextGet(uint32_t *mdIndex, uint32_t *ifIndex);

/*********************************************************************
* @purpose  Add a maintenance end point on an interface given
*           (mdIndex) and (maIndex) mepId rmepId
*           and direction
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP. Use NULL for other UI's
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    rmepId     @b{(input)} Index to identify a remote MEP
* @param    srcMac     @b{(input)} MAC address of remote MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agRMepEntryAdd(uint32_t intIfNum, uint32_t mdIndex,
                                 uint32_t maIndex, uint32_t mepId,
                                 uint32_t rmepId, uint8_t *srcMac);

/*********************************************************************
* @purpose  Delete remote  maintenance end point on an interface given
*           (mdIndex) and (maIndex), mepId and rmepId
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  deleted
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP. Use NULL for
*                                  other UI's
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP. Use NULL for other UI's
* @param    mepId      @b{(input)} Index used to identify a MEP.
* @param    rmepId      @b{(input)} Index used to identify a MEP.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agRMepEntryDelete(uint32_t intIfNum, uint32_t mdIndex,
                                    uint32_t maIndex, uint32_t mepId,
                                    uint32_t rmepId);

/*********************************************************************
* @purpose  Determine if the specified MAC address is a member of the
*            restricted set.
*
* @param  *macaddr - pointer to mac address
*
* @returns OFDPA_TRUE if restricted mac, OFDPA_FALSE otherwise
*
* @end
*
*********************************************************************/
OFDPA_BOOL dot1agIsRestrictedMacAddress(uint8_t *macaddr);

/***************************************************************************
* @purpose  Returns the status of hardware enabled MEPs
*
* @param    mepId      @b{(input)} Index to identify a MEP
*
* @returns  OFDPA_TRUE    If device supports hardware enabled dot1ag and MEP Id
*                      is within the supported range
* @returns  OFDPA_FALSE   Otherwise
*
* @notes
*
* @end
*****************************************************************************/
OFDPA_BOOL dot1agIsMepHwEnabled(uint32_t mepId);

void dot1agApsRcvCallBackRegister(void *callback);

/*********************************************************************
* @purpose  Create a PROLM on one MEP
*
* @param    intIfNum       @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex        @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex        @b{(input)} Index for MA identification used by
*                                  SNMP. Use NULL for other UI's
* @param    mepId          @b{(input)} Index to identify a MEP
* @param    destMac        @b{(input)} MAC address of MP to which LM is
*                                      measured
* @param    destMepid      @b{(input)} Destination MEPID to which LM is
*                                      measured
* @param    priorityClass  @b{(input)} Priority class (CoS) of the traffic that CCM-based LM tool should monitor
* @param    toolType       @b{(input)} Tool Type to specify CCM, LMM or SLM based LM
* @param    testId         @b{(input)} TestId to be used for SLM based LM.
*                                      can be set to 0 for non-SLM based LM
* @param    controlPeriod  @b{(input)} Periodicity at which LMM and SLM frames should be sent,
*                                      not applicable for CCM based LM
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepProlmCreate(uint32_t intIfNum, uint32_t mdIndex,
                                   uint32_t maIndex, uint32_t mepId,
                                   uint8_t *destMac, uint32_t destMepId,
                                   uint32_t priorityClass, uint8_t toolType,
                                   uint32_t testId, uint32_t controlPeriod);

/*********************************************************************
* @purpose  Delete PROLM on one MEP
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP. Use NULL for other UI's
* @param    mepId      @b{(input)} Index to identify a MEP
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMepProlmDelete(uint32_t intIfNum, uint32_t mdIndex,
                                   uint32_t maIndex, uint32_t mepId);

/*********************************************************************
* @purpose  Set PROLM flag of maintenance end point on an interface
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mode       @b{(output)}  CCI enabled flag of MEP (OFDPA_TRUE or
*                                    OFDPA_FALSE)
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepProlmEnabledSet(uint32_t intIfNum, uint32_t mdIndex,
                                       uint32_t maIndex, uint32_t mepId,
                                       OFDPA_BOOL mode);

/*********************************************************************
* @purpose  Get CCM database entry corresponding to mepId and remote mepId 
*
* @param    mdIndex     @b{(input)} Index used for MD indentification
*                                   used by SNMP.
* @param    maIndex     @b{(input)} Index for MA identification used by SNMP.
* @param    mepId       @b{(input)} Index to identify a MEP
* @param    remoteMepId @b{(input)} Index of corresponding rmep entry
* @param    dbEntry     @b{(output)}Structure to return entry details
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_NOT_FOUND
* @returns  OFDPA_E_INTERNAL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agOamMepCCMDatabaseEntryGet(uint32_t mdIndex, uint32_t maIndex, 
                                              uint32_t mepId, uint32_t remoteMepId,
                                              ofdpaOamCcmDatabaseEntry_t *dbEntry);

/*********************************************************************
* @purpose  Find CCM database entry corresponding to mepId and remote mepId 
*           and return next valid remote mepId 
*
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                       used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification 
*                                       used by SNMP.
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    remoteMepId     @b{(input)} Index of corresponding rmep entry
* @param    nextRemoteMepId @b{(output)}Index of next rmep entry
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_NOT_FOUND
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agOamMepCCMDatabaseEntryNextGet(uint32_t mdIndex, uint32_t maIndex, 
                                                  uint32_t mepId, uint32_t remoteMepId,
                                                  uint32_t *nextRemoteMepId);

/*********************************************************************
* @purpose  start Ondem LM on MEP
*
*
* @param    intIfNum        @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification used by
*                                  SNMP. Use NULL for other UI's
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    destMac         @b{(input)} MAC address of MP to which LBM is
*                                  sent
* @param    destMepid       @b{(input)} Destination MEPID to which LBM is
*                                  to be sent
* @param    controlPriority @b{(input)} LMM/LMR frame priority, 0 -7, which will be carried in LMM/LMR frames
* @param    priorityClass   @b{(input)} Priority class (CoS) of the traffic that CCM-based LM tool should monitor
* @param    controlPeriod   @b{(input)} could be 100ms (by default), 10ms, 1s. It will not carried in LMM/LMR frames
* @param    msgCount        @b{(input)}
* @param    toolType        @b{(input)} Tool Type to specify CCM, LMM or SLM based LM
* @param    testId          @b{(input)} TestId to be used for SLM based LM.
*                                       can be set to 0 for non-SLM based LM
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmOndemLmmStart(uint32_t intIfNum, uint32_t mdIndex,
                                     uint32_t maIndex, uint32_t mepId,
                                     uint8_t *destMac, uint32_t destMepId,
                                     uint32_t controlPriority, uint32_t priorityClass,
                                     uint32_t controlPeriod, uint32_t msgCount, uint8_t toolType,
                                     uint32_t testId);

/*********************************************************************
* @purpose  stop Ondem LM on MEP
*
*
* @param    intIfNum        @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification used by
*                                  SNMP. Use NULL for other UI's
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    controlPriority @b{(input)} LMM/LMR frame priority, 0 -7, which will be carried in LMM/LMR frames
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmOndemLmmStop(uint32_t intIfNum, uint32_t mdIndex,
                                    uint32_t maIndex, uint32_t mepId,
                                    uint32_t controlPriority);

/*********************************************************************
* @purpose  start DM on MEP
*
*
* @param    intIfNum        @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification used by
*                                  SNMP. Use NULL for other UI's
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    destMac         @b{(input)} MAC address of MP to which LBM is
*                                  sent
* @param    destMepid       @b{(input)} Destination MEPID to which LBM is
*                                  to be sent
* @param    controlPriority @b{(input)} LMM/LMR frame priority, 0 -7, which will be carried in LMM/LMR frames
* @param    controlPeriod   @b{(input)} could be 100ms (by default), 10ms, 1s. It will not carried in LMM/LMR frames
* @param    dmType          @b{(input)} ProDM or OnDemDM
* @param    msgCount        @b{(input)}
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmDmmStart(uint32_t intIfNum, uint32_t mdIndex,
                                uint32_t maIndex, uint32_t mepId,
                                uint8_t *destMac, uint32_t destMepId,
                                uint32_t controlPriority, uint32_t controlPeriod,
                                uint32_t frameLen, uint32_t dmType, uint32_t msgCount);

/*********************************************************************
* @purpose  stop DM on MEP
*
*
* @param    intIfNum        @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex         @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex         @b{(input)} Index for MA identification used by
*                                  SNMP. Use NULL for other UI's
* @param    mepId           @b{(input)} Index to identify a MEP
* @param    controlPriority @b{(input)} LMM/LMR frame priority, 0 -7, which will be carried in LMM/LMR frames
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmDmmStop(uint32_t intIfNum, uint32_t mdIndex,
                               uint32_t maIndex, uint32_t mepId,
                               uint32_t controlPriority);

/*********************************************************************
* @purpose  Obtain maintenance end point on Y1731 PM
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731PmGet(uint32_t mdIndex, uint32_t maIndex,
                                  uint32_t mepId, uint32_t intIfNum);

/*********************************************************************
* @purpose  Obtain maintenance end point on Y1731 PM
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731ProLmGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum);

/*********************************************************************
* @purpose  Obtain maintenance end point on Y1731 PM
*
* @param    mdIndex    @b{(input)} Index used for MD indentification
*                                  used by SNMP.
* @param    maIndex    @b{(input)} Index for MA identification used by
*                                  SNMP.
* @param    mepId      @b{(input)} Index to identify a MEP
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731ProDmGet(uint32_t mdIndex, uint32_t maIndex,
                                     uint32_t mepId, uint32_t intIfNum);

/*********************************************************************
* @purpose  Retrieve Proactive LM counters.
*
* @param    mdIndex       @b{(input)} Index used for MD indentification
*                                     used by SNMP.
* @param    maIndex       @b{(input)} Index for MA identification used by
*                                     SNMP.
* @param    mepId         @b{(input)} Index to identify a MEP
* @param    intIfNum      @b{(input)} Interface on which mep is to be
*                                     created
* @param    proLmCounters @b{(output)} Proactive LM counters.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731ProLmCountersGet(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex,
                                             uint32_t mepId, uint32_t pri, y1731CfmMepProlmTable_t *proLmCounters);

/*********************************************************************
* @purpose  Retrieve Proactive DM counters.
*
* @param    mdIndex       @b{(input)} Index used for MD indentification
*                                     used by SNMP.
* @param    maIndex       @b{(input)} Index for MA identification used by
*                                     SNMP.
* @param    mepId         @b{(input)} Index to identify a MEP
* @param    intIfNum      @b{(input)} Interface on which mep is to be
*                                     created
* @param    proDmCounters @b{(output)} Proactive DM counters.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731ProDmCountersGet(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex,
                                             uint32_t mepId, uint32_t pri, y1731CfmMepDmTable_t *proDmCounters);

/*********************************************************************
* @purpose  Retrieve OnDemand LM counters.
*
* @param    mdIndex       @b{(input)} Index used for MD indentification
*                                     used by SNMP.
* @param    maIndex       @b{(input)} Index for MA identification used by
*                                     SNMP.
* @param    mepId         @b{(input)} Index to identify a MEP
* @param    intIfNum      @b{(input)} Interface on which mep is to be
*                                     created
* @param    odLmCounters @b{(output)} OnDemand LM counters.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731OdLmCountersGet(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t pri, y1731CfmMepOndemlmTable_t *odLmCounters);

/*********************************************************************
* @purpose  Retrieve OnDemand DM counters.
*
* @param    mdIndex       @b{(input)} Index used for MD indentification
*                                     used by SNMP.
* @param    maIndex       @b{(input)} Index for MA identification used by
*                                     SNMP.
* @param    mepId         @b{(input)} Index to identify a MEP
* @param    intIfNum      @b{(input)} Interface on which mep is to be
*                                     created
* @param    odDmCounters @b{(output)} onDemand DM counters.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepY1731OdDmCountersGet(uint32_t intIfNum, uint32_t mdIndex, uint32_t maIndex,
                                            uint32_t mepId, uint32_t pri, y1731CfmMepDmTable_t *odDmCounters);

/*********************************************************************
* @purpose  Retrieve next Link Trace record.
*
* @param    mdIndex        @b{(input)}  Index used for MD indentification.
* @param    maIndex        @b{(input)}  Index for MA identification.
* @param    mepId          @b{(input)}  Index to identify a MEP
* @param    ltrSeqNumber    @b{(input)} Index used to identify the
*                                       remote MEP which sent the LTR
* @param    ltrReceiveOrder @b{(input)} An index to distinguish
*                                       among multiple LTRs with the same
*                                       LTR Transaction Identifier field
*                                       value.
* @param    linkTraceReply @b{(output)} Link Trace Reply.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMepLtResultNextGet(uint32_t mdIndex, uint32_t maIndex, uint32_t mepId, uint32_t ltrSeqNumber,
                                       uint32_t ltrReceiveOrder, dot1agCfmLtrObj_t *linkTraceReply);

OFDPA_ERROR_t dot1agBhhLabelMapEntryAdd(ofdpaMplsFlowEntry_t *mplsFlow);
OFDPA_ERROR_t dot1agBhhLabelMapEntryDelete(ofdpaMplsFlowEntry_t *mplsFlow);

/*********************************************************************
* @purpose  Updating Client MP DB with the MEP created.
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agAddMepToClientMPDB(uint32_t intIfNum, uint32_t mdIndex,
                                       uint32_t maIndex, uint32_t mepId);

/*********************************************************************
* @purpose  Updating Client MP DB with the MIP created.
*
* @param    intIfNum     @b{(input)} Interface on which mep is to be
*                                    created/deleted
* @param    primVID      @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel      @b{(input)} MD level of the MD for which MIP
*                                    is to be created/deleted.
* @param    direction    @b{(input)} Direction of MP (Up or Down)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_FAIL
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agAddMipToClientMPDB(uint32_t intIfNum, uint32_t primVID, int mdLevel,
                                       Dot1agCfmMpDirection_t direction);

/*********************************************************************
* @purpose  Updating Client MP DB with the MEP deleted.
*
* @param    intIfNum   @b{(input)} Interface on which mep is to be
*                                  created
* @param    mdIndex    @b{(input)} Index used for MD indentification
* @param    maIndex    @b{(input)} Index for MA identification
* @param    mepId      @b{(input)} Index to identify a MEP
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_EXISTS
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agDeleteMepFromClientMPDB(uint32_t intIfNum, uint32_t mdIndex,
                                       uint32_t maIndex, uint32_t mepId);

/*********************************************************************
* @purpose  Updating Client MP DB with the MIP deleted.
*
* @param    intIfNum     @b{(input)} Interface on which mep is to be
*                                    created/deleted
* @param    primVID      @b{(input)} VLAN ID associated with the MA.
* @param    mdLevel      @b{(input)} MD level of the MD for which MIP
*                                    is to be created/deleted.
* @param    direction    @b{(input)} Direction of MP (Up or Down)
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_EXISTS
* @returns  OFDPA_E_ERROR
* @returns  OFDPA_E_FAIL
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agDeleteMipFromClientMPDB(uint32_t intIfNum, uint32_t primVID, int mdLevel,
                                       Dot1agCfmMpDirection_t direction);
#endif /* INCLUDE_DOT1AG_API_H */
