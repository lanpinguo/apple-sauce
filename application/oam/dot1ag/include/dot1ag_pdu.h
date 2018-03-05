/*********************************************************************
 *
 * (C) Copyright Broadcom Corporation 2000-2015
 *
 **********************************************************************
 * @filename  dot1ag_pdu.h
 *
 * @purpose   Header file for Global Data of PDUs Dot1ag/CFM
 *
 * @component DOT1AG
 *
 * @comments  none
 *
 * @create    02/16/2009
 *
 * @end
 **********************************************************************/
#ifndef INCLUDE_DOT1AG_PDU_H
#define INCLUDE_DOT1AG_PDU_H

#include "ofdb_api.h"
#include "dot1ag.h"
//#include "driver_oam.h"

#define DOT1AG_ETH_TYPE_LEN 2

/* CFM PDU Data Structures */

/* Destination MAC Address for CFM PDU */
#define CFM_GET_CCM_DEST_MAC(D, L)                                            \
     do {                                                                     \
          (D)[0] = 0x01;                                                      \
          (D)[1] = 0x80;                                                      \
          (D)[2] = 0xC2;                                                      \
          (D)[3] = 0x11;                                                      \
          (D)[4] = 0x11;                                                      \
          (D)[5] = (0x10 | (L));                                              \
        } while (0)

#define CFM_GET_LTM_DEST_MAC(D, L)                                            \
     do {                                                                     \
          (D)[0] = 0x01;                                                      \
          (D)[1] = 0x80;                                                      \
          (D)[2] = 0xC2;                                                      \
          (D)[3] = 0x11;                                                      \
          (D)[4] = 0x11;                                                      \
          (D)[5] = (0x10 | ((L) + 7));                                        \
        } while (0)

#define OFDPA_DOT1AG_MPLS_ETHER_TYPE               0x8847
#define OFDPA_DOT1AG_CFM_ETHER_TYPE                0x8902
#define OFDPA_DOT1AG_8021Q_ETHER_TYPE              0x8100

#define OFDPA_DOT1AG_MAX_CFM_PDU_LEN               100
#define DOT1AG_CFM_PROTOCOL_VERSION_0                0
#define DOT1AG_CFM_PROTOCOL_VERSION_1                1
#define OFDPA_DOT1AG_CFM_VERSION_BITS              5

/* OPCODE For CFM Messages 802.1ag d8-2 21.4.3 */
#define DOT1AG_IEEE_8021_OPCODE              0x00
#define DOT1AG_CONTINUITY_CHECK_OPCODE       0x01 /* CCM */
#define DOT1AG_LOOPBACK_REPLY_OPCODE         0x02 /* LBR */
#define DOT1AG_LOOPBACK_MESSAGE_OPCODE       0x03 /* LBM */
#define DOT1AG_LINKTRACE_REPLY_OPCODE        0x04 /* LTR */
#define DOT1AG_LINKTRACE_MESSAGE_OPCODE      0x05 /* LTM */
#define DOT1AG_AIS_CHECK_OPCODE              0x21 /* AIS */
#define DOT1AG_LCK_CHECK_OPCODE              0x23 /* LCK */
#define DOT1AG_CSF_CHECK_OPCODE              0x34 /* CSF */
#define DOT1AG_LOSSMEASUREMENT_MESSAGE_OPCODE       43 /* LMM */
#define DOT1AG_LOSSMEASUREMENT_REPLY_OPCODE         42 /* LMR */
#define DOT1AG_DELAYMEASUREMENT_MESSAGE_OPCODE      47 /* DMM */
#define DOT1AG_DELAYMEASUREMENT_REPLY_OPCODE        46 /* DMR */
#define DOT1AG_SYNTHETIC_LOSSMEASUREMENT_MESSAGE_OPCODE       55 /* SLM */
#define DOT1AG_SYNTHETIC_LOSSMEASUREMENT_REPLY_OPCODE         54 /* SLR */

