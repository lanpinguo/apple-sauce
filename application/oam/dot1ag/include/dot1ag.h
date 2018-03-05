/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
**********************************************************************
*
* @filename       dot1ag.h
* @purpose        Dot1ag configuration functions prototypes
* @component      Dot1ag
* @comments       none
* @create         02/15/2009
* @end
*
**********************************************************************/
#ifndef INCLUDE_DOT1AG_H
#define INCLUDE_DOT1AG_H

#include "avl_api.h"
#include "dot1ag_exports.h"
#include "dot1ag_cnfgr.h"
#include "dot1ag_proto.h"
#include "dot1ag_y1731pm.h"
#include "dot1ag_sm.h"

/* forward declarations -- these are fully defined in dot1ag_db.h */

struct dot1agCfmRMepObj_s;
typedef struct dot1agCfmRMepObj_s dot1agCfmRMepObj_t;

struct dot1agCfmMepObj_s;
typedef struct dot1agCfmMepObj_s dot1agCfmMepObj_t;

struct dot1agCfmMAObj_s;
typedef struct dot1agCfmMAObj_s dot1agCfmMAObj_t;

struct dot1agCfmMAObjPrimVID_s;
typedef struct dot1agCfmMAObjPrimVID_s dot1agCfmMAObjPrimVID_t;


struct dot1agCfmStackObj_s;
typedef struct dot1agCfmStackObj_s dot1agCfmStackObj_t;

/* Synthetic Loss Measurement Counters Table Database */
typedef struct  dot1agCfmSlmObjKey_s
{
  uint32_t dot1agCfmLmepIdentifier;
  uint32_t dot1agCfmRmepIdentifier;
  uint32_t dot1agCfmTestId;
} dot1agCfmSlmObjKey_t;

typedef struct dot1agCfmSlmObjData_s
{
  uint32_t             txFCl;
  uint32_t             rxFCl;
  uint32_t             txFCfStart;
  uint32_t             txFCbStart;
  uint32_t             rxFClStart;
  uint32_t             nNLF;
  uint32_t             nFLF;
  uint32_t             nNTF;
  uint32_t             nFTF;
  void                *next;
} dot1agCfmSlmObjData_t;

typedef struct dot1agCfmSlmObj_s
{
  dot1agCfmSlmObjKey_t key;
  dot1agCfmSlmObjData_t data;
  void                *next;
} dot1agCfmSlmObj_t;

/* Client Level Maintenance Point Database */
typedef struct  dot1agCfmClientMPDBObjKey_s
{
  uint32_t serverLmepId;
  uint32_t clientLmepId;
} dot1agCfmClientMPDBObjKey_t;

typedef struct dot1agCfmClientMPDBObj_s
{
  dot1agCfmClientMPDBObjKey_t key;
  uint32_t                    serverOfdpaLocalMpId;
  dot1agCfmStackObj_t         *pClientMPObj;
  OFDPA_BOOL                  txDirSameAsServer;
  void                        *next;
} dot1agCfmClientMPDBObj_t;

