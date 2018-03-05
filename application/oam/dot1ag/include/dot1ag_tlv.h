/*********************************************************************
 *
 * (C) Copyright Broadcom Corporation 2000-2015
 *
 **********************************************************************
 * @filename  dot1ag_tlv.h
 *
 * @purpose   Header file for dot1ag TLV functions
 *
 * @component DOT1AG/CFM
 *
 * @comments  none
 *
 * @create    02/16/2009
 *
 * @end
 **********************************************************************/
#ifndef INCLUDE_DOT1AG_TLV_H
#define INCLUDE_DOT1AG_TLV_H

#include "dot1ag.h"

#define CRC32_INIT_VALUE 0xffffffff

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
                                             uint8_t *pSubtype);

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
                                      uint8_t    *length);

/*********************************************************************
*
* @purpose  Utility routine to fill the TLV, based on the input routines
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
                                uint8_t *tlvDataPtr, uint8_t *dataPtr);

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
OFDPA_ERROR_t dot1agCfmSenderIdTlvRead(dot1agCfmSenderIdTLV_t *pSenderIdTlv, uint8_t *dataPtr);

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
                                        dot1agCfmSenderIdTLV_t *pSenderIdTlv);

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
                                        uint32_t               *length);

/*********************************************************************
*
* @purpose  Writes Data pattern (all zeros) TLV to a given buffer for LBM messages
*
* @param    pDataTlv      @b{(input)}  Data TLV data holder
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
OFDPA_ERROR_t dot1agCfmLbmDataTlvWrite(dot1agCfmDataTLV_t *pDataTlv,
                                       uint8_t            *dataPtr,
                                       uint32_t           *length);

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
                                    uint32_t           *length);

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
OFDPA_ERROR_t dot1agCfmPortStatusTlvRead(dot1agCfmPortStatusTLV_t *pPortStatusTlv, uint8_t *dataPtr);

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
                                          dot1agCfmPortStatusTLV_t *pPortStatusTlv);

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
                                          uint32_t                 *length);

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
                                              uint8_t                     *dataPtr);

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
                                               dot1agCfmInterfaceStatusTLV_t *pIntfStatusTlv);

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
                                               uint32_t                      *length);

/*********************************************************************
*
* @purpose  Writes Data TLV to a given buffer
*
* @param    data     @b{(input)}     TLV data
* @param    len      @b{(input)}     TLV length
* @param    dataPtr  @b{(output)} buffer to which TLV needs to be written
* @param    length         @b{(output)} length of the TLV that is written
*
* @returns  OFDPA_E_NONE, if successful
* @returns  OFDPA_E_FAIL, otherwise
*
* @comments none
*
* @end
*
*********************************************************************/

OFDPA_ERROR_t dot1agCfmDataTlvWrite(uint8_t *data, uint16_t len, uint8_t *dataPtr, uint32_t *length);

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
OFDPA_ERROR_t dot1agCfmEndTlvWrite(uint8_t *dataPtr, uint32_t *length);

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
OFDPA_ERROR_t dot1agCfmLTMEgressIdTlvRead(dot1agCfmLTMEgressIdTLV_t *pLtmEgressIdTlv, uint8_t *dataPtr);

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
OFDPA_ERROR_t dot1agCfmLTMEgressIdTlvBuild(dot1agCfmStackObj_t *pMEPObj, dot1agCfmLTMEgressIdTLV_t *pLtmEgressIdTlv);

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
                                                   uint32_t                  *length);

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
                                          uint8_t                 *dataPtr);

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
                                           dot1agCfmLTREgressIdTLV_t *pLTREgressIdTlv);

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
                                           uint32_t                  *length);

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
                                           uint8_t                  *dataPtr);

/*********************************************************************
*
* @purpose  Constructs Reply Ingress TLV
*
* @param    intIfNum         @b{(input)}  Internal Interface number
* @param    vlanId          @b{(input)}  PrimVID of the MA
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
                                            dot1agCfmReplyIngressTLV_t *pReplyIngressTlv);

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
OFDPA_ERROR_t dot1agCfmReplyIngressTlvWrite(dot1agCfmReplyIngressTLV_t *pReplyIngressTlv, uint8_t *dataPtr, uint32_t *length);

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
                                          uint8_t                 *dataPtr);

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
                                           dot1agCfmReplyEgressTLV_t *pReplyEgressTlv);

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
                                           uint32_t                  *length);

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
                                          uint32_t                 *length);

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
                                         uint32_t                *length);

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
                                        uint32_t                *length);

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
void dot1agCfmDebugTlvDump(uint8_t tlvType, uint16_t tlvLen, uint8_t *tlvDataPtr);

#endif /* INCLUDE_DOT1AG_TLV_H */
