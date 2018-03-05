/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2005-2015
*
**********************************************************************
* @filename  g8131_pdu.h
*
* @purpose   G8131 Configuration file
*
* @component g8131
*
* @comments
*
* @create    1-NOV-2013
*
* @author    
*
* @end
*
**********************************************************************/

#ifndef INCLUDE_G8131_PDU_H
#define INCLUDE_G8131_PDU_H

#include "g8131.h"

#define  OFDPA_G8131_MAX_FRAME_SIZE  1522

#define G8131_PUT_BYTE(val, cp) (*(cp)++ = (uint8_t)(val))
#define G8131_GET_BYTE(val, cp) ((val) = *(uint8_t *)(cp)++)

#define G8131_PUT_SHORT(val, cp) \
  do { \
       (val) = htons((val));\
       memcpy((cp), &(val), sizeof(uint16_t));\
       (cp) += sizeof(uint16_t); \
     } while (0)

typedef struct g8131CommonHeader
{
  uint8_t level;
  uint8_t version;
  uint8_t opcode;
  uint8_t flags;
  uint8_t first_tlv_offset;
} g8131CommonHdr_t;

typedef struct g8131Payload
{
  uint8_t request;
  uint8_t type;
  uint8_t reqSignal;
  uint8_t brgSignal;
  uint8_t reserved;
} g8131Payload_t;

typedef struct g8131Pkt_s
{
  uint32_t           intIfNum;         /* interface that received the packet  */
  uint32_t           vlanId;
  uint32_t           innerVlanId;
  uint8_t            destMac[OFDPA_MAC_ADDR_LEN];
  uint8_t            srcMac[OFDPA_MAC_ADDR_LEN];
  uint8_t           *g8131PduOffset; /*PDU offset*/
  g8131CommonHdr_t   g8131Hdr;
  uint8_t            payLoad[OFDPA_G8131_MAX_FRAME_SIZE];
  uint32_t           length;           /* payload length */
  OFDPA_BOOL         toFwd;
} g8131ControlPkt_t;

typedef enum g8131RemoteRequest_s
{
  G8131_APS_FIELD_REQUEST_NR   = 0,
  G8131_APS_FIELD_REQUEST_DNR  = 1,
  G8131_APS_FIELD_REQUEST_RR   = 2,
  G8131_APS_FIELD_REQUEST_RSV3 = 3,
  G8131_APS_FIELD_REQUEST_EXER = 4,
  G8131_APS_FIELD_REQUEST_WTR  = 5,
  G8131_APS_FIELD_REQUEST_DEP  = 6, /*never used?*/
  G8131_APS_FIELD_REQUEST_MS   = 7,
  G8131_APS_FIELD_REQUEST_RSV8 = 8,
  G8131_APS_FIELD_REQUEST_SD   = 9,
  G8131_APS_FIELD_REQUEST_RSV10= 10,
  G8131_APS_FIELD_REQUEST_SF_W = 11,
  G8131_APS_FIELD_REQUEST_RSV4 = 12,
  G8131_APS_FIELD_REQUEST_FS   = 13,
  G8131_APS_FIELD_REQUEST_SF_P = 14,
  G8131_APS_FIELD_REQUEST_LO   = 15
} g8131APSRequest_t;

#define G8131_PROT_TYPE_NO_APS_CHANNEL   0
#define G8131_PROT_TYPE_APS_CHANNEL      1

#define G8131_PROT_TYPE_PERMANENT_BRG    0
#define G8131_PROT_TYPE_NO_PERMANENT_BRG 1

#define G8131_PROT_TYPE_UNI_DIR_SWITCH   0
#define G8131_PROT_TYPE_BI_DIR_SWITCH    1

#define G8131_PROT_TYPE_NON_REVERT_OPER  0
#define G8131_PROT_TYPE_REVERT_OPER      1

#define G8131_SIGNAL_NULL   0
#define G8131_SIGNAL_NORMAL 1

/* Ethernet MAC Address */
typedef struct g8131_mac_addr_s
{
  uint8_t  addr[OFDPA_MAC_ADDR_LEN];
}g8131_enetMacAddr_t;

/* Link Layer Address Description */
typedef struct g8131_linkLayerAddr_s
{
  uint8_t  type;      
  uint8_t  len;       /* length of address */
  union
  {
    g8131_enetMacAddr_t  enetAddr;
  } addr;
} g8131_linkLayerAddr_t;

/* Ethernet Header */
typedef struct g8131_enetHeader_s
{
  g8131_enetMacAddr_t    dest;
  g8131_enetMacAddr_t    src;
} g8131_enetHeader_t;

/* Another Ethernet header, but with an ethertype field. Note that the
 * ethertype field can either be an ethertype (Eth II) or a length (802.3). */
typedef struct g8131_ethHeader_s
{
  g8131_enetMacAddr_t  dest;
  g8131_enetMacAddr_t  src;
  uint16_t  ethertype;   /* type or length */
} g8131_ethHeader_t;

#define G8131_ENET_HDR_SIZE      sizeof(g8131_enetHeader_t)
#define G8131_ETYPE_8021Q        0x8100
#define G8131_ETYPE_CFM          0x8902
#define G8131_VLAN_TAG_VLAN_ID_MASK 0x0FFF

OFDPA_ERROR_t g8131PduXmitRequest(uint32_t grpId, uint32_t request);

OFDPA_ERROR_t g8131PduRcvFromDot1ag(uint32_t intIfNum, uint32_t vlanId, uint32_t innerVlanId,
                                    uint8_t *g8131Pkt, uint32_t dataLength, uint32_t cfmPduOffset);

OFDPA_ERROR_t g8131PduRcvHandler(uint32_t intIfNum, uint32_t vlanId, uint32_t innerVlanId,
                                 uint8_t *g8131PktBuffer, uint32_t dataLength, uint32_t cfmPduOffset);
#endif

