/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
* @filename  g8131_pdu.c
*
* @purpose   G8131 pdu related handling
*
* @component g8131
*
* @comments  none
*
* @create    4-NOV-2013
*
* @end
**********************************************************************/
#include <stdlib.h>
#include "g8131_include.h"
#include "dot1ag_outcalls.h"
#include "dot1ag_pdu.h"
#include "ofdb_api.h"

#define G8131_APS_PACKET_OPCODE 39
#define G8131_APS_PACKET_FIRST_TLV_OFFSET 4
#define G8131_APS_PACKET_PAYLOAD_LENGTH 4

extern g8131Cfg_t   *g8131Cfg;
extern void     *g8131PduTxBufPoolId;

/* 
g8131PktFromCpu is running in aps task has been protected by semaphore "g8131RWLock". 
so the global variable are protected with other module data sharing the same semaphore.
*/
static g8131ControlPkt_t g8131PktFromCpu;

/*********************************************************************
* @purpose  Copy common aps hdr data to a packet buffer
*
* @param    header   @b{(input)} APS common header value to be written
* @param    data     @b{(input)} Ptr to data
*
* @returns  none
*
* @notes    aps module - Jan-9-2014
*
* @end
**********************************************************************/
static void g8131HdrWrite(g8131CommonHdr_t *header, uint8_t *data)
{
  uint8_t byteVal = 0;
  uint8_t *pData;

  pData = data;

  byteVal |= header->level << 5;
  byteVal |= (header->version & 0x1F);
  G8131_PUT_BYTE(byteVal, pData);
  G8131_PUT_BYTE(header->opcode, pData);
  G8131_PUT_BYTE(header->flags, pData);
  G8131_PUT_BYTE(header->first_tlv_offset, pData);
}

/*********************************************************************
* @purpose  Get aps common header value from packet buffer
*
* @param    data       @b{(input)} Local storage for aps packet
* @param    header     @b{(output)}aps common header
*
* @returns  none
*
* @notes    aps module Jan-9-2014
*
* @end
**********************************************************************/
static void g8131HdrRead(uint8_t *data, g8131CommonHdr_t *header)
{
  uint8_t byteVal = 0;

  G8131_GET_BYTE(byteVal, data);
  header->level = ((byteVal >> 5) & 0x3);
  header->version = (byteVal & 0x1F);
  G8131_GET_BYTE(byteVal, data);
  header->opcode = byteVal;
  G8131_GET_BYTE(byteVal, data);
  header->flags = byteVal;
  G8131_GET_BYTE(byteVal, data);
  header->first_tlv_offset = byteVal;
}

/*********************************************************************
* @purpose  Copy aps siagnal header data to a packet buffer
*
* @param    header   @b{(input)} APS signal header value to be written
* @param    data     @b{(input)} Ptr to data
*
* @returns  none
*
* @notes    aps module - Jan-9-2014
*
* @end
**********************************************************************/
static void g8131APSWrite(g8131Payload_t *header, uint8_t *data)
{
  uint8_t byteVal = 0, *pData;

  pData = data;

  byteVal |= header->request << 4;
  byteVal |= (header->type & 0x0F);
  G8131_PUT_BYTE(byteVal, pData);
  G8131_PUT_BYTE(header->reqSignal, pData);
  G8131_PUT_BYTE(header->brgSignal, pData);
  G8131_PUT_BYTE(header->reserved, pData);
}

/*********************************************************************
* @purpose  Get aps siagnal header data from a packet buffer
*
* @param    data     @b{(input)} Local storage for aps packet
* @param    header   @b{(output)} aps sogmal header
*
* @returns  none
*
* @notes    aps module - Jan-9-2014
*
* @end
**********************************************************************/
static void g8131APSRead(uint8_t *data, g8131Payload_t *header)
{
  uint8_t byteVal = 0;

  G8131_GET_BYTE(byteVal, data);
  header->request = ((byteVal >> 4) & 0xF);
  header->type = (byteVal & 0xF);
  G8131_GET_BYTE(byteVal, data);
  header->reqSignal = byteVal;
  G8131_GET_BYTE(byteVal, data);
  header->brgSignal = byteVal;
  G8131_GET_BYTE(byteVal, data);
  header->reserved = byteVal;
}

static void g8131TxDumpArray(uint8_t *buf, uint32_t id)
{
  G8131_TRACE(G8131_DEBUG_TX_PKT,
              "[*]%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
              buf[id + 0],  buf[id + 1],  buf[id + 2],  buf[id + 3],
              buf[id + 4],  buf[id + 5],  buf[id + 6],  buf[id + 7],
              buf[id + 8],  buf[id + 9],  buf[id + 10], buf[id + 11],
              buf[id + 12], buf[id + 13], buf[id + 14], buf[id + 15]);
}

static void g8131TxPktDump(g8131ControlPkt_t *pkt)
{
  G8131_TRACE(G8131_DEBUG_TX_PKT,
              "[+]intIfNum %d vlanId %d innerVlanId %d length %d toFwd %d g8131PduOffset 0x%8x",
              pkt->intIfNum, pkt->vlanId, pkt->innerVlanId,
              pkt->length, pkt->toFwd, *(pkt->g8131PduOffset));

  g8131TxDumpArray(pkt->payLoad, 0);
  g8131TxDumpArray(pkt->payLoad, 16);
  g8131TxDumpArray(pkt->payLoad, 32);
}

