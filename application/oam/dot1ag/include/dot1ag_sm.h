/*********************************************************************
 *
 * (C) Copyright Broadcom Corporation 2000-2015
 *
 **********************************************************************
 * @filename  dot1ag_sm.h
 *
 * @purpose   Header file for CFM State Machines
 *
 * @component DOT1AG
 *
 * @comments  none
 *
 * @create    02/19/2009
 *
 * @end
 **********************************************************************/
#ifndef INCLUDE_DOT1AG_SM_H
#define INCLUDE_DOT1AG_SM_H

#include "dot1ag_proto.h"

typedef enum dot1agSMEvents_s
{
  /***************************************************************/
  /* Events 20.12 MEP Continuity Check Initiator state machine (CCI)      */
  /***************************************************************/
  dot1agCCIEventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agCCIBegin,
  dot1agCCINotMepActive,
  dot1agCCIEnabled,
  dot1agCCIMacStatusChanged,
  dot1agCCIWhileZero,
  dot1agCCINotEnabled,
  dot1agCCIEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */


  dot1agCCREventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agCCRBegin,
  dot1agCCRNotMepActive,
  dot1agCCRCCMRcvdEqual,
  dot1agCCRCCMRcvdLow,
  dot1agCCREventsEnd,    /* dummy Event To keep track of appropriate SM allocation */


  dot1agCCRMhfEventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agCCRMhfBegin,
  dot1agCCRMhfRecvdCCM,
  dot1agCCRMhfEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */


  dot1agRmepEventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agRmepBegin,
  dot1agRmepNotMepActive,
  dot1agRmepNotEnableRmepDefect,
  dot1agRmeprCCMRecvd,
  dot1agRmeprCCMRecvdWhileNotZero,
  dot1agRmepWhileZero,
  dot1agRmepEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  dot1agRmepErrEventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agRmepErrBegin,
  dot1agRmepErrNotMepActive,
  dot1agRmepErrCCMRecvd,
  dot1agRmepErrCCMRecvdWhileNotZero,
  dot1agRmepErrCCMWhileZero,
  dot1agRmepErrEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  dot1agXConEventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agXConMepBegin,
  dot1agXConNotNMepActive,
  dot1agXConCCMRecvd,
  dot1agXConCCMRecvdWhileNotZero,
  dot1agXConCCMWhileZero,
  dot1agXConEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  dot1agLBREventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agLBRBegin,
  dot1agLBRNotMepActive,
  dot1agLBRLBMRecvd,
  dot1agLBREventsEnd,    /* dummy Event To keep track of appropriate SM allocation */


  dot1agLBIEventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agLBIBegin,
  dot1agLBINotMepActive,
  dot1agLBILBMsToSend,
  dot1agLBITestStop,
  dot1agLBIEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */


  dot1agLBIREventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agLBIRBegin,
  dot1agLBIRNotMepActive,
  dot1agLBIRLBRRecvd,
  dot1agLBIREventsEnd,    /* dummy Event To keep track of appropriate SM allocation */


  dot1agFNGEventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agFNGBegin,
  dot1agFNGNotMepActive,
  dot1agFNGMaDefectIndicatn,
  dot1agFNGNotMaDefectIndicatn,
  dot1agFNGMaDefectIndicatnFNGWhileZero,
  dot1agFNGMaDefectIndicatnHighDefectPrio,
  dot1agFNGNotMaDefectIndicatnFNGWhileZero,
  dot1agFNGEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  dot1agLTIREventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agLTIRBegin,
  dot1agLTIRNotMepActive,
  dot1agLTIRLTRRecvd,
  dot1agLTIREventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  dot1agLTRREventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agLTRRBegin,
  dot1agLTRRNotMepActive,
  dot1agLTRRLTMRecvd,
  dot1agLTRREventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  dot1agLTRTEventsStart,    /* dummy Event To keep track of appropriate SM event */
  dot1agLTRTBegin,
  dot1agLTRTNotMepActive,
  dot1agLTRTLtfWhileZero,
  dot1agLTRTnPendingLTRs,
  dot1agLTRTnPendingLTRsZero,
  dot1agLTRTEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731PROLMEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731PROLMBegin,
  y1731PROLMNotMepActive,
  y1731PROLMCCMRecvd,
  y1731PROLMLMRRecvd,
  y1731PROLMSLRRecvd,
  y1731PROLMWhileZero,
  y1731PROLMEnabled,
  y1731PROLMNotEnabled,
  y1731PROLMEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731ONDEMLMMEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731ONDEMLMMBegin,
  y1731ONDEMLMMNotMepActive,
  y1731ONDEMLMMStart,
  y1731ONDEMLMMStop,
  y1731ONDEMLMMWhileZero,
  y1731ONDEMLMMLMRRecvd,
  y1731ONDEMLMMSLRRecvd,
  y1731ONDEMLMMEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731LMREventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731LMRBegin,
  y1731LMRNotMepActive,
  y1731LMRLMMRecvd,
  y1731LMRSLMRecvd,
  y1731LMREventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731DMMEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731DMMBegin,
  y1731DMMNotMepActive,
  y1731DMMStart,
  y1731DMMStop,
  y1731DMMWhileZero,
  y1731DMMDMRRecvd,
  y1731DMMEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731DMREventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731DMRBegin,
  y1731DMRNotMepActive,
  y1731DMRDMMRecvd,
  y1731DMREventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731AisSendEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731AisSendBegin,
  y1731AisSendNotMepActive,
  y1731AisSendWhileZero,
  y1731AisSendStart,
  y1731AisSendStop,
  y1731AisSendEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731AisRcvEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731AisRcvBegin,
  y1731AisRcvNotMepActive,
  y1731AisRcvWhileZero,    /* timeout or DCI received. */
  y1731AisRcvReceived,
  y1731AisRcvEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731LckSendEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731LckSendBegin,
  y1731LckSendNotMepActive,
  y1731LckSendWhileZero,
  y1731LckSendStart,
  y1731LckSendStop,
  y1731LckSendEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731LckRcvEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731LckRcvBegin,
  y1731LckRcvNotMepActive,
  y1731LckRcvWhileZero,
  y1731LckRcvReceived,
  y1731LckRcvEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731CsfSendEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731CsfSendBegin,
  y1731CsfSendNotMepActive,
  y1731CsfSendWhileZero,
  y1731CsfSendStart,
  y1731CsfSendStop,
  y1731CsfSendEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731CsfRcvEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731CsfRcvBegin,
  y1731CsfRcvNotMepActive,
  y1731CsfRcvWhileZero,
  y1731CsfRcvReceived,
  y1731CsfRcvEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731dUNPtrEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731dUNPtrMepBegin,
  y1731dUNPtrNotNMepActive,
  y1731dUNPtrCCMRecvd,
  y1731dUNPtrCCMRecvdWhileNotZero,
  y1731dUNPtrCCMWhileZero,
  y1731dUNPtrEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731dUNLEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731dUNLMepBegin,
  y1731dUNLNotNMepActive,
  y1731dUNLCCMRecvd,
  y1731dUNLCCMRecvdWhileNotZero,
  y1731dUNLCCMWhileZero,
  y1731dUNLEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731dMMGEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731dMMGMepBegin,
  y1731dMMGNotNMepActive,
  y1731dMMGCCMRecvd,
  y1731dMMGCCMRecvdWhileNotZero,
  y1731dMMGCCMWhileZero,
  y1731dMMGEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731dUNPEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731dUNPMepBegin,
  y1731dUNPNotNMepActive,
  y1731dUNPCCMRecvd,
  y1731dUNPCCMRecvdWhileNotZero,
  y1731dUNPCCMWhileZero,
  y1731dUNPEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

  y1731dUNMEventsStart,    /* dummy Event To keep track of appropriate SM event */
  y1731dUNMMepBegin,
  y1731dUNMNotNMepActive,
  y1731dUNMCCMRecvd,
  y1731dUNMCCMRecvdWhileNotZero,
  y1731dUNMCCMWhileZero,
  y1731dUNMEventsEnd,    /* dummy Event To keep track of appropriate SM allocation */

} dot1agSMEvents_t;

