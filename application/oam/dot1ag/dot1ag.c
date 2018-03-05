/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2001-2015
*
**********************************************************************
*
* @filename   dot1ag.c
*
* @purpose    Contains definitions of methods used by SMs
*             defined in 802.1ag d8-1
*
* @component  DOT1AG/Connectivity & Fault Management
*
* @comments   none
*
* @create     02-Mar-2009
*
* @end
*
**********************************************************************/

#include <stdlib.h>
#include "ofdpa_porting.h"
//#include "driver_oam.h"
//#include "driver_flow.h"
#include "ofdb_api.h"
#include "dot1ag.h"
#include "dot1ag_api.h"
#include "dot1ag_db.h"
#include "dot1ag_ctrl.h"
#include "dot1ag_pdu.h"
#include "dot1ag_db.h"
#include "dot1ag_sm.h"
#include "dot1ag_procs.h"
#include "dot1ag_outcalls.h"
#include "dot1ag_debug.h"
#include "dot1ag_tlv.h"
#include "msgq.h"

static uint8_t dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN] = {0x01, 0x80, 0xC2, 0x00, 0x00, 0x30};
static dot1agControlPkt_t dot1agPktFromCpu;

void dumpPkt(const char *func, uint16_t line, uint16_t length, uint8_t  *data)
{
  uint32_t i;
  if (!(debugTraceFlags & (CFM_PDU)))
  {
    return;
  }
  printf("\n%s, %d ======== len is %d:", func, line, length);
  for (i = 0; i < length; i++)
  {
    if (i % 16 == 0)
    {
      printf("\n");
    }
    printf("%02x ", data[i]);
  }
  printf("\n ======== \n");
  return;
}

/*********************************************************************
* @purpose  Obtain MEP type
*
* @param    pMEPObj    @b{(input)} MEP on which LBM has to be sent

* @returns  OFDPA_E_FAIL
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes Use
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agMpTypeGet(dot1agCfmStackObj_t *pMPObj,
                              OFDPA_DOT1AG_MP_type_t *mpType)
{
  OFDPA_ERROR_t        rc = OFDPA_E_NONE;
  uint32_t             primVID;
  OFDPA_OAM_MEG_TYPE_t megType;

  /* Validate input */
  if (mpType == NULL || pMPObj == NULL)
  {
    return OFDPA_E_ERROR;
  }

  primVID =pMPObj->key.dot1agCfmStackVlanIdOrNone;
  megType = ofdbMegTypeFromVidGet(primVID);

  switch (megType)
  {
    case OFDPA_OAM_MEG_TYPE_ETHERNET:
      *mpType = OFDPA_DOT1AG_MP_TYPE_ETH;
      break;
    case OFDPA_OAM_MEG_TYPE_G8113_1:
      *mpType = OFDPA_DOT1AG_MP_TYPE_BHH;
      break;
    default:
      rc = OFDPA_E_PARAM;
      break;
  }

  return rc;
}

/* Util APIs */
/*********************************************************************
* @purpose  Copy common cfm hdr data to a packet buffer
*
* @param    cfmHdr   @b{(input)} CFM common hdr value to be written
* @param    data     @b{(input)} Ptr to data
*
* @returns  none
*
* @notes    802.1ag d8-1 20.29.1
*
* @end
**********************************************************************/
void dot1agCfmHdrWrite(dot1agCfmCommonHdr_t *cfmHdr, uint8_t *data)
{
  uint8_t  byteVal = 0;
  uint8_t *pData;

  pData = data;

  byteVal |= cfmHdr->level << 5;
  byteVal |= (cfmHdr->version & 0x1F);
  DOT1AG_PUT_BYTE(byteVal, pData);
  DOT1AG_PUT_BYTE(cfmHdr->opcode, pData);
  DOT1AG_PUT_BYTE(cfmHdr->flags, pData);
  DOT1AG_PUT_BYTE(cfmHdr->first_tlv_offset, pData);
}

/* Get MAID of the MP */
/*********************************************************************
* @purpose  Get Maintenance Association Identifier of receiving MP
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    maid       @b{(output)} MAID of the MP
*
* @returns  none
*
* @notes    802.1ag d8-1 21.6.5
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMPMAIDGet(dot1agCfmStackObj_t *pMPObj, uint8_t *maid)
{
  dot1agCfmMaNetTable_t  *maEntry;

  if (pMPObj != NULL &&
      pMPObj->mepData != NULL &&
      pMPObj->mepData->maData != NULL)
  {
    maEntry = pMPObj->mepData->maData;
    memcpy(maid, maEntry->dot1agCfmMaid, sizeof(maEntry->dot1agCfmMaid));
  }
  else
  {
    return OFDPA_E_ERROR;
  }

  return OFDPA_E_NONE;
}

/* Get MAID of the MP */
/*********************************************************************
* @purpose  Get Maintenance Association Identifier of receiving MP
*
* @param    mdEntry    @b{(input)} md entry
* @param    maEntry    @b{(input)} ma entry
* @param    maid       @b{(output)} MAID of the MP
*
* @returns  none
*
* @notes    802.1ag d8-1 21.6.5
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agMAIDGet(dot1agCfmMdTable_t *mdEntry,
                            dot1agCfmMaNetTable_t *maEntry,
                            uint8_t *maid)
{
  memcpy(maid, maEntry->dot1agCfmMaid, sizeof(maEntry->dot1agCfmMaid));

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Parse CCM Pdu and store int in local CPM PDU message
*           storage
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 21.6
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agParseCCM(dot1agControlPkt_t *dot1agPkt)
{
  uint8_t  *data, type;
  uint16_t  length;

  /* Point to CFM PDU */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;

  /* 21.6.2 */
  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_CCM_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }

  dot1agPkt->cfmPdu.ccm.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;
  dot1agPkt->cfmPdu.ccm.rdi         = DOT1AG_CCM_FLAGS_RDI_GET(dot1agPkt->cfmHdr.flags);
  dot1agPkt->cfmPdu.ccm.ccmInterval = DOT1AG_CCM_FLAGS_CCMI_GET(dot1agPkt->cfmHdr.flags);


  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.ccm.seqNumber, data);

  /* 21.6.4 */
  DOT1AG_GET_SHORT(dot1agPkt->cfmPdu.ccm.mepID, data);
  if ((dot1agPkt->cfmPdu.ccm.mepID < OFDPA_DOT1AG_MEPID_MIN) ||
      (dot1agPkt->cfmPdu.ccm.mepID > OFDPA_DOT1AG_MEPID_MAX))
  {
    return OFDPA_E_FAIL;
  }

  DOT1AG_GET_DATA(dot1agPkt->cfmPdu.ccm.maID, DOT1AG_MAID_MAX_LEN, data);

  /* Parse PROLM info */
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.ccm.txFCf, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.ccm.rxFCb, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.ccm.txFCb, data);

  /* Parse available TLVs */
  data = dot1agPkt->cfmPdu.ccm.pFirstTLV;
  dot1agPkt->cfmPdu.ccm.portStatusTLV.portStatus           = DOT1AG_RMEP_PORT_STATE_psNoPortStateTLV;
  dot1agPkt->cfmPdu.ccm.interfaceStatusTLV.interfaceStatus = DOT1AG_RMEP_INTERFACE_STATUS_isNoInterfaceStatusTLV;

  DOT1AG_GET_BYTE(type, data);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);
    switch (type)
    {
      case DOT1AG_TLV_TYPE_PORT_STATUS:
        dot1agPkt->cfmPdu.ccm.portStatusTLV.length = length;
        if (dot1agCfmPortStatusTlvRead(&dot1agPkt->cfmPdu.ccm.portStatusTLV,
                                       data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        dot1agPkt->cfmPdu.ccm.portStatusTLVRead = OFDPA_TRUE;
        break;
      case DOT1AG_TLV_TYPE_INTERFACE_STATUS:
        dot1agPkt->cfmPdu.ccm.interfaceStatusTLV.length = length;
        if (dot1agCfmInterfaceStatusTlvRead(&dot1agPkt->cfmPdu.ccm.interfaceStatusTLV,
                                            data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        dot1agPkt->cfmPdu.ccm.interfaceStatusTLVRead = OFDPA_TRUE;
        break;
      case DOT1AG_TLV_TYPE_SENDER_ID:
        dot1agPkt->cfmPdu.ccm.senderIdTLV.length = length;
        if (dot1agCfmSenderIdTlvRead(&dot1agPkt->cfmPdu.ccm.senderIdTLV,
                                     data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        dot1agPkt->cfmPdu.ccm.senderIdTLVRead = OFDPA_TRUE;
        break;
        /* Ignore all other TLVs */
      case DOT1AG_TLV_TYPE_DATA:
        break;
        /* RX_COUNTER TLV */
      case DOT1AG_TLV_TYPE_BRCM_RX_COUNTER:
        dot1agPkt->cfmPdu.ccm.rxCtrTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.ccm.pRxCtrTLV = data - 3;
        DOT1AG_GET_LONG(dot1agPkt->cfmPdu.ccm.rxFCl, data); /* set by HW */
        break;

      default:
        break;
    }
    data += length;
    DOT1AG_GET_BYTE(type, data);
  }

  /* Parse internal RX_COUNTER TLVs */
  data = dot1agPkt->payLoad + dot1agPkt->length - 7;
  DOT1AG_GET_BYTE(type, data);
  if (type == DOT1AG_TLV_TYPE_BRCM_RX_COUNTER)
  {
    DOT1AG_GET_SHORT(length, data);
    if (length == DOT1AG_TLV_LEN_BRCM_RX_COUNTER)
    {
      dot1agPkt->cfmPdu.ccm.rxCtrTLVRead = OFDPA_TRUE;
      dot1agPkt->cfmPdu.ccm.pRxCtrTLV = data - 3;
      DOT1AG_GET_LONG(dot1agPkt->cfmPdu.ccm.rxFCl, data); /* set by HW */
    }
  }

  /* Deletes the RX COUNTER TLV, if present in the LMM */
  if (dot1agPkt->cfmPdu.ccm.rxCtrTLVRead)
  {
    uint32_t bytesToMove;
    bytesToMove = dot1agPkt->length -
      (dot1agPkt->cfmPdu.ccm.pRxCtrTLV - dot1agPkt->payLoad) -
      (DOT1AG_TLV_LEN_BRCM_RX_COUNTER + 3);
    memcpy(dot1agPkt->cfmPdu.ccm.pRxCtrTLV,
           dot1agPkt->cfmPdu.ccm.pRxCtrTLV + DOT1AG_TLV_LEN_BRCM_RX_COUNTER + 3,
           bytesToMove);
    dot1agPkt->length -= DOT1AG_TLV_LEN_BRCM_RX_COUNTER + 3;

    dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  }
#ifdef Y1731_PM_SIM_DROP
  static uint32_t rxFCbDrop = 0;
  static uint32_t rxFClDrop = 0;

  rxFCbDrop += (rand() % 10);
  if (dot1agPkt->cfmPdu.ccm.rxFCb < rxFCbDrop)
  {
    rxFCbDrop = 0;
  }
  dot1agPkt->cfmPdu.ccm.rxFCb -= rxFCbDrop;

  rxFClDrop += (rand() % 10);
  if (dot1agPkt->cfmPdu.ccm.rxFCl < rxFClDrop)
  {
    rxFClDrop = 0;
  }
  dot1agPkt->cfmPdu.ccm.rxFCl -= rxFClDrop;
#endif

  return OFDPA_E_NONE;
}

/* MEP CCR Methods 20.17.1 */
/*********************************************************************
* @purpose  Method called by the CCR SM to process the received CCM
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.17.1
*
* @end
**********************************************************************/
OFDPA_ERROR_t MEPprocessEqualCCM(dot1agControlPkt_t *dot1agPkt,
                                 dot1agCfmStackObj_t *pMPObj)
{
  uint8_t                maID[DOT1AG_MAID_MAX_LEN];
  dot1agCfmSMEventMsg_t  eventMsg;
  dot1agCfmRMepObj_t     rmepObj;
  dot1agCfmRMepObj_t    *pRMEPObj  = NULL;
  uint32_t               mdIndex, maIndex, mepId;
  OFDPA_BOOL             hwEnabled = OFDPA_FALSE;
  OFDPA_DOT1AG_MP_type_t mpType;

  if ((NULL == pMPObj) || (NULL == pMPObj->mepData))
  {
    return OFDPA_E_FAIL;
  }

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;

  if (dot1agMPMAIDGet(pMPObj, maID) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  /* Parse CCM PDU */
  if (dot1agParseCCM(dot1agPkt) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  if (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier))
  {
    hwEnabled = OFDPA_TRUE;
  }

  if (dot1agMpTypeGet(pMPObj, &mpType) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  /* Check if incoming MAID is same as MPs MAID */
  if (memcmp(maID, dot1agPkt->cfmPdu.ccm.maID, DOT1AG_MAID_MAX_LEN) != 0)
  {
    /* Set Y1731_DEFECT_TYPE_MMG to true and generate an DMM SM event */
    if (pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_MMG].y1731DefCCMTimer != NULL)
    {
      eventMsg.event = y1731dMMGCCMRecvdWhileNotZero;
    }
    else
    {
      eventMsg.event = y1731dMMGCCMRecvd;
    }

    if ((NULL != pMPObj->mepData->maData) &&
        (NULL != pMPObj->mepData->maData->mdData))
    {
      DOT1AG_TRACE(CFM_CCM, "Received MAID: \"%s\" is not equal to MEP's "
                            "(mepId: %d, maIndex: %d mdIndex: %d) MAID \"%s\"\r\n",
                   dot1agPkt->cfmPdu.ccm.maID,
                   pMPObj->mepData->dot1agCfmMepIdentifier,
                   pMPObj->mepData->maData->dot1agCfmMaIndex,
                   pMPObj->mepData->maData->mdData->dot1agCfmMdIndex, maID);
    }
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    /* Set xconCcmReceived to true and generate an xCon SM event */
    if (pMPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer != NULL)
    {
      eventMsg.event = dot1agXConCCMRecvdWhileNotZero;
    }
    else
    {
      eventMsg.event = dot1agXConCCMRecvd;
    }

    DOT1AG_TRACE(CFM_CCM, "Received MAID: \"%s\" is not equal to MEP's "
                          "(mepId: %d, maIndex: %d mdIndex: %d) MAID \"%s\"\r\n",
                 dot1agPkt->cfmPdu.ccm.maID,
                 pMPObj->mepData->dot1agCfmMepIdentifier,
                 pMPObj->mepData->maData->dot1agCfmMaIndex,
                 pMPObj->mepData->maData->mdData->dot1agCfmMdIndex, maID);

    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    return OFDPA_E_NONE;
  }

  /* Check if incoming MEPID is configured in this MA  (Ignoring this condition for now )
     or Check if incoming MEPID is same as receiving MEP
     or Check if CCMInterval is not same as receiving MEP CCMInterval */
  if (pMPObj->mepData->dot1agCfmMepIdentifier == dot1agPkt->cfmPdu.ccm.mepID ||
      pMPObj->mepData->maData->dot1agCfmMaNetCcmInterval != dot1agPkt->cfmPdu.ccm.ccmInterval)
  {
    if (pMPObj->mepData->maData->dot1agCfmMaNetCcmInterval != dot1agPkt->cfmPdu.ccm.ccmInterval)
    {
      if (pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNP].y1731DefCCMTimer != NULL)
      {
        eventMsg.event = y1731dUNPCCMRecvdWhileNotZero;
      }
      else
      {
        eventMsg.event = y1731dUNPCCMRecvd;
      }
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
    }

    if (pMPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer != NULL)
    {
      eventMsg.event = dot1agRmepErrCCMRecvdWhileNotZero;
    }
    else
    {
      eventMsg.event = dot1agRmepErrCCMRecvd;
    }
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

    return OFDPA_E_NONE;
  }
  /* Check if any RMEP Obj exists for the received CCM MEP,
     if not create one */
  if (pMPObj->mepData->maData != NULL &&
      pMPObj->mepData->maData->mdData != NULL)
  {
    mdIndex = pMPObj->mepData->maData->mdData->dot1agCfmMdIndex;
  }
  else
  {
    mdIndex = 0;
  }

  maIndex = pMPObj->mepData->maData->dot1agCfmMaIndex;
  mepId   = pMPObj->mepData->dot1agCfmMepIdentifier;

  if ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                      dot1agPkt->cfmPdu.ccm.mepID, AVL_EXACT)) == NULL)
  {
    if (OFDPA_DOT1AG_MP_TYPE_BHH == mpType)
    {
      if (pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNM].y1731DefCCMTimer != NULL)
      {
        eventMsg.event = y1731dUNMCCMRecvdWhileNotZero;
      }
      else
      {
        eventMsg.event = y1731dUNMCCMRecvd;
      }
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);

      if (pMPObj->mepData->dot1agCfmRmepErrVar.errorCCMTimer != NULL)
      {
        eventMsg.event = dot1agRmepErrCCMRecvdWhileNotZero;
      }
      else
      {
        eventMsg.event = dot1agRmepErrCCMRecvd;
      }
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
      return OFDPA_E_NONE;
    }
    else
    {
      /* Add a new RMEP entry */
      memset(&rmepObj, 0x0, sizeof(rmepObj));
      rmepObj.key.dot1agCfmMdIndex             = mdIndex;
      rmepObj.key.dot1agCfmMaIndex             = maIndex;
      rmepObj.key.dot1agCfmMepIdentifier       = mepId;
      rmepObj.key.dot1agCfmMepDbRMepIdentifier = dot1agPkt->cfmPdu.ccm.mepID;
      rmepObj.dot1agCfmRmepState               = DOT1AG_RMEP_IDLE;
      if (dot1agDBTreeEntryAdd(DOT1AG_DB_ID_RMEP_TREE, &dot1agCFM.dot1agRMepTree,
                               &rmepObj) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to create RMEP for mdIndex %d maIndex %d mepId %d rmepId %d",
                           mdIndex, maIndex, mepId, dot1agPkt->cfmPdu.ccm.mepID);
        return OFDPA_E_FAIL;
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Create one  RMEP for mdIndex %d maIndex %d mepId %d rmepId %d",
                           mdIndex, maIndex, mepId, dot1agPkt->cfmPdu.ccm.mepID);
      }

      if ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                          dot1agPkt->cfmPdu.ccm.mepID, AVL_EXACT)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to get RMEP for mdIndex %d maIndex %d mepId %d rmepId %d",
                           mdIndex, maIndex, mepId, dot1agPkt->cfmPdu.ccm.mepID);
        return OFDPA_E_FAIL;
      }

      /* Init the SM */
      eventMsg.event = dot1agRmepBegin;
      eventMsg.pRMEPObj = pRMEPObj;
      eventMsg.pMepData = pMPObj->mepData;
      (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
    }
  }

  eventMsg.pRMEPObj = pRMEPObj;
  eventMsg.pMepData = pMPObj->mepData;
  /* Pass rMEPCCMReceived event to RMEP SM */
  if (pRMEPObj->dot1agCfmRmepVar.rmepTimer != NULL)
  {
    eventMsg.event = dot1agRmeprCCMRecvdWhileNotZero;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
  }
  else
  {
    eventMsg.event = dot1agRmeprCCMRecvd;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
  }

  /* Pass y1731PROLMCCMRecvd to PROLM SM: could be ahead of RMEP handling? */
  if ((pMPObj->mepData->y1731CfmMepProlmVar.prolmEnabled) &&
      (pMPObj->mepData->y1731CfmMepProlmVar.prolmToolType == DOT1AG_OAM_LMM_TOOL_TYPE_CCM)) /* CCM based LM */
  {
    /* get RxFCI from counter module*/
    /*
      pMPObj->mepData->y1731CfmMepProlmEntry->rxFClPrev = pMPObj->mepData->y1731CfmMepProlmVar.rxFCl;
      pMPObj->mepData->y1731CfmMepProlmEntry->rxFCl = 0xFFFFFFFF;
    */

    /* goto PROLM SM*/
    eventMsg.event = y1731PROLMCCMRecvd;
    (void)dot1agCfmStateMachineClassifier(&eventMsg, hwEnabled);
  }

  /* Compare sequence number in received CCM to value in MEP CCM database */
  if (pRMEPObj->dot1agCfmMepDbseqNumber && dot1agPkt->cfmPdu.ccm.seqNumber)
  {
    /* Compare the Sequence Number field in the received CCM to the value saved in the MEP CCM
       Database. If both values are not 0, and if the new value is not 1 greater than the last, increment
       CCMsequenceErrors */
    if (dot1agPkt->cfmPdu.ccm.seqNumber != (pRMEPObj->dot1agCfmMepDbseqNumber + 1))
    {
      pMPObj->mepData->dot1agCfmMepCcmSequenceErrors++;
    }
  }
  /* Store the received Sequence Number in the MEP CCM Database */
  pRMEPObj->dot1agCfmMepDbseqNumber = dot1agPkt->cfmPdu.ccm.seqNumber;

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Method called by the CCR SM to process the received CCM
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.17.2
*
* @end
**********************************************************************/
OFDPA_ERROR_t MEPprocessLowCCM(dot1agControlPkt_t *dot1agPkt,
                               dot1agCfmStackObj_t *pMPObj)
{
  uint8_t               maID[DOT1AG_MAID_MAX_LEN];
  dot1agCfmSMEventMsg_t eventMsg;

  if ((NULL == pMPObj) || (NULL == pMPObj->mepData))
  {
    return OFDPA_E_FAIL;
  }

  /* Fill SM event msg */
  eventMsg.pMPObj    = pMPObj;
  eventMsg.dot1agPkt = dot1agPkt;

  /* Parse CCM PDU */
  if (dot1agParseCCM(dot1agPkt) != OFDPA_E_NONE)
  {
    return OFDPA_E_NONE;
  }

  if (dot1agMPMAIDGet(pMPObj, maID) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  /* Set xconCcmReceived to true and generate an xCon SM event */
  if (pMPObj->mepData->y1731DefectVar[Y1731_DEFECT_TYPE_UNL].y1731DefCCMTimer != NULL)
  {
    eventMsg.event = y1731dUNLCCMRecvdWhileNotZero;
  }
  else
  {
    eventMsg.event = y1731dUNLCCMRecvd;
  }

  if ((NULL != pMPObj->mepData->maData) &&
      (NULL != pMPObj->mepData->maData->mdData))
  {
    DOT1AG_TRACE(CFM_CCM, "Received MAID: \"%s\" is lower than MEP's "
                          "(mepId: %d, maIndex: %d mdIndex: %d) MAID \"%s\"\r\n",
                 dot1agPkt->cfmPdu.ccm.maID,
                 pMPObj->mepData->dot1agCfmMepIdentifier,
                 pMPObj->mepData->maData->dot1agCfmMaIndex,
                 pMPObj->mepData->maData->mdData->dot1agCfmMdIndex, maID);
  }

  if (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }

  /* Set xconCcmReceived to true and generate an xCon SM event */
  if (pMPObj->mepData->dot1agCfmMepXConVar.xconCCMTimer != NULL)
  {
    eventMsg.event = dot1agXConCCMRecvdWhileNotZero;
  }
  else
  {
    eventMsg.event = dot1agXConCCMRecvd;
  }

  if ((NULL != pMPObj->mepData->maData) &&
      (NULL != pMPObj->mepData->maData->mdData))
  {
    DOT1AG_TRACE(CFM_CCM, "Received MAID: \"%s\" is lower than MEP's "
                          "(mepId: %d, maIndex: %d mdIndex: %d) MAID \"%s\"\r\n",
                 dot1agPkt->cfmPdu.ccm.maID,
                 pMPObj->mepData->dot1agCfmMepIdentifier,
                 pMPObj->mepData->maData->dot1agCfmMaIndex,
                 pMPObj->mepData->maData->mdData->dot1agCfmMdIndex, maID);
  }

  if (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMPObj->mepData->dot1agCfmMepIdentifier))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }

  return OFDPA_E_NONE;
}

/* LBIR Methods */
/*********************************************************************
* @purpose  Method called by the LBIR SM to process the received LBR
*           message
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.31.1
*
* @end
**********************************************************************/
OFDPA_ERROR_t ProcessLBR(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  uint8_t                *data;
  OFDPA_DOT1AG_MP_type_t mpType = OFDPA_DOT1AG_MP_TYPE_ETH;

  if (pMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* get MP type */
  if (dot1agMpTypeGet(pMPObj, &mpType) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  /* Validate destination mac address */

  DOT1AG_TRACE(CFM_LBR, "Start Processing LBR received on interface: %d\r\n", dot1agPkt->intIfNum);

  /* If the I/G bit of the source_address indicates a Group address, or if
     the destination_address does not match the MAC address of the receiving
     MP, ProcessLBR() discards the received LBR. */

  if ((dot1agPkt->destMac[0] & 0x1) ||
      (memcmp(dot1agPkt->destMac, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN) != 0))
  {
    if (OFDPA_DOT1AG_MP_TYPE_ETH == mpType)
    {
      if (pMPObj->mepData != NULL)
      {
        pMPObj->mepData->dot1agCfmMepLbrBadMsdu++;
      }
      return OFDPA_E_NONE;
    }
  }

  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_LBR_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }

  /* Parse the LBR */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;

  dot1agPkt->cfmPdu.lbr.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.lbr.lbrTransId, data);

  /* If the LBR is not discarded, and if and only if LBIactive is true */
  if (pMPObj->mepData != NULL &&
      pMPObj->mepData->dot1agCfmMepLbiVar.lbiActive == OFDPA_TRUE)
  {
    /* The Loopback Transaction Identifier field of the LBR should be next immediate of lastRcvdLbrTransId*/
    if (dot1agPkt->cfmPdu.lbr.lbrTransId != (pMPObj->mepData->dot1agCfmMepLbiVar.lastRcvdLbrTransId + 1))
    {
      /* If the two values are not consecquent, then number of incorrect LBRs received is incremented by 1 */
      pMPObj->mepData->dot1agCfmMepLbrInOutOfOrder++;
    }
    else
    {
      /* If the two values are equal, then number of correct LBRs received is incremented by 1 */
      pMPObj->mepData->dot1agCfmMepLbrIn++;
    }

    pMPObj->mepData->dot1agCfmMepLbiVar.lastRcvdLbrTransId = dot1agPkt->cfmPdu.lbr.lbrTransId;

    /* ProcessLBR() may perform a bit-by-by comparison of the received LBR against the LBM with the
       matching Loopback Transaction Identifier, except for the OpCode field, and increment a managed
       object (12.14.7.1.3:aa) if they do not match. (Not yet implemented) */
  }
  return OFDPA_E_NONE;
}