OFDPA_ERROR_t g8131PduRcvFromDot1ag(uint32_t intIfNum,
                                    uint32_t vlanId,
                                    uint32_t innerVlanId,
                                    uint8_t *g8131Pkt,
                                    uint32_t dataLength,
                                    uint32_t cfmPduOffset)
{
  OFDPA_ERROR_t status;
  g8131PduMsg_t pduMsg;

  memset(&pduMsg, 0, sizeof(pduMsg));

  pduMsg.g8131Pkt = malloc(dataLength);
  if (pduMsg.g8131Pkt == NULL)
  {
    return OFDPA_E_FULL;
  }
  memcpy(pduMsg.g8131Pkt, g8131Pkt, dataLength);

  pduMsg.vlanId       = vlanId;
  pduMsg.innerVlanId  = innerVlanId;
  pduMsg.dataLength   = dataLength;
  pduMsg.cfmPduOffset = cfmPduOffset;

  status = g8131IssueCmd(G8131_EVENT_PDU_RECEIVE, intIfNum, &pduMsg);

  if (OFDPA_E_NONE != status)
  {
    free(pduMsg.g8131Pkt);
  }

  return status;
}

OFDPA_ERROR_t g8131PduRcvHandler(uint32_t  intIfNum,
                                 uint32_t  vlanId,
                                 uint32_t  innerVlanId,
                                 uint8_t  *g8131Pkt,
                                 uint32_t  dataLength,
                                 uint32_t  cfmPduOffset)
{
  OFDPA_ERROR_t status = OFDPA_E_NONE;
  uint32_t grpId;
  uint32_t offset;
  g8131ReqMsg_t msg;
  g8131SMRequest_t smReq;
  g8131CommonHdr_t cmnHdr;
  g8131Payload_t   payload;

  if (OFDPA_TRUE != g8131StateMachineIsEnabled())
  {
    return OFDPA_E_NONE;
  }

  for (grpId = OFDPA_G8131_PG_MIN_ID; grpId <= OFDPA_G8131_PG_MAX_ID; grpId++)
  {
    if ((g8131Cfg->pgData[grpId].primVidWorking    == vlanId) ||
        (g8131Cfg->pgData[grpId].primVidProtection == vlanId))
    {
      break;
    }
  }

  /**/
  if (grpId > OFDPA_G8131_PG_MAX_ID)
  {
    /*leishenghua modify 20170120, 两端保护组创建不同步，
       可能出现找不到保护组情况*/
    G8131_TRACE(G8131_DEBUG_RX_PKT, "[#ERROR#] PG not found: intf %d vlanId %d innerVlanId %d",
                intIfNum, vlanId, innerVlanId);
    free(g8131Pkt);
	
    return OFDPA_E_NONE/*OFDPA_E_NOT_FOUND*/;
  }

  if (g8131Cfg->pgData[grpId].primVidWorking == vlanId)
  {
    /*ALARM: G8131_EVENT_ALARM_DFOP_CM*/
    memset(&msg, 0, sizeof(msg));
    msg.grpId = grpId;
    status = g8131IssueCmd(G8131_EVENT_ALARM_DFOP_CM, 0, &msg);
    if (OFDPA_E_NONE != status)
    {
      G8131_TRACE(G8131_DEBUG_ERROR, "[#ERROR#]");
      free(g8131Pkt);
      return status;
    }
  }

  /*parse MEL/VERSION/OPCODE/FLAGS/TLV OFFSET*/

  offset = cfmPduOffset;

  g8131HdrRead(&g8131Pkt[offset], &cmnHdr);
  offset += 4;

  g8131APSRead(&g8131Pkt[offset], &payload);

  /* done analysing packet content, free storage here */
  free(g8131Pkt);

  if (G8131_PROT_TYPE_NO_PERMANENT_BRG != ((payload.type >> 2) & 1))
  {
    /*ALARM: G8131_EVENT_ALARM_DFOP_PM*/
    memset(&msg, 0, sizeof(msg));
    msg.grpId = grpId;
    status = g8131IssueCmd(G8131_EVENT_ALARM_DFOP_PM, 0, &msg);
    _G8131_RETURN_CODE_CHECK(status);

    /*Drop the packet directly*/
    return OFDPA_E_NONE;
  }

  if (OFDPA_E_NONE != g8131ConvField2Request(payload.request,
                                             payload.type, payload.reqSignal, payload.brgSignal, &smReq))
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] g8131 convert req %d type %d reqSig %d brgSig %d failed.",
                payload.request, payload.type, payload.reqSignal, payload.brgSignal);
    return OFDPA_E_FAIL;
  }

  /*send event message.*/
  memset(&msg, 0, sizeof(msg));
  msg.grpId   = grpId;
  msg.request = smReq;
  msg.signal  = payload.reqSignal;
  status = g8131IssueCmd(G8131_EVENT_REQUEST, 0, &msg);
  _G8131_RETURN_CODE_CHECK(status);

  return status;
}