/* CFM Data Structures */
/***********************************************************************
                     IEEE P802.1ag/D8.1 Fig. 12-1
                    -----------------------------

                              -----------------------
                              |     CFM Master      |
                              -----------------------
                                   |  | . . . |
                               -------------
                              |       ----------
                              -------|       ----------
                                      ------|   Bridge | (Bridge-list)
                                             ----------
                                              ||
                                              \/
                              -----------------------
                              |     Bridge          |
                              -----------------------
                                        |
                                   -------------
                                  |    CFM      |
                                   -------------

                                        ||
                                        \/

                              -----------------------       -------------
                              |     CFM              |------| Default MD|
                              -----------------------       -------------
                                   |  | . . . |
                               -------------
                              |       ----------
                              -------|       ---------
                                      ------|  MD     | (Maintenance Domian list)
                                             ---------

                                        ||
                                        \/

                              -----------------------
                              |       Default MD    |
                              -----------------------
                                   |  | . . . |
                               -------------
                              |       ----------
                              -------|       ---------
                                      ------|   MHF   | (MIP Half Function list)
                                             ---------

                                        ||
                                        \/

                              -----------------------
                              |         MD          | (Maintenance Domain)
                              -----------------------
                                   |  | . . . |
                               -------------
                              |       ----------
                              -------|       ---------
                                      ------|   MA    | (Maintenance Association list)
                                             ---------

                                        ||
                                        \/

                              ----------------------
                              |        MA          | (Maintenance Association)
                              ----------------------
                                   |  | . . . |
                               -------------
                              |       ----------
                              -------|       ---------
                                      ------|   MEP   | (Maintenance End Point list)
                                             ---------
******************************************************************************/
typedef enum
{
  /***************************************************************/
  /* Events shared with all                                      */
  /***************************************************************/
  dot1agControlBegin = 100,

  /***************************************************************/
  /* Events from Management commands                             */
  /***************************************************************/
  dot1agCfmCreate,
  dot1agCfmDelete,
  dot1agCfmMDCreate,
  dot1agCfmMDDelete,
  dot1agCfmMACreate,
  dot1agCfmMADelete,
  dot1agCfmMEPCreate,
  dot1agCfmMEPDelete,
  dot1agCfmMIPCreate,
  dot1agCfmMIPDelete,
  dot1agCfmRMEPEntryAdd,
  dot1agCfmRMEPEntryDelete,
  dot1agCfmRMEPDelete,
  dot1agCfmMEPActiveMode,
  dot1agCfmMEPCciEnabled,
  dot1agCfmLBMSend,
  dot1agCfmLBMStop,
  dot1agCfmLTMSend,
  dot1agCfmLTRCacheClear,
  dot1agCfmArchiveTimeout,

  dot1agCfmAdminModeEnable,
  dot1agCfmAdminModeDisable,
  dot1agCfmPortInitializeSet,
  dot1agCfmLogicalPortInitializeSet,
  dot1agMepAdminControlledDirectionsSet,
  dot1agMepControlModeSet,
  dot1agMepVlanModeEnable,
  dot1agMepVlanModeDisable,
  dot1agCfmLTFwhileTimerSet,
  dot1agCfmCCIwhileTimerSet,
  dot1agCfmErrorCCMWhileTimerSet,
  dot1agCfmXconCCMWhileTimerSet,
  dot1agCfmLTITimeoutSet,
  dot1agCfmFngTimeoutSet,
  dot1agMEPStatsClear,
  dot1agMDStatsClear,
  dot1agMAStatsClear,
  dot1agMgmtApplyConfigData,
  dot1agMgmtApplyPortConfigData,
  dot1agCfmCCMIntervalSet,
  dot1agCfmMEPProlmCreate,
  dot1agCfmMEPProlmDelete,
  dot1agCfmMEPProlmEnabled,
  dot1agMepOndemLMSet,
  dot1agMepOndemLMMStart,
  dot1agMepOndemLMMStop,
  dot1agMepDMMStart,
  dot1agMepDMMStop,
  dot1agMgmtEvents, /*keep this last in sub group*/
  /***************************************************************/
  /* Events from HW received                                     */
  /***************************************************************/
  dot1agHwEventReceived,

  /***************************************************************/
  /* Events from interface state changes                         */
  /***************************************************************/
  dot1agIntfChange,

  dot1agIntfChangeEvents,

  dot1agCfmTimerTick,

  /***************************************************************/
  /* DOT1AG queue synchronize                                    */
  /***************************************************************/
  dot1agQueueSynchronize,

  /***************************************************************/
  /* DOT1AG AIS LCK CSF                                          */
  /***************************************************************/
  dot1agCfmDefectAis,
  dot1agCfmDefectLck,
  dot1agCfmDefectCsf,

  /***************************************************************/
  /* Clear Synthetic Loss Measurement LM Counters Cache           */
  /***************************************************************/
  dot1agCfmSLMCacheClear,
} dot1agControlEvents_t;