#define  DOT1AG_MEP_CCI_EVENTS_MAX (dot1agCCIEventsEnd - dot1agCCIEventsStart - 1)
typedef enum dot1agMEPCcistates_s
{
  DOT1AG_MEP_CCI_IDLE,
  DOT1AG_MEP_CCI_WAITING,
  DOT1AG_MEP_CCI_DEFAULT
} dot1agMEPCciStates_t;

#define  DOT1AG_MEP_CCR_EVENTS_MAX (dot1agCCREventsEnd - dot1agCCREventsStart - 1)
typedef enum dot1agMEPCcrstates_s
{
  DOT1AG_MEP_CCR_RESET,
  DOT1AG_MEP_CCR_IDLE,
  DOT1AG_MEP_CCR_RECORD_EQUAL,
  DOT1AG_MEP_CCR_RECORD_LOW,
  DOT1AG_MEP_CCR_DEFAULT
} dot1agMEPCcrStates_t;

#define  DOT1AG_MHF_CCR_EVENTS_MAX (dot1agCCRMhfEventsEnd - dot1agCCRMhfEventsStart - 1)
typedef enum dot1agMHFCcrStates_s
{
  DOT1AG_MHF_CCR_IDLE,
  DOT1AG_MHF_CCR_RECORD,
  DOT1AG_MHF_CCR_DEFAULT
} dot1agMHFCcrStates_t;

