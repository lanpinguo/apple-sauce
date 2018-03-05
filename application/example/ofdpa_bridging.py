#! /usr/bin/env python

"""
Set up OF-DPA groups and flows configuring L2 forwarding between two physical ports. Flows
in both directions are configured by this script.

This script invokes OF-DPA API services via RPC. The RPC calls are served by the ofdpa
process running on the switch.

"""
from OFDPA_python import *

def main():
    rc = ofdpaClientInitialize("OFDPA_example")
    if rc == OFDPA_E_NONE:
        # variables used to set up bridging scenario
        vlanId = 10
        firstPhysicalPort = 3
        secondPhysicalPort = 4
        firstMacAddress = "00:00:00:00:00:01"
        secondMacAddress = "00:00:00:00:00:02"

        groupId1_p = new_uint32_tp()
        groupId2_p = new_uint32_tp()

        # set up L2 Interface groups on physical ports
        l2IfaceGroupEntry1 = ofdpaGroupEntry_t()
        l2IfaceGroupEntry2 = ofdpaGroupEntry_t()
        l2IfaceGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(groupId1_p, OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
        ofdpaGroupVlanSet(groupId1_p, vlanId)
        ofdpaGroupPortIdSet(groupId1_p, firstPhysicalPort)
        l2IfaceGroupEntry1.groupId = uint32_tp_value(groupId1_p)

        l2IfaceGroupBucket.groupId = l2IfaceGroupEntry1.groupId
        l2IfaceGroupBucket.bucketIndex = 0
        l2IfaceGroupBucket.bucketData.l2Interface.outputPort = firstPhysicalPort

        ofdpaGroupAdd(l2IfaceGroupEntry1)
        ofdpaGroupBucketEntryAdd(l2IfaceGroupBucket)

        ofdpaGroupTypeSet(groupId2_p, OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
        ofdpaGroupVlanSet(groupId2_p, vlanId)
        ofdpaGroupPortIdSet(groupId2_p, secondPhysicalPort)
        l2IfaceGroupEntry2.groupId = uint32_tp_value(groupId2_p)

        l2IfaceGroupBucket.groupId = l2IfaceGroupEntry2.groupId
        l2IfaceGroupBucket.bucketData.l2Interface.outputPort = secondPhysicalPort

        ofdpaGroupAdd(l2IfaceGroupEntry2)
        ofdpaGroupBucketEntryAdd(l2IfaceGroupBucket)

        # set up VLAN Filtering flow entries
        vlanFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_VLAN, vlanFlowEntry)
        vlanFlowEntry.flowData.vlanFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.inPort = firstPhysicalPort
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanId = (OFDPA_VID_PRESENT | vlanId)
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanIdMask = (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)

        ofdpaFlowAdd(vlanFlowEntry)

        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.inPort = secondPhysicalPort

        ofdpaFlowAdd(vlanFlowEntry)

        # set up first Bridging flow entry
        bridgingFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_BRIDGING, bridgingFlowEntry)
        bridgingFlowEntry.flowData.bridgingFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_ACL_POLICY
        bridgingFlowEntry.flowData.bridgingFlowEntry.groupID = l2IfaceGroupEntry1.groupId
        bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.vlanId = (OFDPA_VID_PRESENT | vlanId)
        bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.vlanIdMask = (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)
        MACAddress_set(bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.destMac, firstMacAddress)
        MACAddress_set(bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.destMacMask, "ff:ff:ff:ff:ff:ff")

        ofdpaFlowAdd(bridgingFlowEntry)

        # set up second Bridging flow entry
        bridgingFlowEntry.flowData.bridgingFlowEntry.groupID = l2IfaceGroupEntry2.groupId
        MACAddress_set(bridgingFlowEntry.flowData.bridgingFlowEntry.match_criteria.destMac, secondMacAddress)

        ofdpaFlowAdd(bridgingFlowEntry)

        # clean up
        delete_uint32_tp(groupId1_p)
        delete_uint32_tp(groupId2_p)

    else:
        print "Unable to initialize."


if __name__ == '__main__': main()
