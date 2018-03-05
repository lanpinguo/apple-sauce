/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2016
*
**********************************************************************
*
* @filename     ofdb.c
*
* @purpose      Openflow Database Manager (ofdb) control functions
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
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "ofdb.h"
#include "ofdpa_util.h"
#include "ofdpa_porting.h"
//#include "driver_util.h"

ofdpaRWlock_t ofdbLock;

ofdbGlobalData_t               *ofdbGlobalData;
ofdbFlowTableStatus_t          *ofdbFlowTableStatus;
ofdbGroupTableStatus_t         *ofdbGroupTableStatus;
ofdbGroupBucketTableStatus_t   *ofdbGroupBucketTableStatus;
ofdbMeterTableStatus_t         *ofdbMeterTableStatus;
ofdbClassBasedCounterTableStatus_t *ofdbClassBasedCounterTableStatus;
ofdbColorBasedCounterTableStatus_t *ofdbColorBasedCounterTableStatus;
ofdbAVLTrees_t                  ofdbAVLTrees;

int ofdbIp6AddrCmp(struct in6_addr *pA, struct in6_addr *pB)
{
  uint32_t i;

  for (i = 0; i < 4; i++)
  {
    if (pA->s6_addr32[i] > pB->s6_addr32[i])
    {
      return 1;
    }
    if (pA->s6_addr32[i] < pB->s6_addr32[i])
    {
      return -1;
    }
  }
  return 0;
}

int ofdbIngressPortFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbIngressPortFlowEntryKey_t *pA = a;
  ofdbIngressPortFlowEntryKey_t *pB = b;

  if (pA->inPort > pB->inPort)
  {
    return 1;
  }
  if (pA->inPort < pB->inPort)
  {
    return -1;
  }

  if (pA->tunnelId > pB->tunnelId)
  {
    return 1;
  }
  if (pA->tunnelId < pB->tunnelId)
  {
    return -1;
  }

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  //added by yjg begin ingressPort流表增加lmepId作为key
  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }
  //added by yjg end

  return 0;
}

int ofdbInjectedOamFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbInjectedOamFlowEntryKey_t *pA = a;
  ofdbInjectedOamFlowEntryKey_t *pB = b;

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  return 0;
}

int ofdbVlanFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbVlanFlowEntryKey_t *pA = a;
  ofdbVlanFlowEntryKey_t *pB = b;

  if (pA->inPort > pB->inPort)
  {
    return 1;
  }
  if (pA->inPort < pB->inPort)
  {
    return -1;
  }

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  return 0;
}

int ofdbVlan1FlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbVlan1FlowEntryKey_t *pA = a;
  ofdbVlan1FlowEntryKey_t *pB = b;

  if (pA->inPort > pB->inPort)
  {
    return 1;
  }
  if (pA->inPort < pB->inPort)
  {
    return -1;
  }

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  if (pA->ovid > pB->ovid)
  {
    return 1;
  }
  if (pA->ovid < pB->ovid)
  {
    return -1;
  }

  return 0;
}

int ofdbMpFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbMpFlowEntryKey_t *pA = a;
  ofdbMpFlowEntryKey_t *pB = b;

  if (pA->inPort > pB->inPort)
  {
    return 1;
  }
  if (pA->inPort < pB->inPort)
  {
    return -1;
  }

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  if (pA->oamY1731Mdl > pB->oamY1731Mdl)
  {
    return 1;
  }
  if (pA->oamY1731Mdl < pB->oamY1731Mdl)
  {
    return -1;
  }

  if (pA->oamY1731Opcode > pB->oamY1731Opcode)
  {
    return 1;
  }
  if (pA->oamY1731Opcode < pB->oamY1731Opcode)
  {
    return -1;
  }

  return(memcmp(&pA->destMac, &pB->destMac, sizeof(pA->destMac)));
}

int ofdbTerminationMacFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbTerminationMacFlowEntryKey_t *pA = a;
  ofdbTerminationMacFlowEntryKey_t *pB = b;

  if (pA->inPort > pB->inPort)
  {
    return 1;
  }
  if (pA->inPort < pB->inPort)
  {
    return -1;
  }

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  return(memcmp(&pA->destMac, &pB->destMac, sizeof(pA->destMac)));
}

int ofdbMplsFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbMplsFlowEntryKey_t *pA = a;
  ofdbMplsFlowEntryKey_t *pB = b;

#if 0
  int result;
#endif

  if (pA->mplsLabel > pB->mplsLabel)
  {
    return 1;
  }
  if (pA->mplsLabel < pB->mplsLabel)
  {
    return -1;
  }

  if (pA->mplsDataFirstNibble > pB->mplsDataFirstNibble)
  {
    return 1;
  }

  if (pA->mplsDataFirstNibble < pB->mplsDataFirstNibble)
  {
    return -1;
  }

  if (pA->mplsAchChannel > pB->mplsAchChannel)
  {
    return 1;
  }
  if (pA->mplsAchChannel < pB->mplsAchChannel)
  {
    return -1;
  }

#if 0

  if (pA->nextLabelIsGal > pB->nextLabelIsGal)
  {
    return 1;
  }
  if (pA->nextLabelIsGal < pB->nextLabelIsGal)
  {
    return -1;
  }

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  if (pA->mplsBos > pB->mplsBos)
  {
    return 1;
  }
  if (pA->mplsBos < pB->mplsBos)
  {
    return -1;
  }

  if (pA->inPort > pB->inPort)
  {
    return 1;
  }
  if (pA->inPort < pB->inPort)
  {
    return -1;
  }

  if (pA->mplsTtl > pB->mplsTtl)
  {
    return 1;
  }

  if (pA->mplsTtl < pB->mplsTtl)
  {
    return -1;
  }

  if (pA->destIp4 > pB->destIp4)
  {
    return 1;
  }
  if (pA->destIp4 < pB->destIp4)
  {
    return -1;
  }

  if ((result = ofdbIp6AddrCmp(&pA->destIp6, &pB->destIp6))!= 0)
  {
    return result;
  }

  if ((result = ofdbIp6AddrCmp(&pA->destIp6, &pB->destIp6))!= 0)
  {
    return result;
  }

  if (pA->ipProto > pB->ipProto)
  {
    return 1;
  }
  if (pA->ipProto < pB->ipProto)
  {
    return -1;
  }

  if (pA->udpSrcPort > pB->udpSrcPort)
  {
    return 1;
  }
  if (pA->udpSrcPort < pB->udpSrcPort)
  {
    return -1;
  }

  if (pA->udpDstPort > pB->udpDstPort)
  {
    return 1;
  }
  if (pA->udpDstPort < pB->udpDstPort)
  {
    return -1;
  }
#endif

  return(0);
}

int ofdbMplsMpFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbMplsMpFlowEntryKey_t *pA = a;
  ofdbMplsMpFlowEntryKey_t *pB = b;

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  if (pA->oamY1731Opcode > pB->oamY1731Opcode)
  {
    return 1;
  }
  if (pA->oamY1731Opcode < pB->oamY1731Opcode)
  {
    return -1;
  }

  return 0;
}

int ofdbUnicastRoutingFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbUnicastRoutingFlowEntryKey_t *pA = a;
  ofdbUnicastRoutingFlowEntryKey_t *pB = b;

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  if (pA->vrf > pB->vrf)
  {
    return 1;
  }
  if (pA->vrf < pB->vrf)
  {
    return -1;
  }

  if (pA->dstIp4 > pB->dstIp4)
  {
    return 1;
  }
  if (pA->dstIp4 < pB->dstIp4)
  {
    return -1;
  }

  return(ofdbIp6AddrCmp(&pA->dstIp6, &pB->dstIp6));
}

int ofdbMulticastRoutingFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbMulticastRoutingFlowEntryKey_t *pA = a;
  ofdbMulticastRoutingFlowEntryKey_t *pB = b;
  int result;

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  if (pA->vrf > pB->vrf)
  {
    return 1;
  }
  if (pA->vrf < pB->vrf)
  {
    return -1;
  }

  if (pA->srcIp4 > pB->srcIp4)
  {
    return 1;
  }
  if (pA->srcIp4 < pB->srcIp4)
  {
    return -1;
  }

  if (pA->dstIp4 > pB->dstIp4)
  {
    return 1;
  }
  if (pA->dstIp4 < pB->dstIp4)
  {
    return -1;
  }

  if ((result = ofdbIp6AddrCmp(&pA->srcIp6, &pB->srcIp6))!= 0)
  {
    return result;
  }

  return(ofdbIp6AddrCmp(&pA->dstIp6, &pB->dstIp6));
}

int ofdbBridgingFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbBridgingFlowEntryKey_t *pA = a;
  ofdbBridgingFlowEntryKey_t *pB = b;

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  if (pA->tunnelId > pB->tunnelId)
  {
    return 1;
  }
  if (pA->tunnelId < pB->tunnelId)
  {
    return -1;
  }

  return(memcmp(&pA->destMac, &pB->destMac, sizeof(pA->destMac)));
}

int ofdbPolicyAclFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbPolicyAclFlowEntryKey_t *pA = a;
  ofdbPolicyAclFlowEntryKey_t *pB = b;
  int result;

  if (pA->inPort > pB->inPort)
  {
    return 1;
  }
  if (pA->inPort < pB->inPort)
  {
    return -1;
  }

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  if ((result = memcmp(&pA->srcMac, &pB->srcMac, sizeof(pA->srcMac)))!= 0)
  {
    return result;
  }

  if ((result = memcmp(&pA->destMac, &pB->destMac, sizeof(pA->destMac)))!= 0)
  {
    return result;
  }

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  if (pA->vlanPcp > pB->vlanPcp)
  {
    return 1;
  }
  if (pA->vlanPcp < pB->vlanPcp)
  {
    return -1;
  }

  if (pA->vlanDei > pB->vlanDei)
  {
    return 1;
  }
  if (pA->vlanDei < pB->vlanDei)
  {
    return -1;
  }

  if (pA->tunnelId > pB->tunnelId)
  {
    return 1;
  }
  if (pA->tunnelId < pB->tunnelId)
  {
    return -1;
  }

  if (pA->vrf > pB->vrf)
  {
    return 1;
  }
  if (pA->vrf < pB->vrf)
  {
    return -1;
  }

  if (pA->sourceIp4 > pB->sourceIp4)
  {
    return 1;
  }
  if (pA->sourceIp4 < pB->sourceIp4)
  {
    return -1;
  }

  if (pA->destIp4 > pB->destIp4)
  {
    return 1;
  }
  if (pA->destIp4 < pB->destIp4)
  {
    return -1;
  }

  if ((result = ofdbIp6AddrCmp(&pA->sourceIp6, &pB->sourceIp6))!= 0)
  {
    return result;
  }

  if ((result = ofdbIp6AddrCmp(&pA->destIp6, &pB->destIp6))!= 0)
  {
    return result;
  }

  if (pA->ipv4ArpSpa > pB->ipv4ArpSpa)
  {
    return 1;
  }
  if (pA->ipv4ArpSpa < pB->ipv4ArpSpa)
  {
    return -1;
  }

  if (pA->ipProto > pB->ipProto)
  {
    return 1;
  }
  if (pA->ipProto < pB->ipProto)
  {
    return -1;
  }

  if (pA->dscp > pB->dscp)
  {
    return 1;
  }
  if (pA->dscp < pB->dscp)
  {
    return -1;
  }

  if (pA->ecn > pB->ecn)
  {
    return 1;
  }
  if (pA->ecn < pB->ecn)
  {
    return -1;
  }

  if (pA->srcL4Port > pB->srcL4Port)
  {
    return 1;
  }
  if (pA->srcL4Port < pB->srcL4Port)
  {
    return -1;
  }

  if (pA->destL4Port > pB->destL4Port)
  {
    return 1;
  }
  if (pA->destL4Port < pB->destL4Port)
  {
    return -1;
  }

  if (pA->icmpType > pB->icmpType)
  {
    return 1;
  }
  if (pA->icmpType < pB->icmpType)
  {
    return -1;
  }

  if (pA->icmpCode > pB->icmpCode)
  {
    return 1;
  }
  if (pA->icmpCode < pB->icmpCode)
  {
    return -1;
  }

  if (pA->ipv6FlowLabel > pB->ipv6FlowLabel)
  {
    return 1;
  }
  if (pA->ipv6FlowLabel < pB->ipv6FlowLabel)
  {
    return -1;
  }

  return 0;
}

int ofdbColorActionsFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbColorActionsFlowEntryKey_t *pA = a;
  ofdbColorActionsFlowEntryKey_t *pB = b;

  if (pA->color > pB->color)
  {
    return 1;
  }
  if (pA->color < pB->color)
  {
    return -1;
  }

  if (pA->index > pB->index)
  {
    return 1;
  }
  if (pA->index < pB->index)
  {
    return -1;
  }

  return 0;
}

int ofdbEgressVlanFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbEgressVlanFlowEntryKey_t *pA = a;
  ofdbEgressVlanFlowEntryKey_t *pB = b;

  if (pA->outPort > pB->outPort)
  {
    return 1;
  }
  if (pA->outPort < pB->outPort)
  {
    return -1;
  }

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  if (pA->tunnelId > pB->tunnelId)
  {
    return 1;
  }
  if (pA->tunnelId < pB->tunnelId)
  {
    return -1;
  }

  if (pA->mplsL2Port > pB->mplsL2Port)
  {
    return 1;
  }
  if (pA->mplsL2Port < pB->mplsL2Port)
  {
    return -1;
  }

  return 0;
}

int ofdbEgressVlan1FlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbEgressVlan1FlowEntryKey_t *pA = a;
  ofdbEgressVlan1FlowEntryKey_t *pB = b;

  if (pA->outPort > pB->outPort)
  {
    return 1;
  }
  if (pA->outPort < pB->outPort)
  {
    return -1;
  }

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  if (pA->ovid > pB->ovid)
  {
    return 1;
  }
  if (pA->ovid < pB->ovid)
  {
    return -1;
  }

  return 0;
}

int ofdbEgressMpFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbEgressMpFlowEntryKey_t *pA = a;
  ofdbEgressMpFlowEntryKey_t *pB = b;

  if (pA->outPort > pB->outPort)
  {
    return 1;
  }
  if (pA->outPort < pB->outPort)
  {
    return -1;
  }

  if (pA->vlanId > pB->vlanId)
  {
    return 1;
  }
  if (pA->vlanId < pB->vlanId)
  {
    return -1;
  }

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  if (pA->oamY1731Mdl > pB->oamY1731Mdl)
  {
    return 1;
  }
  if (pA->oamY1731Mdl < pB->oamY1731Mdl)
  {
    return -1;
  }

  if (pA->oamY1731Opcode > pB->oamY1731Opcode)
  {
    return 1;
  }
  if (pA->oamY1731Opcode < pB->oamY1731Opcode)
  {
    return -1;
  }

  return(memcmp(&pA->destMac, &pB->destMac, sizeof(pA->destMac)));
}


int ofdbEgressDscpPcpRemarkFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbEgressDscpPcpRemarkFlowEntryKey_t *pA = a;
  ofdbEgressDscpPcpRemarkFlowEntryKey_t *pB = b;

  if (pA->outPort > pB->outPort)
  {
    return 1;
  }
  if (pA->outPort < pB->outPort)
  {
    return -1;
  }

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  if (pA->trafficClass > pB->trafficClass)
  {
    return 1;
  }
  if (pA->trafficClass < pB->trafficClass)
  {
    return -1;
  }

  if (pA->color > pB->color)
  {
    return 1;
  }
  if (pA->color < pB->color)
  {
    return -1;
  }

  return 0;
}

int ofdbEgressTpidFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbEgressTpidFlowEntryKey_t *pA = a;
  ofdbEgressTpidFlowEntryKey_t *pB = b;

  if (pA->outPort > pB->outPort)
  {
    return 1;
  }
  if (pA->outPort < pB->outPort)
  {
    return -1;
  }

  return 0;
}

int ofdbMplsL2PortFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbMplsL2PortFlowEntryKey_t *pA = a;
  ofdbMplsL2PortFlowEntryKey_t *pB = b;

  if (pA->mplsL2Port > pB->mplsL2Port)
  {
    return 1;
  }
  if (pA->mplsL2Port < pB->mplsL2Port)
  {
    return -1;
  }

  if (pA->tunnelId > pB->tunnelId)
  {
    return 1;
  }
  if (pA->tunnelId < pB->tunnelId)
  {
    return -1;
  }

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }

  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  return 0;
}

int ofdbDscpTrustFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbDscpTrustFlowEntryKey_t *pA = a;
  ofdbDscpTrustFlowEntryKey_t *pB = b;

  if (pA->qosIndex > pB->qosIndex)
  {
    return 1;
  }
  if (pA->qosIndex < pB->qosIndex)
  {
    return -1;
  }

  if (pA->dscpValue > pB->dscpValue)
  {
    return 1;
  }
  if (pA->dscpValue < pB->dscpValue)
  {
    return -1;
  }

  return 0;
}

int ofdbPcpTrustFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbPcpTrustFlowEntryKey_t *pA = a;
  ofdbPcpTrustFlowEntryKey_t *pB = b;

  if (pA->qosIndex > pB->qosIndex)
  {
    return 1;
  }
  if (pA->qosIndex < pB->qosIndex)
  {
    return -1;
  }

  if (pA->pcpValue > pB->pcpValue)
  {
    return 1;
  }
  if (pA->pcpValue < pB->pcpValue)
  {
    return -1;
  }

  if (pA->dei > pB->dei)
  {
    return 1;
  }
  if (pA->dei < pB->dei)
  {
    return -1;
  }

  return 0;
}

int ofdbMplsQosClassFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbMplsQosClassFlowEntryKey_t *pA = a;
  ofdbMplsQosClassFlowEntryKey_t *pB = b;
  int result;

  if (pA->mplsL2Port > pB->mplsL2Port)
  {
    return 1;
  }
  if (pA->mplsL2Port < pB->mplsL2Port)
  {
    return -1;
  }

  if (pA->tunnelId > pB->tunnelId)
  {
    return 1;
  }
  if (pA->tunnelId < pB->tunnelId)
  {
    return -1;
  }

  if (pA->etherType > pB->etherType)
  {
    return 1;
  }
  if (pA->etherType < pB->etherType)
  {
    return -1;
  }

  if ((result = memcmp(&pA->srcMac, &pB->srcMac, sizeof(pA->srcMac)))!= 0)
  {
    return result;
  }

  if ((result = memcmp(&pA->destMac, &pB->destMac, sizeof(pA->destMac)))!= 0)
  {
    return result;
  }

  if (pA->vlanPcp > pB->vlanPcp)
  {
    return 1;
  }
  if (pA->vlanPcp < pB->vlanPcp)
  {
    return -1;
  }

  if (pA->vlanDei > pB->vlanDei)
  {
    return 1;
  }
  if (pA->vlanDei < pB->vlanDei)
  {
    return -1;
  }

  if (pA->sourceIp4 > pB->sourceIp4)
  {
    return 1;
  }
  if (pA->sourceIp4 < pB->sourceIp4)
  {
    return -1;
  }

  if (pA->destIp4 > pB->destIp4)
  {
    return 1;
  }
  if (pA->destIp4 < pB->destIp4)
  {
    return -1;
  }

  if ((result = ofdbIp6AddrCmp(&pA->sourceIp6, &pB->sourceIp6))!= 0)
  {
    return result;
  }

  if ((result = ofdbIp6AddrCmp(&pA->destIp6, &pB->destIp6))!= 0)
  {
    return result;
  }

  if (pA->ipProto > pB->ipProto)
  {
    return 1;
  }
  if (pA->ipProto < pB->ipProto)
  {
    return -1;
  }

  if (pA->dscp > pB->dscp)
  {
    return 1;
  }
  if (pA->dscp < pB->dscp)
  {
    return -1;
  }

  if (pA->srcL4Port > pB->srcL4Port)
  {
    return 1;
  }
  if (pA->srcL4Port < pB->srcL4Port)
  {
    return -1;
  }

  if (pA->destL4Port > pB->destL4Port)
  {
    return 1;
  }
  if (pA->destL4Port < pB->destL4Port)
  {
    return -1;
  }

  return 0;
}

int ofdbL2PolicerFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbL2PolicerFlowEntryKey_t *pA = a;
  ofdbL2PolicerFlowEntryKey_t *pB = b;

  if (pA->tunnelId > pB->tunnelId)
  {
    return 1;
  }
  if (pA->tunnelId < pB->tunnelId)
  {
    return -1;
  }

  if (pA->mplsL2Port > pB->mplsL2Port)
  {
    return 1;
  }
  if (pA->mplsL2Port < pB->mplsL2Port)
  {
    return -1;
  }

  return 0;
}

int ofdbL2PolicerActionsFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbL2PolicerActionsFlowEntryKey_t *pA = a;
  ofdbL2PolicerActionsFlowEntryKey_t *pB = b;

  if (pA->colorActionsIndex > pB->colorActionsIndex)
  {
    return 1;
  }
  if (pA->colorActionsIndex < pB->colorActionsIndex)
  {
    return -1;
  }

  if (pA->color > pB->color)
  {
    return 1;
  }
  if (pA->color < pB->color)
  {
    return -1;
  }

  return 0;
}

int ofdbGroupBucketRefTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbGroupBucketRefTable_node_t *pA = a;
  ofdbGroupBucketRefTable_node_t *pB = b;

  if (pA->groupBucket.groupId > pB->groupBucket.groupId)
  {
    return 1;
  }
  if (pA->groupBucket.groupId < pB->groupBucket.groupId)
  {
    return -1;
  }

  if (pA->groupBucket.bucketIndex > pB->groupBucket.bucketIndex)
  {
    return 1;
  }
  if (pA->groupBucket.bucketIndex < pB->groupBucket.bucketIndex)
  {
    return -1;
  }
  return 0;
}

int ofdbGroupBucketDataTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbGroupBucketDataTable_node_t *pA = a;
  ofdbGroupBucketDataTable_node_t *pB = b;

  if (pA->groupBucket.groupId > pB->groupBucket.groupId)
  {
    return 1;
  }
  if (pA->groupBucket.groupId < pB->groupBucket.groupId)
  {
    return -1;
  }

  if (pA->groupBucket.bucketIndex > pB->groupBucket.bucketIndex)
  {
    return 1;
  }
  if (pA->groupBucket.bucketIndex < pB->groupBucket.bucketIndex)
  {
    return -1;
  }
  return 0;
}

int ofdbTunnelPortTenantTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbTunnelPortTenantTable_node_t *pA = a;
  ofdbTunnelPortTenantTable_node_t *pB = b;

  if (pA->portNum > pB->portNum)
  {
    return 1;
  }
  if (pA->portNum < pB->portNum)
  {
    return -1;
  }

  if (pA->tunnelId > pB->tunnelId)
  {
    return 1;
  }
  if (pA->tunnelId < pB->tunnelId)
  {
    return -1;
  }
  return 0;
}

int ofdbTunnelEcmpNextHopGroupMemberTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbTunnelEcmpNextHopGroupMemberTable_node_t *pA = a;
  ofdbTunnelEcmpNextHopGroupMemberTable_node_t *pB = b;

  if (pA->ecmpNextHopGroupId > pB->ecmpNextHopGroupId)
  {
    return 1;
  }
  if (pA->ecmpNextHopGroupId < pB->ecmpNextHopGroupId)
  {
    return -1;
  }

  if (pA->nextHopId > pB->nextHopId)
  {
    return 1;
  }
  if (pA->nextHopId < pB->nextHopId)
  {
    return -1;
  }
  return 0;
}

int ofdbOamMegTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbOamMegTable_node_t *pA = a;
  ofdbOamMegTable_node_t *pB = b;

  if (pA->megIndex > pB->megIndex)
  {
    return 1;
  }
  if (pA->megIndex < pB->megIndex)
  {
    return -1;
  }

  return 0;
}

int ofdbOamMepTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbOamMepTable_node_t *pA = a;
  ofdbOamMepTable_node_t *pB = b;

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  return 0;
}

int ofdbOamMipTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbOamMipTable_node_t *pA = a;
  ofdbOamMipTable_node_t *pB = b;

  if (pA->localMpId > pB->localMpId)
  {
    return 1;
  }
  if (pA->localMpId < pB->localMpId)
  {
    return -1;
  }

  return 0;
}

int ofdbOamRemoteMpTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbOamRemoteMpTable_node_t *pA = a;
  ofdbOamRemoteMpTable_node_t *pB = b;

  if (pA->rmpId > pB->rmpId)
  {
    return 1;
  }
  if (pA->rmpId < pB->rmpId)
  {
    return -1;
  }

  return 0;
}

int ofdbOamMLPGroupMepListNodeCompare(void *a, void *b, size_t l)
{
  ofdbOamMLPGroupMepList_node_t *pA = a;
  ofdbOamMLPGroupMepList_node_t *pB = b;

  if (pA->mlpGroupIndex > pB->mlpGroupIndex)
  {
    return 1;
  }
  if (pA->mlpGroupIndex < pB->mlpGroupIndex)
  {
    return -1;
  }

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  return 0;
}

int ofdbMplsLabelTrustFlowTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbMplsLabelTrustFlowEntryKey_t *pA = a;
  ofdbMplsLabelTrustFlowEntryKey_t *pB = b;

  if (pA->qosIndex > pB->qosIndex)
  {
    return 1;
  }
  if (pA->qosIndex < pB->qosIndex)
  {
    return -1;
  }

  if (pA->mplsTC > pB->mplsTC)
  {
    return 1;
  }
  if (pA->mplsTC < pB->mplsTC)
  {
    return -1;
  }

  return 0;
}

int ofdbDropStatusTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbDropStatusTable_node_t *pA = a;
  ofdbDropStatusTable_node_t *pB = b;

  if (pA->dropEntry.lmepId > pB->dropEntry.lmepId)
  {
    return 1;
  }
  if (pA->dropEntry.lmepId < pB->dropEntry.lmepId)
  {
    return -1;
  }

  return 0;
}

int ofdbOamDataCounterTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbOamDataCounterTable_node_t *pA = a;
  ofdbOamDataCounterTable_node_t *pB = b;

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  if (pA->trafficClass > pB->trafficClass)
  {
    return 1;
  }
  if (pA->trafficClass < pB->trafficClass)
  {
    return -1;
  }

  return 0;
}

int ofdbRemarkActionTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbRemarkActionEntryKey_t *pA = a;
  ofdbRemarkActionEntryKey_t *pB = b;

  if (pA->actionTableType > pB->actionTableType)
  {
    return 1;
  }
  if (pA->actionTableType < pB->actionTableType)
  {
    return -1;
  }

  if (pA->index > pB->index)
  {
    return 1;
  }
  if (pA->index < pB->index)
  {
    return -1;
  }

  if (pA->trafficClass > pB->trafficClass)
  {
    return 1;
  }
  if (pA->trafficClass < pB->trafficClass)
  {
    return -1;
  }

  if (pA->color > pB->color)
  {
    return 1;
  }
  if (pA->color < pB->color)
  {
    return -1;
  }

  return 0;
}


#if 1

int ofdbBfdTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbBfdTable_node_t *pA = a;
  ofdbBfdTable_node_t *pB = b;

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  return 0;
}




int ofdbPwRedunMepListNodeCompare(void *a, void *b, size_t l)
{
  ofdbPwRedunMepList_node_t *pA = a;
  ofdbPwRedunMepList_node_t *pB = b;

  if (pA->mlpGroupIndex > pB->mlpGroupIndex)
  {
    return 1;
  }
  if (pA->mlpGroupIndex < pB->mlpGroupIndex)
  {
    return -1;
  }

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  return 0;
}


int ofdbY1564TableNodeCompare(void *a, void *b, size_t l)
{
  ofdbY1564Table_node_t *pA = a;
  ofdbY1564Table_node_t *pB = b;

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  return 0;
}


int ofdbLoopbackTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbLbTable_node_t *pA = a;
  ofdbLbTable_node_t *pB = b;

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  return 0;
}



int ofdbVlanPortTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbVlanPortTable_node_t *pA = a;
  ofdbVlanPortTable_node_t *pB = b;

  if (pA->port > pB->port)
  {
    return 1;
  }
  if (pA->port < pB->port)
  {
    return -1;
  }

  return 0;
}



int ofdbVlanIpTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbVlanIpTable_node_t *pA = a;
  ofdbVlanIpTable_node_t *pB = b;

  if (pA->vlan > pB->vlan)
  {
    return 1;
  }
  if (pA->vlan < pB->vlan)
  {
    return -1;
  }

  return 0;
}


int ofdbLmepidTableNodeCompare(void *a, void *b, size_t l)
{
  ofdbLmepidTable_node_t *pA = a;
  ofdbLmepidTable_node_t *pB = b;

  if (pA->lmepId > pB->lmepId)
  {
    return 1;
  }
  if (pA->lmepId < pB->lmepId)
  {
    return -1;
  }

  return 0;
}


#endif


