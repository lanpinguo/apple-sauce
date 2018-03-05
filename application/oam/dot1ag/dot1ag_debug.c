/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
* @filename  dot1ag_debug.c
*
* @purpose   dot1ag debug utilities
*
* @component dot1ag
*
* @comments  none
*
* @create    02/14/2009
*
* @end
*
**********************************************************************/

#include <stdint.h>
#include <stdarg.h>
#include "dot1ag.h"
#include "dot1ag_pdu.h"
#include "dot1ag_debug.h"
#include "dot1ag_db.h"

#define DOT1AG_DEBUG_PACKET_RX_FORMAT "Pkt RX - Intf: (%d), Svid: %d SrcMac: %s DestMac: %s Code: %s"
#define DOT1AG_DEBUG_PACKET_TX_FORMAT "Pkt TX - Intf: (%d), Svid: %d SrcMac: %s DestMac: %s Code: %s"

#define DOT1AG_USER_TRACE_TX(__fmt__, ... )                             \
  if (dot1agDebugPacketTraceTxFlag == OFDPA_TRUE)                       \
  {                                                                     \
    ofdpaDebugPrintf(__FUNCTION__, OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_TOO_VERBOSE, __fmt__, __VA_ARGS__); \
  }

#define DOT1AG_USER_TRACE_RX(__fmt__, ... )                             \
  if (dot1agDebugPacketTraceRxFlag == OFDPA_TRUE)                       \
  {                                                                     \
    ofdpaDebugPrintf(__FUNCTION__, OFDPA_COMPONENT_Y1731, OFDPA_DEBUG_TOO_VERBOSE, __fmt__, __VA_ARGS__); \
  }

OFDPA_BOOL dot1agDebugPacketTraceTxFlag = OFDPA_FALSE;
OFDPA_BOOL dot1agDebugPacketTraceRxFlag = OFDPA_FALSE;
void dot1agDebugPacketTrace(uint32_t intIfNum, uint32_t svid,
                            void *bufHandle,
                            OFDPA_BOOL rxFlag, OFDPA_BOOL txFlag);

uint32_t debugTraceFlags = 0;

const char *dot1agDebugHwEventNames[] =
{
  "OFDPA_DOT1AG_EVENT_none",
  "OFDPA_DOT1AG_EVENT_defRDICCM",
  "OFDPA_DOT1AG_EVENT_defMACstatus",
  "OFDPA_DOT1AG_EVENT_defRemoteCCM",
  "OFDPA_DOT1AG_EVENT_defErrorCCM",
  "OFDPA_DOT1AG_EVENT_defXconCCM",
  "OFDPA_DOT1AG_EVENT_defIntfStatusDefect",
  "OFDPA_DOT1AG_EVENT_defPortStatusDefect"
};

