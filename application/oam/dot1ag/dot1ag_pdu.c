/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
**********************************************************************
*
* @filename       dot1ag_pdu.c
*
* @purpose        Dot1ag CFM PDU Receive and Transmit Handling
*
* @component      Dot1ag
*
* @comments       none
*
* @create         02/18/2009
*
* @end
*
**********************************************************************/

#include <stdio.h>
#include "ofdpa_porting.h"
#include "ofdpa_datatypes.h"
#include "ofdb_api.h"
//#include "driver_oam.h"
#include "dot1ag.h"
#include "dot1ag_api.h"
#include "dot1ag_pdu.h"
#include "dot1ag_sm.h"
#include "dot1ag_db.h"
#include "dot1ag_proto.h"
#include "dot1ag_outcalls.h"
#include "dot1ag_debug.h"
#include "msgq.h"

OFDPA_ERROR_t mplsTpProcessMplsHeader(dot1agControlPkt_t *dot1agPkt, dot1agCfmStackObj_t **mpObj);
void mplsTpLabelGet(uint8_t *buf, uint32_t *label, uint8_t *exp, uint8_t *bos, uint8_t *ttl);

#define G_ACH  0x10008902

#define APS_PACKET_OPCODE 39

typedef struct dot1agLabelToMpMappingTable_node_s
{
  uint32_t             label;           /* index */
  dot1agCfmStackObj_t *mpObj;           /* pointer to dot1ag MEP or MIP object */
  void                *next;            /* This field must be the last element in the AVL Tree Node */
} dot1agLabelToMpMappingTable_node_t;

static dot1agControlPkt_t tmpdot1agPkt;

static avlTree_t     dot1agLabelToMpMappingTable_tree;
static ofdpaRWlock_t dot1agLabelMapRWLock;

void dot1agDebugPktDump(uint8_t *pkt)
{
  int row, column;
  uint32_t row_count;
  uint32_t pkt_dump_len = 64;
  uint32_t pkt_start_offset = 0;
  uint8_t buf[100];

  printf("\r\n===================\n");
  row_count = pkt_dump_len / 16;
#if 0
  if (pkt_dump_len % 16)
  {
    row_count++;
  }
#endif
  for (row = 0; row < row_count; row++)
  {
    snprintf(buf, sizeof(buf), "%04x ", row * 16);
    for (column = 0; (column < 16) && ((row * 16 + column) < pkt_dump_len); column++)
    {
      snprintf(buf, sizeof(buf), "%2.2x ", pkt[pkt_start_offset + row * 16 + column]);
    }

    snprintf(buf, sizeof(buf), "\n");
    printf("%s", buf);
  }
  printf("===================\n");
}

OFDPA_ERROR_t dot1agBhhLabelMapEntryAdd(ofdpaMplsFlowEntry_t *mplsFlow)
{
  dot1agLabelToMpMappingTable_node_t  mapNode;
  dot1agLabelToMpMappingTable_node_t *dataPtr;
  ofdpaOamMepConfig_t                 mepConfig;
  ofdpaOamMipConfig_t                 mipConfig;
  ofdpaOamMegConfig_t                 megConfig;
  ofdbOamMegInfo_t                    megInfo;
  dot1agCfmStackObj_t                *pMPObj = NULL;
  uint32_t                            megIndex;
  uint32_t                            intIfNum;
  OFDPA_ERROR_t                       rc     = OFDPA_E_NONE;

  /* retrieve MEP table entry */
  rc = ofdbOamMepGet(mplsFlow->lmepId, &mepConfig, NULL);
  if (OFDPA_E_NONE != rc)
  {
    rc = ofdbOamMipGet(mplsFlow->lmepId, &mipConfig, NULL);
    if (OFDPA_E_NONE != rc)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Failed retrieving table entry for lmepId = %d.\r\n",
                         mplsFlow->lmepId);
      return rc;
    }

    megIndex = mipConfig.megIndex;
    intIfNum = mipConfig.ifIndex;
  }
  else
  {
    megIndex = mepConfig.megIndex;
    intIfNum = mepConfig.ifIndex;
  }

  /* retrieve MEG table entry */
  rc = ofdbOamMegGet(megIndex, &megConfig, NULL, &megInfo);
  if (OFDPA_E_NONE != rc)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Failed retrieving MEG table entry for lmepId = %d (megIndex = %d).\r\n",
                       mplsFlow->lmepId, megIndex);
    return OFDPA_E_ERROR;
  }

  if (megConfig.megType != OFDPA_OAM_MEG_TYPE_G8113_1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Wrong MEG type for lmepId = %d (megIndex = %d, megType = %d).\r\n",
                       mplsFlow->lmepId, megIndex, megConfig.megType);
    return rc;
  }

  pMPObj = dot1agCfmStackObjGet(intIfNum, megInfo.primVid, megConfig.level, 0, AVL_NEXT);
  if ((NULL == pMPObj)                                            ||
      (pMPObj->key.dot1agCfmStackifIndex      != intIfNum)        ||
      (pMPObj->key.dot1agCfmStackVlanIdOrNone != megInfo.primVid) ||
      (pMPObj->key.dot1agCfmStackMdLevel      != megConfig.level))
  {      
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MP Object not found for lmepId = %d.\r\n",
                       mplsFlow->lmepId);
    return OFDPA_E_ERROR;
  }

  memset(&mapNode, 0, sizeof(mapNode));
  mapNode.label = mplsFlow->match_criteria.mplsLabel;
  mapNode.mpObj = pMPObj;

  dpaRWlockWritelock(&dot1agLabelMapRWLock);

  dataPtr = avlInsertEntry(&dot1agLabelToMpMappingTable_tree, &mapNode);

  if (dataPtr == &mapNode)
  {
    /* Table is full */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Table is full.\r\n", 0);
    rc = OFDPA_E_FULL;
  }
  else if (dataPtr != 0)
  {
    /* Duplicate entry. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry exists.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  dpaRWlockUnlock(&dot1agLabelMapRWLock);

  return rc;
}

OFDPA_ERROR_t dot1agBhhLabelMapEntryDelete(ofdpaMplsFlowEntry_t *mplsFlow)
{
  dot1agLabelToMpMappingTable_node_t *dataPtr;
  OFDPA_ERROR_t                       rc = OFDPA_E_NONE;

  dpaRWlockWritelock(&dot1agLabelMapRWLock);

  dataPtr = avlDeleteEntry(&dot1agLabelToMpMappingTable_tree, &mplsFlow->match_criteria.mplsLabel);

  dpaRWlockUnlock(&dot1agLabelMapRWLock);

  if (dataPtr == NULL)
  {
    /* Entry not found */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry not found.\r\n", 0);
    rc = OFDPA_E_NOT_FOUND;
  }

  return rc;
}

static OFDPA_ERROR_t dot1agBhhLabelMapMpGet(uint32_t mplsLabel, dot1agCfmStackObj_t **mpObj)
{
  dot1agLabelToMpMappingTable_node_t *dataPtr;
  OFDPA_ERROR_t                       rc = OFDPA_E_NONE;

  if (mpObj == NULL)
  {
    rc = OFDPA_E_PARAM;
  }
  else
  {
    dpaRWlockReadlock(&dot1agLabelMapRWLock);

    dataPtr = avlSearch(&dot1agLabelToMpMappingTable_tree, &mplsLabel, AVL_EXACT);

    if (dataPtr == NULL)
    {
      /* Entry not found */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "Entry not found.\r\n", 0);
      rc = OFDPA_E_NOT_FOUND;
    }
    else
    {
      *mpObj = dataPtr->mpObj;
    }

    dpaRWlockUnlock(&dot1agLabelMapRWLock);
  }

  return rc;
}