int ofdbInit(void)
{
  uint32_t ii;
  uint32_t tableFlowEntryCount, flowEntriesTotal, entryCount;

  
/* BEGIN: Added by Hushouqiang, 2016/3/22   问题单号:P10002 */
	OFDPA_ERROR_t rc;
	uint32_t tmp;
/* END:   Added by Hushouqiang, 2016/3/22 */
  if (dpaRWlockInit(&ofdbLock, NULL) != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB read-write lock.\r\n", 0);
    return -1;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "Allocating %u bytes for OFDB Global Data.\r\n",
                     sizeof(ofdbGlobalData_t));

  ofdbGlobalData = malloc(sizeof(*ofdbGlobalData));
  if (NULL == ofdbGlobalData)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to allocate memory for OFDB Global Data.\r\n", 0);
    return -1;
  }
  memset(ofdbGlobalData, 0, sizeof(*ofdbGlobalData));

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "Allocating %u bytes for OFDB Flow Table Status Data.\r\n",
                     sizeof(ofdbFlowTableStatus_t));

  ofdbFlowTableStatus = malloc(sizeof(*ofdbFlowTableStatus));
  if (NULL == ofdbFlowTableStatus)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to allocate memory for OFDB Flow Table Status Data.\r\n", 0);
    return -1;
  }
  memset(ofdbFlowTableStatus, 0, sizeof(*ofdbFlowTableStatus));

  
  flowEntriesTotal = 0;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Ingress Port Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_INGRESS_PORT);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->ingressPortMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbIngressPortFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbIngressPortFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbIngressPortFlowTableNodeCompare,
		                           0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Ingress Port Flow Table AVL Tree.\r\n", 0);
			return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Port DSCP Trust Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->portDscpTrustMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbPortDscpTrustFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbDscpTrustFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbDscpTrustFlowTableNodeCompare,
		                           0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Port DSCP Trust Flow Table AVL Tree.\r\n", 0);
			return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
  

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Port PCP Trust Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->portPcpTrustMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbPortPcpTrustFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbPcpTrustFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbPcpTrustFlowTableNodeCompare,
		                           0) != 0)
		{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Port PCP Trust Flow Table AVL Tree.\r\n", 0);
		return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}


  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Tunnel DSCP Trust Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST);
  if (0 == tableFlowEntryCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "OFDB Tunnel DSCP Trust Flow Table size 0 returned.\r\n", 0);
  }
  else
  {
    ofdbFlowTableStatus->tunnelDscpTrustMaxCount = tableFlowEntryCount;
    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbTunnelDscpTrustFlowTable_tree,
                                 tableFlowEntryCount,
                                 sizeof(ofdbDscpTrustFlowTable_node_t),
                                 0x10,
                                 (avlComparator_t)ofdbDscpTrustFlowTableNodeCompare,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Tunnel DSCP Trust Flow Table AVL Tree.\r\n", 0);
      return -1;
    }
  }
  flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Tunnel PCP Trust Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST);
  if (0 == tableFlowEntryCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "OFDB Tunnel PCP Trust Flow Table size 0 returned.\r\n", 0);
  }
  else
  {
    ofdbFlowTableStatus->tunnelPcpTrustMaxCount = tableFlowEntryCount;
    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbTunnelPcpTrustFlowTable_tree,
                                 tableFlowEntryCount,
                                 sizeof(ofdbPcpTrustFlowTable_node_t),
                                 0x10,
                                 (avlComparator_t)ofdbPcpTrustFlowTableNodeCompare,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Tunnel PCP Trust Flow Table AVL Tree.\r\n", 0);
      return -1;
    }
  }
  flowEntriesTotal += tableFlowEntryCount;


  

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB VLAN Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_VLAN);
	if (tableFlowEntryCount > 0)
	{

		ofdbFlowTableStatus->vlanMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbVlanFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbVlanFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbVlanFlowTableNodeCompare,
		                           0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB VLAN Flow Table AVL Tree.\r\n", 0);
			return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else {
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create VLAN Flow Table AVL Tree.\r\n", 0);
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB VLAN_1 Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_VLAN_1);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->vlan1MaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbVlan1FlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbVlan1FlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbVlan1FlowTableNodeCompare,
		                           0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB VLAN_1 Flow Table AVL Tree.\r\n", 0);
			return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}	
	else {
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create VLAN_1 Flow Table AVL Tree.\r\n", 0);
	}
  

	OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Creating OFDB MPLS L2 Port Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT);
	if (0 == tableFlowEntryCount)
	{
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDB MPLS L2 Port Flow Table size 0 returned.\r\n", 0);
	}
	else
	{
	  ofdbFlowTableStatus->mplsL2PortMaxCount = tableFlowEntryCount;
	  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMplsL2PortFlowTable_tree,
	                               tableFlowEntryCount,
	                               sizeof(ofdbMplsL2PortFlowTable_node_t),
	                               0x10,
	                               (avlComparator_t)ofdbMplsL2PortFlowTableNodeCompare,
	                               0) != 0)
	  {
	    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB MPLS L2 Port Flow Table AVL Tree.\r\n", 0);
	    return -1;
	  }
	}
	flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB MPLS DSCP Trust Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST);
  if (0 == tableFlowEntryCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "OFDB MPLS DSCP Trust Flow Table size 0 returned.\r\n", 0);
  }
  else
  {
    ofdbFlowTableStatus->mplsDscpTrustMaxCount = tableFlowEntryCount;
    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMplsDscpTrustFlowTable_tree,
                                 tableFlowEntryCount,
                                 sizeof(ofdbDscpTrustFlowTable_node_t),
                                 0x10,
                                 (avlComparator_t)ofdbDscpTrustFlowTableNodeCompare,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB MPLS DSCP Trust Flow Table AVL Tree.\r\n", 0);
      return -1;
    }
  }
  flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB MPLS PCP Trust Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST);
  if (0 == tableFlowEntryCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "OFDB MPLS PCP Trust Flow Table size 0 returned.\r\n", 0);
  }
  else
  {
    ofdbFlowTableStatus->mplsPcpTrustMaxCount = tableFlowEntryCount;
    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMplsPcpTrustFlowTable_tree,
                                 tableFlowEntryCount,
                                 sizeof(ofdbPcpTrustFlowTable_node_t),
                                 0x10,
                                 (avlComparator_t)ofdbPcpTrustFlowTableNodeCompare,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB MPLS PCP Trust Flow Table AVL Tree.\r\n", 0);
      return -1;
    }
  }
  flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB MPLS Label Trust Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST);
  if (0 == tableFlowEntryCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "OFDB MPLS Label Trust Flow Table size 0 returned.\r\n", 0);
  }
  else
  {
    ofdbFlowTableStatus->mplsLabelTrustMaxCount = tableFlowEntryCount;
    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMplsLabelTrustFlowTable_tree,
                                 tableFlowEntryCount,
                                 sizeof(ofdbMplsLabelTrustFlowTable_node_t),
                                 0x10,
                                 (avlComparator_t)ofdbMplsLabelTrustFlowTableNodeCompare,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB MPLS Label Trust Flow Table AVL Tree.\r\n", 0);
      return -1;
    }
  }
  flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB MPLS QoS Class Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS);
  if (0 == tableFlowEntryCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "OFDB MPLS QoS Class Flow Table size 0 returned.\r\n", 0);
  }
  else
  {
    ofdbFlowTableStatus->mplsQosClassMaxCount = tableFlowEntryCount;
    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMplsQosClassFlowTable_tree,
                                 tableFlowEntryCount,
                                 sizeof(ofdbMplsQosClassFlowTable_node_t),
                                 0x10,
                                 (avlComparator_t)ofdbMplsQosClassFlowTableNodeCompare,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB MPLS QoS Class Flow Table AVL Tree.\r\n", 0);
      return -1;
    }
  }
  flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB L2 Policer Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_L2_POLICER);
  if (0 == tableFlowEntryCount)
  {
    return -1;
  }
  ofdbFlowTableStatus->l2PolicerMaxCount = tableFlowEntryCount;
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbL2PolicerFlowTable_tree,
                               tableFlowEntryCount,
                               sizeof(ofdbL2PolicerFlowTable_node_t),
                               0x10,
                               (avlComparator_t)ofdbL2PolicerFlowTableNodeCompare,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB L2 Policer Flow Table AVL Tree.\r\n", 0);
    return -1;
  }
  flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB L2 Policer Actions Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS);
	if (0 == tableFlowEntryCount)
	{
		/**/
	}
	else{
		ofdbFlowTableStatus->l2PolicerActionsMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbL2PolicerActionsFlowTable_tree,
		                             tableFlowEntryCount,
		                             sizeof(ofdbL2PolicerActionsFlowTable_node_t),
		                             0x10,
		                             (avlComparator_t)ofdbL2PolicerActionsFlowTableNodeCompare,
		                             0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB L2 Policer Actions Flow Table AVL Tree.\r\n", 0);
		  return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Term MAC Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_TERMINATION_MAC);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->terminationMacMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbTerminationMacFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbTerminationMacFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbTerminationMacFlowTableNodeCompare,
		                           0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Term MAC Flow Table AVL Tree.\r\n", 0);
			return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Term MAC Flow Table AVL Tree.\r\n", 0);
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB MPLS 0 Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_MPLS_0);
  if (0 == tableFlowEntryCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "OFDB MPLS 0 Flow Table size 0 returned.\r\n", 0);
  }
  else
  {
    ofdbFlowTableStatus->mpls0MaxCount = tableFlowEntryCount;
    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMpls0FlowTable_tree,
                                 tableFlowEntryCount,
                                 sizeof(ofdbMplsFlowTable_node_t),
                                 0x10,
                                 (avlComparator_t)ofdbMplsFlowTableNodeCompare,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB MPLS 0 Flow Table AVL Tree.\r\n", 0);
      return -1;
    }
  }
  flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB MPLS 1 Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_MPLS_1);
  if (0 == tableFlowEntryCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "OFDB MPLS 1 Flow Table size 0 returned.\r\n", 0);
  }
  else
  {
    ofdbFlowTableStatus->mpls1MaxCount = tableFlowEntryCount;
    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMpls1FlowTable_tree,
                                 tableFlowEntryCount,
                                 sizeof(ofdbMplsFlowTable_node_t),
                                 0x10,
                                 (avlComparator_t)ofdbMplsFlowTableNodeCompare,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB MPLS 1 Flow Table AVL Tree.\r\n", 0);
      return -1;
    }
  }
  flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB MPLS-TP Maintenance Point Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT);
  if (0 == tableFlowEntryCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT.\r\n", 0);
    /*return -1;*/	/*leishenghua modify 20161111, 8800 not support*/
  }
  ofdbFlowTableStatus->mplsMpMaxCount = tableFlowEntryCount;
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMplsMpFlowTable_tree,
                               tableFlowEntryCount,
                               sizeof(ofdbMplsMpFlowTable_node_t),
                               0x10,
                               (avlComparator_t)ofdbMplsMpFlowTableNodeCompare,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB MPLS-TP Maintenance Point Flow Table AVL Tree.\r\n", 0);
    return -1;
  }
  flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Unicast Routing Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->unicastRoutingMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbUnicastRoutingFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbUnicastRoutingFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbUnicastRoutingFlowTableNodeCompare,
		                           0) != 0)
		{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Unicast Routing Flow Table AVL Tree.\r\n", 0);
		return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else	
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING.\r\n", 0);
	}
	
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Multicast Routing Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->multicastRoutingMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMulticastRoutingFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbMulticastRoutingFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbMulticastRoutingFlowTableNodeCompare,
		                           0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Multicast Routing Flow Table AVL Tree.\r\n", 0);
			return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING.\r\n", 0);
		//return -1;
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Bridging Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_BRIDGING);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->bridgingMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbBridgingFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbBridgingFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbBridgingFlowTableNodeCompare,
		                           0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Bridging Flow Table AVL Tree.\r\n", 0);
			return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_BRIDGING.\r\n", 0);
		//return -1;
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Policy ACL Flow Table AVL Tree.\r\n", 0);
	tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_ACL_POLICY);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->policyAclMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbPolicyAclFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbPolicyAclFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbPolicyAclFlowTableNodeCompare,
		                           0) != 0)
		{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Policy ACL Flow Table AVL Tree.\r\n", 0);
		return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_ACL_POLICY.\r\n", 0);
		//return -1;
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Color Based Actions Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS);
	if (0 == tableFlowEntryCount)
	{
		ofdbFlowTableStatus->colorActionsMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbColorActionsFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbColorActionsFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbColorActionsFlowTableNodeCompare,
		                           0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Color Based Actions Flow Table AVL Tree.\r\n", 0);
			return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS.\r\n", 0);
		//return -1;
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Egress VLAN Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_EGRESS_VLAN);
	if (0 == tableFlowEntryCount)
	{
		ofdbFlowTableStatus->egressVlanMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbEgressVlanFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbEgressVlanFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbEgressVlanFlowTableNodeCompare,
		                           0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Egress VLAN Flow Table AVL Tree.\r\n", 0);
			return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_EGRESS_VLAN.\r\n", 0);
		//return -1;
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Egress VLAN_1 Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1);
	if (0 == tableFlowEntryCount)
	{
		ofdbFlowTableStatus->egressVlan1MaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbEgressVlan1FlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbEgressVlan1FlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbEgressVlan1FlowTableNodeCompare,
		                           0) != 0)
		{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Egress VLAN_1 Flow Table AVL Tree.\r\n", 0);
		return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else							   
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1.\r\n", 0);
		//return -1;
	}


  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Egress Maintenance Point Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->egressMpMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbEgressMpFlowTable_tree,
		                             tableFlowEntryCount,
		                             sizeof(ofdbEgressMpFlowTable_node_t),
		                             0x10,
		                             (avlComparator_t)ofdbEgressMpFlowTableNodeCompare,
		                             0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Egress Maintenance Point Flow Table AVL Tree.\r\n", 0);
		  return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT.\r\n", 0);
		//return -1;
	}
	flowEntriesTotal += tableFlowEntryCount;

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Egress DSCP PCP Remark Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->egressDscpPcpRemarkMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbEgressDscpPcpRemarkFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbEgressDscpPcpRemarkFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbEgressDscpPcpRemarkFlowTableNodeCompare,
		                           0) != 0)
		{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Egress DSCP PCP Remark Flow Table AVL Tree.\r\n", 0);
		return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK.\r\n", 0);
		//return -1;
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Egress TPID Flow Table AVL Tree.\r\n", 0);
  tableFlowEntryCount = dpaFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_EGRESS_TPID);
	if (tableFlowEntryCount > 0)
	{
		ofdbFlowTableStatus->egressTpidMaxCount = tableFlowEntryCount;
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbEgressTpidFlowTable_tree,
		                           tableFlowEntryCount,
		                           sizeof(ofdbEgressTpidFlowTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbEgressTpidFlowTableNodeCompare,
		                           0) != 0)
		{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Egress TPID Flow Table AVL Tree.\r\n", 0);
		return -1;
		}
		flowEntriesTotal += tableFlowEntryCount;
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "OFDPA_FLOW_TABLE_ID_EGRESS_TPID.\r\n", 0);
		//return -1;
	}

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Flow ID Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbFlowFlowIdTable_tree,
                               flowEntriesTotal,
                               sizeof(ofdbFlowIndexTable_node_t),
                               0x10,
                               avlCompareULongLong64,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Flow ID Table AVL Tree.\r\n", 0);
    return -1;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Flow Cookie Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbFlowCookieTable_tree,
                               flowEntriesTotal,
                               sizeof(ofdbFlowIndexTable_node_t),
                               0x10,
                               avlCompareULongLong64,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Flow Cookie Table AVL Tree.\r\n", 0);
    return -1;
  }

  /* Group Table Global Status.
   */
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "Allocating %u bytes for OFDB Group Table Status Data.\r\n",
                     sizeof(ofdbGroupTableStatus_t));

  if (!(ofdbGroupTableStatus = malloc (sizeof(ofdbGroupTableStatus_t))))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to allocate memory for OFDB Group Table Status Data.\r\n", 0);
    return -1;
  }
  

  memset(ofdbGroupTableStatus, 0, sizeof(ofdbGroupTableStatus_t));

  /* Group Table AVL Tree.
   */

  for (ii = OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE; ii < OFDPA_GROUP_ENTRY_TYPE_LAST; ii++)
  {
    ofdbGroupTableStatus->groupTableMaxCount[ii] = dpaGroupMaxSizeGet(ii);
    ofdbGroupTableStatus->group_database_size += dpaGroupMaxSizeGet(ii);

    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group count for type %d = %d.\r\n",
                       ii, ofdbGroupTableStatus->groupTableMaxCount[ii]);
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group_table_size = %d.\r\n",
                     ofdbGroupTableStatus->group_database_size);

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Group Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbGroupTable_tree,
                               ofdbGroupTableStatus->group_database_size,
                               sizeof(ofdbGroupTable_node_t),
                               0x10,
                               avlCompareULong32,
                              0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Group Table AVL Tree.\r\n", 0);
    return -1;
  }


  /* Group Table Global Status.
   */
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "Allocating %u bytes for OFDB Group Bucket Table Status Data.\r\n",
                     sizeof(ofdbGroupBucketTableStatus_t));

  if (!(ofdbGroupBucketTableStatus = malloc (sizeof(ofdbGroupBucketTableStatus_t))))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to allocate memory for OFDB Group Bucket Table Status Data.\r\n", 0);
    return -1;
  }

  memset(ofdbGroupBucketTableStatus, 0, sizeof(ofdbGroupBucketTableStatus_t));

  /* Group Bucket Table AVL Trees.
   */
	for (ii = OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE; ii < OFDPA_GROUP_ENTRY_TYPE_LAST; ii++)
	{
		if((ii == OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
				|| (ii == OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
				//|| (ii == OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING)
				|| (ii == OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE)
				|| (ii == OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST)
				|| (ii == OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD))
				
		 {
			 ofdbGroupBucketTableStatus->groupBucketTableMaxCount[ii] = dpaGroupBucketMaxSizeGet(ii);
			 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group bucket count for type %d = %d.\r\n",
													ii, ofdbGroupBucketTableStatus->groupBucketTableMaxCount[ii]);
	 
			 switch(ii)
			 {
				 case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
				 case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
				 case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
				 case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
				 case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
				 case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
				 case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
					 ofdbGroupBucketTableStatus->group_bucket_data_database_size +=
						 (dpaGroupMaxSizeGet(ii) * ofdbGroupBucketTableStatus->groupBucketTableMaxCount[ii]);
					 break;
	 
				 case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
				 case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
				 case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
				 case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
					 ofdbGroupBucketTableStatus->group_bucket_ref_database_size +=
						 (dpaGroupMaxSizeGet(ii) * ofdbGroupBucketTableStatus->groupBucketTableMaxCount[ii]);
					 break;
	 
				 case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
					 /* Divide equally into data and reference table as this Group has both types  */
					 ofdbGroupBucketTableStatus->group_bucket_data_database_size +=
						 ((dpaGroupMaxSizeGet(ii)/2) * ofdbGroupBucketTableStatus->groupBucketTableMaxCount[ii]);
	 
					 ofdbGroupBucketTableStatus->group_bucket_ref_database_size +=
						 ((dpaGroupMaxSizeGet(ii)/2) * ofdbGroupBucketTableStatus->groupBucketTableMaxCount[ii]);
	 
					 break;
	 
					 /* coverity[dead_error_begin] */
				 default:
					 break;
			 }
		 }
	}


  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group_bucket_ref_table_size = %d.\r\n",
                     ofdbGroupBucketTableStatus->group_bucket_ref_database_size);

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "group_bucket_data_table_size = %d.\r\n",
                     ofdbGroupBucketTableStatus->group_bucket_data_database_size);

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Group Bucket Ref Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbGroupBucketRefTable_tree,
                               ofdbGroupBucketTableStatus->group_bucket_ref_database_size,
                               sizeof(ofdbGroupBucketRefTable_node_t),
                               0x10,
                               (avlComparator_t)ofdbGroupBucketRefTableNodeCompare,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Group Bucket Ref Table AVL Tree.\r\n", 0);
    return -1;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Group Bucket Data Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbGroupBucketDataTable_tree,
                               ofdbGroupBucketTableStatus->group_bucket_data_database_size,
                               sizeof(ofdbGroupBucketDataTable_node_t),
                               0x10,
                               (avlComparator_t)ofdbGroupBucketDataTableNodeCompare,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Group Bucket Data Table AVL Tree.\r\n", 0);
    return -1;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Port Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbPortTable_tree,
                               dpaPortTableEntryCountGet() * 2,
                               sizeof(ofdbPortTable_node_t),
                               0x10,
                               avlCompareULong32,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Port Table AVL Tree.\r\n", 0);
    return -1;
  }

