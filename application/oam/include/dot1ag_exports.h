/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2000-2016
*
**********************************************************************
*
* @filename dot1ag_exports.h
*
* @purpose  Defines constants and feature definitions that are shared
*           by Management and the application
*
* @component
*
* @comments
*
*
* @Notes
*
* @created
*
* @end
*
 **********************************************************************/

#ifndef __DOT1AG_EXPORTS_H_
#define __DOT1AG_EXPORTS_H_

#include <stdint.h>
#include "ofdpa_datatypes.h"

#define OFDPA_DOT1AG_MAX_MEP_COUNT                        512
#define OFDPA_DOT1AG_MAID_MAX_LEN                         48
#define OFDPA_DOT1AG_MAINTENANCE_DOMAINS_MAX              8
#define OFDPA_DOT1AG_MAX_MA_COUNT                         512

/* Relocate these */
#define OFDPA_DOT1P_MIN_PRIORITY             0
#define OFDPA_DOT1P_MAX_PRIORITY             7

#define DOT1AG_LTR_ENTRIES_MAX                            256
#define DOT1AG_SLM_ENTRIES_MAX                            256

#define OFDPA_DOT1AG_MA_NAME_LEN_MIN                      1
#define OFDPA_DOT1AG_MA_NAME_LEN                          45
#define OFDPA_DOT1AG_MD_NAME_LEN_MIN                      1
#define OFDPA_DOT1AG_MD_NAME_LEN                          43
#define OFDPA_DOT1AG_MEPID_MIN                            1
#define OFDPA_DOT1AG_MEPID_MAX                            8191
#define OFDPA_DOT1AG_MD_LEVEL_MIN                         0
#define OFDPA_DOT1AG_MD_LEVEL_MAX                         7
#define OFDPA_DOT1AG_MD_LEVEL_COUNT                       OFDPA_DOT1AG_MD_LEVEL_MAX+1
#define OFDPA_DOT1AG_FNG_ALARM_TIME_MIN                   250
#define OFDPA_DOT1AG_FNG_ALARM_TIME_MAX                   1000
#define OFDPA_DOT1AG_FNG_ALARM_RESET_TIME_MIN             250
#define OFDPA_DOT1AG_FNG_ALARM_RESET_TIME_MAX             1000

#define OFDPA_DOT1AG_NUM_LBM_MIN                          1
#define OFDPA_DOT1AG_NUM_LBM_MAX                          255
#define OFDPA_DOT1AG_LBM_DEFAULT_LBM                      1
#define OFDPA_DOT1AG_LBM_TIMEOUT_IN_SECS                  5

#define OFDPA_DOT1AG_LTM_TTL_MIN                          1
#define OFDPA_DOT1AG_LTM_TTL_MAX                          255
#define OFDPA_DOT1AG_LTM_TIMEOUT_IN_SECS                  1
#define OFDPA_DOT1AG_LTM_DEFAULT_TTL                      64

#define OFDPA_DOT1AG_CCM_ARCHIVE_HOLD_TIME_MIN            1
#define OFDPA_DOT1AG_CCM_ARCHIVE_HOLD_TIME_MAX            65535

#define OFDPA_DOT1AG_MAX_VLANS                            OFDPA_MAX_VLANS

#define OFDPA_DOT1AG_VLANID_MIN                           1
#define OFDPA_DOT1AG_VLANID_MAX                           L7_PLATFORM_MAX_VLAN_ID
#define OFDPA_DOT1AG_CCM_FRAME_LEN                        1522

//#define DOT1AG_MSG_COUNT (OFDPA_DOT1AG_MAX_VLANS + (OFDPA_MAX_INTERFACE_COUNT * 2))

/* Default.h */
#define FD_DOT1AG_ADMIN_MODE                           OFDPA_DISABLE
#define FD_DOT1AG_ARCHIVE_HOLD_TIME                    600
#define FD_DOT1AG_FNG_ALARM_TIME                       250
#define FD_DOT1AG_FNG_ALARM_RESET_TIME                 1000
#define FD_DOT1AG_MEP_CCI_ENABLED_MODE                 OFDPA_FALSE
#define FD_DOT1AG_MEP_ACTIVE_MODE                      OFDPA_FALSE
#define FD_DOT1AG_MIP_MODE                             OFDPA_DISABLE
#define FD_DOT1AG_FNG_LOWEST_ALARM_PRI                 1

#define OFDPA_DOT1AG_LLDP_PORT_ID_LEN                     255

/* OPCODE For CFM Messages */
#define DOT1AG_EGRESS_ID_LEN                    8
#define DOT1AG_LLDP_CHASSIS_ID_LEN              255
#define DOT1AG_LLDP_PORT_ID_LEN                 255
#define DOT1AG_LTR_ORG_TLV_LEN                  1500