#define DOT1AG_PKT_SEND_GROUP      10
#define DOT1AG_PKT_MAX_PKTS_IN_GRP 34
#define DOT1AG_PKT_GROUP_INVALID   -1
#define DOT1AG_CCM_PKT_LEN         512

typedef struct dot1agCfmMipTable_s
{
  /* Non standard config param */
  /* MIP configured:  1 = enabled, 0 = disabled */
  OFDPA_INTF_MASK_t   mipMode;

} dot1agCfmMipTable_t;

typedef struct dot1agCfmDefaultMdTable_s
{
  /* 802.1ag clause 12.14.3 */
  OFDPA_BOOL              dot1agCfmDefaultMdStatus;
  uint32_t                dot1agCfmDefaultMdLevel;
  Dot1agCfmMhfCreation_t  dot1agCfmDefaultMdMhfCreation;
  Dot1agCfmIdPermission_t dot1agCfmDefaultMdIdPermission;

} dot1agCfmDefaultMdTable_t;

typedef struct dot1agCfmMepTable_s
{
  /* 802.1ag clauses 12.14.7 and 19.2 */
  struct dot1agCfmMaNetTable_s *maData;
  uint32_t                      dot1agCfmMepIdentifier;
  uint32_t                      dot1agCfmMepIfIndex;
  Dot1agCfmMpDirection_t        dot1agCfmMepDirection;
  uint32_t                      dot1agCfmMepPrimaryVid;
  uint8_t                       dot1agCfmMepMacAddress[OFDPA_MAC_ADDR_LEN];

  OFDPA_BOOL                    dot1agCfmMepActive;
  OFDPA_BOOL                    dot1agCfmMepCciEnabled;
  uint32_t                      dot1agCfmMepFngAlarmTime;
  uint32_t                      dot1agCfmMepFngResetTime;
  uint8_t                       dot1agCfmMepCcmLtmPriority;

  uint8_t                       dot1agCfmMepErrorCcmLastFailure[OFDPA_DOT1AG_CCM_FRAME_LEN];
  uint8_t                       dot1agCfmMepXconCcmLastFailure[OFDPA_DOT1AG_CCM_FRAME_LEN];
  uint32_t                      dot1agCfmMepCcmSequenceErrors;
  uint32_t                      dot1agCfmMepLbrIn;
  uint32_t                      dot1agCfmMepLbrInOutOfOrder;
  uint32_t                      dot1agCfmMepLbrBadMsdu;
  uint32_t                      dot1agCfmMepUnexpLtrIn;
  uint32_t                      dot1agCfmMepLbrOut;
  /* LBM - Params*/
  OFDPA_BOOL                    dot1agCfmMepTransmitLbmStatus;
  uint8_t                       dot1agCfmMepTransmitLbmDestMacAddress[OFDPA_MAC_ADDR_LEN];
  uint32_t                      dot1agCfmMepTransmitLbmDestMepId;
  OFDPA_BOOL                    dot1agCfmMepTransmitLbmDestIsMepId;
  uint32_t                      dot1agCfmMepTransmitLbmVlanPriority;
  OFDPA_BOOL                    dot1agCfmMepTransmitLbmVlanDropEnable;
  OFDPA_BOOL                    dot1agCfmMepTransmitLbmTestTlvPresent;
  OFDPA_OAM_LB_TEST_TLV_TYPE_t  dot1agCfmMepTransmitLbmTestTlvType;
  uint32_t                      dot1agCfmMepTransmitLbmPacketLength;
  OFDPA_OAM_LB_DISCOVERY_TYPE_t dot1agCfmMepTransmitLbmDiscovery;
  uint32_t                      dot1agCfmMepTransmitLbmTtl;
  OFDPA_BOOL                    dot1agCfmMepTransmitLbmResultOk;
  uint32_t                      dot1agCfmMepTransmitLbmInterval;
#ifdef NOT_IMPLEMENTED
  uint8_t                       dot1agCfmMepTransmitLbmDataTlv[DOT1AG_LBM_DATA_LEN];
#endif
  /* LTR - Params */
  uint8_t                       dot1agCfmMepTransmitLtmFlags;
  uint8_t                       dot1agCfmMepTransmitLtmTargetMacAddress[OFDPA_MAC_ADDR_LEN];
  uint32_t                      dot1agCfmMepTransmitLtmTargetMepId;
  OFDPA_BOOL                    dot1agCfmMepTransmitLtmTargetIsMepId;
  uint32_t                      dot1agCfmMepTransmitLtmTtl;
  OFDPA_BOOL                    dot1agCfmMepTransmitLtmResult;
  uint32_t                      dot1agCfmMepTransmitLtmSeqNumber;
#ifdef NOT_IMPLEMENTED
  uint8_t                       dot1agCfmMepTransmitLtmEgressIdentifier[DOT1AG_EGRESS_ID_LEN];
#endif
  /* The global variables to be maintained per MEP.*/
  dot1agCfmSMGlobalMepVars_t    dot1agCfmGblMepVar;
  /* The state and variables information required to run CCI SM.*/
  dot1agMEPCciStates_t          dot1agCfmMepCCIState;
  dot1agCfmSMCciVars_t          dot1agCfmMepCCIVar;
  /* The state and variables information required to run CCR SM.*/
  dot1agMEPCcrStates_t          dot1agCfmMepCCRState;
  dot1agCfmSMCcrMepVars_t       dot1agCfmMepCCRVar;
  /* The state and variable information required to run RMEP Error SM.*/
  dot1agRmepErrStates_t         dot1agCfmRmepErrState;
  dot1agCfmSMRmepErrVars_t      dot1agCfmRmepErrVar;
  /* The state and variable information required to run RMEP Error SM.*/
  dot1agXConMepStates_t         dot1agCfmMepXConState;
  dot1agCfmSMXConMepVars_t      dot1agCfmMepXConVar;
  y1731MepDefectStates_t        y1731DefectState[Y1731_DEFECT_TYPE_MAX];
  y1731DefectMepVars_t          y1731DefectVar[Y1731_DEFECT_TYPE_MAX];
  /* The state and variable information required to run LBI SM.*/
  dot1agLBIStates_t             dot1agCfmMepLbiState;
  dot1agCfmSMLbiMepVars_t       dot1agCfmMepLbiVar;
  /* The state information required to run LBIR SM. The variable in LBI SM is reused here.*/
  dot1agLBIRStates_t            dot1agCfmMepLBIRState;
  /* The information to be maintained per MEP for Link Trace Initiation.*/
  dot1agCfmSMLtiMepVars_t       dot1agCfmMepLtiVar;
  /* The state and information to be maintained per MEP for Link Trace Initiation.*/
  dot1agLTIRStates_t            dot1agCfmMepLtirState;
  dot1agCfmSMLtirMepVars_t      dot1agCfmMepLtirVar;
  /* The state and information to be maintained per MEP for FNG SM.*/
  dot1agMepFNGStates_t          dot1agCfmMepSMFngState;
  dot1agCfmSMFngMepVars_t       dot1agCfmMepFngVar;
  /* The state and information to be maintained per MEP for PROLM.*/
  y1731MepProlmStates_t         y1731CfmMepProlmState;
  y1731CfmSMProlmMepVars_t      y1731CfmMepProlmVar;
  y1731CfmMepProlmTable_t      *y1731CfmMepProlmEntry;
  /* The state and information to be maintained per MEP for Ondem LM.*/
  y1731OndemLMMStates_t         y1731CfmOndemLMMState[8];
  y1731CfmMepOndemlmTable_t    *y1731CfmMepOndemlmEntry[8];
  /* The state to be maintained per MEP for processing LMM.*/
  y1731LMRStates_t              y1731CfmLMRState[8];
  /* The state and information to be maintained per MEP for DM.*/
  y1731DMMStates_t              y1731CfmDMMState[8];
  y1731DMRStates_t              y1731CfmDMRState[8];
  y1731CfmMepDmTable_t         *y1731CfmMepDmEntry[8];
  /* The state and variables information required to run AIS.*/
  y1731AISSendStates_t          y1731CfmMepAisSendState;
  y1731CfmSMAisSendVars_t       y1731CfmMepAisSendVar;
  /* The state and variables information required to run AIS.*/
  y1731AISRcvStates_t           y1731CfmMepAisRcvState;
  y1731CfmSMAisRcvVars_t        y1731CfmMepAisRcvVar;
  /* The state and variables information required to run LCK.*/
  y1731LCKSendStates_t          y1731CfmMepLckSendState;
  y1731CfmSMLckSendVars_t       y1731CfmMepLckSendVar;
  /* The state and variables information required to run LCK.*/
  y1731LCKRcvStates_t           y1731CfmMepLckRcvState;
  y1731CfmSMLckRcvVars_t        y1731CfmMepLckRcvVar;
  /* The state and variables information required to run CSF.*/
  y1731CSFSendStates_t          y1731CfmMepCsfSendState;
  y1731CfmSMCsfSendVars_t       y1731CfmMepCsfSendVar;
  /* The state and variables information required to run CSF.*/
  y1731CSFRcvStates_t           y1731CfmMepCsfRcvState;
  y1731CfmSMCsfRcvVars_t        y1731CfmMepCsfRcvVar;
  DOT1AG_ROW_STATUS_t           active;
  OFDPA_BOOL                    mepPktChanged;

} dot1agCfmMepTable_t;