/* BEGIN: Added by Hushouqiang, 2016/3/22   问题单号:P10002 */
  rc = dpaCosQueuesMaxGet(&tmp);

	if(rc == OFDPA_E_NONE)
	{
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Cos Queue AVL Tree.\r\n", 0);
	  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbCosQueues_tree,
	                               tmp,
	                               sizeof(ofdbCosQueue_node_t),
	                               0x10,
	                               avlCompareULong32,
	                               0) != 0)
	  {
	    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB CoS Queue AVL Tree.\r\n", 0);
	    return -1;
	  }
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to get CoS queues Max.\r\n", 0);
	}
/* END:   Added by Hushouqiang, 2016/3/22 */

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Port Tenant Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbPortTenantTable_tree,
                               dpaTunnelPortTenantCountGet(),
                               sizeof(ofdbTunnelPortTenantTable_node_t),
                               0x10,
                               (avlComparator_t)ofdbTunnelPortTenantTableNodeCompare,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Port Tenant Table AVL Tree.\r\n", 0);
    return -1;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Tunnel Tenant Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbTunnelTenantTable_tree,
                               dpaTunnelTenantCountGet(),
                               sizeof(ofdbTunnelNextHopTable_node_t),
                               0x10,
                               avlCompareULong32,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Tunnel Tenant Table AVL Tree.\r\n", 0);
    return -1;
  }


  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Tunnel Next Hop Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbTunnelNextHopTable_tree,
                               dpaTunnelNextHopTableEntryCountGet(),
                               sizeof(ofdbTunnelNextHopTable_node_t),
                               0x10,
                               avlCompareULong32,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Tunnel Next Hop Table AVL Tree.\r\n", 0);
    return -1;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Tunnel Ecmp Next Hop Group Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupTable_tree,
                               dpaTunnelEcmpNextHopGroupTableEntryCountGet(),
                               sizeof(ofdbTunnelEcmpNextHopGroupTable_node_t),
                               0x10,
                               avlCompareULong32,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Tunnel Ecmp Next Hop Group Table AVL Tree.\r\n", 0);
    return -1;
  }

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Tunnel Ecmp Next Hop Group Member Table AVL Tree.\r\n", 0);
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbTunnelEcmpNextHopGroupMemberTable_tree,
                               dpaTunnelEcmpNextHopGroupTableEntryCountGet() * dpaTunnelEcmpNextHopGroupMemberCountGet(),
                               sizeof(ofdbTunnelEcmpNextHopGroupMemberTable_node_t),
                               0x10,
                               (avlComparator_t)ofdbTunnelEcmpNextHopGroupMemberTableNodeCompare,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Tunnel Ecmp Next Hop Group Member Table AVL Tree.\r\n", 0);
    return -1;
  }

  /* Meter Table */
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE,
                     "Allocating %u bytes for OFDB Meter Table Status Data.\r\n",
                     sizeof(ofdbMeterTableStatus_t));

  if (!(ofdbMeterTableStatus = malloc (sizeof(ofdbMeterTableStatus_t))))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to allocate memory for OFDB Meter Table Status Data.\r\n", 0);
    return -1;
  }

  memset(ofdbMeterTableStatus, 0, sizeof(ofdbMeterTableStatus_t));

  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Meter Table AVL Tree.\r\n", 0);

  ofdbMeterTableStatus->meterTableMaxCount = dpaMeterTableEntryCountGet();

	if (ofdbMeterTableStatus->meterTableMaxCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbMeterTable_tree,
								   ofdbMeterTableStatus->meterTableMaxCount,
								   sizeof(ofdbMeterTable_node_t),
								   0x10,
								   avlCompareULong32,
								   0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Meter Table AVL Tree.\r\n", 0);
			return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaMeterTableEntryCountGet.\r\n", 0);
		//return -1;
	}
	
  if (!(ofdbClassBasedCounterTableStatus = calloc(1, sizeof(*ofdbClassBasedCounterTableStatus))))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to allocate memory for OFDB Class Based Counter Table Status Data.\r\n", 0);
    return -1;
  }

  ofdbClassBasedCounterTableStatus->tableMaxCount = dpaClassBasedCounterTableEntryCountGet();

  if (0 != ofdbClassBasedCounterTableStatus->tableMaxCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Class Based Counter Table AVL Tree.\r\n", 0);

    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbClassBasedCounterTable_tree,
                                 ofdbClassBasedCounterTableStatus->tableMaxCount,
                                 sizeof(ofdbClassBasedCounterTable_node_t),
                                 0x10,
                                 avlCompareULong32,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Class Based Counter Table AVL Tree.\r\n", 0);
      return -1;
    }
  }

  if (!(ofdbColorBasedCounterTableStatus = calloc(1, sizeof(*ofdbColorBasedCounterTableStatus))))
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to allocate memory for OFDB Color Based Counter Table Status Data.\r\n", 0);
    return -1;
  }

  ofdbColorBasedCounterTableStatus->tableMaxCount = dpaColorBasedCounterTableEntryCountGet();

  if (0 != ofdbColorBasedCounterTableStatus->tableMaxCount)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Color Based Counter Table AVL Tree.\r\n", 0);

    if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbColorBasedCounterTable_tree,
                                 ofdbColorBasedCounterTableStatus->tableMaxCount,
                                 sizeof(ofdbColorBasedCounterTable_node_t),
                                 0x10,
                                 avlCompareULong32,
                                 0) != 0)
    {
      OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Color Based Counter Table AVL Tree.\r\n", 0);
      return -1;
    }
  }
  
  /* OAM Tables */
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB OAM MEG Table AVL Tree.\r\n", 0);

  entryCount = dpaOamMegTableEntryCountGet();

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbOamMegTable_tree,
		                             entryCount,
		                             sizeof(ofdbOamMegTable_node_t),
		                             0x10,
		                             (avlComparator_t)ofdbOamMegTableNodeCompare,
		                             0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB OAM MEG Table AVL Tree.\r\n", 0);
		  return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaOamMegTableEntryCountGet.\r\n", 0);
		//return -1;
	}

	
	entryCount = dpaOamMepTableEntryCountGet();

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbOamMepTable_tree,
									 entryCount,
									 sizeof(ofdbOamMepTable_node_t),
									 0x10,
									 (avlComparator_t)ofdbOamMepTableNodeCompare,
									 0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB OAM MEP Table AVL Tree.\r\n", 0);
		  return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaOamMepTableEntryCountGet.\r\n", 0);

		//return -1;
	}

	
	entryCount = dpaOamMipTableEntryCountGet();

	if (entryCount > 0)
	{
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbOamMipTable_tree,
                               entryCount,
                               sizeof(ofdbOamMipTable_node_t),
                               0x10,
                               (avlComparator_t)ofdbOamMipTableNodeCompare,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB OAM MIP Table AVL Tree.\r\n", 0);
    return -1;
  }
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaOamMipTableEntryCountGet.\r\n", 0);

		//return -1;
	}

	
	entryCount = dpaOamRemoteMpTableEntryCountGet();

	if (entryCount > 0)
	{
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbOamRemoteMpTable_tree,
                               entryCount,
                               sizeof(ofdbOamRemoteMpTable_node_t),
                               0x10,
                               (avlComparator_t)ofdbOamRemoteMpTableNodeCompare,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB OAM Remote MP Table AVL Tree.\r\n", 0);
    return -1;
  }
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaOamRemoteMpTableEntryCountGet.\r\n", 0);

		//return -1;
	}
		
	entryCount = dpaOamMlpGroupTableEntryCountGet();

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbOamMLPGroupTable_tree,
		                             entryCount,
		                             sizeof(ofdbOamMlpGroupTable_node_t),
		                             0x10,
		                             avlCompareULong32,
		                             0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB OAM MLP Group Table AVL Tree.\r\n", 0);
		  return -1;
		}
	}
	else		
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaOamMlpGroupTableEntryCountGet.\r\n", 0);

		//return -1;
	}

	
	entryCount = dpaOamProtectionEntryCountGet();

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbOamMLPGroupMepTable_tree,
		                             entryCount,
		                             sizeof(ofdbOamMLPGroupMepList_node_t),
		                             0x10,
		                             (avlComparator_t)ofdbOamMLPGroupMepListNodeCompare,
		                             0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB OAM MLP Group MEP Table AVL Tree.\r\n", 0);
		  return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaOamProtectionEntryCountGet.\r\n", 0);

		//return -1;
	}

  /* Drop Status Table */
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Drop Status Table AVL Tree.\r\n", 0);
  entryCount = dpaDropStatusTableEntryCountGet();

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbDropStatusTable_tree,
		                             entryCount,
		                             sizeof(ofdbDropStatusTable_node_t),
		                             0x10,
		                             (avlComparator_t)ofdbDropStatusTableNodeCompare,
		                             0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Drop Status Table AVL Tree.\r\n", 0);
		  return -1;
		}
	}
	else
	{
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaDropStatusTableEntryCountGet.\r\n", 0);

	  //return -1;
	}
  /* OAM Data Plane Counter Table */
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Data Plane Counter Table AVL Tree.\r\n", 0);
  entryCount = dpaDataPlaneCounterTableEntryCountGet();

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbOamDataCounterTable_tree,
		                             entryCount,
		                             sizeof(ofdbOamDataCounterTable_node_t),
		                             0x10,
		                             (avlComparator_t)ofdbOamDataCounterTableNodeCompare,
		                             0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Data Plane Counter Table AVL Tree.\r\n", 0);
		  return -1;
		}
	}
	else		
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaDataPlaneCounterTableEntryCountGet.\r\n", 0);

		//return -1;
	}
	
  if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbInjectedOamLmepIdDataMappingTable_tree,
                               1000,
                               sizeof(ofdbInjectedOamLmepIdDataMappingTable_node_t),
                               0x10,
                               avlCompareULong32,
                               0) != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Injected OAM LMEP ID to Encap Header Data Table AVL Tree.\r\n", 0);
    return -1;
  }

  /* Remark Action Table */
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_VERBOSE, "Creating OFDB Remark Action Table AVL Tree.\r\n", 0);
  entryCount = dpaRemarkActionTableEntryCountGet();

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbRemarkActionTable_tree,
		                           entryCount,
		                           sizeof(ofdbRemarkActionTable_node_t),
		                           0x10,
		                           (avlComparator_t)ofdbRemarkActionTableNodeCompare,
		                           0) != 0)
		{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB Remark Action Table AVL Tree.\r\n", 0);
		return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaRemarkActionTableEntryCountGet.\r\n", 0);
		
		//return -1;
	}
		
