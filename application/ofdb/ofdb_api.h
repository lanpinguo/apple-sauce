/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2015
*
**********************************************************************
*
* @filename     ofdb_api.h
*
* @purpose      Openflow API header
*
* @component    Openflow
*
* @comments     none
*
* @create       03/22/2013
*
* @end
*
**********************************************************************/
#ifndef INCLUDE_OFDB_API_H
#define INCLUDE_OFDB_API_H

#include "ofdpa_datatypes.h"
#include "ofdpa_api.h"

void ofdbReadLockTake(char *file, uint32_t line_num);
void ofdbWriteLockTake(char *file, uint32_t line_num);
void ofdbLockGive(void);

#define OFDB_READ_LOCK_TAKE  ofdbReadLockTake(__FILE__, __LINE__)
#define OFDB_WRITE_LOCK_TAKE ofdbWriteLockTake(__FILE__, __LINE__)
#define OFDB_LOCK_GIVE       ofdbLockGive()

int ofdbInit(void);

/*********************************************************************
* @purpose  Tests whether a MAC address is a multicast address by checking
*           if the first three octets are 01-00-5e.
*
* @param    mac pointer to MAC address structure
*
* returns   1 - the first three octets in the MAC address are 01-00-5e
*           0 - otherwise
*
* @comments
*
* @end
*
*********************************************************************/
int ofdbMacIsIpv4Multicast(ofdpaMacAddr_t *mac);

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
int ofdbMacIsIpv6Multicast(ofdpaMacAddr_t *mac);

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
int ofdbIpv4MaskLengthGet(in_addr_t mask);

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
int ofdbIpv6MaskLengthGet(struct in6_addr *mask);

OFDPA_ERROR_t ofdbSrcMacLearnModeCfgDataSet(uint32_t mode, ofdpaSrcMacLearnModeCfg_t *cfgData);
OFDPA_ERROR_t ofdbSrcMacLearnModeCfgDataGet(uint32_t *mode, ofdpaSrcMacLearnModeCfg_t *cfgData);

typedef enum
{
  OFDB_ENTRY_HARD_TIMEOUT         = 1 << 0,
  OFDB_ENTRY_IDLE_TIMEOUT         = 1 << 1,

  OFDB_PORT_ADDED                 = 1 << 2,
  OFDB_PORT_DELETED               = 1 << 3,

  OFDB_PORT_LINK_UP               = 1 << 4,
  OFDB_PORT_LINK_DOWN             = 1 << 5,
  OFDB_PORT_BLOCKED               = 1 << 6,
  OFDB_PORT_NOT_BLOCKED           = 1 << 7,
  OFDB_PORT_LIVE                  = 1 << 8,
  OFDB_PORT_NOT_LIVE              = 1 << 9,

} OFDB_ENTRY_FLAG_t;


typedef enum
{
	OFDB_OAM_TYPE_OAM_MEP = 1,		
	OFDB_OAM_TYPE_OAM_MIP,
	OFDB_OAM_TYPE_BFD,
	OFDB_OAM_TYPE_Y1564,
	OFDB_OAM_TYPE_Y1564_LB,
}OFDB_OAM_TYPE_t;


/* set of flags that indicate a port state change event */
#define OFDB_ENTRY_PORT_STATE_EVENT_FLAGS (OFDB_PORT_LINK_UP | OFDB_PORT_LINK_DOWN | OFDB_PORT_BLOCKED | OFDB_PORT_NOT_BLOCKED | OFDB_PORT_LIVE | OFDB_PORT_NOT_LIVE)

#define OFDB_ENTRY_FLAG_SET(__storage__, __flag__)   (__storage__ |= __flag__)
#define OFDB_ENTRY_FLAG_UNSET(__storage__, __flag__) (__storage__ &= ~__flag__)

/* OpenFlow Global Configuration and Status
*/
typedef struct
{

  uint32_t destPort;

} ofdbSrcMacLearnCfg_t;

typedef struct ofdbGlobalData_s
{
  uint32_t srcMaclearnMode; /* enable: 1 or disable: 0 */
  ofdbSrcMacLearnCfg_t srcMacLearnCfg;

  /* global configuration related to tunnel logical ports */
  /* number of vxlan tunnel enpoints configured in port table */
  uint32_t    vxlanTunnelEndpointCount;

  uint32_t    vxlanUseEntropy;
  uint32_t    vxlanTerminatorUdpDestPort;

} ofdbGlobalData_t;

/*
 * Macros for manipulating flow datatypes
 */

/*
** The Flow Table.
*/

#define IN6_IS_ADDR_ALL_ONES(a) \
  (   ((__const uint32_t *) (a))[0] == htonl(0xffffffffu)  \
   && ((__const uint32_t *) (a))[1] == htonl(0xffffffffu)  \
   && ((__const uint32_t *) (a))[2] == htonl(0xffffffffu)  \
   && ((__const uint32_t *) (a))[3] == htonl(0xffffffffu))


/* Flow Status information.
*/
typedef struct ofdbFlowStatus_s
{
  /* Uniquely assigned flow identifier.
  */
  uint64_t flow_id;

  /* System Up Time in seconds when this flow was added to the flow table.
  */
  uint32_t flow_add_up_time;

  /* System Up Time in seconds when this flow had was hit in hardware.  Not
     all flows support idle time out.
  */
  uint32_t flow_hit_up_time;

} ofdbFlowStatus_t;

/************** GROUP TABLE **************/

OFDPA_ERROR_t ofdbGroupAdd(ofdpaGroupEntry_t *group);
OFDPA_ERROR_t ofdbGroupDelete(uint32_t groupId);
OFDPA_ERROR_t ofdbGroupGet(uint32_t groupId, ofdpaGroupEntry_t *group);
OFDPA_ERROR_t ofdbGroupDataUpdate(ofdpaGroupEntry_t *group);
OFDPA_ERROR_t ofdbGroupNextGet(uint32_t groupId, ofdpaGroupEntry_t *nextGroup);
OFDPA_ERROR_t ofdbGroupTypeNextGet(uint32_t groupId, OFDPA_GROUP_ENTRY_TYPE_t groupType, ofdpaGroupEntry_t *nextGroup);
OFDPA_ERROR_t ofdbGroupStatsGet(uint32_t groupId, ofdpaGroupEntryStats_t *groupStats);
OFDPA_ERROR_t ofdbGroupReferenceUpdate(uint32_t groupId, uint32_t increment);
OFDPA_ERROR_t ofdbGroupBucketCountUpdate(uint32_t groupId, uint32_t increment);
OFDPA_ERROR_t ofdbGroupMPLSLabelSubTypeNextGet(uint32_t groupId,
                                               OFDPA_MPLS_LABEL_SUBTYPE_t subType,
                                               ofdpaGroupEntry_t *nextGroup);

