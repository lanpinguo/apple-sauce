/*********************************************************************
 *
 * (C) Copyright Broadcom Corporation 2003-2016
 *
 **********************************************************************
 *
 * @filename     ofdb.h
 *
 * @purpose      Openflow Database Manager (OFDB) header
 *
 * @component    OFDB
 *
 * @comments     none
 *
 * @create       05/23/2010
 *
 * @end
 *
 **********************************************************************/

#ifndef INCLUDE_OFDB_H
#define INCLUDE_OFDB_H

#include <stdint.h>
#include "avl_api.h"

#include "ofdb_api.h"

/*  Flow Table Status. This information is aggregated for all flows in the flow table.
*/
typedef struct ofdbFlowTableStatus_s
{
  /* Maximum flows in each flow table  */
  uint32_t ingressPortMaxCount;
  uint32_t portDscpTrustMaxCount;
  uint32_t portPcpTrustMaxCount;
  uint32_t tunnelDscpTrustMaxCount;
  uint32_t tunnelPcpTrustMaxCount;
  uint32_t injectedOamMaxCount;
  uint32_t vlanMaxCount;
  uint32_t vlan1MaxCount;
  uint32_t mpMaxCount;
  uint32_t mplsL2PortMaxCount;
  uint32_t mplsDscpTrustMaxCount;
  uint32_t mplsPcpTrustMaxCount;
  uint32_t mplsQosClassMaxCount;
  uint32_t l2PolicerMaxCount;
  uint32_t l2PolicerActionsMaxCount;
  uint32_t terminationMacMaxCount;
  uint32_t mpls0MaxCount;
  uint32_t mpls1MaxCount;
  uint32_t mplsMpMaxCount;
  uint32_t mplsLabelTrustMaxCount;
  uint32_t unicastRoutingMaxCount;
  uint32_t multicastRoutingMaxCount;
  uint32_t bridgingMaxCount;
  uint32_t policyAclMaxCount;
  uint32_t colorActionsMaxCount;
  uint32_t egressVlanMaxCount;
  uint32_t egressVlan1MaxCount;
  uint32_t egressMpMaxCount;
  uint32_t egressDscpPcpRemarkMaxCount;
  uint32_t egressTpidMaxCount;

  /* Identifier assigned to the last added flow.  */
  uint64_t last_flow_id;

} ofdbFlowTableStatus_t;

extern ofdbGlobalData_t               *ofdbGlobalData;
extern ofdbFlowTableStatus_t          *ofdbFlowTableStatus;
extern ofdbGroupTableStatus_t         *ofdbGroupTableStatus;
extern ofdbGroupBucketTableStatus_t   *ofdbGroupBucketTableStatus;
extern ofdbMeterTableStatus_t         *ofdbMeterTableStatus;
extern ofdbClassBasedCounterTableStatus_t *ofdbClassBasedCounterTableStatus;
extern ofdbColorBasedCounterTableStatus_t *ofdbColorBasedCounterTableStatus;

/* AVL Tree and AVL Nodes definitions.
*/
typedef struct ofdbAVLTrees_s
{
  avlTree_t ofdbIngressPortFlowTable_tree;
  avlTree_t ofdbPortDscpTrustFlowTable_tree;
  avlTree_t ofdbPortPcpTrustFlowTable_tree;
  avlTree_t ofdbTunnelDscpTrustFlowTable_tree;
  avlTree_t ofdbTunnelPcpTrustFlowTable_tree;
  avlTree_t ofdbInjectedOamFlowTable_tree;
  avlTree_t ofdbVlanFlowTable_tree;
  avlTree_t ofdbVlan1FlowTable_tree;
  avlTree_t ofdbMpFlowTable_tree;
  avlTree_t ofdbMplsL2PortFlowTable_tree;
  avlTree_t ofdbMplsDscpTrustFlowTable_tree;
  avlTree_t ofdbMplsPcpTrustFlowTable_tree;
  avlTree_t ofdbMplsQosClassFlowTable_tree;
  avlTree_t ofdbL2PolicerFlowTable_tree;
  avlTree_t ofdbL2PolicerActionsFlowTable_tree;
  avlTree_t ofdbTerminationMacFlowTable_tree;
  avlTree_t ofdbMpls0FlowTable_tree;
  avlTree_t ofdbMpls1FlowTable_tree;
  avlTree_t ofdbMplsMpFlowTable_tree;
  avlTree_t ofdbMplsLabelTrustFlowTable_tree;
  avlTree_t ofdbUnicastRoutingFlowTable_tree;
  avlTree_t ofdbMulticastRoutingFlowTable_tree;
  avlTree_t ofdbBridgingFlowTable_tree;
  avlTree_t ofdbPolicyAclFlowTable_tree;
  avlTree_t ofdbColorActionsFlowTable_tree;
  avlTree_t ofdbEgressVlanFlowTable_tree;
  avlTree_t ofdbEgressVlan1FlowTable_tree;
  avlTree_t ofdbEgressMpFlowTable_tree;
  avlTree_t ofdbEgressDscpPcpRemarkFlowTable_tree;
  avlTree_t ofdbEgressTpidFlowTable_tree;
  avlTree_t ofdbFlowCookieTable_tree;
  avlTree_t ofdbFlowFlowIdTable_tree;
  avlTree_t ofdbPortTable_tree;
  avlTree_t ofdbPortTenantTable_tree;
  avlTree_t ofdbTunnelTenantTable_tree;
  avlTree_t ofdbTunnelNextHopTable_tree;
  avlTree_t ofdbTunnelEcmpNextHopGroupTable_tree;
  avlTree_t ofdbTunnelEcmpNextHopGroupMemberTable_tree;
  avlTree_t ofdbGroupTable_tree;
  avlTree_t ofdbGroupBucketRefTable_tree;
  avlTree_t ofdbGroupBucketDataTable_tree;
  avlTree_t ofdbMeterTable_tree;
  avlTree_t ofdbClassBasedCounterTable_tree;
  avlTree_t ofdbColorBasedCounterTable_tree;
  avlTree_t ofdbOamMegTable_tree;
  avlTree_t ofdbOamMepTable_tree;
  avlTree_t ofdbOamMipTable_tree;
  avlTree_t ofdbOamRemoteMpTable_tree;
  avlTree_t ofdbOamMLPGroupTable_tree;
  avlTree_t ofdbOamMLPGroupMepTable_tree;
  avlTree_t ofdbDropStatusTable_tree;
  avlTree_t ofdbOamDataCounterTable_tree;
  avlTree_t ofdbRemarkActionTable_tree;
  avlTree_t ofdbInjectedOamLmepIdDataMappingTable_tree;

/* BEGIN: Added by Hushouqiang, 2016/3/22   问题单号:P10002 */
	avlTree_t ofdbCosQueues_tree;
/* END:   Added by Hushouqiang, 2016/3/22 */
  avlTree_t ofdbBfdTable_tree;		/*leishenghua add 20161031*/
  avlTree_t ofdbPwRedunTable_tree;	
  avlTree_t ofdbPwRedunMepTable_tree;  
  avlTree_t ofdbY1564Table_tree;		/*leishenghua add 20170302*/
  avlTree_t ofdbLbTable_tree;		/*leishenghua add 20170306*/
  avlTree_t ofdbVlanPortTable_tree;		/*leishenghua add 20170504*/
  avlTree_t ofdbVlanIpTable_tree;		/*leishenghua add 20170505*/
  avlTree_t ofdbLmepidTable_tree;		/*用于暂存lmepid对应的oam类型，不删除*/
  
} ofdbAVLTrees_t;