#define  DOT1AG_RMEP_EVENTS_MAX (dot1agRmepEventsEnd - dot1agRmepEventsStart - 1)
typedef enum dot1agRmepStates_s
{
  DOT1AG_RMEP_IDLE,
  DOT1AG_RMEP_START,
  DOT1AG_RMEP_OK,
  DOT1AG_RMEP_FAILED,
  DOT1AG_RMEP_DEFAULT
} dot1agRmepStates_t;

#define  DOT1AG_RMEP_ERR_EVENTS_MAX (dot1agRmepErrEventsEnd - dot1agRmepErrEventsStart - 1)
typedef enum dot1agRmepErrStates_s
{
  DOT1AG_RMEP_ERRCCM_IDLE,
  DOT1AG_RMEP_ERRCCM_NO_DEFECT,
  DOT1AG_RMEP_ERRCCM_DEFECT,
  DOT1AG_RMEP_ERRCCM_DEFAULT
} dot1agRmepErrStates_t;

#define  DOT1AG_MEP_XCON_EVENTS_MAX (dot1agXConEventsEnd - dot1agXConEventsStart - 1)
typedef enum dot1agXConMepState_s
{
  DOT1AG_MEP_XCON_IDLE,
  DOT1AG_MEP_XCON_NO_DEFECT,
  DOT1AG_MEP_XCON_DEFECT,
  DOT1AG_MEP_XCON_DEFAULT
} dot1agXConMepStates_t;

#define  DOT1AG_LBR_EVENTS_MAX (dot1agLBREventsEnd - dot1agLBREventsStart - 1)
typedef enum dot1agLBRStates_s
{
  DOT1AG_LBR_IDLE,
  DOT1AG_LBR_RESPOND,
  DOT1AG_LBR_DEFAULT
} dot1agLBRStates_t;

#define  DOT1AG_LBI_EVENTS_MAX (dot1agLBIEventsEnd - dot1agLBIEventsStart - 1)
typedef enum dot1agLBIStates_s
{
  DOT1AG_LBI_IDLE,
  DOT1AG_LBI_STARTING,
  DOT1AG_LBI_XMITTING,
  DOT1AG_LBI_XMIT,
  DOT1AG_LBI_WAITING,
  DOT1AG_LBI_DEFAULT
} dot1agLBIStates_t;

#define  DOT1AG_LBIR_EVENTS_MAX (dot1agLBIREventsEnd - dot1agLBIREventsStart - 1)
typedef enum dot1agLBIRStates_t
{
  DOT1AG_LBIR_IDLE,
  DOT1AG_LBIR_RESPOND,
  DOT1AG_LBIR_DEFAULT
} dot1agLBIRStates_t;