/*  Group Table Status. This information is aggregated for all Groups.
*/
typedef struct ofdbGroupTableStatus_s
{
  uint32_t group_database_size; /* Maximum number of group records in the database */
  uint32_t num_groups_in_database; /* Current number of group records in the database */

  /* Maximum groups in each group table  */
  uint32_t groupTableMaxCount[OFDPA_GROUP_ENTRY_TYPE_LAST];

  /* Current number of groups in each group table  */
  uint32_t groupTableEntryCount[OFDPA_GROUP_ENTRY_TYPE_LAST];

} ofdbGroupTableStatus_t;


/************** GROUP BUCKET TABLE **************/

/*
 * Macros for manipulating group IDs.
 */
/* For all Group Types */
#define OFDB_GROUP_TYPE(x)   ((x) >> 28)

/* For Group Types L2 Interface, L2 Multicast, L2 Flood and L3 Multicast */
#define OFDB_GROUP_VLANID(x)              (((x) & 0x0fff0000u) >> 16)

/* For Group Types L2 Interface */
#define OFDB_GROUP_PORTID(x)              ((x) & 0x0000ffffu)

/* For Group Types L2 Flood, L2 Multicast, L3 Multicast */
#define OFDB_GROUP_SHORT_INDEX(x)         ((x) & 0x0000ffffu)

/* For Group Types L2 Rewrite, L3 Unicast, L3 Interface, L3 ECMP */
#define OFDB_GROUP_INDEX(x)               ((x) & 0x0fffffffu)

/* For all Group Types */
#define OFDB_GROUP_TYPE_SET(g, x)         (((g) & ~0xf0000000u) | (((x) & 0xfu) << 28))

/* For Group Types L2 Interface, L2 Multicast, L2 Flood and L3 Multicast */
#define OFDB_GROUP_VLANID_SET(g, x)       (((g) & ~0x0fff0000u) | (((x) & 0x0fffu) << 16))

/* For Group Types L2 Interface */
#define OFDB_GROUP_PORTID_SET(g, x)       (((g) & ~0x0000ffffu) | ((x) & 0xffffu))

/* For Group Types L2 Flood, L2 Multicast, L3 Multicast */
#define OFDB_GROUP_SHORT_INDEX_SET(g, x)  (((g) & ~0xffffu) | ((x) & 0xffffu))

/* For Group Types L2 Rewrite, L3 Unicast, L3 Interface, L3 ECMP */
#define OFDB_GROUP_INDEX_SET(g, x)        (((g) & ~0xfffffffu) | ((x) & 0xfffffffu))

/* For Group Types L2 Overlay */
#define OFDB_GROUP_TUNNELID(x)              (((x) & 0x0ffff000u) >> 12)
#define OFDB_GROUP_TUNNEL_SUBTYPE(x)        (((x) & 0x00000c00u) >> 10)
#define OFDB_GROUP_TUNNEL_INDEX(x)           ((x) & 0x000003ffu)

#define OFDB_GROUP_TUNNELID_SET(g, x)       (((g) & ~0x0ffff000u) | (((x) & 0x0000ffffu) << 12))
#define OFDB_GROUP_TUNNEL_SUBTYPE_SET(g, x) (((g) & ~0x00000c00u) | (((x) & 0x00000003u) << 10))
#define OFDB_GROUP_TUNNEL_INDEX_SET(g, x)   (((g) & ~0x000003ffu) |  ((x) & 0x000003ffu))

/* For Group Types MPLS Label and Forwarding */
#define OFDB_GROUP_MPLS_SUBTYPE(x)        (((x) & 0x0f000000u) >> 24)
#define OFDB_GROUP_MPLS_SUBTYPE_SET(g, x) (((g) & ~0x0f000000u) | (((x) & 0x0000000fu) << 24))

/* For MPLS Group Sub-Types Label, Interface, protection, Fast Failover/Reroute and L2 Tag */
#define OFDB_GROUP_MPLS_INDEX(x)           ((x) & 0x00ffffffu)
#define OFDB_GROUP_MPLS_INDEX_SET(g, x)   (((g) & ~0x00ffffffu) |  ((x) & 0x00ffffffu))

/* For MPLS Group L2 Sub-Types */
#define OFDB_GROUP_MPLS_L2_INDEX(x)       (((x) & 0x00ff0000u) >> 16)
#define OFDB_GROUP_MPLS_L2_TUNNELID(x)     ((x) & 0x0000ffffu)

#define OFDB_GROUP_MPLS_L2_INDEX_SET(g, x)     (((g) & ~0x00ff0000u) | (((x) & 0x000000ffu) << 16))
#define OFDB_GROUP_MPLS_L2_TUNNELID_SET(g, x)  (((g) & ~0x0000ffffu) |  ((x) & 0x0000ffffu))

/* macros for testing MPLS group ID classifications */
#define OFDB_GROUP_IS_MPLS_LABEL(g)      (OFDB_GROUP_TYPE(g) == OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)

#define OFDB_GROUP_IS_MPLS_INTERFACE(g)       (OFDB_GROUP_IS_MPLS_LABEL(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_INTERFACE))
#define OFDB_GROUP_IS_MPLS_L2_VPN_LABEL(g)    (OFDB_GROUP_IS_MPLS_LABEL(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_L2_VPN_LABEL))
#define OFDB_GROUP_IS_MPLS_L3_VPN_LABEL(g)    (OFDB_GROUP_IS_MPLS_LABEL(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_L3_VPN_LABEL))
#define OFDB_GROUP_IS_MPLS_TUNNEL_LABEL1(g)   (OFDB_GROUP_IS_MPLS_LABEL(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_TUNNEL_LABEL1))
#define OFDB_GROUP_IS_MPLS_TUNNEL_LABEL2(g)   (OFDB_GROUP_IS_MPLS_LABEL(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_TUNNEL_LABEL2))
#define OFDB_GROUP_IS_MPLS_SWAP_LABEL(g)      (OFDB_GROUP_IS_MPLS_LABEL(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_SWAP_LABEL))

#define OFDB_GROUP_IS_MPLS_FORWARDING(g)             (OFDB_GROUP_TYPE(g) == OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING)

#define OFDB_GROUP_IS_MPLS_L2_FLOOD(g)                    (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_L2_FLOOD))
#define OFDB_GROUP_IS_MPLS_L2_MULTICAST(g)                (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_L2_MULTICAST))
#define OFDB_GROUP_IS_MPLS_L2_LOCAL_FLOOD(g)              (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_L2_LOCAL_FLOOD))
#define OFDB_GROUP_IS_MPLS_L2_LOCAL_MULTICAST(g)          (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_L2_LOCAL_MULTICAST))
#define OFDB_GROUP_IS_MPLS_L2_FLOOD_SPLIT_HORIZON(g)      (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON))
#define OFDB_GROUP_IS_MPLS_L2_MULTICAST_SPLIT_HORIZON(g)  (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON))
#define OFDB_GROUP_IS_MPLS_FAST_FAILOVER(g)               (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_FAST_FAILOVER))
#define OFDB_GROUP_IS_MPLS_1_1_HEAD_END_PROTECT(g)        (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_1_1_HEAD_END_PROTECT))
#define OFDB_GROUP_IS_MPLS_ECMP(g)                        (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_ECMP))
#define OFDB_GROUP_IS_MPLS_L2_TAG(g)                      (OFDB_GROUP_IS_MPLS_FORWARDING(g) && (OFDB_GROUP_MPLS_SUBTYPE(g) == OFDPA_MPLS_L2_TAG))

