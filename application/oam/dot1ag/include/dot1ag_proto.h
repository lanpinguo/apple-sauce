/*********************************************************************
*
*  (C) Copyright Broadcom Corporation 2008-2015
*
* **********************************************************************
*
* @filename       dot1ag_proto.h
* @purpose        Dot1ag protocol specification defines
* @component      Dot1ag
* @comments       none
* @create         11/19/2008
* @end
*
* **********************************************************************/
#ifndef INCLUDE_DOT1AG_PROTO_H
#define INCLUDE_DOT1AG_PROTO_H

#define DOT1AG_ETHER_TYPE                 0x8902
#define DOT1AG_PBB_COMPONENTS_COUNT       1
#define DOT1AG_PBB_COMPONENT_DEFAULT      0
#define DOT1AG_TIMER_INTERVAL             1000 /* milliseconds */


typedef enum
{
/* 802.1ag clause 12.14.5.1.3:c and 22.2.3 */
  DOT1AG_defMHFnone     = 1,
  DOT1AG_defMHFdefault  = 2,
  DOT1AG_defMHFexplicit = 3,
  DOT1AG_defMHFdefer    = 4

} Dot1agCfmMhfCreation_t;

typedef enum
{
/* 802.1ag clause 12.14.5.1.3:c and 22.2.3 */
  DOT1AG_sendIdNone          = 1,
  DOT1AG_sendIdChassis       = 2,
  DOT1AG_sendIdManage        = 3,
  DOT1AG_sendIdChassisManage = 4,
  DOT1AG_sendIdDefer         = 5

} Dot1agCfmIdPermission_t;

typedef enum
{
/* 802.1ag clause 21.6.5.4, Table 21-20 */
  DOT1AG_MA_NAME_TYPE_primaryVid = 1,
  DOT1AG_MA_NAME_TYPE_charString = 2,
  DOT1AG_MA_NAME_TYPE_unsignedInt16 = 3,
  DOT1AG_MA_NAME_TYPE_rfc2865VpnId = 4

} Dot1agCfmMaintAssocNameType_t;

typedef enum
{
/* 802.1ag clause 21.6.5, Table 21-19 */
  DOT1AG_MD_NAME_TYPE_none               = 1,
  DOT1AG_MD_NAME_TYPE_dnsLikeName        = 2,
  DOT1AG_MD_NAME_TYPE_macAddressAndUint  = 3,
  DOT1AG_MD_NAME_TYPE_charString         = 4
} Dot1agCfmMaintDomainNameType_t;

typedef enum
{
  DOT1AG_ALARM_PRIORITY_defRDICCM        = 1/*1*/,
  DOT1AG_ALARM_PRIORITY_defMACstatus     = 2/*2*/,
  DOT1AG_ALARM_PRIORITY_defRemoteCCM     = 3/*3*/,
  DOT1AG_ALARM_PRIORITY_defErrorCCM      = 4/*4*/,
  DOT1AG_ALARM_PRIORITY_defXconCCM       = 5/*5*/,
  DOT1AG_ALARM_PRIORITY_none             = 6/*6*/,

#define DOT1AG_ALARM_PRIORITY_MIN DOT1AG_ALARM_PRIORITY_defRDICCM
#define DOT1AG_ALARM_PRIORITY_MAX DOT1AG_ALARM_PRIORITY_none
} Dot1agCfmLowestAlarmPri_t;

typedef enum
{
  DOT1AG_FNG_STATE_fngReset          = 1,
  DOT1AG_FNG_STATE_fngDefect         = 2,
  DOT1AG_FNG_STATE_fngReportDefect   = 3,
  DOT1AG_FNG_STATE_fngDefectReported = 4,
  DOT1AG_FNG_STATE_fngDefectClearing = 5,

} Dot1agCfmFngState_t;

typedef enum
{
  DOT1AG_DEFECT_PRIORITY_none              = 0/*0*/,
  DOT1AG_DEFECT_PRIORITY_defRDICCM         = 1/*1*/,
  DOT1AG_DEFECT_PRIORITY_defMACstatus      = 2/*2*/,
  DOT1AG_DEFECT_PRIORITY_defRemoteCCM      = 3/*3*/,
  DOT1AG_DEFECT_PRIORITY_defErrorCCM       = 4/*4*/,
  DOT1AG_DEFECT_PRIORITY_defXconCCM        = 5/*5*/,

  DOT1AG_DEFECT_PRIORITY_MAX
} Dot1agCfmHighestDefectPri_t;