#if 1

  entryCount = dpaBfdTableEntryCountGet();
  
	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbBfdTable_tree,
								   entryCount,
								   sizeof(ofdbBfdTable_node_t),
								   0x10,
								   (avlComparator_t)ofdbBfdTableNodeCompare,
								   0) != 0)
		{
			OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create ofdbBfdTable_tree.\r\n", 0);
			return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaBfdTableEntryCountGet is 0.\r\n", 0);
	
		//return -1;
	}

  

  /**/
  entryCount = dpaPwRedunTableEntryCountGet();
  
	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbPwRedunTable_tree,
									entryCount,
									sizeof(ofdbPwRedunTable_node_t),
									0x10,
									avlCompareULong32,
									0) != 0)
		{
		 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create OFDB ofdbPwRedunTable_tree.\r\n", 0);
		 return -1;
		}
	}
	else
	{
		 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaPwRedunTableEntryCountGet is 0.\r\n", 0);
	
	 	//return -1;
	}
		
	entryCount = dpaBfdProtectionEntryCountGet();

	if (entryCount > 0)
	{
   if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbPwRedunMepTable_tree,
								entryCount,
								sizeof(ofdbPwRedunMepList_node_t),
								0x10,
								(avlComparator_t)ofdbPwRedunMepListNodeCompare,
								0) != 0)
   {
	 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create ofdbPwRedunMepTable_tree.\r\n", 0);
	 return -1;
   }
	}
	else
	{
		 OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaBfdProtectionEntryCountGet is 0.\r\n", 0);
	
	 	//return -1;
	}
		
	entryCount = dpaY1564TableEntryCountGet();

	if (entryCount > 0)
	{
	if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbY1564Table_tree,
								 entryCount,
								 sizeof(ofdbY1564Table_node_t),
								 0x10,
								 (avlComparator_t)ofdbY1564TableNodeCompare,
								 0) != 0)
	{
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create ofdbBfdTable_tree.\r\n", 0);
	  return -1;
	}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaBfdTableEntryCountGet is 0.\r\n", 0);
	
	  //return -1;
	}
		
	entryCount = dpaLoopbackTableEntryCountGet();

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbLbTable_tree,
									 entryCount,
									 sizeof(ofdbLbTable_node_t),
									 0x10,
									 (avlComparator_t)ofdbLoopbackTableNodeCompare,
									 0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create ofdbBfdTable_tree.\r\n", 0);
		  return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaBfdTableEntryCountGet is 0.\r\n", 0);

		//return -1;
	}
		
	entryCount = 2;

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbVlanPortTable_tree,
									 entryCount,
									 sizeof(ofdbVlanPortTable_node_t),
									 0x10,
									 (avlComparator_t)ofdbVlanPortTableNodeCompare,
									 0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create ofdbBfdTable_tree.\r\n", 0);
		  return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "platformNumPortsGet is 0.\r\n", 0);

		//return -1;
	}
		

	entryCount = 4094;

	if (entryCount > 0)
	{
		if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbVlanIpTable_tree,
									 entryCount,
									 sizeof(ofdbVlanIpTable_node_t),
									 0x10,
									 (avlComparator_t)ofdbVlanIpTableNodeCompare,
									 0) != 0)
		{
		  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create ofdbBfdTable_tree.\r\n", 0);
		  return -1;
		}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "platformNumPortsGet is 0.\r\n", 0);

		//return -1;
	}
		

	entryCount = dpaBfdTableEntryCountGet();
	entryCount += dpaOamMepTableEntryCountGet();

	if (entryCount > 0)
	{
	if (avlAllocAndCreateAvlTree(&ofdbAVLTrees.ofdbLmepidTable_tree,
								 entryCount,
								 sizeof(ofdbLmepidTable_node_t),
								 0x10,
								 (avlComparator_t)ofdbLmepidTableNodeCompare,
								 0) != 0)
	{
	  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to create ofdbLmepidTable_tree.\r\n", 0);
	  return -1;
	}
	}
	else
	{
		OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "dpaBfdTableEntryCountGet is 0.\r\n", 0);

		//return -1;
	}
	