/* LBR Methods 20.26.1 */
/*********************************************************************
* @purpose  Method called by the LBR SM to process the received LBM
*           message
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.26.1
*
* @end
**********************************************************************/
OFDPA_ERROR_t ProcessLBM(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  uint8_t                *data;
  OFDPA_DOT1AG_MP_type_t  mpType = OFDPA_DOT1AG_MP_TYPE_ETH;
  uint16_t                targetMepId;
  uint8_t                *targetMpId;
  OFDPA_BOOL              forwardLBM = OFDPA_TRUE;

  if (pMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* get MP type */
  if (dot1agMpTypeGet(pMPObj, &mpType) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  /* Validate destination mac address */

  DOT1AG_TRACE(CFM_LBM, "Start Processing LBM received on interface: %d\r\n", dot1agPkt->intIfNum);
  DOT1AG_TRACE(CFM_LBM, "DMAC: %02X:%02X:%02X:%02X:%02X:%02X SMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
               dot1agPkt->destMac[0], dot1agPkt->destMac[1], dot1agPkt->destMac[2],
               dot1agPkt->destMac[3], dot1agPkt->destMac[4], dot1agPkt->destMac[5],
               dot1agPkt->srcMac[0], dot1agPkt->srcMac[1], dot1agPkt->srcMac[2],
               dot1agPkt->srcMac[3], dot1agPkt->srcMac[4], dot1agPkt->srcMac[5]);

  /* Check if dest_mac is group address. If Unicast it should be same
     as receiving MP mac address. If group address, it should follow table 8-9.
  */
  if (((dot1agPkt->destMac[0] & 0x01) == 0) &&
      memcmp(dot1agPkt->destMac, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN) != 0)
  {
    DOT1AG_TRACE(CFM_LBM, "Unicast DMAC, but not mine\r\n");
    if (OFDPA_DOT1AG_MP_TYPE_ETH == mpType)
    {
      return OFDPA_E_NONE;
    }
  }
  else if ((dot1agPkt->destMac[0] & 0x1) &&
           !(memcmp(dot1agRsrvdMac, dot1agPkt->destMac, OFDPA_MAC_ADDR_LEN-1) == 0 &&
             dot1agPkt->destMac[OFDPA_MAC_ADDR_LEN-1] ==
             (dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMPObj->key.dot1agCfmStackMdLevel)))
  {
    DOT1AG_TRACE(CFM_LBM, "Invalid Multicat DMAC\r\n");
    if (OFDPA_DOT1AG_MP_TYPE_ETH == mpType)
    {
      return OFDPA_E_NONE;
    }
  }

  /* If the destination_address parameter contains a Group address and the MP Loopback
     Responder state machine resides in an MHF (rather than in a MEP), ProcessLBM()
     discards the LBM and performs no further processing */
  if ((dot1agPkt->destMac[0] & 0x1) && pMPObj->mepData == NULL)
  {
    DOT1AG_TRACE(CFM_LBM, "Invalid Multicast DMAC\r\n");
    if (OFDPA_DOT1AG_MP_TYPE_ETH == mpType)
      return OFDPA_E_NONE;
  }

  /* If the source_address parameter is a Group, and not an Individual MAC address,
     ProcessLBM() discards the frame and performs no further processing.  */
  if (dot1agPkt->srcMac[0] & 0x1)
  {
    return OFDPA_E_NONE;
  }

  /* Parse the LBM */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.lbm.lbmTransId, data);

  if ((OFDPA_DOT1AG_MP_TYPE_BHH == mpType) &&
      (*(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_TYPE_OFFSET) == DOT1AG_TLV_TYPE_TARGET_MEP))
  {
    if ((*(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_ID_SUBTYPE_OFFSET) == DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_ICC_MEPID))
    {
      memcpy(&targetMepId, (dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_MPID_OFFSET), sizeof(targetMepId));
      targetMepId = htons(targetMepId);
      if ((pMPObj->mepData != NULL) && (targetMepId != pMPObj->mepData->dot1agCfmMepIdentifier))
      {
        forwardLBM = OFDPA_FALSE;
      }
    }
    else if ((*(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_ID_SUBTYPE_OFFSET) == DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_ICC_MIPID))
    {
      targetMpId = dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_MPID_OFFSET;
      if(memcmp(targetMpId, pMPObj->mipIdTlvData, sizeof(pMPObj->mipIdTlvData)) != 0)
      {
        forwardLBM = OFDPA_FALSE;
      }
    }
  }

  if (OFDPA_TRUE == forwardLBM)
  {
    /* Terminate LBM */
    dot1agPkt->toFwd = OFDPA_FALSE;
    /* Call xmitLBR() to generate and transmit an LBR */
    xmitLBR(dot1agPkt, pMPObj);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Transmit LBR
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.26.2
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitLBR(dot1agControlPkt_t *dot1agPkt,
                      dot1agCfmStackObj_t *pMPObj)
{
  ofdbOamLmepData_t       *oamMapData;
  uint8_t                  payLoad[OFDPA_DOT1AG_MAX_FRAME_SIZE];
  OFDPA_ERROR_t            rc;
  OFDPA_DOT1AG_MP_type_t   mpType = OFDPA_DOT1AG_MP_TYPE_ETH;
  uint8_t                 *data;
  uint8_t                  priority = 0;
  uint32_t                 len;
  uint16_t                 vlanId;
  uint8_t                  ethTypeOffset = (dot1agPkt->cfmPduOffset - dot1agPkt->payLoad) - DOT1AG_ETH_TYPE_LEN;
  dot1agCfmRMepObj_t      *pRMEPObj;
  uint16_t                 rmepId;
  dot1agCfmTargetMepTLV_t  replyMepTlv;

  DOT1AG_TRACE(CFM_LBR, "Preparing LBR\r\n");

  if (pMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  vlanId = pMPObj->key.dot1agCfmStackVlanIdOrNone;
  /* get MP type */
  rc = dot1agMpTypeGet(pMPObj, &mpType);
  if (rc != OFDPA_E_NONE)
  {
    return (rc);
  }

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pMPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  /* Sets the destination_address parameter */
  if (dot1agPkt->destMac[0] & 0x1)
  {
    /* to the value from Table 8-9 corresponding to MEP.s MD Level; */
    data = dot1agPkt->payLoad;
    DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);
    DOT1AG_PUT_BYTE(dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMPObj->key.dot1agCfmStackMdLevel, data);
  }
  else
  {
    /* to the source_address of the received LBM */
    memcpy(dot1agPkt->payLoad, dot1agPkt->srcMac, OFDPA_MAC_ADDR_LEN);
  }

  /* Sets the source_address parameter to the MAC address of the replying MP */
  memcpy(dot1agPkt->payLoad + OFDPA_MAC_ADDR_LEN, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

  /* Changes the OpCode field (21.4.3) from LBM to LBR */
  *(dot1agPkt->cfmPduOffset + 1) = DOT1AG_LOOPBACK_REPLY_OPCODE;

  if (OFDPA_DOT1AG_MP_TYPE_BHH == mpType)
  {
    /* Changes MEPID TLV type for MPLS-TP OAM */
    /*
       TLV type will be located after Transaction ID/Sequence Number fields in LBM PDU, so
       CFM_PDU_OFFSET + DOT1AG_CFM_COMMON_HDR_LEN + 4(4 bytes of Transaction ID/Sequence Number)
       TLV ID Sub-Type will be located 3 bytes from start of the TLV header, so
       CFM_PDU_OFFSET + DOT1AG_CFM_COMMON_HDR_LEN + 4(4 bytes of Transaction ID/Sequence Number) + 3 bytes
    */
    if ((*(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_TYPE_OFFSET) == DOT1AG_TLV_TYPE_TARGET_MEP) &&
        ((*(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_ID_SUBTYPE_OFFSET) != DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_INGRESS) &&
         (*(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_ID_SUBTYPE_OFFSET) != DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_EGRESS)))
    {
      if (*(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_ID_SUBTYPE_OFFSET) == DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_ICC_MEPID)
      {
        /* Change TLV Type to Reply MEP TLV Type */
        *(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_TYPE_OFFSET) = DOT1AG_TLV_TYPE_REPLY_MEP;
        /* Section 8.2.2.1 G.8113.1 - Assign ID sub-type, should be 0x2 for ICC-based MEP ID or 0x3 for ICC-based MIP ID*/
        if (pMPObj->mepData == NULL) /* MIP */
        {
          *(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_ID_SUBTYPE_OFFSET) = DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_ICC_MIPID;
          /* Assign MIP Identifier */
          memcpy((dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_MPID_OFFSET), pMPObj->mipIdTlvData, sizeof(pMPObj->mipIdTlvData));
        }
        else /* MEP */
        {
          /*
             Check if there is a Requesting MEP ID TLV in LBM PDU and verify MEP generating this LBM is configured as
             RMEP for this MEP in this system or not
          */
          if (*(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_REQUESTING_MEP_TLV_TYPE_OFFSET) == DOT1AG_TLV_TYPE_REQUEST_MEP)
          {
            memcpy(&rmepId, (dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_REQUESTING_MEP_TLV_MPID_OFFSET), sizeof(rmepId));
            rmepId = htons(rmepId);
            if(pMPObj->mepData->maData != NULL &&
               pMPObj->mepData->maData->mdData != NULL)
            {
              if ((pRMEPObj = dot1agCfmRMepObjGet(pMPObj->mepData->maData->mdData->dot1agCfmMdIndex, pMPObj->mepData->maData->dot1agCfmMaIndex,
                                                  pMPObj->mepData->dot1agCfmMepIdentifier, (uint32_t )rmepId, AVL_EXACT)) != 0)
              {
                /* Update Loopback Indication field in Requesting MEP ID TLV*/
                *(dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_REQUESTING_MEP_TLV_LOOPBACK_INDICATION_OFFSET) = 1;
              }
            }
          }

          /* Overwrite Target MEP TLV with Replying MEP TLV*/
          replyMepTlv.length = DOT1AG_TLV_FIELD_LEN_TARGET_MEP;
          replyMepTlv.subType = DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_ICC_MEPID; /* ICC-based MEP ID */
          replyMepTlv.MepId = pMPObj->mepData->dot1agCfmMepIdentifier;
          rc = dot1agCfmReplyMepTlvWrite(&replyMepTlv, (dot1agPkt->cfmPduOffset + DOT1AG_BHH_LB_TARGET_MEP_TLV_TYPE_OFFSET), &len);

          if (rc != OFDPA_E_NONE)
          {
            return rc;
          }
        }
      }
    }
  }

  dot1agPkt->length = dot1agPkt->length - 4; //Removing CRC

  memcpy(payLoad, (dot1agPkt->payLoad + ethTypeOffset), (dot1agPkt->length - ethTypeOffset));

  data = dot1agPkt->payLoad;

  if (OFDPA_DOT1AG_MP_TYPE_BHH == mpType)
  {
    /* Fill MPLS-TP header */
    if (pMPObj->mepData != NULL)
    {
      priority = pMPObj->mepData->dot1agCfmMepTransmitLbmVlanPriority;
    }

    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0 || (OFDPA_DOT1AG_MAX_FRAME_SIZE < (dot1agPkt->length - ethTypeOffset + len)))
    {
      return OFDPA_E_FAIL;
    }

    data += len;
    memcpy(data, payLoad, (dot1agPkt->length - ethTypeOffset));
    dot1agPkt->length = (dot1agPkt->length - ethTypeOffset) + len;
    vlanId = oamMapData->vlanId;
  }

  /* If the replying MP is a MEP, increments the LBR transmission counter */
  if (pMPObj->mepData != NULL)
  {
    pMPObj->mepData->dot1agCfmMepLbrOut++;
  }

  dot1agPkt->lmepId    = pMPObj->ofdpaLocalMpId;
  dot1agPkt->oamOpcode = DOT1AG_LOOPBACK_REPLY_OPCODE;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  DOT1AG_TRACE(CFM_LBR, "Transmitting LBR\r\n");
  (void)dot1agPduTransmit(pMPObj->key.dot1agCfmStackifIndex,
                          vlanId,
                          dot1agPkt);

  return  OFDPA_E_NONE;
}

/* LTIR Methods */
/*********************************************************************
* @purpose  Parse LTR Pdu and store int in local CFM PDU message
*           storage
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 21.9
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agParseLTR(dot1agControlPkt_t *dot1agPkt)
{
  uint8_t  *data, type;
  uint16_t  length;

  /* Point to CFM PDU */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;
  /* 802.1ag d8-1 21.9.2 */
  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_LTR_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }

  dot1agPkt->cfmPdu.ltr.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;

  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.ltr.ltrTransId, data);
  DOT1AG_GET_BYTE(dot1agPkt->cfmPdu.ltr.ttl, data);
  /*802.1ag d8-1 21.9.5 */
  DOT1AG_GET_BYTE(dot1agPkt->cfmPdu.ltr.relayAction, data);
  switch (dot1agPkt->cfmPdu.ltr.relayAction)
  {
    case DOT1AG_RELAY_ACTION_rlyHit:
    case DOT1AG_RELAY_ACTION_rlyFdb:
    case DOT1AG_RELAY_ACTION_rlyMpdb:
      break;
    default:
      DOT1AG_TRACE(CFM_LTR, "Invalid Relay Action received.\r\n");
      return OFDPA_E_FAIL;
      break;
  }

  dot1agPkt->cfmPdu.ltr.useFDBOnly  = DOT1AG_LTR_FLAGS_USE_FDB_GET(dot1agPkt->cfmHdr.flags);
  dot1agPkt->cfmPdu.ltr.fwdYes      = DOT1AG_LTR_FLAGS_FWD_YES_GET(dot1agPkt->cfmHdr.flags);
  dot1agPkt->cfmPdu.ltr.terminalMEP = DOT1AG_LTR_TERMINAL_MEP_GET(dot1agPkt->cfmHdr.flags);

  /* Parse available TLVs */
  data = dot1agPkt->cfmPdu.ltr.pFirstTLV;

  DOT1AG_GET_BYTE(type, data);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);
    switch (type)
    {
      /* LTR Egress TLV */
      case DOT1AG_TLV_TYPE_LTR_EGR_ID:
        dot1agPkt->cfmPdu.ltr.ltrEgressIdTLV.length = length;
        if (dot1agCfmLTREgressIdTlvRead(&dot1agPkt->cfmPdu.ltr.ltrEgressIdTLV,
                                        data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        dot1agPkt->cfmPdu.ltr.ltrEgressIdTLVRead = OFDPA_TRUE;
        break;
        /* Reply Ingress TLV */
      case DOT1AG_TLV_TYPE_REPLY_INGRESS:
        dot1agPkt->cfmPdu.ltr.replyIngressTLV.length = length;
        if (dot1agCfmReplyIngressTlvRead(&dot1agPkt->cfmPdu.ltr.replyIngressTLV,
                                         data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        dot1agPkt->cfmPdu.ltr.replyIngressTLVRead = OFDPA_TRUE;
        break;
        /* Reply Egress TLV */
      case DOT1AG_TLV_TYPE_REPLY_EGRESS:
        dot1agPkt->cfmPdu.ltr.replyEgressTLV.length = length;
        if (dot1agCfmReplyEgressTlvRead(&dot1agPkt->cfmPdu.ltr.replyEgressTLV,
                                        data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        dot1agPkt->cfmPdu.ltr.replyEgressTLVRead = OFDPA_TRUE;
        break;
        /* Sender ID TLV */
      case DOT1AG_TLV_TYPE_SENDER_ID:
        dot1agPkt->cfmPdu.ltr.senderIdTLV.length = length;
        if (dot1agCfmSenderIdTlvRead(&dot1agPkt->cfmPdu.ltr.senderIdTLV,
                                     data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        dot1agPkt->cfmPdu.ltr.senderIdTLVRead = OFDPA_TRUE;
        break;
        /* Org TLV */
      case DOT1AG_TLV_TYPE_ORG_SPECIFIC:
        /* Ignore all other TLVs */
      default:
        break;
    }
    data += length;
    DOT1AG_GET_BYTE(type, data);
  }
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose   Method called to insert newly learnt LTR into the
*            ltmReplyList
*
* @param    pLTREntry  @b{(input)} reference to a received LTR Entry
*
* @returns  OFDPA_E_NONE
*           OFDPA_E_FAIL
*
* @notes    802.1ag d8-1 20.39.1
*           If the addition of this LTR entry would exceed the
*           resources allocated to ltmReplyList, then the oldest LTM
*           entries in ltmReplyList (the oldest LTR entries, if only one
*           LTM entry is present) are deleted until sufficient
*           resources are available to hold the new LTR entry.
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agLtrEntryInsert(dot1agCfmLtrObj_t *pLTREntry)
{
  dot1agCfmLtrObj_t *ptmpLTREntry = NULL;
  uint32_t           mdIndex, maIndex, mepId;
  uint32_t           ltrSeqNumber, ltrReceiveOrder;
  OFDPA_ERROR_t      rc;

  if ((rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_LTR_TREE, &dot1agCFM.dot1agLtrTree,
                                 pLTREntry)) == OFDPA_E_FULL)
  {
    mdIndex = 0;
    maIndex = 0;
    mepId = 0;
    ltrSeqNumber = 0;
    ltrReceiveOrder = 0;
    /* Delete just before entry */
    while ((ptmpLTREntry = dot1agCfmLtrObjGet(mdIndex, maIndex, mepId,
                                              ltrSeqNumber, ltrReceiveOrder,
                                              AVL_NEXT)) != NULL)
    {
      if (mdIndex == ptmpLTREntry->key.dot1agCfmMdIndex &&
          maIndex == ptmpLTREntry->key.dot1agCfmMaIndex &&
          mepId   == ptmpLTREntry->key.dot1agCfmMepIdentifier)
      {
        break;
      }

      /* Get next entry */
      mdIndex         = ptmpLTREntry->key.dot1agCfmMdIndex;
      maIndex         = ptmpLTREntry->key.dot1agCfmMaIndex;
      mepId           = ptmpLTREntry->key.dot1agCfmMepIdentifier;
      ltrSeqNumber    = ptmpLTREntry->key.dot1agCfmLtrSeqNumber;
      ltrReceiveOrder = ptmpLTREntry->key.dot1agCfmLtrReceiveOrder;
    }

    if (ptmpLTREntry != NULL)
    {
      /* Delete the oldest entry and add new entry */
      if (dot1agDBTreeEntryDelete(DOT1AG_DB_ID_LTR_TREE, &dot1agCFM.dot1agLtrTree,
                                  ptmpLTREntry) == OFDPA_E_NONE)
      {
        if (dot1agDBTreeEntryAdd(DOT1AG_DB_ID_LTR_TREE, &dot1agCFM.dot1agLtrTree,
                                 pLTREntry) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to create LTR for mdIndex %d maIndex %d mepId %d transId %d recvOrder %d",
                             pLTREntry->key.dot1agCfmMdIndex, pLTREntry->key.dot1agCfmMaIndex, pLTREntry->key.dot1agCfmMepIdentifier,
                             pLTREntry->key.dot1agCfmLtrSeqNumber, pLTREntry->key.dot1agCfmLtrReceiveOrder);
          return OFDPA_E_FAIL;
        }
        return OFDPA_E_NONE;
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to delete LTR for mdIndex %d maIndex %d mepId %d transId %d recvOrder %d",
                           ptmpLTREntry->key.dot1agCfmMdIndex, ptmpLTREntry->key.dot1agCfmMaIndex, ptmpLTREntry->key.dot1agCfmMepIdentifier,
                           ptmpLTREntry->key.dot1agCfmLtrSeqNumber, ptmpLTREntry->key.dot1agCfmLtrReceiveOrder);
      }
    }
  }
  else if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to create LTR for mdIndex %d maIndex %d mepId %d transId %d recvOrder %d",
                       pLTREntry->key.dot1agCfmMdIndex, pLTREntry->key.dot1agCfmMaIndex, pLTREntry->key.dot1agCfmMepIdentifier,
                       pLTREntry->key.dot1agCfmLtrSeqNumber, pLTREntry->key.dot1agCfmLtrReceiveOrder);
  }

  return rc;
}

/*********************************************************************
* @purpose   Method called by the LRIR SM to process the received LTR
*            message
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.39.1
*
* @end
**********************************************************************/
OFDPA_ERROR_t ProcessLTR(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  dot1agCfmLtrObj_t *pLTREntry    = NULL, ltrEntry;
  OFDPA_BOOL         foundEntry   = OFDPA_FALSE;
  uint32_t           lastRcvOrder = 0;

  DOT1AG_TRACE(CFM_LTR, "Start Processing LTR received on interface: %d\r\n", dot1agPkt->intIfNum);

  if (pMPObj == NULL ||
      pMPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }
  /* If the destination_address of the LTR does not match the
     MAC address of the MEP */
  if (memcmp(dot1agPkt->destMac, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN) != 0)
  {
    return OFDPA_E_NONE;
  }

  /* Parse LTR message */
  if (dot1agParseLTR(dot1agPkt) != OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_LTR, "LTR parsing failed.\r\n");
    return OFDPA_E_FAIL;
  }

  if (pMPObj->mepData->maData == NULL ||
      pMPObj->mepData->maData->mdData == NULL)
  {
    DOT1AG_TRACE(CFM_LTR, "maData/mdData is NULL.\r\n");
    return OFDPA_E_FAIL;
  }

  if ((pLTREntry = dot1agCfmLtrObjGet(pMPObj->mepData->maData->mdData->dot1agCfmMdIndex,
                                      pMPObj->mepData->maData->dot1agCfmMaIndex,
                                      pMPObj->mepData->dot1agCfmMepIdentifier,
                                      dot1agPkt->cfmPdu.ltr.ltrTransId, lastRcvOrder,
                                      AVL_EXACT)) != NULL)
  {
    if (pLTREntry->key.dot1agCfmMdIndex == pMPObj->mepData->maData->mdData->dot1agCfmMdIndex &&
        pLTREntry->key.dot1agCfmMaIndex == pMPObj->mepData->maData->dot1agCfmMaIndex &&
        pLTREntry->key.dot1agCfmMepIdentifier == pMPObj->mepData->dot1agCfmMepIdentifier &&
        pLTREntry->key.dot1agCfmLtrSeqNumber == dot1agPkt->cfmPdu.ltr.ltrTransId)
    {
      foundEntry = OFDPA_TRUE;
    }
  }
  /* If the LTR Transaction Identifier field matches an LTM entry in
     the ltmReplyList variable, then a new LTR entry is attached to that
     LTM entry containing the information returned in the LTR. */
  lastRcvOrder = 0;
  while ((pLTREntry = dot1agCfmLtrObjGet(pMPObj->mepData->maData->mdData->dot1agCfmMdIndex,
                                         pMPObj->mepData->maData->dot1agCfmMaIndex,
                                         pMPObj->mepData->dot1agCfmMepIdentifier,
                                         dot1agPkt->cfmPdu.ltr.ltrTransId, lastRcvOrder,
                                         AVL_NEXT)) != NULL)
  {
    if (pMPObj->mepData->maData->mdData->dot1agCfmMdIndex != pLTREntry->key.dot1agCfmMdIndex       ||
        pMPObj->mepData->maData->dot1agCfmMaIndex         != pLTREntry->key.dot1agCfmMaIndex       ||
        pMPObj->mepData->dot1agCfmMepIdentifier           != pLTREntry->key.dot1agCfmMepIdentifier ||
        dot1agPkt->cfmPdu.ltr.ltrTransId                  != pLTREntry->key.dot1agCfmLtrSeqNumber)
    {
      break;
    }
    else
    {
      lastRcvOrder = pLTREntry->key.dot1agCfmLtrReceiveOrder;
    }
  }/* Loop through ltmReplyList */

  if (foundEntry == OFDPA_TRUE)
  {
    /* An LTM entry exists - Create a new LTM entry */
    memset(&ltrEntry, 0x0, sizeof(ltrEntry));

    ltrEntry.key.dot1agCfmMdIndex         = pMPObj->mepData->maData->mdData->dot1agCfmMdIndex;
    ltrEntry.key.dot1agCfmMaIndex         = pMPObj->mepData->maData->dot1agCfmMaIndex;
    ltrEntry.key.dot1agCfmMepIdentifier   = pMPObj->mepData->dot1agCfmMepIdentifier;
    ltrEntry.key.dot1agCfmLtrSeqNumber    = dot1agPkt->cfmPdu.ltr.ltrTransId;
    ltrEntry.key.dot1agCfmLtrReceiveOrder = lastRcvOrder + 1;

    ltrEntry.dot1agCfmLtrTtl              = dot1agPkt->cfmPdu.ltr.ttl;
    ltrEntry.dot1agCfmLtrForwarded        = dot1agPkt->cfmPdu.ltr.fwdYes;
    ltrEntry.dot1agCfmLtrTerminalMep      = dot1agPkt->cfmPdu.ltr.terminalMEP;
    ltrEntry.dot1agCfmLtrRelay            = dot1agPkt->cfmPdu.ltr.relayAction;

/* Optionally add all the TLVs learnt */
    if (dot1agPkt->cfmPdu.ltr.ltrEgressIdTLVRead == OFDPA_TRUE)
    {
      memcpy(ltrEntry.dot1agCfmLtrLastEgressIdentifier,
             dot1agPkt->cfmPdu.ltr.ltrEgressIdTLV.lastEgressId,
             DOT1AG_TLV_FIELD_LEN_LTM_EGRESS_ID);
      memcpy(ltrEntry.dot1agCfmLtrNextEgressIdentifier,
             dot1agPkt->cfmPdu.ltr.ltrEgressIdTLV.nextEgressId,
             DOT1AG_TLV_FIELD_LEN_LTM_EGRESS_ID);
    }

    if (dot1agPkt->cfmPdu.ltr.replyIngressTLVRead == OFDPA_TRUE)
    { /* replyIngressTLV */
      ltrEntry.dot1agCfmLtrIngress = dot1agPkt->cfmPdu.ltr.replyIngressTLV.ingressAction;
      memcpy(ltrEntry.dot1agCfmLtrIngressMac,
             dot1agPkt->cfmPdu.ltr.replyIngressTLV.ingressMacAdrs,
             OFDPA_MAC_ADDR_LEN);
      ltrEntry.dot1agCfmLtrIngressPortIdLen     = dot1agPkt->cfmPdu.ltr.replyIngressTLV.ingressPortIdLength;
      ltrEntry.dot1agCfmLtrIngressPortIdSubtype = dot1agPkt->cfmPdu.ltr.replyIngressTLV.ingressPortIdSubType;
      memcpy(ltrEntry.dot1agCfmLtrIngressPortId,
             dot1agPkt->cfmPdu.ltr.replyIngressTLV.ingressPortId,
             DOT1AG_TLV_FIELD_LEN_INGRESS_PORT_ID);
    }

    if (dot1agPkt->cfmPdu.ltr.replyEgressTLVRead == OFDPA_TRUE)
    {
      /* replyEgressTLV */
      ltrEntry.dot1agCfmLtrEgress = dot1agPkt->cfmPdu.ltr.replyEgressTLV.egressAction;
      memcpy(ltrEntry.dot1agCfmLtrEgressMac,
             dot1agPkt->cfmPdu.ltr.replyEgressTLV.egressMacAdrs,
             OFDPA_MAC_ADDR_LEN);
      ltrEntry.dot1agCfmLtrEgressPortIdLen     = dot1agPkt->cfmPdu.ltr.replyEgressTLV.egressPortIdLength;
      ltrEntry.dot1agCfmLtrEgressPortIdSubtype = dot1agPkt->cfmPdu.ltr.replyEgressTLV.egressPortIdSubType;
      memcpy(ltrEntry.dot1agCfmLtrEgressPortId,
             dot1agPkt->cfmPdu.ltr.replyEgressTLV.egressPortId,
             DOT1AG_TLV_FIELD_LEN_EGRESS_PORT_ID);
    }

    if (dot1agPkt->cfmPdu.ltr.senderIdTLVRead == OFDPA_TRUE)
    {
      /* senderIdTLV */
      ltrEntry.dot1agCfmLtrChassisIdSubtype = dot1agPkt->cfmPdu.ltr.senderIdTLV.chassisIdSubtype;
      ltrEntry.dot1agCfmLtrChassisIdLen = dot1agPkt->cfmPdu.ltr.senderIdTLV.chassisIdLen;
      memcpy(ltrEntry.dot1agCfmLtrChassisId,
             dot1agPkt->cfmPdu.ltr.senderIdTLV.chassisId,
             dot1agPkt->cfmPdu.ltr.senderIdTLV.chassisIdLen);

      ltrEntry.dot1agCfmLtrManAddressDomainLen = dot1agPkt->cfmPdu.ltr.senderIdTLV.addressDomainLen;
      memcpy(ltrEntry.dot1agCfmLtrManAddressDomain,
             dot1agPkt->cfmPdu.ltr.senderIdTLV.addressDomain,
             dot1agPkt->cfmPdu.ltr.senderIdTLV.addressDomainLen);

      ltrEntry.dot1agCfmLtrManAddressLen = dot1agPkt->cfmPdu.ltr.senderIdTLV.manAddressLen;
      memcpy(ltrEntry.dot1agCfmLtrManAddress,
             dot1agPkt->cfmPdu.ltr.senderIdTLV.manAddress,
             dot1agPkt->cfmPdu.ltr.senderIdTLV.manAddressLen);
    }

    if (dot1agLtrEntryInsert(&ltrEntry) != OFDPA_E_NONE)
    {
      return OFDPA_E_FAIL;
    }
  }
  else
  {
    /* Otherwise, the number of unexpected LTRs received
       is incremented by 1 */
    pMPObj->mepData->dot1agCfmMepUnexpLtrIn++;
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Method used to convert CCMInterval variable into equivalen
*           time interval according to 21.6.1.3
*
* @param    ccmInterval  @b{(input)} CCMInterval read from the CCM PDU
*
* @returns  A value, the corresponding time interval, in the
*           form used by the CCIwhile or rMEPwhile timer. Returns 0
*           if invalid
*
* @notes    802.1ag d8-1 20.6.1, 21.6.1.3
*
* @end
**********************************************************************/
uint32_t CCMtime(uint32_t ccmInterval, OFDPA_BOOL hwEnabled)
{
  uint32_t timeInSeconds = 0;

  switch (ccmInterval)
  {
    case DOT1AG_CCM_intervalInvalid: /* No CCMs are sent (disabled). */
      break;
    case DOT1AG_CCM_interval300Hz:   /* CCMs are sent every 3 1/3 milliseconds (300Hz). */
      timeInSeconds = 3;
      break;
    case DOT1AG_CCM_interval10ms:    /* CCMs are sent every 10 milliseconds. */
      timeInSeconds = 10;
      break;
    case DOT1AG_CCM_interval100ms:   /* CCMs are sent every 100 milliseconds. */
      timeInSeconds = 100;
      break;
    case DOT1AG_CCM_interval1s:      /* CCMs are sent every 1 second. */
      timeInSeconds = 1000;
      break;
    case DOT1AG_CCM_interval10s:     /* CCMs are sent every 10 seconds. */
      timeInSeconds = 10000;
      break;
    case DOT1AG_CCM_interval1min:    /* CCMs are sent every minute. */
      timeInSeconds = 60000;
      break;
    case DOT1AG_CCM_interval10min:  /* CCMs are sent every 10 minute. */
      timeInSeconds = 600000;
      break;
    default:
      break;
  }

  return timeInSeconds;
}

/*********************************************************************
* @purpose  Transmit Fault Alarm
*
* @param    mdIndex   @b{(input)} Maintenance Domain identifier
* @param    maIndex   @b{(input)} Maintenance Association identifier
* @param    mepId     @b{(input)} MEP whose FNGSM is reporting a defect
* @param    fngDefect   @b{(input)} Defect to be reported
*
* @returns  none
*
* @notes    802.1ag d8-1 20.34.1
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitFaultAlarm(uint32_t mdIndex, uint32_t maIndex,
                             uint32_t mepId, uint32_t fngDefect)
{
  /* Call SNMP Trap manager routine to report the defect to NMS */
  return dot1agCfmFaultNotification(mdIndex, maIndex, mepId, fngDefect);
}

/*********************************************************************
* @purpose  Evaluate the MADefectIndication variable and
*           execute FNG SM if required for a  given
*           MEP (mdIndex, maIndex, mepId)
*
* @param    mepData   @b{(input)} Handle to the MEP data.
*
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @comments Generates events for the FNG SM
*           802.1ag d8-1 20.33, 20.9.3, 20.9.5, 20.9.6
*
* @end
**********************************************************************/
void MAdefectIndicationNotify(dot1agCfmMepTable_t *mepData)
{
  dot1agCfmRMepObj_t    *pRMEPObj                = NULL;
  uint32_t               rmepId                  = 0, mdIndex, maIndex, mepId;
  OFDPA_BOOL             someRDIDefect           = OFDPA_FALSE;
  OFDPA_BOOL             someRMEPCCMdefect       = OFDPA_FALSE;
  OFDPA_BOOL             someMACPortstatusDefect = OFDPA_FALSE;
  OFDPA_BOOL             someMACIntfstatusDefect = OFDPA_FALSE;
  OFDPA_BOOL             someMACstatusDefect     = OFDPA_FALSE;
  OFDPA_BOOL             firstTime               = OFDPA_TRUE;
  dot1agCfmSMEventMsg_t  eventMsg;

  if (mepData == NULL ||
      mepData->maData == NULL ||
      mepData->maData->mdData == NULL)
  {
    return;
  }

  mdIndex = mepData->maData->mdData->dot1agCfmMdIndex;
  maIndex = mepData->maData->dot1agCfmMaIndex;
  mepId   = mepData->dot1agCfmMepIdentifier;

  /* Calculate the defect status after checking each Rmep entry */
  while ((pRMEPObj = dot1agCfmRMepObjGet(mdIndex, maIndex, mepId,
                                         rmepId, AVL_NEXT)) != NULL)
  {
    /* Check if no more RMEPs exist for this (mdIndex, maIndex, mepId) combination */
    if (pRMEPObj->key.dot1agCfmMdIndex != mdIndex ||
        pRMEPObj->key.dot1agCfmMaIndex != maIndex ||
        pRMEPObj->key.dot1agCfmMepIdentifier != mepId)
    {
      break;
    }

    if (firstTime == OFDPA_TRUE)
    {
      firstTime = OFDPA_FALSE;
      someMACPortstatusDefect = pRMEPObj->dot1agCfmRmepVar.rMepPortStatusDefect;
    }
    else
    {
      someMACPortstatusDefect &= pRMEPObj->dot1agCfmRmepVar.rMepPortStatusDefect;
    }
    someMACIntfstatusDefect |= pRMEPObj->dot1agCfmRmepVar.rMepInterfaceStatusDefect;

    if (OFDPA_TRUE != dot1agIsMepHwEnabled (mepData->dot1agCfmMepIdentifier))
    {
      someRMEPCCMdefect |= pRMEPObj->dot1agCfmRmepVar.rMepCcmDefect;
      someRDIDefect |= pRMEPObj->dot1agCfmRmepVar.rMepLastRDI;
    }

    rmepId = pRMEPObj->key.dot1agCfmMepDbRMepIdentifier;
  }/* Loop through all the RMEPs */
  someMACstatusDefect = someMACPortstatusDefect | someMACIntfstatusDefect;
  /* Copy calculated values to the MEP FNG SM variables */
  mepData->dot1agCfmMepFngVar.someMacStatusDefect = someMACstatusDefect;

  /* For hardware enabled dot1ag, hardware generates the events
   * for someRMEPCCMdefect and someRDIDefect occurs.
   */
  if (OFDPA_TRUE != dot1agIsMepHwEnabled (mepData->dot1agCfmMepIdentifier))
  {
    mepData->dot1agCfmMepFngVar.someRMEPCCMDefect = someRMEPCCMdefect;
    mepData->dot1agCfmMepFngVar.someRDIDefect     = someRDIDefect;
  }

  mepData->dot1agCfmGblMepVar.maDefectIndication = OFDPA_FALSE;
  mepData->dot1agCfmGblMepVar.presentRDI = OFDPA_FALSE;
  mepData->dot1agCfmMepFngVar.highestDefect = DOT1AG_DEFECT_none;
  mepData->dot1agCfmMepFngVar.highestDefectPri = DOT1AG_DEFECT_PRIORITY_none;
  /*Re-Calculate MADefectIndication variable */
  switch (mepData->dot1agCfmGblMepVar.lowestAlarmPri)
  {
    case DOT1AG_ALARM_PRIORITY_defRDICCM:
      if (mepData->dot1agCfmMepFngVar.someRDIDefect == OFDPA_TRUE)
      {
        mepData->dot1agCfmGblMepVar.maDefectIndication = OFDPA_TRUE;
        mepData->y1731DefectVar[Y1731_DEFECT_TYPE_RDI].y1731DefCcmDefect = OFDPA_TRUE;
        mepData->y1731DefectState[Y1731_DEFECT_TYPE_RDI] = Y1731_MEP_DEFECT_DEFECT;
        mepData->dot1agCfmMepFngVar.highestDefect = DOT1AG_DEFECT_defRDICCM;
        mepData->dot1agCfmMepFngVar.highestDefectPri = DOT1AG_DEFECT_PRIORITY_defRDICCM;
      }
      else
      {
        mepData->y1731DefectVar[Y1731_DEFECT_TYPE_RDI].y1731DefCcmDefect = OFDPA_FALSE;
        mepData->y1731DefectState[Y1731_DEFECT_TYPE_RDI] = Y1731_MEP_DEFECT_NO_DEFECT;
      }
      /*passthru*/
    case DOT1AG_ALARM_PRIORITY_defMACstatus:
      if (someMACstatusDefect == OFDPA_TRUE)
      {
        mepData->dot1agCfmGblMepVar.maDefectIndication = OFDPA_TRUE;
        mepData->dot1agCfmMepFngVar.highestDefect = DOT1AG_DEFECT_defMACstatus;
        mepData->dot1agCfmMepFngVar.highestDefectPri = DOT1AG_DEFECT_PRIORITY_defMACstatus;
        mepData->dot1agCfmGblMepVar.presentRDI = OFDPA_TRUE;
      }
      /*passthru*/
    case DOT1AG_ALARM_PRIORITY_defRemoteCCM:
      if (mepData->dot1agCfmMepFngVar.someRMEPCCMDefect == OFDPA_TRUE)
      {
        mepData->dot1agCfmGblMepVar.maDefectIndication = OFDPA_TRUE;
        mepData->y1731DefectVar[Y1731_DEFECT_TYPE_LOC].y1731DefCcmDefect = OFDPA_TRUE;
        mepData->y1731DefectState[Y1731_DEFECT_TYPE_LOC] = Y1731_MEP_DEFECT_DEFECT;
        mepData->dot1agCfmMepFngVar.highestDefect = DOT1AG_DEFECT_defRemoteCCM;
        mepData->dot1agCfmMepFngVar.highestDefectPri = DOT1AG_DEFECT_PRIORITY_defRemoteCCM;
        mepData->dot1agCfmGblMepVar.presentRDI = OFDPA_TRUE;
      }
      else
      {
        mepData->y1731DefectVar[Y1731_DEFECT_TYPE_LOC].y1731DefCcmDefect = OFDPA_FALSE;
        mepData->y1731DefectState[Y1731_DEFECT_TYPE_LOC] = Y1731_MEP_DEFECT_NO_DEFECT;
      }
      /*passthru*/
    case DOT1AG_ALARM_PRIORITY_defErrorCCM:
      if (mepData->dot1agCfmRmepErrVar.errorCcmDefect == OFDPA_TRUE)
      {
        mepData->dot1agCfmGblMepVar.maDefectIndication = OFDPA_TRUE;
        mepData->dot1agCfmMepFngVar.highestDefect = DOT1AG_DEFECT_defErrorCCM;
        mepData->dot1agCfmMepFngVar.highestDefectPri = DOT1AG_DEFECT_PRIORITY_defErrorCCM;
        mepData->dot1agCfmGblMepVar.presentRDI = OFDPA_TRUE;
      }
      /*passthru*/
    case DOT1AG_ALARM_PRIORITY_defXconCCM:
      if (mepData->dot1agCfmMepXConVar.xconCcmDefect == OFDPA_TRUE)
      {
        mepData->dot1agCfmGblMepVar.maDefectIndication = OFDPA_TRUE;
        mepData->dot1agCfmMepFngVar.highestDefect = DOT1AG_DEFECT_defXconCCM;
        mepData->dot1agCfmMepFngVar.highestDefectPri = DOT1AG_DEFECT_PRIORITY_defXconCCM;
        mepData->dot1agCfmGblMepVar.presentRDI = OFDPA_TRUE;
      }
      break;
    case DOT1AG_ALARM_PRIORITY_none:
    default:
      break;
  }
  eventMsg.pMepData = mepData;
  DOT1AG_TRACE(CFM_SM_FNG, "someRDIDefect %d someMACstatusDefect %d someRMEPCCMdefect %d errorCcmDefect %d xconCcmDefect %d\r\n",
               mepData->dot1agCfmMepFngVar.someRDIDefect, someMACstatusDefect,
               mepData->dot1agCfmMepFngVar.someRMEPCCMDefect,
               mepData->dot1agCfmRmepErrVar.errorCcmDefect,
               mepData->dot1agCfmMepXConVar.xconCcmDefect);

  if (mepData->dot1agCfmMepFngVar.fngTimer == NULL)
  {
    if (mepData->dot1agCfmGblMepVar.maDefectIndication == OFDPA_TRUE)
    {
      if (mepData->dot1agCfmMepSMFngState == DOT1AG_FNG_DEFECT_REPORTED)
      {
        if (mepData->dot1agCfmMepFngVar.highestDefectPri > mepData->dot1agCfmMepFngVar.fngPriority)
        {
          eventMsg.event = dot1agFNGMaDefectIndicatnHighDefectPrio;
        }
        else
        {
          /* Ignore this Defect indication as it is of lower priority than
             previously generated defect*/
          DOT1AG_TRACE(CFM_SM_FNG, "Ignoring defect indication for mepId:%d, mdId:%d, maId:%d "
                                   "MEPFNGSTATE:FNG_DEFECT_REPORTED fngTimer:NULL, highestDefectPri:%d, "
                                   "fngPriority:%d\r\n", mepId, mdIndex, maIndex,
                       mepData->dot1agCfmMepFngVar.highestDefectPri,
                       mepData->dot1agCfmMepFngVar.fngPriority);
          return;
        }
      }
      else
      {
        eventMsg.event = dot1agFNGMaDefectIndicatnFNGWhileZero;
      }
    }
    else
    {
      eventMsg.event = dot1agFNGNotMaDefectIndicatnFNGWhileZero;
    }
  }
  else
  {
    if (mepData->dot1agCfmGblMepVar.maDefectIndication == OFDPA_TRUE)
    {
      if (mepData->dot1agCfmMepSMFngState == DOT1AG_FNG_DEFECT_REPORTED)
      {
        if (mepData->dot1agCfmMepFngVar.highestDefectPri > mepData->dot1agCfmMepFngVar.fngPriority)
        {
          eventMsg.event = dot1agFNGMaDefectIndicatnHighDefectPrio;
        }
        else
        {
          /* Ignore this Defect indication as it is of lower priority than
             previously generated defect*/
          DOT1AG_TRACE(CFM_SM_FNG, "Ignoring defect indication for mepId:%d, mdId:%d, maId:%d"
                                   "MEPFNGSTATE:FNG_DEFECT_REPORTED, highestDefectPri:%d, fngPriority:%d\r\n",
                       mepId, mdIndex, maIndex, mepData->dot1agCfmMepFngVar.highestDefectPri,
                       mepData->dot1agCfmMepFngVar.fngPriority);

          return;
        }
      }
      else
      {
        eventMsg.event = dot1agFNGMaDefectIndicatn;
      }
    }
    else
    {
      eventMsg.event = dot1agFNGNotMaDefectIndicatn;
    }
  }

  if ((eventMsg.pMepData != NULL) &&
      (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMepData->dot1agCfmMepIdentifier)))
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
  }
  else
  {
    (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
  }

  /* CSF */
  CSFTriggerHandler(mepData);

  /* AIS */
  if (OFDPA_TRUE == mepData->y1731CfmMepAisSendVar.aisEnable)
  {
    memset(&eventMsg, 0, sizeof(eventMsg));
    eventMsg.pMepData = mepData;
    eventMsg.event  = y1731AisSendStart;

    if ((eventMsg.pMepData != NULL) &&
        (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMepData->dot1agCfmMepIdentifier)))
    {
      dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
    }
    else
    {
      dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
    }
  }
}