/*********************************************************************
* @purpose  Transmit PDU on a given interface
*
* @param    uint32_t          intIfNum  @b((input)) internal interface number
* @param    uint32_t          vlanId    @b((input)) vlan identifier
* @param    g8131ControlPkt_t  g8131Pkt  @b((input)) pointer to G8131 PDU type.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t g8131PduTransmit(uint32_t intIfNum,
                               uint16_t vlanId,
                               g8131ControlPkt_t *g8131Pkt)
{
	return 0;
}

/*********************************************************************
* @purpose  Transmit request packets
*
* @param    grpId   @b{(input)}
* @param    request @b{(input)}
*
* @returns  none
*
* @notes    g8131
*
* @end
**********************************************************************/
OFDPA_ERROR_t g8131PduXmitRequest(uint32_t grpId,
                                  uint32_t request)
{
  OFDPA_ERROR_t     status;
  uint8_t          *data;
  uint8_t           byteVal;
  g8131CommonHdr_t  g8131Hdr;
  g8131Payload_t    g8131Payload;
  uint16_t          shortVal;
  uint32_t          intIfNum;
  uint32_t          primVlan;
  uint32_t          level;
  uint8_t           rr_req;
  uint8_t           rr_type;
  uint8_t           rr_reqSig;
  uint8_t           rr_brgSig;
//  ofdbOamBhhData_t *bhhDataPtr;
  ofdbOamLmepData_t *oamMapData;
  uint32_t          len = 0;
  
  G8131_TRACE(G8131_DEBUG_SM, "For test grp %d request %d", grpId, request);

  if (OFDPA_TRUE != g8131Cfg->enable)
  {
    /*If G8131 is not enabled, not send packet out.*/
    return OFDPA_E_NONE;
  }

  if (OFDPA_E_NONE != g8131ConvRequest2Field(request,
                                             &rr_req, &rr_type, &rr_reqSig, &rr_brgSig))
  {
    G8131_TRACE(G8131_DEBUG_ERROR,
                "[#ERROR#] g8131 convert request %dfailed.", request);
    return OFDPA_E_FAIL;
  }

  memset(&g8131PktFromCpu, 0x0, sizeof(g8131PktFromCpu));
  data = g8131PktFromCpu.payLoad;

  primVlan = g8131Cfg->pgData[grpId].primVidProtection;

//  status = ofdbOamBhhPrimVlanMapHeaderDataNodeGet(primVlan, &bhhDataPtr);
  status = ofdbInjectedOamLmepIdMapDataGet(g8131Cfg->pgData[grpId].lmepIdProtection, &oamMapData);

  if (status != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  intIfNum = oamMapData->ifNum;
  level    = oamMapData->megLevel;

  g8131PktFromCpu.intIfNum = intIfNum;
  g8131PktFromCpu.vlanId   = primVlan;

  len = mplsTpPrepareMplsHeader(oamMapData, 0, data, 0);

  if (len == 0)
  {
    return OFDPA_E_FAIL;
  }
  
  data += len;

  /*Ether Type */
  shortVal = G8131_ETYPE_CFM;
  G8131_PUT_SHORT(shortVal, data);
  g8131PktFromCpu.g8131PduOffset = data;

  /*HEAD 1 <MEL/VER/OPCODE/FLAGS/OFFSET>*/
  g8131Hdr.level   = level;
  g8131Hdr.version = 0;
  g8131Hdr.opcode  = G8131_APS_PACKET_OPCODE;
  g8131Hdr.flags   = 0;
  g8131Hdr.first_tlv_offset = G8131_APS_PACKET_FIRST_TLV_OFFSET;
  g8131HdrWrite(&g8131Hdr, data);
  data += G8131_APS_PACKET_FIRST_TLV_OFFSET;
  memcpy(&g8131PktFromCpu.g8131Hdr, &g8131Hdr, sizeof(g8131PktFromCpu.g8131Hdr));

  /*HEAD 1 <Request/Type/RequestSignal/BridgeSignal/Reserved>*/

  memset(&g8131Payload, 0, sizeof(g8131Payload));
  g8131Payload.request   = rr_req;
  g8131Payload.type      = rr_type;
  g8131Payload.reqSignal = rr_reqSig;
  g8131Payload.brgSignal = rr_brgSig;
  g8131APSWrite(&g8131Payload, data);
  data += G8131_APS_PACKET_PAYLOAD_LENGTH;

  /*End of TLV*/
  byteVal = 0;
  G8131_PUT_BYTE(byteVal, data);

  g8131PktFromCpu.length = data - g8131PktFromCpu.payLoad;

  G8131_TRACE(G8131_DEBUG_TX_PKT,
              "For test grp %d request %d type %d regSig %d brg %d",
              grpId, rr_req, rr_type, rr_reqSig, rr_brgSig);

  g8131TxPktDump(&g8131PktFromCpu);

  status = g8131PduTransmit(intIfNum, primVlan, &g8131PktFromCpu);
  _G8131_RETURN_CODE_CHECK(status);

  return status;
}