#endif
  

  /* Initialize Port Table */
  if (dpaPlatformPhysicalPortTableInit() != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to initialize port table\r\n", 0);
    return -1;
  }

  
  return(0);
}

/*********************************************************************
* @purpose  Take the OFDB read-write lock for a read operation.
*
* @param   file - File that got the lock.
* @param   line_num - Line number that got the lock.
*
* returns   nothing
*
* @comments
*
* @end
*
*********************************************************************/
void ofdbReadLockTake(char *file, uint32_t line_num)
{
  OFDPA_ERROR_t rc;

  rc = dpaRWlockReadlock(&ofdbLock);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to take read lock. Return code = %d, file %s line %d\r\n", rc, file, line_num);
  }
}

/*********************************************************************
* @purpose  Take the OFDB read-write lock for a write operation.
*
* @param   file - File that got the lock.
* @param   line_num - Line number that got the lock.
*
* returns   nothing
*
* @comments
*
* @end
*
*********************************************************************/
void ofdbWriteLockTake(char *file, uint32_t line_num)
{
  OFDPA_ERROR_t rc;

  rc = dpaRWlockWritelock(&ofdbLock);
  if (rc != OFDPA_E_NONE)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to take read lock. Return code = %d\r\n", rc);
  }
}

/*********************************************************************
* @purpose  Release the OFDB read-write lock.
*
* @param   None
*
* returns   nothing
*
* @comments
*
* @end
*
*********************************************************************/
void ofdbLockGive(void)
{
  int rc;

  rc = dpaRWlockUnlock(&ofdbLock);

  if (rc != 0)
  {
    OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_OFDB, OFDPA_DEBUG_ALWAYS, "Failed to free semaphore. Return code = %d\r\n", rc);
  }
}

/*********************************************************************
* @purpose  Tests whether a MAC address is a multicast address by checking
*           if the first three octets are 01-00-5e.
*
* @param    mac pointer to MAC address structure
*
* returns   1 - the first three octets in the address are 01-00-5e and the
*               fourth octet has the highest order bit equal to 0
*           0 - otherwise
*
* @comments
*
* @end
*
*********************************************************************/
int ofdbMacIsIpv4Multicast(ofdpaMacAddr_t *mac)
{
  int result = 0;

  if ((mac != NULL) &&
      (mac->addr[0] == 0x01) &&
      (mac->addr[1] == 0x00) &&
      (mac->addr[2] == 0x5e) &&
      ((mac->addr[3] & 0x80) == 0))
  {
    result = 1;
  }
  return(result);
}

/*********************************************************************
* @purpose  Tests whether a MAC address is a multicast address by checking
*           if the first two octets are 33-33.
*
* @param    mac pointer to MAC address structure
*
* returns   1 - the first three octets in the MAC address are 33-33
*           0 - otherwise
*
* @comments
*
* @end
*
*********************************************************************/
int ofdbMacIsIpv6Multicast(ofdpaMacAddr_t *mac)
{
  int result = 0;

  if ((mac != NULL) &&
      (mac->addr[0] == 0x33) &&
      (mac->addr[1] == 0x33))
  {
    result = 1;
  }
  return(result);
}

OFDPA_ERROR_t ofdbSrcMacLearnModeCfgDataSet(uint32_t mode, ofdpaSrcMacLearnModeCfg_t *cfgData)
{
  ofdbGlobalData->srcMaclearnMode = mode;
  memcpy(&(ofdbGlobalData->srcMacLearnCfg), cfgData, sizeof(ofdbSrcMacLearnCfg_t));

  return OFDPA_E_NONE;


}

OFDPA_ERROR_t ofdbSrcMacLearnModeCfgDataGet(uint32_t *mode, ofdpaSrcMacLearnModeCfg_t *cfgData)
{
  if (!mode || !cfgData)
  {
    return OFDPA_E_PARAM;
  }

  *mode = ofdbGlobalData->srcMaclearnMode;
  memcpy(cfgData, &(ofdbGlobalData->srcMacLearnCfg), sizeof(ofdpaSrcMacLearnModeCfg_t));

  return OFDPA_E_NONE;
}
#if 0
void ofdbDebugOamBhhPrimVlanMapHeaderDataTableDump(void)
{
  uint32_t  primVid;
  ofdbOamPrimVlanToMplsBhhDataMappingTable_node_t    *dataPtr;
  ofdbOamBhhData_t *bhhData;
  char macStringBuffer[15];

  primVid = 0;

  printf(" primVid   port popV destination MAC   source MAC    VLAN TunLb2 TunLb1 GAL    liveness   meglvl\r\n");
  printf("---------- ---- ---- --------------- --------------- ---- ------ ------ ------ ---------- ------\r\n");

  while ((dataPtr = avlSearch(&ofdbAVLTrees.ofdbOamPrimVlanToBhhDataMappingTable_tree, &primVid, AVL_NEXT)) != NULL)
  {
    bhhData = &dataPtr->bhhData;

    printf("%10d %4d %s %s  %s  %4d 0x%04x 0x%04x 0x%04x 0x%08x %d\r\n",
           dataPtr->primVid,
           bhhData->ifNum,
           bhhData->popVlanTag ? "Yes ":"No  ",
           ofdpaMacAddrToString(&bhhData->destMac, macStringBuffer),
           ofdpaMacAddrToString(&bhhData->srcMac, macStringBuffer),
           bhhData->vlanId,
           bhhData->mplsTunnel2Label,
           bhhData->mplsTunnel1Label,
           bhhData->mplsGAL,
           bhhData->livenessLogicalPortId,
           bhhData->megLevel);

    primVid = dataPtr->primVid;
  }
}
#endif