#define DOT1AG_OPCODE_INVALID(P)                                 \
        ((P) != DOT1AG_CONTINUITY_CHECK_OPCODE &&                \
         (P) != DOT1AG_LOOPBACK_REPLY_OPCODE &&                  \
         (P) != DOT1AG_LOOPBACK_MESSAGE_OPCODE &&                \
         (P) != DOT1AG_LINKTRACE_REPLY_OPCODE &&                 \
         (P) != DOT1AG_LOSSMEASUREMENT_MESSAGE_OPCODE &&         \
         (P) != DOT1AG_LOSSMEASUREMENT_REPLY_OPCODE &&           \
         (P) != DOT1AG_DELAYMEASUREMENT_MESSAGE_OPCODE &&        \
         (P) != DOT1AG_DELAYMEASUREMENT_REPLY_OPCODE &&          \
         (P) != DOT1AG_LINKTRACE_MESSAGE_OPCODE)

#define OFDPA_DOT1AG_CFM_MIN_MD_LEVEL             0
#define OFDPA_DOT1AG_CFM_MAX_MD_LEVEL             7

/* Flags for CFM Messages */
#define OFDPA_DOT1AG_CFM_LTM_FLAG_HW_ONLY         (1 << 7)
#define OFDPA_DOT1AG_CFM_LTM_FORWARD_YES          (1 << 7)

typedef enum dot1agCfmLmmOperationType_e
{
  DOT1AG_CFM_LMM_OPERATION_TYPE_ONDEMAND,
  DOT1AG_CFM_LMM_OPERATION_TYPE_PROACTIVE
} Dot1agCfmLmmOperationType_t;

#define DOT1AG_CFM_LMM_OPERATION_TYPE_GET(F)      ((F) & DOT1AG_CFM_LMM_OPERATION_TYPE_PROACTIVE)

typedef enum dot1agCfmDmmOperationType_e
{
  DOT1AG_CFM_DMM_OPERATION_TYPE_ONDEMAND,
  DOT1AG_CFM_DMM_OPERATION_TYPE_PROACTIVE
} Dot1agCfmDmmOperationType_t;

#define DOT1AG_CFM_DMM_OPERATION_TYPE_GET(F)      ((F) & DOT1AG_CFM_DMM_OPERATION_TYPE_PROACTIVE)

#define DOT1AG_ETH_HDR_LEN                      14

#define DOT1AG_CFM_COMMON_HDR_LEN               4

/* First TLV Offset Values */
#define DOT1AG_CFM_CCM_FIRST_TLV_OFFSET     70
#define DOT1AG_CFM_LBM_FIRST_TLV_OFFSET     4
#define DOT1AG_CFM_LBR_FIRST_TLV_OFFSET     4
#define DOT1AG_CFM_LTM_FIRST_TLV_OFFSET     17
#define DOT1AG_CFM_LTR_FIRST_TLV_OFFSET     6
#define DOT1AG_CFM_AIS_FIRST_TLV_OFFSET     0
#define DOT1AG_CFM_LCK_FIRST_TLV_OFFSET     0
#define DOT1AG_CFM_CSF_FIRST_TLV_OFFSET     0
#define DOT1AG_CFM_LMM_FIRST_TLV_OFFSET     12
#define DOT1AG_CFM_LMR_FIRST_TLV_OFFSET     12
#define DOT1AG_CFM_DMM_FIRST_TLV_OFFSET     32
#define DOT1AG_CFM_DMR_FIRST_TLV_OFFSET     32
#define DOT1AG_CFM_SLM_FIRST_TLV_OFFSET     16
#define DOT1AG_CFM_SLR_FIRST_TLV_OFFSET     16

#define  OFDPA_DOT1AG_MAX_FRAME_SIZE  1522
//#define  OFDPA_DOT1AG_CFM_PDU_COUNT   (2*L7_MAX_PORT_COUNT)
//#define  OFDPA_DOT1AG_CFM_PDU_COUNT   (2*OFDPA_MAX_INTERFACE_COUNT)
#define  OFDPA_DOT1AG_CFM_LTR_QUEUE_COUNT 32

typedef enum dot1agCfmXmitType_e
{
  DOT1AG_NORMAL_UNICAST,
  DOT1AG_VLAN_MULTICAST,
  DOT1AG_L2RAW_UNICAST,
  DOT1AG_L2RAW_VLAN_MULTICAST,
  DOT1AG_NUM_OF_FRAME_TYPE   /*always the last in the enum*/
} Dot1agCfmXmitType_t;