/* Sender ID TLV holder */
#define DOT1AG_TLV_FIELD_LEN_MAX_CHASSIS_ID        255
#define DOT1AG_TLV_FIELD_LEN_MAX_MGMT_ADDR_DOMAIN  255
#define DOT1AG_TLV_FIELD_LEN_MAX_MGMT_ADDR         32

typedef enum
{
  OFDPA_DOT1AG_LBM_LBR_TRAP_TO_CPU = 1,
  OFDPA_DOT1AG_LBR_TRAP_TO_CPU
} OFDPA_DOT1AG_LBM_t;

typedef enum
{
  OFDPA_DOT1AG_CC_TRAP_CONFIG          = 0x01,
  OFDPA_DOT1AG_CC_TRAP_CROSS_CONNECT   = 0x02,
  OFDPA_DOT1AG_CC_TRAP_LOOP            = 0x04,
  OFDPA_DOT1AG_CC_TRAP_MEP_UP          = 0x08,
  OFDPA_DOT1AG_CC_TRAP_MEP_DOWN        = 0x10
} Do1agCcmSnmpTraps_t;

typedef enum
{
  OFDPA_DOT1AG_MP_TYPE_ETH = 0,
  OFDPA_DOT1AG_MP_TYPE_BHH = 1
} OFDPA_DOT1AG_MP_type_t;

typedef enum
{
  /* 802.1ag clauses 12.14.6.1.3:e, 20.8.1 and 21.6.1.3 */
  DOT1AG_CCM_intervalInvalid = 0, /* No CCMs are sent (disabled). */
  DOT1AG_CCM_interval300Hz   = 1, /* CCMs are sent every 3 1/3 milliseconds (300Hz). */
  DOT1AG_CCM_interval10ms    = 2, /* CCMs are sent every 10 milliseconds. */
  DOT1AG_CCM_interval100ms   = 3, /* CCMs are sent every 100 milliseconds. */
  DOT1AG_CCM_interval1s      = 4, /* CCMs are sent every 1 second. */
  DOT1AG_CCM_interval10s     = 5, /* CCMs are sent every 10 seconds. */
  DOT1AG_CCM_interval1min    = 6, /* CCMs are sent every minute. */
  DOT1AG_CCM_interval10min   = 7

} Dot1agCfmCcmInterval_t;

#define OFDPA_DOT1AG_CCM_INTERVAL_MIN                  DOT1AG_CCM_interval300Hz
#define OFDPA_DOT1AG_CCM_INTERVAL_MAX                  DOT1AG_CCM_interval10min
#define FD_DOT1AG_CCM_INTERVAL                         DOT1AG_CCM_interval300Hz

typedef enum
{
  DOT1AG_MP_DIRECTION_DOWN = 1,
  DOT1AG_MP_DIRECTION_UP   = 2

} Dot1agCfmMpDirection_t;