#define  DOT1AG_FNG_EVENTS_MAX (dot1agFNGEventsEnd - dot1agFNGEventsStart - 1)
typedef enum dot1agMepFNGStates_t
{
  DOT1AG_FNG_RESET,
  DOT1AG_FNG_DEFECT,
  DOT1AG_FNG_REPORT_DEFECT,
  DOT1AG_FNG_DEFECT_REPORTED,
  DOT1AG_FNG_DEFECT_CLEARING,
  DOT1AG_FNG_DEFAULT
} dot1agMepFNGStates_t;

#define  DOT1AG_LTIR_EVENTS_MAX (dot1agLTIREventsEnd - dot1agLTIREventsStart - 1)
typedef enum dot1agLTIRStates_s
{
  DOT1AG_LTIR_IDLE,
  DOT1AG_LTIR_RESPOND,
  DOT1AG_LTIR_DEFAULT
} dot1agLTIRStates_t;

#define  DOT1AG_LTRR_EVENTS_MAX (dot1agLTRREventsEnd - dot1agLTRREventsStart - 1)
typedef enum dot1agLTRRStates_s
{
  DOT1AG_LTRR_IDLE,
  DOT1AG_LTRR_RESPOND,
  DOT1AG_LTRR_DEFAULT
} dot1agLTRRStates_t;

#define  DOT1AG_LTRT_EVENTS_MAX (dot1agLTRTEventsEnd - dot1agLTRTEventsStart - 1)
typedef enum dot1agLTRTStates_s
{
  DOT1AG_LTRT_IDLE,
  DOT1AG_LTRT_WAITING,
  DOT1AG_LTRT_XMITTING,
  DOT1AG_LTRT_DEFAULT
} dot1agLTRTStates_t;

#define  Y1731_PROLM_EVENTS_MAX (y1731PROLMEventsEnd - y1731PROLMEventsStart - 1)
typedef enum y1731MepProlmStates_s
{
  Y1731_PROLM_DISABLE,
  Y1731_PROLM_ENABLE,
  Y1731_PROLM_DEFAULT
} y1731MepProlmStates_t;

#define  Y1731_ONDEMLMM_EVENTS_MAX (y1731ONDEMLMMEventsEnd - y1731ONDEMLMMEventsStart - 1)
typedef enum y1731OndemLMMStates_s
{
  Y1731_ONDEMLMM_IDLE,
  Y1731_ONDEMLMM_XMIT,
  Y1731_ONDEMLMM_DEFAULT
} y1731OndemLMMStates_t;

#define  Y1731_LMR_EVENTS_MAX (y1731LMREventsEnd - y1731LMREventsStart - 1)
typedef enum y1731LMRStates_s
{
  Y1731_LMR_IDLE,
  Y1731_LMR_RESPOND,
  Y1731_LMR_DEFAULT
} y1731LMRStates_t;

#define  Y1731_DMM_EVENTS_MAX (y1731DMMEventsEnd - y1731DMMEventsStart - 1)
typedef enum y1731DMMStates_s
{
  Y1731_DMM_IDLE,
  Y1731_DMM_XMIT,
  Y1731_DMM_DEFAULT
} y1731DMMStates_t;

#define  Y1731_DMR_EVENTS_MAX (y1731DMREventsEnd - y1731DMREventsStart - 1)
typedef enum y1731DMRStates_s
{
  Y1731_DMR_IDLE,
  Y1731_DMR_RESPOND,
  Y1731_DMR_DEFAULT
} y1731DMRStates_t;

#define  Y1731_AIS_SEND_EVENTS_MAX (y1731AisSendEventsEnd - y1731AisSendEventsStart - 1)
typedef enum y1731AISSendStates_s
{
  Y1731_AIS_SEND_STOP,
  Y1731_AIS_SEND_START, /* Defect detected */
  Y1731_AIS_SEND_DEFAULT
} y1731AISSendStates_t;

#define  Y1731_AIS_RCV_EVENTS_MAX (y1731AisRcvEventsEnd - y1731AisRcvEventsStart - 1)
typedef enum y1731AISRcvStates_s
{
  Y1731_AIS_RCV_IDLE,    /* Defect cleared */
  Y1731_AIS_RCV_WAITING, /* Defect detected */
  Y1731_AIS_RCV_DEFAULT
} y1731AISRcvStates_t;