typedef struct dot1agCfmMaCompTable_s
{
  /* 802.1ag clause 18.2 */
  uint32_t                dot1agCfmMaCompPrimaryVlanId;
  Dot1agCfmMhfCreation_t  dot1agCfmMaCompMhfCreation;
  Dot1agCfmIdPermission_t dot1agCfmMaCompIdPermission;
  uint32_t                dot1agCfmMaCompNumberOfVids;
} dot1agCfmMaCompTable_t;

typedef struct dot1agCfmMaNetTable_s
{
  /* 802.1ag clause 18.2 */
  uint32_t                       dot1agCfmMaIndex;
  uint8_t                        dot1agCfmMaid[OFDPA_MEG_ID_LENGTH];
  Dot1agCfmCcmInterval_t         dot1agCfmMaNetCcmInterval;
  dot1agCfmMaCompTable_t         dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENTS_COUNT];
  struct dot1agCfmMdTable_s     *mdData;

  OFDPA_BOOL                     pktChanged;

  DOT1AG_ROW_STATUS_t            active;
} dot1agCfmMaNetTable_t;

typedef struct dot1agCfmMdTable_s
{
  /* 802.1ag clause 12.14.7.7 */
  uint32_t                       dot1agCfmMdIndex;
  Dot1agCfmMaintDomainNameType_t dot1agCfmMdFormat;
  uint8_t                        dot1agCfmMdName[OFDPA_DOT1AG_MD_NAME_LEN];
  int                            dot1agCfmMdMdLevel;
  uint32_t                       dot1agCfmMdMaNextIndex;
  Dot1agCfmMhfCreation_t         dot1agCfmMdMhfCreation;
  Dot1agCfmIdPermission_t        dot1agCfmMdMhfIdPermission;

  DOT1AG_ROW_STATUS_t      active;
} dot1agCfmMdTable_t;