typedef enum
{
  MEP_PARAM_dot1agCfmMepIdentifier = 0,
  MEP_PARAM_dot1agCfmMepIfIndex,
  MEP_PARAM_dot1agCfmMepDirection,
  MEP_PARAM_dot1agCfmMepPrimaryVid,
  MEP_PARAM_dot1agCfmMepActive,
  MEP_PARAM_dot1agCfmMepFngState,
  MEP_PARAM_dot1agCfmMepCciEnabled,
  MEP_PARAM_dot1agCfmMepCcmLtmPriority,
  MEP_PARAM_dot1agCfmMepMacAddress,
  MEP_PARAM_dot1agCfmMepLowPrDef,
  MEP_PARAM_dot1agCfmMepFngAlarmTime,
  MEP_PARAM_dot1agCfmMepFngResetTime,
  MEP_PARAM_dot1agCfmMepHighestPrDefect,
  MEP_PARAM_dot1agCfmMepDefects,
  MEP_PARAM_dot1agCfmMepErrorCcmLastFailure,
  MEP_PARAM_dot1agCfmMepXconCcmLastFailure,
  MEP_PARAM_dot1agCfmMepCcmSequenceErrors,
  MEP_PARAM_dot1agCfmMepCciSentCcms,
  MEP_PARAM_dot1agCfmMepNextLbmTransId,
  MEP_PARAM_dot1agCfmMepLbrIn,
  MEP_PARAM_dot1agCfmMepLbrInOutOfOrder,
  MEP_PARAM_dot1agCfmMepLbrBadMsdu,
  MEP_PARAM_dot1agCfmMepLtmNextSeqNumber,
  MEP_PARAM_dot1agCfmMepUnexpLtrIn,
  MEP_PARAM_dot1agCfmMepLbrOut,
  MEP_PARAM_dot1agCfmMepTransmitLbmStatus,
  MEP_PARAM_dot1agCfmMepTransmitLbmDestMacAddress,
  MEP_PARAM_dot1agCfmMepTransmitLbmDestMepId,
  MEP_PARAM_dot1agCfmMepTransmitLbmDestIsMepId,
  MEP_PARAM_dot1agCfmMepTransmitLbmMessages,
  MEP_PARAM_dot1agCfmMepTransmitLbmDataTlv,
  MEP_PARAM_dot1agCfmMepTransmitLbmVlanPriority,
  MEP_PARAM_dot1agCfmMepTransmitLbmVlanDropEnable,
  MEP_PARAM_dot1agCfmMepTransmitLbmResultOK,
  MEP_PARAM_dot1agCfmMepTransmitLbmSeqNumber,
  MEP_PARAM_dot1agCfmMepTransmitLtmStatus,
  MEP_PARAM_dot1agCfmMepTransmitLtmFlags,
  MEP_PARAM_dot1agCfmMepTransmitLtmTargetMacAddress,
  MEP_PARAM_dot1agCfmMepTransmitLtmTargetMepId,
  MEP_PARAM_dot1agCfmMepTransmitLtmTargetIsMepId,
  MEP_PARAM_dot1agCfmMepTransmitLtmTtl,
  MEP_PARAM_dot1agCfmMepTransmitLtmResult,
  MEP_PARAM_dot1agCfmMepTransmitLtmSeqNumber,
  MEP_PARAM_dot1agCfmMepTransmitLtmEgressIdentifier,
  MEP_PARAM_y1731CfmMepDefects,
  MEP_PARAM_y1731CfmMepAisTx,
  MEP_PARAM_y1731CfmMepAisRx,
  MEP_PARAM_y1731CfmMepLckTx,
  MEP_PARAM_y1731CfmMepLckRx,
  MEP_PARAM_y1731CfmMepCsfTx,
  MEP_PARAM_y1731CfmMepCsfRx
} Dot1agCfmMepEntry_t;

typedef enum
{
  LTR_PARAM_dot1agCfmLtrSeqNumber,
  LTR_PARAM_dot1agCfmLtrReceiveOrder,
  LTR_PARAM_dot1agCfmLtrTtl,
  LTR_PARAM_dot1agCfmLtrForwarded,
  LTR_PARAM_dot1agCfmLtrTerminalMep,
  LTR_PARAM_dot1agCfmLtrLastEgressIdentifier,
  LTR_PARAM_dot1agCfmLtrNextEgressIdentifier,
  LTR_PARAM_dot1agCfmLtrRelay,
  LTR_PARAM_dot1agCfmLtrChassisIdLen,
  LTR_PARAM_dot1agCfmLtrChassisIdSubtype,
  LTR_PARAM_dot1agCfmLtrChassisId,
  LTR_PARAM_dot1agCfmLtrManAddressDomainLen,
  LTR_PARAM_dot1agCfmLtrManAddressDomain,
  LTR_PARAM_dot1agCfmLtrManAddressLen,
  LTR_PARAM_dot1agCfmLtrManAddress,
  LTR_PARAM_dot1agCfmLtrIngress,
  LTR_PARAM_dot1agCfmLtrIngressMac,
  LTR_PARAM_dot1agCfmLtrIngressPortIdLen,
  LTR_PARAM_dot1agCfmLtrIngressPortIdSubtype,
  LTR_PARAM_dot1agCfmLtrIngressPortId,
  LTR_PARAM_dot1agCfmLtrEgress,
  LTR_PARAM_dot1agCfmLtrEgressMac,
  LTR_PARAM_dot1agCfmLtrEgressPortIdLen,
  LTR_PARAM_dot1agCfmLtrEgressPortIdSubtype,
  LTR_PARAM_dot1agCfmLtrEgressPortId,
  LTR_PARAM_dot1agCfmLtrOrganizationSpecificTlv

} Dot1agCfmLtrEntry_t;