/*********************************************************************
* @purpose  Transmit LBM on requested MEP
*
* @param    pMEPObj    @b{(input)} MEP on which LBM has to be sent
*
* @returns  none
*
* @notes    802.1ag d8-1 20.29.1
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitLBM(dot1agCfmStackObj_t *pMEPObj)
{
  uint8_t                 *data;
  dot1agCfmRMepObj_t      *pRMEPObj = NULL;
  dot1agCfmCommonHdr_t     cfmHdr;
  uint16_t                 shortVal;
  uint32_t                 uintVal;
  dot1agCfmSenderIdTLV_t   senderIdTlv;
  dot1agCfmTargetMepTLV_t  targetMepTlv;
  dot1agCfmTestTLV_t       testTlv;
  dot1agCfmDataTLV_t       dataTlv;
  OFDPA_DOT1AG_MP_type_t   mpType;
  OFDPA_ERROR_t            rc;
  uint8_t                  priority;
  ofdbOamLmepData_t      *oamMapData;
  uint32_t                 len = 0;
  uint16_t                 vlanId = 0;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  memset(&dot1agPktFromCpu, 0x0, sizeof(dot1agPktFromCpu));
  data = dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.intIfNum = pMEPObj->key.dot1agCfmStackifIndex;
  dot1agPktFromCpu.fromWire = OFDPA_FALSE;

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pMEPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  rc = dot1agMpTypeGet(pMEPObj, &mpType);
  if (rc != OFDPA_E_NONE)
  {
    return (rc);
  }

  if (mpType == OFDPA_DOT1AG_MP_TYPE_BHH)
  {
    /* MPLS-TP Tunnel OAM */
    /* Fill MPLS-TP header */
    priority = pMEPObj->mepData->dot1agCfmMepTransmitLbmVlanPriority;
    vlanId = oamMapData->vlanId;
    if (pMEPObj->mepData->dot1agCfmMepTransmitLbmDiscovery != OFDPA_OAM_LB_DISCOVERY_NONE)
    {
      len = mplsTpPrepareMplsHeader(oamMapData, priority, data, pMEPObj->mepData->dot1agCfmMepLbiVar.nextLbmDiscoveryTTL);
    }
    else
    {
      len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);
    }

    if (len == 0)
    {
      return OFDPA_E_FAIL;
    }

    data += len;
  }
  else
  {
    /* Ethernet OAM */
    /* Fill DMAC */
    if (pMEPObj->mepData->dot1agCfmMepTransmitLbmDestIsMepId == OFDPA_FALSE)
    {
      DOT1AG_PUT_DATA(pMEPObj->mepData->dot1agCfmMepTransmitLbmDestMacAddress, OFDPA_MAC_ADDR_LEN, data);
      memcpy(dot1agPktFromCpu.destMac, pMEPObj->mepData->dot1agCfmMepTransmitLbmDestMacAddress, OFDPA_MAC_ADDR_LEN);
    }
    else
    {
      if (pMEPObj->mepData->maData == NULL ||
          pMEPObj->mepData->maData->mdData == NULL)
      {
        DOT1AG_TRACE(CFM_LBM, "maData/mdData is NULL.\r\n");
        return OFDPA_E_FAIL;
      }

      /* Obtain the MAC from MEP DB */
      if ((pRMEPObj = dot1agCfmRMepObjGet(pMEPObj->mepData->maData->mdData->dot1agCfmMdIndex,
                                          pMEPObj->mepData->maData->dot1agCfmMaIndex,
                                          pMEPObj->mepData->dot1agCfmMepIdentifier,
                                          pMEPObj->mepData->dot1agCfmMepTransmitLbmDestMepId,
                                          AVL_EXACT)) == NULL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to transmit LBM. Invalid RMEP mdIndex %d maIndex %d rmepId %d",
                           pMEPObj->mepData->maData->mdData->dot1agCfmMdIndex, pMEPObj->mepData->maData->dot1agCfmMaIndex,
                           pMEPObj->mepData->dot1agCfmMepIdentifier, pMEPObj->mepData->dot1agCfmMepTransmitLbmDestMepId);
        /* Move the SM to Idle state */
        return OFDPA_E_FAIL;
      }
      DOT1AG_PUT_DATA(pRMEPObj->dot1agCfmRmepVar.rMepMacAddress, OFDPA_MAC_ADDR_LEN, data);
      memcpy(dot1agPktFromCpu.destMac, pRMEPObj->dot1agCfmRmepVar.rMepMacAddress, OFDPA_MAC_ADDR_LEN);
    }

    /* Fill SMAC */
    DOT1AG_PUT_DATA(pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN, data);
    memcpy(dot1agPktFromCpu.srcMac, pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

    /* Fill DOT1Q tag here, so that validations would go through in the case of relay logic.
     * This gets overwritten anyway with default TPID while transmitting onto the wire.
     */
    shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
    DOT1AG_PUT_SHORT(shortVal, data);

    /* Construct TCI */
    shortVal = 0;
    shortVal  = ((uint16_t)(pMEPObj->mepData->dot1agCfmMepTransmitLbmVlanPriority)) << 13;
    shortVal |= ((uint16_t)(pMEPObj->mepData->dot1agCfmMepTransmitLbmVlanDropEnable & 0x1)) << 12;
    shortVal |= ((uint16_t)pMEPObj->key.dot1agCfmStackVlanIdOrNone) & 0x0FFF;
    vlanId = pMEPObj->key.dot1agCfmStackVlanIdOrNone;
    DOT1AG_PUT_SHORT(shortVal, data);
  }

  dot1agPktFromCpu.vlanId   = pMEPObj->key.dot1agCfmStackVlanIdOrNone;

  /* setting dot1ag eth type / G-Ach Channel Type*/
  shortVal = OFDPA_DOT1AG_CFM_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);
  dot1agPktFromCpu.cfmPduOffset =  data;
  /* Fill CFM Common Header */
  cfmHdr.level   = pMEPObj->key.dot1agCfmStackMdLevel;
  cfmHdr.version = DOT1AG_CFM_PROTOCOL_VERSION_0;
  cfmHdr.opcode  = DOT1AG_LOOPBACK_MESSAGE_OPCODE;
  cfmHdr.flags   = 0;
  cfmHdr.first_tlv_offset = DOT1AG_CFM_LBM_FIRST_TLV_OFFSET;
  dot1agCfmHdrWrite(&cfmHdr, data);
  data += DOT1AG_CFM_COMMON_HDR_LEN;
  memcpy(&dot1agPktFromCpu.cfmHdr, &cfmHdr, sizeof(dot1agPktFromCpu.cfmHdr));

  /* Fill LBM header */
  uintVal = pMEPObj->mepData->dot1agCfmMepLbiVar.nextLbmTransId;
  DOT1AG_PUT_LONG(uintVal, data);
  data = dot1agPktFromCpu.cfmPduOffset + cfmHdr.first_tlv_offset + DOT1AG_CFM_COMMON_HDR_LEN;

  if (mpType == OFDPA_DOT1AG_MP_TYPE_BHH)
  {
    /* Fill Target MEP/MIP ID TLV */
    switch (pMEPObj->mepData->dot1agCfmMepTransmitLbmDiscovery)
    {
      case OFDPA_OAM_LB_DISCOVERY_NONE:
      case OFDPA_OAM_LB_DISCOVERY_ICC_MEPID:
        targetMepTlv.length = DOT1AG_TLV_FIELD_LEN_TARGET_MEP;
        targetMepTlv.subType = DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_ICC_MEPID; /* ICC-based MEP ID */
        targetMepTlv.MepId = pMEPObj->mepData->dot1agCfmMepTransmitLbmDestMepId;
        rc = dot1agCfmTargetMepTlvWrite(&targetMepTlv, data, &len);
        break;
      case OFDPA_OAM_LB_DISCOVERY_INGRESS:
        targetMepTlv.length = DOT1AG_TLV_FIELD_LEN_TARGET_MEP;
        targetMepTlv.subType = DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_INGRESS;
        targetMepTlv.MepId = 0;
        rc = dot1agCfmTargetMepTlvWrite(&targetMepTlv, data, &len);
        break;
      case OFDPA_OAM_LB_DISCOVERY_EGRESS:
        targetMepTlv.length = DOT1AG_TLV_FIELD_LEN_TARGET_MEP;
        targetMepTlv.subType = DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_EGRESS;
        targetMepTlv.MepId = 0;
        rc = dot1agCfmTargetMepTlvWrite(&targetMepTlv, data, &len);
        break;
      default:
        return (OFDPA_E_PARAM);
    }

    if (rc == OFDPA_E_NONE)
    {
      data += len;
    }
    else
    {
      return rc;
    }

    /* optional Requesting MEP ID TLV */
  }
  else
  {
    /* optional Sender ID TLV */
    if (dot1agCfmSenderIdTlvBuild(pMEPObj, &senderIdTlv) == OFDPA_E_NONE)
    {
      rc = dot1agCfmSenderIdTlvWrite(&senderIdTlv, data, &len);

      if (rc == OFDPA_E_NONE)
      {
        data += len;
      }
      else
      {
        return rc;
      }
    }
  }

  if (pMEPObj->mepData->dot1agCfmMepTransmitLbmDiscovery == OFDPA_OAM_LB_DISCOVERY_NONE)
  {
    /* optional test or data TLV */
    if (pMEPObj->mepData->dot1agCfmMepTransmitLbmTestTlvPresent == OFDPA_TRUE)
    {
      if ((pMEPObj->mepData->dot1agCfmMepTransmitLbmPacketLength - (data - dot1agPktFromCpu.payLoad) - 4) < 0)
      {
        return OFDPA_E_FAIL;
      }

      /* Subtract the packet length formed upto now and test tlv header (3)  + end tlv (1) */
      testTlv.length = pMEPObj->mepData->dot1agCfmMepTransmitLbmPacketLength - (data - dot1agPktFromCpu.payLoad) - 4;
      if (testTlv.length > DOT1AG_TEST_TLV_VALUE_LEN_MAX)
      {
        return OFDPA_E_FAIL;
      }
      testTlv.patternType = pMEPObj->mepData->dot1agCfmMepTransmitLbmTestTlvType;

      rc = dot1agCfmTestTlvWrite(&testTlv, data, &len);

      if (rc == OFDPA_E_NONE)
      {
        data += len;
      }
      else
      {
        return rc;
      }
    }
    else
    {
      if ((pMEPObj->mepData->dot1agCfmMepTransmitLbmPacketLength - (data - dot1agPktFromCpu.payLoad)) < 0)
      {
        return OFDPA_E_FAIL;
      }

      /* Subtract the packet length formed upto now and data tlv header (3) + end tlv (1) */
      dataTlv.length = pMEPObj->mepData->dot1agCfmMepTransmitLbmPacketLength - (data - dot1agPktFromCpu.payLoad) - 4;
      if (dataTlv.length > DOT1AG_DATA_TLV_VALUE_LEN_MAX)
      {
        return OFDPA_E_FAIL;
      }

      rc = dot1agCfmLbmDataTlvWrite(&dataTlv, data, &len);

      if (rc == OFDPA_E_NONE)
      {
        data += len;
      }
      else
      {
        return rc;
      }
    }
  }

  /* End TLV */
  rc = dot1agCfmEndTlvWrite(data, &len);

  if (rc == OFDPA_E_NONE)
  {
    data += len;
  }
  else
  {
    return rc;
  }

  dot1agPktFromCpu.length = data - dot1agPktFromCpu.payLoad;

  dot1agPktFromCpu.lmepId    = pMEPObj->ofdpaLocalMpId;
  dot1agPktFromCpu.oamOpcode = DOT1AG_LOOPBACK_MESSAGE_OPCODE;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPktFromCpu.length, dot1agPktFromCpu.payLoad);
  /* Put the pkt on wire */
  (void)dot1agPduTransmit(pMEPObj->key.dot1agCfmStackifIndex,
                          vlanId,
                          &dot1agPktFromCpu);

  /* Send the LBM PDU on the wire */
  /* Increment nextLBMtransID */
  pMEPObj->mepData->dot1agCfmMepLbiVar.nextLbmTransId++;
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Transmit LTM on requested MEP
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @notes    802.1ag d8-1 20.37.1, 21.8
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitLTM(dot1agCfmStackObj_t *pMEPObj)
{
  uint8_t                   *data, byteVal;
  dot1agCfmRMepObj_t        *pRMEPObj = NULL;
  dot1agCfmCommonHdr_t       cfmHdr;
  uint16_t                   shortVal;
  uint32_t                   uintVal;
  dot1agCfmLtrObj_t          ltrEntry;
  dot1agCfmSenderIdTLV_t     senderIdTlv;
  dot1agCfmLTMEgressIdTLV_t  ltmEgressIdTLV;
  uint32_t                   length = 0;
  OFDPA_ERROR_t              rc = OFDPA_E_NONE;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  memset(&dot1agPktFromCpu, 0x0, sizeof(dot1agPktFromCpu));
  data = dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.vlanId   = pMEPObj->key.dot1agCfmStackVlanIdOrNone;
  dot1agPktFromCpu.intIfNum = pMEPObj->key.dot1agCfmStackifIndex;
  dot1agPktFromCpu.fromWire = OFDPA_FALSE;
  dot1agPktFromCpu.fromOwnUpMEP = OFDPA_FALSE;

  /* Fill DMAC */
  /* Sets the destination_address parameter to the value from
     Table 8-10 corresponding to MEP.s MD Level; */
  DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);
  byteVal = dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | (pMEPObj->key.dot1agCfmStackMdLevel + 8);
  DOT1AG_PUT_BYTE(byteVal, data);
  memcpy(dot1agPktFromCpu.destMac, dot1agPktFromCpu.payLoad, OFDPA_MAC_ADDR_LEN);

  /* Fill SMAC */
  DOT1AG_PUT_DATA(pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN, data);
  memcpy(dot1agPktFromCpu.srcMac, pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

  /* Fill DOT1Q tag here, so that validations would go through in the case of relay logic.
   * This gets overwritten anyway with default TPID while transmitting onto the wire.
   */
  shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);

  /* Construct TCI */
  shortVal = 0;
#ifdef NOT_SUPPORTED
  /* Sets the priority parameter according to the MEP.s managed objects */
  shortVal  = ((uint16_t)(pMEPObj->mepData->dot1agCfmMepCcmLtmPriority)) << 13;