/*  Group Bucket Table Status. This information is aggregated for all Groups.
*/
typedef struct ofdbGroupBucketTableStatus_s
{
  uint32_t group_bucket_ref_database_size; /* Maximum number of group bucket records in the database */
  uint32_t group_bucket_data_database_size; /* Maximum number of group bucket records in the database */

  /* Maximum buckets of each type */
  uint32_t groupBucketTableMaxCount[OFDPA_GROUP_ENTRY_TYPE_LAST];

} ofdbGroupBucketTableStatus_t;

OFDPA_ERROR_t ofdbGroupBucketEntryAdd(ofdpaGroupBucketEntry_t *groupBucket);
OFDPA_ERROR_t ofdbGroupBucketEntryDelete(uint32_t groupId, uint32_t bucketIndex);
OFDPA_ERROR_t ofdbGroupBucketEntryGet(uint32_t groupId, uint32_t bucketIndex,
                             ofdpaGroupBucketEntry_t *groupBucket);
OFDPA_ERROR_t ofdbGroupBucketEntryModify(ofdpaGroupBucketEntry_t *groupBucket);
OFDPA_ERROR_t ofdbGroupBucketEntryFirstGet(uint32_t groupId,
                                           ofdpaGroupBucketEntry_t *firstGroupBucket);
OFDPA_ERROR_t ofdbGroupBucketEntryNextGet(uint32_t groupId, uint32_t bucketIndex,
                                 ofdpaGroupBucketEntry_t *nextGroupBucket);
OFDPA_ERROR_t ofdbGroupBucketValidate(ofdpaGroupBucketEntry_t *groupBucket);

uint32_t ofdbGroupTableTotalEntryCountGet(void);

uint32_t ofdbGroupTableMaxCountGet(OFDPA_GROUP_ENTRY_TYPE_t groupType);

uint32_t ofdbGroupTableEntryCountGet(OFDPA_GROUP_ENTRY_TYPE_t groupType);

uint32_t ofdbGroupBucketTableMaxCountGet(uint32_t groupId);

/* Get next GroupId of type 'referringGroupType' that refers 'referredGroupId' */
OFDPA_ERROR_t ofdbRefGroupNextGet(uint32_t referredGroupId, uint32_t *nextReferringGroupId,
                                  OFDPA_GROUP_ENTRY_TYPE_t referringGroupType);

OFDPA_ERROR_t ofdbRefGroupValidate(uint32_t referredGroupId);

OFDPA_ERROR_t ofdbGroupInitL2VpnLabelsGetFromTermTunnelLabel(uint32_t label, uint32_t *isProt, uint32_t *l2vpnLabels, uint32_t *entryCount);

OFDPA_ERROR_t ofdbInjectedOamRefGroupIdGet(uint32_t lmepId, uint32_t *groupId);
uint32_t ofdbInjectedOamCompareRefGroupId(uint32_t serverMpId, uint32_t clientMpId);
/*
** OpenFlow Flow Table
*/
uint32_t ofdbFlowTableSupported(OFDPA_FLOW_TABLE_ID_t tableId);
uint32_t ofdbFlowTableMaxCountGet(OFDPA_FLOW_TABLE_ID_t tableId);
uint32_t ofdbFlowTableEntryCountGet(OFDPA_FLOW_TABLE_ID_t tableId);

uint32_t ofdbFlowEntryValidate(ofdpaFlowEntry_t *flow);
uint32_t ofdbFlowDeletionValidate(ofdpaFlowEntry_t *flow);

OFDPA_ERROR_t ofdbFlowAdd(ofdpaFlowEntry_t *flow, uint64_t *flowId);
OFDPA_ERROR_t ofdbFlowDelete(ofdpaFlowEntry_t *flow);
OFDPA_ERROR_t ofdbFlowGet(ofdpaFlowEntry_t *flow,
                          OFDB_ENTRY_FLAG_t *flags,
                          ofdbFlowStatus_t *flowStatus);
OFDPA_ERROR_t ofdbFlowNextGet(ofdpaFlowEntry_t *flow,
                              ofdpaFlowEntry_t *nextFlow,
                              OFDB_ENTRY_FLAG_t *nextFlowFlags,
                              ofdbFlowStatus_t *nextFlowStatus);
OFDPA_ERROR_t ofdbFlowStatusSet(ofdpaFlowEntry_t *flow,
                                OFDB_ENTRY_FLAG_t *flags,
                                ofdbFlowStatus_t *flowStatus);
OFDPA_ERROR_t ofdbFlowEventNextGet(ofdpaFlowEvent_t *eventData);
OFDPA_ERROR_t ofdbFlowByCookieGet(uint64_t cookie,
                                  ofdpaFlowEntry_t *flowConfig,
                                  ofdbFlowStatus_t *flowStatus);
OFDPA_ERROR_t ofdbFlowByCookieDelete(uint64_t cookie);
OFDPA_ERROR_t ofdbFlowByFlowIdGet(uint64_t flowId,
                                  ofdpaFlowEntry_t *flowConfig,
                                  ofdbFlowStatus_t *flowStatus);

/*
** The Port Table.
*/

#define OFDB_PORT_TYPE(p)         (((p) & OFDPA_INPORT_TYPE_MASK) >> 16)
#define OFDB_PORT_INDEX(p)         ((p) & OFDPA_INPORT_INDEX_MASK)
#define OFDB_PORT_TYPE_SET(p, x)  (((p) & ~OFDPA_INPORT_TYPE_MASK)  | (((x) & 0xffffu) << 16))
#define OFDB_PORT_INDEX_SET(p, x) (((p) & ~OFDPA_INPORT_INDEX_MASK) | ((x) & 0xffffu))

#define OFDB_MPLS_L2_PORT_TYPE(p)         (((p) & OFDPA_MPLS_L2_PORT_TYPE_MASK) >> 16)
#define OFDB_MPLS_L2_PORT_INDEX(p)         ((p) & OFDPA_MPLS_L2_PORT_INDEX_MASK)
#define OFDB_MPLS_L2_PORT_TYPE_SET(p, x)  (((p) & ~OFDPA_MPLS_L2_PORT_TYPE_MASK)  | (((x) & 0xffffu) << 16))
#define OFDB_MPLS_L2_PORT_INDEX_SET(p, x) (((p) & ~OFDPA_MPLS_L2_PORT_INDEX_MASK) | ((x) & 0xffffu))

#define OFDB_TUNNELID_TYPE(p)         (((p) & OFDPA_TUNNEL_ID_TYPE_MASK) >> 16)
#define OFDB_TUNNELID_INDEX(p)         ((p) & OFDPA_TUNNEL_ID_INDEX_MASK)
#define OFDB_TUNNELID_TYPE_SET(p, x)  (((p) & ~OFDPA_TUNNEL_ID_TYPE_MASK)  | (((x) & 0xffffu) << 16))
#define OFDB_TUNNELID_INDEX_SET(p, x) (((p) & ~OFDPA_TUNNEL_ID_INDEX_MASK) | ((x) & 0xffffu))