#define  Y1731_LCK_SEND_EVENTS_MAX (y1731LckSendEventsEnd - y1731LckSendEventsStart - 1)
typedef enum y1731LCKSendStates_s
{
  Y1731_LCK_SEND_STOP,
  Y1731_LCK_SEND_START, /* Defect detected */
  Y1731_LCK_SEND_DEFAULT
} y1731LCKSendStates_t;

#define  Y1731_LCK_RCV_EVENTS_MAX (y1731LckRcvEventsEnd - y1731LckRcvEventsStart - 1)
typedef enum y1731LCKRcvStates_s
{
  Y1731_LCK_RCV_IDLE,    /* Defect cleared */
  Y1731_LCK_RCV_WAITING, /* Defect detected */
  Y1731_LCK_RCV_DEFAULT
} y1731LCKRcvStates_t;

#define  Y1731_CSF_SEND_EVENTS_MAX (y1731CsfSendEventsEnd - y1731CsfSendEventsStart - 1)
typedef enum y1731CSFSendStates_s
{
  Y1731_CSF_SEND_STOP,
  Y1731_CSF_SEND_START, /* Defect detected */
  Y1731_CSF_SEND_DEFAULT
} y1731CSFSendStates_t;

#define  Y1731_CSF_RCV_EVENTS_MAX (y1731CsfRcvEventsEnd - y1731CsfRcvEventsStart - 1)
typedef enum y1731CSFRcvStates_s
{
  Y1731_CSF_RCV_IDLE,    /* Defect cleared */
  Y1731_CSF_RCV_WAITING, /* Defect detected */
  Y1731_CSF_RCV_DEFAULT
} y1731CSFRcvStates_t;

#define  Y1731_MEP_UNPTR_EVENTS_MAX (y1731dUNPtrEventsEnd - y1731dUNPtrEventsStart - 1)

#define  Y1731_MEP_UNL_EVENTS_MAX (y1731dUNLEventsEnd - y1731dUNLEventsStart - 1)

#define  Y1731_MEP_MMG_EVENTS_MAX (y1731dMMGEventsEnd - y1731dMMGEventsStart - 1)

#define  Y1731_MEP_UNP_EVENTS_MAX (y1731dUNPEventsEnd - y1731dUNPEventsStart - 1)

#define  Y1731_MEP_UNM_EVENTS_MAX (y1731dUNMEventsEnd - y1731dUNMEventsStart - 1)

typedef enum y1731MepDefectState_s
{
  Y1731_MEP_DEFECT_IDLE,
  Y1731_MEP_DEFECT_NO_DEFECT,
  Y1731_MEP_DEFECT_DEFECT,
  Y1731_MEP_DEFECT_DEFAULT
} y1731MepDefectStates_t;

/* MEP Global Variables - 20.9 */
typedef struct dot1agCfmGlobalMepVars_s
{
  OFDPA_BOOL                mepActive;
  OFDPA_BOOL                enableRmepDefect;
  OFDPA_BOOL                maDefectIndication;
#ifdef NOT_IMPLEMENTED
  OFDPA_BOOL                allRMEPsDead;
#endif
  Dot1agCfmLowestAlarmPri_t lowestAlarmPri;
  OFDPA_BOOL                presentRDI;
  uint32_t                  mepPrimaryVid;
} dot1agCfmSMGlobalMepVars_t;

/* MEP CCI State Machine Variables - 20.10 */
typedef struct dot1agCfmCciVars_s
{
  OFDPA_BOOL  cciEnabled;
  OFDPA_BOOL  cciSentCCMs;
  OFDPA_BOOL  macStatusChanged;
  void       *cciTimer;
} dot1agCfmSMCciVars_t;