extern ofdbAVLTrees_t ofdbAVLTrees;

/* AVL Node definitions.
*/

/* OpenFlow Flow AVL Tree.
*/
typedef struct
{
  uint32_t     inPort;                /* OpenFlow Ingress Port number */
  uint32_t     tunnelId;              /* tunnel ID value */
  uint16_t     etherType;             /* Ethertype value */
  //added by yjg begin ingressPort流表增加lmepId作为key
  uint32_t     lmepId;             /* lmep ID value */
  //added by yjg end

} ofdbIngressPortFlowEntryKey_t;

typedef struct
{
  ofdbIngressPortFlowEntryKey_t  key;   /* Key into the table. */
  ofdpaIngressPortFlowEntry_t    ingressPortFlowEntry; /* config unique to flow type */
  uint32_t                       priority; /* config common to all flow types */
  uint32_t                       hard_time;
  uint32_t                       idle_time;
  uint64_t                       cookie;
  ofdbFlowStatus_t               status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t              flags;
  void                          *next;  /* This field must be the last element in the AVL Tree Node */
} ofdbIngressPortFlowTable_node_t;

/* QOS Trust - Port DSCP, tunnel DSCP */
typedef struct
{
  uint8_t qosIndex;
  uint8_t dscpValue;
} ofdbDscpTrustFlowEntryKey_t;

typedef struct
{
  ofdbDscpTrustFlowEntryKey_t  key;     /* Key into the table */
  ofdpaDscpTrustFlowEntry_t    dscpTrustFlowEntry; /* config unique to flow type */
  uint32_t                     priority; /* config common to all flow types */
  uint32_t                     hard_time;
  uint32_t                     idle_time;
  uint64_t                     cookie;
  ofdbFlowStatus_t             status;  /* Flow Status information */
  OFDB_ENTRY_FLAG_t            flags;
  void                        *next;    /* This field must be the last element in the AVL Tree Node */
} ofdbDscpTrustFlowTable_node_t;

typedef struct
{
  uint8_t qosIndex;
  uint8_t pcpValue;
  uint8_t dei;
} ofdbPcpTrustFlowEntryKey_t;

/* QOS Trust - Port PCP, tunnel PCP */
typedef struct
{
  ofdbPcpTrustFlowEntryKey_t  key;      /* Key into the table */
  ofdpaPcpTrustFlowEntry_t    pcpTrustFlowEntry; /* config unique to flow type */
  uint32_t                    priority; /* config common to all flow types */
  uint32_t                    hard_time;
  uint32_t                    idle_time;
  uint64_t                    cookie;
  ofdbFlowStatus_t            status;   /* Flow Status information */
  OFDB_ENTRY_FLAG_t           flags;
  void                       *next;     /* This field must be the last element in the AVL Tree Node */
} ofdbPcpTrustFlowTable_node_t;

typedef struct
{
  uint32_t     lmepId;

} ofdbInjectedOamFlowEntryKey_t;

typedef struct
{
  ofdbInjectedOamFlowEntryKey_t       key;    /* Key into the table. */

  ofdpaInjectedOamFlowEntry_t   injectedOamFlowEntry; /* config unique to flow type */

  uint32_t                      priority;     /* config common to all flow types */
  uint32_t                      hard_time;
  uint32_t                      idle_time;
  uint64_t                      cookie;

  ofdbFlowStatus_t              status; /* Flow Status information */

  OFDB_ENTRY_FLAG_t flags;

  void *next;   /* This field must be the last element in the AVL Tree Node */
} ofdbInjectedOamFlowTable_node_t;