typedef enum
{
  Y1731_DEFECT_TYPE_RDI   = 1, /*Taken from DOT1AG_DEFECT_PRIORITY_defRDICCM*/
  Y1731_DEFECT_TYPE_RSV   = 2,
  Y1731_DEFECT_TYPE_LOC   = 3, /*Taken from DOT1AG_DEFECT_PRIORITY_defRemoteCCM*/
  Y1731_DEFECT_TYPE_UNPTR = 4, /*Not implemented until now*/
  Y1731_DEFECT_TYPE_UNP   = 5, /*DOT1AG_DEFECT_PRIORITY_defErrorCCM*/
  Y1731_DEFECT_TYPE_UNM   = 6, /*DOT1AG_DEFECT_PRIORITY_defErrorCCM*/
  Y1731_DEFECT_TYPE_MMG   = 7, /*DOT1AG_DEFECT_PRIORITY_defXconCCM*/
  Y1731_DEFECT_TYPE_UNL   = 8, /*DOT1AG_DEFECT_PRIORITY_defXconCCM*/
  Y1731_DEFECT_TYPE_NONE,
  Y1731_DEFECT_TYPE_MAX = Y1731_DEFECT_TYPE_NONE
} y1731_defect_type_t;

typedef enum
{
  DOT1AG_RMEP_STATE_rMepIdle    = 1,
  DOT1AG_RMEP_STATE_rMepStart   = 2,
  DOT1AG_RMEP_STATE_rMepFailed  = 3,
  DOT1AG_RMEP_STATE_rMepOk      = 4

} Dot1agCfmRemoteMepState_t;

typedef enum
{
  DOT1AG_RMEP_PORT_STATE_psNoPortStateTLV  = 0,
  DOT1AG_RMEP_PORT_STATE_psBlocked         = 1,
  DOT1AG_RMEP_PORT_STATE_psUp              = 2

} Dot1agCfmPortStatus_t;

typedef enum
{
  DOT1AG_RMEP_INTERFACE_STATUS_isNoInterfaceStatusTLV  = 0,
  DOT1AG_RMEP_INTERFACE_STATUS_isUp                    = 1,
  DOT1AG_RMEP_INTERFACE_STATUS_isDown                  = 2,
  DOT1AG_RMEP_INTERFACE_STATUS_isTesting               = 3,
  DOT1AG_RMEP_INTERFACE_STATUS_isUnknown               = 4,
  DOT1AG_RMEP_INTERFACE_STATUS_isDormant               = 5,
  DOT1AG_RMEP_INTERFACE_STATUS_isNotPresent            = 6,
  DOT1AG_RMEP_INTERFACE_STATUS_isLowerLayerDown        = 7

} Dot1agCfmInterfaceStatus_t;

typedef enum
{
  DOT1AG_CFM_UP_MEP   = 0,
  DOT1AG_CFM_DOWN_MEP = 1,
  DOT1AG_CFM_UP_MHF   = 2,
  DOT1AG_CFM_DOWN_MHF = 3

} Dot1agCfmMPType_t;

#define DOT1AG_MEP_XMIT_LTM_FLAG_DEFVAL         0x80

/* OPCODE For CFM Messages */
#define DOT1AG_LBM_DATA_LEN                     1500

#define DOT1AG_MAID_MAX_LEN                     48
#define DOT1AG_MAID_MD_NAME_MAX_LEN             43

#define DOT1AG_CCM_ITUT_RSVD_LEN                16

#define DOT1AG_CCM_FLAGS_RDI_GET(x)             ((((x) & 0x80)) ? OFDPA_TRUE : OFDPA_FALSE)
#define DOT1AG_CCM_FLAGS_CCMI_GET(x)            ((x) & 0x07)

#define DOT1AG_RMEP_INTERVAL_GET(x)             (((x) % 2) ? (uint32_t)((7 * (x))/2) + 1: (uint32_t)((7 * (x))/2))
#define DOT1AG_FNG_TIME_GET(x)                  (((x) % 100) ? ((x) / 100) + 1 : ((x)/100))

#define DOT1AG_LTR_FLAGS_USE_FDB_GET(x)         ((((x) & 0x80)) ? OFDPA_TRUE : OFDPA_FALSE)
#define DOT1AG_LTR_FLAGS_FWD_YES_GET(x)         ((((x) & 0x40)) ? OFDPA_TRUE : OFDPA_FALSE)
#define DOT1AG_LTR_TERMINAL_MEP_GET(x)          ((((x) & 0x20)) ? OFDPA_TRUE : OFDPA_FALSE)
#define DOT1AG_LTR_FLAGS_USE_FDB_SET(x)         ((x) |= 0x80)
#define DOT1AG_LTR_FLAGS_FWD_YES_SET(x)         ((x) |= 0x40)
#define DOT1AG_LTR_TERMINAL_MEP_SET(x)          ((x) |= 0x20)