typedef struct dot1agCFMCfgData_s
{
  /* CFM global Admin Mode */
  OFDPA_BOOL                dot1agCfmAdminMode;
  /* CFM Archive hold time */
  uint32_t                  dot1agCfmArchiveHoldTime;
  /* 802.1ag clause 12.14.3.1.3:c, 12.14.3.2.2:b */
  int                       dot1agCfmDefaultMdDefLevel;
  /* 802.1ag clause 12.14.3.1.3:d */
  Dot1agCfmMhfCreation_t    dot1agCfmDefaultMdDefMhfCreation;
  /* 802.1ag clause 12.14.3.1.3:e */
  Dot1agCfmIdPermission_t   dot1agCfmDefaultMdDefIdPermission;
  /* The Default MD Level Table - 802.1ag clause 12.14.3 */
  dot1agCfmDefaultMdTable_t dot1agCfmDefaultMdTable[OFDPA_MAX_VLANS];
  /* maintenance domains */
  dot1agCfmMdTable_t        dot1agCfmMdTable[OFDPA_DOT1AG_MAINTENANCE_DOMAINS_MAX];
  /* maintenance Associations */
  dot1agCfmMaNetTable_t     dot1agCfmMaNetTable[OFDPA_DOT1AG_MAX_MA_COUNT];
  /* maintenance end points */
  dot1agCfmMepTable_t       dot1agCfmMepTable[OFDPA_DOT1AG_MAX_MEP_COUNT];
  /* maintenance intermediate points */
  dot1agCfmMipTable_t       dot1agCfmMipTable[OFDPA_DOT1AG_MD_LEVEL_COUNT];
  /* Debug mode */
  OFDPA_BOOL                dot1agLogTraceMode;
} dot1agCFMCfg_t;