typedef struct
{
  uint32_t     inPort;                /* OpenFlow Ingress Port number */
  uint16_t     vlanId;
} ofdbVlanFlowEntryKey_t;

typedef struct
{
  ofdbVlanFlowEntryKey_t  key;          /* Key into the table. */
  ofdpaVlanFlowEntry_t    vlanFlowEntry; /* config unique to flow type */
  uint32_t                priority;     /* config common to all flow types */
  uint32_t                hard_time;
  uint32_t                idle_time;
  uint64_t                cookie;
  ofdbFlowStatus_t        status;       /* Flow Status information */
  OFDB_ENTRY_FLAG_t       flags;
  void                   *next;         /* This field must be the last element in the AVL Tree Node */
} ofdbVlanFlowTable_node_t;

typedef struct
{
  uint32_t       inPort;                /* OpenFlow Ingress Port number */
  uint16_t       vlanId;
  uint16_t       ovid;
} ofdbVlan1FlowEntryKey_t;

typedef struct
{
  ofdbVlan1FlowEntryKey_t  key;         /* Key into the table. */
  ofdpaVlan1FlowEntry_t    vlan1FlowEntry; /* config unique to flow type */
  uint32_t                 priority;    /* config common to all flow types */
  uint32_t                 hard_time;
  uint32_t                 idle_time;
  uint64_t                 cookie;
  ofdbFlowStatus_t         status;      /* Flow Status information */
  OFDB_ENTRY_FLAG_t        flags;
  void                    *next;        /* This field must be the last element in the AVL Tree Node */
} ofdbVlan1FlowTable_node_t;

typedef struct
{
  uint32_t       inPort;
  uint16_t       vlanId;
  uint16_t       etherType;
  uint8_t        oamY1731Mdl;
  uint8_t        oamY1731Opcode;
  ofdpaMacAddr_t destMac;

} ofdbMpFlowEntryKey_t;

typedef struct
{
  ofdbMpFlowEntryKey_t  key;            /* Key into the table. */
  ofdpaMpFlowEntry_t    mpFlowEntry;    /* config unique to flow type */
  uint32_t              priority;       /* config common to all flow types */
  uint32_t              hard_time;
  uint32_t              idle_time;
  uint64_t              cookie;
  ofdbFlowStatus_t      status;         /* Flow Status information */
  OFDB_ENTRY_FLAG_t     flags;
  void                 *next;           /* This field must be the last element in the AVL Tree Node */
} ofdbMpFlowTable_node_t;

typedef struct
{
  uint32_t mplsL2Port;
  uint32_t tunnelId;
  uint16_t etherType;

} ofdbMplsL2PortFlowEntryKey_t;

typedef struct
{
  ofdbMplsL2PortFlowEntryKey_t  key;    /* Key into the table */
  ofdpaMplsL2PortFlowEntry_t    mplsL2PortFlowEntry; /* config unique to flow type */
  uint32_t                      priority; /* config common to all flow types */
  uint32_t                      hard_time;
  uint32_t                      idle_time;
  uint64_t                      cookie;
  ofdbFlowStatus_t              status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t             flags;
  void                         *next;   /* This field must be the last element in the AVL Tree Node */
} ofdbMplsL2PortFlowTable_node_t;

/* MPLS L2 Port QoS Class */
typedef struct
{
  uint32_t mplsL2Port;
  uint32_t tunnelId;
  uint16_t etherType;
  ofdpaMacAddr_t srcMac;
  ofdpaMacAddr_t destMac;
  uint16_t vlanPcp;
  uint8_t vlanDei;
  in_addr_t sourceIp4;
  in_addr_t destIp4;
  struct in6_addr sourceIp6;
  struct in6_addr destIp6;
  uint16_t ipProto;
  uint16_t dscp;
  uint32_t srcL4Port;
  uint32_t destL4Port;
} ofdbMplsQosClassFlowEntryKey_t;

typedef struct
{
  ofdbMplsQosClassFlowEntryKey_t  key;  /* Key into the table */
  ofdpaMplsL2PortQosClassFlowEntry_t   mplsQosClassFlowEntry; /* config unique to flow type */
  uint32_t                        priority; /* config common to all flow types */
  uint32_t                        hard_time;
  uint32_t                        idle_time;
  uint64_t                        cookie;
  ofdbFlowStatus_t                status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t               flags;
  void                           *next; /* This field must be the last element in the AVL Tree Node */
} ofdbMplsQosClassFlowTable_node_t;

typedef struct
{
  uint32_t       tunnelId;
  uint32_t       mplsL2Port;
} ofdbL2PolicerFlowEntryKey_t;

typedef struct
{
  ofdbL2PolicerFlowEntryKey_t  key;     /* Key into the table. */
  ofdpaL2PolicerFlowEntry_t    l2PolicerFlowEntry; /* config unique to flow type */
  uint32_t                     priority; /* config common to all flow types */
  uint32_t                     hard_time;
  uint32_t                     idle_time;
  uint64_t                     cookie;
  ofdbFlowStatus_t             status;  /* Flow Status information */
  OFDB_ENTRY_FLAG_t            flags;
  void                        *next;    /* This field must be the last element in the AVL Tree Node */
} ofdbL2PolicerFlowTable_node_t;

