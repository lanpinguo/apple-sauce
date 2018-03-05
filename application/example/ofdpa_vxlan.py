#! /usr/bin/env python

""" 
Set up OF-DPA groups and flows configuring a Data Center Overlay Tunnel scenario. The
script sets up Access and Network logical ports.

This script invokes OF-DPA API services via RPC. The RPC calls are served by the ofdpa
process running on the switch.

"""
from OFDPA_python import *
from socket import *

def main():
    rc = ofdpaClientInitialize("OFDPA_example")
    if rc == OFDPA_E_NONE:

        # local and remote end station MAC addresses
        localStationMac  = "00:00:00:00:00:11"
        remoteStationMac = "00:00:00:00:00:99"

        # tenant parameters
        tunnelIdIndex = 1
        vnid = 33

        # Tunnel Access Port parameters
        accessPortIndex = 1
        accessPhysicalPort = 1
        accessVlanId = 100
        accessPacketsUntagged = 0

        # Tunnel Next Hop parameters
        nextHopId = 1
        nextHopSourceMac  = "00:00:00:00:00:aa"
        nextHopDestMac    = "00:00:00:00:00:bb"
        nextHopPhysicalPort = 2
        nextHopVlanId = 200

        # Tunnel Endpoint Port parameters
        endpointPortIndex = 2
        remoteEndpointIp =  0x02020202  # 2.2.2.2
        localEndpointIp = 0x01010101    # 1.1.1.1
        ttl = 45
        terminatorUdpDestPort = 132
        initiatorUdpDestPort = 133
        udpSrcPortIfNoEntropy = 0
        useEntropy = 1

        # configure tenant
        tunnelId_p = new_uint32_tp()
        ofdpaTunnelIdTypeSet(tunnelId_p, OFDPA_TUNNELID_TYPE_DATA_CENTER_OVERLAY)
        ofdpaTunnelIdIndexSet(tunnelId_p, tunnelIdIndex)
        tunnelId = uint32_tp_value(tunnelId_p)

        tenantEntry = ofdpaTunnelTenantConfig_t()
        tenantEntry.protocol = OFDPA_TUNNEL_PROTO_VXLAN
        tenantEntry.virtualNetworkId = vnid
        ofdpaTunnelTenantCreate(tunnelId, tenantEntry)

        # create Tunnel Access Port
        accessPortId_p = new_uint32_tp()
        ofdpaPortTypeSet(accessPortId_p, OFDPA_PORT_TYPE_LOGICAL_TUNNEL)
        ofdpaPortIndexSet(accessPortId_p, accessPortIndex)
        accessPortId = uint32_tp_value(accessPortId_p)

        accessPortCfg = ofdpaTunnelPortConfig_t()
        accessPortCfg.type = OFDPA_TUNNEL_PORT_TYPE_ACCESS
        accessPortCfg.tunnelProtocol = OFDPA_TUNNEL_PROTO_VXLAN
        accessPortCfg.configData.access.physicalPortNum = accessPhysicalPort
        accessPortCfg.configData.access.vlanId = accessVlanId
        accessPortCfg.configData.access.untagged = accessPacketsUntagged

        accessPortName = "AccessPort"
        portNameBuffDesc = ofdpa_buffdesc()
        portNameBuffDesc.size = len(accessPortName) + 1
        BuffDesc_pstart_setbytes(portNameBuffDesc, accessPortName)

        ofdpaTunnelPortCreate(accessPortId, portNameBuffDesc, accessPortCfg)

        # create Tunnel Next Hop
        tunnelNextHopCfg = ofdpaTunnelNextHopConfig_t()
        tunnelNextHopCfg.protocol = OFDPA_TUNNEL_PROTO_VXLAN
        MACAddress_set(tunnelNextHopCfg.srcAddr, nextHopSourceMac)
        MACAddress_set(tunnelNextHopCfg.dstAddr, nextHopDestMac)
        tunnelNextHopCfg.physicalPortNum = nextHopPhysicalPort
        tunnelNextHopCfg.vlanId = nextHopVlanId

        ofdpaTunnelNextHopCreate(nextHopId, tunnelNextHopCfg)

        # create Tunnel Endpoint Port
        endpointPortId_p = new_uint32_tp()
        ofdpaPortTypeSet(endpointPortId_p, OFDPA_PORT_TYPE_LOGICAL_TUNNEL)
        ofdpaPortIndexSet(endpointPortId_p, endpointPortIndex)
        endpointPortId = uint32_tp_value(endpointPortId_p)

        endpointPortCfg = ofdpaTunnelPortConfig_t()
        endpointPortCfg.type = OFDPA_TUNNEL_PORT_TYPE_ENDPOINT
        endpointPortCfg.tunnelProtocol = OFDPA_TUNNEL_PROTO_VXLAN
        endpointPortCfg.configData.endpoint.remoteEndpoint = remoteEndpointIp
        endpointPortCfg.configData.endpoint.localEndpoint  = localEndpointIp
        endpointPortCfg.configData.endpoint.ttl = ttl
        endpointPortCfg.configData.endpoint.nextHopId = nextHopId

        endpointPortCfg.configData.endpoint.protocolInfo.vxlan.terminatorUdpDstPort = terminatorUdpDestPort
        endpointPortCfg.configData.endpoint.protocolInfo.vxlan.initiatorUdpDstPort = initiatorUdpDestPort
        endpointPortCfg.configData.endpoint.protocolInfo.vxlan.udpSrcPortIfNoEntropy = udpSrcPortIfNoEntropy
        endpointPortCfg.configData.endpoint.protocolInfo.vxlan.useEntropy = useEntropy

        endpointPortName = "EndpointPort"
        portNameBuffDesc.size = len(endpointPortName) + 1
        BuffDesc_pstart_setbytes(portNameBuffDesc, endpointPortName)

        ofdpaTunnelPortCreate(endpointPortId, portNameBuffDesc, endpointPortCfg)

        # add Access and Endpoint ports to tenant
        ofdpaTunnelPortTenantAdd(accessPortId, tunnelId)
        ofdpaTunnelPortTenantAdd(endpointPortId, tunnelId)

        # set up L2 Interface Group on Next Hop physical ports
        l2IfaceGroupEntry = ofdpaGroupEntry_t()
        l2IfaceGroupBucket = ofdpaGroupBucketEntry_t()
        l2IfaceGroupId_p = new_uint32_tp()

        ofdpaGroupTypeSet(l2IfaceGroupId_p, OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
        ofdpaGroupVlanSet(l2IfaceGroupId_p, nextHopVlanId)
        ofdpaGroupPortIdSet(l2IfaceGroupId_p, nextHopPhysicalPort)
        l2IfaceGroupEntry.groupId = uint32_tp_value(l2IfaceGroupId_p)

        l2IfaceGroupBucket.groupId = l2IfaceGroupEntry.groupId
        l2IfaceGroupBucket.bucketIndex = 0
        l2IfaceGroupBucket.bucketData.l2Interface.outputPort = nextHopPhysicalPort

        ofdpaGroupAdd(l2IfaceGroupEntry)
        ofdpaGroupBucketEntryAdd(l2IfaceGroupBucket)

        # set up Ingress Port flow
        ingressPortFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_INGRESS_PORT, ingressPortFlowEntry)
        ingressPortFlowEntry.flowData.ingressPortFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_BRIDGING

        ingressPortFlowEntry.flowData.ingressPortFlowEntry.match_criteria.tunnelId = tunnelId
        ingressPortFlowEntry.flowData.ingressPortFlowEntry.match_criteria.tunnelIdMask = OFDPA_TUNNEL_ID_EXACT_MASK

        ofdpaFlowAdd(ingressPortFlowEntry)

        # set up VLAN flow admitting tagged traffic on next hop port
        vlanFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_VLAN, vlanFlowEntry)
        vlanFlowEntry.flowData.vlanFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.inPort = nextHopPhysicalPort
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanId = (OFDPA_VID_PRESENT | nextHopVlanId)
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanIdMask = (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)

        ofdpaFlowAdd(vlanFlowEntry)

        # set up Bridging flows for both directions
        bridgingFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_BRIDGING, bridgingFlowEntry)
        bridgingFlowEntry.flowData.bridgingFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_ACL_POLICY
        bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.tunnelId = tunnelId
        bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.tunnelIdMask = OFDPA_TUNNEL_ID_EXACT_MASK
        MACAddress_set(bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.destMacMask, "ffff.ffff.ffff")

        MACAddress_set(bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.destMac, localStationMac)
        bridgingFlowEntry.flowData.bridgingFlowEntry.tunnelLogicalPort = accessPortId

        ofdpaFlowAdd(bridgingFlowEntry)

        MACAddress_set(bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.destMac, remoteStationMac)
        bridgingFlowEntry.flowData.bridgingFlowEntry.tunnelLogicalPort = endpointPortId

        ofdpaFlowAdd(bridgingFlowEntry)

    else:
        print "Unable to initialize."


if __name__ == '__main__': main()