/* TLV macros */
/* Table 21-6. TLV Type Field values*/
#define DOT1AG_TLV_TYPE_END                      0x00
#define DOT1AG_TLV_TYPE_SENDER_ID                0x01
#define DOT1AG_TLV_TYPE_PORT_STATUS              0x02
#define DOT1AG_TLV_TYPE_DATA                     0x03
#define DOT1AG_TLV_TYPE_INTERFACE_STATUS         0x04
#define DOT1AG_TLV_TYPE_REPLY_INGRESS            0x05
#define DOT1AG_TLV_TYPE_REPLY_EGRESS             0x06
#define DOT1AG_TLV_TYPE_LTM_EGR_ID               0x07
#define DOT1AG_TLV_TYPE_LTR_EGR_ID               0x08
#define DOT1AG_TLV_TYPE_ORG_SPECIFIC             0x1F
#define DOT1AG_TLV_TYPE_IEEE_RESERVED_MIN        0x09
#define DOT1AG_TLV_TYPE_IEEE_RESERVED_MAX        0x1E
#define DOT1AG_TLV_TYPE_ITU_T_Y_1731_MIN         0x20
#define DOT1AG_TLV_TYPE_ITU_T_Y_1731_MAX         0x3F
#define DOT1AG_TLV_TYPE_IEEE_RESERVED_MIN1       0x40
#define DOT1AG_TLV_TYPE_IEEE_RESERVED_MAX1       0xFF
#define DOT1AG_TLV_TYPE_BRCM_RX_COUNTER          0xFA
#define DOT1AG_TLV_TYPE_BRCM_RX_TIMESTAMP        0xFB
#define DOT1AG_TLV_TYPE_TEST                     0x20
#define DOT1AG_TLV_TYPE_TARGET_MEP               0x21
#define DOT1AG_TLV_TYPE_TARGET_MIP               0x21
#define DOT1AG_TLV_TYPE_REPLY_MEP                0x22
#define DOT1AG_TLV_TYPE_REPLY_MIP                0x22
#define DOT1AG_TLV_TYPE_REQUEST_MEP              0x23

#define DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_INGRESS   0x0
#define DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_EGRESS    0x1
#define DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_ICC_MEPID 0x2
#define DOT1AG_TLV_TARGET_MEP_SUBTYPE_DISCOVERY_ICC_MIPID 0x3

#define DOT1AG_BHH_TLV_ID_SUBTYPE_OFFSET                  3
#define DOT1AG_BHH_TLV_MPID_OFFSET                        4
#define DOT1AG_BHH_LB_TARGET_MEP_TLV_TYPE_OFFSET                    (DOT1AG_CFM_COMMON_HDR_LEN + DOT1AG_CFM_LBR_FIRST_TLV_OFFSET)
#define DOT1AG_BHH_LB_TARGET_MEP_TLV_ID_SUBTYPE_OFFSET              (DOT1AG_BHH_LB_TARGET_MEP_TLV_TYPE_OFFSET + DOT1AG_BHH_TLV_ID_SUBTYPE_OFFSET)
#define DOT1AG_BHH_LB_TARGET_MEP_TLV_MPID_OFFSET                    (DOT1AG_BHH_LB_TARGET_MEP_TLV_TYPE_OFFSET + DOT1AG_BHH_TLV_MPID_OFFSET)
#define DOT1AG_BHH_LB_REQUESTING_MEP_TLV_TYPE_OFFSET                (DOT1AG_BHH_LB_TARGET_MEP_TLV_TYPE_OFFSET + DOT1AG_TLV_FIELD_LEN_TARGET_MEP)
#define DOT1AG_BHH_LB_REQUESTING_MEP_TLV_LOOPBACK_INDICATION_OFFSET (DOT1AG_BHH_LB_REQUESTING_MEP_TLV_TYPE_OFFSET + DOT1AG_BHH_TLV_ID_SUBTYPE_OFFSET)
#define DOT1AG_BHH_LB_REQUESTING_MEP_TLV_MPID_OFFSET                (DOT1AG_BHH_LB_REQUESTING_MEP_TLV_TYPE_OFFSET + DOT1AG_BHH_TLV_MPID_OFFSET)

#define DOT1AG_TLV_LEN_LEN                       2