#define DOT1AG_MD_CFG(idx)         dot1agCFM.dot1agCFMCfg->dot1agCfmMdTable[(idx)]
#define DOT1AG_MA_NET_TABLE(idx)   dot1agCFM.dot1agCFMCfg->dot1agCfmMaNetTable[(idx)]
#define DOT1AG_MA_COMP_TABLE(idx)  dot1agCFM.dot1agCFMCfg->dot1agCfmMaNetTable[(idx)].dot1agCfmMaCompTable[DOT1AG_PBB_COMPONENT_DEFAULT]
#define DOT1AG_MEP_TABLE(idx)      dot1agCFM.dot1agCFMCfg->dot1agCfmMepTable[(idx)]
#define DOT1AG_MIP_TABLE(lvl)      dot1agCFM.dot1agCFMCfg->dot1agCfmMipTable[(lvl)]
#define DOT1AG_CFG                 dot1agCFM.dot1agCFMCfg

/* Debug configuration structures */
typedef struct dot1agDebugCfgData_s
{
  OFDPA_BOOL dot1agDebugPacketTraceTxFlag;
  OFDPA_BOOL dot1agDebugPacketTraceRxFlag;
} dot1agDebugCfgData_t;

typedef void (*dot1ag_tmr_fn)(void *pParam);

typedef struct dot1ag_timer_node_s
{
  int            timeLeft;
  dot1ag_tmr_fn  func_p;
  void          *para;
} dot1ag_timer_node_t;

typedef struct dot1ag_timer_list_s
{
  dot1ag_timer_node_t         node;
  struct dot1ag_timer_list_s *next;
} dot1ag_timer_list_t;

typedef OFDPA_ERROR_t (*g8131PduRcvCallBack)(uint32_t,  uint32_t, uint32_t, uint8_t *, uint32_t, uint32_t);