const char *dot1agDebugEventNames[] =
{
  /***************************************************************/
  /* Events shared with all                                      */
  /***************************************************************/
  "dot1agControlBegin",

  /***************************************************************/
  /* Events from Management commands                             */
  /***************************************************************/
  "dot1agCfmCreate",
  "dot1agCfmDelete",
  "dot1agCfmMDCreate",
  "dot1agCfmMDDelete",
  "dot1agCfmMACreate",
  "dot1agCfmMADelete",
  "dot1agCfmMEPCreate",
  "dot1agCfmMEPDelete",
  "dot1agCfmMIPCreate",
  "dot1agCfmMIPDelete",
  "dot1agCfmRMEPEntryAdd",
  "dot1agCfmRMEPEntryDelete",
  "dot1agCfmRMEPDelete",
  "dot1agCfmMEPActiveMode",
  "dot1agCfmMEPCciEnabled",
  "dot1agCfmLBMSend",
  "dot1agCfmLBMStop",
  "dot1agCfmLTMSend",
  "dot1agCfmLTRCacheClear",
  "dot1agCfmArchiveTimeout",

  "dot1agCfmAdminModeEnable",
  "dot1agCfmAdminModeDisable",
  "dot1agCfmPortInitializeSet",
  "dot1agCfmLogicalPortInitializeSet",
  "dot1agMepAdminControlledDirectionsSet",
  "dot1agMepControlModeSet",
  "dot1agMepVlanModeEnable",
  "dot1agMepVlanModeDisable",
  "dot1agCfmLTFwhileTimerSet",
  "dot1agCfmCCIwhileTimerSet",
  "dot1agCfmErrorCCMWhileTimerSet",
  "dot1agCfmXconCCMWhileTimerSet",
  "dot1agCfmLTITimeoutSet",
  "dot1agCfmFngTimeoutSet",
  "dot1agMEPStatsClear",
  "dot1agMDStatsClear",
  "dot1agMAStatsClear",
  "dot1agMgmtApplyConfigData",
  "dot1agMgmtApplyPortConfigData",
  "dot1agCfmCCMIntervalSet",
  "dot1agCfmMEPProlmCreate",
  "dot1agCfmMEPProlmDelete",
  "dot1agCfmMEPProlmEnabled",
  "dot1agMepOndemLMSet",
  "dot1agMepOndemLMMStart",
  "dot1agMepOndemLMMStop",
  "dot1agMepDMMStart",
  "dot1agMepDMMStop",
  "dot1agMgmtEvents",
  /***************************************************************/
  /* Events from network pdu received                            */
  /***************************************************************/
  /* Modified by JiaoYong, 2016/6/24 Óëdot1agControlEvents_t¶ÔÓ¦*/
  #if 0
  "dot1agPduReceived",
  "dot1agNetworkEvents",
  #else
  "dot1agHwEventReceived",
  #endif
  /***************************************************************/
  /* Events from interface state changes                         */
  /***************************************************************/
  "dot1agIntfChange",

  "dot1agIntfChangeEvents",

  /* Modified by JiaoYong, 2016/6/24 Óëdot1agControlEvents_t¶ÔÓ¦*/
  #if 1
  "dot1agCfmTimerTick",
  #endif

  /***************************************************************/
  /* Events from Dot1s state changes                              */
  /***************************************************************/
  "dot1agDot1sIntfChange",

  /***************************************************************/
  /* Events from Vlan state changes                              */
  /***************************************************************/

  "dot1agVlanDeleteEvent",
  "dot1agVlanAddEvent",
  "dot1agVlanAddPortEvent",
  "dot1agVlanDeletePortEvent",
  "dot1agVlanSvidChangeEvent",
  /***************************************************************/
  /* Events from configurator                                    */
  /***************************************************************/
  "dot1agCnfgr",

  "dot1agCnfgrEvents",

  /***************************************************************/
  /* Events from configurator                                    */
  /***************************************************************/
  "dot1agCfmTimerTick",

  /***************************************************************/
  /* NIM startup events                                          */
  /***************************************************************/
  "dot1agIntfStartupActivate",

  /***************************************************************/
  /* "dot1ag queue synchronize                                    */
  /***************************************************************/
  "dot1agQueueSynchronize",

  /***************************************************************/
  /* "dot1ag AIS LCK CSF                                   */
  /***************************************************************/
  "dot1agCfmDefectAis",
  "dot1agCfmDefectLck",
  "dot1agCfmDefectCsf"
};

#define DOT1AG_DEBUG_EVENT_NAME_TABLE_COUNT (sizeof(dot1agDebugEventNames)/sizeof(dot1agDebugEventNames[0]))

const char *dot1agDebugEventNameGet(uint32_t event)
{
  int dot1agEventNameTableIndex;

  if (event < dot1agControlBegin)
  {
    return("[event value out of range]");
  }

  dot1agEventNameTableIndex = event - dot1agControlBegin;

  if (dot1agEventNameTableIndex >= DOT1AG_DEBUG_EVENT_NAME_TABLE_COUNT)
  {
    return("[event value out of range]");
  }

  return(dot1agDebugEventNames[dot1agEventNameTableIndex]);
}

/*********************************************************************
*
* @purpose Trace dot1ag packets received
*
* @param   intIfNum @b{(input)} Internal Interface Number
* @param   bufHandle  @b{(input)} packet
*
* @returns void
*
* @notes
*
* @end
*
*********************************************************************/
void dot1agDebugPacketRxTrace(uint32_t intIfNum, uint32_t svid,
                              void *bufHandle)
{
  if ((dot1agDebugPacketTraceRxFlag != OFDPA_TRUE))
  {
    return;
  }

  dot1agDebugPacketTrace(intIfNum, svid, bufHandle, OFDPA_TRUE, OFDPA_FALSE);
  return;
}

/*********************************************************************
*
* @purpose Trace dot1ag packets transmitted
*
* @param   intIfNum @b{(input)} Internal Interface Number
* @param   bufHandle  @b{(input)} packet
*
* @returns void
*
* @notes
*
* @end
*
*********************************************************************/
void dot1agDebugPacketTxTrace(uint32_t intIfNum, uint32_t svid, void *bufHandle)
{
  if ((dot1agDebugPacketTraceTxFlag != OFDPA_TRUE))
  {
    return;
  }

  dot1agDebugPacketTrace(intIfNum, svid, bufHandle, OFDPA_FALSE, OFDPA_TRUE);
  return;
}