#define DOT1AG_TLV_LEN_PORT_STATUS               1
#define DOT1AG_TLV_LEN_INTERFACE_STATUS          1
#define DOT1AG_TLV_LEN_LTM_EGR_ID                8
#define DOT1AG_TLV_LEN_LTR_EGR_ID                16

#define DOT1AG_TLV_LEN_BRCM_RX_COUNTER           4
#define DOT1AG_TLV_LEN_BRCM_RX_TIMESTAMP         8
#define DOT1AG_TLV_LEN_TEST                      1

#define DOT1AG_TLV_FIELD_LEN_EGRESS_ID           8
#define DOT1AG_TLV_FIELD_LEN_PORT_ID             6

#define DOT1AG_TLV_VALUE_LEN_MAX                 255
#define DOT1AG_DATA_TLV_VALUE_LEN_MAX            OFDPA_DOT1AG_MAX_ETHERNET_FRAME_SIZE - 20
#define DOT1AG_TEST_TLV_VALUE_LEN_MAX            OFDPA_DOT1AG_MAX_ETHERNET_FRAME_SIZE - 20
#define MPLS_HDR_LEN_MAX                         255
#define DOT1AG_FRAME_TLV_VALUE_LEN_MIN           64 
#define DOT1AG_FRAME_TLV_VALUE_LEN_MAX           9600

typedef enum
{
  DOT1AG_TLV_VAL_PORT_STATUS_BLOCK = 1,
  DOT1AG_TLV_VAL_PORT_STATUS_UP    = 2
} dot1agCfmPortStatusTlvValue_t;

#define dot1agCfmIsCCMTlvValid(type)                                    \
  {                                                                     \
    (((type >= OFDPA_DOT1AG_END_TLV) && (type <= OFDPA_DOT1AG_LTR_EGR_ID_TLV)) || \
     ((type >= OFDPA_DOT1AG_IEEE_RESERVED_TLV_MIN) && (type <= OFDPA_DOT1AG_IEEE_RESERVED_TLV_MAX)) || \
     (type == OFDPA_DOT1AG_ORG_SPECIFIC_TLV) ||                         \
     ((type >= OFDPA_DOT1AG_IEEE_RESERVED_TLV_MIN1) && (type <= OFDPA_DOT1AG_IEEE_RESERVED_TLV_MAX1)) || \
     ((type >= OFDPA_DOT1AG_ITU_T_Y_1731_TLV_MIN) && (type <= OFDPA_DOT1AG_ITU_T_Y_1731_TLV_MAX))) \
  }

/* Sender ID TLV holder */
#define DOT1AG_TLV_FIELD_LEN_CHASSIS_ID_LENGTH     1
#define DOT1AG_TLV_FIELD_LEN_CHASSIS_ID_SUBTYPE    1
#define DOT1AG_TLV_FIELD_LEN_MGMT_ADRS_DOMAIN_LEN  1
#define DOT1AG_TLV_FIELD_LEN_MGMT_ADRS_LEN         1
#define DOT1AG_MGMT_ADDR_LEN                       6
#define DOT1AG_L4PORT_SNMP_LEN                     2
#define OFDPA_DOT1AG_L4PORT_SNMP                   161

typedef struct dot1agCfmSenderIdTLV_s
{
  uint16_t                     length;
  uint8_t                      chassisIdLen;
  Dot1agLldpChassisIdSubtype_t chassisIdSubtype;
  uint8_t                      chassisId[DOT1AG_LLDP_CHASSIS_ID_LEN];
  uint8_t                      addressDomainLen;
  uint8_t                      addressDomain[DOT1AG_TLV_FIELD_LEN_MAX_MGMT_ADDR_DOMAIN];
  uint8_t                      manAddressLen;
  uint8_t                      manAddress[DOT1AG_TLV_FIELD_LEN_MAX_MGMT_ADDR];
} dot1agCfmSenderIdTLV_t;

#define DOT1AG_TLV_FIELD_LEN_PORT_STATUS           1
typedef struct dot1agCfmPortStatusTLV_s
{
  uint16_t                   length;
  uint8_t                    portStatus;
} dot1agCfmPortStatusTLV_t;

#define DOT1AG_TLV_FIELD_LEN_INTERFACE_STATUS      1
typedef struct dot1agCfmInterfaceStatusTLV_s
{
  uint16_t                   length;
  uint8_t                    interfaceStatus;
} dot1agCfmInterfaceStatusTLV_t;

