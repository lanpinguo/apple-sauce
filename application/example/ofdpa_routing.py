#! /usr/bin/env python

"""
Set up OF-DPA groups and flows configuring unicast routing between two physical ports. The
script sets up forwarding in one direction.

This script invokes OF-DPA API services via RPC. The RPC calls are served by the ofdpa
process running on the switch.

"""
from OFDPA_python import *
import socket
import struct

def ip2int(addr):
    return struct.unpack("!L", socket.inet_aton(addr))[0]

def int2ip(addr):
    return socket.inet_ntoa(struct.pack("!L", addr))

def main():
    rc = ofdpaClientInitialize("OFDPA_example")
    if rc == OFDPA_E_NONE:
        # variables used to set up routing scenario
        ingressVlanId = 10
        egressVlanId = 11
        ingressPhysicalPort = 3
        egressPhysicalPort = 4
        routerMacAddress  = "00:01:02:03:04:05"
        nextHopMacAddress = "00:00:00:00:00:02"
        destinationIpAddress = ip2int('192.168.5.1')

        # set up L2 Interface group on egress physical port
        l2IfaceGroupEntry = ofdpaGroupEntry_t()
        l2IfaceGroupBucket = ofdpaGroupBucketEntry_t()
        l2IfaceGroupId_p = new_uint32_tp()

        ofdpaGroupTypeSet(l2IfaceGroupId_p, OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
        ofdpaGroupVlanSet(l2IfaceGroupId_p, egressVlanId)
        ofdpaGroupPortIdSet(l2IfaceGroupId_p, egressPhysicalPort)
        l2IfaceGroupEntry.groupId = uint32_tp_value(l2IfaceGroupId_p)

        l2IfaceGroupBucket.groupId = l2IfaceGroupEntry.groupId
        l2IfaceGroupBucket.bucketIndex = 0
        l2IfaceGroupBucket.bucketData.l2Interface.outputPort = egressPhysicalPort

        ofdpaGroupAdd(l2IfaceGroupEntry)
        ofdpaGroupBucketEntryAdd(l2IfaceGroupBucket)

        # set up L3 Unicast group using L2 Interface group just created
        l3UcastGroupEntry = ofdpaGroupEntry_t()
        l3UcastGroupBucket = ofdpaGroupBucketEntry_t()
        l3UcastGroupId_p = new_uint32_tp()

        ofdpaGroupTypeSet(l3UcastGroupId_p, OFDPA_GROUP_ENTRY_TYPE_L3_UNICAST)
        ofdpaGroupIndexSet(l3UcastGroupId_p, 1)
        l3UcastGroupEntry.groupId = uint32_tp_value(l3UcastGroupId_p)

        l3UcastGroupBucket.groupId = l3UcastGroupEntry.groupId
        l3UcastGroupBucket.referenceGroupId = l2IfaceGroupEntry.groupId
        MACAddress_set(l3UcastGroupBucket.bucketData.l3Unicast.srcMac, routerMacAddress)
        MACAddress_set(l3UcastGroupBucket.bucketData.l3Unicast.dstMac, nextHopMacAddress)
        l3UcastGroupBucket.bucketData.l3Unicast.vlanId = (egressVlanId | OFDPA_VID_PRESENT)

        ofdpaGroupAdd(l3UcastGroupEntry)
        ofdpaGroupBucketEntryAdd(l3UcastGroupBucket)

        # set up VLAN Filtering flow entry to admit tagged packets on ingress physical port
        vlanFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_VLAN, vlanFlowEntry)
        vlanFlowEntry.flowData.vlanFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.inPort = ingressPhysicalPort
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanId = (OFDPA_VID_PRESENT | ingressVlanId)
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanIdMask = (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)

        ofdpaFlowAdd(vlanFlowEntry)

        # set up Termination MAC flow entry
        termMacFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_TERMINATION_MAC, termMacFlowEntry)
        termMacFlowEntry.flowData.terminationMacFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.inPort = ingressPhysicalPort
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.inPortMask = OFDPA_INPORT_EXACT_MASK
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.etherType = 0x0800
        MACAddress_set(termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.destMac, routerMacAddress)
        MACAddress_set(termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.destMacMask, "ff:ff:ff:ff:ff:ff")
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.vlanId = OFDPA_VID_PRESENT | ingressVlanId
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.vlanIdMask = OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK

        ofdpaFlowAdd(termMacFlowEntry)

        # set up Unicast Routing flow entry
        l3UcastRoutingFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_UNICAST_ROUTING, l3UcastRoutingFlowEntry)
        l3UcastRoutingFlowEntry.flowData.unicastRoutingFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_ACL_POLICY
        l3UcastRoutingFlowEntry.flowData.unicastRoutingFlowEntry.groupID = l3UcastGroupEntry.groupId
        l3UcastRoutingFlowEntry.flowData.unicastRoutingFlowEntry.match_criteria.etherType = 0x0800

        l3UcastRoutingFlowEntry.flowData.unicastRoutingFlowEntry.match_criteria.dstIp4 = destinationIpAddress
        l3UcastRoutingFlowEntry.flowData.unicastRoutingFlowEntry.match_criteria.dstIp4Mask = OFDPA_IPV4_ADDR_EXACT_MASK

        ofdpaFlowAdd(l3UcastRoutingFlowEntry)

        # clean up
        delete_uint32_tp(l2IfaceGroupId_p)
        delete_uint32_tp(l3UcastGroupId_p)

    else:
        print "Unable to initialize."


if __name__ == '__main__': main()