typedef struct dot1agCFM_s
{
  /* Semaphores */
  void                    *dot1agMsgQSema;
  void                    *dot1agTimerMsgSem;
  ofdpaRWlock_t            dot1agCfgRWLock;
  /* Internal Event Message Queue */
  void                    *dot1agEventQueue;
  void                    *dot1agPduQueue;
  void                    *dot1agLtrQueue;
  /* Packet transmission group */
  unsigned int             dot1agPktTxGroup;
  /* dot1agTask */
  void                    *dot1agTaskId;
  /* CFM Configuration */
  dot1agCFMCfg_t          *dot1agCFMCfg;
  /* Component Configurator state */
  OFDPA_BOOL               dot1agState;
  /* RMepTree */
  avlTree_t                dot1agRMepTree;
  avlTreeTables_t         *dot1agRMepTreeHeap;
  dot1agCfmRMepObj_t      *dot1agRMepDataHeap;
  /* MEP Tree */
  uint32_t                 dot1agMEPTableIndex;
  avlTree_t                dot1agMepTree;
  avlTreeTables_t         *dot1agMepTreeHeap;
  dot1agCfmMepObj_t       *dot1agMepDataHeap;
  uint32_t                 dot1agMepTableIndex;
  /* MA Tree - MIB view */
  uint32_t                 dot1agMATableIndex;
  avlTree_t                dot1agMATree;
  avlTreeTables_t         *dot1agMATreeHeap;
  dot1agCfmMAObj_t        *dot1agMADataHeap;
  /* MA Tree - PrimVID view */
  avlTree_t                dot1agMAPrimVIDTree;
  avlTreeTables_t         *dot1agMAPrimVIDTreeHeap;
  dot1agCfmMAObjPrimVID_t *dot1agMAPrimVIDDataHeap;
  /* LTR Tree */
  avlTree_t                dot1agLtrTree;
  avlTreeTables_t         *dot1agLtrTreeHeap;
  dot1agCfmLtrObj_t       *dot1agLtrDataHeap;

  /* SLM Tree */
  avlTree_t                dot1agSlmTree;
  avlTreeTables_t         *dot1agSlmTreeHeap;
  dot1agCfmSlmObj_t       *dot1agSlmDataHeap;

  /* Client MP DB Tree */
  avlTree_t                 dot1agClientMPDBTree;
  avlTreeTables_t          *dot1agClientMPDBTreeHeap;
  dot1agCfmClientMPDBObj_t *dot1agClientMPDBDataHeap;

  /* CFM Stack Table */
  avlTree_t                dot1agCfmStackTree;
  avlTreeTables_t         *dot1agCfmStackTreeHeap;
  dot1agCfmStackObj_t     *dot1agCfmStackDataHeap;
  /* Buffer pool to hold PDUs */
  /* Timer Data */
  void                    *cfmPDUTxBufferPoolId;

  /* Buffer pool to hold LTRs before FTWhile expires */
  void                    *ltrBufferPoolId;
  timer_t                  timerCB;     /* dot1ag App Timer Control Block */
  dot1ag_timer_list_t      timerList;
  /* The link trace responder instance is per-bridge. The state and information
     need to run LinkTrace Message Responder SMs.*/
  dot1agLTRRStates_t       dot1agCfmLtrrState;
  dot1agLTRTStates_t       dot1agCfmLtrtState;
  dot1agCfmLtrVars_t       dot1agCfmLtmrVar;
  g8131PduRcvCallBack      apsPduRcvCallBack;
  ofdpaRWlock_t            dot1agCfgClientMPDBRWLock;

} dot1agCFM_t;

extern dot1agCFM_t dot1agCFM;

OFDPA_ERROR_t dot1agMpTypeGet(dot1agCfmStackObj_t *pMPObj,
                              OFDPA_DOT1AG_MP_type_t *mpType);

/*********************************************************************
* @purpose  get one entry for Ondem LM
*
* @param    primVID    @b{(input)} VLAN ID associated with the MA. Can be
*                                  NULL when used with SNMP.
*
*
* @returns  Ondemlm Entry
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
y1731CfmMepOndemlmTable_t *dot1agMepOndemlmEntryGet(uint32_t intIfNum, uint16_t primVID,
                                                    uint8_t mdLevel,
                                                    Dot1agCfmMpDirection_t direction,
                                                    uint32_t controlPriority);

/*********************************************************************
* @purpose  get one entry for DM
*
* @param    primVID    @b{(input)} VLAN ID associated with the MA. Can be
*                                  NULL when used with SNMP.
*
*
* @returns  DM Entry
*
* @notes    Non standard config param
*
* @end
**********************************************************************/
y1731CfmMepDmTable_t *dot1agMepDmEntryGet(uint32_t intIfNum, uint16_t primVID,
                                          uint8_t mdLevel,
                                          Dot1agCfmMpDirection_t direction,
                                          uint32_t controlPriority);

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
                                        dot1agCfmLtrObj_t *linkTraceReply);

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
OFDPA_ERROR_t dot1agSlmEntryInsert(dot1agCfmSlmObj_t *pSlmEntry);

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
OFDPA_ERROR_t dot1agSlmEntryDelete(dot1agCfmSlmObj_t *pSlmEntry);
#endif