/* CFM Common Header */
typedef struct cfm_header
{
  uint8_t level;
#define DOT1AG_CFM_HDR_GET_LEVEL(l)        ((l) >> 5)
  uint8_t version;
#define DOT1AG_CFM_HDR_GET_VERSION(v)      ((v) & 0x1F)
  uint8_t opcode;
  uint8_t flags;
  uint8_t first_tlv_offset;
} dot1agCfmCommonHdr_t;

/* 802.1ag d8-1 21.6 */
typedef struct dot1agCfmCcmPdu_s
{
  uint32_t    seqNumber;
  uint16_t    mepID;
  uint8_t     maID[DOT1AG_MAID_MAX_LEN];
  uint8_t     itutRsvd[DOT1AG_CCM_ITUT_RSVD_LEN];
  uint8_t    *pFirstTLV;                /* Should point to DOT1AG_CCM_FIRST_TLV_OFFSET+5 */
  OFDPA_BOOL  rdi;                      /* OFDPA_FALSE or OFDPA_TRUE */
  uint8_t     ccmInterval;              /* 0-7 */

  /* PROLM Data */
  uint32_t txFCf;
  uint32_t rxFCb;
  uint32_t txFCb;
  uint32_t rxFCl;                       /* set by HW */

  /* Sender ID TLV Data */
  OFDPA_BOOL                     senderIdTLVRead;
  dot1agCfmSenderIdTLV_t         senderIdTLV;
  /* PortStatus TLV Data */
  OFDPA_BOOL                     portStatusTLVRead;
  dot1agCfmPortStatusTLV_t       portStatusTLV;
  /* Interface TLV Data */
  OFDPA_BOOL                     interfaceStatusTLVRead;
  dot1agCfmInterfaceStatusTLV_t  interfaceStatusTLV;
  /* RX COUNTER TLV */
  OFDPA_BOOL                     rxCtrTLVRead;
  uint8_t                       *pRxCtrTLV;
  /* Org TLV */

} dot1agCfmCcmPdu_t;

typedef struct dot1agCfmLbmPdu_s
{
  uint32_t                lbmTransId;
  uint8_t                *pFirstTLV;    /* Should point to DOT1AG_LBM_FIRST_TLV_OFFSET+5 */
  /* Sender ID TLV */
  OFDPA_BOOL              senderIdTLVRead;
  dot1agCfmSenderIdTLV_t  senderIdTLV;
  /* Data TLV */
  /* Org TLV */
} dot1agCfmLbmPdu_t;

typedef struct dot1agCfmLbrPdu_s
{
  uint32_t                lbrTransId;
  uint8_t                *pFirstTLV;    /* Should point to DOT1AG_LBR_FIRST_TLV_OFFSET+5 */
  /* Sender ID TLV */
  OFDPA_BOOL              senderIdTLVRead;
  dot1agCfmSenderIdTLV_t  senderIdTLV;
  /* Data TLV */
  /* Org TLV */

} dot1agCfmLbrPdu_t;

typedef struct dot1agCfmLtrPdu_s
{
  uint32_t ltrTransId;
  uint8_t  ttl;
  uint8_t  relayAction;

  OFDPA_BOOL                  useFDBOnly;
  OFDPA_BOOL                  fwdYes;
  OFDPA_BOOL                  terminalMEP;
  uint8_t                    *pFirstTLV; /* Should point to DOT1AG_LTR_FIRST_TLV_OFFSET+5 */
  /* LTR Egress TLV */
  OFDPA_BOOL                  ltrEgressIdTLVRead;
  dot1agCfmLTREgressIdTLV_t   ltrEgressIdTLV;
  /* Reply Ingress TLV */
  OFDPA_BOOL                  replyIngressTLVRead;
  dot1agCfmReplyIngressTLV_t  replyIngressTLV;
  /* Reply Egress TLV */
  OFDPA_BOOL                  replyEgressTLVRead;
  dot1agCfmReplyEgressTLV_t   replyEgressTLV;
  /* Sender ID TLV */
  OFDPA_BOOL                  senderIdTLVRead;
  dot1agCfmSenderIdTLV_t      senderIdTLV;
  /* Org TLV */
} dot1agCfmLtrPdu_t;