typedef struct
{
  OFDPA_QOS_COLORS_t color;
  uint32_t           colorActionsIndex;
} ofdbL2PolicerActionsFlowEntryKey_t;

typedef struct
{
  ofdbL2PolicerActionsFlowEntryKey_t  key; /* Key into the table. */
  ofdpaL2PolicerActionsFlowEntry_t    l2PolicerActionsFlowEntry; /* config unique to flow type */
  uint32_t                            priority; /* config common to all flow types */
  uint32_t                            hard_time;
  uint32_t                            idle_time;
  uint64_t                            cookie;
  ofdbFlowStatus_t                    status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t                   flags;
  void                               *next; /* This field must be the last element in the AVL Tree Node */
} ofdbL2PolicerActionsFlowTable_node_t;

typedef struct
{
  uint32_t       inPort;                /* OpenFlow Ingress Port number */
  uint16_t       vlanId;
  uint16_t       etherType;
  ofdpaMacAddr_t destMac;
} ofdbTerminationMacFlowEntryKey_t;

typedef struct
{
  ofdbTerminationMacFlowEntryKey_t  key; /* Key into the table. */
  ofdpaTerminationMacFlowEntry_t    terminationMacFlowEntry; /* config unique to flow type */
  uint32_t                          priority; /* config common to all flow types */
  uint32_t                          hard_time;
  uint32_t                          idle_time;
  uint64_t                          cookie;
  ofdbFlowStatus_t                  status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t                 flags;
  void                             *next; /* This field must be the last element in the AVL Tree Node */
} ofdbTerminationMacFlowTable_node_t;

typedef struct
{
  /* First nibble is what distinguishes OAM flows from data flows.  Otherwise, there can
   * only be one entry per label. */
  uint32_t        mplsLabel;
  uint8_t         mplsDataFirstNibble;  
  uint16_t        mplsAchChannel;
#if 0
  uint8_t         nextLabelIsGal;
  uint16_t        etherType;
  uint16_t        mplsBos;
  uint32_t        inPort;
  uint8_t         mplsTtl;
  in_addr_t       destIp4;
  struct in6_addr destIp6;
  uint8_t         ipProto;
  uint16_t        udpSrcPort;
  uint16_t        udpDstPort;
#endif
} ofdbMplsFlowEntryKey_t;

typedef struct
{
  ofdbMplsFlowEntryKey_t  key;          /* Key into the table. */
  ofdpaMplsFlowEntry_t    mplsFlowEntry; /* config unique to flow type */
  OFDPA_FLOW_TABLE_ID_t   tableId;
  uint32_t                priority;     /* config common to all flow types */
  uint32_t                hard_time;
  uint64_t                cookie;
  ofdbFlowStatus_t        status;       /* Flow Status information */
  OFDB_ENTRY_FLAG_t       flags;
  void                   *next;         /* This field must be the last element in the AVL Tree Node */
} ofdbMplsFlowTable_node_t;

typedef struct
{
  uint32_t     lmepId;                /* Local identifier for the MEP or MIP */
  uint8_t      oamY1731Opcode;        /* Parsed from the IEEE 802.1ag/Y.1731 header. */
} ofdbMplsMpFlowEntryKey_t;

typedef struct
{
  ofdbMplsMpFlowEntryKey_t  key;        /* Key into the table. */
  ofdpaMplsMpFlowEntry_t    mplsMpFlowEntry; /* config unique to flow type */
  uint32_t                  priority;   /* config common to all flow types */
  uint32_t                  hard_time;
  uint32_t                  idle_time;
  uint64_t                  cookie;
  ofdbFlowStatus_t          status;     /* Flow Status information */
  OFDB_ENTRY_FLAG_t         flags;
  void                     *next;       /* This field must be the last element in the AVL Tree Node */
} ofdbMplsMpFlowTable_node_t;

typedef struct
{
  uint16_t       vlanId;
  uint32_t       tunnelId;
  ofdpaMacAddr_t destMac;
} ofdbBridgingFlowEntryKey_t;

typedef struct
{
  ofdbBridgingFlowEntryKey_t  key;      /* Key into the table. */
  ofdpaBridgingFlowEntry_t    bridgingFlowEntry; /* config unique to flow type */
  uint32_t                    priority; /* config common to all flow types */
  uint32_t                    hard_time;
  uint32_t                    idle_time;
  uint64_t                    cookie;
  ofdbFlowStatus_t            status;   /* Flow Status information */
  OFDB_ENTRY_FLAG_t           flags;
  void                       *next;     /* This field must be the last element in the AVL Tree Node */
} ofdbBridgingFlowTable_node_t;

typedef struct
{
  uint8_t qosIndex;
  uint8_t mplsTC;

} ofdbMplsLabelTrustFlowEntryKey_t;

typedef struct
{
  ofdbMplsLabelTrustFlowEntryKey_t key;       /* Key into the table */
  ofdpaMplsLabelTrustFlowEntry_t   mplsLabelTrustFlowEntry;  /* config unique to flow type */
  uint32_t                         priority;  /* config common to all flow types */
  uint32_t                         hard_time;
  uint32_t                         idle_time;
  uint64_t                         cookie;
  ofdbFlowStatus_t                 status;   /* Flow Status information */
  OFDB_ENTRY_FLAG_t                flags;
  void                            *next;    /* This field must be the last element in the AVL Tree Node */
} ofdbMplsLabelTrustFlowTable_node_t;