OFDPA_ERROR_t dot1agBhhLabelMapMpInit(void)
{
  OFDPA_ERROR_t rc;

  rc = dpaRWlockInit(&dot1agLabelMapRWLock, NULL);
  if (OFDPA_E_NONE != rc)
  {
    return rc;
  }

  if (avlAllocAndCreateAvlTree(&dot1agLabelToMpMappingTable_tree,
                               1000,
                               sizeof(dot1agLabelToMpMappingTable_node_t),
                               0x10,
                               avlCompareULong32,
                               0) != 0)
  {
    return OFDPA_E_INTERNAL;
  }

  return rc;
}

/*********************************************************************
* @purpose  Interceptor func for CFM PDUs
*
* @param    buffer    @b{(input)} Handle to buffer
*
* @returns  packetConsumed
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agPduRecv(void *buffer)
{
	return 0;
}

/*********************************************************************
* @purpose  Process received CFM PDU
*
* @param    msg    @b{(input)} Handle to the CFM PDU message
*
* @returns  OFDPA_E_NONE  if the PDU was successfully processed
* @returns  OFDPA_E_FAIL  Error in processing the PDU
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agPduReceiveProcess(dot1agPduMsg_t *msg)
{
	return 0;
}

/*********************************************************************
* @purpose  Parse received CFM PDU
*
* @param    dot1agPkt  @b{(input/output)} Local storage for the CFM PDU message
*
* @returns  OFDPA_E_NONE  if the PDU was successfully parsed
* @returns  OFDPA_E_FAIL  if the PDU is in incorrect
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmPacketParse(dot1agControlPkt_t *dot1agPkt)
{
  uint8_t                *data;
  Dot1agCfmMpDirection_t  direction;
  uint32_t                mdLevel;
  uint32_t                intIfNum;
  dot1agCfmStackObj_t    *pMPObj        = NULL;
  dot1agCfmStackObj_t    *pMIPObj       = NULL;
  dot1agCfmStackObj_t    *pMEPObj       = NULL;
  OFDPA_BOOL              foundSomeMP   = OFDPA_FALSE;
  uint32_t                ethHeaderSize = 0;
  uint8_t                *ethType;
  OFDPA_ERROR_t           rc;

  data = dot1agPkt->payLoad;
  if (0 == dot1agPkt->outPort)
  {
    intIfNum = dot1agPkt->intIfNum;
  }
  else
  {
    intIfNum = dot1agPkt->outPort;
  }

  /* Copy the Destination mac of the PDU */
  memcpy(dot1agPkt->destMac, data, OFDPA_MAC_ADDR_LEN);
  data += OFDPA_MAC_ADDR_LEN;

  /* Copy the Source mac of the PDU */
  memcpy(dot1agPkt->srcMac, data, OFDPA_MAC_ADDR_LEN);
  data += OFDPA_MAC_ADDR_LEN;

  /* check if src mac is a group mac 802.1ag d8-2 21.3.2*/
  if (dot1agPkt->srcMac[0] & 0x01)
  {
    DOT1AG_TRACE(CFM_PDU, "Invalid src mac\r\n");
    return OFDPA_E_FAIL;
  }

  ethHeaderSize = dpaDataOffsetGet(dot1agPkt->payLoad);
  data = dot1agPkt->payLoad + ethHeaderSize;

  if (dot1agPkt->fromWire == OFDPA_TRUE)
  {
    if ((dot1agPkt->payLoad[16] == 0x81) && (dot1agPkt->payLoad[17] == 0x0))
    {
      dot1agPkt->cfmPduOffset = data + 4;
    }
    else
    {
      dot1agPkt->cfmPduOffset = data;
    }
  }
  else
  {
    dot1agPkt->cfmPduOffset = data;
  }

  ethType = dot1agPkt->cfmPduOffset - 2;
  if ((ethType[0] == 0x88) &&(ethType[1] == 0x47))
  {
    rc = mplsTpProcessMplsHeader(dot1agPkt, &pMPObj);
    if (OFDPA_E_NONE != rc)
    {
      DOT1AG_TRACE(CFM_PDU, "Invalid MPLS Packet\r\n");
      return rc;
    }
  }

  /* Extract CFM common header from the CFM PDU and validate it */
  if (dot1agCfmCommonHeaderExtract(dot1agPkt) == OFDPA_E_FAIL)
  {
    DOT1AG_TRACE(CFM_PDU, "Invalid CFM Header\r\n");
    return OFDPA_E_FAIL;
  }

  DOT1AG_TRACE(CFM_PDU, "CFM PDU Rx for intIfNum %d vlanId %d opCode %d level %d version %d fromWire: %d\r\n",
               intIfNum, dot1agPkt->vlanId, dot1agPkt->cfmHdr.opcode, dot1agPkt->cfmHdr.level,
               dot1agPkt->cfmHdr.version, dot1agPkt->fromWire);

  /*APS packet*/
  if (dot1agPkt->cfmHdr.opcode == APS_PACKET_OPCODE)
  {
    if (NULL != dot1agCFM.apsPduRcvCallBack)
    {
      /* NOTE: dot1agPkt->cfmPduOffset is NOT an offset to the CFM header, rather it is a
       * POINTER to that header */
      dot1agCFM.apsPduRcvCallBack(dot1agPkt->intIfNum, dot1agPkt->vlanId,
                                  dot1agPkt->innerVlanId, dot1agPkt->payLoad, dot1agPkt->length,
                                  (uint32_t)(dot1agPkt->cfmPduOffset - dot1agPkt->payLoad));
    }
    return OFDPA_E_NONE;
  }


  if (dot1agPkt->fromWire == OFDPA_TRUE)
  {
    mdLevel = 0;
    direction = 0;
    dot1agPkt->toFwd = OFDPA_FALSE;

    /* If BHH MP (can only be one - MEP or MIP), mplsTpProcessMplsHeader has set pMPObj */
    if (NULL != pMPObj)
    {
      if (pMPObj->operMode == OFDPA_TRUE)
      {
        foundSomeMP = OFDPA_TRUE;
        if (pMPObj->mepData != NULL)
        {
          /* Found an MEP configured */
          pMEPObj = pMPObj;
          /* Can only be down MEP -- BHH up MEPs not supported */
          dot1agCfmDownMEPWireSideRxProcess(dot1agPkt, pMEPObj);
        }
        else
        {
          pMIPObj = pMPObj;
          dot1agCfmMIPWireSideRxProcess(dot1agPkt, pMIPObj);
        }
      }
    }
    else
    {
      /* Get MP Configured on the PDU Interface. */
      while ((pMPObj = dot1agCfmStackObjGet(intIfNum, dot1agPkt->vlanId, mdLevel,
                                            direction, AVL_NEXT)) != NULL)
      {
        pMEPObj = NULL;
        pMIPObj = NULL;
        if ((pMPObj->key.dot1agCfmStackifIndex == intIfNum) &&
            (pMPObj->key.dot1agCfmStackVlanIdOrNone == dot1agPkt->vlanId))
        {
          if (pMPObj->operMode == OFDPA_TRUE)
          {
            foundSomeMP = OFDPA_TRUE;
            if (pMPObj->mepData != NULL)
            {
              /* Found an MEP configured */
              pMEPObj = pMPObj;
              if (pMEPObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP)
              {
                /* Old HW could only get packets to Up MEPs via a relay process.  New HW
                 * can support Up MEPs in HW so the PDUs are received from the wire. */
                dot1agCfmUpMEPRelaySideRxProcess(dot1agPkt, pMEPObj);
              }
              else
              {
                dot1agCfmDownMEPWireSideRxProcess(dot1agPkt, pMEPObj);
              }
            }
            else
            {
              /* Find the MIP with Frame mdLevel >= MIP mdLevel */
              pMIPObj = pMPObj;
              dot1agCfmMIPWireSideRxProcess(dot1agPkt, pMIPObj);
            }
          }
        }
        else
        {
          /* End of valid stack objects for this intIfNum, MA */
          break;
        }

        mdLevel = pMPObj->key.dot1agCfmStackMdLevel;
        direction = pMPObj->key.dot1agCfmStackDirection;
      } /* Iterate through CFM stack to find appropriate MPs */
    }

    /* Call Bridge relay logic */
    if (dot1agPkt->toFwd == OFDPA_TRUE)
    {
      (void)dot1agPduTransmit(pMEPObj->key.dot1agCfmStackifIndex,
                              pMEPObj->key.dot1agCfmStackVlanIdOrNone,
                              dot1agPkt);
    }
    /* 802.1ag d8-1 20.42.1.1, 20.42.1.5, 20.42.1.6 */
    if (foundSomeMP == OFDPA_FALSE &&
        dot1agPkt->cfmHdr.opcode == DOT1AG_LINKTRACE_MESSAGE_OPCODE)
    {
      dot1agLinkTraceResponder(dot1agPkt, NULL);
    }
  }
  else                                  /* dot1agPkt->fromWire */
  {
    if (dot1agPkt->fromOwnUpMEP == OFDPA_TRUE)
    {
      if (dot1agPkt->cfmHdr.opcode == DOT1AG_LINKTRACE_MESSAGE_OPCODE)
      {
        dot1agLinkTraceResponder(dot1agPkt, NULL);
      }
      else if (dot1agPkt->cfmHdr.opcode == DOT1AG_LINKTRACE_REPLY_OPCODE)
      {
        pMEPObj = dot1agCfmStackObjGet(intIfNum, dot1agPkt->vlanId,
                                       dot1agPkt->cfmHdr.level,
                                       DOT1AG_MP_DIRECTION_UP,
                                       AVL_EXACT);
        if ((pMEPObj != NULL) &&
            (pMEPObj->operMode == OFDPA_TRUE) &&
            (pMEPObj->mepData != NULL))
        {
          dot1agMEPLinkTraceInitiator(dot1agPkt, pMEPObj);
        }
      }
    }
    else
    {
      dot1agCfmPacketRelay(dot1agPkt->intIfNum, dot1agPkt);
    }
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Packet processing post bridge relay
*
* @param    dot1agPkt  @b{(input)} Interface to which PDU is to
*                                     be relayed
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                            PDU message
*
* @returns  OFDPA_E_NONE  if the PDU was successfully processed
* @returns  OFDPA_E_FAIL  Encountered error
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmPacketRelay(uint32_t intIfNum, dot1agControlPkt_t *dot1agPkt)
{
  Dot1agCfmMpDirection_t direction;
  uint32_t              mdLevel;
  dot1agCfmStackObj_t   *pMPObj = NULL, *pMIPObj = NULL;
  dot1agCfmStackObj_t   *pMEPObj = NULL;

  dot1agPkt->toFwd = OFDPA_FALSE;

  mdLevel = 0;
  direction = 0;

  /* Get MP Configured on the received PDU Interface. */
  while ((pMPObj = dot1agCfmStackObjGet(intIfNum, dot1agPkt->vlanId, mdLevel,
                                        direction, AVL_NEXT)) != NULL)
  {
    pMEPObj = NULL;
    pMIPObj = NULL;
    if (pMPObj->key.dot1agCfmStackifIndex == intIfNum &&
        pMPObj->key.dot1agCfmStackVlanIdOrNone == dot1agPkt->vlanId)
    {
      if (pMPObj->operMode == OFDPA_TRUE)
      {
        if (pMPObj->mepData != NULL)
        {
          /* Found an MEP configured */
          pMEPObj = pMPObj;
        }
        else
        {
          /* Find the MIP with Frame mdLevel >= MIP mdLevel */
          pMIPObj = pMPObj;
        }
      }
    }
    else
    {
      /* End of valid stack objects for this intIfNum, MA */
      break;
    }

    if (pMEPObj != NULL)
    {
      if (pMEPObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP)
      {
        dot1agCfmUpMEPRelaySideRxProcess(dot1agPkt, pMEPObj);
      }
      else
      {
        dot1agCfmDownMEPRelaySideRxProcess(dot1agPkt, pMEPObj);
      }
    }

    /* Process MIP too, and forward the frame if not forwarded in MEP processing */
    if (pMIPObj != NULL)
    {
      dot1agCfmMIPRelaySideRxProcess(dot1agPkt, pMIPObj);
    }
    mdLevel = pMPObj->key.dot1agCfmStackMdLevel;
    direction = pMPObj->key.dot1agCfmStackDirection;
  }/* Iterate through CFM stack to find appopriate MPs */

  /* Call Bridge relay logic */
  if (dot1agPkt->toFwd == OFDPA_TRUE)
  {
    dot1agPduTransmit(intIfNum, dot1agPkt->vlanId, dot1agPkt);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Process CFM PDU received from wire by an UP MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    none
*
* @end
**********************************************************************/
void dot1agCfmUpMEPWireSideRxProcess(dot1agControlPkt_t *dot1agPkt,
                                     dot1agCfmStackObj_t *pMPObj)
{
  /* Terminate frames with Frame mdLevel <= MEP MD Level */
  if (dot1agPkt->cfmHdr.level <= pMPObj->key.dot1agCfmStackMdLevel)
  {
    dot1agPkt->toFwd = OFDPA_FALSE;
    return;
  }
  /* forward to relay process if Frame mdLevel > MEP MD Level */
  dot1agPkt->toFwd = OFDPA_TRUE;
}

/*********************************************************************
* @purpose  Process CFM PDU received from relay logic by an UP MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    none
*
* @end
**********************************************************************/
void dot1agCfmUpMEPRelaySideRxProcess(dot1agControlPkt_t *dot1agPkt,
                                      dot1agCfmStackObj_t *pMPObj)
{
  DOT1AG_TRACE(CFM_OPCODE_DEMUX, "Frame Level %d MP Level %d\r\n",
               dot1agPkt->cfmHdr.level, pMPObj->key.dot1agCfmStackMdLevel);
  /* Forward to relay process if Frame mdLevel > MEP MD Level */
  if (dot1agPkt->cfmHdr.level > pMPObj->key.dot1agCfmStackMdLevel)
  {
    /* Forward to relay process */
    dot1agPkt->toFwd = OFDPA_TRUE;
    return;
  }
  else if (dot1agPkt->cfmHdr.level == pMPObj->key.dot1agCfmStackMdLevel)
  {
    if (dot1agCfmPduFwdCheck(dot1agPkt, pMPObj) == OFDPA_TRUE)
    {
      /* Forward to relay process */
      dot1agPkt->toFwd = OFDPA_TRUE;
      return;
    }

    /* Process PDU if Frame mdLevel == MEP MD Level. Give pdu to
       EqualOpcodeDemux */
    dot1agCfmMpOpcodeDemultiplexer(dot1agPkt, pMPObj);
  }
  else if (dot1agPkt->cfmHdr.opcode == DOT1AG_CONTINUITY_CHECK_OPCODE)
  {
    /* Process PDU if Frame mdLevel < MEP MD Level and opcode == CCM. Give it
       to MEP Continuity check receiver */
    dot1agCfmMpOpcodeDemultiplexer(dot1agPkt, pMPObj);
  }
  dot1agPkt->toFwd = OFDPA_FALSE;
  /* Terminate PDU otherwise */
  return;
}

/*********************************************************************
* @purpose  Process CFM PDU received from wire by an DOWN MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    none
*
* @end
**********************************************************************/
void dot1agCfmDownMEPWireSideRxProcess(dot1agControlPkt_t *dot1agPkt,
                                       dot1agCfmStackObj_t *pMPObj)
{
  DOT1AG_TRACE(CFM_OPCODE_DEMUX, "Frame Level %d MP Level %d\r\n",
               dot1agPkt->cfmHdr.level, pMPObj->key.dot1agCfmStackMdLevel);
  /* Forward to relay process if Frame mdLevel > MEP MD Level */
  if (dot1agPkt->cfmHdr.level > pMPObj->key.dot1agCfmStackMdLevel)
  {
    /* Forward to relay process */
    dot1agPkt->toFwd = OFDPA_TRUE;
    return;
  }
  else if (dot1agPkt->cfmHdr.level == pMPObj->key.dot1agCfmStackMdLevel)
  {
    if (dot1agCfmPduFwdCheck(dot1agPkt, pMPObj) == OFDPA_TRUE)
    {
      /* Forward to relay process */
      dot1agPkt->toFwd = OFDPA_TRUE;
      return;
    }

    /* Process PDU if Frame mdLevel == MEP MD Level. Give pdu to
       EqualOpcodeDemux */
    dot1agCfmMpOpcodeDemultiplexer(dot1agPkt, pMPObj);
  }
  else if (dot1agPkt->cfmHdr.opcode == DOT1AG_CONTINUITY_CHECK_OPCODE)
  {
    /* Process PDU if Frame mdLevel < MEP MD Level and opcode == CCM. Give it
       to MEP Continuity check receiver */
    dot1agCfmMpOpcodeDemultiplexer(dot1agPkt, pMPObj);
  }
  dot1agPkt->toFwd = OFDPA_FALSE;
  /* Terminate PDU otherwise */
  return;
}

/*********************************************************************
* @purpose  Process CFM PDU received from relay logic by an DOWN MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    none
*
* @end
**********************************************************************/
void dot1agCfmDownMEPRelaySideRxProcess(dot1agControlPkt_t *dot1agPkt,
                                        dot1agCfmStackObj_t *pMPObj)
{
  DOT1AG_TRACE(CFM_OPCODE_DEMUX, "Frame Level %d MP Level %d\r\n",
               dot1agPkt->cfmHdr.level, pMPObj->key.dot1agCfmStackMdLevel);
  /* Terminate frames with Frame mdLevel <= MEP MD Level */
  if (dot1agPkt->cfmHdr.level <= pMPObj->key.dot1agCfmStackMdLevel)
  {
    dot1agPkt->toFwd = OFDPA_FALSE;
    return;
  }
  dot1agPkt->toFwd = OFDPA_TRUE;
  /* Transmit on wire if Frame mdLevel > MEP MD Level */
}

/*********************************************************************
* @purpose  Process CFM PDU received from wire by an MIP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    none
*
* @end
**********************************************************************/
void dot1agCfmMIPWireSideRxProcess(dot1agControlPkt_t *dot1agPkt,
                                   dot1agCfmStackObj_t *pMPObj)
{
  DOT1AG_TRACE(CFM_OPCODE_DEMUX, "Frame Level %d MP Level %d\r\n",
               dot1agPkt->cfmHdr.level, pMPObj->key.dot1agCfmStackMdLevel);

  if (dot1agPkt->cfmHdr.level == pMPObj->key.dot1agCfmStackMdLevel)
  {
    /* Process PDU if Frame mdLevel == MEP MD Level and PDU is LTM, LBM */
    dot1agPkt->toFwd = OFDPA_TRUE;
    if ((dot1agPkt->cfmHdr.opcode == DOT1AG_LOOPBACK_MESSAGE_OPCODE) ||
        (dot1agPkt->cfmHdr.opcode == DOT1AG_LINKTRACE_MESSAGE_OPCODE))
    {
      dot1agCfmMpOpcodeDemultiplexer(dot1agPkt, pMPObj);
    }
  }
  else                                  /* Forward non-matching MDL PDUs */
  {
    /* Forward to relay process */
    dot1agPkt->toFwd = OFDPA_TRUE;
  }
}

/*********************************************************************
* @purpose  Process CFM PDU received from relay logic by an MIP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    none
*
* @end
**********************************************************************/
void dot1agCfmMIPRelaySideRxProcess(dot1agControlPkt_t *dot1agPkt,
                                    dot1agCfmStackObj_t *pMPObj)
{
  DOT1AG_TRACE(CFM_OPCODE_DEMUX, "Frame Level %d MP Level %d\r\n",
               dot1agPkt->cfmHdr.level, pMPObj->key.dot1agCfmStackMdLevel);
  /* Terminate frames with Frame mdLevel < MEP MD Level */
  if (dot1agPkt->cfmHdr.level < pMPObj->key.dot1agCfmStackMdLevel)
  {
    dot1agPkt->toFwd = OFDPA_TRUE;
    return;
  }
  else if (dot1agPkt->cfmHdr.level > pMPObj->key.dot1agCfmStackMdLevel)
  {
    /* Transmit on wire */
    dot1agPkt->toFwd = OFDPA_TRUE;
  }
  else if (dot1agPkt->cfmHdr.level == pMPObj->key.dot1agCfmStackMdLevel)
  {
    /* Process PDU if Frame mdLevel == MEP MD Level and PDU is LTM, LBM */
    dot1agPkt->toFwd = OFDPA_TRUE;
    if ((dot1agPkt->cfmHdr.opcode == DOT1AG_LOOPBACK_MESSAGE_OPCODE) ||
        (dot1agPkt->cfmHdr.opcode == DOT1AG_LINKTRACE_MESSAGE_OPCODE))
    {
      dot1agCfmMpOpcodeDemultiplexer(dot1agPkt, pMPObj);
    }
  }
}

/*********************************************************************
* @purpose  Validate Common CFM header of the CFM PDU
*
* @param    pcfmHdr  @b{(input)} Common CFM header of the CFM PDU
*
* @returns  OFDPA_TRUE   if the common CFM header is valid
* @returns  OFDPA_FALSE  if the common CFM header is invalid
*
* @notes    802.1ag d8-1 20.46.4.3
*
* @end
**********************************************************************/
OFDPA_BOOL dot1agCfmIsCommonHdrValid(dot1agCfmCommonHdr_t *pcfmHdr)
{
  OFDPA_BOOL rv = OFDPA_FALSE;

  /* MD Level Check */
  if (pcfmHdr->level > OFDPA_DOT1AG_CFM_MAX_MD_LEVEL)
  {
    return rv;
  }

  /* Opcode and  First TLV Offset Check */
  switch (pcfmHdr->opcode)
  {
    case DOT1AG_CONTINUITY_CHECK_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_CCM_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_LOOPBACK_MESSAGE_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_LBM_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_LOOPBACK_REPLY_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_LBR_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_LINKTRACE_REPLY_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_LTR_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_LINKTRACE_MESSAGE_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_LTM_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_LOSSMEASUREMENT_MESSAGE_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_LMM_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_LOSSMEASUREMENT_REPLY_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_LMR_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_DELAYMEASUREMENT_MESSAGE_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_DMM_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_DELAYMEASUREMENT_REPLY_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_DMR_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_AIS_CHECK_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_AIS_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_LCK_CHECK_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_LCK_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    case DOT1AG_CSF_CHECK_OPCODE:
      if (pcfmHdr->first_tlv_offset < DOT1AG_CFM_CSF_FIRST_TLV_OFFSET)
      {
        return rv;
      }
      break;

    default:
      /* Unknwon opcodes should be forwarded by MHF Multiplexer as per
       * figure 19-3 in 802.1ag standard.
       */
      break;
  }
  return OFDPA_TRUE;
}

/*********************************************************************
* @purpose  Extract CFM common header from the received CFM PDU
*
* @param    dot1agPkt  @b{(input/output)} Local storage for the CFM
*                                            PDU message
*
* @returns  OFDPA_E_NONE  if the PDU was successfully parsed
* @returns  OFDPA_E_FAIL  if the PDU is in incorrect
*
* @notes    802.1ag d8-2 21.4
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmCommonHeaderExtract(dot1agControlPkt_t *dot1agPkt)
{
  uint8_t *data;
  uint8_t  byteVal;
  uint16_t length, currOffset;

  data = dot1agPkt->cfmPduOffset;

  DOT1AG_GET_BYTE(byteVal, data);
  dot1agPkt->cfmHdr.level   = DOT1AG_CFM_HDR_GET_LEVEL(byteVal);
  dot1agPkt->cfmHdr.version = DOT1AG_CFM_HDR_GET_VERSION(byteVal);
  DOT1AG_GET_BYTE(dot1agPkt->cfmHdr.opcode, data);
  DOT1AG_GET_BYTE(dot1agPkt->cfmHdr.flags, data);
  DOT1AG_GET_BYTE(dot1agPkt->cfmHdr.first_tlv_offset, data);

  if (dot1agCfmIsCommonHdrValid (&dot1agPkt->cfmHdr) == OFDPA_FALSE)
  {
    return OFDPA_E_FAIL;
  }

  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN + dot1agPkt->cfmHdr.first_tlv_offset;
  currOffset = data - dot1agPkt->payLoad;

  if (currOffset > dot1agPkt->length)
  {
    return OFDPA_E_FAIL;
  }

  DOT1AG_GET_BYTE(byteVal, data);
  while (byteVal != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);

    currOffset += 1 + 2 + length;
    if (currOffset > dot1agPkt->length)
    {
      return OFDPA_E_FAIL;
    }
    data += length;
    DOT1AG_GET_BYTE(byteVal, data);
  }/* End of TLV loop */

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Process CFM PDU according to the opcode and further
*           parse the pdu
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 19.2, 19.3
*
* @end
**********************************************************************/
void dot1agCfmMpOpcodeDemultiplexer(dot1agControlPkt_t *dot1agPkt,
                                    dot1agCfmStackObj_t *pMPObj)
{
  switch (dot1agPkt->cfmHdr.opcode)
  {
    case DOT1AG_CONTINUITY_CHECK_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPContinuityCheckReceiver(dot1agPkt, pMPObj);
      }
      break;
    case DOT1AG_LOOPBACK_REPLY_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPLoopBackInitiatorReceive(dot1agPkt, pMPObj);
      }
      break;
    case DOT1AG_LOOPBACK_MESSAGE_OPCODE:
      dot1agMPLoopBackResponder(dot1agPkt, pMPObj);
      break;

    case DOT1AG_LINKTRACE_REPLY_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPLinkTraceInitiator(dot1agPkt, pMPObj);
      }
      break;

    case DOT1AG_LINKTRACE_MESSAGE_OPCODE:
      dot1agLinkTraceResponder(dot1agPkt, pMPObj);
      break;

    case DOT1AG_LOSSMEASUREMENT_REPLY_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPLMRReceive(dot1agPkt, pMPObj);
      }
      break;
    case DOT1AG_LOSSMEASUREMENT_MESSAGE_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPLMMReceive(dot1agPkt, pMPObj);
      }
      break;
    case DOT1AG_DELAYMEASUREMENT_REPLY_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPDMRReceive(dot1agPkt, pMPObj);
      }
      break;
    case DOT1AG_DELAYMEASUREMENT_MESSAGE_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPDMMReceive(dot1agPkt, pMPObj);
      }
      break;
    case DOT1AG_AIS_CHECK_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPAisReceive(dot1agPkt, pMPObj);
      }
      break;
    case DOT1AG_LCK_CHECK_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPLckReceive(dot1agPkt, pMPObj);
      }
      break;
    case DOT1AG_CSF_CHECK_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPCsfReceive(dot1agPkt, pMPObj);
      }
      break;

    case DOT1AG_SYNTHETIC_LOSSMEASUREMENT_REPLY_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPSLRReceive(dot1agPkt, pMPObj);
      }
      break;
    case DOT1AG_SYNTHETIC_LOSSMEASUREMENT_MESSAGE_OPCODE:
      if (pMPObj->mepData != NULL)
      {
        dot1agMEPSLMReceive(dot1agPkt, pMPObj);
      }
      break;

    default:
      break;
  }
}