typedef struct dot1agCfmLtmPdu_s
{
  uint32_t                   ltmTransId;
  uint8_t                    ttl;
  uint8_t                    orgMacAddr[OFDPA_MAC_ADDR_LEN];
  uint8_t                    targetMacAddr[OFDPA_MAC_ADDR_LEN];
  OFDPA_BOOL                 useFDBOnly;
  uint8_t                   *pFirstTLV; /* Should point to DOT1AG_LTM_FIRST_TLV_OFFSET+5 */
  uint8_t                   *pLastTLV;
  /* LTM Egress Identifier TLV */
  OFDPA_BOOL                 ltmEgressIdTLVRead;
  uint8_t                   *pltmEgressIdTLV;
  dot1agCfmLTMEgressIdTLV_t  ltmEgressIdTLV;
  /* Sender ID TLV */
  OFDPA_BOOL                 senderIdTLVRead;
  uint8_t                   *pSenderIdTLV;
  dot1agCfmSenderIdTLV_t     senderIdTLV;
  /* Org TLV */

} dot1agCfmLtmPdu_t;

typedef struct dot1agCfmLmmPdu_s
{
  OFDPA_BOOL              lmmType;
  uint32_t                txFCf;
  uint32_t                rxFCl;        /* set by HW */
  uint8_t                *pFirstTLV;    /* Should point to DOT1AG_LBM_FIRST_TLV_OFFSET+5 */
  /* Sender ID TLV */
  OFDPA_BOOL              senderIdTLVRead;
  dot1agCfmSenderIdTLV_t  senderIdTLV;
  uint8_t                *pSenderIdTLV;
  /* RX COUNTER TLV */
  OFDPA_BOOL              rxCtrTLVRead;
  uint8_t                *pRxCtrTLV;
  /* Data TLV */
  /* Org TLV */
} dot1agCfmLmmPdu_t;

typedef struct dot1agCfmLmrPdu_s
{
  uint32_t                flags;
  uint32_t                txFCf;
  uint32_t                rxFCf;
  uint32_t                txFCb;
  uint32_t                rxFCl;        /* set by HW */
  uint8_t                *pFirstTLV;    /* Should point to DOT1AG_LBM_FIRST_TLV_OFFSET+5 */
  /* Sender ID TLV */
  OFDPA_BOOL              senderIdTLVRead;
  dot1agCfmSenderIdTLV_t  senderIdTLV;
  uint8_t                *pSenderIdTLV;
  /* RX COUNTER TLV */
  OFDPA_BOOL              rxCtrTLVRead;
  uint8_t                *pRxCtrTLV;
  /* Data TLV */
  /* Org TLV */
} dot1agCfmLmrPdu_t;

typedef struct dot1agCfmDmmPdu_s
{
  OFDPA_BOOL              dmmType;
  y1731DmTs_t             txTimeStampf;
  y1731DmTs_t             rxTimeStampf; /* set by HW */
  uint8_t                *pFirstTLV;    /* Should point to DOT1AG_LBM_FIRST_TLV_OFFSET+5 */
  /* Sender ID TLV */
  OFDPA_BOOL              senderIdTLVRead;
  dot1agCfmSenderIdTLV_t  senderIdTLV;
  uint8_t                *pSenderIdTLV;
  /* RX TIMESTAMP TLV */
  OFDPA_BOOL              rxTsTLVRead;
  uint8_t                *pRxTsTLV;
  /* Test ID TLV */
  /* Data TLV */
  /* Org TLV */
} dot1agCfmDmmPdu_t;

typedef struct dot1agCfmDmrPdu_s
{
  uint32_t                flags;
  y1731DmTs_t             txTimeStampf;
  y1731DmTs_t             rxTimeStampf;
  y1731DmTs_t             txTimeStampb;
  y1731DmTs_t             rxTimeb;      /* set by HW */
  uint8_t                *pFirstTLV;    /* Should point to DOT1AG_LBM_FIRST_TLV_OFFSET+5 */
  /* Sender ID TLV */
  OFDPA_BOOL              senderIdTLVRead;
  dot1agCfmSenderIdTLV_t  senderIdTLV;
  uint8_t                *pSenderIdTLV;
  /* RX TIMESTAMP TLV */
  OFDPA_BOOL              rxTsTLVRead;
  uint8_t                *pRxTsTLV;
  /* Test ID TLV */
  /* Data TLV */
  /* Org TLV */
} dot1agCfmDmrPdu_t;

