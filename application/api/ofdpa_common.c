/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2003-2016
*
**********************************************************************
*
* @filename     ofdpa_common.c
*
* @purpose      OF-DPA common non-RPC API implementation
*
* @component    OF-DPA
*
* @comments     This file is used by both client and server.
*
* @create       17 May 2013
*
* @end
*
**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ofdpa_api.h"
#include "ofdb_api.h"
#include "datapath_api.h"

typedef enum
{
  OFPP_IN_PORT    = 0xfffffff8u,  /* Send the packet out the input port.  This
                                     reserved port must be explicitly used
                                     in order to send back out of the input
                                     port. */
  OFPP_TABLE      = 0xfffffff9u,  /* Submit the packet to the first flow table
                                     NB: This destination port can only be
                                     used in packet-out messages. */
  OFPP_NORMAL     = 0xfffffffau,  /* Forward using non-OpenFlow pipeline. */
  OFPP_FLOOD      = 0xfffffffbu,  /* Flood using non-OpenFlow pipeline. */
  OFPP_ALL        = 0xfffffffcu,  /* All standard ports except input port. */
  OFPP_CONTROLLER = 0xfffffffdu,  /* Send to controller. */
  OFPP_LOCAL      = 0xfffffffeu,  /* Local openflow "port". */
  OFPP_ANY        = 0xffffffffu   /* Special value used in some requests when
                                     no port is specified (i.e. wildcarded). */
} of_reserved_ports_t;

typedef struct
{
  OFDPA_FLOW_TABLE_ID_t type;
  char *name;
} tableNameList_t;

static tableNameList_t tableNameList[] =
{
  { OFDPA_FLOW_TABLE_ID_INGRESS_PORT,             "Ingress Port"},
  { OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST,          "Port DSCP Trust"},
  { OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST,           "Port PCP Trust"},
  { OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST,        "Tunnel DSCP Trust"},
  { OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST,         "Tunnel PCP Trust"},
  { OFDPA_FLOW_TABLE_ID_INJECTED_OAM,             "Injected OAM"},
  { OFDPA_FLOW_TABLE_ID_VLAN,                     "VLAN"},
  { OFDPA_FLOW_TABLE_ID_VLAN_1,                   "VLAN 1"},
  { OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT,        "Ingress Maintenance Point"},
  { OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT,             "MPLS L2 Port"},
  { OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST,          "MPLS DSCP Trust"},
  { OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST,           "MPLS PCP Trust"},
  { OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS,           "MPLS L2 Port QoS Class"},
  { OFDPA_FLOW_TABLE_ID_L2_POLICER,               "L2 Policer"},
  { OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS,       "L2 Policer Actions"},
  { OFDPA_FLOW_TABLE_ID_TERMINATION_MAC,          "Termination MAC"},
  { OFDPA_FLOW_TABLE_ID_L3_TYPE,                  "L3 Type"},
  { OFDPA_FLOW_TABLE_ID_MPLS_0,                   "MPLS 0"},
  { OFDPA_FLOW_TABLE_ID_MPLS_1,                   "MPLS 1"},
  { OFDPA_FLOW_TABLE_ID_MPLS_2,                   "MPLS 2"},
  { OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT,   "MPLS Maintenance Point"},
  { OFDPA_FLOW_TABLE_ID_MPLS_L3_TYPE,             "MPLS L3 Type"},
  { OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST,         "MPLS Label Trust"},
  { OFDPA_FLOW_TABLE_ID_MPLS_TYPE,                "MPLS Type"},
  { OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING,          "Unicast Routing"},
  { OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING,        "Multicast Routing"},
  { OFDPA_FLOW_TABLE_ID_BRIDGING,                 "Bridging"},
  { OFDPA_FLOW_TABLE_ID_ACL_POLICY,               "ACL Policy"},
  { OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS,      "Color Based Actions"},
  { OFDPA_FLOW_TABLE_ID_EGRESS_VLAN,              "Egress VLAN"},
  { OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1,            "Egress VLAN 1"},
  { OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT, "Egress Maintenance Point"},
  { OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK,   "Egress DSCP PCP Remark"},
  { OFDPA_FLOW_TABLE_ID_EGRESS_TPID,              "Egress TPID"},
};
#define TABLE_NAME_LIST_SIZE (sizeof(tableNameList)/sizeof(tableNameList[0]))

static char *groupTypeNames[OFDPA_GROUP_ENTRY_TYPE_LAST] =
{
  [OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE           ] = "L2 Interface",
  [OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE             ] = "L2 Rewrite",
  [OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST             ] = "L3 Unicast",
  [OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST           ] = "L2 Multicast",
  [OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD               ] = "L2 Flood",
  [OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE           ] = "L3 Interface",
  [OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST           ] = "L3 Multicast",
  [OFDPA_GROUP_ENTRY_TYPE_L3_ECMP                ] = "L3 ECMP",
  [OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY             ] = "L2 Overlay",
  [OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL             ] = "MPLS Label",
  [OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING        ] = "MPLS Forwarding",
  [OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE] = "L2 Unfiltered Interface",
};

static char *overlaySubTypeNames[] =
{
  [OFDPA_L2_OVERLAY_FLOOD_UNICAST_TUNNEL      ] = "Flood over Unicast",
  [OFDPA_L2_OVERLAY_FLOOD_MULTICAST_TUNNEL    ] = "Flood over Multicast",
  [OFDPA_L2_OVERLAY_MULTICAST_UNICAST_TUNNEL  ] = "Multicast over Unicast",
  [OFDPA_L2_OVERLAY_MULTICAST_MULTICAST_TUNNEL] = "Multicast over Multicast",
};

static char *mplsLabelSubTypeNames[] =
{
  [OFDPA_MPLS_INTERFACE    ] = "MPLS Interface",
  [OFDPA_MPLS_L2_VPN_LABEL ] = "MPLS L2 VPN Label",
  [OFDPA_MPLS_L3_VPN_LABEL ] = "MPLS L3 VPN Label",
  [OFDPA_MPLS_TUNNEL_LABEL1] = "MPLS Tunnel Label1",
  [OFDPA_MPLS_TUNNEL_LABEL2] = "MPLS Tunnel Label2",
  [OFDPA_MPLS_SWAP_LABEL   ] = "MPLS Swap Label",
};

static char *mplsForwardingSubTypeNames[] =
{
  [OFDPA_MPLS_L2_FLOOD                  ] = "MPLS L2 Flood",
  [OFDPA_MPLS_L2_MULTICAST              ] = "MPLS L2 Multicast",
  [OFDPA_MPLS_L2_LOCAL_FLOOD            ] = "MPLS L2 Local Flood",
  [OFDPA_MPLS_L2_LOCAL_MULTICAST        ] = "MPLS L2 Local Multicast",
  [OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON    ] = "MPLS L2 Flood Split Horizon",
  [OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON] = "MPLS L2 Multicast Split Horizon",
  [OFDPA_MPLS_FAST_FAILOVER             ] = "MPLS Fast Failover",
  [OFDPA_MPLS_1_1_HEAD_END_PROTECT      ] = "MPLS 1:1 Head End Protect",
  [OFDPA_MPLS_ECMP                      ] = "MPLS ECMP",
  [OFDPA_MPLS_L2_TAG                    ] = "MPLS L2 Tag",
};

static char *mplsTypeSubTypeName[] =
{
  [OFDPA_MPLS_TYPE_VPWS         ] = "VPWS",
  [OFDPA_MPLS_TYPE_VPLS         ] = "VPLS",
  [OFDPA_MPLS_TYPE_OAM          ] = "OAM",
  [OFDPA_MPLS_TYPE_L3_UNICAST   ] = "L3 Route Unicast",
  [OFDPA_MPLS_TYPE_L3_MULTICAST ] = "L3 Route Multicast",
  [OFDPA_MPLS_TYPE_L3_PHP       ] = "L3 PHP",
};

typedef struct
{
  OFDPA_ACTION_COLOR_BASED_COUNT_t type;
  char *name;
} colorCountActionNameList_t;

static colorCountActionNameList_t colorCountActionNameList[] =
{
  { OFDPA_ACTION_COLOR_BASED_COUNT_NONE, "None"},
  { OFDPA_ACTION_COLOR_BASED_COUNT,      "VC"},
  { OFDPA_ACTION_COLOR_BASED_COUNT_1,    "Tunnel 1"},
  { OFDPA_ACTION_COLOR_BASED_COUNT_2,    "Tunnel 2"},
  { OFDPA_ACTION_COLOR_BASED_COUNT_3,    "Section"},
};
#define COLOR_BASED_COUNT_ACTION_NAME_LIST_SIZE (sizeof(colorCountActionNameList)/sizeof(colorCountActionNameList[0]))