typedef enum
{
   RMEP_PARAM_dot1agCfmMepDbRMepState,
   RMEP_PARAM_dot1agCfmMepDbRMepFailedOkTime,
   RMEP_PARAM_dot1agCfmMepDbMacAddress,
   RMEP_PARAM_dot1agCfmMepDbRdi,
   RMEP_PARAM_dot1agCfmMepDbPortStatusTlv,
   RMEP_PARAM_dot1agCfmMepDbInterfaceStatusTlv,
   RMEP_PARAM_dot1agCfmMepDbChassisIdLen,
   RMEP_PARAM_dot1agCfmMepDbChassisIdSubtype,
   RMEP_PARAM_dot1agCfmMepDbChassisId,
   RMEP_PARAM_dot1agCfmMepDbManAddressDomainLen,
   RMEP_PARAM_dot1agCfmMepDbManAddressDomain,
   RMEP_PARAM_dot1agCfmMepDbManAddressLen,
   RMEP_PARAM_dot1agCfmMepDbManAddress,
   /* User defined parameters */
   RMEP_PARAM_dot1agCfmMepDbCcmExpiryTime /* RMEP entry expiry timer */

} Dot1agCfmMepDbEntry_t;

typedef enum
{
   CFM_MP_PARAM_dot1agCfmStackifIndex,
   CFM_MP_PARAM_dot1agCfmStackVlanIdOrNone,
   CFM_MP_PARAM_dot1agCfmStackMdLevel,
   CFM_MP_PARAM_dot1agCfmStackDirection,
   CFM_MP_PARAM_dot1agCfmStackMdIndex,
   CFM_MP_PARAM_dot1agCfmStackMaIndex,
   CFM_MP_PARAM_dot1agCfmStackMepId,
   CFM_MP_PARAM_dot1agCfmStackMacAddress,
   /* params that are not part of standard MIB follows */
   CFM_MP_PARAM_dot1agCfmStackMepOperMode,
   CFM_MP_PARAM_dot1agCfmStackMipIdTlvData,

} Dot1agCfmStackEntry_t;

typedef enum dot1agLtrRelayAction_e
{
  OFDPA_DOT1AG_LTR_RELAY_ACTION_HIT = 1,
  OFDPA_DOT1AG_LTR_RELAY_ACTION_FDB,
  OFDPA_DOT1AG_LTR_RELAY_ACTION_MPDB
} Dot1agLtrRelayAction_t;

typedef enum dot1agLtrIngressAction_e
{
  OFDPA_DOT1AG_LTR_INGRESS_OK = 1,
  OFDPA_DOT1AG_LTR_INGRESS_DOWN,
  OFDPA_DOT1AG_LTR_INGRESS_BLOCKED,
  OFDPA_DOT1AG_LTR_INGRESS_VID
} Dot1agLtrIngressAction_t;

typedef enum dot1agLtrEgressAction_e
{
  OFDPA_DOT1AG_LTR_EGRESS_OK = 1,
  OFDPA_DOT1AG_LTR_EGRESS_DOWN,
  OFDPA_DOT1AG_LTR_EGRESS_BLOCKED,
  OFDPA_DOT1AG_LTR_EGRESS_VID
} Dot1agLtrEgressAction_t;

typedef enum
{
  DOT1AG_CHASSIS_ID_SUBTYPE_chassisComponent = 1,
  DOT1AG_CHASSIS_ID_SUBTYPE_interfaceAlias   = 2,
  DOT1AG_CHASSIS_ID_SUBTYPE_portComponent    = 3,
  DOT1AG_CHASSIS_ID_SUBTYPE_macAddress       = 4,
  DOT1AG_CHASSIS_ID_SUBTYPE_networkAddress   = 5,
  DOT1AG_CHASSIS_ID_SUBTYPE_interfaceName    = 6,
  DOT1AG_CHASSIS_ID_SUBTYPE_local            = 7

} Dot1agLldpChassisIdSubtype_t;

typedef enum
{
  DOT1AG_MEP_DEFECT_BIT_bDefRDICCM    = 0x01,
  DOT1AG_MEP_DEFECT_BIT_bDefMACstatus = 0x02,
  DOT1AG_MEP_DEFECT_BIT_bDefRemoteCCM = 0x04,
  DOT1AG_MEP_DEFECT_BIT_bDefErrorCCM  = 0x08,
  DOT1AG_MEP_DEFECT_BIT_bDefXconCCM   = 0x10
} Dot1agCfmMepDefectsBitMask_t;

typedef enum
{
  Y1731_MEP_DEFECT_BIT_bDefRDICCM   = 0x01,
  Y1731_MEP_DEFECT_BIT_bDefRSV      = 0x02,
  Y1731_MEP_DEFECT_BIT_bDefLOCCCM   = 0x08,
  Y1731_MEP_DEFECT_BIT_bDefUNPtrCCM = 0x10,
  Y1731_MEP_DEFECT_BIT_bDefUNPCCM   = 0x20,
  Y1731_MEP_DEFECT_BIT_bDefUNMCCM   = 0x40,
  Y1731_MEP_DEFECT_BIT_bDefMMGCCM   = 0x80,
  Y1731_MEP_DEFECT_BIT_bDefUNLCCM   = 0x100
} Y1731CfmMepDefectsBitMask_t;