typedef struct dot1agCfmSlmPdu_s
{
  OFDPA_BOOL              slmType;
  uint16_t                srcMepId;
  uint32_t                testId;
  uint32_t                txFCf;
  uint32_t                rxFCl;        /* set by HW */
  uint8_t                *pFirstTLV;    /* Should point to DOT1AG_LBM_FIRST_TLV_OFFSET+5 */
  /* Sender ID TLV */
  OFDPA_BOOL              senderIdTLVRead;
  dot1agCfmSenderIdTLV_t  senderIdTLV;
  uint8_t                *pSenderIdTLV;
  /* RX COUNTER TLV */
  OFDPA_BOOL              rxCtrTLVRead;
  uint8_t                *pRxCtrTLV;
  /* Data TLV */
  /* Org TLV */
} dot1agCfmSlmPdu_t;

typedef struct dot1agCfmSlrPdu_s
{
  uint32_t                flags;
  uint16_t                srcMepId;
  uint16_t                rspMepId;
  uint32_t                testId;
  uint32_t                txFCf;
  uint32_t                rxFCf;
  uint32_t                txFCb;
  uint32_t                rxFCl;        /* set by HW */
  uint8_t                *pFirstTLV;    /* Should point to DOT1AG_LBM_FIRST_TLV_OFFSET+5 */
  /* Sender ID TLV */
  OFDPA_BOOL              senderIdTLVRead;
  dot1agCfmSenderIdTLV_t  senderIdTLV;
  uint8_t                *pSenderIdTLV;
  /* RX COUNTER TLV */
  OFDPA_BOOL              rxCtrTLVRead;
  uint8_t                *pRxCtrTLV;
  /* Data TLV */
  /* Org TLV */
} dot1agCfmSlrPdu_t;

/* Place Holder for the CFM PDU */
typedef struct dot1agControlPkt_s
{
  uint32_t              intIfNum;       /* interface that received the packet  */
  uint32_t              vlanId;
  uint32_t              innerVlanId;
  uint8_t               priority;
  uint8_t               destMac[OFDPA_MAC_ADDR_LEN];
  uint8_t               srcMac[OFDPA_MAC_ADDR_LEN];
  OFDPA_BOOL            fromWire;       /* OFDPA_TRUE - Rx from wire, OFDPA_FALSE - Rx from Relay */
  OFDPA_BOOL            fromOwnUpMEP;   /* OFDPA_TRUE - for LTM generated by UP MEP */
  uint8_t              *cfmPduOffset;   /* Offset for CFM PDU from PayLoad */
  dot1agCfmCommonHdr_t  cfmHdr;         /* CfmHdr extracted from a given Payload */
  union
  {
    dot1agCfmCcmPdu_t ccm;
    dot1agCfmLbmPdu_t lbm;
    dot1agCfmLbrPdu_t lbr;
    dot1agCfmLtmPdu_t ltm;
    dot1agCfmLtrPdu_t ltr;
    dot1agCfmLmmPdu_t lmm;
    dot1agCfmLmrPdu_t lmr;
    dot1agCfmDmmPdu_t dmm;
    dot1agCfmDmrPdu_t dmr;
    dot1agCfmSlmPdu_t slm;
    dot1agCfmSlrPdu_t slr;
  } cfmPdu;
  uint8_t               payLoad[OFDPA_DOT1AG_MAX_FRAME_SIZE];
  uint32_t              length;         /* payload length */
  OFDPA_BOOL            toFwd;
  uint32_t              timestamp;                   /* For time sync or OAM DM */
  uint32_t              timestamp_upper;             /* For OAM DM */
  uint32_t              lmepId;                      /* For OAM */
  uint8_t               oamOpcode;                   /* For OAM Opcode */
  uint8_t               oamReplacementOffset;        /* For OAM LM and DM */
  uint8_t               cwPresent;                   /* For MPLS (OAM), non-zero if CW present */
  uint32_t              megIndex;                    /* For OAM */
  uint32_t              outPort;                     /* For OAM Up MEPs */
} dot1agControlPkt_t;

/* Dot1ag PDU Message format */
typedef struct dot1agPduMsg_s
{
  void *dot1agBuffer;         /* Pointer to the received PDU */
  OFDPA_BOOL      fromWire;             /* Flag to indicate if the pkt is from wire
                                           side or bridge relay side */
  OFDPA_BOOL      fromOwnUpMEP;         /* OFDPA_TRUE - for LTM generated by UP MEP */
} dot1agPduMsg_t;