/* MEP CCR State Machine Variables - 20.16 */
typedef struct dot1agCfmCcrMepVars_s
{
  uint8_t    recvdMacAddress[OFDPA_MAC_ADDR_LEN];
  OFDPA_BOOL recvdRDI;
  uint32_t   recvdInterval;
  /* Commented for compilation. This has to be cleaned once header files are cleaned.
  Dot1agCfmPSTlvVal_t       recvdPortState;
  Dot1agCfmIntfStatTlvVal_t  recvdInterfaceStatus; */
  uint32_t   recvdPortState;
  uint32_t   recvdInterfaceStatus;
  /* TODO: recvdSenderId field is missing. This needs re-evaluation. */
  uint32_t   ccmSequenceErrors;
} dot1agCfmSMCcrMepVars_t;

/* MHF CCR Variabled - 20.13 */
typedef struct dot1agCfmCcrMhfVars_s
{
  OFDPA_BOOL mhfRecvdCcm;
  uint8_t    mdhRecvdPdu[OFDPA_DOT1AG_CCM_FRAME_LEN];
} dot1agCfmSMCcrMhfVars_t;

/* RMEP State Machine Variables - 20.19 */
typedef struct dot1agCfmRmepVars_s
{
  OFDPA_BOOL  rMepCcmDefect;
  OFDPA_BOOL  rMepLastRDI;
  uint32_t    rMepLastPortState;
  uint32_t    rMepLastIntfStatus;
  OFDPA_BOOL  rCCMreceived;
  uint8_t     rMepMacAddress[OFDPA_MAC_ADDR_LEN];
  OFDPA_BOOL  rMepPortStatusDefect;
  OFDPA_BOOL  rMepInterfaceStatusDefect;
  void       *rmepTimer;
} dot1agCfmSMRmepsVars_t;

/* RMEP Error State Machine Variables - 20.21 */
typedef struct dot1agCfmRmepErrVars_s
{
  uint8_t     errorCcmLastFailure[OFDPA_DOT1AG_CCM_FRAME_LEN];
  OFDPA_BOOL  errorCcmDefect;
  void       *errorCCMTimer;
} dot1agCfmSMRmepErrVars_t;

/* MEP XCONNECT State Machine Variables - 20.23 */
typedef struct dot1agCfmXConMepVars_s
{
  uint8_t     xconCcmLastFailure[OFDPA_DOT1AG_CCM_FRAME_LEN];
  OFDPA_BOOL  xconCcmDefect;
  void       *xconCCMTimer;
} dot1agCfmSMXConMepVars_t;

typedef struct y1731DefectMepVars_s
{
  uint8_t     y1731DefCcmLastFailure[OFDPA_DOT1AG_CCM_FRAME_LEN];
  OFDPA_BOOL  y1731DefCcmDefect;
  void       *y1731DefCCMTimer;
} y1731DefectMepVars_t;

/* MP Loopback Responder State Machine Variables - 20.25 */
typedef struct dot1agCfmLbrMpVars_s
{
#ifdef NOT_IMPLEMENTED
  uint8_t lbmPdu[DOT1AG_LBM_DATA_LEN];
#endif
} dot1agCfmSMLbrMpVars_t;

/* The timer variable used by the MEP Loopback Initiator transmit state machine to time out the expected
reception of LBRs. Default : 5 seconds.*/
#define DOT1AG_CFM_LBI_TIMER_DEFAULT             5
#define DOT1AG_CFM_LBI_TIMER_DEFAULT_MS          (DOT1AG_CFM_LBI_TIMER_DEFAULT * 1000)
#define DOT1AG_CFM_LBI_TIMER_3MS_SUPPORT_DEFAULT 1500 /*5*300*/

#define DOT1AG_CFM_LBI_TIMER_BHH_DISCOVERY_DEFAULT_MS 250 /* 250 milli seconds: In BHH case, LBM Discovery PDUs should be sent at 4 packets/sec rate. */

/* MEP LBI State Machine Variables - 20.28 */
typedef struct dot1agCfmLbiMepVars_s
{
  uint32_t    lbmsToSend;
  uint32_t    nextLbmTransId;
  int32_t     lastRcvdLbrTransId;
  OFDPA_BOOL  lbiActive;
  OFDPA_BOOL  xmitReady;
  void       *lbiTimer;
  void       *lbiTxTimer;
#ifdef NOT_IMPLEMENTED
  uint8_t     lbrPdu[DOT1AG_LBM_DATA_LEN];
#endif
  uint8_t     nextLbmDiscoveryTTL; /* Used only in BHH discovery case */
} dot1agCfmSMLbiMepVars_t;