#endif
  shortVal |= ((uint16_t)pMEPObj->key.dot1agCfmStackVlanIdOrNone) & 0x0FFF;
  DOT1AG_PUT_SHORT(shortVal, data);

  /* Fill Ether Type */
  shortVal = OFDPA_DOT1AG_CFM_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);
  dot1agPktFromCpu.cfmPduOffset =  data;

  /* Fill CFM Common Header */
  cfmHdr.level   = pMEPObj->key.dot1agCfmStackMdLevel;
  cfmHdr.version = DOT1AG_CFM_PROTOCOL_VERSION_0;
  cfmHdr.opcode  = DOT1AG_LINKTRACE_MESSAGE_OPCODE;
  cfmHdr.flags   = pMEPObj->mepData->dot1agCfmMepTransmitLtmFlags;
  cfmHdr.first_tlv_offset = DOT1AG_CFM_LTM_FIRST_TLV_OFFSET;
  dot1agCfmHdrWrite(&cfmHdr, data);
  data += DOT1AG_CFM_COMMON_HDR_LEN;
  memcpy(&dot1agPktFromCpu.cfmHdr, &cfmHdr, sizeof(dot1agPktFromCpu.cfmHdr));

  /* Fill LTM header 802.1ag d8-1 21.8*/
  /* LTM Transaction Identifier */
  uintVal = dot1agCFM.dot1agCfmLtmrVar.nextLtmTransId;
  DOT1AG_PUT_LONG(uintVal, data);
  /* LTM TTL */
  byteVal = pMEPObj->mepData->dot1agCfmMepTransmitLtmTtl;
  DOT1AG_PUT_BYTE(byteVal, data);
  /* Original MAC Address */
  DOT1AG_PUT_DATA(pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN, data);

  /* Target MAC Address */
  if (pMEPObj->mepData->dot1agCfmMepTransmitLtmTargetIsMepId == OFDPA_FALSE)
  {
    DOT1AG_PUT_DATA(pMEPObj->mepData->dot1agCfmMepTransmitLtmTargetMacAddress, OFDPA_MAC_ADDR_LEN, data);
  }
  else
  {
    if (pMEPObj->mepData->maData == NULL ||
        pMEPObj->mepData->maData->mdData == NULL)
    {
      DOT1AG_TRACE(CFM_LTM, "maData/mdData is NULL.\r\n");
      return OFDPA_E_FAIL;
    }

    /* Obtain the MAC from MEP DB */
    if ((pRMEPObj = dot1agCfmRMepObjGet(pMEPObj->mepData->maData->mdData->dot1agCfmMdIndex,
                                        pMEPObj->mepData->maData->dot1agCfmMaIndex,
                                        pMEPObj->mepData->dot1agCfmMepIdentifier,
                                        pMEPObj->mepData->dot1agCfmMepTransmitLtmTargetMepId,
                                        AVL_EXACT)) == NULL)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to transmit LTM. Invalid RMEP mdIndex %d maIndex %d rmepId %d",
                         pMEPObj->mepData->maData->mdData->dot1agCfmMdIndex, pMEPObj->mepData->maData->dot1agCfmMaIndex,
                         pMEPObj->mepData->dot1agCfmMepIdentifier, pMEPObj->mepData->dot1agCfmMepTransmitLtmTargetMepId);
      return OFDPA_E_FAIL;
    }
    DOT1AG_PUT_DATA(pRMEPObj->dot1agCfmRmepVar.rMepMacAddress, OFDPA_MAC_ADDR_LEN, data);
  }

  data = dot1agPktFromCpu.cfmPduOffset + cfmHdr.first_tlv_offset + DOT1AG_CFM_COMMON_HDR_LEN;
  /* LTM Egress Identifier TLV */
  if (dot1agCfmLTMEgressIdTlvBuild(pMEPObj, &ltmEgressIdTLV) == OFDPA_E_NONE)
  {
    rc = dot1agCfmLTMEgressIdentifierTlvWrite(&ltmEgressIdTLV, data, &length);

    if (rc == OFDPA_E_NONE)
    {
      data += length;
    }
    else
    {
      return rc;
    }
  }
  /* Sender ID TLV */
  if (dot1agCfmSenderIdTlvBuild(pMEPObj, &senderIdTlv) == OFDPA_E_NONE)
  {
    rc = dot1agCfmSenderIdTlvWrite(&senderIdTlv, data, &length);

    if (rc == OFDPA_E_NONE)
    {
      data += length;
    }
    else
    {
      return rc;
    }
  }
  /* End TLV */
  rc = dot1agCfmEndTlvWrite(data, &length);

  if (rc == OFDPA_E_NONE)
  {
    data += length;
  }
  else
  {
    return rc;
  }

  dot1agPktFromCpu.length = data - dot1agPktFromCpu.payLoad;

  /* Creates a new entry in the ltmReplyList variable for this LTM, identified by the LTM
     Transaction Identifier in nextLTMtransID */
  memset(&ltrEntry, 0x0, sizeof(ltrEntry));

  ltrEntry.key.dot1agCfmMdIndex         = pMEPObj->mepData->maData->mdData->dot1agCfmMdIndex;
  ltrEntry.key.dot1agCfmMaIndex         = pMEPObj->mepData->maData->dot1agCfmMaIndex;
  ltrEntry.key.dot1agCfmMepIdentifier   = pMEPObj->mepData->dot1agCfmMepIdentifier;
  ltrEntry.key.dot1agCfmLtrSeqNumber    = dot1agCFM.dot1agCfmLtmrVar.nextLtmTransId;
  ltrEntry.key.dot1agCfmLtrReceiveOrder = 0; /* First Entry denotes the LTM issued */
  ltrEntry.dot1agCfmLtrTtl              = pMEPObj->mepData->dot1agCfmMepTransmitLtmTtl;

  if (dot1agLtrEntryInsert(&ltrEntry) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  dot1agPktFromCpu.lmepId    = pMEPObj->ofdpaLocalMpId;
  dot1agPktFromCpu.oamOpcode = DOT1AG_LINKTRACE_MESSAGE_OPCODE;

  /* Send the LTM PDU on the wire */
  (void)dot1agPduTransmit(pMEPObj->key.dot1agCfmStackifIndex,
                          pMEPObj->key.dot1agCfmStackVlanIdOrNone,
                          &dot1agPktFromCpu);

  pMEPObj->mepData->dot1agCfmMepTransmitLtmSeqNumber = dot1agCFM.dot1agCfmLtmrVar.nextLtmTransId;
  pMEPObj->mepData->dot1agCfmMepTransmitLtmResult = OFDPA_TRUE;
  /* Increment nextLTMtransID */
  dot1agCFM.dot1agCfmLtmrVar.nextLtmTransId++;
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Parse LTM Pdu and store int in local CFM PDU message
*           storage
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 21.8
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agParseLTM(dot1agControlPkt_t *dot1agPkt)
{
  uint8_t  *data, type;
  uint16_t  length;

  /* Point to CFM PDU */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;
  /* 802.1ag d8-1 21.8.2 */
  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_LTM_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }

  dot1agPkt->cfmPdu.ltm.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;

  /* LTM Transaction Identifier 802.1ag d8-1 21.8.3 */
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.ltm.ltmTransId, data);
  /* LTM TTL 802.1ag d8-1 21.8.4 */
  DOT1AG_GET_BYTE(dot1agPkt->cfmPdu.ltm.ttl, data);
  /* Original MAC Address 802.1ag d8-1 21.8.5 */
  DOT1AG_GET_DATA(dot1agPkt->cfmPdu.ltm.orgMacAddr, OFDPA_MAC_ADDR_LEN, data);
  /* Validation Test: The Original MAC Address field contains an Individual,
     and not a Group, MAC address. */
  if (dot1agPkt->cfmPdu.ltm.orgMacAddr[0] & 0x01)
  {
    return OFDPA_E_FAIL;
  }
  /* Target MAC Address 802.1ag d8-1 21.8.6 */
  DOT1AG_GET_DATA(dot1agPkt->cfmPdu.ltm.targetMacAddr, OFDPA_MAC_ADDR_LEN, data);
  /* Validation Test: The Target MAC Address field contains an Individual,
     and not a Group, MAC address. */
  if (dot1agPkt->cfmPdu.ltm.targetMacAddr[0] & 0x01)
  {
    return OFDPA_E_FAIL;
  }

  dot1agPkt->cfmPdu.ltm.useFDBOnly  = DOT1AG_LTR_FLAGS_USE_FDB_GET(dot1agPkt->cfmHdr.flags);

  /* Parse available TLVs */
  data = dot1agPkt->cfmPdu.ltm.pFirstTLV;

  DOT1AG_GET_BYTE(type, data);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);
    switch (type)
    {
      /* Sender ID TLV */
      case DOT1AG_TLV_TYPE_SENDER_ID:
        dot1agPkt->cfmPdu.ltm.senderIdTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.ltm.pSenderIdTLV = data - 3;
        dot1agPkt->cfmPdu.ltm.senderIdTLV.length = length;
        if (dot1agCfmSenderIdTlvRead(&dot1agPkt->cfmPdu.ltm.senderIdTLV,
                                     data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        break;
        /* LTM Egress Identifier TLV */
      case DOT1AG_TLV_TYPE_LTM_EGR_ID:
        dot1agPkt->cfmPdu.ltm.ltmEgressIdTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.ltm.pltmEgressIdTLV    = data - 3;
        dot1agPkt->cfmPdu.ltm.ltmEgressIdTLV.length = length;
        if (dot1agCfmLTMEgressIdTlvRead(&dot1agPkt->cfmPdu.ltm.ltmEgressIdTLV,
                                        data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        break;
        /* Org TLV */
      case DOT1AG_TLV_TYPE_ORG_SPECIFIC:
        /* Ignore all other TLVs */
      default:
        break;
    }
    data += length;
    DOT1AG_GET_BYTE(type, data);
  }
  dot1agPkt->cfmPdu.ltm.pLastTLV = data - 1;
  return OFDPA_E_NONE;
}

/* LTMR Methods */
/*********************************************************************
* @purpose   Method called by the LTMR SM to process the received LTM
*            message
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.42.1
*
* @end
**********************************************************************/
OFDPA_ERROR_t ProcessLTM(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  DOT1AG_TRACE(CFM_LTM, "Start Processing LTM received on interface: %d\r\n", dot1agPkt->intIfNum);

  if (pMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* Check that dest_mac is group address. If group address, it should
     follow table 8-10. If not a group address, discard. */
  if (!(memcmp(dot1agRsrvdMac, dot1agPkt->destMac, OFDPA_MAC_ADDR_LEN-1) == 0 &&
        dot1agPkt->destMac[OFDPA_MAC_ADDR_LEN-1] ==
        (dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | (pMPObj->key.dot1agCfmStackMdLevel + 8))))
  {
    dot1agPkt->toFwd = OFDPA_FALSE;
    return OFDPA_E_NONE;
  }

  /* Parse LTM message */
  if (dot1agParseLTM(dot1agPkt) != OFDPA_E_NONE)
  {
    dot1agPkt->toFwd = OFDPA_FALSE;
    return OFDPA_E_FAIL;
  }

  /* The LTM TTL field of the received LTM is examined. If its value is 0, the LTM
     is invalid */
  if (dot1agPkt->cfmPdu.ltm.ttl == 0)
  {
    dot1agPkt->toFwd = OFDPA_FALSE;
    return OFDPA_E_NONE;
  }

  /* MEP */
  if (pMPObj->mepData != NULL)
  {
    if (pMPObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP &&
        dot1agPkt->fromOwnUpMEP == OFDPA_TRUE)
    {
      ProcessLTMRxDownMHFTxUpMEP(dot1agPkt, pMPObj);
    }
    else if (pMPObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP)
    {
      ProcessLTMRxUpMEP(dot1agPkt, pMPObj);
    }
    else
    {
      ProcessLTMRxDownMEP(dot1agPkt, pMPObj);
    }
  }
  else
  {
    /* MHF */
    if (dot1agPkt->fromWire == OFDPA_TRUE)
    {
      ProcessLTMRxDownMHFTxUpMEP(dot1agPkt, pMPObj);
    }
    else
    {
      ProcessLTMRxUpMHF(dot1agPkt, pMPObj);
    }
  }
  /* Enqueing and Forwarding of LTM is handled by this time. So, Consume the packet */
  dot1agPkt->toFwd = OFDPA_FALSE;
  /* Conditionally call ForwardLTM() */
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Method called by the ProcessLTM to determine egress port
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    rxpMPObj   @b{(input)} CMF Stack MP on which LTM was
*                                  received
* @param    txIntIfNum @b{(input)} Egress port to which LTM can be
*                                  forwarded
*
* @returns  OFDPA_E_NONE  On successful determination of egress port
*           OFDPA_E_FAIL  Could not uniquely determine an egress port.
*                       LTM should be dropped.
*
* @notes    802.1ag d8-1 20.42.1.2, 20.3.2
*           Current implementation uses only FDB for determining the
*           egress ports because we do not support MIP CCM database.
*
* @end
**********************************************************************/
OFDPA_ERROR_t ProcessLTMEgressIdFind(dot1agControlPkt_t *dot1agPkt,
                                     dot1agCfmStackObj_t *rxpMPObj,
                                     uint32_t *txIntIfnum)
{
#if OFDPA_NOT_SUPPORTED
  dot1dTpFdbData_t data;
  uint16_t         shortVal;
  uint8_t          macBuff[L7_FDB_KEY_SIZE];
  uint32_t         peerIntf;

  if (OFDPA_E_NONE == mplsTpOamMipPeerIntfGet(rxpMPObj->key.dot1agCfmStackifIndex,
                                              rxpMPObj->key.dot1agCfmStackVlanIdOrNone, &peerIntf))
  {
    *txIntIfnum = peerIntf;
    return OFDPA_E_NONE;
  }
  DOT1AG_TRACE(CFM_LTM, "Failed to get peer port.\r\n");
  return OFDPA_E_FAIL;

  /* a)
     ProcessLTM() first queries the Filtering Database (8.8). The set of
     potential transmission ports, normally created by Active topology
     enforcement (8.6.1), is the set of all Bridge Ports that are both
     in the active set of the vlan_identifier of the LTM, and that are in
     the Forwarding state for that vlan_identifier, except that the Ingress
     Port is excluded from the set. The query uses the Target MACAddress
     field of the LTM as the destination_address of the lookup, the Original
     MAC Address field of the LTM as the source_address, and the vlan_identifier
     of the LTM. The output from this query is a (perhaps reduced) set of potential
     transmission ports. If the resultant set contains one and only one
     Bridge Port, that Bridge Port is the Egress Port, and step b is not performed.
  */

  shortVal = rxpMPObj->key.dot1agCfmStackVlanIdOrNone;
  memcpy(macBuff, &shortVal, L7_MFDB_VLANID_LEN);
  memcpy(&macBuff[L7_MFDB_VLANID_LEN], dot1agPkt->cfmPdu.ltm.targetMacAddr, OFDPA_MAC_ADDR_LEN);

  memset(&data, 0x0, sizeof(data));
  if (fdbFind(macBuff, L7_MATCH_EXACT, &data) == OFDPA_E_NONE &&
      memcmp(macBuff, data.dot1dTpFdbAddress, L7_FDB_KEY_SIZE) == 0)
  {
    if ((dot1agPkt->fromWire == OFDPA_FALSE) ||
        ((dot1agPkt->fromWire == OFDPA_TRUE) && (data.dot1dTpFdbPort != rxpMPObj->key.dot1agCfmStackifIndex)))
    {
      /* Destination interface */
      *txIntIfnum = data.dot1dTpFdbPort;
      return OFDPA_E_NONE;
    }
  }

  /* b) ***( Not Supported as MIP CCM database is not implemented )***
     If the Filtering Database could not produce a unique Egress Port, and the MPs
     serving the vlan_identifier of the LTM are maintaining a MIP CCM Database, and
     the UseFDBonly bit of the Flags field of the LTM is 0, then ProcessLTM() queries
     the MIP CCM Database to see whether the target MAC address and vlan_identifier have
     been retained in that database. If so, and if the Port number in the MIP CCM Database
     is not the same as the Ingress Port, that Port number identifies the Egress Port.
  */
  return OFDPA_E_FAIL;
#endif
  return OFDPA_E_FAIL;
}

/*********************************************************************
* @purpose   Method called by the ProcessLTM to conditionally forward
*            received LTM. The LTM was received on a Down MEP.
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.42.1.3
*
* @end
**********************************************************************/
void ProcessLTMRxDownMEP(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  uint32_t txIntIfNum, vlanId;

  if (pMPObj == NULL)
  {
    return;
  }

  /* If the Target MAC Address carried in the LTM is the MAC address of the receiving MEP,
     then the LTM has reached its target. ProcessLTM() calls enqueLTR() (20.42.4) to
     enqueue an LTR for the MEP Linktrace SAP of the receiving Down MEP */
  if (memcmp(pMPObj->mpMacAddress, dot1agPkt->cfmPdu.ltm.targetMacAddr, OFDPA_MAC_ADDR_LEN) == 0)
  {
    DOT1AG_TRACE(CFM_LTM, "Target MAC Address matched\r\n");
    enqueLTR(dot1agPkt, pMPObj, OFDPA_FALSE, DOT1AG_CFM_DOWN_MEP,
             DOT1AG_CFM_DOWN_MEP, dot1agPkt->intIfNum, dot1agPkt->intIfNum);
    return;
  }

  /* Otherwise If the spanning tree state of the Bridge Port and vlan_identifier of the
     LTM is not Forwarding, the LTM is discarded and no further processing takes place */
  vlanId = pMPObj->key.dot1agCfmStackVlanIdOrNone;
  if (dot1agCfmIntfVlanIsForwarding(pMPObj->key.dot1agCfmStackifIndex,
                                    vlanId) == OFDPA_FALSE)
  {
    return;
  }

  /* Otherwise, the Ingress and Egress Ports are determined according to 20.42.1.2.  If a
     unique Egress Port cannot be determined, then the LTM is discarded, and no further
     processing takes place. */
  if (ProcessLTMEgressIdFind(dot1agPkt, pMPObj, &txIntIfNum) != OFDPA_E_NONE)
  {
    return;
  }
  /* Otherwise, i.e., a unique Egress Port was found, ProcessLTM() calls enqueLTR()
     (20.42.4) to enqueue an LTR for the Linktrace SAP through which the LTM was
     received. The LTM is discarded, and no further processing takes place */
  DOT1AG_TRACE(CFM_LTM, "Ingress interface: %d Egress Interface: %d found.\r\n",
               dot1agPkt->intIfNum, txIntIfNum);
  enqueLTR(dot1agPkt, pMPObj, OFDPA_FALSE, DOT1AG_CFM_DOWN_MEP,
           DOT1AG_CFM_DOWN_MEP, dot1agPkt->intIfNum, txIntIfNum);
}

/*********************************************************************
* @purpose   Method called by the ProcessLTM to conditionally forward
*            received LTM. The LTM was received on a Down MHF or was
*            initiated by an Up MEP.
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.42.1.4
*
* @end
**********************************************************************/
void ProcessLTMRxDownMHFTxUpMEP(dot1agControlPkt_t *dot1agPkt,
                                dot1agCfmStackObj_t *pMPObj)
{
  uint32_t             txIntIfNum, vlanId, direction, level;
  dot1agCfmStackObj_t *pTmpMPObj = NULL;
  Dot1agCfmMPType_t    rxMPType;

  if (pMPObj == NULL)
  {
    return;
  }

  vlanId = pMPObj->key.dot1agCfmStackVlanIdOrNone;
  /* If the LTM was generated by an Up MEP, following two steps are skipped */
  if (dot1agPkt->fromOwnUpMEP == OFDPA_FALSE)
  {
    rxMPType = DOT1AG_CFM_DOWN_MHF;
    if (memcmp(pMPObj->mpMacAddress, dot1agPkt->cfmPdu.ltm.targetMacAddr, OFDPA_MAC_ADDR_LEN) == 0)
    {
      DOT1AG_TRACE(CFM_LTM, "Target MAC Address matched.\r\n");
      enqueLTR(dot1agPkt, pMPObj, OFDPA_FALSE, DOT1AG_CFM_DOWN_MHF,
               rxMPType, dot1agPkt->intIfNum,
               dot1agPkt->intIfNum);
      return;
    }

    /* Otherwise, if the spanning tree state of the Bridge Port and vlan_identifier of the
       LTM is not Forwarding, the LTM is discarded and no further processing takes
       place. */
    if (dot1agCfmIntfVlanIsForwarding(pMPObj->key.dot1agCfmStackifIndex,
                                      vlanId) == OFDPA_FALSE)
    {
      return;
    }
  }
  else
  {
    rxMPType = DOT1AG_CFM_UP_MEP;
  }

  /* Otherwise, the Ingress and Egress Ports are determined according to 20.42.1.2. If a
     unique Egress Port cannot be determined, then the LTM is discarded, and no further
     processing takes place. */
  if (ProcessLTMEgressIdFind(dot1agPkt, pMPObj, &txIntIfNum) != OFDPA_E_NONE)
  {
    DOT1AG_TRACE(CFM_LTM, "Egress interface not found.\r\n");
    return;
  }

  DOT1AG_TRACE(CFM_LTM, "Egress interface: %d found.\r\n", txIntIfNum);
  /* There are three cases for further processing of the LTM, depending on whether, as it
     passes through the Egress Port, a frame with the LTM.s vlan_identifier would first
     encounter an Up MEP, an Up MHF, or neither. These described in case f, case g, case
     h, and case i, following. */

  level = 0;
  direction = 0;
  while ((pTmpMPObj = dot1agCfmStackObjGet(txIntIfNum, vlanId, level,
                                           direction, AVL_NEXT)) != NULL)
  {
    if (pTmpMPObj->key.dot1agCfmStackifIndex  != txIntIfNum ||
        pTmpMPObj->key.dot1agCfmStackVlanIdOrNone != vlanId)
    {
      pTmpMPObj = NULL;
      break;
    }

    /* We have a MEP */
    if (pTmpMPObj->mepData != NULL)
    {
      /* Check if MEP is of same level as ingress MP */
      if (pTmpMPObj->key.dot1agCfmStackMdLevel == pMPObj->key.dot1agCfmStackMdLevel &&
          pTmpMPObj->operMode == OFDPA_TRUE &&
          pTmpMPObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP)
      {
        break;
      }
    }
    else
    {
      /* we have a MIP. Check if it is of same or lower level than ingress MP level */
      if (pTmpMPObj->key.dot1agCfmStackMdLevel <= pMPObj->key.dot1agCfmStackMdLevel)
      {
        break;
      }
    }
    level     = pTmpMPObj->key.dot1agCfmStackMdLevel;
    direction = pTmpMPObj->key.dot1agCfmStackDirection;
  }/* End of stack object iterations */


  /* f) If an Up MHF would be encountered on the Egress Port, then: */
  if (pTmpMPObj != NULL && pTmpMPObj->mepData == NULL)
  {
    /*   1) ProcessLTM() calls enqueLTR() (20.42.4) to enqueue an LTR for the MHF Linktrace SAP of
         the Egress Port.s Up MHF. */
    /*   2) If the target MAC address carried in the LTM is the Egress Port Up MHF.s MAC address, then
         the LTM is discarded, and no further processing takes place. */
    if (memcmp(pMPObj->mpMacAddress, dot1agPkt->cfmPdu.ltm.targetMacAddr, OFDPA_MAC_ADDR_LEN) == 0)
    {
      DOT1AG_TRACE(CFM_LTM, "Target MAC Address matched with Egress MHF.\r\n");
      enqueLTR(dot1agPkt, pTmpMPObj, OFDPA_FALSE, DOT1AG_CFM_UP_MHF,
               rxMPType, dot1agPkt->intIfNum, txIntIfNum);
      return;
    }

    /*   3) Otherwise, if the LTM TTL field equals 0 or 1, the LTM is discarded and no
         further processing takes place. */
    if (dot1agPkt->cfmPdu.ltm.ttl == 0 || dot1agPkt->cfmPdu.ltm.ttl == 1)
    {
      DOT1AG_TRACE(CFM_LTM, "Egress MFH found. Ingress intf: %d Egress intf: %d. Do not forward LTM as TTL value is 0 or 1.\r\n", dot1agPkt->intIfNum, txIntIfNum);
      enqueLTR(dot1agPkt, pTmpMPObj, OFDPA_FALSE, DOT1AG_CFM_UP_MHF,
               rxMPType, dot1agPkt->intIfNum, txIntIfNum);
      return;
    }
    DOT1AG_TRACE(CFM_LTM, "Egress MFH found. Ingress intf: %d Egress intf: %d. Forward LTM.\r\n", dot1agPkt->intIfNum, txIntIfNum);
    enqueLTR(dot1agPkt, pTmpMPObj, OFDPA_TRUE, DOT1AG_CFM_UP_MHF,
             rxMPType, dot1agPkt->intIfNum, txIntIfNum);
    /*   4) Otherwise, ProcessLTM() calls ForwardLTM() (20.42.3) to forward an altered
         copy of the LTM through the LOM Linktrace SAP of the LOM on the identified Egress
         Port.*/
    ForwardLTM(dot1agPkt, pTmpMPObj);
    return;
  }

  /* g) If an Up MEP at the MD Level of the LTM would be encountered on the Egress Port, then: */
  if (pTmpMPObj != NULL && pTmpMPObj->mepData != NULL)
  {
    /*   1) ProcessLTM() calls enqueLTR() (20.42.4) to enqueue an LTR for the MHF
         Linktrace SAP of the Ingress Port.s Down MHF, or the MEP LTI SAP of the Up MEP,
         that delivered the LTM to the Linktrace Responder. */
    DOT1AG_TRACE(CFM_LTM, "Egress UP MEP found. Ingress intf: %d Egress intf: %d. Do not forward LTM.\r\n", dot1agPkt->intIfNum, txIntIfNum);
    enqueLTR(dot1agPkt, pMPObj, OFDPA_FALSE, DOT1AG_CFM_DOWN_MHF,
             rxMPType, dot1agPkt->intIfNum, txIntIfNum);

    /*   2) The LTM is discarded, and no further processing takes place. */
    return;
  }

  /* h) Otherwise, if an Up MEP higher than the MD Level of the LTM would be encountered
     on the Egress Port, then the LTM is discarded, and no further processing takes
     place. */
  if (pTmpMPObj == NULL)
  {
    level     = pMPObj->key.dot1agCfmStackMdLevel + 1;
    direction = 0;
    while ((pTmpMPObj = dot1agCfmStackObjGet(txIntIfNum, vlanId,
                                             level, direction, AVL_NEXT))
           != NULL)
    {
      if (txIntIfNum != pTmpMPObj->key.dot1agCfmStackifIndex ||
          vlanId     != pTmpMPObj->key.dot1agCfmStackVlanIdOrNone)
      {
        break;
      }

      if (pTmpMPObj->operMode == OFDPA_TRUE &&
          pTmpMPObj->key.dot1agCfmStackDirection == DOT1AG_MP_DIRECTION_UP &&
          pTmpMPObj->mepData != NULL &&
          pTmpMPObj->key.dot1agCfmStackMdLevel > pMPObj->key.dot1agCfmStackMdLevel)
      {
        /* Found an UP MEP at a higher level. Discard the LTM. */
        return;
      }

      direction = pTmpMPObj->key.dot1agCfmStackDirection;
      level = pTmpMPObj->key.dot1agCfmStackMdLevel;
    }/* Loop thru the CFM Stack objects */
  }/*End of if no MP object was found on egress port */

  /* i) If neither an Up MHF, nor an Up MEP at an MD Level higher than or equal to the LTM, would be
     encountered on the Egress Port, then:
     1) ProcessLTM() calls enqueLTR() (20.42.4) to enqueue an LTR for the MHF Linktrace SAP of
     the Ingress Port.s Down MHF. */
  DOT1AG_TRACE(CFM_LTM, "Neither an UP MHF or UP MEP at higher MD level found.\r\n");
  enqueLTR(dot1agPkt, pMPObj, OFDPA_FALSE, DOT1AG_CFM_DOWN_MHF,
           rxMPType, dot1agPkt->intIfNum, 0);


  /*  2) If the LTM TTL field equals 0 or 1, the LTM is discarded and no further
   *  processing takes place. */
  if (dot1agPkt->cfmPdu.ltm.ttl == 0 || dot1agPkt->cfmPdu.ltm.ttl == 1)
  {
    return;
  }
  /*  3) Otherwise, ProcessLTM() calls ForwardLTM() (20.42.3) to forward an altered copy
      of the LTM through the LOM Linktrace SAP of the LOM that a frame with the LTM.s
      vlan_identifier would first encounter when passing out through the identified Egress
      Port. */

  /* Found a MP with first lower domain level on the egress interface.*/
  level  = pMPObj->key.dot1agCfmStackMdLevel - 1;
  direction = 0;
  while ((pTmpMPObj = dot1agCfmStackObjGet(txIntIfNum, vlanId,
                                           level, direction, AVL_NEXT))
         != NULL)
  {
    if (txIntIfNum != pTmpMPObj->key.dot1agCfmStackifIndex ||
        vlanId     != pTmpMPObj->key.dot1agCfmStackVlanIdOrNone)
    {
      break;
    }
    if (pTmpMPObj->operMode == OFDPA_TRUE)
    {
      ForwardLTM(dot1agPkt, pTmpMPObj);
      return;
    }
    direction = pTmpMPObj->key.dot1agCfmStackDirection;
    level = pTmpMPObj->key.dot1agCfmStackMdLevel;
  }
  return;
}

/*********************************************************************
* @purpose   Method called by the ProcessLTM to conditionally forward
*            received LTM. The LTM was received on an Up MEP.
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.42.1.5
*
* @end
**********************************************************************/
void ProcessLTMRxUpMEP(dot1agControlPkt_t *dot1agPkt,
                       dot1agCfmStackObj_t *pMPObj)
{
  uint32_t txIntIfNum;

  if (pMPObj == NULL)
  {
    return;
  }

  /* If the Target MAC Address carried in the LTM is the MAC address of the receiving MEP,
     then the LTM has reached its target. ProcessLTM() calls enqueLTR() (20.42.4) to
     enqueue an LTR for the MEP Linktrace SAP of the receiving Up MEP. The LTM is
     discarded, and no further processing takes place. */
  if (memcmp(pMPObj->mpMacAddress, dot1agPkt->cfmPdu.ltm.targetMacAddr, OFDPA_MAC_ADDR_LEN) == 0)
  {
    DOT1AG_TRACE(CFM_LTM, "Target MAC address found.\r\n");
    enqueLTR(dot1agPkt, pMPObj, OFDPA_FALSE, DOT1AG_CFM_UP_MEP,
             DOT1AG_CFM_UP_MEP, dot1agPkt->intIfNum,
             pMPObj->key.dot1agCfmStackifIndex);
    return;
  }

  /* Otherwise, the Ingress and Egress Ports are determined according to 20.42.1.2. If
     the Egress Port cannot be determined, or if the Egress Port is not the Bridge Port
     on which the receiving MEP is configured, then the LTM is discarded and no further
     processing takes place. */
  if (ProcessLTMEgressIdFind(dot1agPkt, pMPObj, &txIntIfNum) != OFDPA_E_NONE ||
      txIntIfNum != pMPObj->key.dot1agCfmStackifIndex)
  {
    return;
  }

  /* Otherwise, (i.e., the Egress Port is that of the receiving Up MEP) ProcessLTM() calls
     enqueLTR()(20.42.4) to enqueue an LTR for the MEP Linktrace SAP of the receiving Up
     MEP. The LTM is then discarded. */
  DOT1AG_TRACE(CFM_LTM, "Egress Port is that of the receiving Up MEP.\r\n");
  enqueLTR(dot1agPkt, pMPObj, OFDPA_FALSE, DOT1AG_CFM_UP_MEP,
           DOT1AG_CFM_UP_MEP,
           dot1agPkt->intIfNum, txIntIfNum);
}

/*********************************************************************
* @purpose   Method called by the ProcessLTM to conditionally forward
*            received LTM. The MHF was received by an Up MHF.
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @notes    802.1ag d8-1 20.42.1.6
*
* @end
**********************************************************************/
void ProcessLTMRxUpMHF(dot1agControlPkt_t *dot1agPkt,
                       dot1agCfmStackObj_t *pMPObj)
{
  uint32_t txIntIfNum;

  if (pMPObj == NULL)
  {
    return;
  }

  /* If the Target MAC Address carried in the LTM is the MAC address of the receiving Up
     MHF (i.e., that of the Bridge Port on which that MHF resides), then the LTM has
     reached its target. ProcessLTM() calls enqueLTR() (20.42.4) to enqueue an LTR for the
     MHF Linktrace SAP of the receiving Up MHF.  The LTM is discarded, and no further
     processing takes place. */
  if (memcmp(pMPObj->mpMacAddress, dot1agPkt->cfmPdu.ltm.targetMacAddr, OFDPA_MAC_ADDR_LEN) == 0)
  {
    DOT1AG_TRACE(CFM_LTM, "Target MAC Address matched with UP MHF.\r\n");
    enqueLTR(dot1agPkt, pMPObj, OFDPA_FALSE, DOT1AG_CFM_UP_MHF,
             DOT1AG_CFM_UP_MHF, dot1agPkt->intIfNum,
             pMPObj->key.dot1agCfmStackifIndex);
    return;
  }

  /* Otherwise, the Ingress and Egress Ports are determined according to 20.42.1.2.  If
     the Egress Port cannot be determined, or if the Egress Port is not the Bridge Port on
     which the receiving MHF is configured, then the LTM is discarded, and no further
     processing takes place. */
  if (ProcessLTMEgressIdFind(dot1agPkt, pMPObj, &txIntIfNum) != OFDPA_E_NONE ||
      txIntIfNum != pMPObj->key.dot1agCfmStackifIndex)
  {
    return;
  }

  /* Otherwise:
     1) ProcessLTM() calls enqueLTR() (20.42.4) to enqueue an LTR for the
     MHF Linktrace SAP of the receiving Up MHF. */
  DOT1AG_TRACE(CFM_LTM, "Ingress intf: %d Egress intf: %d\r\n", dot1agPkt->intIfNum, txIntIfNum);
  enqueLTR(dot1agPkt, pMPObj, OFDPA_FALSE, DOT1AG_CFM_UP_MHF, DOT1AG_CFM_UP_MHF,
           dot1agPkt->intIfNum, txIntIfNum);

  /* 2) If the LTM TTL field equals 0 or 1, the LTM is discarded and no
     further processing takes place. */
  if (dot1agPkt->cfmPdu.ltm.ttl <= 1)
  {
    return;
  }

  /* 3) Otherwise, ProcessLTM() calls ForwardLTM() (20.42.3) to forward an altered copy of
     the LTM through the MHF Linktrace SAP of the LOM on the same Bridge Port as the Up
     MHF that received the LTM. */
  ForwardLTM(dot1agPkt, pMPObj);
}

/*********************************************************************
* @purpose   Method called by the ProcessLTM to forward the received
*            LTM message
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    txpMPObj   @b{(input)} CMF Stack MP being used for
*                                  forwarding the LTM
*
* @returns  none
*
* @notes    802.1ag d8-1 20.42.3
*
* @end
**********************************************************************/
OFDPA_ERROR_t ForwardLTM(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *txpMPObj)
{
  uint8_t                *data, byteVal;
  uint16_t                shortVal;
  dot1agCfmSenderIdTLV_t  senderIdTLV;
  uint32_t                length = 0;
  OFDPA_ERROR_t           rc = OFDPA_E_NONE;

  if (txpMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  DOT1AG_TRACE(CFM_LTM, "Forwarding LTM to interface: %d\r\n", txpMPObj->key.dot1agCfmStackifIndex);

  /* Point to srcMAC field of the PDU payload */
  data = dot1agPkt->payLoad + OFDPA_MAC_ADDR_LEN;

  /* Uses the MAC address of the LOM owning the SAP specified for output as the source_address
     parameter of the LTM */
  DOT1AG_PUT_DATA(txpMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN, data);
  memcpy(dot1agPkt->srcMac, txpMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

  /* Fill DOT1Q tag here, so that validations would go through in the case of relay logic.
   * This gets overwritten anyway with default TPID while transmitting onto the wire.
   */
  shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);

  DOT1AG_GET_SHORT(shortVal, data);
  /* Sets the drop_eligible parameter of the forwarded LTM to false */
  shortVal &= 0xEFFF;
  data -= 2;
  /* Modify the TCI field */
  DOT1AG_PUT_SHORT(shortVal, data);

  /* Places in the forwarded LTM TTL field the value from the input LTM TTL field decremented
     by 1; */
  byteVal = dot1agPkt->cfmPdu.ltm.ttl - 1;
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN + 4;
  DOT1AG_PUT_BYTE(byteVal, data);

  /* Changes the value in the LTM Egress Identifier TLV to identify the forwarding Linktrace
     Responder */
  if (dot1agPkt->cfmPdu.ltm.ltmEgressIdTLVRead == OFDPA_TRUE)
  {
    /* write pMPOBJ's egresssId TLV */
    if (dot1agCfmLTMEgressIdTlvBuild(txpMPObj, &dot1agPkt->cfmPdu.ltm.ltmEgressIdTLV) == OFDPA_E_NONE)
    {
      rc = dot1agCfmLTMEgressIdentifierTlvWrite(&dot1agPkt->cfmPdu.ltm.ltmEgressIdTLV, dot1agPkt->cfmPdu.ltm.pltmEgressIdTLV, &length);

      if (rc != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS,
                           "Error modifying EgressId TLV, while trying to forward LTM", 0);
      }
    }
  }

  /* Deletes the Sender ID TLV, if present in the LTM */
  if (dot1agPkt->cfmPdu.ltm.senderIdTLVRead == OFDPA_TRUE)
  {
    uint32_t bytesToMove;

    bytesToMove = dot1agPkt->length -
      (dot1agPkt->cfmPdu.ltm.pSenderIdTLV - dot1agPkt->payLoad) -
      (dot1agPkt->cfmPdu.ltm.senderIdTLV.length + 3);
    memcpy(dot1agPkt->cfmPdu.ltm.pSenderIdTLV,
           dot1agPkt->cfmPdu.ltm.pSenderIdTLV + dot1agPkt->cfmPdu.ltm.senderIdTLV.length + 3,
           bytesToMove);
    dot1agPkt->length -= dot1agPkt->cfmPdu.ltm.senderIdTLV.length + 3;
    dot1agPkt->cfmPdu.ltm.pLastTLV -= dot1agPkt->cfmPdu.ltm.senderIdTLV.length + 3;
  }
  /* Reset the data to the last TLV of the PDU */
  data = dot1agPkt->cfmPdu.ltm.pLastTLV;
  /* optionally, places a Sender ID TLV, identifying the forwarding system, in the LTM */
  if (dot1agCfmSenderIdTlvBuild(txpMPObj, &senderIdTLV) == OFDPA_E_NONE)
  {
    rc = dot1agCfmSenderIdTlvWrite(&senderIdTLV, data, &length);

    if (rc == OFDPA_E_NONE)
    {
      data += length;
    }
    else
    {
      return rc;
    }
  }
  /* write END TLV */
  rc = dot1agCfmEndTlvWrite(data, &length);

  if (rc == OFDPA_E_NONE)
  {
    data += length;
  }
  else
  {
    return rc;
  }

  /* Mark length of the dot1agPkt payLoad */
  dot1agPkt->length = data - dot1agPkt->payLoad;
  /* Forward the pkt based on flags set by ProcessLTM*() funcs */
  (void)dot1agPduTransmit(txpMPObj->key.dot1agCfmStackifIndex,
                          txpMPObj->key.dot1agCfmStackVlanIdOrNone,
                          dot1agPkt);
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose   Method called by the ProcessLTM to respond to the received
*            LTM message. An LTR is constructed and queued for tx by
*            xmitOldestLTR later (FTWhile timer)
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
* @param    ltmFwdFlag @b{(input)} Flag indicating if the LTM was
*                                  forwarded by Linktrace responder
* @param    ltrMPType       @b{(input)} Type of MP on which LTR
*                                       is enqueued
* @param    rxMPType        @b{(input)} Type of MP on which LTM has
*                                       ingressed
* @param    ingressIntIfNum @b{(input)} Identifies ingress Port.
* @param    egressIntIfNum  @b{(input)} Identifies egress Port. NULL
*                                       if egress port could not be
*                                       identified
*
* @returns  none
*
* @notes    802.1ag d8-1 20.42.4
*
* @end
**********************************************************************/
OFDPA_ERROR_t enqueLTR(dot1agControlPkt_t *dot1agPkt, dot1agCfmStackObj_t *pMPObj,
                       OFDPA_BOOL ltmFwdFlag, Dot1agCfmMPType_t ltrMPType,
                       Dot1agCfmMPType_t rxMPType, uint32_t ingressIntIfNum,
                       uint32_t egressIntIfNum)
{
  uint8_t                    *data, byteVal, *pLtmData, type;
  dot1agCfmCommonHdr_t        cfmHdr;
  uint16_t                    shortVal, length;
  uint32_t                    uintVal;
  OFDPA_BOOL                  skip;
  dot1agCfmLtrPduMsg_t        ltrQMsg;
  dot1agCfmLTREgressIdTLV_t   ltrEgressIdTlv;
  dot1agCfmSenderIdTLV_t      senderIdTLV;
  dot1agCfmReplyEgressTLV_t   replyEgressTlv;
  dot1agCfmReplyIngressTLV_t  replyIngressTlv;
  OFDPA_ERROR_t               rc = OFDPA_E_NONE;
  uint32_t                    len = 0;

  if (pMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  DOT1AG_TRACE(CFM_LTR, "Enqueue LTR. ingress interface; %d egress interface; %d\r\n", ingressIntIfNum, egressIntIfNum);
  memset(&dot1agPktFromCpu, 0x0, sizeof(dot1agPktFromCpu));
  data = dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.vlanId   = pMPObj->key.dot1agCfmStackVlanIdOrNone;
  dot1agPktFromCpu.intIfNum = pMPObj->key.dot1agCfmStackifIndex;
  dot1agPktFromCpu.fromWire = OFDPA_FALSE;

  /* Uses the MAC address contained in the LTM.s Original MAC Address field as the
     destination_address of the LTR */
  DOT1AG_PUT_DATA(dot1agPkt->cfmPdu.ltm.orgMacAddr, OFDPA_MAC_ADDR_LEN, data);
  memcpy(dot1agPktFromCpu.destMac, dot1agPktFromCpu.payLoad, OFDPA_MAC_ADDR_LEN);

  /* Uses the MAC address of the MP owning the SAP specified for output as the
     source_address of the LTR */
  DOT1AG_PUT_DATA(pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN, data);
  memcpy(dot1agPktFromCpu.srcMac, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);


  /* If the LTR includes a Reply Egress TLV (see below, step p), uses the Primary VID of
     the MP on that Egress Port as the vlan_identifier of the LTR, else it uses the
     Primary VID of the MP on the Ingress Port as the vlan_identifier of the LTR; */

  /* Fill DOT1Q tag here, so that validations would go through in the case of relay logic.
   * This gets overwritten anyway with default TPID while transmitting onto the wire.
   */
  shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);

  /* Construct TCI */
  shortVal = 0;
#ifdef NOT_SUPPORTED
  /* d) Sets the priority parameter to the same value as for CCMs (12.14.7.1.3:h); */
  shortVal  = ((uint16_t)(pMPObj->mepData->dot1agCfmMepCcmLtmPriority)) << 13;
#endif

  shortVal |= ((uint16_t)pMPObj->key.dot1agCfmStackVlanIdOrNone) & 0x0FFF;
  DOT1AG_PUT_SHORT(shortVal, data);

  /* Fill Ether Type */
  shortVal = OFDPA_DOT1AG_CFM_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);
  dot1agPktFromCpu.cfmPduOffset =  data;

  /* Fill CFM Common Header */
  cfmHdr.level   = dot1agPkt->cfmHdr.level;
  /* e) Places its own version in the Version field; */
  cfmHdr.version = DOT1AG_CFM_PROTOCOL_VERSION_0;
  /* f) If its own version is lower than that of the received LTM, sets all bits and
     fields in the transmitted PDU that are reserved in its own version, including all
     bits between the portion of the last header field defined for its own version and the
     first TLV, to 0; - Ignore this condition for now */
  cfmHdr.opcode  = DOT1AG_LINKTRACE_REPLY_OPCODE;
  /* i) Copies the Flags field, excepting the FwdYes bit and the TerminalMEP bit, from the
     LTM to the LTR; */
  cfmHdr.flags   = dot1agPkt->cfmHdr.flags;
  /* g) Sets the FwdYes bit of the Flags field to 1 if the LTM was forwarded by the
     Linktrace Responder, or 0 if not; */
  if (ltmFwdFlag == OFDPA_TRUE)
  {
    DOT1AG_LTR_FLAGS_FWD_YES_SET(cfmHdr.flags);
  }
  /* h) Sets the TerminalMEP bit of the Flags field to 1 if the MP reported in either the
     Reply Ingress TLV or the Reply Egress TLV is a MEP, or 0 if not; */
  if (pMPObj->mepData != NULL)
  {
    DOT1AG_LTR_TERMINAL_MEP_SET(cfmHdr.flags);
  }

  cfmHdr.first_tlv_offset = DOT1AG_CFM_LTR_FIRST_TLV_OFFSET;
  dot1agCfmHdrWrite(&cfmHdr, data);
  data += DOT1AG_CFM_COMMON_HDR_LEN;
  memcpy(&dot1agPktFromCpu.cfmHdr, &cfmHdr, sizeof(dot1agPktFromCpu.cfmHdr));

  /* j) Copies the LTM Transaction Identifier field from the LTM to the LTR Transaction
     Identifier field of the LTR; */
  uintVal = dot1agPkt->cfmPdu.ltm.ltmTransId;
  DOT1AG_PUT_LONG(uintVal, data);
  /* m) Places one less than the value in the LTM TTL field of the LTM in the Reply TTL field of the LTR; */
  byteVal = dot1agPkt->cfmPdu.ltm.ttl - 1;
  DOT1AG_PUT_BYTE(byteVal, data);
  /* n) Sets the Relay Action field according to Table 21-27 on page 199; */
  byteVal = DOT1AG_RELAY_ACTION_rlyFdb;
  if (memcmp(pMPObj->mpMacAddress, dot1agPkt->cfmPdu.ltm.targetMacAddr, OFDPA_MAC_ADDR_LEN) == 0)
  {
    /* The LTM reached an MP whose MAC address matches the target MAC address */
    byteVal =  DOT1AG_RELAY_ACTION_rlyHit;
  }
  else
  {
    /* The Egress Port was determined by consulting the Filtering Database */
    byteVal =  DOT1AG_RELAY_ACTION_rlyFdb;
  }
  DOT1AG_PUT_BYTE(byteVal, data);

  /* Add TLV's to the packet */
  data = dot1agPktFromCpu.cfmPduOffset + dot1agPktFromCpu.cfmHdr.first_tlv_offset + DOT1AG_CFM_COMMON_HDR_LEN;
  pLtmData = dot1agPkt->cfmPduOffset + dot1agPkt->cfmHdr.first_tlv_offset + DOT1AG_CFM_COMMON_HDR_LEN;

  DOT1AG_GET_BYTE(type, pLtmData);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    skip = OFDPA_FALSE;
    DOT1AG_GET_SHORT(length, pLtmData);

    switch (type)
    {
      case DOT1AG_TLV_TYPE_SENDER_ID:
      case DOT1AG_TLV_TYPE_LTM_EGR_ID:
        /* r) Copies, verbatim, all other TLVs in the input LTM to the LTR, except for the
           Sender ID TLV (21.5.3), which is not copied;  */
        skip = OFDPA_TRUE;
        break;
      case DOT1AG_TLV_TYPE_ORG_SPECIFIC:
      default:
        break;
    }

    /* l) Sets the Next Egress Identifier field (21.9.7.2) of the LTR Egress Identifier
       TLV to a value that identifies the forwarding Linktrace Responder (See 21.8.8); */

    if (skip == OFDPA_FALSE)
    {
      DOT1AG_PUT_BYTE(type, data);
      DOT1AG_PUT_SHORT(length, data);
      DOT1AG_PUT_DATA(pLtmData, length, data);
    }
    pLtmData += length;
    DOT1AG_GET_BYTE(type, pLtmData);
  } /* Loop thru the LTM TLV contents and copy/modify the ones necessary */

  /* k) Copies the LTM Egress Identifier TLV (21.8.8) value from the LTM to the Last
     Egress Identifier field (21.9.7.1) of the LTR Egress Identifier TLV, or places 0 in
     that field, if there is no LTM Egress Identifier TLV in the received LTM (see Annex
     J.4); */
  if (dot1agCfmLTREgressIdTlvBuild(pMPObj, &ltrEgressIdTlv) == OFDPA_E_NONE)
  {
    if (dot1agPkt->cfmPdu.ltm.ltmEgressIdTLVRead == OFDPA_TRUE)
    {
      memcpy(ltrEgressIdTlv.lastEgressId, dot1agPkt->cfmPdu.ltm.ltmEgressIdTLV.egressId,
             DOT1AG_TLV_FIELD_LEN_LTR_EGRESS_ID);
    }
    else
    {
      memset(ltrEgressIdTlv.lastEgressId, 0x0, DOT1AG_TLV_FIELD_LEN_LTR_EGRESS_ID);
    }

    rc = dot1agCfmLTREgressIdTlvWrite(&ltrEgressIdTlv, data, &len);

    if (rc == OFDPA_E_NONE)
    {
      data += len;
    }
    else
    {
      return rc;
    }
  }

  /* o) If the LTM was not received by a Down MEP or Down MHF, does not place a Reply
     Ingress TLV in the LTR; otherwise:*/
  if (rxMPType == DOT1AG_CFM_DOWN_MEP || rxMPType == DOT1AG_CFM_DOWN_MHF)
  {
    /*    1) Fills the Ingress Action field of a Reply Ingress TLV (21.9.8) with the appropriate value
          according to Table 21-30 on page 201; */
    /*    2) Places the receiving MP.s MAC address in the Ingress MAC Address field of the Reply Ingress
          TLV; and */
    /*    3) Optionally, fills the remainder of the Reply Ingress TLV with the receiving MP.s Port ID
          information; */
    if (dot1agCfmReplyIngressTlvBuild(ingressIntIfNum, pMPObj->key.dot1agCfmStackVlanIdOrNone, pMPObj->mpMacAddress,
                                      &replyIngressTlv) == OFDPA_E_NONE)
    {
      rc = dot1agCfmReplyIngressTlvWrite(&replyIngressTlv, data, &len);

      if (rc == OFDPA_E_NONE)
      {
        data += len;
      }
      else
      {
        return rc;
      }
    }
  }
  /* p) If the LTM was received by a Down MEP, or if no Egress Port was identified, or if
     no Up MEP nor Up MHF belonging to the LTM.s MA is configured on the Egress Port, does
     not place a Reply Egress TLV in the LTR; otherwise: */
  if (rxMPType != DOT1AG_CFM_DOWN_MEP && egressIntIfNum != 0)
  {
    /*    1) Fills the Egress Action field of a Reply Egress TLV (21.9.9) with the appropriate value
          according to Table 21-32 on page 202; */
    /*    2) Places the Egress Port.s Up MP.s MAC address in the Egress MAC Address field of a Reply
          Egress TLV in the LTM; and */
    /*    3) Optionally, fills the remainder of the Reply Egress TLV with Egress Port.s Port ID information; */
    if (dot1agCfmReplyEgressTlvBuild(egressIntIfNum, pMPObj->key.dot1agCfmStackVlanIdOrNone, pMPObj->mpMacAddress,
                                     &replyEgressTlv) == OFDPA_E_NONE)
    {
      rc = dot1agCfmReplyEgressTlvWrite(&replyEgressTlv, data, &len);

      if (rc == OFDPA_E_NONE)
      {
        data += len;
      }
      else
      {
        return rc;
      }
    }
  }
  /* q) As controlled by the managed objects 12.14.3.1.3:e, 12.14.5.1.3:d, and
     12.14.6.1.3:d, places a Sender ID TLV (21.5.3), identifying the replying Bridge, in
     the LTM; Current implementation sends senderIdTLV by default*/
  if (dot1agCfmSenderIdTlvBuild(pMPObj, &senderIdTLV) == OFDPA_E_NONE)
  {
    rc = dot1agCfmSenderIdTlvWrite(&senderIdTLV, data, &len);

    if (rc == OFDPA_E_NONE)
    {
      data += len;
    }
    else
    {
      return rc;
    }
  }
  /* Write END TLV */
  rc = dot1agCfmEndTlvWrite(data, &len);

  if (rc == OFDPA_E_NONE)
  {
    data += len;
  }
  else
  {
    return rc;
  }

  dot1agPktFromCpu.length = data - dot1agPktFromCpu.payLoad;

#if 0
  if (dot1agPkt->fromOwnUpMEP == OFDPA_TRUE)
  {
    dot1agPktFromCpu.fromOwnUpMEP = OFDPA_TRUE;
    return dot1agPktInject(dot1agPkt->intIfNum, &dot1agPktFromCpu);
  }
#endif

  dot1agPktFromCpu.lmepId   = pMPObj->ofdpaLocalMpId;
  dot1agPktFromCpu.oamOpcode = DOT1AG_LINKTRACE_REPLY_OPCODE;

  memset(&ltrQMsg, 0x0, sizeof(ltrQMsg));
  ltrQMsg.intIfNum = pMPObj->key.dot1agCfmStackifIndex;
  ltrQMsg.vlanId = pMPObj->key.dot1agCfmStackVlanIdOrNone;
  ltrQMsg.length = dot1agPktFromCpu.length;
  ltrQMsg.mpType = ltrMPType;
  memcpy(&(ltrQMsg.dot1agPkt), &dot1agPktFromCpu, sizeof(dot1agPktFromCpu));

  if (msgqSnd(dot1agCFM.dot1agLtrQueue, &ltrQMsg) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC, "Failed to post message into queue, while trying to enque LTR", 0);
    return OFDPA_E_FAIL;
  }

  /* Increments nPendingLTRs by 1 */
  dot1agCFM.dot1agCfmLtmrVar.nPendingLTRs++;
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Method called by the LTRT SM to send responses to LTR
*
* @param    none
*
* @returns  none
*
* @notes    802.1ag d8-1 20.44.1
*
* @end
**********************************************************************/
void xmitOldestLTR()
{
  dot1agCfmLtrPduMsg_t msg;

  while ((msgqRcv(dot1agCFM.dot1agLtrQueue, &msg) == OFDPA_E_NONE) && (dot1agCFM.dot1agCfmLtmrVar.nPendingLTRs != 0))
  {
    DOT1AG_TRACE(CFM_LTR, "MP Type: %d interface: %d vlanId: %d Pending LTR Count: %d\r\n",
                 msg.mpType, msg.intIfNum, msg.vlanId, dot1agCFM.dot1agCfmLtmrVar.nPendingLTRs);

    dot1agCFM.dot1agCfmLtmrVar.nPendingLTRs--;

    if (dot1agPduTransmit(msg.intIfNum, msg.vlanId, &(msg.dot1agPkt)) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to transmit LTR on intf %d vlanId %d MP type %d",
                         msg.intIfNum, msg.vlanId, msg.mpType);
    }
  } /* End of Queue loop */
}

/*********************************************************************
* @purpose  Method called by the LTRT SM to send responses to LTR
*
* @param    none
*
* @returns  none
*
* @notes    802.1ag d8-1 20.42.2
*
* @end
**********************************************************************/
void clearPendingLTRs()
{
  xmitOldestLTR();
  dot1agCFM.dot1agCfmLtmrVar.nPendingLTRs = 0;
}

OFDPA_ERROR_t fillCCM(dot1agCfmStackObj_t *pMEPObj, void *ccmPktDriverBuffer)
{
	return 0;
}

OFDPA_ERROR_t addCCMToArray(dot1agCfmStackObj_t *pMEPObj)
{
	return 0;
}

/*********************************************************************
* @purpose  Transmit LMM on requested MEP for proactive LM session
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitProLMM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority)
{
  uint8_t                    lmDestMacAddress[OFDPA_MAC_ADDR_LEN];
  OFDPA_BOOL                 lmDestIsMepId;
  y1731CfmMepProlmTable_t   *y1731CfmMepProlmEntry;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL ||
      pMEPObj->mepData->y1731CfmMepProlmEntry == NULL)
  {
    return OFDPA_E_FAIL;
  }

  y1731CfmMepProlmEntry = pMEPObj->mepData->y1731CfmMepProlmEntry;

  lmDestIsMepId = y1731CfmMepProlmEntry->prolmDestIsMepId;
  if (lmDestIsMepId == OFDPA_FALSE)
  {
    memcpy(lmDestMacAddress, y1731CfmMepProlmEntry->prolmDestMacAddress, OFDPA_MAC_ADDR_LEN);
  }

  return xmitLMM(pMEPObj, priority, lmDestIsMepId, lmDestMacAddress, DOT1AG_CFM_LMM_OPERATION_TYPE_PROACTIVE);
}

/*********************************************************************
* @purpose  Transmit LMM on requested MEP for Ondemand LM session
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitOndemLMM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority)
{
  uint8_t                    lmDestMacAddress[OFDPA_MAC_ADDR_LEN];
  OFDPA_BOOL                 lmDestIsMepId;
  y1731CfmMepOndemlmTable_t *y1731MepOndemlmEntry;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL ||
      pMEPObj->mepData->y1731CfmMepOndemlmEntry[priority] == NULL)
  {
    return OFDPA_E_FAIL;
  }

  y1731MepOndemlmEntry = pMEPObj->mepData->y1731CfmMepOndemlmEntry[priority];

  lmDestIsMepId = y1731MepOndemlmEntry->ondemlmDestIsMepId;
  if (lmDestIsMepId == OFDPA_FALSE)
  {
    memcpy(lmDestMacAddress, y1731MepOndemlmEntry->ondemlmDestMacAddress, OFDPA_MAC_ADDR_LEN);
  }

  return xmitLMM(pMEPObj, priority, lmDestIsMepId, lmDestMacAddress, DOT1AG_CFM_LMM_OPERATION_TYPE_ONDEMAND);
}

/*********************************************************************
* @purpose  Transmit LMM on requested MEP
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitLMM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority, OFDPA_BOOL lmDestIsMepId, uint8_t *lmDestMacAddress, Dot1agCfmLmmOperationType_t lmmTypeFlags)
{
  uint8_t                   *data, byteVal;
  dot1agCfmCommonHdr_t       cfmHdr;
  uint16_t                   shortVal;
  uint32_t                   uintVal;
  uint32_t                   len      = 0;
  OFDPA_ERROR_t              rc;
  OFDPA_OAM_MEG_TYPE_t       oamMegType;
  uint16_t                   vlanId;
  int                        offset;
  ofdbOamLmepData_t         *oamMapData;

  if (pMEPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  oamMegType = ofdbMegTypeFromVidGet(pMEPObj->key.dot1agCfmStackVlanIdOrNone);
  memset(&dot1agPktFromCpu, 0x0, sizeof(dot1agPktFromCpu));
  data = dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.vlanId   = pMEPObj->key.dot1agCfmStackVlanIdOrNone;
  dot1agPktFromCpu.intIfNum = pMEPObj->key.dot1agCfmStackifIndex;
  dot1agPktFromCpu.fromWire = OFDPA_FALSE;

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pMEPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_OAM_MEG_TYPE_ETHERNET != oamMegType)
  {
    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0)
    {
      return OFDPA_E_FAIL;
    }
    data += len;
    vlanId = oamMapData->vlanId;
  }
  else
  {
    /* Fill DMAC */
    if (lmDestIsMepId == OFDPA_FALSE)
    {
      DOT1AG_PUT_DATA(lmDestMacAddress, OFDPA_MAC_ADDR_LEN, data);
      memcpy(dot1agPktFromCpu.destMac, lmDestMacAddress, OFDPA_MAC_ADDR_LEN);
    }
    else
    {
      /* Sets the destination_address parameter to the value from
         Table 8-9 corresponding to MEP.s MD Level; */
      DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);
      byteVal = dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMEPObj->key.dot1agCfmStackMdLevel;
      DOT1AG_PUT_BYTE(byteVal, data);
      memcpy(dot1agPktFromCpu.destMac, dot1agPktFromCpu.payLoad, OFDPA_MAC_ADDR_LEN);
    }

    /* Fill SMAC */
    DOT1AG_PUT_DATA(pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN, data);
    memcpy(dot1agPktFromCpu.srcMac, pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

    shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
    DOT1AG_PUT_SHORT(shortVal, data);

    /* Construct TCI */
    shortVal = 0;
    shortVal  = ((uint16_t)(priority)) << 13;
    shortVal |= ((uint16_t)pMEPObj->key.dot1agCfmStackVlanIdOrNone) & 0x0FFF;
    DOT1AG_PUT_SHORT(shortVal, data);

    vlanId = pMEPObj->key.dot1agCfmStackVlanIdOrNone;
  }

  /* Fill Ether Type */
  shortVal = OFDPA_DOT1AG_CFM_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);
  dot1agPktFromCpu.cfmPduOffset = data;

  /* Fill CFM Common Header */
  cfmHdr.level   = pMEPObj->key.dot1agCfmStackMdLevel;
  cfmHdr.version = DOT1AG_CFM_PROTOCOL_VERSION_1;
  cfmHdr.opcode  = DOT1AG_LOSSMEASUREMENT_MESSAGE_OPCODE;
  cfmHdr.flags   = lmmTypeFlags;
  cfmHdr.first_tlv_offset = DOT1AG_CFM_LMM_FIRST_TLV_OFFSET;
  dot1agCfmHdrWrite(&cfmHdr, data);
  data += DOT1AG_CFM_COMMON_HDR_LEN;
  memcpy(&dot1agPktFromCpu.cfmHdr, &cfmHdr, sizeof(dot1agPktFromCpu.cfmHdr));

  /* Fill LMM header, txFCf should be filled by HW */
  /* oamReplacementOffset should point to end of CFM common header*/
  offset = data - dot1agPktFromCpu.payLoad;
  uintVal = 0;
  DOT1AG_PUT_LONG(uintVal, data);

  data = dot1agPktFromCpu.cfmPduOffset + cfmHdr.first_tlv_offset + DOT1AG_CFM_COMMON_HDR_LEN;
  /* End TLV */
  rc = dot1agCfmEndTlvWrite(data, &len);

  if (rc == OFDPA_E_NONE)
  {
    data += len;
  }
  else
  {
    return rc;
  }

  dot1agPktFromCpu.length               = data - dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.lmepId               = pMEPObj->ofdpaLocalMpId;
  dot1agPktFromCpu.oamOpcode            = DOT1AG_LOSSMEASUREMENT_MESSAGE_OPCODE;
  /* oamReplacementOffset should be half of number of bytes from end of ethernet header to end CFM common header*/
  dot1agPktFromCpu.oamReplacementOffset = offset;
  dot1agPktFromCpu.priority = priority;

  /* Put the pkt on wire */
  (void)dot1agPduTransmit(pMEPObj->key.dot1agCfmStackifIndex,
                          vlanId,
                          &dot1agPktFromCpu);

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose   Method called by the LM SM to process the received LMM
*            message
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t ProcessLMM(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  uint8_t                *data, type;
  uint16_t                length;
  OFDPA_DOT1AG_MP_type_t  mpType = OFDPA_DOT1AG_MP_TYPE_ETH;

  if (pMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* get MP type */
  if (dot1agMpTypeGet(pMPObj, &mpType) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  /* Point to CFM PDU */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;
  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_LMM_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }
  dot1agPkt->cfmPdu.lmm.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;

  DOT1AG_TRACE(CFM_LM, "Start Processing LMM received on interface: %d\r\n", dot1agPkt->intIfNum);

  if (OFDPA_DOT1AG_MP_TYPE_ETH == mpType)
  {
    /* Check if dest_mac is group address. If Unicast it should be same
       as receiving MP mac address. If group address, it should follow table 8-9.
    */
    if (((dot1agPkt->destMac[0] & 0x01) == 0) &&
        memcmp(dot1agPkt->destMac, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN) != 0)
    {
      DOT1AG_TRACE(CFM_LM, "Unicast DMAC, but not mine\r\n");
      return OFDPA_E_NONE;
    }
    else if ((dot1agPkt->destMac[0] & 0x1) &&
             !(memcmp(dot1agRsrvdMac, dot1agPkt->destMac, OFDPA_MAC_ADDR_LEN-1) == 0 &&
               dot1agPkt->destMac[OFDPA_MAC_ADDR_LEN-1] ==
               (dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMPObj->key.dot1agCfmStackMdLevel)))
    {
      DOT1AG_TRACE(CFM_LM, "Invalid Multicat DMAC[0] %d\r\n", dot1agPkt->destMac[0]);
      return OFDPA_E_NONE;
    }

    /* If the destination_address parameter contains a Group address and the MP Loopback Responder
       state machine resides in an MHF (rather than in a MEP), ProcessLMM() discards the LMM and
       performs no further processing */
    if ((dot1agPkt->destMac[0] & 0x1) && pMPObj->mepData == NULL)
    {
      return OFDPA_E_NONE;
    }

    /* If the source_address parameter is a Group, and not an Individual MAC address, ProcessLMM()
       discards the frame and performs no further processing.  */
    if (dot1agPkt->srcMac[0] & 0x1)
    {
      return OFDPA_E_NONE;
    }
  }

  /* Parse the LMM */
  data = dot1agPkt->cfmPdu.lmm.pFirstTLV;

  DOT1AG_GET_BYTE(type, data);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);
    switch (type)
    {
      /* Sender ID TLV */
      case DOT1AG_TLV_TYPE_SENDER_ID:
        dot1agPkt->cfmPdu.lmm.senderIdTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.lmm.pSenderIdTLV = data - 3;
        dot1agPkt->cfmPdu.lmm.senderIdTLV.length = length;
        if (dot1agCfmSenderIdTlvRead(&dot1agPkt->cfmPdu.lmm.senderIdTLV,
                                     data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        break;
        /* RX_COUNTER TLV */
      case DOT1AG_TLV_TYPE_BRCM_RX_COUNTER:
        dot1agPkt->cfmPdu.lmm.rxCtrTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.lmm.pRxCtrTLV = data - 3;
        DOT1AG_GET_LONG(dot1agPkt->cfmPdu.lmm.rxFCl, data); /* set by HW */
        break;

        /* Org TLV */
      case DOT1AG_TLV_TYPE_ORG_SPECIFIC:
        /* Ignore all other TLVs */
      default:
        break;
    }
    data += length;
    DOT1AG_GET_BYTE(type, data);
  }

  dot1agPkt->cfmPdu.lmm.rxFCl = dot1agPkt->timestamp;

  /* Terminate LMM */
  dot1agPkt->toFwd = OFDPA_FALSE;
  /* Call xmitLMR() to generate and transmit an LMR */
  return xmitLMR(dot1agPkt, pMPObj);
}

/*********************************************************************
* @purpose  Transmit LMR on requested MEP
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitLMR(dot1agControlPkt_t *dot1agPkt,
                      dot1agCfmStackObj_t *pMPObj)
{
  uint32_t          uintVal;
  uint8_t          *data;
  uint8_t           payLoad[OFDPA_DOT1AG_MAX_FRAME_SIZE];
  uint8_t           priority = 0;
  uint32_t          len      = 0;
  OFDPA_ERROR_t     rc;
  uint8_t           ethTypeOffset = (dot1agPkt->cfmPduOffset - dot1agPkt->payLoad) - DOT1AG_ETH_TYPE_LEN;
  uint16_t          vlanId;
  int               offset;
  ofdbOamLmepData_t *oamMapData;

  if (NULL == pMPObj ||
      NULL == pMPObj->mepData)
  {
    DOT1AG_TRACE(CFM_LM, "LMR - pMPObj/mepData missing.\r\n");
    return OFDPA_E_INTERNAL;
  }

  /* Validate destination mac address */
  DOT1AG_TRACE(CFM_LM, "Preparing LMR\r\n");

  /* Sets the destination_address parameter */
  if (dot1agPkt->destMac[0] & 0x1)
  {
    /* to the value from Table 8-9 corresponding to MEP.s MD Level; */
    data = dot1agPkt->payLoad;
    DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);
    DOT1AG_PUT_BYTE(dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMPObj->key.dot1agCfmStackMdLevel, data);
  }
  else
  {
    /* to the source_address of the received LMM */
    memcpy(dot1agPkt->payLoad, dot1agPkt->srcMac, OFDPA_MAC_ADDR_LEN);
  }

  /* Sets the source_address parameter to the MAC address of the replying MP */
  memcpy(dot1agPkt->payLoad + OFDPA_MAC_ADDR_LEN, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

  /* Changes the OpCode field (21.4.3) from LMM to LMR */
  *(dot1agPkt->cfmPduOffset + 1) = DOT1AG_LOSSMEASUREMENT_REPLY_OPCODE;

  /* Fill LMR header */
  /* keep txFCf */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN + 4;
  /*fill rxFCf according to HW*/
  uintVal = dot1agPkt->cfmPdu.lmm.rxFCl;
  DOT1AG_PUT_LONG(uintVal, data);
  /*debug: txFCb should be filled by HW*/
  uintVal = 0x0;
  DOT1AG_PUT_LONG(uintVal, data);

  memcpy(payLoad, (dot1agPkt->payLoad + ethTypeOffset), (dot1agPkt->length - ethTypeOffset));
  data = dot1agPkt->payLoad;

  dot1agPkt->length = dot1agPkt->length - 4; //Removing CRC

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pMPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_OAM_MEG_TYPE_ETHERNET != ofdbMegTypeFromVidGet(pMPObj->key.dot1agCfmStackVlanIdOrNone))
  {
    priority = dot1agPkt->priority;
    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0 || (OFDPA_DOT1AG_MAX_FRAME_SIZE < (dot1agPkt->length - ethTypeOffset + len)))
    {
      return OFDPA_E_FAIL;
    }

    data += len;
    memcpy(data, payLoad, (dot1agPkt->length - ethTypeOffset));

    /* oamReplacementOffset should point to TxFCb in LMR pdu*/
    offset = len + DOT1AG_ETH_TYPE_LEN + DOT1AG_CFM_COMMON_HDR_LEN + sizeof(dot1agPkt->cfmPdu.lmr.txFCf) + sizeof(dot1agPkt->cfmPdu.lmr.rxFCf);
    dot1agPkt->length = (dot1agPkt->length - ethTypeOffset) + len;
    vlanId = oamMapData->vlanId;
  }
  else
  {
    /* oamReplacementOffset should point to TxFCb in LMR pdu*/
    offset = (dot1agPkt->cfmPduOffset - dot1agPkt->payLoad) + DOT1AG_CFM_COMMON_HDR_LEN + sizeof(dot1agPkt->cfmPdu.lmr.txFCf) +
      sizeof(dot1agPkt->cfmPdu.lmr.rxFCf);
    vlanId = pMPObj->key.dot1agCfmStackVlanIdOrNone;
  }

  dot1agPkt->lmepId               = pMPObj->ofdpaLocalMpId;
  dot1agPkt->oamOpcode            = DOT1AG_LOSSMEASUREMENT_REPLY_OPCODE;
  dot1agPkt->oamReplacementOffset = offset;

  /* Put the pkt on wire */
  DOT1AG_TRACE(CFM_LM, "Transmitting LMR\r\n");
  (void)dot1agPduTransmit(pMPObj->key.dot1agCfmStackifIndex,
                            vlanId,
                          dot1agPkt);

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  return  OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Parse LMR Pdu and store int in local CFM PDU message
*           storage
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agParseLMR(dot1agControlPkt_t *dot1agPkt)
{
  uint8_t  *data, type;
  uint16_t  length;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  /* Point to CFM PDU */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;

  /* 21.6.2 */
  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_LMR_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }
  dot1agPkt->cfmPdu.lmr.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;

  /* Parse LMR info */
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.lmr.txFCf, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.lmr.rxFCf, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.lmr.txFCb, data);

  /* timestamp field is reused to get the local rx stats */
  dot1agPkt->cfmPdu.lmr.rxFCl = dot1agPkt->timestamp;

  /* Parse available TLVs */
  data = dot1agPkt->cfmPdu.lmr.pFirstTLV;

  DOT1AG_GET_BYTE(type, data);
  DOT1AG_TRACE(CFM_LM, "Parse LMR, type %d\r\n", type);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);
    switch (type)
    {
      /* RX_COUNTER TLV */
      case DOT1AG_TLV_TYPE_BRCM_RX_COUNTER:
        dot1agPkt->cfmPdu.lmr.rxCtrTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.lmr.pRxCtrTLV = data - 3;
        DOT1AG_GET_LONG(dot1agPkt->cfmPdu.lmr.rxFCl, data); /* set by HW */
        break;

        /* Org TLV */
      case DOT1AG_TLV_TYPE_ORG_SPECIFIC:
        /* Ignore all other TLVs */
      default:
        break;
    }
    data += length;
    DOT1AG_GET_BYTE(type, data);
  }

  /* Parse internal RX_COUNTER TLVs */
  data = dot1agPkt->payLoad + dot1agPkt->length - 7;
  DOT1AG_GET_BYTE(type, data);
  if (type == DOT1AG_TLV_TYPE_BRCM_RX_COUNTER)
  {
    DOT1AG_GET_SHORT(length, data);
    if (length == DOT1AG_TLV_LEN_BRCM_RX_COUNTER)
    {
      dot1agPkt->cfmPdu.lmr.rxCtrTLVRead = OFDPA_TRUE;
      dot1agPkt->cfmPdu.lmr.pRxCtrTLV = data - 3;
      DOT1AG_GET_LONG(dot1agPkt->cfmPdu.lmr.rxFCl, data); /* set by HW */
    }
  }

  /* Deletes the RX COUNTER TLV, if present in the LMR */
  if (dot1agPkt->cfmPdu.lmr.rxCtrTLVRead)
  {
    uint32_t bytesToMove;

    bytesToMove = dot1agPkt->length -
      (dot1agPkt->cfmPdu.lmr.pRxCtrTLV - dot1agPkt->payLoad) -
      (DOT1AG_TLV_LEN_BRCM_RX_COUNTER + 3);
    memcpy(dot1agPkt->cfmPdu.lmr.pRxCtrTLV,
           dot1agPkt->cfmPdu.lmr.pRxCtrTLV + DOT1AG_TLV_LEN_BRCM_RX_COUNTER + 3,
           bytesToMove);
    dot1agPkt->length -= DOT1AG_TLV_LEN_BRCM_RX_COUNTER + 3;
    dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Transmit DMM on requested MEP
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitDMM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority)
{
  uint8_t              *data, byteVal;
  dot1agCfmCommonHdr_t  cfmHdr;
  uint16_t              shortVal;
  uint64_t              llVal;
  y1731CfmMepDmTable_t *y1731MepDmEntry;
  uint32_t              len      = 0;
  OFDPA_ERROR_t         rc;
  OFDPA_OAM_MEG_TYPE_t  oamMegType;
  uint16_t              vlanId;
  int                   offset;
  ofdbOamLmepData_t    *oamMapData;
  dot1agCfmDataTLV_t    dataTlv;
  uint8_t               buf[DOT1AG_DATA_TLV_VALUE_LEN_MAX];
  int32_t               packetLength = 0;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL ||
      pMEPObj->mepData->y1731CfmMepDmEntry[priority] == NULL)
  {
    return OFDPA_E_FAIL;
  }

  oamMegType = ofdbMegTypeFromVidGet(pMEPObj->key.dot1agCfmStackVlanIdOrNone);
  y1731MepDmEntry = pMEPObj->mepData->y1731CfmMepDmEntry[priority];

  memset(&dot1agPktFromCpu, 0x0, sizeof(dot1agPktFromCpu));
  data = dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.intIfNum = pMEPObj->key.dot1agCfmStackifIndex;
  dot1agPktFromCpu.fromWire = OFDPA_FALSE;

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pMEPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_OAM_MEG_TYPE_ETHERNET != oamMegType)
  {
    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0)
    {
      return OFDPA_E_FAIL;
    }

    data += len;
    vlanId = oamMapData->vlanId;
  }
  else
  {
    /* Fill DMAC */
    if (y1731MepDmEntry->dmDestIsMepId == OFDPA_FALSE)
    {
      DOT1AG_PUT_DATA(y1731MepDmEntry->dmDestMacAddress, OFDPA_MAC_ADDR_LEN, data);
      memcpy(dot1agPktFromCpu.destMac, y1731MepDmEntry->dmDestMacAddress, OFDPA_MAC_ADDR_LEN);
    }
    else
    {
      /* Sets the destination_address parameter to the value from
         Table 8-9 corresponding to MEP.s MD Level; */
      DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);
      byteVal = dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMEPObj->key.dot1agCfmStackMdLevel;
      DOT1AG_PUT_BYTE(byteVal, data);
      memcpy(dot1agPktFromCpu.destMac, dot1agPktFromCpu.payLoad, OFDPA_MAC_ADDR_LEN);

    }

    /* Fill SMAC */
    DOT1AG_PUT_DATA(pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN, data);
    memcpy(dot1agPktFromCpu.srcMac, pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

    /* Fill DOT1Q tag here, so that validations would go through in the case of relay logic.
     * This gets overwritten anyway with default TPID while transmitting onto the wire.
     */
    shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
    DOT1AG_PUT_SHORT(shortVal, data);

    /* Construct TCI */
    shortVal = 0;
    shortVal  = ((uint16_t)(priority)) << 13;
    shortVal |= ((uint16_t)pMEPObj->key.dot1agCfmStackVlanIdOrNone) & 0x0FFF;
    DOT1AG_PUT_SHORT(shortVal, data);
    vlanId = pMEPObj->key.dot1agCfmStackVlanIdOrNone;
  }

  dot1agPktFromCpu.vlanId   = vlanId;
  /* Fill Ether Type */
  shortVal = OFDPA_DOT1AG_CFM_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);
  dot1agPktFromCpu.cfmPduOffset =  data;

  /* Fill CFM Common Header */
  cfmHdr.level   = pMEPObj->key.dot1agCfmStackMdLevel;
  cfmHdr.version = DOT1AG_CFM_PROTOCOL_VERSION_1;
  cfmHdr.opcode  = DOT1AG_DELAYMEASUREMENT_MESSAGE_OPCODE;
  cfmHdr.flags   = y1731MepDmEntry->dmType & 0x1;
  cfmHdr.first_tlv_offset = DOT1AG_CFM_DMM_FIRST_TLV_OFFSET;
  dot1agCfmHdrWrite(&cfmHdr, data);
  data += DOT1AG_CFM_COMMON_HDR_LEN;
  memcpy(&dot1agPktFromCpu.cfmHdr, &cfmHdr, sizeof(dot1agPktFromCpu.cfmHdr));

  /* Fill DMM header */
  /*debug: txTimeStampf should be filled by HW*/
  /* oamReplacementOffset should point to end CFM common header*/
  offset = data - dot1agPktFromCpu.payLoad;
  llVal = 0x0;
  DOT1AG_PUT_64(llVal, data);
  data = dot1agPktFromCpu.cfmPduOffset + cfmHdr.first_tlv_offset + DOT1AG_CFM_COMMON_HDR_LEN;

  /* optional data TLV */
  packetLength = y1731MepDmEntry->dmPacketLength - (data - dot1agPktFromCpu.payLoad) - 4;
  if (OFDPA_OAM_MEG_TYPE_ETHERNET == oamMegType)
  {
    if (packetLength < 0)
    {
      return OFDPA_E_FAIL;
    }
  }

  if (packetLength > DOT1AG_DATA_TLV_VALUE_LEN_MAX)
  {
    return OFDPA_E_FAIL;
  }

  dataTlv.length =  (y1731MepDmEntry->dmPacketLength - (data - dot1agPktFromCpu.payLoad) - 4 );

  if (packetLength > 0)
  {
    memset(buf, 0, sizeof(buf));
    rc = dot1agCfmDataTlvWrite(buf, dataTlv.length, data, &len);

    if (rc == OFDPA_E_NONE)
    {
      data += len;
    }
  }

  /* End TLV */
  rc = dot1agCfmEndTlvWrite(data, &len);

  if (rc == OFDPA_E_NONE)
  {
    data += len;
  }
  else
  {
    return rc;
  }

  dot1agPktFromCpu.length = data - dot1agPktFromCpu.payLoad;

  dot1agPktFromCpu.lmepId               = pMEPObj->ofdpaLocalMpId;
  dot1agPktFromCpu.oamOpcode            = DOT1AG_DELAYMEASUREMENT_MESSAGE_OPCODE;
  dot1agPktFromCpu.oamReplacementOffset = offset;
  /* Put the pkt on wire */
  (void)dot1agPduTransmit(pMEPObj->key.dot1agCfmStackifIndex,
                          vlanId,
                          &dot1agPktFromCpu);

  DOT1AG_TRACE(CFM_DM, "now tick %d\r\n", dpaUpTimeMilliSeconds());
  /* Send the DMM PDU on the wire */
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose   Method called by the DM SM to process the received LMM
*            message
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t ProcessDMM(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)