typedef struct
{
  uint16_t         etherType;
  uint16_t         vrf;
  in_addr_t        dstIp4;
  struct in6_addr  dstIp6;
} ofdbUnicastRoutingFlowEntryKey_t;

typedef struct
{
  ofdbUnicastRoutingFlowEntryKey_t  key; /* Key into the table. */
  ofdpaUnicastRoutingFlowEntry_t    unicastRoutingFlowEntry; /* config unique to flow type */
  uint32_t                          priority; /* config common to all flow types */
  uint32_t                          hard_time;
  uint32_t                          idle_time;
  uint64_t                          cookie;
  ofdbFlowStatus_t                  status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t                 flags;
  void                             *next; /* This field must be the last element in the AVL Tree Node */
} ofdbUnicastRoutingFlowTable_node_t;

typedef struct
{
  uint16_t        etherType;
  uint16_t        vrf;
  uint16_t        vlanId;
  in_addr_t       srcIp4;
  in_addr_t       dstIp4;
  struct in6_addr srcIp6;
  struct in6_addr dstIp6;
} ofdbMulticastRoutingFlowEntryKey_t;

typedef struct
{
  ofdbMulticastRoutingFlowEntryKey_t  key; /* Key into the table. */
  ofdpaMulticastRoutingFlowEntry_t    multicastRoutingFlowEntry; /* config unique to flow type */
  uint32_t                            priority; /* config common to all flow types */
  uint32_t                            hard_time;
  uint32_t                            idle_time;
  uint64_t                            cookie;
  ofdbFlowStatus_t                    status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t                   flags;
  void                               *next; /* This field must be the last element in the AVL Tree Node */
} ofdbMulticastRoutingFlowTable_node_t;

typedef struct
{
  uint32_t        inPort;
  uint16_t        etherType;
  uint32_t        mplsL2Port;
  ofdpaMacAddr_t  srcMac;
  ofdpaMacAddr_t  destMac;
  uint16_t        vlanId;
  uint16_t        vlanPcp;
  uint8_t         vlanDei;
  uint32_t        tunnelId;
  uint16_t        vrf;
  in_addr_t       sourceIp4;
  in_addr_t       destIp4;
  struct in6_addr sourceIp6;
  struct in6_addr destIp6;
  uint32_t        ipv4ArpSpa;
  uint16_t        ipProto;
  uint16_t        dscp;
  uint16_t        ecn;
  uint32_t        srcL4Port;
  uint32_t        destL4Port;
  uint8_t         icmpType;
  uint8_t         icmpCode;
  uint32_t        ipv6FlowLabel;
} ofdbPolicyAclFlowEntryKey_t;

typedef struct
{
  ofdbPolicyAclFlowEntryKey_t  key;     /* Key into the table. */
  ofdpaPolicyAclFlowEntry_t    policyAclFlowEntry; /* config unique to flow type */
  uint32_t                     priority; /* config common to all flow types */
  uint32_t                     hard_time;
  uint32_t                     idle_time;
  uint64_t                     cookie;
  ofdbFlowStatus_t             status;  /* Flow Status information */
  OFDB_ENTRY_FLAG_t            flags;
  void                        *next;    /* This field must be the last element in the AVL Tree Node */
} ofdbPolicyAclFlowTable_node_t;

typedef struct
{
  OFDPA_QOS_COLORS_t    color;
  uint32_t              index;

} ofdbColorActionsFlowEntryKey_t;

typedef struct
{
  ofdbColorActionsFlowEntryKey_t  key;  /* Key into the table. */
  ofdpaColorActionsFlowEntry_t    colorActionsFlowEntry; /* color actions entry */
  uint32_t                        priority; /* config common to all flow types */
  uint32_t                        hard_time;
  uint32_t                        idle_time;
  uint64_t                        cookie;
  ofdbFlowStatus_t                status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t               flags;
  void                           *next; /* This field must be the last element in the AVL Tree Node */
} ofdbColorActionsFlowTable_node_t;

typedef struct
{
  uint32_t outPort;                     /* OpenFlow Ingress Port number */
  uint16_t vlanId;
  uint16_t allowVlanTranslation;
  uint32_t tunnelId;
  uint32_t mplsL2Port;
} ofdbEgressVlanFlowEntryKey_t;

typedef struct
{
  ofdbEgressVlanFlowEntryKey_t  key;    /* Key into the table. */
  ofdpaEgressVlanFlowEntry_t    egressVlanFlowEntry; /* config unique to flow type */
  uint32_t                      priority; /* config common to all flow types */
  uint32_t                      hard_time;
  uint32_t                      idle_time;
  uint64_t                      cookie;
  ofdbFlowStatus_t              status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t             flags;
  void                         *next;   /* This field must be the last element in the AVL Tree Node */
} ofdbEgressVlanFlowTable_node_t;

typedef struct
{
  uint32_t       outPort;                /* OpenFlow Egress Port number */
  uint16_t       vlanId;
  uint16_t       ovid;
} ofdbEgressVlan1FlowEntryKey_t;

typedef struct
{
  ofdbEgressVlan1FlowEntryKey_t  key;   /* Key into the table. */
  ofdpaEgressVlan1FlowEntry_t    egressVlan1FlowEntry; /* config unique to flow type */
  uint32_t                       priority; /* config common to all flow types */
  uint32_t                       hard_time;
  uint32_t                       idle_time;
  uint64_t                       cookie;
  ofdbFlowStatus_t               status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t              flags;
  void                          *next;  /* This field must be the last element in the AVL Tree Node */
} ofdbEgressVlan1FlowTable_node_t;