typedef struct dot1agCfmLtrPduMsg_s
{
  uint32_t           intIfNum;          /* Interface on which LTR is to generated */
  uint32_t           vlanId;            /* VLAN on which LTR is to be sent */
  uint32_t           length;            /* PDU length */
  dot1agControlPkt_t dot1agPkt;
  Dot1agCfmMPType_t  mpType;
} dot1agCfmLtrPduMsg_t;

/*Table 21-19. Maintenance Domain Name Format */
typedef enum md_format
{
  DOT1AG_MD_IEEE_802_1_RESERVED = 0,
  DOT1AG_NO_MD_PRESENT,
  DOT1AG_DNS_BASED_NAME,
  DOT1AG_MAC_ADDR_TWO_OCTET_INTEGER,
  DOT1AG_MD_CHAR_STRING
} dot1agCfmMDFormat_t;

/* 21.6.5.4 Short MA Name Format Table 21-20. */
typedef enum ma_format
{
  DOT1AG_MA_IEEE_802_1_RESERVED = 0,
  DOT1AG_MA_PRIMARY_VID,
  DOT1AG_MA_CHAR_STRING,
  DOT1AG_TWO_OCTET_INTEGER,
  DOT1AG_RFC_2685_VPN_ID
} dot1agCfmMAFormat_t;

/* Port Status TLV Values - 21.5.4 */
typedef enum dot1agCfmPortStatusTlvVal_e
{
  DOT1AG_PS_NO_TLV = 0,
  DOT1AG_PS_BLOCKED = 1,
  DOT1AG_PS_UP
} Dot1agCfmPSTlvVal_t;

/* Interface Status TLV - 21.5.5 */
typedef enum dot1agCfmIntfStatusTlvVal_e
{
  DOT1AG_INTF_STATUS_NO_TLV = 0,
  DOT1AG_INTF_STATUS_TLV_UP = 1,
  DOT1AG_INTF_STATUS_TLV_DOWN,
  DOT1AG_INTF_STATUS_TLV_TESTING,
  DOT1AG_INTF_STATUS_TLV_UNKNOWN,
  DOT1AG_INTF_STATUS_TLV_DORMANT,
  DOT1AG_INTF_STATUS_TLV_NOT_PRESENT,
  DOT1AG_INTF_STATUS_TLV_LOWER_LAYER_DOWN
} Dot1agCfmIntfStatTlvVal_t;

typedef enum dot1agCfmCsfTypeVal_e
{
  DOT1AF_CSF_TYPE_LOS = 0,
  DOT1AF_CSF_TYPE_AIS = 1,
  DOT1AF_CSF_TYPE_RDI = 2,
  DOT1AF_CSF_TYPE_DCI = 3,
  DOT1AF_CSF_TYPE_INVALID
} Dot1agCfmCsfTypeVal_t;

#define OFDPA_DOT1AG_MD_MAX_NAME_LENGTH 27
#define OFDPA_DOT1AG_MA_MAX_NAME_LENGTH 27

typedef struct ccm_maid_field_s
{
  uint8_t  mdNameFormat;
  uint8_t  mdNameLength;
  uint8_t  mdName[OFDPA_DOT1AG_MD_MAX_NAME_LENGTH];
  uint8_t  maNameFormat;
  uint8_t  maNameLength;
  uint8_t  maName[OFDPA_DOT1AG_MA_MAX_NAME_LENGTH];
} dot1agCfmCcmMaidField_t;

typedef struct dot1agCcmFlags_s
{
  uint8_t rdi:1,             /* 1 - transmitting MEP.s presentRDI variable is set */
          reserved:4,
          ccmInterval:3;     /* 1-7 (10.83 msecs - 32.5 mins) */
} dot1agCcmFlags_t;

typedef struct dot1agLtmFlags_s
{
  uint8_t useFdbonly:1,      /* 1 - Use MAC addresses in FDB */
          reserved:7;
} dot1agLtmFlags_t;

typedef struct dot1agLtrFlags_s
{
  uint8_t useFdbonly:1,      /* copied from LTM */
          fwdYes:1,          /* LTM was or was not forwarded */
          terminalMep:1,     /* MP reported in Reply Egress/Ingress TLV is a MEP */
          reserved:5;
} dot1agLtrFlags_t;

#define DOT1AG_CCM_PDU_LEN_MAX      128       /* MdLevel through EndTLV */