/* Port Queue information
*/

typedef struct
{
  uint32_t minRate;
  uint32_t maxRate;
  uint32_t weight;
} ofdbPortQueueInfo_t;

/* Port Status information.
*/
typedef struct
{
  /* status items */
  ofdpaMacAddr_t hwAddr;
  char name[OFDPA_PORT_NAME_STRING_SIZE];
  /*Description,added by Hushouqiang 20160316*/
  char descp[OFDPA_PORT_DESCP_STRING_SIZE];

  uint32_t maxSpeed;

  OFDPA_PORT_CONFIG_t config;
  OFDPA_PORT_STATE_t state;

/* BEGIN: Added by HuShouqiang, 2016/3/17   ÎÊÌâµ¥ºÅ:P10001 */
	/*last-change date-and-time in calendar time*/
	uint32_t last_chg;

/* END:   Added by HuShouqiang, 2016/3/17 */

  uint32_t  schedMode;  /* port congestion scheduler mode */

  OFDPA_PORT_FEATURE_t advertised;
  OFDPA_PORT_FEATURE_t supported;

  ofdbPortQueueInfo_t *portQueueInfo;

  /* flag indicating a physical is used in an untagged tunnel access port */
  uint32_t physicalPortOnUntaggedAccessPort;

  ofdpaTunnelPortConfig_t tunnelPortConfig;
  ofdpaTunnelPortStatus_t tunnelPortStatus;

  uint32_t ffGroupId;  /* used to store fast failover group that caused a liveness port to be created */

  uint32_t ulIpAddr;//management ip
  uint32_t ulIpMask;//management mask
} ofdbPortInfo_t;

/* For qos*/
typedef enum
{
  OFDB_QOS_QUEUE_SCHED_MODE_NONE                  =     0x00,       /* Pass through. */
  OFDB_QOS_QUEUE_SCHED_MODE_SP                    =     0x01,       /* Weights are ignored. */
  OFDB_QOS_QUEUE_SCHED_MODE_RR                    =     0x02,       /* Weights are ignored. */
  OFDB_QOS_QUEUE_SCHED_MODE_WRR                   =     0x04,       
  OFDB_QOS_QUEUE_SCHED_MODE_WFQ                   =     0x08,       
  OFDB_QOS_QUEUE_SCHED_MODE_WDRR                  =     0x10,       
} OFDB_QOS_QUEUE_SCHED_MODE_t;

typedef enum
{
  OFDB_PORT_DISCARD_MODE_TD                       =     1,
  OFDB_PORT_DISCARD_MODE_WRED                     =     2,
}OFDB_PORT_DISCARD_MODE_t;

typedef enum
{
  OFDB_COSQ_DISCARD_COLOR_GREEN  = 1,
  OFDB_COSQ_DISCARD_COLOR_YELLOW  = 2,
  OFDB_COSQ_DISCARD_COLOR_RED    = 3
}OFDB_COSQ_DISCARD_COLOR_enum;


extern uint32_t ofdbPortNumQueues_g;
int ofdbPortIsValid(uint32_t portNum);
OFDPA_ERROR_t ofdbPortAdd(uint32_t port, ofdbPortInfo_t *portInfo);
OFDPA_ERROR_t ofdbPortDelete(uint32_t portNum);
OFDPA_ERROR_t ofdbPortNextGet(uint32_t portNum, uint32_t *nextPortNum);
OFDPA_ERROR_t ofdbPortMacGet(uint32_t portNum, ofdpaMacAddr_t *mac);
OFDPA_ERROR_t ofdbPortInfoSet(uint32_t portNum, ofdbPortInfo_t *portInfo);
OFDPA_ERROR_t ofdbPortNameGet(uint32_t portNum, ofdpa_buffdesc *name);
OFDPA_ERROR_t ofdbPortNameSet(uint32_t portNum, ofdpa_buffdesc *name);
OFDPA_ERROR_t ofdbPortDescpGet(uint32_t portNum, ofdpa_buffdesc *descp);
OFDPA_ERROR_t ofdbPortDescpSet(uint32_t portNum, ofdpa_buffdesc *descp);
OFDPA_ERROR_t ofdbPortEnableSet(uint32_t  portNum, int enable);
OFDPA_ERROR_t ofdbPortEnableGet(uint32_t port, int *enable);
OFDPA_ERROR_t ofdbPortLastChgTSGet(uint32_t portNum, uint32_t *ts);
OFDPA_ERROR_t ofdbPortStateSet(uint32_t  portNum, OFDPA_PORT_STATE_t  state);
OFDPA_ERROR_t ofdbPortDurationGet(uint32_t  portNum, uint32_t *duration);
OFDPA_ERROR_t ofdbPortStateGet(uint32_t  portNum, OFDPA_PORT_STATE_t  *state);
OFDPA_ERROR_t ofdbPortConfigSet(uint32_t portNum, OFDPA_PORT_CONFIG_t config);
OFDPA_ERROR_t ofdbPortConfigGet(uint32_t portNum, OFDPA_PORT_CONFIG_t  *config);
OFDPA_ERROR_t ofdbPortMaxSpeedGet(uint32_t  portNum, uint32_t  *maxSpeed);
OFDPA_ERROR_t ofdbPortAdvertiseFeatureSet(uint32_t portNum, OFDPA_PORT_FEATURE_t advertise);
OFDPA_ERROR_t ofdbPortEventNextGet(ofdpaPortEvent_t *eventData);
OFDPA_ERROR_t ofdbPortEventGet(ofdpaPortEvent_t *eventData);
OFDPA_ERROR_t ofdbPortQueueMinMaxRateGet(uint32_t portNum, uint32_t queueId, uint32_t *minrate, uint32_t *maxrate);
OFDPA_ERROR_t ofdbPortQueueMinMaxRateSet(uint32_t portNum, uint32_t queueId, uint32_t minrate, uint32_t maxrate);
OFDPA_ERROR_t ofdbPortQueueIdIsValid(uint32_t portNum, uint32_t queueId);

OFDPA_ERROR_t ofdbPortGet(uint32_t portNum, ofdbPortInfo_t *portInfo, OFDB_ENTRY_FLAG_t *flags);
OFDPA_ERROR_t ofdbPortVxlanTunnelEndpointGlobalConfigGet(uint16_t *termUdpDestPort, uint16_t *useEntropy);
OFDPA_ERROR_t ofdbTunnelPortReferenceUpdate(uint32_t port, uint32_t increment);
OFDPA_ERROR_t ofdbPhysPortInUntaggedAccessFlagUpdate(uint32_t portNum, uint32_t set);

OFDPA_ERROR_t ofdbPortTenantAdd(uint32_t port, uint32_t tunnelId);
OFDPA_ERROR_t ofdbPortTenantDelete(uint32_t port, uint32_t tunnelId);
OFDPA_ERROR_t ofdbPortTenantGet(uint32_t port, uint32_t tunnelId, ofdpaTunnelPortTenantStatus_t *status);
OFDPA_ERROR_t ofdbPortTenantNextGet(uint32_t port, uint32_t tunnelId, uint32_t *nextTunnelId);
OFDPA_ERROR_t ofdbPortTenantReferenceUpdate(uint32_t port, uint32_t tunnelId, uint32_t increment);