{
  uint8_t                *data, type;
  uint16_t                length;
  OFDPA_DOT1AG_MP_type_t  mpType = OFDPA_DOT1AG_MP_TYPE_ETH;

  if (pMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* get MP type */
  if (dot1agMpTypeGet(pMPObj, &mpType) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  /* Point to CFM PDU */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;
  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_DMM_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }
  dot1agPkt->cfmPdu.dmm.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;

  DOT1AG_TRACE(CFM_DM, "Start Processing DMM received on interface: %d\r\n", dot1agPkt->intIfNum);

  if (OFDPA_DOT1AG_MP_TYPE_ETH == mpType)
  {
    /* Check if dest_mac is group address. If Unicast it should be same
       as receiving MP mac address. If group address, it should follow table 8-9.
    */
    if (((dot1agPkt->destMac[0] & 0x01) == 0) &&
        memcmp(dot1agPkt->destMac, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN) != 0)
    {
      DOT1AG_TRACE(CFM_DM, "Unicast DMAC, but not mine\r\n");
      return OFDPA_E_NONE;
    }
    else if ((dot1agPkt->destMac[0] & 0x1) &&
             !(memcmp(dot1agRsrvdMac, dot1agPkt->destMac, OFDPA_MAC_ADDR_LEN-1) == 0 &&
               dot1agPkt->destMac[OFDPA_MAC_ADDR_LEN-1] ==
               (dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMPObj->key.dot1agCfmStackMdLevel)))
    {
      DOT1AG_TRACE(CFM_DM, "Invalid Multicast DMAC[0] %d\r\n", dot1agPkt->destMac[0]);
      return OFDPA_E_NONE;
    }

    /* If the destination_address parameter contains a Group address and the MP Loopback Responder
       state machine resides in an MHF (rather than in a MEP), ProcessLBM() discards the LBM and
       performs no further processing */
    if ((dot1agPkt->destMac[0] & 0x1) && pMPObj->mepData == NULL)
    {
      return OFDPA_E_NONE;
    }

    /* If the source_address parameter is a Group, and not an Individual MAC address, ProcessLBM()
       discards the frame and performs no further processing.  */
    if (dot1agPkt->srcMac[0] & 0x1)
    {
      return OFDPA_E_NONE;
    }
  }

  /* Parse the DMM */
  data = dot1agPkt->cfmPdu.dmm.pFirstTLV;

  DOT1AG_GET_BYTE(type, data);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);
    switch (type)
    {
      /* Sender ID TLV */
      case DOT1AG_TLV_TYPE_SENDER_ID:
        dot1agPkt->cfmPdu.dmm.senderIdTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.dmm.pSenderIdTLV = data - 3;
        dot1agPkt->cfmPdu.dmm.senderIdTLV.length = length;
        if (dot1agCfmSenderIdTlvRead(&dot1agPkt->cfmPdu.dmm.senderIdTLV,
                                     data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        break;
        /* RX_COUNTER TLV */
      case DOT1AG_TLV_TYPE_BRCM_RX_TIMESTAMP:
        dot1agPkt->cfmPdu.dmm.rxTsTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.dmm.pRxTsTLV = data - 3;
        DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmm.rxTimeStampf.second, data); /* set by HW */
        DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmm.rxTimeStampf.ns, data); /* set by HW */
        break;

        /* Org TLV */
      case DOT1AG_TLV_TYPE_ORG_SPECIFIC:
        /* Ignore all other TLVs */
      default:
        break;
    }
    data += length;
    DOT1AG_GET_BYTE(type, data);
  }

  dot1agPkt->cfmPdu.dmm.rxTimeStampf.second = dot1agPkt->timestamp_upper;
  dot1agPkt->cfmPdu.dmm.rxTimeStampf.ns     = dot1agPkt->timestamp;

  /* Terminate DMM */
  dot1agPkt->toFwd = OFDPA_FALSE;
  /* Call xmitDMR() to generate and transmit an DMR */
  xmitDMR(dot1agPkt, pMPObj);
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Transmit DMR on requested MEP
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitDMR(dot1agControlPkt_t *dot1agPkt,
                      dot1agCfmStackObj_t *pMPObj)
{
  uint64_t          llVal;
  uint32_t          uintVal;
  uint8_t          *data;
  uint8_t           payLoad[OFDPA_DOT1AG_MAX_FRAME_SIZE];
  uint8_t           priority = 0;
  uint32_t          len      = 0;
  uint16_t          vlanId;
  OFDPA_ERROR_t     rc;
  uint8_t           ethTypeOffset = (dot1agPkt->cfmPduOffset - dot1agPkt->payLoad) - DOT1AG_ETH_TYPE_LEN;
  int               offset;
  ofdbOamLmepData_t *oamMapData;

  /* Validate destination mac address */
  DOT1AG_TRACE(CFM_DM, "Preparing DMR\r\n");

  if (pMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* Sets the destination_address parameter */
  if (dot1agPkt->destMac[0] & 0x1)
  {
    /* to the value from Table 8-9 corresponding to MEP.s MD Level; */
    data = dot1agPkt->payLoad;
    DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);
    DOT1AG_PUT_BYTE(dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMPObj->key.dot1agCfmStackMdLevel, data);
  }
  else
  {
    /* to the source_address of the received DMM */
    memcpy(dot1agPkt->payLoad, dot1agPkt->srcMac, OFDPA_MAC_ADDR_LEN);
  }

  /* Sets the source_address parameter to the MAC address of the replying MP */
  memcpy(dot1agPkt->payLoad + OFDPA_MAC_ADDR_LEN, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

  /* Changes the OpCode field (21.4.3) from DMM to DMR */
  *(dot1agPkt->cfmPduOffset + 1) = DOT1AG_DELAYMEASUREMENT_REPLY_OPCODE;

  /* Fill DMR header */
  /* keep txTimeStampf */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN + 8;
  /*fill rxTimeStampf according to HW*/
  uintVal = dot1agPkt->cfmPdu.dmm.rxTimeStampf.second;
  DOT1AG_PUT_LONG(uintVal, data);
  uintVal = dot1agPkt->cfmPdu.dmm.rxTimeStampf.ns;
  DOT1AG_PUT_LONG(uintVal, data);
  /*debug: txTimeStampb should be filled by HW*/
  llVal = 0x0;
  DOT1AG_PUT_64(llVal, data);

  dot1agPkt->length = dot1agPkt->length - 4; //Removing CRC.
  memcpy(payLoad, (dot1agPkt->payLoad + ethTypeOffset), (dot1agPkt->length - ethTypeOffset));
  data = dot1agPkt->payLoad;

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pMPObj->ofdpaLocalMpId, &oamMapData);
  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_OAM_MEG_TYPE_ETHERNET != ofdbMegTypeFromVidGet(pMPObj->key.dot1agCfmStackVlanIdOrNone))
  {
    priority = dot1agPkt->priority;

    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0 || (OFDPA_DOT1AG_MAX_FRAME_SIZE < (dot1agPkt->length - ethTypeOffset + len)))
    {
      return OFDPA_E_FAIL;
    }

    data += len;
    memcpy(data, payLoad, (dot1agPkt->length - ethTypeOffset));

    /* oamReplacementOffset should point to TxTimeStampb in DMR PDU.*/
    offset = len + DOT1AG_ETH_TYPE_LEN + DOT1AG_CFM_COMMON_HDR_LEN + sizeof(dot1agPkt->cfmPdu.dmr.txTimeStampf) + sizeof(dot1agPkt->cfmPdu.dmr.rxTimeStampf);
    dot1agPkt->length = (dot1agPkt->length - ethTypeOffset) + len;
    vlanId = oamMapData->vlanId;
  }
  else
  {
    /* oamReplacementOffset should point to TxTimeStampb in DMR PDU.*/
    offset = (dot1agPkt->cfmPduOffset - dot1agPkt->payLoad)  + DOT1AG_CFM_COMMON_HDR_LEN + sizeof(dot1agPkt->cfmPdu.dmr.txTimeStampf) +
      sizeof(dot1agPkt->cfmPdu.dmr.rxTimeStampf);
    vlanId = pMPObj->key.dot1agCfmStackVlanIdOrNone;
  }

  /* Put the pkt on wire */
  if (pMPObj->mepData != NULL)
  {
    dot1agPkt->lmepId               = pMPObj->ofdpaLocalMpId;
    dot1agPkt->oamOpcode            = DOT1AG_DELAYMEASUREMENT_REPLY_OPCODE;
    dot1agPkt->oamReplacementOffset = offset;

    DOT1AG_TRACE(CFM_DM, "Transmitting DMR\r\n");
    (void)dot1agPduTransmit(pMPObj->key.dot1agCfmStackifIndex,
                            vlanId,
                            dot1agPkt);
  }
  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  return  OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Parse DMR Pdu and store int in local CFM PDU message