typedef enum
{
  Y1731_DEFECT_FUNCTION_NONE = 0,
  Y1731_DEFECT_FUNCTION_AIS  = 1,
  Y1731_DEFECT_FUNCTION_LCK  = 2,
  Y1731_DEFECT_FUNCTION_CSF  = 3
} Y1731CfmDefectFunction_t;

typedef enum
{
  DOT1AG_DEFECT_none         = 0,
  DOT1AG_DEFECT_defRDICCM    = 1,
  DOT1AG_DEFECT_defMACstatus = 2,
  DOT1AG_DEFECT_defRemoteCCM = 3,
  DOT1AG_DEFECT_defErrorCCM  = 4,
  DOT1AG_DEFECT_defXconCCM   = 5
} Dot1agCfmMepDefects_t;

typedef enum
{
  OFDPA_DOT1AG_EVENT_none,
  OFDPA_DOT1AG_EVENT_defRDICCM,
  OFDPA_DOT1AG_EVENT_defMACstatus,
  OFDPA_DOT1AG_EVENT_defRemoteCCM,
  OFDPA_DOT1AG_EVENT_defErrorCCM,
  OFDPA_DOT1AG_EVENT_defXconCCM,
  OFDPA_DOT1AG_EVENT_defIntfStatusDefect,
  OFDPA_DOT1AG_EVENT_defPortStatusDefect
} OFDPA_DOT1AG_EVENT_TYPE_t;

typedef struct dot1agEventInfo_s
{
  uint32_t                  megIndex;
  uint32_t                  lmepId;
  uint32_t                  lmipId;
  uint32_t                  rmepId;
  uint32_t                  rmipId;
  OFDPA_DOT1AG_EVENT_TYPE_t eventType;
  OFDPA_BOOL                faultOn;
} dot1agEventInfo_t;

/* Debug trace flags */
typedef enum
{
  CFM_NONE         =        0x00,
  CFM_PDU          =        0x01,
  CFM_OPCODE_DEMUX =        0x02,
  CFM_SM_CCI       =        0x04,
  CFM_SM_CCR       =        0x08,
  CFM_SM_RMEP      =        0x10,
  CFM_SM_RMEP_ERR  =        0x20,
  CFM_SM_XCON      =        0x40,
  CFM_SM_LBR       =        0x80,
  CFM_SM_LBI       =       0x100,
  CFM_SM_LBIR      =       0x200,
  CFM_SM_FNG       =       0x400,
  CFM_SM_LTIR      =       0x800,
  CFM_SM_LTRR      =      0x1000,
  CFM_SM_LTRT      =      0x2000,
  CFM_LTM          =      0x4000,
  CFM_LBM          =      0x8000,
  CFM_CCM          =     0x10000,
  CFM_LTR          =     0x20000,
  CFM_LBR          =     0x40000,
  CFM_TIMERS       =     0x80000,
  CFM_MPOBJ        =    0x100000,
  CFM_HW_EVENTS    =    0x200000,
  CFM_EVENTS       =    0x400000,
  CFM_STP_EVENTS   =    0x800000,
  CFM_LM           =   0x1000000,
  CFM_DM           =   0x2000000,
  CFM_PM           =   0x4000000,
  CFM_DEFECT       =   0x8000000,
} DOT1AG_TRACE_FLAGS_t;

typedef enum
{
  DOT1AG_OAM_LMM_TOOL_TYPE_CCM = 0,
  DOT1AG_OAM_LMM_TOOL_TYPE_LM  = 1,
  DOT1AG_OAM_LMM_TOOL_TYPE_SLM = 2
} Dot1agOamLmmToolType_t;

#define MAX_Y1731_HISTORY_BIN 16

typedef enum
{
  DOT1AG_ROW_INACTIVE         = 0,
  DOT1AG_ROW_INACTIVE_PENDING = 0x1,
  DOT1AG_ROW_ACTIVE           = 0x2,
  DOT1AG_ROW_OPERATIONAL      = 0x4,

} DOT1AG_ROW_STATUS_t;

typedef enum
{
  y1731DmTypeInvalid,
  y1731DmTypeProactive,
  y1731DmTypeOndemand,

} y1731MepDmType_t;

typedef enum
{
  Y1731_BIN_MODE_15M,
  Y1731_BIN_MODE_24H,
  MAX_Y1731_BIN_MODE,
} y1731BinModeType_t;

typedef struct y1731DmTs_s
{
  uint32_t  second;
  uint32_t  ns;
} y1731DmTs_t;