OFDPA_ERROR_t  ofdbPortQueueSchedSet(uint32_t port, int32_t mode,int32_t *weights);
OFDPA_ERROR_t  ofdbPortQueueWeightModify(uint32_t port,uint32_t queueId, int32_t weight);
OFDPA_ERROR_t  ofdbPortQueueSchedGet(uint32_t port, int32_t *mode,int32_t *weights);
OFDPA_ERROR_t  ofdbPortDiscardModeSet(uint32_t mode);
OFDPA_ERROR_t  ofdbPortDiscardModeGet(uint32_t *mode);
OFDPA_ERROR_t  ofdbPortDiscardConfig(uint32_t port, uint32_t cosq,uint32_t color,
  int drop_start, int drop_slope,int drop_end);
OFDPA_ERROR_t  ofdbPortDiscardConfigGet(uint32_t port, uint32_t cosq,uint32_t color,
  int *drop_start, int *drop_slope,int *drop_end);


OFDPA_ERROR_t ofdbTenantAdd(uint32_t tunnelId, ofdpaTunnelTenantConfig_t *config);
OFDPA_ERROR_t ofdbTenantDelete(uint32_t tunnelId);
OFDPA_ERROR_t ofdbTenantGet(uint32_t tunnelId, ofdpaTunnelTenantConfig_t *config, ofdpaTunnelTenantStatus_t *status);
OFDPA_ERROR_t ofdbTenantNextGet(uint32_t tunnelId, uint32_t *nextTunnelId);
OFDPA_ERROR_t ofdbTenantReferenceUpdate(uint32_t tunnelId, uint32_t increment);

OFDPA_ERROR_t ofdbNextHopAdd(uint32_t nextHopId, ofdpaTunnelNextHopConfig_t *nextHopInfo);
OFDPA_ERROR_t ofdbNextHopDelete(uint32_t nextHopId);
OFDPA_ERROR_t ofdbNextHopModify(uint32_t nextHopId, ofdpaTunnelNextHopConfig_t *nextHopInfo);
OFDPA_ERROR_t ofdbNextHopGet(uint32_t nextHopId,
                             ofdpaTunnelNextHopConfig_t *config,
                             ofdpaTunnelNextHopStatus_t *status);
OFDPA_ERROR_t ofdbNextHopNextGet(uint32_t nextHopId, uint32_t *nextNextHopId);
OFDPA_ERROR_t ofdbNextHopReferenceUpdate(uint32_t nextHopId, uint32_t increment);

OFDPA_ERROR_t ofdbEcmpNextHopGroupAdd(uint32_t ecmpNextHopGroupId, ofdpaTunnelEcmpNextHopGroupConfig_t *config);
OFDPA_ERROR_t ofdbEcmpNextHopGroupDelete(uint32_t ecmpNextHopGroupId);
OFDPA_ERROR_t ofdbEcmpNextHopGroupGet(uint32_t ecmpNextHopGroupId,
                                      ofdpaTunnelEcmpNextHopGroupConfig_t *config,
                                      ofdpaTunnelEcmpNextHopGroupStatus_t *status);
OFDPA_ERROR_t ofdbEcmpNextHopGroupNextGet(uint32_t ecmpNextHopGroupId, uint32_t *nextEcmpNextHopGroupId);
OFDPA_ERROR_t ofdbEcmpNextHopGroupReferenceUpdate(uint32_t ecmpNextHopGroupId, uint32_t increment);

OFDPA_ERROR_t ofdbEcmpNextHopGroupMemberAdd(uint32_t ecmpNextHopGroupId, uint32_t nextHopId);
OFDPA_ERROR_t ofdbEcmpNextHopGroupMemberDelete(uint32_t ecmpNextHopGroupId, uint32_t nextHopId);
OFDPA_ERROR_t ofdbEcmpNextHopGroupMemberGet(uint32_t ecmpNextHopGroupId, uint32_t nextHopId);
OFDPA_ERROR_t ofdbEcmpNextHopGroupMemberNextGet(uint32_t ecmpNextHopGroupId, uint32_t nextHopId, uint32_t *nextNextHopId);

/* Meter Table */

typedef struct ofdbMeterTableStatus_s
{
  /* Maximum entries in meter table  */
  uint32_t meterTableMaxCount;

} ofdbMeterTableStatus_t;

uint32_t ofdbMeterTableEntryCountGet(void);
uint32_t ofdbMeterTableMaxCountGet(void);
OFDPA_ERROR_t ofdbMeterAdd(uint32_t meterId, ofdpaMeterEntry_t *meter);
OFDPA_ERROR_t ofdbMeterDelete(uint32_t meterId);
OFDPA_ERROR_t ofdbMeterGet(uint32_t meterId, ofdpaMeterEntry_t *meter);
OFDPA_ERROR_t ofdbMeterNextGet(uint32_t meterId, uint32_t *nextMeterId);
OFDPA_ERROR_t ofdbMeterStatsGet(uint32_t meterId, ofdpaMeterEntryStats_t *meterStats);
OFDPA_ERROR_t ofdbMeterEntryValidate(uint32_t meterId, ofdpaMeterEntry_t *meter);
OFDPA_ERROR_t ofdbMeterReferenceUpdate(uint32_t meterId, uint32_t increment);

#define OFDB_IS_SERVICE_METER(id) ((id >= OFDPA_L2_POLICER_METERID_MIN_VALUE) ? 1 : 0)

/* Class and Color Based Counters */
typedef struct ofdbClassBasedCounterTableStatus_s
{
  uint32_t tableMaxCount;

} ofdbClassBasedCounterTableStatus_t;

uint32_t ofdbClassBasedCounterTableEntryCountGet(void);
uint32_t ofdbClassBasedCounterTableMaxCountGet(void);
OFDPA_ERROR_t ofdbClassBasedCounterAdd(uint32_t counterId);
OFDPA_ERROR_t ofdbClassBasedCounterDelete(uint32_t counterId);
OFDPA_ERROR_t ofdbClassBasedCounterGet(uint32_t counterId);
OFDPA_ERROR_t ofdbClassBasedCounterIdNextGet(uint32_t counterId, uint32_t *nextCounterId);
OFDPA_ERROR_t ofdbClassBasedCounterStatsGet(uint32_t counterId, ofdpaClassBasedCounterStats_t *stats);
OFDPA_ERROR_t ofdbClassBasedCounterRefCountGet(uint32_t counterId, uint32_t *refCount);
OFDPA_ERROR_t ofdbClassBasedCounterReferenceUpdate(uint32_t counterId, uint32_t increment);

typedef struct ofdbColorBasedCounterTableStatus_s
{
  uint32_t tableMaxCount;

} ofdbColorBasedCounterTableStatus_t;