*           storage
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agParseDMR(dot1agControlPkt_t *dot1agPkt)
{
  uint8_t  *data, type;
  uint16_t  length;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);

  /* Point to CFM PDU */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;

  /* 21.6.2 */
  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_DMR_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }
  dot1agPkt->cfmPdu.dmr.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;

  /* Parse DMR info */
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.txTimeStampf.second, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.txTimeStampf.ns, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.rxTimeStampf.second, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.rxTimeStampf.ns, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.txTimeStampb.second, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.txTimeStampb.ns, data);

  dot1agPkt->cfmPdu.dmr.rxTimeb.second = dot1agPkt->timestamp_upper;
  dot1agPkt->cfmPdu.dmr.rxTimeb.ns = dot1agPkt->timestamp;

  /* Parse available TLVs */
  data = dot1agPkt->cfmPdu.dmr.pFirstTLV;

  DOT1AG_GET_BYTE(type, data);
  DOT1AG_TRACE(CFM_DM, "Parse DMR, type %d\r\n", type);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);
    switch (type)
    {
      /* RX_TIMESTAMP TLV */
      case DOT1AG_TLV_TYPE_BRCM_RX_TIMESTAMP:
        dot1agPkt->cfmPdu.dmr.rxTsTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.dmr.pRxTsTLV = data - 3;
        DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.rxTimeb.second, data); /* set by HW */
        DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.rxTimeb.ns, data); /* set by HW */
        break;

        /* Org TLV */
      case DOT1AG_TLV_TYPE_ORG_SPECIFIC:
        /* Ignore all other TLVs */
      default:
        break;
    }
    data += length;
    DOT1AG_GET_BYTE(type, data);
  }

  /* Parse internal RX_COUNTER TLVs */
  data = dot1agPkt->payLoad + dot1agPkt->length - 11;
  DOT1AG_GET_BYTE(type, data);
  if (type == DOT1AG_TLV_TYPE_BRCM_RX_TIMESTAMP)
  {
    DOT1AG_GET_SHORT(length, data);
    if (length == DOT1AG_TLV_LEN_BRCM_RX_TIMESTAMP)
    {
      dot1agPkt->cfmPdu.dmr.rxTsTLVRead = OFDPA_TRUE;
      dot1agPkt->cfmPdu.dmr.pRxTsTLV = data - 3;
      DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.rxTimeb.second, data); /* set by HW */
      DOT1AG_GET_LONG(dot1agPkt->cfmPdu.dmr.rxTimeb.ns, data); /* set by HW */
    }
  }

  /* Deletes the RX COUNTER TLV, if present in the DMR */
  if (dot1agPkt->cfmPdu.dmr.rxTsTLVRead)
  {
    uint32_t bytesToMove;

    bytesToMove = dot1agPkt->length -
      (dot1agPkt->cfmPdu.dmr.pRxTsTLV - dot1agPkt->payLoad) -
      (DOT1AG_TLV_LEN_BRCM_RX_TIMESTAMP + 3);
    memcpy(dot1agPkt->cfmPdu.dmr.pRxTsTLV,
           dot1agPkt->cfmPdu.dmr.pRxTsTLV + DOT1AG_TLV_LEN_BRCM_RX_TIMESTAMP + 3,
           bytesToMove);
    dot1agPkt->length -= DOT1AG_TLV_LEN_BRCM_RX_TIMESTAMP + 3;
    dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Transmit AIS on requested MEP
*
* @param    pClientMEPData    @b{(input)} MEP on which AIS has to be sent
* @param    pServerMEPData    @b{(input)} MEP on which AIS is enabled
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitAIS(dot1agCfmStackObj_t *pClientMPObj, dot1agCfmMepTable_t *pServerMEPData)
{
  uint8_t              *data, byteVal;
  dot1agCfmCommonHdr_t  cfmHdr;
  uint16_t              shortVal;
  ofdbOamLmepData_t      *oamMapData;
  OFDPA_ERROR_t         rc;
  uint8_t               priority = 0;
  uint32_t              len      = 0;
  uint8_t               srcMac[OFDPA_MAC_ADDR_LEN];
  uint32_t              ifIndex, primVID, mdLevel;

  if (pClientMPObj == NULL ||
      pServerMEPData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  if (pClientMPObj->mepData == NULL) /* If Client is MIP */
  {
    ifIndex = pClientMPObj->key.dot1agCfmStackifIndex;
    primVID = pClientMPObj->key.dot1agCfmStackVlanIdOrNone;
    mdLevel = pClientMPObj->key.dot1agCfmStackMdLevel;
    memcpy(srcMac, pClientMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);
  }
  else /* If Client is MEP */
  {
    ifIndex = pClientMPObj->mepData->dot1agCfmMepIfIndex;
    primVID = pClientMPObj->mepData->dot1agCfmMepPrimaryVid;

    if (NULL == pClientMPObj->mepData->maData ||
        NULL == pClientMPObj->mepData->maData->mdData)
    {
      return OFDPA_E_FAIL;
    }

    mdLevel = pClientMPObj->mepData->maData->mdData->dot1agCfmMdMdLevel;
    memcpy(srcMac, pClientMPObj->mepData->dot1agCfmMepMacAddress, OFDPA_MAC_ADDR_LEN);
  }

  memset(&dot1agPktFromCpu, 0x0, sizeof(dot1agPktFromCpu));
  data = dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.intIfNum   = ifIndex;
  dot1agPktFromCpu.vlanId     = primVID;
  dot1agPktFromCpu.fromWire   = OFDPA_FALSE;

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pClientMPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_OAM_MEG_TYPE_ETHERNET != ofdbMegTypeFromVidGet(primVID))
  {
    priority = pServerMEPData->y1731CfmMepAisSendVar.aisPriority;
    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0)
    {
      return OFDPA_E_FAIL;
    }
    data += len;
  }
  else
  {
    /* Sets the destination_address parameter to the value from
       Table 8-9 corresponding to MEP.s MD Level; */
    DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);

    byteVal = dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | mdLevel;

    DOT1AG_PUT_BYTE(byteVal, data);
    memcpy(dot1agPktFromCpu.destMac, dot1agPktFromCpu.payLoad, OFDPA_MAC_ADDR_LEN);

    /* Sets the source_address parameter to the MAC address of the MEP */
    DOT1AG_PUT_DATA(srcMac, OFDPA_MAC_ADDR_LEN, data);
    memcpy(dot1agPktFromCpu.srcMac, srcMac, OFDPA_MAC_ADDR_LEN);

    /* Sets the priority parameter according to the MEP.s managed objects */
    /* Fill DOT1Q tag here, so that validations would go through in the case of relay logic.
     * This gets overwritten anyway with default TPID while transmitting onto the wire.
     */
    shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
    DOT1AG_PUT_SHORT(shortVal, data);
    /* Construct TCI */
    /* Sets the drop_eligible parameter to false */
    shortVal = 0;

    shortVal  = ((uint16_t)(pServerMEPData->y1731CfmMepAisSendVar.aisPriority)) << 13;
    shortVal |= ((uint16_t)primVID) & 0x0FFF;
    DOT1AG_PUT_SHORT(shortVal, data);
  }

  /* Fill Ether Type */
  shortVal = OFDPA_DOT1AG_CFM_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);
  dot1agPktFromCpu.cfmPduOffset = data;

  /* Places the MEP.s MD Level (mdLevel, 20.7.1) in the MD Level field */
  cfmHdr.level   = mdLevel;
  cfmHdr.version = DOT1AG_CFM_PROTOCOL_VERSION_0;
  cfmHdr.opcode  = DOT1AG_AIS_CHECK_OPCODE;
  cfmHdr.flags   = Y1731_AIS_PERIOD_FIELD_GET(pServerMEPData->y1731CfmMepAisSendVar.aisInterval);

  cfmHdr.first_tlv_offset = DOT1AG_CFM_AIS_FIRST_TLV_OFFSET;
  dot1agCfmHdrWrite(&cfmHdr, data);
  data += DOT1AG_CFM_COMMON_HDR_LEN;
  memcpy(&dot1agPktFromCpu.cfmHdr, &cfmHdr, sizeof(dot1agPktFromCpu.cfmHdr));

  /* End TLV */
  rc = dot1agCfmEndTlvWrite(data, &len);

  if (rc == OFDPA_E_NONE)
  {
    data += len;
  }
  else
  {
    return rc;
  }

  dot1agPktFromCpu.length = data - dot1agPktFromCpu.payLoad;

  dot1agPktFromCpu.lmepId    = pClientMPObj->ofdpaLocalMpId;
  dot1agPktFromCpu.oamOpcode = DOT1AG_AIS_CHECK_OPCODE;

  /* Put the pkt on wire */
  if (dot1agPduTransmit(ifIndex,
                        primVID,
                        &dot1agPktFromCpu) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Failed to transmit AIS packet", 0);
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Check whether AIS is triggered.
*
* @param    pServerMEPObj    @b{(input)}  Server MEP to be checked.
*
* @returns  OFDPA_TRUE or OFDPA_FALSE
*
* @end
**********************************************************************/
OFDPA_BOOL IsAISTriggered(dot1agCfmMepTable_t *pServerMEPObj)
{
  if (pServerMEPObj == NULL)
  {
    return OFDPA_FALSE;
  }

  if (OFDPA_TRUE != pServerMEPObj->y1731CfmMepAisSendVar.aisEnable)
  {
    return OFDPA_FALSE;
  }

  /* (dLOC[1..n] and MI_CC_Enable) or (dAIS and not(MI_CC_Enable)) or
     (dLCK and not(MI_CC_Enable)) or dUNL or dMMG or dUNM  */
  if ((Y1731_MEP_DEFECT_DEFECT == pServerMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_LOC]) &&
      (OFDPA_TRUE == pServerMEPObj->dot1agCfmMepCCIVar.cciEnabled))
  {
    return OFDPA_TRUE;
  }

  if ((Y1731_AIS_RCV_WAITING == pServerMEPObj->y1731CfmMepAisRcvState) &&
      (OFDPA_TRUE != pServerMEPObj->dot1agCfmMepCCIVar.cciEnabled))
  {
    return OFDPA_TRUE;
  }

  if ((Y1731_LCK_RCV_WAITING == pServerMEPObj->y1731CfmMepLckRcvState) &&
      (OFDPA_TRUE != pServerMEPObj->dot1agCfmMepCCIVar.cciEnabled))
  {
    return OFDPA_TRUE;
  }

  if ((Y1731_MEP_DEFECT_DEFECT == pServerMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNL]) ||
      (Y1731_MEP_DEFECT_DEFECT == pServerMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_MMG]) ||
      (Y1731_MEP_DEFECT_DEFECT == pServerMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNM]))
  {
    return OFDPA_TRUE;
  }

  return OFDPA_FALSE;
}

