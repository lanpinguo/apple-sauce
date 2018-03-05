#! /usr/bin/env python

""" 
This script sets up MPLS LSR ECMP forwarding by configuring the associated flows and groups.

Ingress packet configuration:
Destination MAC = 'localMacAddress'
VLAN Id = 'srcVlanId'
Ingress port = 'srcPort'
Ethertype = 'etherType'
Topmost MPLS label
  Value = 'srclabel'
  BOS = 0
Inner MPLS label, if any
  Value = someValue
  BOS = 1
Data: IP data
  
Send multiple streams with varying inner MPLS label, IP destination or source addresses for hashing.
  
Switch MPLS LSR ECMP configuration is done in main().
Incoming topmost label 'srclabel' is swapped with label 'destlabel1' or 'destlabel2' based on which ECMP link the packet egresses on.
Capture packets on egress interfaces and verify the topmost label, vlan tag, source and destination MAC addresses and TTL.

Expected Egress packet on egress port destPort1:
Destination MAC = 'destMacAddress1'
Source MAC = 'localMacAddress'
VLAN Id = 'destVlanId1'
Topmost label value = 'destlabel1'
Topmost label TTL should be decremented by 1.
Inner label stays as it is

Expected Egress packet on egress port destPort2:
Destination MAC = 'destMacAddress2'
Source MAC = 'localMacAddress'
VLAN Id = 'destVlanId2'
Topmost label value = 'destlabel2'
Topmost label TTL should be decremented by 1.
Inner label stays as it is
  
This script invokes OF-DPA API services via RPC. The RPC calls are served by the ofdpa
process running on the switch.

"""
from OFDPA_python import *