uint32_t ofdbColorBasedCounterTableEntryCountGet(void);
uint32_t ofdbColorBasedCounterTableMaxCountGet(void);
OFDPA_ERROR_t ofdbColorBasedCounterAdd(uint32_t counterId);
OFDPA_ERROR_t ofdbColorBasedCounterDelete(uint32_t counterId);
OFDPA_ERROR_t ofdbColorBasedCounterGet(uint32_t counterId);
OFDPA_ERROR_t ofdbColorBasedCounterIdNextGet(uint32_t counterId, uint32_t *nextCounterId);
OFDPA_ERROR_t ofdbColorBasedCounterStatsGet(uint32_t counterId, ofdpaColorBasedCounterStats_t *stats);
OFDPA_ERROR_t ofdbColorBasedCounterRefCountGet(uint32_t counterId, uint32_t *refCount);
OFDPA_ERROR_t ofdbColorBasedCounterReferenceUpdate(uint32_t counterId, uint32_t increment);

OFDPA_ERROR_t ofdbOamLogicalLivenessPortCreate(uint32_t portId, uint32_t ffGroupId);

typedef struct
{
  uint32_t dot1agMdIndex;
  uint32_t dot1agMaIndex;
  uint32_t primVid;

} ofdbOamMegInfo_t;

int ofdbOamMegTypeIsValid(OFDPA_OAM_MEG_TYPE_t megType);
int ofdbOamMegConfigValidate(uint32_t megIndex, ofdpaOamMegConfig_t *config);
OFDPA_ERROR_t ofdbOamMegAdd(uint32_t megIndex, ofdpaOamMegConfig_t *config);
OFDPA_ERROR_t ofdbOamMegDelete(uint32_t megIndex);
OFDPA_ERROR_t ofdbOamMegModify(uint32_t megIndex, ofdpaOamMegConfig_t *newConfig);
OFDPA_ERROR_t ofdbOamMegGet(uint32_t megIndex, ofdpaOamMegConfig_t *config, ofdpaOamMegStatus_t *status, ofdbOamMegInfo_t *info);
OFDPA_ERROR_t ofdbOamMegNextGet(uint32_t megIndex, uint32_t *nextMegIndex);
OFDPA_ERROR_t ofdbOamMegReferenceUpdate(uint32_t megIndex, uint32_t increment);

OFDPA_OAM_MEG_TYPE_t ofdbMegTypeFromVidGet(uint32_t primVid);

int ofdbOamMepIdIsValid(uint16_t mepId);
int ofdbOamMepConfigValidate(uint32_t lmepId, ofdpaOamMepConfig_t *config);
OFDPA_ERROR_t ofdbOamMepAdd(uint32_t lmepId, ofdpaOamMepConfig_t *config);
OFDPA_ERROR_t ofdbOamMepDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamMepModify(uint32_t lmepId, ofdpaOamMepConfig_t *newConfig);
OFDPA_ERROR_t ofdbOamMepGet(uint32_t lmepId, ofdpaOamMepConfig_t *config, ofdpaOamMepStatus_t *status);
OFDPA_ERROR_t ofdbOamMepNextGet(uint32_t lmepId, uint32_t *nextLmepId);
OFDPA_ERROR_t ofdbOamMepReferenceUpdate(uint32_t lmepId, uint32_t increment);
OFDPA_ERROR_t ofdbOamMepTimeoutUpdate(uint32_t lmepId, uint32_t timeout);
OFDPA_ERROR_t ofdbOamMepTimeoutGet(uint32_t lmepId, uint32_t *timeout);

int ofdbOamMipConfigValidate(uint32_t localMpId, ofdpaOamMipConfig_t *config);
OFDPA_ERROR_t ofdbOamMipAdd(uint32_t localMpId, ofdpaOamMipConfig_t *config);
OFDPA_ERROR_t ofdbOamMipDelete(uint32_t localMpId);
OFDPA_ERROR_t ofdbOamMipGet(uint32_t localMpId, ofdpaOamMipConfig_t *config, ofdpaOamMipStatus_t *status);
OFDPA_ERROR_t ofdbOamMipNextGet(uint32_t localMpId, uint32_t *nextMipId);
OFDPA_ERROR_t ofdbOamMipReferenceUpdate(uint32_t localMpId, uint32_t increment);

int ofdbOamRemoteMpConfigValidate(uint32_t remoteMpId, ofdpaOamRemoteMpConfig_t *config);
OFDPA_ERROR_t ofdbOamRemoteMpAdd(uint32_t remoteMpId, ofdpaOamRemoteMpConfig_t *config);
OFDPA_ERROR_t ofdbOamRemoteMpDelete(uint32_t remoteMpId);
OFDPA_ERROR_t ofdbOamRemoteMpGet(uint32_t remoteMpId, ofdpaOamRemoteMpConfig_t *config);
OFDPA_ERROR_t ofdbOamRemoteMpNextGet(uint32_t remoteMpId, uint32_t *nextRemoteMpId);
OFDPA_ERROR_t ofdbOamRemoteMpFind(uint32_t rmpId, ofdpaOamRemoteMpConfig_t *config);

OFDPA_ERROR_t ofdbOamMepCCMDatabaseEntryGet(uint32_t lmepId, uint32_t remoteMepId, ofdpaOamCcmDatabaseEntry_t *dbEntry);
OFDPA_ERROR_t ofdbOamMepCCMDatabaseEntryNextGet(uint32_t lmepId, uint32_t remoteMepId, uint32_t *nextRemoteMepId);

int ofdbOamMLPGroupConfigValidate(uint32_t index, ofdpaOamMLPGroupConfig_t *config);
OFDPA_ERROR_t ofdbOamMLPGroupAdd(uint32_t index, ofdpaOamMLPGroupConfig_t *config);
OFDPA_ERROR_t ofdbOamMLPGroupDelete(uint32_t index);
OFDPA_ERROR_t ofdbOamMLPGroupGet(uint32_t index, ofdpaOamMLPGroupConfig_t *config, ofdpaOamMLPGroupStatus_t *status);
OFDPA_ERROR_t ofdbOamMLPGroupNextGet(uint32_t index, uint32_t *nextIndex);
OFDPA_ERROR_t ofdbOamMLPGroupReferenceUpdate(uint32_t index, uint32_t increment);
OFDPA_ERROR_t ofdbOamMLPGroupRequest(uint32_t index, OFDPA_PROTECTION_SWITCH_REQUEST_TYPE_t request);

int ofdbOamMepProtectionValidate(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg);
OFDPA_ERROR_t ofdbOamMepProtectionAdd(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg);
OFDPA_ERROR_t ofdbOamMepProtectionDelete(uint32_t mlpGroupIndex, uint32_t lmepId);
OFDPA_ERROR_t ofdbOamMepProtectionGet(uint32_t mlpGroupIndex, uint32_t lmepId);
OFDPA_ERROR_t ofdbOamMepProtectionNextGet(uint32_t mlpGroupIndex, uint32_t lmepId, uint32_t *nextLmepId);