/*********************************************************************
* @purpose  AIS entry or exist handling
*
* @param    pServerMEPObj    @b{(input)}  Server MEP to be checked.
*
* @returns  pointer to dot1agCfmMepTable_t
*
* @end
**********************************************************************/
void AISTriggerHandler(dot1agCfmMepTable_t *pServerMEPObj)
{
  uint32_t                 mdIndex, maIndex, mepIndex, intIfNum;
  uint32_t                 serverMpid;
  uint32_t                 aisClientLevel;
  dot1agCfmStackObj_t     *pServerMPObj = NULL, *pClientMPObj = NULL;
  dot1agCfmMepObj_t       *pClientMEPObj = NULL;
  OFDPA_DOT1AG_MP_type_t   serverMPType;
  OFDPA_ERROR_t            rc = OFDPA_E_NONE;
  dot1agCfmClientMPDBObj_t clientMPDBEntry, *pServerMPDBObj = NULL;

  if (pServerMEPObj == NULL)
  {
    return;
  }

  pServerMPObj = dot1agCfmStackObjGet(pServerMEPObj->dot1agCfmMepIfIndex,
                                      pServerMEPObj->dot1agCfmMepPrimaryVid,
                                      pServerMEPObj->maData->mdData->dot1agCfmMdMdLevel,
                                      pServerMEPObj->dot1agCfmMepDirection, AVL_EXACT);
  if (pServerMPObj == NULL)
  {
    return;
  }

  rc = dot1agMpTypeGet(pServerMPObj, &serverMPType);
  if (rc != OFDPA_E_NONE)
  {
    return;
  }

  if (OFDPA_DOT1AG_MP_TYPE_ETH == serverMPType)
  {

    /* FIXUP: Ethernet support should be modified to use only server MEP information and transmit AIS frame. */
    serverMpid     = pServerMEPObj->dot1agCfmMepIdentifier;
    aisClientLevel = pServerMEPObj->y1731CfmMepAisSendVar.aisClientLevel;
    mdIndex = maIndex = mepIndex = intIfNum = 0;
    while ((pClientMEPObj = dot1agActiveMEPGet(mdIndex, maIndex,
                                               mepIndex, intIfNum, AVL_NEXT)) != NULL)
    {
      mdIndex  = pClientMEPObj->key.dot1agCfmMdIndex;
      maIndex  = pClientMEPObj->key.dot1agCfmMaIndex;
      mepIndex = pClientMEPObj->key.dot1agCfmMepIdentifier;
      intIfNum = pClientMEPObj->key.dot1agCfmMepIfIndex;

      if(NULL == pClientMEPObj->mepData ||
         NULL == pClientMEPObj->mepData->maData ||
         NULL == pClientMEPObj->mepData->maData->mdData)
      {
        return;
      }

      if (pClientMEPObj->mepData->maData->mdData->dot1agCfmMdMdLevel == aisClientLevel)
      {
        DOT1AG_TRACE(CFM_DEFECT,
                     "Matched MEP [%d:%d] mdIndex %d maIndex %d mepIndex %d intIfNum %d.\r\n",
                     serverMpid, mepIndex, mdIndex, maIndex, mepIndex, intIfNum);

        if ((pClientMPObj = dot1agCfmStackObjGet(pClientMEPObj->mepData->dot1agCfmMepIfIndex,
                                                 pClientMEPObj->mepData->dot1agCfmMepPrimaryVid,
                                                 pClientMEPObj->mepData->maData->mdData->dot1agCfmMdMdLevel,
                                                 pClientMEPObj->mepData->dot1agCfmMepDirection, AVL_EXACT)) != NULL)
        {
          xmitAIS(pClientMPObj, pServerMEPObj);
        }
      }
    }
  }
  else if (OFDPA_DOT1AG_MP_TYPE_BHH == serverMPType)
  {
    /* Run through ClientMPDB and transmit AIS PDUs for each entry corresponding to server MEP */
    clientMPDBEntry.key.serverLmepId = pServerMEPObj->dot1agCfmMepIdentifier;
    clientMPDBEntry.key.clientLmepId = 0;
    while ((pServerMPDBObj = dot1agCfmClientMPDBObjGet(clientMPDBEntry.key, AVL_NEXT)) != NULL)
    {
      DOT1AG_TRACE(CFM_DEFECT,
                   "Matched MEP [%d:%d] mdIndex %d maIndex %d mepIndex %d intIfNum %d txDirSameAsServer = %d.\r\n",
                   pServerMPDBObj->key.serverLmepId, pServerMPDBObj->key.clientLmepId,
                   ((pServerMPDBObj->pClientMPObj->mepData != NULL) ? pServerMPDBObj->pClientMPObj->mepData->maData->mdData->dot1agCfmMdIndex : 0),
                   ((pServerMPDBObj->pClientMPObj->mepData != NULL) ? pServerMPDBObj->pClientMPObj->mepData->maData->dot1agCfmMaIndex : 0),
                   ((pServerMPDBObj->pClientMPObj->mepData != NULL) ? pServerMPDBObj->pClientMPObj->mepData->dot1agCfmMepIdentifier : 0),
                   pServerMPDBObj->pClientMPObj->key.dot1agCfmStackifIndex,
                   pServerMPDBObj->txDirSameAsServer);
      /* Transmit AIS in the opposite direction of server MEP */
      if (pServerMPDBObj->txDirSameAsServer == OFDPA_FALSE)
      {
        xmitAIS(pServerMPDBObj->pClientMPObj, pServerMEPObj);
      }

      clientMPDBEntry.key.clientLmepId = pServerMPDBObj->key.clientLmepId;
    }
  }
}

/*********************************************************************
* @purpose  Parse AIS Pdu and store int in local CFM PDU message
*           storage
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCheckAIS(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg, uint32_t *interval)
{
//  uint8_t  *data;
  dot1agControlPkt_t  *dot1agPkt = dot1agCfmEventMsg->dot1agPkt;
  dot1agCfmStackObj_t *pMEPObj;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);

  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* Point to CFM PDU */
//  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;

  if (dot1agPkt->cfmHdr.first_tlv_offset != DOT1AG_CFM_AIS_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }

  if ((dot1agPkt->cfmHdr.flags != Y1731_AIS_PERIOD_FIELD_GET(Y1731_CFM_PERIOD_1PPS)) &&
      (dot1agPkt->cfmHdr.flags != Y1731_AIS_PERIOD_FIELD_GET(Y1731_CFM_PERIOD_1PPM)))
  {
    DOT1AG_TRACE(CFM_DEFECT, "dot1agPkt->cfmHdr.flags = %d\r\n",
                 dot1agPkt->cfmHdr.flags);
    return OFDPA_E_FAIL;
  }

  *interval = Y1731_AIS_PERIOD_FIELD_PARSE(dot1agPkt->cfmHdr.flags);

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Transmit LCK on requested MEP
*
* @param    pServerMEPObj    @b{(input)} Server MEP
* @param    pClientMEPObj    @b{(input)} Client MEP on which LCK has to be sent
*
* @returns  OFDPA_E_NONE or OFDPA_E_NOT_FOUND or OFDPA_E_FAIL
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitLCK(dot1agCfmStackObj_t *pServerMEPObj, dot1agCfmStackObj_t *pClientMEPObj)
{
  uint8_t              *data, byteVal;
  dot1agCfmCommonHdr_t  cfmHdr;
  uint16_t              shortVal;
  OFDPA_ERROR_t         rc;
  uint8_t               priority = 0;
  uint32_t              len      = 0;
  ofdbOamLmepData_t      *oamMapData;
  uint16_t              vlanId;

  if (pServerMEPObj == NULL ||
      pClientMEPObj == NULL ||
      pServerMEPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  memset(&dot1agPktFromCpu, 0x0, sizeof(dot1agPktFromCpu));
  data = dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.intIfNum   = pClientMEPObj->key.dot1agCfmStackifIndex;
  dot1agPktFromCpu.vlanId     = pClientMEPObj->key.dot1agCfmStackVlanIdOrNone;
  dot1agPktFromCpu.fromWire   = OFDPA_FALSE;

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pClientMEPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_OAM_MEG_TYPE_ETHERNET != ofdbMegTypeFromVidGet(pClientMEPObj->key.dot1agCfmStackVlanIdOrNone))
  {
    priority = pServerMEPObj->mepData->y1731CfmMepLckSendVar.lckPriority;

    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0)
    {
      return OFDPA_E_FAIL;
    }
    vlanId = oamMapData->vlanId;
    data += len;
  }
  else
  {
    /* Sets the destination_address parameter to the value from
       Table 8-9 corresponding to MEP.s MD Level; */
    DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);
    byteVal = dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pClientMEPObj->key.dot1agCfmStackMdLevel;
    DOT1AG_PUT_BYTE(byteVal, data);
    memcpy(dot1agPktFromCpu.destMac, dot1agPktFromCpu.payLoad, OFDPA_MAC_ADDR_LEN);

    /* Sets the source_address parameter to the MAC address of the MEP */
    DOT1AG_PUT_DATA(pClientMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN, data);
    memcpy(dot1agPktFromCpu.srcMac, pClientMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

    /* Sets the priority parameter according to the MEP.s managed objects */
    /* Fill DOT1Q tag here, so that validations would go through in the case of relay logic.
     * This gets overwritten anyway with default TPID while transmitting onto the wire.
     */
    shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
    DOT1AG_PUT_SHORT(shortVal, data);
    /* Construct TCI */
    /* Sets the drop_eligible parameter to false */
    shortVal = 0;

    shortVal  = ((uint16_t)(pServerMEPObj->mepData->y1731CfmMepLckSendVar.lckPriority)) << 13;
    shortVal |= ((uint16_t)pClientMEPObj->key.dot1agCfmStackVlanIdOrNone) & 0x0FFF;
    DOT1AG_PUT_SHORT(shortVal, data);
    vlanId = pClientMEPObj->key.dot1agCfmStackVlanIdOrNone;
  }

  /* Fill Ether Type */
  shortVal = OFDPA_DOT1AG_CFM_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);
  dot1agPktFromCpu.cfmPduOffset = data;

  /* Places the MEP.s MD Level (mdLevel, 20.7.1) in the MD Level field */
  cfmHdr.level   = pClientMEPObj->key.dot1agCfmStackMdLevel;
  cfmHdr.version = DOT1AG_CFM_PROTOCOL_VERSION_0;
  cfmHdr.opcode  = DOT1AG_LCK_CHECK_OPCODE;
  cfmHdr.flags   = Y1731_LCK_PERIOD_FIELD_GET(pServerMEPObj->mepData->y1731CfmMepLckSendVar.lckInterval);

  cfmHdr.first_tlv_offset = DOT1AG_CFM_AIS_FIRST_TLV_OFFSET;
  dot1agCfmHdrWrite(&cfmHdr, data);
  data += DOT1AG_CFM_COMMON_HDR_LEN;
  memcpy(&dot1agPktFromCpu.cfmHdr, &cfmHdr, sizeof(dot1agPktFromCpu.cfmHdr));

  /* End TLV */
  rc = dot1agCfmEndTlvWrite(data, &len);

  if (rc == OFDPA_E_NONE)
  {
    data += len;
  }
  else
  {
    return rc;
  }

  dot1agPktFromCpu.length = data - dot1agPktFromCpu.payLoad;

  dot1agPktFromCpu.lmepId    = pClientMEPObj->ofdpaLocalMpId;
  dot1agPktFromCpu.oamOpcode = DOT1AG_LCK_CHECK_OPCODE;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPktFromCpu.length, dot1agPktFromCpu.payLoad);
  /* Put the pkt on wire */
  if (dot1agPduTransmit(pClientMEPObj->key.dot1agCfmStackifIndex,
                        vlanId,
                        &dot1agPktFromCpu) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_BASIC,
                       "Failed to transmit LCK packet", 0);
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Search for the matched client MEP and send out LCK
*
* @param    pServerMEPObj @b{(input)} All matched client MEPs send out LCK
*                                  based on current MEP.
*
* @returns  OFDPA_E_NONE
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitClientLCK(dot1agCfmStackObj_t *pServerMEPObj)
{
  uint32_t                 intIfNum, primVID, mdLevel;
  uint32_t                 clientMpid;
  uint32_t                 serverMpid;
  uint32_t                 clientLevel;
  Dot1agCfmMpDirection_t   direction;
  dot1agCfmStackObj_t     *pClientMEPObj = NULL;
  OFDPA_DOT1AG_MP_type_t   serverMPType;
  OFDPA_ERROR_t            rc = OFDPA_E_NONE;
  dot1agCfmClientMPDBObj_t clientMPDBEntry, *pServerMPDBObj = NULL;

  if (pServerMEPObj == NULL ||
      pServerMEPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  rc = dot1agMpTypeGet(pServerMEPObj, &serverMPType);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  if (OFDPA_DOT1AG_MP_TYPE_ETH == serverMPType)
  {
    /* FIXUP: Ethernet support should be modified to use only server MEP information and transmit LCK frame. */
    serverMpid    = pServerMEPObj->mepData->dot1agCfmMepIdentifier;
    clientLevel   = pServerMEPObj->mepData->y1731CfmMepLckSendVar.lckClientLevel;

    intIfNum = primVID = mdLevel = direction = 0;
    while ((pClientMEPObj = dot1agCfmStackObjGet(intIfNum, primVID, mdLevel,
                                                 direction, AVL_NEXT)) != NULL)
    {
      if (pClientMEPObj->mepData == NULL)
      {
        return OFDPA_E_FAIL;
      }

      intIfNum   = pClientMEPObj->key.dot1agCfmStackifIndex;
      primVID    = pClientMEPObj->key.dot1agCfmStackVlanIdOrNone;
      mdLevel    = pClientMEPObj->key.dot1agCfmStackMdLevel;
      direction  = pClientMEPObj->key.dot1agCfmStackDirection;
      clientMpid = pClientMEPObj->mepData->dot1agCfmMepIdentifier;

      /*Find correct client MEP object.*/
      if (mdLevel == clientLevel)
      {
        DOT1AG_TRACE(CFM_DEFECT,
                     "Matched MEP [%d:%d] intf %d primVID %d mdLevel %d direction %d client level %d.\r\n",
                     serverMpid, clientMpid, intIfNum, primVID, mdLevel, direction, clientLevel);
        xmitLCK(pServerMEPObj, pClientMEPObj);
      }
    }
  }
  else if (OFDPA_DOT1AG_MP_TYPE_BHH == serverMPType)
  {
    /* Run through ClientMPDB and transmit LCK frames for each entry corresponding to server MEP */
    clientMPDBEntry.key.serverLmepId = pServerMEPObj->mepData->dot1agCfmMepIdentifier;
    clientMPDBEntry.key.clientLmepId = 0;
    while (((pServerMPDBObj = dot1agCfmClientMPDBObjGet(clientMPDBEntry.key, AVL_NEXT)) != NULL) &&
           (pServerMPDBObj->key.serverLmepId == pServerMEPObj->mepData->dot1agCfmMepIdentifier))
    {
       DOT1AG_TRACE(CFM_DEFECT,
                   "Matched MEP [%d:%d] intf %d primVID %d mdLevel %d direction %d.\r\n",
                   pServerMPDBObj->key.serverLmepId, pServerMPDBObj->key.clientLmepId,
                   pServerMPDBObj->pClientMPObj->key.dot1agCfmStackifIndex,
                   pServerMPDBObj->pClientMPObj->key.dot1agCfmStackVlanIdOrNone,
                   pServerMPDBObj->pClientMPObj->key.dot1agCfmStackMdLevel,
                   pServerMPDBObj->pClientMPObj->key.dot1agCfmStackDirection);

        xmitLCK(pServerMEPObj, pServerMPDBObj->pClientMPObj);

        clientMPDBEntry.key.clientLmepId = pServerMPDBObj->key.clientLmepId;
    }
  }
  return  OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Parse LCK PDU and return interval field.
*
* @param    dot1agCfmEventMsg  @b{(input)} Local storage for the CFM
*                                  PDU message
*
* @returns  OFDPA_E_NONE or OFDPA_E_FAIL
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCheckLCK(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg, uint32_t *interval)
{
  dot1agControlPkt_t  *dot1agPkt = dot1agCfmEventMsg->dot1agPkt;
  dot1agCfmStackObj_t *pMEPObj;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);

  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL)
  {
    DOT1AG_TRACE(CFM_DEFECT, "Invalid message\r\n");
    return OFDPA_E_FAIL;
  }

  if (dot1agPkt->cfmHdr.first_tlv_offset != DOT1AG_CFM_LCK_FIRST_TLV_OFFSET)
  {
    DOT1AG_TRACE(CFM_DEFECT, "dot1agPkt->cfmHdr.first_tlv_offset = %d\r\n",
                 dot1agPkt->cfmHdr.first_tlv_offset);
    return OFDPA_E_FAIL;
  }

  if ((dot1agPkt->cfmHdr.flags != Y1731_LCK_PERIOD_FIELD_GET(Y1731_CFM_PERIOD_1PPS)) &&
      (dot1agPkt->cfmHdr.flags != Y1731_LCK_PERIOD_FIELD_GET(Y1731_CFM_PERIOD_1PPM)))
  {
    DOT1AG_TRACE(CFM_DEFECT, "dot1agPkt->cfmHdr.flags = %d\r\n",
                 dot1agPkt->cfmHdr.flags);
    return OFDPA_E_FAIL;
  }

  *interval = Y1731_LCK_PERIOD_FIELD_PARSE(dot1agPkt->cfmHdr.flags);

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Transmit CSF on requested MEP
*
* @param    pServerMEPObj @b{(input)} MEP on which CSF has to be sent
* @param    csfType       @b{(input)} CSF type value
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitCSF(dot1agCfmStackObj_t *pMPObj, uint32_t csfType)
{
  uint8_t              *data, byteVal;
  dot1agCfmCommonHdr_t  cfmHdr;
  uint16_t              shortVal;
  ofdbOamLmepData_t      *oamMapData;
  OFDPA_ERROR_t           rc;
  uint8_t                 priority = 0;
  uint32_t                len      = 0;
  dot1agCfmMepTable_t  *pMEPData;

  if (NULL == pMPObj ||
      NULL == pMPObj->mepData)
  {
    return OFDPA_E_FAIL;
  }

  pMEPData = pMPObj->mepData;
  memset(&dot1agPktFromCpu, 0x0, sizeof(dot1agPktFromCpu));
  data = dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.intIfNum   = pMEPData->dot1agCfmMepIfIndex;
  dot1agPktFromCpu.vlanId     = pMEPData->dot1agCfmMepPrimaryVid;
  dot1agPktFromCpu.fromWire   = OFDPA_FALSE;

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pMPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_OAM_MEG_TYPE_ETHERNET != ofdbMegTypeFromVidGet(pMEPData->dot1agCfmMepPrimaryVid))
  {
    priority = pMEPData->y1731CfmMepCsfSendVar.csfPriority;
    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0)
    {
      return OFDPA_E_FAIL;
    }

    data += len;
  }
  else
  {
    /* Sets the destination_address parameter to the value from
       Table 8-9 corresponding to MEP.s MD Level; */
    DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);

    if (NULL == pMEPData->maData ||
        NULL == pMEPData->maData->mdData)
    {
      return OFDPA_E_FAIL;
    }

    byteVal = dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMEPData->maData->mdData->dot1agCfmMdMdLevel;
    DOT1AG_PUT_BYTE(byteVal, data);
    memcpy(dot1agPktFromCpu.destMac, dot1agPktFromCpu.payLoad, OFDPA_MAC_ADDR_LEN);

    /* Sets the source_address parameter to the MAC address of the MEP */
    DOT1AG_PUT_DATA(pMEPData->dot1agCfmMepMacAddress, OFDPA_MAC_ADDR_LEN, data);
    memcpy(dot1agPktFromCpu.srcMac, pMEPData->dot1agCfmMepMacAddress, OFDPA_MAC_ADDR_LEN);

    /* Sets the priority parameter according to the MEP.s managed objects */
    /* Fill DOT1Q tag here, so that validations would go through in the case of relay logic.
     * This gets overwritten anyway with default TPID while transmitting onto the wire.
     */
    shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
    DOT1AG_PUT_SHORT(shortVal, data);
    /* Construct TCI */
    /* Sets the drop_eligible parameter to false */
    shortVal = 0;

    shortVal  = ((uint16_t)(pMEPData->y1731CfmMepCsfSendVar.csfPriority)) << 13;
    shortVal |= ((uint16_t)pMEPData->dot1agCfmMepPrimaryVid) & 0x0FFF;
    DOT1AG_PUT_SHORT(shortVal, data);
  }

  /* Fill Ether Type */
  shortVal = OFDPA_DOT1AG_CFM_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);
  dot1agPktFromCpu.cfmPduOffset = data;

  if (NULL == pMEPData->maData ||
      NULL == pMEPData->maData->mdData)
  {
    return OFDPA_E_FAIL;
  }

  /* Places the MEP.s MD Level (mdLevel, 20.7.1) in the MD Level field */
  cfmHdr.level   = pMEPData->maData->mdData->dot1agCfmMdMdLevel;
  cfmHdr.version = DOT1AG_CFM_PROTOCOL_VERSION_0;
  cfmHdr.opcode  = DOT1AG_CSF_CHECK_OPCODE;
  cfmHdr.flags   = (csfType << 3) |
  Y1731_AIS_PERIOD_FIELD_GET(pMEPData->y1731CfmMepCsfSendVar.csfInterval);

  cfmHdr.first_tlv_offset = DOT1AG_CFM_AIS_FIRST_TLV_OFFSET;
  dot1agCfmHdrWrite(&cfmHdr, data);
  data += DOT1AG_CFM_COMMON_HDR_LEN;
  memcpy(&dot1agPktFromCpu.cfmHdr, &cfmHdr, sizeof(dot1agPktFromCpu.cfmHdr));

  /* End TLV */
  rc = dot1agCfmEndTlvWrite(data, &len);

  if (rc == OFDPA_E_NONE)
  {
    data += len;
  }
  else
  {
    return rc;
  }

  dot1agPktFromCpu.length = data - dot1agPktFromCpu.payLoad;

  dot1agPktFromCpu.lmepId    = pMPObj->ofdpaLocalMpId;
  dot1agPktFromCpu.oamOpcode = DOT1AG_CSF_CHECK_OPCODE;

  /* Put the pkt on wire */
  (void)dot1agPduTransmit(pMEPData->dot1agCfmMepIfIndex,
                          pMEPData->dot1agCfmMepPrimaryVid,
                          &dot1agPktFromCpu);

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Transmit CSF on all Client MEPs
*
* @param    pServerMEPObj @b{(input)} MEP on which CSF has to be sent
* @param    csfType       @b{(input)} CSF type value
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitClientCSF(dot1agCfmStackObj_t *pMPObj, uint32_t csfType)
{
  OFDPA_ERROR_t             rc;
  dot1agCfmMepTable_t      *pMEPData;
  OFDPA_DOT1AG_MP_type_t    mpType;
  dot1agCfmClientMPDBObj_t  clientMPDBEntry, *pMPDBObj = NULL;

  if (NULL == pMPObj)
  {
    return OFDPA_E_FAIL;
  }

  rc = dot1agMpTypeGet(pMPObj, &mpType);
  if (rc != OFDPA_E_NONE)
  {
    return rc;
  }

  pMEPData = pMPObj->mepData;

  if (OFDPA_DOT1AG_MP_TYPE_ETH == mpType)
  {
    rc = xmitCSF(pMPObj, csfType);
  }
  else if (OFDPA_DOT1AG_MP_TYPE_BHH == mpType)
  {
    /* Run through ClientMPDB and transmit CSF frames for each entry corresponding to MEP */
    clientMPDBEntry.key.serverLmepId = pMEPData->dot1agCfmMepIdentifier;
    clientMPDBEntry.key.clientLmepId = 0;
    while (((pMPDBObj = dot1agCfmClientMPDBObjGet(clientMPDBEntry.key, AVL_NEXT)) != NULL) &&
           (pMPDBObj->key.serverLmepId == pMEPData->dot1agCfmMepIdentifier))
    {
      DOT1AG_TRACE(CFM_DEFECT,
                   "Matched MEP [%d:%d] intf %d primVID %d mdLevel %d direction %d.\r\n",
                   pMPDBObj->key.serverLmepId, pMPDBObj->key.clientLmepId,
                   pMPDBObj->pClientMPObj->key.dot1agCfmStackifIndex,
                   pMPDBObj->pClientMPObj->key.dot1agCfmStackVlanIdOrNone,
                   pMPDBObj->pClientMPObj->key.dot1agCfmStackMdLevel,
                   pMPDBObj->pClientMPObj->key.dot1agCfmStackDirection);

      if (pMPDBObj->pClientMPObj->mepData != NULL) /* For BHH case, CSF is generated only by Client MEPs*/
      {
        xmitCSF(pMPDBObj->pClientMPObj, csfType);
      }
      clientMPDBEntry.key.clientLmepId = pMPDBObj->key.clientLmepId;
    }
  }

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Search for the server MEP object through client MEP object
*
* @param    pClientMEPObj    @b{(input)}  Client MEP to be checked.
*
* @returns  pointer to dot1agCfmMepTable_t
*
* @end
**********************************************************************/
dot1agCfmMepTable_t *CSFServerGet(dot1agCfmMepTable_t *pClientMEPObj)
{
  uint32_t           mdIndex, maIndex, mepIndex, intIfNum;
  uint32_t           clientMpid;
  uint32_t           clientLevel;
  dot1agCfmMepObj_t *pServerMEPObj = NULL;

  if (pClientMEPObj == NULL ||
      pClientMEPObj->maData == NULL ||
      pClientMEPObj->maData->mdData == NULL)
  {
    return NULL;
  }

  clientMpid    = pClientMEPObj->dot1agCfmMepIdentifier;
  clientLevel   = pClientMEPObj->maData->mdData->dot1agCfmMdMdLevel;
  mdIndex = maIndex = mepIndex = intIfNum = 0;
  while ((pServerMEPObj = dot1agActiveMEPGet(mdIndex, maIndex,
                                             mepIndex, intIfNum,
                                             AVL_NEXT)) != NULL)
  {
    mdIndex = pServerMEPObj->key.dot1agCfmMdIndex;
    maIndex = pServerMEPObj->key.dot1agCfmMaIndex;
    mepIndex = pServerMEPObj->key.dot1agCfmMepIdentifier;
    intIfNum = pServerMEPObj->key.dot1agCfmMepIfIndex;

    if ((pServerMEPObj->mepData != NULL) &&
        (OFDPA_TRUE == pServerMEPObj->mepData->y1731CfmMepCsfSendVar.csfEnable) &&
        (clientLevel == pServerMEPObj->mepData->y1731CfmMepCsfSendVar.csfClientLevel))
    {
//      if (OFDPA_TRUE == mplsTpOamMpidClientCheck(mepIndex, clientMpid))
      {
        DOT1AG_TRACE(CFM_DEFECT,
                     "Matched MEP [%d:%d] mdIndex %d maIndex %d mepIndex %d intIfNum %d.\r\n",
                     mepIndex, clientMpid, mdIndex, maIndex, mepIndex, intIfNum);
        return pServerMEPObj->mepData;
      }
    }
  }

  return  NULL;
}

/*********************************************************************
* @purpose  CSF entry or exist handling
*
* @param    pServerMEPObj    @b{(input)}  Server MEP to be checked.
*
* @returns  pointer to dot1agCfmMepTable_t
*
* @end
**********************************************************************/
void CSFTriggerHandler(dot1agCfmMepTable_t *pClientMEPObj)
{
  dot1agCfmSMEventMsg_t  eventMsg;
  dot1agCfmMepTable_t   *pServerMEPObj = NULL;

  if (pClientMEPObj == NULL)
  {
    return;
  }

  pServerMEPObj = CSFServerGet(pClientMEPObj);
  if (NULL != pServerMEPObj)
  {
    if (!((pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_RDI] == Y1731_MEP_DEFECT_DEFECT) ||
          (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_LOC] == Y1731_MEP_DEFECT_DEFECT) ||
          (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNPTR] == Y1731_MEP_DEFECT_DEFECT) ||
          (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNP] == Y1731_MEP_DEFECT_DEFECT) ||
          (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNM] == Y1731_MEP_DEFECT_DEFECT) ||
          (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_MMG] == Y1731_MEP_DEFECT_DEFECT) ||
          (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNL] == Y1731_MEP_DEFECT_DEFECT) ||
          (pClientMEPObj->y1731CfmMepAisRcvState == Y1731_AIS_RCV_WAITING)) &&
        (pServerMEPObj->y1731CfmMepCsfSendState == Y1731_CSF_SEND_START))
    {
      eventMsg.pMepData = pServerMEPObj;
      eventMsg.event = y1731CsfSendStop;
      DOT1AG_TRACE(CFM_DEFECT, "Send CSF stop\r\n");

      if ((eventMsg.pMepData != NULL) &&
          (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMepData->dot1agCfmMepIdentifier)))
      {
        (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_TRUE);
      }
      else
      {
        (void)dot1agCfmStateMachineClassifier(&eventMsg, OFDPA_FALSE);
      }
    }
    else if (((pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_RDI] == Y1731_MEP_DEFECT_DEFECT)   ||
              (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_LOC] == Y1731_MEP_DEFECT_DEFECT)   ||
              (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNPTR] == Y1731_MEP_DEFECT_DEFECT) ||
              (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNP] == Y1731_MEP_DEFECT_DEFECT)   ||
              (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNM] == Y1731_MEP_DEFECT_DEFECT)   ||
              (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_MMG] == Y1731_MEP_DEFECT_DEFECT)   ||
              (pClientMEPObj->y1731DefectState[Y1731_DEFECT_TYPE_UNL] == Y1731_MEP_DEFECT_DEFECT)   ||
              (pClientMEPObj->y1731CfmMepAisRcvState == Y1731_AIS_RCV_WAITING)) &&
             (pServerMEPObj->y1731CfmMepCsfSendState == Y1731_CSF_SEND_STOP))
    {
      eventMsg.pMepData = pServerMEPObj;
      eventMsg.event = y1731CsfSendStart;
      DOT1AG_TRACE(CFM_DEFECT, "Send CSF start\r\n");

      if ((eventMsg.pMepData != NULL) &&
          (OFDPA_TRUE == dot1agIsMepHwEnabled(eventMsg.pMepData->dot1agCfmMepIdentifier)))
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
* @purpose  Parse CSF Pdu and store int in local CFM PDU message
*           storage
*
* @param    dot1agCfmEventMsg  @b{(input)} CFM PDU message
* @param    interval           @b{(output)} interval
* @param    type               @b{(output)} type
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCheckCSF(dot1agCfmSMEventMsg_t *dot1agCfmEventMsg,
                             uint32_t *interval, uint32_t *type)
{
//  uint8_t  *data;
  dot1agControlPkt_t  *dot1agPkt = dot1agCfmEventMsg->dot1agPkt;
  dot1agCfmStackObj_t *pMEPObj;
  uint8_t              period;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);

  pMEPObj = dot1agCfmEventMsg->pMPObj;
  if (pMEPObj == NULL)
  {
    DOT1AG_TRACE(CFM_DEFECT, "Invalid message\r\n");
    return OFDPA_E_FAIL;
  }

  /* Point to CFM PDU */
//  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;

  if (dot1agPkt->cfmHdr.first_tlv_offset != DOT1AG_CFM_LCK_FIRST_TLV_OFFSET)
  {
    DOT1AG_TRACE(CFM_DEFECT, "dot1agPkt->cfmHdr.first_tlv_offset = %d\r\n",
                 dot1agPkt->cfmHdr.first_tlv_offset);
    return OFDPA_E_FAIL;
  }

  period = dot1agPkt->cfmHdr.flags & 0x7;
  *type   = ((dot1agPkt->cfmHdr.flags >> 3) & 0x7);

  if ((period != Y1731_AIS_PERIOD_FIELD_GET(Y1731_CFM_PERIOD_1PPS)) &&
      (period != Y1731_AIS_PERIOD_FIELD_GET(Y1731_CFM_PERIOD_1PPM)) &&
      ((*type >= (uint32_t)DOT1AF_CSF_TYPE_INVALID)))
  {
    DOT1AG_TRACE(CFM_DEFECT, "dot1agPkt->cfmHdr.flags = %d\r\n",
                 dot1agPkt->cfmHdr.flags);
    return OFDPA_E_FAIL;
  }

  *interval = Y1731_AIS_PERIOD_FIELD_PARSE(period);

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Retrieves link trace result on a a given MEP
*
* @param    mdIndex        @b{(input)}  Index value to Identify an MD
* @param    maIndex        @b{(input)}  Index value to identify an MA
* @param    mepId          @b{(input)}  Index value to identify an MEP
* @param    ltrSeqNumber    @b{(input)} Index used to identify the
*                                       remote MEP which sent the LTR
* @param    ltrReceiveOrder @b{(input)} An index to distinguish
*                                       among multiple LTRs with the same
*                                       LTR Transaction Identifier field
*                                       value.
* @param    linkTraceReply @b{(output)} link trace session result.
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_ERROR
*
* @notes
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agCfmLtrResultNextGet(uint32_t mdIndex, uint32_t maIndex, uint32_t mepId,
                                        uint32_t ltrSeqNumber, uint32_t ltrReceiveOrder,
                                        dot1agCfmLtrObj_t *linkTraceReply)
{
  OFDPA_ERROR_t      rc = OFDPA_E_FAIL;
  dot1agCfmLtrObj_t *ltrObj;

  /* Validate input */
  if (mdIndex == 0 || maIndex == 0 || mepId == 0)
  {
    return OFDPA_E_ERROR;
  }

  DOT1AG_TRACE(CFM_LTR, "Retrieving Next LTR record for MD[%d] MA[%d] "
                        "MEPID[%d] SequenceNum[%d] ReceiveOrder[%d]\n",
                         mdIndex, maIndex, mepId, ltrSeqNumber, ltrReceiveOrder);

  dpaRWlockReadlock(&dot1agCFM.dot1agCfgRWLock);
  if ((ltrObj = dot1agCfmLtrObjGet(mdIndex, maIndex, mepId, ltrSeqNumber,
                                      ltrReceiveOrder, AVL_NEXT)) != NULL)
  {
    DOT1AG_TRACE(CFM_LTR, "Next entry found for : "
                          "MD[%d] MA[%d] MEPID[%d] SequenceNum[%d] ReceiveOrder[%d]",
                           mdIndex, maIndex, mepId, ltrSeqNumber, ltrReceiveOrder);
    memcpy(linkTraceReply, ltrObj, sizeof(dot1agCfmLtrObj_t));
    rc = OFDPA_E_NONE;
  }
  else
  {
    DOT1AG_TRACE(CFM_LTR, "Next entry not found for : "
                          "MD[%d] MA[%d] MEPID[%d] SequenceNum[%d] ReceiveOrder[%d]",
                           mdIndex, maIndex, mepId, ltrSeqNumber, ltrReceiveOrder);
    rc = OFDPA_E_NOT_FOUND;
  }

  dpaRWlockUnlock(&dot1agCFM.dot1agCfgRWLock);
  return rc;
}

/*********************************************************************
* @purpose   Method called to store and increment SLM based LM counters.
*
* @param    pSlmEntry  @b{(input)} reference to a received SLM Entry
*
* @returns  OFDPA_E_NONE
*           OFDPA_E_FAIL
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agSlmEntryInsert(dot1agCfmSlmObj_t *pSlmEntry)
{
  OFDPA_ERROR_t      rc;

  if (pSlmEntry == NULL)
  {
    return OFDPA_E_FAIL;
  }

  rc = dot1agDBTreeEntryAdd(DOT1AG_DB_ID_SLM_TREE, &dot1agCFM.dot1agSlmTree, pSlmEntry);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to create SLM for mepId 0x%x rmepId 0x%x testId 0x%x\n",
                       pSlmEntry->key.dot1agCfmLmepIdentifier, pSlmEntry->key.dot1agCfmRmepIdentifier, pSlmEntry->key.dot1agCfmTestId);
  }

  return rc;
}

/*********************************************************************
* @purpose   Method called to delete SLM entry.
*
* @param    pSlmEntry  @b{(input)} reference to a received SLM Entry
*
* @returns  OFDPA_E_NONE
*           OFDPA_E_FAIL
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agSlmEntryDelete(dot1agCfmSlmObj_t *pSlmEntry)
{
  OFDPA_ERROR_t      rc;

  if (pSlmEntry == NULL)
  {
    return OFDPA_E_FAIL;
  }

  rc = dot1agDBTreeEntryDelete(DOT1AG_DB_ID_SLM_TREE, &dot1agCFM.dot1agSlmTree, pSlmEntry);

  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_ALWAYS, "Failed to delete SLM for mepId 0x%x rmepId 0x%x testId 0x%x\n",
                       pSlmEntry->key.dot1agCfmLmepIdentifier, pSlmEntry->key.dot1agCfmRmepIdentifier, pSlmEntry->key.dot1agCfmTestId);
  }

  return rc;
}

/*********************************************************************
* @purpose  Transmit SLM on requested MEP for Proactive session
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitProSLM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority)
{
  uint8_t                    lmDestMacAddress[OFDPA_MAC_ADDR_LEN];
  OFDPA_BOOL                 lmDestIsMepId;
  uint16_t                   lmDestMepId;
  y1731CfmMepProlmTable_t   *y1731CfmMepProlmEntry;
  uint32_t                   slmTestId;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL ||
      pMEPObj->mepData->y1731CfmMepProlmEntry == NULL)
  {
    return OFDPA_E_FAIL;
  }

  y1731CfmMepProlmEntry = pMEPObj->mepData->y1731CfmMepProlmEntry;
  slmTestId = pMEPObj->mepData->y1731CfmMepProlmEntry->prolmSlmTestId;
  lmDestIsMepId = y1731CfmMepProlmEntry->prolmDestIsMepId;
  lmDestMepId = y1731CfmMepProlmEntry->prolmDestMepId;

  if (lmDestIsMepId == OFDPA_FALSE)
  {
    memcpy(lmDestMacAddress, y1731CfmMepProlmEntry->prolmDestMacAddress, OFDPA_MAC_ADDR_LEN);
  }

  return xmitSLM(pMEPObj, priority, lmDestMepId, lmDestIsMepId, lmDestMacAddress, slmTestId);
}

/*********************************************************************
* @purpose  Transmit SLM on requested MEP for Ondemand session
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitOndemSLM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority)
{
  uint8_t                    lmDestMacAddress[OFDPA_MAC_ADDR_LEN];
  OFDPA_BOOL                 lmDestIsMepId;
  y1731CfmMepOndemlmTable_t *y1731MepOndemlmEntry;
  uint32_t                   slmTestId;
  uint16_t                   lmDestMepId;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL ||
      pMEPObj->mepData->y1731CfmMepOndemlmEntry[priority] == NULL)
  {
    return OFDPA_E_FAIL;
  }

  y1731MepOndemlmEntry = pMEPObj->mepData->y1731CfmMepOndemlmEntry[priority];
  slmTestId = y1731MepOndemlmEntry->ondemlmSlmTestId;
  lmDestIsMepId = y1731MepOndemlmEntry->ondemlmDestIsMepId;
  lmDestMepId = y1731MepOndemlmEntry->ondemlmDestMepId;
  if (lmDestIsMepId == OFDPA_FALSE)
  {
    memcpy(lmDestMacAddress, y1731MepOndemlmEntry->ondemlmDestMacAddress, OFDPA_MAC_ADDR_LEN);
  }

  return xmitSLM(pMEPObj, priority, lmDestMepId, lmDestIsMepId, lmDestMacAddress, slmTestId);
}

/*********************************************************************
* @purpose  Transmit SLM on requested MEP
*
* @param    pMEPObj    @b{(input)} MEP on which LTM has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitSLM(dot1agCfmStackObj_t *pMEPObj, uint8_t priority, uint32_t lmDestMepId,
                      OFDPA_BOOL lmDestIsMepId, uint8_t *lmDestMacAddress, uint32_t slmTestId)
{
  uint8_t                   *data, byteVal;
  dot1agCfmCommonHdr_t       cfmHdr;
  uint16_t                   shortVal;
  uint32_t                   uintVal;
  uint32_t                   len      = 0;
  OFDPA_ERROR_t              rc;
  OFDPA_OAM_MEG_TYPE_t       oamMegType;
  uint16_t                   vlanId;
  dot1agCfmSlmObj_t         *pSlmEntry    = NULL;
  uint32_t                   txFCf = 0;
  ofdbOamLmepData_t         *oamMapData;

  if (pMEPObj == NULL ||
      pMEPObj->mepData == NULL)
  {
    return OFDPA_E_FAIL;
  }

  oamMegType = ofdbMegTypeFromVidGet(pMEPObj->key.dot1agCfmStackVlanIdOrNone);
  memset(&dot1agPktFromCpu, 0x0, sizeof(dot1agPktFromCpu));
  data = dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.vlanId   = pMEPObj->key.dot1agCfmStackVlanIdOrNone;
  dot1agPktFromCpu.intIfNum = pMEPObj->key.dot1agCfmStackifIndex;
  dot1agPktFromCpu.fromWire = OFDPA_FALSE;

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pMEPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_OAM_MEG_TYPE_ETHERNET != oamMegType)
  {
    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0)
    {
      return OFDPA_E_FAIL;
    }

    data += len;
    vlanId = oamMapData->vlanId;
  }
  else
  {
    /* Fill DMAC */
    if (lmDestIsMepId == OFDPA_FALSE)
    {
      DOT1AG_PUT_DATA(lmDestMacAddress, OFDPA_MAC_ADDR_LEN, data);
      memcpy(dot1agPktFromCpu.destMac, lmDestMacAddress, OFDPA_MAC_ADDR_LEN);
    }
    else
    {
      /* Sets the destination_address parameter to the value from
         Table 8-9 corresponding to MEP.s MD Level; */
      DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);
      byteVal = dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMEPObj->key.dot1agCfmStackMdLevel;
      DOT1AG_PUT_BYTE(byteVal, data);
      memcpy(dot1agPktFromCpu.destMac, dot1agPktFromCpu.payLoad, OFDPA_MAC_ADDR_LEN);
    }

    /* Fill SMAC */
    DOT1AG_PUT_DATA(pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN, data);
    memcpy(dot1agPktFromCpu.srcMac, pMEPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

    shortVal = OFDPA_DOT1AG_8021Q_ETHER_TYPE;
    DOT1AG_PUT_SHORT(shortVal, data);

    /* Construct TCI */
    shortVal = 0;
    shortVal  = ((uint16_t)(priority)) << 13;
    shortVal |= ((uint16_t)pMEPObj->key.dot1agCfmStackVlanIdOrNone) & 0x0FFF;
    DOT1AG_PUT_SHORT(shortVal, data);
    vlanId = pMEPObj->key.dot1agCfmStackVlanIdOrNone;
  }

  /* Fill Ether Type */
  shortVal = OFDPA_DOT1AG_CFM_ETHER_TYPE;
  DOT1AG_PUT_SHORT(shortVal, data);
  dot1agPktFromCpu.cfmPduOffset =  data;

  /* Fill CFM Common Header */
  cfmHdr.level   = pMEPObj->key.dot1agCfmStackMdLevel;
  cfmHdr.version = DOT1AG_CFM_PROTOCOL_VERSION_0;
  cfmHdr.opcode  = DOT1AG_SYNTHETIC_LOSSMEASUREMENT_MESSAGE_OPCODE;
  cfmHdr.flags   = 0;
  cfmHdr.first_tlv_offset = DOT1AG_CFM_SLM_FIRST_TLV_OFFSET;
  dot1agCfmHdrWrite(&cfmHdr, data);
  data += DOT1AG_CFM_COMMON_HDR_LEN;
  memcpy(&dot1agPktFromCpu.cfmHdr, &cfmHdr, sizeof(dot1agPktFromCpu.cfmHdr));

  /* Fill Source MEPID in SLM header */
  shortVal = pMEPObj->mepData->dot1agCfmMepIdentifier;
  DOT1AG_PUT_SHORT(shortVal, data);
  /* Fill Destination MEPID in SLM header */
  shortVal = lmDestMepId;
  DOT1AG_PUT_SHORT(shortVal, data);
  /* Fill TestId in SLM header */
  uintVal = slmTestId;
  DOT1AG_PUT_LONG(uintVal, data);

  pSlmEntry = dot1agCfmSlmObjGet(pMEPObj->mepData->dot1agCfmMepIdentifier, lmDestMepId, slmTestId, AVL_EXACT);
  if (NULL != pSlmEntry)
  {
    txFCf = pSlmEntry->data.txFCl;
  }
  else
  {
    return OFDPA_E_ERROR;
  }

  uintVal = txFCf++;
  DOT1AG_PUT_LONG(uintVal, data);

  data = dot1agPktFromCpu.cfmPduOffset + cfmHdr.first_tlv_offset + DOT1AG_CFM_COMMON_HDR_LEN;
  /* End TLV */
  rc = dot1agCfmEndTlvWrite(data, &len);

  if (rc == OFDPA_E_NONE)
  {
    data += len;
  }
  else
  {
    return rc;
  }

  dot1agPktFromCpu.length = data - dot1agPktFromCpu.payLoad;
  dot1agPktFromCpu.lmepId = pMEPObj->ofdpaLocalMpId;
  dot1agPktFromCpu.oamOpcode = DOT1AG_SYNTHETIC_LOSSMEASUREMENT_MESSAGE_OPCODE;

  /* Put the pkt on wire */
  (void)dot1agPduTransmit(pMEPObj->key.dot1agCfmStackifIndex,
                          vlanId,
                          &dot1agPktFromCpu);

  pSlmEntry->data.txFCl++;

  /* Send the LBM PDU on the wire */
  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose   Method called by the LM SM to process the received SLM