/* MEP FNG State Machine Variables - 20.33 */
typedef struct dot1agCfmFngMepVars_s
{
  uint32_t                     fngPriority;
  uint32_t                     fngDefect;
  uint32_t                     fngAlarmTime;
  uint32_t                     fngResetTime;
  OFDPA_BOOL                   someRMEPCCMDefect;
  OFDPA_BOOL                   someMacStatusDefect;
  OFDPA_BOOL                   someRDIDefect;
  uint32_t                     highestDefectPri;
  Dot1agCfmHighestDefectPri_t  highestDefect;
  void                        *fngTimer;
} dot1agCfmSMFngMepVars_t;

/* MEP Linktrace ReplyList - 20.36.2 */
typedef struct dot1agCfmLtmReplyList_s
{
  /* Commented for compilation. This has to be cleaned once header files are cleaned. */
  uint32_t /*dot1agLtrFlags_t */        ltrFlags;
  uint32_t ltrReplyTTL;
  uint8_t  ltrLastEgressId[DOT1AG_EGRESS_ID_LEN];
  uint8_t  ltrNextEgressId[DOT1AG_EGRESS_ID_LEN];
  /* Commented for compilation. This has to be cleaned once header files are cleaned.
  Dot1agLtrRelayAction_t   ltrReplyAction;
  Dot1agLtrIngressAction_t ltrIngressAction;
  Dot1agLtrEgressAction_t  ltrEgressAction;
   */
  uint32_t ltrReplyAction;
  uint32_t ltrIngressAction;
#ifdef NOT_IMPLEMENTED
  uint8_t  ltrIngressAddress[OFDPA_MAC_ADDR_LEN];
#endif
  uint32_t ltrIngressPortIdSubtype;
#ifdef NOT_IMPLEMENTED
  uint8_t  ltrIngressPortId[];          /* As specified by IEEE 802.1AB-2005 */
#endif
  uint32_t ltrEgressAction;
#ifdef NOT_IMPLEMENTED
  uint8_t  ltrEgressAddress[OFDPA_MAC_ADDR_LEN];
#endif
  uint32_t ltrEgressPortIdSubtype;
#ifdef NOT_IMPLEMENTED
  uint8_t  ltrEgressPortId[];           /* As specified by IEEE 802.1AB-2005 */
#endif
} dot1agCfmLtmReplyList_t;

/* MEP Linktrace Initiator State Machine Variables - 20.36 */
typedef struct dot1agCfmLtiMepVars_s
{
#if 0
  uint32_t                nextLtmTransId;
#endif
  dot1agCfmLtmReplyList_t ltmReplyList;
} dot1agCfmSMLtiMepVars_t;

/* MEP Linktrace Initiator Receive State Machine Variables - 20.38 */
typedef struct dot1agCfmSMLtirMepVars_s
{
#ifdef NOT_IMPLEMENTED
  uint8_t           ltrPdu[DOT1AG_LTR_PDU_LEN];
#endif
} dot1agCfmSMLtirMepVars_t;

/* The timer variable used by the Link Trace Responder transmit state machine to
   respond to the reception of LTMs. Default : 1 second.*/
#define DOT1AG_CFM_LTF_TIMER_DEFAULT             1
#define DOT1AG_CFM_LTF_TIMER_DEFAULT_MS          (DOT1AG_CFM_LTF_TIMER_DEFAULT * 1000)
#define DOT1AG_CFM_LTF_TIMER_3MS_SUPPORT_DEFAULT 300

/* This timer will prevent LTIT to issue any more LTMs until this expires */
#define DOT1AG_CFM_LTM_DELAY_TIMER_DEFAULT             5
#define DOT1AG_CFM_LTM_DELAY_TIMER_DEFAULT_MS          (DOT1AG_CFM_LTM_DELAY_TIMER_DEFAULT * 1000)
#define DOT1AG_CFM_LTM_DELAY_TIMER_3MS_SUPPORT_DEFAULT 1500