#define DOT1AG_TLV_FIELD_LEN_LTM_EGRESS_ID      8
typedef struct dot1agCfmLTMEgressIdTLV_s
{
  uint16_t                   length;
  uint8_t                    egressId[DOT1AG_TLV_FIELD_LEN_LTM_EGRESS_ID];
} dot1agCfmLTMEgressIdTLV_t;

#define DOT1AG_TLV_FIELD_LEN_LTR_EGRESS_ID      8
typedef struct dot1agCfmLTREgressIdTLV_s
{
  uint16_t                   length;
  uint8_t                    lastEgressId[DOT1AG_TLV_FIELD_LEN_LTM_EGRESS_ID];
  uint8_t                    nextEgressId[DOT1AG_TLV_FIELD_LEN_LTM_EGRESS_ID];
} dot1agCfmLTREgressIdTLV_t;

#define DOT1AG_TLV_FIELD_LEN_REPLY_INGRESS_ACTION          1
#define DOT1AG_TLV_FIELD_LEN_REPLY_INGRESS_PORT_ID_LEN     1
#define DOT1AG_TLV_FIELD_LEN_REPLY_INGRESS_PORT_ID_SUBTYPE 1
#define DOT1AG_TLV_FIELD_LEN_INGRESS_PORT_ID               255
#define DOT1AG_TLV_LEN_MIN_PORT_ID_PARAM_PRESENT           8
typedef struct dot1agCfmReplyIngressIdTLV_s
{
  uint16_t                   length;
  uint8_t                    ingressAction;
  uint8_t                    ingressMacAdrs[OFDPA_MAC_ADDR_LEN];
  uint8_t                    ingressPortIdLength;
  uint8_t                    ingressPortIdSubType;
  uint8_t                    ingressPortId[DOT1AG_TLV_FIELD_LEN_INGRESS_PORT_ID];
} dot1agCfmReplyIngressTLV_t;

#define DOT1AG_TLV_FIELD_LEN_REPLY_EGRESS_ACTION          1
#define DOT1AG_TLV_FIELD_LEN_REPLY_EGRESS_PORT_ID_LEN     1
#define DOT1AG_TLV_FIELD_LEN_REPLY_EGRESS_PORT_ID_SUBTYPE 1
#define DOT1AG_TLV_FIELD_LEN_EGRESS_PORT_ID               255
#define DOT1AG_TLV_LEN_MIN_PORT_ID_PARAM_PRESENT          8
typedef struct dot1agCfmReplyEgressIdTLV_s
{
  uint16_t                   length;
  uint8_t                    egressAction;
  uint8_t                    egressMacAdrs[OFDPA_MAC_ADDR_LEN];
  uint8_t                    egressPortIdLength;
  uint8_t                    egressPortIdSubType;
  uint8_t                    egressPortId[DOT1AG_TLV_FIELD_LEN_EGRESS_PORT_ID];
} dot1agCfmReplyEgressTLV_t;

#define DOT1AG_TLV_FIELD_LEN_TARGET_MEP     25
typedef struct dot1agCfmTargetMepTLV_s
{
  uint16_t                   length;
  uint8_t                    subType;
  uint16_t                   MepId;
} dot1agCfmTargetMepTLV_t;
#define DOT1AG_TLV_FIELD_ITUT_CARRIER_CODE  6
#define DOT1AG_TLV_FIELD_NODE_ID            4
#define DOT1AG_TLV_FIELD_IF_NUM             4
typedef struct dot1agCfmTargetMipTLV_s
{
  uint16_t                   length;
  uint8_t                    subType;
  uint8_t                    ICC[DOT1AG_TLV_FIELD_ITUT_CARRIER_CODE];
  uint8_t                    NodeId[DOT1AG_TLV_FIELD_NODE_ID];
  uint8_t                    IfNum[DOT1AG_TLV_FIELD_IF_NUM];
} dot1agCfmTargetMipTLV_t;

#define DOT1AG_TLV_FIELD_MEG_ID             48
typedef struct dot1agCfmRequestMepTLV_s
{
  uint16_t                   length;
  uint8_t                    subType;
  uint8_t                    LbInd;
  uint16_t                   MepId;
  uint8_t                    MegId[DOT1AG_TLV_FIELD_MEG_ID];
} dot1agCfmRequestMepTLV_t;

typedef struct dot1agCfmTestTLV_s
{
  uint16_t                     length;
  OFDPA_OAM_LB_TEST_TLV_TYPE_t patternType;
} dot1agCfmTestTLV_t;

typedef struct dot1agCfmDataTLV_s
{
  uint16_t                     length;
} dot1agCfmDataTLV_t;

#endif