def main():
    rc = ofdpaClientInitialize("OFDPA_example")
    if rc == OFDPA_E_NONE:

        # Variables used to set up MPLS LSR ECMP scenario
        destVlanId1 = 1
        destVlanId2 = 3
        srcVlanId = 2
        destPort1 = 1
        destPort2 = 2
        srcPort = 3
        localMacAddress = "00:01:02:03:04:05"
        destMacAddress1 = "00:01:03:05:07:09"
        destMacAddress2 = "00:01:03:05:07:99"
        index1 = 1
        index2 = 2
        destlabel1 = 0x1ff
        destlabel2 = 0x2ff
        srclabel = 0x177
        srclabel2 = 0x277
        etherType = 0x8847
        
        
        # Set up 1st L2 Interface group. This configures the 1st egress port.

        l2IntfGroupId1 = new_uint32_tp()

        l2IfaceGroupEntry  = ofdpaGroupEntry_t()
        l2IfaceGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(l2IntfGroupId1, OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
        ofdpaGroupVlanSet(l2IntfGroupId1, destVlanId1)
        ofdpaGroupPortIdSet(l2IntfGroupId1, destPort1)

        l2IfaceGroupEntry.groupId = uint32_tp_value(l2IntfGroupId1)

        l2IfaceGroupBucket.groupId = l2IfaceGroupEntry.groupId
        l2IfaceGroupBucket.bucketIndex = 0
        l2IfaceGroupBucket.bucketData.l2Interface.outputPort = destPort1

        rc = ofdpaGroupAdd(l2IfaceGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add 1st L2 interface group. rc = %d " % (rc)
          return

        rc = ofdpaGroupBucketEntryAdd(l2IfaceGroupBucket)
        if rc != OFDPA_E_NONE:
          print "Unable to add 1st L2 interface group bucket. rc = %d " % (rc)
          return

        # Set up 2nd L2 Interface group. This configures the 2nd egress port.
        l2IntfGroupId2 = new_uint32_tp()

        l2IfaceGroupEntry  = ofdpaGroupEntry_t()
        l2IfaceGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(l2IntfGroupId2, OFDPA_GROUP_ENTRY_TYPE_L2_INTERFACE)
        ofdpaGroupVlanSet(l2IntfGroupId2, destVlanId2)
        ofdpaGroupPortIdSet(l2IntfGroupId2, destPort2)

        l2IfaceGroupEntry.groupId = uint32_tp_value(l2IntfGroupId2)

        l2IfaceGroupBucket.groupId = l2IfaceGroupEntry.groupId
        l2IfaceGroupBucket.bucketIndex = 0
        l2IfaceGroupBucket.bucketData.l2Interface.outputPort = destPort2

        rc = ofdpaGroupAdd(l2IfaceGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add 2nd L2 interface group. rc = %d " % (rc)
          return

        rc = ofdpaGroupBucketEntryAdd(l2IfaceGroupBucket)
        if rc != OFDPA_E_NONE:
          print "Unable to add 2nd L2 interface group bucket. rc = %d " % (rc)
          return

        # Set up 1st MPLS Interface group. This configures the source and destination MAC and the VLAN ID of the egress packet.

        mplsIntfGroupId1 = new_uint32_tp()

        mplsIfaceGroupEntry  = ofdpaGroupEntry_t()
        mplsIfaceGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(mplsIntfGroupId1, OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
        ofdpaGroupMplsSubTypeSet(mplsIntfGroupId1, OFDPA_MPLS_INTERFACE)
        ofdpaGroupIndexSet(mplsIntfGroupId1, index1)

        mplsIfaceGroupEntry.groupId = uint32_tp_value(mplsIntfGroupId1)

        mplsIfaceGroupBucket.groupId = mplsIfaceGroupEntry.groupId
        mplsIfaceGroupBucket.bucketIndex = 0
        mplsIfaceGroupBucket.bucketData.mplsInterface.vlanId = (destVlanId1 | OFDPA_VID_PRESENT)
        MACAddress_set(mplsIfaceGroupBucket.bucketData.mplsInterface.srcMac, localMacAddress)
        MACAddress_set(mplsIfaceGroupBucket.bucketData.mplsInterface.dstMac, destMacAddress1)
        mplsIfaceGroupBucket.referenceGroupId = uint32_tp_value(l2IntfGroupId1)

        rc = ofdpaGroupAdd(mplsIfaceGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add 1st MPLS interface group. rc = %d " % (rc)
          return

        rc = ofdpaGroupBucketEntryAdd(mplsIfaceGroupBucket)
        if rc != OFDPA_E_NONE:
          print "Unable to add 1st MPLS interface group bucket. rc = %d " % (rc)
          return

        # Set up 2nd MPLS Interface group. This configures the source and destination MAC and the VLAN ID of the egress packet.
        mplsIntfGroupId2 = new_uint32_tp()

        mplsIfaceGroupEntry  = ofdpaGroupEntry_t()
        mplsIfaceGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(mplsIntfGroupId2, OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
        ofdpaGroupMplsSubTypeSet(mplsIntfGroupId2, OFDPA_MPLS_INTERFACE)
        ofdpaGroupIndexSet(mplsIntfGroupId2, index2)

        mplsIfaceGroupEntry.groupId = uint32_tp_value(mplsIntfGroupId2)

        mplsIfaceGroupBucket.groupId = mplsIfaceGroupEntry.groupId
        mplsIfaceGroupBucket.bucketIndex = 0
        mplsIfaceGroupBucket.bucketData.mplsInterface.vlanId = (destVlanId2 | OFDPA_VID_PRESENT)
        MACAddress_set(mplsIfaceGroupBucket.bucketData.mplsInterface.srcMac, localMacAddress)
        MACAddress_set(mplsIfaceGroupBucket.bucketData.mplsInterface.dstMac, destMacAddress2)
        mplsIfaceGroupBucket.referenceGroupId = uint32_tp_value(l2IntfGroupId2)

        rc = ofdpaGroupAdd(mplsIfaceGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add 2nd MPLS interface group. rc = %d " % (rc)
          return

        rc = ofdpaGroupBucketEntryAdd(mplsIfaceGroupBucket)
        if rc != OFDPA_E_NONE:
          print "Unable to add 2nd MPLS interface group bucket. rc = %d " % (rc)
          return

        # Set up 1st MPLS Swap group. This configures the swap label and the reference MPLS Interface group to be used. 
        
        mplsSwapGroupId1 = new_uint32_tp()

        mplsSwapGroupEntry  = ofdpaGroupEntry_t()
        mplsSwapGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(mplsSwapGroupId1, OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
        ofdpaGroupMplsSubTypeSet(mplsSwapGroupId1, OFDPA_MPLS_SWAP_LABEL)
        ofdpaGroupIndexSet(mplsSwapGroupId1, index1)

        mplsSwapGroupEntry.groupId = uint32_tp_value(mplsSwapGroupId1)

        mplsSwapGroupBucket.groupId = mplsSwapGroupEntry.groupId
        mplsSwapGroupBucket.bucketIndex = 0
        mplsSwapGroupBucket.bucketData.mplsLabel.mplsLabel = destlabel1
        mplsSwapGroupBucket.referenceGroupId = uint32_tp_value(mplsIntfGroupId1)

        rc = ofdpaGroupAdd(mplsSwapGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add 1st MPLS swap group. rc = %d " % (rc)
          return
        
        rc = ofdpaGroupBucketEntryAdd(mplsSwapGroupBucket)
        if rc != OFDPA_E_NONE:
          print "Unable to add 1st MPLS swap group bucket. rc = %d " % (rc)
          return

        # Set up 2nd MPLS Swap group. This configures the swap label and the reference MPLS Interface group to be used. 
        
        mplsSwapGroupId2 = new_uint32_tp()

        mplsSwapGroupEntry  = ofdpaGroupEntry_t()
        mplsSwapGroupBucket = ofdpaGroupBucketEntry_t()

        ofdpaGroupTypeSet(mplsSwapGroupId2, OFDPA_GROUP_ENTRY_TYPE_MPLS_LABEL)
        ofdpaGroupMplsSubTypeSet(mplsSwapGroupId2, OFDPA_MPLS_SWAP_LABEL)
        ofdpaGroupIndexSet(mplsSwapGroupId2, index2)

        mplsSwapGroupEntry.groupId = uint32_tp_value(mplsSwapGroupId2)

        mplsSwapGroupBucket.groupId = mplsSwapGroupEntry.groupId
        mplsSwapGroupBucket.bucketIndex = 0
        mplsSwapGroupBucket.bucketData.mplsLabel.mplsLabel = destlabel2
        mplsSwapGroupBucket.referenceGroupId = uint32_tp_value(mplsIntfGroupId2)

        rc = ofdpaGroupAdd(mplsSwapGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add 2nd MPLS swap group. rc = %d " % (rc)
          return
        
        rc = ofdpaGroupBucketEntryAdd(mplsSwapGroupBucket)
        if rc != OFDPA_E_NONE:
          print "Unable to add 2nd MPLS swap group bucket. rc = %d " % (rc)
          return

        # Set up MPLS ECMP group.  This configures the next hops in an ECMP group. 
        
        mplsECMPGroupId = new_uint32_tp()

        ofdpaGroupTypeSet(mplsECMPGroupId, OFDPA_GROUP_ENTRY_TYPE_MPLS_FORWARDING)
        ofdpaGroupMplsSubTypeSet(mplsECMPGroupId, OFDPA_MPLS_ECMP)
        ofdpaGroupIndexSet(mplsECMPGroupId, index1)

        mplsECMPGroupEntry  = ofdpaGroupEntry_t()
        mplsECMPGroupEntry.groupId = uint32_tp_value(mplsECMPGroupId)

        mplsECMPGroupBucket1 = ofdpaGroupBucketEntry_t()
        mplsECMPGroupBucket1.groupId = mplsECMPGroupEntry.groupId
        mplsECMPGroupBucket1.bucketIndex = 0
        mplsECMPGroupBucket1.referenceGroupId = uint32_tp_value(mplsSwapGroupId1)

        mplsECMPGroupBucket2 = ofdpaGroupBucketEntry_t()
        mplsECMPGroupBucket2.groupId = mplsECMPGroupEntry.groupId
        mplsECMPGroupBucket2.bucketIndex = 1
        mplsECMPGroupBucket2.referenceGroupId = uint32_tp_value(mplsSwapGroupId2)

        rc = ofdpaGroupAdd(mplsECMPGroupEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add MPLS ECMP group. rc = %d " % (rc)
          return
        
        rc = ofdpaGroupBucketEntryAdd(mplsECMPGroupBucket1)
        if rc != OFDPA_E_NONE:
          print "Unable to add MPLS ECMP group 1st bucket. rc = %d " % (rc)
          return
        
        rc = ofdpaGroupBucketEntryAdd(mplsECMPGroupBucket2)
        if rc != OFDPA_E_NONE:
          print "Unable to add MPLS ECMP group 2nd bucket. rc = %d " % (rc)
          return

        # Set up MPLS1 flow entry. This configures the matching ingress label and the reference ECMP Group to be used for load balancing and egress swap label.
        mpls1FlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_MPLS_1, mpls1FlowEntry)
        mpls1FlowEntry.flowData.mplsFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_ACL_POLICY
        mpls1FlowEntry.flowData.mplsFlowEntry.groupID = uint32_tp_value(mplsECMPGroupId)
        mpls1FlowEntry.flowData.mplsFlowEntry.decrementTtlAction = 1
        
        mpls1FlowEntry.flowData.mplsFlowEntry.match_criteria.etherType = etherType
        mpls1FlowEntry.flowData.mplsFlowEntry.match_criteria.mplsLabel = srclabel

        ofdpaFlowAdd(mpls1FlowEntry)

        mpls1FlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_MPLS_1, mpls1FlowEntry)
        mpls1FlowEntry.flowData.mplsFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_ACL_POLICY
        mpls1FlowEntry.flowData.mplsFlowEntry.groupID = uint32_tp_value(mplsECMPGroupId)
        mpls1FlowEntry.flowData.mplsFlowEntry.decrementTtlAction = 1
        
        mpls1FlowEntry.flowData.mplsFlowEntry.match_criteria.etherType = etherType
        mpls1FlowEntry.flowData.mplsFlowEntry.match_criteria.mplsLabel = srclabel2

        rc = ofdpaFlowAdd(mpls1FlowEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add MPLS 1 flow entry. rc = %d " % (rc)
          return

        # Set up Termination MAC flow entry. This configures the matching destination MAC, VLAN Id and ingress interface of the ingress packet to set it up for MPLS processing.
        termMacFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_TERMINATION_MAC, termMacFlowEntry)
        termMacFlowEntry.flowData.terminationMacFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_MPLS_1
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.vlanId = (OFDPA_VID_PRESENT | srcVlanId)
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.vlanIdMask = (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)
        termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.etherType = etherType
        MACAddress_set(termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.destMac, localMacAddress)
        MACAddress_set(termMacFlowEntry.flowData.terminationMacFlowEntry.match_criteria.destMacMask, "ff:ff:ff:ff:ff:ff")

        rc = ofdpaFlowAdd(termMacFlowEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add Termination MAC flow entry. rc = %d " % (rc)
          return

        # Set up VLAN Filtering flow entries. This configures the VLAN participation of the ingress port.
        vlanFlowEntry = ofdpaFlowEntry_t()
        ofdpaFlowEntryInit(OFDPA_FLOW_TABLE_ID_VLAN, vlanFlowEntry)
        vlanFlowEntry.flowData.vlanFlowEntry.gotoTableId = OFDPA_FLOW_TABLE_ID_TERMINATION_MAC
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.inPort = srcPort
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanId = (OFDPA_VID_PRESENT | srcVlanId)
        vlanFlowEntry.flowData.vlanFlowEntry.match_criteria.vlanIdMask = (OFDPA_VID_PRESENT | OFDPA_VID_EXACT_MASK)

        rc = ofdpaFlowAdd(vlanFlowEntry)
        if rc != OFDPA_E_NONE:
          print "Unable to add VLAN flow entry. rc = %d " % (rc)
          return

        # clean up
        delete_uint32_tp(l2IntfGroupId1)
        delete_uint32_tp(l2IntfGroupId2)
        delete_uint32_tp(mplsIntfGroupId1)
        delete_uint32_tp(mplsIntfGroupId2)
        delete_uint32_tp(mplsSwapGroupId1)
        delete_uint32_tp(mplsSwapGroupId2)
        delete_uint32_tp(mplsECMPGroupId)

    else:
        print "Unable to initialize."


if __name__ == '__main__': main()