OFDPA_ERROR_t ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_t tableId, ofdpaFlowEntry_t *flow)
{
  if (flow == NULL)
  {
    return OFDPA_E_PARAM;
  }

  memset(flow, 0, sizeof(ofdpaFlowEntry_t));
  flow->tableId = tableId;

  /* per flow table entry cases where default member values should not be 0 are handled here */
  switch (tableId)
  {
    /*
     * Insert case statements to update flow entry members to correct values when 0 is
     * not the appropriate default.
     */
    default:
      break;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaFlowEntryInstructionsInit(ofdpaFlowEntry_t *flow)
{
  OFDPA_ERROR_t rc = OFDPA_E_FAIL;
  ofdpaFlowEntry_t flowCopy;

  if (flow == NULL)
  {
    return OFDPA_E_PARAM;
  }

  /* save current flow content */
  memcpy(&flowCopy, flow, sizeof(flowCopy));
  /* initialize the flow table entry */
  ofdpaFlowEntryInit(flow->tableId, flow);

  /* restore fields common to all flow entry types */
  flow->tableId = flowCopy.tableId;
  flow->priority = flowCopy.priority;
  flow->hard_time = flowCopy.hard_time;
  flow->idle_time = flowCopy.idle_time;
  flow->cookie = flowCopy.cookie;

  /* match criteria are restored based on type */
  switch (flow->tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      {
        memcpy(&flow->flowData.ingressPortFlowEntry.match_criteria,
               &flowCopy.flowData.ingressPortFlowEntry.match_criteria,
               sizeof(flow->flowData.ingressPortFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST:
      {
        memcpy(&flow->flowData.dscpTrustFlowEntry.match_criteria,
               &flowCopy.flowData.dscpTrustFlowEntry.match_criteria,
               sizeof(flow->flowData.dscpTrustFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_MPLS_PCP_TRUST:
      {
        memcpy(&flow->flowData.pcpTrustFlowEntry.match_criteria,
               &flowCopy.flowData.pcpTrustFlowEntry.match_criteria,
               sizeof(flow->flowData.pcpTrustFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
      {
        memcpy(&flow->flowData.injectedOamFlowEntry.match_criteria,
               &flowCopy.flowData.injectedOamFlowEntry.match_criteria,
               sizeof(flow->flowData.injectedOamFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_VLAN:
      {
        memcpy(&flow->flowData.vlanFlowEntry.match_criteria,
               &flowCopy.flowData.vlanFlowEntry.match_criteria,
               sizeof(flow->flowData.vlanFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_VLAN_1:
      {
        memcpy(&flow->flowData.vlan1FlowEntry.match_criteria,
               &flowCopy.flowData.vlan1FlowEntry.match_criteria,
               sizeof(flow->flowData.vlan1FlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      {
        memcpy(&flow->flowData.mpFlowEntry.match_criteria,
               &flowCopy.flowData.mpFlowEntry.match_criteria,
               sizeof(flow->flowData.mpFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      {
        memcpy(&flow->flowData.mplsL2PortFlowEntry.match_criteria,
               &flowCopy.flowData.mplsL2PortFlowEntry.match_criteria,
               sizeof(flow->flowData.mplsL2PortFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
      {
        memcpy(&flow->flowData.mplsL2PortQosFlowEntry.match_criteria,
               &flowCopy.flowData.mplsL2PortQosFlowEntry.match_criteria,
               sizeof(flow->flowData.mplsL2PortQosFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
      {
        memcpy(&flow->flowData.l2PolicerFlowEntry.match_criteria,
               &flowCopy.flowData.l2PolicerFlowEntry.match_criteria,
               sizeof(flow->flowData.l2PolicerFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
      {
        memcpy(&flow->flowData.l2PolicerActionsFlowEntry.match_criteria,
               &flowCopy.flowData.l2PolicerActionsFlowEntry.match_criteria,
               sizeof(flow->flowData.l2PolicerActionsFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      {
        memcpy(&flow->flowData.terminationMacFlowEntry.match_criteria,
               &flowCopy.flowData.terminationMacFlowEntry.match_criteria,
               sizeof(flow->flowData.terminationMacFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_MPLS_0:
    case OFDPA_FLOW_TABLE_ID_MPLS_1:
      {
        memcpy(&flow->flowData.mplsFlowEntry.match_criteria,
               &flowCopy.flowData.mplsFlowEntry.match_criteria,
               sizeof(flow->flowData.mplsFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
      {
        memcpy(&flow->flowData.mplsMpFlowEntry.match_criteria,
               &flowCopy.flowData.mplsMpFlowEntry.match_criteria,
               sizeof(flow->flowData.mplsMpFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
      {
        memcpy(&flow->flowData.mplsLabelTrustFlowEntry.match_criteria,
               &flowCopy.flowData.mplsLabelTrustFlowEntry.match_criteria,
               sizeof(flow->flowData.mplsLabelTrustFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      {
        memcpy(&flow->flowData.unicastRoutingFlowEntry.match_criteria,
               &flowCopy.flowData.unicastRoutingFlowEntry.match_criteria,
               sizeof(flow->flowData.unicastRoutingFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      {
        memcpy(&flow->flowData.multicastRoutingFlowEntry.match_criteria,
               &flowCopy.flowData.multicastRoutingFlowEntry.match_criteria,
               sizeof(flow->flowData.multicastRoutingFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      {
        memcpy(&flow->flowData.bridgingFlowEntry.match_criteria,
               &flowCopy.flowData.bridgingFlowEntry.match_criteria,
               sizeof(flow->flowData.bridgingFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      {
        memcpy(&flow->flowData.policyAclFlowEntry.match_criteria,
               &flowCopy.flowData.policyAclFlowEntry.match_criteria,
               sizeof(flow->flowData.policyAclFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
      {
        memcpy(&flow->flowData.colorActionsFlowEntry.match_criteria,
               &flowCopy.flowData.colorActionsFlowEntry.match_criteria,
               sizeof(flow->flowData.colorActionsFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
      {
        memcpy(&flow->flowData.egressVlanFlowEntry.match_criteria,
               &flowCopy.flowData.egressVlanFlowEntry.match_criteria,
               sizeof(flow->flowData.egressVlanFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
      {
        memcpy(&flow->flowData.egressVlan1FlowEntry.match_criteria,
               &flowCopy.flowData.egressVlan1FlowEntry.match_criteria,
               sizeof(flow->flowData.egressVlan1FlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
      {
        memcpy(&flow->flowData.egressMpFlowEntry.match_criteria,
               &flowCopy.flowData.egressMpFlowEntry.match_criteria,
               sizeof(flow->flowData.egressMpFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
      {
        memcpy(&flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria,
               &flowCopy.flowData.egressDscpPcpRemarkFlowEntry.match_criteria,
               sizeof(flow->flowData.egressDscpPcpRemarkFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
      {
        memcpy(&flow->flowData.egressTpidFlowEntry.match_criteria,
               &flowCopy.flowData.egressTpidFlowEntry.match_criteria,
               sizeof(flow->flowData.egressTpidFlowEntry.match_criteria));
        rc = OFDPA_E_NONE;
      }
      break;
    default:
      break;
  }

  return(rc);
}

OFDPA_ERROR_t ofdpaGroupEntryInit(OFDPA_GROUP_ENTRY_TYPE_t groupType, ofdpaGroupEntry_t *group)
{
  if (group == NULL)
  {
    return OFDPA_E_PARAM;
  }

  if (groupType >= OFDPA_GROUP_ENTRY_TYPE_LAST)
  {
    return OFDPA_E_PARAM;
  }

  memset(group, 0, sizeof(ofdpaGroupEntry_t));

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaGroupBucketEntryInit(OFDPA_GROUP_ENTRY_TYPE_t groupType, ofdpaGroupBucketEntry_t *bucket)
{
  if (bucket == NULL)
  {
    return OFDPA_E_PARAM;
  }

  if (groupType >= OFDPA_GROUP_ENTRY_TYPE_LAST)
  {
    return OFDPA_E_PARAM;
  }

  memset(bucket, 0, sizeof(ofdpaGroupBucketEntry_t));

  return OFDPA_E_NONE;
}

/* APIs for getting/setting group IDs */
/* For all Group Types */
OFDPA_ERROR_t ofdpaGroupTypeGet(uint32_t groupId, uint32_t *type)
{
  *type = OFDB_GROUP_TYPE(groupId);

  return OFDPA_E_NONE;
}

/* For Group Types L2 Interface, L2 Multicast, L2 Flood and L3 Multicast */
OFDPA_ERROR_t ofdpaGroupVlanGet(uint32_t groupId, uint32_t *vlanId)
{
  switch (OFDB_GROUP_TYPE(groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      *vlanId = OFDB_GROUP_VLANID(groupId);
      break;

    default:
      return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

/* For Group Types L2 Interface */
OFDPA_ERROR_t ofdpaGroupPortIdGet(uint32_t groupId, uint32_t *portId)
{
  switch (OFDB_GROUP_TYPE(groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
      *portId = OFDB_GROUP_PORTID(groupId);
      break;

    default:
      return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaGroupIndexGet(uint32_t groupId, uint32_t *index)
{
  switch (OFDB_GROUP_TYPE(groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      *index = OFDB_GROUP_SHORT_INDEX(groupId);
      break;
    case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      *index = OFDB_GROUP_INDEX(groupId);
      break;
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
      switch (OFDB_GROUP_MPLS_SUBTYPE(groupId))
      {
        case OFDPA_MPLS_INTERFACE:
        case OFDPA_MPLS_L2_VPN_LABEL:
        case OFDPA_MPLS_L3_VPN_LABEL:
        case OFDPA_MPLS_TUNNEL_LABEL1:
        case OFDPA_MPLS_TUNNEL_LABEL2:
        case OFDPA_MPLS_SWAP_LABEL:
          *index = OFDB_GROUP_MPLS_INDEX(groupId);
          break;
        default:
          return OFDPA_E_UNAVAIL;
      }
      break;
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      switch (OFDB_GROUP_MPLS_SUBTYPE(groupId))
      {
        case OFDPA_MPLS_FAST_FAILOVER:
        case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        case OFDPA_MPLS_L2_TAG:
        case OFDPA_MPLS_ECMP:
          *index = OFDB_GROUP_MPLS_INDEX(groupId);
          break;
        case OFDPA_MPLS_L2_FLOOD:
        case OFDPA_MPLS_L2_MULTICAST:
        case OFDPA_MPLS_L2_LOCAL_FLOOD:
        case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
        case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
          *index = OFDB_GROUP_MPLS_L2_INDEX(groupId);
          break;
        default:
          return OFDPA_E_UNAVAIL;
      }
      break;

    default:
      return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

/* For all Group Types */
OFDPA_ERROR_t ofdpaGroupTypeSet(uint32_t *groupId, uint32_t type)
{
  *groupId = OFDB_GROUP_TYPE_SET(*groupId, type);

  return OFDPA_E_NONE;
}

/* For Group Types L2 Interface, L2 Multicast, L2 Flood and L3 Multicast */
OFDPA_ERROR_t ofdpaGroupVlanSet(uint32_t *groupId, uint32_t vlanId)
{
  switch (OFDB_GROUP_TYPE(*groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      *groupId = OFDB_GROUP_VLANID_SET(*groupId, vlanId);
      break;

    default:
      return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

/* For Group Type L2 Overlay */
OFDPA_ERROR_t ofdpaGroupOverlayTunnelIdGet(uint32_t groupId, uint32_t *tunnelId)
{
  if (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN))
  {
    switch (OFDB_GROUP_TYPE(groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        *tunnelId = OFDB_GROUP_TUNNELID(groupId);
        break;

      default:
        return OFDPA_E_UNAVAIL;
    }

    return OFDPA_E_NONE;
  }
  else
  {
    return OFDPA_E_UNAVAIL;
  }
}

OFDPA_ERROR_t ofdpaGroupOverlayTunnelIdSet(uint32_t *groupId, uint32_t tunnelId)
{
  if (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN))
  {
    switch (OFDB_GROUP_TYPE(*groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        *groupId = OFDB_GROUP_TUNNELID_SET(*groupId, tunnelId);
        break;

      default:
        return OFDPA_E_UNAVAIL;
    }

    return OFDPA_E_NONE;
  }
  else
  {
    return OFDPA_E_UNAVAIL;
  }
}

OFDPA_ERROR_t ofdpaGroupOverlaySubTypeGet(uint32_t groupId, OFDPA_L2_OVERLAY_SUBTYPE_t *subType)
{
  if (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN))
  {
    switch (OFDB_GROUP_TYPE(groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        *subType = OFDB_GROUP_TUNNEL_SUBTYPE(groupId);
        break;

      default:
        return OFDPA_E_UNAVAIL;
    }

    return OFDPA_E_NONE;
  }
  else
  {
    return OFDPA_E_UNAVAIL;
  }

}

OFDPA_ERROR_t ofdpaGroupOverlaySubTypeSet(uint32_t *groupId, OFDPA_L2_OVERLAY_SUBTYPE_t subType)
{
  if (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN))
  {
    switch (OFDB_GROUP_TYPE(*groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        *groupId = OFDB_GROUP_TUNNEL_SUBTYPE_SET(*groupId, subType);
        break;

      default:
        return OFDPA_E_UNAVAIL;
    }

    return OFDPA_E_NONE;
  }
  else
  {
    return OFDPA_E_UNAVAIL;
  }

}

OFDPA_ERROR_t ofdpaGroupOverlayIndexGet(uint32_t groupId, uint32_t *index)
{
  if (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN))
  {
    switch (OFDB_GROUP_TYPE(groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        *index = OFDB_GROUP_TUNNEL_INDEX(groupId);
        break;

      default:
        return OFDPA_E_UNAVAIL;
    }

    return OFDPA_E_NONE;
  }
  else
  {
    return OFDPA_E_UNAVAIL;
  }
}

OFDPA_ERROR_t ofdpaGroupOverlayIndexSet(uint32_t *groupId, uint32_t index)
{
  if (OFDPA_E_NONE == ofdpaFeatureSupported(OFDPA_FEATURE_VXLAN))
  {
    switch (OFDB_GROUP_TYPE(*groupId))
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        *groupId = OFDB_GROUP_TUNNEL_INDEX_SET(*groupId, index);
        break;

      default:
        return OFDPA_E_UNAVAIL;
    }

    return OFDPA_E_NONE;
  }
  else
  {
    return OFDPA_E_UNAVAIL;
  }

}

/* For Group Types L2 Interface */
OFDPA_ERROR_t ofdpaGroupPortIdSet(uint32_t *groupId, uint32_t portId)
{
  switch (OFDB_GROUP_TYPE(*groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
      *groupId = OFDB_GROUP_PORTID_SET(*groupId, portId);
      break;

    default:
      return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaGroupIndexSet(uint32_t *groupId, uint32_t index)
{
  switch (OFDB_GROUP_TYPE(*groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      *groupId = OFDB_GROUP_SHORT_INDEX_SET(*groupId, index);
      break;
    case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
      *groupId = OFDB_GROUP_INDEX_SET(*groupId, index);
      break;
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
      switch (OFDB_GROUP_MPLS_SUBTYPE(*groupId))
      {
        case OFDPA_MPLS_INTERFACE:
        case OFDPA_MPLS_L2_VPN_LABEL:
        case OFDPA_MPLS_L3_VPN_LABEL:
        case OFDPA_MPLS_TUNNEL_LABEL1:
        case OFDPA_MPLS_TUNNEL_LABEL2:
        case OFDPA_MPLS_SWAP_LABEL:
          *groupId = OFDB_GROUP_MPLS_INDEX_SET(*groupId, index);
          break;
        default:
          return OFDPA_E_UNAVAIL;
      }
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      switch (OFDB_GROUP_MPLS_SUBTYPE(*groupId))
      {
        case OFDPA_MPLS_FAST_FAILOVER:
        case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        case OFDPA_MPLS_L2_TAG:
        case OFDPA_MPLS_ECMP:
          *groupId = OFDB_GROUP_MPLS_INDEX_SET(*groupId, index);
          break;
        case OFDPA_MPLS_L2_FLOOD:
        case OFDPA_MPLS_L2_MULTICAST:
        case OFDPA_MPLS_L2_LOCAL_FLOOD:
        case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
        case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
          *groupId = OFDB_GROUP_MPLS_L2_INDEX_SET(*groupId, index);
          break;
        default:
          return OFDPA_E_UNAVAIL;
      }
      break;

    default:
      return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaGroupMplsSubTypeGet(uint32_t groupId, uint32_t *subType)
{
  switch (OFDB_GROUP_TYPE(groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      *subType = OFDB_GROUP_MPLS_SUBTYPE(groupId);
      break;

    default:
      return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaGroupMplsSubTypeSet(uint32_t *groupId, uint32_t subType)
{
  switch (OFDB_GROUP_TYPE(*groupId))
  {
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      *groupId = OFDB_GROUP_MPLS_SUBTYPE_SET(*groupId, subType);
      break;

    default:
      return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

/* For MPLS Group Sub-types L2 Flood and L2 Multicast */
OFDPA_ERROR_t ofdpaGroupMplsL2TunnelIdGet(uint32_t groupId, uint32_t *tunnelId)
{
  if (OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING == OFDB_GROUP_TYPE(groupId) &&
      ((OFDPA_MPLS_L2_FLOOD == OFDB_GROUP_MPLS_SUBTYPE(groupId)) ||
       (OFDPA_MPLS_L2_MULTICAST == OFDB_GROUP_MPLS_SUBTYPE(groupId)) ||
       (OFDPA_MPLS_L2_LOCAL_FLOOD == OFDB_GROUP_MPLS_SUBTYPE(groupId)) ||
       (OFDPA_MPLS_L2_LOCAL_MULTICAST == OFDB_GROUP_MPLS_SUBTYPE(groupId)) ||
       (OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON == OFDB_GROUP_MPLS_SUBTYPE(groupId)) ||
       (OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON == OFDB_GROUP_MPLS_SUBTYPE(groupId))))
  {
    *tunnelId = OFDB_GROUP_MPLS_L2_TUNNELID(groupId);
  }
  else
  {
    return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaGroupMplsL2TunnelIdSet(uint32_t *groupId, uint32_t tunnelId)
{
  if (OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING == OFDB_GROUP_TYPE(*groupId) &&
      ((OFDPA_MPLS_L2_FLOOD == OFDB_GROUP_MPLS_SUBTYPE(*groupId)) ||
       (OFDPA_MPLS_L2_MULTICAST == OFDB_GROUP_MPLS_SUBTYPE(*groupId)) ||
       (OFDPA_MPLS_L2_LOCAL_FLOOD == OFDB_GROUP_MPLS_SUBTYPE(*groupId)) ||
       (OFDPA_MPLS_L2_LOCAL_MULTICAST == OFDB_GROUP_MPLS_SUBTYPE(*groupId)) ||
       (OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON == OFDB_GROUP_MPLS_SUBTYPE(*groupId)) ||
       (OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON == OFDB_GROUP_MPLS_SUBTYPE(*groupId))))
  {
    *groupId = OFDB_GROUP_MPLS_L2_TUNNELID_SET(*groupId, tunnelId);
  }
  else
  {
    return OFDPA_E_UNAVAIL;
  }

  return OFDPA_E_NONE;
}

/* APIs for getting/setting Port IDs */
void ofdpaPortTypeGet(uint32_t portNum, uint32_t *type)
{
  *type = OFDB_PORT_TYPE(portNum);
}

void ofdpaPortIndexGet(uint32_t portNum, uint32_t *index)
{
  *index = OFDB_PORT_INDEX(portNum);
}

void ofdpaPortTypeSet(uint32_t *portNum, uint32_t type)
{
  *portNum = OFDB_PORT_TYPE_SET(*portNum, type);
}

void ofdpaPortIndexSet(uint32_t *portNum, uint32_t index)
{
  *portNum = OFDB_PORT_INDEX_SET(*portNum, index);
}

/* APIs for getting/setting MPLS L2 Port IDs */
void ofdpaMplsL2PortTypeGet(uint32_t portNum, uint32_t *type)
{
  *type = OFDB_MPLS_L2_PORT_TYPE(portNum);
}

void ofdpaMplsL2PortIndexGet(uint32_t portNum, uint32_t *index)
{
  *index = OFDB_MPLS_L2_PORT_INDEX(portNum);
}

void ofdpaMplsL2PortTypeSet(uint32_t *portNum, uint32_t type)
{
  *portNum = OFDB_MPLS_L2_PORT_TYPE_SET(*portNum, type);
}

void ofdpaMplsL2PortIndexSet(uint32_t *portNum, uint32_t index)
{
  *portNum = OFDB_MPLS_L2_PORT_INDEX_SET(*portNum, index);
}

/* APIs for getting/setting Tunnel IDs */
void ofdpaTunnelIdTypeGet(uint32_t tunnelId, uint32_t *type)
{
  *type = OFDB_TUNNELID_TYPE(tunnelId);
}

void ofdpaTunnelIdIndexGet(uint32_t tunnelId, uint32_t *index)
{
  *index = OFDB_TUNNELID_INDEX(tunnelId);
}

void ofdpaTunnelIdTypeSet(uint32_t *tunnelId, uint32_t type)
{
  *tunnelId = OFDB_TUNNELID_TYPE_SET(*tunnelId, type);
}

void ofdpaTunnelIdIndexSet(uint32_t *tunnelId, uint32_t index)
{
  *tunnelId = OFDB_TUNNELID_INDEX_SET(*tunnelId, index);
}

OFDPA_ERROR_t ofdpaPortDecode(char *outBuf, int bufSize, int *count, uint32_t port)
{
  uint32_t portType;
  uint32_t portIndex;

  ofdpaPortTypeGet(port, &portType);
  ofdpaPortIndexGet(port, &portIndex);

  switch (portType)
  {
    case OFDPA_PORT_TYPE_PHYSICAL:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "%d (Physical) ", portIndex);
      break;
    case OFDPA_PORT_TYPE_LOGICAL_TUNNEL:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "%d (Tunnel) ", portIndex);
      break;
    case OFDPA_PORT_TYPE_OAM_PROTECTION_LIVENESS_LOGICAL_PORT:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "%d (Liveness) ", portIndex);
      break;
    default:
      switch (port)
      {
        case OFPP_IN_PORT:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "IN_PORT (Reserved) ");
          break;
        case OFPP_TABLE:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "TABLE (Reserved) ");
          break;
        case OFPP_NORMAL:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "NORMAL (Reserved) ");
          break;
        case OFPP_FLOOD:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "FLOOD (Reserved) ");
          break;
        case OFPP_ALL:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "ALL (Reserved) ");
          break;
        case OFPP_CONTROLLER:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "CONTROLLER (Reserved) ");
          break;
        case OFPP_LOCAL:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "LOCAL (Reserved) ");
          break;
        case OFPP_ANY:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "ANY (Reserved) ");
          break;
        default:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, *count, "0x%x (Unknown) ", port);
          break;
      }
      break;
  }

  return OFDPA_E_NONE;
}

OFDPA_ERROR_t ofdpaGroupDecode(uint32_t groupId, char *outBuf, int bufSize)
{
  int  groupType = OFDB_GROUP_TYPE(groupId);
  int  count = 0;

  APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "%s", groupTypeNames[groupType]);

  switch (groupType)
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", VLAN ID = %d", OFDB_GROUP_VLANID(groupId));
      if (OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE == groupType)
      {
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Port ID = %d", OFDB_GROUP_PORTID(groupId));
      }
      else
      {
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Index = %d", OFDB_GROUP_SHORT_INDEX(groupId));
      }
      break;

    case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Tunnel ID = %d", OFDB_GROUP_TUNNELID(groupId));
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Sub-type = %s", overlaySubTypeNames[OFDB_GROUP_TUNNEL_SUBTYPE(groupId)]);
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Index = %d", OFDB_GROUP_TUNNEL_INDEX(groupId));
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Sub-type = %s", mplsLabelSubTypeNames[OFDB_GROUP_MPLS_SUBTYPE(groupId)]);
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Index = %d", OFDB_GROUP_MPLS_INDEX(groupId));
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      switch (OFDB_GROUP_MPLS_SUBTYPE(groupId))
      {
        case OFDPA_MPLS_FAST_FAILOVER:
        case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        case OFDPA_MPLS_L2_TAG:
        case OFDPA_MPLS_ECMP:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Sub-type = %s", mplsForwardingSubTypeNames[OFDB_GROUP_MPLS_SUBTYPE(groupId)]);
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Index = %d", OFDB_GROUP_MPLS_INDEX(groupId));
          break;

        case OFDPA_MPLS_L2_FLOOD:
        case OFDPA_MPLS_L2_MULTICAST:
        case OFDPA_MPLS_L2_LOCAL_FLOOD:
        case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
        case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Tunnel ID = %d", OFDB_GROUP_MPLS_L2_TUNNELID(groupId));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Sub-type = %s", mplsForwardingSubTypeNames[OFDB_GROUP_MPLS_SUBTYPE(groupId)]);
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Index = %d", OFDB_GROUP_MPLS_L2_INDEX(groupId));
          break;

        default:
          break;
      }
      break;

    default:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Index = %d", OFDB_GROUP_INDEX(groupId));
      break;
  }

  return OFDPA_E_NONE;
}


OFDPA_ERROR_t ofdpaGroupDecodeShort(uint32_t groupId, char *outBuf, int bufSize)
{
  int  groupType = OFDB_GROUP_TYPE(groupId);
  int  count = 0;

  APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "%s", groupTypeNames[groupType]);

  switch (groupType)
  {
    case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
    case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
    case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
    case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:  
      if (OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE == groupType)
      {      
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", VLAN=%d", OFDB_GROUP_VLANID(groupId));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Port=%d", OFDB_GROUP_PORTID(groupId));
      }

      break;	
     

    case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Tunnel ID = %d", OFDB_GROUP_TUNNELID(groupId));
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Sub-type = %s", overlaySubTypeNames[OFDB_GROUP_TUNNEL_SUBTYPE(groupId)]);
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Index = %d", OFDB_GROUP_TUNNEL_INDEX(groupId));
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "(%s)", mplsLabelSubTypeNames[OFDB_GROUP_MPLS_SUBTYPE(groupId)]);
      break;

    case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
      switch (OFDB_GROUP_MPLS_SUBTYPE(groupId))
      {
        case OFDPA_MPLS_FAST_FAILOVER:
        case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
        case OFDPA_MPLS_L2_TAG:
        case OFDPA_MPLS_ECMP:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "(%s)", mplsForwardingSubTypeNames[OFDB_GROUP_MPLS_SUBTYPE(groupId)]);
          break;

        case OFDPA_MPLS_L2_FLOOD:
        case OFDPA_MPLS_L2_MULTICAST:
        case OFDPA_MPLS_L2_LOCAL_FLOOD:
        case OFDPA_MPLS_L2_LOCAL_MULTICAST:
        case OFDPA_MPLS_L2_FLOOD_SPLIT_HORIZON:
        case OFDPA_MPLS_L2_MULTICAST_SPLIT_HORIZON:
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Tunnel ID = %d", OFDB_GROUP_MPLS_L2_TUNNELID(groupId));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Sub-type = %s", mplsForwardingSubTypeNames[OFDB_GROUP_MPLS_SUBTYPE(groupId)]);
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Index = %d", OFDB_GROUP_MPLS_L2_INDEX(groupId));
          break;

        default:
          break;
      }
      break;

    default:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, ", Index = %d", OFDB_GROUP_INDEX(groupId));
      break;
  }

  return OFDPA_E_NONE;
}


OFDPA_ERROR_t ofdpaGroupBucketEntryDecode(ofdpaGroupBucketEntry_t *bucketEntry, char *outBuf, int bufSize)
{
#if 0
  OFDPA_ERROR_t rc;
  OFDPA_GROUP_ENTRY_TYPE_t groupType;
  uint32_t subType;
  uint32_t count = 0;

  rc = ofdpaGroupTypeGet(bucketEntry->groupId, &groupType);
  if (rc != OFDPA_E_NONE)
  {
    rc = OFDPA_E_ERROR;
  }
  else
  {
    APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "bucketIndex = %d: ", bucketEntry->bucketIndex);

    switch (groupType)
    {
      case OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE:
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "outputPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, bucketEntry->bucketData.l2Interface.outputPort);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "popVlanTag = %d ", bucketEntry->bucketData.l2Interface.popVlanTag);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "allowVlanTranslation = %d ", bucketEntry->bucketData.l2Interface.allowVlanTranslation);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_UNFILTERED_INTERFACE:
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "outputPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, bucketEntry->bucketData.l2UnfilteredInterface.outputPort);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "allowVlanTranslation = %d ", bucketEntry->bucketData.l2UnfilteredInterface.allowVlanTranslation);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_INTERFACE:
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.l3Interface.vlanId, bucketEntry->bucketData.l3Interface.vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "srcMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[0],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[1],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[2],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[3],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[4],
                                 bucketEntry->bucketData.l3Interface.srcMac.addr[5]);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST:
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.l3Unicast.vlanId, bucketEntry->bucketData.l3Unicast.vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "srcMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[0],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[1],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[2],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[3],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[4],
                                 bucketEntry->bucketData.l3Unicast.srcMac.addr[5]);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "dstMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[0],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[1],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[2],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[3],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[4],
                                 bucketEntry->bucketData.l3Unicast.dstMac.addr[5]);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_REWRITE:
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.l2Rewrite.vlanId, bucketEntry->bucketData.l2Rewrite.vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "srcMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[0],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[1],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[2],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[3],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[4],
                                 bucketEntry->bucketData.l2Rewrite.srcMac.addr[5]);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "dstMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[0],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[1],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[2],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[3],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[4],
                                 bucketEntry->bucketData.l2Rewrite.dstMac.addr[5]);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_OVERLAY:
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "outputPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, bucketEntry->bucketData.l2Overlay.outputPort);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_L2_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L2_FLOOD:
      case OFDPA_GROUP_ENTRY_TYPE_L3_MULTICAST:
      case OFDPA_GROUP_ENTRY_TYPE_L3_ECMP:
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
        break;

      case OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL:
        rc = ofdpaGroupMplsSubTypeGet(bucketEntry->groupId, &subType);
        if (rc != OFDPA_E_NONE)
        {
          rc = OFDPA_E_ERROR;
        }
        else
        {
          switch (subType)
          {
            case OFDPA_MPLS_INTERFACE:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "oamLmTxCountAction = %d ", bucketEntry->bucketData.mplsInterface.oamLmTxCountAction);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.mplsInterface.vlanId, bucketEntry->bucketData.mplsInterface.vlanId & OFDPA_VID_EXACT_MASK);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "lmepIdAction = %d ", bucketEntry->bucketData.mplsInterface.lmepIdAction);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "lmepId = %d ", bucketEntry->bucketData.mplsInterface.lmepId);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "srcMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[0],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[1],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[2],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[3],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[4],
                                       bucketEntry->bucketData.mplsInterface.srcMac.addr[5]);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "dstMac: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X ",
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[0],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[1],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[2],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[3],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[4],
                                       bucketEntry->bucketData.mplsInterface.dstMac.addr[5]);
              break;

            case OFDPA_MPLS_L2_VPN_LABEL:
            case OFDPA_MPLS_L3_VPN_LABEL:
            case OFDPA_MPLS_TUNNEL_LABEL1:
            case OFDPA_MPLS_TUNNEL_LABEL2:
            case OFDPA_MPLS_SWAP_LABEL:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "pushL2Hdr = %d ", bucketEntry->bucketData.mplsLabel.pushL2Hdr);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "pushVlan = %d ", bucketEntry->bucketData.mplsLabel.pushVlan);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "newTpid = 0x%04x ", bucketEntry->bucketData.mplsLabel.newTpid);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "pushMplsHdr = %d ", bucketEntry->bucketData.mplsLabel.pushMplsHdr);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "pushCW = %d ", bucketEntry->bucketData.mplsLabel.pushCW);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "mplsLabel = 0x%x ", bucketEntry->bucketData.mplsLabel.mplsLabel);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "mplsBOS = %d ", bucketEntry->bucketData.mplsLabel.mplsBOS);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "mplsEXPAction = %d ", bucketEntry->bucketData.mplsLabel.mplsEXPAction);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "mplsEXP = %d ", bucketEntry->bucketData.mplsLabel.mplsEXP);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "mplsCopyEXPOutwards = %d ", bucketEntry->bucketData.mplsLabel.mplsCopyEXPOutwards);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "remarkTableIndexAction = %d ", bucketEntry->bucketData.mplsLabel.remarkTableIndexAction);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "RemarkTableIndex = %d ", bucketEntry->bucketData.mplsLabel.remarkTableIndex);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "mplsTTLAction = %d ", bucketEntry->bucketData.mplsLabel.mplsTTLAction);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "mplsTTL = %d ", bucketEntry->bucketData.mplsLabel.mplsTTL);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "mplsCopyTTLOutwards = %d ", bucketEntry->bucketData.mplsLabel.mplsCopyTTLOutwards);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "lmepIdAction = %d ", bucketEntry->bucketData.mplsLabel.lmepIdAction);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "lmepId = %d ", bucketEntry->bucketData.mplsLabel.lmepId);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "oamLmTxCountAction = %d ", bucketEntry->bucketData.mplsLabel.oamLmTxCountAction);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "colorBasedCountAction = %d ", bucketEntry->bucketData.mplsLabel.colorBasedCountAction);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "colorBasedCountId = %d ", bucketEntry->bucketData.mplsLabel.colorBasedCountId);
              break;

            default:
              count += printf("Unknown group sub-type (0x%x)", subType);
              break;
          }
        }
        break;

      case OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING:
        rc = ofdpaGroupMplsSubTypeGet(bucketEntry->groupId, &subType);
        if (rc != OFDPA_E_NONE)
        {
          rc = OFDPA_E_ERROR;
        }
        else
        {
          switch (subType)
          {
            /* these subtypes are MPLS L2 Replication entries, all have same bucket actions */
            case OFDPA_MPLS_L2_FLOOD:
            case OFDPA_MPLS_L2_MULTICAST:
            case OFDPA_MPLS_L2_LOCAL_FLOOD:
            case OFDPA_MPLS_L2_LOCAL_MULTICAST:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              break;

            case OFDPA_MPLS_FAST_FAILOVER:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "watchPort = ");
              ofdpaPortDecode(outBuf, bufSize, &count, bucketEntry->bucketData.mplsFastFailOver.watchPort);
              break;

            case OFDPA_MPLS_1_1_HEAD_END_PROTECT:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              break;

            case OFDPA_MPLS_ECMP:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              break;

            case OFDPA_MPLS_L2_TAG:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "referenceGroupId = 0x%08x ", bucketEntry->referenceGroupId);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "pushVlan = %d ", bucketEntry->bucketData.mplsL2Tag.pushVlan);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "newTpid = 0x%04x ", bucketEntry->bucketData.mplsL2Tag.newTpid);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "popVlan = %d ", bucketEntry->bucketData.mplsL2Tag.popVlan);
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "vlanId = 0x%04x (VLAN %d) ", bucketEntry->bucketData.mplsL2Tag.vlanId, bucketEntry->bucketData.mplsL2Tag.vlanId & OFDPA_VID_EXACT_MASK);
              break;

            default:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "Unknown group sub-type (0x%x)", subType);
              break;
          }
        }
        break;

      default:
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "Unknown Group type (0x%x)", groupType);
        break;
    }
  }
  return(rc);
#endif
	return OFDPA_E_FAIL;
}

const char *ofdpaQosColorDecode(OFDPA_QOS_COLORS_t color)
{
  switch (color)
  {
    case OFDPA_QOS_GREEN:
      return("green");
      break;

    case OFDPA_QOS_YELLOW:
      return("yellow");
      break;

    case OFDPA_QOS_RED:
      return("red");
      break;

    default:
      return("[Unknown color]");
      break;
  }
}

const char *ofdpaFlowTableNameGet(OFDPA_FLOW_TABLE_ID_t tableId)
{
  int i;

  for (i = 0; i < TABLE_NAME_LIST_SIZE; i++)
  {
    if (tableNameList[i].type == tableId)
    {
      return(tableNameList[i].name);
    }
  }
  return("[Table name not found]");
}

const char *gotoFlowTableNameGet(OFDPA_FLOW_TABLE_ID_t tableId)
{
  /* when used as a Goto table value, return "None" instead of the name for TableId == 0 */
  return(tableId ? ofdpaFlowTableNameGet(tableId) : "None");
}

const char *colorBasedCountActionDecode(OFDPA_ACTION_COLOR_BASED_COUNT_t colorBasedCountAction)
{
  int i;

  for (i = 0; i < COLOR_BASED_COUNT_ACTION_NAME_LIST_SIZE; i++)
  {
    if (colorCountActionNameList[i].type == colorBasedCountAction)
    {
      return(colorCountActionNameList[i].name);
    }
  }
  return("[Color Count Action string not found]");
}

static int isMacZero(ofdpaMacAddr_t *macAddr)
{
  int i;

  for (i = 0; i < sizeof(macAddr->addr); i++)
  {
    if (macAddr->addr[i] != 0)
    {
      return 0;
    }
  }
  return 1;
}

OFDPA_ERROR_t ofdpaFlowEntryDecode(ofdpaFlowEntry_t *flow, char *outBuf, int bufSize)
{
  OFDPA_ERROR_t rc = OFDPA_E_NONE;
  uint32_t count = 0, mdlLevel = 0;;

  switch (flow->tableId)
  {
    case OFDPA_FLOW_TABLE_ID_INGRESS_PORT:
      {
        ofdpaIngressPortFlowEntry_t *flowData;
        ofdpaIngressPortFlowMatch_t *match;

        flowData = &flow->flowData.ingressPortFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " inPort:mask = 0x%08x:0x%08x", match->inPort, match->inPortMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType:mask = 0x%04x:0x%04x", match->etherType, match->etherTypeMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelId:mask = 0x%08x:0x%08x", match->tunnelId, match->tunnelIdMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepId:mask = 0x%08x:0x%08x", match->lmepId, match->lmepIdMask);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        if (flowData->vrfAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vrf = %d ", flowData->vrf);
        }

        if (flowData->qosIndexAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " qosIndex = %d ", flowData->qosIndex);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_DSCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_TUNNEL_DSCP_TRUST:
      {
        ofdpaDscpTrustFlowEntry_t *flowData;
        ofdpaDscpTrustFlowMatch_t *match;

        flowData = &flow->flowData.dscpTrustFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " qosIndex = %d", match->qosIndex);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dscpValue = %d", match->dscpValue);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " | ");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Traffic Class = %d", flowData->trafficClass);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Color = %d (%s)", flowData->color, ofdpaQosColorDecode(flowData->color));
      }
      break;

    case OFDPA_FLOW_TABLE_ID_PORT_PCP_TRUST:
    case OFDPA_FLOW_TABLE_ID_TUNNEL_PCP_TRUST:
      {
        ofdpaPcpTrustFlowEntry_t *flowData;
        ofdpaPcpTrustFlowMatch_t *match;

        /* match criteria */
        flowData = &flow->flowData.pcpTrustFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " qosIndex = %d", match->qosIndex);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " pcpValue = %d", match->pcpValue);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dei = %d", match->dei);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Traffic Class = %d", flowData->trafficClass);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Color = %d (%s)", flowData->color, ofdpaQosColorDecode(flowData->color));
      }
      break;

    case OFDPA_FLOW_TABLE_ID_INJECTED_OAM:
      {
        ofdpaInjectedOamFlowEntry_t *flowData;
        ofdpaInjectedOamFlowMatch_t *match;

        flowData = &flow->flowData.injectedOamFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepId = %d", match->lmepId);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        if (flowData->tunnelIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelId = 0x%x", flowData->tunnelId);
        }

        if (flowData->mplsL2PortAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsL2Port = 0x%x", flowData->mplsL2Port);
        }

        if (flowData->pushMplsGalHdr)
        {
          if (flowData->mplsGalTTLAction)
          {
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsGalTTL = %d", flowData->mplsGalTTL);
          }

          if (flowData->mplsGalTCAction)
          {
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsGalTC = %d", flowData->mplsGalTC);
          }
        }

        if (flowData->pushMplsLabelHdr)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsLabel = 0x%x", flowData->mplsLabel);
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsBOS = %d", flowData->mplsBOS);

          if (flowData->mplsTCAction)
          {
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsTC = %d", flowData->mplsTC);
          }

          if (flowData->mplsTTLAction)
          {
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsTTL = %d", flowData->mplsTTL);
          }
        }

        if (flowData->mplsDataFirstNibbleAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsDataFirstNibble = %d", flowData->mplsDataFirstNibble);
        }

        if (flowData->mplsAchChannelAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsAchChannel = 0x%x", flowData->mplsAchChannel);
        }

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " pushCW = %d", flowData->pushCW);

        if (flowData->mplsVlanPcpAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsVlanPcp = %d", flowData->mplsVlanPcp);
        }

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " oamLmTxCountAction = %d", flowData->oamLmTxCountAction);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " groupId = 0x%x", flowData->groupId);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN:
      {
        ofdpaVlanFlowEntry_t *flowData;
        ofdpaVlanFlowMatch_t *match;
				int i;
				ofdpaActionFuncOpt_t	ops;
				
        flowData = &flow->flowData.vlanFlowEntry;
        match = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " inPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, match->inPort);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId:mask = 0x%04x:0x%04x (VLAN %d)", match->vlanId, match->vlanIdMask, match->vlanId & OFDPA_VID_EXACT_MASK);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

				for(i = 0; i < flowData->apply_cnt ; i ++){
					if (count < bufSize) 																															 
					{ 																																											 
						ops.buf = &outBuf[count];
						ops.bufSize = bufSize - count;
						count += (flowData->apply_actions[i].act)(&ops,NULL,flowData->apply_actions[i].arg);
					} 																																											 
					if (count >= bufSize)																															 
					{ 																																											 
						outBuf[bufSize - 1] = '\0';																													
						return OFDPA_E_FULL;																																	 
					} 																																											 
				}
				
				for(i = 0; i < flowData->write_cnt ; i ++){
					if (count < bufSize) 																															 
					{ 																																											 
						ops.buf = &outBuf[count];
						ops.bufSize = bufSize - count;
						count += (flowData->write_actions[i].act)(&ops,NULL,flowData->write_actions[i].arg);
					} 																																											 
					if (count >= bufSize)																															 
					{ 																																											 
						outBuf[bufSize - 1] = '\0';																													
						return OFDPA_E_FULL;																																	 
					} 																																											 
	      }
			}
      break;

    case OFDPA_FLOW_TABLE_ID_VLAN_1:
      {
        ofdpaVlan1FlowEntry_t *flowData;
        ofdpaVlan1FlowMatch_t *match;

        flowData = &flow->flowData.vlan1FlowEntry;
        match = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " inPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, match->inPort);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId = 0x%04x (VLAN %d)", match->vlanId, match->vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " ovid = 0x%04x (VLAN %d)", match->ovid, match->ovid & OFDPA_VID_EXACT_MASK);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        if (flowData->setVlanIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newVlanId = 0x%04x (VLAN %d)", flowData->newVlanId, flowData->newVlanId  & OFDPA_VID_EXACT_MASK);
        }
        if (flowData->pushVlan2Action)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newTpid2 = 0x%x", flowData->newTpid2);
        }
        if (flowData->setVlanId2Action)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newVlanId2 = 0x%04x (VLAN %d)", flowData->newVlanId2, flowData->newVlanId2  & OFDPA_VID_EXACT_MASK);
        }
        if (flowData->vrfAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vrf = %d", flowData->vrf);
        }
        if (flowData->mplsL2PortAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsL2Port = 0x%x", flowData->mplsL2Port);
        }
        if (flowData->tunnelIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelId = 0x%x", flowData->tunnelId);
        }
        if (flowData->classBasedCountAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " classBasedCountId = %d", flowData->classBasedCountId);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MAINTENANCE_POINT:
      {
        ofdpaMpFlowEntry_t *flowData;
        ofdpaMpFlowMatch_t *match;

        flowData = &flow->flowData.mpFlowEntry;
        match = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " inPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, match->inPort);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId:mask = 0x%04x:0x%04x (VLAN %d)", match->vlanId, match->vlanIdMask, match->vlanId & OFDPA_VID_EXACT_MASK);
        if (0 != match->etherTypeMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType:mask = 0x%04x:0x%04x", match->etherType, match->etherTypeMask);
        }
        if (match->oamY1731MdlMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " oamY1731Mdl = %d (0x%x)", match->oamY1731Mdl, match->oamY1731Mdl);
        }
        if (match->oamY1731OpcodeMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " oamY1731Opcode = %d (0x%x)", match->oamY1731Opcode, match->oamY1731Opcode);
        }
        if (!isMacZero(&match->destMacMask))
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " destMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
                                   match->destMac.addr[0], match->destMac.addr[1],
                                   match->destMac.addr[2], match->destMac.addr[3],
                                   match->destMac.addr[4], match->destMac.addr[5],
                                   match->destMacMask.addr[0], match->destMacMask.addr[1],
                                   match->destMacMask.addr[2], match->destMacMask.addr[3],
                                   match->destMacMask.addr[4], match->destMacMask.addr[5]);
        }

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        if (flowData->clearAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " clearAction");
        }
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        if (flowData->lmepIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepId = %d (0x%04x)", flowData->lmepId, flowData->lmepId);
        }
        if ((flowData->lmepIdAction) ||
            (flowData->checkDropStatusAction))
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepId = %d (0x%04x)", flowData->lmepId, flowData->lmepId);
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepIdAction = %d ", flowData->lmepIdAction);
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " checkDropStatusAction = %d ", flowData->checkDropStatusAction);
        }

        for (mdlLevel = OFDPA_MDL_MIN_VALUE; mdlLevel <= OFDPA_MDL_MAX_VALUE; mdlLevel++)
        {
          if (0 != flowData->rxLmCountingLmepId[mdlLevel])
          {
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " rxLmCountingLmepId[%d] = %d", mdlLevel, flowData->rxLmCountingLmepId[mdlLevel]);
          }

          if (0 != flowData->txLmCountingLmepId[mdlLevel])
          {
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " txLmCountingLmepId[%d] = %d", mdlLevel, flowData->txLmCountingLmepId[mdlLevel]);
          }
        }

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputPort);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_L2_PORT:
      {
        ofdpaMplsL2PortFlowEntry_t *flowData;
        ofdpaMplsL2PortFlowMatch_t *match;

        flowData = &flow->flowData.mplsL2PortFlowEntry;
        match = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsL2Port:mask = 0x%08x:0x%08x", match->mplsL2Port, match->mplsL2PortMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelId = 0x%x", match->tunnelId);

        if (OFDPA_FLOW_TABLE_ID_MPLS_DSCP_TRUST == flowData->gotoTableId)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType:mask = 0x%04x:0x%04x", match->etherType, match->etherTypeMask);
        }

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "%s", " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
        /* Apply actions */
        if (0 != flowData->qosIndexAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " QOS Index = %d", flowData->qosIndex);
        }
        /* Write actions */
        if (flowData->groupId != 0)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " groupId = 0x%08x", flowData->groupId);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_QOS_CLASS:
      {
        ofdpaMplsL2PortQosClassFlowEntry_t *flowData;
        ofdpaMplsL2PortQosClassFlowMatch_t *match;
        struct in_addr ipv4Addr;
        char addrBuf[INET6_ADDRSTRLEN];
        char addrMaskBuf[INET6_ADDRSTRLEN];

        flowData = &flow->flowData.mplsL2PortQosFlowEntry;
        match = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsL2Port = 0x%08x", match->mplsL2Port);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelId = 0x%x", match->tunnelId);

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType:mask = 0x%04x:0x%04x", match->etherType, match->etherTypeMask);

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " srcMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
                                 match->srcMac.addr[0], match->srcMac.addr[1],
                                 match->srcMac.addr[2], match->srcMac.addr[3],
                                 match->srcMac.addr[4], match->srcMac.addr[5],
                                 match->srcMacMask.addr[0], match->srcMacMask.addr[1],
                                 match->srcMacMask.addr[2], match->srcMacMask.addr[3],
                                 match->srcMacMask.addr[4], match->srcMacMask.addr[5]);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " destMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
                                 match->destMac.addr[0], match->destMac.addr[1],
                                 match->destMac.addr[2], match->destMac.addr[3],
                                 match->destMac.addr[4], match->destMac.addr[5],
                                 match->destMacMask.addr[0], match->destMacMask.addr[1],
                                 match->destMacMask.addr[2], match->destMacMask.addr[3],
                                 match->destMacMask.addr[4], match->destMacMask.addr[5]);

        if (match->vlanPcpMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanPcp:mask = %d:0x%x", match->vlanPcp, match->vlanPcpMask);
        }

        if (0 != match->vlanDeiMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " DEI = %u", match->vlanDei);
        }

        if (0 != match->sourceIp4Mask)
        {
          ipv4Addr.s_addr = htonl(match->sourceIp4);
          inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
          ipv4Addr.s_addr = htonl(match->sourceIp4Mask);
          inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " srcIp4 = %s/%s", addrBuf, addrMaskBuf);
        }

        if (0 != match->destIp4Mask)
        {
          ipv4Addr.s_addr = htonl(match->destIp4);
          inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
          ipv4Addr.s_addr = htonl(match->destIp4Mask);
          inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp4 = %s/%s", addrBuf, addrMaskBuf);
        }

        if (0x86dd == (match->etherType & match->etherTypeMask))
        {
          inet_ntop(AF_INET6, &match->sourceIp6, addrBuf, sizeof(addrBuf));
          inet_ntop(AF_INET6, &match->sourceIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " srcIp6 = %s/%s", addrBuf, addrMaskBuf);

          inet_ntop(AF_INET6, &match->destIp6, addrBuf, sizeof(addrBuf));
          inet_ntop(AF_INET6, &match->destIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp6 = %s/%s", addrBuf, addrMaskBuf);
        }

        if (0 != match->ipProtoMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " IP Protocol = 0x%2.2x", match->ipProto);
        }

        if (0 != match->dscpMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " DSCP = %u", match->dscp);
        }

        if (0 != match->srcL4PortMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Source L4 Port = %u", match->srcL4Port);
        }

        if (0 != match->destL4PortMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Destination L4 Port = %u", match->destL4Port);
        }

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
        /* Apply actions */
        if (0 != flowData->colorAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Color = %d (%s)", flowData->color, ofdpaQosColorDecode(flowData->color));
        }
        if (0 != flowData->trafficClassAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Traffic Class = %u", flowData->trafficClass);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER:
      {
        ofdpaL2PolicerFlowEntry_t *flowData;
        ofdpaL2PolicerFlowMatch_t *match;

        flowData = &flow->flowData.l2PolicerFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelId = %u", match->tunnelId);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsL2Port:mask = %u:0x%x", match->mplsL2Port, match->mplsL2PortMask);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " colorActionsIndex = %u", flowData->colorActionsIndex);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " meterId = %u\n", flowData->meterId);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_L2_POLICER_ACTIONS:
      {
        ofdpaL2PolicerActionsFlowEntry_t *flowData;
        ofdpaL2PolicerActionsFlowMatch_t *match;

        flowData = &flow->flowData.l2PolicerActionsFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " match:colorActionsIndex = %u", match->colorActionsIndex);

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Color = %d", match->color);
        switch (match->color)
        {
          case OFDPA_QOS_GREEN:
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " (green) ");
            break;
          case OFDPA_QOS_YELLOW:
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " (yellow)");
            break;
          case OFDPA_QOS_RED:
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " (red)   ");
            break;
          default:
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " (illegal value)");
            break;
        }

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        if (flowData->clearAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " clearAction");
        }

        if (flowData->dscpAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set DSCP = %u", flowData->dscp);
        }

        if (flowData->ecnAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set ECN = %u", flowData->ecn);
        }

        if (flowData->vlanPcpAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set VLAN PCP = %u", flowData->vlanPcp);
        }

        if (flowData->trafficClassAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Traffic Class = %u", flowData->trafficClass);
        }

        /** Apply-Action(s) instruction */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " data:colorActionsIndex = %u", flowData->colorActionsIndex);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_TERMINATION_MAC:
      {
        ofdpaTerminationMacFlowEntry_t *flowData;
        ofdpaTerminationMacFlowMatch_t *match;

        flowData = &flow->flowData.terminationMacFlowEntry;
        match = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " inPort:mask = %d:0x%x", match->inPort, match->inPortMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType = 0x%04x", match->etherType);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " destMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
                                 match->destMac.addr[0], match->destMac.addr[1],
                                 match->destMac.addr[2], match->destMac.addr[3],
                                 match->destMac.addr[4], match->destMac.addr[5],
                                 match->destMacMask.addr[0], match->destMacMask.addr[1],
                                 match->destMacMask.addr[2], match->destMacMask.addr[3],
                                 match->destMacMask.addr[4], match->destMacMask.addr[5]);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId:mask = 0x%04x:0x%04x (VLAN %d)", match->vlanId, match->vlanIdMask, match->vlanId & OFDPA_VID_EXACT_MASK);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = %d", flowData->outputPort);
        ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputPort);

        if (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " colorBasedCountAction = %d (%s) colorBasedCountId = %d",
                                   flowData->colorBasedCountAction, colorBasedCountActionDecode(flowData->colorBasedCountAction),
                                   flowData->colorBasedCountId);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_0:
    case OFDPA_FLOW_TABLE_ID_MPLS_1:
    case OFDPA_FLOW_TABLE_ID_MPLS_2:
      {
        ofdpaMplsFlowEntry_t *flowData;
        ofdpaMplsFlowMatch_t *match;
        struct in_addr ipv4Addr;
        char addrBuf[INET6_ADDRSTRLEN];
        char addrMaskBuf[INET6_ADDRSTRLEN];

        flowData = &flow->flowData.mplsFlowEntry;
        match = &flowData->match_criteria;

        /* MPLS flow tables are folded into a single flow table, display the table ID used to add flow */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tableId = %d", flow->tableId);

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType = 0x%04x", match->etherType);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " BoS = %s", (match->mplsBos != 0) ? "TRUE" : "FALSE");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsLabel = 0x%x", match->mplsLabel);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " inPort:mask = %d:0x%x", match->inPort, match->inPortMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsTtl:mask = %d:0x%x", match->mplsTtl, match->mplsTtlMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsDataFirstNibble:mask = %d:0x%x", match->mplsDataFirstNibble, match->mplsDataFirstNibbleMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsAchChannel:mask = 0x%x:0x%x", match->mplsAchChannel, match->mplsAchChannelMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " nextLabelIsGal:mask = %d:0x%x", match->nextLabelIsGal, match->nextLabelIsGalMask);

        ipv4Addr.s_addr = htonl(match->destIp4);
        inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
        ipv4Addr.s_addr = htonl(match->destIp4Mask);
        inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp4 = %s/%s", addrBuf, addrMaskBuf);

        inet_ntop(AF_INET6, &match->destIp6, addrBuf, sizeof(addrBuf));
        inet_ntop(AF_INET6, &match->destIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp6 = %s/%s", addrBuf, addrMaskBuf);

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " ipProto:mask = %d:0x%x", match->ipProto, match->ipProtoMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " udpSrcPort:mask = %d:0x%x", match->udpSrcPort, match->udpSrcPortMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " udpDstPort:mask = %d:0x%x", match->udpDstPort, match->udpDstPortMask);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        if (flowData->popLabelAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " popLabel");
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newEtherType = 0x%04x", flowData->newEtherType);
        }
        if (flowData->decrementTtlAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " decrementTtl");
        }
        if (flowData->copyTtlInAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " copyTtlIn");
        }
        if (flowData->copyTcInAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " copyTcIn");
        }
        if (flowData->vrfAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vrf = %d", flowData->vrf);
        }
        if (flowData->mplsL2PortAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsL2Port = 0x%08x", flowData->mplsL2Port);
        }
        if (flowData->tunnelIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelId = 0x%08x", flowData->tunnelId);
        }

        if (flowData->qosIndexAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " qosIndex = %d", flowData->qosIndex);
        }
        if (flowData->trafficClassAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " trafficClass %d", flowData->trafficClass);
        }
        if (flowData->copyFieldAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " copyFieldAction %d", flowData->copyFieldAction);
        }
        if (flowData->popCwAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " popCw");
        }
        if (flowData->popL2HeaderAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " popL2Header");
        }
        if (flowData->lmepIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepId = %d", flowData->lmepId);
        }
        if (0 != flowData->oamLmRxCountAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " oamLmRxCountAction = %d", flowData->oamLmRxCountAction);
        }
        if (0 != flowData->protectionIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " protectionId = %s (%d)", (flowData->protectionId == 0) ? "Protection path" : "Working path", flowData->protectionId);
        }
        if (0 != flowData->mplsTypeAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsType = %d (%s)", flowData->mplsType,mplsTypeSubTypeName[flowData->mplsType]);
        }
        if (flowData->colorBasedCountAction != OFDPA_ACTION_COLOR_BASED_COUNT_NONE)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " colorBasedCountAction = %d (%s) colorBasedCountId = %d",
                                   flowData->colorBasedCountAction, colorBasedCountActionDecode(flowData->colorBasedCountAction),
                                   flowData->colorBasedCountId);
        }

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " groupId = 0x%08x", flowData->groupID);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputPort);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_MAINTENANCE_POINT:
      {
        ofdpaMplsMpFlowEntry_t *flowData;
        ofdpaMplsMpFlowMatch_t *match;

        flowData = &flow->flowData.mplsMpFlowEntry;
        match = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepId = %d (0x%04x)", match->lmepId, match->lmepId);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " oamY1731Opcode = %d (0x%x)", match->oamY1731Opcode, match->oamY1731Opcode);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType = 0x%x", match->etherType);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        if (flowData->clearAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " clearAction");
        }
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        if (flowData->oamSetCounterFieldsAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " oamSetCounterFieldsAction");
        }
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepId = %d (0x%04x)", flowData->lmepId, flowData->lmepId);

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputPort);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MPLS_LABEL_TRUST:
      {
        ofdpaMplsLabelTrustFlowEntry_t *flowData;
        ofdpaMplsLabelTrustFlowMatch_t *match;

        flowData = &flow->flowData.mplsLabelTrustFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " qosIndex = %d", match->qosIndex);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsTC = %d", match->mplsTC);
        
        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " | ");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Traffic Class = %d", flowData->trafficClass);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Color = %d (%s)", flowData->color, ofdpaQosColorDecode(flowData->color));
      }
      break;

    case OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING:
      {
        ofdpaUnicastRoutingFlowEntry_t *flowData;
        ofdpaUnicastRoutingFlowMatch_t *match;
        struct in_addr ipv4Addr;
        char addrBuf[INET6_ADDRSTRLEN];
        char addrMaskBuf[INET6_ADDRSTRLEN];

        flowData = &flow->flowData.unicastRoutingFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType = 0x%04x", match->etherType);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vrf:mask = 0x%04x:0x%04x", match->vrf, match->vrfMask);

        ipv4Addr.s_addr = htonl(match->dstIp4);
        inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
        ipv4Addr.s_addr = htonl(match->dstIp4Mask);
        inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp4 = %s/%s", addrBuf, addrMaskBuf);

        inet_ntop(AF_INET6, &match->dstIp6, addrBuf, sizeof(addrBuf));
        inet_ntop(AF_INET6, &match->dstIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp6 = %s/%s", addrBuf, addrMaskBuf);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
        if (0 != flowData->groupID)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " groupId = 0x%08x", flowData->groupID);
        }
        if (0 != flowData->outputPort)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
          ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputPort);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_MULTICAST_ROUTING:
      {
        ofdpaMulticastRoutingFlowEntry_t *flowData;
        ofdpaMulticastRoutingFlowMatch_t *match;
        struct in_addr ipv4Addr;
        char addrBuf[INET6_ADDRSTRLEN];
        char addrMaskBuf[INET6_ADDRSTRLEN];

        flowData = &flow->flowData.multicastRoutingFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType = 0x%04x", match->etherType);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId = 0x%04x (VLAN %d)", match->vlanId, match->vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vrf:mask = 0x%04x:0x%04x", match->vrf, match->vrfMask);

        ipv4Addr.s_addr = htonl(match->srcIp4);
        inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
        ipv4Addr.s_addr = htonl(match->srcIp4Mask);
        inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " srcIp4 = %s/%s", addrBuf, addrMaskBuf);

        ipv4Addr.s_addr = htonl(match->dstIp4);
        inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp4 = %s", addrBuf);

        inet_ntop(AF_INET6, &match->srcIp6, addrBuf, sizeof(addrBuf));
        inet_ntop(AF_INET6, &match->srcIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " srcIp6 = %s/%s", addrBuf, addrMaskBuf);

        inet_ntop(AF_INET6, &match->dstIp6, addrBuf, sizeof(addrBuf));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp6 = %s", addrBuf);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " groupId = 0x%08x", flowData->groupID);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_BRIDGING:
      {
        ofdpaBridgingFlowEntry_t *flowData;
        ofdpaBridgingFlowMatch_t *match;

        flowData = &flow->flowData.bridgingFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */
        if (match->vlanId)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId:mask = 0x%04x:0x%04x (VLAN %d)", match->vlanId, match->vlanIdMask, match->vlanId & OFDPA_VID_EXACT_MASK);
        }
        else
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelId:mask = 0x%x:0x%x", match->tunnelId, match->tunnelIdMask);
        }

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " destMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
                                 match->destMac.addr[0], match->destMac.addr[1],
                                 match->destMac.addr[2], match->destMac.addr[3],
                                 match->destMac.addr[4], match->destMac.addr[5],
                                 match->destMacMask.addr[0], match->destMacMask.addr[1],
                                 match->destMacMask.addr[2], match->destMacMask.addr[3],
                                 match->destMacMask.addr[4], match->destMacMask.addr[5]);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " groupId = 0x%08x", flowData->groupID);
        if (flowData->tunnelLogicalPort)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelLogicalPort = ");
          ofdpaPortDecode(outBuf, bufSize, &count, flowData->tunnelLogicalPort);
        }
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputPort);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_ACL_POLICY:
      {
        ofdpaPolicyAclFlowEntry_t *flowData;
        ofdpaPolicyAclFlowMatch_t *match;
        struct in_addr ipv4Addr;
        char addrBuf[INET6_ADDRSTRLEN];
        char addrMaskBuf[INET6_ADDRSTRLEN];

        flowData = &flow->flowData.policyAclFlowEntry;
        match = &flowData->match_criteria;

        /* match criteria */

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " inPort:mask = %d:0x%x", match->inPort, match->inPortMask);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " srcMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
                                 match->srcMac.addr[0], match->srcMac.addr[1],
                                 match->srcMac.addr[2], match->srcMac.addr[3],
                                 match->srcMac.addr[4], match->srcMac.addr[5],
                                 match->srcMacMask.addr[0], match->srcMacMask.addr[1],
                                 match->srcMacMask.addr[2], match->srcMacMask.addr[3],
                                 match->srcMacMask.addr[4], match->srcMacMask.addr[5]);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " destMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
                                 match->destMac.addr[0], match->destMac.addr[1],
                                 match->destMac.addr[2], match->destMac.addr[3],
                                 match->destMac.addr[4], match->destMac.addr[5],
                                 match->destMacMask.addr[0], match->destMacMask.addr[1],
                                 match->destMacMask.addr[2], match->destMacMask.addr[3],
                                 match->destMacMask.addr[4], match->destMacMask.addr[5]);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType:mask = 0x%04x:0x%04x", match->etherType, match->etherTypeMask);
        if (match->mplsL2PortMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " mplsL2Port:mask = %d:0x%x", match->mplsL2Port, match->mplsL2PortMask);
        }

        if (match->vlanIdMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId:mask = 0x%04x:0x%04x (VLAN %d)", match->vlanId, match->vlanIdMask, match->vlanId & OFDPA_VID_EXACT_MASK);
        }

        if (match->tunnelIdMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " tunnelId = %d", match->tunnelId);
        }

        if (match->vlanPcpMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanPcp:mask = %d:0x%x", match->vlanPcp, match->vlanPcpMask);
        }

        if (0 != match->vlanDeiMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " DEI = %u", match->vlanDei);
        }

        if (0x86dd == match->etherType)
        {
          inet_ntop(AF_INET6, &match->sourceIp6, addrBuf, sizeof(addrBuf));
          inet_ntop(AF_INET6, &match->sourceIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " srcIp6 = %s/%s", addrBuf, addrMaskBuf);

          inet_ntop(AF_INET6, &match->destIp6, addrBuf, sizeof(addrBuf));
          inet_ntop(AF_INET6, &match->destIp6Mask, addrMaskBuf, sizeof(addrMaskBuf));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp6 = %s/%s", addrBuf, addrMaskBuf);

          if (match->ipv6FlowLabelMask != 0)
          {
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " IPv6 Flow Label = 0x%05x/0x%05x", match->ipv6FlowLabel, match->ipv6FlowLabelMask);
          }
        }

        if (0 != match->sourceIp4Mask)
        {
        ipv4Addr.s_addr = htonl(match->sourceIp4);
        inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
        ipv4Addr.s_addr = htonl(match->sourceIp4Mask);
        inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " srcIp4 = %s/%s", addrBuf, addrMaskBuf);
        }

        if (0 != match->destIp4Mask)
        {
          ipv4Addr.s_addr = htonl(match->destIp4);
          inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
          ipv4Addr.s_addr = htonl(match->destIp4Mask);
          inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " dstIp4 = %s/%s", addrBuf, addrMaskBuf);
        }

        if (0 != match->ipv4ArpSpaMask)
        {
          ipv4Addr.s_addr = htonl(match->ipv4ArpSpa);
          inet_ntop(AF_INET, &ipv4Addr, addrBuf, sizeof(addrBuf));
          ipv4Addr.s_addr = htonl(match->ipv4ArpSpaMask);
          inet_ntop(AF_INET, &ipv4Addr, addrMaskBuf, sizeof(addrMaskBuf));
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " arpSpa = %s/%s", addrBuf, addrMaskBuf);
        }

        if (0 != match->vrfMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " VRF = %u", match->vrf);
        }

        if (0 != match->dscpMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " DSCP = %u", match->dscp);
        }

        if (0 != match->ecnMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " ECN = %u", match->ecn);
        }

        if (0 != match->ipProtoMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " IP Protocol = 0x%2.2x", match->ipProto);
        }

        if (0 != match->srcL4PortMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Source L4 Port = %u", match->srcL4Port);
        }

        if (0 != match->destL4PortMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Destination L4 Port = %u", match->destL4Port);
        }

        if (0 != match->icmpTypeMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " ICMP Type = %u", match->icmpType);
        }

        if (0 != match->icmpCodeMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " ICMP Code = %u", match->icmpCode);
        }

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTable, gotoFlowTableNameGet(flowData->gotoTable));
        if (flowData->groupID)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set output group ID = 0x%8x", flowData->groupID);
        }
        if (flowData->trafficClassAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Traffic Class = %d", flowData->trafficClass);
        }
        if (flowData->vlanPcpAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set VLAN PCP = %u", flowData->vlanPcp);
        }
        if (flowData->ecnAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set ECN = %u", flowData->ecn);
        }
        if (flowData->dscpAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set DSCP = %u", flowData->dscp);
        }

        if (flowData->colorAction)
        {
          switch (flowData->color)
          {
            case OFDPA_QOS_GREEN:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Color = green:0");
              break;
            case OFDPA_QOS_YELLOW:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Color = yellow:1");
              break;
            case OFDPA_QOS_RED:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Color = red:2");
              break;
            default:
              APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Color = incorrect value:%d", flowData->color);
              break;
          }
        }

        if (flowData->colorEntryIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Color Entry ID = %d", flowData->colorEntryId);
        }

        if (flowData->meterIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Meter ID = %d", flowData->meterId);
        }

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputPort);

        if (flowData->outputTunnelPort != 0)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outputTunnelPort = ");
          ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputTunnelPort);
        }

        if (flowData->clearAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " clearAction");
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_COLOR_BASED_ACTIONS:
      {
        ofdpaColorActionsFlowEntry_t  *flowData;
        ofdpaColorActionsFlowMatch_t  *match;

        flowData = &flow->flowData.colorActionsFlowEntry;
        match    = &flowData->match_criteria;

        switch (match->color)
        {
          case OFDPA_QOS_GREEN:
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Color =  green:0");
            break;
          case OFDPA_QOS_YELLOW:
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Color = yellow:1");
            break;
          case OFDPA_QOS_RED:
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Color =    red:2");
            break;
          default:
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Color = incorrect value:%d", match->color);
            break;
        }
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " index = %d", match->index);

        /* instructions */

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");

        if (flowData->clearAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " clearAction");
        }
        if (flowData->outputPort)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outputPort = ");
          ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputPort);
        }
        if (flowData->trafficClassAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set Traffic Class = %u", flowData->trafficClass);
        }
        if (flowData->vlanPcpAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set VLAN PCP = %u", flowData->vlanPcp);
        }
        if (flowData->ecnAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set ECN = %u", flowData->ecn);
        }
        if (flowData->dscpAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set DSCP = %u", flowData->dscp);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN:
      {
        ofdpaEgressVlanFlowEntry_t *flowData;
        ofdpaEgressVlanFlowMatch_t *match;

        flowData = &flow->flowData.egressVlanFlowEntry;
        match = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, match->outPort);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId = 0x%04x (VLAN %d)", match->vlanId, match->vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " allowVlanTranslation = %d", match->allowVlanTranslation);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        if (flowData->setVlanIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newVlanId = 0x%04x (VLAN %d)", flowData->newVlanId, flowData->newVlanId  & OFDPA_VID_EXACT_MASK);
        }
        if (flowData->popVlanAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " popVlanAction");
        }
        if (flowData->pushVlan2Action)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newTpid2 = 0x%x", flowData->newTpid2);
        }
        if (flowData->setVlanId2Action)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newVlanId2 = 0x%04x (VLAN %d)", flowData->newVlanId2, flowData->newVlanId2  & OFDPA_VID_EXACT_MASK);
        }
        if (flowData->ovidAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " ovid = %d", flowData->ovid);
        }
        if (flowData->classBasedCountAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " classBasedCountId = %d", flowData->classBasedCountId);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_VLAN_1:
      {
        ofdpaEgressVlan1FlowEntry_t *flowData;
        ofdpaEgressVlan1FlowMatch_t *match;

        flowData = &flow->flowData.egressVlan1FlowEntry;
        match = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, match->outPort);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId = 0x%04x (VLAN %d)", match->vlanId, match->vlanId & OFDPA_VID_EXACT_MASK);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " ovid = 0x%04x (VLAN %d)", match->ovid, match->ovid & OFDPA_VID_EXACT_MASK);

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        if (flowData->setVlanIdAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newVlanId = 0x%04x (VLAN %d)", flowData->newVlanId, flowData->newVlanId  & OFDPA_VID_EXACT_MASK);
        }
        if (flowData->pushVlan2Action)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newTpid2 = 0x%x", flowData->newTpid2);
        }
        if (flowData->setVlanId2Action)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newVlanId2 = 0x%04x (VLAN %d)", flowData->newVlanId2, flowData->newVlanId2  & OFDPA_VID_EXACT_MASK);
        }
        if (flowData->classBasedCountAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " classBasedCountId = %d", flowData->classBasedCountId);
        }
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_MAINTENANCE_POINT:
      {
        ofdpaEgressMpFlowEntry_t *flowData;
        ofdpaEgressMpFlowMatch_t *match;

        flowData = &flow->flowData.egressMpFlowEntry;
        match = &flowData->match_criteria;

        if (0 != match->etherTypeMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType:mask = 0x%04x:0x%04x", match->etherType, match->etherTypeMask);
        }
        if (match->oamY1731MdlMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " oamY1731Mdl = %d (0x%x)", match->oamY1731Mdl, match->oamY1731Mdl);
        }
        if (match->oamY1731OpcodeMask)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " oamY1731Opcode = %d (0x%x)", match->oamY1731Opcode, match->oamY1731Opcode);
        }
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, match->outPort);
        if (match->vlanId)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " vlanId:mask = 0x%04x (VLAN %d):0x%x", match->vlanId, match->vlanId & OFDPA_VID_EXACT_MASK, match->vlanIdMask);
        }
        if (!isMacZero(&match->destMacMask))
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " destMac:mask = %2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x:%2.2x%2.2x.%2.2x%2.2x.%2.2x%2.2x",
                                   match->destMac.addr[0], match->destMac.addr[1],
                                   match->destMac.addr[2], match->destMac.addr[3],
                                   match->destMac.addr[4], match->destMac.addr[5],
                                   match->destMacMask.addr[0], match->destMacMask.addr[1],
                                   match->destMacMask.addr[2], match->destMacMask.addr[3],
                                   match->destMacMask.addr[4], match->destMacMask.addr[5]);
        }

        /* instructions */
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        if (flowData->clearAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " clearAction");
        }

        if ((flowData->lmepIdAction) ||
            (flowData->oamSetCounterFieldsAction) ||
            (flowData->checkDropStatusAction))
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepId = %d (0x%04x)", flowData->lmepId, flowData->lmepId);
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " lmepIdAction = %d ", flowData->lmepIdAction);
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " oamSetCounterFieldsAction = %d ", flowData->oamSetCounterFieldsAction);
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " checkDropStatusAction = %d ", flowData->checkDropStatusAction);
        }

        for (mdlLevel = OFDPA_MDL_MIN_VALUE; mdlLevel <= OFDPA_MDL_MAX_VALUE; mdlLevel++)
        {
          if (0 != flowData->rxLmCountingLmepId[mdlLevel])
          {
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " rxLmCountingLmepId[%d] = %d", mdlLevel, flowData->rxLmCountingLmepId[mdlLevel]);
          }

          if (0 != flowData->txLmCountingLmepId[mdlLevel])
          {
            APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " txLmCountingLmepId[%d] = %d", mdlLevel, flowData->txLmCountingLmepId[mdlLevel]);
          }
        }

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
        ofdpaPortDecode(outBuf, bufSize, &count, flowData->outputPort);
      }
      break;

    case OFDPA_FLOW_TABLE_ID_EGRESS_DSCP_PCP_REMARK:
      {
        ofdpaEgressDscpPcpRemarkFlowEntry_t  *flowData;
        ofdpaEgressDscpPcpRemarkFlowMatch_t  *match;

        flowData = &flow->flowData.egressDscpPcpRemarkFlowEntry;
        match    = &flowData->match_criteria;

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Output Port = ");
        ofdpaPortDecode(outBuf, bufSize, &count, match->outPort);

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " etherType:mask = 0x%04x:0x%04x", match->etherType, match->etherTypeMask);

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Traffic Class = %d", match->trafficClass);
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Color = %d (%s)", match->color, ofdpaQosColorDecode(match->color));

        /* instructions */

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");
        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " GoTo = %d (%s)", flowData->gotoTableId, gotoFlowTableNameGet(flowData->gotoTableId));

        APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");

        if (flowData->vlanPcpAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set VLAN PCP = %u", flowData->vlanPcp);
        }
        if (flowData->vlanDeiAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set DEI = %u", flowData->vlanDei);
        }
        if (flowData->dscpAction)
        {
          APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " Set DSCP = %u", flowData->dscp);
        }
      }
      break;

  case OFDPA_FLOW_TABLE_ID_EGRESS_TPID:
    {
      ofdpaEgressTpidFlowEntry_t *flowData;
      ofdpaEgressTpidFlowMatch_t *match;

      flowData = &flow->flowData.egressTpidFlowEntry;
      match = &flowData->match_criteria;

      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " outPort = ");
      ofdpaPortDecode(outBuf, bufSize, &count, match->outPort);

      /* instructions */
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " |");

      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " newTpid = 0x%04x", flowData->newTpid);
    }
    break;


    default:
      APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "Unable to decode flow entry for table ID %d ", flow->tableId);
      rc = OFDPA_E_NOT_FOUND;
      break;
  }

  /* configuration data common to all flow entries */
  APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, " | ");
  APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "priority = %d ", flow->priority);
  APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "hard_time = %d ", flow->hard_time);
  APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "idle_time = %d ", flow->idle_time);
  APPEND_BUFFER_CHECK_SIZE(outBuf, bufSize, count, "cookie = %llu", (unsigned long long int)flow->cookie);

  return(rc);
}