typedef struct
{
  uint32_t       outPort;                /* OpenFlow Egress Port number */
  uint16_t       vlanId;
  uint16_t       etherType;
  uint8_t        oamY1731Mdl;
  uint8_t        oamY1731Opcode;
  ofdpaMacAddr_t destMac;
} ofdbEgressMpFlowEntryKey_t;

typedef struct
{
  ofdbEgressMpFlowEntryKey_t  key;      /* Key into the table. */
  ofdpaEgressMpFlowEntry_t    egressMpFlowEntry; /* config unique to flow type */
  uint32_t                    priority; /* config common to all flow types */
  uint32_t                    hard_time;
  uint32_t                    idle_time;
  uint64_t                    cookie;
  ofdbFlowStatus_t            status;   /* Flow Status information */
  OFDB_ENTRY_FLAG_t           flags;
  void                       *next;     /* This field must be the last element in the AVL Tree Node */
} ofdbEgressMpFlowTable_node_t;

typedef struct
{
  uint32_t              outPort;        /* ACTSET output/egress port */
  uint16_t              etherType;      /* Ethertype value */
  uint8_t               trafficClass;   /* Traffic Class */
  OFDPA_QOS_COLORS_t    color;          /* Color */

} ofdbEgressDscpPcpRemarkFlowEntryKey_t;

typedef struct
{
  ofdbEgressDscpPcpRemarkFlowEntryKey_t key;                         /* key into the table. */
  ofdpaEgressDscpPcpRemarkFlowEntry_t   egressDscpPcpRemarkFlowEntry;/* egress remark entry */
  uint32_t                              priority; /* config common to all flow types */
  uint32_t                              hard_time;
  uint32_t                              idle_time;
  uint64_t                              cookie;
  ofdbFlowStatus_t                      status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t                     flags;
  void                                  *next; /* This field must be the last element in the AVL Tree Node */
} ofdbEgressDscpPcpRemarkFlowTable_node_t;

typedef struct
{
  uint32_t       outPort;                /* OpenFlow Egress Port number */
} ofdbEgressTpidFlowEntryKey_t;

typedef struct
{
  ofdbEgressTpidFlowEntryKey_t   key;   /* Key into the table. */
  ofdpaEgressTpidFlowEntry_t     egressTpidFlowEntry; /* config unique to flow type */
  uint32_t                       priority; /* config common to all flow types */
  uint32_t                       hard_time;
  uint32_t                       idle_time;
  uint64_t                       cookie;
  ofdbFlowStatus_t               status; /* Flow Status information */
  OFDB_ENTRY_FLAG_t              flags;
  void                          *next;  /* This field must be the last element in the AVL Tree Node */
} ofdbEgressTpidFlowTable_node_t;

typedef struct
{
  uint64_t     index;    /* Key into the table. */
  OFDPA_FLOW_TABLE_ID_t  tableId;
  void                  *flowEntryNode;
  void                  *next;          /* This field must be the last element in the AVL Tree Node */
} ofdbFlowIndexTable_node_t;

/************** GROUP TABLE **************/

typedef struct ofdbGroupEntryStats_s
{
  uint32_t                refCount;
  uint64_t                packet_count;
  uint64_t                byte_count;
  uint32_t                groupAddUpTime;
  uint32_t                bucketCount;
} ofdbGroupEntryStats_t;

typedef struct ofdbGroupTable_node_s
{
  ofdpaGroupEntry_t       group;        /* Group configuration information */
  ofdbGroupEntryStats_t   stats;        /* Group Stats information */
  void                   *next;         /* This field must be the last element in the AVL Tree Node */
} ofdbGroupTable_node_t;

/************** GROUP BUCKET TABLE **************/

/** Group Bucket Reference Table Entry */
/* Used for Buckets that use only a Reference Group Id
   and do not have any other data to store */

typedef struct ofdbGroupBucketRefEntry_s
{
  uint32_t    groupId;
  uint32_t    bucketIndex;
  uint32_t    referenceGroupId;

} ofdbGroupBucketRefEntry_t;


typedef struct ofdbGroupBucketRefTable_node_s
{
  ofdbGroupBucketRefEntry_t  groupBucket; /* Group Bucket configuration information */
  void                      *next;      /* This field must be the last element in the AVL Tree Node */
} ofdbGroupBucketRefTable_node_t;

typedef struct ofdbGroupBucketDataTable_node_s
{
  ofdpaGroupBucketEntry_t  groupBucket; /* Group Bucket configuration information */
  void                    *next;        /* This field must be the last element in the AVL Tree Node */
} ofdbGroupBucketDataTable_node_t;

/* OpenFlow Port AVL Tree.
*/
typedef struct
{
  /* index */
  uint32_t           portNum;
  ofdbPortInfo_t     portInfo;
  uint32_t           port_add_up_time;
  OFDB_ENTRY_FLAG_t  flags;
  void              *next;              /* This field must be the last element in the AVL Tree Node */
} ofdbPortTable_node_t;

/* BEGIN: Added by Hushouqiang, 2016/3/22   问题单号:P10002 */
/* OpenFlow Port AVL Tree.
*/
typedef struct
{
  /* index */
  uint64_t           queueId;
	uint32_t					 port;

	/*properties*/
  uint32_t           maxRate;
	uint32_t					 minRate;
	
  void              *next;              /* This field must be the last element in the AVL Tree Node */
} ofdbCosQueue_node_t;

