/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2016
*
**********************************************************************
*
* @filename     ofdb_flow_api.c
*
* @purpose      Openflow tables related to flow management.
*
* @component    OFDB
*
* @comments     none
*
* @create       03/22/2013
*
* @end
*
**********************************************************************/
#include <string.h>
#include <stdio.h>

#include "ofdb_api.h"
#include "ofdb.h"
#include "ofdpa_util.h"
#include "datapath.h"
#include "datapath_api.h"

#include "ofdpa_porting.h"
#include "dot1ag_api.h"

/********
******** Flow Tables
********/

/*********************************************************************
*
* @purpose  Combines MAC address match criterion with its mask and
*           stores the result.
*
* @param    match @b((input))  MAC address data
* @param    mask  @b((input))  MAC mask data
* @param    result  @b((output))  the data resulting from a byte-wise
*                                 ANDing of match and mask data
*
* @returns  none
*
* @end
*
*********************************************************************/
static void ofdbMacMaskApply(ofdpaMacAddr_t *match, ofdpaMacAddr_t *mask, ofdpaMacAddr_t *result)
{
  uint32_t i;

  for (i = 0; i < OFDPA_MAC_ADDR_LEN; i++)
  {
    result->addr[i] = match->addr[i] & mask->addr[i];
  }
}

/*********************************************************************
*
* @purpose  Compares two MAC addresses.
*
* @param    macA @b((input))  one MAC address
* @param    macB @b((input))  another MAC address
*
* @returns  0 if equal
*           -1 if macA less than macB
*           1 if macA greater than macB
*
* @end
*
*********************************************************************/
int ofdbMacCompare(ofdpaMacAddr_t *macA, ofdpaMacAddr_t *macB)
{
  return(memcmp(macA, macB, sizeof(ofdpaMacAddr_t)));
}

/*********************************************************************
*
* @purpose  Compares bit in two MAC addresses as indicated by a mask.
*
* @param    macA @b((input))  one MAC address
* @param    macB @b((input))  another MAC address
* @param    mask @b((input))  mask indicating which bits in the
*                             two addresses are compared, 1 in
*                             a bit position indicates that bit
*                             should be compared.
*
* @returns  0 if equal
*           -1 if macA less than macB
*           1 if macA greater than macB
*
* @end
*
*********************************************************************/
int ofdbMaskedMacCompare(ofdpaMacAddr_t *macA, ofdpaMacAddr_t *macB, ofdpaMacAddr_t *mask)
{
  ofdpaMacAddr_t maskedMacA, maskedMacB;

  ofdbMacMaskApply(macA, mask, &maskedMacA);
  ofdbMacMaskApply(macB, mask, &maskedMacB);

  return(ofdbMacCompare(&maskedMacA, &maskedMacB));
}

/*********************************************************************
*
* @purpose  Combines IPv6 address match criterion with its mask and
*           stores the result.
*
* @param    match @b((input))  IP address data
* @param    mask  @b((input))  IP mask data
* @param    result  @b((output))  the data resulting from a byte-wise
*                                 ANDing of match and mask data
*
* @returns  none
*
* @end
*
*********************************************************************/
static void ofdbIp6AddrMaskApply(struct in6_addr *match, struct in6_addr *mask, struct in6_addr *result)
{
  uint32_t i;

  for (i = 0; i < 4; i++)
  {
    result->s6_addr32[i] = match->s6_addr32[i] & mask->s6_addr32[i];
  }
}

int ofdbIpv4MaskValidate(in_addr_t *mask)
{
  uint32_t inverseMask = ~(*mask);

  /* verify that mask is contiguous */
  if ((inverseMask & (inverseMask + 1)) != 0)
    return 0;
  else
    return 1;
}

int ofdbIpv6MaskValidate(struct in6_addr *mask)
{
  int i = 0;
  uint32_t maskWord;
  uint32_t inverseMaskWord;

  /* scan from left until find long word that is not all ones */
  while ((i < 4) &&
         ((maskWord = ntohl(mask->s6_addr32[i])) == 0xffffffffu))
  {
    i++;
  }

  /* if all words are all ones, no need to go further */
  if (i != 4)
  {
    /* if current word not 0, test that it is valid mask */
    if (maskWord != 0)
    {
      inverseMaskWord = ~maskWord;
      if ((inverseMaskWord & (inverseMaskWord + 1)) != 0)
        return 0;

      i++;
    }

    if (i != 4)
    {
      /* scan remaining words for all 0 */
      while ((i < 4) &&
             ((maskWord = ntohl(mask->s6_addr32[i])) == 0))
      {
        i++;
      }
    }

    if (i != 4)
    {
      return 0;
    }
  }
  return 1;
}

/*********************************************************************
 * @purpose  Convert a 32-bit network mask to a length
 *
 * @param    mask        @b{(input)} Input netmask
 *
 * @returns  Length of mask or -1 if invalid
 *
 * @notes    Validation includes mask to be contiguous bits of 1
 *           return zero if mask is invalid
 *           11100100 - is invalid mask --->masklen = 0
 *           11100000 - is valid mask  -->masklen = 3
 *
 * @end
 *
 *********************************************************************/
int ofdbIpv4MaskLengthGet(in_addr_t mask)
{
  in_addr_t maskBit;
  int maskSize;
  int maskLen;
  int i;
  int zeroFound = 0;

  maskSize = sizeof(mask) << 3;
  maskBit  = 0x1 << (maskSize-1);       /* The highest order bit */
  maskLen  = 0;

  for (i = 0; i < maskSize; i++)
  {
    if (mask & maskBit)
    {
      if (zeroFound)
      {
        return -1;
      }
      maskLen++;
    }
    else
    {
      zeroFound = 1;
    }
    mask = mask << 1;
  }

  return maskLen;
}

/*********************************************************************
 * @purpose  Convert an IPv6 network mask to a length
 *
 * @param    inetAddr    @b{(input)} Input netmask
 *
 * @returns  Length of mask or -1 if invalid
 *
 * @notes
 *
 * @end
 *
 *********************************************************************/
int ofdbIpv6MaskLengthGet(struct in6_addr *mask)
{
  in_addr_t maskval  = 0;
  int       i        = 0;
  int       totalLen = 0;
  int       tempMaskLen;
  int       zeroFound = 0;

  if (NULL == mask)
  {
    return -1;
  }

  for (i = 0; i < sizeof(*mask); i += sizeof(maskval))
  {
    memcpy(&maskval, &mask->s6_addr[i], sizeof(maskval));
    maskval = ntohl(maskval);
    tempMaskLen = ofdbIpv4MaskLengthGet(maskval);
    if ((-1 == tempMaskLen) ||
        ((zeroFound   != 0) &&
         (tempMaskLen != 0)))
    {
      return -1;
    }
    else
    {
      totalLen += tempMaskLen;
      if (tempMaskLen != 32)
      {
        zeroFound = 1;
      }
    }
  }

  return totalLen;
}

uint32_t ofdbFlowTableSupported(OFDPA_FLOW_TABLE_ID_t tableId)
{
  uint32_t valid = 0;

  if (ofdbFlowTableMaxCountGet(tableId) != 0)
  {
    valid = 1;
  }

  return(valid);
}

uint32_t ofdbFlowIngressPortEntryValidate(ofdpaIngressPortFlowEntry_t *flowData)
{
  ofdpaIngressPortFlowMatch_t *matchCriteria;
  uint8_t validTypeFound = 0;
  uint8_t isNormalEthernetDSCPType = 0;
  uint8_t isNormalEthernetPCPType = 0;
  uint8_t isNormalEthernetType = 0;
  uint8_t isOverlayTunnelDSCPType = 0;
  uint8_t isOverlayTunnelPCPType = 0;
  uint8_t isOverlayTunnelType = 0;
  uint8_t isOAMPDUInjType = 0;

  /* check flow type */
  matchCriteria = &flowData->match_criteria;

  if ((matchCriteria->lmepIdMask == OFDPA_LMEP_ID_EXACT_MASK) &&
      (matchCriteria->lmepId != 0))
  {
    isOAMPDUInjType = 1;
    validTypeFound = 1;
  }
  else if (matchCriteria->inPortMask == OFDPA_INPORT_EXACT_MASK)
  {
    if (OFDB_PORT_TYPE(matchCriteria->inPort) == OFDPA_PORT_TYPE_PHYSICAL)
    {
      /* port validation */
      if (!ofdbPortIsValid(matchCriteria->inPort))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Port not valid. (inPort: 0x%x)\r\n",
                           matchCriteria->inPort);
        return(0);
      }

      /* for all Normal types, tunnelId must be exact match on 0 */
      if ((OFDPA_TUNNEL_ID_EXACT_MASK != matchCriteria->tunnelIdMask) ||
          (0 != matchCriteria->tunnelId))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Flows that match on physical ports must EXACT match tunnelId == 0. (inPort: 0x%x, tunnelId: 0x%x, tunnelIdMask: 0x%x)\r\n",
                           matchCriteria->inPort, matchCriteria->tunnelId, matchCriteria->tunnelIdMask);
        return(0);
      }

      if (OFDPA_ETHERTYPE_EXACT_MASK == matchCriteria->etherTypeMask)
      {
        /* Normal Ethernet DSCP type */
        if ((0x0800 == matchCriteria->etherType) || (0x86dd == matchCriteria->etherType))
        {
            isNormalEthernetDSCPType = 1;
            validTypeFound = 1;
        }
      }
      else if (OFDPA_ETHERTYPE_ALL_MASK == matchCriteria->etherTypeMask)
      {
        /* differentiate Normal Ethernet from Normal Ethernet PCP by whether qosIndexAction specified */

        /* Normal Ethernet type */
        if (0 == flowData->qosIndexAction)
        {
          isNormalEthernetType = 1;
        }
        /* Normal Ethernet PCP type */
        else
        {
          isNormalEthernetPCPType = 1;
        }

        validTypeFound = 1;
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid etherTypeMask value. (etherTypeMask: 0x%x)\r\n",
                           matchCriteria->etherTypeMask);

        /* fall through to where validTypeFound flag is checked so more information about flow can be printed */
      }
    }
    else if ((OFDB_PORT_TYPE(matchCriteria->inPort) == OFDPA_PORT_TYPE_LOGICAL_TUNNEL) &&
             (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN)))
    {
      /* all types specifying a Overlay Logical type port must exact match a valid Tunnel ID */
      if (matchCriteria->tunnelIdMask != OFDPA_TUNNEL_ID_EXACT_MASK)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Port type and tunnelIdMask combination not valid. (inPort: 0x%x, tunnelIdMask: 0x%x)\r\n",
                           matchCriteria->inPort, matchCriteria->tunnelIdMask);
        return(0);
      }

      if (OFDPA_ETHERTYPE_EXACT_MASK == matchCriteria->etherTypeMask)
      {
        /* Overlay Tunnel DSCP type */
        if ((0x0800 == matchCriteria->etherType) || (0x86dd == matchCriteria->etherType))
        {
          isOverlayTunnelDSCPType = 1;
          validTypeFound = 1;
        }
      }
      else if (OFDPA_ETHERTYPE_ALL_MASK == matchCriteria->etherTypeMask)
      {
        /* Overlay Tunnel PCP type */
        if (OFDPA_TUNNEL_ID_EXACT_MASK == matchCriteria->tunnelIdMask)
        {
          isOverlayTunnelPCPType = 1;
          validTypeFound = 1;
        }
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid etherTypeMask value. (etherTypeMask: 0x%x)\r\n",
                           matchCriteria->etherTypeMask);

        /* fall through to where validTypeFound flag is checked so more information about flow can be printed */
      }
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid port type.\r\n", 0);

      /* fall through to where validTypeFound flag is checked so more information about flow can be printed */
    }
  }
  else if (matchCriteria->inPortMask == OFDPA_INPORT_FIELD_MASK)
  {
    if (OFDPA_ETHERTYPE_ALL_MASK == matchCriteria->etherTypeMask)
    {
      /* Overlay Tunnel type */
      /*
       * NOTE: allowing tunnelIdMask to be OFDPA_TUNNEL_ID_TYPE_MASK preserves
       * backward compatibility to earlier TTP versions. Currently, only "exact"
       * match is listed in the TTP
       */
      if ((OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN)) &&
          ((OFDPA_TUNNEL_ID_EXACT_MASK == matchCriteria->tunnelIdMask) ||
           (OFDPA_TUNNEL_ID_TYPE_MASK == matchCriteria->tunnelIdMask)) &&
          (OFDPA_TUNNELID_TYPE_DATA_CENTER_OVERLAY == OFDB_TUNNELID_TYPE(matchCriteria->tunnelId)))
      {
        isOverlayTunnelType = 1;
        validTypeFound = 1;
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid tunnelId/tunnelIdMask combination.\r\n", 0);

        /* fall through to where validTypeFound flag is checked so more information about flow can be printed */
      }
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid etherTypeMask value.\r\n", 0);

      /* fall through to where validTypeFound flag is checked so more information about flow can be printed */
    }
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid port mask value.\r\n", 0);

    /* fall through to where validTypeFound flag is checked so more information about flow can be printed */
  }

  if (validTypeFound == 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "The match criteria are not a valid combination. "
                       "Values specified: inPort: 0x%x, inPortMask: 0x%x, "
                       "tunnelId: %d, tunnelIdMask: 0x%x, etherType: 0x%x, etherTypeMask: 0x%x\r\n",
                       matchCriteria->inPort, matchCriteria->inPortMask,
                       matchCriteria->tunnelId, matchCriteria->tunnelIdMask,
                       matchCriteria->etherType, matchCriteria->etherTypeMask);
    return(0);
  }

  /* Instructions validation */
  /* Goto Table validation */
  if (isNormalEthernetDSCPType &&
      (OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST != flowData->gotoTableId))
  {
     OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto table for Normal Ethernet DSCP type. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
     return(0);
  }

  if (isNormalEthernetPCPType &&
      (OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST != flowData->gotoTableId))
  {
     OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto table for Normal Ethernet PCP type. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
     return(0);
  }

  if (isNormalEthernetType &&
      (OFDPA_FLOW_TABLE_ID_VLAN != flowData->gotoTableId))
  {
     OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto table for Normal Ethernet type. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
     return(0);
  }

  if (isOverlayTunnelDSCPType &&
      (OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST != flowData->gotoTableId))
  {
     OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto table for Overlay Tunnel DSCP type. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
     return(0);
  }

  if (isOverlayTunnelPCPType &&
      (OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST != flowData->gotoTableId))
  {
     OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto table for Overlay Tunnel PCP type. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
     return(0);
  }

  if (isOverlayTunnelType &&
      (OFDPA_FLOW_TABLE_ID_BRIDGING != flowData->gotoTableId))
  {
     OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto table for Overlay Tunnel type. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
     return(0);
  }

  if (isOAMPDUInjType &&
      (OFDPA_FLOW_TABLE_ID_INJECTED_OAM != flowData->gotoTableId))
  {
     OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto table for OAM PDU Injection type. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
     return(0);
  }

  /* Apply actions */
  /* QoS Index action validation */
  if ((isNormalEthernetDSCPType || isNormalEthernetPCPType || isOverlayTunnelDSCPType || isOverlayTunnelPCPType) &&
      0 == flowData->qosIndexAction)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "QoS Index Action should be non zero. (qosIndexAction: %d)\r\n",
                       flowData->qosIndexAction);
    return(0);
  }

  /* vrf validation */
  if ((isOverlayTunnelDSCPType || isOverlayTunnelPCPType || isOverlayTunnelType) &&
      0 != flowData->vrfAction)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "VRF Action should be zero. (vrfAction: %d)\r\n",
                       flowData->vrfAction);
    return(0);
  }

  if (isOAMPDUInjType)
  {
    if ((matchCriteria->inPortMask != OFDPA_ZERO_MASK) ||
        (matchCriteria->etherTypeMask != OFDPA_ZERO_MASK) ||
        (matchCriteria->tunnelIdMask != OFDPA_ZERO_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "For OAM PDU Injection type flow the following match criteria may only have a mask of all. "
                         "inPortMask = 0x%x etherTypeMask = 0x%x tunnelIdMask = 0x%x \r\n",
                         matchCriteria->inPortMask, matchCriteria->etherTypeMask, matchCriteria->tunnelIdMask);
      return (0);
    }
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowDscpTrustEntryCommonValidate(ofdpaDscpTrustFlowEntry_t *flowData)
{
//  uint32_t numQueues = 0;

  ofdpaDscpTrustFlowMatch_t *matchCriteria;

  matchCriteria = &flowData->match_criteria;

  /* Validate DSCP index */
  if (matchCriteria->dscpValue > OFDPA_DSCP_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "DSCP value out of range. DSCP = 0x%08x\r\n",
                       matchCriteria->dscpValue);
    return(0);
  }

  /* Validate Set field: Color */
  if (flowData->color >= OFDPA_QOS_RESERVED)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid packet color (%d).\r\n",
                       flowData->color);
    return(0);
  }

  /* Traffic class is the internal priority that is mapped to the COS Queues.
   There is no macro or API available in the SDK that returns range from the hardware.
   Leaving the validation to SDK, till the API or macro is available */
#if 0
  /* Validate Set field: Traffic Class */
  if (OFDPA_E_NONE != dpaCosQueuesMaxGet(&numQueues))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Failed to get max COS queues.\r\n", 0);
    return(0);
  }

  if (flowData->trafficClass >= numQueues)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Traffic class value out-of-range (trafficClass = %d, max value = %d).\r\n",
                       flowData->trafficClass,
                       numQueues-1);
    return(0);
  }
#endif

  return(1);
}

uint32_t ofdbFlowPortDscpTrustEntryValidate(ofdpaDscpTrustFlowEntry_t *flowData)
{
  if (ofdbFlowDscpTrustEntryCommonValidate(flowData) != 1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Port DSCP Trust flow failed common validation.\r\n", 0);
    return 0;
  }

  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_VLAN)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto Table (%d) for Port DSCP Trust flow table.\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* Valid Flow Entry */
  return (1);

}

uint32_t ofdbFlowPcpTrustEntryCommonValidate(ofdpaPcpTrustFlowEntry_t *flowData)
{
//  uint32_t numQueues = 0;

  ofdpaPcpTrustFlowMatch_t *matchCriteria;

  matchCriteria = &flowData->match_criteria;


  /* Validate DSCP index */
  if (matchCriteria->pcpValue > 0x7)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "PCP value out of range. DSCP = 0x%08x\r\n",
                       matchCriteria->pcpValue);
    return(0);
  }

  /* Validate DEI */
  if (matchCriteria->dei > 1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid DEI value. (DEI = %d)\r\n",
                       matchCriteria->dei);
    return(0);
  }

  /* Validate Set field: Color */
  if (flowData->color >= OFDPA_QOS_RESERVED)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid packet color (%d).\r\n",
                       flowData->color);
    return(0);
  }


 /*Traffic class is the internal priority that is mapped to the COS Queues.
   There is no macro or API available in the SDK that returns range from the hardware.
   Leaving the validation to SDK, till the API or macro is available */
#if 0
  /* Validate Set field: Traffic Class */
  if (OFDPA_E_NONE != dpaCosQueuesMaxGet(&numQueues))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Failed to get max COS queues.\r\n", 0);
    return(0);
  }

  if (flowData->trafficClass >= numQueues)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Traffic class value out-of-range (trafficClass = %d, max value = %d).\r\n",
                       flowData->trafficClass,
                       numQueues-1);
    return(0);
  }
#endif

  return(1);
}

uint32_t ofdbFlowPortPcpTrustEntryValidate(ofdpaPcpTrustFlowEntry_t *flowData)
{
  if (ofdbFlowPcpTrustEntryCommonValidate(flowData) != 1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Port DSCP Trust flow failed common validation.\r\n", 0);
    return(0);
  }

  /* Validate Goto table */
  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_VLAN)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto Table (%d) for Port PCP Trust flow table.\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* Valid Flow Entry */
  return (1);
}

uint32_t ofdbFlowTunnelDscpTrustEntryValidate(ofdpaDscpTrustFlowEntry_t *flowData)
{
  if (ofdbFlowDscpTrustEntryCommonValidate(flowData) != 1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Tunnel DSCP Trust flow failed common validation.\r\n", 0);
    return(0);
  }

  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_BRIDGING)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto Table (%d) for Tunnel DSCP Trust flow table.\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* Valid Flow Entry */
  return (1);
}

uint32_t ofdbFlowTunnelPcpTrustEntryValidate(ofdpaPcpTrustFlowEntry_t *flowData)
{
  if (ofdbFlowPcpTrustEntryCommonValidate(flowData) != 1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Tunnel PCP Trust flow failed common validation.\r\n", 0);
    return(0);
  }

  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_BRIDGING)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto Table (%d) for Tunnel PCP Trust flow table.\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* Valid Flow Entry */
  return (1);
}

uint32_t ofdbFlowVrfValidate(uint16_t vlanId, uint16_t vrfAction, uint16_t vrf)
{
	return 1;
}

OFDPA_ERROR_t ofdbFlowInjectedOamEntryTypeGet(ofdpaInjectedOamFlowEntry_t *flowData,
                                              OFDB_OAM_TYPE_ID_t *oamType)
{
  ofdpaInjectedOamFlowMatch_t *matchCriteria;
  OFDPA_ERROR_t                rc = OFDPA_E_NOT_FOUND;

  *oamType = 0;

  /* check flow type */
  matchCriteria = &flowData->match_criteria;

  if (0 == matchCriteria->lmepId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid lmepId %d. ", matchCriteria->lmepId);
    return rc;
  }

  //added by yjg begin,Gal无法配置，按内置规则设定
  if(OFDB_GROUP_IS_MPLS_INTERFACE(flowData->groupId))
  {//section or lsp
	  flowData->pushMplsGalHdr=1;
	  flowData->mplsGalTTLAction=1;
	  flowData->mplsGalTTL=255;
  }
  
  if(OFDB_GROUP_IS_MPLS_TUNNEL_LABEL1(flowData->groupId) || 
	  OFDB_GROUP_IS_MPLS_1_1_HEAD_END_PROTECT(flowData->groupId) ||
	  OFDB_GROUP_IS_MPLS_FAST_FAILOVER(flowData->groupId))
  {//pw
	  if(flowData->mplsBOS == 0)//vccv type 4
	  {
		  flowData->pushMplsGalHdr=1;
		  flowData->mplsGalTTLAction=1;
		  flowData->mplsGalTTL=255;
	  }
  }
  //added by yjg end
  
  if ((0 != flowData->tunnelIdAction) && (0 != flowData->mplsL2PortAction))
  {
    if ((0 != flowData->tunnelId) && (0 != flowData->mplsL2Port))
    {
      *oamType = OFDB_OAM_TYPE_ETHERNET_UP;
      rc = OFDPA_E_NONE;
    }
  }
  else if (0 != flowData->pushMplsGalHdr)
  {
    if (0 == flowData->mplsGalTTLAction)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid mplsGalTTLAction %d. ", flowData->mplsGalTTLAction);
      return rc;
    }

    if (flowData->mplsGalTTL & 0xffffff00ul)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid mplsGalTTL %d. ", flowData->mplsGalTTL);
      return rc;
    }

    if (0 != flowData->mplsGalTCAction)
    {
      if (flowData->mplsGalTC & 0xfffffff8ul)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid mplsGalTC %d. ", flowData->mplsGalTC);
        return rc;
      }
    }

    if ((0 != flowData->pushMplsLabelHdr))
    {
      if (flowData->mplsLabel & 0xfff00000ul)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid mplsLabel %d. ", flowData->mplsLabel);
        return rc;
      }

      if (0 != flowData->mplsBOS)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid mplsBOS %d. ", flowData->mplsBOS);
        return rc;
      }

      if (0 == flowData->mplsTTLAction)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid input: mplsTTLAction %d. ",
                           flowData->mplsTTLAction);
        return rc;
      }

      if(flowData->mplsTCAction != 0)
      {
          if (flowData->mplsTC & 0xfffffff8ul)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Invalid mplsTC %d. ", flowData->mplsTC);
            return rc;
          }
      }

      if (flowData->mplsTTL & 0xffffff00ul)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid mplsTTL %d. ", flowData->mplsTTL);
        return rc;
      }

      if (flowData->mplsVlanPcpAction != 0)
      {
        if ((flowData->mplsVlanPcp < 0) ||
           (flowData->mplsVlanPcp > 7))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Invalid mplsVlanPcp %d. ", flowData->mplsVlanPcp);
          return rc;
        }
      }

      if (OFDB_GROUP_IS_MPLS_INTERFACE(flowData->groupId))
      {
        *oamType = OFDB_OAM_TYPE_LSP;
        rc = OFDPA_E_NONE;
      }
      else if (OFDB_GROUP_IS_MPLS_FAST_FAILOVER(flowData->groupId) ||
               OFDB_GROUP_IS_MPLS_1_1_HEAD_END_PROTECT(flowData->groupId) ||
               OFDB_GROUP_IS_MPLS_TUNNEL_LABEL1(flowData->groupId))
      {
        *oamType = OFDB_OAM_TYPE_PW_VCCV_TYPE_4;
        rc = OFDPA_E_NONE;
      }
    }
    else
    {
      *oamType = OFDB_OAM_TYPE_SECTION;
      rc = OFDPA_E_NONE;
    }
  }
  else if ((0 != flowData->pushMplsLabelHdr))
  {
    if (flowData->mplsLabel & 0xfff00000ul)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid mplsLabel %d. ", flowData->mplsLabel);
      return rc;
    }

    if (0 == flowData->mplsBOS)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid mplsBOS %d. ", flowData->mplsBOS);
      return rc;
    }

    if (0 == flowData->mplsTTLAction)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid input: mplsTTLAction %d. ",
                         flowData->mplsTTLAction);
      return rc;
    }

    if (flowData->mplsTTL & 0xffffff00ul)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid input: mplsTTL %d. ",
                         flowData->mplsTTL);
      return rc;
    }

    if (0 != flowData->mplsTCAction)
    {
      if (flowData->mplsTC & 0xfffffff8ul)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid input: mplsTC %d. ",
                           flowData->mplsTC);
        return rc;
      }
    }

    if (flowData->mplsVlanPcpAction != 0)
    {
      if ((flowData->mplsVlanPcp < 0) ||
         (flowData->mplsVlanPcp > 7))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid mplsVlanPcp %d. ", flowData->mplsVlanPcp);
        return rc;
      }
    }

    *oamType = OFDB_OAM_TYPE_PW_VCCV_TYPE_1_OR_3;
    rc = OFDPA_E_NONE;
  }
  else
  {
    *oamType = OFDB_OAM_TYPE_ETHERNET_DOWN;
    rc = OFDPA_E_NONE;
  }

  return rc;
}


uint32_t ofdbFlowInjectedOamEntryValidate(ofdpaInjectedOamFlowEntry_t *flowData)
{
  ofdpaInjectedOamFlowMatch_t *matchCriteria;
  OFDB_OAM_TYPE_ID_t           oamType;

  /* check flow type */
  matchCriteria = &flowData->match_criteria;

  if (0 == matchCriteria->lmepId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid lmepId %d. ", matchCriteria->lmepId);
    return 0;
  }

  if (OFDPA_E_NONE != ofdbFlowInjectedOamEntryTypeGet(flowData, &oamType))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "The match criteria are not a valid combination. ", 0);
    return(0);
  }

  if ((0 != flowData->tunnelIdAction) && (OFDB_OAM_TYPE_ETHERNET_UP != oamType))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "tunnelIdAction not supported for OAM Type %d. ", oamType);
    return(0);
  }

  if ((OFDB_OAM_TYPE_ETHERNET_UP == oamType) ||(OFDB_OAM_TYPE_ETHERNET_DOWN == oamType))
  {
    if ((0 != flowData->mplsDataFirstNibbleAction) || (0 != flowData->mplsAchChannelAction))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid GACH input FirstNibbleAction %d, ChannelAction %d. ",
                         flowData->mplsDataFirstNibbleAction, flowData->mplsAchChannelAction);
      return 0;
    }
  }
  else
  {
  	OFDB_OAM_TYPE_t ofdbOamtype = OFDB_OAM_TYPE_OAM_MEP;
	
    if ((0 == flowData->mplsDataFirstNibbleAction) || (0 == flowData->mplsAchChannelAction))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid GACH input FirstNibbleAction %d, ChannelAction %d. ",
                         flowData->mplsDataFirstNibbleAction, flowData->mplsAchChannelAction);
      return 0;
    }

	if (OFDPA_E_NONE != ofdbOamTypeGet(matchCriteria->lmepId, &ofdbOamtype))
	{
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
						 "ofdbOamTypeGet error lmepid %d. ", matchCriteria->lmepId);
	  return(0);
	}


    if ((1 != flowData->mplsDataFirstNibble)  
		||(OFDPA_MPLS_ACH_CHANNEL_TYPE != flowData->mplsAchChannel && ofdbOamtype != OFDB_OAM_TYPE_BFD)
		|| (OFDPA_MPLS_BFD_ACH_CHANNEL_TYPE != flowData->mplsAchChannel && ofdbOamtype == OFDB_OAM_TYPE_BFD))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid GACH input Nibble %d, Channelvalue %d. ofdbOamtype %d",
                         flowData->mplsDataFirstNibble, flowData->mplsAchChannel, ofdbOamtype);
      return 0;
    }
  }

  if ((OFDB_OAM_TYPE_PW_VCCV_TYPE_1_OR_3 == oamType) ||
      (OFDB_OAM_TYPE_PW_VCCV_TYPE_4      == oamType))
  {
    if (!OFDB_GROUP_IS_MPLS_TUNNEL_LABEL1(flowData->groupId) && !OFDB_GROUP_IS_MPLS_FAST_FAILOVER(flowData->groupId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid group type: group %x for oamType %d. ",
                         flowData->groupId, oamType);
      return 0;
    }
  }

  if (OFDB_OAM_TYPE_ETHERNET_UP == oamType)
  {
    /* check Tunnel_ID is MPLS type */
    if (OFDB_TUNNELID_TYPE(flowData->tunnelId) != OFDPA_TUNNELID_TYPE_MPLS_TP)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid tunnelId type. Should be of type MPLS_TP (tunnelId = 0x%x)\r\n",
                         flowData->tunnelId);
      return(0);
    }

    if (OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT != flowData->gotoTableId)
    {
       OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid Goto table for Ethernet UP MEP or MIP type. (gotoTableId: %d)\r\n",
                         flowData->gotoTableId);
       return(0);
    }

    if (0 != flowData->groupId)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid groupId %d. ",
                         flowData->groupId);
      return 0;
    }
  }

  if (OFDB_OAM_TYPE_ETHERNET_DOWN == oamType)
  {
    if ((OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE != OFDB_GROUP_TYPE(flowData->groupId)) &&
        (OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE != OFDB_GROUP_TYPE(flowData->groupId)))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid group type %d for oamType %d. ",
                         OFDB_GROUP_TYPE(flowData->groupId), oamType);
      return 0;
    }
  }

  if (OFDB_OAM_TYPE_SECTION == oamType)
  {
    if (!OFDB_GROUP_IS_MPLS_INTERFACE(flowData->groupId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid group %x for oamType %d. ",
                         flowData->groupId, oamType);
      return 0;
    }
  }

  return 1;
}

uint32_t ofdbFlowVlanEntryValidate(ofdpaVlanFlowEntry_t *flowData)
{
	return 1;
}
#if 0
uint32_t ofdbFlowVlanEntryValidate(ofdpaVlanFlowEntry_t *flowData)
{
  ofdpaVlanFlowMatch_t *matchCriteria;
  uint32_t isFilteringType = 0;
  uint32_t isAssignmentUntaggedType = 0;
  uint32_t isAssignmentPriorityTaggedType = 0;
  uint32_t isAllowAllType = 0;
  uint32_t isXlateSingleType = 0;
  uint32_t isXlateDoubleToSingleType = 0;
  uint32_t isMplsL2SingleTagType = 0;
/*  uint32_t isMplsL2DoubleTagType = 0; --> this entry type is indistinguishable from isXlateDoubletoSingleType */
  uint32_t isMplsL2UntaggedType = 0;
  uint32_t isMplsL2PriorityTaggedType = 0;
  uint32_t isMplsL2AllOnPortType = 0;
  uint32_t isVlanCrossConnectSingleTagType = 0;
  uint32_t portNum = 0;
  uint32_t vlanId;
  ofdpaFlowEntry_t taggedFlow;
  ofdbPortInfo_t portInfo;
  OFDB_ENTRY_FLAG_t portFlags;
  OFDPA_MPLS_L2_PORT_TYPE_t mplsL2PortType;

  /* check match criteria constraints */
  matchCriteria = &flowData->match_criteria;

  /* inPort must be a valid interface */
  if ((OFDB_PORT_TYPE(matchCriteria->inPort) != OFDPA_PORT_TYPE_PHYSICAL) ||
      (!ofdbPortIsValid(matchCriteria->inPort)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Port is not valid. port = %d\r\n",
                       matchCriteria->inPort);
    return(0);
  }

  /* determine what entry type we have here */

  if ((matchCriteria->vlanIdMask == (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)) &&
      (matchCriteria->vlanId == 0))
  {
    /* mask must be 0x1fff (exact) and match field is 0 */
    if (flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_TERMINATION_MAC)
    {
      isAssignmentUntaggedType = 1;
    }
    else if ((flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT) ||
             (flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT))
    {
      isMplsL2UntaggedType = 1;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid goto table for untagged/assignment flow.\r\n",
                         0);
      return(0);
    }
  }
  else if ((matchCriteria->vlanIdMask == (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)) &&
           (matchCriteria->vlanId == OFDPA_VID_PRESENT))
  {
    /* mask must be 0x1fff (exact) and match field is 0x1000 */
    if (flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_TERMINATION_MAC)
    {
      isAssignmentPriorityTaggedType = 1;
    }
    else if ((flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT) ||
             (flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT))
    {
      isMplsL2PriorityTaggedType = 1;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid goto table for priority tagged assignment flow.\r\n",
                         0);
      return(0);
    }
  }
  else if ((matchCriteria->vlanIdMask == (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)) &&
           ((matchCriteria->vlanId & OFDPA_VID_PRESENT) == OFDPA_VID_PRESENT) &&
           dpaVlanIsValid(matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
  {
    /* mask must be 0x1fff, VLAN ID must be form 0x1nnn, and lower 12 bits of VLAN ID is valid */

    switch (flowData->gotoTableId)
    {
      case OFDPA_FLOW_TABLE_ID_VLAN_1:
        /* could be either VLAN Translate Double to Single or MPLS L2 Double VLAN type entry */
        /* these two entry types are undifferentiated */
        isXlateDoubleToSingleType = 1;
        break;
      case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
        /* Can be either MPLS L2 Single Tag Translate, MPLS L2 Untagged, MPLS L2  Priority Tagged, */
        /* MPLS L2 All Traffic on Port or VLAN Cross Connect - Single Tagged */
        if (OFDB_TUNNELID_TYPE(flowData->tunnelId) == OFDPA_TUNNELID_TYPE_VLAN)
        {
          isVlanCrossConnectSingleTagType = 1;
        }
        else
        {
          isMplsL2SingleTagType = 1;
        }
        break;
      case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
        /* can only be MPLS L2 Single Tag type entry */
        isMplsL2SingleTagType = 1;
        break;
      case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
        /* could be either VLAN Filtering,  VLAN Translate Single, or VLAN Translate Single to Double type entry */
        if ((flowData->setVlanIdAction == 0) && (flowData->setVlanId2Action == 0))
        {
          isFilteringType = 1;
        }
        else
        {
          /* VLAN Translate Single and VLAN Translate Single to Double entries specify a Set-Field VLAN action */
          isXlateSingleType = 1;
        }
        break;
      default:
        /* invalid combination of match criteria and goto table */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid vlanId/mask/gotoTableId combination. vlanId = %d (0x%x), vlanIdMask = 0x%x, gotoTableId = %d\r\n",
                           matchCriteria->vlanId, matchCriteria->vlanId, matchCriteria->vlanIdMask, flowData->gotoTableId);
        return(0);
        break;
    }
  }
  else if ((matchCriteria->vlanIdMask == OFDPA_VID_PRESENT) &&
           ((matchCriteria->vlanId & matchCriteria->vlanIdMask) == OFDPA_VID_PRESENT))
  {
    /* match any tagged - mask must be 0x1000 and match field is 0x1000 (VLAN ID present, but we don't care its value) */
    if (flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_TERMINATION_MAC)
    {
      isAllowAllType = 1;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid goto table for VLAN Allow All flow.\r\n",
                         0);
      return(0);
    }
  }
  else if (matchCriteria->vlanIdMask == OFDPA_VID_NONE)
  {
    /* match all regardless of VLAN tagging status */
    if ((flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT) || (flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT))
    {
      isMplsL2AllOnPortType = 1;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid goto table for MPLS L2 All Traffic on Port flow.\r\n",
                         0);
      return(0);
    }
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid vlanId/mask combination. vlanId = %d (0x%x), vlanIdMask = 0x%x\r\n",
                       matchCriteria->vlanId, matchCriteria->vlanId, matchCriteria->vlanIdMask);
    return(0);
  }

  if (((0 != isMplsL2SingleTagType) ||
       (0 != isMplsL2UntaggedType)  ||
       (0 != isMplsL2PriorityTaggedType)  ||
       (0 != isMplsL2AllOnPortType)) &&
      (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_FEATURE_MPLSSUPPORTED)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS flows are not supported.\r\n", 0);
    return 0;
  }

  /* check constraints between flow and VXLAN Access Port configuration */
  /* no VLAN flow entry may match on a port if that port is configured as a tunnel access port that includes all packets on that port */
  /* no VLAN flow entry may match on a port/VLAN if that port/VLAN is configured as a tunnel access port for the same port/vlan */

  /*
   * iterate over all logical port entries
   * if an entry is an access port and the ports match
   * if the access port is for untagged, VLAN flow entry rejected
   * else access port is for tagged, if VLAN matches, VLAN flow rejected
   */
  portNum = OFDB_PORT_INDEX_SET(portNum, 0);
  portNum = OFDB_PORT_TYPE_SET(portNum, OFDPA_PORT_TYPE_LOGICAL_TUNNEL);

  while ((ofdbPortNextGet(portNum, &portNum) == OFDPA_E_NONE) &&
         (OFDB_PORT_TYPE(portNum) == OFDPA_PORT_TYPE_LOGICAL_TUNNEL))
  {
    if (ofdbPortGet(portNum, &portInfo, &portFlags) == OFDPA_E_NONE)
    {
      if ((portFlags & OFDB_PORT_DELETED) == 0)
      {
        if ((portInfo.tunnelPortConfig.type == OFDPA_TUNNEL_PORT_TYPE_ACCESS) &&
            (portInfo.tunnelPortConfig.configData.access.physicalPortNum == matchCriteria->inPort))
        {
          if (portInfo.tunnelPortConfig.configData.access.untagged)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Tunnel access port configured to match all traffic on same physical port. inPort = %d, portNum = 0x%x\r\n",
                               matchCriteria->inPort, portNum);
            return(0);
          }
          else
          {
            vlanId = (matchCriteria->vlanId & OFDPA_VID_EXACT_MASK);
            if (isFilteringType &&
                (portInfo.tunnelPortConfig.configData.access.vlanId == vlanId))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Tunnel access port configured to match same VLAN traffic on same physical port. "
                                 "inPort = %d, vlanId = %d, portNum = 0x%x\r\n",
                                 matchCriteria->inPort, vlanId, portNum);
              return(0);
            }
          }
        }
      }
    }
  }

  /* if goto table is Ingress Maintenance Point, check if OAM feature is supported */
  if (OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT == flowData->gotoTableId)
  {
    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_FEATURE_OAM))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "OAM not supported on platform.\r\n",
                         0);
      return(0);
    }
  }

  /* check instruction constraints that apply to all entry types */

  /* data for SET-FIELD VLAN must have the OFPVID_PRESENT bit set */
  if ((flowData->setVlanIdAction != 0) && ((flowData->newVlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry specifies newVlanId without OFDPA_VID_PRESENT bit set. "
                       "setVlanIdAction = %d, newVlanId = 0x%x\r\n",
                       flowData->setVlanIdAction, flowData->newVlanId);
    return(0);
  }
  if ((flowData->setVlanId2Action != 0) && ((flowData->newVlanId2 & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry specifies newVlanId2 without OFDPA_VID_PRESENT bit set. "
                       "setVlanId2Action = %d, newVlanId2 = 0x%x\r\n",
                       flowData->setVlanId2Action, flowData->newVlanId2);
    return(0);
  }

  /* if classBasedCountAction set, counter must exist in counter table */
  if (flowData->classBasedCountAction != 0)
  {
    if (ofdbClassBasedCounterRefCountGet(flowData->classBasedCountId, NULL) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Entry specifies classBasedCountAction but counter not found in table. "
                         "classBasedCountAction = %d, classBasedCountId = 0x%x\r\n",
                         flowData->classBasedCountAction, flowData->classBasedCountId);
      return(0);
    }
  }

  /* check instructions based on entry type */

  if (isFilteringType)
  {
    /* next table must be Termination MAC Flow Table */
    if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_TERMINATION_MAC)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect goto table for VLAN Filtering type entry. gotoTableId = %d\r\n",
                         flowData->gotoTableId);
      return(0);
    }

    /* VLAN Filtering entries may not specify the following actions */
    if ((flowData->setVlanIdAction != 0) ||
        (flowData->popVlanAction != 0) ||
        (flowData->pushVlan2Action != 0) ||
        (flowData->setVlanId2Action != 0) ||
        (flowData->ovidAction != 0) ||
        (flowData->mplsL2PortAction != 0) ||
        (flowData->tunnelIdAction != 0) ||
        (flowData->classBasedCountAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Filtering type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    if (ofdbFlowVrfValidate((matchCriteria->vlanId & OFDPA_VID_EXACT_MASK), flowData->vrfAction, flowData->vrf) == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Filtering type entry failed VRF action/value validation.\r\n", 0);
      return(0);
    }
  }

  if (isAssignmentUntaggedType || isAssignmentPriorityTaggedType)
  {
    /* next table must be Termination MAC Flow Table */
    if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_TERMINATION_MAC)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect goto table for VLAN Assignment type entry. gotoTableId = %d\r\n",
                         flowData->gotoTableId);
      return(0);
    }

    /* Untagged Packet Port VLAN Assignment entries may not specify the following actions */
    if ((flowData->popVlanAction != 0) ||
        (flowData->ovidAction != 0) ||
        (flowData->mplsL2PortAction != 0) ||
        (flowData->tunnelIdAction != 0) ||
        (flowData->classBasedCountAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Untagged Packet Port VLAN Assignment type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    /* for Untagged Packet Port VLAN Assignment set-VLAN action must be included */
    if ((flowData->setVlanIdAction == 0) || !dpaVlanIsValid(flowData->newVlanId & OFDPA_VID_EXACT_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Assignment type entry does not specify setVlanIdAction with a valid newVlanId. "
                         "setVlanIdAction = %d, newVlanId = 0x%x\r\n",
                         flowData->setVlanIdAction, flowData->newVlanId);
      return(0);
    }

    if (flowData->pushVlan2Action != 0)
    {
      /* if pushing an outer VLAN tag, TPID for second tag must be 0x8100 */
      if (flowData->newTpid2 != 0x8100)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN Assignment type entry specifies pushVlan2Action with incorrect newTpid2 value (must be 0x8100). "
                           "pushVlan2Action = %d, newTpid2 = 0x%04x\r\n",
                           flowData->pushVlan2Action, flowData->newTpid2);
        return(0);
      }

      /* if pushing an outer VLAN tag, set-VLAN action for second tag must be included */
      if ((flowData->setVlanId2Action == 0) || !dpaVlanIsValid(flowData->newVlanId2 & OFDPA_VID_EXACT_MASK))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN Assignment type entry does not specify setVlanId2Action with a valid newVlanId2. "
                           "setVlanId2Action = %d, newVlanId2 = 0x%x\r\n",
                           flowData->setVlanId2Action, flowData->newVlanId2);
        return(0);
      }
    }

    if (flowData->setVlanId2Action != 0)
    {
      vlanId = (flowData->newVlanId2 & OFDPA_VID_EXACT_MASK);
    }
    else
    {
      vlanId = (flowData->newVlanId & OFDPA_VID_EXACT_MASK);
    }

    if (ofdbFlowVrfValidate(vlanId, flowData->vrfAction, flowData->vrf) == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Assignment type entry failed VRF action/value validation.\r\n", 0);
      return(0);
    }

    /* there must be an existing tagged flow entry for port|vlan combination */
    memset(&taggedFlow, 0, sizeof(taggedFlow));
    taggedFlow.tableId = OFDPA_FLOW_TABLE_ID_VLAN;

    taggedFlow.flowData.vlanFlowEntry.match_criteria.inPort = matchCriteria->inPort;
    if (flowData->setVlanId2Action == 0)
    {
      taggedFlow.flowData.vlanFlowEntry.match_criteria.vlanId = flowData->newVlanId;
    }
    else
    {
      taggedFlow.flowData.vlanFlowEntry.match_criteria.vlanId = flowData->newVlanId2;
    }
    taggedFlow.flowData.vlanFlowEntry.match_criteria.vlanIdMask = OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK;

    if ((ofdbFlowGet(&taggedFlow, NULL, NULL) != OFDPA_E_NONE) ||
        (taggedFlow.flowData.vlanFlowEntry.gotoTableId != OFDPA_FLOW_TABLE_ID_TERMINATION_MAC))
    {
      /* prerequisite flow not found */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "untagged flow rejected: flow admitting tagged traffic on port/VLAN not found. port = %d. vlanId = %d\r\n",
                         matchCriteria->inPort, (taggedFlow.flowData.vlanFlowEntry.match_criteria.vlanId & OFDPA_VID_EXACT_MASK));
      return(0);
    }
  }

  if (isAllowAllType)
  {
    /* next table must be Termination MAC Flow Table */
    if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_TERMINATION_MAC)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect goto table for VLAN Allow All type entry. gotoTableId = %d\r\n",
                         flowData->gotoTableId);
      return(0);
    }

    /* VLAN Allow All entries may not specify the following actions */
    if ((flowData->setVlanIdAction != 0) ||
        (flowData->popVlanAction != 0) ||
        (flowData->pushVlan2Action != 0) ||
        (flowData->setVlanId2Action != 0) ||
        (flowData->ovidAction != 0) ||
        (flowData->mplsL2PortAction != 0) ||
        (flowData->tunnelIdAction != 0) ||
        (flowData->classBasedCountAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Allow All type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }
  }

  if (isXlateSingleType)
  {
    /* next table must be Termination MAC Flow Table */
    if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_TERMINATION_MAC)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect goto table for VLAN Translate Single or Single to Double type entry. gotoTableId = %d\r\n",
                         flowData->gotoTableId);
      return(0);
    }

    /* VLAN Translate Single or Single to Double entries may not specify the following actions */
    if ((flowData->popVlanAction != 0) ||
        (flowData->ovidAction != 0) ||
        (flowData->mplsL2PortAction != 0) ||
        (flowData->tunnelIdAction != 0) ||
        (flowData->classBasedCountAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Single or Single to Double type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    if (flowData->setVlanIdAction != 0)
    {
      if (!dpaVlanIsValid(flowData->newVlanId & OFDPA_VID_EXACT_MASK))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN Translate Single or Single to Double type entry does not specify a valid newVlanId. newVlanId = %d\r\n",
                           (flowData->newVlanId & OFDPA_VID_EXACT_MASK));
        return(0);
      }
    }

    /* both or neither pushVlan2Action and setVlandId2Action */
    if (((flowData->pushVlan2Action == 0) && (flowData->setVlanId2Action != 0)) ||
        ((flowData->pushVlan2Action != 0) && (flowData->setVlanId2Action == 0)))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Single or Single to Double type entry must "
                         "specify both pushVlan2Action and setVlandId2Action or neither.\r\n", 0);
      return(0);
    }

    /* if single-to-double, added TPID must be valid */
    if (flowData->pushVlan2Action != 0)
    {
      if (0x8100 != flowData->newTpid2)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN Translate Single to Double type entry does not specify a valid newTpid2. (newTpid2 = 0x%x)\r\n",
                           flowData->newTpid2);
        return(0);
      }
    }

    /* if single-to-double, added VLAN must be valid */
    if (flowData->setVlanId2Action != 0)
    {
      if (!dpaVlanIsValid(flowData->newVlanId2 & OFDPA_VID_EXACT_MASK))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN Translate Single to Double type entry does not specify a valid newVlanId2. newVlanId2 = %d\r\n",
                           (flowData->newVlanId2 & OFDPA_VID_EXACT_MASK));
        return(0);
      }
    }

    if (flowData->setVlanId2Action != 0)
    {
      vlanId = (flowData->newVlanId2 & OFDPA_VID_EXACT_MASK);
    }
    else
    {
      vlanId = (flowData->newVlanId & OFDPA_VID_EXACT_MASK);
    }

    if (ofdbFlowVrfValidate(vlanId, flowData->vrfAction, flowData->vrf) == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Single or Single to Double type entry failed VRF action/value validation.\r\n", 0);
      return(0);
    }
  }

  if (isXlateDoubleToSingleType)
  {
    /* next table must be VLAN_1 Flow Table */
    if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_VLAN_1)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect goto table for VLAN Translate Double to Single type entry. gotoTableId = %d\r\n",
                         flowData->gotoTableId);
      return(0);
    }

    /* VLAN Translate Double to Single entries may not specify the following actions */
    if ((flowData->setVlanIdAction != 0) ||
        (flowData->pushVlan2Action != 0) ||
        (flowData->setVlanId2Action != 0) ||
        (flowData->vrfAction != 0) ||
        (flowData->mplsL2PortAction != 0) ||
        (flowData->tunnelIdAction != 0) ||
        (flowData->classBasedCountAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Double to Single type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    /* set OVID action must be included */
    if (flowData->ovidAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Double to Single type entry does not specify ovidAction.\r\n", 0);
      return(0);
    }

    /* the value of the OVID metadata field must be the same as the matched outer tag */
    vlanId = matchCriteria->vlanId & OFDPA_VID_EXACT_MASK;
    if (vlanId != (flowData->ovid & OFDPA_VID_EXACT_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Double to Single type entry does not specify OVID matching outer VID match criterion. "
                         "(vlanId = %d, OVID = %d)\r\n",
                         vlanId, (flowData->ovid & OFDPA_VID_EXACT_MASK));
      return(0);
    }

    /* the value of the OVID metadata field must have OFDPA_VID_PRESENT set */
    if (OFDPA_VID_PRESENT != (flowData->ovid & OFDPA_VID_PRESENT))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Double to Single type entry does not specify OVID with OFDPA_VID_PRESENT set. "
                         "(OVID = 0x%x)\r\n",
                         flowData->ovid);
      return(0);
    }

    /* pop VLAN action must be included */
    if (flowData->popVlanAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Double to Single type entry does not specify popVlanAction.\r\n", 0);
      return(0);
    }
  }

  if (isMplsL2SingleTagType)
  {
    /* next table must be MPLS L2 Port or Maintenance Point Flow Table */
    if ((flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT) && (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect goto table for MPLS L2 Single Tag type entry. gotoTableId = %d\r\n",
                         flowData->gotoTableId);
      return(0);
    }

    /* MPLS L2 Single Tag entries may not specify the following actions */
	/*leishenghua modify 20170209, 单tag 支持pop*/
#ifdef PUSH_POP_SWAP_SUPPORT
	if ((flowData->pushVlan2Action != 0) ||
		(flowData->setVlanId2Action != 0) ||
		(flowData->ovidAction != 0) ||
		(flowData->vrfAction != 0))

#else
    if ((flowData->popVlanAction != 0) ||
        (flowData->pushVlan2Action != 0) ||
        (flowData->setVlanId2Action != 0) ||
        (flowData->ovidAction != 0) ||
        (flowData->vrfAction != 0))
#endif        
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Single Tag type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    /* set Tunnel ID action must be included */
    if (flowData->tunnelIdAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Single Tag type entry does not specify tunnelIdAction.\r\n", 0);
      return(0);
    }

    /* check Tunnel_ID is MPLS type */
    if (OFDB_TUNNELID_TYPE(flowData->tunnelId) != OFDPA_TUNNELID_TYPE_MPLS_TP)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Single Tag type entry specifies tunnelId of wrong type. (tunnelId = 0x%x)\r\n",
                         flowData->tunnelId);
      return(0);
    }

    /* set MPLS_L2_Port action must be included */
    if (flowData->mplsL2PortAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Single Tag type entry does not specify mplsL2PortAction.\r\n", 0);
      return(0);
    }

    /* check MPLS_L2_Port is local type */
    mplsL2PortType = OFDB_MPLS_L2_PORT_TYPE(flowData->mplsL2Port);
    if ((mplsL2PortType != OFDPA_MPLS_L2_PORT_TYPE_VPWS_LOCAL) &&
        (mplsL2PortType != OFDPA_MPLS_L2_PORT_TYPE_VPLS_LOCAL))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Single Tag type entry specifies mplsL2Port of wrong type. (mplsL2Port = 0x%x)\r\n",
                         flowData->mplsL2Port);
      return(0);
    }
  }

  if (isMplsL2UntaggedType || isMplsL2PriorityTaggedType)
  {
    /* next table must be MPLS L2 Port or Maintenance Point Flow Table */
    if ((flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT) && (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect goto table for MPLS L2 Untagged/Priority Tagged type entry. gotoTableId = %d\r\n",
                         flowData->gotoTableId);
      return(0);
    }

#ifdef PUSH_POP_SWAP_SUPPORT
	/* MPLS L2 Untagged entries may not specify the following actions */
	if ((flowData->popVlanAction != 0) ||
	/*(flowData->pushVlan2Action != 0) ||
	(flowData->setVlanId2Action != 0) ||*/
	(flowData->ovidAction != 0) ||
	(flowData->vrfAction != 0))

#else
    /* MPLS L2 Untagged entries may not specify the following actions */
    if ((flowData->popVlanAction != 0) ||
        (flowData->pushVlan2Action != 0) ||
        (flowData->setVlanId2Action != 0) ||
        (flowData->ovidAction != 0) ||
        (flowData->vrfAction != 0))
#endif        
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Untagged/Priority Tagged type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    /*JiaoYong 20160527 modified for MPLS Untag L2 packet*/
    #if 0
    /* for MPLS L2 Untagged set-VLAN action must be included */
    if ((flowData->setVlanIdAction == 0) || !dpaVlanIsValid(flowData->newVlanId & OFDPA_VID_EXACT_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Untagged/Priority Tagged type entry does not specify setVlanIdAction with "
                         "a valid newVlanId. setVlanIdAction = %d, newVlanId = %d\r\n",
                         flowData->setVlanIdAction, (flowData->newVlanId & OFDPA_VID_EXACT_MASK));
      return(0);
    }
#endif
    /* set Tunnel ID action must be included */
    if (flowData->tunnelIdAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Untagged/Priority Tagged type entry does not specify tunnelIdAction.\r\n", 0);
      return(0);
    }

    /* set MPLS_L2_Port action must be included */
    if (flowData->mplsL2PortAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Untagged/Priority Tagged  type entry does not specify mplsL2PortAction.\r\n", 0);
      return(0);
    }
  }

  if (isMplsL2AllOnPortType)
  {
    /* next table must be MPLS L2 Port or Maintenance Point Flow Table */
    if ((flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT) && (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect goto table for MPLS All Traffic on Port type entry. gotoTableId = %d\r\n",
                         flowData->gotoTableId);
      return(0);
    }

    /* MPLS All Traffic on Port entries may not specify the following actions */
    if ((flowData->setVlanIdAction != 0) ||
        (flowData->popVlanAction != 0) ||
        (flowData->pushVlan2Action != 0) ||
        (flowData->setVlanId2Action != 0) ||
        (flowData->ovidAction != 0) ||
        (flowData->vrfAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS All Traffic on Port type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    /* set Tunnel ID action must be included */
    if (flowData->tunnelIdAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS All Traffic on Port type entry does not specify tunnelIdAction.\r\n", 0);
      return(0);
    }

    /* set MPLS_L2_Port action must be included */
    if (flowData->mplsL2PortAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS All Traffic on Port type entry does not specify mplsL2PortAction.\r\n", 0);
      return(0);
    }
  }

  if (isVlanCrossConnectSingleTagType)
  {
    /* VLAN Cross Connect Single Tag entries may not specify the following actions */
    if ((flowData->popVlanAction != 0) ||
        (flowData->pushVlan2Action != 0) ||
        (flowData->setVlanId2Action != 0) ||
        (flowData->ovidAction != 0) ||
        (flowData->mplsTypeAction != 0) ||
        (flowData->vrfAction != 0) ||
        (flowData->classBasedCountAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Cross Connect Single Tag type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    /* set Tunnel ID action must be included */
    if (flowData->tunnelIdAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Cross Connect Single Tag type entry does not specify tunnelIdAction.\r\n", 0);
      return(0);
    }

    /* check Tunnel_ID is VLAN type */
    if (OFDB_TUNNELID_TYPE(flowData->tunnelId) != OFDPA_TUNNELID_TYPE_VLAN)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Cross Connect Single Tag type entry specifies tunnelId of wrong type. (tunnelId = 0x%x)\r\n",
                         flowData->tunnelId);
      return(0);
    }

    /* set MPLS_L2_Port action must be included */
    if (flowData->mplsL2PortAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Cross Connect Single Tag type entry does not specify mplsL2PortAction.\r\n", 0);
      return(0);
    }

    /* TODO: Need to add check MPLS_L2_Port type */
  }

  if ((0 != isMplsL2SingleTagType) ||
      (0 != isMplsL2UntaggedType) ||
      (0 != isMplsL2PriorityTaggedType) ||
      (0 != isMplsL2AllOnPortType))
  {
    /* set MPLS type action must be included */
    if (0 == flowData->mplsTypeAction)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS type action should be 1.\r\n", 0);
      return 0;
    }

    /*  MPLS_TYPE metadata can only be set to VPWS for MPLS type flows. */
    if (flowData->mplsType != OFDPA_MPLS_TYPE_VPWS)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS type can only be set to VPWS for MPLS type flows..\r\n", 0);
      return 0;
    }
  }
  else
  {
    /* set MPLS type action must not be included */
    if (0 != flowData->mplsTypeAction)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS type action should not be non-zero.\r\n", 0);
      return 0;
    }
  }

  /* if you get here, you are valid */
  return(1);
}
#endif

uint32_t ofdbFlowVlan1EntryValidate(ofdpaVlan1FlowEntry_t *flowData)
{
  ofdpaVlan1FlowMatch_t *matchCriteria;
  uint32_t isVlanAssignmentType = 0;
  uint32_t isMplsL2StackedVlanType = 0;
  uint32_t isVlanCrossConnectType = 0;
  OFDPA_MPLS_L2_PORT_TYPE_t mplsL2PortType;
  uint16_t vlanId;

  /* check match criteria constraints */

  matchCriteria = &flowData->match_criteria;

  /* inPort must be a valid interface */
  if ((OFDB_PORT_TYPE(matchCriteria->inPort) != OFDPA_PORT_TYPE_PHYSICAL) ||
      (!ofdbPortIsValid(matchCriteria->inPort)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Port is not valid. port = %d\r\n",
                       matchCriteria->inPort);
    return(0);
  }

  /* matched VLAN must be present and valid (form 0x1nnn where nnn is valid VLAN ID value) */
  if (((matchCriteria->vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT) ||
      (!dpaVlanIsValid(matchCriteria->vlanId & OFDPA_VID_EXACT_MASK)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Match vlanId is not valid. vlanId = 0x%x\r\n",
                       matchCriteria->vlanId);
    return(0);
  }

  /* matched OVID must be present and valid (form 0x1nnn where nnn is valid VLAN ID value) */
  if (((matchCriteria->ovid & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT) ||
      (!dpaVlanIsValid(matchCriteria->ovid & OFDPA_VID_EXACT_MASK)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Match ovid is not valid. ovid = 0x%x\r\n",
                       matchCriteria->ovid);
    return(0);
  }

  /* check instructions */

  /* goto table indicates the entry type */
  switch (flowData->gotoTableId)
  {
    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      isVlanAssignmentType = 1;
      break;
    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      /* Can be either MPLS L2 Stacked VLAN or VLAN Cross Connect */
      if (OFDB_TUNNELID_TYPE(flowData->tunnelId) == OFDPA_TUNNELID_TYPE_VLAN)
      {
        isVlanCrossConnectType = 1;
      }
      else
      {
        isMplsL2StackedVlanType = 1;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      isMplsL2StackedVlanType = 1;
      break;
    default:
      /* invalid goto table ID */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect goto table. gotoTableId = %d\r\n",
                         flowData->gotoTableId);
      return(0);
      break;
  }

  /* if goto table is Ingress Maintenance Point, check if OAM feature is supported */
  if (OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT == flowData->gotoTableId)
  {
    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_FEATURE_OAM))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "OAM not supported on platform.\r\n",
                         0);
      return(0);
    }
  }

  /* if set-field VLAN_ID is specified, new VLAN_ID must be valid */
  if ((flowData->setVlanIdAction != 0) &&
      (!dpaVlanIsValid(flowData->newVlanId & OFDPA_VID_EXACT_MASK)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid newVlanId for set-field VLAN_ID action. (newVlanId = %d)\r\n",
                       (flowData->newVlanId & OFDPA_VID_EXACT_MASK));
    return(0);
  }

  if (flowData->pushVlan2Action != 0)
  {
    /* if push VLAN action specified, must also include a set-field VLAN action */
    if (0 == flowData->setVlanId2Action)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Rule specifies push VLAN but does not include set-field VLAN_ID.\r\n", 0);
      return(0);
    }

    /* if pushActionVlan specified, added TPID must be valid */
    if (0x8100 != flowData->newTpid2)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN_1 flow entry does not specify a valid newTpid. (newTpid2 = 0x%x)\r\n",
                         flowData->newTpid2);
      return(0);
    }
  }

  /* check instruction constraints that apply to all entry types */

  /* data for SET-FIELD VLAN must have the OFPVID_PRESENT bit set */
  if ((flowData->setVlanIdAction != 0) && ((flowData->newVlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry specifies newVlanId without OFDPA_VID_PRESENT bit set. "
                       "setVlanIdAction = %d, newVlanId = 0x%x\r\n",
                       flowData->setVlanIdAction, flowData->newVlanId);
    return(0);
  }
  if ((flowData->setVlanId2Action != 0) && ((flowData->newVlanId2 & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry specifies newVlanId2 without OFDPA_VID_PRESENT bit set. "
                       "setVlanId2Action = %d, newVlanId2 = 0x%x\r\n",
                       flowData->setVlanId2Action, flowData->newVlanId2);
    return(0);
  }

  /* if classBasedCountAction set, counter must exist in counter table */
  if (flowData->classBasedCountAction != 0)
  {
    if (ofdbClassBasedCounterRefCountGet(flowData->classBasedCountId, NULL) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Entry specifies classBasedCountAction but counter not found in table. "
                         "classBasedCountAction = %d, classBasedCountId = 0x%x\r\n",
                         flowData->classBasedCountAction, flowData->classBasedCountId);
      return(0);
    }
  }

  /* entry type specific instruction checks */
  if (isVlanAssignmentType)
  {
    /* VLAN Assignment entries may not specify the following actions */
    if ((flowData->mplsL2PortAction != 0) ||
        (flowData->tunnelIdAction != 0) ||
        (flowData->classBasedCountAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Assignment type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    if (flowData->setVlanId2Action != 0)
    {
      vlanId = (flowData->newVlanId2 & OFDPA_VID_EXACT_MASK);
    }
    else if (flowData->setVlanIdAction != 0)
    {
      vlanId = (flowData->newVlanId & OFDPA_VID_EXACT_MASK);
    }
    else
    {
      vlanId = (matchCriteria->vlanId & OFDPA_VID_EXACT_MASK);
    }

    if (ofdbFlowVrfValidate(vlanId, flowData->vrfAction, flowData->vrf) == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN_1 Stacked VLAN Assignment type entry failed VRF action/value validation.\r\n", 0);
      return(0);
    }
  }

  if (isMplsL2StackedVlanType)
  {
    /* MPLS L2 Stacked VLAN entries may not specify the following actions */
    if (flowData->vrfAction != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Stacked VLAN type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    /* set Tunnel ID action must be included */
    if (flowData->tunnelIdAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Stacked VLAN type entry does not specify tunnelIdAction.\r\n", 0);
      return(0);
    }

    /* check Tunnel_ID is MPLS type */
    if (OFDB_TUNNELID_TYPE(flowData->tunnelId) != OFDPA_TUNNELID_TYPE_MPLS_TP)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Stacked VLAN type entry specifies tunnelId of wrong type. (tunnelId = 0x%x)\r\n",
                         flowData->tunnelId);
      return(0);
    }

    /* set MPLS_L2_Port action must be included */
    if (flowData->mplsL2PortAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Stacked VLAN type entry does not specify mplsL2PortAction.\r\n", 0);
      return(0);
    }

    /* check MPLS_L2_Port is local type */
    mplsL2PortType = OFDB_MPLS_L2_PORT_TYPE(flowData->mplsL2Port);
    if ((mplsL2PortType != OFDPA_MPLS_L2_PORT_TYPE_VPWS_LOCAL) &&
        (mplsL2PortType != OFDPA_MPLS_L2_PORT_TYPE_VPLS_LOCAL))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS L2 Stacked VLAN type entry specifies mplsL2Port of wrong type. (mplsL2Port = 0x%x)\r\n",
                         flowData->mplsL2Port);
      return(0);
    }

    /* set MPLS type action must be included */
    if (0 == flowData->mplsTypeAction)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS type action should be 1.\r\n", 0);
      return 0;
    }

    /*  MPLS_TYPE metadata can only be set to VPWS for MPLS type flows. */
    if (flowData->mplsType != OFDPA_MPLS_TYPE_VPWS)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS type can only be set to VPWS for MPLS type flows..\r\n", 0);
      return 0;
    }
  }

  if (isVlanCrossConnectType)
  {
    /* VLAN Cross Connect entries may not specify the following actions */
    if ((flowData->vrfAction != 0) ||
        (flowData->mplsTypeAction != 0) ||
        (flowData->classBasedCountAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Cross Connect type entry specifies an unsupported Action.\r\n", 0);
      return(0);
    }

    /* set Tunnel ID action must be included */
    if (flowData->tunnelIdAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Cross Connect type entry does not specify tunnelIdAction.\r\n", 0);
      return(0);
    }

    /* check Tunnel_ID is VLAN type */
    if (OFDB_TUNNELID_TYPE(flowData->tunnelId) != OFDPA_TUNNELID_TYPE_VLAN)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Cross Connect type entry specifies tunnelId of wrong type. (tunnelId = 0x%x)\r\n",
                         flowData->tunnelId);
      return(0);
    }

    /* set MPLS_L2_Port action must be included */
    if (flowData->mplsL2PortAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Cross Connect type entry does not specify mplsL2PortAction.\r\n", 0);
      return(0);
    }
    /* TODO: Need to add check MPLS_L2_Port type */
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowMpEntryValidate(ofdpaMpFlowEntry_t *flowData)
{
  char buffer[15];
  ofdpaMpFlowMatch_t *matchCriteria = &flowData->match_criteria;
  ofdpaOamMegConfig_t megConfig;
  ofdpaOamMepConfig_t mepConfig, lmMepConfig;
  uint32_t isMipLbmProcessType = 0;
  uint32_t isMipLtmProcessType = 0;
  uint32_t isDownMepPduType = 0;
  uint32_t isPduPassthruHigherMdlType = 0;
  uint32_t isPduDropLowerMdlType = 0;
  uint32_t isMepDataType = 0;
  uint8_t  mdlIter1 = 0, mdlIter2 = 0, ingressFlowFound = 0, egressFlowFound = 0, direction = 0;
  ofdpaFlowEntry_t lmIngressMpFlow, lmEgressMpFlow;
  uint32_t lmepId;
  uint32_t rxLmepPresent = 0, txLmepPresent = 0;
  uint32_t megIndex = 0;
  ofdpaOamMipConfig_t mipConfig;

  /* first check things all flow entry types must match */

  /* inPort must be a valid interface */
  if ((OFDB_PORT_TYPE(matchCriteria->inPort) != OFDPA_PORT_TYPE_PHYSICAL) ||
      (!ofdbPortIsValid(matchCriteria->inPort)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Port is not valid. inPort = %d\r\n",
                       matchCriteria->inPort);
    return(0);
  }

  /* VLAN mask is all or nothing for all types */
  if ((matchCriteria->vlanIdMask != OFDPA_VID_FIELD_MASK) &&
      (matchCriteria->vlanIdMask != (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid VLAN mask. (0x%04x)\r\n", matchCriteria->vlanIdMask);
    return(0);
  }

  /* if vlanId specified, vlanId must be valid VLAN */
  if ((matchCriteria->vlanIdMask == (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)) &&
      !dpaVlanIsValid(matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry does not specify a valid vlanId.  vlanId = 0x%04x\r\n",
                       matchCriteria->vlanId);
    return(0);
  }
  else
  {
    matchCriteria->vlanId = (matchCriteria->vlanId & matchCriteria->vlanIdMask);
  }

  /* determine entry type */
  if ((matchCriteria->etherType == 0x8902) && (matchCriteria->etherTypeMask == OFDPA_ETHERTYPE_EXACT_MASK))
  {
    if (matchCriteria->oamY1731MdlMask == OFDPA_OAM_Y1731_MDL_EXACT_MASK)
    {
      /* validate Y1731 opcode mask value */
      if (matchCriteria->oamY1731OpcodeMask == OFDPA_OAM_Y1731_OPCODE_EXACT_MASK)
      {
        /* examine Y1731 opcode value */
        if (matchCriteria->oamY1731Opcode == 3)
        {
          isMipLbmProcessType = 1;
        }
        else if (matchCriteria->oamY1731Opcode == 5)
        {
          isMipLtmProcessType = 1;
        }
        else
        {
          /* unsupported OAM opcode matched */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Invalid oamY1731Opcode value (oamY1731Opcode = %d)\r\n",
                             matchCriteria->oamY1731Opcode);
          return(0);
        }
      }
      else if (matchCriteria->oamY1731OpcodeMask == OFDPA_OAM_Y1731_OPCODE_ALL_MASK)
      {
        /* can only be "Down MEP PDU" or "PDU Passthru (higher MDL)", at this point */
        /* difference is goto table and clearActions flag */
        if (flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT)
        {
          isPduPassthruHigherMdlType = 1;
        }
        else if (flowData->clearAction != 0)
        {
          isDownMepPduType = 1;
        }
        else
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Invalid flow match criteria combination.\r\n",
                             0);
          return(0);
        }
      }
      else
      {
        /* opcode mask must be all or exact */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid oamY1731OpcodeMask value (oamY1731OpcodeMask = 0x%x)\r\n",
                           matchCriteria->oamY1731OpcodeMask);
        return(0);
      }
    }
    else if (matchCriteria->oamY1731MdlMask == OFDPA_OAM_Y1731_MDL_ALL_MASK)
    {
      isPduDropLowerMdlType = 1;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid oamY1731MdlMask value (oamY1731MdlMask = 0x%x)\r\n",
                         matchCriteria->oamY1731MdlMask);
      return(0);
    }
  }
  else if (matchCriteria->etherTypeMask == OFDPA_ETHERTYPE_ALL_MASK)
  {
    isMepDataType = 1;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Incorrect etherType and mask combination (etherType/etherTypeMask = 0x%x/0x%x)\r\n",
                       matchCriteria->etherType, matchCriteria->etherTypeMask);
    return(0);
  }

#ifndef OFDPA_OAM_ETHERNET_DOWN_MEPS_SUPPORTED
  if (isDownMepPduType || isPduPassthruHigherMdlType || isPduDropLowerMdlType || isMepDataType)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Ingress Maintenance Point Flow entry type not supported.\r\n",
                       0);
    return(0);
  }
#endif

  /* perform any further match criteria validation based on flow entry type */
  if (isMipLbmProcessType || isMipLtmProcessType || isDownMepPduType || isPduPassthruHigherMdlType)
  {
    if (matchCriteria->oamY1731Mdl > OFDPA_OAM_Y1731_MDL_EXACT_MASK)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid oamY1731Mdl value (oamY1731Mdl = 0x%x)\r\n",
                         matchCriteria->oamY1731Mdl);
      return(0);
    }
  }

  /* only MIP LBM Process (our MDL) type entries match on destination MAC, others must accept all */
  if (isMipLbmProcessType)
  {
    if (!OFDPA_MAC_ADDR_IS_ALL_ONES(matchCriteria->destMacMask.addr))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "destMacMask must be exact for MIP LBM Process (our MDL) type flow (destMacMask: %s)\r\n",
                         ofdpaMacAddrToString(&matchCriteria->destMacMask, buffer));
      return(0);
    }
  }
  else
  {
    if (!OFDPA_MAC_ADDR_IS_NULL(matchCriteria->destMacMask.addr))
    {
      /* if destMacMask is not all, it is invalid */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "destMacMask must be all unless MIP LBM Process (our MDL) type flow (destMacMask: %s)\r\n",
                         ofdpaMacAddrToString(&matchCriteria->destMacMask, buffer));
      return(0);
    }
  }

  /* check instructions */
  if (isMipLbmProcessType || isMipLtmProcessType || isDownMepPduType || isPduDropLowerMdlType)
  {
    /* required to have clearAction set */
    if (flowData->clearAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type required to specify clearAction.\r\n",
                         0);
      return(0);
    }
    /* since clearAction is specified, GoTo should be 0 */
    if (flowData->gotoTableId != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect GoTo Table (gotoTableId = %d)\r\n",
                         flowData->gotoTableId);
      return(0);
    }
  }

  if (isPduPassthruHigherMdlType || isMepDataType)
  {
    /* cannot have clearAction set */
    if (flowData->clearAction != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type cannot specify clearAction.\r\n",
                         0);
      return(0);
    }
    /* these entry types must goto MPLS L2 Port table */
    if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect GoTo Table for PDU Passthru (higher MDL) type flow (gotoTableId = %d)\r\n",
                         flowData->gotoTableId);
      return(0);
    }
  }

  /* check apply actions */
  if (isMipLbmProcessType || isMipLtmProcessType || isDownMepPduType)
  {
    /* required to have setField LMEP_ID */
    if (flowData->lmepIdAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type required to specify setField LMEP_ID.\r\n",
                         0);
      return(0);
    }
  }
  else
  {
    /* all other types cannot have setField LMEP_ID */
    if (flowData->lmepIdAction != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type cannot to specify setField LMEP_ID.\r\n",
                         0);
      return(0);
    }
  }

  if (isMipLbmProcessType || isMipLtmProcessType || isDownMepPduType)
  {
    /* must specify Output to either CONTROLLER or LOCAL port */
    if ((flowData->outputPort != OFDPA_PORT_CONTROLLER) && (flowData->outputPort != OFDPA_PORT_LOCAL))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type required to specify ouputPort as CONTROLLER or LOCAL.\r\n",
                         0);
      return(0);
    }
  }
  else if (isPduDropLowerMdlType)
  {
    /* may specify Output to either CONTROLLER or LOCAL port */
    if ((flowData->outputPort != 0) &&
        (flowData->outputPort != OFDPA_PORT_CONTROLLER) &&
        (flowData->outputPort != OFDPA_PORT_LOCAL))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type required to specify ouputPort as CONTROLLER or LOCAL.\r\n",
                         0);
      return(0);
    }
  }
  else
  {
    /* all other types may not specify Output  */
    if (flowData->outputPort != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow may not specify ouput action.\r\n",
                         0);
      return(0);
    }
  }

  /* only these entry types may specify check drop status and LM Tx Count actions */
  if (flowData->checkDropStatusAction)
  {
    if (!isPduPassthruHigherMdlType && !isMepDataType && !isDownMepPduType)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow may not specify checkDropStatusAction.\r\n",
                         0);
      return(0);
    }
  }

  /* check the lmepId action field validity */
  if ((flowData->lmepIdAction) ||
      (flowData->checkDropStatusAction))
  {
    /* verify MP is configured for this LMEP_ID */
    if (ofdbOamMepGet(flowData->lmepId, &mepConfig, NULL) != OFDPA_E_NONE)
    {
      /* retrieve MIP table entry */
      if (ofdbOamMipGet(flowData->lmepId, &mipConfig, NULL) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "MPLS flow specifies mpId not found in MEP/MIP table. (lmpId = %d)\r\n",
                           flowData->lmepId);
        return(0);
      }
      else
      {
        megIndex = mipConfig.megIndex;
      }
    }
    else
    {
      megIndex = mepConfig.megIndex;
    }

    /* verify MEG is configured for MPs MEG index */
    if (ofdbOamMegGet(megIndex, &megConfig, NULL, NULL) == OFDPA_E_NONE)
    {
      /* verify MEG is of type Ethernet */
      if (megConfig.megType != OFDPA_OAM_MEG_TYPE_ETHERNET)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "G8113.1 or BHH megtype is not supported. (MEG index = %d)\r\n",
                           mepConfig.megIndex);
        return(0);
      }
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Specified LMEP_ID does not have any MEG configured. (MEG index = %d)\r\n",
                         mepConfig.megIndex);
      return(0);
    }
  }

  for (mdlIter1 = OFDPA_MDL_MIN_VALUE; mdlIter1 <= OFDPA_MDL_MAX_VALUE; mdlIter1++)
  {
    if ((0 != flowData->rxLmCountingLmepId[mdlIter1]) ||
        (0 != flowData->txLmCountingLmepId[mdlIter1]))
    {
      /* Verify that rxLmCountingLmepId/txLmCountingLmepId are specified only for DownMepPdu and MepData type flow only */
      if (!isDownMepPduType &&
          !isMepDataType &&
          !isMipLbmProcessType &&
          !isMipLtmProcessType)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "rxLmCountingLmepId/txLmCountingLmepId can be specified for DownMepPdu "
                           "or MepData or MipLbmProcess or MipLtmProcess flow types. \r\n", 0);
        return(0);
      }

      if (isMepDataType)
      {
        direction = 0;
        do
        {
          /* When direction is zero validate rxLmCountingLmepId
             When direction is one validate txLmCountingLmepId */
          if (direction == 0)
          {
            lmepId = flowData->rxLmCountingLmepId[mdlIter1];
            if ((rxLmepPresent == 0) &&
                (lmepId != 0))
            {
              rxLmepPresent = 1;
            }
          }
          else
          {
            lmepId = flowData->txLmCountingLmepId[mdlIter1];
            if ((txLmepPresent == 0) &&
                (lmepId != 0))
            {
              txLmepPresent = 1;
            }
          }

          if (0 != lmepId)
          {
            /* verify MP is configured for this LMEP_ID */
            if (ofdbOamMepGet(lmepId, &lmMepConfig, NULL) == OFDPA_E_NONE)
            {
              /* verify MEG is configured for MPs MEG index and verify MEG is of type Ethernet */
              if ((ofdbOamMegGet(lmMepConfig.megIndex, &megConfig, NULL, NULL) == OFDPA_E_NONE) &&
                  (megConfig.megType == OFDPA_OAM_MEG_TYPE_ETHERNET))
              {
                if (lmMepConfig.ifIndex != matchCriteria->inPort)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d should be configured on same interface specified "
                                     "in inPort (%d) of match criteria. \r\n",
                                     lmepId, matchCriteria->inPort);
                  return(0);
                }

                if (megConfig.primVid != (matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d should be configured with same VLAN as specified "
                                     "in vlanId (%d) of match criteria. \r\n",
                                     lmepId, (matchCriteria->vlanId | OFDPA_VID_EXACT_MASK));
                  return(0);
                }

                /* verify that rxLmCountingLmepId is set for Ethernet Down MEP. */
                if ((direction == 0) && (lmMepConfig.direction != OFDPA_MP_DIRECTION_DOWN))
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d is not Ethernet Down MEP. \r\n",
                                     lmepId);
                  return(0);
                }

                /* verify that txLmCountingLmepId is set for Ethernet Up MEP. */
                if ((direction == 1) && (lmMepConfig.direction != OFDPA_MP_DIRECTION_UP))
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d is not Ethernet Up MEP. \r\n",
                                     lmepId);
                  return(0);
                }
              }
              else
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                   "MEG corresponding to lmepId %d either doesnot exists or is not of type G8013.1 or Ethernet \r\n",
                                   lmepId);
                return(0);
              }
            }
            else
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Specified lmepId %d is not configured for any MEP. \r\n",
                                 lmepId);
              return(0);
            }
          }
          direction++;
        } while (direction < 2);
      }

      if (isDownMepPduType ||
          isMipLbmProcessType ||
          isMipLtmProcessType)
      {
        direction = 0;
        do
        {
          /* When direction is zero validate rxLmCountingLmepId
             When direction is one validate txLmCountingLmepId */
          if (direction == 0)
          {
            lmepId = flowData->rxLmCountingLmepId[mdlIter1];
          }
          else
          {
            lmepId = flowData->txLmCountingLmepId[mdlIter1];
          }

          if (0 != lmepId)
          {
            /* verify MP is configured for this LMEP_ID */
            if (ofdbOamMepGet(lmepId, &lmMepConfig, NULL) == OFDPA_E_NONE)
            {
              /* verify MEG is configured for MPs MEG index and verify MEG is of type Ethernet */
              if ((ofdbOamMegGet(lmMepConfig.megIndex, &megConfig, NULL, NULL) == OFDPA_E_NONE) &&
                  (megConfig.megType == OFDPA_OAM_MEG_TYPE_ETHERNET))
              {
                /* verify that rxLmCountingLmepId/txLmCountingLmepId is not enabled for MDL level greater than or equal to MDL level in flow rule*/
                if (matchCriteria->oamY1731Mdl <= megConfig.level)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "For DownMepPduType/MipLtmProcess/MipLbmProcess flow, rxLmCountingLmepId/txLmCountingLmepId cannot be enabled "
                                     "for MDL level %d greater than or equal to MDL level %d (oamY1731Mdl field in match criteria) \r\n",
                                     megConfig.level, matchCriteria->oamY1731Mdl);
                  return(0);
                }

                if (lmMepConfig.ifIndex != matchCriteria->inPort)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d in rxLmCountingLmepId/txLmCountingLmepId should be configured on same interface specified "
                                     "in inPort (%d) of match criteria. \r\n",
                                     lmepId, matchCriteria->inPort);
                  return(0);
                }

                if (megConfig.primVid != (matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d in rxLmCountingLmepId/txLmCountingLmepId should be configured with same VLAN as specified "
                                     "in vlanId (%d) of match criteria. \r\n",
                                     lmepId, (matchCriteria->vlanId | OFDPA_VID_EXACT_MASK));
                  return(0);
                }

                /* verify that LM is enabled on lmepId */
                ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT, &lmIngressMpFlow);
                lmIngressMpFlow.flowData.mpFlowEntry.match_criteria.inPort = matchCriteria->inPort;
                lmIngressMpFlow.flowData.mpFlowEntry.match_criteria.vlanId = matchCriteria->vlanId;
                lmIngressMpFlow.flowData.mpFlowEntry.match_criteria.vlanIdMask = matchCriteria->vlanIdMask;

                if (ofdbFlowGet(&lmIngressMpFlow, NULL, NULL) == OFDPA_E_NONE)
                {
                  if (lmMepConfig.direction == OFDPA_MP_DIRECTION_DOWN)
                  {
                    for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
                    {
                      if (lmIngressMpFlow.flowData.mpFlowEntry.rxLmCountingLmepId[mdlIter2] == lmepId)
                      {
                        ingressFlowFound = 1;
                        break;
                      }
                    }
                  }
                  else
                  {
                    for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
                    {
                      if (lmIngressMpFlow.flowData.mpFlowEntry.txLmCountingLmepId[mdlIter2] == lmepId)
                      {
                        ingressFlowFound = 1;
                        break;
                      }
                    }
                  }
                }

                if (!ingressFlowFound)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "LM is not enabled on lmepId %d \r\n",
                                     lmepId);
                  return(0);
                }

                ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT, &lmEgressMpFlow);
                lmEgressMpFlow.flowData.egressMpFlowEntry.match_criteria.outPort = matchCriteria->inPort;
                lmEgressMpFlow.flowData.egressMpFlowEntry.match_criteria.vlanId = matchCriteria->vlanId;
                lmEgressMpFlow.flowData.egressMpFlowEntry.match_criteria.vlanIdMask = matchCriteria->vlanIdMask;

                if (ofdbFlowGet(&lmEgressMpFlow, NULL, NULL) == OFDPA_E_NONE)
                {
                  if (lmMepConfig.direction == OFDPA_MP_DIRECTION_DOWN)
                  {
                    for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
                    {
                      if (lmEgressMpFlow.flowData.egressMpFlowEntry.txLmCountingLmepId[mdlIter2] == lmepId)
                      {
                        egressFlowFound = 1;
                        break;
                      }
                    }
                  }

                  if (lmMepConfig.direction == OFDPA_MP_DIRECTION_UP)
                  {
                    for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
                    {
                      if (lmEgressMpFlow.flowData.egressMpFlowEntry.rxLmCountingLmepId[mdlIter2] == lmepId)
                      {
                        egressFlowFound = 1;
                        break;
                      }
                    }
                  }
                }

                if (!egressFlowFound)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "LM is not enabled on lmepId %d %d \r\n",
                                     lmepId);
                  return(0);
                }
              }
              else
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                   "MEG corresponding to lmepId %d either doesnot exists or is not of type G8013.1 or Ethernet \r\n",
                                   lmepId);
                return(0);
              }
            }
            else
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Specified lmepId %d is not configured for any MEP. \r\n",
                                 lmepId);
              return(0);
            }
          }
          direction++;
        } while (direction < 2);

        /* verify that lmepId specified in rxLmCountingLmepId and txLmCountingLmepId are not same */
        for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
        {
          if ((flowData->rxLmCountingLmepId[mdlIter1] != 0) &&
              (flowData->txLmCountingLmepId[mdlIter2] != 0) &&
              (flowData->rxLmCountingLmepId[mdlIter1] == flowData->txLmCountingLmepId[mdlIter2]))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "lmepId %d in rxLmCountingLmepId and lmepId %d in txLmCountingLmepId cannot be same. \r\n",
                               flowData->rxLmCountingLmepId[mdlIter1], flowData->txLmCountingLmepId[mdlIter2]);
            return(0);
          }
        }
      }
    }
  }

  if (isMepDataType)
  {
    if (!(rxLmepPresent ||
          txLmepPresent))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "For MEP data type flow atleast one element in rxLmCountingLmepId/txLmCountingLmepId should exist. \r\n",
                         0);
      return(0);
    }

    /* check for unwanted match criteria. */
    if ((matchCriteria->oamY1731MdlMask != OFDPA_OAM_Y1731_MDL_ALL_MASK) ||
        (matchCriteria->oamY1731OpcodeMask != OFDPA_OAM_Y1731_OPCODE_ALL_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "For MEP data type flow the following match criteria may only have a mask of all. "
                         "oamY1731MdlMask = 0x%x oamY1731OpcodeMask = 0x%x \r\n",
                         matchCriteria->oamY1731MdlMask, matchCriteria->oamY1731OpcodeMask);
      return(0);
    }
  }
  else if (isPduDropLowerMdlType)
  {
    /* check for unwanted match criteria. */
    if (matchCriteria->oamY1731OpcodeMask != OFDPA_OAM_Y1731_OPCODE_ALL_MASK)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "For PDU drop type flow the following match criteria may only have a mask of all. "
                         "oamY1731OpcodeMask = 0x%x\r\n", matchCriteria->oamY1731OpcodeMask);
      return(0);
    }
  }

  return(1);
}

uint32_t ofdbFlowMplsMpEntryValidate(ofdpaMplsMpFlowEntry_t *flowData)
{
  OFDB_OAM_TYPE_t oamtype = OFDB_OAM_TYPE_OAM_MEP;
  
  if(ofdbOamTypeGet(flowData->match_criteria.lmepId, &oamtype) != OFDPA_E_NONE)
  {
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid lmepId match field. (%d)\r\n",
																						   flowData->match_criteria.lmepId);
	  return(0);

  }
  
  /* etherType must be 0x8902 */
  if ((flowData->match_criteria.etherType != 0x8902 && oamtype != OFDB_OAM_TYPE_BFD)
  	|| (flowData->match_criteria.etherType != 0x0021 && oamtype == OFDB_OAM_TYPE_BFD))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid Ethertype match field. (%04x)\r\n",
                                                                                         flowData->match_criteria.etherType);
    return(0);
  }

        /* check instructions */
  if (0 != flowData->oamSetCounterFieldsAction)
  {
    /* verify that LMEP ID appears in OAM configuration */
    if (OFDPA_E_NONE != ofdbOamMepGet(flowData->lmepId, NULL, NULL))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Specified LMEP ID is not configured in the MEP table (lmepId = %d)\r\n",
                         flowData->lmepId);
      return(0);
    }
  }

  return(1);
}

uint32_t ofdbFlowMplsL2PortEntryValidate(ofdpaMplsL2PortFlowEntry_t *flowData)
{
  ofdpaMplsL2PortFlowMatch_t *matchCriteria;
  uint8_t isVlanCrossConnectType = 0;

  matchCriteria = &flowData->match_criteria;

  /* tunnelId must be MPLS type or VLAN type*/
  if ((OFDB_TUNNELID_TYPE(matchCriteria->tunnelId) != OFDPA_TUNNELID_TYPE_MPLS_TP) &&
      (OFDB_TUNNELID_TYPE(matchCriteria->tunnelId) != OFDPA_TUNNELID_TYPE_VLAN))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Matched tunnelId must be either MPLS type or VLAN type. tunnelId = 0x%08x\r\n",
                       matchCriteria->tunnelId);
    return(0);
  }

  if (OFDB_TUNNELID_TYPE(matchCriteria->tunnelId) == OFDPA_TUNNELID_TYPE_VLAN)
  {
    isVlanCrossConnectType = 1;
  }

  switch (flowData->gotoTableId)
  {
    case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
      if ((0x0800 != matchCriteria->etherType) &&
          (0x86dd != matchCriteria->etherType))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid ethertype (0x%04x) for DSCP rule.\r\n",
                           matchCriteria->etherType);
        return(0);
      }
      /* checking ethertype mask for DSCP rule */
      if (OFDPA_ETHERTYPE_EXACT_MASK != matchCriteria->etherTypeMask)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid ethertype mask (0x%04x) for DSCP rule.\r\n",
                           matchCriteria->etherTypeMask);
        return(0);
      }
      /* checking qos index instruction */
      if (flowData->qosIndexAction == 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Set field action - QOS Index required for QOS Trust "
                           "MPLS L2 Port flow.\r\n",
                           0);
        return(0);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
      /* checking ethertype mask for PCP rule */
      if (OFDPA_ETHERTYPE_ALL_MASK != matchCriteria->etherTypeMask)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid ethertype mask (0x%04x) for PCP rule.\r\n",
                           matchCriteria->etherTypeMask);
        return(0);
      }
      /* checking qos index indstruction */
      if (flowData->qosIndexAction == 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Set field action - QOS Index required for QOS Trust "
                           "MPLS L2 Port flow.\r\n",
                           0);
        return(0);
      }
      break;
#if 0
      /* VPLS is unsupported as of now; fall through to the default case */
    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      break;
#endif
    case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      /* checking ethertype mask for VPWS rule or VLAN Cross Connect rule */
      if (OFDPA_ETHERTYPE_ALL_MASK != matchCriteria->etherTypeMask)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid ethertype mask (0x%04x) for VPWS or VLAN Cross Connect rule.\r\n",
                           matchCriteria->etherTypeMask);
        return(0);
      }
      /* checking qos index indstruction */
      if (flowData->qosIndexAction != 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid set field action QOS Index for VPWS or VLAN Cross Connect rule in "
                           "MPLS L2 Port flow.\r\n",
                           0);
        return(0);
      }
      break;

    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid Goto Table (%d) for MPLS L2 port (0x%08x).\r\n",
                         flowData->gotoTableId,
                         matchCriteria->mplsL2Port);
      return(0);
      break;
  }

  if (isVlanCrossConnectType)
  {
    /* Specified Group ID must exist and be of appropriate type */
    if (ofdbRefGroupValidate(flowData->groupId) != OFDPA_E_NONE)
    {
      /* referenced group ID not found in tables */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Referenced group entry not valid. groupId = 0x%08x\r\n",
                         flowData->groupId);
      return(0);
    }

    /* Check for the group type: L2 Interface or L2 Unfiltered Interface*/
    if ((OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE != OFDB_GROUP_TYPE(flowData->groupId)) &&
        (OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE != OFDB_GROUP_TYPE(flowData->groupId)))
    {
      /* referenced group ID is wrong type */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Reference group type for VLAN Cross Connect type rule is wrong, groupId (0x%08x)\r\n",
                         OFDB_GROUP_TYPE(flowData->groupId));
      return(0);
    }
  }
  else
  {
    switch (OFDB_MPLS_L2_PORT_TYPE(matchCriteria->mplsL2Port))
    {
      case OFDPA_MPLS_L2_PORT_TYPE_VPWS_LOCAL:
        if (matchCriteria->mplsL2PortMask != OFDPA_MPLS_L2_PORT_EXACT_MASK)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Invalid MPLS L2 port/mask combination (0x%08x/0x%08x).\r\n",
                             matchCriteria->mplsL2Port,
                             matchCriteria->mplsL2PortMask);
          return(0);
        }

        /* Specified Group ID must exist and be of appropriate type */
        if (ofdbRefGroupValidate(flowData->groupId) != OFDPA_E_NONE)
        {
          /* referenced group ID not found in tables */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Referenced group entry not valid. groupId = 0x%08x\r\n",
                             flowData->groupId);
          return(0);
        }

        /* Check for the group type: MPLS L2 VPN label*/
        if (!OFDB_GROUP_IS_MPLS_L2_VPN_LABEL(flowData->groupId) &&
            !OFDB_GROUP_IS_MPLS_FAST_FAILOVER(flowData->groupId))
        {
          /* referenced group ID is wrong type */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid group type (0x%08x)\r\n",
                             OFDB_GROUP_TYPE(flowData->groupId));
          return(0);
        }
        break;

      case OFDPA_MPLS_L2_PORT_TYPE_VPLS_LOCAL:
        /* VPLS is unsupported as of now; fall through to the default case */

#if 0
        if ((flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_BRIDGING)        &&
            (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST) &&
            (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Invalid Goto Table (%d) for MPLS L2 port type (0x%08x).\r\n",
                             flowData->gotoTableId,
                             matchCriteria->mplsL2Port);
          return(0);
        }

        /* Groups are not valid for VPLS port types */
        if (flowData->groupId != 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Group not valid for MPLS L2 port (0x%08x).\r\n",
                             OFDB_MPLS_L2_PORT_TYPE(matchCriteria->mplsL2Port));
          return(0);
        }
        break;
#endif

      case OFDPA_MPLS_L2_PORT_TYPE_VPLS_NW:
      case OFDPA_MPLS_L2_PORT_TYPE_VPWS_NW:
      default:
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid/unsupported MPLS L2 Port (0x%08x).\r\n",
                           matchCriteria->mplsL2Port);
        return(0);
    }
  }

  /* Valid Flow Entry */
  return(1);
}

uint32_t ofdbFlowMplsDscpTrustEntryValidate(ofdpaDscpTrustFlowEntry_t *flowData)
{
  if (ofdbFlowDscpTrustEntryCommonValidate(flowData) != 1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS DSCP Trust flow failed common validation.\r\n", 0);
    return(0);
  }

  /* Check the goto table */
  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto Table (%d) for MPLS DSCP Trust flow table.\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* Valid Flow Entry */
  return (1);
}

uint32_t ofdbFlowMplsPcpTrustEntryValidate(ofdpaPcpTrustFlowEntry_t *flowData)
{
  if (ofdbFlowPcpTrustEntryCommonValidate(flowData) != 1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS PCP Trust flow failed common validation.\r\n", 0);
    return(0);
  }

  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto Table (%d) for MPLS PCP Trust flow table.\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* Valid Flow Entry */
  return(1);
}

uint32_t ofdbFlowMplsQosClassEntryValidate(ofdpaMplsL2PortQosClassFlowEntry_t *flowData)
{
  ofdpaMplsL2PortQosClassFlowMatch_t *matchCriteria;
  const struct in6_addr in6addr_null = { { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } } };

  matchCriteria = &flowData->match_criteria;

  /* MPLS L2 port must be one of local types */
  switch (OFDB_MPLS_L2_PORT_TYPE(matchCriteria->mplsL2Port))
  {
    case OFDPA_MPLS_L2_PORT_TYPE_VPLS_LOCAL:
    case OFDPA_MPLS_L2_PORT_TYPE_VPWS_LOCAL:
      break;

    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid MPLS L2 Port type (0x%x) for MPLS QoS Class flow table.\r\n",
                         matchCriteria->mplsL2Port);
      return(0);
      break;
  }

  switch (OFDB_TUNNELID_TYPE(matchCriteria->tunnelId))
  {
    case OFDPA_TUNNELID_TYPE_MPLS_TP:
      break;
    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid Tunnel ID type (0x%x) for MPLS QoS Class flow table.\r\n",
                         matchCriteria->tunnelId);
      return(0);
  }

  if ((OFDPA_ETHERTYPE_EXACT_MASK != matchCriteria->etherTypeMask) &&
      (OFDPA_ETHERTYPE_ALL_MASK   != matchCriteria->etherTypeMask))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Ethertype mask: 0x%x\r\n",
                       matchCriteria->etherTypeMask);
    return(0);
  }

  /* Check if IPv4 ether type is missing or incorrect */
  if (((matchCriteria->etherTypeMask & matchCriteria->etherType) != 0x0800) &&
      ((0 != matchCriteria->destIp4Mask) || (0 != matchCriteria->sourceIp4Mask)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid ethertype for IPv4 match fields.\r\n", 0);
    return(0);
  }

  if (((matchCriteria->etherTypeMask & matchCriteria->etherType) != 0x86dd) &&
      ((0 != memcmp(&matchCriteria->destIp6Mask, &in6addr_null, sizeof(matchCriteria->destIp6Mask))) ||
       (0 != memcmp(&matchCriteria->sourceIp6Mask, &in6addr_null, sizeof(matchCriteria->sourceIp6Mask)))))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid ethertype for IPv6 match fields.\r\n", 0);
    return(0);
  }

  /* If not IPv6 or IPv4, then no IP match fields are allowed */
  if (((matchCriteria->etherTypeMask & matchCriteria->etherType) != 0x86dd) &&
      ((matchCriteria->etherTypeMask & matchCriteria->etherType) != 0x0800))
  {
    if (matchCriteria->dscpMask != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid ethertype (0x%x) for IP DSCP match field.\r\n", matchCriteria->etherType);
      return(0);
    }
    if (matchCriteria->ipProtoMask != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid ethertype (0x%x) for IP Protocol match field.\r\n", matchCriteria->etherType);
      return(0);
    }

    if ((0 != matchCriteria->srcL4PortMask) ||
        (0 != matchCriteria->destL4PortMask))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid ethertype for IP L4 ports match fields.\r\n", 0);
      return(0);
    }
  }

  if (((0 != matchCriteria->srcL4PortMask) || (0 != matchCriteria->destL4PortMask)) &&
      (((matchCriteria->ipProto & matchCriteria->ipProtoMask) != IPPROTO_TCP) &&
       ((matchCriteria->ipProto & matchCriteria->ipProtoMask) != IPPROTO_UDP)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid IP Protocol type for L4 match fields.\r\n", 0);
    return(0);
  }

  /* check instructions */

  /* validate goto instruction */
  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_L2_POLICER)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid goto table id. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* validate color value */
  if (flowData->colorAction != 0)
  {
    if (flowData->color >= OFDPA_QOS_RESERVED)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid set-field color value (%d).\r\n",
                         flowData->color);
      return(0);
    }
  }

  /* validate traffic class value */
  if (flowData->trafficClassAction != 0)
  {
    if (flowData->trafficClass > OFDPA_TRAFFIC_CLASS_MAX_VALUE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Traffic class value out of range. trafficClass = %d\r\n",
                         flowData->trafficClass);
      return(0);
    }
  }

  /* Valid Flow Entry */
  return(1);
}

uint32_t ofdbFlowL2PolicerEntryValidate(ofdpaL2PolicerFlowEntry_t *flowData)
{
  ofdpaL2PolicerFlowMatch_t *matchCriteria;
  ofdpaMeterEntryStats_t meterStats;

  matchCriteria = &flowData->match_criteria;

  /* tunnel ID must be non-zero */
  if (0 == OFDB_TUNNELID_INDEX(matchCriteria->tunnelId))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Tunnel Id must be non-zero for MPLS entry types. (tunnelId:0x%x)\r\n",
                       matchCriteria->tunnelId);
    return(0);
  }

  /* Tunnel Id type must be MPLS-TP type */
  if (OFDPA_TUNNELID_TYPE_MPLS_TP != OFDB_TUNNELID_TYPE(matchCriteria->tunnelId))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Tunnel Id type must be MPLS-TP type. (tunnelId:0x%x)\r\n",
                       matchCriteria->tunnelId);
    return(0);
  }

  /* Check if the MPLS L2 Port mask is valid for the flow */
  if (matchCriteria->mplsL2PortMask != OFDPA_MPLS_L2_PORT_EXACT_MASK)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid MPLS L2 Port Mask (0x%x) for L2 Policer flow table.\r\n",
                       matchCriteria->mplsL2PortMask);
    return(0);
  }

  /* validate match fields */
  if (OFDPA_MPLS_L2_PORT_TYPE_VPWS_LOCAL != OFDB_MPLS_L2_PORT_TYPE(matchCriteria->mplsL2Port))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid MPLS L2 Port type. (mplsL2Port:0x%x)\r\n",
                       matchCriteria->mplsL2Port);
    return(0);
  }

  /* check instructions */

  /* validate goto instruction */
  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS &&
      flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_TYPE &&
      flowData->gotoTableId != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid goto table id. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* validate meterId instruction */
  if (flowData->meterId < OFDPA_L2_POLICER_METERID_MIN_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Meter Id not in L2 Policer meter id range. (meterId: %d)\r\n",
                       flowData->meterId);
    return(0);
  }

  if (OFDPA_E_NONE != ofdbMeterStatsGet(flowData->meterId, &meterStats))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_VERBOSE,
                       "Meter not found, meterId = %d.\r\n",
                       flowData->meterId);
    return(0);
  }

  /* apply actions */

  if (!flowData->colorActionsIndex)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "colorActionsIndex cannot be 0\r\n", 0);
    return(0);
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowL2PolicerActionsEntryValidate(ofdpaL2PolicerActionsFlowEntry_t *flowData)
{
  ofdpaL2PolicerActionsFlowMatch_t *matchCriteria;

  matchCriteria = &flowData->match_criteria;

  /* validate QOS color match field */
  switch (matchCriteria->color)
  {
    case OFDPA_QOS_GREEN:
    case OFDPA_QOS_YELLOW:
    case OFDPA_QOS_RED:
      break;
    default:
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "QOS color value is not valid. color = %d\r\n",
                           matchCriteria->color);
        return 0;
      }
  }

  /* validate color Actions Index match field */
  if (!matchCriteria->colorActionsIndex)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Match Criteria colorActionsIndex value %d is not valid.\r\n",
                       matchCriteria->colorActionsIndex);
    return 0;
  }

  /* check instructions */

  /* validate goto instruction */
  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_TYPE &&
      flowData->gotoTableId != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid goto table id. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* apply actions */

  /* validate color Actions Index set field action (must set to zero) */
  if (0 != flowData->colorActionsIndex)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Set-Field colorActionsIndex value %d is not valid.\r\n",
                       flowData->colorActionsIndex);
    return 0;
  }

  /* validate DSCP value */
  if (flowData->dscpAction &&
      flowData->dscp > OFDPA_DSCP_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "DSCP value out of range. DSCP = %d\r\n",
                       flowData->dscp);
    return(0);
  }

  /* validate ECN value */
  if (flowData->ecnAction &&
      flowData->ecn > OFDPA_ECN_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "ECN value out of range. ecn = %d\r\n",
                       flowData->ecn);
    return(0);
  }

  /* validate VLAN PCP value */
  if (flowData->vlanPcpAction &&
      flowData->vlanPcp > OFDPA_VLAN_PCP_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "VLAN PCP value out of range. vlanPcp = %d\r\n",
                       flowData->vlanPcp);
    return(0);
  }

  /* validate Traffic Class value */
  if (flowData->trafficClassAction &&
      flowData->trafficClass > OFDPA_TRAFFIC_CLASS_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Traffic class value out of range. trafficClass = %d\r\n",
                       flowData->trafficClass);
    return(0);
  }

  /* set VLAN PCP and set DSCP actions cannot be established both in the same flow */
  if (flowData->dscpAction && flowData->vlanPcpAction)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "set VLAN PCP and set DSCP actions cannot be established both in the same flow.\r\n", 0);
    return(0);
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowTerminationMacEntryValidate(ofdpaTerminationMacFlowEntry_t *flowData)
{
  ofdpaTerminationMacFlowMatch_t *matchCriteria;
  ofdpaMacAddr_t mcastV4MaskForm = { { 0xff, 0xff, 0xff, 0x80, 0x00, 0x00 } };
  ofdpaMacAddr_t mcastV6MaskForm = { { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 } };
  uint32_t isIpUnicastType = 0;
  uint32_t isMPLSUnicastType = 0;
  uint32_t isV4McastType = 0;
  uint32_t isV6McastType = 0;

  matchCriteria = &flowData->match_criteria;

  /* VLAN mask is all or nothing for all types */
  if ((matchCriteria->vlanIdMask != OFDPA_VID_FIELD_MASK) &&
      (matchCriteria->vlanIdMask != (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid VLAN mask. (%04x)\r\n", matchCriteria->vlanIdMask);
    return(0);
  }

  /* VLAN, if not masked, must be valid */
  if (matchCriteria->vlanIdMask != OFDPA_VID_FIELD_MASK)
  {
    /* check that OFDPA_VID_PRESENT is set in match field */
    if ((matchCriteria->vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "vlanId field must have OFDPA_VID_PRESENT set. (0x%04x)\r\n",
                         matchCriteria->vlanId);
      return(0);
    }
    if (!dpaVlanIsValid(matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid VLAN ID. (0x%04x)\r\n",
                         matchCriteria->vlanId);
      return(0);
    }
  }

  /* etherType must be 0x0800 or 0x86dd or 0x8847*/
  if ((matchCriteria->etherType != 0x0800) &&
      (matchCriteria->etherType != 0x86dd) &&
      (matchCriteria->etherType != 0x8847))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid Ethertype. (%04x)\r\n", matchCriteria->etherType);
    return(0);
  }

  if ((0x8847 == matchCriteria->etherType) &&
      (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_FEATURE_MPLSSUPPORTED)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid Ethertype. (%04x)\r\n", matchCriteria->etherType);
    return(0);
  }

  /* determine what entry type we have here */
  if (!(matchCriteria->destMac.addr[0] & 0x01) &&
      (OFDPA_MAC_ADDR_IS_ALL_ONES(matchCriteria->destMacMask.addr)))
  {
    /* Unicast MAC type entry */
    /* MAC must be unicast and MAC mask must be all ones */
    if (matchCriteria->etherType == 0x8847)
    {
      isMPLSUnicastType = 1;
    }
    else
    {
      isIpUnicastType = 1;
    }
  }
  else if ((matchCriteria->etherType == 0x0800) &&
           (ofdbMacCompare(&matchCriteria->destMacMask, &mcastV4MaskForm) == 0) &&
           ofdbMacIsIpv4Multicast(&matchCriteria->destMac))
  {
    /* IPv4 Multicast MAC type entry */
    /* MAC address must be 01-00-5e-0bbbbbbb-xx-xx and MAC mask must be ff-ff-ff-80-00-00 */
    isV4McastType = 1;
  }
  else if ((matchCriteria->etherType == 0x86dd) &&
           (ofdbMacCompare(&matchCriteria->destMacMask, &mcastV6MaskForm) == 0) &&
           ofdbMacIsIpv6Multicast(&matchCriteria->destMac))
  {
    /* IPv6 Multicast MAC type entry */
    /* MAC address must be 33-33-xx-xx-xx-xx and MAC mask must be ff-ff-00-00-00-00 */
    isV6McastType = 1;
  }
  else
  {
    /* not a valid type, we're done */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Destination MAC/Ethertype match criteria does not meet contraints for valid entry type.\r\n", 0);
    return(0);
  }

  /* fully masked port is valid for all entry types */
  if (matchCriteria->inPortMask != OFDPA_INPORT_FIELD_MASK)
  {
    if (isIpUnicastType || isMPLSUnicastType)
    {
      /* for unicast type, port field must be exact match and specify a valid physical port */
      if ((matchCriteria->inPortMask != OFDPA_INPORT_EXACT_MASK) ||
          (OFDB_PORT_TYPE(matchCriteria->inPort) != OFDPA_PORT_TYPE_PHYSICAL) ||
          !ofdbPortIsValid(matchCriteria->inPort))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Unicast type entry with invalid port or port mask. (port:0x%x, portMask:0x%x)\r\n",
                           matchCriteria->inPort, matchCriteria->inPortMask);
        return(0);
      }
    }
    else
    {
      /* for both multicast types, port must be fully wildcarded */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Multicast type entry with invalid port mask. (port:0x%x, portMask:0x%x)\r\n",
                         matchCriteria->inPort, matchCriteria->inPortMask);
      return(0);
    }
  }

  /* check instructions */

  /* if there is a goto table specified, there are rules to follow */
  if (flowData->gotoTableId != 0)
  {
    if (isIpUnicastType &&
        (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING) &&
        (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_L3_TYPE))
    {
      /* goto table can only be Unicast Routing Flow Table */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid unicast goto table.\r\n", 0);
      return(0);
    }
    if (isMPLSUnicastType &&
        (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_0) &&
        (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_1))
    {
      /* goto table can only be MPLS Flow Table */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid MPLS goto table.\r\n", 0);
      return(0);
    }
    if ((isV4McastType || isV6McastType) &&
        (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING))
    {
      /* goto table can only be Multicast Routing Flow Table */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid multicast goto table.\r\n", 0);
      return(0);
    }
  }
  else
  {
    /* TTP does not allow for gotoTableId == 0 */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid goto table.\r\n", 0);
    return(0);
  }

  /* if OUTPUT action is specified, only OUTPUT to CONTROLLER is acceptable */
  if ((flowData->outputPort != 0) &&
      (flowData->outputPort != OFDPA_PORT_CONTROLLER))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Output port specified that is not CONTROLLER port. (outputPort = %d)\r\n",
                       flowData->outputPort);
    return(0);
  }

  if (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
  {
    /* only MPLS type flow entries may specify Color Based Count action */
    if (!isMPLSUnicastType)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Color Based Count action specified for incorrect flow entry type.\r\n", 0);
      return(0);
    }
    if (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_3)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Incorrect Color Based Count action type specified. (colorBasedCountAction = %d)\r\n",
                         flowData->colorBasedCountAction);
      return(0);
    }
    if (ofdbColorBasedCounterRefCountGet(flowData->colorBasedCountId, NULL) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Entry specifies colorBasedCountAction but counter not found in table. "
                         "colorBasedCountAction = %d, colorBasedCountId = 0x%x\r\n",
                         flowData->colorBasedCountAction, flowData->colorBasedCountId);
      return(0);
    }
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowMplsLabelTrustEntryValidate(ofdpaMplsLabelTrustFlowEntry_t *flowData)
{
  ofdpaMplsLabelTrustFlowMatch_t *matchCriteria;
  
  matchCriteria = &flowData->match_criteria;

  if (matchCriteria->mplsTC > OFDPA_MPLS_TC_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS TC (EXP) out of range (%d)\r\n ", matchCriteria->mplsTC);
    return (0);
  }

  /* validate Traffic Class */
  if (flowData->trafficClass > OFDPA_TRAFFIC_CLASS_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Traffic class value out of range. trafficClass = %d\r\n",
                       flowData->trafficClass);
    return(0);
  }
  
  /* Validate Color */
  if (flowData->color >= OFDPA_QOS_RESERVED)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid color (%d).\r\n",
                       flowData->color);
    return(0);
  }

  /* validate goto instruction */
  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_TYPE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid goto table id. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowMplsCommonValidate(ofdpaMplsFlowEntry_t *flowData)
{
  ofdpaMplsFlowMatch_t *matchCriteria;

  matchCriteria = &flowData->match_criteria;

  /* general parameter checks, same for all entry types */

  /* etherType must be 0x8847 */
  if (matchCriteria->etherType != 0x8847)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid Ethertype. (%04x)\r\n", matchCriteria->etherType);
    return(0);
  }

  /* BoS must be 0 or 1 */
  if (1 < matchCriteria->mplsBos)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS BOS value invalid. mplsBos = %d\r\n", matchCriteria->mplsBos);
    return(0);
  }

  /* MPLS label field limited to 20 bits */
  if ((matchCriteria->mplsLabel & 0xfff00000ul) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS label value invalid. mplsLabel = 0x%x\r\n", matchCriteria->mplsLabel);
    return(0);
  }

  if ((flowData->gotoTableId == OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST) &&
     (0 == flowData->qosIndexAction))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS qosIndexAction %d invalid with gotoTableId %d \r\n", 
                       flowData->qosIndexAction, flowData->gotoTableId);
    return(0);
  }

  if ((0 != flowData->qosIndexAction) && 
      (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST) &&
       (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_MPLS_L3_TYPE)) 
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS qosIndexAction %d invalid with gotoTableId %d \r\n", 
                       flowData->qosIndexAction, flowData->gotoTableId);
    return(0);
  }

  /* copyFieldAction should be non zero if qosIndexAction is set */
  if (0 != flowData->qosIndexAction)
  {
    if (0 == flowData->copyFieldAction) 
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS copyFieldAction %d invalid with qosIndexAction %d \r\n", 
                         flowData->copyFieldAction, flowData->qosIndexAction);
      return(0);
    }
  }

  /* lmepId must be configured */
  if (0 != flowData->lmepIdAction)
  {
    ofdpaOamMepConfig_t mepConfig;
    ofdpaOamMegConfig_t megConfig;
    ofdpaOamMipConfig_t mipConfig;
    uint32_t megIndex = 0;
	OFDPA_ERROR_t rc = OFDPA_E_NONE;	
	OFDB_OAM_TYPE_t oamtype= OFDB_OAM_TYPE_OAM_MEP;
	
	rc = ofdbOamTypeGet(flowData->lmepId, &oamtype); 
	if(rc != OFDPA_E_NONE)
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_BASIC, 
			  "lmepId %d not found, rc = %d.\r\n", flowData->lmepId, rc);
		return rc;
	
	}
	
	if(oamtype == OFDB_OAM_TYPE_Y1564)
	{
	  	return 1;
	}
	else if(oamtype == OFDB_OAM_TYPE_Y1564_LB)
	{
	  	return 1;
	}	
	else if(oamtype == OFDB_OAM_TYPE_BFD)
	{
		 return(1);
	}
  		
      /* retrieve MEP table entry */
      if (OFDPA_E_NONE != ofdbOamMepGet(flowData->lmepId, &mepConfig, NULL))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "MPLS flow specifies lmepId not found in MEP table. (lmepId = %d)\r\n",
                           flowData->lmepId);

        /* retrieve MIP table entry */
        if (OFDPA_E_NONE != ofdbOamMipGet(flowData->lmepId, &mipConfig, NULL))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "MPLS flow specifies lmpId not found in MIP table. (lmpId = %d)\r\n",
                             flowData->lmepId);
          return(0);
        }
        else
        {
          megIndex = mipConfig.megIndex;
        }
      }
      else
      {
        megIndex = mepConfig.megIndex;
      }

      /* retrieve MEG table entry */
      if (OFDPA_E_NONE != ofdbOamMegGet(megIndex, &megConfig, NULL, NULL))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Failed retrieving MEG table entry for lmepId = %d (megIndex = %d).\r\n",
                           flowData->lmepId, megIndex);
        return(0);
      }

      if (megConfig.megType != OFDPA_OAM_MEG_TYPE_G8113_1)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Wrong MEG type for lmepId = %d (megIndex = %d, megType = %d).\r\n",
                           flowData->lmepId, megIndex, megConfig.megType);
        return(0);
      }
    }
  

  /* if colorBasedCountAction set, counter must exist in counter table */
  if (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
  {
    if (ofdbColorBasedCounterRefCountGet(flowData->colorBasedCountId, NULL) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Entry specifies colorBasedCountAction but counter not found in table. "
                         "colorBasedCountAction = %d, colorBasedCountId = 0x%x\r\n",
                         flowData->colorBasedCountAction, flowData->colorBasedCountId);
      return(0);
    }
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowMpls0EntryValidate(ofdpaMplsFlowEntry_t *flowData)
{
  ofdpaMplsFlowMatch_t *matchCriteria;

  if (ofdbFlowMplsCommonValidate(flowData) != 1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS Table 0 flow failed common validation.\r\n", 0);
    return 0;
  }
  matchCriteria = &flowData->match_criteria;

  /* check table and entry specific parameter constraints */

  /* MPLS_BOS must be 0 */
  if (matchCriteria->mplsBos != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS_BOS must be 0 for flows in MPLS Flow Table 0.\r\n", 0);
    return 0;
  }

  if ((OFDPA_MPLS_NEXT_LABEL_GAL_EXACT_MASK != matchCriteria->nextLabelIsGalMask) &&
      (OFDPA_MPLS_NEXT_LABEL_GAL_ALL_MASK   != matchCriteria->nextLabelIsGalMask))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid mask (0x%x) for \"next label is GAL\".\r\n", matchCriteria->nextLabelIsGalMask);
    return 0;
  }

  /* check instructions */

  /* must contain the Pop Label action */
  if (0 == flowData->popLabelAction)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS 0 flow must contain Pop Label action. (popLabelAction = %d)\r\n", flowData->popLabelAction);
    return(0);
  }

  /* Modified by JiaoYong, 2016/4/2 
    这个要注掉，因为保护的时候需要靠这个来区分主备对应的epInfo.intf_id
  */  
#if 0
  /* protection index available only for MPLS1 flows */
  if (0 != flowData->protectionIdAction)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "MPLS 0 flow can not contain protection index action.\r\n", 0);
    return(0);
  }
#endif  

  if (0 != (matchCriteria->nextLabelIsGalMask & matchCriteria->nextLabelIsGal))
  {
    /* only valid Goto is MPLS Maintenance Point Flow Table */
    if (OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT != flowData->gotoTableId)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Only valid Goto is MPLS Maintenance Point Flow Table when next label is GAL (gotoTableId: %d)\r\n", flowData->gotoTableId);
      return(0);
    }

    /* must contain LMEP ID action*/
    if (0 == flowData->lmepIdAction)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Must specify LMEP ID action (lmepIdAction = %d)\r\n",
                         flowData->lmepIdAction);
      return(0);
    }

    /* verify that LMEP ID appears in OAM configuration */
    if ((OFDPA_E_NONE != ofdbOamMepGet(flowData->lmepId, NULL, NULL)) &&
        (OFDPA_E_NONE != ofdbOamMipGet(flowData->lmepId, NULL, NULL)))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Specified LMEP ID is not configured in the MEP and MIP able (lmepId = %d)\r\n",
                         flowData->lmepId);
      return(0);
    }

//changed by yjg must not contain pop L2 header action
    if (0 != flowData->popL2HeaderAction)
    {
      /* must not contain pop L2 header action*/
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Must specify pop L2 header action.  (popL2HeaderAction = %d)\r\n",
                         flowData->popL2HeaderAction);
      return(0);
    }

    if (0 == flowData->popCwAction)
    {
      /* must contain Pop CW or ACH action*/
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Must specify pop Pop CW or ACH action. (popCwAction = %d)\r\n",
                         flowData->popCwAction);
      return(0);
    }

  }
  else
  {
    /* only valid Goto is MPLS Table 1 */
    if (OFDPA_FLOW_TABLE_ID_MPLS_1 != flowData->gotoTableId)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS 0 flow must specify Goto Mpls Table 1 except when next label is GAL. (gotoTableId: %d)\r\n", flowData->gotoTableId);
      return(0);
    }
  }

  /* Decrement TTL must be 0 */
  if (flowData->decrementTtlAction != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Decrement TTL must be 0 for flows in MPLS Flow Table 0.\r\n", 0);
    return 0;
  }

  if ((matchCriteria->mplsTtlMask != OFDPA_ZERO_MASK) ||
      (matchCriteria->mplsDataFirstNibbleMask != OFDPA_ZERO_MASK) ||
      (matchCriteria->mplsAchChannelMask != OFDPA_ZERO_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "For MPLS 0 flow entries the following match criteria may only have a mask of all. "
                       "mplsTtlMask = 0x%x mplsDataFirstNibbleMask = 0x%x mplsAchChannelMask = 0x%x \r\n",
                       matchCriteria->mplsTtlMask, matchCriteria->mplsDataFirstNibbleMask,
                       matchCriteria->mplsAchChannelMask);
    return (0);
  }

  /* if colorBasedCountAction set, it must be OFDPA_ACTION_COLOR_BASED_COUNT_2 */
  if ((flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_NONE) &&
      (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_2))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry specifies wrong type of colorBasedCountAction (colorBasedCountAction = %d)\r\n",
                       flowData->colorBasedCountAction);
    return(0);
  }
  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowMpls1EntryValidate(ofdpaMplsFlowEntry_t *flowData)
{
  ofdpaMplsFlowMatch_t *matchCriteria;
  OFDB_OAM_TYPE_t oamtype = OFDB_OAM_TYPE_OAM_MEP;

  if (ofdbFlowMplsCommonValidate(flowData) != 1)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Flow failed common validation.\r\n", 0);
    return 0;
  }

  matchCriteria = &flowData->match_criteria;

  /*
   * Entry type is determined by examining match fields and instruction parameters.
   * The logic checks items in this order: BoS -> BFD -> BHH -> Goto tableID -> groupId
   */

  if (flowData->groupID != 0)
  {
    if (ofdbRefGroupValidate(flowData->groupID) != OFDPA_E_NONE)
    {
      /* referenced group ID not found in tables */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Referenced group entry not valid. groupID = 0x%x\r\n",
                         flowData->groupID);
      return(0);
    }
  }

  /* TBD: Validate the non-applicable fields for all entry types.
     Return error if such fields are passed in the OF message
     for a particular type.
     Fields like inPort, ttl, Nibble, channelType,
     nextLabelGAL, dstIPv6, dstIPv4, ipProto, UdpSrcPort, udpDstPort etc. are
     not applicable to all entry types */

  if (matchCriteria->mplsBos == 0)
  {
    /* differentiate between types with mplsBos == 0:
     *   Pop Tunnel Label    - GoTo Mpls Table 2, no Group Instruction
     *   Penultimate Hop Pop - Goto MPLS Type, Group: MPLS Interface,
     *   Swap Tunnel Label   - Goto MPLS Type, Group: MPLS Swap OR MPLS Fast Failover
     */

    /* examine gotoTableId */
    switch (flowData->gotoTableId)
    {
      case OFDPA_FLOW_TABLE_ID_MPLS_2: /* Pop Tunnel Label and Pop Tunnel Label (MEP Data)*/
        /* must not specify group action */
        if (flowData->groupID != 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Pop Tunnel Label type flow cannot specify Group action.\r\n", 0);
          return 0;
        }
        /* must contain the Pop Label action, setting Ethertype to 0x8847 */
        if ((0 == flowData->popLabelAction) ||
            (0x8847 != flowData->newEtherType))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Pop Tunnel Label flow must specify Pop Label action setting Ethertype to 0x8847. "
                             "popLabelAction = %d, newEtherType = 0x%04x\r\n",
                             flowData->popLabelAction, flowData->newEtherType);
          return(0);
        }

        if ((matchCriteria->mplsTtlMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->mplsDataFirstNibbleMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->mplsAchChannelMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->nextLabelIsGalMask != OFDPA_ZERO_MASK))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "For Pop Tunnel Label type flow the following match criteria may only have a mask of all. "
                             "matchCriteria->mplsTtlMask = 0x%x matchCriteria->mplsDataFirstNibbleMask = 0x%x"
                             "matchCriteria->mplsAchChannelMask = 0x%x matchCriteria->nextLabelIsGalMask = 0x%x\r\n",
                             matchCriteria->mplsTtlMask, matchCriteria->mplsDataFirstNibbleMask,
                             matchCriteria->mplsAchChannelMask, matchCriteria->nextLabelIsGalMask);
          return (0);
        }
        break;

      case OFDPA_FLOW_TABLE_ID_MPLS_TYPE:
      case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
        /* Penultimate Hop Pop or Swap Tunnel Label type */
        if (OFDB_GROUP_IS_MPLS_INTERFACE(flowData->groupID))
        {
          /* Penultimate Hop Pop type flow entry must contain the Pop Label action */
          if (1 != flowData->popLabelAction)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Penultimate Hop Pop type flow must specify Pop Label action. popLabelAction = %d\r\n",
                               flowData->popLabelAction);
            return(0);
          }
          /* must contain the Pop Label action, setting Ethertype to 0x8847 */
          if ((0 == flowData->popLabelAction) ||
              (0x8847 != flowData->newEtherType))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Penultimate Hop Pop flow must specify Pop Label action setting Ethertype to 0x8847. "
                               "popLabelAction = %d, newEtherType = 0x%04x\r\n",
                               flowData->popLabelAction, flowData->newEtherType);
            return(0);
          }
        }
        else if (OFDB_GROUP_IS_MPLS_SWAP_LABEL(flowData->groupID) ||
                 OFDB_GROUP_IS_MPLS_FAST_FAILOVER(flowData->groupID) ||
                 OFDB_GROUP_IS_MPLS_ECMP(flowData->groupID))
        {
          /* Swap Tunnel Label flow, nothing more to check here */
        }
        else
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Penultimate Hop Pop or Swap Tunnel Label type flow specifies "
                             "invalid groupID type/subtype. (groupId = 0x%08x)\r\n",
                             flowData->groupID);
          return 0;
        }

        /* only support 0 or 1 */
        if ((0 != flowData->mplsTypeAction)  && (1 != flowData->mplsTypeAction) )
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "MPLS type action only support 0 or 1.\r\n", 0);
          return 0;
        }

        if ((matchCriteria->mplsTtlMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->mplsDataFirstNibbleMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->mplsAchChannelMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->nextLabelIsGalMask != OFDPA_ZERO_MASK))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "For Penultimate Hop Pop or Swap Tunnel Label type flow the following match criteria may only have a mask of all. "
                             "matchCriteria->mplsTtlMask = 0x%x matchCriteria->mplsDataFirstNibbleMask = 0x%x"
                             "matchCriteria->mplsAchChannelMask = 0x%x matchCriteria->nextLabelIsGalMask = 0x%x\r\n",
                             matchCriteria->mplsTtlMask, matchCriteria->mplsDataFirstNibbleMask,
                             matchCriteria->mplsAchChannelMask, matchCriteria->nextLabelIsGalMask);
          return (0);
        }
        break;

      case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
        /* Pop Tunnel Label (MEP OAM Frame) type;
         * Swap Tunnel Label (MIP OAM Frame);
         * PW VCCV 4 (MEP OAM Frame)
         */
        if (flowData->groupID != 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Swap Tunnel Label (MIP OAM Frame)/Pop Tunnel Label (MEP OAM  Frame)/PW VCCV 4 (MEP OAM Frame) "
                             "flow cannot specify Group action.\r\n", 0);
          return 0;
        }

        if (OFDPA_MPLS_NEXT_LABEL_GAL_ALL_MASK == matchCriteria->nextLabelIsGalMask)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Swap Tunnel Label (MIP OAM Frame)/Pop Tunnel Label (MEP OAM  Frame)/PW VCCV 4 (MEP OAM Frame) "
                             "flow must specify next label GAL match field\r\n",
                             0);
          return(0);
        }
        else
        {
          if (1 != matchCriteria->nextLabelIsGal)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Swap Tunnel Label (MIP OAM Frame)/Pop Tunnel Label (MEP OAM  Frame)/PW VCCV 4 (MEP OAM Frame) "
                               "specify next Label GAL == 1 (nextLabelIsGal = %d)\r\n",
                               matchCriteria->nextLabelIsGal);
            return(0);
          }
        }

        if (0 == flowData->lmepIdAction)
        {
          /* must contain LMEP ID action*/
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Swap Tunnel Label (MIP OAM Frame)/Pop Tunnel Label (MEP OAM  Frame)/PW VCCV 4 (MEP OAM Frame) "
                             "must specify LMEP ID action. "
                             "(lmepIdAction = %d)\r\n",
                             flowData->lmepIdAction);
          return(0);
        }

		if (OFDPA_E_NONE != ofdbOamTypeGet(flowData->lmepId, &oamtype))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Specified LMEP ID is not configured in the MEP and MIP able (lmepId = %d)\r\n",
                             flowData->lmepId);
          return(0);
        }

		/* must contain the Pop Label action, setting Ethertype to 0x8902 */
		if (flowData->popLabelAction == 0 
            ||(flowData->newEtherType != 0x8902 && oamtype != OFDB_OAM_TYPE_BFD)
            ||(flowData->newEtherType != 0x0021 && oamtype == OFDB_OAM_TYPE_BFD))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Swap Tunnel Label (MIP OAM Frame)/Pop Tunnel Label (MEP OAM  Frame)/PW VCCV 4 (MEP OAM Frame) "
                             "must specify Pop Label action setting Ethertype to 0x8902. "
                             "popLabelAction = %d, newEtherType = 0x%04x\r\n",
                             flowData->popLabelAction, flowData->newEtherType);
          return(0);
        }

        if (0 != flowData->popL2HeaderAction)
        {
          /* must not contain pop L2 header action*/
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Swap Tunnel Label (MIP OAM Frame)/Pop Tunnel Label (MEP OAM  Frame)/PW VCCV 4 (MEP OAM Frame) "
                             "must not pop L2 header action. "
                             "(popL2HeaderAction = %d)\r\n",
                             flowData->popL2HeaderAction);
          return(0);
        }

        if (0 == flowData->popCwAction)
        {
          /* must contain Pop CW or ACH action*/
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Swap Tunnel Label (MIP OAM Frame)/Pop Tunnel Label (MEP OAM  Frame)/PW VCCV 4 (MEP OAM Frame) "
                             "must pop Pop CW or ACH action. "
                             "(popCwAction = %d)\r\n",
                             flowData->popCwAction);
          return(0);
        }

        if ((matchCriteria->mplsTtlMask != OFDPA_ZERO_MASK) &&
            (matchCriteria->mplsTtlMask != OFDPA_MPLS_TTL_EXACT_MASK))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "mplsTtlMask for Swap Tunnel Label (MIP OAM Frame)/Pop Tunnel Label (MEP OAM  Frame)/ "
                             "PW VCCV 4 (MEP OAM Frame) flow should be either all or exact.\r\n", 0);
          return (0);
        }
        break;

      default:
        /* invalid gotoTableId */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Flow specifies mplsBos == 0 with invalid gotoTableId. (gotoTableId = %d)\r\n",
                           flowData->gotoTableId);
        return 0;
    }

    /* Only support OFDPA_ACTION_COLOR_BASED_COUNT in SPTN extensions */
#if 0    
    /*
     * All entry types matching BoS == 0 must specify OFDPA_ACTION_COLOR_BASED_COUNT_1, if any,
     * with the exception of 'PW VCCV 4 (PW Terminate OAM MEP Control)' which must specify
     * OFDPA_ACTION_COLOR_BASED_COUNT. This flow entry type is distinguished by the fact
     * that it must contain a SET_FIELD TUNNEL_ID action.
     */
    if (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
    {
      if (((flowData->tunnelIdAction == 0) && (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_1)) ||
          ((flowData->tunnelIdAction != 0) && (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT)))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Incorrect Color Based Count action type specified. (colorBasedCountAction = %d)\r\n",
                           flowData->colorBasedCountAction);
        return(0);
      }
    }
#endif    
  }
  else  /* mplsBos == 1 */
  {
    switch (flowData->gotoTableId)
    {
      case OFDPA_FLOW_TABLE_ID_MPLS_TYPE:
      case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
        if (OFDB_GROUP_IS_MPLS_SWAP_LABEL(flowData->groupID) ||
            OFDB_GROUP_IS_MPLS_FAST_FAILOVER(flowData->groupID) ||
            OFDB_GROUP_IS_MPLS_ECMP(flowData->groupID))
        {
          /* Swap Pseudo-wire Label */
          /* set MPLS type action must not be included */
          if (0 != flowData->mplsTypeAction)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "MPLS type action should be 0.\r\n", 0);
            return 0;
          }
        }
        else if ((0 != flowData->groupID) && 
                 (OFDB_GROUP_IS_MPLS_L2_TAG(flowData->groupID) ||
                 (OFDB_GROUP_TYPE(flowData->groupID) == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) ||
                 (OFDB_GROUP_TYPE(flowData->groupID) == OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE)))
        {
          /* L2 VPWS Terminate (a.k.a. L2 Switch VPWS) */

          /* Must contain the Pop Label action.  Don't care what newEtherType is as it is ignored. */
          if (0 == flowData->popLabelAction)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "L2 VPWS Terminate flow must specify Pop Label action.  popLabelAction = %d\r\n",
                               flowData->popLabelAction);
            return(0);
          }

          /* must contain the Pop Outer L2 Header action */
          if (0 == flowData->popL2HeaderAction)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "L2 VPWS Terminate flow must specify Pop Outer L2 Header action. "
                               "popL2HeaderAction = %d\r\n",
                               flowData->popL2HeaderAction);
            return(0);
          }

          /* must contain the MPLS L2 Port action */
          if (0 == flowData->mplsL2PortAction)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "L2 VPWS Terminate flow must specify MPLS L2 Port action (mplsL2PortAction = %d)\r\n",
                               flowData->mplsL2PortAction);
            return(0);
          }
          /* check MPLS_L2_Port is network type */
          if ((OFDPA_MPLS_L2_PORT_TYPE_VPWS_NW) != OFDB_MPLS_L2_PORT_TYPE(flowData->mplsL2Port))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Invalid MPLS L2 Port type for L2 VPWS Terminate flow. Must specify VPWS Network Port. "
                               "mplsL2Port = 0x%8x\r\n",
                               flowData->mplsL2Port);
            return(0);
          }

          /* must contain the Tunnel ID action */
          if (0 == flowData->tunnelIdAction)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "L2 VPWS Terminate flow must specify Tunnel ID action (tunnelIdAction = %d)\r\n",
                               flowData->tunnelIdAction);
            return(0);
          }
          else
          {
            /* check Tunnel_ID is MPLS type */
            if (OFDB_TUNNELID_TYPE(flowData->tunnelId) != OFDPA_TUNNELID_TYPE_MPLS_TP)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "L2 VPWS Terminate flow specifies tunnelId of wrong type. (tunnelId = 0x%x\r\n",
                                 flowData->tunnelId);
              return(0);
            }
          }

          /* set MPLS type action must be included */
          if (0 == flowData->mplsTypeAction)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "MPLS type action should be 1.\r\n", 0);
            return 0;
          }

          /* Check if MPLS type is valid or not. */
          if ((flowData->mplsType < OFDPA_MPLS_TYPE_VPWS) ||
              (flowData->mplsType > OFDPA_MPLS_TYPE_L3_PHP))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Invalid MPLS type.\r\n", 0);
            return 0;
          }

          /* OAM_LM_Rx_Count (LMEP_ID, Traffic Class) for L2 Switch VPWS (PW Terminate) (MEP Data) ?*/
        }
        else
        {
          /* flow does not match any type */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Invalid groupID (groupID == 0x%08x) with gotoTableId == OFDPA_FLOW_TABLE_ID_MPLS_TYPE for BOS == 1.\r\n",
                             flowData->groupID);
          return 0;
        }

        if ((matchCriteria->mplsTtlMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->mplsDataFirstNibbleMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->mplsAchChannelMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->nextLabelIsGalMask != OFDPA_ZERO_MASK))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "For Swap Pseudo-wire Label/L2 VPWS Terminate/L2 Switch VPWS type flow the following match "
                             "criteria may only have a mask of all. "
                             "matchCriteria->mplsTtlMask = 0x%x matchCriteria->mplsDataFirstNibbleMask = 0x%x"
                             "matchCriteria->mplsAchChannelMask = 0x%x matchCriteria->nextLabelIsGalMask = 0x%x\r\n",
                             matchCriteria->mplsTtlMask, matchCriteria->mplsDataFirstNibbleMask,
                             matchCriteria->mplsAchChannelMask, matchCriteria->nextLabelIsGalMask);
          return (0);
        }
        break;

      case OFDPA_FLOW_TABLE_ID_MPLS_L3_TYPE:
        /* L3 VPN Forward */
        if (0 != flowData->groupID)
        {
          /* set MPLS type action must be included */
          if (0 == flowData->mplsTypeAction)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "MPLS type action should be 1.\r\n", 0);
            return 0;
          }

          /* Check if MPLS type is valid or not. */
          if (flowData->mplsType != OFDPA_MPLS_TYPE_L3_PHP)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Invalid MPLS type, mpls type should be L3 PHP.\r\n", 0);
            return 0;
          }
        }
        else /* L3 VPN Route*/
        {
          /* must not include MPLS Data first nibble or MPLS ACH channel match types*/
          if ((matchCriteria->mplsDataFirstNibbleMask != OFDPA_MPLS_DATA_FIRST_NIBBLE_ALL_MASK) ||
              (matchCriteria->mplsAchChannelMask != OFDPA_MPLS_CHANNEL_TYPE_ALL_MASK))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "L3 VPN Route flow must not specify MPLS Data first nibble or "
                               "MPLS ACH channel match types.\r\n", 0);
            return(0);
          }

          /* must contain set field VRF */
          if (0 == flowData->vrfAction)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "L3 VPN Route flow must specify VRF action. "
                               "vrfAction = %d\r\n",
                               flowData->vrfAction);
            return(0);
          }
        }

        if ((matchCriteria->mplsTtlMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->mplsDataFirstNibbleMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->mplsAchChannelMask != OFDPA_ZERO_MASK) ||
            (matchCriteria->nextLabelIsGalMask != OFDPA_ZERO_MASK))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "For L3 VPN Route or L3 VPN Forward type flow the following match criteria may only have a mask of all. "
                             "matchCriteria->mplsTtlMask = 0x%x matchCriteria->mplsDataFirstNibbleMask = 0x%x"
                             "matchCriteria->mplsAchChannelMask = 0x%x matchCriteria->nextLabelIsGalMask = 0x%x\r\n",
                             matchCriteria->mplsTtlMask, matchCriteria->mplsDataFirstNibbleMask,
                             matchCriteria->mplsAchChannelMask, matchCriteria->nextLabelIsGalMask);
          return(0);
        }

        if (1 == flowData->popLabelAction)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "L3 VPN Route or L3 VPN Forward type flow must not specify Pop Label action. "
                             "popLabelAction = %d\r\n",
                             flowData->popLabelAction);
          return(0);
        }
        break;

      case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
        /* Swap PW Label BHH MS-PW OAM (MIP Control);
         * PW BHH VCCV 1 (PW Terminate) (MEP Control);
         * PW BHH VCCV 3 (MIP Control);
         * Section OAM
         */
        if (flowData->groupID != 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM termination flow type cannot specify Group action.\r\n", 0);
          return 0;
        }

		if (OFDPA_E_NONE != ofdbOamTypeGet(flowData->lmepId, &oamtype))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Specified LMEP ID is not configured in the MEP and MIP table (lmepId = %d)\r\n",
                             flowData->lmepId);
          return(0);
        }

        if (OFDPA_MPLS_DATA_FIRST_NIBBLE_EXACT_MASK != matchCriteria->mplsDataFirstNibbleMask)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM termination flow type must specify MPLS Data First Nibble match.\r\n",
                             0);
          return(0);
        }
        else
        {
          if (1 != matchCriteria->mplsDataFirstNibble)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Invalid MPLS Data First Nibble match value for OAM termination flow type."
                               "(mplsDataFirstNibble = %d).\r\n",
                               matchCriteria->mplsDataFirstNibble);
            return(0);
          }
        }

        if (matchCriteria->mplsAchChannelMask != OFDPA_MPLS_CHANNEL_TYPE_EXACT_MASK)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM termination flow type must specify MPLS ACH Channel match. \r\n",
                             0);
          return(0);
        }
        else
        {
          if ((matchCriteria->mplsAchChannel != OFDPA_MPLS_ACH_CHANNEL_TYPE	&& oamtype != OFDB_OAM_TYPE_BFD)
		  	|| (matchCriteria->mplsAchChannel != OFDPA_MPLS_BFD_ACH_CHANNEL_TYPE && oamtype == OFDB_OAM_TYPE_BFD))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Invalid MPLS ACH Channel match (0x%x) for OAM termination flow type.\r\n",
                               matchCriteria->mplsAchChannel);
            return(0);
          }
        }

        if (0 == flowData->lmepIdAction)
        {
          /* must contain LMEP ID action*/
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM termination flow type must specify LMEP ID action. "
                             "(lmepIdAction = %d)\r\n",
                             flowData->lmepIdAction);
          return(0);
        }


        /* must contain the Pop Label action, setting Ethertype to 0x8902 */
        if (flowData->popLabelAction == 0 ||
            (flowData->newEtherType != 0x8902 && oamtype != OFDB_OAM_TYPE_BFD)||
            (flowData->newEtherType != 0x0021 && oamtype == OFDB_OAM_TYPE_BFD))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM termination flow type must specify Pop Label action setting Ethertype to 0x8902. "
                             "popLabelAction = %d, newEtherType = 0x%04x\r\n",
                             flowData->popLabelAction, flowData->newEtherType);
          return(0);
        }

        if (0 != flowData->popL2HeaderAction)
        {
          /* must not contain pop L2 header action*/
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM termination flow type must not have pop L2 header action. "
                             "(lmepIdAction = %d)\r\n",
                             flowData->popL2HeaderAction);
          return(0);
        }

        if (0 == flowData->popCwAction)
        {
          /* must contain Pop CW or ACH action*/
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM termination flow type must have pop Pop CW or ACH action. "
                             "(lmepIdAction = %d)\r\n",
                             flowData->popCwAction);
          return(0);
        }

        if (matchCriteria->ipProtoMask != 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM Termination flow type cannot match an IP protocol.\r\n", 0);
          return(0);
        }

        if (flowData->copyTtlInAction != 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM Termination flow type cannot copy the TTL to an inner label.\r\n", 0);
          return(0);
        }

        if (flowData->copyTcInAction != 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM Termination flow type cannot copy the traffic class to an inner label.\r\n", 0);
          return(0);
        }

        if (flowData->vrfAction != 0)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "OAM Termination flow type cannot set a VRF.\r\n", 0);
          return(0);
        }

        /* If section OAM */
        if (OFDPA_MPLS_LABEL_GAL == matchCriteria->mplsLabel)
        {
          if (OFDPA_INPORT_EXACT_MASK != matchCriteria->inPortMask)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Section OAM flow requires an inPort match.\r\n", 0);
            return(0);
          }

          if (OFDB_PORT_TYPE(matchCriteria->inPort) != OFDPA_PORT_TYPE_PHYSICAL)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Section OAM flow inPort match must match a physical port.\r\n", 0);
            return(0);
          }

          if (matchCriteria->mplsTtlMask != 0)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Section OAM flow must match any TTL.\r\n", 0);
            return(0);
          }

          if (matchCriteria->nextLabelIsGalMask != 0)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Section OAM flow cannot match next label is GAL.\r\n", 0);
            return(0);
          }

          if (flowData->mplsL2PortAction != 0)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Section OAM cannot set an MPLS L2 Port.\r\n", 0);
            return(0);
          }

          if (flowData->tunnelIdAction != 0)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Section OAM cannot set a tunnel ID.\r\n", 0);
            return(0);
          }
        }

        if ((matchCriteria->mplsTtlMask != OFDPA_ZERO_MASK) &&
            (matchCriteria->mplsTtlMask != OFDPA_MPLS_TTL_EXACT_MASK))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "mplsTtlMask for Swap PW Label BHH MS-PW OAM (MIP Control)/PW BHH VCCV 1 (PW Terminate) (MEP Control) "
                             "/PW BHH VCCV 3 (MIP Control) type flow should be either all or exact.\r\n", 0);
          return (0);
        }
        break;

      default:
        /* invalid gotoTableId */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Flow entry with mplsBos == 1 specifies "
                           "invalid gotoTableId. (gotoTableId = %d)\r\n",
                           flowData->gotoTableId);
        return 0;

    }

    /* all entry types matching BoS == 1 must specify OFDPA_ACTION_COLOR_BASED_COUNT, if any */
    if (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
    {
      if (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Incorrect Color Based Count action type specified. (colorBasedCountAction = %d)\r\n",
                           flowData->colorBasedCountAction);
        return(0);
      }
    }
  }

  /* Modified by JiaoYong, 2016/11/7 
    根据TTP，tunnelId可以不下发，而protection_index在保护时需要下发，不保护不下
    tunnelId不下发时，根据标签查找tunnelId，再找VPN信息
  */
  #if 0
  if (flowData->tunnelIdAction == 0 && flowData->protectionIdAction)
  {
    /* Only PW terminate flow entry types can contain protection id action */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Only PW terminate flow entry types can contain protection id action\r\n", 0);
    return 0;
  }
  #endif

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowUnicastRoutingEntryValidate(ofdpaUnicastRoutingFlowEntry_t *flowData)
{
  ofdpaUnicastRoutingFlowMatch_t *matchCriteria;

  matchCriteria = &flowData->match_criteria;

  switch (matchCriteria->etherType)
  {
    case 0x0800:
      /* must be unicast address */
      if (!(matchCriteria->dstIp4 < 0xe0000000UL))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "IPv4 destination address not unicast. (0x%x)\r\n",
                           matchCriteria->dstIp4);
        return(0);
      }

      if (matchCriteria->dstIp4Mask != 0)
      {
        if (!ofdbIpv4MaskValidate(&matchCriteria->dstIp4Mask))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "IPv4 destination address mask invalid. (0x%x)\r\n",
                             matchCriteria->dstIp4Mask);
          return(0);
        }
      }
      break;
    case 0x86dd:
      /* must be unicast address */
      if (IN6_IS_ADDR_MULTICAST(&matchCriteria->dstIp6))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "IPv6 destination address not unicast.\r\n", 0);
        return(0);
      }

      if (!IN6_IS_ADDR_UNSPECIFIED(&matchCriteria->dstIp6Mask))
      {
        if(!ofdbIpv6MaskValidate(&matchCriteria->dstIp6Mask))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "IPv6 destination address mask invalid.\r\n", 0);
          return(0);
        }
      }
      break;
    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid etherType. (%04x)\r\n",
                         matchCriteria->etherType);
      return(0);
      break;
  }

  /* check instructions */

  /* Apply actions */
  /* -- none */

  /* Write actions */

  /* if there is a goto table specified, there are rules to follow */
  if (flowData->gotoTableId != 0)
  {
    if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_ACL_POLICY)
    {
      /* goto table can only be ACL Policy Flow Table */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid goto table. (%d)\r\n",
                         flowData->gotoTableId);
      return(0);
    }

    if (0 != flowData->groupID)
    {
      /* must specify GROUP ID that is valid L3 Unicast Group Type */
      if ((OFDB_GROUP_TYPE(flowData->groupID) != OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST) &&
          (OFDB_GROUP_TYPE(flowData->groupID) != OFDPA_GROUP_ENTRY_TYPE_L3_ECMP) &&
          (!(OFDB_GROUP_IS_MPLS_L3_VPN_LABEL(flowData->groupID))))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Referenced group entry not valid type. (%d)\r\n",
                           flowData->groupID);
        return(0);
      }
      if (ofdbRefGroupValidate(flowData->groupID) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Referenced group entry not valid. (%d)\r\n",
                           flowData->groupID);
        return(0);
      }

      if (flowData->outputPort != 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Output write action is ignored -- group write action takes precedence.\r\n", 0);
      }
    }
    else if ((flowData->outputPort != 0) &&
             (flowData->outputPort != OFDPA_PORT_CONTROLLER))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid write action output port %d\r\n",
                         flowData->outputPort);
      return(0);
    }
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowMulticastRoutingEntryValidate(ofdpaMulticastRoutingFlowEntry_t *flowData)
{
  ofdpaMulticastRoutingFlowMatch_t *matchCriteria;

  matchCriteria = &flowData->match_criteria;

  if (!dpaVlanIsValid(matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid VLAN ID. (0x%04x)\r\n", matchCriteria->vlanId);
    return(0);
  }

  switch (matchCriteria->etherType)
  {
    case 0x0800:
      /* destination address must be multicast */
      if (!IN_MULTICAST(matchCriteria->dstIp4))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "IPv4 destination address not multicast. (0x%x)\r\n",
                           matchCriteria->dstIp4);
        return(0);
      }
      switch (matchCriteria->srcIp4Mask)
      {
        case OFDPA_IPV4_ADDR_EXACT_MASK:
          /* if source address provided, must be exact match on unicast address */
          if (!(matchCriteria->srcIp4 < 0xe0000000UL))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "IPv4 source address not unicast. (0x%x)\r\n",
                               matchCriteria->srcIp4);
            return(0);
          }
          break;
        case OFDPA_IPV4_ADDR_FIELD_MASK:
          /* source address may be field masked */
          break;
        default:
          /* cannot be neither */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "IPv4 source address mask invalid. (0x%x)\r\n",
                             matchCriteria->srcIp4Mask);
          return(0);
          break;
      }

      /* Unwanted match criteria check*/
      if (!IN6_IS_ADDR_UNSPECIFIED(&matchCriteria->srcIp6Mask))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "IPv6 source address mask should be NULL for IPv4 type flows. \r\n", 0);
        return(0);
      }
      break;

    case 0x86dd:
      /* destination address must be multicast address */
      if (!IN6_IS_ADDR_MULTICAST(&matchCriteria->dstIp6))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "IPv6 destination address not multicast.\r\n", 0);
        return(0);
      }

      if (IN6_IS_ADDR_UNSPECIFIED(&matchCriteria->srcIp6Mask))
      {
        /* source address may be field masked */
      }
      else if (IN6_IS_ADDR_ALL_ONES(&matchCriteria->srcIp6Mask))
      {
        /* if source address provided, must be exact match on unicast address */
        if (IN6_IS_ADDR_MULTICAST(&matchCriteria->srcIp6))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "IPv6 source address not unicast.\r\n", 0);
          return(0);
        }
      }
      else
      {
        /* cannot be neither */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "IPv6 source address mask invalid.\r\n", 0);
        return(0);
      }

      /* Unwanted match criteria check*/
      if (matchCriteria->srcIp4Mask != 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "IPv4 source address mask should be NULL for IPv6 type flows. \r\n", 0);
        return(0);
      }

      break;

    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid etherType. (%04x)\r\n",
                         matchCriteria->etherType);
      return(0);
      break;
  }

  /* check instructions */

  /* if there is a goto table specified, there are rules to follow */
  if (flowData->gotoTableId != 0)
  {
    if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_ACL_POLICY)
    {
      /* goto table can only be ACL Policy Flow Table */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid goto table. (%d)\r\n", flowData->gotoTableId);
      return(0);
    }

    /* must specify GROUP ID that is valid L3 Multicast Group Type */
    if (OFDB_GROUP_TYPE(flowData->groupID) != OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Referenced group entry not valid type. (%d)\r\n",
                         flowData->groupID);
      return(0);
    }
    if (ofdbRefGroupValidate(flowData->groupID) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Referenced group entry not valid. (%d)\r\n",
                         flowData->groupID);
      return(0);
    }
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowBridgingEntryValidate(ofdpaBridgingFlowEntry_t *flowData)
{
  ofdpaBridgingFlowMatch_t *matchCriteria;
  uint32_t isBridgingRule = 0;
  uint32_t isTunnelRule = 0;
  uint32_t isUnicastType = 0;
  uint32_t isMcastType = 0;
  uint32_t isDlfType = 0;
  uint32_t matchFound = 0;
  ofdpaFlowEntry_t flow;
  uint32_t portTenantTunnelId = 0;
  ofdbPortInfo_t portInfo;
  uint32_t foundRequiredIngressPortFlow = 0;
  uint32_t flags;
  OFDPA_GROUP_ENTRY_TYPE_t groupType;

  matchCriteria = &flowData->match_criteria;

  /* the valid values for VLAN ID mask are exact or nothing */
  if ((matchCriteria->vlanIdMask != (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)) &&
      (matchCriteria->vlanIdMask != OFDPA_VID_FIELD_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid VLAN ID mask. Must be an exact match or 0. (vlanIdMask = 0x%x)\r\n",
                       matchCriteria->vlanIdMask);
    return(0);
  }
  /* the valid values for Tunnel ID mask are exact or nothing */
  if ((matchCriteria->tunnelIdMask != OFDPA_TUNNEL_ID_EXACT_MASK) &&
      (matchCriteria->tunnelIdMask != OFDPA_TUNNEL_ID_FIELD_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Tunnel ID mask. Must be an exact match or 0. (tunnelIdMask = 0x%x)\r\n",
                       matchCriteria->tunnelIdMask);
    return(0);
  }

  /* one and only one of VLAN ID or Tunnel ID must be matched */
  if ((matchCriteria->vlanIdMask == (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)) &&
      (matchCriteria->tunnelIdMask == OFDPA_TUNNEL_ID_EXACT_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Rule may not match VLAN ID and Tunnel ID in same entry.\r\n", 0);
    return(0);
  }
  if ((matchCriteria->vlanIdMask == OFDPA_VID_FIELD_MASK) &&
      (matchCriteria->tunnelIdMask == OFDPA_TUNNEL_ID_FIELD_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Either VLAN ID or Tunnel ID must be matched.\r\n", 0);
    return(0);
  }

  /* if VLAN ID is not 0, this is a bridging rule */
  /* if Tunnel ID is not 0, this is a tunnel rule */
  if (matchCriteria->vlanIdMask != OFDPA_VID_FIELD_MASK)
  {
    /* VLAN ID match field must have OFDPA_VID_PRESENT set*/
    if ((matchCriteria->vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN ID match field must have OFDPA_VID_PRESENT set. (%04x)\r\n",
                         matchCriteria->vlanId);
      return(0);
    }
    /* VLAN, if not masked, must be valid */
    if (!dpaVlanIsValid(matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
    {
      /* not a valid VLAN ID, we're done */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid VLAN ID. (%04x)\r\n",
                         matchCriteria->vlanId);
      return(0);
    }

    isBridgingRule = 1;
  }

  if ((matchCriteria->tunnelIdMask != OFDPA_TUNNEL_ID_FIELD_MASK) &&
      (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN)))
  {
    if (ofdbTenantGet(matchCriteria->tunnelId, NULL, NULL) != OFDPA_E_NONE)
    {
      /* cannot find tenant for tunnelId, we're done */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "No tenant entry for tunnelId. (%d)\r\n", matchCriteria->tunnelId);
      return(0);
    }

    isTunnelRule = 1;
  }

  /* determine what entry type we have here */
  if (!(matchCriteria->destMac.addr[0] & 0x01) &&
      (OFDPA_MAC_ADDR_IS_ALL_ONES(matchCriteria->destMacMask.addr)))
  {
    /* Unicast type entry */
    /* MAC must be unicast and MAC mask must be all ones */
    isUnicastType = 1;
  }
  else if ((matchCriteria->destMac.addr[0] & 0x01) &&
           (OFDPA_MAC_ADDR_IS_ALL_ONES(matchCriteria->destMacMask.addr)))
  {
    /* Multicast type entry */
    /* MAC must be multicast and MAC mask must be all ones */
    isMcastType = 1;
  }
  else if (OFDPA_MAC_ADDR_IS_NULL(matchCriteria->destMacMask.addr))
  {
    /* DLF type entry */
    /* destination MAC must be field masked (all 0) */
    isDlfType = 1;
  }
  else
  {
    /* not a valid type, we're done */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid MAC address/mask\r\n", 0);
    return(0);
  }

  /* cannot accept Tunnel Bridging entry unless an appropriate entry is in the Ingress Port flow table */
  if (isTunnelRule)
  {
    memset(&flow, 0, sizeof(flow));
    flow.tableId = OFDPA_FLOW_TABLE_ID_INGRESS_PORT;

    /* search the Ingress Port flows for an entry matching on the tunnelId used in the Bridging flow */
    /*
     * Both an Ingress Port table that matches the tunnelId exactly, or one that matches any tunnelId of
     * type OFDPA_TUNNELID_TYPE_DATA_CENTER_OVERLAY are acceptable. Note that the second of these forms
     * is retained for backward compatibility with pre-OFDPA 2.0 configurations
     */
    if (ofdbFlowGet(&flow, NULL, NULL) != OFDPA_E_NONE)
    {
      if (ofdbFlowNextGet(&flow, &flow, NULL, NULL) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Attempt to add Tunnel Bridging flow without required Ingress Port flow present.\r\n", 0);
        return(0);
      }
    }

    do
    {
      if ((flow.flowData.ingressPortFlowEntry.match_criteria.tunnelId & flow.flowData.ingressPortFlowEntry.match_criteria.tunnelIdMask) ==
          (matchCriteria->tunnelId & flow.flowData.ingressPortFlowEntry.match_criteria.tunnelIdMask))
      {
        foundRequiredIngressPortFlow = 1;
      }
    } while ((foundRequiredIngressPortFlow == 0) && (ofdbFlowNextGet(&flow, &flow, NULL, NULL) == OFDPA_E_NONE));

    if (foundRequiredIngressPortFlow == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Attempt to add Tunnel Bridging flow without required Ingress Port flow present.\r\n", 0);
      return(0);
    }
  }

  /* check instructions */

  /* if apply action is specified, only OUTPUT to CONTROLLER is acceptable */
  if ((flowData->outputPort != 0) &&
      (flowData->outputPort != OFDPA_PORT_CONTROLLER))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid apply action output port %d\r\n", flowData->outputPort);
    return(0);
  }

  /* either group or logical port parameter must be specified */
  if ((flowData->groupID == 0 && flowData->tunnelLogicalPort == 0) ||
      (flowData->groupID != 0 && flowData->tunnelLogicalPort != 0))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid write action parameters (groupID = 0x%x, tunnelLogicalPort = 0x%x)\r\n",
                       flowData->groupID, flowData->tunnelLogicalPort);
    return(0);
  }

  if (isBridgingRule)
  {
    /* Specified Group ID must exist and be of appropriate type */
    if (ofdbRefGroupValidate(flowData->groupID) != OFDPA_E_NONE)
    {
      /* referenced group ID not found in tables */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Referenced group entry not valid. groupID = 0x%x\r\n",
                         flowData->groupID);
      return(0);
    }

    groupType = OFDB_GROUP_TYPE(flowData->groupID);

    if ((isUnicastType && ((groupType != OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE) &&
                           (groupType != OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE))) ||
        (isMcastType   && (groupType != OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST)) ||
        (isDlfType     && (groupType != OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD)))
    {
      /* referenced group ID is wrong type */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "MAC type (%s) does not match group type (%d)\r\n",
                         isUnicastType ? "unicast" : (isMcastType ? "multicast" : (isDlfType ? "DLF" : "Unknown!")),
                         groupType);
      return(0);
    }
  }

  if (isTunnelRule)
  {
    if (isUnicastType)
    {
      /* unicast destination to overlay logical port (access or unicast) */
      if (OFDB_PORT_TYPE(flowData->tunnelLogicalPort) != OFDPA_PORT_TYPE_LOGICAL_TUNNEL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Write action output port not a logical tunnel type (0x%x)\r\n",
                           flowData->tunnelLogicalPort);
        return(0);
      }

      if ((ofdbPortGet(flowData->tunnelLogicalPort, &portInfo, &flags) != OFDPA_E_NONE) ||
          (flags & OFDB_PORT_DELETED))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Write action output port not found in port table. (0x%x)\r\n",
                           flowData->tunnelLogicalPort);
        return(0);
      }

      if ((portInfo.tunnelPortConfig.type != OFDPA_TUNNEL_PORT_TYPE_ENDPOINT) &&
          (portInfo.tunnelPortConfig.type != OFDPA_TUNNEL_PORT_TYPE_ACCESS))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Write action output port in Unicast Bridging rule not valid type. (%d)\r\n",
                           portInfo.tunnelPortConfig.type);
        return(0);
      }

      /* must be a logical port associated with the matched tunnel ID */
      matchFound = 0;
      portTenantTunnelId = 0;
      while (ofdbPortTenantNextGet(flowData->tunnelLogicalPort, portTenantTunnelId, &portTenantTunnelId) == OFDPA_E_NONE)
      {
        if (portTenantTunnelId == matchCriteria->tunnelId)
        {
          matchFound = 1;
          break;
        }
      }
      if (!matchFound)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Write action output port is not configured for tenant with flow match tunnelId. flow tunnelId - %d, port - 0x%x\r\n",
                           matchCriteria->tunnelId,
                           flowData->tunnelLogicalPort);
        return(0);
      }
    }
    else
    {
      /* Specified Group ID must exist and be of appropriate type */
      if (ofdbRefGroupValidate(flowData->groupID) != OFDPA_E_NONE)
      {
        /* referenced group ID not found in tables */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Referenced group entry not valid. groupID = 0x%x\r\n",
                           flowData->groupID);
        return(0);
      }

      if ((isMcastType &&
          (OFDB_GROUP_TUNNEL_SUBTYPE(flowData->groupID) != OFDPA_L2_OVERLAY_MULTICAST_MULTICAST_TUNNEL) &&
          (OFDB_GROUP_TUNNEL_SUBTYPE(flowData->groupID) != OFDPA_L2_OVERLAY_MULTICAST_UNICAST_TUNNEL)) ||
          (isDlfType &&
          (OFDB_GROUP_TUNNEL_SUBTYPE(flowData->groupID) != OFDPA_L2_OVERLAY_FLOOD_UNICAST_TUNNEL) &&
          (OFDB_GROUP_TUNNEL_SUBTYPE(flowData->groupID) != OFDPA_L2_OVERLAY_FLOOD_MULTICAST_TUNNEL)))
      {
        /* referenced group ID is wrong type */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Tunnel rule MAC type (%s) does not match group sub-type (%d)\r\n",
                           isMcastType ? "multicast" : "DLF",
                           OFDB_GROUP_TUNNEL_SUBTYPE(flowData->groupID));
        return(0);
      }
    }
  }
  /* if there is a goto table specified, there are rules to follow */
  if ((flowData->gotoTableId != 0) &&
      (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_ACL_POLICY))
  {
    /* goto table can only be ACL Policy Flow Table */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid goto table ID %d\r\n", flowData->gotoTableId);
    return(0);
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowPolicyAclEntryValidate(ofdpaPolicyAclFlowEntry_t *flowData)
{
  ofdpaPolicyAclFlowMatch_t *matchCriteria;
  ofdbPortInfo_t portInfo;
  ofdpaGroupBucketEntry_t groupBucket;
  OFDB_ENTRY_FLAG_t flags;
  uint32_t isVlanRule = 0;
  uint32_t isTenantRule = 0;
  uint32_t isMplsL2Rule = 0;
  ofdpaTunnelTenantConfig_t tunnelTenantConfig;
  const struct in6_addr in6addr_null = { { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } } };
  ofdpaMeterEntryStats_t meterStats;
  uint32_t groupId;

  matchCriteria = &flowData->match_criteria;

  /* set flags indicating entry types */
  if (0 == matchCriteria->tunnelId &&
      OFDPA_TUNNEL_ID_FIELD_MASK == matchCriteria->tunnelIdMask)
  {
    isVlanRule = 1;
  }
  else if (OFDPA_TUNNEL_ID_EXACT_MASK == matchCriteria->tunnelIdMask &&
           OFDPA_TUNNELID_TYPE_DATA_CENTER_OVERLAY == OFDB_TUNNELID_TYPE(matchCriteria->tunnelId) &&
           (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN)))
  {
    isTenantRule = 1;
  }
  else if (OFDPA_TUNNEL_ID_EXACT_MASK == matchCriteria->tunnelIdMask &&
           OFDPA_TUNNELID_TYPE_MPLS_TP == OFDB_TUNNELID_TYPE(matchCriteria->tunnelId))
  {
    isMplsL2Rule = 1;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid match criteria values: vlanId: %d, tunnelId: 0x%x, tunnelIdMask: 0x%x\r\n",
                       matchCriteria->vlanId, matchCriteria->tunnelId, matchCriteria->tunnelIdMask);
    return(0);
  }

  if ((OFDPA_ETHERTYPE_EXACT_MASK != matchCriteria->etherTypeMask) &&
      (OFDPA_ETHERTYPE_ALL_MASK   != matchCriteria->etherTypeMask))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Ethertype mask: 0x%x\r\n",
                       matchCriteria->etherTypeMask);
    return(0);
  }

  /* validate inPortMask data */
  switch (matchCriteria->inPortMask)
  {
    case OFDPA_INPORT_FIELD_MASK:
      /* flow matches all ports  */
      /* only physical ports are supported */
      break;

    case OFDPA_INPORT_EXACT_MASK:
      /* if single port specified, inPort must be a valid interface */
      if (OFDPA_PORT_TYPE_PHYSICAL == OFDB_PORT_TYPE(matchCriteria->inPort) &&
          !ofdbPortIsValid(matchCriteria->inPort))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid port. (inPort:%d)\r\n",
                           matchCriteria->inPort);
        return(0);
      }
      break;

    case OFDPA_INPORT_TYPE_MASK:
      /* flow matches all ports of a certain type */
      /* only physical ports are supported */
      if (OFDB_PORT_TYPE(matchCriteria->inPort) != OFDPA_PORT_TYPE_PHYSICAL)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Flow matching all ports of unsupported type. (inPort:%d, inPortMask:0x%x)\r\n",
                           matchCriteria->inPort, matchCriteria->inPortMask);
        return(0);
      }
      break;

    default:
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid port mask. (inPortMask:0x%x)\r\n",
                         matchCriteria->inPortMask);
      return(0);
      break;
  }

  /* validate match fields */
  if (isMplsL2Rule)
  {
    if (OFDPA_E_NONE != ofdpaFeatureSupported(OFDPA_FEATURE_MPLSSUPPORTED))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "MPLS is not supported on this switch.\r\n", 0);
      return(0);
    }

    if (OFDPA_MPLS_L2_PORT_TYPE_VPWS_NW != OFDB_MPLS_L2_PORT_TYPE(matchCriteria->mplsL2Port))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid MPLS L2 Port type. (mplsL2Port:0x%x)\r\n",
                         matchCriteria->mplsL2Port);
      return(0);
    }
    /* tunnel ID mask must be exact mask */
    if (OFDPA_INPORT_EXACT_MASK != matchCriteria->tunnelIdMask)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid Tunnel ID mask. (tunnelIdMask:0x%x)\r\n",
                         matchCriteria->tunnelIdMask);
      return(0);
    }
    /* tunnel ID must be non-zero */
    if (0 == OFDB_TUNNELID_INDEX(matchCriteria->tunnelId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Tunnel Id must be non-zero for MPLS entry types. (tunnelId:0x%x)\r\n",
                         matchCriteria->tunnelId);
      return(0);
    }
    /* tunnel ID type must be MPLS TP */
    if (OFDPA_TUNNELID_TYPE_MPLS_TP != OFDB_TUNNELID_TYPE(matchCriteria->tunnelId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Tunnel Id type must be MPLS-TP type. (tunnelId:0x%x)\r\n",
                         matchCriteria->tunnelId);
      return(0);
    }
    /* port type should be physical */
    if (OFDPA_PORT_TYPE_PHYSICAL != OFDB_PORT_TYPE(matchCriteria->inPort))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid port type. (inPort:0x%x)\r\n",
                         matchCriteria->inPort);
      return(0);
    }
  }

  if (isVlanRule)
  {
    /* Tunnel ID is field maskable and must be zero if supplied. */
    if (matchCriteria->tunnelId != 0 ||
        OFDPA_TUNNEL_ID_FIELD_MASK != matchCriteria->tunnelIdMask)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Tunnel ID is field maskable and must be zero if supplied. (tunnelId:%d, tunnelIdMask:0x%x)\r\n",
                         matchCriteria->tunnelId, matchCriteria->tunnelIdMask);
      return(0);
    }

    /* VLAN id is optional but OFDPA_VID_PRESENT must be set if match specified */
    if (OFDPA_VID_FIELD_MASK != matchCriteria->vlanIdMask)
    {
      if ((matchCriteria->vlanIdMask & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN id mask must have OFDPA_VID_PRESENT set(vlanIdMask:0x%x)\r\n",
                           matchCriteria->vlanIdMask);
        return(0);
      }
      /* VLAN mask restricted to 13 bits */
      if ((matchCriteria->vlanIdMask & (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)) != matchCriteria->vlanIdMask)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN id mask has invalid bits set(vlanIdMask:0x%x)\r\n",
                           matchCriteria->vlanIdMask);
        return(0);
      }
      if ((matchCriteria->vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN id must have OFDPA_VID_PRESENT set(vlanId:0x%x)\r\n",
                           matchCriteria->vlanId);
        return(0);
      }
    }

    /* port type should be physical */
    if (OFDPA_PORT_TYPE_PHYSICAL != OFDB_PORT_TYPE(matchCriteria->inPort))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid port type. (inPort:0x%x)\r\n",
                         matchCriteria->inPort);
      return(0);
    }
    /* mpls l2 port should be zero */
    if (matchCriteria->mplsL2Port != 0 ||
        OFDPA_MPLS_L2_PORT_FIELD_MASK != matchCriteria->mplsL2PortMask)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Mpls L2 port is not allowed for vlan entry type (mplsL2Port: 0x%x, mplsL2PortMask: 0x%x).\r\n",
                         matchCriteria->mplsL2Port, matchCriteria->mplsL2PortMask);
      return(0);
    }
  }
  if (isTenantRule)
  {
    /* tunnel ID must be non-zero */
    if (matchCriteria->tunnelId == 0 ||
        OFDPA_INPORT_EXACT_MASK != matchCriteria->tunnelIdMask)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "A non-zero Tunnel id is required and is not maskable. (tunnelId:%d, tunnelIdMask:0x%x)\r\n",
                         matchCriteria->tunnelId, matchCriteria->tunnelIdMask);
      return(0);
    }

    /* tunnel ID type must be Data Center Overlay */
    if (OFDPA_TUNNELID_TYPE_DATA_CENTER_OVERLAY != OFDB_TUNNELID_TYPE(matchCriteria->tunnelId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Tunnel Id type must be Data Center Overlay type. (tunnelId:0x%x)\r\n",
                         matchCriteria->tunnelId);
      return(0);
    }

    /* the tunnel ID must indicate a configured tenant */
    if (ofdbTenantGet(matchCriteria->tunnelId, &tunnelTenantConfig, NULL) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Tenant not configured for flow entry tunnelId. (%d)\r\n",
                         matchCriteria->tunnelId);
      return(0);
    }

    /* port type should be logical */
    if (OFDPA_PORT_TYPE_LOGICAL_TUNNEL != OFDB_PORT_TYPE(matchCriteria->inPort))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid port type. (inPort:0x%x)\r\n",
                         matchCriteria->inPort);
      return(0);
    }
    /* mpls l2 port should be zero */
    if (matchCriteria->mplsL2Port != 0 ||
        OFDPA_MPLS_L2_PORT_FIELD_MASK != matchCriteria->mplsL2PortMask)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Mpls L2 port is not allowed for Data Center Overlay entry type (mplsL2Port: 0x%x, mplsL2PortMask: 0x%x).\r\n",
                         matchCriteria->mplsL2Port, matchCriteria->mplsL2PortMask);
      return(0);
    }
  }

  /* The maximum value for DEI match field is 1 */
  if (OFDPA_VLAN_DEI_VALUE_MASK == matchCriteria->vlanDeiMask)
  {
    if (OFDPA_VLAN_DEI_MAX_VALUE < matchCriteria->vlanDei)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "match field VLAN DEI out of range (%d)\r\n", matchCriteria->vlanDei);
      return(0);
    }
  }

  /* Check if IPv4 ether type is missed/incorrect */
  if ((matchCriteria->etherTypeMask & matchCriteria->etherType) != 0x0800 &&
      ((0 != matchCriteria->destIp4Mask) || (0 != matchCriteria->sourceIp4Mask)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid ethertype for IPv4 match fields.\r\n", 0);
    return(0);
  }

  if (matchCriteria->etherTypeMask == OFDPA_ETHERTYPE_EXACT_MASK &&
      matchCriteria->etherType != 0x86dd &&
      ((0 != memcmp(&matchCriteria->destIp6Mask, &in6addr_null, sizeof(matchCriteria->destIp6Mask))) ||
       (0 != memcmp(&matchCriteria->sourceIp6Mask, &in6addr_null, sizeof(matchCriteria->sourceIp6Mask))) ||
       (0 != matchCriteria->ipv6FlowLabelMask)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid ethertype for IPv6 match fields.\r\n", 0);
    return(0);
  }

  /* If not IPv6 or IPv4, then no IP match fields are allowed */
  if (matchCriteria->etherType != 0x86dd && matchCriteria->etherType != 0x0800)
  {
    if (matchCriteria->dscpMask != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid ethertype (0x%x) for IP DSCP match field.\r\n", matchCriteria->etherType);
      return(0);
    }
    if (matchCriteria->ecnMask != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid ethertype (0x%x) for IP ECN match field.\r\n", matchCriteria->etherType);
      return(0);
    }
    if (matchCriteria->ipProtoMask != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid ethertype (0x%x) for IP Protocol match field.\r\n", matchCriteria->etherType);
      return(0);
    }

    if ((0 != matchCriteria->srcL4PortMask) ||
        (0 != matchCriteria->destL4PortMask) ||
        (0 != matchCriteria->icmpTypeMask) ||
        (0 != matchCriteria->icmpCodeMask))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid ethertype for IP match fields.\r\n", 0);
      return(0);
    }
  }

  if (((0 != matchCriteria->srcL4PortMask) ||
       (0 != matchCriteria->destL4PortMask)) &&
      (((matchCriteria->ipProto & matchCriteria->ipProtoMask) != IPPROTO_TCP) &&
       ((matchCriteria->ipProto & matchCriteria->ipProtoMask) != IPPROTO_UDP) &&
       ((matchCriteria->ipProto & matchCriteria->ipProtoMask) != IPPROTO_SCTP)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid IP Protocol type for L4 match fields.\r\n", 0);
    return(0);
  }

  if (((matchCriteria->etherTypeMask & matchCriteria->etherType) != 0x0806) &&
      (0 != matchCriteria->ipv4ArpSpaMask))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid ethertype for ARP match fields.\r\n", 0);
    return(0);
  }

  /* Vlan PCP must be allowed only when preceded by Vlan ID */
  if ((!(matchCriteria->vlanIdMask & matchCriteria->vlanId)) &&
      (matchCriteria->vlanPcpMask & matchCriteria->vlanPcp))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Vlan PCP match field must be preceded by Vlan ID match field.\r\n", 0);
    return(0);
  }

  /* Vlan DEI must be allowed only when preceded by Vlan ID */
  if ((!(matchCriteria->vlanIdMask & matchCriteria->vlanId)) &&
      (matchCriteria->vlanDeiMask & matchCriteria->vlanDei))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Vlan DEI match field must be preceded by Vlan ID match field.\r\n", 0);
    return(0);
  }

  if (((matchCriteria->icmpCodeMask & matchCriteria->icmpCode) ||
       (matchCriteria->icmpTypeMask & matchCriteria->icmpType)) &&
      (((matchCriteria->ipProto & matchCriteria->ipProtoMask) != IPPROTO_ICMP) &&
       ((matchCriteria->ipProto & matchCriteria->ipProtoMask) != IPPROTO_ICMPV6)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid protocol ID %d for ICMPv4 type/code.\r\n", matchCriteria->ipProto);
    return(0);
  }

  /* check instructions */

  /* validate goto instruction */
  if (flowData->gotoTable != OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS &&
      flowData->gotoTable != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Goto table must be Color Based Actions flow table or 0. (gotoTableId: %d)\r\n",
                       flowData->gotoTable);
    return(0);
  }

  /* apply actions */

  if ((flowData->outputPort != 0) &&
      (flowData->outputPort != OFDPA_PORT_CONTROLLER))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid output port %d\r\n", flowData->outputPort);
    return(0);
  }

  /* make sure action data is valid */
  if (flowData->vlanPcpAction && flowData->vlanPcp > OFDPA_VLAN_PCP_MAX_VALUE)
  {
    /* VLAN PCP value is out of range */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Value for set VLAN PCP out of range (%d)\n",
                       flowData->vlanPcp);
    return(0);
  }

  /* make sure action data is valid */
  if (flowData->ecnAction && OFDPA_ECN_MAX_VALUE < flowData->ecn)
  {
    /* ECN value is out of range */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Value for set ECN out of range (%d)\n",
                       flowData->ecn);
    return(0);
  }

  /* make sure action data is valid */
  if (flowData->dscpAction && OFDPA_DSCP_MAX_VALUE < flowData->dscp)
  {
    /* DSCP value is out of range */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Value for Set DSCP out of range (%d)\n",
                       flowData->dscp);
    return(0);
  }

  /* validate Traffic Class value */
  if (flowData->trafficClassAction &&
      flowData->trafficClass > OFDPA_TRAFFIC_CLASS_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Traffic class value out of range. trafficClass = %d\r\n",
                       flowData->trafficClass);
    return(0);
  }

  /* if clear actions specified, write actions ignored */
  if (!flowData->clearAction)
  {
    if (flowData->groupID != 0)
    {
      /* group entry must exist */
      if (ofdbRefGroupValidate(flowData->groupID) != OFDPA_E_NONE)
      {
        /* referenced group ID not found in tables */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Referenced group entry not valid. groupID = 0x%x\r\n",
                           flowData->groupID);
        return(0);
      }
    }

    /* validate color actions */
    if (flowData->gotoTable == OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS &&
        (flowData->colorEntryIdAction == 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "if goto table is Color Based Actions flow table, color action and color entry id action must be defined\r\n", 0);
      return(0);
    }

    /* Validate color value */
    if (flowData->colorAction != 0 && flowData->color >= OFDPA_QOS_RESERVED)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "color value is not valid. color = %d\r\n", flowData->color);
      return(0);
    }

    if (isMplsL2Rule)
    {
      /* checking groupID for MPLS type entries */
      if (flowData->groupID != 0)
      {
        switch (OFDB_GROUP_TYPE(flowData->groupID))
        {
          case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
          case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
          case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
            /* acceptible types, no action */
            break;
          default:
            /* not a supported group type */
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Reference group in MPLS flow entry is not compatible type. groupID = 0x%x\r\n",
                               flowData->groupID);
            return(0);
            break;
        }
      }
    }

    /* action checking for VLAN type entries */
    if (isVlanRule)
    {
      if (flowData->groupID != 0)
      {
        switch (OFDB_GROUP_TYPE(flowData->groupID))
        {
          case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
            if (!OFDB_GROUP_IS_MPLS_L3_VPN_LABEL(flowData->groupID))
            {
              /* not a supported group sub type */
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Reference group is not compatible sub type. groupID = 0x%x\r\n",
                                 flowData->groupID);
              return(0);
            }
            break;
          case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
            if (OFDB_GROUP_IS_MPLS_FAST_FAILOVER(flowData->groupID))
            {
              /* not a supported group sub type */
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Reference group currently is not supported. groupID = 0x%x\r\n",
                                 flowData->groupID);
              return(0);
            }
            else
            {
              /* not a supported group sub type */
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Reference group is not compatible sub type. groupID = 0x%x\r\n",
                                 flowData->groupID);
              return(0);
            }
            break;
          case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
          case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
          case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
          case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
          case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
          case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
          case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
            /* acceptible types, no action */
            break;
          default:
            /* not a supported group type */
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Reference group in VLAN Flow is not compatible type. groupID = 0x%x\r\n",
                               flowData->groupID);
            return(0);
            break;
        }
      }

    }

    /* action checking for Tenant type entries */
    if (isTenantRule)
    {
      if (flowData->groupID != 0)
      {
        switch (OFDB_GROUP_TYPE(flowData->groupID))
        {
          case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
            /* group entry must be Multicast sub-type */
            if ((OFDB_GROUP_TUNNEL_SUBTYPE(flowData->groupID) != OFDPA_L2_OVERLAY_MULTICAST_UNICAST_TUNNEL) &&
                (OFDB_GROUP_TUNNEL_SUBTYPE(flowData->groupID) != OFDPA_L2_OVERLAY_MULTICAST_MULTICAST_TUNNEL))
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Referenced group in Tenant Flow is not Multicast subtype. groupID = 0x%x\r\n",
                                 flowData->groupID);
              return(0);
            }
            /* must be in tenant forwarding domain */
            if (OFDB_GROUP_TUNNELID(flowData->groupID) != matchCriteria->tunnelId)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Referenced group in Tenant Flow is not in tenant domain. groupID = 0x%x, tunnelId = %d\r\n",
                                 flowData->groupID, matchCriteria->tunnelId);
              return(0);
            }
            break;
          default:
            /* not a supported group type */
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "Referenced group in Tenant Flow is not compatible type. groupID = 0x%x\r\n",
                               flowData->groupID);
            return(0);
            break;
        }
      }

      /* tunnel rules can only forward to logical tunnel port */
      if (0 != flowData->outputTunnelPort)
      {
        if (OFDB_PORT_TYPE(flowData->outputTunnelPort) != OFDPA_PORT_TYPE_LOGICAL_TUNNEL)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Output port not a logical tunnel type (0x%x)\r\n",
                             flowData->outputTunnelPort);
          return(0);
        }

        if ((ofdbPortGet(flowData->outputTunnelPort, &portInfo, &flags) != OFDPA_E_NONE) ||
            (flags & OFDB_PORT_DELETED))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Output port not found in port table. (0x%x)\r\n",
                             flowData->outputTunnelPort);
          return(0);
        }

        if ((portInfo.tunnelPortConfig.type != OFDPA_TUNNEL_PORT_TYPE_ENDPOINT) &&
            (portInfo.tunnelPortConfig.type != OFDPA_TUNNEL_PORT_TYPE_ACCESS))
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Output port in Tenant rule not valid tunnel port type. (%d)\r\n",
                             portInfo.tunnelPortConfig.type);
          return(0);
        }

        /* must be in tenant forwarding domain */
        if (ofdbPortTenantGet(flowData->outputTunnelPort, matchCriteria->tunnelId, NULL) != OFDPA_E_NONE)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Referenced outputTunnelPort in Tenant Flow is not in tenant domain. outputTunnelPort = 0x%x, tunnelId = %d\r\n",
                             flowData->outputTunnelPort, matchCriteria->tunnelId);
          return(0);
        }
      }
    }

    /* If this flow is matching a VLAN ID, ensure the VLAN ID meets the following criteria:
     * 1) If group is L2 Interface, must be the same as for the L2 Interface.
     * 2) If the group is L2 Rewrite and
     *       the group does not rewrite the VLAN ID and
     *       the group references a L2 Interface group then
     *       the VLAN ID must be the same as for the referenced L2 Interface group.
     */
    if ((OFDPA_VID_FIELD_MASK != matchCriteria->vlanIdMask) &&
        (flowData->groupID != 0))
    {
      groupId = flowData->groupID;
      if (OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE == OFDB_GROUP_TYPE(groupId))
      {
        /* If no group bucket, then assume valid.  Will have to validate later when bucket added. */
        if ((OFDPA_E_NONE == ofdbGroupBucketEntryFirstGet(groupId, &groupBucket)) &&
            (0 == groupBucket.bucketData.l2Rewrite.vlanId))
        {
          /* VLAN ID is not set by rewrite group, ensure that the L2 Interface group VLAN matches. */
          groupId = groupBucket.referenceGroupId;
        }
      }

      if ((OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE == OFDB_GROUP_TYPE(groupId)) &&
          ((matchCriteria->vlanId & OFDPA_VID_EXACT_MASK) != OFDB_GROUP_VLANID(groupId)))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN ID %d (%x) must match VLAN ID in L2 Interface group.\r\n",
                           matchCriteria->vlanId, matchCriteria->vlanId);
        return(0);
      }
    }
  }

  if (0 != flowData->meterIdAction)
  {
    if (OFDPA_E_NONE != ofdbMeterStatsGet(flowData->meterId, &meterStats))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_VERBOSE,
                         "Meter not found, meterId = %d.\r\n",
                         flowData->meterId);
      return(0);
    }
  }

  /* Unwanted match criteria checks */
  if (isTenantRule)
  {
    if ((matchCriteria->vlanIdMask != OFDPA_ZERO_MASK) ||
        (matchCriteria->vlanPcpMask != OFDPA_ZERO_MASK) ||
        (matchCriteria->vlanDeiMask != OFDPA_ZERO_MASK) ||
        (matchCriteria->ipv6FlowLabelMask != OFDPA_ZERO_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "For Datacenter Overlay type flow the following match criteria may only have a mask of all. "
                         "vlanIdMask = 0x%x vlanPcpMask = 0x%x vlanDeiMask = 0x%x \r\n",matchCriteria->vlanIdMask,
                         matchCriteria->vlanPcpMask, matchCriteria->vlanDeiMask);
      return(0);
    }
  }

  if (matchCriteria->etherTypeMask == OFDPA_ETHERTYPE_EXACT_MASK)
  {
    if (matchCriteria->etherType == 0x86dd)
    {
      if ((matchCriteria->sourceIp4Mask != OFDPA_ZERO_MASK) ||
          (matchCriteria->destIp4Mask != OFDPA_ZERO_MASK) ||
          (matchCriteria->ipv4ArpSpaMask != OFDPA_ZERO_MASK))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "For IPv6 type flow the following match criteria may only have a mask of all. "
                           "sourceIp4Mask = 0x%x destIp4Mask = 0x%x ipv4ArpSpaMask = 0x%x \r\n",
                           matchCriteria->sourceIp4Mask, matchCriteria->destIp4Mask, matchCriteria->ipv4ArpSpaMask);
        return(0);
      }
    }
    else
    {
      if (!IN6_IS_ADDR_UNSPECIFIED(&matchCriteria->sourceIp6Mask) ||
          !IN6_IS_ADDR_UNSPECIFIED(&matchCriteria->destIp6Mask))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "For IPv4 type flow the sourceIp6Mask and destIp6Mask match criteria may only have a mask of all. \r\n", 0);
        return(0);
      }
    }
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowColorActionsEntryValidate(ofdpaColorActionsFlowEntry_t *flowData)
{
  ofdpaColorActionsFlowMatch_t *matchCriteria;

  matchCriteria = &flowData->match_criteria;

  /* validate QOS color match field */
  switch (matchCriteria->color)
  {
    case OFDPA_QOS_GREEN:
    case OFDPA_QOS_YELLOW:
    case OFDPA_QOS_RED:
      break;
    default:
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "QOS color value is not valid. color = %d\r\n",
                           matchCriteria->color);
        return 0;
      }
  }

  /* check instructions */

  /* apply actions */

  if ((flowData->outputPort != 0) &&
      (flowData->outputPort != OFDPA_PORT_CONTROLLER))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid outputPort field %d\r\n", flowData->outputPort);
    return(0);
  }

  /* if clear actions specified, write actions ignored */
  if (!flowData->clearAction)
  {
    /* validate Traffic Class value */
    if (flowData->trafficClassAction &&
        flowData->trafficClass > OFDPA_TRAFFIC_CLASS_MAX_VALUE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Traffic class value out of range. trafficClass = %d\r\n",
                         flowData->trafficClass);
      return(0);
    }

    /* validate VLAN PCP value */
    if (flowData->vlanPcpAction &&
        flowData->vlanPcp > OFDPA_VLAN_PCP_MAX_VALUE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN PCP value out of range. vlanPcp = %d\r\n",
                         flowData->vlanPcp);
      return(0);
    }

    /* validate ECN value */
    if (flowData->ecnAction &&
        flowData->ecn > OFDPA_ECN_MAX_VALUE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "ECN value out of range. ecn = %d\r\n",
                         flowData->ecn);
      return(0);
    }

    /* validate DSCP value */
    if (flowData->dscpAction &&
        flowData->dscp > OFDPA_DSCP_MAX_VALUE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "DSCP value out of range. DSCP = %d\r\n",
                         flowData->dscp);
      return(0);
    }

    /* set VLAN PCP and set DSCP actions cannot be established both in the same flow */
    if (flowData->dscpAction && flowData->vlanPcpAction)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "set VLAN PCP and set DSCP actions cannot be established both in the same flow.\r\n", 0);
      return(0);
    }
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowEgressVlanEntryValidate(ofdpaEgressVlanFlowEntry_t *flowData)
{
  ofdpaEgressVlanFlowMatch_t *matchCriteria;
  int isXlateSingleType = 0;
  int isXlateDoubleType = 0;
  int isXlateUntaggedType = 0;
  int isPriorityTaggedType = 0;
  int isXlateAllOnPortType = 0;
  int isMipMepType = 0;
  int isMplsRuleType = 0;
  uint32_t groupId;

  /* check match criteria constraints */
  matchCriteria = &flowData->match_criteria;

  /* outPort must be a valid interface */
  if ((OFDB_PORT_TYPE(matchCriteria->outPort) != OFDPA_PORT_TYPE_PHYSICAL) ||
      (!ofdbPortIsValid(matchCriteria->outPort)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Port is not valid. outPort = %d\r\n",
                       matchCriteria->outPort);
    return(0);
  }

  if (0 == matchCriteria->allowVlanTranslation)
  {
    isMipMepType = 1;
    if ((OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK) != matchCriteria->vlanIdMask)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid VLAN ID mask. (0x%04x)\r\n",
                         matchCriteria->vlanIdMask);
      return(0);
    }
  }
  else if (OFDPA_VID_FIELD_MASK == matchCriteria->vlanIdMask)
  {
    isXlateAllOnPortType = 1;
  }
  else if ((OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK) != matchCriteria->vlanIdMask)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid VLAN ID mask. (0x%04x)\r\n",
                       matchCriteria->vlanIdMask);
    return(0);
  }
  else if (OFDPA_VID_NONE == matchCriteria->vlanId)
  {
    isXlateUntaggedType = 1;
  }
  else if (OFDPA_VID_PRESENT == matchCriteria->vlanId)
  {
    isPriorityTaggedType = 1;
  }
  else if (OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1 == flowData->gotoTableId)
  {
    isXlateDoubleType = 1;
  }
  else
  {
    isXlateSingleType = 1;
  }

  if ((0 != matchCriteria->mplsL2PortMask) ||
      (0 != matchCriteria->tunnelIdMask))
  {
    isMplsRuleType = 1;
    if (OFDPA_MPLS_L2_PORT_EXACT_MASK != matchCriteria->mplsL2PortMask)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid MPLS L2 Port mask. (0x%08x)\r\n",
                         matchCriteria->mplsL2PortMask);
      return(0);
    }
    if (OFDPA_TUNNEL_ID_EXACT_MASK != matchCriteria->tunnelIdMask)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid Tunnel mask. (0x%08x)\r\n",
                         matchCriteria->tunnelIdMask);
      return(0);
    }
    if (OFDPA_TUNNELID_TYPE_MPLS_TP != OFDB_TUNNELID_TYPE(matchCriteria->tunnelId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid Tunnel type. (0x%04x)\r\n",
                         OFDB_TUNNELID_TYPE(matchCriteria->tunnelId));
      return(0);
    }
    if (0 != isXlateDoubleType)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid Goto table ID (gotoTableId = %d)\r\n", flowData->gotoTableId);
      return(0);
    }
  }
  else if ((OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT == flowData->gotoTableId) &&
           (0 != isMipMepType))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid Goto table ID (gotoTableId = %d)\r\n", flowData->gotoTableId);
    return(0);
  }

  if ((0 == isXlateDoubleType) &&
      (0 != flowData->gotoTableId))
  {
    if ((OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT != flowData->gotoTableId) &&
        (OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK   != flowData->gotoTableId))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid Goto table ID (gotoTableId = %d)\r\n", flowData->gotoTableId);
      return(0);
    }
  }

  if (isMipMepType || isXlateSingleType || isXlateDoubleType)
  {
    if (!dpaVlanIsValid(matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid VLAN ID. (0x%04x)\r\n",
                         matchCriteria->vlanId);
      return(0);
    }
  }

  /* check instructions */

  /* if classBasedCountAction set, counter must exist in counter table */
  if (flowData->classBasedCountAction != 0)
  {
    if (0 == isMplsRuleType)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "classBasedCountAction only permitted for MPLS Terminate flow types.", 0);
      return(0);
    }

    if (isXlateDoubleType)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "classBasedCountAction not permitted for Translate Double Tag flow types.", 0);
      return(0);
    }

    if (ofdbClassBasedCounterRefCountGet(flowData->classBasedCountId, NULL) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Entry specifies classBasedCountAction but counter not found in table. "
                         "classBasedCountAction = %d, classBasedCountId = 0x%x\r\n",
                         flowData->classBasedCountAction, flowData->classBasedCountId);
      return(0);
    }
  }

  if (0 == isMipMepType)
  {
    /* verify that L2 Unfiltered Interface group is installed for this port */
    groupId = 0;
    groupId = OFDB_GROUP_TYPE_SET(groupId, OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE);
    groupId = OFDB_GROUP_PORTID_SET(groupId, matchCriteria->outPort);

    if (ofdbRefGroupValidate(groupId) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Egress VLAN Translate entry translates VLAN but valid L2 Unfiltered "
                         "Interface Group entry is not installed.\r\n", 0);
      return(0);
    }
  }

  if ((0 == isXlateSingleType) && (0 == isXlateDoubleType))
  {
    if (flowData->popVlanAction != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "popVlanAction only permitted for Single or Double tagged flow types.\r\n", 0);
      return(0);
    }
  }

  if (isMipMepType || isXlateDoubleType || isXlateAllOnPortType || isXlateUntaggedType)
  {
    if (flowData->setVlanIdAction != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "setVlanIdAction not permitted for Translate Double Tag, All Traffic on Port, Untagged or MEP/MIP flow types.", 0);
      return(0);
    }
    if (flowData->pushVlan2Action != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "pushVlan2Action not permitted for Translate Double Tag, All Traffic on Port, Untagged or MEP/MIP flow types.", 0);
      return(0);
    }
    if (flowData->setVlanId2Action != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "setVlanId2Action not permitted for Translate Double Tag, All Traffic on Port, Untagged or MEP/MIP flow types.", 0);
      return(0);
    }
  }
  else
  {
    /* if setVlanIdAction specified, newVlanId must be valid VLAN */
    if ((flowData->setVlanIdAction != 0) && !dpaVlanIsValid(flowData->newVlanId & OFDPA_VID_EXACT_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Single Tag entry does not specify a valid newVlanId for setVlanIdAction. newVlanId = %d\r\n",
                         (flowData->newVlanId & OFDPA_VID_EXACT_MASK));
      return(0);
    }

    /* can specify either popVlanAction or setVlanIdAction but not both */
    if ((flowData->setVlanIdAction != 0) && (flowData->popVlanAction != 0))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Single Tag entry must not specify both setVlanIdAction and popVlanAction.\r\n",
                         0);
      return(0);
    }

    /* if pushVlan2Action specified... */
    if (flowData->pushVlan2Action != 0)
    {
      if (0 != isPriorityTaggedType)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN Translate Priority Tagged entry must not specify pushVlan2Action.\r\n", 0);
        return(0);
      }

      /* setVlanId2Action must be valid VLAN */
      if (flowData->setVlanId2Action == 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN Translate Single Tag entry specifies pushVlan2Action but does not specify setVlanId2Action.\r\n", 0);
        return(0);
      }

      if (!dpaVlanIsValid(flowData->newVlanId2 & OFDPA_VID_EXACT_MASK))
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN Translate Single Tag entry does not specify a valid newVlanId2 for setVlanId2Action. newVlanId2 = %d\r\n",
                           flowData->newVlanId2 & OFDPA_VID_EXACT_MASK);
        return(0);
      }

      /* newTpid2 must be valid */
      if (flowData->newTpid2 != 0x8100)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "VLAN Translate Single Tag entry specifies pushVlan2Action but newTpid2 is not 0x8100.\r\n", 0);
        return(0);
      }
    }

#ifdef PUSH_POP_SWAP_SUPPORT
	/*egress vlan 支持POP*/
	if ((flowData->ovidAction != 0))
#else
    /* all other actions may not be included */
    if ((flowData->ovidAction != 0) ||
        (flowData->popVlanAction != 0))
#endif        
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Single Tag type entry specifies invalid action.\r\n", 0);
      return(0);
    }
  }

  if (isXlateDoubleType)
  {
    /* set-field Ovid must be included */
    if (flowData->ovidAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Double Tag type entry must specify ovidAction.\r\n", 0);
      return(0);
    }

    /* pop VLAN action must be included */
    if (flowData->popVlanAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "VLAN Translate Double Tag type entry must specify popVlanAction.\r\n", 0);
      return(0);
    }
  }
  else
  {
    if (flowData->ovidAction != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "ovidAction only permitted on Translate Double Tag flow types.\r\n", 0);
      return(0);
    }
  }

  /* data for SET-FIELD VLAN must have the OFPVID_PRESENT bit set */
  if ((flowData->setVlanIdAction != 0) && ((flowData->newVlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry specifies newVlanId without OFDPA_VID_PRESENT bit set. "
                       "setVlanIdAction = %d, newVlanId = 0x%x\r\n",
                       flowData->setVlanIdAction, flowData->newVlanId);
    return(0);
  }
  if ((flowData->setVlanId2Action != 0) && ((flowData->newVlanId2 & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry specifies newVlanId2 without OFDPA_VID_PRESENT bit set. "
                       "setVlanId2Action = %d, newVlanId2 = 0x%x\r\n",
                       flowData->setVlanId2Action, flowData->newVlanId2);
    return(0);
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowEgressVlan1EntryValidate(ofdpaEgressVlan1FlowEntry_t *flowData)
{
  ofdpaEgressVlan1FlowMatch_t *matchCriteria;

  /* check match criteria constraints */
  matchCriteria = &flowData->match_criteria;

  /* check that OFDPA_VID_PRESENT is set in match field */
  if ((matchCriteria->vlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "vlanId field must have OFDPA_VID_PRESENT set. (vlanId = 0x%04x)\r\n",
                       matchCriteria->vlanId);
    return(0);
  }
  if (!dpaVlanIsValid(matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid VLAN ID. (0x%04x)\r\n",
                       matchCriteria->vlanId);
    return(0);
  }

  if ((matchCriteria->ovid & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "ovid field must have OFDPA_VID_PRESENT set. (ovid = 0x%04x)\r\n",
                       matchCriteria->ovid);
    return(0);
  }
  if (!dpaVlanIsValid(matchCriteria->ovid & OFDPA_VID_EXACT_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Invalid ovid. (0x%04x)\r\n",
                       matchCriteria->ovid);
    return(0);
  }

  /* data for SET-FIELD VLAN must have the OFPVID_PRESENT bit set */
  if ((flowData->setVlanIdAction != 0) && ((flowData->newVlanId & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry specifies newVlanId without OFDPA_VID_PRESENT bit set. "
                       "setVlanIdAction = %d, newVlanId = 0x%x\r\n",
                       flowData->setVlanIdAction, flowData->newVlanId);
    return(0);
  }
  if ((flowData->setVlanId2Action != 0) && ((flowData->newVlanId2 & OFDPA_VID_PRESENT) != OFDPA_VID_PRESENT))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry specifies newVlanId2 without OFDPA_VID_PRESENT bit set. "
                       "setVlanId2Action = %d, newVlanId2 = 0x%x\r\n",
                       flowData->setVlanId2Action, flowData->newVlanId2);
    return(0);
  }

  /* if classBasedCountAction set, counter must exist in counter table */
  if (flowData->classBasedCountAction != 0)
  {
#ifdef NOT_SUPPORTED
    if (ofdbClassBasedCounterRefCountGet(flowData->classBasedCountId, NULL) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Entry specifies classBasedCountAction but counter not found in table. "
                         "classBasedCountAction = %d, classBasedCountId = 0x%x\r\n",
                         flowData->classBasedCountAction, flowData->classBasedCountId);
      return(0);
    }
#else
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "classBasedCountAction only permitted for MPLS Terminate flow types.", 0);
    return(0);
#endif
  }

  /* outPort must be a valid interface */
  if ((OFDB_PORT_TYPE(matchCriteria->outPort) != OFDPA_PORT_TYPE_PHYSICAL) ||
      (!ofdbPortIsValid(matchCriteria->outPort)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Port is not valid. outPort = %d\r\n",
                       matchCriteria->outPort);
    return(0);
  }

  /* if setVlanIdAction specified, newVlanId must be valid VLAN */
  if ((flowData->setVlanIdAction != 0) && !dpaVlanIsValid(flowData->newVlanId & OFDPA_VID_EXACT_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry does not specify a valid newVlanId for setVlanIdAction. newVlanId2 = %d\r\n",
                       flowData->newVlanId2 & OFDPA_VID_EXACT_MASK);
    return(0);
  }

  /* if pushVlan2Action specified... */
  if (flowData->pushVlan2Action != 0)
  {
    /* setVlanId2Action must be valid VLAN */
    if (flowData->setVlanId2Action == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Entry specifies pushVlan2Action but does not specify setVlanId2Action.\r\n", 0);
      return(0);
    }

    if (!dpaVlanIsValid(flowData->newVlanId2 & OFDPA_VID_EXACT_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Entry does not specify a valid newVlanId2 for setVlanId2Action. newVlanId2 = %d\r\n",
                         flowData->newVlanId2 & OFDPA_VID_EXACT_MASK);
      return(0);
    }

    /* newTpid2 must be valid */
    if (flowData->newTpid2 != 0x8100)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Egress VLAN_1  entry specifies pushVlan2Action but newTpid2 is not 0x8100.\r\n", 0);
      return(0);
    }
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowEgressMpEntryValidate(ofdpaEgressMpFlowEntry_t *flowData)
{
  char buffer[15];
  ofdpaEgressMpFlowMatch_t *matchCriteria = &flowData->match_criteria;
  ofdpaOamMegConfig_t megConfig;
  ofdpaOamMepConfig_t mepConfig, lmMepConfig;
  uint32_t isMipLbmProcessType = 0;
  uint32_t isMipLtmProcessType = 0;
  uint32_t isPduProcessType = 0;
  uint32_t isPduPassthruHigherMdlType = 0;
  uint32_t isPduDropLowerMdlType = 0;
  uint32_t isDataPassthruType = 0;
  uint8_t  mdlIter1 = 0, mdlIter2 = 0, ingressFlowFound = 0, egressFlowFound = 0, direction = 0;
  ofdpaFlowEntry_t lmIngressMpFlow, lmEgressMpFlow;
  uint32_t lmepId;
  uint32_t rxLmepPresent = 0, txLmepPresent = 0;
  uint32_t megIndex = 0;
  ofdpaOamMipConfig_t mipConfig;

  /* first check things all flow entry types must match */

  /* outPort must be a valid interface */
  if ((OFDB_PORT_TYPE(matchCriteria->outPort) != OFDPA_PORT_TYPE_PHYSICAL) ||
      (!ofdbPortIsValid(matchCriteria->outPort)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Port is not valid. outPort = %d\r\n",
                       matchCriteria->outPort);
    return(0);
  }

  /* VLAN mask is all or nothing for all types */
  if ((matchCriteria->vlanIdMask != OFDPA_VID_FIELD_MASK) &&
      (matchCriteria->vlanIdMask != (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Invalid VLAN mask. (0x%04x)\r\n", matchCriteria->vlanIdMask);
    return(0);
  }

  /* if vlanId specified, vlanId must be valid VLAN */
  if ((matchCriteria->vlanIdMask == (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)) &&
      !dpaVlanIsValid(matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Entry does not specify a valid vlanId.  vlanId = 0x%04x\r\n",
                       matchCriteria->vlanId);
    return(0);
  }

  /* determine entry type */
  if ((matchCriteria->etherType == 0x8902) && (matchCriteria->etherTypeMask == OFDPA_ETHERTYPE_EXACT_MASK))
  {
    if (matchCriteria->oamY1731MdlMask == OFDPA_OAM_Y1731_MDL_EXACT_MASK)
    {
      if (matchCriteria->oamY1731OpcodeMask == OFDPA_OAM_Y1731_OPCODE_EXACT_MASK)
      {
        if (matchCriteria->oamY1731Opcode == 3)
        {
          isMipLbmProcessType = 1;
        }
        else if (matchCriteria->oamY1731Opcode == 5)
        {
          isMipLtmProcessType = 1;
        }
        else
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Invalid oamY1731Opcode value (oamY1731Opcode = 0x%x)\r\n",
                             matchCriteria->oamY1731Opcode);
          return(0);
        }
      }
      else if (matchCriteria->oamY1731OpcodeMask == OFDPA_OAM_Y1731_OPCODE_ALL_MASK)
      {
        /* differentiate based on clearAction */
        if (flowData->clearAction != 0)
        {
          isPduProcessType = 1;
        }
        else
        {
          isPduPassthruHigherMdlType = 1;
        }
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "Invalid match combination: destMacMask = %s, oamY1731OpcodeMask = 0x%x, oamY1731Opcode = %d. (\r\n",
                           ofdpaMacAddrToString(&matchCriteria->destMacMask, buffer), matchCriteria->oamY1731OpcodeMask, matchCriteria->oamY1731Opcode);
        return(0);
      }
    }
    else if (matchCriteria->oamY1731MdlMask == OFDPA_OAM_Y1731_MDL_ALL_MASK)
    {
      isPduDropLowerMdlType = 1;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid oamY1731MdlMask value (oamY1731MdlMask = 0x%x)\r\n",
                         matchCriteria->oamY1731MdlMask);
      return(0);
    }
  }
  else if (matchCriteria->etherTypeMask == OFDPA_ETHERTYPE_ALL_MASK)
  {
    isDataPassthruType = 1;
  }
  else
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Incorrect etherType and mask combination (etherType/etherTypeMask = 0x%x/0x%x)\r\n",
                       matchCriteria->etherType, matchCriteria->etherTypeMask);
    return(0);
  }

  /* perform any further match criteria validation based on flow entry type */
  if (isMipLbmProcessType || isMipLtmProcessType || isPduProcessType || isPduPassthruHigherMdlType)
  {
    if (matchCriteria->oamY1731Mdl > OFDPA_OAM_Y1731_MDL_EXACT_MASK)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Invalid oamY1731Mdl value (oamY1731Mdl = 0x%x)\r\n",
                         matchCriteria->oamY1731Mdl);
      return(0);
    }
  }

  if (isPduProcessType || isPduPassthruHigherMdlType || isPduDropLowerMdlType || isDataPassthruType)
  {
    if (!OFDPA_MAC_ADDR_IS_NULL(matchCriteria->destMacMask.addr))
    {
      /* if destMacMask is not all, it is invalid */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "destMacMask must be all for type flow (destMacMask: %s)\r\n",
                         ofdpaMacAddrToString(&matchCriteria->destMacMask, buffer));
      return(0);
    }
  }

  /* check instructions */
  if (isMipLbmProcessType || isMipLtmProcessType || isPduProcessType || isPduDropLowerMdlType)
  {
    /* required to have clearAction set */
    if (flowData->clearAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type required to specify clearAction.\r\n",
                         0);
      return(0);
    }
  }

  if (isPduPassthruHigherMdlType || isDataPassthruType)
  {
    /* cannot have clearAction set */
    if (flowData->clearAction != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type cannot specify clearAction.\r\n",
                         0);
      return(0);
    }
  }

  /* check apply actions */
  if (isMipLbmProcessType || isMipLtmProcessType || isPduProcessType)
  {
    /* required to have setField LMEP_ID */
    if (flowData->lmepIdAction == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type required to specify setField LMEP_ID.\r\n",
                         0);
      return(0);
    }
  }
  else
  {
    /* all other types cannot have setField LMEP_ID */
    if (flowData->lmepIdAction != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type cannot to specify setField LMEP_ID.\r\n",
                         0);
      return(0);
    }
  }

  if (isMipLbmProcessType || isMipLtmProcessType || isPduProcessType)
  {
    /* must specify Output to either CONTROLLER or LOCAL port */
    if ((flowData->outputPort != OFDPA_PORT_CONTROLLER) && (flowData->outputPort != OFDPA_PORT_LOCAL))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type required to specify ouputPort as CONTROLLER or LOCAL.\r\n",
                         0);
      return(0);
    }
  }
  else if (isPduDropLowerMdlType)
  {
    /* may specify Output to either CONTROLLER or LOCAL port */
    if ((flowData->outputPort != 0) &&
        (flowData->outputPort != OFDPA_PORT_CONTROLLER) &&
        (flowData->outputPort != OFDPA_PORT_LOCAL))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow type required to specify ouputPort as CONTROLLER or LOCAL.\r\n",
                         0);
      return(0);
    }
  }
  else
  {
    /* all other types may not specify Output  */
    if (flowData->outputPort != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow may not specify ouput action.\r\n",
                         0);
      return(0);
    }
  }

  /* only these entry types may specify check drop status */
  if (flowData->checkDropStatusAction)
  {
    if (!isPduPassthruHigherMdlType && !isDataPassthruType)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Flow may not specify checkDropStatusAction.\r\n",
                         0);
      return(0);
    }
  }

  /* check the lmepId action field validity */
  if ((flowData->lmepIdAction) ||
      (flowData->oamSetCounterFieldsAction) ||
      (flowData->checkDropStatusAction))
  {
    /* verify MP is configured for this LMEP_ID */
    if (ofdbOamMepGet(flowData->lmepId, &mepConfig, NULL) != OFDPA_E_NONE)
    {
      /* retrieve MIP table entry */
      if (ofdbOamMipGet(flowData->lmepId, &mipConfig, NULL) != OFDPA_E_NONE)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "MPLS flow specifies mpId not found in MEP/MIP table. (lmpId = %d)\r\n",
                           flowData->lmepId);
        return(0);
      }
      else
      {
        megIndex = mipConfig.megIndex;
      }
    }
    else
    {
      megIndex = mepConfig.megIndex;
    }

    /* verify MEG is configured for MPs MEG index */
    if (ofdbOamMegGet(megIndex, &megConfig, NULL, NULL) == OFDPA_E_NONE)
    {
      /* verify MEG is of type Ethernet */
      if (megConfig.megType != OFDPA_OAM_MEG_TYPE_ETHERNET)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "G8113.1 or BHH megtype is not supported. (MEG index = %d)\r\n",
                           megIndex);
        return(0);
      }
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Specified LMEP_ID does not have any MEG configured. (MEG index = %d)\r\n",
                         megIndex);
      return(0);
    }
  }

  for (mdlIter1 = OFDPA_MDL_MIN_VALUE; mdlIter1 <= OFDPA_MDL_MAX_VALUE; mdlIter1++)
  {
    if ((0 != flowData->rxLmCountingLmepId[mdlIter1]) ||
        (0 != flowData->txLmCountingLmepId[mdlIter1]))
    {
      /* Verify that rxLmCountingLmepId/txLmCountingLmepId are specified only for PduProcess and DataPassthru type flow only */
      if (!isPduProcessType &&
          !isDataPassthruType &&
          !isMipLtmProcessType &&
          !isMipLbmProcessType)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                           "rxLmCountingLmepId/txLmCountingLmepId can be specified for PduProcess "
                           "or DataPassthru or MipLtmProcess or MipLbmProcess flow types. \r\n", 0);
        return(0);
      }

      if (isDataPassthruType)
      {
        direction = 0;
        do
        {
          /* When direction is zero validate rxLmCountingLmepId
             When direction is one validate txLmCountingLmepId */
          if (direction == 0)
          {
            lmepId = flowData->rxLmCountingLmepId[mdlIter1];
            if ((rxLmepPresent == 0) &&
                (lmepId != 0))
            {
              rxLmepPresent = 1;
            }
          }
          else
          {
            lmepId = flowData->txLmCountingLmepId[mdlIter1];
            if ((txLmepPresent == 0) &&
                (lmepId != 0))
            {
              txLmepPresent = 1;
            }
          }

          if (0 != lmepId)
          {
            /* verify MP is configured for this LMEP_ID */
            if (ofdbOamMepGet(lmepId, &lmMepConfig, NULL) == OFDPA_E_NONE)
            {
              /* verify MEG is configured for MPs MEG index and verify MEG is of type Ethernet */
              if ((ofdbOamMegGet(lmMepConfig.megIndex, &megConfig, NULL, NULL) == OFDPA_E_NONE) &&
                  (megConfig.megType == OFDPA_OAM_MEG_TYPE_ETHERNET))
              {
                if (lmMepConfig.ifIndex != matchCriteria->outPort)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d should be configured on same interface specified "
                                     "in inPort (%d) of match criteria. \r\n",
                                     lmepId, matchCriteria->outPort);
                  return(0);
                }

                if (megConfig.primVid != (matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d should be configured with same VLAN as specified "
                                     "in vlanId (%d) of match criteria. \r\n",
                                     lmepId, (matchCriteria->vlanId | OFDPA_VID_EXACT_MASK));
                  return(0);
                }

                /* verify that rxLmCountAction is set for Ethernet Up MEP. */
                if ((direction == 0) && (lmMepConfig.direction != OFDPA_MP_DIRECTION_UP))
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d is not Ethernet Up MEP. \r\n",
                                     lmepId);
                  return(0);
                }

                /* verify that txLmCountAction is set for Ethernet Down MEP. */
                if ((direction == 1) && (lmMepConfig.direction != OFDPA_MP_DIRECTION_DOWN))
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d is not Ethernet Down MEP. \r\n",
                                     lmepId);
                  return(0);
                }
              }
              else
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                   "MEG corresponding to lmepId %d either doesnot exists or is not of type G8013.1 or Ethernet \r\n",
                                   lmepId);
                return(0);
              }
            }
            else
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Specified lmepId %d is not configured for any MEP. \r\n",
                                 lmepId);
              return(0);
            }
          }
          direction++;
        } while (direction < 2);
      }

      if (isPduProcessType ||
          isMipLbmProcessType ||
          isMipLtmProcessType)
      {
        direction = 0;
        do
        {
          /* When direction is zero validate rxLmCountingLmepId
             When direction is one validate txLmCountingLmepId */
          if (direction == 0)
          {
            lmepId = flowData->rxLmCountingLmepId[mdlIter1];
          }
          else
          {
            lmepId = flowData->txLmCountingLmepId[mdlIter1];
          }

          if (0 != lmepId)
          {
            /* verify MP is configured for this LMEP_ID */
            if (ofdbOamMepGet(lmepId, &lmMepConfig, NULL) == OFDPA_E_NONE)
            {
              /* verify MEG is configured for MPs MEG index and verify MEG is of type Ethernet */
              if ((ofdbOamMegGet(lmMepConfig.megIndex, &megConfig, NULL, NULL) == OFDPA_E_NONE) &&
                  (megConfig.megType == OFDPA_OAM_MEG_TYPE_ETHERNET))
              {
                /* verify that rxLmCountingLmepId/txLmCountingLmepId is not enabled for MDL level greater than or equal to MDL level in flow rule*/
                if (matchCriteria->oamY1731Mdl <= megConfig.level)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "For PduProcessType/MipLtmProcess/MipLbmProcess flow, rxLmCountingLmepId/txLmCountingLmepId cannot be enabled "
                                     "for MDL level %d greater than or equal to MDL level %d (oamY1731Mdl field in match criteria) \r\n",
                                     megConfig.level, matchCriteria->oamY1731Mdl);
                  return(0);
                }

                if (lmMepConfig.ifIndex != matchCriteria->outPort)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d should be configured on same interface specified "
                                     "in outPort (%d) of match criteria. \r\n",
                                     lmepId, matchCriteria->outPort);
                  return(0);
                }

                if (megConfig.primVid != (matchCriteria->vlanId & OFDPA_VID_EXACT_MASK))
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "Specified lmepId %d should be configured with same VLAN as specified "
                                     "in vlanId (%d) of match criteria. \r\n",
                                     lmepId, (matchCriteria->vlanId | OFDPA_VID_EXACT_MASK));
                  return(0);
                }

                /* verify that LM is enabled on lmepId specifiedin rxLmCountingLmepId/txLmCountingLmepId */
                ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT, &lmIngressMpFlow);
                lmIngressMpFlow.flowData.mpFlowEntry.match_criteria.inPort = matchCriteria->outPort;
                lmIngressMpFlow.flowData.mpFlowEntry.match_criteria.vlanId = matchCriteria->vlanId;
                lmIngressMpFlow.flowData.mpFlowEntry.match_criteria.vlanIdMask = matchCriteria->vlanIdMask;

                if (ofdbFlowGet(&lmIngressMpFlow, NULL, NULL) == OFDPA_E_NONE)
                {
                  if (lmMepConfig.direction == OFDPA_MP_DIRECTION_DOWN)
                  {
                    for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
                    {
                      if (lmIngressMpFlow.flowData.mpFlowEntry.rxLmCountingLmepId[mdlIter2] == lmepId)
                      {
                        ingressFlowFound = 1;
                        break;
                      }
                    }
                  }
                  else
                  {
                    for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
                    {
                      if (lmIngressMpFlow.flowData.mpFlowEntry.txLmCountingLmepId[mdlIter2] == lmepId)
                      {
                        ingressFlowFound = 1;
                        break;
                      }
                    }
                  }
                }

                if (!ingressFlowFound)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "LM is not enabled on lmepId %d \r\n",
                                     lmepId);
                  return(0);
                }

                ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT, &lmEgressMpFlow);
                lmEgressMpFlow.flowData.egressMpFlowEntry.match_criteria.outPort = matchCriteria->outPort;
                lmEgressMpFlow.flowData.egressMpFlowEntry.match_criteria.vlanId = matchCriteria->vlanId;
                lmEgressMpFlow.flowData.egressMpFlowEntry.match_criteria.vlanIdMask = matchCriteria->vlanIdMask;

                if (ofdbFlowGet(&lmEgressMpFlow, NULL, NULL) == OFDPA_E_NONE)
                {
                  if (lmMepConfig.direction == OFDPA_MP_DIRECTION_DOWN)
                  {
                    for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
                    {
                      if (lmEgressMpFlow.flowData.egressMpFlowEntry.txLmCountingLmepId[mdlIter2] == lmepId)
                      {
                        egressFlowFound = 1;
                        break;
                      }
                    }
                  }

                  if (lmMepConfig.direction == OFDPA_MP_DIRECTION_UP)
                  {
                    for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
                    {
                      if (lmEgressMpFlow.flowData.egressMpFlowEntry.rxLmCountingLmepId[mdlIter2] == lmepId)
                      {
                        egressFlowFound = 1;
                        break;
                      }
                    }
                  }
                }

                if (!egressFlowFound)
                {
                  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                     "LM is not enabled on lmepId %d \r\n",
                                     lmepId);
                  return(0);
                }
              }
              else
              {
                OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                   "MEG corresponding to lmepId %d either doesnot exists or is not of type G8013.1 or Ethernet \r\n",
                                   lmepId);
                return(0);
              }
            }
            else
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Specified lmepId %d is not configured for any MEP. \r\n",
                                 lmepId);
              return(0);
            }
          }
          direction++;
        } while (direction < 2);

        /* verify that lmepId specified in rxLmCountingLmepId and txLmCountingLmepId are not same */
        for (mdlIter2 = OFDPA_MDL_MIN_VALUE; mdlIter2 <= OFDPA_MDL_MAX_VALUE; mdlIter2++)
        {
          if ((flowData->rxLmCountingLmepId[mdlIter1] != 0) &&
              (flowData->txLmCountingLmepId[mdlIter2] != 0) &&
              (flowData->rxLmCountingLmepId[mdlIter1] == flowData->txLmCountingLmepId[mdlIter2]))
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                               "lmepId %d in rxLmCountingLmepId and lmepId %d in txLmCountingLmepId cannot be same. \r\n",
                               flowData->rxLmCountingLmepId[mdlIter1], flowData->txLmCountingLmepId[mdlIter2]);
            return(0);
          }
        }

      }
    }
  }

  if (isDataPassthruType)
  {
    if (!(rxLmepPresent ||
          txLmepPresent))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "For data pass through type flow atleast one element in rxLmCountingLmepId/txLmCountingLmepId should exist. \r\n",
                         0);
      return(0);
    }

    /* check for unwanted match criteria. */
    if ((matchCriteria->oamY1731MdlMask != OFDPA_OAM_Y1731_MDL_ALL_MASK) ||
        (matchCriteria->oamY1731OpcodeMask != OFDPA_OAM_Y1731_OPCODE_ALL_MASK))
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "For data pass through type flow the following match criteria may only have a mask of all. "
                         "oamY1731MdlMask = 0x%x oamY1731OpcodeMask = 0x%x \r\n",
                         matchCriteria->oamY1731MdlMask, matchCriteria->oamY1731OpcodeMask);
      return(0);
    }
  }
  else if (isPduDropLowerMdlType)
  {
    /* check for unwanted match criteria. */
    if (matchCriteria->oamY1731OpcodeMask != OFDPA_OAM_Y1731_OPCODE_ALL_MASK)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "For PDU drop type flow the following match criteria may only have a mask of all. "
                         "oamY1731OpcodeMask = 0x%x \r\n", matchCriteria->oamY1731OpcodeMask);
      return(0);
    }
  }

  return(1);
}


uint32_t ofdbFlowEgressDscpPcpRemarkEntryValidate(ofdpaEgressDscpPcpRemarkFlowEntry_t *flowData)
{
  ofdpaEgressDscpPcpRemarkFlowMatch_t *matchCriteria;
  uint8_t                             isDscpType = 0;

  matchCriteria = &flowData->match_criteria;

  /* validate QOS color match field */
  switch (matchCriteria->color)
  {
    case OFDPA_QOS_GREEN:
    case OFDPA_QOS_YELLOW:
    case OFDPA_QOS_RED:
      break;
    default:
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "QOS color value is not valid. color = %d\r\n",
                           matchCriteria->color);
        return 0;
      }
  }

  /* validate packet traffic class */
  if (matchCriteria->trafficClass > OFDPA_TRAFFIC_CLASS_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Traffic class value out of range. trafficClass = %d\r\n",
                       matchCriteria->trafficClass);
    return(0);
  }

  /* Remark DSCP PCP type - exact etherType required */
  if (OFDPA_ETHERTYPE_EXACT_MASK == matchCriteria->etherTypeMask)
  {
    if ((0x0800 == matchCriteria->etherType) || (0x86dd == matchCriteria->etherType))
    {
      isDscpType = 1;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                     "Invalid etherType value for exact match criteria. (etherType: 0x%x, etherTypeMask: 0x%x)\r\n",
                     matchCriteria->etherType, matchCriteria->etherTypeMask);
      return(0);
    }
  }
  else
  {
    isDscpType = 0;
  }

  /* validate output port */
  if (!ofdbPortIsValid(matchCriteria->outPort))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                       "Port not valid - 0x%x)\r\n",
                       matchCriteria->outPort);
    return(0);
  }

  /* validate goto instruction */
  if (flowData->gotoTableId != OFDPA_FLOW_TABLE_ID_EGRESS_TPID)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Goto table must be Egress TPID for Egress DSCP PCP Remark flow table. (gotoTableId: %d)\r\n",
                       flowData->gotoTableId);
    return(0);
  }

  /* apply actions */

  /* validate VLAN PCP value */
  if (flowData->vlanPcpAction &&
      flowData->vlanPcp > OFDPA_VLAN_PCP_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "VLAN PCP value out of range. vlanPcp = %d\r\n",
                       flowData->vlanPcp);
    return(0);
  }

  /* validate DEI value */
  if (flowData->vlanDeiAction &&
      flowData->vlanDei > OFDPA_VLAN_DEI_MAX_VALUE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "VLAN DEI value out of range. dei = %d\r\n",
                       flowData->vlanDei);
    return(0);
  }

  /* validate DSCP value */
  if (flowData->dscpAction)
  {
    if (flowData->dscp > OFDPA_DSCP_MAX_VALUE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "DSCP value out of range. dscp = %d\r\n",
                       flowData->dscp);
      return(0);
    }

    if (isDscpType == 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "Invalid etherType(Mask) value. (etherType: 0x%x, etherTypeMask: 0x%x)\r\n",
                     matchCriteria->etherType, matchCriteria->etherTypeMask);
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "Valid etherType(Mask) required for DSCP action\r\n", 0);
      return(0);
    }
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowEgressTpidEntryValidate(ofdpaEgressTpidFlowEntry_t *flowData)
{
  ofdpaEgressTpidFlowMatch_t *matchCriteria;

  /* check match criteria constraints */
  matchCriteria = &flowData->match_criteria;

  /* outPort must be a valid interface */
  if ((OFDB_PORT_TYPE(matchCriteria->outPort) != OFDPA_PORT_TYPE_PHYSICAL) ||
      (!ofdbPortIsValid(matchCriteria->outPort)))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Port is not valid. outPort = %d\r\n",
                       matchCriteria->outPort);
    return(0);
  }

  if (OFDPA_VID_PRESENT != matchCriteria->vlanId)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "VLAN ID must only have the VID present bit set. VLAN ID = %d\r\n",
                       matchCriteria->vlanId);
    return(0);
  }

  if (OFDPA_VID_PRESENT != matchCriteria->vlanIdMask)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "VLAN ID mask must only have the VID present bit set. mask = %d\r\n",
                       matchCriteria->vlanIdMask);
    return(0);
  }

  /* check instructions */
  if (flowData->newTpid != 0x88a8)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                       "Egress TPID flow must specify newTpid of 0x88a8. (newTpid = 0x%04x\r\n",
                       flowData->newTpid);
    return(0);
  }

  /* if you get here, you are valid */
  return(1);
}

uint32_t ofdbFlowEntryValidate(ofdpaFlowEntry_t *flow)
{
  uint32_t flowValid;

  /* first check if idle time specified for table that does not support it */
  if (flow->idle_time != 0)
  {
    /* the tables in the case are those that support idle_time */
    switch (flow->tableId)
    {
      case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      case OFDPA_FLOW_TABLE_ID_BRIDGING:
      case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
        /* continue to further validation steps */
        break;

      default:
        flowValid = 0;
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                           "Flow table does not support idle_time. %d\r\n",
                           flow->tableId);
        return(flowValid);
    }
  }

  switch (flow->tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      flowValid = ofdbFlowIngressPortEntryValidate(&flow->flowData.ingressPortFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
      flowValid = ofdbFlowPortDscpTrustEntryValidate(&flow->flowData.dscpTrustFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
      flowValid = ofdbFlowPortPcpTrustEntryValidate(&flow->flowData.pcpTrustFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
      flowValid = ofdbFlowTunnelDscpTrustEntryValidate(&flow->flowData.dscpTrustFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
      flowValid = ofdbFlowTunnelPcpTrustEntryValidate(&flow->flowData.pcpTrustFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
      flowValid = ofdbFlowInjectedOamEntryValidate(&flow->flowData.injectedOamFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN:
      flowValid = dpFlowVlanEntryValidate(&flow->flowData.vlanFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN_1:
      flowValid = ofdbFlowVlan1EntryValidate(&flow->flowData.vlan1FlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      flowValid = ofdbFlowMpEntryValidate(&flow->flowData.mpFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      flowValid = ofdbFlowMplsL2PortEntryValidate(&flow->flowData.mplsL2PortFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
      flowValid = ofdbFlowMplsDscpTrustEntryValidate(&flow->flowData.dscpTrustFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
      flowValid = ofdbFlowMplsPcpTrustEntryValidate(&flow->flowData.pcpTrustFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
      flowValid = ofdbFlowMplsQosClassEntryValidate(&flow->flowData.mplsL2PortQosFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
      flowValid = ofdbFlowL2PolicerEntryValidate(&flow->flowData.l2PolicerFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
      flowValid = ofdbFlowL2PolicerActionsEntryValidate(&flow->flowData.l2PolicerActionsFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      flowValid = ofdbFlowTerminationMacEntryValidate(&flow->flowData.terminationMacFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_0:
      flowValid = ofdbFlowMpls0EntryValidate(&flow->flowData.mplsFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_1:
      flowValid = ofdbFlowMpls1EntryValidate(&flow->flowData.mplsFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
      flowValid = ofdbFlowMplsMpEntryValidate(&flow->flowData.mplsMpFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
      flowValid = ofdbFlowMplsLabelTrustEntryValidate(&flow->flowData.mplsLabelTrustFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      flowValid = ofdbFlowUnicastRoutingEntryValidate(&flow->flowData.unicastRoutingFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      flowValid = ofdbFlowMulticastRoutingEntryValidate(&flow->flowData.multicastRoutingFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      flowValid = ofdbFlowBridgingEntryValidate(&flow->flowData.bridgingFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      flowValid = ofdbFlowPolicyAclEntryValidate(&flow->flowData.policyAclFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
      flowValid = ofdbFlowColorActionsEntryValidate(&flow->flowData.colorActionsFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
      flowValid = ofdbFlowEgressVlanEntryValidate(&flow->flowData.egressVlanFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
      flowValid = ofdbFlowEgressVlan1EntryValidate(&flow->flowData.egressVlan1FlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
      flowValid = ofdbFlowEgressMpEntryValidate(&flow->flowData.egressMpFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
      flowValid = ofdbFlowEgressDscpPcpRemarkEntryValidate(&flow->flowData.egressDscpPcpRemarkFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
      flowValid = ofdbFlowEgressTpidEntryValidate(&flow->flowData.egressTpidFlowEntry);
      break;

    default:
      flowValid = 0;
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "unknown flow table ID. %d\r\n", flow->tableId);
      break;
  }
  return(flowValid);
}

uint32_t ofdbFlowIngressPortDeletionValidate(ofdpaIngressPortFlowEntry_t *flowData)
{
  ofdpaFlowEntry_t flow;

  /* the flow entry that allows tunnel packets cannot be deleted if a tunnel bridging flow exists */
  memset(&flow, 0, sizeof(flow));
  flow.tableId = OFDPA_FLOW_TABLE_ID_BRIDGING;

  while (ofdbFlowNextGet(&flow, &flow, NULL, NULL) == OFDPA_E_NONE)
  {
    if (flow.flowData.bridgingFlowEntry.match_criteria.tunnelId != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                         "flow for tunnel packets exists in Bridging table, deletion invalid.\r\n", 0);
      return(0);
    }
  }

  /* if you get here, OK to delete flow */
  return(1);
}

uint32_t ofdbFlowVlanDeletionValidate(ofdpaVlanFlowEntry_t *flowData)
{
  /* if you get here, OK to delete flow */
  return(1);
}

uint32_t ofdbFlowDeletionValidate(ofdpaFlowEntry_t *flow)
{
  uint32_t deletionValid;

  switch (flow->tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      deletionValid = ofdbFlowIngressPortDeletionValidate(&flow->flowData.ingressPortFlowEntry);
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN:
      deletionValid = ofdbFlowVlanDeletionValidate(&flow->flowData.vlanFlowEntry);
      break;

    default:
      deletionValid = 1;
      break;
  }

  return(deletionValid);
}

uint32_t ofdbFlowTableMaxCountGet(OFDPA_FLOW_TABLE_ID_t tableId)
{
  uint32_t value;

  switch (tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      value = ofdbFlowTableStatus->ingressPortMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
      value = ofdbFlowTableStatus->portDscpTrustMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
      value = ofdbFlowTableStatus->portPcpTrustMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
      value = ofdbFlowTableStatus->tunnelDscpTrustMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
      value = ofdbFlowTableStatus->tunnelPcpTrustMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
      value = ofdbFlowTableStatus->injectedOamMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN:
      value = ofdbFlowTableStatus->vlanMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN_1:
      value = ofdbFlowTableStatus->vlan1MaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      value = ofdbFlowTableStatus->mpMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      value = ofdbFlowTableStatus->mplsL2PortMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
      value = ofdbFlowTableStatus->mplsDscpTrustMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
      value = ofdbFlowTableStatus->mplsPcpTrustMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
      value = ofdbFlowTableStatus->mplsQosClassMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
      value = ofdbFlowTableStatus->l2PolicerMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
      value = ofdbFlowTableStatus->l2PolicerActionsMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      value = ofdbFlowTableStatus->terminationMacMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_0:
      value = ofdbFlowTableStatus->mpls0MaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_1:
      value = ofdbFlowTableStatus->mpls1MaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
      value = ofdbFlowTableStatus->mplsMpMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
      value = ofdbFlowTableStatus->mplsLabelTrustMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      value = ofdbFlowTableStatus->unicastRoutingMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      value = ofdbFlowTableStatus->multicastRoutingMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      value = ofdbFlowTableStatus->bridgingMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      value = ofdbFlowTableStatus->policyAclMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
      value = ofdbFlowTableStatus->colorActionsMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
      value = ofdbFlowTableStatus->egressVlanMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
      value = ofdbFlowTableStatus->egressVlan1MaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
      value = ofdbFlowTableStatus->egressMpMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
      value = ofdbFlowTableStatus->egressDscpPcpRemarkMaxCount;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
      value = ofdbFlowTableStatus->egressTpidMaxCount;
      break;

    default:
      value = 0;
      break;
  }

  return(value);
}

uint32_t ofdbFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_t tableId)
{
  uint32_t value;

  switch (tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      value = ofdbAVLTrees.ofdbIngressPortFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
      value = ofdbAVLTrees.ofdbPortDscpTrustFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
      value = ofdbAVLTrees.ofdbPortPcpTrustFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
      value = ofdbAVLTrees.ofdbTunnelDscpTrustFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
      value = ofdbAVLTrees.ofdbTunnelPcpTrustFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
      value = ofdbAVLTrees.ofdbInjectedOamFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN:
      value = ofdbAVLTrees.ofdbVlanFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN_1:
      value = ofdbAVLTrees.ofdbVlan1FlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      value = ofdbAVLTrees.ofdbMpFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      value = ofdbAVLTrees.ofdbMplsL2PortFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
      value = ofdbAVLTrees.ofdbMplsDscpTrustFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
      value = ofdbAVLTrees.ofdbMplsPcpTrustFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
      value = ofdbAVLTrees.ofdbMplsQosClassFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
      value = ofdbAVLTrees.ofdbL2PolicerFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
      value = ofdbAVLTrees.ofdbL2PolicerActionsFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      value = ofdbAVLTrees.ofdbTerminationMacFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_0:
      value = ofdbAVLTrees.ofdbMpls0FlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_1:
      value = ofdbAVLTrees.ofdbMpls1FlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
      value = ofdbAVLTrees.ofdbMplsMpFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
      value = ofdbAVLTrees.ofdbMplsLabelTrustFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      value = ofdbAVLTrees.ofdbUnicastRoutingFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      value = ofdbAVLTrees.ofdbMulticastRoutingFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      value = ofdbAVLTrees.ofdbBridgingFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      value = ofdbAVLTrees.ofdbPolicyAclFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
      value = ofdbAVLTrees.ofdbColorActionsFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
      value = ofdbAVLTrees.ofdbEgressVlanFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
      value = ofdbAVLTrees.ofdbEgressVlan1FlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
      value = ofdbAVLTrees.ofdbEgressMpFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
      value = ofdbAVLTrees.ofdbEgressDscpPcpRemarkFlowTable_tree.count;
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
      value = ofdbAVLTrees.ofdbEgressTpidFlowTable_tree.count;
      break;

    default:
      value = 0;
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC, "unknown flow table ID. %d\r\n", tableId);
      break;
  }

  return(value);
}

/* Flow table utility functions
*/
/*********************************************************************
*
* @purpose  Convert match criteria into the Flow table key.
*
* @param    match @b{(input)} Match Criteria for this flow.
* @param    key @b{(output)} Key into the table.
*
* @returns  None
*
* @comments none
*
* @end
*
*********************************************************************/
void ofdbIngressPortFlowUtilKeyCreate(ofdpaIngressPortFlowMatch_t *match,
                                      ofdbIngressPortFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->inPort = match->inPort & match->inPortMask;
  key->tunnelId = match->tunnelId & match->tunnelIdMask;
  key->etherType = match->etherType & match->etherTypeMask;
  //added by yjg begin ingressPort流表增加lmepId作为key
  key->lmepId = match->lmepId & match->lmepIdMask;
  //added by yjg end
}

void ofdbInjectedOamFlowUtilKeyCreate(ofdpaInjectedOamFlowMatch_t *match,
                                      ofdbInjectedOamFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->lmepId = match->lmepId;
}

void ofdbVlanFlowUtilKeyCreate(ofdpaVlanFlowMatch_t *match,
                               ofdbVlanFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->inPort = match->inPort;
  key->vlanId = match->vlanId & match->vlanIdMask;
}

void ofdbVlan1FlowUtilKeyCreate(ofdpaVlan1FlowMatch_t *match,
                                ofdbVlan1FlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->inPort = match->inPort;
  key->vlanId = match->vlanId;
  key->ovid   = match->ovid;
}

void ofdbMpFlowUtilKeyCreate(ofdpaMpFlowMatch_t *match,
                             ofdbMpFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->inPort         = match->inPort;
  key->vlanId         = match->vlanId & match->vlanIdMask;
  key->etherType      = match->etherType & match->etherTypeMask;
  key->oamY1731Mdl    = match->oamY1731Mdl & match->oamY1731MdlMask;
  key->oamY1731Opcode = match->oamY1731Opcode & match->oamY1731OpcodeMask;
  ofdbMacMaskApply(&match->destMac, &match->destMacMask, &key->destMac);
}

void ofdbL2PolicerFlowUtilKeyCreate(ofdpaL2PolicerFlowMatch_t *match,
                                    ofdbL2PolicerFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->tunnelId = match->tunnelId;
  key->mplsL2Port = match->mplsL2Port & match->mplsL2PortMask;
}

void ofdbL2PolicerActionsFlowUtilKeyCreate(ofdpaL2PolicerActionsFlowMatch_t *match,
                                           ofdbL2PolicerActionsFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));
  key->color = match->color;
  key->colorActionsIndex = match->colorActionsIndex;
}

void ofdbTerminationMacFlowUtilKeyCreate(ofdpaTerminationMacFlowMatch_t *match,
                                         ofdbTerminationMacFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->inPort = match->inPort & match->inPortMask;
  key->vlanId = match->vlanId & match->vlanIdMask;
  key->etherType = match->etherType;
  ofdbMacMaskApply(&match->destMac, &match->destMacMask, &key->destMac);
}

void ofdbMplsFlowUtilKeyCreate(ofdpaMplsFlowMatch_t *match,
                               ofdbMplsFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->mplsLabel = match->mplsLabel;
  key->mplsAchChannel = match->mplsAchChannel & match->mplsAchChannelMask;
  key->mplsDataFirstNibble = match->mplsDataFirstNibble & match->mplsDataFirstNibbleMask;
#if 0
  key->nextLabelIsGal = match->nextLabelIsGal & match->nextLabelIsGalMask;
  key->mplsAchChannel = match->mplsAchChannel & match->mplsAchChannelMask;
  key->inPort = match->inPort & match->inPortMask;
  key->etherType = match->etherType;
  key->mplsBos = match->mplsBos;
  key->mplsTtl = match->mplsTtl & match->mplsTtlMask;
  key->destIp4 = match->destIp4 & match->destIp4Mask;

  ofdbIp6AddrMaskApply(&match->destIp6, &match->destIp6Mask, &key->destIp6);

  key->ipProto = match->ipProto & match->ipProtoMask;
  key->udpSrcPort = match->udpSrcPort & match->udpSrcPortMask;
  key->udpDstPort = match->udpDstPort & match->udpDstPortMask;
#endif
}

void ofdbMplsMpFlowUtilKeyCreate(ofdpaMplsMpFlowMatch_t *match,
                                ofdbMplsMpFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->lmepId         = match->lmepId;
  key->oamY1731Opcode = match->oamY1731Opcode;
}

void ofdbBridgingFlowUtilKeyCreate(ofdpaBridgingFlowMatch_t *match,
                                   ofdbBridgingFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->vlanId = match->vlanId;
  key->tunnelId = match->tunnelId & match->tunnelIdMask;
  ofdbMacMaskApply(&match->destMac, &match->destMacMask, &key->destMac);
}

void ofdbMplsLabelTrustFlowUtilKeyCreate(ofdpaMplsLabelTrustFlowMatch_t *match,
                                         ofdbMplsLabelTrustFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->qosIndex = match->qosIndex;
  key->mplsTC = match->mplsTC;
}

void ofdbUnicastRoutingFlowUtilKeyCreate(ofdpaUnicastRoutingFlowMatch_t *match,
                                         ofdbUnicastRoutingFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->etherType = match->etherType;
  key->vrf = match->vrf & match->vrfMask;

  switch (match->etherType)
  {
    case 0x0800:
      key->dstIp4 = match->dstIp4 & match->dstIp4Mask;
      break;
    case 0x86dd:
      ofdbIp6AddrMaskApply(&match->dstIp6, &match->dstIp6Mask, &key->dstIp6);
      break;
    default:
      break;
  }
}

void ofdbMulticastRoutingFlowUtilKeyCreate(ofdpaMulticastRoutingFlowMatch_t *match,
                                           ofdbMulticastRoutingFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->etherType = match->etherType;
  key->vlanId = match->vlanId;
  key->vrf = match->vrf & match->vrfMask;

  switch (match->etherType)
  {
    case 0x0800:
      key->srcIp4 = match->srcIp4 & match->srcIp4Mask;
      key->dstIp4 = match->dstIp4;
      break;
    case 0x86dd:
      ofdbIp6AddrMaskApply(&match->srcIp6, &match->srcIp6Mask, &key->srcIp6);
      memcpy(&key->dstIp6, &match->dstIp6, sizeof(key->dstIp6));
      break;
    default:
      break;
  }
}

void ofdbPolicyAclFlowUtilKeyCreate(ofdpaPolicyAclFlowMatch_t *match,
                                    ofdbPolicyAclFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->inPort = match->inPort & match->inPortMask;
  key->mplsL2Port = match->mplsL2Port & match->mplsL2PortMask;
  key->etherType = match->etherType & match->etherTypeMask;

  ofdbMacMaskApply(&match->srcMac, &match->srcMacMask, &key->srcMac);
  ofdbMacMaskApply(&match->destMac, &match->destMacMask, &key->destMac);

  key->vlanId = match->vlanId & match->vlanIdMask;
  key->vlanPcp = match->vlanPcp & match->vlanPcpMask;
  key->vlanDei = match->vlanDei & match->vlanDeiMask;

  key->tunnelId = match->tunnelId & match->tunnelIdMask;

  key->vrf = match->vrf & match->vrfMask;

  key->sourceIp4 = match->sourceIp4 & match->sourceIp4Mask;
  key->destIp4 = match->destIp4 & match->destIp4Mask;

  ofdbIp6AddrMaskApply(&match->sourceIp6, &match->sourceIp6Mask, &key->sourceIp6);
  ofdbIp6AddrMaskApply(&match->destIp6, &match->destIp6Mask, &key->destIp6);

  key->ipv4ArpSpa = match->ipv4ArpSpa & match->ipv4ArpSpaMask;
  key->ipProto = match->ipProto & match->ipProtoMask;
  key->dscp = match->dscp & match->dscpMask;
  key->ecn = match->ecn & match->ecnMask;

  key->srcL4Port = match->srcL4Port & match->srcL4PortMask;
  key->destL4Port = match->destL4Port & match->destL4PortMask;

  key->icmpType = match->icmpType & match->icmpTypeMask;
  key->icmpCode = match->icmpCode & match->icmpCodeMask;

  key->ipv6FlowLabel = match->ipv6FlowLabel & match->ipv6FlowLabelMask;
}

void ofdbColorActionsFlowUtilKeyCreate(ofdpaColorActionsFlowMatch_t *match,
                                       ofdbColorActionsFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));
  key->color = match->color;
  key->index = match->index;
}

void ofdbEgressVlanFlowUtilKeyCreate(ofdpaEgressVlanFlowMatch_t *match,
                                     ofdbEgressVlanFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->outPort              = match->outPort;
  key->vlanId               = (match->vlanId & match->vlanIdMask);
  key->allowVlanTranslation = match->allowVlanTranslation;
  key->tunnelId             = (match->tunnelId & match->tunnelIdMask);
  key->mplsL2Port           = (match->mplsL2Port & match->mplsL2PortMask);
}

void ofdbEgressVlan1FlowUtilKeyCreate(ofdpaEgressVlan1FlowMatch_t *match,
                                      ofdbEgressVlan1FlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->outPort  = match->outPort;
  key->vlanId   = match->vlanId;
  key->ovid     = match->ovid;
}

void ofdbEgressMpFlowUtilKeyCreate(ofdpaEgressMpFlowMatch_t *match,
                                   ofdbEgressMpFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->outPort        = match->outPort;
  key->vlanId         = match->vlanId & match->vlanIdMask;
  key->etherType      = match->etherType & match->etherTypeMask;
  key->oamY1731Mdl    = match->oamY1731Mdl & match->oamY1731MdlMask;
  key->oamY1731Opcode = match->oamY1731Opcode & match->oamY1731OpcodeMask;
  ofdbMacMaskApply(&match->destMac, &match->destMacMask, &key->destMac);
}

void ofdbEgressDscpPcpRemarkFlowUtilKeyCreate(ofdpaEgressDscpPcpRemarkFlowMatch_t *match,
                                              ofdbEgressDscpPcpRemarkFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->outPort        = match->outPort;
  key->etherType      = match->etherType & match->etherTypeMask;
  key->trafficClass   = match->trafficClass;
  key->color          = match->color;
}

void ofdbEgressTpidFlowUtilKeyCreate(ofdpaEgressTpidFlowMatch_t *match,
                                     ofdbEgressTpidFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->outPort = match->outPort;
}

void ofdbMplsL2PortFlowUtilKeyCreate(ofdpaMplsL2PortFlowMatch_t *match,
                                     ofdbMplsL2PortFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->mplsL2Port = match->mplsL2Port & match->mplsL2PortMask;
  key->tunnelId   = match->tunnelId;
  key->etherType    = match->etherType & match->etherTypeMask;
}

void ofdbDscpTrustFlowUtilKeyCreate(ofdpaDscpTrustFlowMatch_t *match,
                                    ofdbDscpTrustFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->qosIndex = match->qosIndex;
  key->dscpValue = match->dscpValue;
}

void ofdbPcpTrustFlowUtilKeyCreate(ofdpaPcpTrustFlowMatch_t *match,
                                   ofdbPcpTrustFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->qosIndex = match->qosIndex;
  key->pcpValue = match->pcpValue;
  key->dei = match->dei;
}

void ofdbMplsQosClassFlowUtilKeyCreate(ofdpaMplsL2PortQosClassFlowMatch_t *match,
                                       ofdbMplsQosClassFlowEntryKey_t *key)
{
  memset(key, 0, sizeof(*key));

  key->mplsL2Port = match->mplsL2Port;
  key->tunnelId = match->tunnelId;
  key->etherType = match->etherType & match->etherTypeMask;

  ofdbMacMaskApply(&match->srcMac, &match->srcMacMask, &key->srcMac);
  ofdbMacMaskApply(&match->destMac, &match->destMacMask, &key->destMac);

  key->vlanPcp = match->vlanPcp & match->vlanPcpMask;
  key->vlanDei = match->vlanDei & match->vlanDeiMask;

  key->sourceIp4 = match->sourceIp4 & match->sourceIp4Mask;
  key->destIp4 = match->destIp4 & match->destIp4Mask;

  ofdbIp6AddrMaskApply(&match->sourceIp6, &match->sourceIp6Mask, &key->sourceIp6);
  ofdbIp6AddrMaskApply(&match->destIp6, &match->destIp6Mask, &key->destIp6);

  key->ipProto = match->ipProto & match->ipProtoMask;
  key->dscp = match->dscp & match->dscpMask;

  key->srcL4Port = match->srcL4Port & match->srcL4PortMask;
  key->destL4Port = match->destL4Port & match->destL4PortMask;
}

void ofdbIngressPortFlowNodePack(ofdpaFlowEntry_t *flow,
                                 ofdbIngressPortFlowTable_node_t *node)
{
  memcpy(&node->ingressPortFlowEntry, &flow->flowData.ingressPortFlowEntry, sizeof(ofdpaIngressPortFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbIngressPortFlowNodeUnpack(ofdbIngressPortFlowTable_node_t *node,
                                   ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_INGRESS_PORT;

  memcpy(&flow->flowData.ingressPortFlowEntry, &node->ingressPortFlowEntry, sizeof(ofdpaIngressPortFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbInjectedOamFlowNodePack(ofdpaFlowEntry_t *flow,
                                 ofdbInjectedOamFlowTable_node_t *node)
{
  memcpy(&node->injectedOamFlowEntry, &flow->flowData.injectedOamFlowEntry, sizeof(ofdpaInjectedOamFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbInjectedOamFlowNodeUnPack(ofdbInjectedOamFlowTable_node_t *node,
                                   ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_INJECTED_OAM;

  memcpy(&flow->flowData.injectedOamFlowEntry, &node->injectedOamFlowEntry, sizeof(ofdpaInjectedOamFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbVlanFlowNodePack(ofdpaFlowEntry_t *flow,
                          ofdbVlanFlowTable_node_t *node)
{
  memcpy(&node->vlanFlowEntry, &flow->flowData.vlanFlowEntry, sizeof(ofdpaVlanFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbVlanFlowNodeUnpack(ofdbVlanFlowTable_node_t *node,
                            ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_VLAN;

  memcpy(&flow->flowData.vlanFlowEntry, &node->vlanFlowEntry, sizeof(ofdpaVlanFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbVlan1FlowNodePack(ofdpaFlowEntry_t *flow,
                           ofdbVlan1FlowTable_node_t *node)
{
  memcpy(&node->vlan1FlowEntry, &flow->flowData.vlan1FlowEntry, sizeof(ofdpaVlan1FlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbVlan1FlowNodeUnpack(ofdbVlan1FlowTable_node_t *node,
                             ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_VLAN_1;

  memcpy(&flow->flowData.vlan1FlowEntry, &node->vlan1FlowEntry, sizeof(ofdpaVlan1FlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbMpFlowNodePack(ofdpaFlowEntry_t *flow,
                           ofdbMpFlowTable_node_t *node)
{
  memcpy(&node->mpFlowEntry, &flow->flowData.mpFlowEntry, sizeof(ofdpaMpFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbMpFlowNodeUnpack(ofdbMpFlowTable_node_t *node,
                             ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT;

  memcpy(&flow->flowData.mpFlowEntry, &node->mpFlowEntry, sizeof(ofdpaMpFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbMplsL2PortFlowNodePack(ofdpaFlowEntry_t *flow,
                                ofdbMplsL2PortFlowTable_node_t *node)
{
  memcpy(&node->mplsL2PortFlowEntry, &flow->flowData.mplsL2PortFlowEntry, sizeof(ofdpaMplsL2PortFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbMplsL2PortFlowNodeUnpack(ofdbMplsL2PortFlowTable_node_t *node,
                                  ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT;

  memcpy(&flow->flowData.mplsL2PortFlowEntry, &node->mplsL2PortFlowEntry, sizeof(ofdpaMplsL2PortFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbMplsQosClassFlowNodePack(ofdpaFlowEntry_t *flow,
                                  ofdbMplsQosClassFlowTable_node_t *node)
{
  memcpy(&node->mplsQosClassFlowEntry, &flow->flowData.mplsL2PortQosFlowEntry, sizeof(ofdpaMplsL2PortQosClassFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbMplsQosClassFlowNodeUnpack(ofdbMplsQosClassFlowTable_node_t *node,
                                    ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS;

  memcpy(&flow->flowData.mplsL2PortQosFlowEntry, &node->mplsQosClassFlowEntry, sizeof(ofdpaMplsL2PortQosClassFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbL2PolicerFlowNodePack(ofdpaFlowEntry_t *flow,
                               ofdbL2PolicerFlowTable_node_t *node)
{
  memcpy(&node->l2PolicerFlowEntry, &flow->flowData.l2PolicerFlowEntry, sizeof(ofdpaL2PolicerFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbL2PolicerFlowNodeUnpack(ofdbL2PolicerFlowTable_node_t *node,
                                 ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_L2_POLICER;

  memcpy(&flow->flowData.l2PolicerFlowEntry, &node->l2PolicerFlowEntry, sizeof(ofdpaL2PolicerFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbL2PolicerActionsFlowNodePack(ofdpaFlowEntry_t *flow,
                                      ofdbL2PolicerActionsFlowTable_node_t *node)
{
  memcpy(&node->l2PolicerActionsFlowEntry, &flow->flowData.l2PolicerActionsFlowEntry, sizeof(ofdpaL2PolicerActionsFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbL2PolicerActionsFlowNodeUnpack(ofdbL2PolicerActionsFlowTable_node_t *node,
                                        ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS;

  memcpy(&flow->flowData.l2PolicerActionsFlowEntry, &node->l2PolicerActionsFlowEntry, sizeof(ofdpaL2PolicerActionsFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbTerminationMacFlowNodePack(ofdpaFlowEntry_t *flow,
                                    ofdbTerminationMacFlowTable_node_t *node)
{
  memcpy(&node->terminationMacFlowEntry, &flow->flowData.terminationMacFlowEntry, sizeof(ofdpaTerminationMacFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbTerminationMacFlowNodeUnpack(ofdbTerminationMacFlowTable_node_t *node,
                                      ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC;

  memcpy(&flow->flowData.terminationMacFlowEntry, &node->terminationMacFlowEntry, sizeof(ofdpaTerminationMacFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbBridgingFlowNodePack(ofdpaFlowEntry_t *flow,
                              ofdbBridgingFlowTable_node_t *node)
{
  memcpy(&node->bridgingFlowEntry, &flow->flowData.bridgingFlowEntry, sizeof(ofdpaBridgingFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbBridgingFlowNodeUnpack(ofdbBridgingFlowTable_node_t *node,
                                ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_BRIDGING;

  memcpy(&flow->flowData.bridgingFlowEntry, &node->bridgingFlowEntry, sizeof(ofdpaBridgingFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbMplsLabelTrustFlowNodePack(ofdpaFlowEntry_t *flow,
                                    ofdbMplsLabelTrustFlowTable_node_t *node)
{
  memcpy(&node->mplsLabelTrustFlowEntry, &flow->flowData.mplsLabelTrustFlowEntry, sizeof(ofdpaMplsLabelTrustFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbMplsLabelTrustFlowNodeUnpack(ofdbMplsLabelTrustFlowTable_node_t *node,
                                      ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST;

  memcpy(&flow->flowData.mplsLabelTrustFlowEntry, &node->mplsLabelTrustFlowEntry, sizeof(ofdpaMplsLabelTrustFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbUnicastRoutingFlowNodePack(ofdpaFlowEntry_t *flow,
                                    ofdbUnicastRoutingFlowTable_node_t *node)
{
  memcpy(&node->unicastRoutingFlowEntry, &flow->flowData.unicastRoutingFlowEntry, sizeof(ofdpaUnicastRoutingFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbUnicastRoutingFlowNodeUnpack(ofdbUnicastRoutingFlowTable_node_t *node,
                                      ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING;

  memcpy(&flow->flowData.unicastRoutingFlowEntry, &node->unicastRoutingFlowEntry, sizeof(ofdpaUnicastRoutingFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbMulticastRoutingFlowNodePack(ofdpaFlowEntry_t *flow,
                                      ofdbMulticastRoutingFlowTable_node_t *node)
{
  memcpy(&node->multicastRoutingFlowEntry, &flow->flowData.multicastRoutingFlowEntry, sizeof(ofdpaMulticastRoutingFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbMulticastRoutingFlowNodeUnpack(ofdbMulticastRoutingFlowTable_node_t *node,
                                        ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING;

  memcpy(&flow->flowData.multicastRoutingFlowEntry, &node->multicastRoutingFlowEntry, sizeof(ofdpaMulticastRoutingFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbMplsFlowNodePack(ofdpaFlowEntry_t *flow,
                          ofdbMplsFlowTable_node_t *node)
{
  memcpy(&node->mplsFlowEntry, &flow->flowData.mplsFlowEntry, sizeof(ofdpaMplsFlowEntry_t));

  node->tableId   = flow->tableId;

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->cookie    = flow->cookie;
}

void ofdbMplsFlowNodeUnpack(ofdbMplsFlowTable_node_t *node,
                            ofdpaFlowEntry_t *flow)
{
  flow->tableId = node->tableId;

  memcpy(&flow->flowData.mplsFlowEntry, &node->mplsFlowEntry, sizeof(ofdpaMplsFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->cookie    = node->cookie;
}

void ofdbMplsMpFlowNodePack(ofdpaFlowEntry_t *flow,
                            ofdbMplsMpFlowTable_node_t *node)
{
  memcpy(&node->mplsMpFlowEntry, &flow->flowData.mplsMpFlowEntry, sizeof(ofdpaMplsMpFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbMplsMpFlowNodeUnpack(ofdbMplsMpFlowTable_node_t *node,
                              ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT;

  memcpy(&flow->flowData.mplsMpFlowEntry, &node->mplsMpFlowEntry, sizeof(ofdpaMplsMpFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbPolicyAclFlowNodePack(ofdpaFlowEntry_t *flow,
                               ofdbPolicyAclFlowTable_node_t *node)
{
  memcpy(&node->policyAclFlowEntry, &flow->flowData.policyAclFlowEntry, sizeof(ofdpaPolicyAclFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbPolicyAclFlowNodeUnpack(ofdbPolicyAclFlowTable_node_t *node,
                                 ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_ACL_POLICY;

  memcpy(&flow->flowData.policyAclFlowEntry, &node->policyAclFlowEntry, sizeof(ofdpaPolicyAclFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbColorActionsFlowNodePack(ofdpaFlowEntry_t *flow,
                                  ofdbColorActionsFlowTable_node_t *node)
{
  memcpy(&node->colorActionsFlowEntry, &flow->flowData.colorActionsFlowEntry, sizeof(ofdpaColorActionsFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbColorActionsFlowNodeUnpack(ofdbColorActionsFlowTable_node_t *node,
                                    ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS;

  memcpy(&flow->flowData.colorActionsFlowEntry, &node->colorActionsFlowEntry, sizeof(ofdpaColorActionsFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbEgressVlanFlowNodePack(ofdpaFlowEntry_t *flow,
                                ofdbEgressVlanFlowTable_node_t *node)
{
  memcpy(&node->egressVlanFlowEntry, &flow->flowData.egressVlanFlowEntry, sizeof(ofdpaEgressVlanFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbEgressVlanFlowNodeUnpack(ofdbEgressVlanFlowTable_node_t *node,
                                  ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_EGRESS_VLAN;

  memcpy(&flow->flowData.egressVlanFlowEntry, &node->egressVlanFlowEntry, sizeof(ofdpaEgressVlanFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbEgressVlan1FlowNodePack(ofdpaFlowEntry_t *flow,
                                 ofdbEgressVlan1FlowTable_node_t *node)
{
  memcpy(&node->egressVlan1FlowEntry, &flow->flowData.egressVlan1FlowEntry, sizeof(ofdpaEgressVlan1FlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbEgressVlan1FlowNodeUnpack(ofdbEgressVlan1FlowTable_node_t *node,
                                   ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1;

  memcpy(&flow->flowData.egressVlan1FlowEntry, &node->egressVlan1FlowEntry, sizeof(ofdpaEgressVlan1FlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbEgressMpFlowNodePack(ofdpaFlowEntry_t *flow,
                           ofdbEgressMpFlowTable_node_t *node)
{
  memcpy(&node->egressMpFlowEntry, &flow->flowData.egressMpFlowEntry, sizeof(ofdpaEgressMpFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbEgressMpFlowNodeUnpack(ofdbEgressMpFlowTable_node_t *node,
                             ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT;

  memcpy(&flow->flowData.egressMpFlowEntry, &node->egressMpFlowEntry, sizeof(ofdpaEgressMpFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbEgressDscpPcpRemarkFlowNodePack(ofdpaFlowEntry_t *flow,
                                   ofdbEgressDscpPcpRemarkFlowTable_node_t *node)
{
  memcpy(&node->egressDscpPcpRemarkFlowEntry, &flow->flowData.egressDscpPcpRemarkFlowEntry,
         sizeof(ofdpaEgressDscpPcpRemarkFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbEgressDscpPcpRemarkFlowNodeUnpack(ofdbEgressDscpPcpRemarkFlowTable_node_t *node,
                                     ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK;

  memcpy(&flow->flowData.egressDscpPcpRemarkFlowEntry, &node->egressDscpPcpRemarkFlowEntry,
         sizeof(ofdpaEgressDscpPcpRemarkFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbEgressTpidFlowNodePack(ofdpaFlowEntry_t *flow,
                                ofdbEgressTpidFlowTable_node_t *node)
{
  memcpy(&node->egressTpidFlowEntry, &flow->flowData.egressTpidFlowEntry, sizeof(ofdpaEgressTpidFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbEgressTpidFlowNodeUnpack(ofdbEgressTpidFlowTable_node_t *node,
                                  ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_EGRESS_TPID;

  memcpy(&flow->flowData.egressTpidFlowEntry, &node->egressTpidFlowEntry, sizeof(ofdpaEgressTpidFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbDscpTrustFlowNodePack(ofdpaFlowEntry_t *flow,
                               ofdbDscpTrustFlowTable_node_t *node)
{
  memcpy(&node->dscpTrustFlowEntry, &flow->flowData.dscpTrustFlowEntry, sizeof(ofdpaDscpTrustFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}

void ofdbDscpTrustFlowCommonNodeUnpack(ofdbDscpTrustFlowTable_node_t *node,
                                 ofdpaFlowEntry_t *flow)
{
  memcpy(&flow->flowData.dscpTrustFlowEntry, &node->dscpTrustFlowEntry, sizeof(ofdpaDscpTrustFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbPortDscpTrustFlowNodeUnpack(ofdbDscpTrustFlowTable_node_t *node,
                                     ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST;

  ofdbDscpTrustFlowCommonNodeUnpack(node, flow);
}

void ofdbTunnelDscpTrustFlowNodeUnpack(ofdbDscpTrustFlowTable_node_t *node,
                                       ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST;

  ofdbDscpTrustFlowCommonNodeUnpack(node, flow);
}

void ofdbMplsDscpTrustFlowNodeUnpack(ofdbDscpTrustFlowTable_node_t *node,
                                     ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST;

  ofdbDscpTrustFlowCommonNodeUnpack(node, flow);
}

void ofdbPcpTrustFlowNodePack(ofdpaFlowEntry_t *flow,
                              ofdbPcpTrustFlowTable_node_t *node)
{
  memcpy(&node->pcpTrustFlowEntry, &flow->flowData.pcpTrustFlowEntry, sizeof(ofdpaPcpTrustFlowEntry_t));

  node->priority  = flow->priority;
  node->hard_time = flow->hard_time;
  node->idle_time = flow->idle_time;
  node->cookie    = flow->cookie;
}


void ofdbPcpTrustFlowCommonNodeUnpack(ofdbPcpTrustFlowTable_node_t *node,
                                      ofdpaFlowEntry_t *flow)
{
  memcpy(&flow->flowData.pcpTrustFlowEntry, &node->pcpTrustFlowEntry, sizeof(ofdpaPcpTrustFlowEntry_t));

  flow->priority  = node->priority;
  flow->hard_time = node->hard_time;
  flow->idle_time = node->idle_time;
  flow->cookie    = node->cookie;
}

void ofdbPortPcpTrustFlowNodeUnpack(ofdbPcpTrustFlowTable_node_t *node,
                                    ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST;

  ofdbPcpTrustFlowCommonNodeUnpack(node, flow);
}

void ofdbTunnelPcpTrustFlowNodeUnpack(ofdbPcpTrustFlowTable_node_t *node,
                                      ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST;

  ofdbPcpTrustFlowCommonNodeUnpack(node, flow);
}

void ofdbMplsPcpTrustFlowNodeUnpack(ofdbPcpTrustFlowTable_node_t *node,
                                    ofdpaFlowEntry_t *flow)
{
  flow->tableId = OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST;

  ofdbPcpTrustFlowCommonNodeUnpack(node, flow);
}

OFDPA_ERROR_t ofdbOamClientMPDBUpdate(uint32_t localMPId, OFDPA_BOOL isCreating)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  ofdpaOamMepConfig_t mepConfig;
  ofdpaOamMipConfig_t mipConfig;
  ofdbOamMegTable_node_t *megNode = NULL;
  OFDB_OAM_TYPE_t oamType = OFDB_OAM_TYPE_OAM_MEP;
  
  rc = ofdbOamTypeGet(localMPId, &oamType);
  if(rc != OFDPA_E_NONE)
  {
	return rc;
  }
  
  /* retrieve MEP table entry */
  rc = ofdbOamMepGet(localMPId, &mepConfig, NULL);
  if (rc == OFDPA_E_NONE)
  {
    /* retrieve MEG table entry */
    if ((megNode = avlSearch(&ofdbAVLTrees.ofdbOamMegTable_tree, &mepConfig.megIndex, AVL_EXACT)) != NULL)
    {
      if (megNode->config.megType == OFDPA_OAM_MEG_TYPE_G8113_1)
      {
        if (isCreating == OFDPA_TRUE)
        {
          rc = dot1agAddMepToClientMPDB(mepConfig.ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, mepConfig.mepId);
          return rc;
        }
        else
        {
          rc = dot1agDeleteMepFromClientMPDB(mepConfig.ifIndex, megNode->megInfo.dot1agMdIndex, megNode->megInfo.dot1agMaIndex, mepConfig.mepId);
          return rc;
        }
      }
      else
      {
        return rc;
      }
    }
  }

  /* retrieve MIP table entry */
  rc = ofdbOamMipGet(localMPId, &mipConfig, NULL);
  if (rc == OFDPA_E_NONE) 
  {
    /* retrieve MEG table entry */
    if ((megNode = avlSearch(&ofdbAVLTrees.ofdbOamMegTable_tree, &mipConfig.megIndex, AVL_EXACT)) != NULL)
    {
      if (megNode->config.megType == OFDPA_OAM_MEG_TYPE_G8113_1)
      {
        if (isCreating == OFDPA_TRUE)
        {
          rc = dot1agAddMipToClientMPDB(mipConfig.ifIndex, megNode->megInfo.primVid, megNode->config.level, mipConfig.direction);
          return rc;
        }
        else
        {
          rc = dot1agDeleteMipFromClientMPDB(mipConfig.ifIndex, megNode->megInfo.primVid, megNode->config.level, mipConfig.direction);
          return rc;
        }
      }
      else
      {
        return rc;
      }
    }
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Add an entry to the OpenFlow Flow Table.
*
* @param    flow @b{(input)} Info for the new flow.
*           flowId @b{(output)} unique flow identifier assigned by OFDB
*
* @returns  OFDPA_E_NONE - Entry added.
* @returns  OFDPA_E_EXISTS - Duplicate entry already in table.
* @returns  OFDPA_E_FULL - Table is full.
*
* @comments
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t ofdbFlowAdd(ofdpaFlowEntry_t *flow, uint64_t *flowId)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  void *dataPtr;
  ofdbFlowIndexTable_node_t flowIndexNode;
  ofdbFlowIndexTable_node_t *pFlowIndexNode;
  uint32_t currentUpTime;
  int flowInserted = 0;

  ofdbFlowTableStatus->last_flow_id++;
  currentUpTime = dpaUpTimeSeconds();

  switch (flow->tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      {
        ofdbIngressPortFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbIngressPortFlowUtilKeyCreate(&flow->flowData.ingressPortFlowEntry.match_criteria, &flow_node.key);

        ofdbIngressPortFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbIngressPortFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Ingress Port Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr for further processing below */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbIngressPortFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
      {
        ofdbDscpTrustFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbDscpTrustFlowUtilKeyCreate(&flow->flowData.dscpTrustFlowEntry.match_criteria, &flow_node.key);

        ofdbDscpTrustFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbPortDscpTrustFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Port DSCP Trust Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortDscpTrustFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
      {
        ofdbPcpTrustFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbPcpTrustFlowUtilKeyCreate(&flow->flowData.pcpTrustFlowEntry.match_criteria, &flow_node.key);

        ofdbPcpTrustFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbPortPcpTrustFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Port PCP Trust Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortPcpTrustFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
      {
        ofdbDscpTrustFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbDscpTrustFlowUtilKeyCreate(&flow->flowData.dscpTrustFlowEntry.match_criteria, &flow_node.key);

        ofdbDscpTrustFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbTunnelDscpTrustFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Tunnel DSCP Trust Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelDscpTrustFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
      {
        ofdbPcpTrustFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbPcpTrustFlowUtilKeyCreate(&flow->flowData.pcpTrustFlowEntry.match_criteria, &flow_node.key);

        ofdbPcpTrustFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbTunnelPcpTrustFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Tunnel PCP Trust Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr  */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelPcpTrustFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
      {
        ofdbInjectedOamFlowTable_node_t flow_node;
	      OFDB_OAM_TYPE_t oamtype;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbInjectedOamFlowUtilKeyCreate(&flow->flowData.injectedOamFlowEntry.match_criteria, &flow_node.key);

        ofdbInjectedOamFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbInjectedOamFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Injected OAM Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr for cookie table processing below */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbInjectedOamFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
		
			  rc = ofdbOamTypeGet(flow->flowData.injectedOamFlowEntry.match_criteria.lmepId, &oamtype);
			  if(rc != OFDPA_E_NONE)
			  {
			      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_BASIC, 
				  		"lmepId %d not found, rc = %d.\r\n", flow->flowData.injectedOamFlowEntry.match_criteria.lmepId, rc);
				  return rc;

			  }	  
			  if(oamtype == OFDB_OAM_TYPE_Y1564)
			  {
				  break;
			  }
			  else if(oamtype == OFDB_OAM_TYPE_Y1564_LB)
			  {
				  break;
			  }
	  
        rc = ofdbInjectedOamLmepIdMapEntryAdd(&flow->flowData.injectedOamFlowEntry);

        if (OFDPA_E_NONE != rc)
        {
					(void)avlDeleteEntry(&ofdbAVLTrees.ofdbInjectedOamFlowTable_tree, &flow_node.key);

					OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
					                   "Injected OAM Flow: could not add LMep map data. rv = %d\r\n", rc);
					return(rc);
        }

				if(oamtype == OFDB_OAM_TYPE_OAM_MEP || oamtype == OFDB_OAM_TYPE_OAM_MIP)
				{
					/* Trigger DOT1AG sub system's Client MP DB update */
					rc = ofdbOamClientMPDBUpdate(flow->flowData.injectedOamFlowEntry.match_criteria.lmepId, OFDPA_TRUE);
					if (OFDPA_E_NONE != rc)
					{
					  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
					                     "Injected OAM Flow: could not update OAM Client MP DB. rv = %d\r\n", rc);
					  return(rc);
					}
	    	}	
      }
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN:
      {
        ofdbVlanFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbVlanFlowUtilKeyCreate(&flow->flowData.vlanFlowEntry.match_criteria, &flow_node.key);

        ofdbVlanFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbVlanFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "VLAN Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {

					rc = vlanPipeFlowAdd(&flow_node);
					if(rc != OFDPA_E_NONE){
						dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbVlanFlowTable_tree, &flow_node.key);
						OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "add VLAN pipe entry failed.\r\n", 0);
						return rc;
					}
          /* successful insertion */
          flowInserted = 1;

					#if 0
          /* update counter entry reference count */
          if (flow->flowData.vlanFlowEntry.classBasedCountAction != 0)
          {
            ofdbClassBasedCounterReferenceUpdate(flow->flowData.vlanFlowEntry.classBasedCountId, 1);
          }
					#endif
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN_1:
      {
        ofdbVlan1FlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbVlan1FlowUtilKeyCreate(&flow->flowData.vlan1FlowEntry.match_criteria, &flow_node.key);

        ofdbVlan1FlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbVlan1FlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "VLAN_1 Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* update counter entry reference count */
          if (flow->flowData.vlan1FlowEntry.classBasedCountAction != 0)
          {
            ofdbClassBasedCounterReferenceUpdate(flow->flowData.vlan1FlowEntry.classBasedCountId, 1);
          }

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlan1FlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      {
        ofdbMpFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbMpFlowUtilKeyCreate(&flow->flowData.mpFlowEntry.match_criteria, &flow_node.key);

        ofdbMpFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMpFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Maintenance Point Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbMpFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      {
        ofdbMplsL2PortFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));

        ofdbMplsL2PortFlowUtilKeyCreate(&flow->flowData.mplsL2PortFlowEntry.match_criteria, &flow_node.key);

        ofdbMplsL2PortFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMplsL2PortFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "MPLS L2 Port Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
					rc = mplsL2PortPipeFlowAdd(&flow_node);
					if(rc != OFDPA_E_NONE){
						dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbVlanFlowTable_tree, &flow_node.key);
						OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "add mpls l2 port pipe entry failed.\r\n", 0);
						return rc;
					}
					
          /* successful insertion */
          flowInserted = 1;

          /* MPLS L2 port table may refer to a group */
          if (flow->flowData.mplsL2PortFlowEntry.groupId != 0)
          {
            ofdbGroupReferenceUpdate(flow->flowData.mplsL2PortFlowEntry.groupId, 1);
          }

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsL2PortFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
      {
        ofdbDscpTrustFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbDscpTrustFlowUtilKeyCreate(&flow->flowData.dscpTrustFlowEntry.match_criteria, &flow_node.key);

        ofdbDscpTrustFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMplsDscpTrustFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "MPLS DSCP Trust Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsDscpTrustFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
      {
        ofdbPcpTrustFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbPcpTrustFlowUtilKeyCreate(&flow->flowData.pcpTrustFlowEntry.match_criteria, &flow_node.key);

        ofdbPcpTrustFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMplsPcpTrustFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "MPLS PCP Trust Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsPcpTrustFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
      {
        ofdbMplsQosClassFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbMplsQosClassFlowUtilKeyCreate(&flow->flowData.mplsL2PortQosFlowEntry.match_criteria, &flow_node.key);

        ofdbMplsQosClassFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMplsQosClassFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "MPLS L2 Port QoS Class Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsQosClassFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
      {
        ofdbL2PolicerFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbL2PolicerFlowUtilKeyCreate(&flow->flowData.l2PolicerFlowEntry.match_criteria, &flow_node.key);

        ofdbL2PolicerFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbL2PolicerFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "L2 Policer Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbL2PolicerFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
      {
        ofdbL2PolicerActionsFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbL2PolicerActionsFlowUtilKeyCreate(&flow->flowData.l2PolicerActionsFlowEntry.match_criteria, &flow_node.key);

        ofdbL2PolicerActionsFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbL2PolicerActionsFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "L2 Policer Actions Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbL2PolicerActionsFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      {
        ofdbTerminationMacFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbTerminationMacFlowUtilKeyCreate(&flow->flowData.terminationMacFlowEntry.match_criteria, &flow_node.key);

        ofdbTerminationMacFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbTerminationMacFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Termination MAC Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* update counter entry reference count */
          if (flow->flowData.terminationMacFlowEntry.colorBasedCountAction != 
                                                      OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
          {
            ofdbColorBasedCounterReferenceUpdate(flow->flowData.terminationMacFlowEntry.colorBasedCountId, 1);
          }

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbTerminationMacFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_0:
    case OFDPA_FLOW_TABLE_ID_MPLS_1:
      {
        ofdbMplsFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbMplsFlowUtilKeyCreate(&flow->flowData.mplsFlowEntry.match_criteria, &flow_node.key);

        ofdbMplsFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0)
        {
          /*
           * MPLS 0 may not be allocated. Some platforms do not support this flow table.
           */
          if (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_MPLSTHREELABELS))
          {
            dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMpls0FlowTable_tree, &flow_node);
          }
          else
          {
            return(OFDPA_E_UNAVAIL);
          }
        }
        else
        {
          dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMpls1FlowTable_tree, &flow_node);
        }

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "MPLS Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* determine whether a BHH label mapping entry needs to be created */

          if (flow->flowData.mplsFlowEntry.gotoTableId == OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT)
          {
		  
	          OFDB_OAM_TYPE_t oamtype= OFDB_OAM_TYPE_OAM_MEP;
			  
	          rc = ofdbOamTypeGet(flow->flowData.mplsFlowEntry.lmepId, &oamtype); 
						if(oamtype== OFDB_OAM_TYPE_OAM_MEP || oamtype==OFDB_OAM_TYPE_OAM_MIP)
						{
							rc = dot1agBhhLabelMapEntryAdd(&flow->flowData.mplsFlowEntry);
						}	
		  
            if (rc != OFDPA_E_NONE)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Failed to add BHH label mapping entry. (rc = %d)\r\n",
                                 rc);
              if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0)
              {
                avlDeleteEntry(&ofdbAVLTrees.ofdbMpls0FlowTable_tree, &flow_node.key);
              }
              else
              {
                avlDeleteEntry(&ofdbAVLTrees.ofdbMpls1FlowTable_tree, &flow_node.key);
              }

              return rc;
            }
          }

          /* successful insertion */
          flowInserted = 1;

          /* update counter entry reference count */
          if (flow->flowData.mplsFlowEntry.colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
          {
            ofdbColorBasedCounterReferenceUpdate(flow->flowData.mplsFlowEntry.colorBasedCountId, 1);
          }

          /* retrieve dataPtr */
          if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0)
          {
            dataPtr = avlSearch(&ofdbAVLTrees.ofdbMpls0FlowTable_tree, &flow_node.key, AVL_EXACT);
          }
          else
          {
            dataPtr = avlSearch(&ofdbAVLTrees.ofdbMpls1FlowTable_tree, &flow_node.key, AVL_EXACT);
          }
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
      {
        ofdbMplsMpFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbMplsMpFlowUtilKeyCreate(&flow->flowData.mplsMpFlowEntry.match_criteria, &flow_node.key);

        ofdbMplsMpFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMplsMpFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "MPLS-TP Maintenance Point Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsMpFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
      {
        ofdbMplsLabelTrustFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbMplsLabelTrustFlowUtilKeyCreate(&flow->flowData.mplsLabelTrustFlowEntry.match_criteria, &flow_node.key);

        ofdbMplsLabelTrustFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMplsLabelTrustFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "MPLS Label Trust Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsLabelTrustFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      {
        ofdbUnicastRoutingFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbUnicastRoutingFlowUtilKeyCreate(&flow->flowData.unicastRoutingFlowEntry.match_criteria, &flow_node.key);

        ofdbUnicastRoutingFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbUnicastRoutingFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Unicast Routing Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* group ID only used if packets are not being dropped */
          if (flow->flowData.unicastRoutingFlowEntry.gotoTableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
          {
            if (ofdbGroupReferenceUpdate(flow->flowData.unicastRoutingFlowEntry.groupID, 1) != OFDPA_E_NONE)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Error from ofdbGroupReferenceUpdate. groupId = %d\r\n",
                                 flow->flowData.unicastRoutingFlowEntry.groupID);
            }
          }

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbUnicastRoutingFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      {
        ofdbMulticastRoutingFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbMulticastRoutingFlowUtilKeyCreate(&flow->flowData.multicastRoutingFlowEntry.match_criteria, &flow_node.key);

        ofdbMulticastRoutingFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbMulticastRoutingFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Multicast Routing Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* group ID only used if packets are not being dropped */
          if (flow->flowData.unicastRoutingFlowEntry.gotoTableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
          {
            if (ofdbGroupReferenceUpdate(flow->flowData.multicastRoutingFlowEntry.groupID, 1) != OFDPA_E_NONE)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                                 "Error from ofdbGroupReferenceUpdate. groupId = %d\r\n",
                                 flow->flowData.unicastRoutingFlowEntry.groupID);
            }
          }

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbMulticastRoutingFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      {
        ofdbBridgingFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbBridgingFlowUtilKeyCreate(&flow->flowData.bridgingFlowEntry.match_criteria, &flow_node.key);

        ofdbBridgingFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbBridgingFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Bridging Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* bridging rules refer to a group, tunnel rules refer to group or logical port, not both (checked in validation logic) */
          if (flow->flowData.bridgingFlowEntry.groupID != 0)
          {
            ofdbGroupReferenceUpdate(flow->flowData.bridgingFlowEntry.groupID, 1);
          }
          if (flow->flowData.bridgingFlowEntry.tunnelLogicalPort != 0)
          {
            ofdbTunnelPortReferenceUpdate(flow->flowData.bridgingFlowEntry.tunnelLogicalPort, 1);
          }

          /* tenant bridging flows have tunnelId match criteria not equal to 0 (checked in validation logic) */
          if (flow->flowData.bridgingFlowEntry.match_criteria.tunnelId != 0)
          {
            ofdbPortTenantReferenceUpdate(flow->flowData.bridgingFlowEntry.tunnelLogicalPort, flow->flowData.bridgingFlowEntry.match_criteria.tunnelId, 1);
          }

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbBridgingFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      {
        ofdbPolicyAclFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbPolicyAclFlowUtilKeyCreate(&flow->flowData.policyAclFlowEntry.match_criteria, &flow_node.key);

        ofdbPolicyAclFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbPolicyAclFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "ACL Policy Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* policy ACL rules may refer to logical port */
          ofdbTunnelPortReferenceUpdate(flow->flowData.policyAclFlowEntry.outputTunnelPort, 1);

          /* policy ACL rules may refer to a group */
          ofdbGroupReferenceUpdate(flow->flowData.policyAclFlowEntry.groupID, 1);

          /* policy ACL rules may have tunnelId match criteria not equal to 0 (checked in validation logic) */
          ofdbTenantReferenceUpdate(flow->flowData.policyAclFlowEntry.match_criteria.tunnelId, 1);

          /* policy ACL rules may refer to a meter */
          ofdbMeterReferenceUpdate(flow->flowData.policyAclFlowEntry.meterId, 1);

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbPolicyAclFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
      {
        ofdbColorActionsFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbColorActionsFlowUtilKeyCreate(&flow->flowData.colorActionsFlowEntry.match_criteria, &flow_node.key);

        ofdbColorActionsFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbColorActionsFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Color Actions Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbColorActionsFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;


    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
      {
        ofdbEgressVlanFlowTable_node_t flow_node;
        ofdpaEgressVlanFlowMatch_t *matchCriteria;
        uint32_t groupId;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbEgressVlanFlowUtilKeyCreate(&flow->flowData.egressVlanFlowEntry.match_criteria, &flow_node.key);

        ofdbEgressVlanFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbEgressVlanFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Egress VLAN Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* increment reference count to L2 Unfiltered Interface Group entry if required for this flow */
          matchCriteria = &flow->flowData.egressVlanFlowEntry.match_criteria;
          if (matchCriteria->allowVlanTranslation == 1)
          {
            groupId = 0;
            groupId = OFDB_GROUP_TYPE_SET(groupId, OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE);
            groupId = OFDB_GROUP_PORTID_SET(groupId, matchCriteria->outPort);

            ofdbGroupReferenceUpdate(groupId, 1);
          }

          /* update counter entry reference count */
          if (flow->flowData.egressVlanFlowEntry.classBasedCountAction != 0)
          {
            ofdbClassBasedCounterReferenceUpdate(flow->flowData.egressVlanFlowEntry.classBasedCountId, 1);
          }

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressVlanFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
      {
        ofdbEgressVlan1FlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbEgressVlan1FlowUtilKeyCreate(&flow->flowData.egressVlan1FlowEntry.match_criteria, &flow_node.key);

        ofdbEgressVlan1FlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbEgressVlan1FlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Egress VLAN 1 Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;

          /* update counter entry reference count */
          if (flow->flowData.egressVlan1FlowEntry.classBasedCountAction != 0)
          {
            ofdbClassBasedCounterReferenceUpdate(flow->flowData.egressVlan1FlowEntry.classBasedCountId, 1);
          }

          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressVlan1FlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
      {
        ofdbEgressMpFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbEgressMpFlowUtilKeyCreate(&flow->flowData.egressMpFlowEntry.match_criteria, &flow_node.key);

        ofdbEgressMpFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbEgressMpFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Egress Maintenance Point Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr for flow ID and cookie table processing below */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressMpFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
      {
        ofdbEgressDscpPcpRemarkFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbEgressDscpPcpRemarkFlowUtilKeyCreate(&flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria, &flow_node.key);

        ofdbEgressDscpPcpRemarkFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id          = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbEgressDscpPcpRemarkFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Egress DSCP PCP Remark Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr for flow ID and cookie table processing below */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressDscpPcpRemarkFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
      {
        ofdbEgressTpidFlowTable_node_t flow_node;

        memset(&flow_node, 0, sizeof(flow_node));
        ofdbEgressTpidFlowUtilKeyCreate(&flow->flowData.egressTpidFlowEntry.match_criteria, &flow_node.key);

        ofdbEgressTpidFlowNodePack(flow, &flow_node);

        flow_node.status.flow_id = ofdbFlowTableStatus->last_flow_id;
        flow_node.status.flow_add_up_time = currentUpTime;
        flow_node.status.flow_hit_up_time = currentUpTime;

        dataPtr = avlInsertEntry(&ofdbAVLTrees.ofdbEgressTpidFlowTable_tree, &flow_node);

        if (dataPtr == &flow_node)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Egress TPID Flow AVL tree full.\r\n", 0);
          return(OFDPA_E_FULL);
        }
        else if (dataPtr == 0)
        {
          /* successful insertion */
          flowInserted = 1;
          /* retrieve dataPtr */
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressTpidFlowTable_tree, &flow_node.key, AVL_EXACT);
        }
      }
      break;

    default:
      /* unknown table ID */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Unknown flow table ID. (%d)\r\n",
                         flow->tableId);
      return(OFDPA_E_FAIL);
      break;

  }

  if (flowInserted)
  {
    if (dataPtr != NULL)
    {
      memset(&flowIndexNode, 0, sizeof(flowIndexNode));
      flowIndexNode.index = ofdbFlowTableStatus->last_flow_id;
      flowIndexNode.tableId = flow->tableId;
      flowIndexNode.flowEntryNode = dataPtr;

      /* store entry in flowId cross reference table */
      pFlowIndexNode = avlInsertEntry(&ofdbAVLTrees.ofdbFlowFlowIdTable_tree, &flowIndexNode);

      if (pFlowIndexNode == &flowIndexNode)
      {
        /* Table is full */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Flow ID AVL tree full.\r\n", 0);
      }
      else if (pFlowIndexNode != 0)
      {
        /* Existing Entry found. */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Flow ID AVL tree entry present.\r\n", 0);
      }

      /* if cookie provided, insert record in cookie lookup table */
      if (flow->cookie)
      {
        flowIndexNode.index = flow->cookie;
        pFlowIndexNode = avlInsertEntry(&ofdbAVLTrees.ofdbFlowCookieTable_tree, &flowIndexNode);

        if (pFlowIndexNode == &flowIndexNode)
        {
          /* Table is full */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Flow cookie AVL tree full.\r\n", 0);
        }
        else if (pFlowIndexNode != 0)
        {
          /* Existing Entry found. */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Flow cookie AVL tree entry present.\r\n", 0);
        }
      }
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Newly inserted flow table node not retrieved. (%d)\r\n",
                         flow->tableId);
    }

    /* give caller flow identifier for added/modified entry */
    *flowId = ofdbFlowTableStatus->last_flow_id;
  }
  else
  {
    /* Existing Entry found. */
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Duplicate entry found.\r\n", 0);
    rc = OFDPA_E_EXISTS;
  }

  return rc;
}

OFDPA_ERROR_t ofdbFlowDelete(ofdpaFlowEntry_t *flow)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  void *dataPtr = 0;
  ofdbFlowIndexTable_node_t *dataPtrCookie;

  switch (flow->tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      {
        ofdbIngressPortFlowEntryKey_t key;

        ofdbIngressPortFlowUtilKeyCreate(&flow->flowData.ingressPortFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbIngressPortFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
      {
        ofdbDscpTrustFlowEntryKey_t key;

        ofdbDscpTrustFlowUtilKeyCreate(&flow->flowData.dscpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbPortDscpTrustFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
      {
        ofdbPcpTrustFlowEntryKey_t key;

        ofdbPcpTrustFlowUtilKeyCreate(&flow->flowData.pcpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbPortPcpTrustFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
      {
        ofdbDscpTrustFlowEntryKey_t key;

        ofdbDscpTrustFlowUtilKeyCreate(&flow->flowData.dscpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbTunnelDscpTrustFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
      {
        ofdbPcpTrustFlowEntryKey_t key;

        ofdbPcpTrustFlowUtilKeyCreate(&flow->flowData.pcpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbTunnelPcpTrustFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
      {
      ofdbInjectedOamFlowEntryKey_t key;
      OFDB_OAM_TYPE_t oamtype = OFDB_OAM_TYPE_OAM_MEP;
      /* Modified by JiaoYong, 2017/4/20 
      		如果是BFD类型的流表，就直接删除。解决netconf和openflow删除顺序的问题。
      		*/  
      if(OFDPA_MPLS_BFD_ACH_CHANNEL_TYPE == flow->flowData.injectedOamFlowEntry.mplsAchChannel)
      {
        oamtype = OFDB_OAM_TYPE_BFD;
      }
      else
      {
        rc = ofdbOamTypeGet(flow->flowData.injectedOamFlowEntry.match_criteria.lmepId, &oamtype); 
        if (OFDPA_E_NONE != rc)
        {
        	 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
        						 "Failed get lmepId %d type. (rc = %d)\r\n",
        						 flow->flowData.injectedOamFlowEntry.match_criteria.lmepId, rc);
        }	
      } 	  
	  
	  
      if(oamtype != OFDB_OAM_TYPE_Y1564 
	  	&& oamtype != OFDB_OAM_TYPE_Y1564_LB)
      {
        /* Modified by JiaoYong, 2017/4/20 BFD无需更新OAM相关数据*/        
        if(oamtype == OFDB_OAM_TYPE_OAM_MEP || oamtype == OFDB_OAM_TYPE_OAM_MIP)
        {

#if 1        
            /* Trigger DOT1AG sub system's Client MP DB update */
            rc = ofdbOamClientMPDBUpdate(flow->flowData.injectedOamFlowEntry.match_criteria.lmepId, OFDPA_FALSE);

			/*leishenghua modify 20170508, 参考 dot1agCfmClientMPDBEntryAdd,
			只有section oam时才会真正创建和删除，不处理*/
			if(OFDPA_E_NOT_FOUND == rc)
			{
				OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Injected OAM Flow: not found OAM Client MP DB. rv = %d\r\n", rc);
				rc = OFDPA_E_NONE;
			}
			
            if (OFDPA_E_NONE != rc)
            {
              OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                                 "Injected OAM Flow: could not update OAM Client MP DB. rv = %d\r\n", rc);
              return rc;
            }
#endif			
        }


        /* Delete Lmep map data */
        rc = ofdbInjectedOamLmepIdMapEntryDelete(flow->flowData.injectedOamFlowEntry.match_criteria.lmepId);
        if (OFDPA_E_NONE != rc)
        {
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                             "Could not delete Injected OAM LMepId Mapping entry. \r\n", 0);
          return rc;
        }
      }
        /* Delete entry from database */
        ofdbInjectedOamFlowUtilKeyCreate(&flow->flowData.injectedOamFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbInjectedOamFlowTable_tree, &key);

      }
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN:
      {
        ofdbVlanFlowEntryKey_t key;
				#if 0
        /* update counter entry reference count */
        if (flow->flowData.vlanFlowEntry.classBasedCountAction != 0)
        {
          ofdbClassBasedCounterReferenceUpdate(flow->flowData.vlanFlowEntry.classBasedCountId, 0);
        }
				#endif
        ofdbVlanFlowUtilKeyCreate(&flow->flowData.vlanFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbVlanFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN_1:
      {
        ofdbVlan1FlowEntryKey_t key;

        /* update counter entry reference count */
        if (flow->flowData.vlan1FlowEntry.classBasedCountAction != 0)
        {
          ofdbClassBasedCounterReferenceUpdate(flow->flowData.vlan1FlowEntry.classBasedCountId, 0);
        }

        ofdbVlan1FlowUtilKeyCreate(&flow->flowData.vlan1FlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbVlan1FlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      {
        ofdbMpFlowEntryKey_t key;

        ofdbMpFlowUtilKeyCreate(&flow->flowData.mpFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMpFlowTable_tree, &key);			
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      {
        ofdbMplsL2PortFlowEntryKey_t key;

        ofdbMplsL2PortFlowUtilKeyCreate(&flow->flowData.mplsL2PortFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMplsL2PortFlowTable_tree, &key);

        if (flow->flowData.mplsL2PortFlowEntry.groupId != 0)
        {
          ofdbGroupReferenceUpdate(flow->flowData.mplsL2PortFlowEntry.groupId, 0);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
      {
        ofdbDscpTrustFlowEntryKey_t key;

        ofdbDscpTrustFlowUtilKeyCreate(&flow->flowData.dscpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMplsDscpTrustFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
      {
        ofdbPcpTrustFlowEntryKey_t key;

        ofdbPcpTrustFlowUtilKeyCreate(&flow->flowData.pcpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMplsPcpTrustFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
      {
        ofdbMplsQosClassFlowEntryKey_t key;

        ofdbMplsQosClassFlowUtilKeyCreate(&flow->flowData.mplsL2PortQosFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMplsQosClassFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
      {
        ofdbL2PolicerFlowEntryKey_t key;

        ofdbL2PolicerFlowUtilKeyCreate(&flow->flowData.l2PolicerFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbL2PolicerFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
      {
        ofdbL2PolicerActionsFlowEntryKey_t key;

        ofdbL2PolicerActionsFlowUtilKeyCreate(&flow->flowData.l2PolicerActionsFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbL2PolicerActionsFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      {
        ofdbTerminationMacFlowEntryKey_t key;

        /* update counter entry reference count */
        if (flow->flowData.terminationMacFlowEntry.colorBasedCountAction != 
                                                          OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
        {
          ofdbColorBasedCounterReferenceUpdate(flow->flowData.terminationMacFlowEntry.colorBasedCountId, 0);
        }

        ofdbTerminationMacFlowUtilKeyCreate(&flow->flowData.terminationMacFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbTerminationMacFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_0:
    case OFDPA_FLOW_TABLE_ID_MPLS_1:
      {
        ofdbMplsFlowEntryKey_t key;

        /* determine whether a BHH label mapping entry needs to be deleted */

        if (flow->flowData.mplsFlowEntry.gotoTableId == OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT)
        {
		
        OFDB_OAM_TYPE_t oamtype= OFDB_OAM_TYPE_OAM_MEP;
        /* Modified by JiaoYong, 2017/4/20 
        	如果是BFD类型的流表，就直接删除。解决netconf和openflow删除顺序的问题。
        	*/  
        if(OFDPA_MPLS_BFD_ACH_CHANNEL_TYPE == flow->flowData.mplsFlowEntry.match_criteria.mplsAchChannel)
        {
          oamtype = OFDB_OAM_TYPE_BFD;
        }
        else
        {
          rc = ofdbOamTypeGet(flow->flowData.mplsFlowEntry.lmepId, &oamtype);  
          if (OFDPA_E_NONE != rc)
          {
          	 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
          						 "Failed get lmepId %d type. (rc = %d)\r\n",
          						 flow->flowData.mplsFlowEntry.lmepId, rc);
          }	
        }   
        
		if(oamtype == OFDB_OAM_TYPE_OAM_MEP || oamtype == OFDB_OAM_TYPE_OAM_MIP)
		{
        	rc = dot1agBhhLabelMapEntryDelete(&flow->flowData.mplsFlowEntry);
		}
		
          if (OFDPA_E_NONE != rc)
          {
            OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                               "Failed deleting BHH tunnel mapping entry. (rc = %d)\r\n",
                               rc);
            /* Fall through and delete from OFDB anyway */
          }

        }

        /* update counter entry reference count */
        if (flow->flowData.mplsFlowEntry.colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
        {
          ofdbColorBasedCounterReferenceUpdate(flow->flowData.mplsFlowEntry.colorBasedCountId, 0);
        }

        ofdbMplsFlowUtilKeyCreate(&flow->flowData.mplsFlowEntry.match_criteria, &key);

        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0)
        {
          dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMpls0FlowTable_tree, &key);
        }
        else
        {
          dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMpls1FlowTable_tree, &key);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
      {
        ofdbMplsMpFlowEntryKey_t key;

        ofdbMplsMpFlowUtilKeyCreate(&flow->flowData.mplsMpFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMplsMpFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
      {
        ofdbMplsLabelTrustFlowEntryKey_t key;

        ofdbMplsLabelTrustFlowUtilKeyCreate(&flow->flowData.mplsLabelTrustFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMplsLabelTrustFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      {
        ofdbUnicastRoutingFlowEntryKey_t key;

        ofdbUnicastRoutingFlowUtilKeyCreate(&flow->flowData.unicastRoutingFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbUnicastRoutingFlowTable_tree, &key);

        /* group ID only used if packets are not being dropped */
        if (flow->flowData.unicastRoutingFlowEntry.gotoTableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
        {
          ofdbGroupReferenceUpdate(flow->flowData.unicastRoutingFlowEntry.groupID, 0);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      {
        ofdbMulticastRoutingFlowEntryKey_t key;

        ofdbMulticastRoutingFlowUtilKeyCreate(&flow->flowData.multicastRoutingFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbMulticastRoutingFlowTable_tree, &key);

        /* group ID only used if packets are not being dropped */
        if (flow->flowData.multicastRoutingFlowEntry.gotoTableId == OFDPA_FLOW_TABLE_ID_ACL_POLICY)
        {
          ofdbGroupReferenceUpdate(flow->flowData.multicastRoutingFlowEntry.groupID, 0);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      {
        ofdbBridgingFlowEntryKey_t key;

        ofdbBridgingFlowUtilKeyCreate(&flow->flowData.bridgingFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbBridgingFlowTable_tree, &key);

        /* bridging rules refer to a group, tunnel rules refer to group or logical port, not both (checked in validation logic) */
        if (flow->flowData.bridgingFlowEntry.groupID != 0)
        {
          ofdbGroupReferenceUpdate(flow->flowData.bridgingFlowEntry.groupID, 0);
        }
        if (flow->flowData.bridgingFlowEntry.tunnelLogicalPort != 0)
        {
          ofdbTunnelPortReferenceUpdate(flow->flowData.bridgingFlowEntry.tunnelLogicalPort, 0);
        }

        /* tenant bridging flows have tunnelId match criteria not equal to 0 (checked in validation logic) */
        if (flow->flowData.bridgingFlowEntry.match_criteria.tunnelId != 0)
        {
          ofdbPortTenantReferenceUpdate(flow->flowData.bridgingFlowEntry.tunnelLogicalPort, flow->flowData.bridgingFlowEntry.match_criteria.tunnelId, 0);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      {
        ofdbPolicyAclFlowEntryKey_t key;

        ofdbPolicyAclFlowUtilKeyCreate(&flow->flowData.policyAclFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbPolicyAclFlowTable_tree, &key);

        ofdbGroupReferenceUpdate(flow->flowData.policyAclFlowEntry.groupID, 0);
        ofdbTunnelPortReferenceUpdate(flow->flowData.policyAclFlowEntry.outputTunnelPort, 0);
        ofdbTenantReferenceUpdate(flow->flowData.policyAclFlowEntry.match_criteria.tunnelId, 0);
        ofdbMeterReferenceUpdate(flow->flowData.policyAclFlowEntry.meterId, 0);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
      {
        ofdbColorActionsFlowEntryKey_t key;

        ofdbColorActionsFlowUtilKeyCreate(&flow->flowData.colorActionsFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbColorActionsFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
      {
        ofdbEgressVlanFlowEntryKey_t key;
        ofdpaEgressVlanFlowMatch_t *matchCriteria;
        uint32_t groupId;

        /* decrement reference count to L2 Unfiltered Interface Group entry if required for this flow */
        matchCriteria = &flow->flowData.egressVlanFlowEntry.match_criteria;
        if (matchCriteria->allowVlanTranslation == 1)
        {
          groupId = 0;
          groupId = OFDB_GROUP_TYPE_SET(groupId, OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE);
          groupId = OFDB_GROUP_PORTID_SET(groupId, matchCriteria->outPort);

          ofdbGroupReferenceUpdate(groupId, 0);
        }

        /* update counter entry reference count */
        if (flow->flowData.egressVlanFlowEntry.classBasedCountAction != 0)
        {
          ofdbClassBasedCounterReferenceUpdate(flow->flowData.egressVlanFlowEntry.classBasedCountId, 0);
        }

        ofdbEgressVlanFlowUtilKeyCreate(&flow->flowData.egressVlanFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbEgressVlanFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
      {
        ofdbEgressVlan1FlowEntryKey_t key;

        /* update counter entry reference count */
        if (flow->flowData.egressVlan1FlowEntry.classBasedCountAction != 0)
        {
          ofdbClassBasedCounterReferenceUpdate(flow->flowData.egressVlan1FlowEntry.classBasedCountId, 0);
        }

        ofdbEgressVlan1FlowUtilKeyCreate(&flow->flowData.egressVlan1FlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbEgressVlan1FlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
      {
        ofdbEgressMpFlowEntryKey_t key;

        ofdbEgressMpFlowUtilKeyCreate(&flow->flowData.egressMpFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbEgressMpFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
      {
        ofdbEgressDscpPcpRemarkFlowEntryKey_t key;

        ofdbEgressDscpPcpRemarkFlowUtilKeyCreate(&flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbEgressDscpPcpRemarkFlowTable_tree, &key);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
      {
        ofdbEgressTpidFlowEntryKey_t key;

        ofdbEgressTpidFlowUtilKeyCreate(&flow->flowData.egressTpidFlowEntry.match_criteria, &key);
        dataPtr = avlDeleteEntry(&ofdbAVLTrees.ofdbEgressTpidFlowTable_tree, &key);
      }
      break;

    default:
      /* unknown table ID */
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_BASIC,
                         "Unknown flow table ID. (%d)\r\n",
                         flow->tableId);
      return(OFDPA_E_FAIL);
      break;

  }

  if (dataPtr == 0)
  {
    /* Entry was not found. */
    rc = OFDPA_E_NOT_FOUND;
  }
  else
  {
    /* delete associated entry in flow cookie table */
    if (flow->cookie)
    {
      dataPtrCookie = avlDeleteEntry(&ofdbAVLTrees.ofdbFlowCookieTable_tree, &flow->cookie);
      if (dataPtrCookie == 0)
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Flow cookie AVL tree node not found for delete.\r\n", 0);
      }
    }
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Get pointers to content of the specified flow.
*
* @param    flow @b{(input)} Flow search information.
* @param    searchType @b{(input)} specifies exact or next flow search
* @param    flowConfig @b{(output)} matching entry's configuration.
* @param    flags @b{(output)} pointer to matching entry's flags.
* @param    flow_status @b{(output)} pointer to matching entry's status information.
*
*
* @returns  OFDPA_E_NONE - Entry found.
* @returns  OFDPA_E_NOT_FOUND - Entry is not found.
*
* @note This utility function shall only be called from within the OFDB implementation.
*       Caller must be holding the OFDB lock. Dereferencing any pointers returned
*       must occur before releasing the OFDB lock.
*
* @end
*
*********************************************************************/
static OFDPA_ERROR_t ofdbFlowSearch(ofdpaFlowEntry_t *flow,
                                    unsigned int searchType,
                                    ofdpaFlowEntry_t *flowConfig,
                                    OFDB_ENTRY_FLAG_t **flags,
                                    ofdbFlowStatus_t **flowStatus)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;

  switch (flow->tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      {
        ofdbIngressPortFlowEntryKey_t key;
        ofdbIngressPortFlowTable_node_t *dataPtr;

        ofdbIngressPortFlowUtilKeyCreate(&flow->flowData.ingressPortFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbIngressPortFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbIngressPortFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
      {
        ofdbDscpTrustFlowEntryKey_t key;
        ofdbDscpTrustFlowTable_node_t *dataPtr;

        ofdbDscpTrustFlowUtilKeyCreate(&flow->flowData.dscpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortDscpTrustFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbPortDscpTrustFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
      {
        ofdbPcpTrustFlowEntryKey_t key;
        ofdbPcpTrustFlowTable_node_t *dataPtr;

        ofdbPcpTrustFlowUtilKeyCreate(&flow->flowData.pcpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbPortPcpTrustFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbPortPcpTrustFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
      {
        ofdbDscpTrustFlowEntryKey_t key;
        ofdbDscpTrustFlowTable_node_t *dataPtr;

        ofdbDscpTrustFlowUtilKeyCreate(&flow->flowData.dscpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelDscpTrustFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbTunnelDscpTrustFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
      {
        ofdbPcpTrustFlowEntryKey_t key;
        ofdbPcpTrustFlowTable_node_t *dataPtr;

        ofdbPcpTrustFlowUtilKeyCreate(&flow->flowData.pcpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbTunnelPcpTrustFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbTunnelPcpTrustFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
      {
        ofdbInjectedOamFlowEntryKey_t key;
        ofdbInjectedOamFlowTable_node_t *dataPtr;

        ofdbInjectedOamFlowUtilKeyCreate(&flow->flowData.injectedOamFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbInjectedOamFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbInjectedOamFlowNodeUnPack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN:
      {
        ofdbVlanFlowEntryKey_t key;
        ofdbVlanFlowTable_node_t *dataPtr;

        ofdbVlanFlowUtilKeyCreate(&flow->flowData.vlanFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlanFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbVlanFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN_1:
      {
        ofdbVlan1FlowEntryKey_t key;
        ofdbVlan1FlowTable_node_t *dataPtr;

        ofdbVlan1FlowUtilKeyCreate(&flow->flowData.vlan1FlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbVlan1FlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbVlan1FlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      {
        ofdbMpFlowEntryKey_t key;
        ofdbMpFlowTable_node_t *dataPtr;

        ofdbMpFlowUtilKeyCreate(&flow->flowData.mpFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbMpFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbMpFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      {
        ofdbMplsL2PortFlowEntryKey_t key;
        ofdbMplsL2PortFlowTable_node_t *dataPtr;

        ofdbMplsL2PortFlowUtilKeyCreate(&flow->flowData.mplsL2PortFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsL2PortFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbMplsL2PortFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
      {
        ofdbDscpTrustFlowEntryKey_t key;
        ofdbDscpTrustFlowTable_node_t *dataPtr;

        ofdbDscpTrustFlowUtilKeyCreate(&flow->flowData.dscpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsDscpTrustFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbMplsDscpTrustFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
      {
        ofdbPcpTrustFlowEntryKey_t key;
        ofdbPcpTrustFlowTable_node_t *dataPtr;

        ofdbPcpTrustFlowUtilKeyCreate(&flow->flowData.pcpTrustFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsPcpTrustFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbMplsPcpTrustFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
      {
        ofdbMplsQosClassFlowEntryKey_t key;
        ofdbMplsQosClassFlowTable_node_t *dataPtr;

        ofdbMplsQosClassFlowUtilKeyCreate(&flow->flowData.mplsL2PortQosFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsQosClassFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbMplsQosClassFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
      {
        ofdbL2PolicerFlowEntryKey_t key;
        ofdbL2PolicerFlowTable_node_t *dataPtr;

        ofdbL2PolicerFlowUtilKeyCreate(&flow->flowData.l2PolicerFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbL2PolicerFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbL2PolicerFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
      {
        ofdbL2PolicerActionsFlowEntryKey_t key;
        ofdbL2PolicerActionsFlowTable_node_t *dataPtr;

        ofdbL2PolicerActionsFlowUtilKeyCreate(&flow->flowData.l2PolicerActionsFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbL2PolicerActionsFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbL2PolicerActionsFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      {
        ofdbTerminationMacFlowEntryKey_t key;
        ofdbTerminationMacFlowTable_node_t *dataPtr;

        ofdbTerminationMacFlowUtilKeyCreate(&flow->flowData.terminationMacFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbTerminationMacFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbTerminationMacFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_0:
    case OFDPA_FLOW_TABLE_ID_MPLS_1:
      {
        ofdbMplsFlowEntryKey_t key;
        ofdbMplsFlowTable_node_t *dataPtr = 0;

        ofdbMplsFlowUtilKeyCreate(&flow->flowData.mplsFlowEntry.match_criteria, &key);
        if (flow->tableId == OFDPA_FLOW_TABLE_ID_MPLS_0)
        {
          /*
           * MPLS 0 may not be allocated. Some platforms do not support this flow table.
           */
          if (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_MPLSTHREELABELS))
          {
            dataPtr = avlSearch(&ofdbAVLTrees.ofdbMpls0FlowTable_tree, &key, searchType);
          }
        }
        else
        {
          dataPtr = avlSearch(&ofdbAVLTrees.ofdbMpls1FlowTable_tree, &key, searchType);
        }

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbMplsFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_2:
      /* Table can not have any entries. */
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
      {
        ofdbMplsMpFlowEntryKey_t key;
        ofdbMplsMpFlowTable_node_t *dataPtr;

        ofdbMplsMpFlowUtilKeyCreate(&flow->flowData.mplsMpFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsMpFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbMplsMpFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_L3_TYPE:
    case OFDPA_FLOW_TABLE_ID_MPLS_TYPE:
      /* Table can not have any entries. */
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
      {
        ofdbMplsLabelTrustFlowEntryKey_t key;
        ofdbMplsLabelTrustFlowTable_node_t *dataPtr;

        ofdbMplsLabelTrustFlowUtilKeyCreate(&flow->flowData.mplsLabelTrustFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbMplsLabelTrustFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbMplsLabelTrustFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      {
        ofdbUnicastRoutingFlowEntryKey_t key;
        ofdbUnicastRoutingFlowTable_node_t *dataPtr;

        ofdbUnicastRoutingFlowUtilKeyCreate(&flow->flowData.unicastRoutingFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbUnicastRoutingFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbUnicastRoutingFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      {
        ofdbMulticastRoutingFlowEntryKey_t key;
        ofdbMulticastRoutingFlowTable_node_t *dataPtr;

        ofdbMulticastRoutingFlowUtilKeyCreate(&flow->flowData.multicastRoutingFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbMulticastRoutingFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbMulticastRoutingFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      {
        ofdbBridgingFlowEntryKey_t key;
        ofdbBridgingFlowTable_node_t *dataPtr;

        ofdbBridgingFlowUtilKeyCreate(&flow->flowData.bridgingFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbBridgingFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbBridgingFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      {
        ofdbPolicyAclFlowEntryKey_t key;
        ofdbPolicyAclFlowTable_node_t *dataPtr;

        ofdbPolicyAclFlowUtilKeyCreate(&flow->flowData.policyAclFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbPolicyAclFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbPolicyAclFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
      {
        ofdbColorActionsFlowEntryKey_t key;
        ofdbColorActionsFlowTable_node_t *dataPtr;

        ofdbColorActionsFlowUtilKeyCreate(&flow->flowData.colorActionsFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbColorActionsFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbColorActionsFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
      {
        ofdbEgressVlanFlowEntryKey_t key;
        ofdbEgressVlanFlowTable_node_t *dataPtr;

        ofdbEgressVlanFlowUtilKeyCreate(&flow->flowData.egressVlanFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressVlanFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbEgressVlanFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
      {
        ofdbEgressVlan1FlowEntryKey_t key;
        ofdbEgressVlan1FlowTable_node_t *dataPtr;

        ofdbEgressVlan1FlowUtilKeyCreate(&flow->flowData.egressVlan1FlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressVlan1FlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbEgressVlan1FlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
      {
        ofdbEgressMpFlowEntryKey_t key;
        ofdbEgressMpFlowTable_node_t *dataPtr;

        ofdbEgressMpFlowUtilKeyCreate(&flow->flowData.egressMpFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressMpFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbEgressMpFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
      {
        ofdbEgressDscpPcpRemarkFlowEntryKey_t key;
        ofdbEgressDscpPcpRemarkFlowTable_node_t *dataPtr;

        ofdbEgressDscpPcpRemarkFlowUtilKeyCreate(&flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressDscpPcpRemarkFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbEgressDscpPcpRemarkFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
      {
        ofdbEgressTpidFlowEntryKey_t key;
        ofdbEgressTpidFlowTable_node_t *dataPtr;

        ofdbEgressTpidFlowUtilKeyCreate(&flow->flowData.egressTpidFlowEntry.match_criteria, &key);
        dataPtr = avlSearch(&ofdbAVLTrees.ofdbEgressTpidFlowTable_tree, &key, searchType);

        if (dataPtr != 0)
        {
          rc = OFDPA_E_NONE;
          if (flowConfig)
          {
            ofdbEgressTpidFlowNodeUnpack(dataPtr, flowConfig);
          }

          if (flags)
            *flags = &dataPtr->flags;

          if (flowStatus)
            *flowStatus = &dataPtr->status;
        }
      }
      break;

    default:
      {
        /* unknown table ID */
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERY_VERBOSE,
                           "Unknown flow table ID. (%d)\r\n",
                           flow->tableId);
      }
      rc = OFDPA_E_PARAM;
      break;


  }

  return rc;
}

OFDPA_ERROR_t ofdbFlowGet(ofdpaFlowEntry_t *flow,
                          OFDB_ENTRY_FLAG_t *flags,
                          ofdbFlowStatus_t *flowStatus)
{
  OFDPA_ERROR_t rc;
  OFDB_ENTRY_FLAG_t *flowEntryFlags = NULL;
  ofdbFlowStatus_t *flowEntryStatus = NULL;

  rc = ofdbFlowSearch(flow, AVL_EXACT, flow, &flowEntryFlags, &flowEntryStatus);
  if (rc == OFDPA_E_NONE)
  {
    if (flags)
      *flags = *flowEntryFlags;

    if (flowStatus)
      memcpy(flowStatus, flowEntryStatus, sizeof(*flowStatus));
  }

  return(rc);
}

/*********************************************************************
*
* @purpose  Get information for the next flow after the specified flow.
*
* @param    flow @b{(input)} Flow information.
* @param    nextFlow @b{(output)} Configured information for next flow.
* @param    nextFlowFlags @b{(output)}  Entry flags for next flow.
* @param    nextFlowStatus @b{(output)} Status information for next flow.
*
* @returns  OFDPA_E_NONE - Entry found.
* @returns  OFDPA_E_FAIL - Entry is not found.
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t ofdbFlowNextGet(ofdpaFlowEntry_t *flow,
                              ofdpaFlowEntry_t *nextFlow,
                              OFDB_ENTRY_FLAG_t *nextFlowFlags,
                              ofdbFlowStatus_t *nextFlowStatus)
{
  OFDPA_ERROR_t rc;
  OFDB_ENTRY_FLAG_t *flowEntryFlags = NULL;
  ofdbFlowStatus_t *flowEntryStatus = NULL;

  rc = ofdbFlowSearch(flow, AVL_NEXT, nextFlow, &flowEntryFlags, &flowEntryStatus);
  if (OFDPA_E_NONE == rc)
  {
    if (nextFlowFlags)
    {
      *nextFlowFlags = *flowEntryFlags;
    }

    if (nextFlowStatus)
    {
      memcpy(nextFlowStatus, flowEntryStatus, sizeof(*nextFlowStatus));
    }
  }

  return(rc);
}

OFDPA_ERROR_t ofdbFlowStatusSet(ofdpaFlowEntry_t *flow,
                                OFDB_ENTRY_FLAG_t *flags,
                                ofdbFlowStatus_t *flowStatus)
{
  OFDPA_ERROR_t rc;
  OFDB_ENTRY_FLAG_t *flowEntryFlags = NULL;
  ofdbFlowStatus_t *flowEntryStatus = NULL;

  if ((rc = ofdbFlowSearch(flow, AVL_EXACT, NULL, &flowEntryFlags, &flowEntryStatus)) == OFDPA_E_NONE)
  {
    if (flags)
      *flowEntryFlags = *flags;

    if (flowStatus)
      memcpy(flowEntryStatus, flowStatus, sizeof(*flowEntryStatus));
  }

  return(rc);
}

OFDPA_ERROR_t ofdbFlowEventNextGet(ofdpaFlowEvent_t *eventData)
{
  OFDPA_ERROR_t rc;
  OFDB_ENTRY_FLAG_t *flowEntryFlags = NULL;
  ofdbFlowStatus_t *flowEntryStatus = NULL;
  int flowFound;

  flowFound = 0;
  rc = OFDPA_E_NOT_FOUND;

  /* find flow table entry that has been marked for an event */
  while ((!flowFound) &&
         (ofdbFlowSearch(&eventData->flowMatch, AVL_NEXT, &eventData->flowMatch, &flowEntryFlags, &flowEntryStatus) == OFDPA_E_NONE))
  {
    if (*flowEntryFlags & (OFDB_ENTRY_HARD_TIMEOUT | OFDB_ENTRY_IDLE_TIMEOUT))
    {
      eventData->eventMask = 0;
      if (*flowEntryFlags & OFDB_ENTRY_HARD_TIMEOUT)
      {
        eventData->eventMask |= OFDPA_FLOW_EVENT_HARD_TIMEOUT;
      }
      if (*flowEntryFlags & OFDB_ENTRY_IDLE_TIMEOUT)
      {
        eventData->eventMask |= OFDPA_FLOW_EVENT_IDLE_TIMEOUT;
      }

      /* clear the event flags now that they have been reported */
      *flowEntryFlags &= ~(OFDB_ENTRY_HARD_TIMEOUT | OFDB_ENTRY_IDLE_TIMEOUT);

      flowFound = 1;
      rc = OFDPA_E_NONE;
    }
  }

  return(rc);
}


OFDPA_ERROR_t ofdbFlowIndexTableNodeUnpack(ofdbFlowIndexTable_node_t *flowIndexTableNode,
                                           ofdpaFlowEntry_t *flowConfig,
                                           ofdbFlowStatus_t *flowStatus)
{
  OFDPA_ERROR_t rc = OFDPA_E_PARAM;

  if (flowIndexTableNode != NULL)
  {
    if (flowIndexTableNode->flowEntryNode != NULL)
    {
      /* accessing data depends on flow table ID */
      switch (flowIndexTableNode->tableId)
      {
        case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
          {
            ofdbIngressPortFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbIngressPortFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
          {
            ofdbDscpTrustFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbPortDscpTrustFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
          {
            ofdbPcpTrustFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbPortPcpTrustFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
          {
            ofdbDscpTrustFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbTunnelDscpTrustFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
          {
            ofdbPcpTrustFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbTunnelPcpTrustFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
          {
            ofdbInjectedOamFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbInjectedOamFlowNodeUnPack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_VLAN:
          {
            ofdbVlanFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbVlanFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_VLAN_1:
          {
            ofdbVlan1FlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbVlan1FlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
          {
            ofdbMpFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbMpFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
          {
            ofdbMplsL2PortFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbMplsL2PortFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
          {
            ofdbDscpTrustFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbMplsDscpTrustFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
          {
            ofdbPcpTrustFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbMplsPcpTrustFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
          {
            ofdbMplsQosClassFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbMplsQosClassFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_L2_POLICER:
          {
            ofdbL2PolicerFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbL2PolicerFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
          {
            ofdbL2PolicerActionsFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbL2PolicerActionsFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
          {
            ofdbTerminationMacFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbTerminationMacFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_MPLS_0:
        case OFDPA_FLOW_TABLE_ID_MPLS_1:
          {
            ofdbMplsFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbMplsFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
          {
            ofdbMplsMpFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbMplsMpFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
          {
            ofdbMplsLabelTrustFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbMplsLabelTrustFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }

          }
          break;

        case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
          {
            ofdbUnicastRoutingFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbUnicastRoutingFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }

          }
          break;

        case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
          {
            ofdbMulticastRoutingFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbMulticastRoutingFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_BRIDGING:
          {
            ofdbBridgingFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbBridgingFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
          {
            ofdbPolicyAclFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbPolicyAclFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
          {
            ofdbColorActionsFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbColorActionsFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
          {
            ofdbEgressVlanFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbEgressVlanFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
          {
            ofdbEgressVlan1FlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbEgressVlan1FlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
          {
            ofdbEgressMpFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbEgressMpFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
          {
            ofdbEgressDscpPcpRemarkFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbEgressDscpPcpRemarkFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
          {
            ofdbEgressTpidFlowTable_node_t *ptr;

            ptr = flowIndexTableNode->flowEntryNode;

            if (flowConfig)
            {
              ofdbEgressTpidFlowNodeUnpack(ptr, flowConfig);
            }

            if (flowStatus)
            {
              memcpy(flowStatus, &ptr->status, sizeof(ofdbFlowStatus_t));
            }
          }
          break;

        default:
          /* unknown table ID */
          OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                             "Unknown flow table ID. (%d)\r\n",
                             flowIndexTableNode->tableId);
          return(OFDPA_E_FAIL);
          break;

      }

      rc = OFDPA_E_NONE;
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Flow index table entry with NULL flow entry pointer found.\r\n", 0);
      rc = OFDPA_E_ERROR;
    }
  }

  return(rc);
}

/*********************************************************************
*
* @purpose  Get information for the specified flow.
*
* @param    cookie @b{(input)} The cookie identifying the flow.
* @param    flowConfig @b{(output)} Configured flow information.
* @param    flowStatus @b{(output)} Flow Status information.
*
*
* @returns  OFDPA_E_NONE - Entry found.
* @returns  OFDPA_E_NOT_FOUND - Entry is not found.
*
* @comments The cookie value must not be 0.
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t ofdbFlowByCookieGet(uint64_t cookie,
                                  ofdpaFlowEntry_t *flowConfig,
                                  ofdbFlowStatus_t *flowStatus)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbFlowIndexTable_node_t *dataPtrCookie;

  if (cookie != 0)
  {
    dataPtrCookie = avlSearch(&ofdbAVLTrees.ofdbFlowCookieTable_tree, &cookie, AVL_EXACT);

    if (dataPtrCookie != NULL)
    {
      if (dataPtrCookie->flowEntryNode != NULL)
      {
        rc = ofdbFlowIndexTableNodeUnpack(dataPtrCookie, flowConfig, flowStatus);
      }
      else
      {
        OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                           "Flow cookie table entry with NULL flow entry pointer found.\r\n", 0);
        /* clean up errant entry */
        avlDeleteEntry(&ofdbAVLTrees.ofdbFlowCookieTable_tree, &cookie);

        rc = OFDPA_E_INTERNAL;
      }
    }
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Delete the specified flow.
*
* @param    cookie @b{(input)} The cookie identifying the flow.
*
*
* @returns  OFDPA_E_NONE - Entry found.
* @returns  OFDPA_E_NOT_FOUND - Entry is not found.
*
* @comments The cookie value must not be 0.
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t ofdbFlowByCookieDelete(uint64_t cookie)
{
  OFDPA_ERROR_t rc;
  ofdpaFlowEntry_t flow;

  if ((rc = ofdbFlowByCookieGet(cookie, &flow, NULL)) == OFDPA_E_NONE)
  {
    if ((rc = ofdbFlowDelete(&flow)) != OFDPA_E_NONE)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Error from ofdbFlowDelete(). (%d)\r\n", rc);
    }
  }

  return rc;
}

/*********************************************************************
*
* @purpose  Get information for the specified flow.
*
* @param    flowId @b{(input)} The flowId identifying the flow.
* @param    flowConfig @b{(output)} Configured flow information.
* @param    flowStatus @b{(output)} Flow Status information.
*
*
* @returns  OFDPA_E_NONE - Entry found.
* @returns  OFDPA_E_NOT_FOUND - Entry is not found.
*
* @end
*
*********************************************************************/
OFDPA_ERROR_t ofdbFlowByFlowIdGet(uint64_t flowId,
                                  ofdpaFlowEntry_t *flowConfig,
                                  ofdbFlowStatus_t *flowStatus)
{
  OFDPA_ERROR_t rc = OFDPA_E_NOT_FOUND;
  ofdbFlowIndexTable_node_t *dataPtr;

  dataPtr = avlSearch(&ofdbAVLTrees.ofdbFlowFlowIdTable_tree, &flowId, AVL_EXACT);

  if (dataPtr != NULL)
  {
    if (dataPtr->flowEntryNode != NULL)
    {
      rc = ofdbFlowIndexTableNodeUnpack(dataPtr, flowConfig, flowStatus);
    }
    else
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS,
                         "Flow ID table entry with NULL flow entry pointer found.\r\n", 0);
      /* clean up errant entry */
      avlDeleteEntry(&ofdbAVLTrees.ofdbFlowFlowIdTable_tree, &flowId);

      rc = OFDPA_E_INTERNAL;
    }
  }

  return rc;
}