/*********************************************************************
*
* @purpose Trace dot1ag packets
*
* @param   intIfNum @b{(input)} Internal Interface Number
* @param   bufHandle  @b{(input)} packet
*
* @returns void
*
* @notes
*
* @end
*
*********************************************************************/
void dot1agDebugPacketTrace(uint32_t intIfNum, uint32_t svid, void *bufHandle,
                            OFDPA_BOOL rxFlag, OFDPA_BOOL txFlag)
{
  ofdpa_enetHeader_t *enetHdr;
  uint8_t             srcMac[20];
  uint8_t             destMac[20];
  uint8_t             code[25];
  uint32_t            pduDataOffset;
  uint8_t            *data, *opCode;

  data          = (bufHandle);
  enetHdr       = (ofdpa_enetHeader_t *)(data);
  pduDataOffset = dpaDataOffsetGet(data);
  opCode        = data + pduDataOffset + sizeof(uint8_t);

  snprintf(srcMac, sizeof(srcMac), "%02x:%02x:%02x:%02x:%02x:%02x", enetHdr->src.addr[0], enetHdr->src.addr[1],
           enetHdr->src.addr[2], enetHdr->src.addr[3], enetHdr->src.addr[4], enetHdr->src.addr[5]);
  snprintf(destMac, sizeof(destMac), "%02x:%02x:%02x:%02x:%02x:%02x", enetHdr->dest.addr[0], enetHdr->dest.addr[1],
           enetHdr->dest.addr[2], enetHdr->dest.addr[3], enetHdr->dest.addr[4], enetHdr->dest.addr[5]);

  switch(*opCode)
  {
    case DOT1AG_CONTINUITY_CHECK_OPCODE:
      snprintf(code, sizeof(code), "CCM PDU");
      break;

    case DOT1AG_LOOPBACK_REPLY_OPCODE:
      snprintf(code, sizeof(code), "LB REPLY PDU");
      break;

    case DOT1AG_LOOPBACK_MESSAGE_OPCODE:
      snprintf(code, sizeof(code), "LBM PDU");
      break;

    case DOT1AG_LINKTRACE_REPLY_OPCODE:
      snprintf(code, sizeof(code), "LTM REPLY PDU");
      break;

    case DOT1AG_LINKTRACE_MESSAGE_OPCODE:
      snprintf(code, sizeof(code), "LTM PDU");
      break;

    case DOT1AG_LOSSMEASUREMENT_MESSAGE_OPCODE:
      snprintf(code, sizeof(code), "LMM PDU");
      break;

    case DOT1AG_LOSSMEASUREMENT_REPLY_OPCODE:
      snprintf(code, sizeof(code), "LMR PDU");
      break;

    case DOT1AG_DELAYMEASUREMENT_MESSAGE_OPCODE:
      snprintf(code, sizeof(code), "DMM PDU");
      break;

    case DOT1AG_DELAYMEASUREMENT_REPLY_OPCODE:
      snprintf(code, sizeof(code), "DMR PDU");
      break;

    case DOT1AG_AIS_CHECK_OPCODE:
      snprintf(code, sizeof(code), "AIS PDU");
      break;

    case DOT1AG_LCK_CHECK_OPCODE:
      snprintf(code, sizeof(code), "LCK PDU");
      break;

    case DOT1AG_CSF_CHECK_OPCODE:
      snprintf(code, sizeof(code), "CSF PDU");
      break;

    default:
      break;
  }

  if ((dot1agDebugPacketTraceRxFlag == OFDPA_TRUE) && (rxFlag == OFDPA_TRUE))
  {
    DOT1AG_USER_TRACE_RX(DOT1AG_DEBUG_PACKET_RX_FORMAT,
                         intIfNum, svid, srcMac, destMac, code);
  }
  else if ((dot1agDebugPacketTraceTxFlag == OFDPA_TRUE) && (txFlag == OFDPA_TRUE))
  {
    DOT1AG_USER_TRACE_TX(DOT1AG_DEBUG_PACKET_TX_FORMAT,
                         intIfNum, svid, srcMac, destMac, code);
  }

  return;
}

/*********************************************************************
* @purpose  Turns on/off the displaying of dot1ag packet debug info
*
* @param    flag         new value of the Packet Debug flag
*
* @returns  OFDPA_E_NONE
* @returns  OFDPA_E_FAIL
*
* @notes
*
* @end
*********************************************************************/
OFDPA_ERROR_t dot1agDebugPacketTraceFlagSet(OFDPA_BOOL transmitFlag, OFDPA_BOOL receiveFlag)
{
  dot1agDebugPacketTraceTxFlag = transmitFlag;
  dot1agDebugPacketTraceRxFlag = receiveFlag;

  /* Indicate outstanding configuration change to be copied.
     The config will be copied from the operational structure
     into the config structure when the user requests a
     debug config save.
  */

  return OFDPA_E_NONE;
}