/* END:   Added by Hushouqiang, 2016/3/22 */

typedef struct
{
  /* index */
  uint32_t                       portNum;
  uint32_t                       tunnelId;
  ofdpaTunnelPortTenantStatus_t  portTenantStatus;
  void                          *next;  /* This field must be the last element in the AVL Tree Node */
} ofdbTunnelPortTenantTable_node_t;

typedef struct
{
  /* index */
  uint32_t                   tunnelId;
  ofdpaTunnelTenantConfig_t  tenantConfig;
  ofdpaTunnelTenantStatus_t  tenantStatus;
  void                      *next;      /* This field must be the last element in the AVL Tree Node */
} ofdbTunnelTenantTable_node_t;

typedef struct
{
  /* index */
  uint32_t                    nextHopId;
  ofdpaTunnelNextHopConfig_t  nhConfig;
  ofdpaTunnelNextHopStatus_t  nhStatus;
  void                       *next;     /* This field must be the last element in the AVL Tree Node */
} ofdbTunnelNextHopTable_node_t;

typedef struct
{
  /* index */
  uint32_t                             ecmpNextHopGroupId;
  ofdpaTunnelEcmpNextHopGroupConfig_t  nhgConfig;
  ofdpaTunnelEcmpNextHopGroupStatus_t  nhgStatus;
  void                                *next; /* This field must be the last element in the AVL Tree Node */
} ofdbTunnelEcmpNextHopGroupTable_node_t;

typedef struct
{
  /* index */
  uint32_t  ecmpNextHopGroupId;
  uint32_t  nextHopId;
  void     *next;                       /* This field must be the last element in the AVL Tree Node */
} ofdbTunnelEcmpNextHopGroupMemberTable_node_t;

/************** METER TABLE **************/

typedef struct ofdbMeterEntryStats_s
{
  uint32_t                refCount;
  uint32_t                meterAddUpTime;
} ofdbMeterEntryStats_t;

typedef struct ofdbMeterTable_node_s
{
  uint32_t                meterId;      /* index */
  ofdpaMeterEntry_t       meter;        /* Meter configuration information */
  ofdbMeterEntryStats_t   stats;        /* Meter Stats information */
  void                   *next;         /* This field must be the last element in the AVL Tree Node */
} ofdbMeterTable_node_t;

/************** CLASS BASED COUNTER TABLE **************/

typedef struct ofdbClassBasedCounterTable_node_s
{
  uint32_t                counterId;    /* index */

  uint32_t                refCount;
  void                   *next;         /* This field must be the last element in the AVL Tree Node */
} ofdbClassBasedCounterTable_node_t;

/************** COLOR BASED COUNTER TABLE **************/

typedef struct ofdbColorBasedCounterTable_node_s
{
  uint32_t                counterId;    /* index */

  uint32_t                refCount;
  void                   *next;         /* This field must be the last element in the AVL Tree Node */
} ofdbColorBasedCounterTable_node_t;

/************** OAM TABLES **************/

typedef struct ofdbOamMegTable_node_s
{
  uint32_t             megIndex;        /* index */
  ofdpaOamMegConfig_t  config;
  ofdpaOamMegStatus_t  status;
  ofdbOamMegInfo_t     megInfo;         /* internal data used for managing this MEG */
  void                *next;            /* This field must be the last element in the AVL Tree Node */
} ofdbOamMegTable_node_t;

typedef struct ofdbOamMepTable_node_s
{
  uint32_t               lmepId;        /* index */
  ofdpaOamMepConfig_t    config;
  ofdpaOamMepStatus_t    status;
  ofdpaOamLbConfig_t     lbConfig;
  ofdpaOamLbStatus_t     lbStatus;
  ofdpaOamLckConfig_t    lckConfig;
  ofdpaOamAisConfig_t    aisConfig;
  ofdpaOamCsfConfig_t    csfConfig;
  ofdpaOamLtConfig_t     ltConfig;
  ofdpaOamLtStatus_t     ltStatus;
  ofdpaOamProLmConfig_t  pmProLmConfig; /* Performance Measurement Proactive Loss Measurement Config */
  ofdpaOamProLmStatus_t  pmProLmStatus; /* Performance Measurement Proactive Loss Measurement Status */
  ofdpaOamOdLmConfig_t   pmOdLmConfig;  /* Performance Measurement OnDemand Loss Measurement Config */
  ofdpaOamOdLmStatus_t   pmOdLmStatus;  /* Performance Measurement OnDemand Loss Measurement Status */
  ofdpaOamProDmConfig_t  pmProDmConfig; /* Performance Measurement Proactive Delay Measurement Config */
  ofdpaOamProDmStatus_t  pmProDmStatus; /* Performance Measurement Proactive Delay Measurement Status */
  ofdpaOamOdDmConfig_t   pmOdDmConfig;  /* Performance Measurement OnDemand Delay Measurement Config */
  ofdpaOamOdDmStatus_t   pmOdDmStatus;  /* Performance Measurement OnDemand Delay Measurement Status */
  void                  *next;          /* This field must be the last element in the AVL Tree Node */
} ofdbOamMepTable_node_t;

typedef struct ofdbOamMipTable_node_s
{
  uint32_t             localMpId;       /* index */
  ofdpaOamMipConfig_t  config;
  ofdpaOamMipStatus_t  status;
  void                *next;            /* This field must be the last element in the AVL Tree Node */
} ofdbOamMipTable_node_t;