typedef struct
{
  y1731DmTs_t  tBFD;
  y1731DmTs_t  tFFD;
  y1731DmTs_t  tNFD;
} y1731CfmMepDmPrim;

typedef struct
{
  uint32_t nNTF;
  uint32_t nNLF;
  uint32_t nFTF;
  uint32_t nFLF;
  uint32_t tTFCnt;
} y1731CfmMepLmPrim;

typedef struct y1731ProLmBin_s
{
  uint32_t   mN_FLR;
  uint32_t   aN_FLR;
  uint32_t   xN_FLR;
  uint32_t   mF_FLR;
  uint32_t   aF_FLR;
  uint32_t   xF_FLR;
  uint32_t   Total_N_TF;                /* total N_TF */
  uint32_t   Total_N_LF;                /* total N_LF */
  uint32_t   Total_F_TF;                /* total F_TF */
  uint32_t   Total_F_LF;                /* total F_LF */
  uint32_t   N_FLR_Total_Num;           /* total N_FLR event number */
  uint32_t   F_FLR_Total_Num;           /* total F_FLR event number */
  OFDPA_BOOL isValid;
  uint32_t   startTime;                 /* the start time of current bin, unit: second */
  uint32_t   endTime;                   /*only meaningful for history bin */
  OFDPA_BOOL suspectFlag;               /* true - current bin data is incomplete or invalid */
} y1731ProLmBin_t;

typedef struct y1731DmBin_s
{
  uint32_t   mB_FD;
  uint32_t   aB_FD;
  uint32_t   xB_FD;
  uint32_t   N_FD_Pre;                  /*the previous N_FD*/
  uint32_t   mN_FDV;
  uint32_t   aN_FDV;
  uint32_t   xN_FDV;
  uint32_t   F_FD_Pre;                  /*the previous F_FD*/
  uint32_t   mF_FDV;
  uint32_t   aF_FDV;
  uint32_t   xF_FDV;
  uint32_t   Total_Num;                 /* total B_FD, F_FD, N_FD event */
  uint32_t   Total_B_FD;                /* total B_FD */
  uint32_t   Total_N_FDV;               /* total N_FDV */
  uint32_t   Total_F_FDV;               /* total F_FDV */
  OFDPA_BOOL isValid;
  uint32_t   startTime;                 /* the start time of current bin, unit: second */
  uint32_t   endTime;                   /*only meaningful for history bin */
  uint32_t   nFDM;                      /* number of frame delay measurements */
  OFDPA_BOOL suspectFlag;               /* true - current bin data is incomplete or invalid */
} y1731DmBin_t;

typedef struct y1731CfmMepProlmTable_s
{
  /* primitive */
  y1731CfmMepLmPrim           lmPrim;
  y1731ProLmBin_t             prolmCurBin[MAX_Y1731_BIN_MODE];
  y1731ProLmBin_t             prolmHistoryBin[MAX_Y1731_HISTORY_BIN][MAX_Y1731_BIN_MODE];
  uint32_t                    prolmHistoryBinTail[MAX_Y1731_BIN_MODE];
  uint32_t                    prolmBinEnable;
  DOT1AG_ROW_STATUS_t         active;
  OFDPA_BOOL                  prolmEnabled;
  uint8_t                     prolmDestMacAddress[OFDPA_MAC_ADDR_LEN];
  uint32_t                    prolmDestMepId;
  OFDPA_BOOL                  prolmDestIsMepId;
  OFDPA_BOOL                  saved;
  /* from PDU */
  uint32_t                    txFCfPrev;
  uint32_t                    rxFCfPrev;
  uint32_t                    rxFcbPrev;
  uint32_t                    txFCbPrev;
  /* counter nead-end */
  uint32_t                    txFCf;
  uint32_t                    txFCl;
  uint32_t                    rxFCl;
  uint32_t                    rxFClPrev;
  struct dot1agCfmStackObj_s *pMPObj;   /* MP holder  */
  uint32_t                    prolmSlmTestId;
  uint32_t                    prolmPriorityClass;
  Dot1agCfmCcmInterval_t      prolmControlPeriod;
} y1731CfmMepProlmTable_t;