/*********************************************************************
* @purpose  Process CCM PDU received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 19.2.8
*
* @end
**********************************************************************/
void dot1agMEPContinuityCheckReceiver(dot1agControlPkt_t *dot1agPkt,
                                      dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;
  OFDPA_BOOL            hwEnabled = OFDPA_FALSE;

  /* Fill SM event msg */
  eventMsg.pMPObj = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    hwEnabled = OFDPA_TRUE;
  }
  else
  {
    hwEnabled = OFDPA_FALSE;
  }

  if (dot1agPkt->cfmHdr.level < pMPObj->key.dot1agCfmStackMdLevel)
  {
    eventMsg.event = dot1agCCRCCMRcvdLow;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
  }
  else if (dot1agPkt->cfmHdr.level == pMPObj->key.dot1agCfmStackMdLevel)
  {
    eventMsg.event = dot1agCCRCCMRcvdEqual;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_VERY_VERBOSE, "Received CCM of level %d by MEP at level %d",
                       dot1agPkt->cfmHdr.level, pMPObj->key.dot1agCfmStackMdLevel);
    return;
  }
  /* Set CCMreceivedLow or CCMreceivedEqual var and generate event */
}

/*********************************************************************
* @purpose  Process Loop Back Response PDU received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 19.2.11, 20.32
* *
* @end
**********************************************************************/
void dot1agMEPLoopBackInitiatorReceive(dot1agControlPkt_t *dot1agPkt,
                                       dot1agCfmStackObj_t *pMPObj)
{
  /* Set LBRreceived = true and generate event */
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.event     = dot1agLBIRLBRRecvd;

  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process Loop Back Message PDU received on an MP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 19.2.11, 20.25
*
* @end
**********************************************************************/
void dot1agMPLoopBackResponder(dot1agControlPkt_t *dot1agPkt,
                               dot1agCfmStackObj_t *pMPObj)
{
  /* Set LBMreceived = true and generate event */
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.event     = dot1agLBRLBMRecvd;

  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process Link Trace Reply PDU received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.40
*
* @end
**********************************************************************/
void dot1agMEPLinkTraceInitiator(dot1agControlPkt_t *dot1agPkt,
                                 dot1agCfmStackObj_t *pMPObj)
{
  /* Set LTRreceived = true and generate event */
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.event     = dot1agLTIRLTRRecvd;

  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process Link Trace Message PDU received on an MP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU. This can be
*                                  NULLPTR when no MP is found on the
*                                  ingress interface. In such a case
*                                  LTM is handed over to Up MEP/MHF
*
* @returns  none
*
* @notes    802.1ag d8-1 20.41
*
* @end
**********************************************************************/
void dot1agLinkTraceResponder(dot1agControlPkt_t *dot1agPkt,
                              dot1agCfmStackObj_t *pMPObj)
{
  uint32_t              intIfNum;
  dot1agCfmSMEventMsg_t eventMsg;

  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.event     = dot1agLTRRLTMRecvd;

  if (pMPObj != NULL)
  {
    /* Fill SM event msg */
    eventMsg.pMPObj = pMPObj;
    if ((pMPObj->mepData != NULL) &&
        (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
    {
      (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
    }
    else
    {
      (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
    }
  }
  else if (dot1agPkt->fromOwnUpMEP == OFDPA_FALSE)
  {
    intIfNum = 0;
    while ((OFDPA_E_NONE == ofdbPortNextGet(intIfNum, &intIfNum)) &&
           (OFDPA_PORT_TYPE_PHYSICAL == OFDB_PORT_TYPE(intIfNum)))
        {
          /* Skip incoming interface */
          if (dot1agPkt->intIfNum == intIfNum)
          {
            continue;
          }

          /* Get MP Configured on the received PDU Interface. */
          if ((pMPObj = dot1agCfmStackObjGet(intIfNum, dot1agPkt->vlanId, dot1agPkt->cfmHdr.level,
                                             DOT1AG_MP_DIRECTION_UP, AVL_EXACT)) != NULL &&
              pMPObj->operMode == OFDPA_TRUE)
          {
            /* Fill SM event msg */
            memcpy(&tmpdot1agPkt, dot1agPkt, sizeof(tmpdot1agPkt));
            eventMsg.pMPObj = pMPObj;
            eventMsg.dot1agPkt = &tmpdot1agPkt;
            if ((pMPObj->mepData != NULL) &&
                (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
            {
              (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
            }
            else
            {
              (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
            }
      } /* Get the MP object from CFM Stack */
    } /* End of member port loop */
  }
  else
  {
    /* LTM is the one generated by UP MEP, find the UP MEP obj and send
       the LTM to corresponding ProcessLTM() */
    if ((pMPObj = dot1agCfmStackObjGet(dot1agPkt->intIfNum, dot1agPkt->vlanId,
                                       dot1agPkt->cfmHdr.level,
                                       DOT1AG_MP_DIRECTION_UP, AVL_EXACT)) != NULL &&
        pMPObj->mepData != NULL &&
        pMPObj->operMode == OFDPA_TRUE)
    {
      eventMsg.pMPObj = pMPObj;
      if ((pMPObj->mepData != NULL) &&
          (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
      {
        (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
      }
      else
      {
        (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
      }
    }
  }
}

/*********************************************************************
* @purpose  Transmit PDU on a given interface
*
* @param    uint32_t          intIfNum  @b((input)) internal interface number
* @param    uint32_t          vlanId    @b((input)) vlan identifier
* @param    dot1agControlPkt_t pduHdl    @b((input)) pointer to DOT1AG PDU type.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agPduTransmit(uint32_t intIfNum, uint16_t vlanId,
                                dot1agControlPkt_t *dot1agPkt)
{
	return 0;
}

/*********************************************************************
* @purpose  Checks CFM PDU received should be forwarded or not
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  OFDPA_TRUE or OFDPA_FALSE
*
* @notes    This API returns true only when the received PDU is not
*           destined to us and the opcode is one of LBM, LBR OR LTR.
*
* @end
**********************************************************************/
OFDPA_BOOL dot1agCfmPduFwdCheck(dot1agControlPkt_t *dot1agPkt,
                                dot1agCfmStackObj_t *pMPObj)
{
  OFDPA_DOT1AG_MP_type_t mpType;

  /* NOT check destMac for MPLS-TP OAM */
  if ((dot1agMpTypeGet(pMPObj, &mpType) != OFDPA_E_NONE) ||
      (mpType != OFDPA_DOT1AG_MP_TYPE_ETH))
  {
    return OFDPA_FALSE;
  }

  if ((dot1agPkt->cfmHdr.opcode == DOT1AG_LINKTRACE_REPLY_OPCODE) ||
      (dot1agPkt->cfmHdr.opcode == DOT1AG_LOOPBACK_MESSAGE_OPCODE) ||
      (dot1agPkt->cfmHdr.opcode == DOT1AG_LOOPBACK_REPLY_OPCODE))
  {
    if (memcmp(dot1agPkt->destMac, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN) != 0)
    {
      return OFDPA_TRUE;
    }
    else
    {
      return OFDPA_FALSE;
    }
  }
  else
  {
    return OFDPA_FALSE;
  }
}

void dot1agSendCcmArray()
{

  return;
}

/*********************************************************************
* @purpose  Process LMM PDU received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
void dot1agMEPLMMReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.priority  = dot1agPkt->priority;
  eventMsg.event     = y1731LMRLMMRecvd;

  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process LMR PDU received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
void dot1agMEPLMRReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.priority  = dot1agPkt->priority;

  if (NULL == pMPObj->mepData)
  {
    return;
  }

  /*
     LSB bit of Flags field in CFM header specifies whether LMM operation is proactive or ondemand.
     Bit 1 is set to 1 if it is the proactive operation, or set to 0 if it is the on-demand operation.
  */
  if ((DOT1AG_CFM_LMM_OPERATION_TYPE_GET(dot1agPkt->cfmHdr.flags) == DOT1AG_CFM_LMM_OPERATION_TYPE_PROACTIVE) &&
      (pMPObj->mepData->y1731CfmMepProlmVar.prolmEnabled) &&
      (pMPObj->mepData->y1731CfmMepProlmVar.prolmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_LM)) /* CCM based LM */
  {
    eventMsg.event     = y1731PROLMLMRRecvd;
  }
  else if ((DOT1AG_CFM_LMM_OPERATION_TYPE_GET(dot1agPkt->cfmHdr.flags) == DOT1AG_CFM_LMM_OPERATION_TYPE_ONDEMAND) &&
           (pMPObj->mepData->y1731CfmMepOndemlmEntry[eventMsg.priority] != NULL) &&
           (pMPObj->mepData->y1731CfmMepOndemlmEntry[eventMsg.priority]->ondemlmEnabled) &&
           (pMPObj->mepData->y1731CfmMepOndemlmEntry[eventMsg.priority]->ondemlmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_LM))
  {
    eventMsg.event     = y1731ONDEMLMMLMRRecvd;
  }
  else
  {
    return;
  }

  if (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process DMM PDU received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
void dot1agMEPDMMReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.event     = y1731DMRDMMRecvd;
  eventMsg.priority  = dot1agPkt->priority;

  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process DMR PDU received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
void dot1agMEPDMRReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;

  if (NULL == pMPObj->mepData)
  {
    return;
  }

  /*
    Process DMR only if operation type in CFM header flags is same as MEP's delay measurement type.
  */

  if (pMPObj->mepData->y1731CfmMepDmEntry[dot1agPkt->priority] != NULL &&
      (((DOT1AG_CFM_DMM_OPERATION_TYPE_GET(dot1agPkt->cfmHdr.flags) == DOT1AG_CFM_DMM_OPERATION_TYPE_PROACTIVE) &&
        (pMPObj->mepData->y1731CfmMepDmEntry[dot1agPkt->priority]->dmType != y1731DmTypeProactive)) ||
       ((DOT1AG_CFM_DMM_OPERATION_TYPE_GET(dot1agPkt->cfmHdr.flags) == DOT1AG_CFM_DMM_OPERATION_TYPE_ONDEMAND) &&
        (pMPObj->mepData->y1731CfmMepDmEntry[dot1agPkt->priority]->dmType != y1731DmTypeOndemand))))
  {
    return;
  }

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.event     = y1731DMMDMRRecvd;
  eventMsg.priority  = dot1agPkt->priority;

  if (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process AIS frame received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
void dot1agMEPAisReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.event     = y1731AisRcvReceived;
  eventMsg.priority  = dot1agPkt->priority;

  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process LCK frame received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
void dot1agMEPLckReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.event     = y1731LckRcvReceived;
  eventMsg.priority  = dot1agPkt->priority;

  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process CSF frame received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
void dot1agMEPCsfReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.event     = y1731CsfRcvReceived;
  eventMsg.priority  = dot1agPkt->priority;

  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process received BHH PDU MPLS headers
*
* @param    dot1agPkt  @b{(input/output)} Local storage for the CFM PDU message
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes    none
*
* @end
**********************************************************************/
OFDPA_ERROR_t mplsTpProcessMplsHeader(dot1agControlPkt_t *dot1agPkt, dot1agCfmStackObj_t **mpObj)
{
  ofdpaOamMepConfig_t  mepConfig;
  ofdpaOamMegConfig_t  megConfig;
  ofdbOamMegInfo_t     megInfo;
  uint8_t             *mpls = dot1agPkt->cfmPduOffset;
  OFDPA_ERROR_t        rc   = OFDPA_E_NOT_FOUND;
  uint32_t             currentMplsLabel, prevMplsLabel;
  uint32_t             associatedCH;
  uint16_t             uShortVal;
  uint8_t              exp;
  uint8_t              bos;
  uint8_t              ttl;
  int                  newLength;

  /* Read MPLS Header */
  mplsTpLabelGet(mpls, &currentMplsLabel, &exp, &bos, &ttl);

  if (0 == ttl)
  {
    /* MIP? */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                       "TTL == 0!", 0);
    return OFDPA_E_FAIL;
  }

  if (!bos)
  {
    do
    {
      mpls += 4;
      prevMplsLabel = currentMplsLabel;
      mplsTpLabelGet(mpls, &currentMplsLabel, &exp, &bos, &ttl);

      if (0 == ttl)
      {
        /* MIP? */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "TTL == 0!", 0);
        return OFDPA_E_FAIL;
      }
    } while ((!bos) && (0 != ttl));

    mpls += 4;
  }
  else                                  /* Only one label and it is BOS. */
  {
    /*
     * If the first and only label is GAL, this must be section OAM.
     */
    if (OFDPA_MPLS_LABEL_GAL == currentMplsLabel)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_TOO_VERBOSE,
                         "Section OAM!", 0);

      mpls += 4;

      associatedCH = ((mpls[0] << 24) |
                      (mpls[1] << 16) |
                      (mpls[2] << 8)  |
                      (mpls[3]));

      /* Ensure that G-ACH is present. */
      if (G_ACH != associatedCH)
      {
        DOT1AG_TRACE(CFM_PDU, "CFM G-ACH missing from section\r\n");
        return OFDPA_E_FAIL;
      }

      /* Skip over G-ACH */
      mpls += 4;

      dot1agPkt->cfmPduOffset = mpls;

      /*
       * Get the relevant section lookup information here, can't use label
       */

      /* If section OAM PDU copied to CPU by IFP rule, then a valid lmepId will be
       * present.  However, the megIndex may not be valid. */
      if (0 != dot1agPkt->lmepId)
      {
        if (OFDPA_E_NONE != ofdbOamMepGet(dot1agPkt->lmepId, &mepConfig, NULL))
        {
          DOT1AG_TRACE(CFM_PDU, "Cannot retrieve section MEP.\r\n");
          return OFDPA_E_FAIL;
        }

        if (OFDPA_E_NONE != ofdbOamMegGet(mepConfig.megIndex, &megConfig, NULL, &megInfo))
        {
          DOT1AG_TRACE(CFM_PDU, "Cannot retrieve section MEG info.\r\n");
          return OFDPA_E_FAIL;
        }

        /* megIndex could not be set in the driver for section OAM so set it here. */
        dot1agPkt->megIndex = mepConfig.megIndex;
      }
      else                              /* megIndex is valid. */
      {
        if (OFDPA_E_NONE != ofdbOamMegGet(dot1agPkt->megIndex, &megConfig, NULL, &megInfo))
        {
          DOT1AG_TRACE(CFM_PDU, "Cannot retrieve section MEG info.\r\n");
          return OFDPA_E_FAIL;
        }
      }

      *mpObj = dot1agCfmStackObjGet(dot1agPkt->intIfNum, megInfo.primVid, megConfig.level,
                                    DOT1AG_MP_DIRECTION_DOWN, AVL_EXACT);
      if (NULL != *mpObj)
      {
        rc = OFDPA_E_NONE;
      }

      return rc;
    }

    prevMplsLabel = currentMplsLabel;
  }

  associatedCH = ((mpls[0] << 24) |
                  (mpls[1] << 16) |
                  (mpls[2] << 8)  |
                  (mpls[3]));

  /*
   * The following OAM packets do not have a GAL label:
   * PW VCCV 1
   * PW VCCV 3
   * Ethernet OAM over MPLS-TP
   */
  if (currentMplsLabel != OFDPA_MPLS_LABEL_GAL)
  {
    prevMplsLabel = currentMplsLabel;

    /* If no G-ACH, then assume Ethernet OAM (Up MEP) over MPLS-TP */
    if (G_ACH != associatedCH)
    {
      /* CW is next, skip over it */
      if (dot1agPkt->cwPresent)
      {
        mpls += 4;
      }

      /* Move the encapsulated Ethernet frame to the beginning of the payload */
      newLength = dot1agPkt->length - (mpls - dot1agPkt->payLoad);
      memmove(dot1agPkt->payLoad, mpls, newLength);
      dot1agPkt->length = newLength;
      mpls = dot1agPkt->payLoad;

      /* Copy the Destination mac of the PDU */
      memcpy(dot1agPkt->destMac, mpls, OFDPA_MAC_ADDR_LEN);
      mpls += OFDPA_MAC_ADDR_LEN;

      /* Copy the Source mac of the PDU */
      memcpy(dot1agPkt->srcMac, mpls, OFDPA_MAC_ADDR_LEN);
      mpls += OFDPA_MAC_ADDR_LEN;

      /* check if src mac is a group mac 802.1ag d8-2 21.3.2*/
      if (dot1agPkt->srcMac[0] & 0x01)
      {
        DOT1AG_TRACE(CFM_PDU, "Invalid src mac\r\n");
        return OFDPA_E_FAIL;
      }

      /* Need to use inner VLAN for MEG identification -- VLAN tag must be present */
      DOT1AG_GET_SHORT(uShortVal, mpls);
      if (OFDPA_DOT1AG_8021Q_ETHER_TYPE != uShortVal)
      {
        DOT1AG_TRACE(CFM_PDU, "VLAN tag missing\r\n");
        return OFDPA_E_FAIL;
      }

      DOT1AG_GET_SHORT(uShortVal, mpls);
      dot1agPkt->vlanId = (uShortVal & 0x0FFF);
      dot1agPkt->priority = ((uShortVal >> 13) & 0x7);

      /* Skip etherType (0x8902)*/
      DOT1AG_GET_SHORT(uShortVal, mpls);
      if (OFDPA_DOT1AG_CFM_ETHER_TYPE != uShortVal)
      {
        DOT1AG_TRACE(CFM_PDU, "CFM Ethertype missing\r\n");
        return OFDPA_E_FAIL;
      }

      dot1agPkt->cfmPduOffset = mpls;

      return OFDPA_E_NONE;
    }
  }

  /* Must be a BHH OAM packet, ensure that G-ACH is present. */
  if (G_ACH != associatedCH)
  {
    DOT1AG_TRACE(CFM_PDU, "CFM G-ACH missing\r\n");
    return OFDPA_E_FAIL;
  }

  mpls += 4;

  /* CFM PDU begins immediately after G-ACH. */
  dot1agPkt->cfmPduOffset = mpls;

  /* Find the LMEP ID for the CFM session using label lookups */
  rc = dot1agBhhLabelMapMpGet(prevMplsLabel, mpObj);

  if(OFDPA_E_NONE == rc)
  {
  	dot1agPkt->vlanId = (*mpObj)->key.dot1agCfmStackVlanIdOrNone;
  }	
  else
  {
	  DOT1AG_TRACE(CFM_PDU, "dot1agBhhLabelMapMpGet fail\r\n");
  }
  
  return rc;
}

/*********************************************************************
* @purpose  Process received BHH PDU MPLS headers
*
* @param    data    @b{(input)} Handle to the BHH PDU MPLS Header
* @param    offset  @b{(output)} offset to CFM PDU
*
* @returns  length of data added
*
* @notes    none
*
* @end
**********************************************************************/
uint32_t mplsTpPrepareMplsHeader(ofdbOamLmepData_t *oamMapData, uint8_t priority, uint8_t *data, uint8_t ttl)
{
  uint8_t   buf[MPLS_HDR_LEN_MAX];
  uint8_t   mpMacAddr[OFDPA_MAC_ADDR_LEN];
  uint16_t  shortVal;
  uint8_t  *bufPtr = &buf[0];
  int       i;
  uint32_t  cw = 0;

  memset(mpMacAddr, 0, OFDPA_MAC_ADDR_LEN);

  /*setting Dst MAC addr*/
  DOT1AG_PUT_DATA(oamMapData->destMac.addr, OFDPA_MAC_ADDR_LEN, bufPtr);

  /*setting Src MAC addr*/
  DOT1AG_PUT_DATA(oamMapData->srcMac.addr, OFDPA_MAC_ADDR_LEN, bufPtr);

  /*setting VLAN Tag*/
  if (oamMapData->vlanId)
  {
    shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
    DOT1AG_PUT_SHORT(shortVal, bufPtr);
    /* FIXME: set pcp equals to priority, final MPLS Label Remark Action Tables should be used */
    shortVal  = ((uint16_t)priority) << 13;
    shortVal |= ((uint16_t)oamMapData->vlanId) & 0x0FFF;
    DOT1AG_PUT_SHORT(shortVal, bufPtr);
  }

  /*setting MPLS eth type*/
  shortVal = OFDPA_DOT1AG_MPLS_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, bufPtr);

  /* Set up MPLS label stack */
  for (i = (oamMapData->numLabels - 1); i >= 0; i--)
  {
    DOT1AG_PUT_DATA(&oamMapData->mplsHeader[i], 4, bufPtr);
    /* FIXME: set exp equals to priority, final MPLS Label Remark Action Tables should be used */
    *(bufPtr - 2) = (*(bufPtr - 2) & 0xf1) + ((priority << 1) & 0xe);
    if (ttl != 0)
    {
      /* Update TTL in MPLS header*/
      *(bufPtr - 1) = ttl;
    }
  }

  /* If OAM PDU, add G-ACH.
   * Channel type is added by the caller of this function.
   *
   * TODO: Need to add different check for OAM PDU passed in as an argument.
   * BHH OAM PDUs will always have a G-ACH.  If this is for a PW label, the
   * data on that PW must also carry a CW.  That is, for a given label, if a
   * G-ACH can be present (for OAM), then a CW must be present for all
   * other traffic (a G-ACH is a type of CW). */
  if (0 != oamMapData->mplsDataFirstNibble)
  {
    shortVal = 0x1000;
    DOT1AG_PUT_SHORT(shortVal, bufPtr);
  }
  else if (0 != oamMapData->pushCW)
  {
    /* Must be data, add Control Word */
    DOT1AG_PUT_DATA(&cw, 4, bufPtr);
  }


  memcpy(data, buf, (bufPtr - &buf[0]));

  return (bufPtr - &buf[0]);
}

/*********************************************************************
* @purpose  Prepare an MPLS Label header
*
* @param    label   @b{(input)} mpls label
* @param    exp     @b{(input)} EXP
* @param    bos     @b{(input)} Bottom of Stack
* @param    ttl     @b{(input)} TTL
* @param    buffer  @b{(output)} MPLS Label Header
*
* @returns  void
*
* @comments none
*
* @end
*********************************************************************/
void mplsTpLabelPrepare(uint32_t label, uint8_t exp, uint8_t bos,
                        uint8_t ttl, uint8_t *buffer)
{
/*
  Label on MPLS Header = 0xAB 0xCD 0xE0
  label value =  0x000ABCDE

  Big Endian label =  0x000ABCDE
  Little Endian label =  0xDEBC0A00
*/
  uint32_t header = 0;

  header |= (label << 12);
  header |= (((uint32_t)exp & 0x00000007L) << 9);
  header |= (((uint32_t)bos & 0x00000001L) << 8);
  header |= ((uint32_t)ttl & 0x000000FFL);

  header = htonl(header);
  memcpy(buffer, &header, sizeof(header));
}

/*********************************************************************
* @purpose  Prepare an MPLS Label header
*
* @param    buf  @b{(input)} Buffer
* @param    label   @b{(output)} mpls label
* @param    exp     @b{(output)} EXP
* @param    bos     @b{(output)} Bottom of Stack
* @param    ttl     @b{(output)} TTL
*
* @returns  void
*
* @comments none
*
* @end
*********************************************************************/
void mplsTpLabelGet(uint8_t *buf, uint32_t *label, uint8_t *exp,
                    uint8_t *bos, uint8_t *ttl)
{
/*
  Label on MPLS Header = 0xAB 0xCD 0xE0
  label value =  0x000ABCDE

  Big Endian label =  0x000ABCDE
  Little Endian label =  0xDEBC0A00
*/
//  uint8_t buffer[4];
  uint32_t header = 0;

  header = ntohl(*((uint32_t *)buf));

  *label = header >> 12;
  *exp = (header >> 9) & 0x00000007L;
  *bos = (header >> 8) & 0x00000001L;
  *ttl = header & 0x000000FFL;

//  memset(buffer, 0, sizeof(buffer));
//  mplsTpLabelPrepare(*label, *exp, *bos, *ttl, buffer);
}

/*********************************************************************
* @purpose  Process SLR PDU received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
void dot1agMEPSLRReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.priority  = dot1agPkt->priority;

  if (NULL == pMPObj->mepData)
  {
    return;
  }

  if ((pMPObj->mepData->y1731CfmMepProlmVar.prolmEnabled) &&
      (pMPObj->mepData->y1731CfmMepProlmVar.prolmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM)) /* SLM based LM */
  {
    eventMsg.event     = y1731PROLMSLRRecvd;
  }
  else if ((pMPObj->mepData->y1731CfmMepOndemlmEntry[eventMsg.priority] != NULL) &&
           (pMPObj->mepData->y1731CfmMepOndemlmEntry[eventMsg.priority]->ondemlmEnabled) &&
           (pMPObj->mepData->y1731CfmMepOndemlmEntry[eventMsg.priority]->ondemlmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_SLM)) /* SLM based LM */
  {
    eventMsg.event     = y1731ONDEMLMMSLRRecvd;
  }
  else
  {
    return;
  }

  if (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}

/*********************************************************************
* @purpose  Process SLM PDU received on an MEP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                     PDU message
* @param    pMPObj        @b{(input)} CMF Stack MP being used for
*                                     processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
void dot1agMEPSLMReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmSMEventMsg_t eventMsg;

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;
  eventMsg.priority  = dot1agPkt->priority;
  eventMsg.event     = y1731LMRSLMRecvd;

  if ((pMPObj->mepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }
}