/* Drop Status Table */
uint32_t ofdbDropStatusTableEntryCountGet(void);
uint32_t ofdbDropStatusTableMaxCountGet(void);
OFDPA_ERROR_t ofdbDropStatusAdd(ofdpaDropStatusEntry_t *dropEntry);
OFDPA_ERROR_t ofdbDropStatusDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbDropStatusGet(uint32_t lmepId, ofdpaDropStatusEntry_t *dropEntry);
OFDPA_ERROR_t ofdbDropStatusNextGet(uint32_t lmepId, ofdpaDropStatusEntry_t *nextDropEntry);
OFDPA_ERROR_t ofdbDropStatusActionGet(uint32_t lmepId, uint8_t *dropAction);
OFDPA_ERROR_t ofdbDropStatusEntryValidate(ofdpaDropStatusEntry_t *dropEntry);

/* Data Plane Counter Table */
uint32_t ofdbOamDataCounterTableEntryCountGet(void);
uint32_t ofdbOamDataCounterTableMaxCountGet(void);
OFDPA_ERROR_t ofdbOamDataCounterAdd(uint32_t lmepId, uint8_t trafficClass);
OFDPA_ERROR_t ofdbOamDataCounterDelete(uint32_t lmepId, uint8_t trafficClass);
OFDPA_ERROR_t ofdbOamDataCounterStatusGet(uint32_t lmepId, uint8_t trafficClass, ofdpaOamDataCounterStatus_t *status);
OFDPA_ERROR_t ofdbOamDataCounterGet(ofdpaOamDataCounterIndex_t index, ofdpaOamDataCounterStatus_t *status);
OFDPA_ERROR_t ofdbOamDataCounterNextGet(ofdpaOamDataCounterIndex_t index, ofdpaOamDataCounterIndex_t *nextIndex,
                                        ofdpaOamDataCounterStatus_t *status);
OFDPA_ERROR_t ofdbOamDataCounterReferenceUpdate(uint32_t lmepId, uint8_t trafficClass, uint32_t increment);
OFDPA_ERROR_t ofdbOamDataCounterEntryValidate(uint32_t lmepId, uint8_t trafficClass);

/* Remark Action Table */
uint32_t ofdbRemarkActionTableEntryCountGet(void);
uint32_t ofdbRemarkActionTableMaxCountGet(void);
OFDPA_ERROR_t ofdbRemarkActionAdd(ofdpaRemarkActionEntry_t *remarkEntry);
OFDPA_ERROR_t ofdbRemarkActionDelete(ofdpaRemarkActionEntry_t *remarkEntry);
OFDPA_ERROR_t ofdbRemarkActionEntryGet(ofdpaRemarkActionEntry_t *remarkEntry);
OFDPA_ERROR_t ofdbRemarkActionNextGet(ofdpaRemarkActionEntry_t *remarkEntry, ofdpaRemarkActionEntry_t *nextEntry);
OFDPA_ERROR_t ofdbRemarkActionEntryValidate(ofdpaRemarkActionEntry_t *remarkEntry);

OFDPA_ERROR_t ofdbBfdGet(uint32_t lmepId, ofdpaBfdConfig_t *config, ofdpaBfdStatus_t *status);
OFDPA_ERROR_t ofdbBfdNextGet(uint32_t lmepId, uint32_t *nextLmepId);
OFDPA_ERROR_t ofdbBfdDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbBfdAdd(uint32_t lmepId, ofdpaBfdConfig_t *config);
OFDPA_ERROR_t ofdbBfdReferenceUpdate(uint32_t lmepId, uint32_t increment);
int ofdbBfdConfigValidate(uint32_t lmepId, ofdpaBfdConfig_t *config);
OFDPA_ERROR_t ofdbBfdEventNotify(  uint32_t flags,
                                        uint32_t event_type,
                                        uint32_t endpoint,
                                        ofdpaBfdStatus_t *bfdStatus
                                      );
OFDPA_ERROR_t ofdbBfdStateGet(uint32_t lmepId, uint32_t *bfdState);


typedef struct
{
  uint32_t             ifNum;
  uint32_t             popVlanTag;
  ofdpaMacAddr_t       destMac;
  ofdpaMacAddr_t       srcMac;
  uint32_t             vlanId;

  uint32_t             mplsTunnel2Label;
  uint32_t             mplsTunnel1Label;
  uint32_t             mplsGAL;

  uint32_t             livenessLogicalPortId;
  uint32_t             megLevel;
} ofdbOamBhhData_t;

/** OAM Type Id enumerator */
typedef enum
{
  OFDB_OAM_TYPE_ETHERNET_UP           = 0,
  OFDB_OAM_TYPE_ETHERNET_DOWN         = 1,
  OFDB_OAM_TYPE_SECTION               = 2,
  OFDB_OAM_TYPE_LSP                   = 3,
  OFDB_OAM_TYPE_PW_VCCV_TYPE_1_OR_3   = 4,
  OFDB_OAM_TYPE_PW_VCCV_TYPE_4        = 5

} OFDB_OAM_TYPE_ID_t;

#define OFDPA_MPLS_LABELS_MAX     3

typedef struct
{
  OFDB_OAM_TYPE_ID_t  oamType;

  uint32_t            tunnelId;
  uint32_t            mplsL2Port;
  uint32_t            oamLmTxCountAction;

  uint32_t            ifNum;
  uint32_t            popVlanTag;
  ofdpaMacAddr_t      destMac;
  ofdpaMacAddr_t      srcMac;
  uint32_t            vlanId;

  uint32_t            mplsDataFirstNibble;
  uint32_t            mplsAchChannel;

  uint32_t            pushCW;

  uint32_t            livenessLogicalPortId;
  uint32_t            megLevel;

  /* Encap mpls labels.
     mplsHeader[0]: innermost label
     mplsHeader[2]: outermost label */
  uint32_t            mplsHeader[OFDPA_MPLS_LABELS_MAX];
  uint32_t            numLabels;
} ofdbOamLmepData_t;

OFDPA_ERROR_t ofdbFlowInjectedOamEntryTypeGet(ofdpaInjectedOamFlowEntry_t *flowData,
                                              OFDB_OAM_TYPE_ID_t *oamType);
OFDPA_ERROR_t ofdbInjectedOamLmepIdMapEntryAdd(ofdpaInjectedOamFlowEntry_t *flowData);
OFDPA_ERROR_t ofdbInjectedOamLmepIdMapEntryDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbInjectedOamLmepIdMapDataGet(uint32_t lmepId, ofdbOamLmepData_t **oamMapData);
OFDPA_ERROR_t ofdbInjectedOamLmepIdMapDataUpdate(uint32_t ffGroupId);