typedef struct y1731CfmMepDmTable_s
{
  DOT1AG_ROW_STATUS_t         active;
  OFDPA_BOOL                  dmEnabled;
  y1731DmBin_t                dmCurBin[MAX_Y1731_BIN_MODE];
  y1731DmBin_t                dmHistoryBin[MAX_Y1731_HISTORY_BIN][MAX_Y1731_BIN_MODE];
  uint32_t                    dmHistoryBinTail[MAX_Y1731_BIN_MODE];
  uint32_t                    dmBinEnable;
  y1731MepDmType_t            dmType;
  Dot1agCfmCcmInterval_t      dmControlPeriod;
  uint32_t                    dmToSend;
  uint32_t                    dmSent;
  uint32_t                    dmRecvd;
  void                       *dmTimer;
  uint32_t                    dmStartTime;
  uint32_t                    dmEndTime;
  void                       *dmStartTimer;
  void                       *dmStopTimer;
  uint8_t                     dmDestMacAddress[OFDPA_MAC_ADDR_LEN];
  uint32_t                    dmDestMepId;
  OFDPA_BOOL                  dmDestIsMepId;
  uint32_t                    dmControlPriority;
  OFDPA_BOOL                  saved;
  int32_t                    dmPacketLength;
  /* primitive */
  y1731CfmMepDmPrim           dmPrim;
  y1731DmTs_t                 rxTimeb;
  struct dot1agCfmStackObj_s *pMPObj;   /* MP holder  */
} y1731CfmMepDmTable_t;

typedef struct y1731OndemLmBin_s
{
  uint32_t tNTFCnt;
  uint32_t tNLFCnt;
  uint32_t tNFLRCnt;
  uint32_t tFTFCnt;
  uint32_t tFLFCnt;
  uint32_t tFFLRCnt;
  uint32_t sLMCnt;                      //successful lm
  uint32_t uLMCnt;                      //unsuccessful lm
  uint32_t elapsedTime;
} y1731OndemLmBin_t;


typedef struct y1731CfmMepOndemlmTable_s
{
  DOT1AG_ROW_STATUS_t         active;
  OFDPA_BOOL                  ondemlmEnabled;
  Dot1agCfmCcmInterval_t      ondemlmControlPeriod;
  uint32_t                    ondemlmToSend;
  uint32_t                    ondemlmSent;
  uint32_t                    ondemlmRecvd;
  void                       *ondemlmTimer;
  uint32_t                    ondemlmStartTime;
  uint32_t                    ondemlmEndTime;
  void                       *ondemlmStartTimer;
  void                       *ondemlmStopTimer;
  uint8_t                     ondemlmDestMacAddress[OFDPA_MAC_ADDR_LEN];
  uint32_t                    ondemlmDestMepId;
  OFDPA_BOOL                  ondemlmDestIsMepId;
  uint32_t                    ondemlmControlPriority;
  uint32_t                    ondemlmPriorityClass;
  OFDPA_BOOL                  saved;
  /* primitive */
  y1731CfmMepLmPrim           lmPrim;
  uint32_t                    txFCfPrev;
  uint32_t                    rxFcfPrev;
  uint32_t                    txFCbPrev;
  uint32_t                    txFCf;
  uint32_t                    rxFCl;
  uint32_t                    rxFClPrev;
  y1731OndemLmBin_t           ondemlmBin;
  struct dot1agCfmStackObj_s *pMPObj;   /* MP holder  */
  uint8_t                     ondemlmToolType; /* 0 - CCM, 1- LM, 2 - SLM*/
  uint32_t                    ondemlmSlmTestId;
} y1731CfmMepOndemlmTable_t;

typedef enum y1731CfmPeriod_e
{
  Y1731_CFM_PERIOD_1PPS = 1000,
  Y1731_CFM_PERIOD_1PPM = 60000,
  Y1731_CFM_PERIOD_MAX
} y1731CfmPeriod_t;

#define Y1731_PERIOD_FIELD_GET(type) ((type == Y1731_CFM_PERIOD_1PPS) ? 4 : 6)
#define Y1731_PERIOD_FIELD_PARSE(value) ((value == 4) ? Y1731_CFM_PERIOD_1PPS : Y1731_CFM_PERIOD_1PPM)

#define Y1731_AIS_PERIOD_FIELD_GET(type) Y1731_PERIOD_FIELD_GET(type)
#define Y1731_AIS_PERIOD_FIELD_PARSE(value) Y1731_PERIOD_FIELD_PARSE(value)
#define Y1731_LCK_PERIOD_FIELD_GET(type) Y1731_PERIOD_FIELD_GET(type)
#define Y1731_LCK_PERIOD_FIELD_PARSE(value) Y1731_PERIOD_FIELD_PARSE(value)

/* Linktrace Reply Table Database */
typedef struct  dot1agCfmLtrObjKey_s
{
  uint32_t dot1agCfmMdIndex;
  uint32_t dot1agCfmMaIndex;
  uint32_t dot1agCfmMepIdentifier;
  uint32_t dot1agCfmLtrSeqNumber;
  uint32_t dot1agCfmLtrReceiveOrder;

} dot1agCfmLtrObjKey_t;