#define DOT1AG_CCM_MAX_TLV_SIZE     (DOT1AG_CCM_PDU_LEN_MAX -                 \
                                     (sizeof(dot1agCfmCommonHdr_t) +             \
                                      sizeof(uint32_t) + sizeof(uint16_t) +  \
                                      OFDPA_DOT1AG_MAID_MAX_LEN + 16))

typedef struct dot1agCcmFrame_s
{
  dot1agCfmCommonHdr_t hdr;
  uint32_t             seqNumber;
  uint16_t             mepId;           /* 1-8191 */
  uint8_t              maId[OFDPA_DOT1AG_MAID_MAX_LEN]; /* Pad unused bytes with Zero */
  uint8_t              itu_t_y_1731[16]; /* defined by ITU-T Recommendation Y.1731. Use zero for now */
  uint8_t              ccmTlv[100];     /* Optional TLVs */
  uint8_t              tlvOffset;       /* Indicates Optional TLVS, 0 -> No Operational TLVS */
} dot1agCfmCcmFrame_t;

typedef struct dot1agLbmLbrFrame_s
{
  dot1agCfmCommonHdr_t hdr;
  uint32_t             transactionId;
} dot1agCfmLbmLbrFrame_t;

typedef struct dot1agLtmFrame_s
{
  dot1agCfmCommonHdr_t hdr;
  uint32_t             transactionId;
  uint8_t              ttl;
  uint8_t              origMacAddress[OFDPA_MAC_ADDR_LEN];
  uint8_t              targetMacAddress[OFDPA_MAC_ADDR_LEN];
} dot1agCfmLtmFrame_t;

typedef struct dot1agLtrFrame_s
{
  dot1agCfmCommonHdr_t hdr;
  uint32_t             transactionId;
  uint8_t              ttl;
  uint8_t              relayAction;
} dot1agCfmLtrFrame_t;

/***************************************************************
  Utility routines
 ***************************************************************/
#define DOT1AG_GET_BYTE(val, cp) ((val) = *(uint8_t *)(cp)++)

#define DOT1AG_GET_LONG(val, cp)                \
  do {                                          \
    memcpy(&(val), (cp), sizeof(uint32_t));     \
    (val) = ntohl((val));                       \
    (cp) += sizeof(uint32_t);                   \
  } while (0)

#define DOT1AG_GET_64(val, cp)                  \
  do {                                          \
    memcpy(&(val), (cp), sizeof(uint64_t));     \
    (val) = dpaNtohll((val));                   \
    (cp) += sizeof(uint64_t);                   \
  } while (0)

#define DOT1AG_GET_SHORT(val, cp)               \
  do {                                          \
    memcpy(&(val), (cp), sizeof(uint16_t));     \
    (val) = ntohs((val));                       \
    (cp) += sizeof(uint16_t);                   \
  } while (0)

#define DOT1AG_GET_ADDR(addr, cp)               \
  do {                                          \
    memcpy((addr), (cp), sizeof(uint32_t));     \
    (*addr) = ntohl((*addr));                   \
    (cp) += sizeof(uint32_t);                   \
  } while (0)

#define DOT1AG_GET_ADDR6(addr, cp)              \
  do {                                          \
    register uint8_t *Xap;                      \
    register int i;                             \
    Xap = (uint8_t *)(addr);                    \
    for (i = 0; i < 16; i++)                    \
      *Xap++ = *(cp)++;                         \
  } while (0)

#define DOT1AG_PUT_BYTE(val, cp) (*(cp)++ = (uint8_t)(val))

#define DOT1AG_PUT_SHORT(val, cp)               \
  do {                                          \
    (val) = htons((val));                       \
    memcpy((cp), &(val), sizeof(uint16_t));     \
    (cp) += sizeof(uint16_t);                   \
  } while (0)

#define DOT1AG_PUT_LONG(val, cp)                \
  do {                                          \
    (val) = htonl((val));                       \
    memcpy((cp), &(val), sizeof(uint32_t));     \
    (cp) += sizeof(uint32_t);                   \
  } while (0)

#define DOT1AG_PUT_64(val, cp)                  \
  do {                                          \
    (val) = dpaHtonll((val));                   \
    memcpy((cp), &(val), sizeof(uint64_t));     \
    (cp) += sizeof(uint64_t);                   \
  } while (0)