*            message
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t ProcessSLM(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj)
{
  uint8_t                *data, type;
  uint16_t                length;
//  uint8_t    priority;
  OFDPA_DOT1AG_MP_type_t  mpType = OFDPA_DOT1AG_MP_TYPE_ETH;
  dot1agCfmSlmObj_t      *pSlmEntry    = NULL, slmEntry;
  uint16_t                srcMepId, rspMepId;
  uint32_t                testId, txFCf;

  if (pMPObj == NULL)
  {
    return OFDPA_E_FAIL;
  }

  /* get MP type */
  if (dot1agMpTypeGet(pMPObj, &mpType) != OFDPA_E_NONE)
  {
    return OFDPA_E_FAIL;
  }

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  /* Point to CFM PDU */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;
  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_SLM_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }
  dot1agPkt->cfmPdu.slm.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;

  DOT1AG_GET_SHORT(srcMepId, data);
  DOT1AG_GET_SHORT(rspMepId, data);
  DOT1AG_GET_LONG(testId, data);
  DOT1AG_GET_LONG(txFCf, data);

  DOT1AG_TRACE(CFM_LM, "Start Processing SLM received on interface: %d\r\n", dot1agPkt->intIfNum);

  /* Check if dest_mac is group address. If Unicast it should be same
     as receiving MP mac address. If group address, it should follow table 8-9.
  */
  if (((dot1agPkt->destMac[0] & 0x01) == 0) &&
      memcmp(dot1agPkt->destMac, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN) != 0)
  {
    DOT1AG_TRACE(CFM_LM, "Unicast DMAC, but not mine\r\n");
    if (OFDPA_DOT1AG_MP_TYPE_ETH == mpType)
      return OFDPA_E_NONE;
  }
  else if ((dot1agPkt->destMac[0] & 0x1) &&
           !(memcmp(dot1agRsrvdMac, dot1agPkt->destMac, OFDPA_MAC_ADDR_LEN-1) == 0 &&
             dot1agPkt->destMac[OFDPA_MAC_ADDR_LEN-1] ==
             (dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMPObj->key.dot1agCfmStackMdLevel)))
  {
    DOT1AG_TRACE(CFM_LM, "Invalid Multicat DMAC[0] %d\r\n", dot1agPkt->destMac[0]);
    if (OFDPA_DOT1AG_MP_TYPE_ETH == mpType)
      return OFDPA_E_NONE;
  }

  /* If the destination_address parameter contains a Group address and the MP Loopback Responder
     state machine resides in an MHF (rather than in a MEP), ProcessSLM() discards the SLM and
     performs no further processing */
  if ((dot1agPkt->destMac[0] & 0x1) && pMPObj->mepData == NULL)
  {
    return OFDPA_E_NONE;
  }

  /* If the source_address parameter is a Group, and not an Individual MAC address, ProcessSLM()
     discards the frame and performs no further processing.  */
  if (dot1agPkt->srcMac[0] & 0x1)
  {
    return OFDPA_E_NONE;
  }

  /* Parse the LMM */
  data = dot1agPkt->cfmPdu.slm.pFirstTLV;

  DOT1AG_GET_BYTE(type, data);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);
    switch (type)
    {
      /* Sender ID TLV */
      case DOT1AG_TLV_TYPE_SENDER_ID:
        dot1agPkt->cfmPdu.slm.senderIdTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.slm.pSenderIdTLV = data - 3;
        dot1agPkt->cfmPdu.slm.senderIdTLV.length = length;
        if (dot1agCfmSenderIdTlvRead(&dot1agPkt->cfmPdu.slm.senderIdTLV,
                                     data) != OFDPA_E_NONE)
        {
          return OFDPA_E_FAIL;
        }
        break;
        /* RX_COUNTER TLV */
      case DOT1AG_TLV_TYPE_BRCM_RX_COUNTER:
        dot1agPkt->cfmPdu.slm.rxCtrTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.slm.pRxCtrTLV = data - 3;
        DOT1AG_GET_LONG(dot1agPkt->cfmPdu.slm.rxFCl, data); /* set by HW */
        break;

        /* Org TLV */
      case DOT1AG_TLV_TYPE_ORG_SPECIFIC:
        /* Ignore all other TLVs */
      default:
        break;
    }
    data += length;
    DOT1AG_GET_BYTE(type, data);
  }

  pSlmEntry = dot1agCfmSlmObjGet(srcMepId, rspMepId, testId, AVL_EXACT);
  if (NULL != pSlmEntry)
  {
    if (txFCf == 1)
    {
      memset(&(pSlmEntry->data), 0x0, sizeof(pSlmEntry->data));
    }

    pSlmEntry->data.rxFCl++;
  }
  else
  {
    /* Creates a new entry in the SLM tree. */
    memset(&slmEntry, 0x0, sizeof(slmEntry));

    slmEntry.key.dot1agCfmLmepIdentifier  = srcMepId;
    slmEntry.key.dot1agCfmRmepIdentifier  = rspMepId;
    slmEntry.key.dot1agCfmTestId          = testId;

    if (dot1agSlmEntryInsert(&slmEntry) != OFDPA_E_NONE)
    {
      return OFDPA_E_FAIL;
    }
    else
    {
      pSlmEntry = dot1agCfmSlmObjGet(slmEntry.key.dot1agCfmLmepIdentifier, slmEntry.key.dot1agCfmRmepIdentifier, slmEntry.key.dot1agCfmTestId, AVL_EXACT);
      if (NULL != pSlmEntry)
      {
        pSlmEntry->data.rxFCl++;
      }
      else
      {
        return OFDPA_E_FAIL;
      }
    }
  }

  /* Terminate SLM */
  dot1agPkt->toFwd = OFDPA_FALSE;
  /* Call xmitSLR() to generate and transmit an SLR */
  return xmitSLR(dot1agPkt, pMPObj, pSlmEntry);
}

/*********************************************************************
* @purpose  Transmit SLR on requested MEP
*
* @param    pMEPObj    @b{(input)} MEP on which SLR has to be sent
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t xmitSLR(dot1agControlPkt_t *dot1agPkt,
                      dot1agCfmStackObj_t *pMPObj,
                      dot1agCfmSlmObj_t *pSlmEntry)
{
  uint32_t          uintVal;
  uint8_t          *data;
  uint8_t           payLoad[OFDPA_DOT1AG_MAX_FRAME_SIZE];
  uint8_t           priority = 0;
  uint32_t          len      = 0;
  OFDPA_ERROR_t     rc;
  uint8_t           ethTypeOffset = (dot1agPkt->cfmPduOffset - dot1agPkt->payLoad) - DOT1AG_ETH_TYPE_LEN;
  uint16_t          vlanId;
  ofdbOamLmepData_t *oamMapData;

  if (NULL == pMPObj ||
      NULL == pMPObj->mepData)
  {
    DOT1AG_TRACE(CFM_LM, "SLR - pMPObj/mepData missing.\r\n");
    return OFDPA_E_INTERNAL;
  }

  vlanId = pMPObj->key.dot1agCfmStackVlanIdOrNone;

  if (NULL == pSlmEntry)
  {
    DOT1AG_TRACE(CFM_LM, "SLR - pSlmEntry missing.\r\n");
    return OFDPA_E_INTERNAL;
  }

  /* Validate destination mac address */
  DOT1AG_TRACE(CFM_LM, "Preparing SLR\r\n");

  /* Sets the destination_address parameter */
  if (dot1agPkt->destMac[0] & 0x1)
  {
    /* to the value from Table 8-9 corresponding to MEP.s MD Level; */
    data = dot1agPkt->payLoad;
    DOT1AG_PUT_DATA(dot1agRsrvdMac, (OFDPA_MAC_ADDR_LEN-1), data);
    DOT1AG_PUT_BYTE(dot1agRsrvdMac[OFDPA_MAC_ADDR_LEN-1] | pMPObj->key.dot1agCfmStackMdLevel, data);
  }
  else
  {
    /* to the source_address of the received LMM */
    memcpy(dot1agPkt->payLoad, dot1agPkt->srcMac, OFDPA_MAC_ADDR_LEN);
  }

  /* Sets the source_address parameter to the MAC address of the replying MP */
  memcpy(dot1agPkt->payLoad + OFDPA_MAC_ADDR_LEN, pMPObj->mpMacAddress, OFDPA_MAC_ADDR_LEN);

  /* Changes the OpCode field from SLM to SLR */
  *(dot1agPkt->cfmPduOffset + 1) = DOT1AG_SYNTHETIC_LOSSMEASUREMENT_REPLY_OPCODE;

  /* Fill SLR header */
  /* keep txFCf */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN + 12;

  /*fill txFCb according to HW*/
  uintVal = pSlmEntry->data.rxFCl;
  DOT1AG_PUT_LONG(uintVal, data);

  dot1agPkt->length = dot1agPkt->length - 4; //Removing CRC
  memcpy(payLoad, (dot1agPkt->payLoad + ethTypeOffset), (dot1agPkt->length - ethTypeOffset));
  data = dot1agPkt->payLoad;

  /* check Injected Oam Mapping entry */
  rc = ofdbInjectedOamLmepIdMapDataGet(pMPObj->ofdpaLocalMpId, &oamMapData);

  if (rc != OFDPA_E_NONE)
  {
    return OFDPA_E_NOT_FOUND;
  }

  if (OFDPA_OAM_MEG_TYPE_ETHERNET != ofdbMegTypeFromVidGet(pMPObj->key.dot1agCfmStackVlanIdOrNone))
  {
    priority = dot1agPkt->priority;

    len = mplsTpPrepareMplsHeader(oamMapData, priority, data, 0);

    if (len == 0 || (OFDPA_DOT1AG_MAX_FRAME_SIZE < (dot1agPkt->length - ethTypeOffset + len)))
    {
      return OFDPA_E_FAIL;
    }

    data += len;
    memcpy(data, payLoad, (dot1agPkt->length - ethTypeOffset));
    dot1agPkt->length = (dot1agPkt->length - ethTypeOffset) + len;
    vlanId = oamMapData->vlanId;
  }

  dot1agPkt->lmepId = pMPObj->ofdpaLocalMpId;
  dot1agPkt->oamOpcode = DOT1AG_SYNTHETIC_LOSSMEASUREMENT_REPLY_OPCODE;
  /* Put the pkt on wire */
  /* Down entity */
  DOT1AG_TRACE(CFM_LM, "Transmitting SLR in the case of DOWN MEP\r\n");
  (void)dot1agPduTransmit(pMPObj->key.dot1agCfmStackifIndex,
                          vlanId,
                          dot1agPkt);
  pSlmEntry->data.txFCl++;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  return  OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Parse SLR Pdu and store int in local CFM PDU message
*           storage
*
* @param    dot1agPkt  @b{(input)} Local storage for the CFM
*                                  PDU message
* @param    pMPObj     @b{(input)} CMF Stack MP being used for
*                                  processing the PDU
*
* @returns  none
*
* @end
**********************************************************************/
OFDPA_ERROR_t dot1agParseSLR(dot1agControlPkt_t *dot1agPkt)
{
  uint8_t  *data, type;
  uint16_t  length;

  dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  /* Point to CFM PDU */
  data = dot1agPkt->cfmPduOffset + DOT1AG_CFM_COMMON_HDR_LEN;

  /* 21.6.2 */
  if (dot1agPkt->cfmHdr.first_tlv_offset < DOT1AG_CFM_SLR_FIRST_TLV_OFFSET)
  {
    return OFDPA_E_FAIL;
  }
  dot1agPkt->cfmPdu.slr.pFirstTLV   = data + dot1agPkt->cfmHdr.first_tlv_offset;

  /* Parse SLR info */
  DOT1AG_GET_SHORT(dot1agPkt->cfmPdu.slr.srcMepId, data);
  DOT1AG_GET_SHORT(dot1agPkt->cfmPdu.slr.rspMepId, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.slr.testId, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.slr.txFCf, data);
  DOT1AG_GET_LONG(dot1agPkt->cfmPdu.slr.txFCb, data);

  /* Parse available TLVs */
  data = dot1agPkt->cfmPdu.slr.pFirstTLV;

  DOT1AG_GET_BYTE(type, data);
  DOT1AG_TRACE(CFM_LM, "Parse SLR, type %d\r\n", type);
  while (type != DOT1AG_TLV_TYPE_END)
  {
    DOT1AG_GET_SHORT(length, data);
    switch (type)
    {
      /* RX_COUNTER TLV */
      case DOT1AG_TLV_TYPE_BRCM_RX_COUNTER:
        dot1agPkt->cfmPdu.slr.rxCtrTLVRead = OFDPA_TRUE;
        dot1agPkt->cfmPdu.slr.pRxCtrTLV = data - 3;
        DOT1AG_GET_LONG(dot1agPkt->cfmPdu.slr.rxFCl, data); /* set by HW */
        break;

        /* Org TLV */
      case DOT1AG_TLV_TYPE_ORG_SPECIFIC:
        /* Ignore all other TLVs */
      default:
        break;
    }
    data += length;
    DOT1AG_GET_BYTE(type, data);
  }

  /* Parse internal RX_COUNTER TLVs */
  data = dot1agPkt->payLoad + dot1agPkt->length - 7;
  DOT1AG_GET_BYTE(type, data);
  if (type == DOT1AG_TLV_TYPE_BRCM_RX_COUNTER)
  {
    DOT1AG_GET_SHORT(length, data);
    if (length == DOT1AG_TLV_LEN_BRCM_RX_COUNTER)
    {
      dot1agPkt->cfmPdu.slr.rxCtrTLVRead = OFDPA_TRUE;
      dot1agPkt->cfmPdu.slr.pRxCtrTLV = data - 3;
      DOT1AG_GET_LONG(dot1agPkt->cfmPdu.slr.rxFCl, data); /* set by HW */
    }
  }

  /* Deletes the RX COUNTER TLV, if present in the SLR */
  if (dot1agPkt->cfmPdu.slr.rxCtrTLVRead)
  {
    uint32_t bytesToMove;

    bytesToMove = dot1agPkt->length -
      (dot1agPkt->cfmPdu.slr.pRxCtrTLV - dot1agPkt->payLoad) -
      (DOT1AG_TLV_LEN_BRCM_RX_COUNTER + 3);
    memcpy(dot1agPkt->cfmPdu.slr.pRxCtrTLV,
           dot1agPkt->cfmPdu.slr.pRxCtrTLV + DOT1AG_TLV_LEN_BRCM_RX_COUNTER + 3,
           bytesToMove);
    dot1agPkt->length -= DOT1AG_TLV_LEN_BRCM_RX_COUNTER + 3;
    dumpPkt(__FUNCTION__, __LINE__, dot1agPkt->length, dot1agPkt->payLoad);
  }

  return OFDPA_E_NONE;
}