typedef enum
{
  DOT1AG_PORT_ID_SUBTYPE_interfaceAlias  = 1,
  DOT1AG_PORT_ID_SUBTYPE_portComponent   = 2,
  DOT1AG_PORT_ID_SUBTYPE_macAddress      = 3,
  DOT1AG_PORT_ID_SUBTYPE_networkAddress  = 4,
  DOT1AG_PORT_ID_SUBTYPE_interfaceName   = 5,
  DOT1AG_PORT_ID_SUBTYPE_agentCircuitId  = 6,
  DOT1AG_PORT_ID_SUBTYPE_local           = 7
} Dot1agLldpPortIdSubtype_t;

typedef enum
{
  DOT1AG_RELAY_ACTION_rlyHit   =  1,
  DOT1AG_RELAY_ACTION_rlyFdb   =  2,
  DOT1AG_RELAY_ACTION_rlyMpdb  =  3

} Dot1agCfmRelayActionFieldValue_t;

typedef enum
{
  DOT1AG_INGRESSS_ACTION_ingNoTlv    = 0,
  DOT1AG_INGRESSS_ACTION_ingOk       = 1,
  DOT1AG_INGRESSS_ACTION_ingDown     = 2,
  DOT1AG_INGRESSS_ACTION_ingBlocked  = 3,
  DOT1AG_INGRESSS_ACTION_ingVid      = 4

} Dot1agCfmIngressActionFieldValue_t;

typedef enum
{
  DOT1AG_EGRESSS_ACTION_egrNoTlv    = 0,
  DOT1AG_EGRESSS_ACTION_egrOk       = 1,
  DOT1AG_EGRESSS_ACTION_egrDown     = 2,
  DOT1AG_EGRESSS_ACTION_egrBlocked  = 3,
  DOT1AG_EGRESSS_ACTION_egrVid      = 4

} Dot1agCfmEgressActionFieldValue_t;

typedef struct dot1agCfmLtrObj_s
{
  dot1agCfmLtrObjKey_t                key;
  uint32_t                            dot1agCfmLtrTtl;
  OFDPA_BOOL                          dot1agCfmLtrForwarded;
  OFDPA_BOOL                          dot1agCfmLtrTerminalMep;
  uint8_t                             dot1agCfmLtrLastEgressIdentifier[DOT1AG_EGRESS_ID_LEN];
  uint8_t                             dot1agCfmLtrNextEgressIdentifier[DOT1AG_EGRESS_ID_LEN];
  Dot1agCfmRelayActionFieldValue_t    dot1agCfmLtrRelay;
  uint8_t                             dot1agCfmLtrChassisIdLen;
  Dot1agLldpChassisIdSubtype_t        dot1agCfmLtrChassisIdSubtype;
  uint8_t                             dot1agCfmLtrChassisId[DOT1AG_LLDP_CHASSIS_ID_LEN];
  uint8_t                             dot1agCfmLtrManAddressDomainLen;
  uint8_t                             dot1agCfmLtrManAddressDomain[DOT1AG_TLV_FIELD_LEN_MAX_MGMT_ADDR_DOMAIN];
  uint8_t                             dot1agCfmLtrManAddressLen;
  uint8_t                             dot1agCfmLtrManAddress[DOT1AG_TLV_FIELD_LEN_MAX_MGMT_ADDR];
  Dot1agCfmIngressActionFieldValue_t  dot1agCfmLtrIngress;
  uint8_t                             dot1agCfmLtrIngressMac[OFDPA_MAC_ADDR_LEN];
  uint8_t                             dot1agCfmLtrIngressPortIdLen;
  Dot1agLldpPortIdSubtype_t           dot1agCfmLtrIngressPortIdSubtype;
  uint8_t                             dot1agCfmLtrIngressPortId[DOT1AG_LLDP_PORT_ID_LEN];
  Dot1agCfmEgressActionFieldValue_t   dot1agCfmLtrEgress;
  uint8_t                             dot1agCfmLtrEgressMac[OFDPA_MAC_ADDR_LEN];
  uint8_t                             dot1agCfmLtrEgressPortIdLen;
  Dot1agLldpPortIdSubtype_t           dot1agCfmLtrEgressPortIdSubtype;
  uint8_t                             dot1agCfmLtrEgressPortId[DOT1AG_LLDP_PORT_ID_LEN];
  uint8_t                             dot1agCfmLtrOrganizationSpecificTlv[DOT1AG_LTR_ORG_TLV_LEN];
  void                               *next;
} dot1agCfmLtrObj_t;

#define OFDPA_DOT1AG_MAX_ETHERNET_FRAME_SIZE        1500
#endif /* __DOT1AG_EXPORTS_H_*/
