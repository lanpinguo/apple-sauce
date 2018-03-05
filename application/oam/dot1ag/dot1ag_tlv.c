/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2015
*
**********************************************************************
*
* @filename   dot1ag_tlv.c
*
* @purpose    Contains DOT1AG TLV's
*
* @component  DOT1AG/CFM
*
* @comments   none
*
* @create     18 Feb 2009
*
* @end
*
**********************************************************************/

#include <stdio.h>
#include "ofdpa_datatypes.h"
#include "ofdb_api.h"
#include "dot1ag_outcalls.h"
#include "dot1ag.h"
#include "dot1ag_db.h"
#include "dot1ag_pdu.h"
#include "dot1ag_tlv.h"

uint8_t DOT1AG_MGMT_ADDR_DOMAIN_transportDomainUdpIpv4[] = {0x01, 0x03, 0x06, 0x01, 0x02, 0x01, 0x64, 0x01, 0x01};

/*********************************************************************
*
* @purpose  Gets the 802.1AG local port subtype for the specified interface
*
* @param    uint32_t            intIfNum @b((input))   internal interface number
* @param    uint8_t  pSubtype @b((output))  Port ID Subtype
*
* @returns  OFDPA_E_NONE, if value was retrieved successfully
* @returns  OFDPA_E_FAIL, if parameter check is failed
*
* @notes    none
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLocalPortIdSubtypeGet(uint32_t intIfNum,
                                             uint8_t *pSubtype)
{
  if (pSubtype == 0)
  {
    return OFDPA_E_FAIL;
  }

//  *pSubtype = LLDP_PORT_ID_SUBTYPE_MAC_ADDR;
  *pSubtype = 3;
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Gets the 802.1AG local port id for the specified interface
*
* @param    uint32_t   intIfNum  @b((input))   internal interface number
* @param    uint8_t   *portId   @b((output))  Port ID
* @param    uint8_t   *length   @b((output))  Port ID length
*
* @returns  OFDPA_E_NONE, if value was retrieved successfully
* @returns  OFDPA_E_FAIL, if interface out of range
*
* @notes    portId buffer must be at least LLDP_MGMT_STRING_SIZE_MAX
*           portId is encoded based on the ChassisIdSubtype
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLocalPortIdGet(uint32_t   intIfNum,
                                      uint8_t   *portId,
                                      uint8_t   *length)
{
#ifdef ODFPA_NOT_SUPPORTED
  uint32_t addrType = 0;

  if (portId == NULL)
  {
    return OFDPA_E_FAIL;
  }

  if (nimGetIntfAddrType(intIfNum, &addrType) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  if (nimGetIntfAddress(intIfNum, addrType, portId) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }
#endif
  *portId = intIfNum;
  *length = OFDPA_MAC_ADDR_LEN;
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Utility routine to fill the TLV in a given data buffer
*
* @param    tlvType    @b((input))   TLV identifier
* @param    tlvLen     @b((input))   TLV length
* @param    tlvDataPtr @b((input))   TLV source data
* @param    dataPtr    @b((output))  TLV destination data
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agCfmTlvWrite(uint8_t tlvType, uint16_t tlvLen,
                                uint8_t *tlvDataPtr, uint8_t *dataPtr)
{
  uint16_t uShortVal;

  DOT1AG_PUT_BYTE (tlvType, dataPtr);

  if (tlvType == DOT1AG_TLV_TYPE_END)
  {
    return OFDPA_E_NONE;
  }

  if (tlvDataPtr == NULL || dataPtr == NULL)
  {
    return OFDPA_E_FAIL;
  }

  uShortVal = tlvLen;
  DOT1AG_PUT_SHORT(uShortVal, dataPtr);

  if (tlvLen == 0)
  {
    return OFDPA_E_NONE;
  }

  memcpy (dataPtr, tlvDataPtr, tlvLen);

  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Extracts sender Id TLV parameters from the data buffer
*
* @param    pSenderIdTlv  @b{(output)} Sender Id TLV data holder
* @param    dataPtr       @b((input))  data buffer from which TLV
*                                      should be extracted
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments TLV length field should be updated by the caller
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmSenderIdTlvRead(dot1agCfmSenderIdTLV_t *pSenderIdTlv, uint8_t *dataPtr)
{
  uint16_t  chsIdLen;

  if (pSenderIdTlv->length == 0)
  {
    return OFDPA_E_NONE;
  }
  /* Get Chassis Id len, Chassis Id Subtype and Chassis Id */
  DOT1AG_GET_BYTE(pSenderIdTlv->chassisIdLen, dataPtr);
  if (pSenderIdTlv->chassisIdLen == 0)
  {
    return OFDPA_E_NONE;
  }
  DOT1AG_GET_BYTE(pSenderIdTlv->chassisIdSubtype, dataPtr);
  DOT1AG_GET_DATA(pSenderIdTlv->chassisId, pSenderIdTlv->chassisIdLen, dataPtr);

  /* Get management address domain length and address domain */
  chsIdLen = DOT1AG_TLV_FIELD_LEN_CHASSIS_ID_LENGTH +
    DOT1AG_TLV_FIELD_LEN_CHASSIS_ID_SUBTYPE + DOT1AG_TLV_FIELD_LEN_CHASSIS_ID_SUBTYPE;

  if (pSenderIdTlv->length > chsIdLen)
  {
    DOT1AG_GET_BYTE(pSenderIdTlv->addressDomainLen, dataPtr);
    if (pSenderIdTlv->addressDomainLen == 0)
    {
      return OFDPA_E_NONE;
    }
    DOT1AG_GET_DATA(pSenderIdTlv->addressDomain, pSenderIdTlv->addressDomainLen, dataPtr);

    /* Get management address domain length and address domain */
    DOT1AG_GET_BYTE(pSenderIdTlv->manAddressLen, dataPtr);
    if (pSenderIdTlv->manAddressLen == 0)
    {
      return OFDPA_E_NONE;
    }
    DOT1AG_GET_DATA(pSenderIdTlv->manAddress, pSenderIdTlv->manAddressLen, dataPtr);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Constructs Sender Id TLV
*
* @param    pMEPObj       @b{(input)}  MEP for which TLV should be constructed
* @param    pSenderIdTlv  @b{(output)} Sender Id TLV data holder
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmSenderIdTlvBuild(dot1agCfmStackObj_t    *pMEPObj,
                                        dot1agCfmSenderIdTLV_t *pSenderIdTlv)
{
//  lldpChassisIdSubtype_t chsIdSubType;
//  uint16_t            uShortVal;
  uint32_t      uIntVal = 0;
  OFDPA_ERROR_t rc      = OFDPA_E_NONE;
  uint32_t      manAddr;
  uint16_t      portId;

  /* Type                             : 1
   * Length                           : 2
   * Chassis Id Length                : 1
   * Chassis Id SubType               : 1
   * Chassis Id                       : As specified by Chassis Id Length
   * Management Address Domain Length : 1
   * Management Address Domain        : As specified by Management Address Domain Length
   * Management Address Length        : 1
   * Management Address               : As specified by Management Address Length
   */

  if (pMEPObj == NULL || pSenderIdTlv == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* This is controlled by 12.14.3.1.3:e, 12.14.5.1.3:d, 12.14.6.1.3:d */
#ifdef NOT_IMPLEMENTED
  if (pMEPObj->mepData->maData->mdData->dot1agCfmMdMhfIdPermission == DOT1AG_sendIdNone)
  {
    /* TLV should not be sent in this case. So, return FAILURE */
    return OFDPA_E_FAIL;
  }
#endif

  pSenderIdTlv->length = 0;
  pSenderIdTlv->chassisIdLen = 0;
  pSenderIdTlv->manAddressLen = 0;
#ifdef NOT_IMPLEMENTED
  if (pMEPObj->mepData->maData->mdData->dot1agCfmMdMhfIdPermission == DOT1AG_sendIdChassis ||
      pMEPObj->mepData->maData->mdData->dot1agCfmMdMhfIdPermission == DOT1AG_sendIdChassisManage)
#endif
  {
    /* Fill Chassis ID Length and Chassis ID */
    /* Specified by IEEE Std 802.1AB-2005 subclause 9.5.2.3 */

    /* TODO Get MAC Address*/

//    lldpLocChassisIdGet(pSenderIdTlv->chassisId, &uShortVal);
//    pSenderIdTlv->chassisIdLen = (uint8_t)uShortVal;
    pSenderIdTlv->chassisIdLen = OFDPA_MAC_ADDR_LEN;

    /* Fill Chassis ID Sub Type */
//    lldpLocChassisIdSubtypeGet(&chsIdSubType);
//    pSenderIdTlv->chassisIdSubtype = (Dot1agLldpChassisIdSubtype_t)(chsIdSubType);
    pSenderIdTlv->chassisIdSubtype = DOT1AG_PORT_ID_SUBTYPE_macAddress;

    pSenderIdTlv->length += DOT1AG_TLV_FIELD_LEN_CHASSIS_ID_LENGTH;
    pSenderIdTlv->length += DOT1AG_TLV_FIELD_LEN_CHASSIS_ID_SUBTYPE;
    pSenderIdTlv->length += pSenderIdTlv->chassisIdLen;
  }
#ifdef NOT_IMPLEMENTED
  if (pMEPObj->mepData->maData->mdData->dot1agCfmMdMhfIdPermission == DOT1AG_sendIdManage ||
      pMEPObj->mepData->maData->mdData->dot1agCfmMdMhfIdPermission == DOT1AG_sendIdChassisManage)
#endif
  {
    /* Fill Management Address Domain Length and Management Address Domain */
    memcpy(pSenderIdTlv->addressDomain, DOT1AG_MGMT_ADDR_DOMAIN_transportDomainUdpIpv4,
           sizeof(DOT1AG_MGMT_ADDR_DOMAIN_transportDomainUdpIpv4));
    pSenderIdTlv->addressDomainLen = sizeof(DOT1AG_MGMT_ADDR_DOMAIN_transportDomainUdpIpv4);

    pSenderIdTlv->length += DOT1AG_TLV_FIELD_LEN_MGMT_ADRS_DOMAIN_LEN;
    pSenderIdTlv->length += pSenderIdTlv->addressDomainLen;
#if OFDPA_NOT_SUPPORTED
    /* Fill Management Address Length and Management Address */
    uIntVal = simGetSystemIPAddr();
    if ((uIntVal == 0) && (cnfgrIsComponentPresent(L7_SERVICE_PORT_PRESENT_ID) == OFDPA_TRUE))
    {
      uIntVal = simGetServPortIPAddr();
    }
#endif
    memset(pSenderIdTlv->manAddress, 0x00, sizeof(pSenderIdTlv->manAddress));
    manAddr = (uint32_t)htonl((uint32_t)uIntVal);
    portId = htons(OFDPA_DOT1AG_L4PORT_SNMP);
    memcpy(pSenderIdTlv->manAddress, &manAddr, sizeof(manAddr));
    memcpy(&pSenderIdTlv->manAddress[OFDPA_IP_ADDR_LEN], &portId, DOT1AG_L4PORT_SNMP_LEN);
    pSenderIdTlv->manAddressLen = DOT1AG_MGMT_ADDR_LEN;

    pSenderIdTlv->length += DOT1AG_TLV_FIELD_LEN_MGMT_ADRS_LEN;
    pSenderIdTlv->length += pSenderIdTlv->manAddressLen;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Writes Sender Id TLV to a given buffer
*
* @param    pSenderIdTlv  @b{(input)}  Sender Id TLV data holder
* @param    dataPtr       @b{(output)} buffer to which TLV needs to be written
* @param    length        @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmSenderIdTlvWrite(dot1agCfmSenderIdTLV_t *pSenderIdTlv,
                                        uint8_t                *dataPtr,
                                        uint32_t               *length)

{
  uint8_t        buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t       *bufPtr;
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;

  memset(buf, 0, sizeof(buf));
  bufPtr = &buf[0];

  if (pSenderIdTlv->chassisIdLen != 0)
  {
    /* Fill Chassis ID Length */
    DOT1AG_PUT_BYTE(pSenderIdTlv->chassisIdLen, bufPtr);
    /* Fill Chassis ID Sub Type */
    DOT1AG_PUT_BYTE(pSenderIdTlv->chassisIdSubtype, bufPtr);
    /* Fill Chassis ID */
    DOT1AG_PUT_DATA(pSenderIdTlv->chassisId, pSenderIdTlv->chassisIdLen, bufPtr);
  }

  if (pSenderIdTlv->addressDomainLen != 0)
  {
    /* Fill Management Address Domain Length and Management Address Domain */
    DOT1AG_PUT_BYTE(pSenderIdTlv->addressDomainLen, bufPtr);
    DOT1AG_PUT_DATA(pSenderIdTlv->addressDomain, pSenderIdTlv->addressDomainLen, bufPtr);
  }

  if (pSenderIdTlv->manAddressLen != 0)
  {
    /* Fill Management Address Length and Management Address */
    DOT1AG_PUT_BYTE(pSenderIdTlv->manAddressLen, bufPtr);
    DOT1AG_PUT_DATA(pSenderIdTlv->manAddress, pSenderIdTlv->manAddressLen, bufPtr);
  }

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_SENDER_ID, pSenderIdTlv->length, buf, dataPtr);

  if (rc == OFDPA_E_NONE)
  {
    *length = pSenderIdTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Writes Test pattern TLV to a given buffer
*
* @param    pTestTlv      @b{(input)}  Test TLV data holder
* @param    dataPtr       @b{(output)} buffer to which TLV needs to be written
* @param    length        @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmTestTlvWrite(dot1agCfmTestTLV_t *pTestTlv,
                                    uint8_t            *dataPtr,
                                    uint32_t           *length)
{
  uint8_t        buf[DOT1AG_TEST_TLV_VALUE_LEN_MAX];
  uint8_t       *bufPtr, *startDataPtr;
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;
  uint32_t       crc32 = 0;

  memset(buf, 0, sizeof(buf));
  bufPtr = &buf[0];

  if (pTestTlv->length != 0)
  {
    DOT1AG_PUT_BYTE(pTestTlv->patternType, bufPtr);

    switch(pTestTlv->patternType)
    {
    case OFDPA_OAM_LB_TEST_TLV_NULL:
    case OFDPA_OAM_LB_TEST_TLV_NULL_CRC32:
      break;
    case OFDPA_OAM_LB_TEST_TLV_PRBS:
    case OFDPA_OAM_LB_TEST_TLV_PRBS_CRC32:
      break;
    default:
      break;
    }
  }

  startDataPtr = dataPtr;
  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_TEST, pTestTlv->length, buf, dataPtr);

  if (rc == OFDPA_E_NONE)
  {
    if ((pTestTlv->patternType == OFDPA_OAM_LB_TEST_TLV_NULL_CRC32) ||
        (pTestTlv->patternType == OFDPA_OAM_LB_TEST_TLV_PRBS_CRC32))
    {
      rc = crcCalculate(CRC32_INIT_VALUE, startDataPtr, pTestTlv->length - 1, CRC_32, &crc32);
      if (rc == OFDPA_E_NONE)
      {
        crc32 = htonl(crc32);
        memcpy((startDataPtr + pTestTlv->length - 1), &crc32, sizeof(uint32_t));
      }
      else
      {
        return 0;
      }
    }
    *length = pTestTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Extracts Port Status TLV parameters from the data buffer
*
* @param    pPortStatusTlv  @b{(output)} Port Status TLV data holder
* @param    dataPtr         @b((input))  data buffer from which TLV
*                                        should be extracted
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments TLV length field should be updated by the caller
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmPortStatusTlvRead(dot1agCfmPortStatusTLV_t *pPortStatusTlv, uint8_t *dataPtr)
{
  if (pPortStatusTlv->length != 0 && pPortStatusTlv->length != DOT1AG_TLV_FIELD_LEN_PORT_STATUS)
  {
    return OFDPA_E_FAIL;
  }

  if (pPortStatusTlv->length != 0)
  {
    /* Get Chassis Id len, Chassis Id Subtype and Chassis Id */
    DOT1AG_GET_BYTE(pPortStatusTlv->portStatus, dataPtr);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Constructs Port Status TLV
*
* @param    pMEPObj        @b{(input)}  MEP for which TLV should be constructed
* @param    pPortStatusTlv @b{(output)} Port Status TLV data holder
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmPortStatusTlvBuild(dot1agCfmStackObj_t      *pMEPObj,
                                          dot1agCfmPortStatusTLV_t *pPortStatusTlv)
{
  /* Type                   : 1
   * Length                 : 2
   * Value                  : 1
   */

  if (pMEPObj == NULL || pPortStatusTlv == NULL)
  {
    return OFDPA_E_FAIL;
  }
  /* Fill Port Status value */
  if (dot1agCfmIntfVlanIsForwarding(pMEPObj->key.dot1agCfmStackifIndex,
                                    pMEPObj->key.dot1agCfmStackVlanIdOrNone)
      == OFDPA_TRUE)
  {
    pPortStatusTlv->portStatus = DOT1AG_RMEP_PORT_STATE_psUp;
  }
  else
  {
    pPortStatusTlv->portStatus = DOT1AG_RMEP_PORT_STATE_psBlocked;
  }
  pPortStatusTlv->length = DOT1AG_TLV_FIELD_LEN_PORT_STATUS;
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Writes Port Status TLV to a given buffer
*
* @param    pPortStatusTlv @b{(input)}  Port Status TLV data holder
* @param    dataPtr        @b{(output)} buffer to which TLV needs to be written
* @param    length         @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmPortStatusTlvWrite(dot1agCfmPortStatusTLV_t *pPortStatusTlv,
                                          uint8_t                  *dataPtr,
                                          uint32_t                 *length)
{
  uint8_t              buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t              *bufPtr;
  OFDPA_ERROR_t                rc = OFDPA_E_FAIL;

  /* Type                   : 1
   * Length                 : 2
   * Value                  : 1
   */

  memset(buf, 0x00, sizeof(buf));
  bufPtr = buf;
  if (pPortStatusTlv->length != 0)
  {
    DOT1AG_PUT_BYTE(pPortStatusTlv->portStatus, bufPtr);
  }

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_PORT_STATUS,
                         pPortStatusTlv->length, buf, dataPtr);
  if (rc == OFDPA_E_NONE)
  {
    *length = pPortStatusTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Extracts Interface Status TLV parameters from the data buffer
*
* @param    pIntfStatusTlv  @b{(output)} Interface Status TLV data holder
* @param    dataPtr         @b((input))  data buffer from which TLV
*                                        should be extracted
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments TLV length field should be updated by the caller
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmInterfaceStatusTlvRead(dot1agCfmInterfaceStatusTLV_t *pIntfStatusTlv,
                                              uint8_t                     *dataPtr)
{
  if (pIntfStatusTlv->length != 0
      && pIntfStatusTlv->length != DOT1AG_TLV_FIELD_LEN_INTERFACE_STATUS)
  {
    return OFDPA_E_FAIL;
  }

  if (pIntfStatusTlv->length != 0)
  {
    /* Get Chassis Id len, Chassis Id Subtype and Chassis Id */
    DOT1AG_GET_BYTE(pIntfStatusTlv->interfaceStatus, dataPtr);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Constructs Interface Status TLV
*
* @param    intIfNum        @b{(input)}  Internal Interface number
* @param    pIntfStatusTlv  @b{(output)} Interface Status TLV data holder
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmInterfaceStatusTlvBuild(uint32_t                     intIfNum,
                                               dot1agCfmInterfaceStatusTLV_t *pIntfStatusTlv)
{
  uint32_t              ifStatus;

  /* Type                   : 1
   * Length                 : 2
   * Value                  : 1
   */

  if (pIntfStatusTlv == NULL)
  {
    return OFDPA_E_FAIL;
  }
  /* There is no seperate API to get the ifOperStatus. Instead, link status is used. */
#if 0
  if (nimGetIntfLinkState(intIfNum, &ifStatus) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }
#endif

  //OFDB_WRITE_LOCK_TAKE;
  if (ofdbPortStateGet(intIfNum, &ifStatus) != OFDPA_E_NONE)
  {
    //OFDB_WRITE_LOCK_GIVE;
    return OFDPA_E_FAIL;
  }
  //OFDB_WRITE_LOCK_GIVE;

//  if (ifStatus == L7_UP)
  if (ifStatus != OFDPA_PORT_STATE_LINK_DOWN)
  {
    pIntfStatusTlv->interfaceStatus = DOT1AG_RMEP_INTERFACE_STATUS_isUp;
  }
  else
  {
    pIntfStatusTlv->interfaceStatus = DOT1AG_RMEP_INTERFACE_STATUS_isDown;
  }
  pIntfStatusTlv->length = DOT1AG_TLV_FIELD_LEN_INTERFACE_STATUS;
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Writes Interface Status TLV to a given buffer
*
* @param    pIntfStatusTlv @b{(input)}  Interface Status TLV data holder
* @param    dataPtr        @b{(output)} buffer to which TLV needs to be written
* @param    length         @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmInterfaceStatusTlvWrite(dot1agCfmInterfaceStatusTLV_t *pIntfStatusTlv,
                                               uint8_t                       *dataPtr,
                                               uint32_t                      *length)
{
  uint8_t              buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t              *bufPtr;
  OFDPA_ERROR_t                rc = OFDPA_E_FAIL;

  memset(buf, 0x00, sizeof(buf));
  bufPtr = buf;
  if (pIntfStatusTlv->length != 0)
  {
    DOT1AG_PUT_BYTE(pIntfStatusTlv->interfaceStatus, bufPtr);
  }

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_INTERFACE_STATUS, pIntfStatusTlv->length, buf, dataPtr);

  if (rc == OFDPA_E_NONE)
  {
    *length = pIntfStatusTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Writes Data TLV to a given buffer
*
* @param    data     @b{(input)}     TLV data
* @param    len      @b{(input)}     TLV length
* @param    dataPtr  @b{(output)} buffer to which TLV needs to be written
* @param    length   @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments none
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmDataTlvWrite(uint8_t *data, uint16_t len, uint8_t *dataPtr, uint32_t *length)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_DATA, len, data, dataPtr);
  if (rc == OFDPA_E_NONE)
  {
    *length = len + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Writes Data pattern (all zeros) TLV to a given buffer for LBM messages
*
* @param    pDataTlv      @b{(input)}  Data TLV data holder
* @param    dataPtr       @b{(output)} buffer to which TLV needs to be written
* @param    length         @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLbmDataTlvWrite(dot1agCfmDataTLV_t *pDataTlv,
                                       uint8_t            *dataPtr,
                                       uint32_t           *length)
{
  uint8_t        buf[DOT1AG_DATA_TLV_VALUE_LEN_MAX];

  memset(buf, 0, sizeof(buf));
  return (dot1agCfmDataTlvWrite(buf, pDataTlv->length, dataPtr, length));
}

/*********************************************************************
*
* @purpose  Writes END TLV to a given buffer
*
* @param    dataPtr  @b{(output)} buffer to which TLV needs to be written
* @param    length   @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments none
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmEndTlvWrite(uint8_t *dataPtr, uint32_t *length)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_END, 0, NULL, dataPtr);

  if (rc == OFDPA_E_NONE)
  {
    *length = 1;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Extracts LTM Egress Identifier TLV parameters from the data buffer
*
* @param    pLtmEgressIdTlv @b{(output)} LTM Egress Identifier TLV data holder
* @param    dataPtr         @b((input))  data buffer from which TLV
*                                        should be extracted
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments TLV length field should be updated by the caller
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTMEgressIdTlvRead(dot1agCfmLTMEgressIdTLV_t *pLtmEgressIdTlv, uint8_t *dataPtr)
{
  if (pLtmEgressIdTlv->length != 0 && pLtmEgressIdTlv->length != DOT1AG_TLV_LEN_LTM_EGR_ID)
  {
    return OFDPA_E_FAIL;
  }

  if (pLtmEgressIdTlv->length != 0)
  {
    /* Get Chassis Id len, Chassis Id Subtype and Chassis Id */
    DOT1AG_GET_DATA(pLtmEgressIdTlv->egressId, pLtmEgressIdTlv->length, dataPtr);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Constructs LTM Egress Identifier TLV
*
* @param    pMEPObj         @b{(input)}  MEP Object
* @param    pLtmEgressIdTlv @b{(output)} LTM Egress Identifier TLV data holder
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTMEgressIdTlvBuild(dot1agCfmStackObj_t *pMEPObj, dot1agCfmLTMEgressIdTLV_t *pLtmEgressIdTlv)
{
  /* Fill Egress Identifier. Higher two octets of Egress Identifier shall be
     zero and the remaining six bytes should contains the system MAC Address */
  if (pLtmEgressIdTlv == NULL)
  {
    return OFDPA_E_FAIL;
  }
  memset(pLtmEgressIdTlv->egressId, 0x00, sizeof(pLtmEgressIdTlv->egressId));
  memcpy(&pLtmEgressIdTlv->egressId[2], pMEPObj->mpMacAddress, sizeof(pMEPObj->mpMacAddress));
  pLtmEgressIdTlv->length = DOT1AG_TLV_FIELD_LEN_LTM_EGRESS_ID;
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Writes LTM Egress Identifier TLV to a given buffer
*
* @param    pLtmEgressIdTlv @b{(input)}  LTM Egress Identifier TLV data holder
* @param    dataPtr         @b{(output)} buffer to which TLV needs to be written
* @param    length         @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTMEgressIdentifierTlvWrite(dot1agCfmLTMEgressIdTLV_t *pLtmEgressIdTlv,
                                                   uint8_t                   *dataPtr,
                                                   uint32_t                  *length)
{
  uint8_t        buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t       *bufPtr;
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;

  /* Type                   : 1
   * Length                 : 2
   * Egress Identifier      : 8
   */

  memset(buf, 0x00, sizeof(buf));
  bufPtr = buf;

  if (pLtmEgressIdTlv->length != 0)
  {
    DOT1AG_PUT_DATA(pLtmEgressIdTlv->egressId, pLtmEgressIdTlv->length, bufPtr);
  }

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_LTM_EGR_ID, pLtmEgressIdTlv->length, buf, dataPtr);
  if (rc == OFDPA_E_NONE)
  {
    *length = pLtmEgressIdTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Extracts LTR Egress Identifier TLV parameters from the data buffer
*
* @param    pLtrEgressIdTlv @b{(output)} LTR Egress Identifier TLV data holder
* @param    dataPtr         @b((input))  data buffer from which TLV
*                                        should be extracted
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments TLV length field should be updated by the caller
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTREgressIdTlvRead(dot1agCfmLTREgressIdTLV_t *pLTREgressIdTlv,
                                          uint8_t                 *dataPtr)
{
  if (pLTREgressIdTlv->length != 0 && pLTREgressIdTlv->length != (DOT1AG_TLV_LEN_LTR_EGR_ID))
  {
    return OFDPA_E_FAIL;
  }

  if (pLTREgressIdTlv->length != 0)
  {
    DOT1AG_GET_DATA(pLTREgressIdTlv->lastEgressId, DOT1AG_TLV_FIELD_LEN_LTR_EGRESS_ID, dataPtr);
    DOT1AG_GET_DATA(pLTREgressIdTlv->nextEgressId, DOT1AG_TLV_FIELD_LEN_LTR_EGRESS_ID, dataPtr);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Constructs LTR Egress Identifier TLV
*
* @param    pMEPObj         @b{(input)}  MEP Object
* @param    pLtrEgressIdTlv @b{(output)} LTR Egress Identifier TLV data holder
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTREgressIdTlvBuild(dot1agCfmStackObj_t *pMEPObj,
                                           dot1agCfmLTREgressIdTLV_t *pLTREgressIdTlv)
{
  /* Type                   : 1
   * Length                 : 2
   * Last Egress Identifier : 8
   * Next Egress Identifier : 8
   */

  if (pLTREgressIdTlv == NULL)
  {
    return OFDPA_E_FAIL;
  }
  /* Fill Last Egress Identifier.
     This field takes the same value as the LTM Egress Identifier TLV of that LTM,
     or the value 0, if no LTM Egress Identifier TLV was present in that LTM */
  memset(pLTREgressIdTlv->lastEgressId, 0x00, sizeof(pLTREgressIdTlv->lastEgressId));
  memcpy(&pLTREgressIdTlv->lastEgressId[2], pMEPObj->mpMacAddress, sizeof(pMEPObj->mpMacAddress));
  pLTREgressIdTlv->length = DOT1AG_TLV_FIELD_LEN_LTR_EGRESS_ID;

  memset(pLTREgressIdTlv->nextEgressId, 0x00, sizeof(pLTREgressIdTlv->nextEgressId));
  memcpy(&pLTREgressIdTlv->nextEgressId[2], pMEPObj->mpMacAddress, sizeof(pMEPObj->mpMacAddress));
  pLTREgressIdTlv->length += DOT1AG_TLV_FIELD_LEN_LTR_EGRESS_ID;
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Writes LTR Egress Identifier TLV to a given buffer
*
* @param    pLTREgressIdTlv @b{(input)}  LTR Egress Identifier TLV data holder
* @param    dataPtr         @b{(output)} buffer to which TLV needs to be written
* @param    length          @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmLTREgressIdTlvWrite(dot1agCfmLTREgressIdTLV_t *pLTREgressIdTlv,
                                           uint8_t                   *dataPtr,
                                           uint32_t                  *length)
{
  uint8_t        buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t       *bufPtr;
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;

  memset(buf, 0x00, sizeof(buf));
  bufPtr = buf;

  if (pLTREgressIdTlv->length != 0)
  {
    DOT1AG_PUT_DATA(pLTREgressIdTlv->lastEgressId, DOT1AG_TLV_FIELD_LEN_LTR_EGRESS_ID, bufPtr);
    DOT1AG_PUT_DATA(pLTREgressIdTlv->nextEgressId, DOT1AG_TLV_FIELD_LEN_LTR_EGRESS_ID, bufPtr);
  }
  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_LTR_EGR_ID, pLTREgressIdTlv->length, buf, dataPtr);
  if (rc == OFDPA_E_NONE)
  {
    *length = pLTREgressIdTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Extracts Reply Ingress TLV parameters from the data buffer
*
* @param    pReplyIngressTlv @b{(output)} Reply Ingress TLV data holder
* @param    dataPtr          @b((input))  data buffer from which TLV
*                                         should be extracted
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments TLV length field should be updated by the caller
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmReplyIngressTlvRead(dot1agCfmReplyIngressTLV_t *pReplyIngressTlv,
                                           uint8_t                  *dataPtr)
{
  if (pReplyIngressTlv == NULL || dataPtr == NULL)
  {
    return OFDPA_E_FAIL;
  }
  if (pReplyIngressTlv->length == 0)
  {
    return OFDPA_E_NONE;
  }

  DOT1AG_GET_BYTE(pReplyIngressTlv->ingressAction, dataPtr);
  DOT1AG_GET_DATA(pReplyIngressTlv->ingressMacAdrs, OFDPA_MAC_ADDR_LEN, dataPtr);

  if (pReplyIngressTlv->length >= DOT1AG_TLV_LEN_MIN_PORT_ID_PARAM_PRESENT)
  {
    DOT1AG_GET_BYTE(pReplyIngressTlv->ingressPortIdLength, dataPtr);
    DOT1AG_GET_BYTE(pReplyIngressTlv->ingressPortIdSubType, dataPtr);
    DOT1AG_GET_DATA(pReplyIngressTlv->ingressPortId, pReplyIngressTlv->ingressPortIdLength, dataPtr);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Constructs Reply Ingress TLV
*
* @param    intIfNum         @b{(input)}  Internal Interface number
* @param    vlanId           @b{(input)}  PrimVID of the MA
* @param    macAddr          @b{(input)}  MAC address of MP at ingress
* @param    pReplyIngressTlv @b{(output)} Reply Ingress TLV data holder
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmReplyIngressTlvBuild(uint32_t intIfNum,
                                            uint16_t vlanId,
                                            uint8_t *macAddr,
                                            dot1agCfmReplyIngressTLV_t *pReplyIngressTlv)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint32_t      linkState;

  /* Type                   : 1
   * Length                 : 2
   * Ingress Action         : 1
   * Ingress MAC Address    : 6
   */

  if (pReplyIngressTlv == NULL)
  {
    return OFDPA_E_FAIL;
  }
  /* Fill Ingress Action */
  pReplyIngressTlv->length = DOT1AG_TLV_FIELD_LEN_REPLY_INGRESS_ACTION;

  //OFDB_WRITE_LOCK_TAKE;
  if (dot1agCfmIntfVlanIsForwarding(intIfNum, vlanId) == OFDPA_TRUE)
  {
    pReplyIngressTlv->ingressAction = DOT1AG_INGRESSS_ACTION_ingOk;
  }
  else if (ofdbPortStateGet(intIfNum, &linkState) == OFDPA_E_NONE &&
           linkState != OFDPA_PORT_STATE_LINK_DOWN)
  {
    pReplyIngressTlv->ingressAction = DOT1AG_INGRESSS_ACTION_ingDown;
  }
  else
  {
    pReplyIngressTlv->ingressAction = DOT1AG_INGRESSS_ACTION_ingBlocked;
  }
  //OFDB_WRITE_LOCK_GIVE;

  pReplyIngressTlv->ingressPortIdLength = 0;

  /* Fill Ingress MAC Address. This can be used as the destination MAC
     Address by other MP's in LBM's */
  memcpy(pReplyIngressTlv->ingressMacAdrs, macAddr, sizeof(pReplyIngressTlv->ingressMacAdrs));
  pReplyIngressTlv->length += OFDPA_MAC_ADDR_LEN;

  return rc;
}

/*********************************************************************
*
* @purpose  Writes Reply Ingress TLV to a given buffer
*
* @param    pReplyIngressTlv @b{(input)}  Reply Ingress TLV data holder
* @param    dataPtr          @b{(output)} buffer to which TLV needs to be written
* @param    length           @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmReplyIngressTlvWrite(dot1agCfmReplyIngressTLV_t *pReplyIngressTlv, uint8_t *dataPtr, uint32_t *length)
{
  uint8_t        buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t       *bufPtr;
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;

  memset(buf, 0x00, sizeof(buf));
  bufPtr = buf;

  if (pReplyIngressTlv->length != 0)
  {
    DOT1AG_PUT_BYTE(pReplyIngressTlv->ingressAction, bufPtr);
    DOT1AG_PUT_DATA(pReplyIngressTlv->ingressMacAdrs, OFDPA_MAC_ADDR_LEN, bufPtr);
  }

  /* Fill Ingress Port ID Length, Port ID SubType, Port ID */
  if (pReplyIngressTlv->length >= DOT1AG_TLV_LEN_MIN_PORT_ID_PARAM_PRESENT)
  {
    DOT1AG_PUT_BYTE(pReplyIngressTlv->ingressPortIdLength, bufPtr);
    DOT1AG_PUT_BYTE(pReplyIngressTlv->ingressPortIdSubType, bufPtr);
    DOT1AG_PUT_DATA(pReplyIngressTlv->ingressPortId, pReplyIngressTlv->ingressPortIdLength, bufPtr);
  }

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_REPLY_INGRESS, pReplyIngressTlv->length, buf, dataPtr);
  if (rc == OFDPA_E_NONE)
  {
    *length = pReplyIngressTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Extracts Reply Egress TLV parameters from the data buffer
*
* @param    pReplyEgressTlv @b{(output)} Reply Egress TLV data holder
* @param    dataPtr         @b((input))  data buffer from which TLV
*                                        should be extracted
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments TLV length field should be updated by the caller
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmReplyEgressTlvRead(dot1agCfmReplyEgressTLV_t *pReplyEgressTlv,
                                          uint8_t                 *dataPtr)
{
  if (pReplyEgressTlv == NULL || dataPtr == NULL)
  {
    return OFDPA_E_FAIL;
  }
  if (pReplyEgressTlv->length == 0)
  {
    return OFDPA_E_NONE;
  }
  DOT1AG_GET_BYTE(pReplyEgressTlv->egressAction, dataPtr);
  DOT1AG_GET_DATA(pReplyEgressTlv->egressMacAdrs, OFDPA_MAC_ADDR_LEN, dataPtr);

  if (pReplyEgressTlv->length >= DOT1AG_TLV_LEN_MIN_PORT_ID_PARAM_PRESENT)
  {
    DOT1AG_GET_BYTE(pReplyEgressTlv->egressPortIdLength, dataPtr);
    DOT1AG_GET_BYTE(pReplyEgressTlv->egressPortIdSubType, dataPtr);
    DOT1AG_GET_DATA(pReplyEgressTlv->egressPortId, pReplyEgressTlv->egressPortIdLength, dataPtr);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
*
* @purpose  Constructs Reply Egress TLV
*
* @param    intIfNum         @b{(input)}  Internal Interface number
* @param    vlanId           @b{(input)}  PrimVID of the MA
* @param    macAddr          @b{(input)}  MAC address of MP at ingress
* @param    pReplyEgressTlv  @b{(output)} Reply Egress TLV data holder
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmReplyEgressTlvBuild(uint32_t intIfNum,
                                           uint16_t vlanId,
                                           uint8_t *macAddr,
                                           dot1agCfmReplyEgressTLV_t *pReplyEgressTlv)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint32_t      linkState;

  /* Type                   : 1
   * Length                 : 2
   * Egress Action          : 1
   * Egress MAC Address     : 6
   */

  if (pReplyEgressTlv == NULL)
  {
    return OFDPA_E_FAIL;
  }
  /* Fill Egress Action */

  //OFDB_WRITE_LOCK_TAKE;

  if (dot1agCfmIntfVlanIsForwarding(intIfNum, vlanId) == OFDPA_TRUE)
  {
    pReplyEgressTlv->egressAction = DOT1AG_EGRESSS_ACTION_egrOk;
  }
  else if (ofdbPortStateGet(intIfNum, &linkState) == OFDPA_E_NONE &&
           linkState != OFDPA_PORT_STATE_LINK_DOWN)
//  else if (nimGetIntfLinkState(intIfNum, &linkState) == OFDPA_E_NONE &&
//           linkState == L7_UP)
  {
    pReplyEgressTlv->egressAction = DOT1AG_EGRESSS_ACTION_egrBlocked;
  }
  else
  {
    pReplyEgressTlv->egressAction = DOT1AG_EGRESSS_ACTION_egrDown;
  }
  //OFDB_WRITE_LOCK_GIVE;

  pReplyEgressTlv->length = DOT1AG_TLV_FIELD_LEN_REPLY_EGRESS_ACTION;

  /* Fill Egress MAC Address. This can be used as the destination MAC
     Address by other MP's in LBM's */
  memcpy(pReplyEgressTlv->egressMacAdrs, macAddr, sizeof(pReplyEgressTlv->egressMacAdrs));
  pReplyEgressTlv->length += OFDPA_MAC_ADDR_LEN;

  return rc;
}

/*********************************************************************
*
* @purpose  Writes Reply Egress TLV to a given buffer
*
* @param    pReplyEgressTlv  @b{(input)}  Reply Egress TLV data holder
* @param    dataPtr          @b{(output)} buffer to which TLV needs to be written
* @param    length           @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmReplyEgressTlvWrite(dot1agCfmReplyEgressTLV_t *pReplyEgressTlv,
                                           uint8_t                   *dataPtr,
                                           uint32_t                  *length)
{
  uint8_t        buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t       *bufPtr;
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;

  memset(buf, 0x00, sizeof(buf));
  bufPtr = buf;

  if (pReplyEgressTlv->length != 0)
  {
    DOT1AG_PUT_BYTE(pReplyEgressTlv->egressAction, bufPtr);
    DOT1AG_PUT_DATA(pReplyEgressTlv->egressMacAdrs, OFDPA_MAC_ADDR_LEN, bufPtr);
  }
  /* Fill Egress Port ID Length, Port ID SubType, Port ID */
  if (pReplyEgressTlv->length >= DOT1AG_TLV_LEN_MIN_PORT_ID_PARAM_PRESENT)
  {
    DOT1AG_PUT_BYTE(pReplyEgressTlv->egressPortIdLength, bufPtr);
    DOT1AG_PUT_BYTE(pReplyEgressTlv->egressPortIdSubType, bufPtr);
    DOT1AG_PUT_DATA(pReplyEgressTlv->egressPortId, pReplyEgressTlv->egressPortIdLength, bufPtr);
  }
  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_REPLY_EGRESS, pReplyEgressTlv->length, buf, dataPtr);
  if (rc == OFDPA_E_NONE)
  {
    *length = pReplyEgressTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Writes Request Mep ID TLV to a given buffer
*
* @param    pRequestMepTlv @b{(input)}  TLV data holder
* @param    dataPtr        @b{(output)} buffer to which TLV needs to be written
* @param    length         @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmRequestMepTlvWrite(dot1agCfmRequestMepTLV_t *pRequestMepTlv,
                                          uint8_t                  *dataPtr,
                                          uint32_t                 *length)
{
  uint8_t        buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t       *bufPtr;
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;

  /* Type                   : 1
   * Length                 : 2
   * Value                  :
   */

  memset(buf, 0x00, sizeof(buf));
  bufPtr = buf;
  if (pRequestMepTlv->length != 0)
  {
    DOT1AG_PUT_BYTE(pRequestMepTlv->subType, bufPtr);
    DOT1AG_PUT_BYTE(pRequestMepTlv->LbInd, bufPtr);
    DOT1AG_PUT_SHORT(pRequestMepTlv->MepId, bufPtr);
    DOT1AG_PUT_DATA(pRequestMepTlv->MegId, DOT1AG_TLV_FIELD_MEG_ID, bufPtr);
  }

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_REQUEST_MEP,
                         pRequestMepTlv->length, buf, dataPtr);
  if (rc == OFDPA_E_NONE)
  {
    *length = pRequestMepTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Writes Target Mep ID TLV to a given buffer
*
* @param    pTargetMepTlv  @b{(input)}  TLV data holder
* @param    dataPtr        @b{(output)} buffer to which TLV needs to be written
* @param    length         @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmTargetMepTlvWrite(dot1agCfmTargetMepTLV_t *pTargetMepTlv,
                                         uint8_t                 *dataPtr,
                                         uint32_t                *length)
{
  uint8_t        buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t       *bufPtr;
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;

  /* Type                   : 1
   * Length                 : 2
   * Value                  :
   */

  memset(buf, 0x00, sizeof(buf));
  bufPtr = buf;
  if (pTargetMepTlv->length != 0)
  {
    DOT1AG_PUT_BYTE(pTargetMepTlv->subType, bufPtr);
    DOT1AG_PUT_SHORT(pTargetMepTlv->MepId, bufPtr);
  }

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_TARGET_MEP,
                         pTargetMepTlv->length, buf, dataPtr);
  if (rc == OFDPA_E_NONE)
  {
    *length = pTargetMepTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Writes Reply Mep ID TLV to a given buffer
*
* @param    pTargetMepTlv  @b{(input)}  TLV data holder
* @param    dataPtr        @b{(output)} buffer to which TLV needs to be written
* @param    length         @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t dot1agCfmReplyMepTlvWrite(dot1agCfmTargetMepTLV_t *pTargetMepTlv,
                                        uint8_t                 *dataPtr,
                                        uint32_t                *length)
{
  uint8_t        buf[DOT1AG_TLV_VALUE_LEN_MAX];
  uint8_t       *bufPtr;
  OFDPA_ERROR_t  rc = OFDPA_E_FAIL;

  /* Type                   : 1
   * Length                 : 2
   * Value                  :
   */

  memset(buf, 0x00, sizeof(buf));
  bufPtr = buf;
  if (pTargetMepTlv->length != 0)
  {
    DOT1AG_PUT_BYTE(pTargetMepTlv->subType, bufPtr);
    DOT1AG_PUT_SHORT(pTargetMepTlv->MepId, bufPtr);
  }

  rc = dot1agCfmTlvWrite(DOT1AG_TLV_TYPE_REPLY_MEP,
                         pTargetMepTlv->length, buf, dataPtr);
  if (rc == OFDPA_E_NONE)
  {
    *length = pTargetMepTlv->length + 3;
  }
  else
  {
    *length = 0;
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Debug routine to print the TLV
*
* @param    tlvType    @b((input))   TLV identifier
* @param    tlvLen     @b((input))   TLV length
* @param    tlvDataPtr @b((input))   TLV source data
*
* @returns  none
*
* @notes
*
* @end
*********************************************************************/
void dot1agCfmDebugTlvDump(uint8_t tlvType, uint16_t tlvLen, uint8_t *tlvDataPtr)
{
  uint16_t i;

  printf("\r\n=========================================");
  printf("\r\nType   : %02x   Length: %02x", tlvType, tlvLen);
  {
    printf("\r\nValue  : ");
    for (i = 0; i < tlvLen; i++)
    {
      if (((i)%20) == 0)
      {
        printf("\r\n %04x   : ", i);
      }
      printf("%02x ", tlvDataPtr[i]);
    }
  }
  printf("\r\n=========================================\n");
}