/*********************************************************************
* @purpose  Get the current status of displaying dot1ag packet debug info
*
* @param    none
*
* @returns  OFDPA_TRUE
* @returns  OFDPA_FALSE
*
* @notes
*
* @end
*********************************************************************/
void dot1agDebugPacketTraceFlagGet(OFDPA_BOOL *transmitFlag, OFDPA_BOOL *receiveFlag)
{
  *transmitFlag = dot1agDebugPacketTraceTxFlag;
  *receiveFlag =  dot1agDebugPacketTraceRxFlag;
  return;
}

uint32_t dot1agDebugTraceFlagHelp(void)
{
  printf("\nCFM_NONE         =        0x00");
  printf("\nCFM_PDU          =        0x01");
  printf("\nCFM_OPCODE_DEMUX =        0x02");
  printf("\nCFM_SM_CCI       =        0x04");
  printf("\nCFM_SM_CCR       =        0x08");
  printf("\nCFM_SM_RMEP      =        0x10");
  printf("\nCFM_SM_RMEP_ERR  =        0x20");
  printf("\nCFM_SM_XCON      =        0x40");
  printf("\nCFM_SM_LBR       =        0x80");
  printf("\nCFM_SM_LBI       =       0x100");
  printf("\nCFM_SM_LBIR      =       0x200");
  printf("\nCFM_SM_FNG       =       0x400");
  printf("\nCFM_SM_LTIR      =       0x800");
  printf("\nCFM_SM_LTRR      =      0x1000");
  printf("\nCFM_SM_LTRT      =      0x2000");
  printf("\nCFM_LTM          =      0x4000");
  printf("\nCFM_LBM          =      0x8000");
  printf("\nCFM_CCM          =     0x10000");
  printf("\nCFM_LTR          =     0x20000");
  printf("\nCFM_LBR          =     0x40000");
  printf("\nCFM_TIMERS       =     0x80000");
  printf("\nCFM_MPOBJ        =    0x100000");
  printf("\nCFM_HW_EVENTS    =    0x200000");
  printf("\nCFM_EVENTS       =    0x400000");
  printf("\nCFM_STP_EVENTS   =    0x800000");
  printf("\nCFM_LM           =   0x1000000");
  printf("\nCFM_DM           =   0x2000000");
  printf("\nCFM_PM           =   0x4000000");
  printf("\nCFM_DEFECT       =   0x8000000");
  return 0;
}

void dot1agDebugTraceFlagSet(OFDPA_BOOL mode, DOT1AG_TRACE_FLAGS_t flags)
{
  if (mode == OFDPA_TRUE)
  {
    debugTraceFlags = flags;
  }
  else
  {
    debugTraceFlags = CFM_NONE;
  }
}

void dot1agDebugDumpClientMPDB(uint32_t serverLmepId, uint32_t clientLmepId)
{
  uint8_t i = 0;
  dot1agCfmClientMPDBObj_t clientMPDBEntry, *pServerMPDBObj = NULL;
  clientMPDBEntry.key.serverLmepId = serverLmepId;
  clientMPDBEntry.key.clientLmepId = clientLmepId;
  printf("---------------------- Client MP DB Start ----------------------\n");
  printf("ServerLmep - ClientLmep - ServerLocalMP - TxDirSameAsServer - ClientMPObj\n");
  while ((pServerMPDBObj = dot1agCfmClientMPDBObjGet(clientMPDBEntry.key, AVL_NEXT)) != NULL)
  {
    printf("%-10d - %-10d - %-13d - %-17s - %-11p\n",
           pServerMPDBObj->key.serverLmepId, pServerMPDBObj->key.clientLmepId,
           pServerMPDBObj->serverOfdpaLocalMpId,
           ((pServerMPDBObj->txDirSameAsServer == 1) ? "TRUE":"FALSE"),
           pServerMPDBObj->pClientMPObj);
    printf("\n");
    clientMPDBEntry.key.serverLmepId = pServerMPDBObj->key.serverLmepId;
    clientMPDBEntry.key.clientLmepId = pServerMPDBObj->key.clientLmepId;
    i++;
    if (i > 10) break;
  }
  printf("---------------------- Client MP DB End ----------------------\n");
}