/* Linktrace Responder State Machine Variables - 20.41 */
typedef struct dot1agCfmLtrVars_s
{
  uint32_t  nPendingLTRs;
  uint32_t  nextLtmTransId;
  void     *ltitTimer;                  /* 5 sec timer to block
                                           continuous LTMs */
#ifdef NOT_IMPLEMENTED
  uint8_t   ltmPdu[DOT1AG_LTM_PDU_LEN];
#endif
  void     *ltfTimer;
} dot1agCfmLtrVars_t;

/* TODO : To be moved to appropriate header file.*/
typedef struct dot1agCfmSMEventMsg_s
{
  dot1agSMEvents_t            event;
  uint8_t                     mdIndex;
  uint32_t                    maIndex;
  uint32_t                    mepIndex;
  uint32_t                    rmepIndex;
  uint8_t                     entity;
  uint32_t                    intIfNum;
  uint32_t                    recvdCcmInterval;
  uint8_t                     priority;  /* for LM and DM */
  struct dot1agCfmStackObj_s *pMPObj;    /* MP holder  */
  struct dot1agControlPkt_s  *dot1agPkt; /* PDU holder */
  struct dot1agCfmRMepObj_s  *pRMEPObj;  /* RMEP holder */
  struct dot1agCfmMepTable_s *pMepData;  /* MEP holder */
} dot1agCfmSMEventMsg_t;

/* The timer variable used by the ProLm state machine to time out. Default : 100 milli seconds.*/
#define Y1731_CFM_PROLM_TIMER_DEFAULT       300 /*1*300*/
#define Y1731_CFM_PROLM_TIMER_DEFAULT_MS    100
typedef struct y1731CfmSMProlmMepVars_s
{
  DOT1AG_ROW_STATUS_t  active;
  OFDPA_BOOL           prolmEnabled;
  uint32_t             proLmControlPriority;
  uint8_t              prolmToolType; /* 0 - CCM, 1- LM, 2 - SLM*/
  void                *prolmTimer;
} y1731CfmSMProlmMepVars_t;

typedef struct y1731CfmSMAisSendVars_s
{
  OFDPA_BOOL        aisEnable;
  uint8_t           aisClientLevel;
  uint8_t           aisPriority;
  y1731CfmPeriod_t  aisInterval;        /* Tx/Rx period should be the same. */
  void             *aisSendTimer;
} y1731CfmSMAisSendVars_t;

typedef struct y1731CfmSMAisRcvVars_s
{
  void *aisRcvTimer;
} y1731CfmSMAisRcvVars_t;

typedef struct y1731CfmSMLckSendVars_s
{
  OFDPA_BOOL        lckEnable;
  uint8_t           lckClientLevel;
  uint8_t           lckPriority;
  y1731CfmPeriod_t  lckInterval;        /* Tx/Rx period should be the same. */
  void             *lckSendTimer;
} y1731CfmSMLckSendVars_t;

typedef struct y1731CfmSMLckRcvVars_s
{
  void *lckRcvTimer;
} y1731CfmSMLckRcvVars_t;

typedef struct y1731CfmSMCsfSendVars_s
{
  OFDPA_BOOL        csfEnable;
  uint8_t           csfClientLevel;
  uint8_t           csfPriority;
  y1731CfmPeriod_t  csfInterval;        /* Tx/Rx period should be the same. */
  void             *csfSendTimer;
} y1731CfmSMCsfSendVars_t;

typedef struct y1731CfmSMCsfRcvVars_s
{
  y1731CfmPeriod_t  csfInterval;
  uint32_t          csfType;
  void             *csfRcvTimer;
} y1731CfmSMCsfRcvVars_t;

OFDPA_ERROR_t dot1agCfmStateMachineClassifier(dot1agCfmSMEventMsg_t *eventMsg, OFDPA_BOOL hwEnabled);
void dot1agCfmltitTimerCallback(void *handle);
void dot1agCfmRmepTimerCallback(void *handle);
void dot1agCfmLbiTimerCallback(void *handle);

#endif