typedef struct ofdbOamRemoteMpTable_node_s
{
  uint32_t                  rmpId;      /* index */
  ofdpaOamRemoteMpConfig_t  config;
  void                     *next;       /* This field must be the last element in the AVL Tree Node */
} ofdbOamRemoteMpTable_node_t;

typedef struct ofdbOamMlpGroupTable_node_s
{
  uint32_t                 index;       /* index */
  ofdpaOamMLPGroupConfig_t config;
  /* report PG status directly from OAM subsystem--only refCount values stored in ofdb table */
  struct
  {
    uint32_t  refCount;
    uint32_t  ffGroupId;
  } status;
  void       *next;                     /* This field must be the last element in the AVL Tree Node */
} ofdbOamMlpGroupTable_node_t;

typedef struct
{
  uint32_t  mlpGroupIndex;              /* index */
  uint32_t  lmepId;                     /* index */
  void     *next;                       /* This field must be the last element in the AVL Tree Node */
} ofdbOamMLPGroupMepList_node_t;



typedef struct ofdbPwRedunTable_node_s
{
  uint32_t                 index;       /* index */
  ofdpaPwRedunConfig_t 		config;
  /* report PG status directly from OAM subsystem--only refCount values stored in ofdb table */
  struct
  {
    uint32_t  refCount;
    uint32_t  ffGroupId;
  } status;
  void       *next;                     /* This field must be the last element in the AVL Tree Node */
} ofdbPwRedunTable_node_t;

typedef struct
{
  uint32_t  mlpGroupIndex;              /* index */
  uint32_t  lmepId;                     /* index */
  void     *next;                       /* This field must be the last element in the AVL Tree Node */
} ofdbPwRedunMepList_node_t;

/************** DROP STATUS TABLE **************/

typedef struct ofdbDropStatusTable_node_s
{
  ofdpaDropStatusEntry_t  dropEntry;
  void                   *next;         /* This field must be the last element in the AVL Tree Node */
} ofdbDropStatusTable_node_t;

/************** DATA PLANE COUNTER TABLE **************/

typedef struct ofdbOamDataCounterTable_node_s
{
  uint32_t                     lmepId;  /* index */
  uint8_t                      trafficClass; /* index */
  ofdpaOamDataCounterStatus_t  status;
  void                        *next;    /* This field must be the last element in the AVL Tree Node */
} ofdbOamDataCounterTable_node_t;

/************** REMARK ACTION TABLE **************/

typedef struct
{
  OFDPA_ACTION_TABLE_TYPE_t actionTableType; /* Action table type */
  uint32_t                  index;      /* Mapping profile index */
  uint8_t                   trafficClass; /* Traffic Class */
  OFDPA_QOS_COLORS_t        color;      /* Color */
} ofdbRemarkActionEntryKey_t;

typedef struct ofdbRemarkActionTable_node_s
{
  ofdbRemarkActionEntryKey_t  key;      /* Key into the table. */
  ofdpaRemarkSetActions_t     actions;  /* Remark set actions*/
  void                       *next;     /* This field must be the last element in the AVL Tree Node */
} ofdbRemarkActionTable_node_t;

typedef struct ofdbOamPrimVlanToMplsBhhDataMappingTable_node_s
{
  uint32_t          primVid;            /* index */
  ofdbOamBhhData_t  bhhData;
  void             *next;               /* This field must be the last element in the AVL Tree Node */
} ofdbOamPrimVlanToMplsBhhDataMappingTable_node_t;

typedef struct ofdbInjectedOamLmepIdDataMappingTable_node_s
{
  uint32_t             lmepId;      /* index */

  ofdbOamLmepData_t    oamLmepData;

  void *next;   /* This field must be the last element in the AVL Tree Node */
} ofdbInjectedOamLmepIdDataMappingTable_node_t;


typedef struct ofdbBfdTable_node_s
{
  uint32_t               lmepId;        /* index */
  ofdpaBfdConfig_t    config;
  ofdpaBfdStatus_t    status;
  void                  *next;          /* This field must be the last element in the AVL Tree Node */
} ofdbBfdTable_node_t;




typedef struct ofdbY1564Table_node_s
{
  uint32_t               lmepId;        /* index */  
  ofdpaY1564Status_t    status;
  void                  *next;          /* This field must be the last element in the AVL Tree Node */
} ofdbY1564Table_node_t;


typedef struct ofdbLbTable_node_s
{
  uint32_t               lmepId;        /* index */  
  ofdpaY1564Status_t    status;
  void                  *next;          /* This field must be the last element in the AVL Tree Node */
} ofdbLbTable_node_t;


typedef struct ofdbVlanPortTable_node_s
{
  uint32_t              port;        /* index; tbd 一个端口只能加入到一个VLAN */  
  uint32_t				vlan;
  uint32_t				refCount;
  void                  *next;          /* This field must be the last element in the AVL Tree Node */
} ofdbVlanPortTable_node_t;

typedef struct ofdbVlanIpTable_node_s
{
  uint32_t				vlan;	 /* index */  
  uint8_t              ip[4];      
  uint8_t              mask[4];        
  uint32_t				refCount;
  void                  *next;          /* This field must be the last element in the AVL Tree Node */
} ofdbVlanIpTable_node_t;


typedef struct ofdbLmepidTable_node_s
{
  uint32_t               lmepId;        /* index */
  OFDB_OAM_TYPE_t    	type;
  void                  *next;          /* This field must be the last element in the AVL Tree Node */
} ofdbLmepidTable_node_t;


#endif /* INCLUDE_OFDB_H */