#define DOT1AG_PUT_DATA(data, len, cp) (memcpy (cp, data, len), (cp) += (len))
#define DOT1AG_GET_DATA(data, len, cp) (memcpy (data, cp, len), (cp) += (len))

//driverOamPacketRc_t dot1agPduRecv(driverBuffer_t *buffer);
OFDPA_ERROR_t dot1agPduReceiveProcess(dot1agPduMsg_t *msg);
OFDPA_ERROR_t dot1agCfmPacketParse(dot1agControlPkt_t *dot1agPacket);
OFDPA_ERROR_t dot1agCfmPacketRelay(uint32_t intIfNum, dot1agControlPkt_t *dot1agPacket);
void  dot1agCfmUpMEPWireSideRxProcess(dot1agControlPkt_t *dot1agPacket,
                                      dot1agCfmStackObj_t *pMPObj);
void  dot1agCfmUpMEPRelaySideRxProcess(dot1agControlPkt_t *dot1agPacket,
                                       dot1agCfmStackObj_t *pMPObj);
void  dot1agCfmDownMEPWireSideRxProcess(dot1agControlPkt_t *dot1agPacket,
                                        dot1agCfmStackObj_t *pMPObj);
void  dot1agCfmDownMEPRelaySideRxProcess(dot1agControlPkt_t *dot1agPacket,
                                         dot1agCfmStackObj_t *pMPObj);
void  dot1agCfmMIPWireSideRxProcess(dot1agControlPkt_t *dot1agPacket,
                                    dot1agCfmStackObj_t *pMPObj);
void  dot1agCfmMIPRelaySideRxProcess(dot1agControlPkt_t *dot1agPacket,
                                     dot1agCfmStackObj_t *pMPObj);
OFDPA_BOOL dot1agCfmIsCommonHdrValid (dot1agCfmCommonHdr_t *pcfmHdr);
OFDPA_ERROR_t dot1agCfmCommonHeaderExtract(dot1agControlPkt_t *dot1agPacket);

OFDPA_ERROR_t dot1agPduTransmit(uint32_t intIfNum, uint16_t vlanId,
                          dot1agControlPkt_t *dot1agPkt);

void dot1agCfmMpOpcodeDemultiplexer(dot1agControlPkt_t *dot1agPacket,
                                    dot1agCfmStackObj_t *pMPObj);

void dot1agMEPContinuityCheckReceiver(dot1agControlPkt_t *dot1agPacket,
                                      dot1agCfmStackObj_t *pMPObj);
void dot1agMEPLoopBackInitiatorReceive(dot1agControlPkt_t *dot1agPacket,
                                       dot1agCfmStackObj_t *pMPObj);
void dot1agMPLoopBackResponder(dot1agControlPkt_t *dot1agPacket,
                               dot1agCfmStackObj_t *pMPObj);
void dot1agMHFLoopBackResponder(dot1agControlPkt_t *dot1agPacket,
                                dot1agCfmStackObj_t *pMPObj);
void dot1agMEPLinkTraceInitiator(dot1agControlPkt_t *dot1agPacket,
                                 dot1agCfmStackObj_t *pMPObj);
void dot1agLinkTraceResponder(dot1agControlPkt_t *dot1agPacket,
                              dot1agCfmStackObj_t *pMPObj);
OFDPA_BOOL dot1agCfmPduFwdCheck(dot1agControlPkt_t *dot1agPkt,
                             dot1agCfmStackObj_t *pMPObj);
void dot1agMEPLMMReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);
void dot1agMEPLMRReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);
void dot1agMEPDMMReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);
void dot1agMEPDMRReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);
void dot1agMEPAisReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);
void dot1agMEPLckReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);
void dot1agMEPCsfReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);

uint32_t mplsTpPrepareMplsHeader(ofdbOamLmepData_t *oamMapData, uint8_t priority, uint8_t *data, uint8_t ttl);
void mplsTpLabelPrepare(uint32_t label, uint8_t exp, uint8_t bos, uint8_t ttl, uint8_t *buffer);
void dot1agMEPSLMReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);
void dot1agMEPSLRReceive(dot1agControlPkt_t *dot1agPkt,
                         dot1agCfmStackObj_t *pMPObj);

OFDPA_ERROR_t dot1agBhhLabelMapMpInit(void);

#endif