OFDPA_ERROR_t ofdbOamLbAdd(uint32_t lmepId, ofdpaOamLbConfig_t *config);
OFDPA_ERROR_t ofdbOamLbStart(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamLbStop(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamLbGet(uint32_t lmepId, ofdpaOamLbConfig_t *config, ofdpaOamLbStatus_t *status);
OFDPA_ERROR_t ofdbOamLbConfigValidate(uint32_t lmepId, ofdpaOamLbConfig_t *config);
OFDPA_ERROR_t ofdbOamProLmAdd(uint32_t lmepId, ofdpaOamProLmConfig_t *config);
OFDPA_ERROR_t ofdbOamProDmAdd(uint32_t lmepId, ofdpaOamProDmConfig_t *config);
OFDPA_ERROR_t ofdbOamOdDmAdd(uint32_t lmepId, ofdpaOamOdDmConfig_t *config);
OFDPA_ERROR_t ofdbOamOdLmAdd(uint32_t lmepId, ofdpaOamOdLmConfig_t *config);
OFDPA_ERROR_t ofdbOamProLmDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamProDmDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamOdDmDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamOdLmDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamProLmGet(uint32_t lmepId, ofdpaOamProLmConfig_t *config, ofdpaOamProLmStatus_t *status);
OFDPA_ERROR_t ofdbOamProDmGet(uint32_t lmepId, ofdpaOamProDmConfig_t *config, ofdpaOamProDmStatus_t *status);
OFDPA_ERROR_t ofdbOamOdDmGet(uint32_t lmepId, ofdpaOamOdDmConfig_t *config, ofdpaOamOdDmStatus_t *status);
OFDPA_ERROR_t ofdbOamOdLmGet(uint32_t lmepId, ofdpaOamOdLmConfig_t *config, ofdpaOamOdLmStatus_t *status);
OFDPA_ERROR_t ofdbOamProLmConfigValidate(uint32_t lmepId, ofdpaOamProLmConfig_t *config);
OFDPA_ERROR_t ofdbOamProDmConfigValidate(uint32_t lmepId, ofdpaOamProDmConfig_t *config);
OFDPA_ERROR_t ofdbOamOdLmConfigValidate(uint32_t lmepId, ofdpaOamOdLmConfig_t *config);
OFDPA_ERROR_t ofdbOamOdDmConfigValidate(uint32_t lmepId, ofdpaOamOdDmConfig_t *config);
OFDPA_ERROR_t ofdbOamOdLmStart(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamOdLmStop(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamOdDmStart(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamOdDmStop(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamProDmStart(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamProDmStop(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamProLmEnabledSet(uint32_t lmepId, OFDPA_BOOL mode);
OFDPA_ERROR_t ofdbOamProLmCountersGet(uint32_t lmepId, ofdpaOamProLmCounters_t *counters);
OFDPA_ERROR_t ofdbOamProDmCountersGet(uint32_t lmepId, ofdpaOamProDmCounters_t *counters);
OFDPA_ERROR_t ofdbOamOdDmCountersGet(uint32_t lmepId, ofdpaOamOdDmSnapshot_t *counters);
OFDPA_ERROR_t ofdbOamOdLmCountersGet(uint32_t lmepId, ofdpaOamOdLmSnapshot_t *counters);
OFDPA_ERROR_t ofdbOamLtStart(uint32_t lmepId, ofdpaOamLtConfig_t *ltConfig);
OFDPA_ERROR_t ofdbOamLtGet(uint32_t lmepId, ofdpaOamLtConfig_t *ltConfig, ofdpaOamLtStatus_t *ltStatus);
OFDPA_ERROR_t ofdbOamLtResultNextGet(uint32_t lmepId, uint32_t sequenceNum, uint32_t receiveOrder, ofdpaOamLtrEntry_t *ltResult);

OFDPA_ERROR_t ofdbOamCsfEnable(uint32_t lmepId, ofdpaOamCsfConfig_t *config);
OFDPA_ERROR_t ofdbOamCsfDisable(uint32_t lmepId);

OFDPA_ERROR_t ofdbOamAisEnable(uint32_t lmepId, ofdpaOamAisConfig_t *config);
OFDPA_ERROR_t ofdbOamAisDisable(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamAisConfigGet(uint32_t lmepId, ofdpaOamAisConfig_t *config);

OFDPA_ERROR_t ofdbOamLckEnable(uint32_t lmepId, ofdpaOamLckConfig_t *config);
OFDPA_ERROR_t ofdbOamLckDisable(uint32_t lmepId);
OFDPA_ERROR_t ofdbOamLckConfigGet(uint32_t lmepId, ofdpaOamLckConfig_t *config);
OFDPA_ERROR_t ofdbOamEventNotify(uint32_t flags,uint32_t event_type,uint32_t group,uint32_t endpoint);


OFDPA_ERROR_t ofdbBfdProtectionGet(uint32_t mlpGroupIndex, uint32_t lmepId);
OFDPA_ERROR_t ofdbBfdProtectionAdd(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg);
OFDPA_ERROR_t ofdbBfdProtectionDelete(uint32_t mlpGroupIndex, uint32_t lmepId);
OFDPA_ERROR_t ofdbBfdProtectionNextGet(uint32_t mlpGroupIndex, uint32_t lmepId, uint32_t *nextLmepId);
int ofdbBfdProtectionValidate(uint32_t mlpGroupIndex, uint32_t lmepId, ofdpaMlpheadendcfg_t *mlpcfg);

OFDPA_ERROR_t ofdbPwRedunAdd(uint32_t index, ofdpaPwRedunConfig_t *config);
OFDPA_ERROR_t ofdbPwRedunDelete(uint32_t index);
OFDPA_ERROR_t ofdbPwRedunGet(uint32_t index, ofdpaPwRedunConfig_t *config, ofdpaPwRedunStatus_t *status);
OFDPA_ERROR_t ofdbPwRedunNextGet(uint32_t index, uint32_t *nextIndex);
int ofdbPwRedunConfigValidate(uint32_t index, ofdpaPwRedunConfig_t *config);
OFDPA_ERROR_t ofdbOamTypeGet(uint32_t lmepId, OFDB_OAM_TYPE_t* pOamtype);

int ofdbY1564ConfigValidate(uint32_t lmepId);
OFDPA_ERROR_t ofdbY1564Add(uint32_t lmepId);
OFDPA_ERROR_t ofdbY1564Delete(uint32_t lmepId);
OFDPA_ERROR_t ofdbY1564Get(uint32_t lmepId, ofdpaY1564Status_t *status);
OFDPA_ERROR_t ofdbY1564NextGet(uint32_t lmepId, uint32_t *nextLmepId);
OFDPA_ERROR_t ofdbY1564ReferenceUpdate(uint32_t lmepId, uint32_t increment);

OFDPA_ERROR_t ofdbLoopbackAdd(uint32_t lmepId);
OFDPA_ERROR_t ofdbLoopbackDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbLoopbackGet(uint32_t lmepId, ofdpaY1564Status_t *status);
OFDPA_ERROR_t ofdbLoopbackNextGet(uint32_t lmepId, uint32_t *nextLmepId);
OFDPA_ERROR_t ofdbY1564ReferenceUpdate(uint32_t lmepId, uint32_t increment);

OFDPA_ERROR_t ofdbLmepidAdd(uint32_t lmepId, OFDB_OAM_TYPE_t oamtype);
OFDPA_ERROR_t ofdbLmepidDelete(uint32_t lmepId);
OFDPA_ERROR_t ofdbLmepidGet(uint32_t lmepId, OFDB_OAM_TYPE_t *oamType);


#endif /* INCLUDE_OFDB_API_H */
